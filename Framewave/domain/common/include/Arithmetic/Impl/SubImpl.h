/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SUBIMPL_H__
#define __SUBIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"


namespace OPT_LEVEL
{

namespace SUB_REF
{
	namespace SUB
	{
		template< class TS1, class TS2, class TD >
		ISV SubF( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = (TD)((s2[3] - s1[3]));
			case C3:
			case AC4:	d[2] = (TD)((s2[2] - s1[2]));
						d[1] = (TD)((s2[1] - s1[1]));
			case C1:	d[0] = (TD)((s2[0] - s1[0]));
			}
		}

		template< class TS1, class TS2, class TD >
		ISV SubI( const TS1 * s1, CH cs1, const TS2 * s2, CH cs2, TD * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s2[3] - s1[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s2[2] - s1[2]), scale) );
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s2[1] - s1[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( (s2[0] - s1[0]), scale) );
			}
		}

		SYS_INLINE STATIC void SubI( const A32S * s1, CH cs1, const A32S * s2, CH cs2, A32S * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( ((A64F)s2[3] - s1[3]), scale) ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( ((A64F)s2[2] - s1[2]), scale)) );
						d[1] = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( ((A64F)s2[1] - s1[1]), scale)) );
			case C1:	d[0] = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale( ((A64F)s2[0] - s1[0]), scale)) );
			}
		}

		SYS_INLINE STATIC Fw32sc sub_16s(Fw16sc a, Fw16sc b)
		{
			Fw32sc c;
			c.re = b.re - a.re ;
			c.im = b.im - a.im ;
			return c;
		}
		SYS_INLINE STATIC Fw64sc sub_32s(Fw32sc a, Fw32sc b)
		{
			Fw64sc c;
			c.re = (Fw64s)b.re - (Fw64s)a.re ;
			c.im = (Fw64s)b.im - (Fw64s)a.im ;
			return c;
		}

		ISV SubI( const Fw16sc * s1, CH cs1, const Fw16sc * s2, CH cs2, Fw16sc * d, CH cd, int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( sub_16s(s1[3] , s2[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( sub_16s(s1[2] , s2[2]), scale) );
						d[1] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( sub_16s(s1[1] , s2[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( sub_16s(s1[0] , s2[0]), scale) );
			}
		}

		ISV SubI( const Fw32sc * s1, CH cs1, const Fw32sc * s2, CH cs2, Fw32sc * d, CH cd,  int scale )
		{
			ASSERT( ChannelCount(cs1) == ChannelCount(cs2) );
			ASSERT( ChannelCount(cs2) == ChannelCount(cd ) ); cs1; cs2;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( sub_32s(s1[3] , s2[3]), scale) );
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( sub_32s(s1[2] , s2[2]), scale) );
						d[1] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( sub_32s(s1[1] , s2[1]), scale) );
			case C1:	d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( sub_32s(s1[0] , s2[0]), scale) );
			}
		}
	}

	namespace SUBC
	{
		IS Fw32sc subc_16s(Fw16sc a, Fw16sc b)
		{
			Fw32sc c;
			c.re = (Fw32s)a.re - b.re ;
			c.im = (Fw32s)a.im - b.im ;
			return c;
		}
		IS Fw64sc subc_32s(Fw32sc a, Fw32sc b)
		{
			Fw64sc c;
			c.re = (Fw64s)a.re - (Fw64s)b.re ;
			c.im = (Fw64s)a.im - (Fw64s)b.im ;
			return c;
		}


		template< class TS, class TD >
		ISV SubCI_1( const TS * s, CH cs, TD * d, CH cd, const TD v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<TD>::Sat( FW_REF::Scale(*s - v, scale ) );
		}

		ISV SubCI_1( const A32S * s, CH cs, A32S * d, CH cd, A32S v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<A32S>::Sat( CBL_LIBRARY::DoubleToLong(FW_REF::Scale((A64F)*s - v, scale ) ));
		}

		ISV SubCI_1( const A32SC * s, CH cs, A32SC * d, CH cd, A32SC v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_32s(s[0] , v), scale ));
		}

		ISV SubCI_1( const A16SC * s, CH cs, A16SC * d, CH cd, A16SC v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_16s(s[0] , v), scale ));
		}

		template< class TS, class TD >
		ISV SubCI( const TS * s, CH cs, TD * d, CH cd, const TD *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[3] - v[3], scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[2] - v[2], scale ));
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[1] - v[1], scale ));
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::Scale( s[0] - v[0], scale ));
			}
		}

		ISV SubCI( const Fw16sc * s, CH cs, Fw16sc * d, CH cd, const Fw16sc *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_16s(s[3] , v[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_16s(s[2] , v[2]), scale ));
						d[1] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_16s(s[1] , v[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_16s(s[0] , v[0]), scale ));
			}
		}

		ISV SubCI( const Fw32sc * s, CH cs, Fw32sc * d, CH cd, const Fw32sc *v, int scale )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_32s(s[3] , v[3]), scale ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_32s(s[2] , v[2]), scale ));
						d[1] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_32s(s[1] , v[1]), scale ));
			case C1:	d[0] = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale( SUB_REF::SUBC::subc_32s(s[0] , v[0]), scale ));
			}
		}


		template< class TS, class TD >
		ISV SubCF_1( const TS * s, CH, TD * d, CH, const TD val )
		{
			d[0] = (TD)((s[0] -  val) );
		}

		template< class TS, class TD >
		ISV SubCF( const TS * s, CH cs, TD * d, CH cd, const TD *v )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs;

			switch( cd )
			{
			case C4:	d[3] = (TD)((s[3] - v[3]) );
			case C3:
			case AC4:	d[2] = (TD)((s[2] - v[2]) );
						d[1] = (TD)((s[1] - v[1]) );
			case C1:	d[0] = (TD)((s[0] - v[0]) );
			}
		}
	}

	namespace SUBCREV
	{


		ISV SubCRevI_1( const Fw16sc * s, CH cs, Fw16sc * d, CH cd, Fw16sc v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<Fw16sc>::Sat( FW_REF::Scale(SUB_REF::SUBC::subc_16s(v,*s ), scale ) );
		}

		ISV SubCRevI_1( const Fw32sc * s, CH cs, Fw32sc * d, CH cd, Fw32sc v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<Fw32sc>::Sat( FW_REF::Scale(SUB_REF::SUBC::subc_32s(v,*s), scale ) );
		}

		ISV SubCRevI_1( const A32S * s, CH cs, A32S * d, CH cd, A32S v, int scale )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<A32S>::Sat(CBL_LIBRARY::DoubleToLong( FW_REF::Scale((A64F)v - *s, scale ) ));
		}

		template< class TS, class TD >
		ISV SubCRevI_1( const TS * s, CH cs, TD * d, CH cd, TD v, int scale)
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			*d = FW_REF::Limits<TD>::Sat( FW_REF::Scale(v - *s, scale ) );
		}

		template< class TS, class TD >
		ISV SubCRevF_1( const TS * s, CH cs, TD * d, CH cd, TD val )
		{
			ASSERT( 1 == ChannelCount(cd) ); cd;
			ASSERT( 1 == ChannelCount(cs) ); cs;

			d[0] = (TD)(val - s[0]);
		}
	}
}

namespace SUB_SSE2
{
	namespace SUB
	{
		ISV Setup_mask(XMM128 &m1, XMM128 &m2,XMM128 &Src)
		{
			const __m128i mask = _mm_setzero_si128();
			const __m128i mask2 = _mm_cmpeq_epi32(mask,mask);
			m1.i = _mm_cmpeq_epi32(Src.i,mask);
			m2.i = _mm_andnot_si128(m1.i,mask2);
		}

		ISV Sub16s(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128& scale)
		{
			XMM128 srcTemp1, srcTemp2;
			XMM128 srcTemp1_2, srcTemp2_2;
			srcTemp1	= src2;
			srcTemp1_2	= src1;

			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);
			CBL_SSE2::Unpack16STo32S(srcTemp1_2.i,srcTemp2_2.i);

			srcTemp1.i = _mm_sub_epi32(srcTemp1.i, srcTemp1_2.i);
			srcTemp2.i = _mm_sub_epi32(srcTemp2.i, srcTemp2_2.i);

			srcTemp1.f		= _mm_cvtepi32_ps (srcTemp1.i);
			srcTemp2.f		= _mm_cvtepi32_ps (srcTemp2.i);

			srcTemp1.f		= _mm_mul_ps(srcTemp1.f, scale.f);
			srcTemp2.f		= _mm_mul_ps(srcTemp2.f, scale.f);

			srcTemp1.i		= _mm_cvtps_epi32(srcTemp1.f);
			srcTemp2.i		= _mm_cvtps_epi32 (srcTemp2.f);

			FW_SSE2::pack32STo16S(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

		ISV Sub32sc(const XMM128 &src1, const XMM128 &src2, XMM128& dst, const XMM128& scale)
		{
			XMM128 srcTemp1_1,srcTemp2_1;
			XMM128 srcTemp1_2, srcTemp2_2;

			srcTemp1_1.d = _mm_cvtepi32_pd(src2.i);
			srcTemp2_1.d = _mm_cvtepi32_pd(src1.i);

			srcTemp1_2.i = _mm_srli_si128(src2.i, 8);
			srcTemp2_2.i = _mm_srli_si128(src1.i, 8);

			srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
			srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

			srcTemp1_1.d = _mm_sub_pd(srcTemp1_1.d, srcTemp2_1.d);
			srcTemp1_2.d = _mm_sub_pd(srcTemp1_2.d, srcTemp2_2.d);

			dst.d = _mm_mul_pd(srcTemp1_1.d, scale.d);
			srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, scale.d);

			dst.i = CBL_SSE2::Pack64FTo32S (dst.d, srcTemp1_2.d);
		}


		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV Sub_8u_setup(int scale, XMM128 scaleMasks[])
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
				}

				template <int scaleType>
				ISV Sub_16s_setup(int scale, XMM128 &scaleReg)
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
				ISV Sub_16sc_setup(int scale, XMM128 &scaleReg)
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

				ISV Sub_32s_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}

				ISV Sub_32sc_setup(int scale, XMM128 &scaleReg)
				{
					Common::InitScale::To64f(scale, scaleReg);
				}
			}

			namespace AC4
			{
				template <int scaleType>
				ISV Sub_8u_setup(int scale, XMM128 scaleMasks[], XMM128 &ac4mask)
				{
					CBL_SSE2::Setup_ScaleMasks_16s<scaleType>(scale, (__m128i*) scaleMasks);
					Common::AC4::Init::For8(ac4mask);
				}

				template <int scaleType>
				ISV Sub_16s_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
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
				ISV Sub_16sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
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

				ISV Sub_32sc_setup(int scale, XMM128 &scaleReg, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Sub_32fc_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For64(ac4mask);
				}

				ISV Sub_32f_setup(XMM128 &ac4mask)
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
				ISV Sub_8u_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 *scaleMasks)
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

						srcTemp1.i = _mm_subs_epu16(srcTemp1_2.i, srcTemp1.i );
						srcTemp2.i = _mm_subs_epu16(srcTemp2_2.i, srcTemp2.i );
						
						srcTemp1.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp1.i, (__m128i *) scaleMasks);
						srcTemp2.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp2.i, (__m128i *) scaleMasks);

						FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
						dst = srcTemp1;						
						break;
					default:
						dst.i = _mm_subs_epu8(src2.i, src1.i);
					}
				}


				template <int scaleType>
				ISV Sub_16s_unit( const XMM128& src1, const XMM128& src2, XMM128& dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUB::Sub16s(src1, src2, dst, scale);
						break;
					case 0:
						dst.i = _mm_subs_epi16(src2.i, src1.i);
						break;
					}
				}

				template <int scaleType>
				ISV Sub_16sc_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128 &scale)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUB::Sub16s(src1, src2, dst, scale);
						break;
					case 0:
						dst.i = _mm_subs_epi16(src2.i, src1.i);
						break;
					}
				}

				ISV Sub_32s_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					SUB_SSE2::SUB::Sub32sc(s1, s2, dst, scale);
				}

				ISV Sub_32sc_unit( const XMM128& s1, const XMM128& s2, XMM128& dst, const XMM128 &scale)
				{
					SUB_SSE2::SUB::Sub32sc(s1, s2, dst, scale);
				}

				ISV Sub_32fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					dst.f = _mm_sub_ps(src2.f, src1.f);
				}

				ISV Sub_64fc_unit(XMM128 &src1, XMM128& src2, XMM128 &dst)
				{
					dst.d = _mm_sub_pd(src2.d, src1.d);
				}

				ISV Sub_16s32f_unit( const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 temp;
					XMM128 tsrc1 = src1, tsrc2 = src2;

					CBL_SSE2::Unpack16STo32S(tsrc1.i, temp.i);
					CBL_SSE2::Unpack16STo32S(tsrc2.i, temp.i);

					tsrc1.f = _mm_cvtepi32_ps (tsrc1.i);
					tsrc2.f = _mm_cvtepi32_ps (tsrc2.i);
					dst.f  = ::_mm_sub_ps( tsrc2.f, tsrc1.f );
				}
			}
		}
	}

	namespace SUBC
	{
		ISV SubC16s(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128& scale)

		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);

			srcTemp1.i = _mm_sub_epi32(srcTemp1.i, value0.i);
			srcTemp2.i = _mm_sub_epi32(srcTemp2.i, value1.i);

			srcTemp1.f		= _mm_cvtepi32_ps (srcTemp1.i);
			srcTemp2.f		= _mm_cvtepi32_ps (srcTemp2.i);

			srcTemp1.f		= _mm_mul_ps(srcTemp1.f, scale.f);
			srcTemp2.f		= _mm_mul_ps(srcTemp2.f, scale.f);

			srcTemp1.i		= _mm_cvtps_epi32(srcTemp1.f);
			srcTemp2.i		= _mm_cvtps_epi32 (srcTemp2.f);

			FW_SSE2::pack32STo16S(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

		template <int scaleType>
		ISV SubC8u(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128 scaleMasks[])
		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack8UTo16U(srcTemp1.i,srcTemp2.i);
			srcTemp1.i = _mm_subs_epu16(srcTemp1.i, value0.i);
			srcTemp2.i = _mm_subs_epu16(srcTemp2.i, value1.i);

			srcTemp1.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp1.i, (__m128i*) scaleMasks);
			srcTemp2.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp2.i, (__m128i*) scaleMasks);

			FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
			dst = srcTemp1;
		}

		ISV SubC32sc(const XMM128 &src1, const XMM128 &src2, XMM128& dst, const XMM128& scale)
		{
			XMM128 srcTemp1_1,srcTemp2_1;
			XMM128 srcTemp1_2, srcTemp2_2;

			srcTemp1_1.d = _mm_cvtepi32_pd(src1.i);
			srcTemp2_1.d = _mm_cvtepi32_pd(src2.i);

			srcTemp1_2.i = _mm_srli_si128(src1.i, 8);
			srcTemp2_2.i = _mm_srli_si128(src2.i, 8);

			srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
			srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

			srcTemp1_1.d = _mm_sub_pd(srcTemp1_1.d, srcTemp2_1.d);
			srcTemp1_2.d = _mm_sub_pd(srcTemp1_2.d, srcTemp2_2.d);

			srcTemp1_1.d = _mm_mul_pd(srcTemp1_1.d, scale.d);
			srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, scale.d);

			srcTemp1_1.i = CBL_SSE2::Pack64FTo32S (srcTemp1_1.d, srcTemp1_2.d);

			dst = srcTemp1_1;
		}

		namespace Init
		{
			namespace C1
			{
				template <int scaleType>
				ISV SubC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC, XMM128 &val)
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
				ISV SubC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC, XMM128 &val)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C1::To32s::From16s(valC, val);
						break;
					default:;
						Common::InitConst::C1::To16s::From16s(valC, val);
					}
				}

				template <int scaleType>
				ISV SubC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC, XMM128& val)
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

				ISV SubC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC, XMM128 &val)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::C1::To32sc::From32sc(valC, val);
				}

				ISV SubC_32f_setup(Fw32f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV SubC_64f_setup(Fw64f valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV SubC_32fc_setup(Fw32fc valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV SubC_64fc_setup(Fw64fc valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}
			}
			namespace C3
			{
				template <int scaleType>
				ISV SubC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
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
				ISV SubC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C3::To32s::From16s(valC, val0, val1, val2);
						break;
					default:;
						Common::InitConst::C3::To16s::From16s(valC, val0, val1, val2);
					}
				}

				template <int scaleType>
				ISV SubC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
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

				ISV SubC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::C3::To32sc::From32sc(valC, val0, val1, val2);
				}

				ISV SubC_32f_setup(Fw32f valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitConst::C3::To32f::From32f(valC, val0, val1, val2);
				}

				ISV SubC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &val2)
				{
					Common::InitConst::C3::To32fc::From32fc(valC, val0, val1, val2);
				}
			}
			namespace C4
			{
				template <int scaleType>
				ISV SubC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val)
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
				ISV SubC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::C4::To32s::From16s(valC, val);
						break;
					default:;
						Common::InitConst::C4::To16s::From16s(valC, val);
					}
				}

				ISV SubC_32f_setup(Fw32f valC[], XMM128 &val)
				{
					FW_SSE2::Load2(val, valC, OPT_LEVEL::C4, 0);
				}
			}
			namespace AC4
			{
				template <int scaleType>
				ISV SubC_8u_setup(int scale, XMM128 scaleMasks[], Fw8u valC[], XMM128 &val, XMM128 &ac4mask)
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
				ISV SubC_16s_setup(int scale, XMM128 &scaleReg, Fw16s valC[], XMM128 &val, XMM128 &ac4mask)
				{
					switch (scaleType)
					{
					case -1:
					case 1:
						Common::InitScale::To32f(scale, scaleReg);
						Common::InitConst::AC4::To32s::From16s(valC, val);
						break;
					default:;
						Common::InitConst::AC4::To16s::From16s(valC, val);
					}
					Common::AC4::Init::For16(ac4mask);
				}

				template <int scaleType>
				ISV SubC_16sc_setup(int scale, XMM128 &scaleReg, Fw16sc valC[], XMM128 &val, XMM128 &val1, XMM128 &ac4mask)
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

				ISV SubC_32sc_setup(int scale, XMM128 &scaleReg, Fw32sc valC[], XMM128 &val0, XMM128 &val1, XMM128 &ac4mask)
				{
					Common::InitScale::To64f(scale, scaleReg);
					Common::InitConst::AC4::To32sc::From32sc(valC, val0, val1);
					Common::AC4::Init::For64(ac4mask);
				}

				ISV SubC_32f_setup(Fw32f valC[], XMM128 &val, XMM128 &ac4mask)
				{
					Common::InitConst::AC4::To32f::From32f(valC, val);
					Common::AC4::Init::For32(ac4mask);
				}

				ISV SubC_32fc_setup(Fw32fc valC[], XMM128 &val0, XMM128 &val1, XMM128 &ac4mask)
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
				ISV SubC_8u_unit(const XMM128 &src, const XMM128 &valClo, const XMM128 &valChi, XMM128 &dst, const XMM128 scaleMasks[])
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBC::SubC8u<scaleType>(src, dst, valClo, valChi, scaleMasks);
						break;
					default:
						dst.i = _mm_subs_epu8(src.i, valClo.i);
					}
				}
				ISV SubC_8u_scale_zero_unit(const XMM128 &src, const XMM128 &valC, XMM128 &dst)
				{
					dst.i = _mm_subs_epu8(src.i, valC.i);
				}
				
				template <int scaleType>
				ISV SubC_16s_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scaleReg, const XMM128 &val0, const XMM128 &val1)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBC::SubC16s(src, dst, val0, val1, scaleReg);
						break;
					default:
						dst.i = _mm_subs_epi16(src.i, val0.i);
					}
				}

				template <int scaleType>
				ISV SubC_16sc_unit( const XMM128 &src, const XMM128 &val, const XMM128 &val1, XMM128 &dst, const XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBC::SubC16s(src, dst, val, val1, scaleReg);
						break;
					default:
						dst.i = _mm_subs_epi16(src.i, val.i);
					}
				}

				ISV SubC_32sc_unit( const XMM128& s, const XMM128& val, XMM128& dst, const XMM128 &scale)
				{
					SUB_SSE2::SUBC::SubC32sc(s, val, dst, scale);
				}

				ISV SubC_32fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.f = _mm_sub_ps(src.f, val.f);
				}

				ISV SubC_64fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.d = _mm_sub_pd(src.d, val.d);
				}
			}
		}
	}

	namespace SUBCREV
	{

		ISV SubCRev16s(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128& scale)
		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack16STo32S(srcTemp1.i,srcTemp2.i);

			srcTemp1.i = _mm_sub_epi32(value0.i, srcTemp1.i);
			srcTemp2.i = _mm_sub_epi32(value1.i, srcTemp2.i);

			srcTemp1.f		= _mm_cvtepi32_ps (srcTemp1.i);
			srcTemp2.f		= _mm_cvtepi32_ps (srcTemp2.i);

			srcTemp1.f		= _mm_mul_ps(srcTemp1.f, scale.f);
			srcTemp2.f		= _mm_mul_ps(srcTemp2.f, scale.f);

			srcTemp1.i		= _mm_cvtps_epi32(srcTemp1.f);
			srcTemp2.i		= _mm_cvtps_epi32 (srcTemp2.f);

			FW_SSE2::pack32STo16S(srcTemp1.i, srcTemp2.i);

			dst = srcTemp1;
		}

		template <int scaleType>
		ISV SubCRev8u(const XMM128& src, XMM128& dst, const XMM128& value0, const XMM128& value1, const XMM128 scaleMasks[])
		{
			XMM128 srcTemp1, srcTemp2;
			srcTemp1 = src;
			CBL_SSE2::Unpack8UTo16U(srcTemp1.i,srcTemp2.i);
			srcTemp1.i = _mm_subs_epu16(value0.i, srcTemp1.i);
			srcTemp2.i = _mm_subs_epu16(value1.i, srcTemp2.i);

			srcTemp1.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp1.i, (__m128i*) scaleMasks);
			srcTemp2.i = CBL_SSE2::Scale_16s<scaleType>(srcTemp2.i, (__m128i*) scaleMasks);

			FW_SSE2::pack16STo8U(srcTemp1.i,srcTemp2.i);
			dst = srcTemp1;
		}

		ISV SubCRev32sc(const XMM128 &src1, const XMM128 &src2, XMM128& dst, const XMM128& scale)
		{
			XMM128 srcTemp1_1,srcTemp2_1;
			XMM128 srcTemp1_2, srcTemp2_2;

			srcTemp1_1.d = _mm_cvtepi32_pd(src1.i);
			srcTemp2_1.d = _mm_cvtepi32_pd(src2.i);

			srcTemp1_2.i = _mm_srli_si128(src1.i, 8);
			srcTemp2_2.i = _mm_srli_si128(src2.i, 8);

			srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
			srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

			srcTemp1_1.d = _mm_sub_pd(srcTemp2_1.d, srcTemp1_1.d);
			srcTemp1_2.d = _mm_sub_pd(srcTemp2_2.d, srcTemp1_2.d);

			srcTemp1_1.d = _mm_mul_pd(srcTemp1_1.d, scale.d);
			srcTemp1_2.d = _mm_mul_pd(srcTemp1_2.d, scale.d);

			srcTemp1_1.i = CBL_SSE2::Pack64FTo32S (srcTemp1_1.d, srcTemp1_2.d);

			dst = srcTemp1_1;
		}


		namespace Unit
		{
			namespace C1
			{
				template <int scaleType>
				ISV SubCRev_8u_unit(const XMM128 &src, const XMM128 &valClo, const XMM128 &valChi, XMM128 &dst, const XMM128 scaleMasks[])
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBCREV::SubCRev8u<scaleType>(src, dst, valClo, valChi, scaleMasks);
						break;
					default:
						dst.i = _mm_subs_epu8(valClo.i, src.i);
					}
				}


				template <int scaleType>
				ISV SubCRev_16s_unit(const XMM128 &src, XMM128 &dst, const XMM128 &scaleReg, const XMM128 &val0, const XMM128 &val1)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBCREV::SubCRev16s(src, dst, val0, val1, scaleReg);
						break;
					default:
						dst.i = _mm_subs_epi16(val0.i, src.i);
					}
				}

				template <int scaleType>
				ISV SubCRev_16sc_unit( const XMM128 &src, const XMM128 &val, const XMM128 &val1, XMM128 &dst, const XMM128 &scaleReg)
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBCREV::SubCRev16s(src, dst, val, val1, scaleReg);
						break;
					default:
						dst.i = _mm_subs_epi16(val.i, src.i);
					}
				}

				ISV SubCRev_32sc_unit( const XMM128& s, const XMM128& val, XMM128& dst, const XMM128 &scale)
				{
					SUB_SSE2::SUBCREV::SubCRev32sc(s, val, dst, scale);
				}

				ISV SubCRev_32fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.f = _mm_sub_ps(val.f, src.f);
				}

				ISV SubCRev_64fc_unit(const XMM128 &src, const XMM128& val, XMM128 &dst)
				{
					dst.d = _mm_sub_pd(val.d, src.d);
				}
			}
		}
	}
}

} // namespace OPT_LEVEL

#endif	// __SUBIMPL_H__

