/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __DIVDEF_H__
#define __DIVDEF_H__

#include "DivImpl.h"
#include "fe.h"

namespace OPT_LEVEL
{

template<class TS1, class TS2, class TD, CH c>
struct DivI : public fe3< TS1, c, TS2, c, TD, c>   
{
	int scaleFactor;
	XMM128 scale;

	DivI(int scaleFactor_)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIV::DivI(s1, c, s2, c, d, c, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct DivCI_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	DivCI_C1(int scaleFactor_, TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCI_1(s, c, d, c, this->valC, scaleFactor);
	} 
};




template<class TS, class TD, CH c>
struct DivCI_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	DivCI_C3(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}
 
	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct DivCI_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	DivCI_AC4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct DivCI_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	DivCI_C4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCI(s, c, d, c, this->valC, scaleFactor);
	} 
};


template<class TS, class TD>
struct DivCF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	DivCF_C1(const TS valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct DivCRevF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	DivCRevF_C1(const TS valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCRevF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct DivCF_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	DivCF_C3(const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3, this->valC);
	} 
};

template<class TS, class TD>
struct DivCF_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	DivCF_AC4(const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4, this->valC);
	} 
};

template<class TS, class TD>
struct DivCF_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	DivCF_C4(const TD valC[]) : Common::InitConst::C4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		DIV_REF::DIVC::DivCF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4, this->valC);
	} 
};




namespace DEF_DIV
{
	namespace DIV
	{
		namespace C1
		{
			struct Div_8u	: public DivI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				Div_8u(int scaleFactor) : OPT_LEVEL::DivI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				XMM128 ones;
				FE_SSE2_REF 
				IV SSE2_Init()
				{
					DIV_SSE2::DIV::Init::C1::Div_8u_setup(scaleFactor, scale, ones);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_8u_unit(r.src2[0], r.src1[0], r.dst[0], scale, ones);
				}      
			};

			struct Div_16s	: public DivI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				XMM128 ones;
				Div_16s(int scaleFactor) : OPT_LEVEL::DivI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_16s_setup(scaleFactor, scale, ones);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16s_unit(r.src2[0], r.src1[0], r.dst[0], scale, ones);
				}      
			};

			struct Div_32s	: public DivI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				Div_32s(int scaleFactor) : OPT_LEVEL::DivI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32s_setup(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32s_unit(r.src2[0], r.src1[0], r.dst[0], scale);
				}      
			};

			struct Div_32s16s	: public DivI< Fw16s, Fw32s, Fw16s, OPT_LEVEL::C1 >
			{
				Div_32s16s(int scaleFactor) : OPT_LEVEL::DivI< Fw16s, Fw32s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32s_setup(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					__m128i signext = _mm_srai_epi16(r.src1[0].i, 15);
					r.src1[0].i = _mm_unpacklo_epi16( r.src1[0].i, signext );
					DIV_SSE2::DIV::Unit::C1::Div_32s_unit(r.src1[0],r.src2[0],r.dst[0],scale);
					r.dst[0].i = _mm_packs_epi32(r.dst[0].i,r.dst[0].i);   				
				}      
			};

			struct Div_16sc	: public DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				XMM128 min32s;
				Div_16sc(int scaleFactor) : OPT_LEVEL::DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_16sc_setup(scaleFactor, scale, min32s);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src2[0], r.src1[0], r.dst[0], scale, min32s);
				}      
			};

			struct Div_32sc	: public DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				Div_32sc(int scaleFactor) : OPT_LEVEL::DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32sc_setup(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[0], r.src1[0], r.dst[0], scale);
				}      
			};

			struct Div_32f	: public fe3< Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_div_ps( r.src2[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Div_64f	: public fe3< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_div_pd( r.src2[0].d, r.src1[0].d );
				}      
				IV REFR(const Fw64f *s1, const Fw64f *s2, Fw64f *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Div_32fc	: public fe3< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				XMM128 negbmask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32fc_setup(negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[0], r.src1[0], r.dst[0], negbmask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Div_64fc	: public fe3< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_64fc_unit( r.src2[0], r.src1[0], r.dst[0]);
				}      
				IV REFR(const Fw64fc *s1, const Fw64fc *s2, Fw64fc *d) const	// REFR Pixel function
				{         
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};
		}

		namespace C3
		{
			struct Div_8u	: public DivI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				XMM128 ones;
				Div_8u(int scaleFactor) : OPT_LEVEL::DivI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					DIV_SSE2::DIV::Init::C1::Div_8u_setup(scaleFactor, scale, ones);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_8u_unit(r.src2[0], r.src1[0], r.dst[0], scale, ones);
					DIV_SSE2::DIV::Unit::C1::Div_8u_unit(r.src2[1], r.src1[1], r.dst[1], scale, ones);
					DIV_SSE2::DIV::Unit::C1::Div_8u_unit(r.src2[2], r.src1[2], r.dst[2], scale, ones);
				}      
			};

			struct Div_16s	: public DivI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				XMM128 ones;
				Div_16s(int scaleFactor) : OPT_LEVEL::DivI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_16s_setup(scaleFactor, scale, ones);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16s_unit(r.src2[0], r.src1[0], r.dst[0], scale, ones);
					DIV_SSE2::DIV::Unit::C1::Div_16s_unit(r.src2[1], r.src1[1], r.dst[1], scale, ones);
					DIV_SSE2::DIV::Unit::C1::Div_16s_unit(r.src2[2], r.src1[2], r.dst[2], scale, ones);
				}      
			};

			struct Div_32s	: public DivI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C3 >
			{
				Div_32s(int scaleFactor) : OPT_LEVEL::DivI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32s_setup(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32s_unit(r.src2[0], r.src1[0], r.dst[0], scale);
					DIV_SSE2::DIV::Unit::C1::Div_32s_unit(r.src2[1], r.src1[1], r.dst[1], scale);
					DIV_SSE2::DIV::Unit::C1::Div_32s_unit(r.src2[2], r.src1[2], r.dst[2], scale);
				}      
			};

			struct Div_16sc	: public DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				XMM128 min32s;
				Div_16sc(int scaleFactor) : OPT_LEVEL::DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_16sc_setup(scaleFactor, scale, min32s);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src2[0], r.src1[0], r.dst[0], scale, min32s);
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src2[1], r.src1[1], r.dst[1], scale, min32s);
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src2[2], r.src1[2], r.dst[2], scale, min32s);
				}      
			};

			struct Div_32sc	: public DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				Div_32sc(int scaleFactor) : OPT_LEVEL::DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32sc_setup(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[0], r.src1[0], r.dst[0], scale);
					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[1], r.src1[1], r.dst[1], scale);
					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[2], r.src1[2], r.dst[2], scale);
				}      
			};

			struct Div_32f	: public fe3< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_div_ps( r.src2[0].f, r.src1[0].f );
					r.dst[1].f = _mm_div_ps( r.src2[1].f, r.src1[1].f );
					r.dst[2].f = _mm_div_ps( r.src2[2].f, r.src1[2].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};

			struct Div_32fc	: public fe3< Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3 >
			{
				XMM128 negbmask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::C1::Div_32fc_setup(negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[0], r.src1[0], r.dst[0], negbmask);
					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[1], r.src1[1], r.dst[1], negbmask);
					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[2], r.src1[2], r.dst[2], negbmask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}

		namespace AC4
		{
			struct Div_16sc	: public DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, min32s;
				Div_16sc(int scaleFactor) : OPT_LEVEL::DivI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::AC4::Div_16scsetup(scaleFactor, scale, mask, min32s);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src2[0], r.src1[0], temp, scale, min32s);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Div_32sc	: public DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Div_32sc(int scaleFactor) : OPT_LEVEL::DivI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::AC4::Div_32sc_setup(scaleFactor, scale, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[1], r.src1[1], temp, scale);
					DIV_SSE2::DIV::Unit::C1::Div_32sc_unit( r.src2[0], r.src1[0], r.dst[0], scale);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct Div_32f	: public fe3< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::AC4::Div_32f_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_div_ps( r.src2[0].f, r.src1[0].f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};

			struct Div_32fc	: public fe3< Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, negbmask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIV::Init::AC4::Div_32fc_setup(mask, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;

					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[1], r.src1[1], temp, negbmask);
					DIV_SSE2::DIV::Unit::C1::Div_32fc_unit( r.src2[0], r.src1[0], r.dst[0], negbmask);

					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};
		}

		namespace C4
		{
			struct Div_32f	: public fe3< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_div_ps( r.src2[0].f, r.src1[0].f );
					r.dst[1].f = _mm_div_ps( r.src2[1].f, r.src1[1].f );
					r.dst[2].f = _mm_div_ps( r.src2[2].f, r.src1[2].f );
					r.dst[3].f = _mm_div_ps( r.src2[3].f, r.src1[3].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					DIV_REF::DIV::DivF(s1, OPT_LEVEL::C4, s2, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

	namespace DIVC
	{
		namespace C1
		{
			struct DivC_8u	: public DivCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				DivC_8u(int scaleFactor, Fw8u valC) : OPT_LEVEL::DivCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					DIV_SSE2::DIVC::Init::C1::DivC_8u_setup(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C1::DivC_8u_unit(r.src1[0], val, r.dst[0]);
				}      
			};

			
			struct DivC_16s	: public DivCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				DivC_16s(int scaleFactor, Fw16s valC) : OPT_LEVEL::DivCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_16s_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C1::DivC_16s_unit(r.src1[0], val, r.dst[0]);
				}      
			};

			
			struct DivC_16sc	: public DivCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				XMM128 min32s, negbmask;
				DivC_16sc(int scaleFactor, Fw16sc valC) : OPT_LEVEL::DivCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_16sc_setup(scaleFactor, scaleMasks[0], valC, val, min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0], min32s);
				}      
			};

			struct DivC_32sc	: public DivCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				DivC_32sc(int scaleFactor, Fw32sc valC) : OPT_LEVEL::DivCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_32sc_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C1::DivC_32sc_unit( r.src1[0], val, scaleMasks[0], r.dst[0] );
				}
			};

			struct DivC_32f	: public DivCF_C1<Fw32f, Fw32f>
			{
				DivC_32f(Fw32f valC) : OPT_LEVEL::DivCF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, val.f );
				}      
			};

			struct DivCRev_16u	: public DivCRevF_C1<Fw16u, Fw16u>
			{
				DivCRev_16u(Fw16u valC) : OPT_LEVEL::DivCRevF_C1<Fw16u, Fw16u>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					FW_SSE2::Load1(val, (Fw32f)valC);		

				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C1::DivC_16u_unit( r.src1[0], val, r.dst[0]);
				}      
			};

			struct DivCRev_32f	: public DivCRevF_C1<Fw32f, Fw32f>
			{
				DivCRev_32f(Fw32f valC) : OPT_LEVEL::DivCRevF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_div_ps( r.src1[0].f, val.f );
				}      
			};

			struct DivC_64f	: public DivCF_C1<Fw64f, Fw64f>
			{
				DivC_64f(Fw64f valC) : OPT_LEVEL::DivCF_C1<Fw64f, Fw64f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_64f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_mul_pd( r.src1[0].d, val.d );
				}      
			};

			struct DivC_32fc	: public DivCF_C1<Fw32fc, Fw32fc>
			{
				DivC_32fc(Fw32fc valC) : OPT_LEVEL::DivCF_C1<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 valrev;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_32fc_setup(valC, val, valrev);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = CBL_SSE2::DivideC_32fc(r.src1[0].f, val.f, valrev.f);
				}      
			};

			struct DivC_64fc	: public DivCF_C1<Fw64fc, Fw64fc>
			{
				DivC_64fc(Fw64fc valC) : OPT_LEVEL::DivCF_C1<Fw64fc, Fw64fc>(valC) {}
				
				XMM128 valrev;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C1::DivC_64fc_setup(valC, val, valrev);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C1::DivC_64fc_unit(r.src1[0], val, valrev, r.dst[0]);
				}      
			};
		};


		namespace C3
		{
			
			struct DivC_8u	: public DivCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				DivC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::DivCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					DIV_SSE2::DIVC::Init::C3::DivC_8u_setup(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C3::DivC_8u_unit(r.src1[0], val0, val1, val2, val0, r.dst[0]);
					DIV_SSE2::DIVC::Unit::C3::DivC_8u_unit(r.src1[1], val1, val2, val0, val1, r.dst[1]);
					DIV_SSE2::DIVC::Unit::C3::DivC_8u_unit(r.src1[2], val2, val0, val1, val2, r.dst[2]);
				}      
			};

			
			struct DivC_16s	: public DivCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				DivC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::DivCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C3::DivC_16s_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIVC::Unit::C3::DivC_16s_unit(r.src1[0], val0, val1, r.dst[0]);
					DIV_SSE2::DIVC::Unit::C3::DivC_16s_unit(r.src1[1], val2, val0, r.dst[1]);
					DIV_SSE2::DIVC::Unit::C3::DivC_16s_unit(r.src1[2], val1, val2, r.dst[2]);
				}      
			};

			
			struct DivC_16sc	: public DivCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				XMM128 min32s, negbmask;
				DivC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::DivCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C3::DivC_16sc_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2, min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src1[0], val0, r.dst[0], scaleMasks[0], min32s);
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src1[1], val1, r.dst[1], scaleMasks[0], min32s);
					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit( r.src1[2], val2, r.dst[2], scaleMasks[0], min32s);
				}      
			};

			struct DivC_32sc	: public DivCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				DivC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::DivCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C3::DivC_32sc_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 vRev0, vRev1, vRev2;

					vRev0.d	= CBL_SSE2::Shuffle_b0a1_64f(val0.d, val0.d);
					vRev0.d	= _mm_xor_pd(vRev0.d, scaleMasks[0].d);

					vRev1.d	= CBL_SSE2::Shuffle_b0a1_64f(val1.d, val1.d);
					vRev1.d	= _mm_xor_pd(vRev1.d, scaleMasks[0].d);

					vRev2.d	= CBL_SSE2::Shuffle_b0a1_64f(val2.d, val2.d);
					vRev2.d	= _mm_xor_pd(vRev2.d, scaleMasks[0].d);

					DIV_SSE2::DIVC::Unit::C3::DivC_32sc_unit( r.src1[0], val0, vRev0, val1, vRev1, r.dst[0] );
					DIV_SSE2::DIVC::Unit::C3::DivC_32sc_unit( r.src1[1], val2, vRev2, val0, vRev0, r.dst[1] );
					DIV_SSE2::DIVC::Unit::C3::DivC_32sc_unit( r.src1[2], val1, vRev1, val2, vRev2, r.dst[2] );
				}      
			};

			struct DivC_32f	: public DivCF_C3<Fw32f, Fw32f>
			{
				DivC_32f(const Fw32f valC[]) : OPT_LEVEL::DivCF_C3<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C3::DivC_32f_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, val0.f );
					r.dst[1].f = _mm_mul_ps( r.src1[1].f, val1.f );
					r.dst[2].f = _mm_mul_ps( r.src1[2].f, val2.f );
				}      
			};

			struct DivC_32fc	: public DivCF_C3<Fw32fc, Fw32fc>
			{
				DivC_32fc(const Fw32fc valC[]) : OPT_LEVEL::DivCF_C3<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 val0rev, val1rev, val2rev;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::C3::DivC_32fc_setup(valC, val0, val1, val2, val0rev , val1rev , val2rev );
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f	= CBL_SSE2::DivideC_32fc(r.src1[0].f, val0.f, val0rev.f);
					r.dst[1].f	= CBL_SSE2::DivideC_32fc(r.src1[1].f, val1.f, val1rev.f);
					r.dst[2].f	= CBL_SSE2::DivideC_32fc(r.src1[2].f, val2.f, val2rev.f);
				}      
			};
		}



		namespace AC4
		{		
			struct DivC_8u	: public DivCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 ac4mask;
				DivC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::DivCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					DIV_SSE2::DIVC::Init::AC4::DivC_8u_setup(scaleFactor, scaleMasks, valC, val, ac4mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask;
					mask.i = ac4mask.i;
					__m128i tmpDst = _mm_and_si128(r.dst[0].i, mask.i) ;

					DIV_SSE2::DIVC::Unit::AC4::DivC_8u_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
					mask.i = _mm_cmpeq_epi8( mask.i, _mm_setzero_si128() );
					mask.i = _mm_and_si128(r.dst[0].i, mask.i);
					r.dst[0].i = _mm_or_si128(mask.i, tmpDst);
				}      
			};

			
			struct DivC_16s	: public DivCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 ac4mask;
				DivC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::DivCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::AC4::DivC_16s_setup(scaleFactor, scaleMasks[0], valC, val, ac4mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask;
					mask.i = ac4mask.i;
					__m128i tmpDst = _mm_and_si128(r.dst[0].i, mask.i) ;

					DIV_SSE2::DIVC::Unit::C3::DivC_16s_unit( r.src1[0], val, val, r.dst[0] );
					mask.i = _mm_cmpeq_epi16( mask.i, _mm_setzero_si128());
					mask.i = _mm_and_si128(r.dst[0].i, mask.i);
					r.dst[0].i = _mm_or_si128(mask.i, tmpDst);
				}
			};


			struct DivC_16sc	: public DivCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 ac4mask, min32s, negbmask;
				DivC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::DivCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::AC4::DivC_16sc_setup(scaleFactor, scaleMasks[0], valC, val, min32s, negbmask, ac4mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 mask;
					mask.i = ac4mask.i;
					__m128i tmpDst = _mm_and_si128(r.dst[0].i, mask.i) ;

					DIV_SSE2::DIV::Unit::C1::Div_16sc_unit(r.src1[0], val, r.dst[0], scaleMasks[0], min32s);

					mask.i = _mm_cmpeq_epi16( mask.i, _mm_setzero_si128());
					mask.i = _mm_and_si128(r.dst[0].i, mask.i);
					r.dst[0].i = _mm_or_si128(mask.i, tmpDst);
				}      
			};

			struct DivC_32sc	: public DivCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 ac4mask;
				XMM128 val1;
				XMM128 val2;
				XMM128 negamask;

				DivC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::DivCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					Common::AC4::Init::For64(ac4mask);
					DIV_SSE2::DIVC::Init::AC4::DivC_32sc_setup(scaleFactor, scaleMasks[0], valC, val, val1, val2, negamask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					XMM128 vRev0, vRev1, vRev2;


					vRev0.d	= CBL_SSE2::Shuffle_b0a1_64f(val.d, val.d);
					vRev0.d	= _mm_xor_pd(vRev0.d, negamask.d);

					vRev1.d	= CBL_SSE2::Shuffle_b0a1_64f(val1.d, val1.d);
					vRev1.d	= _mm_xor_pd(vRev1.d, negamask.d);

					vRev2.d	= CBL_SSE2::Shuffle_b0a1_64f(val2.d, val2.d);
					vRev2.d	= _mm_xor_pd(vRev2.d, negamask.d);

					DIV_SSE2::DIVC::Unit::C3::DivC_32sc_unit( r.src1[0], val, vRev0, val1, vRev1, r.dst[0] );
					DIV_SSE2::DIVC::Unit::C3::DivC_32sc_unit( r.src1[1], val2, vRev2, temp );

					Common::AC4::Apply::MaskI(r.dst[1], temp, ac4mask);
				}      
			};

			struct DivC_32f	: public DivCF_AC4<Fw32f, Fw32f>
			{
				DivC_32f(const Fw32f valC[]) : OPT_LEVEL::DivCF_AC4<Fw32f, Fw32f>(valC) {}
				
				XMM128 ac4mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::AC4::DivC_32f_setup(valC, val, ac4mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_mul_ps( r.src1[0].f, val.f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, ac4mask);
				}      
			};

			struct DivC_32fc	: public DivCF_AC4<Fw32fc, Fw32fc>
			{
				DivC_32fc(const Fw32fc valC[]) : OPT_LEVEL::DivCF_AC4<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 val1, ac4mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					DIV_SSE2::DIVC::Init::AC4::DivC_32fc_setup(valC, val, val1, ac4mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					XMM128 val0rev, val1rev;

					val0rev.f	= CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f(val.f, val.f);
					val1rev.f	= CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f(val1.f, val1.f);

					r.dst[0].f	= CBL_SSE2::DivideC_32fc(r.src1[0].f, val.f,  val0rev.f);
					temp.f	    = CBL_SSE2::DivideC_32fc(r.src1[1].f, val1.f, val1rev.f);

					Common::AC4::Apply::MaskF(r.dst[1], temp, ac4mask);
				}
			};
		}
	}
}

} // namespace OPT_LEVEL

#endif	// #ifdef __DIVDEF_H__

