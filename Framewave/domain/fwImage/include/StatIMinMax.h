/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
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
    class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

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



template<typename TS1,CH cs1>
struct StatIMinAC4: public fe1St<TS1,cs1>
{
    mutable TS1 min[3];
    mutable XMM128 mMin;
    
    StatIMinAC4(){
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    IV REFR_Init() {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }

    IV SSE2_Init()
    {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();

        TS1 *p_mMin = (TS1*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);

        TS1 *p_mMin = (TS1*)&feData.mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i+=4)
        {
            min[0]= MIN<TS1>(min[0],p_mMin[i]);
            min[1]= MIN<TS1>(min[1],p_mMin[i+1]);
            min[2]= MIN<TS1>(min[2],p_mMin[i+2]);
        }

    }
};

struct StatIMin_8u_AC4R: public StatIMinAC4<Fw8u,AC4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epu8 (mMin.i,r.src1[0].i);
    }
};



struct StatIMin_16s_AC4R: public StatIMinAC4<Fw16s,AC4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epi16(mMin.i,r.src1[0].i);
    }

};

struct StatIMin_32f_AC4R: public StatIMinAC4<Fw32f,AC4>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
    }
};





template<typename TS1,CH cs1>
struct StatIMinC4: public fe1St<TS1,cs1>
{
    mutable TS1 min[4];
    mutable XMM128 mMin;
    
    StatIMinC4(){
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    IV REFR_Init() {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }

    IV SSE2_Init()
    {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();

        TS1 *p_mMin = (TS1*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
        min[3] = MIN<TS1>(min[3],feData.min[3]);
    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);
        min[3]= MIN<TS1>(min[3],s[3]);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
        min[3] = MIN<TS1>(min[3],feData.min[3]);

        TS1 *p_mMin = (TS1*)&feData.mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i+=4)
        {
            min[0]= MIN<TS1>(min[0],p_mMin[i]);
            min[1]= MIN<TS1>(min[1],p_mMin[i+1]);
            min[2]= MIN<TS1>(min[2],p_mMin[i+2]);
            min[3]= MIN<TS1>(min[3],p_mMin[i+3]);
        }

    }
};

struct StatIMin_8u_C4R: public StatIMinC4<Fw8u,C4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epu8 (mMin.i,r.src1[0].i);
    }
};



struct StatIMin_16s_C4R: public StatIMinC4<Fw16s,C4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epi16(mMin.i,r.src1[0].i);
    }

};

struct StatIMin_32f_C4R: public StatIMinC4<Fw32f,C4>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
    }
};


//////////////////////////C3 start

template<typename TS1,CH cs1>
struct StatIMinC3: public fe1St_96<TS1,cs1>
{
    mutable TS1 min[3];
    mutable XMM128 mMin;
    
    StatIMinC3(){
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();

    }
    IV REFR_Init() {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();

    }

    IV SSE2_Init()
    {
        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();


        TS1 *p_mMin = (TS1*)&mMin;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);

    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);

    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);


        TS1 *p_mMin = (TS1*)&feData.mMin;
        for (int i =0; i < ((XMMREGSIZE-4)/ sizeof(TS1)) ; i+= 3)
        {
            min[0]= MIN<TS1>(min[0],p_mMin[i]);
            min[1]= MIN<TS1>(min[1],p_mMin[i+1]);
            min[2]= MIN<TS1>(min[2],p_mMin[i+2]);
        }

    }
};


struct StatIMin_8u_C3R: public StatIMinC3<Fw8u,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epu8 (mMin.i,r.src1[0].i);
    }
};



struct StatIMin_16s_C3R: public StatIMinC3<Fw16s,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epi16(mMin.i,r.src1[0].i);
    }

};

struct StatIMin_32f_C3R: public StatIMinC3<Fw32f,C3>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
    }
};


///////////////////////////////// C3 end



//////////Max


template<typename TS1,CH cs1>
struct StatIMaxGen: public fe1St<TS1,cs1>
{
    mutable TS1 max;
    mutable XMM128 mMax;
    
    StatIMaxGen(){
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
    }
    IV REFR_Init() {
        max = CBL_LIBRARY::Limits<TS1>::MinValue();
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

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max = MAX<TS1>(max,feData.max);
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
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            max = MAX<TS1>(max,p_mMax[i]);
        }
    }
};



struct StatIMax_8u_C1R: public StatIMaxGen<Fw8u,C1>
{

    const static U32 nPIX_SSE = 16 * 2; // Load two registers
    class SRC1: public RegDesc< Fw8u, C1, nPIX_SSE > {};

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i sign1 = _mm_max_epu8 (r.src1[0].i,r.src1[1].i);
        mMax.i = _mm_max_epu8 (mMax.i,sign1);
    }
};


struct StatIMax_16s_C1R: public StatIMaxGen<Fw16s,C1>
{

    const static U32 nPIX_SSE = 8 * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        __m128i max1 = _mm_max_epi16(r.src1[0].i,r.src1[1].i);
        mMax.i = _mm_max_epi16(mMax.i,max1);
    }

};


struct StatIMax_32f_C1R: public StatIMaxGen<Fw32f,C1>
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



template<typename TS1,CH cs1>
struct StatIMaxAC4: public fe1St<TS1,cs1>
{
    mutable TS1 max[3];
    mutable XMM128 mMax;
    
    StatIMaxAC4(){
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
    }
    IV REFR_Init() {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        TS1 *p_mMax = (TS1*)&mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);
    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);

        TS1 *p_mMax = (TS1*)&feData.mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i+=4)
        {
            max[0]= MAX<TS1>(max[0],p_mMax[i]);
            max[1]= MAX<TS1>(max[1],p_mMax[i+1]);
            max[2]= MAX<TS1>(max[2],p_mMax[i+2]);
        }

    }
};

struct StatIMax_8u_AC4R: public StatIMaxAC4<Fw8u,AC4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epu8 (mMax.i,r.src1[0].i);
    }
};



struct StatIMax_16s_AC4R: public StatIMaxAC4<Fw16s,AC4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epi16(mMax.i,r.src1[0].i);
    }

};

struct StatIMax_32f_AC4R: public StatIMaxAC4<Fw32f,AC4>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
    }
};





template<typename TS1,CH cs1>
struct StatIMaxC4: public fe1St<TS1,cs1>
{
    mutable TS1 max[4];
    mutable XMM128 mMax;
    
    StatIMaxC4(){
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();
    }
    IV REFR_Init() {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();
    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();

        TS1 *p_mMax = (TS1*)&mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);
        max[3] = MAX<TS1>(max[3],feData.max[3]);
    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);
        max[3]= MAX<TS1>(max[3],s[3]);
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);
        max[3] = MAX<TS1>(max[3],feData.max[3]);

        TS1 *p_mMax = (TS1*)&feData.mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i+=4)
        {
            max[0]= MAX<TS1>(max[0],p_mMax[i]);
            max[1]= MAX<TS1>(max[1],p_mMax[i+1]);
            max[2]= MAX<TS1>(max[2],p_mMax[i+2]);
            max[3]= MAX<TS1>(max[3],p_mMax[i+3]);
        }

    }
};

struct StatIMax_8u_C4R: public StatIMaxC4<Fw8u,C4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epu8 (mMax.i,r.src1[0].i);
    }
};



struct StatIMax_16s_C4R: public StatIMaxC4<Fw16s,C4>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epi16(mMax.i,r.src1[0].i);
    }

};

struct StatIMax_32f_C4R: public StatIMaxC4<Fw32f,C4>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
    }
};


//////////////////////////C3 start

template<typename TS1,CH cs1>
struct StatIMaxC3: public fe1St_96<TS1,cs1>
{
    mutable TS1 max[3];
    mutable XMM128 mMax;
    
    StatIMaxC3(){
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

    }
    IV REFR_Init() {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();


        TS1 *p_mMax = (TS1*)&mMax;
        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);

    }

    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);

    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);


        TS1 *p_mMax = (TS1*)&feData.mMax;
        for (int i =0; i < ((XMMREGSIZE-4)/ sizeof(TS1)) ; i+= 3)
        {
            max[0]= MAX<TS1>(max[0],p_mMax[i]);
            max[1]= MAX<TS1>(max[1],p_mMax[i+1]);
            max[2]= MAX<TS1>(max[2],p_mMax[i+2]);
        }

    }
};


struct StatIMax_8u_C3R: public StatIMaxC3<Fw8u,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epu8 (mMax.i,r.src1[0].i);
    }
};



struct StatIMax_16s_C3R: public StatIMaxC3<Fw16s,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epi16(mMax.i,r.src1[0].i);
    }

};

struct StatIMax_32f_C3R: public StatIMaxC3<Fw32f,C3>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
    }
};


///////////////////////////////// C3 end

/////////// Max end


/// Min max

template<typename TS1,CH cs1>
struct StatIMinMaxGen: public fe1St<TS1,cs1>
{
    mutable TS1 max;
    mutable TS1 min;
    mutable XMM128 mMax;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatIMinMaxGen()
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

struct StatIMinMax_8u: public StatIMinMaxGen<Fw8u,C1>
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


struct StatIMinMax_16s: public StatIMinMaxGen<Fw16s,C1>
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


struct StatIMinMax_32f: public StatIMinMaxGen<Fw32f,C1>
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


/// AC4
template<typename TS1,CH cs1>
struct StatIMinMaxGenAC4: public fe1St<TS1,cs1>
{
    mutable TS1 max[3];
    mutable TS1 min[3];
    mutable XMM128 mMax;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatIMinMaxGenAC4()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();

    }
    IV REFR_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);

        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
    }
    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);

        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);
    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();


        TS1 *p_mMin = (TS1*)&mMin;
        TS1 *p_mMax = (TS1*)&mMax;

        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {

        min[0]= MIN<TS1>(min[0],feData.min[0]);
        min[1]= MIN<TS1>(min[1],feData.min[1]);
        min[2]= MIN<TS1>(min[2],feData.min[2]);

        max[0]= MAX<TS1>(max[0],feData.max[0]);
        max[1]= MAX<TS1>(max[1],feData.max[1]);
        max[2]= MAX<TS1>(max[2],feData.max[2]);


        TS1 *p_mMin = (TS1*)&feData.mMin;
        TS1 *p_mMax = (TS1*)&feData.mMax;

        for (int i =0; i < (XMMREGSIZE / sizeof(TS1)) ; i+=4)
        {
            min[0] = MIN<TS1>(min[0],p_mMin[i]);
            min[1] = MIN<TS1>(min[1],p_mMin[i+1]);
            min[2] = MIN<TS1>(min[2],p_mMin[i+2]);

            max[0] = MAX<TS1>(max[0],p_mMax[i]);
            max[1] = MAX<TS1>(max[1],p_mMax[i+1]);
            max[2] = MAX<TS1>(max[2],p_mMax[i+2]);
        }
    }


};

struct StatIMinMax_8u_AC4R: public StatIMinMaxGenAC4<Fw8u,AC4>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u) ) * 3; // Load three registers
    class SRC1: public RegDesc< Fw8u, AC4, nPIX_SSE > {};

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


struct StatIMinMax_16s_AC4: public StatIMinMaxGenAC4<Fw16s,AC4>
{

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {

        mMin.i = _mm_min_epi16(r.src1[0].i,mMin.i);
        mMax.i = _mm_max_epi16(r.src1[0].i,mMax.i);
    }
};


struct StatIMinMax_32f_AC4: public StatIMinMaxGenAC4<Fw32f,AC4>
{
 
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {

        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
    }
};

//ac4


/// C4
template<typename TS1,CH cs1>
struct StatIMinMaxGenC4: public fe1St<TS1,cs1>
{
    mutable TS1 max[4];
    mutable TS1 min[4];
    mutable XMM128 mMax;
    mutable XMM128 mMin;
    FEX_SSE2_REF
    StatIMinMaxGenC4()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();

    }
    IV REFR_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);
        max[3] = MAX<TS1>(max[3],feData.max[3]);

        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
        min[3] = MIN<TS1>(min[3],feData.min[3]);
    }
    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);
        min[3]= MIN<TS1>(min[3],s[3]);

        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);
        max[3]= MAX<TS1>(max[3],s[3]);
    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[3] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[3] = CBL_LIBRARY::Limits<TS1>::MaxValue();


        TS1 *p_mMin = (TS1*)&mMin;
        TS1 *p_mMax = (TS1*)&mMax;

        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {

        min[0]= MIN<TS1>(min[0],feData.min[0]);
        min[1]= MIN<TS1>(min[1],feData.min[1]);
        min[2]= MIN<TS1>(min[2],feData.min[2]);
        min[3]= MIN<TS1>(min[3],feData.min[3]);

        max[0]= MAX<TS1>(max[0],feData.max[0]);
        max[1]= MAX<TS1>(max[1],feData.max[1]);
        max[2]= MAX<TS1>(max[2],feData.max[2]);
        max[3]= MAX<TS1>(max[3],feData.max[3]);


        TS1 *p_mMin = (TS1*)&feData.mMin;
        TS1 *p_mMax = (TS1*)&feData.mMax;

        for (int i =0; i < (XMMREGSIZE / sizeof(TS1)) ; i+=4)
        {
            min[0] = MIN<TS1>(min[0],p_mMin[i]);
            min[1] = MIN<TS1>(min[1],p_mMin[i+1]);
            min[2] = MIN<TS1>(min[2],p_mMin[i+2]);
            min[3] = MIN<TS1>(min[3],p_mMin[i+3]);

            max[0] = MAX<TS1>(max[0],p_mMax[i]);
            max[1] = MAX<TS1>(max[1],p_mMax[i+1]);
            max[2] = MAX<TS1>(max[2],p_mMax[i+2]);
            max[3] = MAX<TS1>(max[3],p_mMax[i+3]);
        }
    }


};


struct StatIMinMax_8u_C4R: public StatIMinMaxGenC4<Fw8u,C4>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw8u) ) * 3; // Load three registers
    class SRC1: public RegDesc< Fw8u, C4, nPIX_SSE > {};

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


struct StatIMinMax_16s_C4: public StatIMinMaxGenC4<Fw16s,C4>
{

    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.i = _mm_min_epi16(r.src1[0].i,mMin.i);
        mMax.i = _mm_max_epi16(r.src1[0].i,mMax.i);
    }
};


struct StatIMinMax_32f_C4: public StatIMinMaxGenC4<Fw32f,C4>
{
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
    }
};


/// C4


/// C3
template<typename TS1,CH cs1>
struct StatIMinMaxC3: public fe1St_96<TS1,cs1>
{
    mutable TS1 max[3];
    mutable TS1 min[3];
    mutable XMM128 mMax;
    mutable XMM128 mMin;
    FEX_SSE2_REF

    StatIMinMaxC3()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();

    }
    IV REFR_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();
    }
    template<class FE>
    IV REFR_Post(FE &feData)
    {
        max[0] = MAX<TS1>(max[0],feData.max[0]);
        max[1] = MAX<TS1>(max[1],feData.max[1]);
        max[2] = MAX<TS1>(max[2],feData.max[2]);

        min[0] = MIN<TS1>(min[0],feData.min[0]);
        min[1] = MIN<TS1>(min[1],feData.min[1]);
        min[2] = MIN<TS1>(min[2],feData.min[2]);
    }
    IV REFR(const TS1 *s)     const                    // REFR Pixel function
    {
        min[0]= MIN<TS1>(min[0],s[0]);
        min[1]= MIN<TS1>(min[1],s[1]);
        min[2]= MIN<TS1>(min[2],s[2]);

        max[0]= MAX<TS1>(max[0],s[0]);
        max[1]= MAX<TS1>(max[1],s[1]);
        max[2]= MAX<TS1>(max[2],s[2]);
    }

    IV SSE2_Init()
    {
        max[0] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[1] = CBL_LIBRARY::Limits<TS1>::MinValue();
        max[2] = CBL_LIBRARY::Limits<TS1>::MinValue();

        min[0] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[1] = CBL_LIBRARY::Limits<TS1>::MaxValue();
        min[2] = CBL_LIBRARY::Limits<TS1>::MaxValue();


        TS1 *p_mMin = (TS1*)&mMin;
        TS1 *p_mMax = (TS1*)&mMax;

        for (int i =0; i < (XMMREGSIZE/ sizeof(TS1)) ; i++)
        {
            p_mMin[i] = CBL_LIBRARY::Limits<TS1>::MaxValue();
            p_mMax[i] = CBL_LIBRARY::Limits<TS1>::MinValue();
        }
    }

    template<class FE>
    IV SSE2_Post(FE &feData)
    {

        min[0]= MIN<TS1>(min[0],feData.min[0]);
        min[1]= MIN<TS1>(min[1],feData.min[1]);
        min[2]= MIN<TS1>(min[2],feData.min[2]);

        max[0]= MAX<TS1>(max[0],feData.max[0]);
        max[1]= MAX<TS1>(max[1],feData.max[1]);
        max[2]= MAX<TS1>(max[2],feData.max[2]);


        TS1 *p_mMin = (TS1*)&feData.mMin;
        TS1 *p_mMax = (TS1*)&feData.mMax;

        for (int i =0; i < ((XMMREGSIZE-4)/ sizeof(TS1)) ; i+=3)
        {
            min[0] = MIN<TS1>(min[0],p_mMin[i]);
            min[1] = MIN<TS1>(min[1],p_mMin[i+1]);
            min[2] = MIN<TS1>(min[2],p_mMin[i+2]);

            max[0] = MAX<TS1>(max[0],p_mMax[i]);
            max[1] = MAX<TS1>(max[1],p_mMax[i+1]);
            max[2] = MAX<TS1>(max[2],p_mMax[i+2]);
        }

    }

};

struct StatIMinMax_8u_C3R: public StatIMinMaxC3<Fw8u,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epu8 (mMax.i,r.src1[0].i);
        mMin.i = _mm_min_epu8 (mMin.i,r.src1[0].i);
    }
};



struct StatIMinMax_16s_C3R: public StatIMinMaxC3<Fw16s,C3>
{

    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.i = _mm_max_epi16(mMax.i,r.src1[0].i);
        mMin.i = _mm_min_epi16(mMin.i,r.src1[0].i);
    }

};

struct StatIMinMax_32f_C3R: public StatIMinMaxC3<Fw32f,C3>
{
    FEX_SSE2_REF
    IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
    {
        mMax.f = _mm_max_ps(mMax.f,r.src1[0].f);
        mMin.f = _mm_min_ps(mMin.f,r.src1[0].f);
    }
};


//// C3
/// end min max
}

#endif // __STATIMINMAX_H__
