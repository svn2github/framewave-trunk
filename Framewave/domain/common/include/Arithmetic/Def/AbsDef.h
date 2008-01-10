/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ABSDEF_H__
#define __ABSDEF_H__

#include "AbsImpl.h"
#include "fe.h"


namespace OPT_LEVEL
{

namespace DEF_ABS
{
	namespace ABS
	{
		namespace C1
		{
			struct Abs_16s	: public fe2<Fw16s, OPT_LEVEL::C1, Fw16s, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[0], r.dst[0]);
				}
				IV REFR (const Fw16s *s, Fw16s *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct Abs_32s	: public fe2<Fw32s, OPT_LEVEL::C1, Fw32s, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABS::Unit::C1::Abs_32s_unit(r.src1[0], r.dst[0]);
				}
				IV REFR (const Fw32s *s, Fw32s *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct Abs_32f	: public fe2<Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1>
			{
				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::C1::Abs_32f_setup(signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f	= _mm_and_ps(r.src1[0].f, signmask.f);
				}
				IV REFR (const Fw32f *s, Fw32f *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct Abs_64f	: public fe2<Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1>
			{
				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::C1::Abs_64f_setup(signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d	= _mm_and_pd(r.src1[0].d, signmask.d);
				}
				IV REFR (const Fw64f *s, Fw64f *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};
		}
		namespace C3
		{
			struct Abs_16s	: public fe2<Fw16s, OPT_LEVEL::C3, Fw16s, OPT_LEVEL::C3>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[0], r.dst[0]);
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[1], r.dst[1]);
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[2], r.dst[2]);
				}
				IV REFR (const Fw16s *s, Fw16s *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}
			};

			struct Abs_32f	: public fe2<Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3>
			{
				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::C3::Abs_32f_setup(signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f	= _mm_and_ps(r.src1[0].f, signmask.f);
					r.dst[1].f	= _mm_and_ps(r.src1[1].f, signmask.f);
					r.dst[2].f	= _mm_and_ps(r.src1[2].f, signmask.f);
				}
				IV REFR (const Fw32f *s, Fw32f *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}
			};
		}
		namespace AC4
		{
			struct Abs_16s	: public fe2<Fw16s, OPT_LEVEL::AC4, Fw16s, OPT_LEVEL::AC4>
			{
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::AC4::Abs_16s_setup(mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[0], temp);
					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}
				IV REFR (const Fw16s *s, Fw16s *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};

			struct Abs_32f	: public fe2<Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4>
			{
				XMM128 signmask, mask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::AC4::Abs_32f_setup(signmask, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_and_ps(r.src1[0].f, signmask.f);
					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}
				IV REFR (const Fw32f *s, Fw32f *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};
		}
		namespace C4
		{
			struct Abs_16s	: public fe2<Fw16s, OPT_LEVEL::C4, Fw16s, OPT_LEVEL::C4>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABS::Unit::C1::Abs_16s_unit(r.src1[0], r.dst[0]);
				}
				IV REFR (const Fw16s *s, Fw16s *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}
			};

			struct Abs_32f	: public fe2<Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4>
			{
				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABS::Init::C4::Abs_32f_setup(signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f	= _mm_and_ps(r.src1[0].f, signmask.f);
				}
				IV REFR (const Fw32f *s, Fw32f *d) const
				{
					ABS_REF::ABS::Abs(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}
			};
		}
	}
	namespace ABSDIFF
	{
		namespace C1
		{
			struct AbsDiff_8u	: public fe3<Fw8u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFF::Unit::C1::AbsDiff_8u_unit(r.src1[0], r.src2[0], r.dst[0]);
				}
				IV REFR (const Fw8u *s1, const Fw8u *s2, Fw8u *d) const
				{
					ABS_REF::ABSDIFF::AbsDiff(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct AbsDiff_16u	: public fe3<Fw16u, OPT_LEVEL::C1, Fw16u, OPT_LEVEL::C1, Fw16u, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2_Init()
				{
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFF::Unit::C1::AbsDiff_16u_unit(r.src1[0], r.src2[0], r.dst[0]);
				}
				IV REFR (const Fw16u *s1, const Fw16u *s2, Fw16u *d) const
				{
					ABS_REF::ABSDIFF::AbsDiff(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct AbsDiff_32f	: public fe3<Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1>
			{
				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABSDIFF::Init::C1::AbsDiff_32f_setup(signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFF::Unit::C1::AbsDiff_32f_unit(r.src1[0], r.src2[0], r.dst[0], signmask);
				}
				IV REFR (const Fw32f *s1, const Fw32f *s2, Fw32f *d) const
				{
					ABS_REF::ABSDIFF::AbsDiff(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};
		}
	}

	namespace ABSDIFFC
	{
		namespace C1
		{
			struct AbsDiffC_8u	: public fe2<Fw8u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1>
			{
				Fw8u valC; XMM128 val;
				AbsDiffC_8u(Fw8u valC_)
				{
					valC = valC_;
				}

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABSDIFFC::Init::C1::AbsDiffC_8u_setup(valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFFC::Unit::C1::AbsDiffC_8u_unit(r.src1[0], r.dst[0], val);
				}
				IV REFR (const Fw8u *s,  Fw8u *d) const
				{
					ABS_REF::ABSDIFFC::AbsDiffC(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, valC);
				}
			};

			struct AbsDiffC_16u	: public fe2<Fw16u, OPT_LEVEL::C1, Fw16u, OPT_LEVEL::C1>
			{
				Fw16u valC; XMM128 val;
				AbsDiffC_16u(Fw16u valC_)
				{
					valC = valC_;
				}

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABSDIFFC::Init::C1::AbsDiffC_16u_setup(valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFFC::Unit::C1::AbsDiffC_16u_unit(r.src1[0], r.dst[0], val);
				}
				IV REFR (const Fw16u *s, Fw16u *d) const
				{
					ABS_REF::ABSDIFFC::AbsDiffC(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, valC);
				}
			};

			struct AbsDiffC_32f	: public fe2<Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1>
			{
				Fw32f valC; XMM128 val;
				AbsDiffC_32f(Fw32f valC_)
				{
					valC = valC_;
				}

				XMM128 signmask;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ABS_SSE2::ABSDIFFC::Init::C1::AbsDiffC_32f_setup(valC, val, signmask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ABS_SSE2::ABSDIFFC::Unit::C1::AbsDiffC_32f_unit(r.src1[0], r.dst[0], signmask, val);
				}
				IV REFR (const Fw32f *s, Fw32f *d) const
				{
					ABS_REF::ABSDIFFC::AbsDiffC(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, valC);
				}
			};
		}
	}
}

} // namespace OPT_LEVEL

#endif // __ABSDEF_H__

