/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#define FW_BASE_CPP

#include "fwBase.h"
#include "fwdev.h"
#include "ThreadPool.h"
#include <memory.h>
#include <stdlib.h>

using namespace OPT_LEVEL;

void* STDCALL fwMalloc (int length)
{
	return AlignedMalloc ( length );
}

void STDCALL fwFree (void* ptr)
{
	AlignedFree (ptr);
}

void* STDCALL fwAlignPtr (void* ptr, int alignBytes)
{
	if(alignBytes < 1) return NULL;
	
	int rem = (int)(reinterpret_cast<Fw64u>(ptr) % alignBytes); 
	char *alignedPtr = (rem ==0) ? (char*) ptr : (((char*)ptr)-rem+alignBytes);
	return (void*)alignedPtr;
}

FwStatus STDCALL fwStaticInit()
{
    TP_Create();
	Dispatch::SetBest();	
	return fwStsNoErr;
}

FwStatus STDCALL fwStaticInitCpu( FwCpuType cpu )
{
	if( Dispatch::SetType( cpu ) ) return fwStsNoErr;
	else					       return fwStsCpuMismatch;
}

int STDCALL fwGetInitType()
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_F10H:   return fwCpuFamily10h;
    case DT_SSE3:   return fwCpuSSE3;
    case DT_SSE2:   return fwCpuSSE2;
	default:        return fwCpuUnknown;
	}
}

FwCpuType STDCALL fwGetCpuType()
{
	return GetCpuType();
}

Fw32u STDCALL fwGetNumThreads()
{
	return TP_ThreadCount(); 
}

FwStatus STDCALL fwSetNumThreads( unsigned int numThr )
{
	TP_SetNumThreads_global( numThr );
	return fwStsNoErr;
}

FwStatus STDCALL fwSetNumThreads_local( unsigned int numThr )
{
	TP_SetNumThreads_local( numThr );
	return fwStsNoErr;
}

void STDCALL fwRun( void(*fn)(void*), void* param ){ TP_Run ( fn, param ); }
void STDCALL fwWait()                              { TP_Wait(           ); } 



//
// fwCore* Functions
//

const char * STDCALL fwGetStatusString( FwStatus StsCode )
{	
	switch( StsCode )
	{
		#define LIST_ITEM( a, b ) case a: return b;
		#include "errorList.h"
		#undef LIST_ITEM
	}
	return "Unknown error"; 
}


	
//! fwCoreGetCpuClocks
Fw64u STDCALL fwGetCpuClocks()
{
	return get_rdtsc();
}

/*! Return a pointer to a unuiqe data store.
*/
void * STDCALL fwBaseData()
{
	static BaseData data( UNIQUE_CONSTRUCTOR );	// The only instance of BaseData.
	return (void*) &data;
}



void Mmm_to_mm( const char * in, char * out )
{
#define E3( a,b ) (0==memcmp(a,b,3))
	if(      E3(in,"Jan") ) memcpy( out, "01", 2 );
	else if( E3(in,"Feb") ) memcpy( out, "02", 2 );
	else if( E3(in,"Mar") ) memcpy( out, "03", 2 );
	else if( E3(in,"Apr") ) memcpy( out, "04", 2 );
	else if( E3(in,"May") ) memcpy( out, "05", 2 );
	else if( E3(in,"Jun") ) memcpy( out, "06", 2 );
	else if( E3(in,"Jul") ) memcpy( out, "07", 2 );
	else if( E3(in,"Aug") ) memcpy( out, "08", 2 );
	else if( E3(in,"Sep") ) memcpy( out, "09", 2 );
	else if( E3(in,"Oct") ) memcpy( out, "10", 2 );
	else if( E3(in,"Nov") ) memcpy( out, "11", 2 );
	else if( E3(in,"Dec") ) memcpy( out, "12", 2 );
	else					memcpy( out, "--", 2 );
#undef E3	
}


const FwLibraryVersion* STDCALL fwGetLibVersion() {  

#define NUM( n ) int build=n;
#include "build.h"
#undef NUM

	static FwLibraryVersion v;
	static char time[] = "yyyy mm dd hh:mm:ss";	
	
	memset( &v, 0, sizeof(v) );

	const char * d = __DATE__;
	const char * t = __TIME__;

	Mmm_to_mm( d,   time+5 );		// mm	
	memcpy( time+0, d+7, 4 );		// yyyy
	memcpy( time+8, d+4, 2 );		// dd

	memcpy( time+11, t, 8 );		// hh:mm:ss

	v.major		= FW_MAJOR;
	v.minor		= FW_MINOR;	
	v.majorBuild = FW_MAJOR;
	v.build     = build;
	v.Version	= FW_VERSTR;
	v.BuildDate	= time;
#pragma warning( push )
#pragma warning( disable: 4996 )
	memcpy (v.targetCpu, "0x86", 4);
#pragma warning( pop )

	// Library specific
	v.Name		= "FWBase";
	return &v;	
}
