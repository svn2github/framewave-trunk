/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISTICS_NORM_H__
#define __STATISTICS_NORM_H__

#include "fex.h"
#include "SSE2Intrinsic.h"

namespace OPT_LEVEL
{

union inttofloat
{
	Fw32f fVal;
	Fw32s iVal;
};

union inttodouble
{
	Fw64f dVal;
	Fw32s iVal[2];
};

SYS_INLINE STATIC Fw32s ABS_32S(Fw32s &src)
{
	Fw32s complement, temp  ;		//Finding the absolute value
	temp = (Fw32s)src >> 31;
	complement = src ^ temp;  //one's complement of src
	complement = complement - temp;
	temp  = (Fw32u)complement >> 31;
	return  complement - temp;
}

SYS_INLINE STATIC __m128i _mm_abs_epi32_forNorm(const __m128i &src) // returns the absolute value of the 16bit signed value.
{
	 /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
	__m128i complement, temp,dst  ;
	temp = _mm_srai_epi32(src, 31);
	complement = _mm_xor_si128(src, temp);  //one's complement of src
	dst = _mm_sub_epi32 (complement, temp);
	return dst;
}


template<typename TS1,CH cs1,typename TR>
struct StatNorm_InfGen: public fe1St<TS1,cs1>
{
	mutable TR max;
	mutable XMM128 mMax;
	Fw32s mask;
	XMM128 mMask;
	FEX_SSE2_REF
	StatNorm_InfGen():mask(CBL_LIBRARY::Limits<Fw32s>::MaxValue()) { max = 0;}
	IV REFR_Init() {mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();max =0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		max = MAX<TR>(max,feData.max);
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		max = MAX<TR>(max,feData.max);

		TR *p_mMax = (TR*)&feData.mMax;

		for(int i =0; i < (XMMREGSIZE/ sizeof(TR)) ; i++)
		{
			max = MAX<TR>(max,p_mMax[i]);
		}
	}
};


struct StatNorm_Inf_32f: public StatNorm_InfGen<Fw32f,C1,Fw32f>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw32f))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw32f, C1, nPIX_SSE > {};

	IV REFR(const Fw32f *s)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = *s;
		src.iVal = src.iVal & mask;
		max= MAX<Fw32f>(max,src.fVal);
	}

	IV SSE2_Init()
	{
		max =0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.i = _mm_and_si128(r.src1[0].i,mMask.i);
		mMax.f= _mm_max_ps(dst.f,mMax.f);
		dst.i = _mm_and_si128(r.src1[1].i,mMask.i);
		mMax.f= _mm_max_ps(dst.f,mMax.f);
	}

};

struct StatNorm_Inf_64f: public StatNorm_InfGen<Fw64f,C1,Fw64f>
{

    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw64f))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};

	IV REFR(const Fw64f *s)     const                    // REFR Pixel function
	{
		inttodouble src;
		src.dVal = *s;
		src.iVal[1] = src.iVal[1] & mask;
		max= MAX(max,src.dVal);
	}

	IV SSE2_Init()
	{
		max =0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32u>::MaxValue());
		mMask.i = _mm_srli_epi64(mMask.i,1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.i = _mm_and_si128(r.src1[0].i, mMask.i);
		mMax.d= _mm_max_pd(dst.d,mMax.d);
		dst.i = _mm_and_si128(r.src1[1].i, mMask.i);
		mMax.d= _mm_max_pd(dst.d,mMax.d);

	}
};

struct StatNorm_Inf_16s32f: fe1St<Fw16s,C1>
{
    XMM128 zero;
	mutable Fw16s max;
	mutable XMM128 mMax;
    
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw16s))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw16s, C1, nPIX_SSE > {};

	FEX_SSE2_REF
	StatNorm_Inf_16s32f(){ max = 0;}
	IV REFR_Init() {max =0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		max = MIN<Fw16s>(max,feData.max);
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		max = MIN<Fw16s>(max,feData.max);

		Fw16s *p_mMax = (Fw16s*)&feData.mMax;

		for(int i =0; i < (XMMREGSIZE/ sizeof(Fw16s)) ; i++)
		{
			max = MIN<Fw16s>(max,p_mMax[i]);
		}
	}

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
       
        Fw16s temp = 0-*s;
        temp = MIN<Fw16s>(temp ,*s);
		max = MIN<Fw16s>(max,temp);
	}

	IV SSE2_Init()
	{
		max = 0;
		mMax.i = _mm_setzero_si128();
        zero.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
        XMM128 subval,subval1;
        subval.i = _mm_sub_epi16(zero.i,r.src1[0].i);
        subval1.i = _mm_sub_epi16(zero.i,r.src1[1].i);

        subval.i = _mm_min_epi16(subval.i,r.src1[0].i);
        subval1.i = _mm_min_epi16(subval1.i,r.src1[1].i);
        mMax.i   = _mm_min_epi16(subval.i,mMax.i);
        mMax.i   = _mm_min_epi16(subval1.i,mMax.i);             

	}

};


struct StatNorm_Inf_32fc32f: public StatNorm_InfGen<Fw32fc,C1,Fw64f>
{

	IV REFR(const Fw32fc *s)     const                    // REFR Pixel function
	{
		Fw64f mag = ((Fw64f)s[0].re * s[0].re) + ((Fw64f)s[0].im * s[0].im);
		max = MAX(max,mag);
	}

	IV SSE2_Init()
	{
		max = 0;
		mMax.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag,mag1;
		XMM128 lo,hi;

		_mm_cvt32f_64f(r.src1[0],lo,hi);
		dst.d = _mm_mul_pd(lo.d,lo.d);
		mag.i = _mm_srli_si128(dst.i,8);
		mag.d = _mm_add_sd(dst.d,mag.d);

		dst.d = _mm_mul_pd(hi.d,hi.d);
		mag1.i = _mm_srli_si128(dst.i,8);
		mag1.d = _mm_add_sd(dst.d,mag1.d);

		dst.d =  _mm_unpacklo_pd(mag.d,mag1.d);

		mMax.d= _mm_max_pd(mMax.d,dst.d);
	}

};

struct StatNorm_Inf_64fc64f: public StatNorm_InfGen<Fw64fc,C1,Fw64f>
{

	IV REFR(const Fw64fc *s)     const                    // REFR Pixel function
	{
		Fw64f mag = (s[0].re * s[0].re) + (s[0].im * s[0].im);
		max= MAX(max,mag);
	}

	IV SSE2_Init()
	{
		max = 0;
		mMax.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.d = _mm_mul_pd(r.src1[0].d,r.src1[0].d);
		mag.i = _mm_srli_si128(dst.i,8);
		dst.d = _mm_add_pd(mag.d,dst.d);
		dst.i = _mm_slli_si128 (dst.i,8);
		mMax.d= _mm_max_pd(dst.d,mMax.d);
	}

};

template<typename TS1,CH cs1,typename TR>
struct StatNorm_L1Gen: public fe1St<TS1,cs1>
{
	mutable TR sum;
	mutable XMM128 mSum;
	Fw32s mask;
	XMM128 mMask;
	FEX_SSE2_REF
	StatNorm_L1Gen(): mask(CBL_LIBRARY::Limits<Fw32s>::MaxValue()) {sum = 0;}
	IV REFR_Init() {mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();sum =0; }

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
};

struct StatNorm_L1_32f: public StatNorm_L1Gen<Fw32f,C1,Fw32f>
{
	IV REFR(const Fw32f *s)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = *s;
		src.iVal = src.iVal & mask;
		sum  += src.fVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dest;
		dest.i = _mm_and_si128(r.src1[0].i,mMask.i);
		mSum.f = _mm_add_ps(dest.f,mSum.f);
	}
};

struct StatNorm_L1_64f: public StatNorm_L1Gen<Fw64f,C1,Fw64f>
{
    const static U32 nPIX_SSE = 2 * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};

	IV REFR(const Fw64f *s)     const                    // REFR Pixel function
	{
		inttodouble src;
		src.dVal = *s;
		src.iVal[1] = src.iVal[1] & mask;
		sum  += src.dVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32u>::MaxValue());
		mMask.i = _mm_srli_epi64(mMask.i,1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;

		dst.i = _mm_and_si128(mMask.i, r.src1[0].i);
		mSum.d = _mm_add_pd(dst.d,mSum.d);
		dst.i = _mm_and_si128(mMask.i, r.src1[1].i);
		mSum.d = _mm_add_pd(dst.d,mSum.d);
	}
};

struct StatNorm_L1_16s32f: public StatNorm_L1Gen<Fw16s,C1,Fw32f>
{

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = (Fw32f)*s;
		src.iVal = src.iVal & mask;
		sum += src.fVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_lo, temp_hi;

		temp_lo.i = r.src1[0].i;
		CBL_SSE2::Unpack16STo32S(temp_lo.i,temp_hi.i);

		temp_lo.f = _mm_cvtepi32_ps(temp_lo.i);
		temp_hi.f = _mm_cvtepi32_ps(temp_hi.i);

		temp_lo.i = _mm_and_si128(temp_lo.i,mMask.i);
		temp_hi.i = _mm_and_si128(temp_hi.i,mMask.i);

		temp_lo.f	= _mm_add_ps(temp_lo.f,temp_hi.f);
		mSum.f = _mm_add_ps(mSum.f,temp_lo.f);
	}

};

struct StatNorm_L1_32fc64f: public StatNorm_L1Gen<Fw32fc,C1,Fw64f>
{
	const static U32 nPIX_SSE = 2 * 2; // Load two registers
	class SRC1: public RegDesc< Fw32fc, C1, nPIX_SSE > {};

	IV REFR(const Fw32fc *s)     const                    // REFR Pixel function
	{
		Fw64f mul= ((Fw64f)s[0].re * s[0].re) + ((Fw64f)s[0].im * s[0].im);
		Fw64f mag = sqrt(mul);
		sum += mag;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 des1,des2,des3,des4;
		des1.f = _mm_mul_ps(r.src1[0].f,r.src1[0].f);
		des2.f = _mm_mul_ps(r.src1[1].f,r.src1[1].f);

		des3.f = _mm_shuffle_ps(des1.f,des2.f,_MM_SHUFFLE(2,0,2,0)); // re 2
		des4.f = _mm_shuffle_ps(des1.f,des2.f,_MM_SHUFFLE(3,1,3,1)); // im 2

		des3.f = _mm_add_ps(des3.f,des4.f);
		des3.f = _mm_sqrt_ps(des3.f);

		_mm_cvt32f_64f(des3,des1,des2);
		des1.d = _mm_add_pd(des1.d,des2.d);
		mSum.d= _mm_add_pd(mSum.d,des1.d);
	}
};


struct StatNorm_L1_64fc64f: public StatNorm_L1Gen<Fw64fc,C1,Fw64f>
{
	const static U32 nPIX_SSE = 1 * 2; // Load two registers
	class SRC1: public RegDesc< Fw64fc, C1, nPIX_SSE > {};

	IV REFR(const Fw64fc *s)     const                    // REFR Pixel function
	{
		Fw64f mag = sqrt((s[0].re * s[0].re) + (s[0].im * s[0].im));
		sum += mag;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 des1,des2,des3,des4;
		des1.d = _mm_mul_pd(r.src1[0].d,r.src1[0].d);
		des2.d = _mm_mul_pd(r.src1[1].d,r.src1[1].d);

		des3.f =  CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f(des1.f,des2.f); // re0 2 re1 2
		des4.f =  CBL_OPTLEVEL::Shuffle_a3a2b3b2_32f(des2.f,des1.f); // im0 2 im1 2

		des1.d = _mm_add_pd(des3.d,des4.d);
		des1.d = _mm_sqrt_pd(des1.d);
		mSum.d= _mm_add_pd(mSum.d,des1.d);
	}

};

struct StatNorm_L1_16s32s_Sfs: public StatNorm_L1Gen<Fw16s,C1,Fw64s>
{

	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		Fw32s src = *s;
		src  = ABS_32S(src);
		sum +=src;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 srcLo,srcHi;

		srcLo.i = r.src1[0].i;
		CBL_SSE2::Unpack16STo32S(srcLo.i,srcHi.i);
		srcLo.i = mm_abs_epi32(srcLo.i);
		srcHi.i = mm_abs_epi32(srcHi.i);
		srcLo.i = _mm_add_epi32(srcLo.i,srcHi.i);
		CBL_SSE2::Unpack32UTo64U(srcLo.i,srcHi.i);
		srcLo.i = _mm_add_epi64(srcLo.i,srcHi.i);
		mSum.i = _mm_add_epi64(mSum.i,srcLo.i);
	}

};


template<typename TS1,CH cs1,typename TR>
struct StatNorm_L2Gen: public fe1St<TS1,cs1>
{
	mutable TR sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatNorm_L2Gen() { sum = 0;}
	IV REFR_Init() {sum =0; }
	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

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
};

struct StatNorm_L2_32f: public StatNorm_L2Gen<Fw32f,C1,Fw32f>
{
	IV REFR(const Fw32f *s)     const                    // REFR Pixel function
	{
		sum += *s * *s;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dest;
		dest.f = _mm_mul_ps(r.src1[0].f,r.src1[0].f);
		mSum.f = _mm_add_ps(dest.f,mSum.f);
	}
};

struct StatNorm_L2_64f: public StatNorm_L2Gen<Fw64f,C1,Fw64f>
{
    const static U32 nPIX_SSE = 2 * 2; // Load two registers
    class SRC1: public RegDesc< Fw64f, C1, nPIX_SSE > {};

	IV REFR(const Fw64f *s)     const                    // REFR Pixel function
	{
		sum += *s * *s;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.d = _mm_mul_pd(r.src1[0].d,r.src1[0].d);
		mSum.d = _mm_add_pd(dst.d,mSum.d);
		dst.d = _mm_mul_pd(r.src1[1].d,r.src1[1].d);
		mSum.d = _mm_add_pd(dst.d,mSum.d);
	}
};


struct StatNorm_L2_16s32f: public StatNorm_L2Gen<Fw16s,C1,Fw64f>
{
	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum  += (Fw64f)*s * (Fw64f)*s;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 mul_lo,mul_hi;

		_mm_mul_16s_32s(r.src1[0].i,r.src1[0].i,mul_lo.i,mul_hi.i);

		mul_lo.f =  _mm_cvtepi32_ps(mul_lo.i);
		mul_hi.f =  _mm_cvtepi32_ps(mul_hi.i);
		mul_hi.f = _mm_add_ps(mul_lo.f,mul_hi.f);
		_mm_cvt32f_64f(mul_hi,mul_lo,mul_hi);
		mul_hi.d = _mm_add_pd(mul_lo.d,mul_hi.d);
		mSum.d = _mm_add_pd(mSum.d,mul_hi.d);
	}
};



struct StatNorm_L2_32fc64f: public StatNorm_L2Gen<Fw32fc,C1,Fw64f>
{
	IV REFR(const Fw32fc *s)     const                    // REFR Pixel function
	{
		Fw32f sum1;
		sum1 = s->re * s->re + s->im * s->im;
		sum += sum1;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.f = _mm_mul_ps(r.src1[0].f,r.src1[0].f);
		mag.i = _mm_srli_epi64(dst.i,32);
		dst.f = _mm_add_ps(mag.f,dst.f);
		dst.f = _mm_shuffle_ps(dst.f,dst.f,_MM_SHUFFLE(3,2,2,0));
		dst.d = _mm_cvtps_pd(dst.f);
		mSum.d= _mm_add_pd(dst.d,mSum.d);
	}
};

struct StatNorm_L2_64fc64f: public StatNorm_L2Gen<Fw64fc,C1,Fw64f>
{
	IV REFR(const Fw64fc *s)     const                    // REFR Pixel function
	{
		Fw64f sum1;
		sum1 = s->re * s->re + s->im * s->im;
		sum  += sum1;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.d = _mm_mul_pd(r.src1[0].d,r.src1[0].d);
		mag.i = _mm_srli_si128(dst.i,8);
		dst.d = _mm_add_pd(mag.d,dst.d);
		dst.i = _mm_slli_si128 (dst.i,8);
		mSum.d= _mm_add_pd(dst.d,mSum.d);
	}
};


struct StatNorm_L2_16s32s_Sfs: public StatNorm_L2Gen<Fw16s,C1,Fw64s>
{
	IV REFR(const Fw16s *s)     const                    // REFR Pixel function
	{
		sum += ((Fw32s)*s * *s);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp1,temp2,temp3;
		_mm_mul_16s_32s(r.src1[0].i,r.src1[0].i,temp1.i,temp2.i);
		CBL_SSE2::Unpack32STo64S(temp1.i,temp3.i);
		temp1.i = _mm_add_epi64(temp1.i,temp3.i);
		CBL_SSE2::Unpack32STo64S(temp2.i,temp3.i);
		temp2.i = _mm_add_epi64(temp2.i,temp3.i);
		temp1.i = _mm_add_epi64(temp1.i,temp2.i);
		mSum.i = _mm_add_epi64(mSum.i,temp1.i);
	}
};


template<typename TS1,CH cs1,typename TS2,CH cs2,typename TR>
struct StatNormDiff_InfGen: public fe2St<TS1,cs1,TS2,cs2>
{
	mutable TR max;
	mutable XMM128 mMax;
	Fw32s mask;
	XMM128 mMask;
	FEX_SSE2_REF
	StatNormDiff_InfGen(): mask(CBL_LIBRARY::Limits<Fw32s>::MaxValue()){ max = 0;}
	IV REFR_Init() {mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();max =0; }

	template<class FE>
	IV REFR_Post(FE &feData)
	{
		max = MAX<TR>(max,feData.max);
	}

	template<class FE>
	IV SSE2_Post(FE &feData)
	{
		max = MAX<TR>(max,feData.max);

		TR *p_mMax = (TR*)&feData.mMax;

		for(int i =0; i < (XMMREGSIZE/ sizeof(TR)) ; i++)
		{
			max = MAX<TR>(max,p_mMax[i]);
		}
	}
};

struct StatNormDiff_Inf_32f: public StatNormDiff_InfGen<Fw32f,C1,Fw32f,C1,Fw32f>
{

	IV REFR(const Fw32f *s1,const Fw32f *s2)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = *s1 - *s2;
		src.iVal = src.iVal & mask;
		max= MAX<Fw32f>(max,src.fVal);
	}

	IV SSE2_Init()
	{
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		dst.i = _mm_and_si128(dst.i, mMask.i);
		mMax.f= _mm_max_ps(dst.f,mMax.f);
	}

};

struct StatNormDiff_Inf_64f: public StatNormDiff_InfGen<Fw64f,C1,Fw64f,C1,Fw64f>
{

	IV REFR(const Fw64f *s1,const Fw64f *s2)     const                    // REFR Pixel function
	{
		inttodouble src;
		src.dVal= *s1 - *s2;
		src.iVal[1] = src.iVal[1] & mask;
		max= MAX<Fw64f>(max,src.dVal);
	}

	IV SSE2_Init()
	{
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32u>::MaxValue());
		mMask.i = _mm_srli_epi64(mMask.i,1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);
		dst.i =_mm_and_si128(dst.i, mMask.i);
		mMax.d= _mm_max_pd(dst.d,mMax.d);
	}

};

struct StatNormDiff_Inf_16s32f: public StatNormDiff_InfGen<Fw16s,C1,Fw16s,C1,Fw32f>
{

	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal =(Fw32f) ((Fw32s) *s1-*s2);
		src.iVal = src.iVal & mask;
		max = MAX<Fw32f>(max,src.fVal);
	}

	IV SSE2_Init()
	{
		max = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 src1Hi,src2Hi,src1Lo,src2Lo;

		src1Lo.i = r.src1[0].i;
		src2Lo.i = r.src2[0].i;
		CBL_SSE2::Unpack16STo32S(src1Lo.i,src1Hi.i);
		CBL_SSE2::Unpack16STo32S(src2Lo.i,src2Hi.i);

		src1Lo.i = _mm_sub_epi32(src1Lo.i,src2Lo.i);
		src2Lo.i = _mm_sub_epi32(src1Hi.i,src2Hi.i);

		src1Lo.f = _mm_cvtepi32_ps(src1Lo.i);
		src2Lo.f = _mm_cvtepi32_ps(src2Lo.i);

		src1Lo.i = _mm_and_si128(src1Lo.i,mMask.i );
		src2Lo.i = _mm_and_si128(src2Lo.i,mMask.i );

		src1Lo.f = _mm_max_ps(src1Lo.f,src2Lo.f);
		mMax.f = _mm_max_ps(mMax.f,src1Lo.f);
	}

};


struct StatNormDiff_Inf_32fc32f: public StatNormDiff_InfGen<Fw32fc,C1,Fw32fc,C1,Fw64f>
{

	IV REFR(const Fw32fc *s1,const Fw32fc *s2)     const                    // REFR Pixel function
	{
		Fw32fc diff;
		diff.re = s1->re - s2->re;
		diff.im = s1->im - s2->im;

		Fw64f mag = ((Fw64f)diff.re * diff.re) + ((Fw64f)diff.im * diff.im);
		max= MAX<Fw64f>(max,mag);
	}

	IV SSE2_Init()
	{
		max = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mMax.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag,slo,shi;

		dst.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		//dst.f = _mm_mul_ps(dst.f ,dst.f );
		_mm_cvt32f_64f(dst,slo,shi);
		slo.d = _mm_mul_pd(slo.d,slo.d);
		shi.d = _mm_mul_pd(shi.d,shi.d);

		mag.d = CBL_SSE2::Shuffle_b0a0_64f(slo.d,shi.d); /// 2re2 1re2
		slo.d = CBL_SSE2::Shuffle_b1a1_64f(slo.d,shi.d); /// 2im2 1im2
		slo.d = _mm_add_pd(mag.d,slo.d);

		mMax.d= _mm_max_pd(mMax.d,slo.d);
	}

};

struct StatNormDiff_Inf_64fc64f: public StatNormDiff_InfGen<Fw64fc,C1,Fw64fc,C1,Fw64f>
{

	IV REFR(const Fw64fc *s1,const Fw64fc *s2)     const                    // REFR Pixel function
	{
		Fw64fc diff;
		diff.re = s1->re - s2->re;
		diff.im = s1->im - s2->im;
		Fw64f mag = (diff.re * diff.re) + (diff.im * diff.im);
		max= MAX<Fw64f>(max,mag);
	}

	IV SSE2_Init()
	{
		max = 0;
		mMax.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);
		dst.d = _mm_mul_pd(dst.d,dst.d);

		mag.d = _mm_shuffle_pd(dst.d,dst.d,_MM_SHUFFLE2(0,1));
		dst.d = _mm_add_pd(mag.d,dst.d);
		mMax.d= _mm_max_pd(dst.d,mMax.d);
	}
};



struct StatNormDiff_Inf_16s32s: public StatNormDiff_InfGen<Fw16s,C1,Fw16s,C1,Fw32s>
{

	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		Fw32s src = (Fw32s) *s1-*s2;
		src = ABS_32S(src);
		max= MAX<Fw32s>(max,src);
	}

	IV SSE2_Init()
	{
		max = 0;
		mMax.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 src1Hi,src2Hi,src1Lo,src2Lo;

		src1Lo.i = r.src1[0].i;
		src2Lo.i = r.src2[0].i;
		CBL_SSE2::Unpack16STo32S(src1Lo.i,src1Hi.i);
		CBL_SSE2::Unpack16STo32S(src2Lo.i,src2Hi.i);

		src1Lo.i = _mm_sub_epi32(src1Lo.i,src2Lo.i);
		src2Lo.i = _mm_sub_epi32(src1Hi.i,src2Hi.i);

        src1Lo.i = mm_abs_epi32(src1Lo.i);
        src2Lo.i = mm_abs_epi32(src2Lo.i);

        src1Lo.i = mm_max_epi32(src1Lo.i,src2Lo.i);
		mMax.i = mm_max_epi32(mMax.i,src1Lo.i);
	}

};


template<typename TS1,CH cs1,typename TS2,CH cs2,typename TR>
struct StatNormDiff_L1Gen: public fe2St<TS1,cs1,TS2,cs2>
{
	mutable TR sum;
	mutable XMM128 mSum;
	Fw32s mask;
	XMM128 mMask;
	FEX_SSE2_REF
	StatNormDiff_L1Gen():mask(CBL_LIBRARY::Limits<Fw32s>::MaxValue()) {sum = 0;}
	IV REFR_Init() {mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();sum =0; }

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
};

struct StatNormDiff_L1_32f: public StatNormDiff_L1Gen<Fw32f,C1,Fw32f,C1,Fw32f>
{
	IV REFR(const Fw32f *s1,const Fw32f *s2)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = *s1 - *s2;
		src.iVal = src.iVal & mask;
		sum += src.fVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dest;
		dest.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		dest.i = _mm_and_si128(dest.i,mMask.i);
		mSum.f = _mm_add_ps(dest.f,mSum.f);
	}
};

struct StatNormDiff_L1_64f: public StatNormDiff_L1Gen<Fw64f,C1,Fw64f,C1,Fw64f>
{
	IV REFR(const Fw64f *s1,const Fw64f *s2)     const                    // REFR Pixel function
	{
		inttodouble src;
		src.dVal = *s1-*s2;
		src.iVal[1] = src.iVal[1] & mask;
		sum += src.dVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32u>::MaxValue());
		mMask.i = _mm_srli_epi64(mMask.i,1);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);
		dst.i = _mm_and_si128(dst.i,mMask.i);

		mSum.d = _mm_add_pd(dst.d,mSum.d);
	}
};

struct StatNormDiff_L1_16s32f: public StatNormDiff_L1Gen<Fw16s,C1,Fw16s,C1,Fw32f>
{

	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		inttofloat src;
		src.fVal = (Fw32f)((Fw32s)*s1 - (Fw32s)*s2);
		src.iVal = src.iVal & mask;
		sum += src.fVal;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
		mSum.i = _mm_setzero_si128();
		mMask.i = _mm_set1_epi32(CBL_LIBRARY::Limits<Fw32s>::MaxValue());
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_lo, temp_hi,temp1_lo,temp2_lo;

		CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp_lo.i);
		CBL_SSE2::Unpack16STo32S(r.src2[0].i,temp1_lo.i);

		temp2_lo.i = _mm_sub_epi32(r.src1[0].i,r.src2[0].i);
		temp_hi.i = _mm_sub_epi32(temp_lo.i,temp1_lo.i);

		temp2_lo.f = _mm_cvtepi32_ps(temp2_lo.i);
		temp_hi.f = _mm_cvtepi32_ps(temp_hi.i);

		temp_hi.i = _mm_and_si128(temp_hi.i,mMask.i);
		temp2_lo.i = _mm_and_si128(temp2_lo.i,mMask.i);

		temp2_lo.f	= _mm_add_ps(temp2_lo.f,temp_hi.f);
		mSum.f = _mm_add_ps(temp2_lo.f,mSum.f);
	}

};

struct StatNormDiff_L1_32fc64f: public StatNormDiff_L1Gen<Fw32fc,C1,Fw32fc,C1,Fw64f>
{
    const static U32 nPIX_SSE = (XMMREGSIZE / sizeof(Fw32fc))  * 2; // Load two registers
    class SRC1: public RegDesc< Fw32fc, C1, nPIX_SSE > {};
    class SRC2: public RegDesc< Fw32fc, C1, nPIX_SSE > {};

	IV REFR(const Fw32fc *s1,const Fw32fc *s2)     const                    // REFR Pixel function
	{
		Fw64fc diff;
		diff.re = (Fw64f)s1->re - s2->re;
		diff.im = (Fw64f)s1->im - s2->im;

		Fw64f mag= (diff.re * diff.re) + (diff.im * diff.im);
		mag = sqrt(mag);
		sum  += mag;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,dst1;
		dst.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		dst1.f = _mm_sub_ps(r.src1[1].f,r.src2[1].f);

		__m128d re1 = _mm_cvtps_pd(dst.f);
		dst.d = _mm_unpackhi_pd(dst.d,dst.d);
		__m128d re2 = _mm_cvtps_pd(dst.f);

		re1 = _mm_mul_pd(re1,re1);
		re2 = _mm_mul_pd(re2,re2);

		__m128d t1 = CBL_SSE2::Shuffle_b0a0_64f(re1,re2);	//re2, re1
		__m128d t2 = CBL_SSE2::Shuffle_b1a1_64f(re1,re1);   //im2, im1

		t1 = _mm_add_pd(t1,t2);
		t1 = _mm_sqrt_pd(t1);
		mSum.d= _mm_add_pd(mSum.d,t1 );

		re1 = _mm_cvtps_pd(dst1.f);
		dst1.d = _mm_unpackhi_pd(dst1.d,dst1.d);
		re2 = _mm_cvtps_pd(dst1.f);

		re1 = _mm_mul_pd(re1,re1);
		re2 = _mm_mul_pd(re2,re2);

		t1 = CBL_SSE2::Shuffle_b0a0_64f(re1,re2);	//re2, re1
		t2 = CBL_SSE2::Shuffle_b1a1_64f(re1,re1);   //im2, im1

		t1 = _mm_add_pd(t1,t2);
		t1 = _mm_sqrt_pd(t1);
		mSum.d= _mm_add_pd(mSum.d,t1 );
	}

};


struct StatNormDiff_L1_64fc64f: public StatNormDiff_L1Gen<Fw64fc,C1,Fw64fc,C1,Fw64f>
{

	IV REFR(const Fw64fc *s1,const Fw64fc *s2)     const                    // REFR Pixel function
	{
		Fw64fc diff;
		diff.re = s1->re - s2->re;
		diff.im = s1->im - s2->im;
		Fw64f mag = sqrt((diff.re * diff.re) + (diff.im * diff.im));
		sum += mag;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 des1,des2;
		des1.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);

		des1.d = _mm_mul_pd(des1.d,des1.d);
		des2.f = CBL_OPTLEVEL::Shuffle_a3a2b3b2_32f(des1.f,des1.f);

		des1.d = _mm_add_sd(des1.d,des2.d);
		des1.d = _mm_sqrt_sd(des1.d,des1.d);
		mSum.d= _mm_add_sd(mSum.d,des1.d);
	}

};

struct StatNormDiff_L1_16s32s_Sfs: public StatNormDiff_L1Gen<Fw16s,C1,Fw16s,C1,Fw64s>
{

	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		Fw32s src=(Fw32s)*s1-*s2;
		src = ABS_32S(src);
		sum += src;
	}

	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128  src1_lo,src1_hi,src2_lo,src2_hi;

		src1_lo.i = r.src1[0].i;
		src2_lo.i = r.src2[0].i;

		if((0 == (0xAAAA & _mm_movemask_epi8(src1_lo.i))) && (0 == (0xAAAA & _mm_movemask_epi8(src2_lo.i))))
		{


			src1_hi.i = _mm_max_epi16(src1_lo.i,src2_lo.i);
			src2_hi.i = _mm_min_epi16(src1_lo.i,src2_lo.i);

			src1_lo.i = _mm_sub_epi32(src1_hi.i,src2_hi.i);

			CBL_SSE2::Unpack16UTo32U(src1_lo.i,src1_hi.i);
			src1_lo.i = _mm_add_epi32(src1_lo.i,src1_hi.i);


			CBL_SSE2::Unpack32UTo64U(src1_lo.i,src1_hi.i);
			src1_lo.i = _mm_add_epi64(src1_lo.i,src1_hi.i);

			mSum.i = _mm_add_epi64(mSum.i,src1_lo.i);


		}
		else
		{
			CBL_SSE2::Unpack16STo32S(src1_lo.i,src1_hi.i);
			CBL_SSE2::Unpack16STo32S(src2_lo.i,src2_hi.i);

			src1_lo.i = _mm_sub_epi32(src1_lo.i,src2_lo.i);
			src1_hi.i = _mm_sub_epi32(src1_hi.i,src2_hi.i);

			src1_lo.i = _mm_abs_epi32_forNorm(src1_lo.i);
			src1_hi.i = _mm_abs_epi32_forNorm(src1_hi.i);

			src1_lo.i = _mm_add_epi32(src1_lo.i,src1_hi.i);

			CBL_SSE2::Unpack32UTo64U(src1_lo.i,src1_hi.i);
			src1_lo.i = _mm_add_epi64(src1_lo.i,src1_hi.i);

			mSum.i = _mm_add_epi64(mSum.i,src1_lo.i);
		}
	}

};



template<typename TS1,CH cs1,typename TS2,CH cs2,typename TR>
struct StatNormDiff_L2Gen: public fe2St<TS1,cs1,TS2,cs2>
{
	mutable TR sum;
	mutable XMM128 mSum;
	FEX_SSE2_REF
	StatNormDiff_L2Gen() { sum = 0;}
	IV REFR_Init() {sum =0; }
	IV SSE2_Init()
	{
		sum = 0;
		mSum.i = _mm_setzero_si128();
	}

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
};

struct StatNormDiff_L2_32f: public StatNormDiff_L2Gen<Fw32f,C1,Fw32f,C1,Fw32f>
{
	IV REFR(const Fw32f *s1,const Fw32f *s2)     const                    // REFR Pixel function
	{
		Fw32f diff;
		diff = *s1 - *s2;
		sum += diff * diff;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dest;
		dest.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		dest.f = _mm_mul_ps(dest.f,dest.f);
		mSum.f = _mm_add_ps(dest.f,mSum.f);
	}
};

struct StatNormDiff_L2_64f: public StatNormDiff_L2Gen<Fw64f,C1,Fw64f,C1,Fw64f>
{
	IV REFR(const Fw64f *s1,const Fw64f *s2)     const                    // REFR Pixel function
	{
		Fw64f diff;
		diff = *s1 - *s2;
		sum += diff * diff;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst;
		dst.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);
		dst.d = _mm_mul_pd(dst.d, dst.d);
		mSum.d = _mm_add_pd(dst.d,mSum.d);
	}
};


struct StatNormDiff_L2_16s32f: public StatNormDiff_L2Gen<Fw16s,C1,Fw16s,C1,Fw64f>
{
	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		Fw32s diff;
		diff = (Fw32s)*s1 - (Fw32s)*s2;
		sum += (Fw64f)diff * (Fw64f)diff;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 temp_lo, temp_hi,temp1_lo,temp2_lo,temp1,temp2;

		CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp_lo.i);
		CBL_SSE2::Unpack16STo32S(r.src2[0].i,temp1_lo.i);

		temp2_lo.i = _mm_sub_epi32(r.src1[0].i,r.src2[0].i);
		temp_hi.i = _mm_sub_epi32(temp_lo.i,temp1_lo.i);

		temp1.d = _mm_cvtepi32_pd(temp2_lo.i);
		temp1.d = _mm_mul_pd(temp1.d,temp1.d);

		temp2_lo.i = _mm_srli_si128(temp2_lo.i,8);
		temp2_lo.d = _mm_cvtepi32_pd(temp2_lo.i);
		temp2_lo.d = _mm_mul_pd(temp2_lo.d,temp2_lo.d);


		temp2.d = _mm_cvtepi32_pd(temp_hi.i);
		temp2.d = _mm_mul_pd(temp2.d,temp2.d);

		temp_hi.i = _mm_srli_si128(temp_hi.i,8);
		temp_hi.d = _mm_cvtepi32_pd(temp_hi.i);
		temp_hi.d = _mm_mul_pd(temp_hi.d,temp_hi.d);

		temp2_lo.d	= _mm_add_pd(temp2_lo.d,temp_hi.d);
		temp2.d	= _mm_add_pd(temp2.d,temp1.d);
		temp2.d	= _mm_add_pd(temp2.d,temp2_lo.d);
		mSum.d = _mm_add_pd(temp2.d,mSum.d);
	}
};



struct StatNormDiff_L2_32fc64f: public StatNormDiff_L2Gen<Fw32fc,C1,Fw32fc,C1,Fw64f>
{
	IV REFR(const Fw32fc *s1,const Fw32fc *s2)     const                    // REFR Pixel function
	{
		Fw32f sum1;
		Fw32fc diff;
		diff.re = s1->re - s2->re;
		diff.im = s1->im - s2->im;
		sum1 = diff.re * diff.re + diff.im * diff.im;
		sum += sum1;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.f = _mm_sub_ps(r.src1[0].f,r.src2[0].f);
		dst.f = _mm_mul_ps(dst.f,dst.f);
		mag.i = _mm_srli_epi64(dst.i,32);
		dst.f = _mm_add_ps(mag.f,dst.f);
		dst.f = _mm_shuffle_ps(dst.f,dst.f,_MM_SHUFFLE(3,2,2,0));
		dst.d = _mm_cvtps_pd(dst.f);
		mSum.d= _mm_add_pd(dst.d,mSum.d);
	}
};

struct StatNormDiff_L2_64fc64f: public StatNormDiff_L2Gen<Fw64fc,C1,Fw64fc,C1,Fw64f>
{
	IV REFR(const Fw64fc *s1,const Fw64fc *s2)     const                    // REFR Pixel function
	{
		Fw64f sum1;
		Fw64fc diff;
		diff.re = s1->re - s2->re;
		diff.im = s1->im - s2->im;
		sum1 = diff.re * diff.re + diff.im * diff.im;
		sum += sum1;
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128 dst,mag;
		dst.d = _mm_sub_pd(r.src1[0].d,r.src2[0].d);
		dst.d = _mm_mul_pd(dst.d,dst.d );
		mag.i = _mm_srli_si128(dst.i,8);
		dst.d = _mm_add_pd(mag.d,dst.d);
		dst.i = _mm_slli_si128 (dst.i,8);
		mSum.d= _mm_add_pd(dst.d,mSum.d);
	}
};


struct StatNormDiff_L2_16s32s_Sfs: public StatNormDiff_L2Gen<Fw16s,C1,Fw16s,C1,Fw64f>
{
	IV REFR(const Fw16s *s1,const Fw16s *s2)     const                    // REFR Pixel function
	{
		Fw32s diff;
		diff = (Fw32s)*s1 - (Fw32s)*s2;
		sum += ((Fw64f)diff * (Fw64f)diff);
	}

	IV SSE2( RegFile & r )  const                       // SSE2 Pixel function
	{
		XMM128  src1Hi,src2Hi,imRes;

		CBL_SSE2::Unpack16STo32S(r.src1[0].i,src1Hi.i);
		CBL_SSE2::Unpack16STo32S(r.src2[0].i,src2Hi.i);

		imRes.i = _mm_sub_epi32(r.src1[0].i,r.src2[0].i);
		src1Hi.i = _mm_sub_epi32(src1Hi.i,src2Hi.i);

		imRes.f = _mm_cvtepi32_ps(imRes.i);
		src1Hi.f = _mm_cvtepi32_ps(src1Hi.i);

		imRes.f = _mm_mul_ps(imRes.f,imRes.f);
		src1Hi.f = _mm_mul_ps(src1Hi.f,src1Hi.f);

		src1Hi.f = _mm_add_ps(imRes.f,src1Hi.f);

		_mm_cvt32f_64f(src1Hi,src2Hi,imRes);

		src2Hi.d = _mm_add_pd(src2Hi.d,imRes.d);

		mSum.d = _mm_add_pd(mSum.d,src2Hi.d);
	}
};



} // namespace OPT_LEVEL

#endif // __STATISTICS_NORM_H__

