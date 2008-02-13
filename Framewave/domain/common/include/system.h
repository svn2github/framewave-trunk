/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#pragma warning( disable: 4127 )    //warning C4127: conditional expression is constant
#pragma warning( disable: 4530 )    

// FW INTERNAL MACROS ========================================================

// Operating System

#if     defined( WIN64   ) || defined( _WIN64   ) || \
        defined( WIN32   ) || defined( _WIN32   ) || \
        defined( WINDOWS ) || defined( _WINDOWS )

 #define FW_WIN

#elif   defined( LIN64   ) || defined( _LIN64 ) || \
        defined( LIN32   ) || defined( _LIN32 ) || \
        defined( LINUX   ) || defined( _LINUX )

 #define FW_LIN

#elif   defined( SOL64   ) || defined( _SOL64   ) || \
        defined( SOL32   ) || defined( _SOL32   ) || \
        defined( SOLARIS ) || defined( _SOLARIS )

 #define FW_SOL

#elif   defined( MAC64     ) || defined( _MAC64   ) || \
        defined( MAC32     ) || defined( _MAC32   ) || \
        defined( __APPLE__ )

 #define FW_MAC

#endif 

// Bitness

#if     defined( WIN64 ) || defined( _WIN64 ) || \
        defined( LIN64 ) || defined( _LIN64 ) || \
        defined( SOL64 ) || defined( _SOL64 ) || \
        defined( MAC64 ) || defined( _MAC64 )

 #define FW_SYS64
 #define FW_XMM_REG_COUNT 16

#elif   defined( WIN32 ) || defined( _WIN32 ) || \
        defined( LIN32 ) || defined( _LIN32 ) || \
        defined( SOL32 ) || defined( _SOL32 ) || \
        defined( MAC32 ) || defined( _MAC32 )

 #define FW_SYS32
 #define FW_XMM_REG_COUNT 8

#endif

// Compiler

#if     defined( _MSC_VER )

 #define FW_MSVC

#elif   defined( __GNUC__ )

 #define FW_GNUC

#elif   defined( __SUNPRO_CC )

 #define FW_SUNC

#endif


//=======================================
//        External Headers
//=======================================

#if defined( FW_WIN )
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <malloc.h>
#endif

#include <math.h>           // using pow, modf
#include <float.h>          // using _finite
#include <assert.h>

#if defined( FW_LIN ) || defined( FW_SOL ) || defined( FW_MAC )
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#endif

//SKiran: This is not required since FW use Boost LIBS and help to build on 32bit Linux.
//#if defined( FW_LIN )
//#include <asm/atomic.h>
//#endif

#if defined( FW_SOL )
#include <sys/atomic.h>
#include <malloc.h>
#include <stdint.h>
#include <ieeefp.h>
#include <alloca.h>
#endif

#if defined (IS_SYSTEM_CPP)

#if !defined (BLD_REF_ONLY)
#error "system.cpp must be a ref-only build"
#endif

#ifdef FW_MSVC
#include <intrin.h> // to get cpuid, rdtsc, aligned malloc, free
#else
#include "fw_ref_mmdefs.h"
#endif

#else // not system.cpp

#if defined (BLD_REF_ONLY)

#include "fw_ref_mmdefs.h"

#else // not BLD_REF_ONLY

#ifdef FW_MSVC
#include <intrin.h>
#else
#include <pmmintrin.h>
#endif

#endif // BLD_REF_ONLY

#endif // system.cpp

#if defined (OPT_REFR)
#ifdef CBL_LIBRARY
#undef CBL_LIBRARY
#endif // CBL_LIBRARY
#define CBL_LIBRARY CBL_LIB_REFR
#endif // OPT_LEVEL == REFR

#if defined (OPT_SSE2)
#ifdef CBL_LIBRARY
#undef CBL_LIBRARY
#endif // CBL_LIBRARY
#define CBL_LIBRARY CBL_LIB_SSE2
#endif // defined (OPT_SSE2)

#if defined (OPT_SSE3)
#ifdef CBL_LIBRARY
#undef CBL_LIBRARY
#endif // CBL_LIBRARY
#define CBL_LIBRARY CBL_LIB_SSE3
#endif // defined (OPT_SSE3)

#if defined (OPT_F10H)
#ifdef CBL_LIBRARY
#undef CBL_LIBRARY
#endif // CBL_LIBRARY
#define CBL_LIBRARY CBL_LIB_F10H
#endif // defined (OPT_F10H)

#include <math.h>
#include "cbl/cbl.h"
using namespace CBL_LIBRARY;

#if defined( FW_WIN )
//------------------------------------------------------
//            WINDOWS SPECIFIC
//------------------------------------------------------

#define STATIC
#define SYS_FINITE            _finite
#define SYS_WEAKINLINE        inline
#define SYS_INLINE            __forceinline
#define SYS_FORCEALIGN_16     __declspec(align(16))
#define SYS_NOINLINE
#define SCALBN                _scalb

#ifndef _MM_ALIGN16           // GCC does not define this
#define _MM_ALIGN16           __declspec(align(16))
#endif

#elif defined( FW_LIN ) || defined( FW_MAC )
//------------------------------------------------------
//            LINUX SPECIFIC
//------------------------------------------------------

#define STATIC
#define SYS_FINITE            finite
//#define SYS_INLINE          __inline__
#define SYS_WEAKINLINE        __inline__
#define SYS_INLINE            __inline__                //TODO: Investigate this for each release
#define SYS_FORCEALIGN_16     __attribute__((aligned(16)))
#define _MM_ALIGN16           __attribute__((aligned(16)))
#define SYS_NOINLINE
#define SCALBN                scalbn

#elif defined( FW_SOL )
//------------------------------------------------------
//            SOLARIS SPECIFIC
//------------------------------------------------------

#define STATIC
#define SYS_FINITE            finite
#define SYS_WEAKINLINE        inline
#define SYS_INLINE            inline
#define SYS_FORCEALIGN_16     // __attribute__((aligned(16)))
#define _MM_ALIGN16           // __attribute__((aligned(16)))
#define SYS_NOINLINE
#define SCALBN                scalbn

#endif


#if defined(FW_WIN ) && !defined(_IS_FW_LIB)
    #if defined( FW_BASE_CPP ) || defined ( IS_SYSTEM_CPP )
    __declspec(dllexport) extern void *    x_baseDataPtr;
    #else
    __declspec(dllimport) extern void *    x_baseDataPtr;
    #endif
#else
                          extern void *    x_baseDataPtr;
#endif 

FwCpuType GetCpuType();
Fw64u get_rdtsc();

void *AlignedMalloc ( size_t length );
void AlignedFree ( void *ptr );

namespace OPT_LEVEL
{

// the T_OFFSET is intended to be a signed, 32 bit integer on 32 bit architectures 
// and signed 64 bit integer on 64 bit architectures.  It is intended for use in pointer arithmetics.
// See function FW_REF::Offset for details.

#ifdef FW_SYS64
typedef long long T_OFFSET;
#else
typedef int T_OFFSET;
#endif

// MIN & MAX

#ifdef MIN
#error MIN macro should not be defined, MIN used as template function
#endif

#ifdef MAX
#error MAX macro should not be defined, MAX used as template function
#endif

// MIN and MAX defined as inlined template function
// Make sure these functions get INLINED
template <typename T> SYS_INLINE T MIN(const T &a, const T &b)                            { return ( a < b ? a : b );       }
template <typename T> SYS_INLINE T MIN(const T &a, const T &b, const T &c)                { return MIN<T>(a, MIN(b, c));    }
template <typename T> SYS_INLINE T MIN(const T &a, const T &b, const T &c, const T &d)    { return MIN<T>(a, MIN(b, c, d)); }
template <typename T> SYS_INLINE T MAX(const T &a, const T &b)                            { return ( a > b ? a : b );       }
template <typename T> SYS_INLINE T MAX(const T &a, const T &b, const T &c)                { return MAX<T>(a, MAX(b, c));    }
template <typename T> SYS_INLINE T MAX(const T &a, const T &b, const T &c, const T &d)    { return MAX<T>(a, MAX(b, c, d)); }

//
// Dispatch.
//    Matches implementations to CPU feature set. 
//
enum DispatchType
{
    DT_REFR,
    DT_SSE2,
    DT_SSE3,
    DT_F10H,
    DT_COUNT
};

enum ENUM_UNIQUE { UNIQUE_CONSTRUCTOR };

struct BaseData
{
    DispatchType dt;

    BaseData( ENUM_UNIQUE );
    BaseData(){ assert(false); }
};


static bool SetDispatchType( DispatchType &dt, FwCpuType cpu )
{
    cpu = MIN( cpu, GetCpuType() );
    switch( cpu )
    {
    case fwCpuUnknown:      dt = DT_REFR;    return true;
    case fwCpuSSE2:         dt = DT_SSE2;    return true;
    case fwCpuSSE3:         dt = DT_SSE3;    return true;
    case fwCpuFamily10h:    dt = DT_F10H;    return true;
    default:                 dt = DT_REFR;    return false;
    }
}

class Dispatch
{    
    static BaseData & Data()
    {
        static BaseData & dat = *(BaseData*)fwBaseData();  // void* to hide implementation from user.
        x_baseDataPtr = &dat;                               // save away the pointer so we don't have to
                                                            // come this way again
        return dat;
    }

public:

//      Gets data from the table stored in fwBase.dll.
//      We must ensure that there is only one copy of the dispatch table. 

    SYS_INLINE static DispatchType &LibType()
    {
        if (x_baseDataPtr)                                    // check if we've already initialized once
            return ((BaseData*) x_baseDataPtr)->dt;           // if so, use the saved global pointer
        return Data().dt;
    }

//      Get's the dispatch type, ensuring that the maximum returned 
//      dispatch type is not more than what is specified in the parameter

    template <DispatchType DT>
    SYS_INLINE static DispatchType Type( )
    {
        DispatchType & dt = LibType();
        return dt;
    }


//      Get the appropriate dispatch type. 
//      Returns SSE2 only if the passed pixel function pointer is valid. 

    SYS_INLINE static DispatchType Type( bool sse2 )
    { //TODO: use shared memory to avoid call overhead?
        DispatchType & type = LibType();

        switch( type )
        {
        case DT_F10H: // fall through
        case DT_SSE3: // fall through
        case DT_SSE2: return sse2 ? DT_SSE2 : DT_REFR;
        default:      return DT_REFR;
        }
    }


    static bool SetType( FwCpuType cpu )
    {
        return SetDispatchType( Data().dt, cpu );
    }

    static void SetBest()
    {
        SetType( GetCpuType() );
    }
};


    template < >
    SYS_INLINE DispatchType Dispatch::Type<DT_F10H>()
    {
#if   defined (OPT_F10H)
        return DT_F10H;
#elif defined (OPT_SSE3)
        return DT_SSE3;
#elif defined (OPT_SSE2)
        return DT_SSE2;
#elif defined (OPT_REFR)
        return DT_REFR;
#else
        DispatchType & dt = LibType();
        return MIN( DT_F10H, dt );
#endif
    }


    template < >
    SYS_INLINE DispatchType Dispatch::Type<DT_SSE3>()
    {
#if   defined (OPT_F10H) || defined (OPT_SSE3)
        return DT_SSE3;
#elif defined (OPT_SSE2)
        return DT_SSE2;
#elif defined (OPT_REFR)
        return DT_REFR;
#else
        DispatchType & dt = LibType();
        return MIN( DT_SSE3, dt );
#endif
    }

    template < >
    SYS_INLINE DispatchType Dispatch::Type<DT_SSE2>()
    {
#if   defined (OPT_F10H) || defined (OPT_SSE3) || defined (OPT_SSE2)
        return DT_SSE2;
#elif defined (OPT_REFR)
        return DT_REFR;
#else
        DispatchType & dt = LibType();
        return MIN( DT_SSE2, dt );
#endif
    }

    template < >
    SYS_INLINE DispatchType Dispatch::Type<DT_REFR>()
    {
        return DT_REFR;
    }



#define MAX_THREADS 16
// ThreadPool wrapper


namespace ThreadPool
{
    SYS_INLINE unsigned int ThreadCount()                       { return fwGetNumThreads();        }
    SYS_INLINE void Run( void(*fn)(void*), void* param )        {        fwRun( fn, param );       }
    SYS_INLINE void Wait()                                      {        fwWait();                 }
    SYS_INLINE void SetNumThreads_local ( unsigned int threads ){ fwSetNumThreads_local(threads);  }
    SYS_INLINE void SetNumThreads_global( unsigned int threads ){ fwSetNumThreads      (threads);  }
    SYS_INLINE void SetNumThreads( unsigned int threads )       { fwSetNumThreads      (threads);  }
    SYS_INLINE void Init()                                      { fwStaticInit();                  }
};

inline BaseData::BaseData( ENUM_UNIQUE )
{
    SetDispatchType( dt, GetCpuType() );
}

} // namespace OPT_LEVEL


// UNDEF MACROS
// NOT VISIBLE TO OTHER CPP FILES
// ONLY VISIBLE TO system.cpp

#ifndef IS_SYSTEM_CPP

#ifdef FW_WIN
#undef FW_WIN
#endif

#ifdef FW_LIN
#undef FW_LIN
#endif

#ifdef FW_SOL
#undef FW_SOL
#endif

#ifdef FW_MAC
#undef FW_MAC
#endif

#ifdef FW_SYS64
#undef FW_SYS64
#endif

#ifdef FW_SYS32
#undef FW_SYS32
#endif

#ifdef FW_MSVC
#undef FW_MSVC
#endif

#ifdef FW_GNUC
#undef FW_GNUC
#endif

#ifdef FW_SUNC
#undef FW_SUNC
#endif

#endif


#endif // __SYSTEM_H__
