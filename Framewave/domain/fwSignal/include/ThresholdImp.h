/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __THRESHOLD_SIG_IMP__
#define __THRESHOLD_SIG_IMP__ 

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"

namespace OPT_LEVEL
{

namespace MAGREF
{
	SYS_INLINE STATIC void Magnitude32f(const Fw32f * s1,const Fw32f * s2,Fw32f * d)
	{
		Fw64f val  = ((Fw64f)*s1 * *s1) + ((Fw64f)*s2 * *s2);	
		val = sqrt(val);		// Calculate the magnitude of complex number
		*d = (Fw32f)val;		
	}

	SYS_INLINE STATIC void Magnitude64f(const Fw64f * s1,const Fw64f * s2, Fw64f * d)
	{
		Fw64f val  = (*s1 * *s1) + (*s2 * *s2);

		if(!SYS_FINITE(val))			// if a2 + b2 is inf set the result to inf else compute sqrt(a2+b2)
		{
			d[0] = Const::IND_64F;
			
		}
		else
		{
			d[0] =sqrt(val);
		}
	}

	SYS_INLINE STATIC void Magnitude32fc(const Fw32fc *s, Fw32f *d)
	{

		Fw64f val  = ((Fw64f)s->re * s->re) + ((Fw64f)s->im * s->im);
		val = sqrt(val);
		*d = (Fw32f) val;

	}


	SYS_INLINE STATIC void Magnitude64fc(const Fw64fc *s, Fw64f *d)
	{

		Fw64f val  = (s->re * s->re) + (s->im * s->im);

		if(!SYS_FINITE(val)) // if a2 + b2 is inf set the result to inf else compute sqrt(a2+b2)
		{
			d[0] = Const::IND_64F;
			
		}
		else
		{
			d[0] =sqrt(val);
		}		
	}

	SYS_INLINE STATIC void Magnitude16s(const Fw16s * s1,const Fw16s * s2,Fw16s * d,int scale)
	{
		d[0] = FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt(((float) *s1 * (float) *s1) + ((float) *s2 * (float) *s2)), scale)));
	}

	SYS_INLINE STATIC void Magnitude16sc16s(const Fw16sc * s,Fw16s * d,int scale)
	{
		d[0] = FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt(((float) s->re * (float) s->re) + ((float) s->im * (float) s->im)), scale)));
	}

	SYS_INLINE STATIC void Magnitude32sc32s(const Fw32sc * s,Fw32s * d,int scale)
	{
		d[0] = FW_REF::Limits<Fw32s>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale(sqrt(((double) s->re * (double) s->re) + ((double) s->im * (double) s->im)), scale)));
	}

	SYS_INLINE STATIC void Magnitude16s32f(const Fw16s * s1, const Fw16s * s2, Fw32f * d)
	{
		d[0] = sqrt(((float) *s1 * (float) *s1) + ((float) *s2 * (float) *s2));
	}

	SYS_INLINE STATIC void Magnitude16sc32f(const Fw16sc * s, Fw32f * d)
	{
		d[0] = sqrt(((float) s->re * (float) s->re) + ((float) s->im * (float) s->im));
	}

}

namespace MAG
{
	namespace SSE2_SETUP
	{
		SYS_INLINE STATIC void Magnitude_32f_setup_S( __m128 &inf, __m128 &ind)
		{
			inf = _mm_set1_ps(Const::INF_32F);	// Set inf
			ind = _mm_set1_ps(Const::IND_32F);	// set ind
		}

		SYS_INLINE STATIC void Magnitude_64f_setup_S( __m128d &inf, __m128d &ind)
		{
			inf = _mm_set1_pd(Const::INF_64F);	// set inf
			ind = _mm_set1_pd(Const::IND_64F);	// set ind
		}

		SYS_INLINE STATIC void Magnitude_16sc32f_setup_S(__m128i &mNmax)
		{
			mNmax = _mm_set1_epi16(-32768); // level
		}

		SYS_INLINE STATIC void Magnitude_16s_setup_S(__m128 &mScale,const int &scale)
		{
			Fw32f scalef = pow((Fw32f)2.0,-scale);	// calculate scale
			CBL_SSE2::Load_1x4_32f(mScale,scalef);		// set scale
		}

		SYS_INLINE STATIC void Magnitude_32s_setup_S(__m128d &mScale,const int &scale)
		{
			Fw64f scalef = pow((Fw64f)2.0,-scale);	// calculate scale
			CBL_SSE2::Load_1x2_64f(mScale,scalef);		// set scale
		}
	}

	SYS_INLINE STATIC __m128 Magnitude_32f_sse2_S(const __m128 &re,const  __m128 &im)
	{

        __m128 re2 = _mm_mul_ps(re,re);
        __m128 im2  = _mm_mul_ps(im,im);
        re2 = _mm_add_ps(re2,im2);
        re2 = _mm_sqrt_ps(re2);
        return re2;

	}

	SYS_INLINE STATIC __m128d Magnitude_64f_sse2_S(const __m128d &re,const  __m128d &im,const __m128d &inf,const __m128d &ind)
	{
		__m128d  re2 = _mm_mul_pd(re, re);	//a2
		__m128d  im2 = _mm_mul_pd(im, im);	//b2
		re2 = _mm_add_pd(re2, im2);			// a2 + b2
		__m128d mask = _mm_cmpeq_pd(re2,inf);			// if a2 + b2 = Inf then set the result to ind else compute sqrt(a2+b2)
		__m128d inter_ind = _mm_and_pd(mask,ind);
		re2  = _mm_sqrt_pd(re2);	
		re2 = _mm_andnot_pd(mask,re2);
		return  _mm_or_pd(re2,inter_ind);
	}

	SYS_INLINE STATIC __m128 Magnitude_32fc_sse2_S(const __m128 &src)
	{

        __m128 mulval =  _mm_mul_ps(src, src);
        __m128 addval = _mm_shuffle_ps(mulval,mulval,_MM_SHUFFLE(3,3,1,1));
         addval    = _mm_add_ps(addval, mulval); 
         addval = _mm_sqrt_ps(addval);
        return _mm_shuffle_ps(addval,addval,_MM_SHUFFLE(3,2,2,0));

	}

	SYS_INLINE STATIC __m128d Magnitude_64fc_sse2_S(const __m128d &src, const __m128d &inf,const __m128d &ind)
	{
		__m128d  val = _mm_mul_pd(src, src);	
	
		XMM128 val1;
		val1.d = val;
		val1.f  = CBL_OPTLEVEL::Shuffle_a3a2b3b2_32f(val1.f,val1.f);
		val = _mm_add_sd(val, val1.d);
		__m128d mask = _mm_cmpeq_sd(val,inf);		// if re2 + im2  is inf then set the result to ind else compute sqrt(re2 + im2)
		__m128d inter_ind = _mm_and_pd(mask,ind);
		val  = _mm_sqrt_sd(val,val);
		val = _mm_andnot_pd(mask,val);
		return _mm_or_pd(val,inter_ind);
	}

	SYS_INLINE STATIC __m128 Magnitude_16s32f_sse2_S(const __m128i &src1,const __m128i &src2)
	{
		__m128i srcLo = _mm_unpacklo_epi16(src1,src2);
		
		__m128i minVal = _mm_cmpeq_epi32(srcLo,srcLo);
		minVal = _mm_slli_epi16(minVal,15);	
		__m128i val = _mm_sub_epi16(srcLo, _mm_cmpeq_epi16(srcLo, minVal)); // sub -1 if src = minval, otherwise the result will overflow

		__m128i mag = _mm_madd_epi16(val,val);
		__m128 magf = _mm_cvtepi32_ps(mag);
		return  _mm_sqrt_ps(magf);
	}

	SYS_INLINE STATIC __m128 Magnitude_16sc32f_sse2_S(const __m128i &src,const __m128i &mNmax)
	{
		__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, mNmax));

		__m128i mag = _mm_madd_epi16(val,val);
		__m128 magf = _mm_cvtepi32_ps(mag);
				return _mm_sqrt_ps(magf);
	}

	SYS_INLINE STATIC __m128i Magnitude_16s_sse2_S(const __m128i &src1,const __m128i &src2,const __m128 &scale)
	{

		XMM128 ret1, rethi1, ret2, rethi2, temp;

		// sqr (all elements) and place in two regs in 32 bit format
		ret1.i			= _mm_mullo_epi16(src1, src1);
		temp.i			= _mm_mulhi_epi16(src1, src1);
					
		rethi1.i		= _mm_unpackhi_epi16(ret1.i, temp.i);	// we need to use temp to not overwrite rethi
		ret1.i			= _mm_unpacklo_epi16(ret1.i, temp.i);
					
		ret1.f			= _mm_cvtepi32_ps(ret1.i);				// convert to float BEFORE the add since that can overflow 32f
		rethi1.f		= _mm_cvtepi32_ps(rethi1.i);
					
		// sqr (all elements) and place in two regs in 32 bit format
		ret2.i			= _mm_mullo_epi16(src2,src2);
		temp.i			= _mm_mulhi_epi16(src2, src2);

		rethi2.i		= _mm_unpackhi_epi16(ret2.i, temp.i);	// we need to use temp to not overwrite rethi
		ret2.i			= _mm_unpacklo_epi16(ret2.i, temp.i);

		ret2.f			= _mm_cvtepi32_ps(ret2.i);				// convert to float BEFORE the add since that can overflow 32f
		rethi2.f		= _mm_cvtepi32_ps(rethi2.i);

		ret1.f			= _mm_add_ps(ret1.f, ret2.f);
		rethi1.f		= _mm_add_ps(rethi1.f, rethi2.f);

		ret1.f			= _mm_sqrt_ps(ret1.f);										// root
		rethi1.f		= _mm_sqrt_ps(rethi1.f);									// root

		ret1.f			= _mm_mul_ps(ret1.f, scale);					// scale
		rethi1.f		= _mm_mul_ps(rethi1.f, scale);					// scale

		temp.i	= _mm_cvtps_epi32(ret1.f);									// convert back
		rethi1.i		= _mm_cvtps_epi32(rethi1.f);								// convert back

		
		FW_SSE2::pack32STo16S(temp.i, rethi1.i);		// repack to 16 bit
       return temp.i;																					// we can give a junk reg for the upper 4 values
	}

	SYS_INLINE STATIC __m128i Magnitude_16sc_sse2_S(const __m128i &src,const __m128 &scale)
	{
		__m128i minVal = _mm_cmpeq_epi32(src,src);
		minVal = _mm_slli_epi16(minVal,15);
		__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, minVal));	//	sub min most values with -1 to avoid overflow.

		__m128i mag = _mm_madd_epi16(val,val);
		__m128 magf = _mm_cvtepi32_ps(mag);
		       magf = _mm_sqrt_ps(magf); 
			   magf = _mm_mul_ps(magf,scale);
			   mag = _mm_cvtps_epi32(magf);	
		FW_SSE2::pack32STo16S(mag,mag);	
				return mag;
	}

	SYS_INLINE STATIC __m128i  Magnitude_32sc_sse2_S(const __m128i &src,const __m128d &scale)
	{
		XMM128 tempSrcLo;
		tempSrcLo.i= src; 
		XMM128 tempSrcHi, alla, allb;

		// Unpack the 32 bit signed integers to double compute the magnitude, pack and saturate the results to 32 bit.
		CBL_SSE2::Unpack32STo64F(tempSrcLo.i, tempSrcLo.d, tempSrcHi.d);
		XMM128 destHi;

		tempSrcLo.d		= _mm_mul_pd(tempSrcLo.d, tempSrcLo.d);
		tempSrcHi.d		= _mm_mul_pd(tempSrcHi.d, tempSrcHi.d);

		alla.d			= _mm_shuffle_pd(tempSrcLo.d, tempSrcHi.d, _MM_SHUFFLE2(0, 0));
		allb.d			= _mm_shuffle_pd(tempSrcLo.d, tempSrcHi.d, _MM_SHUFFLE2(1, 1));

		alla.d	= _mm_add_pd(alla.d, allb.d);
		alla.d	= _mm_sqrt_pd(alla.d);
		alla.d	= _mm_mul_pd(alla.d, scale);	// scale it
		
		// repack the destination
		alla.i = CBL_SSE2::Pack64FTo32S(alla.d, destHi.d);

		return alla.i;
	}



}
namespace THREF
{
	//Reference implementation
	template<class TS,class TD >
	SYS_INLINE STATIC void ThresholdLTVal(const TS *s,TD *d,TD level,TD value)
	{
		if (*s < level)
			*d = value;
		else
			*d = *s;
	}
	template<class TD >
	SYS_INLINE STATIC void ThresholdLTVal( TD * d, TD level,TD value)
	{
		if (*d < level)
			*d = value;
	}


	template< class TS, class TD >
	SYS_INLINE STATIC void CopyS(const TS * s , TD * d )
	{
		*d = *s;
	}

	SYS_INLINE STATIC void ThresholdLTVal16sc( const Fw16sc *s,Fw16sc *d,Fw16s level,Fw16sc value)
	{

		Fw32f mag = ((float)s->re * s->re) + ((float)s->im * s->im);
		Fw32f inter_leve = (float)level * level;
		if (mag < inter_leve)
		{
			d[0] = value;
		}
		else
		{
			d[0] = s[0];
		}
	}

	SYS_INLINE STATIC void ThresholdLTVal16sc( Fw16sc *d,Fw16s level,Fw16sc value)
	{
		Fw32f mag = ((float)d->re * d->re) + ((float)d->im * d->im);
		Fw32f inter_leve = (float) level * level;
		if (mag < inter_leve)
		{
			d[0] = value;
		}
	}


	SYS_INLINE STATIC void ThresholdLTVal32fc( const Fw32fc *s, Fw32fc * d,Fw32f level,Fw32fc value)
	{
		Fw32f mag = sqrt((s->re * s->re) + (s->im * s->im));

		if (mag < level)
		{
			*d = value;
		}
		else
		{
			*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdLTVal32fc( Fw32fc * d,Fw32f level,Fw32fc value)
	{
		Fw32f mag = sqrt((d->re * d->re) + (d->im * d->im));

		if (mag < level)
			*d = value;
	}

	SYS_INLINE STATIC void ThresholdLTVal64fc( const Fw64fc *s, Fw64fc * d,Fw64f level,Fw64fc value)
	{
		Fw64f mag = sqrt((s->re * s->re) + (s->im * s->im));

		if (mag < level)
			*d = value;
		else
			*d = *s;
	}

	SYS_INLINE STATIC void ThresholdLTVal64fc( Fw64fc * d,Fw64f level,Fw64fc value)
	{
		Fw64f mag = sqrt((d->re * d->re) + (d->im * d->im));

		if (mag < level)
			*d = value;
	}

	template<class TD >
	SYS_INLINE STATIC void ThresholdGTVal(  TD * d, TD level,TD value)
	{
		if (*d > level)
			*d = value;
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void ThresholdGTVal(const TS *s,TD *d,TD level,TD value)
	{
		if (*s > level)
			*d = value;
		else
			*d = *s;
	}

	SYS_INLINE STATIC void ThresholdGTVal16sc(const Fw16sc *s, Fw16sc * d,Fw16s level,Fw16sc value )
	{

		Fw32f mag = ((float)s->re * s->re) + ((float)s->im * s->im);
		Fw32f inter_level = (float)level * level;

		if (mag  > inter_level)
		{
			d[0] = value;
		}
		else
			d[0] = s[0];
	}

	SYS_INLINE STATIC void ThresholdGTVal16sc(Fw16sc * d,Fw16s level,Fw16sc value )
	{

		Fw32f mag = ((float)d->re * d->re) + ((float)d->im * d->im);
		Fw32f inter_level = (float)level * level;

		if (mag  > inter_level)
		{
			d[0] = value;
		}
	}

	SYS_INLINE STATIC void ThresholdGTVal32fc(const Fw32fc *s,Fw32fc * d,Fw32f level,Fw32fc value )
	{
		Fw32f sqr = (s->re * s->re) + (s->im * s->im);

		if ( sqr == Const::INF_32F)
		{
			*d = value;
		}
		else
		{
			Fw32f mag = sqrt(sqr);

			if (mag >  level)
				*d = value;
			else
				*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdGTVal32fc( Fw32fc * d,Fw32f level,Fw32fc value )
	{
		Fw32f sqr = (d->re * d->re) + (d->im * d->im);

		if ( sqr == Const::INF_32F)	// if re2 + im2  equals inf then set destination to  value
		{
			*d = value;
		}
		else
		{

			Fw32f mag = sqrt(sqr);		// magnitude

			if (mag > (double)level)	// if magnitude > level then set destination to value 
				*d = value;
		}
	}

	SYS_INLINE STATIC void ThresholdGTVal64fc(const Fw64fc *s,Fw64fc * d,Fw64f level,Fw64fc value )
	{
		Fw64f sqr = (s->re * s->re) + (s->im * s->im);
		if(!SYS_FINITE(sqr))
		{
			*d = value;
		}
		else
		{

			Fw64f mag = sqrt(sqr);

			if (mag > level)
				*d = value;
			else
				*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdGTVal64fc(Fw64fc * d,Fw64f level,Fw64fc value )
	{

		Fw64f sqr = (d->re * d->re) + (d->im * d->im);


		if(!SYS_FINITE(sqr))	// if sqr is not finite then set destination to value
		{
			*d = value;
		}
		else
		{

			Fw64f mag = sqrt(sqr);	// magnitude
			if (mag > level)
				*d = value;
		}
	}


	template<class TD >
	SYS_INLINE STATIC void ThresholdLT( TD * d,const TD level)
	{
		*d = (*d < level) ? level : *d;
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void ThresholdLT(const TS *s, TD * d,const TD level)
	{
		*d = (*s < level) ? level : *s;
	}


	SYS_INLINE STATIC void ThresholdLT32fc(Fw32fc * d,const Fw32f level)
	{
		Fw64f mag;
		mag = sqrt(((Fw64f)d->re * d->re) + ( (Fw64f)d->im * d->im));
		if (mag < (Fw64f)level)
		{
			d->re = (Fw32f)(((Fw64f)d->re * level) / mag);
			d->im = (Fw32f)(((Fw64f)d->im * level) / mag);
		}
	}

	SYS_INLINE STATIC void ThresholdLT32fc(const Fw32fc *s,Fw32fc * d,const Fw32f level )
	{
		Fw64f mag;
		mag = sqrt(((Fw64f)s->re * s->re) + ( (Fw64f)s->im * s->im));
		if(mag < (Fw64f) level)
		{
			d->re = (Fw32f)(((Fw64f)s->re * level) / mag);
			d->im = (Fw32f)(((Fw64f)s->im * level) / mag);
		}
		else
		{
			*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdLT64fc( const Fw64fc *s,Fw64fc * d,const Fw64f level )
	{
		Fw64f mag;
		mag = sqrt( (s->re * s->re) + (s->im * s->im) );

		if( mag < level)
		{
			d->re  = (s->re * level)/mag;
			d->im  = (s->im * level)/mag;
		}
		else
		{
			*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdLT64fc(Fw64fc * d,const  Fw64f level)
	{
		Fw64f mag;
		mag = sqrt( (d->re * d->re) + (d->im * d->im) );

		if( mag < level)
		{
			d->re  = (d->re * level)/mag;
			d->im  = (d->im * level)/mag;
		}
	}

	SYS_INLINE STATIC void ThresholdLT16sc( Fw16sc * d,const Fw16s level )
	{

		Fw32f mag;
		Fw32fc src;
		
		src.re = (Fw32f) d->re;
		src.im = (Fw32f) d->im;

		Fw32f sqr = (src.re * src.re  )+ (src.im * src.im );
		if(0 == sqr)
		{
			d->re = (Fw16s)level;
			d->im = 0;
			return;
		}

		mag = sqrt(sqr);

		src = src * (MAX(mag,(Fw32f)level) / mag);
		d->re = (Fw16s) ceil(src.re);
		d->im = (Fw16s) ceil(src.im);

	}

	SYS_INLINE STATIC void ThresholdLT16sc( const Fw16sc *s,Fw16sc * d,const Fw16s level )
	{

		Fw32f mag;
		Fw32fc src;
		
		src.re = (Fw32f) s->re;
		src.im = (Fw32f) s->im;

		Fw32f sqr = (src.re * src.re  )+ (src.im * src.im );
		if(0 == sqr)		
		{
			d->re = (Fw16s)level;
			d->im = 0;
			return;
		}
		
		mag = sqrt(sqr);

		src = src * (MAX(mag,(Fw32f)level) / mag);
		d->re = (Fw16s) ceil(src.re);
		d->im = (Fw16s) ceil(src.im);

	}

	template< class TS, class TD >
	SYS_INLINE STATIC void ThresholdGT(const TS *s,TD * d,TS level)
	{
		*d = (*s > level) ? level : *s;
	}


	template<class TD >
	SYS_INLINE STATIC void ThresholdGT( TD * d,TD level)
	{
		*d = (*d > level) ? level : *d;
	}



	SYS_INLINE STATIC void ThresholdGT32fc( const Fw32fc *s,Fw32fc  * d, Fw32f level)
	{

		Fw64f mag = ((Fw64f)s->re * s->re) + ((Fw64f)s->im * s->im);

		Fw32f mag1 = FW_REF::Limits<F32>::Sat(sqrt(mag));

		if(mag1 < level)
		{
			*d = *s;
		}
		else
		{
			d->re = s->re * level;
			d->im = s->im * level;
			d->re= d->re/mag1;
			d->im= d->im/mag1;
		}
	}

	SYS_INLINE STATIC void ThresholdGT32fc(Fw32fc  * d, Fw32f level)
	{

		Fw64f mag = ((Fw64f)d->re * d->re) + ((Fw64f)d->im * d->im);

		Fw32f mag1 = FW_REF::Limits<F32>::Sat(sqrt(mag));

		if(mag1 < level)
		{
			return;
		}
		else
		{
			d->re = d->re * level;
			d->im = d->im * level;
			d->re= d->re/mag1;
			d->im= d->im/mag1;
		}
	}


	SYS_INLINE STATIC void ThresholdGT64fc( const Fw64fc *s,Fw64fc  * d, Fw64f level)
	{

		Fw64f mag;
		mag = sqrt( (s->re * s->re) + (s->im * s->im) );

		if( mag > level)
		{
			d->re  = (s->re * level)/mag;
			d->im  = (s->im * level)/mag;
		}
		else
		{
			*d = *s;
		}
	}

	SYS_INLINE STATIC void ThresholdGT64fc(Fw64fc  * d, Fw64f level)
	{
		Fw64f mag;
		mag = sqrt( (d->re * d->re) + (d->im * d->im) );

		if( mag > level)
		{
			d->re  = (d->re * level)/mag;
			d->im  = (d->im * level)/mag;
		}
	}

	SYS_INLINE STATIC void ThresholdGT16sc(const Fw16sc *s,Fw16sc * d,Fw16s level )
	{

		Fw32f mag;
		Fw32fc src;
		
		src.re = (Fw32f) s->re;
		src.im = (Fw32f) s->im;

		mag = sqrt(((float)s->re * s->re )+ ((float)s->im * s->im ));
		
		if(mag > level )
		{
			src = (src * (Fw32f)level)/mag;
			d->re = (Fw16s) src.re;
			d->im = (Fw16s) src.im;
		}
		else
		{
			*d = *s;
		}
	}


	SYS_INLINE STATIC void ThresholdGT16sc( Fw16sc * d,Fw16s level )
	{

		Fw32f mag;
		Fw32fc src;
		
		src.re = (Fw32f) d->re;
		src.im = (Fw32f) d->im;

		mag = sqrt(((float)d->re * d->re )+ ((float)d->im * d->im ));
		
		if(mag > level )
		{
			src = (src * (Fw32f)level)/mag;
			d->re = (Fw16s) src.re;
			d->im = (Fw16s) src.im;
		}
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void ThresholdLTGT( const  TS *s,TD * d,TS levelLT,TS valueLT, TS levelGT, TS valueGT)
	{

		if (*s < levelLT)
			*d = valueLT;
		else if (*s > levelGT)
				*d = valueGT;
		else
			*d = *s;
	}

	template< class TD>
	SYS_INLINE STATIC void ThresholdLTGT(TD * d,TD levelLT,TD valueLT, TD levelGT, TD valueGT)
	{


		if (*d < levelLT)
			*d = valueLT;
		else if (*d > levelGT)
				*d = valueGT;
	}

	template<class TD >
	SYS_INLINE STATIC void Threshold_LTInv( TD * d,TD level )
	{
		TD absSrc = fabs(*d);

		if (absSrc == 0) 
		{
			*d = 1/level;
		}
		else
		{
			TD minval = MIN(absSrc,level);
			minval = minval * (1 / level);
			*d = minval * (1 / *d);
		}
	}

	template< class TS, class TD >
	SYS_INLINE STATIC void Threshold_LTInv( const  TS *s,TD * d,TS level)
	{
		TS absSrc = fabs(*s);

		if (absSrc == 0) 
		{
			*d = 1/level;
		}
		else
		{
			TD minval = MIN(absSrc,level);
			minval = minval * (1 / level);
			*d = minval * (1 / *s);
		}
	}

	SYS_INLINE STATIC void Threshold_LTInv32fLZero(const Fw32f *s,Fw32f * d)
	{
		
		if (*s == 0)	// if src = 0 then set the result to INF else compute 1/src
		{
			*d = Const::INF_32F;
		}
		else
		{
			*d = (1 / *s);
		}
	}

	SYS_INLINE STATIC void Threshold_LTInv32fLZero( Fw32f * d)
	{
		
		if (*d == 0) 
		{
			*d = Const::INF_32F;
		}
		else
		{
			*d = (1 / *d);
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv32fcLZero(const Fw32fc *s,Fw32fc * d)
	{
			
		Fw32f a2b2 = (s->re * s->re) + (s->im * s->im);
		

		if (s->re == 0 && s->im == 0) 
		{
			d->re = Const::INF_32F;
			d->im = Const::INF_32F;
		}
		else
		{


			d->re = (s->re/a2b2);
			d->im = -(s->im/a2b2);
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv32fcLZero(  Fw32fc * d)
	{
	
		Fw32f a2b2 = (d->re * d->re) + (d->im * d->im);
	
		if (d->re == 0 && d->im == 0) 
		{
			d->re = Const::INF_32F;
			d->im = Const::INF_32F;
		}
		else
		{
			d->re = (d->re/a2b2);
			d->im = -(d->im/a2b2);
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv64fLZero(const Fw64f *s,Fw64f * d)
{
		
		if (*s == 0) 
		{
			*d = Const::INF_64F;
		}
		else
		{
			*d = (1 / *s);
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv64fLZero(Fw64f * d)
	{
		
		if (*d == 0) 
		{
			*d = Const::INF_64F;
		}
		else
		{
			*d = (1 / *d);
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv64fcLZero(const Fw64fc *s,Fw64fc * d)
	{
		
		if (s->re == 0 && s->im == 0) 
		{
			d->re = Const::INF_64F;
			d->im = Const::INF_64F;
		}
		else
		{
			Fw64f a2b2 = (s->re * s->re) + (s->im * s->im);
			d->re = (s->re/a2b2);
			d->im = -(s->im/a2b2);
		}
	}

	SYS_INLINE STATIC void Threshold_LTInv64fcLZero(Fw64fc * d)
	{
		
		if (d->re == 0 && d->im == 0) 
		{
			d->re = Const::INF_64F;
			d->im = Const::INF_64F;
		}
		else
		{
			Fw64f a2b2 = (d->re * d->re) + (d->im * d->im);
			d->re =(d->re/a2b2);
			d->im =-(d->im/a2b2);
		}
	}

	SYS_INLINE STATIC void Threshold_LTInv32fc(const Fw32fc *s,Fw32fc * d,Fw32f level)
	{

		Fw32f a2b2 = (s->re * s->re) + (s->im * s->im);

		Fw32f absSrc = sqrt(a2b2);

		if (absSrc == 0) // if abs of src = 0 then set result to 1/level
		{
			d->re = 1/level;
			d->im = 1/level;
		}
		else
		{
			if ( absSrc < level)
			{
				 Fw32fc den = *s  * level;
				 a2b2 = (den.re * den.re) + (den.im * den.im);
				 den.re = den.re / a2b2;
				 den.im = -den.im / a2b2;
				 

				d->re = absSrc *  den.re ;
				d->im = absSrc *  den.im ;
			}
			else
			{
				
				d->re = s->re/a2b2 ;
				d->im = -s->im/a2b2 ;
			}
		}
	}


	SYS_INLINE STATIC void Threshold_LTInv32fc(Fw32fc * d,Fw32f level)
	{
		Fw32f a2b2 = (d->re * d->re) + (d->im * d->im);
		
		Fw32f absSrc = sqrt(a2b2);

		if (absSrc == 0) 
		{
			d->re = 1/level;
			d->im = 1/level;
		}
		else
		{
			if ( absSrc < level)
			{
				 Fw32fc den = *d  * level;
				 a2b2 = (den.re * den.re) + (den.im * den.im);
				 den.re = den.re / a2b2;
				 den.im = -den.im / a2b2;
				 

				d->re = absSrc *  den.re ;
				d->im = absSrc *  den.im ;
			}
			else
			{
				
				d->re = d->re/a2b2 ;
				d->im = -d->im/a2b2 ;
			}
		}
	}



	SYS_INLINE STATIC void Threshold_LTInv64fc( const Fw64fc *s, Fw64fc *d,Fw64f level)
	{
		Fw64f a2b2 = (s->re * s->re) + (s->im * s->im);

		Fw64f absSrc = sqrt(a2b2);


		if (absSrc == 0) 
		{
			d->re = 1/level;
			d->im = 1/level;
		}
		else
		{
			if ( absSrc < level)
			{
				 Fw64fc den = *s  * level;
				 a2b2 = (den.re * den.re) + (den.im * den.im);
				 den.re = den.re / a2b2;
				 den.im = -den.im / a2b2;
				 

				d->re = absSrc *  den.re ;
				d->im = absSrc *  den.im ;
			}
			else
			{
				
				d->re = s->re/a2b2 ;
				d->im = -s->im/a2b2 ;
			}
		}
	}

	SYS_INLINE STATIC void Threshold_LTInv64fc(Fw64fc *d,Fw64f level)
	{

		Fw64f a2b2 = (d->re * d->re) + (d->im * d->im);
		
		Fw64f absSrc = sqrt(a2b2);

		if (absSrc == 0) 
		{
			d->re = 1/level;
			d->im = 1/level;
		}
		else
		{
			if ( absSrc < level)
			{
				 Fw64fc den = *d  * level;
				 a2b2 = (den.re * den.re) + (den.im * den.im);
				 den.re = den.re / a2b2;
				 den.im = -den.im / a2b2;
				 

				d->re = absSrc *  den.re ;
				d->im = absSrc *  den.im ;
			}
			else
			{
				
				d->re = d->re/a2b2 ;
				d->im = -d->im/a2b2 ;
			}
		}
	}

} // namespace THREF



namespace GENF // generic functions
{
	SYS_INLINE STATIC __m128 a2b2_sum_32fc(const __m128 &src)
	{
		__m128 mag = _mm_mul_ps(src,src);
		__m128 temp= _mm_shuffle_ps(mag, mag, _MM_SHUFFLE(2, 3, 0, 1));
		return _mm_add_ps(mag, temp);						
	}

	SYS_INLINE STATIC __m128d a2b2_sum_64fc(const __m128d &src)
	{
		__m128d mag	= _mm_mul_pd(src, src);
		__m128d temp= _mm_unpackhi_pd(mag, mag);
		temp =  _mm_add_sd(temp,mag);	
		return _mm_unpacklo_pd(temp,temp);
	}

	SYS_INLINE STATIC __m128 Thresh_mm_mag_ps(const __m128 &src)
	{
		__m128 ret, temp;
		ret		= _mm_mul_ps(src, src);
		temp	= _mm_shuffle_ps(ret, ret, _MM_SHUFFLE(2, 3, 0, 1));
		ret		= _mm_add_ps(ret, temp);							// sqr(a) + sqr(b);
		ret		= _mm_sqrt_ps(ret);

		return	  ret;
	}

	SYS_INLINE STATIC void	Unpack16STo32F(const __m128i &src,__m128 &srcHi,__m128 &srcLo)
	{
		__m128i src1=src,srchi;
		CBL_SSE2::Unpack16STo32S(src1,srchi);
		srcLo = _mm_cvtepi32_ps(src1);
		srcHi = _mm_cvtepi32_ps(srchi);
	}

	SYS_INLINE STATIC __m128d Thresh_mm_mag_pd(const __m128d &src)
	{
		__m128d ret, temp;
		ret		= _mm_mul_pd(src, src);
		temp		= _mm_unpackhi_pd(ret, ret);
		ret		= _mm_add_sd(ret, temp);							// sqr(a) + sqr(b);
		ret		= _mm_sqrt_sd(ret, ret);

		return		  _mm_unpacklo_pd(ret,ret);
	}

	SYS_INLINE STATIC __m128d Thresh_mm_mag_pd(const __m128d &src,__m128d &sqr)
	{
		__m128d ret, temp;
		ret		= _mm_mul_pd(src, src);
		temp		= _mm_unpackhi_pd(ret, ret);
		sqr		= _mm_add_sd(ret, temp);							// sqr(a) + sqr(b);
		ret		= _mm_sqrt_sd(sqr, sqr);

		return		  _mm_unpacklo_pd(ret,ret);
	}

	SYS_INLINE STATIC __m128 Thresh_mm_rsqrmag_ps(const __m128 &src,__m128 &sqr)
	{
		__m128 ret, temp;
		ret		= _mm_mul_ps(src, src);
		temp	= _mm_shuffle_ps(ret, ret, _MM_SHUFFLE(2, 3, 0, 1));
		sqr		= _mm_add_ps(ret, temp);							// sqr(a) + sqr(b);
		ret		= _mm_rsqrt_ps(sqr);
		return	  ret;
	}
} // namespace GENF



namespace THRES	// Threshold LT GT 
{
	namespace SSE2_SETUP	
	{
		SYS_INLINE STATIC void sThreshold_16s_setup_S(__m128i &Leveli, const Fw16s &level)
		{
			Leveli = _mm_set1_epi16(level); // level
		}

		SYS_INLINE STATIC void sThreshold_32s_setup_S(__m128i &Leveli, const Fw32s &level)
		{
			Leveli = _mm_set1_epi32(level); // level
		}

		SYS_INLINE STATIC void sThreshold_32f_setup_S(__m128 &Levelf, const Fw32f &level)
		{
			Levelf = _mm_set1_ps(level); // level
		}

		SYS_INLINE STATIC void sThreshold_32fc_setup_S(__m128d &Levelf, const Fw32f &level)
		{

			Levelf = _mm_set1_pd((Fw64f)level); // level
		}

		SYS_INLINE STATIC void sThreshold_64f_setup_S(__m128d &Leveld, const Fw64f &level)
		{
			Leveld = _mm_set1_pd(level); // level
		}

		SYS_INLINE STATIC void sThreshold_64fc_setup_S(__m128d &Leveld, const Fw64f &level)
		{
			Leveld = _mm_set1_pd(level); // level
		}

		SYS_INLINE STATIC void sThreshold_16sc_setup_S(__m128 &Levelf, const Fw16s &level)
		{
			Fw32f temp = (Fw32f) level;
			Levelf = _mm_set1_ps(temp); // level
		}
		SYS_INLINE STATIC void sThresholdGT_32fc_setup_S(__m128d &Levelf,__m128 &Indf,__m128 &Inff,const Fw32f &level)
		{
			Levelf = _mm_set1_pd((Fw64f)level); // level
			Inff = _mm_set1_ps(Const::INF_32F);	 // inf
			Indf = _mm_set1_ps(Const::IND_32F);	 // ind 
		}
	} // namespace SSE2_SETUP


	namespace SSE2_LT
	{

		SYS_INLINE STATIC __m128i sThresholdLT_16s_sse2_S(const __m128i &src,const __m128i &level)
		{
			return _mm_max_epi16(src,level);
		}
		SYS_INLINE STATIC __m128i sThresholdLT_32s_sse2_S(const __m128i &src, const __m128i &level)
		{
			__m128i mask = _mm_cmplt_epi32(src, level);	
			__m128i src1 = _mm_andnot_si128(mask,src);	
			mask = _mm_and_si128(mask, level);			
			return _mm_or_si128(src1, mask);			
		}

		SYS_INLINE STATIC __m128 sThresholdLT_32f_sse2_S(const __m128 &src, const __m128 &level)
		{
			return  _mm_max_ps(src, level);
		}

		SYS_INLINE STATIC __m128d  sThresholdLT_64f_sse2_S(const __m128d &src, const __m128d &level)
		{
			return  _mm_max_pd(src, level);
		}

		SYS_INLINE STATIC __m128 sThresholdLT_32fc_sse2_S(const __m128 &src,const __m128d &level)
		{
			XMM128 dest1,dest2; 
			XMM128 src1,src2,mask1,mask2,mag1,mag2;

			src1.d = _mm_cvtps_pd(src);
			src2.f = src;
			src2.i = _mm_srli_si128(src2.i,8);
			src2.d = _mm_cvtps_pd(src2.f);
			
			mag1.d = GENF::Thresh_mm_mag_pd(src1.d);
			mag2.d = GENF::Thresh_mm_mag_pd(src2.d);
			
			mask1.d   = _mm_cmpge_pd(mag1.d, level);
			if(0x3 == _mm_movemask_pd(mask1.d))
			{
				dest1.f = src;
			}
			else
			{
					dest1.d = _mm_mul_pd(src1.d,level);
					dest1.d = _mm_div_pd(dest1.d,mag1.d);
					dest1.f = _mm_cvtpd_ps(dest1.d);
			}
			mask2.d   = _mm_cmpge_pd(mag2.d, level);
			if(0x3 == _mm_movemask_pd(mask2.d))
			{
				dest2.f = src;
				dest2.i = _mm_srli_si128(dest2.i,8);
			}
			else
			{
					dest2.d = _mm_mul_pd(src2.d,level);
					dest2.d = _mm_div_pd(dest2.d,mag2.d);
					dest2.f = _mm_cvtpd_ps(dest2.d);
			}

			return CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f(dest1.f,dest2.f);
		}

		SYS_INLINE STATIC __m128d sThresholdLT_64fc_sse2_S(const __m128d &src,const __m128d &level)
		{
			XMM128 dest,mag;
			__m128d mask; 
			mag.d = GENF::Thresh_mm_mag_pd(src);
			mask	 = _mm_cmpge_pd(mag.d, level);				// if (mag < level)
			if (0x3 == _mm_movemask_pd(mask))
			{
				return src;
			}
		
			dest.d = _mm_mul_pd(src,level );
			return _mm_div_pd(dest.d,mag.d);
		}

		SYS_INLINE STATIC __m128i sThresholdLT_16sc_sse2_S(const __m128i &src,const __m128 &level)
		{

			__m128i minVal = _mm_cmpeq_epi32(src,src);
			minVal = _mm_slli_epi16(minVal,15);
			__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, minVal));

			__m128i a2_sum_b2 = _mm_madd_epi16(val,val);
			__m128 sqr = _mm_cvtepi32_ps(a2_sum_b2);

			__m128 mag = _mm_sqrt_ps(sqr);

			__m128 mask   = _mm_cmpge_ps(mag, level);			// if (mag < level)

			if (0xF == _mm_movemask_ps(mask))
			{
				return src;
			}
			__m128 level1 = _mm_max_ps(mag,level);
			__m128 maglo   = _mm_unpacklo_ps(level1,level1);
			__m128 maghi  =  _mm_unpackhi_ps(level1,level1);

			__m128 mag1   = _mm_unpacklo_ps(mag,mag);
			__m128 mag2  =  _mm_unpackhi_ps(mag,mag); 

			maglo =  _mm_div_ps(maglo,mag1);
			maghi =  _mm_div_ps(maghi,mag2);

			__m128 srcLo;
			__m128 srchHi;

			GENF::Unpack16STo32F(src,srchHi,srcLo);

			srcLo  = _mm_mul_ps(srcLo,maglo);
			srchHi = _mm_mul_ps(srchHi,maghi);


			__m128i srciLo = _mm_cvtps_epi32(srcLo);
			__m128i srciHi = _mm_cvtps_epi32(srchHi);

			FW_SSE2::pack32STo16S(srciLo,srciHi);

			return srciLo;
		}

		namespace VAL
		{
			namespace SSE2_SETUP
			{
				SYS_INLINE STATIC void sThresholdLTVal_16s_setup_S(__m128i &Leveli,__m128i &Valuei,Fw16s &level,Fw16s &value)
				{
					Leveli  = _mm_set1_epi16(level); // level
					Valuei  = _mm_set1_epi16(value); // value
				}
				SYS_INLINE STATIC void sThresholdLTVal_32f_setup_S(__m128 &Levelf,__m128 &Valuef,Fw32f &level,Fw32f &value)
				{
					Levelf  = _mm_set1_ps(level); // level
					Valuef  = _mm_set1_ps(value); // value
				}
				SYS_INLINE STATIC void sThresholdLTVal_64f_setup_S(__m128d &Leveld,__m128d &Valued,Fw64f &level,Fw64f &value)
				{
					Leveld = _mm_set1_pd( level); // level
					Valued = _mm_set1_pd( value); // value
				}
				SYS_INLINE STATIC void sThresholdLTVal_16sc_setup_S(__m128i &Leveli,__m128i &Valuei,Fw16s level,Fw16sc value)
				{
					Fw32s level1 = (Fw32s)level*level;
					Leveli = _mm_set1_epi32(level1);
					CBL_SSE2::Load_2x4_16s(Valuei, value.re,value.im);
				}

				SYS_INLINE STATIC void sThresholdLTVal_32fc_setup_S(__m128 &Leveli,__m128 &Valuei,Fw32f level,Fw32fc value)
				{
					Leveli = _mm_set1_ps(level * level);
					CBL_SSE2::Load_2x2_32f(Valuei, value.re,value.im);
				}

				SYS_INLINE STATIC void sThresholdLTVal_64fc_setup_S(__m128d &Leveli,__m128d &Valuei,Fw64f level,Fw64fc value)
				{
					Leveli = _mm_set1_pd(level * level);
					CBL_SSE2::Load_2x1_64f(Valuei, value.re,value.im);
				}
			} // namespace SSE2_SETUP


				SYS_INLINE STATIC __m128i sThresholdLTVal_16s(const __m128i &src,const __m128i &level,const __m128i &value)
				{
					__m128i mask = _mm_cmplt_epi16(src, level);			
					__m128i src1 = _mm_andnot_si128(mask,src);			
							mask = _mm_and_si128(mask, value);			
							return _mm_or_si128(src1, mask);			
				}
				SYS_INLINE STATIC __m128 sThresholdLTVal_32f(const __m128 &src,const __m128 &level, const __m128 &value)
				{
					__m128 mask	 = _mm_cmplt_ps(src,level);		
					__m128 src1  = _mm_andnot_ps(mask,src);		
						   mask	 = _mm_and_ps(mask,value);		
					return _mm_or_ps(src1,mask);				
				}
				SYS_INLINE STATIC __m128d sThresholdLTVal_64f(const __m128d &src,const __m128d &level,const __m128d &value)
				{
					
					__m128d mask = _mm_cmplt_pd(src, level);		
					__m128d src1 = _mm_andnot_pd(mask, src);		
							mask = _mm_and_pd(mask, value);			
					return _mm_or_pd(src1, mask);					
				}

				SYS_INLINE STATIC __m128i sThresholdLTVal_16sc(const __m128i &src,const __m128i &level,const __m128i &value)
				{
					__m128i minVal = _mm_cmpeq_epi32(src,src);
					minVal = _mm_slli_epi16(minVal,15);
					__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, minVal));
					__m128i mag = _mm_madd_epi16(val,val);
					__m128i mask = _mm_cmplt_epi32(mag,level);
					__m128i src1  = _mm_andnot_si128(mask,src);		
						   mask	 = _mm_and_si128(mask,value);		
					return _mm_or_si128(src1,mask);					
				}

				SYS_INLINE STATIC __m128 sThresholdLTVal_32fc(const __m128 &src,const __m128 &level,const __m128 &value)
				{
					__m128 sum2 = GENF::a2b2_sum_32fc(src);
					__m128 mask = _mm_cmplt_ps(sum2,level);
					__m128 src1 = _mm_andnot_ps(mask, src);			
							mask = _mm_and_ps(mask, value);			
					return _mm_or_ps(src1, mask);					
				}

				SYS_INLINE STATIC __m128d sThresholdLTVal_64fc(const __m128d &src,const __m128d &level,const __m128d &value)
				{
					__m128d sum2 = GENF::a2b2_sum_64fc(src);
					__m128d mask = _mm_cmplt_pd(sum2,level);
					__m128d src1 = _mm_andnot_pd(mask, src);	
							mask = _mm_and_pd(mask, value);		
					return _mm_or_pd(src1, mask);				
				}
			} // namespace VAL
		} // namespace SSE2_LT
	namespace SSE2_GT
	{

		SYS_INLINE STATIC __m128i sThresholdGT_16s_sse2_S(const __m128i &src,const __m128i &level)
		{
			return _mm_min_epi16(src, level);
		}


		SYS_INLINE STATIC __m128i sThresholdGT_32s_sse2_S(const __m128i &src, const __m128i &level)
		{
			__m128i mask = _mm_cmpgt_epi32(src, level);	
			__m128i src1 = _mm_andnot_si128(mask,src);	
			mask = _mm_and_si128(mask, level);			
			return _mm_or_si128(src1, mask);			
		}

		SYS_INLINE STATIC __m128 sThresholdGT_32f_sse2_S(const __m128 &src, const __m128 &level)
		{
			return _mm_min_ps(src,level);
		}

		SYS_INLINE STATIC __m128d sThresholdGT_64f_sse2_S(const __m128d &src, const __m128d &level)
		{
			return _mm_min_pd(src,level);
		}

		SYS_INLINE STATIC __m128 sThresholdGT_32fc_sse2_S(const __m128 &src,const __m128d &level,const __m128 &, const __m128 &)
		{

			XMM128 dest1,dest2; 
			XMM128 src1,src2,mask1,mask2,mag1,mag2;

			src1.d = _mm_cvtps_pd(src);
			src2.f = src;
			src2.i = _mm_srli_si128(src2.i,8);
			src2.d = _mm_cvtps_pd(src2.f);
			
			mag1.d = GENF::Thresh_mm_mag_pd(src1.d);
			mag2.d = GENF::Thresh_mm_mag_pd(src2.d);
			
			mask1.d   = _mm_cmple_pd(mag1.d, level);
			if(0x3 == _mm_movemask_pd(mask1.d))
			{
				dest1.f = src;
			}
			else
			{
					dest1.d = _mm_mul_pd(src1.d,level);
					dest1.d = _mm_div_pd(dest1.d,mag1.d);
					dest1.f = _mm_cvtpd_ps(dest1.d);
			}
			mask2.d   = _mm_cmple_pd(mag2.d, level);
			if(0x3 == _mm_movemask_pd(mask2.d))
			{
				dest2.f = src;
				dest2.i = _mm_srli_si128(dest2.i,8);
			}
			else
			{
					dest2.d = _mm_mul_pd(src2.d,level);
					dest2.d = _mm_div_pd(dest2.d,mag2.d);
					dest2.f = _mm_cvtpd_ps(dest2.d);
			}

			return CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f(dest1.f,dest2.f);
		}

		SYS_INLINE STATIC __m128d sThresholdGT_64fc_sse2_S(const __m128d &src, const __m128d &level)
		{
			XMM128 dest,mag;
			__m128d mask; 

			mag.d = GENF::Thresh_mm_mag_pd(src);
			mask	 = _mm_cmple_pd(mag.d, level);				// if (mag < level)
			if (0x3 == _mm_movemask_pd(mask))
			{
				
				return src;
			}

			dest.d = _mm_mul_pd(src,level );
			return _mm_div_pd(dest.d,mag.d);
		}

		SYS_INLINE STATIC __m128i sThresholdGT_16sc_sse2_S(const __m128i &src,const __m128 &level)
		{
			__m128i minVal = _mm_cmpeq_epi32(src,src);
			minVal = _mm_slli_epi16(minVal,15);
			__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, minVal));

			__m128i a2_sum_b2 = _mm_madd_epi16(val,val);
			__m128 sqr = _mm_cvtepi32_ps(a2_sum_b2);

			__m128 mag = _mm_sqrt_ps(sqr);

			 __m128 mask   = _mm_cmple_ps(mag, level);			// if (mag < level)

			if (0xF == _mm_movemask_ps(mask))
			{
				return src;
			}
			__m128 level1 = _mm_min_ps(mag,level);
			__m128 maglo   = _mm_unpacklo_ps(level1,level1);
			__m128 maghi  =  _mm_unpackhi_ps(level1,level1);

			__m128 mag1   = _mm_unpacklo_ps(mag,mag);
			__m128 mag2  =  _mm_unpackhi_ps(mag,mag); 

			maglo =  _mm_div_ps(maglo,mag1);
			maghi =  _mm_div_ps(maghi,mag2);

			__m128 srcLo;
			__m128 srchHi;

			GENF::Unpack16STo32F(src,srchHi,srcLo);

			srcLo  = _mm_mul_ps(srcLo,maglo);
			srchHi = _mm_mul_ps(srchHi,maghi);

			__m128i srciLo = _mm_cvttps_epi32(srcLo);
			__m128i srciHi = _mm_cvttps_epi32(srchHi);

			FW_SSE2::pack32STo16S(srciLo,srciHi);

			return srciLo;
		}

		namespace VAL
		{
			SYS_INLINE STATIC __m128i sThresholdGTVal_16s(const __m128i &src,const __m128i &level,const __m128i &value)
			{
				__m128i mask = _mm_cmpgt_epi16(src, level);	
				__m128i src1 = _mm_andnot_si128(mask,src);	
				mask = _mm_and_si128(mask, value);			
				return _mm_or_si128(src1, mask);			
			}

			SYS_INLINE STATIC __m128 sThresholdGTVal_32f(const __m128 &src,const __m128 &level,const __m128 &value)
			{
				__m128 mask = _mm_cmpgt_ps(src, level);	
				__m128 src1 = _mm_andnot_ps(mask,src);	
				mask = _mm_and_ps(mask, value);			
				return _mm_or_ps(src1, mask);			
			}

			SYS_INLINE STATIC __m128d sThresholdGTVal_64f(const __m128d &src,const __m128d &level,const __m128d &value)
			{
				__m128d mask = _mm_cmpgt_pd(src, level);
				__m128d src1 = _mm_andnot_pd(mask,src);	
				mask = _mm_and_pd(mask, value);			
				return _mm_or_pd(src1, mask);			
			}

			SYS_INLINE STATIC __m128i sThresholdGTVal_16sc(const __m128i &src,const __m128i &level,const __m128i &value)
			{
				__m128i minVal = _mm_cmpeq_epi32(src,src);
				minVal = _mm_slli_epi16(minVal,15);
				__m128i val = _mm_sub_epi16(src, _mm_cmpeq_epi16(src, minVal));

				__m128i mag = _mm_madd_epi16(val,val);
				__m128i mask = _mm_cmpgt_epi32(mag,level);
				__m128i src1  = _mm_andnot_si128(mask,src);		
				mask	 = _mm_and_si128(mask,value);			
				return _mm_or_si128(src1,mask);					
			}

			SYS_INLINE STATIC __m128 sThresholdGTVal_32fc(const __m128 &src,const __m128 &level,const __m128 &value)
			{
				__m128 sum2 = GENF::a2b2_sum_32fc(src);
				__m128 mask = _mm_cmpgt_ps(sum2,level);
				__m128 src1 = _mm_andnot_ps(mask, src);	
				mask = _mm_and_ps(mask, value);			
				return _mm_or_ps(src1, mask);			
			}

			SYS_INLINE STATIC __m128d sThresholdGTVal_64fc(const __m128d &src,const __m128d &level,const __m128d &value)
			{
				__m128d sum2 = GENF::a2b2_sum_64fc(src);
				__m128d mask = _mm_cmpgt_pd(sum2,level);
				__m128d src1 = _mm_andnot_pd(mask, src);
				mask = _mm_and_pd(mask, value);			
				return _mm_or_pd(src1, mask);			
			}
		}
	} // SSE2_GT
	namespace SSE2_LTVALGTVAL
	{
		namespace SSE2_SETUP
		{

			SYS_INLINE STATIC void sThresholdLTValGTVal_16s_setup_S(__m128i &mLevel,__m128i &mValue,__m128i &mLevelGt,__m128i &mValueGt,			\
															  const Fw16s level,const Fw16s value,const  Fw16s levelGt,const Fw16s valueGt)
			{
				mLevel = _mm_set1_epi16(level ); // levelLT
				mValue = _mm_set1_epi16(value); // valueLT
				mLevelGt = _mm_set1_epi16(levelGt); // levelGT
				mValueGt = _mm_set1_epi16(valueGt); // valueGT
			}

			SYS_INLINE STATIC void sThresholdLTValGTVal_32s_setup_S(__m128i &mLevel,__m128i &mValue,__m128i &mLevelGt,__m128i &mValueGt,			\
															  const Fw32s level,const Fw32s value,const  Fw32s levelGt,const Fw32s valueGt)
			{
				mLevel = _mm_set1_epi32 ( level); // levelLT
				mValue = _mm_set1_epi32 ( value); // valueLT
				mLevelGt= _mm_set1_epi32(levelGt); // levelGT
				mValueGt= _mm_set1_epi32( valueGt); // valueGT
			}

			SYS_INLINE STATIC void sThresholdLTValGTVal_32f_setup_S(__m128 &mLevel,__m128 &mValue,__m128 &mLevelGt,__m128 &mValueGt,			\
															  const Fw32f level,const Fw32f value,const  Fw32f levelGt,const Fw32f valueGt)
			{
				mLevel =  _mm_set1_ps( level); // levelLT
				mValue =  _mm_set1_ps( value); // valueLT
				mLevelGt= _mm_set1_ps(levelGt); // levelGT
				mValueGt= _mm_set1_ps( valueGt); // valueGT
			}

			SYS_INLINE STATIC void sThresholdLTValGTVal_64f_setup_S(__m128d &mLevel,__m128d &mValue,__m128d &mLevelGt,__m128d &mValueGt,			\
															  const Fw64f level,const Fw64f value,const  Fw64f levelGt,const Fw64f valueGt)
			{
				mLevel =  _mm_set1_pd( level); // levelLT
				mValue =  _mm_set1_pd( value); // valueLT
				mLevelGt= _mm_set1_pd(levelGt); // levelGT
				mValueGt= _mm_set1_pd( valueGt); // valueGT
			}
		}
		SYS_INLINE STATIC __m128i sThresholdLTValGTVal_16s_sse2_S(const __m128i &src,const __m128i &levelLT,const __m128i &valueLT,const __m128i &levelGT,const __m128i &valueGT)
		{
			__m128i masklt = _mm_cmplt_epi16(src,levelLT);
			__m128i desVLT = _mm_and_si128(masklt,valueLT);

			__m128i maskgt = _mm_cmpgt_epi16(src,levelGT);	
			__m128i desVGT = _mm_and_si128(maskgt,valueGT);

			desVLT = _mm_or_si128(desVLT,desVGT);
			masklt = _mm_or_si128(masklt,maskgt);
  			masklt = _mm_andnot_si128(masklt,src);
			return _mm_or_si128(masklt,desVLT);
		}


		SYS_INLINE STATIC __m128i sThresholdLTValGTVal_32s_sse2_S(const __m128i &src,const __m128i &levelLT,const __m128i &valueLT,const __m128i &levelGT,const __m128i &valueGT)
		{
			__m128i masklt = _mm_cmplt_epi32(src,levelLT);
			__m128i desVLT = _mm_and_si128(masklt,valueLT);

			__m128i maskgt = _mm_cmpgt_epi32(src,levelGT);	
			__m128i desVGT = _mm_and_si128(maskgt,valueGT);

			desVLT = _mm_or_si128(desVLT,desVGT);
			masklt = _mm_or_si128(masklt,maskgt);
			masklt = _mm_andnot_si128(masklt,src);
			return _mm_or_si128(masklt,desVLT);
		}




		SYS_INLINE STATIC __m128 sThresholdLTValGTVal_32f_sse2_S(const __m128 &src,const __m128 &levelLT,const __m128 &valueLT,const __m128 &levelGT,const __m128 &valueGT)
		{
			__m128 masklt = _mm_cmplt_ps(src,levelLT);
			__m128 desVLT = _mm_and_ps(masklt,valueLT);

			__m128 maskgt = _mm_cmpgt_ps(src,levelGT);	
			__m128 desVGT = _mm_and_ps(maskgt,valueGT);

			desVLT = _mm_or_ps(desVLT,desVGT);
			masklt = _mm_or_ps(masklt,maskgt);
  			masklt = _mm_andnot_ps(masklt,src);
			return _mm_or_ps(masklt,desVLT);
		}


		SYS_INLINE STATIC __m128d sThresholdLTValGTVal_64f_sse2_S(const __m128d &src,const __m128d &levelLT,const __m128d &valueLT,const __m128d &levelGT,const __m128d &valueGT)
		{
			__m128d masklt = _mm_cmplt_pd(src,levelLT);
			__m128d desVLT = _mm_and_pd(masklt,valueLT);

			__m128d maskgt = _mm_cmpgt_pd(src,levelGT);	
			__m128d desVGT = _mm_and_pd(maskgt,valueGT);

			desVLT = _mm_or_pd(desVLT,desVGT);
			masklt = _mm_or_pd(masklt,maskgt);
  			masklt = _mm_andnot_pd(masklt,src);
			return _mm_or_pd(masklt,desVLT);
		}

	} // namespace SSE2_LTVALGTVAL

	namespace INV
	{
		namespace SSE2_SETUP
		{
			SYS_INLINE void sThreshold_LTInv_32f_setup_S(__m128 &mLevel,const Fw32f level, __m128i &mPmax, __m128 &_level)
			{
				mLevel = _mm_set1_ps(level);
				_level = _mm_set1_ps(1/level);
				mPmax = _mm_set1_epi32((Fw32s)0x7FFFFFFF);
			}

			SYS_INLINE void sThreshold_LTInv_32f_LZero_setup_S(__m128 &inf)
			{ 
				inf = _mm_set1_ps(Const::INF_32F);
			}

			SYS_INLINE void sThreshold_LTInv_64f_LZero_setup_S(__m128d &inf,__m128d &one)
			{
				inf = _mm_set1_pd(Const::INF_64F);
				one = _mm_set1_pd((Fw64f)1.0);
			}

			SYS_INLINE void sThreshold_LTInv_64f_setup_S(__m128d &mLevel,const Fw64f level, __m128i &mPmax, __m128d &_level)
			{
				mLevel = _mm_set1_pd(level);
				_level = _mm_set1_pd(1/level);
				CBL_SSE2::Load_1x2_64s(mPmax,(Fw64s)0x7FFFFFFFFFFFFFFFLL);
			}
			SYS_INLINE void sThreshold_LTInv_32fc_LZero_setup_S(__m128 &inf,__m128i &pMax)
			{
				inf = _mm_set1_ps(Const::INF_32F);
				CBL_SSE2::Load_1x2_64s(pMax,(Fw64s)0x8000000000000000LL);
			}

			SYS_INLINE void sThreshold_LTInv_64fc_LZero_setup_S(__m128d &inf,__m128i &pMax)
			{
				inf = _mm_set1_pd(Const::INF_64F);
				pMax =  _mm_set_epi32(0x80000000,0x0,0x0,0x0);
			}

			SYS_INLINE void sThreshold_LTInv_64fc_setup_S(__m128d &mLevel,const Fw64f level, __m128i &mPmax, __m128d &_level)
			{
				mLevel = _mm_set1_pd(level);
				_level = _mm_set1_pd(1/level);
				mPmax =  _mm_set_epi32(0x80000000,0x0,0x0,0x0);
			}

			SYS_INLINE void sThreshold_LTInv_32fc_setup_S(__m128 &mLevel,const Fw32f level, __m128i &mPmax, __m128 &_level)
			{

				mLevel = _mm_set1_ps(level);
				_level = _mm_set1_ps(1/level);
				CBL_SSE2::Load_1x2_64s(mPmax,(Fw64s)0x8000000000000000LL);
			}

		} // namespace SSE2_SETUP
		SYS_INLINE STATIC __m128 sThreshold_LTInv_32f_sse2_S(const __m128 &src,const __m128 &level,const __m128 &invLevel, const __m128 &positiveMask)
		{
			__m128 absSrc = _mm_and_ps(src,positiveMask);		 // get the absolute value of the source
			__m128 minval = _mm_min_ps(absSrc,level);		         // abs(src)/ ( src*level)   condition 0 < abs(src) < level , otherwise 1/src
			minval = _mm_mul_ps(minval,invLevel);					 // min( abssrc, level) will give the value that needs to be divided with source. 
			minval = _mm_div_ps(minval,src);						

			__m128 zeroMask = _mm_cmpeq_ps(src,_mm_setzero_ps());   // when src = 0  compute 1/level
			__m128 interLevelRes = _mm_and_ps(zeroMask,invLevel);
			__m128 srcRec = _mm_andnot_ps(zeroMask,minval);
			return _mm_or_ps(interLevelRes,srcRec);
		}

		SYS_INLINE STATIC __m128 sThreshold_LTInv_32f_LZero_sse2_S(const __m128 &src,const __m128 &inf, FwStatus &status)
		{
			__m128 zeroMask = _mm_cmpeq_ps(src,_mm_setzero_ps());   // when src = 0  compute 1/level
			if(0x0 != _mm_movemask_ps(zeroMask))
			{	
				status = fwStsInvZero;
			}
			__m128 interInfRes = _mm_and_ps(zeroMask,inf);
			__m128 invSrc = _mm_rcp_ps(src);	
			__m128 srcRec = _mm_andnot_ps(zeroMask,invSrc);
			return _mm_or_ps(interInfRes,srcRec);
		}

		SYS_INLINE STATIC __m128d sThreshold_LTInv_64f_LZero_sse2_S(const __m128d &src,const __m128d &inf,const __m128d &one, FwStatus &status)
		{
			__m128d zeroMask = _mm_cmpeq_pd(src,_mm_setzero_pd());   // when src = 0  compute 1/level

			if(0x0 != _mm_movemask_pd(zeroMask))
			{	
				status = fwStsInvZero;
			}
			__m128d interInfRes = _mm_and_pd(zeroMask,inf);
			__m128d invSrc = _mm_div_pd(one,src);	
			__m128d srcRec = _mm_andnot_pd(zeroMask,invSrc);
			return _mm_or_pd(interInfRes,srcRec);
		}

		SYS_INLINE STATIC __m128d sThreshold_LTInv_64f_sse2_S(const __m128d &src,const __m128d &level,const __m128d &invLevel, const __m128d &positiveMask)
		{
			__m128d absSrc = _mm_and_pd(src,positiveMask);		 // get the absolute value of the source
			__m128d minval = _mm_min_pd(absSrc,level);		         // abs(src)/ ( src*level)   condition 0 < abs(src) < level , otherwise 1/src
			minval = _mm_mul_pd(minval,invLevel);					 // min( abssrc, level) will give the value that needs to be divided with source. 
			minval = _mm_div_pd(minval,src);

			__m128d zeroMask = _mm_cmpeq_pd(src,_mm_setzero_pd());   // when src = 0  compute 1/level
			__m128d interLevelRes = _mm_and_pd(zeroMask,invLevel);
			__m128d srcRec = _mm_andnot_pd(zeroMask,minval);
			return _mm_or_pd(interLevelRes,srcRec);
		}

		SYS_INLINE STATIC __m128 sThreshold_LTInv_32fc_LZero_sse2_S(const __m128 &src,const __m128 &inf,const __m128 &signToggle, FwStatus &status)
		{
			XMM128 zeroMask,src1;
			src1.f = src;
			zeroMask.d = _mm_cmpeq_pd(src1.d,_mm_setzero_pd());   // when src = 0  compute 1/level
			if(0x0 != _mm_movemask_pd(zeroMask.d))
			{	
				status = fwStsInvZero;
			}
			__m128 interInfRes = _mm_and_ps(zeroMask.f,inf);
			__m128 invSrc = GENF::a2b2_sum_32fc(src);	
			invSrc = _mm_div_ps(src,invSrc);
			invSrc = _mm_xor_ps(invSrc,signToggle);
			__m128 srcRec = _mm_andnot_ps(zeroMask.f,invSrc);
			return _mm_or_ps(interInfRes,srcRec);
		}

		SYS_INLINE STATIC __m128 sThreshold_LTInv_32fc_sse2_S(const __m128 &src,const __m128 &level,const __m128 &invLevel,const __m128 &signToggle)
		{
			__m128 sqr;
			__m128 absSrc = GENF::Thresh_mm_rsqrmag_ps(src,sqr);		 // get the absolute value of the source
			absSrc = _mm_mul_ps(absSrc,sqr);
			__m128 minval = _mm_min_ps(absSrc,level);		         // abs(src)/ ( src*level)   condition 0 < abs(src) < level , otherwise 1/src
			minval = _mm_mul_ps(minval,invLevel);				 // min( abssrc, level) will give the value that needs to be divided with source. 
		
			__m128  invSrc =_mm_xor_ps(src,signToggle);
			invSrc = _mm_div_ps(invSrc,sqr);
			minval = _mm_mul_ps(minval,invSrc);

			__m128 zeroMask = _mm_cmpeq_ps(src,_mm_setzero_ps());   // when src = 0  compute 1/level
			__m128 interLevelRes = _mm_and_ps(zeroMask,invLevel);
			__m128 srcRec = _mm_andnot_ps(zeroMask,minval);
			return _mm_or_ps(interLevelRes,srcRec);
		}

		SYS_INLINE STATIC __m128d sThreshold_LTInv_64fc_LZero_sse2_S(const __m128d &src,const __m128d &inf,const __m128d &signToggle, FwStatus &status)
		{
		
			__m128d mask = _mm_cmpeq_pd(src,_mm_setzero_pd());

			if(0x3 == _mm_movemask_pd(mask))
			{	
				status = fwStsInvZero;
				return inf;
			}

			__m128d invSrc = GENF::a2b2_sum_64fc(src);	
			invSrc = _mm_div_pd(src,invSrc);
			return _mm_xor_pd(invSrc,signToggle);
		}

		SYS_INLINE STATIC __m128d sThreshold_LTInv_64fc_sse2_S(const __m128d &src,const __m128d &level,const __m128d &invLevel, const __m128d &signToggle)
		{
			__m128d sqr;
			__m128d absSrc = GENF::Thresh_mm_mag_pd(src,sqr);		 // get the absolute value of the source

			__m128d mask = _mm_cmpeq_pd(absSrc,_mm_setzero_pd());
			if(0x3 == _mm_movemask_pd(mask))
			{
				return invLevel;
			}

			sqr = _mm_unpacklo_pd(sqr,sqr);
			__m128d minval = _mm_min_pd(absSrc,level);		         // abs(src)/ ( src*level)   condition 0 < abs(src) < level , otherwise 1/src
			minval = _mm_mul_pd(minval,invLevel);				 // min( abssrc, level) will give the value that needs to be divided with source. 
		
			__m128d  invSrc =_mm_xor_pd(src,signToggle);
			invSrc = _mm_div_pd(invSrc,sqr);
			minval = _mm_mul_pd(minval,invSrc);
				
			return minval;
		}	
	} // namespace INV
} // namespace THRES

namespace COPY
{
	SYS_INLINE STATIC void copy_128si_sse2_S(const __m128i &src,__m128i &dest)
	{
		 dest = src;
	}
}// namespace Copy

} // namespace OPT_LEVEL
#endif
