/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SUBDEF_H__
#define __SUBDEF_H__

#include "SubImpl.h"
#include "fe.h"


namespace OPT_LEVEL
{

// Sub base structure
template<class TS1, class TS2, class TD, CH c>
struct SubI : public fe3< TS1, c, TS2, c, TD, c>   
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubI(int scaleFactor_)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUB::SubI(s1, c, s2, c, d, c, scaleFactor);
	} 
};




// SubC base structures
template<class TS, class TD, CH c>
struct SubCI_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubCI_C1(int scaleFactor_, TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCI_1(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct SubCI_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubCI_C3(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct SubCI_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubCI_AC4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct SubCI_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubCI_C4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCI(s, c, d, c, this->valC, scaleFactor);
	} 
};


template<class TS, class TD>
struct SubCF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	SubCF_C1(const TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct SubCF_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	SubCF_C3(const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3, this->valC);
	} 
};

template<class TS, class TD>
struct SubCF_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	SubCF_AC4(const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4, this->valC);
	} 
};

template<class TS, class TD>
struct SubCF_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	SubCF_C4(const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBC::SubCF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4, this->valC);
	} 
};




// SubCRev base structures
template<class TS, class TD, CH c>
struct SubCRevI_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SubCRevI_C1(int scaleFactor_, TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBCREV::SubCRevI_1(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD>
struct SubCRevF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	SubCRevF_C1(const TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SUB_REF::SUBCREV::SubCRevF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};


namespace DEF_SUB
{
	namespace SUB
	{
		namespace C1
		{
			template <int scaleType>
			struct Sub_8u	: public SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				Sub_8u(int scaleFactor) : OPT_LEVEL::SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUB::Init::C1::Sub_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Sub_16s	: public SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Sub_16s(int scaleFactor) : OPT_LEVEL::SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Sub_16sc	: public SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				Sub_16sc(int scaleFactor) : OPT_LEVEL::SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_16sc_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16sc_unit<scaleType>( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Sub_32s	: public SubI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				Sub_32s(int scaleFactor) : OPT_LEVEL::SubI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_32s_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_32s_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Sub_32sc	: public SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				Sub_32sc(int scaleFactor) : OPT_LEVEL::SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Sub_32f	: public fe3< Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src2[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

         struct Sub_64s	: public fe3< Fw64s, OPT_LEVEL::C1, Fw64s, OPT_LEVEL::C1, Fw64s, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].i = _mm_sub_epi64( r.src2[0].i, r.src1[0].i );
				}      
				IV REFR(const Fw64s *s1, const Fw64s *s2, Fw64s *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sub_64f	: public fe3< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_sub_pd( r.src2[0].d, r.src1[0].d );
				}      
				IV REFR(const Fw64f *s1, const Fw64f *s2, Fw64f *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};


			struct Sub_32fc	: public fe3< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[0], r.src2[0], r.dst[0]);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sub_64fc	: public fe3< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_64fc_unit( r.src1[0], r.src2[0], r.dst[0]);
				}      
				IV REFR(const Fw64fc *s1, const Fw64fc *s2, Fw64fc *d) const	// REFR Pixel function
				{         
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sub_16s32f	: public SubI< Fw16s, Fw16s, Fw32f, OPT_LEVEL::C1 >
			{
				Sub_16s32f(int scaleFactor) : OPT_LEVEL::SubI< Fw16s, Fw16s, Fw32f, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16s32f_unit(r.src1[0], r.src2[0], r.dst[0]);
				}      
			};
		}

		namespace C3
		{
			template <int scaleType>
			struct Sub_8u	: public SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				Sub_8u(int scaleFactor) : OPT_LEVEL::SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUB::Init::C1::Sub_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks);
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Sub_16s	: public SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				Sub_16s(int scaleFactor) : OPT_LEVEL::SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Sub_16sc	: public SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				Sub_16sc(int scaleFactor) : OPT_LEVEL::SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_16sc_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16sc_unit<scaleType>( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_16sc_unit<scaleType>( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_16sc_unit<scaleType>( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Sub_32sc	: public SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				Sub_32sc(int scaleFactor) : OPT_LEVEL::SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Sub_32f	: public fe3< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src2[0].f, r.src1[0].f );
					r.dst[1].f = _mm_sub_ps( r.src2[1].f, r.src1[1].f );
					r.dst[2].f = _mm_sub_ps( r.src2[2].f, r.src1[2].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};

			struct Sub_32fc	: public fe3< Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[0], r.src2[0], r.dst[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[1], r.src2[1], r.dst[1]);
					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[2], r.src2[2], r.dst[2]);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct Sub_8u	: public SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sub_8u(int scaleFactor) : OPT_LEVEL::SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUB::Init::AC4::Sub_8u_setup<scaleType>(scaleFactor, scaleMasks, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[0], r.src2[0], temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sub_16s	: public SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sub_16s(int scaleFactor) : OPT_LEVEL::SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::AC4::Sub_16s_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[0], r.src2[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sub_16sc	: public SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sub_16sc(int scaleFactor) : OPT_LEVEL::SubI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::AC4::Sub_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUB::Unit::C1::Sub_16sc_unit<scaleType>( r.src1[0], r.src2[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Sub_32sc	: public SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sub_32sc(int scaleFactor) : OPT_LEVEL::SubI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::AC4::Sub_32sc_setup(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[1], r.src2[1], temp, scaleMasks[0]);
					SUB_SSE2::SUB::Unit::C1::Sub_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct Sub_32f	: public fe3< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::AC4::Sub_32f_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_sub_ps( r.src2[0].f, r.src1[0].f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};

			struct Sub_32fc	: public fe3< Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::AC4::Sub_32fc_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;

					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[1], r.src2[1], temp);
					SUB_SSE2::SUB::Unit::C1::Sub_32fc_unit( r.src1[0], r.src2[0], r.dst[0]);

					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct Sub_8u	: public SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				Sub_8u(int scaleFactor) : OPT_LEVEL::SubI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUB::Init::C1::Sub_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Sub_16s	: public SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				Sub_16s(int scaleFactor) : OPT_LEVEL::SubI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUB::Init::C1::Sub_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUB::Unit::C1::Sub_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Sub_32f	: public fe3< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src2[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					SUB_REF::SUB::SubF(s1, OPT_LEVEL::C4, s2, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

	namespace SUBC
	{
		namespace C1
		{
			template <int scaleType>
			struct SubC_8u	: public SubCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				SubC_8u(int scaleFactor, Fw8u valC) : OPT_LEVEL::SubCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::C1::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct SubC_16s	: public SubCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				SubC_16s(int scaleFactor, Fw16s valC) : OPT_LEVEL::SubCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val, val);
				}      
			};

			template <int scaleType>
			struct SubC_16sc	: public SubCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				SubC_16sc(int scaleFactor, Fw16sc valC) : OPT_LEVEL::SubCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[0], val, val, r.dst[0], scaleMasks[0]);
				}      
			};

			struct SubC_32s	: public SubCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				SubC_32s(int scaleFactor, Fw32s valC) : OPT_LEVEL::SubCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					Fw32sc valC32sc = {valC, valC};
					SUB_SSE2::SUBC::Init::C1::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC32sc, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct SubC_32sc	: public SubCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				SubC_32sc(int scaleFactor, Fw32sc valC) : OPT_LEVEL::SubCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct SubC_32f	: public SubCF_C1<Fw32f, Fw32f>
			{
				SubC_32f(Fw32f valC) : OPT_LEVEL::SubCF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src1[0].f, val.f );
				}      
			};

			struct SubC_64f	: public SubCF_C1<Fw64f, Fw64f>
			{
				SubC_64f(Fw64f valC) : OPT_LEVEL::SubCF_C1<Fw64f, Fw64f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_64f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_sub_pd( r.src1[0].d, val.d );
				}      
			};

			struct SubC_32fc	: public SubCF_C1<Fw32fc, Fw32fc>
			{
				SubC_32fc(Fw32fc valC) : OPT_LEVEL::SubCF_C1<Fw32fc, Fw32fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};

			struct SubC_64fc	: public SubCF_C1<Fw64fc, Fw64fc>
			{
				SubC_64fc(Fw64fc valC) : OPT_LEVEL::SubCF_C1<Fw64fc, Fw64fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_64fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_64fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};
		};


		namespace C3
		{
			template <int scaleType>
			struct SubC_8u	: public SubCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				SubC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::SubCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::C3::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[0], val0, val1, r.dst[0], scaleMasks);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[1], val2, val0, r.dst[1], scaleMasks);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[2], val1, val2, r.dst[2], scaleMasks);
				}      
			};
			template <int scaleType>
			struct SubC_8u_scale_zero	: public SubCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				SubC_8u_scale_zero(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::SubCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::C3::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_scale_zero_unit(r.src1[0], val0, r.dst[0]);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_scale_zero_unit(r.src1[1], val1, r.dst[1]);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_scale_zero_unit(r.src1[2], val2, r.dst[2]);
				}      
			};

			template <int scaleType>
			struct SubC_16s	: public SubCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				SubC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::SubCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C3::SubC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
                    switch (scaleType)
                    {
                    case 1:
                    case -1:
					    SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val0, val1);
					    SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0], val2, val0);
					    SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0], val1, val2);
                    break;
                    default:
                        r.dst[0].i = _mm_subs_epi16(r.src1[0].i, val0.i);
                        r.dst[1].i = _mm_subs_epi16(r.src1[1].i, val1.i);
                        r.dst[2].i = _mm_subs_epi16(r.src1[2].i, val2.i);
                    }
				}      
			};

			template <int scaleType>
			struct SubC_16sc	: public SubCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				SubC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::SubCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C3::SubC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[0], val0, val1, r.dst[0], scaleMasks[0]);
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[1], val2, val0, r.dst[1], scaleMasks[0]);
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[2], val1, val2, r.dst[2], scaleMasks[0]);
						break;
					default:
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[0], val0, val0, r.dst[0], scaleMasks[0]);
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[1], val1, val1, r.dst[1], scaleMasks[0]);
						SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[2], val2, val2, r.dst[2], scaleMasks[0]);
					}
				}      
			};

			struct SubC_32sc	: public SubCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				SubC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::SubCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C3::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[0], val0, r.dst[0], scaleMasks[0]);
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[1], val1, r.dst[1], scaleMasks[0]);
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[2], val2, r.dst[2], scaleMasks[0]);
				}      
			};

			struct SubC_32f	: public SubCF_C3<Fw32f, Fw32f>
			{
				SubC_32f(const Fw32f valC[]) : OPT_LEVEL::SubCF_C3<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C3::SubC_32f_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src1[0].f, val0.f );
					r.dst[1].f = _mm_sub_ps( r.src1[1].f, val1.f );
					r.dst[2].f = _mm_sub_ps( r.src1[2].f, val2.f );
				}      
			};

			struct SubC_32fc	: public SubCF_C3<Fw32fc, Fw32fc>
			{
				SubC_32fc(const Fw32fc valC[]) : OPT_LEVEL::SubCF_C3<Fw32fc, Fw32fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C3::SubC_32fc_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[0], val0, r.dst[0]);
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[1], val1, r.dst[1]);
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[2], val2, r.dst[2]);
				}      
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct SubC_8u	: public SubCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				SubC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::SubCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[0], val, val, temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct SubC_16s	: public SubCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				SubC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::SubCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[0], temp, scaleMasks[0], val, val);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct SubC_16sc	: public SubCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, val1;
				SubC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::SubCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUBC::Unit::C1::SubC_16sc_unit<scaleType>( r.src1[0], val, val1, temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct SubC_32sc	: public SubCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				XMM128 val1;
				SubC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::SubCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[1], val1, temp,     scaleMasks[0]);
					SUB_SSE2::SUBC::Unit::C1::SubC_32sc_unit( r.src1[0], val,  r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct SubC_32f	: public SubCF_AC4<Fw32f, Fw32f>
			{
				SubC_32f(const Fw32f valC[]) : OPT_LEVEL::SubCF_AC4<Fw32f, Fw32f>(valC) {}
				
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_32f_setup(valC, val, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_sub_ps( r.src1[0].f, val.f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
			};

			struct SubC_32fc	: public SubCF_AC4<Fw32fc, Fw32fc>
			{
				SubC_32fc(const Fw32fc valC[]) : OPT_LEVEL::SubCF_AC4<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 val1, mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::AC4::SubC_32fc_setup(valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[1], val1, temp		);
					SUB_SSE2::SUBC::Unit::C1::SubC_32fc_unit( r.src1[0], val,  r.dst[0]	);
					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct SubC_8u	: public SubCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				SubC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::SubCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::C4::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[1], val, val, r.dst[1], scaleMasks);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[2], val, val, r.dst[2], scaleMasks);
					SUB_SSE2::SUBC::Unit::C1::SubC_8u_unit<scaleType>(r.src1[3], val, val, r.dst[3], scaleMasks);
				}      
			};

			template <int scaleType>
			struct SubC_16s	: public SubCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				SubC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::SubCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C4::SubC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val, val);
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0], val, val);
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0], val, val);
					SUB_SSE2::SUBC::Unit::C1::SubC_16s_unit<scaleType>(r.src1[3], r.dst[3], scaleMasks[0], val, val);
				}      
			};

			struct SubC_32f	: public SubCF_C4<Fw32f, Fw32f>
			{
				SubC_32f(const Fw32f valC[]) : OPT_LEVEL::SubCF_C4<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C4::SubC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( r.src1[0].f, val.f );
					r.dst[1].f = _mm_sub_ps( r.src1[1].f, val.f );
					r.dst[2].f = _mm_sub_ps( r.src1[2].f, val.f );
					r.dst[3].f = _mm_sub_ps( r.src1[3].f, val.f );
				} 
			};
		}
	}

	namespace SUBCREV
	{
		namespace C1
		{
			template <int scaleType>
			struct SubCRev_8u	: public SubCRevI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				SubCRev_8u(int scaleFactor, Fw8u valC) : OPT_LEVEL::SubCRevI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SUB_SSE2::SUBC::Init::C1::SubC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct SubCRev_16s	: public SubCRevI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				SubCRev_16s(int scaleFactor, Fw16s valC) : OPT_LEVEL::SubCRevI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val, val);
				}      
			};

			template <int scaleType>
			struct SubCRev_16sc	: public SubCRevI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				SubCRev_16sc(int scaleFactor, Fw16sc valC) : OPT_LEVEL::SubCRevI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_16sc_unit<scaleType>( r.src1[0], val, val, r.dst[0], scaleMasks[0]);
				}      
			};

			struct SubCRev_32s	: public SubCRevI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				SubCRev_32s(int scaleFactor, Fw32s valC) : OPT_LEVEL::SubCRevI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					Fw32sc valC32sc = {valC, valC};
					SUB_SSE2::SUBC::Init::C1::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC32sc, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct SubCRev_32sc	: public SubCRevI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				SubCRev_32sc(int scaleFactor, Fw32sc valC) : OPT_LEVEL::SubCRevI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32sc_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct SubCRev_32f	: public SubCRevF_C1<Fw32f, Fw32f>
			{
				SubCRev_32f(Fw32f valC) : OPT_LEVEL::SubCRevF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sub_ps( val.f, r.src1[0].f );
				}      
			};

			struct SubCRev_64f	: public SubCRevF_C1<Fw64f, Fw64f>
			{
				SubCRev_64f(Fw64f valC) : OPT_LEVEL::SubCRevF_C1<Fw64f, Fw64f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_64f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_sub_pd( val.d, r.src1[0].d );
				}      
			};

			struct SubCRev_32fc	: public SubCRevF_C1<Fw32fc, Fw32fc>
			{
				SubCRev_32fc(Fw32fc valC) : OPT_LEVEL::SubCRevF_C1<Fw32fc, Fw32fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_32fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_32fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};

			struct SubCRev_64fc	: public SubCRevF_C1<Fw64fc, Fw64fc>
			{
				SubCRev_64fc(Fw64fc valC) : OPT_LEVEL::SubCRevF_C1<Fw64fc, Fw64fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SUB_SSE2::SUBC::Init::C1::SubC_64fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SUB_SSE2::SUBCREV::Unit::C1::SubCRev_64fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};
		};
	}
}

} // namespace OPT_LEVEL

#endif	// #ifdef __SUBDEF_H__

