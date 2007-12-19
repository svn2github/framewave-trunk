/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ThreadPool.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#include <assert.h>
#include <map>

typedef boost::mutex::scoped_lock Lock;
typedef boost::mutex              Mutex;
typedef boost::thread_group       ThreadGroup;
typedef boost::condition          Condition;

typedef unsigned int       U32;
typedef unsigned long long U64;

const static U32 MAX_CORES      = 16;
const static U32 JOB_QUEUE_SIZE = 16;

#ifndef MAX
#define  MAX(a,b)  ((a)>(b)?(a):(b)) 
#endif
#ifndef MIN
#define  MIN(a,b)  ((a)<(b)?(a):(b)) 
#endif



//
// Operating system specific definitions
//

#if   defined( WIN64   ) || defined( _WIN64   ) || \
      defined( WIN32   ) || defined( _WIN32   ) || \
      defined( WINDOWS ) || defined( _WINDOWS )
#define TP_WIN
#elif defined( LIN64   ) || defined( _LIN64   ) || \
      defined( LIN32   ) || defined( _LIN32   ) || \
      defined( LINUX   ) || defined( _LINUX   )
#define TP_LIN
#elif defined( SOL64   ) || defined( _SOL64   ) || \
      defined( SOL32   ) || defined( _SOL32   ) || \
      defined( SOLARIS ) || defined( _SOLARIS )
#define TP_SOL
#elif defined( MAC64   ) || defined( _MAC64   ) || \
      defined( MAC32   ) || defined( _MAC32   ) || \
      defined( __APPLE__ )
#define TP_MAC
#endif 


#if defined( TP_LIN ) // LINUX
#include <sys/types.h>
#include <linux/unistd.h>
#include <fstream>
#include <string>
#define CPUINFO_FILE "/proc/cpuinfo"

U32 SysCoreCount() 
{
    std::string line,rem;
    U32 count =0;
    std::ifstream cpuinfo_file (CPUINFO_FILE);

    if (cpuinfo_file.is_open())
    {
        while (! cpuinfo_file.eof() )
        {
            getline (cpuinfo_file,line);
            if(line.find("cpu cores")!=std::string::npos)//"cpu cores" is the Constant for file parsing.
                count++;
        }
        cpuinfo_file.close();
    }
    return MAX((U32)1,count);
}

inline U32 ThreadId(){ return (U32)syscall(__NR_gettid); }

#elif defined( TP_SOL ) // SOLARIS
#include <sys/unistd.h>
#include <sys/processor.h>
#include <sys/types.h>
#include <thread.h>

U32 SysCoreCount()
{
    U32 count = 0;
    processorid_t i;
    for (i = 0; i <= MAX_CORES; i++)
        if (p_online(i, P_STATUS) == P_ONLINE)
            count++;

    return MAX((U32)1,count);
}

inline U32 ThreadId(){ return (U32)thr_self(); }

#elif defined( TP_MAC ) // MAC

#include <unistd.h>
#include <sys/sysctl.h>
#include <pthread.h>

U32 SysCoreCount()
{
    U32 count = 0;

    size_t len = sizeof(count);
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    if (sysctl(mib, 2, &count, &len, 0, 0) < 0 || len != sizeof(count))
    {
        count = 1;
    }

    return MAX((U32)1,count);
}

inline U32 ThreadId(){ return (U32)reinterpret_cast<U64>(pthread_self()); }

#elif defined( TP_WIN ) // WINDOWS
#include <windows.h>
U32 SysCoreCount()
{
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
}

inline U32 ThreadId(){ return GetCurrentThreadId(); }
#endif


/*
No starvation
No underutilization 
Limited queue space

Assumptions:
- Jobs can be run out of order
- Jobs are of approximately the same size
*/


//
//  Thread Pool Implementation
//
class TP_private
{
public:  
    struct Job
    {   
        void (*fn)(void *);
        void *param;
        Job(){}
        Job( void(*fn_)(void*), void *param_ ):fn(fn_),param(param_){}
    };

    /**
    Storage container for one user thread. Holds jobs to run. 
    */
    class UserThreadGroup;
    class UserThread
    {  
        friend class UserThreadGroup;       

        U32        maxTLocal, numThreads;
        U64        readPos, writePos, exitCount;  
        UserThread *next, *prev;          

        Job jobs[JOB_QUEUE_SIZE];
    public:
        UserThread( U32 maxThreads )
        {
            maxTLocal = numThreads = maxThreads;      
            readPos   = writePos   = exitCount = 0;
            next      = prev       = 0;
        }

        void SetNumThreads_local ( U32 threads, Lock& ){ numThreads = maxTLocal = threads;    } //TODO: Document this!
        void SetNumThreads_global( U32 threads, Lock& ){ numThreads = MIN(threads,maxTLocal); }

        U32  ThreadCount()const{ return numThreads;                                          }
        bool Full       ()const{ return writePos-readPos == JOB_QUEUE_SIZE || 1==numThreads; }
        bool Empty      ()const{ return writePos-readPos == 0;                               }     
        bool Done       ()const{ return writePos         == exitCount;                       }  
        void Push(Job &j,Lock&){ assert(readPos<=writePos);   jobs[ writePos++ % JOB_QUEUE_SIZE ]=j;  }
        void Pop (Job &j,Lock&){ assert(readPos< writePos); j=jobs[  readPos++ % JOB_QUEUE_SIZE ];    }
        void Run (Job &j,Mutex& mutex )     
        {                  
            j.fn( j.param );              
            Lock lk( mutex );           
            ++exitCount;              
        }         
    };

    /**
    Storage container for all user threads. 

    UserThreads are stored in a circular linked list starting with 'top'.
    When all UserThread jobs have started running, the UserThread is orphaned from the linked list. 

    As workers process jobs, they advance circularly through the linked list to obtain UserThrads with free jobs.
    */
    class UserThreadGroup                                       
    {
        class ThreadLocalStorage
        {
            std::map< U32, void* > map;    
        public:   
            void* Get  (        Lock&){return map[ ThreadId() ];      }
            void  Set  (void* t,Lock&){       map[ ThreadId() ]=t;    }
            void  Erase(        Lock&){       map.erase( ThreadId() );}
        }tls;
          
        U32 maxTSystem, maxTGlobal;  
        UserThread * top;                                   // Pointer to the first user thread          
    public:
        UserThreadGroup ( U32 procCores )                  
            : maxTSystem( procCores     )
            , maxTGlobal( procCores     )
            , top       ( 0             ){}      

        /**
        Returns a pointer to the UserThread structure unique for this user thread. 
        If this is the first call to Run() in this user thread, allocates UserThread.
        The memory is deallocated at the end of Wait().
        */
        UserThread * Current( Mutex & mutex )                
        {    
            Lock lk( mutex ); 
            UserThread *ut = (UserThread*)tls.Get( lk );  

            if( !ut ){                                      // Does the thread local storage have an active thread?                                                                    
                ut = new UserThread( maxTGlobal );          // No, allocate a new user thread
                tls.Set( ut, lk ); 

                if(top){                                    // Add the thread to the list
                    assert( top->next ); assert( top->prev );
                    top->next->prev = ut;
                    ut ->next       = top->next;
                    ut ->prev       = top;
                    top->next       = ut;
                }else{                                      // First entry
                    top       = ut;
                    top->next = ut;        
                    top->prev = ut;
                }                
            }  

            assert(ut); assert(ut->next); assert(ut->prev);
            return ut;
        }    

        void Deactivate( UserThread *ut, Lock& )            // Remove the UserThread from the list       
        {                                                   // Called by Wait() when there are no available jobs
            assert( ut->Empty() );                          // Guarantee that there are not available jobs     
            ut->prev->next = ut->next;
            ut->next->prev = ut->prev;

            if( ut == top      ) top = top->next;           // ut was current, advance
            if( ut == ut->next ) top = 0;                   // ut was only entry
        }  

        void Remove( UserThread *ut, Mutex & mutex )        // Cleanup user thread. Called at end of Wait()
        {
            Lock lk( mutex );
            delete( ut );
            tls.Erase( lk );
        }

        UserThread * Advance( Lock& )                       // Advances through circular list. 
        {
            assert(top); assert(top->next);
            UserThread *tmp = top; 
            top = top->next;
            return tmp; 
        }  

        void SetNumThreads_global( U32 threads, Lock& lk )  // Set thread count for all UserThreads
        {
            maxTGlobal = MIN( threads, maxTSystem );

            if( top ){                                      // If there are active UserThreads
                UserThread *ut=top;                         // Set thread count for each

                do{ ut->SetNumThreads_global( maxTGlobal, lk );
                ut = ut->next;
                }while( ut!=top );
            }
        }

        void SetNumThreads_local( U32 threads, UserThread *ut, Lock& lk )    // Set thread count for one UserThread
        {
            threads = MIN( threads, maxTGlobal );
            ut->SetNumThreads_local( threads, lk );         //TODO: Doc this
        }          
    };

    struct WorkerThread
    {
        class TP_private * parent;
        void Init      ( class TP_private * parent_ ){ parent=parent_;}
        void operator()(                            ){ parent->Loop();}
    };

    // Data   
    U32 totalCores, availableJobs;  
    UserThreadGroup userThreadGroup;
    WorkerThread workerThreads[MAX_CORES];   

    ThreadGroup group;
    Mutex       mutex;
    Condition   condition_jobReady;       

    // Private internal functions
    void GetJob( UserThread * ut, Job &job, Lock &lk )     
    {
        ut->Pop( job, lk );
        --availableJobs;
    }    

    // Private external functions    
    U32 ThreadCount()                                       // Returns max work threads for this user thread
    { 
        return userThreadGroup.Current( mutex )->ThreadCount();  
    }

    void SetNumThreads_local( U32 threads )                 // Set thread count for this thread
    {
        UserThread *ut = userThreadGroup.Current( mutex );  

        Lock lk( mutex );        
        userThreadGroup.SetNumThreads_local( threads, ut, lk );
    }

    void SetNumThreads_global( U32 threads )                // Set thread count for all threads
    {
        Lock lk( mutex ); 
        userThreadGroup.SetNumThreads_global( threads, lk );
    }

    /**
    Threee functions actually run jobs Run(), Loop(), and Wait().

    Run () - If the job queue is full, run in the local thread.
    Loop() - Worker thread, rotates between user threads looking for jobs.
    Wait() - Runs jobs in its local queue.
    */

    void Run( void(*fn)(void*), void *param )
    {   
        UserThread * ut = userThreadGroup.Current( mutex );

        if( ut->Full() )                                    // Queue may become less full asynchronously, but not more full.
            fn( param );                                    // Queue was full, run locally    
        else{                                                
            Lock lk( mutex ); 

            Job j(fn,param);
            ut->Push( j, lk );                              // Store the job
            ++availableJobs;                                // Increment the number of available availableJobs
            condition_jobReady.notify_one();                // Notify worker thread;   
        }             
    }

    void Loop()                                             // Main worker thread loop
    {
        Job job;
        UserThread * ut;
#pragma warning(disable: 4127)
        while( true ){

            {Lock lk( mutex ); 
            while( !availableJobs )                         // No available jobs                    
                condition_jobReady.wait( lk );              // Wait for a job to become ready                

            do{ ut = userThreadGroup.Advance(lk); }         // Get a UserThread from the circular list
            while( ut->Empty() );                           // Make sure it has a job

            GetJob( ut, job, lk );                          // Get a job    
            }
            ut->Run( job, mutex );                          // Run the job
        }
#pragma warning(default: 4127)
    }

    void Wait()
    {        
        UserThread *ut = userThreadGroup.Current( mutex );              
        for( bool jobAvailable=true; jobAvailable; )        // Run every available job
        {
            Job job;
            {Lock lk( mutex );                              // Find a job
            jobAvailable = !ut->Empty();

            if( jobAvailable ) GetJob ( ut, job, lk );
            else               userThreadGroup.Deactivate( ut, lk );   // All jobs have been deployed, deactivate queue.
            }
            if( jobAvailable )                              // Run the job  
                ut->Run( job, mutex );
        }  

        while( !ut->Done() )                                // Wait for worker threads to exit.
            boost::thread::yield(); 

        userThreadGroup.Remove( ut, mutex );        
    } 
   
public:
    TP_private()
        : totalCores     ( SysCoreCount() )                 // Constructor   
        , availableJobs  ( 0              )
        , userThreadGroup( totalCores     )
    {
        for( U32 i=0; i<totalCores-1; ++i )
        {
            workerThreads[i].Init( this );
            group.create_thread( workerThreads[i] );
        }
    }  

    ~TP_private(){}

} *global_tp_ptr_private=0;



//
// External Accessors
//

#ifdef TP_SOL
#define EXT_C_HACK extern "C"
#else
#define EXT_C_HACK 
#endif

void STDCALL TP_Create()
{
    if( 0 == global_tp_ptr_private )
        global_tp_ptr_private = new TP_private();
}

unsigned int STDCALL TP_ThreadCount()
{ 
    TP_Create();  
    return global_tp_ptr_private->ThreadCount(); 
}

EXT_C_HACK void STDCALL TP_Run ( void(*fn)(void*), void* param )
{ 
    TP_Create();  
    global_tp_ptr_private->Run(fn,param ); 
}

void STDCALL TP_Wait()
{   
    TP_Create();  
    global_tp_ptr_private->Wait();         
}

void STDCALL TP_SetNumThreads_local( unsigned int threads )               
{   
    TP_Create();  
    global_tp_ptr_private->SetNumThreads_local( threads );
}

void STDCALL TP_SetNumThreads_global( unsigned int threads )
{   
    TP_Create();  
    global_tp_ptr_private->SetNumThreads_global( threads );
}
