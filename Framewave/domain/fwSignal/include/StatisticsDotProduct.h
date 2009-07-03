/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISTICS_DOTPRODUCT_H__
#define __STATISTICS_DOTPRODUCT_H__

#include "fex.h"
#include "SSE2Intrinsic.h"

namespace OPT_LEVEL
{

SYS_INLINE	STATIC void _mm_mul_16sc_32sc(const __m128i &src1,const __m128i &src2,XMM128 &des1, XMM128 &des2)
{
    XMM128 srcLo,srcHi,imReal,imImg,img;

    img.i = _mm_shufflelo_epi16(src2,_MM_SHUFFLE(2, 3, 0, 1));	// 33 11
    img.i = _mm_shufflehi_epi16(img.i,_MM_SHUFFLE(2, 3, 0, 1));           // 77 55
	//Multiplies the 8 signed 16-bit integers from a by the 8 signed 16-bit integers from b.
    img.i = _mm_madd_epi16(src1,img.i);

    _mm_mul_16s_32s(src1,src2,srcLo.i,srcHi.i); // Multiplies two 16 bit integers to 32 bit integers

    imReal.f = _mm_shuffle_ps(srcLo.f,srcHi.f,_MM_SHUFFLE(2, 0, 2, 0));
    imImg.f = _mm_shuffle_ps(srcLo.f,srcHi.f,_MM_SHUFFLE(3, 1, 3, 1));

    imReal.i = _mm_sub_epi32(imReal.i,imImg.i);
    des1.i = _mm_unpacklo_epi32(imReal.i,img.i);
    des2.i = _mm_unpackhi_epi32(imReal.i,img.i);
}

template<typename TS1,CH cs1,typename TS2,CH cs2,typename SM>
struct StatDotProdGen: public fe2St<TS1,cs1,TS2,cs2>
{
    mutable SM sum;
    mutable XMM128 mSum;
    FEX_SSE2_REF
    StatDotProdGen(){
        sum = 0;    // Initialize the sum to Zero - initial value
    }
    IV REFR_Init() {
        sum = 0;    // Initialize the sum to Zero - initial value. Will be called only for reference path
    }
    IV SSE2_Init()	// Initialize the sum and mSum to Zero  - initial value. Will be called for SSE2 path
    {
        sum = 0;
        mSum.d = _mm_setzero_pd();
    }
    template<class FE>
    IV REFR_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. Reference Path
    {
        sum += feData.sum;
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen64s: public fe2St<TS1,cs1,TS2,cs2>
{
    mutable Fw64s sum;
    mutable XMM128 mSum;
    FEX_SSE2_REF
    StatDotProdGen64s(){
        sum=0;    // Initialize the sum to Zero - initial value
    }
    IV REFR_Init() {
        sum = 0;    // Initialize the sum to Zero - initial value. Will be called only for reference path
    }
    IV SSE2_Init()	// Initialize the sum and mSum to Zero  - initial value. Will be called for SSE2 path
    {
        sum = 0;
        mSum.i = _mm_setzero_si128();
    }
    template<class FE>
    IV REFR_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. Reference Path
    {
        sum += feData.sum;
    }
    template<class FE>
    IV SSE2_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        sum += feData.sum + feData.mSum.s64[0] + feData.mSum.s64[1];
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen64sc: public fe2St<TS1,cs1,TS2,cs2>
{
    mutable Fw64sc sum;
    mutable XMM128 mSum;
    FEX_SSE2_REF
    StatDotProdGen64sc(){
        sum.re=0;    // Initialize the sum to Zero - initial value
        sum.im = 0;
    }
    IV REFR_Init() {
        sum.re = 0;    // Initialize the sum to Zero - initial value. Will be called only for reference path
        sum.im = 0;
    }
    IV SSE2_Init()	// Initialize the sum and mSum to Zero  - initial value. Will be called for SSE2 path
    {
        sum.re = 0;
        sum.im = 0;
        mSum.i = _mm_setzero_si128();
    }
    template<class FE>
    IV REFR_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. Reference Path
    {
        sum += feData.sum;
    }
    template<class FE>
    IV SSE2_Post(FE &feData) // Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        sum += feData.sum;
        sum.re +=  feData.mSum.s64[0];
        sum.im +=  feData.mSum.s64[1];
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen64f: public StatDotProdGen<TS1,cs1,TS2,cs2,Fw64f>
{
    template<class FE>
    IV SSE2_Post(FE &feData) // Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        StatDotProdGen<TS1,cs1,TS2,cs2,Fw64f>::sum += feData.sum + feData.mSum.f64[0] + feData.mSum.f64[1];
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen32f: public StatDotProdGen<TS1,cs1,TS2,cs2,Fw32f>
{
    template<class FE>
    IV SSE2_Post(FE &feData) // Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        StatDotProdGen<TS1,cs1,TS2,cs2,Fw32f>::sum += feData.sum + feData.mSum.f32[0] + feData.mSum.f32[1]+ \
		feData.mSum.f32[2] + feData.mSum.f32[3];
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen32fc: public fe2St<TS1,cs1,TS2,cs2>
{

    mutable Fw32fc sum;
    mutable XMM128 mSum;
    FEX_SSE2_REF
    StatDotProdGen32fc(){
        sum.re = 0;    // Initialize the sum to Zero - initial value
        sum.im = 0;
    }
    IV REFR_Init() {
        sum.re = 0;    // Initialize the sum to Zero - initial value. Will be called only for reference path
        sum.im = 0;
    }
    IV SSE2_Init()	 // Initialize the sum and mSum to Zero  - initial value. Will be called for SSE2 path
    {
        sum.re = 0;
        sum.im = 0;
        mSum.d = _mm_setzero_pd();
    }
    template<class FE>
    IV REFR_Post(FE &feData) // Accumulate the sum's of all the threads. Will be called for each thread. Reference Path
    {
        sum += feData.sum;
    }

    template<class FE>
    IV SSE2_Post(FE &feData) // Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        sum += feData.sum;
        sum.re += feData.mSum.f32[0] + feData.mSum.f32[2];
        sum.im += feData.mSum.f32[1] + feData.mSum.f32[3];
    }
};

template<typename TS1,CH cs1,typename TS2,CH cs2>
struct StatDotProdGen64fc: public fe2St<TS1,cs1,TS2,cs2>
{
    mutable Fw64fc sum;
    mutable XMM128 mSum;
    FEX_SSE2_REF
    StatDotProdGen64fc(){
        sum.re = 0.0;    // Initialize the sum to Zero - initial value
        sum.im = 0.0;
    }
    IV REFR_Init() {
        sum.re = 0.0;    // Initialize the sum to Zero - initial value. Will be called only for reference path
        sum.im = 0.0;
    }
    IV SSE2_Init()	 // Initialize the sum and mSum to Zero  - initial value. Will be called for SSE2 path
    {
        sum.re = 0;
        sum.im = 0;
        mSum.d = _mm_setzero_pd();
    }
    template<class FE>
    IV REFR_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. Reference Path
    {
        sum += feData.sum;
    }
    template<class FE>
    IV SSE2_Post(FE &feData)	// Accumulate the sum's of all the threads. Will be called for each thread. SSE2 path.
    {
        sum += feData.sum;
        sum.re +=  feData.mSum.f64[0];
        sum.im +=  feData.mSum.f64[1];
    }
};


struct StatDotProd_32f: public StatDotProdGen64f<Fw32f,C1,Fw32f,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {	// Multiply two float values and accumulate the result in double
        XMM128 src1Lo,src1Hi,src2Lo,src2Hi;
        _mm_cvt32f_64f(r.src1[0],src1Lo,src1Hi);	// Convert the float operand1 to double to avoid overflow
        _mm_cvt32f_64f(r.src2[0],src2Lo,src2Hi);	// Convert the float operand2 to double to avoid overflow
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);	// Multiply
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);	// Multiply
        src1Lo.d = _mm_add_pd(src1Lo.d,src1Hi.d);	// Add the results.
        mSum.d = _mm_add_pd(mSum.d,src1Lo.d);	// Accumulate the results.
    }
    IV REFR(const Fw32f *s1,const Fw32f *s2)     const    // REFR Pixel function
    {
        Fw64f sqr = ((Fw64f)*s1 * *s2);	// Multiply
        sum += sqr;	// Accumulate the results.
    }
};

struct StatDotProd_32fc: public StatDotProdGen32fc<Fw32fc,C1,Fw32fc,C1>
{

    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 temp;
        temp.f  = _mm_mul_ps32fc(r.src1[0].f,r.src2[0].f);
        mSum.f = _mm_add_ps(mSum.f,temp.f);
    }
    IV REFR(const Fw32fc *s1,const Fw32fc *s2)     const    // REFR Pixel function
    {
        Fw32f p1 = (s1->re * s2->re);	// 32fc complex multiplication.
        Fw32f p2 = (s1->im * s2->im);
        Fw32f p3 = (s1->re * s2->im);
        Fw32f p4 = (s1->im * s2->re);
        sum.re += p1 - p2;
        sum.im += p3 + p4;
    }
};

struct StatDotProd_32f32fc: public StatDotProdGen32fc<Fw32f,C1,Fw32fc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
		XMM128 src1;//,d1;//,d2;
		src1.f = _mm_unpacklo_ps(r.src1[0].f,r.src1[0].f);
		src1.f = _mm_mul_ps(src1.f,r.src2[0].f);
		mSum.f = _mm_add_ps(mSum.f,src1.f);
    }
    IV REFR(const Fw32f *s1,const Fw32fc *s2)     const    // REFR Pixel function
    {
        Fw32fc dest;
        dest.re = (Fw32f)*s1 * (Fw32f)s2->re;
        dest.im = (Fw32f)*s1 * (Fw32f)s2->im;
        sum += dest;
    }
};

struct StatDotProd_32f64f: public StatDotProdGen64f<Fw32f,C1,Fw32f,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1Lo,src1Hi,src2Lo,src2Hi;
        _mm_cvt32f_64f(r.src1[0],src1Lo,src1Hi);	// Convert to double and
        _mm_cvt32f_64f(r.src2[0],src2Lo,src2Hi);    //	do the multiplication
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);   //	in double to get better
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);   //	accuracy.
        src1Lo.d = _mm_add_pd(src1Lo.d,src1Hi.d);   //	accumulate the results in
        mSum.d = _mm_add_pd(mSum.d,src1Lo.d);	    // mSum
    }
    IV REFR(const Fw32f *s1,const Fw32f *s2)     const    // REFR Pixel function
    {
        Fw64f sqr = ((Fw64f)*s1 * *s2);	// Multiplication in double
        sum += sqr;
    }
};

struct StatDotProd_32fc64fc: public StatDotProdGen64fc<Fw32fc,C1,Fw32fc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1Lo,src1Hi,src2Lo,src2Hi;	// Convert to double and
        _mm_cvt32f_64f(r.src1[0],src1Lo,src1Hi);	//	do the multiplication
        _mm_cvt32f_64f(r.src2[0],src2Lo,src2Hi);	//	in double to get better
        src1Lo.d = _mm_mul_pd64fc(src1Lo.d,src2Lo.d);	//	accuracy.
        src1Hi.d = _mm_mul_pd64fc(src1Hi.d,src2Hi.d);	//	accumulate the results in
        src1Lo.d = _mm_add_pd(src1Lo.d,src1Hi.d);	// mSum
        mSum.d = _mm_add_pd(mSum.d,src1Lo.d);
    }
    IV REFR(const Fw32fc *s1,const Fw32fc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;	// Complex multiplication after converting float's to double's
        dest.re = ((Fw64f)s1->re * (Fw64f)s2->re) - ((Fw64f)s1->im * (Fw64f)s2->im);
        dest.im = ((Fw64f)s1->re * (Fw64f)s2->im) + ((Fw64f)s1->im * (Fw64f)s2->re);
        sum.re = sum.re + dest.re;
        sum.im = sum.im + dest.im;
    }
};

struct StatDotProd_32f32fc64fc: public StatDotProdGen64fc<Fw32f,C1,Fw32fc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1,src1Lo,src1Hi,src2Lo,src2Hi;	// Convert to double and
        src1.f = _mm_unpacklo_ps(r.src1[0].f,r.src1[0].f);	//	do the multiplication
        _mm_cvt32f_64f(src1,src1Lo,src1Hi);	//	in double to get better
        _mm_cvt32f_64f(r.src2[0],src2Lo,src2Hi);	//	accuracy.
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);	//	accumulate the results in
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);	// mSum
        src1Lo.d = _mm_add_pd(src1Lo.d,src1Hi.d);
        mSum.d = _mm_add_pd(mSum.d,src1Lo.d);
    }
    IV REFR(const Fw32f *s1,const Fw32fc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;	// Multiplication in double for better accuracy
        dest.re = (Fw64f)*s1 * s2->re;
        dest.im = (Fw64f)*s1 * s2->im;
        sum += dest;
    }
};

struct StatDotProd_64f: public StatDotProdGen64f<Fw64f,C1,Fw64f,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 dest;
        dest.d = _mm_mul_pd(r.src1[0].d,r.src2[0].d);	// Multiply src1 with src2 and
        mSum.d = _mm_add_pd(mSum.d,dest.d);	// accumulate the results.
    }
    IV REFR(const Fw64f *s1,const Fw64f *s2)     const    // REFR Pixel function
    {
        sum += (*s1 * *s2);
    }
};

struct StatDotProd_64fc: public StatDotProdGen64fc<Fw64fc,C1,Fw64fc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        // Perform the complex number multiplication and
        // accumulate the results in mSum
        mSum.d = _mm_add_pd(_mm_mul_pd64fc(r.src1[0].d,r.src2[0].d),mSum.d);
    }
    IV REFR(const Fw64fc *s1,const Fw64fc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest; // Complex number multiplication
        dest.re = (s1->re * s2->re) - (s1->im * s2->im);
        dest.im = (s1->re * s2->im) + (s1->im * s2->re);
        sum += dest;
    }
};

struct StatDotProd_64f64fc: public StatDotProdGen64fc<Fw64f,C1,Fw64fc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        __m128d src1,mulRes;
        src1 = _mm_unpacklo_pd(r.src1[0].d,r.src1[0].d);	// Multiplication of a
        mulRes = _mm_mul_pd(src1,r.src2[0].d);	// double type number with
        mSum.d = _mm_add_pd(mSum.d,mulRes);	// complex double type number
    }
    IV REFR(const Fw64f *s1,const Fw64fc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;	// Multiplication of a number with complex number
        dest.re = (*s1 * s2->re);
        dest.im = (*s1 * s2->im);
        sum += dest;
    }
};

struct StatDotProd_16s64s: public StatDotProdGen64s<Fw16s,C1,Fw16s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,imReshi,resHi;
        // Multiply two 16 bit numbers and store the results in 32 bit.
        _mm_mul_16s_32s(r.src1[0].i,r.src2[0].i,resLo.i,resHi.i);
        // Converting the number to 64bit
        CBL_SSE2::Unpack32STo64S(resLo.i,imReshi.i);
        // Accumulate  the results in 64bit number to avoid overflows
        resLo.i = _mm_add_epi64(resLo.i,imReshi.i);
        CBL_SSE2::Unpack32STo64S(resHi.i,imReshi.i);
        imReshi.i = _mm_add_epi64(imReshi.i,resHi.i);
        resLo.i = _mm_add_epi64(resLo.i,imReshi.i);
        mSum.i = _mm_add_epi64(mSum.i,resLo.i);
    }
    IV REFR(const Fw16s *s1,const Fw16s *s2)     const    // REFR Pixel function
    {
        // Multiply and accumulate the results in 64bit number.
        Fw32s temp = (Fw32s)*s1 * (Fw32s)*s2;
        sum += temp;
    }
};

struct StatDotProd_16sc64sc: public StatDotProdGen64fc<Fw16sc,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,imRes,resHi;
        // Multiply two 16 bit numbers and store the results in 32 bit.
        _mm_mul_16sc_32sc(r.src1[0].i,r.src2[0].i,resLo,resHi);
        imRes.i = _mm_add_epi32(resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(imRes.i,resLo.d,resHi.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        // accumulate the results in double type number to avoid overflows
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw16sc *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        // Multiply and accumulate the results in double type number to avoid overflows
        dest.re = ((Fw64f)s1->re * s2->re) - ((Fw64f)s1->im * s2->im);
        dest.im = ((Fw64f)s1->re * s2->im) + ((Fw64f)s1->im * s2->re);
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_16s16sc64sc: public StatDotProdGen64sc<Fw16s,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1,resLo,imReshi,resHi;
        // Multiply two 16bit numbers and accumulate the
        // results in 64bit to avoid overflow.
        src1.i = _mm_unpacklo_epi16(r.src1[0].i,r.src1[0].i);
        _mm_mul_16s_32s(src1.i,r.src2[0].i,resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64S(resLo.i,src1.i);
        CBL_SSE2::Unpack32STo64S(resHi.i,imReshi.i);
        src1.i = _mm_add_epi64(src1.i,imReshi.i);
        resLo.i = _mm_add_epi64(resLo.i,resHi.i);
        src1.i = _mm_add_epi64(src1.i,resLo.i);
        mSum.i = _mm_add_epi64(mSum.i,src1.i);
    }
    IV REFR(const Fw16s *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64sc dest;
        // Do integer multiplication for better performance
        // Accumulate the results in 64bit to avoid overflows
        dest.re = ((Fw32s)*s1 * s2->re);
        dest.im = ((Fw32s)*s1 * s2->im);
        sum += dest;
    }
};

struct StatDotProd_16s32f: public StatDotProdGen64f<Fw16s,C1,Fw16s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi,imResLo,imReshi;
        // Multiply two 16bit numbers and accumulate
        // the results in double to avoid overflows
        _mm_mul_16s_32s(r.src1[0].i,r.src2[0].i,resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(resLo.i,imResLo.d,imReshi.d);
        resLo.d = _mm_add_pd(imResLo.d,imReshi.d);
        CBL_SSE2::Unpack32STo64F(resHi.i,imResLo.d,imReshi.d);
        resHi.d = _mm_add_pd(imResLo.d,imReshi.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw16s *s1,const Fw16s *s2)     const    // REFR Pixel function
    {
        // Multiply in double and accumulate the results
        sum += ((Fw64f)*s1 * (Fw64f)*s2);
    }
};

struct StatDotProd_16sc32fc: public StatDotProdGen64fc<Fw16sc,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,imRes,resHi;
        // Multiply two 16bit numbers and accumulate
        // the results in double to avoid overflows
        _mm_mul_16sc_32sc(r.src1[0].i,r.src2[0].i,resLo,resHi);
        imRes.i = _mm_add_epi32(resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(imRes.i,resLo.d,resHi.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw16sc *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = (((Fw64f)s1->re * s2->re) - ((Fw64f)s1->im * s2->im));
        dest.im = (((Fw64f)s1->re * s2->im) + ((Fw64f)s1->im * s2->re));
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_16s16sc32fc: public StatDotProdGen64fc<Fw16s,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1,imResLo,resHi,imReshi;
        // Multiply two 16bit numbers and accumulate
        // the results in double to avoid overflows
        src1.i = _mm_unpacklo_epi16(r.src1[0].i,r.src1[0].i);
        _mm_mul_16s_32s(src1.i,r.src2[0].i,src1.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(src1.i,imResLo.d,imReshi.d);
        src1.d = _mm_add_pd(imResLo.d,imReshi.d);
        CBL_SSE2::Unpack32STo64F(resHi.i,imResLo.d,imReshi.d);
        resHi.d = _mm_add_pd(imResLo.d,imReshi.d);
        src1.d = _mm_add_pd(src1.d,resHi.d);
        mSum.d= _mm_add_pd(mSum.d,src1.d);
    }
    IV REFR(const Fw16s *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = ((Fw64f)*s1 * s2->re);
        dest.im = ((Fw64f)*s1 * s2->im);
        sum += dest;
    }
};

struct StatDotProd_16s_Sfs: public StatDotProdGen32f<Fw16s,C1,Fw16s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi;
        // Multiply two 16bit numbers and accumulate the results in
        // 32bit number.
        _mm_mul_16s_32s(r.src1[0].i,r.src2[0].i,resLo.i,resHi.i);
        resLo.f = _mm_cvtepi32_ps(resLo.i);
        resHi.f = _mm_cvtepi32_ps(resHi.i);
        resLo.f = _mm_add_ps(resLo.f,resHi.f);
        // Accumulate the results in float to avoid overflows
        mSum.f = _mm_add_ps(mSum.f,resLo.f);
    }

    IV REFR(const Fw16s *s1,const Fw16s *s2)     const    // REFR Pixel function
    {
        // Multiply and accumulate the results in float
        // to avoid overflows
        Fw32f dest = (Fw32f)*s1 * *s2;
        sum += dest;
    }
};

struct StatDotProd_16sc_Sfs: public StatDotProdGen32fc<Fw16sc,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi;
        // Multiply two 16bit numbers and accumulate the results in
        // 32bit number.
        _mm_mul_16sc_32sc(r.src1[0].i,r.src2[0].i,resLo,resHi);
        resLo.i = _mm_add_epi32(resLo.i,resHi.i);
        resLo.f = _mm_cvtepi32_ps(resLo.i);
        // Accumulate the results in float to avoid overflows
        mSum.f = _mm_add_ps(mSum.f,resLo.f);
    }
    IV REFR(const Fw16sc *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw32fc dest;
        // Multiply and accumulate the results in float
        // to avoid overflows
        dest.re = ((Fw32f)s1->re * s2->re) - ((Fw32f)s1->im * s2->im);
        dest.im = ((Fw32f)s1->re * s2->im) + ((Fw32f)s1->im * s2->re);
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_32s_Sfs: public StatDotProdGen64f<Fw32s,C1,Fw32s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1Lo,src1Hi,src2Lo,src2Hi;
        CBL_SSE2::Unpack32STo64F(r.src1[0].i,src1Lo.d,src1Hi.d); // Multiply and accumulate
        CBL_SSE2::Unpack32STo64F(r.src2[0].i,src2Lo.d,src2Hi.d); //	the results in
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);	 //	Double type to avoid
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);	 // overflows
        src1Lo.d = _mm_add_pd(src1Lo.d,src1Hi.d);
        mSum.d = _mm_add_pd(mSum.d,src1Lo.d);
    }
    IV REFR(const Fw32s *s1,const Fw32s *s2)     const    // REFR Pixel function
    {
        Fw64f dest;
        // Multiply and accumulate the results in double
        dest = (Fw64f)*s1 * *s2;;
        sum += dest;
    }
};

struct StatDotProd_32sc_Sfs: public StatDotProdGen64fc<Fw32sc,C1,Fw32sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi;
        // Multiply two 32bit complex numbers and
        // accumulate the results in double type to avoid overflows
        CBL_SSE2::Multiply_32sc64fc(r.src1[0].i,r.src2[0].i,resLo.d,resHi.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw32sc *s1,const Fw32sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = ((Fw64f)s1->re * s2->re) - ((Fw64f)s1->im * s2->im);
        dest.im = ((Fw64f)s1->re * s2->im) + ((Fw64f)s1->im * s2->re);
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_16s32s_Sfs: public StatDotProdGen64s<Fw16s,C1,Fw16s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi,imReshi;
        // Multiply and accumulate the results in 64bit integer type
        // to avoid overflows.
        _mm_mul_16s_32s(r.src1[0].i,r.src2[0].i,resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64S(resLo.i,imReshi.i);
        resLo.i = _mm_add_epi64(resLo.i,imReshi.i);
        CBL_SSE2::Unpack32STo64S(resHi.i,imReshi.i);
        resHi.i = _mm_add_epi64(resHi.i,imReshi.i);
        resLo.i = _mm_add_epi64(resLo.i,resHi.i);
        mSum.i = _mm_add_epi64(mSum.i,resLo.i);
    }
    IV REFR(const Fw16s *s1,const Fw16s *s2)     const    // REFR Pixel function
    {
        Fw32s dest;
        // Multiply and accumulate the results in 64bit integer type
        // to avoid overflows.
        dest = (Fw32s)*s1 * *s2;
        sum += dest;
    }
};

struct StatDotProd_16s16sc32sc_Sfs: public StatDotProdGen64sc<Fw16s,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi,imReshi,src1;
        // Multiply and accumulate the results in 64bit integer type
        // to avoid overflows.
        src1.i = _mm_unpacklo_epi16(r.src1[0].i,r.src1[0].i);
        _mm_mul_16s_32s(src1.i,r.src2[0].i,resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64S(resLo.i,imReshi.i);
        resLo.i = _mm_add_epi64(resLo.i,imReshi.i);
        CBL_SSE2::Unpack32STo64S(resHi.i,imReshi.i);
        resHi.i = _mm_add_epi64(resHi.i,imReshi.i);
        resLo.i = _mm_add_epi64(resLo.i,resHi.i);
        mSum.i = _mm_add_epi64(mSum.i,resLo.i);
    }
    IV REFR(const Fw16s *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64sc dest;
        // Multiply and accumulate the results in 64bit integer type
        // to avoid overflows.
        dest.re = ((Fw64s)*s1 * s2->re);
        dest.im = ((Fw64s)*s1 * s2->im);
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_16s32s32s_Sfs: public StatDotProdGen64f<Fw16s,C1,Fw32s,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1,src2Lo,src1Lo,src1Hi,src2Hi;
        // Multiply and accumulate the results in double type
        // to avoid overflows.
        __m128i signext = _mm_srai_epi16(r.src1[0].i, 15);
        src1.i = _mm_unpacklo_epi16( r.src1[0].i, signext );
        CBL_SSE2::Unpack32STo64F(src1.i,src1Lo.d,src1Hi.d);
        CBL_SSE2::Unpack32STo64F(r.src2[0].i,src2Lo.d,src2Hi.d);
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);
        src1.d = _mm_add_pd(src1Lo.d,src1Hi.d);
        mSum.d = _mm_add_pd(mSum.d,src1.d);
    }
    IV REFR(const Fw16s *s1,const Fw32s *s2)     const    // REFR Pixel function
    {
        Fw64f dest;
        dest = (Fw64f)*s1 * *s2;
        sum += dest;
    }
};

struct StatDotProd_16s16sc_Sfs: public StatDotProdGen64fc<Fw16s,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,resHi,imResLo,src1;
        // Multiply and accumulate the results in double type
        // to avoid overflows.
        src1.i = _mm_unpacklo_epi16(r.src1[0].i,r.src1[0].i);
        _mm_mul_16s_32s(src1.i,r.src2[0].i,resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(resLo.i,imResLo.d,src1.d);
        resLo.d = _mm_add_pd(imResLo.d,src1.d);
        CBL_SSE2::Unpack32STo64F(resHi.i,imResLo.d,src1.d);
        resHi.d = _mm_add_pd(imResLo.d,src1.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw16s *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = ((Fw64f)*s1 * s2->re);
        dest.im = ((Fw64f)*s1 * s2->im);
        sum += dest;
    }
};

struct StatDotProd_16sc32sc_Sfs: public StatDotProdGen64fc<Fw16sc,C1,Fw16sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 resLo,imRes,resHi;
        // Multiply and accumulate the results in double type
        // to avoid overflows.
        _mm_mul_16sc_32sc(r.src1[0].i,r.src2[0].i,resLo,resHi);
        imRes.i = _mm_add_epi32(resLo.i,resHi.i);
        CBL_SSE2::Unpack32STo64F(imRes.i,resLo.d,resHi.d);
        resLo.d = _mm_add_pd(resLo.d,resHi.d);
        mSum.d = _mm_add_pd(mSum.d,resLo.d);
    }
    IV REFR(const Fw16sc *s1,const Fw16sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = ((Fw64f)s1->re * s2->re) - ((Fw64f)s1->im * s2->im);
        dest.im = ((Fw64f)s1->re * s2->im) + ((Fw64f)s1->im * s2->re);
        sum.re += dest.re;
        sum.im += dest.im;
    }
};

struct StatDotProd_32s32sc_Sfs: public StatDotProdGen64fc<Fw32s,C1,Fw32sc,C1>
{
    IV SSE2( RegFile & r )  const       // SSE2 Pixel function
    {
        XMM128 src1,src1Lo,src1Hi,src2Lo,src2Hi;
        // Multiply and accumulate the results in double type
        // to avoid overflows.
        src1.i = _mm_unpacklo_epi32(r.src1[0].i,r.src1[0].i);
        CBL_SSE2::Unpack32STo64F(src1.i,src1Lo.d,src1Hi.d);
        CBL_SSE2::Unpack32STo64F(r.src2[0].i,src2Lo.d,src2Hi.d);
        src1Lo.d = _mm_mul_pd(src1Lo.d,src2Lo.d);
        src1Hi.d = _mm_mul_pd(src1Hi.d,src2Hi.d);
        src1.d = _mm_add_pd(src1Lo.d,src1Hi.d);
        mSum.d = _mm_add_pd(mSum.d,src1.d);
    }
    IV REFR(const Fw32s *s1,const Fw32sc *s2)     const    // REFR Pixel function
    {
        Fw64fc dest;
        dest.re = ((Fw64f)*s1 * s2->re);
        dest.im = ((Fw64f)*s1 * s2->im);
        sum += dest;
    }
};
} // namespace OPT_LEVEL

#endif // __STATISTICS_DOTPRODUCT_H__


