/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#include <iostream>
#include <iomanip>


#if defined( WIN64   ) || defined( _WIN64   ) || \
    defined( WIN32   ) || defined( _WIN32   ) || \
    defined( WINDOWS ) || defined( _WINDOWS )

#define STDCALL __stdcall
#include <windows.h>

class TimerWall
{
    LARGE_INTEGER start, stop, freq;

public:
    TimerWall() { QueryPerformanceFrequency( &freq ); }

    void Start () { QueryPerformanceCounter(&start); }
    void Sample() { QueryPerformanceCounter(&stop ); }

	double Elapsed()
    {
        double time = (double)(stop.QuadPart-start.QuadPart) / freq.QuadPart;
        return time;
    }

	double SampleElapsed()
    {
        Sample();
        return Elapsed();
    }
};

#else

#define STDCALL 
#include <sys/time.h>

class TimerWall
{
    timeval tval;
    double start, stop;

public:
    TimerWall() {}

    void Start()
    {
        gettimeofday(&tval, NULL);
        start = (double)(tval.tv_sec * 1E6 + tval.tv_usec);
    }

    void Sample()
    {
        gettimeofday(&tval, NULL);
        stop = (double)(tval.tv_sec * 1E6 + tval.tv_usec);
    }

	double Elapsed()
    {
        double time = (stop - start) * 1E-6;
        return time;
    }

	double SampleElapsed()
    {
        Sample();
        return Elapsed();
    }
};

#endif 

#include "fwBase.h"

class TimerCycle
{	
	Fw64u start, stop;
public:
	TimerCycle(){ Start(); }
	void Start (){ start = ::fwGetCpuClocks(); }
	void Sample(){ stop  = ::fwGetCpuClocks(); }

	Fw64u Elapsed(){ return stop-start; }
	Fw64u SampleElapsed(){ Sample(); return Elapsed(); }
};


//const static size_t NUM_THREADS  = ::fwGetNumThreads();

static std::string NoPath( const std::string & file )
{
    size_t pos = file.find_last_of( "/\\" );

    if( std::string::npos == pos )pos  = 0;
    else                          pos += 1;

    return file.substr( pos );
}


#define REPORT( text )  \
    std::cout<< std::setw(16) << NoPath(__FILE__) << "(" << __LINE__ << ") " << text << std::endl;


#define EXPECT( A, reason )         \
    if( !(A) )                      \
    {                               \
        REPORT( reason );           \
        exit( -1 );                 \
    }

#define EXPECT_ERR(COMP,A,B,SA,SB,L){                   \
    std::cout<<"ERROR("<<L<<"): "<<A<<COMP<<B<<" {"<<SA<<"!="<<SB<<"}"<<std::endl;}


// expect a == b
//#define EXPECT_EQUAL( pass, A, B, var ){				\
//	bool tmp=(A<=(B*(1+var)))&&							\
//             (A>=(B*(1-var)));							\
//	if(!tmp){											\
//		EXPECT_ERR( " != ", A, B, #A, #B, __LINE__  )	\
//		pass = false;									\
//	 }													\
//}

#define EXPECT_EQUAL( A, B ){				            \
    if( !(A==B) ){                                      \
    EXPECT_ERR( "!=", #A, #B, A, B, __LINE__  )	\
	 }													\
}

// expect a < b
#define EXPECT_LESS( pass, A, B, var ){					\
	bool tmp=(A<=(B*(1+var)));							\
	if(!tmp){											\
		EXPECT_ERR( " !< ", A, B, #A, #B, __LINE__ )	\
		pass = false;									\
	 }													\
}

#endif // __UNITTEST_H__
