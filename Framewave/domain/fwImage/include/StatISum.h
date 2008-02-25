/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISUM_H__
#define __STATISUM_H__

#include "fex.h"

namespace OPT_LEVEL
{


struct StatISum_8u: public fe1St<Fw8u,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

	mutable  Fw32s sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatISum_8u(){ sum = 0;}
	IV REFR_Init() { sum = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum += feData.sum;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum += feData.sum;
        sum += feData.mSum.s32[0] + feData.mSum.s32[1]+ feData.mSum.s32[2]+feData.mSum.s32[3];

    }

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
		sum = (sum + *s1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcHi,imRes,src;
		src.i = r.src1[0].i;
        XMM128 src2;
        src2.i = r.src1[1].i;
		CBL_SSE2::Unpack8UTo16U(src.i,srcHi.i);
        CBL_SSE2::Unpack8UTo16U(src2.i,imRes.i);
		
        src.i = _mm_add_epi16(src.i,src2.i);
        imRes.i = _mm_add_epi16(srcHi.i,imRes.i);
        imRes.i = _mm_add_epi16(imRes.i,src.i);
        CBL_SSE2::Unpack16UTo32U(imRes.i,src2.i);
        imRes.i = _mm_add_epi32(imRes.i,src2.i);
        mSum.i = _mm_add_epi32(mSum.i,imRes.i);
	}
};

struct StatISum_16s: public fe1St<Fw16s,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16s))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

	mutable  Fw64s sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatISum_16s(){ sum = 0;}
	IV REFR_Init() { sum = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum += feData.sum;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum += feData.sum;
        sum += feData.mSum.s64[0] + feData.mSum.s64[1];
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV REFR(const Fw16s *s1 ) const           // REFR Pixel function
	{
		sum = (sum + *s1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcHi,imRes,src;
		src.i = r.src1[0].i;
        XMM128 src2;
        src2.i =r.src1[1].i;
		CBL_SSE2::Unpack16STo32S(src.i,srcHi.i);
        CBL_SSE2::Unpack16STo32S(src2.i,imRes.i);

		src.i = _mm_add_epi32(src.i,srcHi.i);
        imRes.i = _mm_add_epi32(src2.i,imRes.i);
        imRes.i = _mm_add_epi32(src.i,imRes.i);

        CBL_SSE2::Unpack32STo64S(imRes.i,srcHi.i);
        imRes.i = _mm_add_epi64(imRes.i,srcHi.i);
        mSum.i= _mm_add_epi64(mSum.i,imRes.i);
	}
};

}

#endif // __STATISUM_H__
