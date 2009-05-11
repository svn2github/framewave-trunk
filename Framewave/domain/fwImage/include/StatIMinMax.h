/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATIMINMAX_H__
#define __STATIMINMAX_H__

#include "fex.h"

namespace OPT_LEVEL
{
template<typename TS1,CH cs1>
struct StatIMinGen: public fe1St<TS1,cs1>
{
    mutable TS1 min;
    mutable XMM128 mMin;
    
    StatIMinGen(){
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    IV REFR_Init() {
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }

    IV SSE2_Init()
    {
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
        TS1 *p_mMin = (TS1*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min = MIN<TS1>(min,feData.min);
    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min= MIN<TS1>(min,*s);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min = MIN<TS1>(min,feData.min);
        TS1 *p_mMin = (TS1*)&feData.mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            min = MIN<TS1>(min,p_mMin[i]);
        }
    }
};

struct StatIMin_8u_C1R: public StatIMinGen<Fw8u,C1>
{

    const static U32 nPIX_SSE = 16 * 2; // Load two registers
    class SRC1: public RegDesc< Fw8s, C1, nPIX_SSE > {};

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i sign1 = _mm_min_epu8 (r.src1[0].i,r.src1[1].i);
        mMin.i = _mm_min_epu8 (mMin.i,sign1);
    }
};



struct StatIMin_16s_C1R: public StatIMinGen<Fw16s,C1>
{

    const static U32 nPIX_SSE = 8 * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i min1 = _mm_min_epi16(r.src1[0].i,r.src1[1].i);
        mMin.i = _mm_min_epi16(mMin.i,min1);
    }

};


struct StatIMin_32f_C1R: public StatIMinGen<Fw32f,C1>
{
    const static U32 nPIX_SSE = 4 * 3; // Load three registers
    class SRC1: public RegDesc< Fw32f, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128 min1 = _mm_min_ps(r.src1[0].f,r.src1[1].f);
        min1 = _mm_min_ps(min1,r.src1[2].f);
        mMin.f = _mm_min_ps(mMin.f,min1);
    }
};


}

#endif // __STATIMINMAX_H__
