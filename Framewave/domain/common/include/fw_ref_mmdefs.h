/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FW_REF_MMDEFS_H__
#define __FW_REF_MMDEFS_H__

#ifdef BLD_REF_ONLY

    #define __m64   Ref_m64
    #define __m128  Ref_m128
    #define __m128i Ref_m128i
    #define __m128d Ref_m128d

#endif

#ifdef FW_MSVC
	#ifdef BLD_REF_ONLY
   
   // SSEPlus Integration
   #define _INCLUDED_MM2   
   #define _INCLUDED_EMM  
	typedef union __m64
	{
	    unsigned __int64    m64_u64;
	 } __m64;
	typedef union __m128 {
	     float               m128_f32[4];
	 } __m128;
	typedef union __m128i {
	    __int8              m128i_i8[16];
	 } __m128i;
	typedef struct __m128d {
	    double              m128d_f64[2];
	 } __m128d;
    #define RZERO_m128  { __m128  tmp = { 0.0, 0.0, 0.0, 0.0 };                             return tmp; }
    #define RZERO_m128i { __m128i tmp = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; return tmp; }
    #define RZERO_m128d { __m128d tmp = { 0.0, 0.0 };                                       return tmp; }
	#else
	#include <intrin.h>
	#endif
#else
    // SSEPlus integration
   #define _XMMINTRIN_H_INCLUDED
   #define _EMMINTRIN_H_INCLUDED
    
   #define __SYS_XMMINTRIN_INTRIN_H
   #define __SYS_EMMINTRIN_INTRIN_H
    
	#ifdef BLD_REF_ONLY
	typedef union __m64 {
	    int                  m64_i32[2];
	} __m64;
	typedef union __m128 {
	     float               m128_f32[4];
	 } __m128;
	typedef union __m128i {
	    long long              m128i_i64[2];
	 } __m128i;
	typedef struct __m128d {
	    double              m128d_f64[2];
	} __m128d;
    #define RZERO_m128  { __m128  tmp = { 0.0, 0.0, 0.0, 0.0 }; return tmp; }
    #define RZERO_m128i { __m128i tmp = { 0, 0 };               return tmp; }
    #define RZERO_m128d { __m128d tmp = { 0.0, 0.0 };           return tmp; }
	#else
	#include <pmmintrin.h>
	#endif
#endif

#if defined (BLD_REF_ONLY)

#define _MM_SHUFFLE(fp3,fp2,fp1,fp0) 0
#define _MM_SHUFFLE2(x,y) 0

static void FwInterrupt3()
{
#if defined( FW_WIN )
    DebugBreak();
#else
    asm("int $3");
#endif
}

#include "fw_ref_xmmintrin.h"
#include "fw_ref_emmintrin.h"
#include "fw_ref_pmmintrin.h"

#endif

#endif // __FW_REF_MMDEFS_H__
