/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISTICSS_H__
#define __STATISTICSS_H__

#include "fex.h"
#include "SSE2Intrinsic.h"


namespace OPT_LEVEL
{

template<typename TS1,CH cs1,typename TR>
struct StatSumGen: public fe1St<TS1,cs1>
{
	mutable TR sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatSumGen(){ sum = 0;}
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

		TR *p_mSum = (TR*)&feData.mSum;

		for(int i =0; i < (XMMREGSIZE/ sizeof(TR)) ; i++)
		{
			sum += p_mSum[i];
		}
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}
};

template<typename TS1,CH cs1,typename TR>
struct StatSumGenfc: public fe1St<TS1,cs1>
{
	mutable TR sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatSumGenfc(){ sum.re = 0; sum.im = 0;}
	IV REFR_Init() { sum.re  = 0; sum.im = 0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		sum += feData.sum;
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		sum += feData.sum;

		TR *p_mSum = (TR*)&feData.mSum;

		for(int i =0; i < (XMMREGSIZE/ sizeof(TR)) ; i++)
		{
			sum += p_mSum[i];
		}
	}

	IV SSE2_Init()
	{
		sum.re = 0;
		sum.im = 0;
		mSum.i = _mm_setzero_si128();
	}
};


struct StatSum_64f: public StatSumGen<Fw64f,C1,Fw64f>
{

    const static U32 nPIX_SSE = 2 * 3; // Load three registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};
	IV REFR(const Fw64f *s)     const                    // REFR Pixel function
	{
		sum  =  (sum  + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		mSum.d= _mm_add_pd(mSum.d,r.src1[0].d);
		mSum.d= _mm_add_pd(mSum.d,r.src1[1].d);
		mSum.d= _mm_add_pd(mSum.d,r.src1[2].d);
	}

};

struct StatSum_64fc: public StatSumGenfc<Fw64fc,C1,Fw64fc>
{

	IV REFR(const Fw64fc *s)     const                    // REFR Pixel function
	{
		sum  =  (sum  + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		mSum.d= _mm_add_pd(mSum.d,r.src1[0].d);
	}

};



struct StatMean_16s_Sfs: public StatSumGen<Fw16s,C1,Fw32s>
{

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum = (sum + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcHi,imRes,src;
		src.i = r.src1[0].i;
		CBL_SSE2::Unpack16STo32S(src.i,srcHi.i);

		imRes.i = _mm_add_epi32(src.i,srcHi.i);
		mSum.i= _mm_add_epi32(mSum.i,imRes.i);
	}
};

struct StatMean_16sc_Sfs: public StatSumGenfc<Fw16sc,C1,Fw32sc>
{

	IV REFR(const Fw16sc *s)     const                    // REFR Pixel function
	{
		sum.re = (sum.re + s->re);
		sum.im = (sum.im + s->im);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_hi,imRes,src;
		src.i = r.src1[0].i;
		CBL_SSE2::Unpack16STo32S(src.i,temp_hi.i);

		imRes.i = _mm_add_epi32(src.i,temp_hi.i);
		mSum.i= _mm_add_epi32(mSum.i,imRes.i);
	}
};

struct StatSum_32f: public StatSumGen<Fw32f,C1,Fw64f>
{
    const static U32 nPIX_SSE = 4 * 2; // Load two registers
    class SRC1: public RegDesc< Fw32f, C1, nPIX_SSE > {};

	IV REFR(const Fw32f *s)     const                    // REFR Pixel function
	{
		sum = (sum + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 slo,shi;
		_mm_cvt32f_64f(r.src1[0],slo,shi);
		slo.d = _mm_add_pd(slo.d,shi.d);
		mSum.d= _mm_add_pd(mSum.d,slo.d);

		_mm_cvt32f_64f(r.src1[1],slo,shi);
		slo.d = _mm_add_pd(slo.d,shi.d);
		mSum.d= _mm_add_pd(mSum.d,slo.d);
	}
};

struct StatSum_32fc: public StatSumGenfc<Fw32fc,C1,Fw64fc>
{

	IV REFR(const Fw32fc *s)     const                    // REFR Pixel function
	{
		sum.re = (sum.re + s->re);
		sum.im = (sum.im + s->im);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 slo,shi;
		_mm_cvt32f_64f(r.src1[0],slo,shi);
		slo.d = _mm_add_pd(slo.d,shi.d);
		mSum.d= _mm_add_pd(mSum.d,slo.d);
	}
};

struct StatSum_16s_Sfs: public StatSumGen<Fw16s,C1,Fw64f>
{

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum  =  (sum  + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_hi;
		CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp_hi.i);
		temp_hi.i = _mm_add_epi32(r.src1[0].i,temp_hi.i);
		__m128d src = _mm_cvtepi32_pd(temp_hi.i);
		mSum.d= _mm_add_pd(mSum.d,src);
		temp_hi.i = _mm_srli_si128(temp_hi.i, 8);
		src = _mm_cvtepi32_pd(temp_hi.i);
		mSum.d= _mm_add_pd(mSum.d,src);
	}

};

struct StatSum_16sc_Sfs: public StatSumGenfc<Fw16sc,C1,Fw64fc>
{

	IV REFR(const Fw16sc *s)     const                    // REFR Pixel function
	{
		sum.re = (sum.re + (Fw64f)s->re);
		sum.im = (sum.im + (Fw64f)s->im);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_hi;
		CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp_hi.i);
		temp_hi.i = _mm_add_epi32(r.src1[0].i,temp_hi.i);
		__m128d src = _mm_cvtepi32_pd(temp_hi.i);
		mSum.d= _mm_add_pd(mSum.d,src);
		temp_hi.i = _mm_srli_si128(temp_hi.i, 8);
		src = _mm_cvtepi32_pd(temp_hi.i);
		mSum.d= _mm_add_pd(mSum.d,src);
	}

};

struct StatSum_32s_Sfs: public StatSumGen<Fw32s,C1,Fw64f>
{

	IV REFR(const Fw32s *s)     const                    // REFR Pixel function
	{
		sum  =  (sum  + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcLo,srcHi;

		srcLo.d = _mm_cvtepi32_pd(r.src1[0].i);
		srcHi.i = _mm_srli_si128(r.src1[0].i,8);
		srcHi.d = _mm_cvtepi32_pd(srcHi.i);

		srcLo.d = _mm_add_pd(srcLo.d,srcHi.d);
		mSum.d= _mm_add_pd(mSum.d,srcLo.d);
	}

};

struct StatSum_16s32s_Sfs: public StatSumGen<Fw16s,C1,Fw64f>
{

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum  =  (sum  + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcLo,srcHi,temp_hi;

		CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp_hi.i);
		temp_hi.i = _mm_add_epi32(r.src1[0].i,temp_hi.i);

		srcLo.d = _mm_cvtepi32_pd(temp_hi.i);
		srcHi.i = _mm_srli_si128(temp_hi.i,8);
		srcHi.d = _mm_cvtepi32_pd(srcHi.i);

		srcLo.d = _mm_add_pd(srcLo.d,srcHi.d);
		mSum.d= _mm_add_pd(mSum.d,srcLo.d);
	}

};

struct StatStdDev_64f: public fe1St<Fw64f,C1>
{

    const static U32 nPIX_SSE = 2 * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};

	mutable Fw64f sum;
    XMM128 mMean;
    Fw64f mean;
	mutable XMM128 mSum;
	FEX_SSE2_REF

    StatStdDev_64f(){}
	StatStdDev_64f(Fw64f m){mean = m; sum = 0; }
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

		Fw64f *p_mSum = (Fw64f*)&feData.mSum;

		for(int i =0; i < (XMMREGSIZE/ sizeof(Fw64f)) ; i++)
		{
			sum += p_mSum[i];
		}
	}

	IV SSE2_Init()
	{
        sum = 0;
		mSum.d = _mm_setzero_pd();
        mMean.d = _mm_set1_pd(mean);
	}

	IV REFR(const Fw64f *s)     const                    // REFR Pixel function
	{

        Fw64f imRes = (*s - mean);
		imRes = imRes * imRes;
		sum = sum + imRes;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 imRes,imRes1;

        imRes.d = _mm_sub_pd(r.src1[0].d,mMean.d);
        imRes1.d= _mm_sub_pd(r.src1[1].d,mMean.d);
        imRes.d = _mm_mul_pd(imRes.d,imRes.d);
        imRes1.d = _mm_mul_pd(imRes1.d,imRes1.d);

        imRes.d= _mm_add_pd(imRes.d,imRes1.d);
        mSum.d = _mm_add_pd( mSum.d,imRes.d);
	}
};

struct StatStdDevMean_16s: public fe1St<Fw16s,C1>
{

    const static U32 nPIX_SSE = 8 * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

	mutable Fw32f sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatStdDevMean_16s(){ sum = 0; }
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

		Fw32f *p_mSum = (Fw32f*)&feData.mSum;

		for(int i =0; i < (XMMREGSIZE/ sizeof(Fw32f)) ; i++)
		{
			sum += p_mSum[i];
		}
	}

	IV SSE2_Init()
	{
        sum = 0;
		mSum.f = _mm_setzero_ps();
	}

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum = (sum + *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 srcHi,srcHi1,imRes,imRes1,src,src1;
        src.i = r.src1[0].i;
        src1.i = r.src1[1].i;

        CBL_SSE2::Unpack16STo32S(src.i,srcHi.i);
        imRes.i = _mm_add_epi32(src.i,srcHi.i);

        CBL_SSE2::Unpack16STo32S(src1.i,srcHi1.i);
        imRes1.i = _mm_add_epi32(src1.i,srcHi1.i);

        imRes.i = _mm_add_epi32(imRes.i,imRes1.i );

        imRes.f = _mm_cvtepi32_ps(imRes.i);
        mSum.f= _mm_add_ps(mSum.f,imRes.f);
	}
};
struct StatStdDev_16s: public fe1St<Fw16s,C1>
{

    const static U32 nPIX_SSE = 8 * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

	mutable Fw32f sum;
    XMM128 mMean;
    Fw32s mean;
	mutable XMM128 mSum;
	FEX_SSE2_REF

    StatStdDev_16s(){}
	StatStdDev_16s(Fw32s m){mean = m; sum = 0; }
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

		Fw32f *p_mSum = (Fw32f*)&feData.mSum;

		for(int i =0; i < (XMMREGSIZE/ sizeof(Fw32f)) ; i++)
		{
			sum += p_mSum[i];
		}
	}

	IV SSE2_Init()
	{
        sum = 0;
		mSum.f = _mm_setzero_ps();
        mMean.i = _mm_set1_epi32(mean);
	}

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{

        Fw32f imRes = (Fw32f) (*s - mean);
		imRes = imRes * imRes;
		sum = sum + imRes;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 srcHi,srcHi1,imRes,imRes1,src,src1;
        src.i = r.src1[0].i;
        src1.i = r.src1[1].i;

        CBL_SSE2::Unpack16STo32S(src.i,srcHi.i);
        imRes.i = _mm_sub_epi32(src.i,mMean.i);
        imRes1.i = _mm_sub_epi32(srcHi.i,mMean.i);
        imRes.f = _mm_cvtepi32_ps(imRes.i);
        imRes.f = _mm_mul_ps(imRes.f,imRes.f);
        imRes1.f = _mm_cvtepi32_ps(imRes1.i);
        imRes1.f = _mm_mul_ps(imRes1.f,imRes1.f);

        mSum.f= _mm_add_ps(mSum.f,imRes.f);
        mSum.f= _mm_add_ps(mSum.f,imRes1.f);

        CBL_SSE2::Unpack16STo32S(src1.i,srcHi1.i);

        imRes.i = _mm_sub_epi32(src1.i,mMean.i);
        imRes1.i = _mm_sub_epi32(srcHi1.i,mMean.i);
        imRes.f = _mm_cvtepi32_ps(imRes.i);
        imRes.f = _mm_mul_ps(imRes.f,imRes.f);
        imRes1.f = _mm_cvtepi32_ps(imRes1.i);
        imRes1.f = _mm_mul_ps(imRes1.f,imRes1.f);

        mSum.f= _mm_add_ps(mSum.f,imRes.f);
        mSum.f= _mm_add_ps(mSum.f,imRes1.f);
	}
};

} // namespace OPT_LEVEL

#endif // __STATISTICSS_H__

