/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ADDIMPL_H__
#define __ADDIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"


namespace OPT_LEVEL
{

namespace ADD_REF
{
	namespace ADD
	{
		template< class TS, class TD >
		ISV AddF( const TS * s, CH cs, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = (TD)((d[3] + s[3]) );
			case C3:
			case AC4:	d[2] = (TD)((d[2] + s[2]) );
						d[1] = (TD)((d[1] + s[1]) );
			case C1:	d[0] = (TD)((d[0] + s[0]) );
			}
		}

		template< class TS1, class TS2, class TD >
		ISV AddF( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = (TD)((s1[3] + s2[3]));
			case C3:
			case AC4:	d[2] = (TD)((s1[2] + s2[2]));
						d[1] = (TD)((s1[1] + s2[1]));
			case C1:	d[0] = (TD)((s1[0] + s2[0]));
			}
		}

		template< class TS1, class TS2, class TD >
		ISV AddI( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s1[3] + s2[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s1[2] + s2[2]), scale) );
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s1[1] + s2[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s1[0] + s2[0]), scale) );
			}
		}

		template< class TS1, class TS2, class TD >
		ISV AddINS( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( s1[3] + s2[3] );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( s1[2] + s2[2] );
						d[1] = FW_REF::Limits<TD>::Sat( s1[1] + s2[1] );
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( s1[0] + s2[0] );
			}
		}

		SYS_INLINE STATIC void AddI( const A32S * s1, CH cs1, const A32S * s2, CH cs2, A32S * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong( FW_REF::Scale( ((A64F)s1[3] + s2[3]), scale)) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong(  FW_REF::Scale( ((A64F)s1[2] + s2[2]), scale) ));
						d[1] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong(  FW_REF::Scale( ((A64F)s1[1] + s2[1]), scale)) );
			case C1:	d[0] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong(  FW_REF::Scale( ((A64F)s1[0] + s2[0]), scale)) );
			}
		}

		SYS_INLINE STATIC Fw32sc add_16s(Fw16sc a, Fw16sc b)
		{
			Fw32sc c;
			c.re = a.re + b.re;
			c.im = a.im + b.im;
			return c;
		}
		SYS_INLINE STATIC Fw64sc add_32s(Fw32sc a, Fw32sc b)
		{
			Fw64sc c;
			c.re = (Fw64s)a.re + (Fw64s)b.re;
			c.im = (Fw64s)a.im + (Fw64s)b.im;
			return c;
		}

		ISV AddI( const Fw16sc * s1, CH cs1, const Fw16sc * s2, CH cs2, Fw16sc * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( add_16s(s1[3] , s2[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( add_16s(s1[2] , s2[2]), scale) );
						d[1] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( add_16s(s1[1] , s2[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( add_16s(s1[0] , s2[0]), scale) );
			}
		}

		ISV AddI( const Fw32sc * s1, CH cs1, const Fw32sc * s2, CH cs2, Fw32sc * d, CH cd,  int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( add_32s(s1[3] , s2[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( add_32s(s1[2] , s2[2]), scale) );
						d[1] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( add_32s(s1[1] , s2[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( add_32s(s1[0] , s2[0]), scale) );
			}
		}

		template< class TS, class TM, class TD >
		ISV AddF_m( const TS * s, CH cs, const TM * m, CH cm, TD * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
			ASSERT( 1 == ChannelCount(cm) );
			ASSERT( 1 == ChannelCount(cd) );

			if( *m )
				*d = (TD)( *d + *s );
		}

		ISV AddI( const A16S * s, CH, A32S * d, CH )
		{
			d[0] = FW_REF::Limits<A32S>::Sat( ((A64S)d[0] + (A64S)s[0]) );
		}
	}

	namespace ADDC
	{
		template< class TS, class TD >
		ISV AddCI_1( const TS * s, CH cs, TD * d, CH cd, const TD v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<TD>::Sat( FW_REF::Scale(*s + v, scale ) );
		}

		ISV AddCI_1( const A32S * s, CH cs, A32S * d, CH cd, A32S v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale((A64F)*s + v, scale ) ));
		}

		ISV AddCI_1( const A32SC * s, CH cs, A32SC * d, CH cd, A32SC v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_32s(s[0] , v), scale ));
		}

		ISV AddCI_1( const A16SC * s, CH cs, A16SC * d, CH cd, A16SC v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_16s(s[0] , v), scale ));
		}

		template< class TS, class TD >
		ISV AddCI( const TS * s, CH cs, TD * d, CH cd, const TD *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[3] + v[3], scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[2] + v[2], scale ));
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[1] + v[1], scale ));
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[0] + v[0], scale ));
			}
		}

		ISV AddCI( const Fw16sc * s, CH cs, Fw16sc * d, CH cd, const Fw16sc *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_16s(s[3] , v[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_16s(s[2] , v[2]), scale ));
						d[1] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_16s(s[1] , v[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_16s(s[0] , v[0]), scale ));
			}
		}

		ISV AddCI( const Fw32sc * s, CH cs, Fw32sc * d, CH cd, const Fw32sc *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_32s(s[3] , v[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_32s(s[2] , v[2]), scale ));
						d[1] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_32s(s[1] , v[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( ADD_REF::ADD::add_32s(s[0] , v[0]), scale ));
			}
		}


		template< class TS, class TD >
		ISV AddCF_1( const TS * s, CH, TD * d, CH, const TD val )
		{
			d[0] = (TD)((s[0] +  val) );
		}

		template< class TS, class TD >
		ISV AddCF( const TS * s, CH cs, TD * d, CH cd, const TD *v )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = (TD)((s[3] + v[3]) );
			case C3:
			case AC4:	d[2] = (TD)((s[2] + v[2]) );
						d[1] = (TD)((s[1] + v[1]) );
			case C1:	d[0] = (TD)((s[0] + v[0]) );
			}
		}
	}

	namespace ADDX
	{
		template< class TS1, class TS2, class TD >
		ISV AddProduct( const TS1 * s1, CH cs1, const TS2 *s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1; cs2;
			ASSERT( ChannelCount(cs2) == ChannelCount(cd)  );

			switch( cd )
			{
			case C4:	d[3] = (TD)( d[3] + (TD)s1[3] * s2[3] );
			case C3:			  
			case AC4:	d[2] = (TD)( d[2] + (TD)s1[2] * s2[2] );
						d[1] = (TD)( d[1] + (TD)s1[1] * s2[1] );
			case C1:	d[0] = (TD)( d[0] + (TD)s1[0] * s2[0] );
			}
		}

		ISV AddProduct_32f_C1( const A32F * s1, CH cs1, const A32F *s2, CH cs2, A32F * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1; cs2;
			ASSERT( ChannelCount(cs2) == ChannelCount(cd)  );cd;

			A32F temp1;
			temp1 = (*s1 * *s2);
			if((SYS_FINITE(temp1) != 0))
				d[0] = d[0] + temp1;	
			else
				d[0] = temp1;	
		}


		ISV AddProduct_32fc_C1( const A32FC * s1, CH cs1, const A32FC *s2, CH cs2, A32FC * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1; cs2;
			ASSERT( ChannelCount(cs2) == ChannelCount(cd)  ); cd;

			A32F temp1,temp2;
			temp1 = (s1->re*s2->re);
			temp2 = (s1->im*s2->im);

			if((SYS_FINITE(temp1) == 0) &&	(SYS_FINITE(temp2) == 0))
				d[0].re  = temp1;
			else
				d[0].re = d[0].re + (temp1 - temp2);

			d[0].im = d[0].im + ((s1->re*s2->im) + (s1->im*s2->re));	
		}




		ISV AddProduct_64fc_C1( const A64FC * s1, CH cs1, const A64FC *s2, CH cs2, A64FC * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) ); cs1; cs2;
			ASSERT( ChannelCount(cs2) == ChannelCount(cd)  ); cd;

			A64F temp1,temp2;
			temp1 = (s1->re*s2->re);
			temp2 = (s1->im*s2->im);

			if((SYS_FINITE(temp1) == 0) &&	(SYS_FINITE(temp2) == 0))
				d[0].re  = temp1;
			else
				d[0].re = d[0].re + (temp1 - temp2);

			d[0].im = d[0].im + ((s1->re*s2->im) + (s1->im*s2->re));	
		}

		template< class TS1, class TS2, class TD >
		ISV AddProductI( const TS1 * s1, CH, const TS2 *s2, CH, TD * d, CH, int scale)
		{
			d[0] = FW_REF::Limits<A16S>::Sat( FW_REF::Scale<A64S>( d[0] + (TD)s1[0] * s2[0], scale));
		}

		ISV AddProductI( const A16S * s1, CH, const A16S *s2, CH, A32S * d, CH, int scale)
		{
			d[0] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong(FW_REF::Scale( (A64F)d[0] + (A64F)((A32S)s1[0] * s2[0]), scale)));	
		}

		ISV AddProductI( const A32S * s1, CH, const A32S *s2, CH, A32S * d, CH, int scale)
		{
			Fw64f res = FW_REF::Scale( (A64F)d[0] + (A64F)((A64F)s1[0] * s2[0]), scale);

			if (res > FW_REF::Limits<S32>::MaxValue())
				res = (double) FW_REF::Limits<S32>::MaxValue();
			if (res < FW_REF::Limits<S32>::MinValue())
				res = (double) FW_REF::Limits<S32>::MinValue();
			d[0] = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToInt(res));
		}

		template< class TS1, class TS2, class TD >
		ISV AddProduct_m( const TS1 * s1, CH cs1, const TS2 *s2, CH cs2, const Fw8u *m, CH cm, TD * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs1) ); cs1; cs2; cm; cd;
			ASSERT( 1 == ChannelCount(cs2) );
			ASSERT( ChannelCount(cm ) == 1 );
			ASSERT( ChannelCount(cd ) == 1 );

			if( *m )
				*d += (TD)( (A32F)*s1 * *s2 );
		}


		//
		// AddSquare
		//
		template< class TS, class TD >
		ISV AddSquare( const TS * s, CH cs, TD * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd;
			ASSERT( 1 == ChannelCount(cd) );

			*d = *d + ((A32F)*s * *s);
		}

		template< class TS, class TD >
		ISV AddSquare_m( const TS * s, CH cs, const Fw8u * m, CH cm,  TD * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
			ASSERT( 1 == ChannelCount(cm) );
			ASSERT( 1 == ChannelCount(cd) );

			if( *m )
				*d = *d + ((A32F)*s * *s);
		}

		//
		//	Addweighted
		//
		template< class TS, class TD >
		ISV AddWeighted( const TS * s, CH cs, TD * d, CH cd, A32F alpha )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd;
			ASSERT( 1 == ChannelCount(cd) );

			*d = *d - (alpha *(*d - (A32F)*s));
		}

		template< class TS, class TD >
		ISV AddWeighted_m( const TS * s, CH cs, const Fw8u *m, CH cm, TD * d, CH cd, A32F alpha )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs; cd; cm;
			ASSERT( 1 == ChannelCount(cm) );
			ASSERT( 1 == ChannelCount(cd) );

			if( *m )
				*d = *d - (alpha *(*d - (A32F)*s));
		}
	}
}



namespace ADD_SSE2
{
	namespace ADD
	{
		ISV Setup_mask(XMM128 &m1, XMM128 &m2,XMM128 &Src)
		{
			const __m128i mask = _mm_setzero_si128();
			const __m128i mask2 = _mm_cmpeq_epi32(mask,mask);
			m1.i = _mm_cmpeq_epi32(Src.i,mask);
			m2.i = _mm_andnot_si128(m1.i,mask2);
		}

		ISV Add16s(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128& scale)
		{
			XMM128 srcTemp1, srcTemp2;
			XMM128 srcTemp1_2, srcTemp2_2;
			srcTemp1	= src1;
			srcTemp1_2	= src2;

			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);
			CBL_SSE2::Unpack16STo32S(srcTemp1_2.i,srcTemp2_2.i);

			srcTemp1.i = _mm_add_epi32(srcTemp1.i, srcTemp1_2.i);
			srcTemp2.i = _mm_add_epi32(srcTemp2.i, srcTemp2_2.i);

			srcTemp1.f		= _mm_cvtepi32_ps (srcTemp1.i);
			srcTemp2.f		= _mm_cvtepi32_ps (srcTemp2.i);

			srcTemp1.f		= _mm_mul_ps(srcTemp1.f, scale.f);
			srcTemp2.f		= _mm_mul_ps(srcTemp2.f, scale.f);

			srcTemp1.i		= _mm_cvtps_epi32(srcTemp1.f);
			srcTemp2.i		= _mm_cvtps_epi32 (srcTemp2.f);

			srcTemp1.i = _mm_packs_epi32(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

		ISV Add32sc(const XMM128 &src1, const XMM128 &src2, XMM128& dst, const XMM128& scale)
		{
			XMM128 srcTemp1_1,srcTemp2_1;
			XMM128 srcTemp1_2, srcTemp2_2;

			srcTemp1_1.d = _mm_cvtepi32_pd(src1.i);
			srcTemp2_1.d = _mm_cvtepi32_pd(src2.i);

			srcTemp1_2.i = _mm_srli_si128(src1.i, 8);
			srcTemp2_2.i = _mm_srli_si128(src2.i, 8);

			srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
			srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

			srcTemp1_1.d = _mm_add_pd(srcTemp1_1.d, srcTemp2_1.d);
			srcTemp1_2.d = _mm_add_pd(srcTemp1_2.d, srcTemp2_2.d);

			dst.d = _mm_mul_pd(srcTemp1_1.d, scale.d);
			srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, scale.d);

			dst.i = CBL_SSE2::Pack64FTo32S (dst.d, srcTemp1_2.d);
		}


		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV Add_8u_setup(int scale, XMM128 scaleMasks[])
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
				}

				template <int scaleType>
				ISV Add_16s_setup(int scale, XMM128 &scaleReg)
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
				ISV Add_16sc_setup(int scale, XMM128 &scaleReg)
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

				ISV Add_32s_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}

				ISV Add_32sc_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}
			}

			namespace AC4
			{
				template <int scaleType>
				ISV Add_8u_setup(int scale, XMM128 scaleMasks[], XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV Add_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
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
				ISV Add_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						break;
					default:;
					}
					Common::AC4::Init::For32(ac4mask);
				}

				ISV Add_32sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Add_32fc_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Add_32f_setup(XMM128 &ac4mask)
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
				ISV Add_8u_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 *scaleMasks)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						XMM128 srcTemp1, srcTemp2;
						XMM128 srcTemp1_2, srcTemp2_2;

						srcTemp1	= src1;
						srcTemp1_2	= src2;

						CBL_SSE2::Unpack8UTo16U(srcTemp1.i,srcTemp2.i);
						CBL_SSE2::Unpack8UTo16U(srcTemp1_2.i,srcTemp2_2.i);

						srcTemp1.i = _mm_add_epi16(srcTemp1.i, srcTemp1_2.i);
						srcTemp2.i = _mm_add_epi16(srcTemp2.i, srcTemp2_2.i);
						
						srcTemp1.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp1.i, (__m128i *) scaleMasks);
						srcTemp2.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp2.i, (__m128i *) scaleMasks);

						FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
						dst = srcTemp1;
						break;
					default:
						dst.i = _mm_adds_epu8(src1.i, src2.i);
					}
				}


				template <int scaleType>
				ISV Add_16s_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADD::Add16s(src1, src2, dst, scale);
						break;
					case 0:
						dst.i = _mm_adds_epi16(src1.i, src2.i);
						break;
					}
				}

				template <int scaleType>
				ISV Add_16sc_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADD::Add16s(src1, src2, dst, scale);
						break;
					case 0:
						dst.i = _mm_adds_epi16(src1.i, src2.i);
						break;
					}
				}

				ISV Add_32s_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					ADD_SSE2::ADD::Add32sc(s1, s2, dst, scale);
				}

				ISV Add_32sc_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					ADD_SSE2::ADD::Add32sc(s1, s2, dst, scale);
				}

				ISV Add_32fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					dst.f = _mm_add_ps(src1.f, src2.f);
				}

				ISV Add_64fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					dst.d = _mm_add_pd(src1.d, src2.d);
				}

				ISV Add_8u16u_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 temp;
					XMM128 tsrc1 = src1, tsrc2 = src2;
					CBL_SSE2::Unpack8UTo16U(tsrc1.i, temp.i);
					CBL_SSE2::Unpack8UTo16U(tsrc2.i, temp.i);

					dst.i  = ::_mm_add_epi16( tsrc2.i, tsrc1.i );
				}

				ISV Add_16s32f_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 temp;
					XMM128 tsrc1 = src1, tsrc2 = src2;

					CBL_SSE2::Unpack16STo32S(tsrc1.i, temp.i);
					CBL_SSE2::Unpack16STo32S(tsrc2.i, temp.i);

					tsrc1.f = _mm_cvtepi32_ps (tsrc1.i);
					tsrc2.f = _mm_cvtepi32_ps (tsrc2.i);
					dst.f  = ::_mm_add_ps( tsrc2.f, tsrc1.f );
				}
			}
		}
	}

	namespace ADDC
	{
		ISV AddC16s(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128& scale)

		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);

			srcTemp1.i = _mm_add_epi32(srcTemp1.i, value0.i);
			srcTemp2.i = _mm_add_epi32(srcTemp2.i, value1.i);

			srcTemp1.f		= _mm_cvtepi32_ps (srcTemp1.i);
			srcTemp2.f		= _mm_cvtepi32_ps (srcTemp2.i);

			srcTemp1.f		= _mm_mul_ps(srcTemp1.f, scale.f);
			srcTemp2.f		= _mm_mul_ps(srcTemp2.f, scale.f);

			srcTemp1.i		= _mm_cvtps_epi32(srcTemp1.f);
			srcTemp2.i		= _mm_cvtps_epi32 (srcTemp2.f);

			srcTemp1.i = _mm_packs_epi32(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

        //scale value = 1
		ISV AddC16s(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1)

		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);

			srcTemp1.i = _mm_add_epi32(srcTemp1.i, value0.i);
			srcTemp2.i = _mm_add_epi32(srcTemp2.i, value1.i);
            srcTemp1.i = _mm_srai_epi32(srcTemp1.i,1);
            srcTemp2.i = _mm_srai_epi32(srcTemp2.i,1);

            srcTemp1.i = _mm_packs_epi32(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

		template <int scaleType>
		ISV AddC8u(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128 scaleMasks[])
		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack8UTo16U(srcTemp1.i,srcTemp2.i);
			srcTemp1.i = _mm_add_epi16(srcTemp1.i, value0.i);
			srcTemp2.i = _mm_add_epi16(srcTemp2.i, value1.i);

			srcTemp1.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp1.i, (__m128i*) scaleMasks);
			srcTemp2.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp2.i, (__m128i*) scaleMasks);

			FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
			dst = srcTemp1;
		}

		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV AddC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC, XMM128 &val)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					switch(scaleType)
					{
					case 1:
					case -1:
						Common::InitConst::C1::To16s::From8u(valC, val);
						break;
					default:
						Common::InitConst::C1::To8u::From8u(valC, val);
					}
				}

				template <int scaleType>
				ISV AddC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC, XMM128 &val)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C1::To32s::From16s(valC, val);
						break;
                    case 2:
                        Common::InitConst::C1::To32s::From16s(valC, val);
                        break;
					default:;
						Common::InitConst::C1::To16s::From16s(valC, val);
					}
				}

				template <int scaleType>
				ISV AddC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC, XMM128& val)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C1::To32sc::From16sc(valC, val);
						break;
					default:;
						Common::InitConst::C1::To16s::From16sc(valC, val);
					}
				}

				ISV AddC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC, XMM128 &val)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::C1::To32sc::From32sc(valC, val);
				}

				ISV AddC_32f_setup(Fw32f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV AddC_64f_setup(Fw64f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV AddC_32fc_setup(Fw32fc valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV AddC_64fc_setup(Fw64fc valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}
			}
			namespace C3
			{
				template <int scaleType>
				ISV AddC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					switch(scaleType)
					{
					case 1:
					case -1:
						Common::InitConst::C3::To16s::From8u(valC, val0, val1, val2);
						break;
					default:
						Common::InitConst::C3::To8u::From8u(valC, val0, val1, val2);
					}
				}

				template <int scaleType>
				ISV AddC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C3::To32s::From16s(valC, val0, val1, val2);
						break;
                    case 2:
                        Common::InitConst::C3::To32s::From16s(valC, val0, val1, val2);
                        break;
					default:;
						Common::InitConst::C3::To16s::From16s(valC, val0, val1, val2);
					}
				}

				template <int scaleType>
				ISV AddC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C3::To32sc::From16sc(valC, val0, val1, val2);
						break;
					default:;
						Common::InitConst::C3::To16sc::From16sc(valC, val0, val1, val2);
					}
				}

				ISV AddC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::C3::To32sc::From32sc(valC, val0, val1, val2);
				}

				ISV AddC_32f_setup(Fw32f valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitConst::C3::To32f::From32f(valC, val0, val1, val2);
				}

				ISV AddC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitConst::C3::To32fc::From32fc(valC, val0, val1, val2);
				}
			}
			namespace C4
			{
				template <int scaleType>
				ISV AddC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					switch(scaleType)
					{
					case 1:
					case -1:
						Common::InitConst::C4::To16s::From8u(valC, val);
						break;
					default:
						Common::InitConst::C4::To8u::From8u(valC, val);
					}
				}

				template <int scaleType>
				ISV AddC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C4::To32s::From16s(valC, val);
						break;
                    case 2:
                        Common::InitConst::C4::To32s::From16s(valC, val);
                        break;
					default:;
						Common::InitConst::C4::To16s::From16s(valC, val);
					}
				}

				ISV AddC_32f_setup(Fw32f valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}
			}
			namespace AC4
			{
				template <int scaleType>
				ISV AddC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val, XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					switch(scaleType)
					{
					case 1:
					case -1:
						Common::InitConst::AC4::To16s::From8u(valC, val);
						break;
					default:
						Common::InitConst::AC4::To8u::From8u(valC, val);
					}
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV AddC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::AC4::To32s::From16s(valC, val);
						break;
                    case 2:
                        Common::InitConst::AC4::To32s::From16s(valC, val);
                        break;
					default:;
						Common::InitConst::AC4::To16s::From16s(valC, val);
					}
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV AddC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val, XMM128 &val1, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::AC4::To32sc::From16sc(valC, val, val1);
						break;
					default:;
						Common::InitConst::AC4::To16sc::From16sc(valC, val);
					}
					Common::AC4::Init::For32(ac4mask);
				}

				ISV AddC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::AC4::To32sc::From32sc(valC, val0, val1);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV AddC_32f_setup(Fw32f valC[], XMM128 &val, XMM128 &ac4mask)
				{
					Common::InitConst::AC4::To32f::From32f(valC, val);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV AddC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &ac4mask)
				{
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
				ISV AddC_8u_unit(const XMM128 &src, const XMM128 &valClo, const XMM128 &valChi, XMM128 &dst, const XMM128 scaleMasks[])
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADDC::AddC8u<scaleType>(src, dst, valClo, valChi, scaleMasks);
						break;
					default:
						dst.i = _mm_adds_epu8(src.i, valClo.i);
					}
				}

				ISV AddC_8u_scale_zero_unit(const XMM128 &src, const XMM128 &valC, XMM128 &dst)
				{
					dst.i = _mm_adds_epu8(src.i, valC.i);
				}

				template <int scaleType>
				ISV AddC_16s_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scaleReg, const XMM128 &val0, const XMM128 &val1)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADDC::AddC16s(src, dst, val0, val1, scaleReg);
						break;
                    case 2: // scale value = 1
                        ADD_SSE2::ADDC::AddC16s(src, dst, val0, val1);
                        break;
					default:
						dst.i = _mm_adds_epi16(src.i, val0.i);
					}
				}

				template <int scaleType>
				ISV AddC_16sc_unit( const XMM128 &src, const XMM128 &val, const XMM128 &val1, XMM128 &dst, const XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADDC::AddC16s(src, dst, val, val1, scaleReg);
						break;
					default:
						dst.i = _mm_adds_epi16(src.i, val.i);
					}
				}

				ISV AddC_32sc_unit( const XMM128& s, const XMM128& val, XMM128& dst, const XMM128 &scale)
				{
					ADD_SSE2::ADD::Add32sc(s, val, dst, scale);
				}

				ISV AddC_32fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.f = _mm_add_ps(src.f, val.f);
				}

				ISV AddC_64fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.d = _mm_add_pd(src.d, val.d);
				}
			}
		}
	}
	namespace ADDX
	{
		ISV Setup_mask(XMM128 &m1, XMM128 &m2,XMM128 &Src)
		{
			const __m128i mask = _mm_setzero_si128();
			const __m128i mask2 = _mm_cmpeq_epi32(mask,mask);
			m1.i = _mm_cmpeq_epi32(Src.i,mask);
			m2.i = _mm_andnot_si128(m1.i,mask2);
		}
		namespace Init
		{
			ISV AddX_16s_setup(int scale, XMM128 &scaleReg)
			{
				Common::InitScale::To32f(scale, scaleReg);
			}
			ISV AddX_32s_setup(int scale, XMM128 &scaleReg)
			{
				Common::InitScale::To64f(scale, scaleReg);
			}
			ISV AddX_32f_setup(Fw32f val, XMM128 &reg)
			{
				FW_SSE2::Load1(reg, val);
			}
		}
	}
}

} // namespace OPT_LEVEL


#endif	// __ADDIMPL_H__

