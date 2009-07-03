/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __NORMALIZE_H__
#define __NORMALIZE_H__

#include "fe.h"

namespace OPT_LEVEL
{
	//Normalize
	template< class TS, class TD >
	SYS_INLINE STATIC void NormalizeFS(const TS * s,TD * d,const TS &vsub,const TS &vdiv)
	{
		*d = (TD)((*s - vsub)/vdiv);
	}

	SYS_INLINE STATIC void NormalizeFS(const Fw32fc * s,Fw32fc * d,const Fw32fc &vsub,const Fw32f &vdiv)
	{
		*d = (Fw32fc)(*s - vsub);
		d[0].re  = d[0].re/vdiv;
		d[0].im  = d[0].im/vdiv;
	}

	SYS_INLINE STATIC void NormalizeFS(const Fw64fc * s,Fw64fc * d,const Fw64fc &vsub,const Fw64f &vdiv)
	{
		*d = (Fw64fc)(*s - vsub);
		d[0].re = d[0].re / vdiv;
		d[0].im = d[0].im / vdiv;
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void NormalizeIS(const TS * s,TD * d,const TS &vsub,const int &vdiv,const int &scale)
	{
		*d = FW_REF::Limits<TD>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(((Fw32f)*s - (Fw32f)vsub)/(Fw32f)vdiv, scale)));
	}

	SYS_INLINE STATIC void NormalizeIS(const Fw16sc * s,Fw16sc * d,const Fw16sc &vsub,const int &vdiv,const int &scale)
	{
		Fw32sc temp;
		Fw32f scl;
		temp.re 	= ((Fw32s)s->re - (Fw32s)vsub.re);
		temp.im 	= ((Fw32s)s->im - (Fw32s)vsub.im);

		scl = FW_REF::Scale(1.0F,scale);
		scl = scl / (Fw32f)vdiv;	

		d[0].re = FW_REF::Limits<Fw16s>::Sat((S32)(temp.re * scl));
		d[0].im = FW_REF::Limits<Fw16s>::Sat((S32)(temp.im * scl));
	}

	//Normalize sse2 setup functions

	SYS_INLINE STATIC void Normalize_32f_setup_sse2(__m128 &mVsub,__m128 &mVdiv,const Fw32f vsub,const Fw32f vdiv)
	{
		Fw32f over_vdiv = 1/vdiv;
		mVsub = _mm_set1_ps(vsub);	
		mVdiv = _mm_set1_ps(over_vdiv);	
	}
	SYS_INLINE STATIC void Normalize_64f_setup_sse2(__m128d &mVsub,__m128d &mVdiv,const Fw64f vsub,const Fw64f vdiv)
	{
		Fw64f over_vdiv = 1/vdiv;
		mVsub = _mm_set1_pd(vsub);	
		mVdiv = _mm_set1_pd(over_vdiv);	
	}
	SYS_INLINE STATIC void Normalize_32fc_setup_sse2(__m128 &mVsub,__m128 &mVdiv,const Fw32fc vsub,const Fw32f vdiv)
	{
		
		Fw32f over_vdiv = 1/vdiv;
		mVsub = _mm_set_ps(vsub.im,vsub.re,vsub.im,vsub.re);	
		mVdiv = _mm_set1_ps(over_vdiv);
	}
	SYS_INLINE STATIC void Normalize_64fc_setup_sse2(__m128d &mVsub,__m128d &mVdiv,const Fw64fc vsub,const Fw64f vdiv)
	{
		Fw64f over_vdiv = 1/vdiv;
		mVsub = _mm_set_pd(vsub.im,vsub.re);	
		mVdiv = _mm_set1_pd(over_vdiv);
	}

	SYS_INLINE STATIC void Normalize_16s_setup_sse2(__m128i &mVsub,__m128 &mVdiv,const Fw16s vsub,const int vdiv, const int scaleFactor)
	{

		Fw32f scale = pow((float)2.0,-scaleFactor);

		Fw32f scale_vdiv = scale/(Fw32f)vdiv;
		mVsub = _mm_set1_epi32(vsub);	
		mVdiv = _mm_set1_ps(scale_vdiv);	
	}

	SYS_INLINE STATIC void Normalize_16sc_setup_sse2(__m128i &mVsub,__m128 &mVdiv,const Fw16sc vsub,const int vdiv, const int scaleFactor)
	{
		Fw32f scale = pow((float)2.0,-scaleFactor);

		Fw32f scale_vdiv = scale/(Fw32f)vdiv;
		mVsub = _mm_set_epi32((S32)vsub.im,(S32)vsub.re,(S32)vsub.im,(S32)vsub.re);	
		mVdiv = _mm_set1_ps(scale_vdiv);	
	}


	SYS_INLINE STATIC __m128 Normalize_sse2_32f_c1(const __m128 &src,const __m128 &vsub,const __m128 &vdiv)
	{	
		__m128 dst = _mm_sub_ps(src,vsub);
		return  _mm_mul_ps(dst,vdiv);
	}

	SYS_INLINE STATIC __m128d Normalize_sse2_64f_c1(const __m128d &src,const __m128d &vsub,const __m128d &vdiv)
	{	
		__m128d dst = _mm_sub_pd(src,vsub);
		return  _mm_mul_pd(dst,vdiv);
	}

	SYS_INLINE STATIC __m128i Normalize_sse2_16s_c1(const __m128i &src,const __m128i &vsub,const __m128 &vdiv)
	{	
		XMM128 temp_lo,temp_hi;
		temp_lo.i = src;
		CBL_SSE2::Unpack16STo32S(temp_lo.i,temp_hi.i);
		temp_lo.i = _mm_sub_epi32(temp_lo.i,vsub); 
		temp_hi.i = _mm_sub_epi32(temp_hi.i,vsub); 

		temp_lo.f = _mm_cvtepi32_ps(temp_lo.i);
		temp_hi.f = _mm_cvtepi32_ps(temp_hi.i);
		temp_lo.f = _mm_mul_ps(temp_lo.f,vdiv);
		temp_hi.f = _mm_mul_ps(temp_hi.f,vdiv);
		temp_lo.i = _mm_cvttps_epi32(temp_lo.f); 
		temp_hi.i = _mm_cvttps_epi32(temp_hi.f); 
		return  _mm_packs_epi32(temp_lo.i,temp_hi.i);		
	}
} // namespace OPT_LEVEL

#endif	// __NORMALIZE_H__
