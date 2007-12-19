/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include "SSE2Intrinsic.h"


namespace OPT_LEVEL
{

//
// Pixel Functions
//
namespace
{
	//Phase DEV functions
	SYS_INLINE STATIC void Phase(const A64FC *s, A64F *d)
	{		
		*d = atan2((s->im),(s->re));		
	}

	SYS_INLINE STATIC void Phase(const A32FC *s, A32F *d)
	{		
		*d = atan2f((s->im),(s->re));		
	}	

	SYS_INLINE STATIC void Phase(const A16SC *s, A32F *d)
	{		
		*d = atan2f(A32F(s->im),A32F(s->re));		
	}

	SYS_INLINE STATIC void Phase(const A16SC *s, A16S *d,int scale)
	{		
		*d = FW_REF::Limits<Fw16s>::Sat( FW_REF::Scale( atan2f((A32F)(s->im),(A32F)(s->re)), scale ));
	}
	SYS_INLINE STATIC void Phase(const A32SC *s, A32S *d,int scale)
	{		
		*d = FW_REF::Limits<Fw32s>::Sat( FW_REF::Scale( atan2f(A32F(s->im),A32F(s->re)), scale ));
	}
	SYS_INLINE STATIC void Phase(const A64F *s1, const A64F *s2, A64F *d)
	{		
		*d = atan2(*s2,*s1);	
	}

	SYS_INLINE STATIC void Phase(const A32F *s1, const A32F *s2, A32F *d)
	{		
		*d = atan2f(*s2,*s1);
	}
	SYS_INLINE STATIC void Phase(const A16S *s1, const A16S *s2, A32F *d)
	{		
		*d = atan2f(A32F(*s2),A32F(*s1));
	}
	SYS_INLINE STATIC void Phase(const A16S *s1, const A16S *s2, A16S *d,int scale)
	{		
		*d = FW_REF::Limits<Fw16s>::Sat( FW_REF::Scale( atan2f(A32F(*s2),A32F(*s1)), scale ));
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void CopyS(const TS * s,TD * d)
	{
		*d = *s;
	}


	SYS_INLINE STATIC void copy_128si_sse2_S(const __m128i &src,__m128i &dest)
	{
		dest = src;
	}


	template<class TD >
	SYS_INLINE STATIC void SetS(TD * d,TD value)
	{
		*d = value;
	}

	template<  class TD >
	SYS_INLINE STATIC void ZeroS(TD * d)
	{
		memset((void*) d, 0, sizeof(TD));
	}

	SYS_INLINE STATIC void set_128si_sse2_S(const __m128i &val,__m128i &dest)
	{
		dest = val;
	}

	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuei,Fw8u &value )
	{
		mValuei.i = _mm_set1_epi8(value);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuei,Fw16s &value )
	{
		mValuei.i = _mm_set1_epi16(value);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuei,Fw32s &value )
	{
		mValuei.i = _mm_set1_epi32(value);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuef,Fw32f &value )
	{
		mValuef.f = _mm_set1_ps(value);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuef,Fw32fc &value )
	{
		mValuef.f = _mm_setr_ps(value.im, value.re, value.im, value.re);
	}



	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuei,Fw16sc &val )
	{
		mValuei.i = _mm_setr_epi16(val.im, val.re, val.im, val.re, val.im, val.re, val.im, val.re);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValuei,Fw32sc &val )
	{
		mValuei.i = _mm_setr_epi32(val.im, val.re, val.im, val.re);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValued,Fw64f &val )
	{
		mValued.d = _mm_set1_pd(val);
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValued,Fw64s &val )
	{
		mValued.s64[0] = val;
		mValued.s64[1] = val;
	}


	SYS_INLINE STATIC void set_setup_S (XMM128 &mValued,Fw64sc &val )
	{
		mValued.s64[0] = val.im;
		mValued.s64[1] = val.re;
	}


	SYS_INLINE STATIC void set_setup_S(XMM128 &mValued,Fw64fc &val )
	{
		
		mValued.f64[0] = val.im;
		mValued.f64[1] = val.re;
	}


	SYS_INLINE STATIC void zero_si128_S(__m128i &srcDest)
	{
		srcDest = _mm_setzero_si128();
	}

	// Convert reference functions
	template< class TS, class TD >
	SYS_INLINE STATIC void Convert(const TS *s,  TD * d )
	{
		*d = FW_REF::Limits<TD>::Sat(*s);
	}

	SYS_INLINE STATIC void Convert32f64f(const Fw32f *s,  Fw64f * d )
	{
		*d = (Fw64f) (*s);
	}


	SYS_INLINE STATIC void Convert16u32f(const Fw16u *s,  Fw32f * d )
	{
		*d =(Fw32f)(*s);
	}

	SYS_INLINE STATIC void Convert64f32f( const Fw64f *s,  Fw32f * d )
	{
		Fw64f val = MIN(*s,3.4028235e+38);
			    *d = (Fw32f) MAX(val,-3.4028235e+38);	
	}



	SYS_INLINE STATIC void ConvertScale16s32f(const Fw16s *s,  Fw32f * d, int scale)
	{
		*d = (Fw32f)*s;
		*d = FW_REF::Scale(*d,  scale);	
	}

	SYS_INLINE STATIC void ConvertScale16s64f(const Fw16s *s,  Fw64f * d, int scale)
	{

		*d = (Fw64f)*s;
		*d = FW_REF::Scale(*d, scale);	
	}


	
	SYS_INLINE STATIC void ConvertScale32s64f(const Fw32s *s,  Fw64f * d, int scale)
	{
		*d = (Fw64f)*s;
		*d = FW_REF::Scale(*d,scale);	
	}


	SYS_INLINE STATIC void ConvertScale32s16s(const Fw32s *s,  Fw16s * d, int scale)
	{
		*d = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale((float)*s,scale)));
	}


	SYS_INLINE STATIC void ConvertScale32s16s_noscale(const Fw32s *s,  Fw16s * d )
	{
		*d = FW_REF::Limits<S16>::Sat(*s);
	}

	SYS_INLINE STATIC void ConvertScale32s32f(const Fw32s *s,  Fw32f * d, int scale)
	{
		*d = (Fw32f)(*s);
		*d = FW_REF::Scale(*d,scale);
	}
	

	SYS_INLINE STATIC void ConvertScale32f8u( const Fw32f *s,  Fw8u * d, int scale)
	{
		*d = FW_REF::Limits<U8>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(*s,scale)));
	}

	SYS_INLINE STATIC void ConvertScaleRndZero32f8u(const Fw32f *s,  Fw8u * d, int scale)
	{
		*d = FW_REF::Limits<U8>::Sat(FW_REF::Scale(*s,scale));
	}

	

	SYS_INLINE STATIC void ConvertScale32f8s(const Fw32f *s,  Fw8s * d,  int scale)
	{
		*d = FW_REF::Limits<S8>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(*s,scale)));
	}


	SYS_INLINE STATIC void ConvertScaleRndZero32f8s(const Fw32f *s,  Fw8s * d, int scale)
	{
		*d = FW_REF::Limits<S8>::Sat(FW_REF::Scale(*s,scale));
	}

	SYS_INLINE STATIC void ConvertScale32f16s( const Fw32f *s,  Fw16s * d, int scale)
	{
		*d = FW_REF::Limits<S16>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(*s,scale)));
	}

	SYS_INLINE STATIC void ConvertScaleRndZero32f16s( const Fw32f *s,  Fw16s * d, int scale)
	{
		*d = FW_REF::Limits<S16>::Sat(FW_REF::Scale(*s,scale));
	}

	SYS_INLINE STATIC void ConvertScale32f16u( const Fw32f *s,  Fw16u * d,  int scale)
	{
		*d = FW_REF::Limits<U16>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(*s,scale)));
	}

	SYS_INLINE STATIC void ConvertScaleRndZero32f16u(  const Fw32f *s,  Fw16u * d,  int scale)
	{
		*d = FW_REF::Limits<U16>::Sat(FW_REF::Scale(*s,scale));
	}

	SYS_INLINE STATIC void ConvertScale32f32s(const Fw32f *s,  Fw32s * d, int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale((double)*s,scale)));
	}


	SYS_INLINE STATIC void ConvertScaleRndZero32f32s( const Fw32f *s,  Fw32s * d, int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(FW_REF::Scale((double)*s,scale));
	}


	
	SYS_INLINE STATIC void ConvertScale64s32s(const Fw64s *s,  Fw32s * d, int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale((double)*s,scale)));
	}

	SYS_INLINE STATIC void ConvertScaleRndZero64s32s( const Fw64s *s,  Fw32s * d,  int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(FW_REF::Scale((double)*s,scale));
	}

	SYS_INLINE STATIC void ConvertScale64f32s( const Fw64f *s,  Fw32s * d , int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale(*s,scale)));
	}

	SYS_INLINE STATIC void ConvertScaleRndZero64f32s( const Fw64f *s,  Fw32s * d,  int scale)
	{
		*d = FW_REF::Limits<S32>::Sat(FW_REF::Scale(*s,scale));
	}


	// Convert sse2 functions
	SYS_INLINE void sConvert_8s16s_sse2_S(const __m128i &src,__m128i &dest)
	{
		__m128i mask = _mm_cmplt_epi8(src, _mm_setzero_si128());
		dest = _mm_unpacklo_epi8(src, mask);
	}

	SYS_INLINE void sConvert_8s32f_sse2_S(const __m128i &src,__m128 &dest )
	{
		__m128i mask = _mm_cmplt_epi8(src, _mm_setzero_si128());
		__m128i desti= _mm_unpacklo_epi8(src, mask);	// unpack to 16 bit (sign extend)
		mask = 	_mm_unpacklo_epi8(mask,mask);

		desti = _mm_unpacklo_epi16(desti,mask);	// unpack to 32 bit (sign extend)
		dest = _mm_cvtepi32_ps(desti);
	}
	SYS_INLINE void sConvert_8u32f_sse2_S(const __m128i &src,__m128 &dest )
	{
		__m128i zero = _mm_setzero_si128();

		__m128i desti = _mm_unpacklo_epi8(src, zero);	// unpack to 16 bit (don't sign extend)
		desti = _mm_unpacklo_epi16(desti, zero);	// unpack to 32 bit

		dest = _mm_cvtepi32_ps(desti);
	}
	SYS_INLINE void sConvert_16s32s_sse2_S(const __m128i &src,__m128i &dest )
	{
		const __m128i zero = _mm_setzero_si128();
		__m128i signExt = _mm_cmplt_epi16(src,zero);
		dest = _mm_unpacklo_epi16(src,signExt);	// unpack to 32 bit (sign extend)
	}
	SYS_INLINE void sConvert_16s32f_sse2_S(const __m128i &src,__m128 &dest )
	{
		const __m128i zero = _mm_setzero_si128();
		__m128i signExt = _mm_cmplt_epi16(src,zero);
		__m128i dst  = _mm_unpacklo_epi16(src,signExt);	// unpack to 32 bit (sign extend)
		dest = _mm_cvtepi32_ps(dst);
	}
	SYS_INLINE void sConvert_16u32f_sse2_S(const __m128i &src,__m128 &dest )
	{
		__m128i dst = _mm_unpacklo_epi16(src,  _mm_setzero_si128());	// unpack to 32 bit (sign extend)
		dest = _mm_cvtepi32_ps(dst);
	}
	SYS_INLINE STATIC void sConvert_32s16s_sse2_S(const __m128i &src,__m128i &dest )
	{
		dest = _mm_packs_epi32(src, src);	// Upper half will be ignored
	}
	SYS_INLINE STATIC void sConvert_32s32f_sse2_S(const __m128i &src,__m128 &dest )
	{
		dest = _mm_cvtepi32_ps(src);
	}
	SYS_INLINE STATIC void sConvert_32s64f_sse2_S(const __m128i &src,__m128d &dest )
	{
		dest = _mm_cvtepi32_pd(src);
	}
	SYS_INLINE STATIC void sConvert_32f64f_sse2_S(const __m128 &src,__m128d &dest )
	{
		dest = _mm_cvtps_pd(src);
	}


	SYS_INLINE STATIC void sConvert_64f32f_setup_S(__m128d &imin,__m128d &imax)
	{
		imax  = _mm_set1_pd((float) CBL_F32_MAX); 
		imin  = _mm_set1_pd((float) CBL_F32_MIN);
	}

	SYS_INLINE STATIC void sConvert_16s32f_Sfs_setup_S(__m128 &scalei,const int &scale)
	{
		scalei = _mm_set1_ps(pow((float)2.0,-scale));
	}

	SYS_INLINE STATIC void sConvert_64f32f_sse2_S(const __m128d &src,__m128 &dest,const __m128d &fmin,const __m128d &fmax )
	{
		__m128d vald = _mm_min_pd(src,fmax);
				vald = _mm_max_pd(vald,fmin);
		dest = _mm_cvtpd_ps(vald);
	}


	SYS_INLINE void sConvert_16s32f_Sfs_sse2_S(const __m128i &src,__m128 &dest,const __m128 &scale )
	{
		__m128i signExt = _mm_srai_epi16(src,15);
		__m128i dst  = _mm_unpacklo_epi16(src,signExt);	// unpack to 32 bit (sign extend)
		__m128 dstf = _mm_cvtepi32_ps(dst);
		dest = _mm_mul_ps(dstf,scale);	// scale it
	}


	SYS_INLINE STATIC void sConvert_16s64f_Sfs_setup_S(__m128d &iscale,const int &scale)
	{
		iscale = _mm_set1_pd(pow((double)2.0,-scale));
	}



	SYS_INLINE void sConvert_16s64f_Sfs_sse2_S(const __m128i &src,__m128d &dest,const __m128d &scale )
	{
		__m128i signExt = _mm_srai_epi16(src,15);
		__m128i dst  = _mm_unpacklo_epi16(src,signExt);	// unpack to 32 bit (sign extend)
		__m128d dstd = _mm_cvtepi32_pd(dst);
		dest = _mm_mul_pd(dstd, scale);	// scale it
	}



SYS_INLINE STATIC void sConvert_32s16s_Sfs_sse2_setup(__m128 &iscale,int scale,__m128 &imin,__m128 &imax)
{
	iscale = _mm_set1_ps(pow((float)2.0,-scale));

	imax = _mm_cvtepi32_ps(_mm_set1_epi32(FW_REF::Limits<S16>::MaxValue())); // max int
	imin = _mm_cvtepi32_ps(_mm_set1_epi32(FW_REF::Limits<S16>::MinValue())); // min int
}

	SYS_INLINE STATIC void sConvert_32s16s_Sfs_sse2_S(const __m128i &src,__m128i &dest,const __m128 &scale,const  __m128 &imin,const __m128 &imax)
	{

		__m128 dst = _mm_cvtepi32_ps(src);
		dst = _mm_mul_ps(dst,scale);	// scale it
		dst = _mm_min_ps(dst,imax);
		dst = _mm_max_ps(dst,imin);
		__m128i dsti = _mm_cvtps_epi32(dst);
		dest = _mm_packs_epi32(dsti,dsti);	// upper half will be ignored
	}


	SYS_INLINE STATIC void sConvert_32s16s_Sfs_noscale_sse2_S(const __m128i &src,__m128i &dest)
	{
		dest = _mm_packs_epi32(src,src);	// upper half will be ignored
	}


	SYS_INLINE STATIC void sConvert_32s32f_Sfs_setup_S(__m128 &iscale,const int &scale)
	{
		iscale = _mm_set1_ps( pow((float)2.0,-scale));
	}

	SYS_INLINE STATIC void sConvert_32s32f_Sfs_sse2_S(const __m128i &src,__m128 &dest,const __m128 &scale)
	{
		__m128 destf= _mm_cvtepi32_ps(src);
		dest = _mm_mul_ps(destf, scale);	// scale it
	}


	SYS_INLINE STATIC void sConvert_32s64f_Sfs_setup_S(__m128d &iscale,const int &scale)
	{
		iscale = _mm_set1_pd( pow((double)2.0,-scale));
	}

	SYS_INLINE STATIC void sConvert_32s64f_Sfs_sse2_S(const __m128i &src,__m128d &dest,const __m128d &scale)
	{
		__m128d destd = _mm_cvtepi32_pd(src);
		dest = _mm_mul_pd(destd,scale);	// scale it
	}


	SYS_INLINE STATIC void sConvert_32f8s_Sfs_setup_S(__m128 &fscale,int scale,__m128 &min,__m128 &max)
	{
		fscale= _mm_set1_ps(pow((float)2.0,-scale));
		max  = _mm_set1_ps((float) CBL_S8_MAX);
		min  = _mm_set1_ps((float) CBL_S8_MIN);
	}


	SYS_INLINE void sConvert_32f8s_Sfs_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src, scale);	// scale

		// saturate
		destf = _mm_min_ps(destf,max);
		destf = _mm_max_ps(destf,min);

		__m128i desti = _mm_cvtps_epi32(destf);					// convert to 32s
		desti = _mm_packs_epi32(desti, desti);		// pack down into 16s (upper bits will be ignored)
		dest = _mm_packs_epi16(desti, desti);		// pack down into 8s (upper bits will be ignored)
	}
	
	SYS_INLINE void sConvert_32f8s_Sfs_rndzero_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src, scale);	// scale

		// saturate
		destf = _mm_min_ps(destf,max);
		destf = _mm_max_ps(destf,min);

		__m128i desti = _mm_cvttps_epi32(destf);					// convert to 32s
		desti = _mm_packs_epi32(desti, desti);		// pack down into 16s (upper bits will be ignored)
		dest = _mm_packs_epi16(desti, desti);		// pack down into 8s (upper bits will be ignored)	}
	}

	SYS_INLINE STATIC void sConvert_32f8u_Sfs_setup_S(__m128 &scalef,int scale,__m128 &min,__m128 &max)
	{
		scalef = _mm_set1_ps(pow((float)2.0,-scale));
		max  = _mm_set1_ps((float) 255);
		min  = _mm_set1_ps((float) 0);
	}

	SYS_INLINE void sConvert_32f8u_Sfs_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src, scale);	// scale

		// saturate
		destf = _mm_min_ps(destf, max);
		destf = _mm_max_ps(destf, min);

		__m128i desti = _mm_cvtps_epi32(destf);					// convert to 32s

		desti = _mm_packs_epi32(desti,desti);				// value does not exceed 255 hence pasks 32 can be used
		dest = _mm_packus_epi16(desti,desti);		// pack down into 8s (upper bits will be ignored)
	}
	
	SYS_INLINE void sConvert_32f8u_Sfs_rndzero_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src, scale);	// scale

		// saturate
		destf = _mm_min_ps(destf,max);
		destf = _mm_max_ps(destf,min);

		__m128i desti = _mm_cvttps_epi32(destf);					// convert to 32s

		desti = _mm_packs_epi32(desti,desti);				// value does not exceed 255 hence pasks 32 can be used
		dest = _mm_packus_epi16(desti,desti);		// pack down into 8s (upper bits will be ignored)
	}


	SYS_INLINE STATIC void sConvert_32f16s_Sfs_setup_S(__m128 &scalei,int scale,__m128 &min,__m128 &max)
	{
		scalei = _mm_set1_ps(pow((float)2.0,-scale));
		max  = _mm_set1_ps((float) CBL_S16_MAX);
		min = _mm_set1_ps((float) CBL_S16_MIN);
	}

	SYS_INLINE STATIC void sConvert_32f16u_Sfs_setup_S(__m128 &scalei,const int scale,__m128 &min,__m128 &max)
	{
		scalei = _mm_set1_ps(pow((float)2.0,-scale));
		max  = _mm_set1_ps((float) CBL_U16_MAX);
		min  = _mm_set1_ps((float)CBL_U16_MIN);
	}

	SYS_INLINE void sConvert_32f16s_Sfs_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src, scale);	// scale

		//saturate
		destf = _mm_min_ps(destf, max);
		destf = _mm_max_ps(destf, min);

		__m128i desti = _mm_cvtps_epi32(destf);					// convert to 32s
		dest = _mm_packs_epi32(desti,desti);		// pack down into 16s (upper bits will be ignored)
	}

	SYS_INLINE void sConvert_32f16s_Sfs_rndzero_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src,scale);	// scale

		//saturate
		destf = _mm_min_ps(destf, max);
		destf = _mm_max_ps(destf, min);

		__m128i desti = _mm_cvttps_epi32(destf);					// convert to 32s
		dest = _mm_packs_epi32(desti,desti);		// pack down into 16s (upper bits will be ignored)
	}

	SYS_INLINE void sConvert_32f16u_Sfs_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src,scale);	// scale

		// saturate
		destf = _mm_min_ps(destf, max);
		destf = _mm_max_ps(destf, min);

		__m128i desti = _mm_cvtps_epi32(destf);					// convert to 32s

		desti = _mm_shufflelo_epi16(desti, _MM_SHUFFLE(3, 2, 2, 0));
		desti = _mm_shufflehi_epi16(desti, _MM_SHUFFLE(3, 2, 2, 0));
		dest = _mm_shuffle_epi32(desti, _MM_SHUFFLE(3, 2, 2, 0));
	}
	
	SYS_INLINE void sConvert_32f16u_Sfs_rndzero_sse2_S(const __m128 &src,__m128i &dest,const __m128 &scale,const __m128 &min,const __m128 &max)
	{
		__m128 destf = _mm_mul_ps(src,scale);	// scale

		// saturate
		destf = _mm_min_ps(destf, max);
		destf = _mm_max_ps(destf, min);

		__m128i desti = _mm_cvttps_epi32(destf);					// convert to 32s

		desti = _mm_shufflelo_epi16(desti, _MM_SHUFFLE(3, 2, 2, 0));
		desti = _mm_shufflehi_epi16(desti, _MM_SHUFFLE(3, 2, 2, 0));
		dest = _mm_shuffle_epi32(desti, _MM_SHUFFLE(3, 2, 2, 0));
	}


	SYS_INLINE STATIC void sConvert_32f32s_Sfs_setup_S(__m128d &scalei,const int scale,__m128d &min,__m128d &max)
	{
		scalei = _mm_set1_pd( pow((double)2.0,-scale));

		max = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MAX)); // max int
		min = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MIN)); // min int
	}

	SYS_INLINE STATIC void sConvert_32f32s_Sfs_sse2_S(const __m128 &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{

		// open up floats into two double xmm's
		XMM128 srcLo, srcHi;
		srcLo.d = _mm_cvtps_pd(src);
		srcLo.d = _mm_mul_pd(srcLo.d,scale);

		srcHi.f = src;
		srcHi.i = _mm_srli_si128(srcHi.i, 8);
		srcHi.d = _mm_cvtps_pd(srcHi.f);
		
		srcHi.d = _mm_mul_pd(srcHi.d,scale);

		srcLo.d = _mm_min_pd(srcLo.d, max);
		srcHi.d = _mm_min_pd(srcHi.d, max);

		srcLo.d = _mm_max_pd(srcLo.d, min);
		srcHi.d = _mm_max_pd(srcHi.d, min);

		srcLo.i = _mm_cvtpd_epi32(srcLo.d);						// get the 32s values
		srcHi.i = _mm_cvtpd_epi32(srcHi.d);						// truncated, not rounded

		srcHi.i = _mm_slli_si128(srcHi.i, 8);

		dest = _mm_or_si128(srcHi.i, srcLo.i);
	}
	
	SYS_INLINE STATIC void sConvert_32f32s_Sfs_rndzero_sse2_S(const __m128 &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{

		// open up floats into two double xmm's
		XMM128 srcLo, srcHi;
		srcLo.d = _mm_cvtps_pd(src);
		srcLo.d = _mm_mul_pd(srcLo.d,scale);
		srcHi.f = src;
		srcHi.i = _mm_srli_si128(srcHi.i, 8);
		srcHi.d = _mm_cvtps_pd(srcHi.f);
		
		srcHi.d = _mm_mul_pd(srcHi.d,scale);

		srcLo.d = _mm_min_pd(srcLo.d, max);
		srcHi.d = _mm_min_pd(srcHi.d, max);

		srcLo.d = _mm_max_pd(srcLo.d, min);
		srcHi.d = _mm_max_pd(srcHi.d, min);

		srcLo.i = _mm_cvttpd_epi32(srcLo.d);						// get the 32s values
		srcHi.i = _mm_cvttpd_epi32(srcHi.d);						// truncated, not rounded

		srcHi.i = _mm_slli_si128(srcHi.i, 8);

		dest = _mm_or_si128(srcHi.i, srcLo.i);
	}


	SYS_INLINE STATIC void sConvert_64f32s_Sfs_setup_S(__m128d &scalei,const int scale,__m128d &min,__m128d &max)
	{
		scalei = _mm_set1_pd( pow((double)2.0,-scale));

		max = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MAX)); // max int
		min = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MIN)); // min int
	}

	SYS_INLINE STATIC void sConvert_64f32s_Sfs_sse2_S(const __m128d &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{
		__m128d val = _mm_mul_pd(src, scale);	// scale

		// saturate
		val = _mm_min_pd(val, max);		// take min of num and max int
		val = _mm_max_pd(val,min);		// take max of num and min int

		dest = _mm_cvtpd_epi32(val);					// convert to 32s
	}
	
	SYS_INLINE STATIC void sConvert_64f32s_Sfs_rndzero_sse2_S(const __m128d &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{
		__m128d val = _mm_mul_pd(src,scale);	// scale

		// saturate
		val = _mm_min_pd(val, max);		// take min of num and max int
		val = _mm_max_pd(val, min);		// take max of num and min int

		dest = _mm_cvttpd_epi32(val);			
	}

	SYS_INLINE STATIC void sConvert_64s32s_Sfs_setup_S(__m128d &scalei,const int scale,__m128d &min,__m128d &max)
	{
		scalei = _mm_set1_pd( pow((double)2.0,-scale));

		max = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MAX)); // max int
		min = _mm_cvtepi32_pd(_mm_set1_epi32(CBL_S32_MIN)); // min int
	}


	SYS_INLINE STATIC void sConvert_64s32s_Sfs_sse2_S(const __m128i &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{
		Fw64s *input = (Fw64s*) &src;      
		__m128d val = _mm_set_pd((double) input[1], (double) input[0]); 

		val= _mm_mul_pd(val, scale);	// scale

		// saturate
		val = _mm_min_pd(val, max);		// take min of num and max int
		val = _mm_max_pd(val, min);		// take max of num and min int

		dest = _mm_cvtpd_epi32(val);					// we're gauranteed to have a value which'll fit in 32s
	}
	
	SYS_INLINE STATIC void sConvert_64s32s_Sfs_rndzero_sse2_S(const __m128i &src,__m128i &dest,const __m128d &scale,const __m128d &min,const __m128d &max)
	{
		Fw64s *input = (Fw64s*) &src;      
		__m128d val  = _mm_set_pd((double) input[1], (double) input[0]); 

		val = _mm_mul_pd(val, scale);	// scale

		// saturate
		val = _mm_min_pd(val, max);		// take min of num and max int
		val = _mm_max_pd(val, min);		// take max of num and min int

		dest = _mm_cvttpd_epi32(val);					// we're gauranteed to have a value which'll fit in 32s
	}
};

} // namespace OPT_LEVEL

#endif // __CONVERSION_H__


