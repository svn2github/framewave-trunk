/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISTICS_MINMAX_H__
#define __STATISTICS_MINMAX_H__

#include "fex.h"
#include "SSE2Intrinsic.h"


namespace OPT_LEVEL
{
template<typename TS1,CH cs1>
struct StatMaxGen: public fe1St<TS1,cs1>
{
    mutable TS1 max;
    mutable XMM128 mMax;

    StatMaxGen(){
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
    }
    IV REFR_Init() {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = MAX<TS1>(max,feData.max);
    }
    IV SSE2_Init()
    {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
        TS1 *p_mMax = (TS1*)&mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }
    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        max= MAX<TS1>(max,*s);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        max = MAX<TS1>(max,feData.max);

        TS1 *p_mMax = (TS1*)&feData.mMax;

        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1) ) ; i++)
        {
            max = MAX<TS1>(max,p_mMax[i]);
        }
    }
};

template<typename TS1,CH cs1>
struct StatMinGen: public fe1St<TS1,cs1>
{
    mutable TS1 min;
    mutable XMM128 mMin;
    
    StatMinGen(){
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



struct StatMin_64u: public StatMinGen<Fw64u,C1>
{
    FEX_REF
};

struct StatMax_64u: public StatMaxGen<Fw64u,C1>
{
    FEX_REF
};
struct StatMin_8s: public StatMinGen<Fw8s,C1>
{

    const static U32 nPIX_SSE = 16 * 2; // Load two registers
    class SRC1: public RegDesc< Fw8s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i sign1 = mm_min_epi8(r.src1[0].i,r.src1[1].i);
        mMin.i = mm_min_epi8(mMin.i,sign1);
    }
};

struct StatMax_8s: public StatMaxGen<Fw8s,C1>
{

    const static U32 nPIX_SSE = 16 * 2; // Load two registers
    class SRC1: public RegDesc< Fw8s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i sign1 = mm_max_epi8(r.src1[0].i,r.src1[1].i);
        mMax.i = mm_max_epi8(mMax.i,sign1);
    }
};


struct StatMax_16s: public StatMaxGen<Fw16s,C1>
{

    const static U32 nPIX_SSE = 8 * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i sign1 = _mm_max_epi16(r.src1[0].i,r.src1[1].i);
        mMax.i = _mm_max_epi16(mMax.i,sign1);
    }
};

struct StatMax_32s: public StatMaxGen<Fw32s,C1>
{

    const static U32 nPIX_SSE = 4 * 3; // Load three registers
    class SRC1: public RegDesc< Fw32s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2_Init()
    {
        max = CBL_LIBRARY::Limits<Fw32s>::MinValue();
        mMax.i = _mm_set1_epi32(CBL_LIBRARY::Limits<S32>::MinValue());
    }

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 inter_res, com_inter,res1;
        inter_res.i = _mm_cmpgt_epi32 (r.src1[0].i,r.src1[1].i);
        com_inter.i = _mm_andnot_si128(inter_res.i,r.src1[1].i);
        inter_res.i = _mm_and_si128(r.src1[0].i, inter_res.i);
        res1.i  = _mm_or_si128(inter_res.i,com_inter.i);

        inter_res.i = _mm_cmpgt_epi32 (res1.i,mMax.i);
        com_inter.i = _mm_andnot_si128(inter_res.i,mMax.i);
        inter_res.i = _mm_and_si128(res1.i, inter_res.i);
        mMax.i  = _mm_or_si128(inter_res.i,com_inter.i);

        inter_res.i = _mm_cmpgt_epi32 (r.src1[2].i,mMax.i);
        com_inter.i = _mm_andnot_si128(inter_res.i,mMax.i);
        inter_res.i = _mm_and_si128(r.src1[2].i, inter_res.i);
        mMax.i  = _mm_or_si128(inter_res.i,com_inter.i);
    }
};

struct StatMax_32f: public StatMaxGen<Fw32f,C1>
{
    const static U32 nPIX_SSE = 4 * 3; // Load three registers
    class SRC1: public RegDesc< Fw32f, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128 max1 = _mm_max_ps(r.src1[0].f,r.src1[1].f);
        max1 = _mm_max_ps(max1,r.src1[2].f);
        mMax.f = _mm_max_ps(mMax.f,max1);
    }

};

struct StatMax_64f: public StatMaxGen<Fw64f,C1>
{
    const static U32 nPIX_SSE = 2 * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128d max1 = _mm_max_pd(r.src1[0].d,r.src1[1].d);
        mMax.d = _mm_max_pd(mMax.d,max1);
    }
};


//
struct StatMin_16s: public StatMinGen<Fw16s,C1>
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

struct StatMin_32s: public StatMinGen<Fw32s,C1>
{
    FEX_SSE2_REF

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 inter_res, com_inter;
        inter_res.i = _mm_cmplt_epi32 (r.src1[0].i,mMin.i);
        com_inter.i = _mm_andnot_si128(inter_res.i,mMin.i);
        inter_res.i = _mm_and_si128(r.src1[0].i, inter_res.i);
        mMin.i  = _mm_or_si128(inter_res.i,com_inter.i);
    }
};

struct StatMin_32f: public StatMinGen<Fw32f,C1>
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

struct StatMin_64f: public StatMinGen<Fw64f,C1>
{
    const static U32 nPIX_SSE = 2 * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128d min1 = _mm_min_pd(r.src1[0].d,r.src1[1].d);
        mMin.d = _mm_min_pd(mMin.d,min1);
    }
};

template<typename TS1,CH cs1>
struct StatMinMaxGen: public fe1St<TS1,cs1>
{
    mutable TS1 max;
    mutable TS1 min;
    mutable XMM128 mMax;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatMinMaxGen()
    {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    IV REFR_Init()
    {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = MAX<TS1>(max,feData.max);
        min = MIN<TS1>(min,feData.min);
    }
    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min= MIN<TS1>(min,*s);
        max= MAX<TS1>(max,*s);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min = MIN<TS1>(min,feData.min);
        max = MAX<TS1>(max,feData.max);

        TS1 *p_mMin = (TS1*)&feData.mMin;
        TS1 *p_mMax = (TS1*)&feData.mMax;

        for (int i =0; i < (XMMREGSIZE / sizeof(TS1)) ; i++)
        {
            min = MIN<TS1>(min,p_mMin[i]);
            max = MAX<TS1>(max,p_mMax[i]);
        }
    }

    IV SSE2_Init()
    {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
        min = CBL_LIBRARY::Limits<TS1>::MaxValue();
        TS1 *p_mMin = (TS1*)&mMin;
        TS1 *p_mMax = (TS1*)&mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }
};

struct StatMinMax_8u: public StatMinMaxGen<Fw8u,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u) ) * 3; // Load three registers
    class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 sign_min,sign_max;

        sign_min.i = _mm_min_epu8(r.src1[0].i,r.src1[1].i);
        sign_max.i = _mm_max_epu8(r.src1[0].i,r.src1[1].i);

        sign_min.i = _mm_min_epu8(sign_min.i,r.src1[2].i);
        sign_max.i = _mm_max_epu8(sign_max.i,r.src1[2].i);

        mMin.i = _mm_min_epu8(mMin.i,sign_min.i);
        mMax.i = _mm_max_epu8(mMax.i,sign_max.i);
    }
};


SYS_INLINE STATIC __m128i  Minxmm16u(const __m128i &val1, const __m128i &val2)
{
       __m128i  min1 = _mm_min_epi16(val1,val2);
        __m128i t1    = _mm_srai_epi16(val1, 15);
        __m128i t2    = _mm_srai_epi16(val2, 15);
        __m128i xorval = _mm_xor_si128(t1,t2);

        t1 = _mm_and_si128(val1,xorval);
        t2 = _mm_and_si128(val2,xorval);

        min1 = _mm_xor_si128(t1,min1);
       return  _mm_xor_si128(t2,min1);
}

SYS_INLINE STATIC __m128i  Maxxmm16u(const __m128i &val1, const __m128i &val2)
{
       __m128i  max1 = _mm_max_epi16(val1,val2);
        __m128i t1    = _mm_srai_epi16(val1, 15);
        __m128i t2    = _mm_srai_epi16(val2, 15);
        __m128i xorval = _mm_xor_si128(t1,t2);

        t1 = _mm_and_si128(val1,xorval);
        t2 = _mm_and_si128(val2,xorval);

        max1 = _mm_xor_si128(t1,max1);
       return  _mm_xor_si128(t2,max1);
}

struct StatMinMax_16u: public StatMinMaxGen<Fw16u,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16u) ) * 2; // Load two registers
    class SRC1: public RegDesc< Fw16u, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i min1,max1;

        min1 = Minxmm16u(r.src1[0].i,r.src1[1].i);
        max1 = _mm_xor_si128(min1,r.src1[0].i);
        max1 = _mm_xor_si128(max1,r.src1[1].i);

        mMin.i = Minxmm16u(mMin.i,min1);
        mMax.i = Maxxmm16u(mMax.i,max1);
    }
};

struct StatMinMax_16s: public StatMinMaxGen<Fw16s,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16s)) * 3; // Load three registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 sign_min,sign_max;
        sign_min.i = _mm_min_epi16(r.src1[0].i,r.src1[1].i);
        sign_max.i = _mm_max_epi16(r.src1[0].i,r.src1[1].i);
        sign_min.i = _mm_min_epi16(r.src1[2].i,sign_min.i);
        sign_max.i = _mm_max_epi16(r.src1[2].i,sign_max.i);
        mMin.i = _mm_min_epi16(sign_min.i,mMin.i);
        mMax.i = _mm_max_epi16(sign_max.i,mMax.i);
    }
};

struct StatMinMax_32u: public StatMinMaxGen<Fw32u,C1>
{
    const static U32 nPIX_SSE =  (XMMREGSIZE / sizeof(Fw32u)) * 2; // Load two registers
    class SRC1: public RegDesc< Fw32u, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i max1,min1;

        max1 = mm_max_epu32(r.src1[0].i,r.src1[1].i);
        min1 = _mm_xor_si128(max1,r.src1[0].i);
        min1 = _mm_xor_si128(min1,r.src1[1].i);
        mMax.i = mm_max_epu32(mMax.i,max1);
        mMin.i  = mm_min_epu32(mMin.i,min1);
    }
};

struct StatMinMax_32s: public StatMinMaxGen<Fw32s,C1>
{
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 inter_res, com_inter,max_inter,max_com;
        inter_res.i = _mm_cmplt_epi32 (r.src1[0].i,mMin.i);
        max_inter.i = _mm_cmpgt_epi32 (r.src1[0].i,mMax.i);
        com_inter.i = _mm_andnot_si128(inter_res.i,mMin.i);
        inter_res.i = _mm_and_si128(r.src1[0].i, inter_res.i);
        mMin.i  = _mm_or_si128(inter_res.i,com_inter.i);
        max_com.i = _mm_andnot_si128(max_inter.i,mMax.i);
        max_inter.i = _mm_and_si128(r.src1[0].i, max_inter.i);
        mMax.i  = _mm_or_si128(max_inter.i,max_com.i);
    }
};

struct StatMinMax_32f: public StatMinMaxGen<Fw32f,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw32f))  * 3; // Load three registers
    class SRC1: public RegDesc< Fw32f, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128 min1,max1;
        min1 = _mm_min_ps(r.src1[0].f,r.src1[1].f);
        min1 = _mm_min_ps(min1,r.src1[2].f);
        max1 = _mm_max_ps(r.src1[0].f,r.src1[1].f);
        max1 = _mm_max_ps(max1,r.src1[2].f);
        mMin.f = _mm_min_ps(mMin.f,min1);
        mMax.f = _mm_max_ps(mMax.f,max1);
    }
};

struct StatMinMax_64f: public StatMinMaxGen<Fw64f,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw64f)) * 3; // Load three registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        XMM128 sign_min,sign_max;
        sign_min.d = _mm_min_pd(r.src1[0].d,r.src1[1].d);
        sign_min.d = _mm_min_pd(sign_min.d,r.src1[2].d);
        mMin.d = _mm_min_pd(mMin.d,sign_min.d);
        sign_max.d = _mm_max_pd(r.src1[0].d,r.src1[1].d);
        sign_max.d = _mm_max_pd(sign_max.d,r.src1[2].d);
        mMax.d = _mm_max_pd(mMax.d,sign_max.d);
    }
};

struct StatMaxAbs_32s: public fe1St<Fw32s,C1>
{
    mutable Fw32s max;
    mutable XMM128 mMax;
    FEX_SSE2_REF
    StatMaxAbs_32s(){
        max = (Fw32s)0;
    }
    IV REFR_Init() {
        max = (Fw32s)0;
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = MAX<Fw32s>(max,feData.max);
    }
    IV SSE2_Init()
    {
        max = (Fw32s)0;
        mMax.i = _mm_setzero_si128();
    }
    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        max = MAX<Fw32s>(max,feData.max);

        Fw32s *p_mMax = (Fw32s*)&feData.mMax;

        for (int i =0; i < (XMMREGSIZE/ sizeof(Fw32s)) ; i++)
        {
            max = MAX<Fw32s>(max,p_mMax[i]);
        }
    }

    IV REFR(const Fw32s *s)     const                    // REFR Pixel function
    {
        Fw32s complement, sign  ;		//Finding the absolute value
        sign = (Fw32s)*s >> 31;
        complement = *s ^ sign;  //one's complement of src
        complement = complement - sign;
        sign  = (Fw32u)complement >> 31;
        complement = complement - sign;
        max = MAX<Fw32s>(max,complement);
    }

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        // Find the absolute value
        __m128i  desti = mm_abs_epi32(r.src1[0].i);

        // Find the maximum
        __m128i inter_res, com_inter;

        inter_res = _mm_cmpgt_epi32 (desti,mMax.i);
        com_inter = _mm_andnot_si128(inter_res,mMax.i);
        inter_res = _mm_and_si128(inter_res,desti);
        mMax.i  = _mm_or_si128(inter_res,com_inter);
    }
};

struct StatMinAbs_16s: public fe1St<Fw16s,C1>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16s))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

    mutable Fw16s min;
    XMM128 zero;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatMinAbs_16s(){
        min = (Fw16s)CBL_LIBRARY::Limits<Fw16s>::MinValue();
     }

    IV REFR_Init() {
        min = (Fw16s)CBL_LIBRARY::Limits<Fw16s>::MinValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min = MAX<Fw16s>(min,feData.min);
    }
    IV SSE2_Init()
    {
        min = (Fw16s)CBL_LIBRARY::Limits<Fw16s>::MinValue();
        Fw16s *p_mMin = (Fw16s*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(Fw16s)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<Fw16s>::MinValue();
        }
        zero.i = _mm_setzero_si128();
    }
    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min = MAX<Fw16s>(min,feData.min);

        Fw16s *p_mMin = (Fw16s*)&feData.mMin;

        for (int i =0; i < (XMMREGSIZE/ sizeof(Fw16s)) ; i++)
        {
            min = MAX<Fw16s>(min,p_mMin[i]);
        }
    }

    IV REFR(const Fw16s *s)     const                    // REFR Pixel function
    {
        min = MAX<Fw16s>(min,MIN<Fw16s>(*s,0-*s));
    }

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_max_epi16(mMin.i,_mm_min_epi16(r.src1[0].i, _mm_sub_epi16(zero.i,r.src1[0].i)));
        mMin.i = _mm_max_epi16(mMin.i,_mm_min_epi16(r.src1[1].i, _mm_sub_epi16(zero.i,r.src1[1].i)));
    }
};

struct StatMinAbs_32s: public fe1St<Fw32s,C1>
{
    mutable Fw32s min;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatMinAbs_32s(){
        min = (Fw32s)CBL_LIBRARY::Limits<Fw32s>::MinValue();
    }
    IV REFR_Init() {
        min = (Fw32s)CBL_LIBRARY::Limits<Fw32s>::MinValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min = MAX<Fw32s>(min,feData.min);
    }
    IV SSE2_Init()
    {
        min = (Fw32s)CBL_LIBRARY::Limits<Fw32s>::MinValue();
        Fw32s *p_mMin = (Fw32s*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(Fw32s)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<Fw32s>::MinValue();
        }
    }
    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min = MAX<Fw32s>(min,feData.min);

        Fw32s *p_mMin = (Fw32s*)&feData.mMin;

        for (int i =0; i < (XMMREGSIZE/ sizeof(Fw32s)) ; i++)
        {
            min = MAX<Fw32s>(min,p_mMin[i]);
        }
    }

    IV REFR(const Fw32s *s)     const                    // REFR Pixel function
    {
        min = MAX<Fw32s>(min,MIN<Fw32s>(*s,0-*s));
    }

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i  comp = _mm_srai_epi32(r.src1[0].i,31);
        __m128i  comp1 = _mm_and_si128(comp,r.src1[0].i);

        comp = _mm_sub_epi32(comp1,r.src1[0].i);
        comp = _mm_add_epi32(comp,comp1);
        
        __m128i inter_res, com_inter;

        inter_res = _mm_cmpgt_epi32 (comp,mMin.i);
        com_inter = _mm_andnot_si128(inter_res,mMin.i);
        inter_res = _mm_and_si128(inter_res,comp);
        mMin.i  = _mm_or_si128(inter_res,com_inter);

    }
};

struct StatMaxIndx_64f: public fe1St<Fw64f,C1>
{
    mutable Fw64f max;
    mutable Fw32s index;
    mutable Fw32s maxindex;

    FE_REF

    StatMaxIndx_64f(){
        max = CBL_LIBRARY::Limits<Fw64f>::MinValue();
        index = 0;
        maxindex = 0;
    }
    IV REFR_Init() {
        max = CBL_LIBRARY::Limits<Fw64f>::MinValue();
        index = 0;
        maxindex = 0;
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = MAX<Fw64f>(max,feData.max);
        maxindex = feData.maxindex;
    }

    IV REFR(const Fw64f *s)     const                    // REFR Pixel function
    {
        if (max < *s)
        {
            max = *s;
            maxindex = index;
        }
        index++;
    }
};


struct StatMaxAbsIndx_32s: public fe1St<Fw32s,C1>
{
    mutable Fw32s max;
    mutable Fw32s index;
    mutable Fw32s maxindex;

    FE_REF

    StatMaxAbsIndx_32s(){
        max = 0;
        index = 0;
        maxindex = 0;
    }
    IV REFR_Init() {
        max = 0;
        index = 0;
        maxindex = 0;
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = feData.max;
        maxindex = feData.maxindex;
    }

    IV REFR(const Fw32s *s)     const                    // REFR Pixel function
    {
        Fw32s twoscomp = *s;
        if (twoscomp > 0)
        {
            twoscomp = ~twoscomp;
            twoscomp +=1;
        }
        if (twoscomp < max)
        {
            max = twoscomp;
            maxindex = index;
        }
        index++;
    }
};



} // namespace OPT_LEVEL

#endif // __STATISTICS_MINMAX_H__

