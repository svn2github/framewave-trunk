/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ABSIMPL_H__
#define __ABSIMPL_H__

#include "InitImpl.h"
#include "AC4Impl.h"

namespace OPT_LEVEL
{
namespace ABS_REF
{
	namespace ABS
	{
		template< class TS, class TD >
		ISV Abs( const TS * s, CH cs, TD * d, CH cd )
		{
			ASSERT( ChannelCount(cs) == ChannelCount(cd) ); cs; cd;

			switch( cd )
			{
			case C4:	d[3] = FW_REF::Limits<TD>::Sat( FW_REF::ABS_S<Fw64f>::ABS( (Fw64f) s[3] ));
			case C3:
			case AC4:	d[2] = FW_REF::Limits<TD>::Sat( FW_REF::ABS_S<Fw64f>::ABS( (Fw64f) s[2] ));
						d[1] = FW_REF::Limits<TD>::Sat( FW_REF::ABS_S<Fw64f>::ABS( (Fw64f) s[1] ));
			case C1:	d[0] = FW_REF::Limits<TD>::Sat( FW_REF::ABS_S<Fw64f>::ABS( (Fw64f) s[0] ));
			}
		}
	}

	namespace ABSDIFF
	{
		template< class TS1, class TS2, class TD >
		ISV AbsDiff( const TS1 * s1, CH cs1, const TS2 *s2, CH cs2, TD * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs1) ); cs1;
			ASSERT( 1 == ChannelCount(cs2) ); cs2;
			ASSERT( 1 == ChannelCount(cd)  ); cd;

			*d = FW_REF::Limits<TD>::Sat(FW_REF::ABS_S<Fw32s>::ABS( (Fw32s)*s1 - *s2 ));
		}

		ISV AbsDiff( const Fw32f * s1, CH cs1, const Fw32f *s2, CH cs2, Fw32f * d, CH cd )
		{
			ASSERT( 1 == ChannelCount(cs1) ); cs1;
			ASSERT( 1 == ChannelCount(cs2) ); cs2;
			ASSERT( 1 == ChannelCount(cd)  ); cd;

			*d = FW_REF::ABS_S<Fw32f>::ABS( *s1 - *s2 );
		}
	}

	namespace ABSDIFFC
	{
		template< class TS, class TD >
		ISV AbsDiffC( const TS * s, CH cs, TD * d, CH cd, TD v )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs;
			ASSERT( 1 == ChannelCount(cd) ); cd;

			*d = FW_REF::Limits<TD>::Sat(FW_REF::ABS_S<Fw32s>::ABS( (Fw32s)*s - v ));
		}

		ISV AbsDiffC( const Fw32f * s, CH cs, Fw32f * d, CH cd, Fw32f v )
		{
			ASSERT( 1 == ChannelCount(cs) ); cs;
			ASSERT( 1 == ChannelCount(cd) ); cd;

			*d = FW_REF::ABS_S<Fw32f>::ABS( *s - v );
		}
	}
}

namespace ABS_SSE2
{
	namespace ABS
	{
		namespace Init
		{
			namespace C1
			{
				ISV Abs_32f_setup(XMM128 &signmask)
				{
					Common::InitMisc::Max::Of32s(signmask);
				}

				ISV Abs_64f_setup(XMM128 &signmask)
				{
					Common::InitMisc::Max::Of64s(signmask);
				}
			}
			namespace C3
			{
				ISV Abs_32f_setup(XMM128 &signmask)
				{
					Common::InitMisc::Max::Of32s(signmask);
				}
			}
			namespace AC4
			{
				ISV Abs_16s_setup(XMM128 &ac4mask)
				{
					Common::AC4::Init::For16(ac4mask);
				}
				ISV Abs_32f_setup(XMM128 &signmask, XMM128 &ac4mask)
				{
					Common::InitMisc::Max::Of32s(signmask);
					Common::AC4::Init::For32(ac4mask);
				}
			}
			namespace C4
			{
				ISV Abs_32f_setup(XMM128 &signmask)
				{
					Common::InitMisc::Max::Of32s(signmask);
				}
			}
		}
		namespace Unit
		{
			namespace C1
			{
				ISV Abs_16s_unit(XMM128 &src, XMM128 &dst)
				{
					 /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
					XMM128 complement, temp  ;
					temp.i = _mm_srai_epi16(src.i, 15);
					complement.i = _mm_xor_si128(src.i, temp.i);  //one's complement of src
					dst.i = _mm_sub_epi16 (complement.i, temp.i);
					temp.i  = _mm_srli_epi16(dst.i, 15);
					dst.i = _mm_sub_epi16(dst.i, temp.i);
				}

				ISV Abs_32s_unit(XMM128 &src, XMM128 &dst)
				{
					XMM128 complement, temp  ;
					temp.i = _mm_srai_epi32(src.i, 31);
					complement.i = _mm_xor_si128(src.i, temp.i);  //one's complement of src
					dst.i = _mm_sub_epi32 (complement.i, temp.i);
					temp.i  = _mm_srli_epi32(dst.i, 31);
					dst.i = _mm_sub_epi32(dst.i, temp.i);
				}
			}
		}
	}

	namespace ABSDIFF
	{
		namespace Init
		{
			namespace C1
			{
				ISV AbsDiff_32f_setup(XMM128 &signmask)
				{
					Common::InitMisc::Max::Of32s(signmask);
				}
			}
		}
		namespace Unit
		{
			namespace C1
			{
				ISV AbsDiff_8u_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 min, max;

					min.i = _mm_min_epu8(src1.i, src2.i);
					max.i = _mm_max_epu8(src1.i, src2.i);

					dst.i = _mm_subs_epu8(max.i, min.i);
				}

				ISV AbsDiff_16u_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst)
				{
					XMM128 temp_src1_h,temp_src1_l;
					XMM128 temp_src2_h,temp_src2_l;

					temp_src1_l.i = src1.i;
					CBL_SSE2::Unpack16UTo32U(temp_src1_l.i,temp_src1_h.i);
					temp_src2_l.i = src2.i;
					CBL_SSE2::Unpack16UTo32U(temp_src2_l.i,temp_src2_h.i);

					temp_src1_l.i = _mm_sub_epi32(temp_src1_l.i,temp_src2_l.i);
					temp_src1_h.i = _mm_sub_epi32(temp_src1_h.i,temp_src2_h.i);

					temp_src2_l.i = _mm_srai_epi32(temp_src1_l.i,31);
					temp_src1_l.i = _mm_xor_si128 (temp_src1_l.i,temp_src2_l.i);
					temp_src1_l.i = _mm_sub_epi32(temp_src1_l.i,temp_src2_l.i);

					temp_src2_h.i = _mm_srai_epi32(temp_src1_h.i,31);
					temp_src1_h.i = _mm_xor_si128 (temp_src1_h.i,temp_src2_h.i);
					temp_src1_h.i = _mm_sub_epi32(temp_src1_h.i,temp_src2_h.i);

					dst.i  =   _mm_set_epi16 (temp_src1_h.s16[6],temp_src1_h.s16[4],temp_src1_h.s16[2],temp_src1_h.s16[0],	\
										temp_src1_l.s16[6],temp_src1_l.s16[4],temp_src1_l.s16[2],temp_src1_l.s16[0]);
				}

				ISV AbsDiff_32f_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const XMM128 &signmask)
				{
					dst.f = _mm_sub_ps(src1.f, src2.f); // do the subtraction
					dst.f = _mm_and_ps(dst.f, signmask.f); // make the sign bit zero
				}
			}
		}	
	}

	namespace ABSDIFFC
	{
		namespace Init
		{
			namespace C1
			{
				ISV AbsDiffC_8u_setup(Fw8u valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV AbsDiffC_16u_setup(Fw16u valC, XMM128 &val)
				{
					FW_SSE2::Load1(val, valC);
				}

				ISV AbsDiffC_32f_setup(Fw32f valC, XMM128 &val, XMM128 &signmask)
				{
					FW_SSE2::Load1(val, valC);
					Common::InitMisc::Max::Of32s(signmask);
				}
			}
		}
		namespace Unit
		{
			namespace C1
			{
				ISV AbsDiffC_8u_unit(const XMM128 &src, XMM128 &dst, const XMM128 &val)
				{
					XMM128 min, max;

					min.i = _mm_min_epu8(src.i, val.i);
					max.i = _mm_max_epu8(src.i, val.i);

					dst.i = _mm_subs_epu8(max.i,min.i);
				}

				ISV AbsDiffC_16u_unit(const XMM128 &src, XMM128 &dst, const XMM128 &val)
				{
					XMM128 temp_src1_h,temp_src1_l;
					XMM128 temp_src2_h,temp_src2_l;

					temp_src1_l.i = src.i;
					CBL_SSE2::Unpack16UTo32U(temp_src1_l.i,temp_src1_h.i);
					temp_src2_l.i = 	val.i;
					CBL_SSE2::Unpack16UTo32U(temp_src2_l.i,temp_src2_h.i);

					temp_src1_l.i = _mm_sub_epi32(temp_src1_l.i,temp_src2_l.i);
					temp_src1_h.i = _mm_sub_epi32(temp_src1_h.i,temp_src2_h.i);

					temp_src2_l.i = _mm_srai_epi32(temp_src1_l.i,31);
					temp_src1_l.i = _mm_xor_si128 (temp_src1_l.i,temp_src2_l.i);
					temp_src1_l.i = _mm_sub_epi32(temp_src1_l.i,temp_src2_l.i);

					temp_src2_h.i = _mm_srai_epi32(temp_src1_h.i,31);
					temp_src1_h.i = _mm_xor_si128 (temp_src1_h.i,temp_src2_h.i);
					temp_src1_h.i = _mm_sub_epi32(temp_src1_h.i,temp_src2_h.i);

					dst.i  =   _mm_set_epi16 (temp_src1_h.s16[6],temp_src1_h.s16[4],temp_src1_h.s16[2],temp_src1_h.s16[0],	\
									temp_src1_l.s16[6],temp_src1_l.s16[4],temp_src1_l.s16[2],temp_src1_l.s16[0]);
				}

				ISV AbsDiffC_32f_unit(const XMM128 &src, XMM128 &dst, const XMM128 &signmask, const XMM128 &val)
				{
					dst.f = _mm_sub_ps(src.f, val.f); // do the subtraction
					dst.f = _mm_and_ps(dst.f, signmask.f); // make the sign bit zero
				}
			}
		}	
	}
}

} // namespace OPT_LEVEL

#endif // __ABSIMPL_H__

