/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATICOUNTINRANGE_H__
#define __STATICOUNTINRANGE_H__

#include "fex.h"

namespace OPT_LEVEL
{

struct StatICountInRange_8u: public fe1St<Fw8u,C1>
{
	mutable  Fw32s count;
	mutable XMM128 mCount;

    Fw8u lb, ub;
    XMM128 mlb,mub,one;


    StatICountInRange_8u()
    {
    }
	StatICountInRange_8u(Fw8u lowerb,Fw8u uperb)
        {
            count = 0;
            lb = lowerb;
            ub = uperb;
        }

	FEX_SSE2_REF
	IV REFR_Init() { count = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count += feData.count;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		count += feData.count;

        count += feData.mCount.s32[0] + feData.mCount.s32[1]+ feData.mCount.s32[2]+feData.mCount.s32[3];

    }

	IV SSE2_Init()
	{
		count = 0;
        mlb.i = _mm_set1_epi8(lb);
        mub.i = _mm_set1_epi8(ub);
        one.i =  _mm_set1_epi8(1);
		mCount.i = _mm_setzero_si128();
	}

	IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
        if(*s1 >= lb && *s1 <= ub)
            count++;        
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 lbmask,ubmask;

       lbmask.i  =  _mm_max_epu8(mlb.i,r.src1[0].i);
       lbmask.i  =  _mm_xor_si128(lbmask.i,r.src1[0].i);
       lbmask.i  = _mm_cmpeq_epi8(lbmask.i,_mm_setzero_si128() );
       lbmask.i = _mm_and_si128(lbmask.i,one.i);

       ubmask.i  =  _mm_min_epu8(mub.i,r.src1[0].i);
       ubmask.i  =  _mm_xor_si128(ubmask.i,r.src1[0].i);
       ubmask.i  = _mm_cmpeq_epi8(ubmask.i,_mm_setzero_si128() );
       ubmask.i = _mm_and_si128(ubmask.i,one.i);


       lbmask.i = _mm_and_si128(lbmask.i,ubmask.i);

       ubmask. i = _mm_srli_si128( lbmask.i, 8);
       lbmask.i = _mm_add_epi8(lbmask.i, ubmask. i );
       lbmask.i =  _mm_unpacklo_epi8(lbmask.i, _mm_setzero_si128()); 
       ubmask. i = _mm_srli_si128( lbmask.i, 8);
       lbmask.i = _mm_add_epi16(lbmask.i, ubmask. i );
       lbmask.i =  _mm_unpacklo_epi16(lbmask.i, _mm_setzero_si128()); 
        mCount.i = _mm_add_epi32( mCount.i,lbmask.i);

	}
};


struct StatICountInRange_32f: public fe1St<Fw32f,C1>
{

	

	mutable  Fw32s count;
	mutable XMM128 mCount;

    Fw32f lb, ub;
    XMM128 mlb,mub,one;

    StatICountInRange_32f()
    {
    }

    StatICountInRange_32f(StatICountInRange_32f &data)
    {
            lb = data.lb;
            ub = data.ub;
    }


	StatICountInRange_32f(Fw32f lowerb,Fw32f uperb)
        {
            count = 0;
            lb = lowerb;
            ub = uperb;
        }

    FEX_SSE2_REF
	IV REFR_Init() { count = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count += feData.count;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		count += feData.count;

        count += feData.mCount.s32[0] + feData.mCount.s32[1]+ feData.mCount.s32[2]+feData.mCount.s32[3];

    }

 	IV SSE2_Init()
	{

		count = 0;
        mlb.f = _mm_set1_ps(lb);
        mub.f = _mm_set1_ps(ub);
        one.i =  _mm_set1_epi32(1);
		mCount.i = _mm_setzero_si128();
	}

	IV REFR(const Fw32f *s1 ) const           // REFR Pixel function
	{
        if(*s1 >= lb && *s1 <= ub)
            count++;        
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 lbmask,ubmask;

       lbmask.f  = _mm_cmpge_ps(r.src1[0].f,mlb.f);
       lbmask.i = _mm_and_si128(lbmask.i,one.i);

       ubmask.f  = _mm_cmple_ps(r.src1[0].f,mub.f);
       ubmask.i = _mm_and_si128(ubmask.i,one.i);

       lbmask.i = _mm_and_si128(lbmask.i,ubmask.i);
        mCount.i = _mm_add_epi32( mCount.i,lbmask.i);

	}
};


struct StatICountInRange_8u_C3: public fe1St<Fw8u,C3>
{
	mutable  Fw32s count[3];
    Fw8u lb[3], ub[3];

    StatICountInRange_8u_C3()
    {
    }
	StatICountInRange_8u_C3(Fw8u lowerb[3],Fw8u uperb[3])
        {
            for(int i=0; i<3; i++)
            {
                count[i] = 0;
                lb[i] = lowerb[i];
                ub[i] = uperb[i];
            }
        }

	FEX_REF
	IV REFR_Init() 
    {
  
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count[0] += feData.count[0];
		count[1] += feData.count[1];
		count[2] += feData.count[2];
	}

    IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
        if(s1[0] >= lb[0] && s1[0] <= ub[0])
            count[0]++;   

        if(s1[1] >= lb[1] && s1[1] <= ub[1])
            count[1]++;    

        if(s1[2] >= lb[2] && s1[2] <= ub[2])
            count[2]++;      

	}

};

struct StatICountInRange_32f_C3: public fe1St<Fw32f,C3>
{

	

	mutable  Fw32s count[3];
    Fw32f lb[3], ub[3];

    StatICountInRange_32f_C3()
    {
    }
	StatICountInRange_32f_C3(Fw32f lowerb[3],Fw32f uperb[3])
        {
            for(int i=0; i<3; i++)
            {
                count[i] = 0;
                lb[i] = lowerb[i];
                ub[i] = uperb[i];
            }
        }

    FEX_REF
	IV REFR_Init() { }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count[0] += feData.count[0];
		count[1] += feData.count[1];
		count[2] += feData.count[2];
	}

	IV REFR(const Fw32f *s1 ) const           // REFR Pixel function
	{
        if(s1[0] >= lb[0] && s1[0] <= ub[0])
            count[0]++;   

        if(s1[1] >= lb[1] && s1[1] <= ub[1])
            count[1]++;    

        if(s1[2] >= lb[2] && s1[2] <= ub[2])
            count[2]++;         
	}

};




struct StatICountInRange_8u_AC4: public fe1St<Fw8u,AC4>
{
	mutable  Fw32s count[3];
    Fw8u lb[3], ub[3];

    StatICountInRange_8u_AC4()
    {
    }
	StatICountInRange_8u_AC4(Fw8u lowerb[3],Fw8u uperb[3])
        {
            for(int i=0; i<3; i++)
            {
                count[i] = 0;
                lb[i] = lowerb[i];
                ub[i] = uperb[i];
            }
        }

	FEX_REF
	IV REFR_Init() 
    {
    }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count[0] += feData.count[0];
		count[1] += feData.count[1];
		count[2] += feData.count[2];
	}

    IV REFR(const Fw8u *s1 ) const           // REFR Pixel function
	{
        if(s1[0] >= lb[0] && s1[0] <= ub[0])
            count[0]++;   

        if(s1[1] >= lb[1] && s1[1] <= ub[1])
            count[1]++;    

        if(s1[2] >= lb[2] && s1[2] <= ub[2])
            count[2]++;     

	}

};

struct StatICountInRange_32f_AC4: public fe1St<Fw32f,AC4>
{

	

	mutable  Fw32s count[3];
    Fw32f lb[3], ub[3];

    StatICountInRange_32f_AC4()
    {
    }
	StatICountInRange_32f_AC4(Fw32f lowerb[3],Fw32f uperb[3])
        {
            for(int i=0; i<3; i++)
            {
                count[i] = 0;
                lb[i] = lowerb[i];
                ub[i] = uperb[i];
            }
        }

    FEX_REF
	IV REFR_Init() { }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		count[0] += feData.count[0];
		count[1] += feData.count[1];
		count[2] += feData.count[2];
	}

	IV REFR(const Fw32f *s1 ) const           // REFR Pixel function
	{
        if(s1[0] >= lb[0] && s1[0] <= ub[0])
            count[0]++;   

        if(s1[1] >= lb[1] && s1[1] <= ub[1])
            count[1]++;    

        if(s1[2] >= lb[2] && s1[2] <= ub[2])
            count[2]++;      
	}

};



//
}

#endif // __STATICOUNTINRANGE_H__
