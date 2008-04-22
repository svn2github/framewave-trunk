/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __DIVIMPL_H__
#define __DIVIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"


namespace OPT_LEVEL
{

namespace DIV_REF
{
	template <class TS1, class TS2>
	SYS_INLINE STATIC Fw64f SafeDivI( const TS1 & a, const TS2 & b )
	{
		if (b != 0)
		{
			return (Fw64f) a / b;
		}
		else
		{
			if(!a)		  return 0;
			else if( a>0) return Const::INF_64F;
			else		  return Const::INF_NEG_64F;
		}
	}

	SYS_INLINE STATIC Fw32f SafeDiv( const Fw32f & a, const Fw32f & b )
	{
		if( !b )
		{
			if(!a)								return Const::NAN_32F;
			else if( a > FW_REF::To<Fw32f>::From(0))	return Const::INF_32F;
			else								return Const::INF_NEG_32F;
		}
		
		return a / b; 	
	}

	SYS_INLINE STATIC Fw64f SafeDiv( const Fw64f & a, const Fw64f & b )
	{
		if( !b )
		{
			if(!a)								return Const::NAN_64F;
			else if( a > FW_REF::To<Fw64f>::From(0))	return Const::INF_64F;
			else								return Const::INF_NEG_64F;
		}
		
		return a / b; 	
	}


	SYS_INLINE STATIC Fw32fc SafeDiv(const Fw32fc &s2, const Fw32fc &s1)
	{
		Fw32fc t_s1, t_s2, t_d, d ;

		if(!s1 )
		{
			d =  FW_REF::To< Fw32fc >::From( Const::INF_32F );
		}
		else{
			t_s1.re = s1.re ;  t_s1.im = s1.im ;
			t_s2.re = s2.re ;  t_s2.im = s2.im ;

			t_d.re = ( t_s2.re*t_s1.re + t_s2.im*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );
			t_d.im = ( t_s2.im*t_s1.re - t_s2.re*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );

			d = t_d;
		}
		return d;
	}

	SYS_INLINE STATIC Fw64fc SafeDiv(const Fw64fc &s2, const Fw64fc &s1)
	{
		Fw64fc t_s1, t_s2, t_d, d ;

		if(!s1 )
		{
			d =  FW_REF::To< Fw64fc >::From( Const::INF_64F );
		}
		else{
			t_s1.re = s1.re ;  t_s1.im = s1.im ;
			t_s2.re = s2.re ;  t_s2.im = s2.im ;

			t_d.re = ( t_s2.re*t_s1.re + t_s2.im*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );
			t_d.im = ( t_s2.im*t_s1.re - t_s2.re*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );

			d = t_d;
		}
		return d;
	}

	template <class TS1, class TS2, class TD>
	SYS_INLINE STATIC TD SafeDiv_scale(const TS1 &s2, const TS2 &s1, int scale)
	{
		if(!s1 || !s2 )
			return FW_REF::Limits<TD>::Sat(SafeDivI(s2, s1));
		else
			return FW_REF::Limits<TD>::Sat(CBL_LIBRARY::DoubleToInt(FW_REF::Scale(SafeDivI(s2, s1), scale)));
	}

	template <class TS, class TD>
	SYS_INLINE STATIC TD SafeDiv_scale(const TS &s2, const TD &s1, int scale)
	{
		if(!s1 || !s2 )
			return FW_REF::Limits<TD>::Sat(SafeDivI(s2, s1));
		else
			return FW_REF::Limits<TD>::Sat(CBL_LIBRARY::DoubleToInt(FW_REF::Scale(SafeDivI(s2, s1), scale)));
	}

	SYS_INLINE  STATIC Fw16sc SafeDiv_scale(const Fw16sc &s2, const Fw16sc &s1, int scale)
	{
		Fw32fc t_s1, t_s2, t_d ;
		Fw32sc inter_dest;
		Fw16sc d;

		if(!s1 )
		{
			d =  FW_REF::To< Fw16sc >::From( 0 );
		}
		else{
			t_s1.re = s1.re ;  t_s1.im = s1.im ;
			t_s2.re = s2.re ;  t_s2.im = s2.im ;

			t_d.re = ( t_s2.re*t_s1.re + t_s2.im*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );
			t_d.im = ( t_s2.im*t_s1.re - t_s2.re*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );

			inter_dest.re = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.re, scale));
			inter_dest.im = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.im, scale));

			d = FW_REF::Limits<Fw16sc>::Sat(inter_dest);
		}
		return d;
	}

	SYS_INLINE  STATIC Fw32sc SafeDiv_scale(const Fw32sc &s2, const Fw32sc &s1, int scale)
	{
		Fw64fc t_s1, t_s2, t_d ;
		Fw32sc d;

		if(!s1 )
		{
			d =  FW_REF::To< Fw32sc >::From( 0 );
		}
		else{
			t_s1.re = s1.re ;  t_s1.im = s1.im ;
			t_s2.re = s2.re ;  t_s2.im = s2.im ;

			t_d.re = ( t_s2.re*t_s1.re + t_s2.im*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );
			t_d.im = ( t_s2.im*t_s1.re - t_s2.re*t_s1.im ) / ( t_s1.re*t_s1.re + t_s1.im*t_s1.im );

			d.re = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.re, scale));
			d.im = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.im, scale));
		}
		return d;
	}


	template <class TS>  SYS_INLINE  STATIC
	TS SafeDivC_scale(const TS &s, const TS &v, int scale)
	{
		if(!v)
			return s ;
		else
			return DivC_scale(s, v, scale);
	}

	template <class TS>  SYS_INLINE  STATIC
	TS SafeDivC_scale(const TS &s, const TS &v, const TS &d, int scale)
	{
		if(!v)
			return d ;
		else
			return DivC_scale(s, v, scale );
	}

	template<class TS>  STATIC
	TS SafeDivC(const TS &s, const TS &v, const TS &d )
	{
		if(!v)
			return d;
		else
			return s/v ;
	}

	static A32F SafeDivCR(const A32F &s, const A32F &v)
	{
		if(!v)
		{
			if(s>0)	return Const::INF_32F;
			if(s<0)	return Const::INF_NEG_32F;
			else return Const::IND_32F;

		}
		else
			return s/v ;
	}

	template<class TS>  STATIC
	TS SafeDivCR(const TS &s, const TS &v)
	{
		if(!v)
			if (!s)
				return 0;
			else
				return FW_REF::Limits< TS >::MaxValue();
		else
			return s/v ;
	}

	template <class TS>  SYS_INLINE  STATIC
	TS DivC_scale(const TS &s, const TS &v, int scale)
	{
		return FW_REF::Limits<TS>::Sat(CBL_LIBRARY::DoubleToInt(FW_REF::Scale((float) s / v, scale)));
	}

	SYS_INLINE  STATIC Fw16sc DivC_scale(const Fw16sc &s, const Fw16sc &v, int scale)
	{
		Fw32fc t_s, t_v, t_d ;
		Fw16sc d;

		t_v.re = v.re ;  t_v.im = v.im ;
		t_s.re = s.re ;  t_s.im = s.im ;

		Fw32f denom;
		denom	= t_v.re*t_v.re + t_v.im*t_v.im;

		if (0 == denom)
		{
			t_d.re = t_d.im = 0;
		}
		else
		{
			t_d.re = ( t_s.re*t_v.re + t_s.im*t_v.im ) / denom;
			t_d.im = ( t_s.im*t_v.re - t_s.re*t_v.im ) / denom;
		}

		d.re = FW_REF::Limits<Fw16s>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(t_d.re, scale)));
		d.im = FW_REF::Limits<Fw16s>::Sat(CBL_LIBRARY::FloatToInt(FW_REF::Scale(t_d.im, scale)));

		return d;
	}

	SYS_INLINE  STATIC Fw32sc DivC_scale(const Fw32sc &s, const Fw32sc &v, int scale)
	{
		Fw64fc t_v, t_s, t_d ;
		Fw32sc d;

		t_v.re = v.re ;  t_v.im = v.im ;
		t_s.re = s.re ;  t_s.im = s.im ;

		Fw64f denom;
		denom	= t_v.re*t_v.re + t_v.im*t_v.im;

		if (0 == denom)
		{
			t_d.re = t_d.im = 0;
		}
		else
		{
			t_d.re = ( t_s.re*t_v.re + t_s.im*t_v.im ) / denom;
			t_d.im = ( t_s.im*t_v.re - t_s.re*t_v.im ) / denom;
		}

		d.re = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.re, scale));
		d.im = CBL_LIBRARY::DoubleToInt(FW_REF::Scale(t_d.im, scale));

		return d;
	}

	namespace DIV
	{
		template< class TS1, class TS2, class TD >
		ISV DivF( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = SafeDiv( s2[3], s1[3] );
			case C3:
			case AC4:	d[2] = SafeDiv( s2[2], s1[2] );
						d[1] = SafeDiv( s2[1], s1[1] );
			case C1:	d[0] = SafeDiv( s2[0], s1[0] );
			}
		}

		template< class TS1, class TS2, class TD >
		ISV DivI( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = SafeDiv_scale( s2[3], s1[3], scale);
			case C3:
			case AC4:	d[2] = SafeDiv_scale( s2[2], s1[2], scale);
						d[1] = SafeDiv_scale( s2[1], s1[1], scale);
			case C1:	d[0] = SafeDiv_scale( s2[0], s1[0], scale);
			}
		}
	}
	namespace DIVC
	{
		template< class TS, class TD >
		ISV DivCI_1( const TS * s, CH cs, TD * d, CH cd, TD v, int scale )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs;
			ASSERT( 1 == ChannelCount(cd) ); cd;

			*d = FW_REF::Limits<TD>::Sat( SafeDivC_scale( *s, v, *d, scale) );
		}

		template< class TS, class TD >
		ISV DivCI( const TS * s, CH cs, TD * d, CH cd, const TD *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( SafeDivC_scale( s[3], v[3], d[3], scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( SafeDivC_scale( s[2], v[2], d[2], scale) );
						d[1] = FW_REF::Limits<TD>::Sat( SafeDivC_scale( s[1], v[1], d[1], scale) );
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( SafeDivC_scale( s[0], v[0], d[0], scale) );
			}
		}

		template< class TS, class TD >
		ISV DivCF_1( const TS * s, CH cs, TD * d, CH cd, TD v )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs;
			ASSERT( 1 == ChannelCount(cd) ); cd;

			*d = SafeDivC( *s, v, *d );
		}

		template< class TS, class TD >
		ISV DivCRevF_1( const TS * s, CH cs, TD * d, CH cd, TD v )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs;
			ASSERT( 1 == ChannelCount(cd) ); cd;

			*d = SafeDivCR( v, *s );
		}

		template< class TS, class TD >
		ISV DivCF( const TS * s, CH cs, TD * d, CH cd, const TD *v )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = SafeDivC( s[3], v[3], d[3] );
			case C3:
			case AC4:	d[2] = SafeDivC( s[2], v[2], d[2] );
						d[1] = SafeDivC( s[1], v[1], d[1] );
			case C1:	d[0] = SafeDivC( s[0], v[0], d[0] );
			}
		}
	}
}



namespace DIV_SSE2
{
	ISV Pack32Sto16U(XMM128 &temp_f_l_1,XMM128 &temp_f_h_1,XMM128 &dest)
	{

		XMM128 dest1,dest2;
		
		dest1.i = _mm_shufflelo_epi16(temp_f_l_1.i,_MM_SHUFFLE(3,2,2,0));
		dest1.i = _mm_shufflehi_epi16(dest1.i,_MM_SHUFFLE(3,2,2,0));
		
		dest2.i = _mm_shufflelo_epi16(temp_f_h_1.i,_MM_SHUFFLE(3,2,2,0));
		dest2.i = _mm_shufflehi_epi16(dest2.i,_MM_SHUFFLE(3,2,2,0));

		dest.f =  _mm_shuffle_ps(dest1.f,dest2.f,_MM_SHUFFLE(2,0,2,0));

	}

	namespace DIV
	{
		namespace Init
		{
			namespace C1
			{
				ISV Div_8u_setup(int scale, XMM128 &scaleReg, XMM128 &ones)
				{
					Common::InitScale::To32f(scale, scaleReg);
					Common::InitMisc::Max::Of8u(ones);
				}

				ISV Div_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ones)
				{
					Div_8u_setup(scale, scaleReg, ones);
				}

				ISV Div_32s_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}

				ISV Div_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &min32s)
				{
					Common::InitScale::To32f(scale, scaleReg);
					Common::InitMisc::Min::Of32s(min32s);
				}


				ISV Div_32sc_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}

				ISV Div_32fc_setup(XMM128 &negbmask)
				{
					Common::InitMisc::Constants::negbmask32f(negbmask);
				}
			}

			namespace AC4
			{
				ISV Div_16scsetup(int scale, XMM128 &scaleReg, XMM128 &ac4mask, XMM128 &min32s)
				{
					Common::InitScale::To32f(scale, scaleReg);
					Common::InitMisc::Min::Of32s(min32s);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV Div_32sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Div_32fc_setup(XMM128 &ac4mask, XMM128 &negbmask)
				{
					Common::AC4::Init::For64(ac4mask);
					Common::InitMisc::Constants::negbmask32f(negbmask);
				}

				ISV Div_32f_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For32(ac4mask);
				}
			}
		}

		namespace Unit
		{
			namespace C1
			{
				ISV Div_8u_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128& scale, const XMM128 &ones)
				{
					XMM128 tsrc1_0, tsrc1_1, tsrc1_2, tsrc1_3;
					XMM128 tsrc2_0, tsrc2_1, tsrc2_2, tsrc2_3;
					__m128i zero = _mm_setzero_si128();
					__m128i tmp1, tmp2 ;

					tsrc1_0 = src1;
					tsrc2_0 = src2;

					CBL_SSE2::Unpack8UTo16U(tsrc1_0.i,tsrc1_2.i);
					CBL_SSE2::Unpack16UTo32U(tsrc1_0.i,tsrc1_1.i);
					CBL_SSE2::Unpack16UTo32U(tsrc1_2.i,tsrc1_3.i);

					CBL_SSE2::Unpack8UTo16U(tsrc2_0.i,tsrc2_2.i);
					CBL_SSE2::Unpack16UTo32U(tsrc2_0.i,tsrc2_1.i);
					CBL_SSE2::Unpack16UTo32U(tsrc2_2.i,tsrc2_3.i);

					tsrc1_0.f = _mm_cvtepi32_ps (tsrc1_0.i);
					tsrc2_0.f = _mm_cvtepi32_ps (tsrc2_0.i);
					tsrc1_0.f = _mm_div_ps( tsrc1_0.f, tsrc2_0.f );
					tsrc1_0.f = _mm_mul_ps( tsrc1_0.f, scale.f);
					tsrc1_0.i = _mm_cvtps_epi32(tsrc1_0.f);

					tsrc1_1.f = _mm_cvtepi32_ps (tsrc1_1.i);
					tsrc2_1.f = _mm_cvtepi32_ps (tsrc2_1.i);
					tsrc1_1.f = _mm_div_ps( tsrc1_1.f, tsrc2_1.f );
					tsrc1_1.f = _mm_mul_ps( tsrc1_1.f, scale.f );
					tsrc1_1.i = _mm_cvtps_epi32(tsrc1_1.f);

					tsrc1_2.f = _mm_cvtepi32_ps (tsrc1_2.i);
					tsrc2_2.f = _mm_cvtepi32_ps (tsrc2_2.i);
					tsrc1_2.f = _mm_div_ps( tsrc1_2.f, tsrc2_2.f );
					tsrc1_2.f = _mm_mul_ps( tsrc1_2.f, scale.f );
					tsrc1_2.i = _mm_cvtps_epi32(tsrc1_2.f);

					tsrc1_3.f = _mm_cvtepi32_ps (tsrc1_3.i);
					tsrc2_3.f = _mm_cvtepi32_ps (tsrc2_3.i);
					tsrc1_3.f = _mm_div_ps( tsrc1_3.f, tsrc2_3.f );
					tsrc1_3.f = _mm_mul_ps( tsrc1_3.f, scale.f );
					tsrc1_3.i = _mm_cvtps_epi32(tsrc1_3.f);

					FW_SSE2::pack32STo16S(tsrc1_0.i,tsrc1_1.i);
					FW_SSE2::pack32STo16S(tsrc1_2.i,tsrc1_3.i);
					FW_SSE2::pack16STo8U(tsrc1_0.i,tsrc1_2.i);

					tmp2 = _mm_cmpeq_epi8(src2.i, zero);
					tsrc1_0.i = _mm_or_si128(tsrc1_0.i, tmp2);	//take care of x/0 = MAX

					tmp1 = _mm_cmpeq_epi8(src1.i, zero);
					tmp1 = _mm_and_si128(tmp1, tmp2);
					tmp1 = _mm_xor_si128(tmp1, ones.i);			//take care of 0/0 = 0
					dst.i = _mm_and_si128(tmp1, tsrc1_0.i);
				}

				ISV Div_16s_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 &scale, const XMM128 &)
				{
					XMM128 tsrc1_0, tsrc1_1;
					XMM128 tsrc2_0, tsrc2_1;
					__m128i zero = _mm_setzero_si128();

					__m128i tmp1, tmp2,tmp3,tmp4,tmp5 ;

					tsrc1_0 = src1;
					tsrc2_0 = src2;
					CBL_SSE2::Unpack16STo32S(tsrc1_0.i, tsrc1_1.i);
					CBL_SSE2::Unpack16STo32S(tsrc2_0.i, tsrc2_1.i);

					tsrc1_0.f = _mm_cvtepi32_ps (tsrc1_0.i);
					tsrc2_0.f = _mm_cvtepi32_ps (tsrc2_0.i);
					tsrc1_0.f = _mm_div_ps( tsrc1_0.f, tsrc2_0.f );
					tsrc1_0.f = _mm_mul_ps( tsrc1_0.f, scale.f);
					dst.i = _mm_cvtps_epi32(tsrc1_0.f);

					tsrc1_1.f = _mm_cvtepi32_ps (tsrc1_1.i);
					tsrc2_1.f = _mm_cvtepi32_ps (tsrc2_1.i);
					tsrc1_1.f = _mm_div_ps( tsrc1_1.f, tsrc2_1.f );
					tsrc1_1.f = _mm_mul_ps( tsrc1_1.f, scale.f);
					tsrc1_1.i = _mm_cvtps_epi32(tsrc1_1.f);

					FW_SSE2::pack32STo16S(dst.i, tsrc1_1.i);

					//handling special cases x/0, -x/0, 0/0 (can be optimized)

                	tmp3 = _mm_cmpeq_epi16(src2.i, zero);
                   
                    if(0 != _mm_movemask_epi8(tmp3))
                    {
                        
                         tmp1 = _mm_cmpeq_epi16(src1.i,zero);
                         tmp5 = _mm_and_si128(tmp1,tmp3);
                        // dst.i = _mm_andnot_si128(tmp1,dst.i);
                         
                         tmp1 = _mm_cmpgt_epi16(src1.i, zero);
                         tmp2 = _mm_cmplt_epi16(src1.i, zero);
                         tmp4 = _mm_or_si128(tmp1,tmp2); 
                         tmp4 = _mm_or_si128(tmp4,tmp5); 
                         tmp4 = _mm_and_si128(tmp4,tmp3); 

                         
                         tmp1 = _mm_srli_epi16(tmp1,1);
                         tmp2 = _mm_slli_epi16(tmp2,15);
                         tmp2 = _mm_or_si128(tmp1,tmp2); 
                         tmp2 = _mm_and_si128(tmp2,tmp3); 

                        dst.i = _mm_andnot_si128(tmp4,dst.i);
                        dst.i = _mm_or_si128(dst.i,tmp2);
                    }

				}

				ISV Div_32s_unit(const XMM128 &s1, const XMM128 &s2, XMM128 &dest, const XMM128 &scale)
				{
					XMM128 src1_1,src1_2,src2_1,src2_2;

					XMM128 maskn,maskd,mask_z;//,masks;

					maskn.i =  _mm_cmpeq_epi32(s2.i,_mm_setzero_si128()); // check if numerator is zero
					maskd.i =  _mm_cmpeq_epi32(s1.i,_mm_setzero_si128()); // check if denominator is zero
					mask_z.i = _mm_and_si128(maskn.i,maskd.i);
					maskd.i  = _mm_cmpeq_epi32(maskd.i, maskd.i);
					mask_z.i = _mm_andnot_si128(mask_z.i,maskd.i);

					src1_1.d = _mm_cvtepi32_pd(s1.i);
					src1_2.i = _mm_srli_si128(s1.i,8);
					src1_2.d = _mm_cvtepi32_pd(src1_2.i);

					src2_1.d = _mm_cvtepi32_pd(s2.i);
					src2_2.i = _mm_srli_si128(s2.i,8);
					src2_2.d = _mm_cvtepi32_pd(src2_2.i);

					src1_1.d = _mm_div_pd(src2_1.d,src1_1.d);
					src1_1.d = _mm_mul_pd(src1_1.d,scale.d);
					src2_1.d = _mm_div_pd(src2_2.d,src1_2.d);
					src2_1.d = _mm_mul_pd(src2_1.d,scale.d);

					src1_1.i = CBL_SSE2::Pack64FTo32S(src1_1.d,src2_1.d);
					dest.i = _mm_and_si128(src1_1.i, mask_z.i);
				}

				ISV Div_16sc_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128 &scale, const XMM128 &)
				{
                 
                   
                    XMM128 src1lo,src1hi,src2lo,src2hi;
                    __m128i zero = _mm_setzero_si128();
                    src1lo = src1;
                    src2lo = src2;
                    CBL_SSE2::Unpack16STo32S(src1lo.i,src1hi.i);
                    CBL_SSE2::Unpack16STo32S(src2lo.i,src2hi.i);

                   src1lo.f =  _mm_cvtepi32_ps(src1lo.i);
                   src1hi.f =  _mm_cvtepi32_ps(src1hi.i);
                   src2lo.f =  _mm_cvtepi32_ps(src2lo.i);
                   src2hi.f =  _mm_cvtepi32_ps(src2hi.i);

                    src1lo.f = CBL_SSE2::Divide_32fc(src1lo.f,src2lo.f);
                    src1hi.f = CBL_SSE2::Divide_32fc(src1hi.f,src2hi.f);

                    src1lo.f	= _mm_mul_ps(src1lo.f, scale.f);
					src1hi.f	= _mm_mul_ps(src1hi.f, scale.f);

					src1lo.i	= _mm_cvtps_epi32(src1lo.f);
					src1hi.i	= _mm_cvtps_epi32(src1hi.f);

                    src1lo.i	= _mm_packs_epi32(src1lo.i, src1hi.i);
                    src1hi.i =  _mm_cmpeq_epi16(src2.i,zero);
                    dst.i = _mm_andnot_si128(src1hi.i,src1lo.i);
                    
				}

				ISV Div_32sc_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 &scale)
				{
				__m128d sLo				= _mm_cvtepi32_pd(src1.i);
				__m128d sHi				= _mm_cvtepi32_pd(src2.i);

				__m128d zeromaskLo	= sLo;
				__m128d rLo				= CBL_SSE2::Divide_64fc(sLo, sHi, zeromaskLo);

						sLo				= _mm_cvtepi32_pd(_mm_srli_si128(src1.i, 8));
						sHi				= _mm_cvtepi32_pd(_mm_srli_si128(src2.i, 8));

				__m128d zeromaskHi	= sLo;
				__m128d rHi				= CBL_SSE2::Divide_64fc(sLo, sHi, zeromaskHi);

					rLo					= _mm_mul_pd(rLo, scale.d);
					rHi					= _mm_mul_pd(rHi, scale.d);

					dst.i				= CBL_SSE2::Pack64FTo32S(rLo, rHi);
					__m128d	zeromask	= _mm_unpacklo_pd(zeromaskLo, zeromaskHi);
					dst.d				= _mm_andnot_pd(zeromask, dst.d);
				}

				ISV Div_32fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst, 
							const XMM128 &negbmask)
				{
					dst.f = CBL_SSE2::Divide_32fc_wm(src1.f, src2.f, negbmask.f);
				}

				ISV Div_64fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					__m128d zeromask = src1.d;
					dst.d = CBL_SSE2::Divide_64fc(src1.d, src2.d, zeromask);
				}
			}
		}
	}

	namespace DIVC
	{

		 // 32bit int division : srcDst <- (srcDst*scale)/val
		ISV divC_C1_int(XMM128 &srcDst, const XMM128 &scale_by_value)
		{
			srcDst.f = _mm_cvtepi32_ps (srcDst.i);
			srcDst.f = _mm_mul_ps( srcDst.f, scale_by_value.f);
			srcDst.i = _mm_cvtps_epi32(srcDst.f);
		}

		ISV divC_int(XMM128 &srcDst, const XMM128 &val, const XMM128 &scale)
		{
			srcDst.f = _mm_cvtepi32_ps (srcDst.i);
			srcDst.f = _mm_div_ps( srcDst.f, val.f);
			srcDst.f = _mm_mul_ps( srcDst.f, scale.f);
			srcDst.i = _mm_cvtps_epi32(srcDst.f);
		}

		ISV div_32fc_C3_AC4(XMM128 &src, XMM128 &dst,  const XMM128 &val1, const XMM128 &val2, const XMM128 &mask)
		{
			XMM128 val_re, val_im ;
			XMM128 src_l, t_re, t_im ;

			src_l.d = _mm_cvtps_pd(src.f);
			src.i = _mm_srli_si128(src.i, 8);
			src.d = _mm_cvtps_pd(src.f);

			val_re.d = _mm_shuffle_pd(val1.d, val1.d, _MM_SHUFFLE2(0,0));
			val_im.d = _mm_shuffle_pd(val1.d, val1.d, _MM_SHUFFLE2(1,1));
			val_im.i = _mm_xor_si128(val_im.i, mask.i);

			t_re.d = _mm_mul_pd(src_l.d, val_re.d);
			t_im.d = _mm_mul_pd(src_l.d, val_im.d);
			t_im.d = _mm_shuffle_pd( t_im.d, t_im.d, _MM_SHUFFLE2(0, 1));

			src_l.d = _mm_add_pd(t_re.d, t_im.d);
			src_l.f = _mm_cvtpd_ps(src_l.d);
			
			/* 2nd complex number */
			val_re.d = _mm_shuffle_pd(val2.d, val2.d, _MM_SHUFFLE2(0,0));
			val_im.d = _mm_shuffle_pd(val2.d, val2.d, _MM_SHUFFLE2(1,1));
			val_im.i = _mm_xor_si128(val_im.i, mask.i);

			t_re.d = _mm_mul_pd(src.d, val_re.d);
			t_im.d = _mm_mul_pd(src.d, val_im.d);
			t_im.d = _mm_shuffle_pd( t_im.d, t_im.d, _MM_SHUFFLE2(0, 1));

			src.d = _mm_add_pd(t_re.d, t_im.d);
			src.f = _mm_cvtpd_ps(src.d);

			src.i = _mm_slli_si128(src.i, 8);
			dst.i = _mm_or_si128(src_l.i, src.i);		
		}

		ISV div_32fc_AC4_lower(XMM128 &src, XMM128 &dst, const XMM128 &val1, const XMM128 &mask)
		{
			XMM128 val_re, val_im ;
			XMM128 src_l, t_re, t_im ;

			src_l.d = _mm_cvtps_pd(src.f);
		
			val_re.d = _mm_shuffle_pd(val1.d, val1.d, _MM_SHUFFLE2(0,0));
			val_im.d = _mm_shuffle_pd(val1.d, val1.d, _MM_SHUFFLE2(1,1));
			val_im.i = _mm_xor_si128(val_im.i, mask.i);

			t_re.d = _mm_mul_pd(src_l.d, val_re.d);
			t_im.d = _mm_mul_pd(src_l.d, val_im.d);
			t_im.d = _mm_shuffle_pd( t_im.d, t_im.d, _MM_SHUFFLE2(0, 1));

			src_l.d = _mm_add_pd(t_re.d, t_im.d);
			src_l.f = _mm_cvtpd_ps(src_l.d);
			
			dst.d = _mm_shuffle_pd(src_l.d, dst.d, _MM_SHUFFLE2(1, 0)); 		
		}
		namespace Init
		{
			namespace C1
			{
				ISV DivC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC, XMM128 &val)
				{
					Common::InitScale::To32f(scale, scaleMasks[0]);
					FW_SSE2::Load1(val, (float) 1/valC);
					val.f = _mm_mul_ps(scaleMasks[0].f, val.f);					
				}

				ISV DivC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC, XMM128 &val)
				{
					Common::InitScale::To32f(scale, scaleReg);
					FW_SSE2::Load1(val, (float) 1/valC);
					val.f = _mm_mul_ps(scaleReg.f, val.f);
				}

				ISV DivC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC, XMM128& val, XMM128 &min32s, XMM128 &negbmask)
				{
					Common::InitScale::To32f(scale, scaleReg);
					Common::InitMisc::Min::Of32s(min32s);
					Common::InitMisc::Constants::negbmask16s(negbmask);
					FW_SSE2::Load1(val, valC);
				}

				ISV DivC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC, XMM128 &val)
				{
					double scaleF = pow(2.0, -scale);
					Fw64f denom;

					denom = scaleF / (((Fw64f)valC.re * valC.re) + ((Fw64f)valC.im *valC.im));

					val.d		= _mm_set_pd(valC.im * denom,  valC.re * denom);
					scaleReg.d	= _mm_set_pd(valC.re * denom, -valC.im * denom);
				}

				ISV DivC_32f_setup(Fw32f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, (float)1/valC);
				}

				ISV DivCRev_32f_setup(Fw32f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV DivC_64f_setup(Fw64f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, (double)1/valC);
				}

				ISV DivC_32fc_setup(Fw32fc valC, XMM128 &valx, XMM128 &valrev)
				{
					Fw32fc	v			= valC;
					__m128 val			= _mm_set_ps(v.im, v.re, v.im, v.re);

					__m128	nega		= _mm_set_ps(1.0, -1.0, 1.0, -1.0);
					__m128	denom		= _mm_mul_ps(val, val);
							denom		= _mm_add_ps(denom, _mm_shuffle_ps(denom, denom, _MM_SHUFFLE(2, 3, 0, 1)));

					__m128	dC			= _mm_div_ps(val, denom);
					__m128	dCrev		= _mm_shuffle_ps(dC, dC, _MM_SHUFFLE(2, 3, 0, 1));
							dCrev		= _mm_mul_ps(dCrev, nega);

					valx.f				= dC;
					valrev.f			= dCrev;
				}

				ISV DivC_64fc_setup(Fw64fc valC, XMM128 &val, XMM128 &valrev)
				{
					Fw64fc v = valC;
					Fw64f denom;
					Fw64fc vd ;
					
					vd.re = v.re ; vd.im = v.im ;		
					denom = vd.re * vd.re + vd.im *vd.im ;		
					vd.re = vd.re/denom ; vd.im = -vd.im/denom ;		

					// xmmData0 <- val, xmmData1 <- denom 

					val.d		= _mm_set_pd(vd.re, vd.re);
					valrev.d	= _mm_set_pd(-vd.im, vd.im);
				}
			}
			namespace C3
			{
				ISV DivC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					scaleMasks;
					Fw8u *v = valC;
					Fw32f valF[3];
					Fw32f scaleF = (Fw32f)pow(2.0, -scale);

					valF[0] = scaleF / (Fw32f)v[0];
					valF[1] = scaleF / (Fw32f)v[1];
					valF[2] = scaleF / (Fw32f)v[2];

					FW_SSE2::Load2(val0, valF, OPT_LEVEL::C3, 0);
					FW_SSE2::Load2(val1, valF, OPT_LEVEL::C3, 1);
					FW_SSE2::Load2(val2, valF, OPT_LEVEL::C3, 2);
				}

				ISV DivC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					scaleReg;
					Fw16s *v = valC;
					Fw32f valF[3];
					Fw32f scaleF = (Fw32f)pow(2.0, -scale);

					valF[0] = scaleF / (Fw32f)v[0];
					valF[1] = scaleF / (Fw32f)v[1];
					valF[2] = scaleF / (Fw32f)v[2];

					FW_SSE2::Load2(val0, valF, OPT_LEVEL::C3, 0);
					FW_SSE2::Load2(val1, valF, OPT_LEVEL::C3, 1);
					FW_SSE2::Load2(val2, valF, OPT_LEVEL::C3, 2);
				}

				ISV DivC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2, XMM128 &min32s, XMM128 &negbmask)
				{
					Common::InitScale::To32f(scale, scaleReg);
					Common::InitMisc::Min::Of32s(min32s);
					Common::InitMisc::Constants::negbmask16s(negbmask);
					Common::InitConst::C3::To16sc::From16sc(valC, val0, val1, val2);
				}

				ISV DivC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Fw32sc *v = valC;
					double scaleF = pow(2.0, -scale);

					Fw64f denom0, denom1, denom2;

					denom0 = scaleF / (((Fw64f) v[0].re * v[0].re) + ((Fw64f) v[0].im * v[0].im));
					denom1 = scaleF / (((Fw64f) v[1].re * v[1].re) + ((Fw64f) v[1].im * v[1].im));
					denom2 = scaleF / (((Fw64f) v[2].re * v[2].re) + ((Fw64f) v[2].im * v[2].im));

					val0.d	= _mm_set_pd(v[0].im * denom0, v[0].re * denom0);
					val1.d	= _mm_set_pd(v[1].im * denom1, v[1].re * denom1);
					val2.d	= _mm_set_pd(v[2].im * denom2, v[2].re * denom2);

					scaleReg.i = _mm_set_epi32(0x00000000, 0x00000000, 0x80000000, 0x00000000);
				}

				ISV DivC_32f_setup(Fw32f valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
                    Fw32f valCInv[3];
                    valCInv[0] = 1/valC[0];
                    valCInv[1] = 1/valC[1];
                    valCInv[2] = 1/valC[2];

					FW_SSE2::Load2(val0, valCInv, OPT_LEVEL::C3, 0);
					FW_SSE2::Load2(val1, valCInv, OPT_LEVEL::C3, 1);
					FW_SSE2::Load2(val2, valCInv, OPT_LEVEL::C3, 2);
				}

				ISV DivC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2, XMM128 &val0rev, XMM128 &val1rev, XMM128 &val2rev)
				{
					Fw32fc	v[3];
					memcpy((void*)v, valC, sizeof(Fw32fc) * 3);

					Fw32f	D0			= (v[0].re * v[0].re) + (v[0].im * v[0].im);
					v[0].re				= v[0].re / D0;
					v[0].im				= v[0].im / D0;

					Fw32f	D1			= (v[1].re * v[1].re) + (v[1].im * v[1].im);
					v[1].re				= v[1].re / D1;
					v[1].im				= v[1].im / D1;

					Fw32f	D2			= (v[2].re * v[2].re) + (v[2].im * v[2].im);
					v[2].re				= v[2].re / D2;
					v[2].im				= v[2].im / D2;

					val0.f	= _mm_set_ps(v[1].im, v[1].re, v[0].im, v[0].re);
					val1.f	= _mm_set_ps(v[0].im, v[0].re, v[2].im, v[2].re);
					val2.f	= _mm_set_ps(v[2].im, v[2].re, v[1].im, v[1].re);

					val0rev.f	= _mm_set_ps(v[1].re, -v[1].im, v[0].re, -v[0].im);
					val1rev.f	= _mm_set_ps(v[0].re, -v[0].im, v[2].re, -v[2].im);
					val2rev.f	= _mm_set_ps(v[2].re, -v[2].im, v[1].re, -v[1].im);
				}
			}
			namespace AC4
			{
				ISV DivC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val, XMM128 &ac4mask)
				{
					Fw8u *v = valC;
					Fw32f valF[4];
					Fw32f scaleF = (Fw32f)pow(2.0, -scale);

					valF[0] = (Fw32f)v[0];
					valF[1] = (Fw32f)v[1];
					valF[2] = (Fw32f)v[2];

					FW_SSE2::Load2(val, valF, OPT_LEVEL::AC4, 0);
					val.f32[3] = 1 ;

					FW_SSE2::Load1(scaleMasks[0], scaleF);
					scaleMasks[0].f32[3] = 1 ;

					ac4mask.i = _mm_set1_epi32(0xFF000000);	/* mask */
				}

				ISV DivC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val, XMM128 &ac4mask)
				{
					scaleReg;
					Fw16s *v = valC;
					Fw32f valF[4];
					Fw32f scaleF = (Fw32f)pow(2.0, -scale);

					valF[0] = scaleF / (Fw32f)v[0];
					valF[1] = scaleF / (Fw32f)v[1];
					valF[2] = scaleF / (Fw32f)v[2];

					FW_SSE2::Load2(val, valF, OPT_LEVEL::AC4, 0);
					val.f32[3] = 1 ;

					ac4mask.i = _mm_setzero_si128();
					ac4mask.u32[1] = 0xFFFF0000 ;
					ac4mask.u32[3] = 0xFFFF0000 ;
				}

				ISV DivC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val, XMM128 &min32s, XMM128 &negbmask, XMM128 &ac4mask)
				{
					Fw16sc *v = valC;
					FW_SSE2::Load2(val, v, OPT_LEVEL::AC4, 0);
					
					Fw32f scaleF = (float)pow(2.0, -scale);
					FW_SSE2::Load1( scaleReg, scaleF);

					ac4mask.i = _mm_setzero_si128();
					ac4mask.u32[3] = 0xFFFFFFFF ;

					Common::InitMisc::Min::Of32s(min32s);
					Common::InitMisc::Constants::negbmask16s(negbmask);
				}

				ISV DivC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2, XMM128 &negbmask)
				{
					scaleReg; 
					Fw32sc *v = valC;
					double scaleF = pow(2.0, -scale);

					Fw64f denom0, denom1, denom2;

					denom0 = scaleF / (((Fw64f) v[0].re * v[0].re) + ((Fw64f) v[0].im * v[0].im));
					denom1 = scaleF / (((Fw64f) v[1].re * v[1].re) + ((Fw64f) v[1].im * v[1].im));
					denom2 = scaleF / (((Fw64f) v[2].re * v[2].re) + ((Fw64f) v[2].im * v[2].im));

					val0.d	= _mm_set_pd(v[0].im * denom0, v[0].re * denom0);
					val1.d	= _mm_set_pd(v[1].im * denom1, v[1].re * denom1);
					val2.d	= _mm_set_pd(v[2].im * denom2, v[2].re * denom2);

					negbmask.i	= _mm_set_epi32(0x00000000, 0x00000000, 0x80000000, 0x00000000);
				}

				ISV DivC_32f_setup(Fw32f valC[], XMM128 &val, XMM128 &ac4mask)
				{
					Fw32f *valCR = valC;
					valCR[0] = 1/valC[0];
					valCR[1] = 1/valC[1];
					valCR[2] = 1/valC[2];

					Common::InitConst::AC4::To32f::From32f(valCR, val);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV DivC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val0rev, XMM128 &val1rev, XMM128 &ac4mask)
				{
					//Common::InitConst::AC4::To32fc::From32fc(valC, val0, val1);

                    Fw32fc	v[3];
					memcpy((void*)v, valC, sizeof(Fw32fc) * 3);

					Fw32f	D0			= (v[0].re * v[0].re) + (v[0].im * v[0].im);
					v[0].re				= v[0].re / D0;
					v[0].im				= v[0].im / D0;

					Fw32f	D1			= (v[1].re * v[1].re) + (v[1].im * v[1].im);
					v[1].re				= v[1].re / D1;
					v[1].im				= v[1].im / D1;

					Fw32f	D2			= (v[2].re * v[2].re) + (v[2].im * v[2].im);
					v[2].re				= v[2].re / D2;
					v[2].im				= v[2].im / D2;

					val0.f	= _mm_set_ps(v[1].im, v[1].re, v[0].im, v[0].re);
					val1.f	= _mm_set_ps(v[0].im, v[0].re, v[2].im, v[2].re);
					//val2.f	= _mm_set_ps(v[2].im, v[2].re, v[1].im, v[1].re);

					val0rev.f	= _mm_set_ps(v[1].re, -v[1].im, v[0].re, -v[0].im);
					val1rev.f	= _mm_set_ps(0, 0, v[2].re, -v[2].im);
					//val2rev.f	= _mm_set_ps(v[2].re, -v[2].im, v[1].re, -v[1].im);

					Common::AC4::Init::For64(ac4mask);
				}
			}
		}

		namespace Unit
		{
			namespace C1
			{
				ISV DivC_8u_unit(const XMM128 &src, const XMM128 &valC, XMM128 &dst)
				{
					XMM128 a, b, c, d;
					
					a = src;

					CBL_SSE2::Unpack8UTo16U(a.i,c.i);
					CBL_SSE2::Unpack16UTo32U(a.i,b.i);
					CBL_SSE2::Unpack16UTo32U(c.i,d.i);

					divC_C1_int(a, valC);
					divC_C1_int(b, valC);
					divC_C1_int(c, valC);
					divC_C1_int(d, valC);

					a.i =_mm_packs_epi32(a.i , b.i);
					c.i =_mm_packs_epi32(c.i, d.i );
					dst.i =_mm_packus_epi16(a.i ,c.i);		
				}


				ISV DivC_16s_unit(const XMM128 &src, const XMM128 &scaleReg, XMM128 &dst)
				{
					XMM128 tsrc_0, tsrc_1 ;
				
					tsrc_0 = src;
					CBL_SSE2::Unpack16STo32S(tsrc_0.i, tsrc_1.i);

					divC_C1_int(tsrc_0, scaleReg);
					divC_C1_int(tsrc_1, scaleReg);
					
					dst.i = _mm_packs_epi32 (tsrc_0.i, tsrc_1.i);
				}

				ISV DivC_16u_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 tsrc_0, tsrc_1,temp_dest ;
				
					temp_dest.i = _mm_cmpeq_epi16(src1.i, _mm_setzero_si128()); 


					tsrc_0 = src1;
					CBL_SSE2::Unpack16UTo32U(tsrc_0.i, tsrc_1.i);
					
					tsrc_0.f = _mm_cvtepi32_ps(tsrc_0.i);
					tsrc_1.f = _mm_cvtepi32_ps(tsrc_1.i);

					tsrc_0.f = _mm_div_ps(src2.f,tsrc_0.f);
					tsrc_1.f = _mm_div_ps(src2.f,tsrc_1.f);

					tsrc_0.i = _mm_cvtps_epi32(tsrc_0.f);
					tsrc_1.i = _mm_cvtps_epi32(tsrc_1.f);	

					DIV_SSE2::Pack32Sto16U (tsrc_0, tsrc_1,dst);
					dst.i = _mm_or_si128(dst.i,temp_dest.i); 
				}

				ISV DivC_32sc_unit( const XMM128& src, const XMM128& valC, const XMM128 &valCRev, XMM128& dst)
				{
					__m128d sLo		= _mm_cvtepi32_pd(src.i);
					__m128d rLo		= CBL_SSE2::DivideC_64fc(sLo, valC.d, valCRev.d);

							sLo		= _mm_cvtepi32_pd(_mm_srli_si128(src.i, 8));
					__m128d rHi		= CBL_SSE2::DivideC_64fc(sLo, valC.d, valCRev.d);

						dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}

				ISV DivC_64fc_unit(const XMM128 &src, const XMM128 &val_re, const XMM128 &val_im, XMM128 &dst)
				{
					XMM128  t_re, t_im ;

					t_re.d = _mm_mul_pd(src.d, val_re.d);
					t_im.d = _mm_mul_pd(src.d, val_im.d);
					t_im.d = _mm_shuffle_pd( t_im.d, t_im.d, _MM_SHUFFLE2(0, 1));

					dst.d = _mm_add_pd(t_re.d, t_im.d);				
				}
			}

			namespace C3
			{
				ISV DivC_8u_unit( XMM128& src, const XMM128& val0,
											   const XMM128& val1,
											   const XMM128& val2,
											   const XMM128& val3, XMM128& dst)
				{
					XMM128 a, b, c, d;
					
					a = src;

					CBL_SSE2::Unpack8UTo16U(a.i,c.i);
					CBL_SSE2::Unpack16UTo32U(a.i,b.i);
					CBL_SSE2::Unpack16UTo32U(c.i,d.i);

					divC_C1_int(a, val0);
					divC_C1_int(b, val1);
					divC_C1_int(c, val2);
					divC_C1_int(d, val3);

					a.i =_mm_packs_epi32(a.i , b.i);
					c.i =_mm_packs_epi32(c.i, d.i );
					d.i =_mm_packus_epi16(a.i ,c.i);	// result if value != 0 
					dst.i = d.i;
				}

				ISV DivC_16s_unit( XMM128& src, const XMM128& val0, const XMM128& val1, XMM128& dst)
				{
					XMM128 t0, t1, tdst ;

					t0 = src;
					CBL_SSE2::Unpack16STo32S(t0.i, t1.i);

					divC_C1_int(t0, val0);
					divC_C1_int(t1, val1);
					tdst.i = _mm_packs_epi32(t0.i, t1.i);
					dst.i	= tdst.i;
				}

				ISV DivC_32sc_unit(XMM128 &src, const XMM128& valC, const XMM128& valCRev, XMM128 &dst)
				{
				__m128d sLo		= _mm_cvtepi32_pd(src.i);
				__m128d rLo		= CBL_SSE2::DivideC_64fc(sLo, valC.d, valCRev.d);

					dst.i		= CBL_SSE2::Pack64FTo32SLo(rLo, src.i);
				}

				ISV DivC_32sc_unit(XMM128 &src,	const XMM128& val0C, const XMM128& val0CRev, 
												const XMM128& val1C, const XMM128& val1CRev, 
								   XMM128 &dst)
				{
				__m128d sLo		= _mm_cvtepi32_pd(src.i);
				__m128d rLo		= CBL_SSE2::DivideC_64fc(sLo, val0C.d, val0CRev.d);

						sLo		= _mm_cvtepi32_pd(_mm_srli_si128(src.i, 8));
				__m128d rHi		= CBL_SSE2::DivideC_64fc(sLo, val1C.d, val1CRev.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}

				ISV DivC_32fc_unit(XMM128 &src, const XMM128& valC, const XMM128& valCRev, XMM128 &dst)
				{
				__m128d sLo		= _mm_cvtepi32_pd(src.i);
				__m128d rLo		= CBL_SSE2::DivideC_64fc(sLo, valC.d, valCRev.d);

					dst.i		= CBL_SSE2::Pack64FTo32SLo(rLo, src.i);
				}

				ISV DivC_32fc_unit(XMM128 &src,	const XMM128& val0C, const XMM128& val0CRev, 
												const XMM128& val1C, const XMM128& val1CRev, 
								   XMM128 &dst)
				{
				__m128d sLo		= _mm_cvtepi32_pd(src.i);
				__m128d rLo		= CBL_SSE2::DivideC_64fc(sLo, val0C.d, val0CRev.d);

						sLo		= _mm_cvtepi32_pd(_mm_srli_si128(src.i, 8));
				__m128d rHi		= CBL_SSE2::DivideC_64fc(sLo, val1C.d, val1CRev.d);

					dst.i		= CBL_SSE2::Pack64FTo32S(rLo, rHi);
				}


			}
			namespace AC4
			{
				ISV DivC_8u_unit( XMM128& src, const XMM128& val, XMM128& dst, const XMM128& scale)
				{
					XMM128 a, b, c, d;
					
					a = src;

					CBL_SSE2::Unpack8UTo16U(a.i,c.i);
					CBL_SSE2::Unpack16UTo32U(a.i,b.i);
					CBL_SSE2::Unpack16UTo32U(c.i,d.i);

					divC_int(a, val, scale);
					divC_int(b, val, scale);
					divC_int(c, val, scale);
					divC_int(d, val, scale);

					a.i =_mm_packs_epi32(a.i , b.i);
					c.i =_mm_packs_epi32(c.i, d.i );
					d.i =_mm_packus_epi16(a.i ,c.i);	// result if value != 0 

					a.f = _mm_cmpeq_ps(val.f, _mm_setzero_ps());	//take care of src/0 = src
					a.i = _mm_and_si128(a.i, dst.i);

					b.f = _mm_cmpneq_ps(val.f, _mm_setzero_ps());
					b.i = _mm_and_si128(b.i, d.i);
					dst.i = _mm_or_si128(a.i, b.i);
				}
			}
		}
	}
}

} // namespace OPT_LEVEL


#endif	// __DIVIMPL_H__

