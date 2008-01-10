/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __MULIMPL_H__
#define __MULIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"


namespace OPT_LEVEL
{

namespace MUL_REF
{
	template<class TS, class TD>
	SYS_INLINE STATIC void Mul_Scale_Sat(const TS &s1, const TS &s2, TD &d, int scale)
	{
		d = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s1 * s2, scale) );
	}
	
	SYS_INLINE STATIC void Mul_Scale_Sat(const A16S &s1, const A16S &s2, A32S &d, int scale)
	{
		d = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( (double)s1 * s2, scale)));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const A32S &s1, const A32S &s2, A32S &d, int scale)
	{
		d = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( (double)s1 * s2, scale) )	);
	}
	SYS_INLINE STATIC void Mul_Scale_Sat(const A32F &s1, const A32F &s2, A16S &d, int scale)
	{
		d = FW_REF::Limits<A16S>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(s1 * s2, scale)));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw16s &s1, const Fw16s &s2, Fw16s &d, int scale)
	{
		d = FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale((float)((float)s1 * s2), scale)));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw16sc &s1, const Fw16sc &s2, Fw16sc &d, int scale)
	{
		Fw32fc src1, src2, dst ;
		Fw64sc tmp;

		src1.re = (Fw32f)s1.re ;		src1.im = (Fw32f)s1.im ;
		src2.re = (Fw32f)s2.re ; 		src2.im = (Fw32f)s2.im ;

		dst = FW_REF::Scale(src1 * src2, scale);

		tmp.re = CBL_LIBRARY::DoubleToLong(dst.re);
		tmp.im = CBL_LIBRARY::DoubleToLong(dst.im);

		d = FW_REF::Limits<Fw16sc>::Sat( tmp );
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw32sc &s1, const Fw32sc &s2, Fw32sc &d, int scale)
	{
		scale;
		Fw64fc src1, src2, dst ;

		src1.re = (Fw64f)s1.re ;		src1.im = (Fw64f)s1.im ;
		src2.re = (Fw64f)s2.re ; 		src2.im = (Fw64f)s2.im ;

		dst.re = (src1.re*src2.re) - (src1.im*src2.im);
		dst.im = (src1.re*src2.im) + (src1.im*src2.re);

		dst = FW_REF::Scale(dst, scale);

		d.re =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.re));
		d.im =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.im));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw32s &s1, const Fw32sc &s2, Fw32sc &d, int scale)
	{
		Fw64fc  src2, dst ;

		src2.re = (Fw64f)s2.re ; 		src2.im = (Fw64f)s2.im ;

		src2.re = src2.re * s1;
		src2.im = src2.im * s1;
		
		dst = FW_REF::Scale(src2, scale);

		d.re =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.re));
		d.im =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.im));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw32s &s1,Fw32sc &d, int scale)
	{
		Fw64fc  src2, dst ;

		src2.re = (Fw64f)d.re ; 		src2.im = (Fw64f)d.im ;

		src2.re *= s1;
		src2.im *= s1;

		dst = FW_REF::Scale(src2, scale);

		d.re =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.re));
		d.im =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.im));
	}

	template<class TS, class TD>
	SYS_INLINE STATIC void Mul_Scale_Sat(const TS &s, TD &d, int scale)
	{
		d = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s*d, scale) );
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const A32S &s, A32S &d, int scale)
	{
		d = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale( (double)s*d, scale) ));
	}

	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw32sc &s, Fw32sc &d, int scale)
	{
		Fw64fc src1, src2, dst ;

		src1.re = (Fw64f)s.re ;		src1.im = (Fw64f)s.im ;
		src2.re = (Fw64f)d.re ; 		src2.im = (Fw64f)d.im ;

		dst.re = (src1.re*src2.re) - (src1.im*src2.im);
		dst.im = (src1.re*src2.im) + (src1.im*src2.re);

		dst = FW_REF::Scale(dst, scale);

		d.re =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.re));
		d.im =  FW_REF::Limits<Fw32s>::Sat(CBL_LIBRARY::DoubleToLong(dst.im));

	}


	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw16s &s,  Fw16s &d, int scale)
	{
		d = FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale((float)((float)s * d), scale) ));
	}



	SYS_INLINE STATIC void Mul_Scale_Sat(const Fw16sc &s, Fw16sc &d, int scale)
	{
		Fw32fc src, dst ;
		Fw32sc tmp;

		src.re = (Fw32f)s.re ;		src.im = (Fw32f)s.im ;
		dst.re = (Fw32f)d.re ; 	dst.im = (Fw32f)d.im ;

		dst = FW_REF::Scale(src * dst, scale) ;
		tmp.re = CBL_LIBRARY::FloatToInt(dst.re);
		tmp.im = CBL_LIBRARY::FloatToInt(dst.im);

		d = FW_REF::Limits<Fw16sc>::Sat( tmp );
	}

	SYS_INLINE STATIC void MulComF(const Fw32fc  &s1,const Fw32fc  &s2,Fw32fc &d)
	{
		Fw32fc dest;
		dest.re = s1.re * s2.re - s1.im * s2.im;
		dest.im = (s1.re * s2.im) + (s1.im * s2.re);
		d = dest;
	}

	SYS_INLINE STATIC void MulComF(const Fw64fc  &s1,const Fw64fc  &s2,Fw64fc &d)
	{
		Fw64fc dest;
		dest.re = s1.re * s2.re - s1.im * s2.im;
		dest.im = (s1.re * s2.im) + (s1.im * s2.re);
		d = dest;
	}

	namespace MUL
	{
		template< class TS1, class TS2, class TD >
		ISV MulF( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = (TD)(s1[3]* s2[3]);
			case C3:
			case AC4:	d[2] = (TD)(s1[2]* s2[2]);
						d[1] = (TD)(s1[1]* s2[1]);
			case C1:	d[0] = (TD)(s1[0]* s2[0]);
			}
		}

		ISV MulF( const Fw32fc * s1, CH cs1, const Fw32fc * s2, CH cs2, Fw32fc * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	MulComF(s1[3],s2[3],d[3]);
			case C3:
			case AC4:	MulComF(s1[2],s2[2],d[2]);
						MulComF(s1[1],s2[1],d[1]);
			case C1:	MulComF(s1[0],s2[0],d[0]);
			}
		}

		template< class TS1, class TS2, class TD >
		ISV MulI( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	Mul_Scale_Sat(s1[3], s2[3], d[3], scale);
			case C3:
			case AC4:	Mul_Scale_Sat(s1[2], s2[2], d[2], scale);
						Mul_Scale_Sat(s1[1], s2[1], d[1], scale);
			case C1:	Mul_Scale_Sat(s1[0], s2[0], d[0], scale);
			}
		}

		ISV MulI( const A16U * s1, CH cs1, const A16S * s2, CH cs2, A16S * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2,cd;

			*d = FW_REF::Limits<A16S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale((double)*s1 * *s2,scale) ));
		}

	}

	namespace MULC
	{
		template< class TS, class TD >
		ISV MulCI_1( const TS * s, CH cs, TD * d, CH cd, const TS v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			Mul_Scale_Sat(*s, v, *d, scale);//*d = FW_REF::Limits<TD>::Sat( FW_REF::Scale(*s * v, scale ) );
		}

		ISV MulCI_1( const A32SC * s, CH cs, A32SC * d, CH cd, A32SC v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			Mul_Scale_Sat(*s, v, *d, scale);//*d = FW_REF::Limits<TD>::Sat( FW_REF::Scale(*s * v, scale ) );
		}

		template< class TS, class TD >
		ISV MulCI( const TS * s, CH cs, TD * d, CH cd, const TD *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	Mul_Scale_Sat(s[3], v[3], d[3], scale);//d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[3] * v[3], scale ));
			case C3:
			case AC4:	Mul_Scale_Sat(s[2], v[2], d[2], scale);//d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[2] * v[2], scale ));
						Mul_Scale_Sat(s[1], v[1], d[1], scale);//d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[1] * v[1], scale ));
			case C1:	Mul_Scale_Sat(s[0], v[0], d[0], scale);//d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[0] * v[0], scale ));
			}
		}

		template< class TS, class TD >
		ISV MulCF_1( const TS * s, CH, TD * d, CH, const TS val )
		{
			d[0] = FW_REF::Limits<TD>::Sat((double)s[0] * val );
		}

		ISV MulCF_1( const A32FC * s, CH, A32FC * d, CH cd, Fw32fc val )
		{
			assert( 1 == ChannelCount(cd) ); cd;
			MulComF(s[0], val, d[0]);
		}

		ISV MulCF_1( const A64FC * s, CH, A64FC * d, CH cd, Fw64fc val )
		{
			assert( 1 == ChannelCount(cd) ); cd;
			MulComF(s[0], val, d[0]);
		}
	
		template< class TS, class TD >
		ISV MulCF( const TS * s, CH cs, TD * d, CH cd, const TD *v )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = (TD)((s[3] * v[3]) );
			case C3:
			case AC4:	d[2] = (TD)((s[2] * v[2]) );
						d[1] = (TD)((s[1] * v[1]) );
			case C1:	d[0] = (TD)((s[0] * v[0]) );
			}
		}

		ISV MulCF( const Fw32fc * s, CH cs, Fw32fc * d, CH cd, const Fw32fc *v )
		{
			assert( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	MulComF(s[3], v[3], d[3]);
			case C3:						  
			case AC4:	MulComF(s[2], v[2], d[2]);
						MulComF(s[1], v[1], d[1]);
			case C1:	MulComF(s[0], v[0], d[0]);
			}
		}
	}

	namespace MULXSCALE
	{

		template< class TS1, class TS2, class TD >
		ISV MulScale( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1, cs2;
			ASSERT( ChannelCount(cs1) == ChannelCount(cd ) );

			TD max = FW_REF::Limits< TD >::MaxValue();
			switch( cd )
			{
			case C4:	d[3] = (TD)(((unsigned int)s1[3] * (unsigned int)s2[3]) / max);
			case C3:
			case AC4:	d[2] = (TD)(((unsigned int)s1[2] * (unsigned int)s2[2]) / max);
						d[1] = (TD)(((unsigned int)s1[1] * (unsigned int)s2[1]) / max);
			case C1:	d[0] = (TD)(((unsigned int)s1[0] * (unsigned int)s2[0]) / max);
			}
		}

		template< class TS, class TD >
		ISV MulCScale_1( TS * s, CH cs, TD * d, CH cd, TS v )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd;
			ASSERT( 1 == ChannelCount(cd) );

			TD max = FW_REF::Limits< TD >::MaxValue();
			*d = FW_REF::Limits<TD>::Sat (((float)v * *s) / (float)max);
		}

		template< class TS, class TD >
		ISV MulCScale( TS * s, CH cs, TD * d, CH cd, TS *v  )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			TD max = FW_REF::Limits< TD >::MaxValue();
			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat (((float)s[3] * v[3]) / (float)max);
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat (((float)s[2] * v[2]) / (float)max);
						d[1] = FW_REF::Limits<TD>::Sat (((float)s[1] * v[1]) / (float)max);
			case C1:	d[0] = FW_REF::Limits<TD>::Sat (((float)s[0] * v[0]) / (float)max);
			}
		}

	}

}



namespace MUL_SSE2
{
	namespace MUL
	{

		// -- misc functions
		ISV Multiply_16s(const XMM128 &src1, const XMM128 &src2, XMM128 &dest, const XMM128 &scale)
		{
 			XMM128 temp_f_l, temp_f_h;
 			XMM128 src_hi,src;
 			__m128i srt_lo;
 			srt_lo = _mm_mullo_epi16 (src1.i,src2.i);
 			src_hi.i = _mm_mulhi_epi16 (src1.i,src2.i);
 			src.i  = _mm_unpacklo_epi16(srt_lo,src_hi.i ); // lower 4
 			src_hi.i = _mm_unpackhi_epi16 (srt_lo,src_hi.i); // uper 4

 			temp_f_l.f = _mm_cvtepi32_ps  (src.i);
 			temp_f_h.f = _mm_cvtepi32_ps  (src_hi.i);

 			temp_f_l.f	= _mm_mul_ps(temp_f_l.f,scale.f);
 			temp_f_h.f	= _mm_mul_ps(temp_f_h.f,scale.f);
			dest.i = CBL_SSE2::Pack32FTo16S(temp_f_l.f,temp_f_h.f);
		}

		ISV Multiply_16s_SZ(const XMM128 &s1, const XMM128 &s2, XMM128 &dest)
		{
 			 XMM128 src1,src2;
			 src1.i = _mm_mullo_epi16 (s1.i,s2.i);
			 src2.i = _mm_mulhi_epi16 (s1.i,s2.i);
			 dest.i  = _mm_unpacklo_epi16(src1.i,src2.i); 
			 src1.i=_mm_unpackhi_epi16(src1.i,src2.i); 
			 FW_SSE2::pack32STo16S(dest.i,src1.i);
		}

		ISV Multiply_16s_SL(const XMM128 &s1, const XMM128 &s2, XMM128 &dest, const XMM128  &scale)
		{
			XMM128 src_h_1;
			XMM128 src1;
			XMM128 src2;

			 src1.i = _mm_mullo_epi16(s1.i,s2.i);
			 src2.i = _mm_mulhi_epi16(s1.i,s2.i); 

			 src_h_1.i =  _mm_unpacklo_epi16(src1.i,src2.i);
			 src2.i   =  _mm_unpackhi_epi16(src1.i,src2.i); 

			 src1.i = _mm_packs_epi32( src_h_1.i,src2.i);  

			 src2.i = _mm_mullo_epi16(src1.i,scale.i);
			 src1.i = _mm_mulhi_epi16(src1.i,scale.i); 

			 src_h_1.i =  _mm_unpacklo_epi16(src2.i,src1.i);
			 src2.i   =  _mm_unpackhi_epi16(src2.i,src1.i); 
			
			 dest.i = _mm_packs_epi32( src_h_1.i,src2.i);  
		}


		SYS_INLINE STATIC void  Multiply_16sc_SZ(const XMM128 &s1,const XMM128 &s2,XMM128 &dest, const __m128i &min32s, const __m128i &negb)
		{
			dest.i = CBL_SSE2::Multiply_16sc(s1.i, s2.i, min32s, negb);
		}

		SYS_INLINE  void  Multiply_16sc(const XMM128 &s1, const XMM128 &s2, XMM128 &dest, const XMM128 &scale, const __m128i &min32s, const __m128i &negb)
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

		SYS_INLINE  void  Multiply_16sc_SL(const XMM128 &s1, const XMM128 &s2, XMM128 &dest, const XMM128 &scale, const __m128i &min32s, const __m128i &negb)
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

		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV Mul_8u_setup(int scale, XMM128 scaleMasks[])
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
				}

				template <int scaleType>
				ISV Mul_16s_setup(int scale, XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case -1:
						Common::InitScale::To16s(scale, scaleReg);
						break;
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
				}

				template <int scaleType>
				ISV Mul_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &min32s, XMM128 &negbmask)
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

				ISV Mul_32sc_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}

				ISV Mul_16u16s_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To32f(scale, scaleReg);
				}

				ISV Mul_32fc_setup(XMM128 &negamask)
				{
					Common::InitMisc::Constants::negamask32f(negamask);
				}
			}

			namespace AC4
			{
				template <int scaleType>
				ISV Mul_8u_setup(int scale, XMM128 scaleMasks[], XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV Mul_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
						Common::InitScale::To16s(scale, scaleReg);
						break;
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV Mul_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &min32s, XMM128 &negbmask, XMM128 &ac4mask)
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
					Common::AC4::Init::For32(ac4mask);
				}

				ISV Mul_32sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Mul_32fc_setup(XMM128 &negamask, XMM128 &ac4mask)
				{
					Common::AC4::Init::For64(ac4mask);
					Common::InitMisc::Constants::negamask32f(negamask);
				}

				ISV Mul_32f_setup(XMM128 &ac4mask)
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
				ISV Mul_8u_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 *scaleMasks)
				{
						__m128i tlo, thi;

                        __m128i srcLo, srcHi, src2Lo, src2Hi;

						const __m128i zero =  _mm_setzero_si128();

	                    srcHi   = _mm_unpackhi_epi8( src1.i, zero );
	                    srcLo   = _mm_unpacklo_epi8( src1.i, zero );

                        src2Hi  = _mm_unpackhi_epi8( src2.i, zero );
	                    src2Lo  = _mm_unpacklo_epi8( src2.i, zero );

                        srcLo	= _mm_mullo_epi16(srcLo, src2Lo);
                        srcHi	= _mm_mullo_epi16(srcHi, src2Hi);

 

						switch (scaleType)
						{
						case 0:
							srcLo = CBL_SSE2::Scale_16s<scaleType>(srcLo, (__m128i*) scaleMasks);
							srcHi = CBL_SSE2::Scale_16s<scaleType>(srcHi, (__m128i*) scaleMasks);

							srcLo	= _mm_xor_si128(_mm_srai_epi16(srcLo, 15), srcLo);
							srcHi	= _mm_xor_si128(_mm_srai_epi16(srcHi, 15), srcHi);
							break;

                        case 1://;
							
                            __m128i mask;
                            mask	= _mm_and_si128(srcLo,  scaleMasks[2].i);		// 111 mask (for scale 2)
		                    mask	= _mm_cmpgt_epi16(mask, scaleMasks[1].i);		// 010		(for scale 2)

		                    srcLo	= _mm_srl_epi16(srcLo, scaleMasks[0].i);		// rsh by scale - 1
		                    srcLo	= _mm_sub_epi16(srcLo, mask);				// i.e., if mask true, then +1

		                    srcLo =	  _mm_srli_epi16(srcLo, 1);					// rsh one more to make total rsh by scale

                            mask	= _mm_and_si128(srcHi,  scaleMasks[2].i);		// 111 mask (for scale 2)
		                    mask	= _mm_cmpgt_epi16(mask, scaleMasks[1].i);		// 010		(for scale 2)

		                    srcHi	= _mm_srl_epi16(srcHi, scaleMasks[0].i);		// rsh by scale - 1
		                    srcHi	= _mm_sub_epi16(srcHi, mask);				// i.e., if mask true, then +1

		                    srcHi =	  _mm_srli_epi16(srcHi, 1);					// rsh one more to make total rsh by scale
                            break;

						case -1:
							tlo		= _mm_unpacklo_epi16(srcLo, zero);
							thi		= _mm_unpackhi_epi16(srcLo, zero);

							tlo		= _mm_sll_epi32(tlo, scaleMasks[0].i);
							thi		= _mm_sll_epi32(thi, scaleMasks[0].i);

							srcLo	= _mm_packs_epi32(tlo, thi);

							tlo		= _mm_unpacklo_epi16(srcHi, zero);
							thi		= _mm_unpackhi_epi16(srcHi, zero);

							tlo		= _mm_sll_epi32(tlo, scaleMasks[0].i);
							thi		= _mm_sll_epi32(thi, scaleMasks[0].i);

							srcHi	= _mm_packs_epi32(tlo, thi);

							break;
						

						}
						dst.i	= _mm_packus_epi16(srcLo, srcHi);
				}

 
      


				template <int scaleType>
				ISV Mul_16s_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
						MUL_SSE2::MUL::Multiply_16s(src1, src2, dst, scale);
						break;
					case -1:
						MUL_SSE2::MUL::Multiply_16s_SL(src1, src2, dst, scale);
						break;
					case 0:
						MUL_SSE2::MUL::Multiply_16s_SZ(src1, src2, dst);
						break;
					}
				}

				template <int scaleType>
				ISV Mul_16sc_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128 &scale, const XMM128 &min32s, const XMM128 &negbmask)
				{
					switch (scaleType)
					{
					case 0:
						Multiply_16sc_SZ(src1, src2, dst, min32s.i, negbmask.i);
						break;
					case 1:
						Multiply_16sc(src1, src2, dst, scale, min32s.i, negbmask.i);
						break;
					case -1:
						Multiply_16sc_SL(src1, src2, dst, scale, min32s.i, negbmask.i);
						break;
					default:;
					}
				}

				ISV Mul_32sc_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
				__m128d sLo		= _mm_cvtepi32_pd(s1.i);
				__m128d sHi		= _mm_cvtepi32_pd(s2.i);

				__m128d rLo		= CBL_SSE2::Multiply_64fc(sLo, sHi);

						sLo		= _mm_cvtepi32_pd(_mm_srli_si128(s1.i, 8));
						sHi		= _mm_cvtepi32_pd(_mm_srli_si128(s2.i, 8));

				__m128d rHi		= CBL_SSE2::Multiply_64fc(sLo, sHi);

					rLo			= _mm_mul_pd(rLo, scale.d);
					rHi			= _mm_mul_pd(rHi, scale.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}

				ISV Mul_32s_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
				__m128d sLo		= _mm_cvtepi32_pd(s1.i);
				__m128d sHi		= _mm_cvtepi32_pd(s2.i);

				__m128d rLo		= _mm_mul_pd(sLo, sHi);

						sLo		= _mm_cvtepi32_pd(_mm_srli_si128(s1.i, 8));
						sHi		= _mm_cvtepi32_pd(_mm_srli_si128(s2.i, 8));

				__m128d rHi		= _mm_mul_pd(sLo, sHi);

					rLo			= _mm_mul_pd(rLo, scale.d);
					rHi			= _mm_mul_pd(rHi, scale.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}



				// Odd cases
				ISV Mul_16u16s_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					XMM128 temp_1_hi,temp_2_hi, src1, src2;
					src1 = s1; src2 = s2;

					CBL_SSE2::Unpack16UTo32U(src1.i,temp_1_hi.i);
					CBL_SSE2::Unpack16STo32S(src2.i,temp_2_hi.i);

					src1.f = _mm_cvtepi32_ps(src1.i);
					src2.f = _mm_cvtepi32_ps(src2.i);
					
					temp_1_hi.f = _mm_cvtepi32_ps(temp_1_hi.i);
					temp_2_hi.f = _mm_cvtepi32_ps(temp_2_hi.i);		

					src1.f = _mm_mul_ps(src1.f, src2.f);
					src1.f = _mm_mul_ps(src1.f, scale.f);
					temp_1_hi.f = _mm_mul_ps(temp_1_hi.f, temp_2_hi.f);
					temp_1_hi.f = _mm_mul_ps(temp_1_hi.f, scale.f);
					
					dst.i = CBL_SSE2::Pack32FTo16S(src1.f,temp_1_hi.f);
				}
				ISV Mul_16s32s_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					XMM128 temp_lo,temp_hi, src1, src2;
					src1 = s1; src2 = s2;

					temp_lo.i = _mm_mullo_epi16(src1.i,src2.i);	
					temp_hi.i = _mm_mulhi_epi16(src1.i,src2.i);	

					src1.i = _mm_unpacklo_epi16(temp_lo.i, temp_hi.i);

 					temp_lo.d = _mm_cvtepi32_pd(src1.i);
					src1.i = _mm_srli_si128(src1.i, 8);
 					src1.d = _mm_cvtepi32_pd(src1.i);

 					dst.d = _mm_mul_pd(temp_lo.d ,scale.d); // 
					src1.d= _mm_mul_pd(src1.d ,scale.d);

					dst.i = CBL_SSE2::Pack64FTo32S (dst.d, src1.d);
				}

				ISV Mul_32s32sc_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					XMM128 src1;
					XMM128 dest_lo,dest_hi;

					src1.i		= _mm_shuffle_epi32(s1.i, _MM_SHUFFLE(1, 1, 0, 0));

					CBL_OPT::Multiply_32s64f(src1.i, s2.i, dest_lo.d, dest_hi.d);

					dst.d		= _mm_mul_pd(dest_lo.d, scale.d);
					dest_hi.d	= _mm_mul_pd(dest_hi.d, scale.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(dst.d, dest_hi.d);
				}

				ISV Mul_8u16u_unit( const XMM128& s1, const XMM128& s2, XMM128& dst)
				{
					XMM128 temp_s1,temp_s2, src1, src2;
					src1 = s1; src2 = s2;

					const __m128i zero =  CONST_SETZERO_8I();
					temp_s1.i	= _mm_unpacklo_epi8( src1.i, zero );
					temp_s2.i	= _mm_unpacklo_epi8( src2.i, zero );
					dst.i		= _mm_mullo_epi16(temp_s1.i,temp_s2.i);
				}

				ISV Mul_16s32f_unit( const XMM128& s1, const XMM128& s2, XMM128& dst)
				{
					XMM128 temp_lo,temp_hi;
					temp_lo.i	= _mm_mullo_epi16(s1.i,s2.i);	
					temp_hi.i	= _mm_mulhi_epi16(s1.i,s2.i);	
					temp_hi.i	= _mm_unpacklo_epi16(temp_lo.i, temp_hi.i);
					dst.f		= _mm_cvtepi32_ps(temp_hi.i);
				}

				ISV Mul_32fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst, 
							const XMM128 &negamask)
				{
					dst.f = CBL_SSE2::Multiply_32fc_wm(src1.f, src2.f, negamask.f);
				}

				ISV Mul_64fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					dst.d = CBL_SSE2::Multiply_64fc(src1.d, src2.d);
				}
			}
		}
	}

	namespace MULC
	{
		ISV Pack32fto16S(XMM128 &temp_f_l_1,XMM128 &dest)
		{
			static const __m128 f_val		= CONST_SET1_32F((float)0x7FFF);
			static const __m128 f_min_val	= CONST_SET1_32F(-(float)0x8000);

			temp_f_l_1.f = _mm_min_ps(temp_f_l_1.f,f_val);
			temp_f_l_1.f = _mm_max_ps(temp_f_l_1.f,f_min_val);
			dest.i =  _mm_cvtps_epi32 (temp_f_l_1.f);

			FW_SSE2::pack32STo16S(dest.i,dest.i);
		}

		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV MulC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC, XMM128 &val)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::InitConst::C1::To16u::From8u(valC, val);
				}

				template <int scaleType>
				ISV MulC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
						CBL_SSE2::Load_1x4_32f(scaleReg.f, 1.0);
					}
					XMM128 temp;
					FW_SSE2::Load1(temp, (float) valC);

					scaleReg.f	= _mm_mul_ps(scaleReg.f, temp.f);
				}

				template <int scaleType>
				ISV MulC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC, XMM128& val, XMM128 &min32s, XMM128 &negbmask)
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
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_32s_setup(int scale, XMM128 &scaleReg, Fw32s valC, XMM128 &val)
				{
					Common::InitScale::To64f(scale, scaleReg);
					FW_SSE2::Load1(val, (Fw64f) valC);
				}

				ISV MulC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC, XMM128 &val)
				{
					Common::InitScale::To64f(scale, scaleReg);
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_32f_setup(Fw32f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_64f_setup(Fw64f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_32fc_setup(Fw32fc valC, XMM128 &val, XMM128 &negamask)
				{
					Common::InitMisc::Constants::negamask32f(negamask);
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_64fc_setup(Fw64fc valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV MulC_32f16s_setup(int scale, XMM128 &scaleReg, Fw32f valC, XMM128 &val)
				{
					Common::InitScale::To32f(scale, scaleReg);
					FW_SSE2::Load1(val, valC);
				}
			}
			namespace C3
			{
				template <int scaleType>
				ISV MulC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::InitConst::C3::To16s::From8u(valC, val0, val1, val2);
				}

				template <int scaleType>
				ISV MulC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					switch (scaleType)
					{
					case -1:
						Common::InitScale::To16s(scale, scaleReg);
						break;
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::InitConst::C3::To16s::From16s(valC, val0, val1, val2);
				}

				template <int scaleType>
				ISV MulC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2, XMM128 &min32s, XMM128 &negbmask)
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
					Common::InitConst::C3::To16sc::From16sc(valC, val0, val1, val2);
				}

				ISV MulC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::C3::To32sc::From32sc(valC, val0, val1, val2);
				}

				ISV MulC_32f_setup(Fw32f valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitConst::C3::To32f::From32f(valC, val0, val1, val2);
				}

				ISV MulC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2, XMM128 &negamask)
				{
					Common::InitMisc::Constants::negamask32f(negamask);
					Common::InitConst::C3::To32fc::From32fc(valC, val0, val1, val2);
				}
			}
			namespace C4
			{
				template <int scaleType>
				ISV MulC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::InitConst::C4::To16u::From8u(valC, val);
				}

				template <int scaleType>
				ISV MulC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[])
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
						CBL_SSE2::Load_1x4_32f(scaleReg.f, 1.0);
					}
					XMM128 temp;
					Common::InitConst::C4::To32f::From16s(valC, temp);

					scaleReg.f	= _mm_mul_ps(scaleReg.f, temp.f);
				}

				template <int scaleType>
				ISV MulC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128& val, XMM128 &min32s, XMM128 &negbmask)
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
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}

				ISV MulC_32f_setup(Fw32f valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}
			}
			namespace AC4
			{
				template <int scaleType>
				ISV MulC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val, XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::InitConst::AC4::To16s::From8u(valC, val);
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV MulC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
						CBL_SSE2::Load_1x4_32f(scaleReg.f, 1.0);
					}
					XMM128 temp;
					Common::InitConst::AC4::To32f::From16s(valC, temp);

					scaleReg.f	= _mm_mul_ps(scaleReg.f, temp.f);
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV MulC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val, XMM128 &min32s, XMM128 &negbmask, XMM128 &ac4mask)
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
					Common::InitConst::AC4::To16sc::From16sc(valC, val);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV MulC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::AC4::To32sc::From32sc(valC, val0, val1);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV MulC_32f_setup(Fw32f valC[], XMM128 &val, XMM128 &ac4mask)
				{
					Common::InitConst::AC4::To32f::From32f(valC, val);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV MulC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &negamask, XMM128 &ac4mask)
				{
					Common::InitMisc::Constants::negamask32f(negamask);
					Common::InitConst::AC4::To32fc::From32fc(valC, val0, val1);
					Common::AC4::Init::For64(ac4mask);
				}
			}
		}

		namespace Unit
		{
			namespace C1
			{
				template <int scaleType>
				ISV MulC_8u_unit(const XMM128 &src, const XMM128 &valClo, const XMM128 &valChi, XMM128 &dst, const XMM128 scaleMasks[])
				{
					__m128i srcLo = src.i, srcHi, tlo, thi, zero;

					srcHi = CBL_SSE2::Multiply_8u16s_Lo(srcLo, valClo.i, valChi.i);

					switch (scaleType)
					{
					case 0:
						srcLo	= _mm_xor_si128(_mm_srai_epi16(srcLo, 15), srcLo);
						srcHi	= _mm_xor_si128(_mm_srai_epi16(srcHi, 15), srcHi);
						dst.i	= _mm_packus_epi16(srcLo, srcHi);
						break;
					case -1:
						zero	= _mm_setzero_si128();
						tlo		= _mm_unpacklo_epi16(srcLo, zero);
						thi		= _mm_unpackhi_epi16(srcLo, zero);

						tlo		= _mm_sll_epi32(tlo, scaleMasks[0].i);
						thi		= _mm_sll_epi32(thi, scaleMasks[0].i);

						srcLo	= _mm_packs_epi32(tlo, thi);

						tlo		= _mm_unpacklo_epi16(srcHi, zero);
						thi		= _mm_unpackhi_epi16(srcHi, zero);

						tlo		= _mm_sll_epi32(tlo, scaleMasks[0].i);
						thi		= _mm_sll_epi32(thi, scaleMasks[0].i);

						srcHi	= _mm_packs_epi32(tlo, thi);

						dst.i	= _mm_packus_epi16(srcLo, srcHi);
						break;
					default:;
						srcLo	= CBL_SSE2::Scale_16s<scaleType>(srcLo, (__m128i*) scaleMasks);
						srcHi	= CBL_SSE2::Scale_16s<scaleType>(srcHi, (__m128i*) scaleMasks);
						dst.i	= _mm_packus_epi16(srcLo, srcHi);
					}
				}


				template <int scaleType>
				ISV MulC_16s_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scaleReg)
				{
					XMM128 srcLo, srcHi;

					const __m128i signext = _mm_srai_epi16(src.i, 15);

					srcLo.i		= _mm_unpacklo_epi16( src.i, signext );
					srcHi.i		= _mm_unpackhi_epi16( src.i, signext );

					srcLo.f		= _mm_cvtepi32_ps(srcLo.i);
					srcHi.f		= _mm_cvtepi32_ps(srcHi.i);

					srcLo.f		= _mm_mul_ps(srcLo.f, scaleReg.f);
					srcHi.f		= _mm_mul_ps(srcHi.f, scaleReg.f);

					srcLo.i		= _mm_cvtps_epi32(srcLo.f);
					srcHi.i		= _mm_cvtps_epi32(srcHi.f);

					dst.i		= _mm_packs_epi32(srcLo.i, srcHi.i);
				}

				template <int scaleType>
				ISV MulC_16sc_unit( const XMM128 &src, const XMM128 &val, XMM128 &dst, const XMM128 &scale, const XMM128 &min32s, const XMM128 &negbmask)
				{
					switch (scaleType)
					{
					case 0:
						MUL_SSE2::MUL::Multiply_16sc_SZ(src, val, dst, min32s.i, negbmask.i);
						break;
					case 1:
						MUL_SSE2::MUL::Multiply_16sc(src, val, dst, scale, min32s.i, negbmask.i);
						break;
					case -1:
						MUL_SSE2::MUL::Multiply_16sc_SL(src, val, dst, scale, min32s.i, negbmask.i);
						break;
					default:;
					}
				}

				ISV MulC_32s_unit( const XMM128& s, const XMM128& val, XMM128& dst, const XMM128 &scale)
				{
					XMM128 srcTemp1_1;
					XMM128 srcTemp1_2;

					srcTemp1_1.d = _mm_cvtepi32_pd(s.i);
					srcTemp1_2.i = _mm_srli_si128(s.i, 8);
					srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
					
					srcTemp1_1.d = _mm_mul_pd(srcTemp1_1.d, val.d);
					srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, val.d);
					
					dst.d = _mm_mul_pd(srcTemp1_1.d, scale.d);
					srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, scale.d);

					dst.i = CBL_SSE2::Pack64FTo32S(dst.d, srcTemp1_2.d);
				}

				ISV MulC_32sc_unit( const XMM128& s, const XMM128& val, XMM128& dst, const XMM128 &scale)
				{
				__m128d sLo		= _mm_cvtepi32_pd(s.i);
				__m128d sHi		= _mm_cvtepi32_pd(val.i);

				__m128d rLo		= CBL_SSE2::Multiply_64fc(sLo, sHi);

						sLo		= _mm_cvtepi32_pd(_mm_srli_si128(s.i, 8));
						sHi		= _mm_cvtepi32_pd(_mm_srli_si128(val.i, 8));

				__m128d rHi		= CBL_SSE2::Multiply_64fc(sLo, sHi);

					rLo			= _mm_mul_pd(rLo, scale.d);
					rHi			= _mm_mul_pd(rHi, scale.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}

				ISV MulC_32fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst, 
							const XMM128 &negamask)
				{
					dst.f = CBL_SSE2::Multiply_32fc_wm(src.f, val.f, negamask.f);
				}

				ISV MulC_64fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.d = CBL_SSE2::Multiply_64fc(src.d, val.d);
				}
			}
		}
	}

	namespace MULXSCALE
	{
		ISV MulScale_Mul_16s( const XMM128& src1, const XMM128& src2, XMM128& dst)
		{
			XMM128 tempsrc1_0, tempsrc1_1;

			tempsrc1_0 = src1;
			CBL_SSE2::Unpack16UTo32U(tempsrc1_0.i,tempsrc1_1.i);

			tempsrc1_0.f = _mm_cvtepi32_ps (tempsrc1_0.i);
			tempsrc1_0.f = _mm_mul_ps( tempsrc1_0.f, src2.f );
			dst.i = _mm_cvttps_epi32(tempsrc1_0.f);

			tempsrc1_1.f = _mm_cvtepi32_ps (tempsrc1_1.i);
			tempsrc1_1.f = _mm_mul_ps( tempsrc1_1.f, src2.f );
			tempsrc1_1.i = _mm_cvttps_epi32(tempsrc1_1.f);

			FW_SSE2::pack32STo16S(dst.i,tempsrc1_1.i);

		}
		ISV MulScale_Div_16u( const XMM128& src1, const XMM128& src2, XMM128& dst)
		{
			XMM128 s1 = src1, s2 = src2;
			XMM128 hi_1,hi_2,lo_1,lo_2;

			static const __m128i one = CONST_SET1_16I(1);

			CBL_SSE2::Unpack8UTo16U( s1.i, hi_1.i );		// Convert to 16 bit, 2 regs
			CBL_SSE2::Unpack8UTo16U( s2.i, hi_2.i );

			lo_1.i = _mm_mullo_epi16( s1.i, s2.i );	// low  8 shorts
			hi_1.i = _mm_mullo_epi16( hi_1.i, hi_2.i );	// high 8 shorts

			//lo and hi contains the multiplied results
			lo_2.i = _mm_srli_epi16(lo_1.i, 8);//x--shift8-->a, x+a+1 --shift8=>x/max
			hi_2.i = _mm_srli_epi16(hi_1.i, 8);

			lo_1.i = _mm_adds_epu16(lo_1.i,lo_2.i);
			hi_1.i = _mm_adds_epu16(hi_1.i,hi_2.i);

			lo_1.i = _mm_adds_epu16(lo_1.i,one);
			hi_1.i = _mm_adds_epu16(hi_1.i,one);

			dst.i = _mm_srli_epi16(lo_1.i, 8);
			hi_1.i = _mm_srli_epi16(hi_1.i, 8);

			FW_SSE2::Pack16Uto8( dst, hi_1 );
		}

		ISV MulScale_Div_32u( const XMM128& s1, const XMM128& s2,XMM128 &dst)
		{
			XMM128 src1 = s1, src2 = s2;
			XMM128 tempsrc1_0, tempsrc1_1;
			XMM128 lo,hi;
			XMM128 lo1, hi1;
			static const __m128i one = CONST_SET1_32I(1);
			
			tempsrc1_1.i = _mm_mulhi_epu16(src1.i, src2.i);
			tempsrc1_0.i = _mm_mullo_epi16(src1.i, src2.i);

			lo.i = _mm_unpacklo_epi16(tempsrc1_0.i, tempsrc1_1.i);//lo only
			hi.i = _mm_unpackhi_epi16(tempsrc1_0.i, tempsrc1_1.i);

			//lo and hi contains the multiplied results
			lo1.i = _mm_srli_epi32(lo.i, 16);//x--shift16-->a, x+a+1 --shift16=>x/max
			hi1.i = _mm_srli_epi32(hi.i, 16);

			lo.i = _mm_add_epi32(lo.i,lo1.i);
			hi.i = _mm_add_epi32(hi.i,hi1.i);

			lo.i = _mm_add_epi32(lo.i,one);
			hi.i = _mm_add_epi32(hi.i,one);

			lo.i = _mm_srli_epi32(lo.i, 16);
			hi.i = _mm_srli_epi32(hi.i, 16);
			
			hi.i = _mm_slli_epi32(hi.i, 16);//32s to 16u
			lo.i = _mm_or_si128(lo.i, hi.i);
			
			lo.i = _mm_shufflelo_epi16(lo.i, _MM_SHUFFLE(3,1,2,0));
			lo.i = _mm_shufflehi_epi16(lo.i, _MM_SHUFFLE(3,1,2,0));
			dst.i = _mm_shuffle_epi32(lo.i, _MM_SHUFFLE(3,1,2,0));
		}

		ISV MulScale_Mul_16u( XMM128& src1, const XMM128& src2,XMM128& dst)
		{
			XMM128 tempsrc1_0, tempsrc1_1;

			tempsrc1_0 = src1;

            tempsrc1_0.i = _mm_mullo_epi16(src1.i, src2.i);
            src1.i = _mm_mulhi_epu16(src1.i, src2.i);

            tempsrc1_1.i = _mm_unpackhi_epi16(tempsrc1_0.i, src1.i);
            tempsrc1_0.i = _mm_unpacklo_epi16(tempsrc1_0.i, src1.i);

            tempsrc1_1.i = _mm_srli_epi32(tempsrc1_1.i, 16);
            tempsrc1_0.i = _mm_srli_epi32(tempsrc1_0.i, 16);


			tempsrc1_1.i = _mm_slli_epi32(tempsrc1_1.i, 16);
			tempsrc1_0.i = _mm_or_si128(tempsrc1_0.i, tempsrc1_1.i);
			
			tempsrc1_0.i = _mm_shufflelo_epi16(tempsrc1_0.i, _MM_SHUFFLE(3,1,2,0));
			tempsrc1_0.i = _mm_shufflehi_epi16(tempsrc1_0.i, _MM_SHUFFLE(3,1,2,0));
			dst.i	     = CBL_OPTLEVEL::Shuffle_3120_32s(tempsrc1_0.i);

		}

		ISV MulScale_Mul_16u_C3( const XMM128& src1, const XMM128& src2, const XMM128& src3, XMM128& dst )
		{
			XMM128 tempsrc1_0, tempsrc1_1;

			tempsrc1_0 = src1;
			CBL_SSE2::Unpack16UTo32U(tempsrc1_0.i,tempsrc1_1.i);

			tempsrc1_0.f = _mm_cvtepi32_ps (tempsrc1_0.i);
			tempsrc1_0.f = _mm_mul_ps( tempsrc1_0.f, src2.f );
			tempsrc1_0.i = _mm_cvttps_epi32(tempsrc1_0.f);

			tempsrc1_1.f = _mm_cvtepi32_ps (tempsrc1_1.i);
			tempsrc1_1.f = _mm_mul_ps( tempsrc1_1.f, src3.f );
			tempsrc1_1.i = _mm_cvttps_epi32(tempsrc1_1.f);

			tempsrc1_1.i = _mm_slli_epi32(tempsrc1_1.i, 16);
			tempsrc1_0.i = _mm_or_si128(tempsrc1_0.i, tempsrc1_1.i);
			
			tempsrc1_0.i = _mm_shufflelo_epi16(tempsrc1_0.i, _MM_SHUFFLE(3,1,2,0));
			tempsrc1_0.i = _mm_shufflehi_epi16(tempsrc1_0.i, _MM_SHUFFLE(3,1,2,0));
			dst.i		 = CBL_OPTLEVEL::Shuffle_3120_32s(tempsrc1_0.i);

		}

		ISV MulScale_Mul_16s_C3( const XMM128& src1, const XMM128& src2, const XMM128& src3, XMM128& dst)
		{
			XMM128 tempsrc1_0, tempsrc1_1;

			tempsrc1_0 = src1;
			CBL_SSE2::Unpack16UTo32U(tempsrc1_0.i,tempsrc1_1.i);

			tempsrc1_0.f = _mm_cvtepi32_ps (tempsrc1_0.i);
			tempsrc1_0.f = _mm_mul_ps( tempsrc1_0.f, src2.f );
			dst.i = _mm_cvttps_epi32(tempsrc1_0.f);

			tempsrc1_1.f = _mm_cvtepi32_ps (tempsrc1_1.i);
			tempsrc1_1.f = _mm_mul_ps( tempsrc1_1.f, src3.f );
			tempsrc1_1.i = _mm_cvttps_epi32(tempsrc1_1.f);

			FW_SSE2::pack32STo16S(dst.i,tempsrc1_1.i);

		}

		namespace Init
		{
			namespace C1
			{
				ISV MulCScale_8u(Fw8u valC, XMM128 &val)
				{
                    FW_SSE2::Load1(val, valC);
				}

				ISV MulCScale_16u(Fw16u valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}
			}
			namespace C3
			{
				ISV MulCScale_8u(Fw8u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					FW_SSE2::Load2(val0, valC, OPT_LEVEL::C3, 0);
                    FW_SSE2::Load2(val1, valC, OPT_LEVEL::C3, 1);
                    FW_SSE2::Load2(val2, valC, OPT_LEVEL::C3, 2);
				}

				ISV MulCScale_16u(Fw16u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					A16U *v = valC;
					A16U max = FW_REF::Limits< A16U >::MaxValue();
					A32F vNew[3];
					vNew[0] = (A32F)v[0]/(A32F)max;
					vNew[1] = (A32F)v[1]/(A32F)max;
					vNew[2] = (A32F)v[2]/(A32F)max;


					FW_SSE2::Load2(val0, vNew, OPT_LEVEL::C3, 0);//value for reg1										// 1 3 2 1
					val1.f = _mm_shuffle_ps(val0.f, val0.f, _MM_SHUFFLE(1, 0, 2, 1));	// 2 1 3 2
					val2.f = _mm_shuffle_ps(val0.f, val0.f, _MM_SHUFFLE(2, 1, 0, 2)); // 3 2 1 3
				}
			}

			namespace AC4
			{
				ISV MulCScale_8u(Fw8u valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::AC4, 0);
				}

				ISV MulCScale_16u(Fw16u valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::AC4, 0);
				}
			}

			namespace C4
			{
				ISV MulCScale_8u(Fw8u valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}

				ISV MulCScale_16u(Fw16u valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}
			}
		}

	}
}

template<class TSD, CH c, IsAlign ia>
ISV Mul_8u_unit_PosScale_Custom(const TSD *s1, const TSD *s2, TSD *d, U32 &pixCount, int /*scale*/, const XMM128* scaleMasks)
    {
    TSD  *end	  = d + pixCount * ChannelCount(c);
    XMM128 src1, src2;    
    const __m128i scaleMask0 = scaleMasks[0].i, scaleMask1 = scaleMasks[1].i, scaleMask2 = scaleMasks[2].i;
   
    for( ; d < end; s1+=16, s2+=16, d+=16)
        {
            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src1, (const void*)s1);
            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_TRUE>(&src2, (const void*)s2);

            __m128i srcHi, &src2Lo = src2.i , src2Hi;
            XMM128 &srcLo = src1;
						    
            srcHi   = _mm_unpackhi_epi8( srcLo.i, _mm_setzero_si128() );
            srcLo.i   = _mm_unpacklo_epi8( srcLo.i, _mm_setzero_si128() );

            src2Hi  = _mm_unpackhi_epi8( src2Lo, _mm_setzero_si128() );
            src2Lo  = _mm_unpacklo_epi8( src2Lo, _mm_setzero_si128() );

            srcLo.i	= _mm_mullo_epi16(srcLo.i, src2Lo);
            srcHi	= _mm_mullo_epi16(srcHi, src2Hi);
            
            __m128i mask;

    
            mask	= _mm_and_si128(srcLo.i,  scaleMask2);		// 111 mask (for scale 2)
            mask	= _mm_cmpgt_epi16(mask, scaleMask1);		// 010		(for scale 2)

            srcLo.i	= _mm_srl_epi16(srcLo.i, scaleMask0);		// rsh by scale - 1
            srcLo.i	= _mm_sub_epi16(srcLo.i, mask);				// i.e., if mask true, then +1

            srcLo.i =	  _mm_srli_epi16(srcLo.i, 1);					// rsh one more to make total rsh by scale

            mask	= _mm_and_si128(srcHi,  scaleMask2);		// 111 mask (for scale 2)
            mask	= _mm_cmpgt_epi16(mask, scaleMask1);		// 010		(for scale 2)

            srcHi	= _mm_srl_epi16(srcHi, scaleMask0);		// rsh by scale - 1
            srcHi	= _mm_sub_epi16(srcHi, mask);				// i.e., if mask true, then +1

            srcHi =	  _mm_srli_epi16(srcHi, 1);					// rsh one more to make total rsh by scale

            srcLo.i	= _mm_packus_epi16(srcLo.i, srcHi);
           
           //XMM128 &dst = srcLo;

           LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&srcLo, (void*)(d));
        }
    _mm_mfence();
    }


} // namespace OPT_LEVEL


#endif	// __MULIMPL_H__

