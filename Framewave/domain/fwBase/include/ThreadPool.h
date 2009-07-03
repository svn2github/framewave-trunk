/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

/**
    ThreadPool

    The thread pool has 3 primary API functions: ThreadCount(), Run(), and Wait(). 
    Their behavior is documented below. 

    The behavior of these functions is modified by two additional fucntions: 
    SetNumThreads_local() and SetNumThreads_global(). While both of these function 
    affect the number of worker threads associated with user submitted jobs, 
    SetNumThreads_local() is unique because it acts only on jobs submitted by the 
    calling user thread.  
*/

#if   defined( WIN64   ) || defined( _WIN64   ) || \
      defined( WIN32   ) || defined( _WIN32   ) || \
      defined( WINDOWS ) || defined( _WINDOWS )
#define STDCALL __stdcall
#else
#define STDCALL 
#endif

#ifdef __cplusplus
extern "C" {
#endif	

    void STDCALL TP_Create();
	/**
		Returns the number of work threads associated with the current user thread. 
        Used by software to appropriately divide large jobs.
	*/
	unsigned int STDCALL TP_ThreadCount(); 

	/**
		Place function 'fn' into the thread pool. 'fn' takes parameter 'param'. Functions 
        added to the thread pool may run out of order.
	*/
	void STDCALL TP_Run ( void(*fn)(void*), void* param );

	/**
		Wait for functions passed to Run() from this user thread to complete. 
	*/
	void STDCALL TP_Wait();

    /**
    SetNumThreads*

        These two function restrict the number of worker threads associated with a given user thread. 
        Suppose the user has four user threads in their application and 8 processor cores. 
        
        Initially, each user thread will be allowed to assign jobs to all 8 cores.
        User thread setting: { 8,8,8,8 }
        
        The the user calls SetNumThreads_local() from within three user threads with values of 1, 2, and 4.
        User thread setting: { 1,2,4,8 }

        Then then call SetNumThreads_global( 3 ).
        User thread setting: { 1,2,3,3 }

        Finally call SetNumThreads_global( 8 ).
        User thread setting: { 8,8,8,8 }
    */
   

	/**
		Set the max worker threads for this user thread. 
	*/
	void STDCALL TP_SetNumThreads_local( unsigned int threads ) ;

	/**
		 Set the max worker threads for the entire thread pool. 
	*/
	void STDCALL TP_SetNumThreads_global( unsigned int threads );	

#ifdef __cplusplus
}
#endif


#endif // __THREADPOOL_H__



