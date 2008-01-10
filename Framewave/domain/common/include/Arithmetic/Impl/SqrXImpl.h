/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SQRXIMPL_H__
#define __SQRXIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"

namespace OPT_LEVEL
{

namespace SQRX_REF
{
	namespace SQRT
	{
		template< class TS, class TD >
		ISV SqrtI( const TS * s, CH cs, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( (s[3] < 0 ? 0 : sqrt((float)s[3])) , scale )));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( (s[2] < 0 ? 0 : sqrt((float)s[2])) , scale )));
						d[1] = FW_REF::Limits<TD>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( (s[1] < 0 ? 0 : sqrt((float)s[1])) , scale )));
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( (s[0] < 0 ? 0 : sqrt((float)s[0])) , scale )));
			}
		}

        IS S64 DoubleToLong(F64 x)
        {

	        F64 di, fract = modf( x, &di );
	        S64 i = (S64)di;
	        bool even = !(i&0x1);
	        S64 res;

	        if     ( fract >   0.5 ) res = i+1;									//  1.6-> 2,  2.6-> 3			
	        else if( fract ==  0.5 ) res = even ? i : i+1;						//  1.5-> 2,  2.5-> 2 
	        else if( fract >=  0.0 ) res = i;			 						//  1.4-> 1,  2.4-> 1
	        else if( fract >  -0.5 ) res = i;									// -1.4->-1, -2.4->-2 
	        else if( fract == -0.5 ) res = even ? i : i-1;						// -1.5->-2, -2.5->-2
	        else					 res = i-1;									// -1.6->-2, -2.6->-3

	        return res;	
        }

        ISV SqrtI( const Fw64s * s, CH cs, Fw64s * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;
            switch( cd )
			{
			case C4:	d[3] = DoubleToLong( (s[3] < 0 ? 0 : sqrt((float)s[3])) * pow(2.0,-scale));
			case C3:    
			case AC4:	d[2] = DoubleToLong( (s[2] < 0 ? 0 : sqrt((float)s[2])) * pow(2.0,-scale));
						d[1] = DoubleToLong( (s[1] < 0 ? 0 : sqrt((float)s[1])) * pow(2.0,-scale));
			case C1:	d[0] = DoubleToLong( (s[0] < 0 ? 0 : sqrt((float)s[0])) * pow(2.0,-scale));
			}
		}

		ISV SqrtI( const A16U * s, CH cs, A16U * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<A16U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[3]) , scale )));
			case C3:																			
			case AC4:	d[2] = FW_REF::Limits<A16U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[2]) , scale )));
						d[1] = FW_REF::Limits<A16U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[1]) , scale )));
			case C1:	d[0] = FW_REF::Limits<A16U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[0]) , scale )));
			}
		}

		ISV SqrtI( const A8U * s, CH cs, A8U * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<A8U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[3]) , scale )));
			case C3:													   						 
			case AC4:	d[2] = FW_REF::Limits<A8U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[2]) , scale )));
						d[1] = FW_REF::Limits<A8U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[1]) , scale )));
			case C1:	d[0] = FW_REF::Limits<A8U>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale(sqrt((float)s[0]) , scale )));
			}
		}


		ISV SqrtI( const A16SC * s, CH cs, A16SC * d, CH cd, int scale)
		{
			// Confirm that there is only 1 channel on both sides
			ASSERT( ChannelCount(cs) == ChannelCount(cd) == C1 ); cs; cd;

			A16SC tempDest = {0};
			tempDest.re =  FW_REF::Limits<A16S>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( sqrt((sqrt( ((float)s[0].re * (float) s[0].re)
															+ ((float)s[0].im * (float)s[0].im) ) + (float) s[0].re) /
																									(float) 2.0), scale)));


			float sign = (s[0].im < 0) ? (float) -1.0 : (float) 1.0;
			tempDest.im =  FW_REF::Limits<A16S>::Sat( CBL_LIBRARY::FloatToInt(FW_REF::Scale( sign * sqrt((sqrt( ((float)s[0].re * (float) s[0].re) + ((float)s[0].im * (float)s[0].im) )
																						- (float) s[0].re) / (float) 2.0), scale)));

			d[0] = tempDest;
		}


		template< class TS, class TD >
		ISV SqrtF( const TS * s, CH cs, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = (TD) sqrt( s[3] );
			case C3:
			case AC4:	d[2] = (TD) sqrt( s[2] );
						d[1] = (TD) sqrt( s[1] );
			case C1:	d[0] = (TD) sqrt( s[0] );
			}
		}

		ISV SqrtF( const A32FC * s, CH cs, A32FC * d, CH cd )
		{
			// Confirm that there is only 1 channel on both sides
			ASSERT( ChannelCount(cs) == ChannelCount(cd) == C1 ); cs; cd;

			A32FC tempDest = {0};
			float absqrrt = sqrt((s->re * s->re) + (s->im * s->im));

			tempDest.re =  ( sqrt((absqrrt + s->re) / 2));

			absqrrt -= s->re;
			if (absqrrt < 0)
				tempDest.im = 0;
			else
				tempDest.im =  ((s->im < 0) ? -1 : 1) // sign
								* (sqrt(absqrrt / 2));

			d->re = tempDest.re;
			d->im = tempDest.im;
		}

		ISV SqrtF( const A64FC * s, CH cs, A64FC * d, CH cd )
		{
			// Confirm that there is only 1 channel on both sides
			ASSERT( ChannelCount(cs) == ChannelCount(cd) == C1 ); cs; cd;

			A64FC tempDest = {0};
			double absqrrt = (s->re * s->re) + (s->im * s->im);
			if (absqrrt < 0)
				return;

			else absqrrt = sqrt(absqrrt);

			tempDest.re =  ( sqrt((absqrrt + s->re) / 2));

			absqrrt -= s->re;
			if (absqrrt < 0)
				tempDest.im = 0;
			else
				tempDest.im =  ((s->im < 0) ? -1 : 1) // sign
								* (sqrt(absqrrt / 2));

			d->re = tempDest.re;
			d->im = tempDest.im;
		}

	}

	namespace SQR
	{
		template< class TS, class TD >
		ISV SqrI( const TS * s, CH cs, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale((float)(s[3] * s[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale((float)(s[2] * s[2]), scale ));
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale((float)(s[1] * s[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale((float)(s[0] * s[0]), scale ));
			}
		}


		ISV SqrI( const A16U * s, CH cs, A16U * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<A16U>::Sat( FW_REF::Scale( ((float)s[3] * s[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<A16U>::Sat( FW_REF::Scale( ((float)s[2] * s[2]), scale ));
						d[1] = FW_REF::Limits<A16U>::Sat( FW_REF::Scale( ((float)s[1] * s[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<A16U>::Sat( FW_REF::Scale( ((float)s[0] * s[0]), scale ));
			}
		}

		ISV SqrI( const A16SC * s, CH cs, A16SC * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			float	s0re = 0, s0im = 0, \
					s1re = 0, s1im = 0, \
					s2re = 0, s2im = 0, \
					s3re = 0, s3im = 0;


			switch ( cd )
			{
			case C4:
					s3re = s[3].re; s3im = s[3].im;
			case C3:					   
			case AC4:					   
					s2re = s[2].re; s2im = s[2].im;
					s1re = s[1].re; s1im = s[1].im;
			case C1:					   
					s0re = s[0].re; s0im = s[0].im;
			}

			switch( cd )
			{
			case C4:	d[3].re = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s3re * s3re) - (s3im * s3im), scale ));
						d[3].im = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s3re * s3im * 2.0), scale ));
			case C3:														 
			case AC4:	d[2].re = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s2re * s2re) - (s2im * s2im), scale ));
						d[2].im = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s2re * s2im * 2.0), scale ));
																			 
						d[1].re = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s1re * s1re) - (s1im * s1im), scale ));
						d[1].im = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s1re * s1im * 2.0), scale ));
																			 
			case C1:	d[0].re = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s0re * s0re) - (s0im * s0im), scale ));
						d[0].im = FW_REF::Limits<S16>::Sat( FW_REF::Scale( (s0re * s0im * 2.0), scale ));
			}
		}


		template< class TS, class TD >
		ISV SqrF( const TS * s, CH cs, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = s[3] * s[3];
			case C3:
			case AC4:	d[2] = s[2] * s[2];
						d[1] = s[1] * s[1];
			case C1:	d[0] = s[0] * s[0];
			}
		}
	}
}



namespace SQRX_SSE2
{
	namespace SQRT
	{
		ISV Sqrt_16s(const XMM128 &src,XMM128 &dest, const XMM128 &scale)
		{
			XMM128 src_h;
			XMM128 srcLo = src;
			__m128 temp_f_l, temp_f_h;


			CBL_SSE2::Unpack16STo32S(srcLo.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (srcLo.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);

			static const __m128	zerof	= CONST_SETZERO_32F();
			__m128	isNotZero;

			isNotZero	= _mm_cmpgt_ps(temp_f_l, zerof);
			temp_f_l	= _mm_mul_ps(temp_f_l, _mm_rsqrt_ps(temp_f_l));
			temp_f_l	= _mm_and_ps(temp_f_l, isNotZero);

			isNotZero	= _mm_cmpgt_ps(temp_f_h, zerof);
			temp_f_h	= _mm_mul_ps(temp_f_h, _mm_rsqrt_ps(temp_f_h));
			temp_f_h	= _mm_and_ps(temp_f_h, isNotZero);

			temp_f_l	= _mm_mul_ps(temp_f_l,scale.f);
			temp_f_h	= _mm_mul_ps(temp_f_h,scale.f);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}

		ISV Sqrt_16s_SZ(const XMM128 &src,XMM128 &dest)
		{
			XMM128 src_h;
			XMM128 srcLo = src;
			__m128 temp_f_l, temp_f_h;

			CBL_SSE2::Unpack16STo32S(srcLo.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (srcLo.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);

			static const __m128	zerof	= CONST_SETZERO_32F();
			__m128	isNotZero;

			isNotZero	= _mm_cmpgt_ps(temp_f_l, zerof);
			temp_f_l	= _mm_mul_ps(temp_f_l, _mm_rsqrt_ps(temp_f_l));
			temp_f_l	= _mm_and_ps(temp_f_l, isNotZero);

			isNotZero	= _mm_cmpgt_ps(temp_f_h, zerof);
			temp_f_h	= _mm_mul_ps(temp_f_h, _mm_rsqrt_ps(temp_f_h));
			temp_f_h	= _mm_and_ps(temp_f_h, isNotZero);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}

		ISV Sqrt_16u(const XMM128 &src, XMM128 &dest, const XMM128 &scale)
		{
			XMM128 src_h;
			XMM128 srcLo = src;
			__m128 temp_f_l, temp_f_h;

			CBL_SSE2::Unpack16UTo32U(srcLo.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (srcLo.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);


			temp_f_l	= _mm_sqrt_ps(temp_f_l);
			temp_f_h	= _mm_sqrt_ps(temp_f_h);


			temp_f_l	= _mm_mul_ps(temp_f_l,scale.f);
			temp_f_h	= _mm_mul_ps(temp_f_h,scale.f);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}

		ISV Sqrt_16u_SZ(const XMM128 &src,XMM128 &dest)
		{
			XMM128 src_h;
			XMM128 srcLo = src;
			__m128 temp_f_l, temp_f_h;

			CBL_SSE2::Unpack16UTo32U(srcLo.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (srcLo.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);

			temp_f_l	= _mm_sqrt_ps(temp_f_l);
			temp_f_h	= _mm_sqrt_ps(temp_f_h);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}


		ISV Sqrt_16u_rcp(XMM128 &src,XMM128 &dest, const XMM128 &scale)
		{
			XMM128 src_h;
			__m128 temp_f_l, temp_f_h;

			CBL_SSE2::Unpack16UTo32U(src.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (src.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);

			static const __m128	zerof	= CONST_SETZERO_32F();
			__m128	isNotZero;

			isNotZero	= _mm_cmpneq_ps(temp_f_l, zerof);
			temp_f_l	= _mm_mul_ps(temp_f_l, _mm_rsqrt_ps(temp_f_l));
			temp_f_l	= _mm_and_ps(temp_f_l, isNotZero);

			isNotZero	= _mm_cmpneq_ps(temp_f_h, zerof);
			temp_f_h	= _mm_mul_ps(temp_f_h, _mm_rsqrt_ps(temp_f_h));
			temp_f_h	= _mm_and_ps(temp_f_h, isNotZero);

			temp_f_l	= _mm_mul_ps(temp_f_l,scale.f);
			temp_f_h	= _mm_mul_ps(temp_f_h,scale.f);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}
		ISV Sqrt_16u_SZ_rcp(XMM128 &src,XMM128 &dest)
		{
			XMM128 src_h;
			__m128 temp_f_l, temp_f_h;

			CBL_SSE2::Unpack16UTo32U(src.i,src_h.i);
			temp_f_l = 	_mm_cvtepi32_ps  (src.i);
			temp_f_h = 	_mm_cvtepi32_ps  (src_h.i);

			static const __m128	zerof	= CONST_SETZERO_32F();
			__m128	isNotZero;

			isNotZero	= _mm_cmpneq_ps(temp_f_l, zerof);
			temp_f_l	= _mm_mul_ps(temp_f_l, _mm_rsqrt_ps(temp_f_l));
			temp_f_l	= _mm_and_ps(temp_f_l, isNotZero);

			isNotZero	= _mm_cmpneq_ps(temp_f_h, zerof);
			temp_f_h	= _mm_mul_ps(temp_f_h, _mm_rsqrt_ps(temp_f_h));
			temp_f_h	= _mm_and_ps(temp_f_h, isNotZero);

			dest.i =  _mm_cvtps_epi32 (temp_f_l);
			src_h.i =  _mm_cvtps_epi32(temp_f_h);

			FW_SSE2::pack32STo16S(dest.i,src_h.i);
		}
		ISV Sqrt_8u(const XMM128 &src, XMM128 &dest, const XMM128 &scale)
		{
			XMM128 src_hi, src_hi_1;
			XMM128 srcLo = src;
			CBL_SSE2::Unpack8UTo16U(srcLo.i,src_hi.i); 
			Sqrt_16u(srcLo,dest,scale);
			Sqrt_16u(src_hi,src_hi_1,scale);
			FW_SSE2::pack16STo8U(dest.i,src_hi_1.i);
		}

		ISV Sqrt_8u_SZ(const XMM128 &src, XMM128 &dest)
		{
			XMM128 src_hi, src_hi_1;
			XMM128 srcLo = src;
			CBL_SSE2::Unpack8UTo16U(srcLo.i,src_hi.i); 
			Sqrt_16u_SZ(srcLo,dest);
			Sqrt_16u_SZ(src_hi,src_hi_1);
			FW_SSE2::pack16STo8U(dest.i,src_hi_1.i);
		}

		ISV Sqrt_16sc(const XMM128 &src, XMM128 &dst, const XMM128 &scale, const XMM128 &min32s, const XMM128 &half)
		{
			XMM128 signb;

			__m128 magf			= CBL_SSE2::Magnitude_16sc_32f(src.i, min32s.i);
			__m128i	a			= _mm_srai_epi32(_mm_slli_epi32(src.i, 16), 16);
			__m128	af			= _mm_cvtepi32_ps(a);

			__m128 old_avalsf	= _mm_add_ps(magf, af);
			__m128 old_bvalsf	= _mm_sub_ps(magf, af);

			old_avalsf			= _mm_mul_ps(old_avalsf, half.f);	// div by 2
			old_bvalsf			= _mm_mul_ps(old_bvalsf, half.f);

			__m128 avalsf		= _mm_mul_ps(old_avalsf, _mm_rsqrt_ps(old_avalsf));	// root
			__m128 bvalsf		= _mm_mul_ps(old_bvalsf, _mm_rsqrt_ps(old_bvalsf));

			avalsf				= _mm_and_ps(avalsf, _mm_cmpneq_ps(old_avalsf, _mm_setzero_ps()));
			bvalsf				= _mm_and_ps(bvalsf, _mm_cmpneq_ps(old_bvalsf, _mm_setzero_ps()));

			signb.i				= _mm_and_si128(src.i, min32s.i);
			bvalsf				= _mm_xor_ps(bvalsf, signb.f);

			
			// scale
			avalsf				= _mm_mul_ps(avalsf, scale.f);
			bvalsf				= _mm_mul_ps(bvalsf, scale.f);

			__m128i	avals		= _mm_cvtps_epi32(avalsf);
			__m128i	bvals		= _mm_cvtps_epi32(bvalsf);

			__m128i	dstLo		= _mm_unpacklo_epi32(avals, bvals);
			__m128i	dstHi		= _mm_unpackhi_epi32(avals, bvals);

			dst.i		= _mm_packs_epi32(dstLo, dstHi);
		}

		ISV Sqrt_16sc_SZ(const XMM128 &src, XMM128 &dst, const XMM128 &min32s, const XMM128 &half)
		{
			XMM128 signb;

			__m128 magf			= CBL_SSE2::Magnitude_16sc_32f(src.i, min32s.i);
			__m128i	a			= _mm_srai_epi32(_mm_slli_epi32(src.i, 16), 16);
			__m128	af			= _mm_cvtepi32_ps(a);

			__m128 old_avalsf	= _mm_add_ps(magf, af);
			__m128 old_bvalsf	= _mm_sub_ps(magf, af);

			old_avalsf			= _mm_mul_ps(old_avalsf, half.f);	// div by 2
			old_bvalsf			= _mm_mul_ps(old_bvalsf, half.f);

			__m128 avalsf		= _mm_mul_ps(old_avalsf, _mm_rsqrt_ps(old_avalsf));	// root
			__m128 bvalsf		= _mm_mul_ps(old_bvalsf, _mm_rsqrt_ps(old_bvalsf));

			avalsf				= _mm_and_ps(avalsf, _mm_cmpneq_ps(old_avalsf, _mm_setzero_ps()));
			bvalsf				= _mm_and_ps(bvalsf, _mm_cmpneq_ps(old_bvalsf, _mm_setzero_ps()));

			signb.i				= _mm_and_si128(src.i, min32s.i);
			bvalsf				= _mm_xor_ps(bvalsf, signb.f);

			__m128i	avals		= _mm_cvtps_epi32(avalsf);
			__m128i	bvals		= _mm_cvtps_epi32(bvalsf);

			__m128i	dstLo		= _mm_unpacklo_epi32(avals, bvals);
			__m128i	dstHi		= _mm_unpackhi_epi32(avals, bvals);

			dst.i		= _mm_packs_epi32(dstLo, dstHi);
		}

		ISV Sqrt_32s16s(const XMM128 &src, XMM128 &dst, const XMM128 &scale)
		{
			dst.f = _mm_cvtepi32_ps(src.i);
			
			__m128 zero = _mm_setzero_ps();
			dst.f = _mm_max_ps(dst.f, zero);	// we don't need no, negative values

			dst.f = _mm_sqrt_ps(dst.f);
			dst.f = _mm_mul_ps(dst.f, scale.f);
			dst.i = _mm_cvtps_epi32(dst.f);
			dst.i = _mm_packs_epi32(dst.i, dst.i); // pack and saturate to 16 bit
		}

		ISV Sqrt_32s16s_SZ(const XMM128 &src, XMM128 &dst)
		{
			dst.f = _mm_cvtepi32_ps(src.i);

			__m128 zero = _mm_setzero_ps();
			dst.f = _mm_max_ps(dst.f, zero);	// we don't need no, negative values

			dst.f = _mm_sqrt_ps(dst.f);
			dst.i = _mm_cvtps_epi32(dst.f);
			dst.i = _mm_packs_epi32(dst.i, dst.i); // pack and saturate to 16 bit
		}

		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV Sqrt_8u_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqrt_16s_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqrt_16u_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqrt_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &min32s, XMM128 &half)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::InitMisc::Min::Of32s(min32s);
					Common::InitMisc::Constants::half32f(half);
				}

				ISV Sqrt_32fc_setup(XMM128 &negbmask, XMM128 &half)
				{
					Common::InitMisc::Constants::negbmask32f(negbmask);
					Common::InitMisc::Constants::half32f(half);
				}

				ISV Sqrt_64fc_setup(XMM128 &negbmask, XMM128 &half)
				{
					Common::InitMisc::Constants::negbmask64f(negbmask);
					Common::InitMisc::Constants::half64f(half);
				}

				template <int scaleType>
				ISV Sqrt_32s16s_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

			}

			namespace AC4
			{
				template <int scaleType>
				ISV Sqrt_8u_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					}
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV Sqrt_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV Sqrt_16u_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For16(ac4mask);
				}

				ISV Sqrt_32f_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For32(ac4mask);
				}
			}
		}

		namespace Unit
		{
			namespace C1
			{
				template <int scaleType>
				ISV Sqrt_8u_unit( const XMM128& src,  XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SQRX_SSE2::SQRT::Sqrt_8u(src, dst, scale);
						break;
					default:
						SQRX_SSE2::SQRT::Sqrt_8u_SZ(src, dst);
					}
				}


				template <int scaleType>
				ISV Sqrt_16s_unit( const XMM128& src,  XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SQRX_SSE2::SQRT::Sqrt_16s(src, dst, scale);
						break;
					case 0:
						SQRX_SSE2::SQRT::Sqrt_16s_SZ(src, dst);
						break;
					}
				}

				template <int scaleType>
				ISV Sqrt_16u_unit( const XMM128& src,  XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SQRX_SSE2::SQRT::Sqrt_16u(src, dst, scale);
						break;
					case 0:
						SQRX_SSE2::SQRT::Sqrt_16u_SZ(src, dst);
						break;
					}
				}


				template <int scaleType>
				ISV Sqrt_16sc_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scale, const XMM128 &min32s, const XMM128 &half)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Sqrt_16sc(src, dst, scale, min32s, half);
						break;
					case 0:
						Sqrt_16sc_SZ(src, dst, min32s, half);
						break;
					default:;
					}
				}

				ISV Sqrt_32fc_unit(const XMM128 &src, XMM128 &dest, const XMM128 &mask, const XMM128 &half)
				{
					XMM128 temp, temp2;
					const static __m128 zerof	= CONST_SETZERO_32F();

					temp.f				= _mm_mul_ps(src.f, src.f); // everything's squared now
					temp2.f				= _mm_shuffle_ps(temp.f, temp.f, _MM_SHUFFLE(2, 3, 0, 1)); // sqr(baba) --> sqr(abab)
					temp.f				= _mm_add_ps(temp.f, temp2.f);	

					__m128 isNotZero	= _mm_cmpneq_ps(temp.f, zerof);
					temp.f				= _mm_mul_ps(temp.f, _mm_rsqrt_ps(temp.f));	// Now we have t = sqrt(sqr(a) + sqr(b)) on both elements of each complex
					temp.f				= _mm_and_ps(temp.f, isNotZero);


					temp2.f 			= _mm_shuffle_ps(src.f, src.f, _MM_SHUFFLE(2, 2, 0, 0));	// +a +a +a +a in temp2 now
					temp2.f 			= _mm_xor_ps(temp2.f, mask.f);								// -a +a -a +a in temp2 now

					temp.f				= _mm_add_ps(temp.f, temp2.f);				// b = t - a, a = t + a

					temp.f				= _mm_mul_ps(temp.f, half.f);					// temp2.f contains 0.5 (hence / 2)

					isNotZero			= _mm_cmpneq_ps(temp.f, zerof);
					temp.f				= _mm_mul_ps(temp.f, _mm_rsqrt_ps(temp.f));
					temp.f				= _mm_and_ps(temp.f, isNotZero);

					// now for the sign of b
					temp2.f				= _mm_and_ps(src.f, mask.f);				// pull out sign bit of the b's
					
					// set the correct sign's for the b's in temp
					dest.f				= _mm_or_ps(temp.f, temp2.f);				// 'or' in the sign bit of b for the new b's
				}


				ISV Sqrt_64fc_unit(const XMM128 &src, XMM128 &dest, const XMM128 &mask, const XMM128 &half)
				{
					XMM128 temp = src;
					temp.d = _mm_mul_pd(temp.d, temp.d); // everything's squared now (temp = sqr(b a))
					XMM128 temp2;
					temp2.d = _mm_shuffle_pd(temp.d, temp.d, _MM_SHUFFLE2(0, 1));// change order of a and b to now have temp2 = sqr(a b)

					temp.d = _mm_add_pd(temp.d, temp2.d);	// temp = sqr(a) + sqr(b) (in both slots)
					temp.d = _mm_sqrt_pd(temp.d);			// temp = sqrt( sqr(a) + sqr(b) )

					temp2.d = _mm_shuffle_pd(src.d, src.d, _MM_SHUFFLE2(0, 0));	// +a +a in temp2 now
					temp2.d = _mm_xor_pd(temp2.d, mask.d);						// -a +a in temp2 now

					temp.d = _mm_add_pd(temp.d, temp2.d);				// b = temp - a, a = temp + a

					__m128d zerod = _mm_setzero_pd();
					temp.d = _mm_max_pd(temp.d, zerod);					// we don't want no, negative values

					temp.d = _mm_mul_pd(temp.d, half.d);					// temp2 contains 0.5 (hence / 2)
					temp.d = _mm_sqrt_pd(temp.d);

					// now for the sign of b
					temp2.d = _mm_and_pd(src.d, mask.d);				// pull out sign bit of the b's
					
					// set the correct sign's for the b's in temp
					dest.d = _mm_or_pd(temp.d, temp2.d);				// 'or' in the sign bit of b for the new b's
				}

				template <int scaleType>
				ISV Sqrt_32s16s_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Sqrt_32s16s(src, dst, scale);
						break;
					case 0:
						Sqrt_32s16s_SZ(src, dst);
						break;
					default:;
					}
				}

			}
		}
	}

	namespace SQR
	{

		ISV Saturate_32f_32s(const XMM128 &src, XMM128 &dest)
		{
			static const __m128 f_val  = CONST_SET1_32F((float)0x7FFF);
			static const __m128i mask2 = CONST_SET1_32I(0x7FFFFFFF);

			XMM128 mask;
			mask.f =_mm_cmpgt_ps(src.f,f_val );
			dest.i =  _mm_cvtps_epi32 (src.f);
			dest.i = _mm_or_si128(dest.i,mask.i);
			dest.i = _mm_and_si128  (dest.i,mask2);
		}

		ISV Saturate_32f_32u(const XMM128 &src, XMM128 &dest)
		{
			static const __m128 f_val  = CONST_SET1_32F((float)0xFFFF);
			XMM128 srct = src;
			srct.f = _mm_min_ps(src.f, f_val);
			dest.i =  _mm_cvtps_epi32 (srct.f);
		}

		ISV Sqr_16u_SZ(const XMM128 &src, XMM128 &dst)
		 {

			XMM128 src_h, src_l;
 			XMM128 temp_f_l, temp_f_h;

			src_l = src;
 			CBL_SSE2::Unpack16UTo32U(src_l.i,src_h.i);

			temp_f_l.f = 	_mm_cvtepi32_ps  (src_l.i);
 			temp_f_h.f = 	_mm_cvtepi32_ps  (src_h.i);

 			temp_f_l.f	= _mm_mul_ps(temp_f_l.f,temp_f_l.f);
 			temp_f_h.f	= _mm_mul_ps(temp_f_h.f,temp_f_h.f);

			
 			SQRX_SSE2::SQR::Saturate_32f_32u(temp_f_l,src_l);
 			SQRX_SSE2::SQR::Saturate_32f_32u(temp_f_h,src_h);

			src_h.i = _mm_slli_epi32(src_h.i, 16);//32s to 16u
			src_l.i = _mm_or_si128(src_l.i, src_h.i);
				
			src_l.i = _mm_shufflelo_epi16(src_l.i, _MM_SHUFFLE(3,1,2,0));
			src_l.i = _mm_shufflehi_epi16(src_l.i, _MM_SHUFFLE(3,1,2,0));
			dst.i = _mm_shuffle_epi32(src_l.i, _MM_SHUFFLE(3,1,2,0));
		 }

		ISV Sqr_16u(const XMM128 &src, XMM128 &dst, const XMM128 &scale)
		{

		XMM128 src_h, src_l;
		XMM128 temp_f_l, temp_f_h;

		src_l = src;
		CBL_SSE2::Unpack16UTo32U(src_l.i,src_h.i);

		temp_f_l.f = 	_mm_cvtepi32_ps  (src_l.i);
		temp_f_h.f = 	_mm_cvtepi32_ps  (src_h.i);

		temp_f_l.f	= _mm_mul_ps(temp_f_l.f,temp_f_l.f);
		temp_f_h.f	= _mm_mul_ps(temp_f_h.f,temp_f_h.f);

		temp_f_l.f	= _mm_mul_ps(temp_f_l.f,scale.f);
		temp_f_h.f	= _mm_mul_ps(temp_f_h.f,scale.f);

		SQRX_SSE2::SQR::Saturate_32f_32u(temp_f_l,src_l);
		SQRX_SSE2::SQR::Saturate_32f_32u(temp_f_h,src_h);

		src_h.i = _mm_slli_epi32(src_h.i, 16);//32s to 16u
		src_l.i = _mm_or_si128(src_l.i, src_h.i);
			
		src_l.i = _mm_shufflelo_epi16(src_l.i, _MM_SHUFFLE(3,1,2,0));
		src_l.i = _mm_shufflehi_epi16(src_l.i, _MM_SHUFFLE(3,1,2,0));
		dst.i = _mm_shuffle_epi32(src_l.i, _MM_SHUFFLE(3,1,2,0));
		}

		ISV Sqr_16s(const XMM128 &s, XMM128 &dest, const XMM128 &scale)
		{
			XMM128 temp_f_l, temp_f_h;
			XMM128 src_hi;
			__m128i srt_lo;
			XMM128 src = s;
			srt_lo = _mm_mullo_epi16 (src.i,src.i);
			src_hi.i = _mm_mulhi_epi16 (src.i,src.i);
			src.i  =	_mm_unpacklo_epi16(srt_lo,src_hi.i ); // lower 4
			src_hi.i = _mm_unpackhi_epi16 (srt_lo,src_hi.i); // uper 4

			temp_f_l.f = 	_mm_cvtepi32_ps  (src.i);
			temp_f_h.f = 	_mm_cvtepi32_ps  (src_hi.i);

			temp_f_l.f	= _mm_mul_ps(temp_f_l.f,scale.f);
			temp_f_h.f	= _mm_mul_ps(temp_f_h.f,scale.f);

			SQRX_SSE2::SQR::Saturate_32f_32s(temp_f_l,dest);
			SQRX_SSE2::SQR::Saturate_32f_32s(temp_f_h,src_hi);

			FW_SSE2::pack32STo16S(dest.i,src_hi.i);
		}

		ISV Sqr_16s_SZ(const XMM128 &s, XMM128 &dest)
		{
			XMM128 temp_f_l, temp_f_h;
			XMM128 src_hi;
			__m128i srt_lo;
			XMM128 src = s;
			srt_lo = _mm_mullo_epi16 (src.i,src.i);
			src_hi.i = _mm_mulhi_epi16 (src.i,src.i);
			src.i  =	_mm_unpacklo_epi16(srt_lo,src_hi.i ); // lower 4
			src_hi.i = _mm_unpackhi_epi16 (srt_lo,src_hi.i); // uper 4

			temp_f_l.f = 	_mm_cvtepi32_ps  (src.i);
			temp_f_h.f = 	_mm_cvtepi32_ps  (src_hi.i);

			SQRX_SSE2::SQR::Saturate_32f_32s(temp_f_l,dest);
			SQRX_SSE2::SQR::Saturate_32f_32s(temp_f_h,src_hi);

			FW_SSE2::pack32STo16S(dest.i,src_hi.i);
		}


	   ISV Sqr_16sc(const XMM128 &s1,const XMM128 &s2,XMM128 &dest,const XMM128 &scale, const __m128i &min32s, const __m128i &negb)
		{
			XMM128 src1DstLo = s1;
			dest.i	= CBL_SSE2::Multiply_16sc_32sc(src1DstLo.i, s2.i, min32s, negb);
			src1DstLo.f = _mm_cvtepi32_ps(src1DstLo.i);
			dest.f		= _mm_cvtepi32_ps(dest.i);

			src1DstLo.f = _mm_mul_ps(src1DstLo.f, scale.f);
			dest.f		= _mm_mul_ps(dest.f, scale.f);

			src1DstLo.i = _mm_cvtps_epi32(src1DstLo.f);
			dest.i		= _mm_cvtps_epi32(dest.f);


			dest.i		= _mm_packs_epi32(src1DstLo.i, dest.i);
		}

	   ISV Sqr_16sc_SL(const XMM128 &s1,const XMM128 &s2,XMM128 &dest,const XMM128 &scale, const __m128i &min32s, const __m128i &negb)
		{
			XMM128 src1DstLo = s1;
			dest.i	= CBL_SSE2::Multiply_16sc_32sc(src1DstLo.i, s2.i, min32s, negb);
			src1DstLo.f = _mm_cvtepi32_ps(src1DstLo.i);
			dest.f		= _mm_cvtepi32_ps(dest.i);

			src1DstLo.f = _mm_mul_ps(src1DstLo.f, scale.f);
			dest.f		= _mm_mul_ps(dest.f, scale.f);

			__m128i maskl	= _mm_srai_epi32(_mm_xor_si128(src1DstLo.i, min32s), 31);
			__m128i maskh	= _mm_srai_epi32(_mm_xor_si128(dest.i, min32s), 31);

			src1DstLo.i = _mm_cvtps_epi32(src1DstLo.f);
			dest.i		= _mm_cvtps_epi32(dest.f);

			src1DstLo.i	= _mm_xor_si128(src1DstLo.i, _mm_and_si128(maskl, _mm_cmpeq_epi32(src1DstLo.i, min32s)));
			dest.i		= _mm_xor_si128(dest.i, _mm_and_si128(maskh, _mm_cmpeq_epi32(dest.i, min32s)));

			dest.i		= _mm_packs_epi32(src1DstLo.i, dest.i);
		}

		ISV Sqr_16sc_SZ(const XMM128 &s1,const XMM128 &s2, XMM128 &dest, const __m128i &min32s, const __m128i &negb)
		{
			dest.i	= CBL_SSE2::Multiply_16sc(s1.i, s2.i, min32s, negb);
		}


		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV Sqr_8u_setup(int scale, XMM128 scaleMasks[], XMM128 &sqrCap)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);

					Fw16u	maxVal = 16;
					switch (scaleType)
					{
					case -1:
						maxVal	= ((Fw16u) sqrt( float(((maxVal * maxVal) >> (Fw16u)(-scale))))) + 1;
					case 0:
						CBL_SSE2::Load_1x8_16s(sqrCap.i, maxVal);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqr_16s_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqr_16u_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Sqr_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &min32s, XMM128 &negbmask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}

					Common::InitMisc::Min::Of32s(min32s);
					Common::InitMisc::Constants::negbmask16s(negbmask);
				}

				ISV Sqr_32fc_setup(XMM128 &negbmask, XMM128 &half)
				{
					Common::InitMisc::Constants::negbmask32f(negbmask);
					Common::InitMisc::Constants::half32f(half);
				}

				ISV Sqr_64fc_setup(XMM128 &negbmask, XMM128 &half)
				{
					Common::InitMisc::Constants::negbmask64f(negbmask);
					Common::InitMisc::Constants::half64f(half);
				}
			}

			namespace AC4
			{
				template <int scaleType>
				ISV Sqr_8u_setup(int scale, XMM128 scaleMasks[], XMM128 &sqrCap, XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);

					Fw16u	maxVal = 16;
					switch (scaleType)
					{
					case -1:
						maxVal	= ((Fw16u) sqrt( float(((maxVal * maxVal) >> (Fw16u)(-scale))))) + 1;
					case 0:
						CBL_SSE2::Load_1x8_16s(sqrCap.i, maxVal);
						break;
					default:;
					}
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV Sqr_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV Sqr_16u_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For16(ac4mask);
				}

				ISV Sqr_32f_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For32(ac4mask);
				}
			}
		}

		namespace Unit
		{
			namespace C1
			{
				template <int scaleType>
				ISV Sqr_8u_unit( const XMM128& src,  XMM128& dst, const XMM128 scaleMasks[], const XMM128 &sqrCap)
				{
					// TODO: Move into CBL under appropriate function name and cleaner
					__m128i srcLo = src.i, srcHi, dstLo, dstHi;
					CBL_SSE2::Unpack8UTo16S(srcLo, srcHi);

					switch (scaleType)		// in case of neg scale, cap to max val
					{
					case 0:
					case -1:
						srcLo	= _mm_min_epi16(srcLo, sqrCap.i);
						srcHi	= _mm_min_epi16(srcHi, sqrCap.i);
						break;
					default:;
					}

					dstLo		= _mm_mullo_epi16(srcLo, srcLo);
					dstHi		= _mm_mullo_epi16(srcHi, srcHi);

					dstLo	= CBL_SSE2::Scale_16s<scaleType>(dstLo, (__m128i*) scaleMasks);	// scale (lo)
					dstHi	= CBL_SSE2::Scale_16s<scaleType>(dstHi, (__m128i*) scaleMasks);	// scale (hi)

					dst.i	= _mm_packus_epi16(dstLo, dstHi);								// pack
				}


				template <int scaleType>
				ISV Sqr_16s_unit( const XMM128& src,  XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SQRX_SSE2::SQR::Sqr_16s(src, dst, scale);
						break;
					case 0:
						SQRX_SSE2::SQR::Sqr_16s_SZ(src, dst);
						break;
					}
				}

				template <int scaleType>
				ISV Sqr_16u_unit( const XMM128& src,  XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SQRX_SSE2::SQR::Sqr_16u(src, dst, scale);
						break;
					case 0:
						SQRX_SSE2::SQR::Sqr_16u_SZ(src, dst);
						break;
					}
				}

				template <int scaleType>
				ISV Sqr_16sc_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scale, const XMM128 &min32s, const XMM128 &negb)
				{
					switch (scaleType)
					{
						case 1:
							Sqr_16sc    (src, src, dst, scale, min32s.i, negb.i);
							break;
						case -1:
							Sqr_16sc_SL (src, src, dst, scale, min32s.i, negb.i);
							break;
						case 0:
							Sqr_16sc_SZ (src, src, dst, min32s.i, negb.i);
							break;
					default:;
					}
				}

				ISV Sqr_32fc_unit( const XMM128& src,  XMM128& dst)
				{
					dst.f = CBL_SSE2::Multiply_32fc(src.f, src.f);
				}

				ISV Sqr_64fc_unit( const XMM128& src,  XMM128& dst)
				{
					dst.d = CBL_SSE2::Multiply_64fc(src.d, src.d);
				}
			}
		}
	}
}

} // namespace OPT_LEVEL


#endif	// __SQRTIMPL_H__

