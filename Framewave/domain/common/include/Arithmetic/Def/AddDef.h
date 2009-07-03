/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __ADDDEF_H__
#define __ADDDEF_H__

#include "AddImpl.h"
#include "fe.h"


namespace OPT_LEVEL
{

template<class TS1, class TS2, class TD, CH c>
struct AddI : public fe3< TS1, c, TS2, c, TD, c>   
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddI(int scaleFactor)
	{
		this->scaleFactor = scaleFactor;
	}

	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADD::AddI(s1, c, s2, c, d, c, scaleFactor);
	} 
};

template<class TS1, class TS2, class TD, CH c>
struct AddProduct : public fe3< TS1, c, TS2, c, TD, c>   
{
	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddProduct(s1, c, s2, c, d, c);
	} 
};

template<class TS1, class TS2, class TD, CH c>
struct AddProduct_m : public fe4< TS1, c, TS2, c, const Fw8u, c, TD, c>
{
	IV REFR(const TS1 *s1, const TS2 *s2, const Fw8u *m, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddProduct_m(s1, c, s2, c, m, c, d, c);
	} 
};

template<class TS1, class TS2, class TD, CH c>
struct AddProductI : public fe3< TS1, c, TS2, c, TD, c>   
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddProductI(int scaleFactor)
	{
		this->scaleFactor = scaleFactor;
	}

	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddProductI(s1, c, s2, c, d, c, scaleFactor);
	} 
};


template<class TS, class TD, CH c>
struct AddSquare : public fe2< TS, c, TD, c>   
{
	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddSquare(s, c, d, c);
	} 
};

template<class TS, class TD, CH c>
struct AddSquare_m : public fe3< TS, c, const Fw8u, c, TD, c>   
{
	IV REFR(const TS *s, const Fw8u *m, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddSquare_m(s, c, m, c, d, c);
	} 
};


template<class TS, class TD, CH c>
struct AddWeighted : public fe2< TS, c, TD, c>   
{
	Fw32f alpha;
	XMM128 xmmAlpha;

	AddWeighted(Fw32f alpha)
	{
		this->alpha = alpha;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddWeighted(s, c, d, c, alpha);
	} 
};

template<class TS, class TD, CH c>
struct AddWeighted_m : public fe3< TS, c, const Fw8u, c, TD, c>   
{
	Fw32f alpha;
	XMM128 xmmAlpha;

	AddWeighted_m(Fw32f alpha)
	{
		this->alpha = alpha;
	}

	IV REFR(const TS *s, const Fw8u *m, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDX::AddWeighted_m(s, c, m, c, d, c, alpha);
	} 
};




template<class TS, class TD, CH c>
struct AddCI_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddCI_C1(int scaleFactor_, TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCI_1(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct AddCI_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddCI_C3(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct AddCI_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddCI_AC4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct AddCI_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	AddCI_C4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCI(s, c, d, c, this->valC, scaleFactor);
	} 
};


template<class TS, class TD>
struct AddCF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	AddCF_C1(const TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct AddCF_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	AddCF_C3(const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3, this->valC);
	} 
};

template<class TS, class TD>
struct AddCF_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	AddCF_AC4(const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4, this->valC);
	} 
};

template<class TS, class TD>
struct AddCF_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	AddCF_C4(const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		ADD_REF::ADDC::AddCF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4, this->valC);
	} 
};


namespace DEF_ADD
{
	namespace ADD
	{
		namespace C1
		{
			template <int scaleType>
			struct Add_8u	: public AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				Add_8u(int scaleFactor) : OPT_LEVEL::AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADD::Init::C1::Add_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Add_16s	: public AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Add_16s(int scaleFactor) : OPT_LEVEL::AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Add_16u	: public AddI< Fw16u, Fw16u, Fw16u, OPT_LEVEL::C1 >
			{
				
				Add_16u(int scaleFactor) : OPT_LEVEL::AddI< Fw16u, Fw16u, Fw16u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].i = _mm_adds_epu16(r.src1[0].i, r.src2[0].i);
				}      
			};

			template <int scaleType>
			struct Add_16sc	: public AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				Add_16sc(int scaleFactor) : OPT_LEVEL::AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_16sc_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16sc_unit<scaleType>( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Add_32s	: public AddI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				Add_32s(int scaleFactor) : OPT_LEVEL::AddI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_32s_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_32s_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Add_32u	: public fe3< Fw32u, OPT_LEVEL::C1, Fw32u, OPT_LEVEL::C1, Fw32u, OPT_LEVEL::C1 >
			{
				IV REFR(const Fw32u *s1, const Fw32u *s2, Fw32u *d) const		// REFR Pixel function
				{
					ADD_REF::ADD::AddINS(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				} 

				FE_REF
			};

			struct Add_32sc	: public AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				Add_32sc(int scaleFactor) : OPT_LEVEL::AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Add_32f	: public fe3< Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src2[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

         struct Add_64s	: public fe3< Fw64s, OPT_LEVEL::C1, Fw64s, OPT_LEVEL::C1, Fw64s, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				
            // SSE2 Pixel function
				IV SSE2( RegFile & r ) const								
				{
					r.dst[0].i = _mm_add_epi64(r.src2[0].i, r.src1[0].i); 
				}

            // REFR Pixel Funtion
				IV REFR(const Fw64s *s1, const Fw64s *s2, Fw64s *d) const
				{
					ADD_REF::ADD::AddINS(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}

			};


			struct Add_64f	: public fe3< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_add_pd( r.src2[0].d, r.src1[0].d );
				}      
				IV REFR(const Fw64f *s1, const Fw64f *s2, Fw64f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_32fc	: public fe3< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{

				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src2[0], r.src1[0], r.dst[0]);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_64fc	: public fe3< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_64fc_unit( r.src2[0], r.src1[0], r.dst[0]);
				}      
				IV REFR(const Fw64fc *s1, const Fw64fc *s2, Fw64fc *d) const	// REFR Pixel function
				{         
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_8u32f	: public fe2< Fw8u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,temp.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp.i);
					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw8u *s, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_8s32f	: public fe2< Fw8s, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					CBL_SSE2::Unpack8STo16S(r.src1[0].i,temp.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,temp.i);
					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw8s *s, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_16u32f	: public fe2< Fw16u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,temp.i);
					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw16u *s, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_16s32s	: public fe2< Fw16s, OPT_LEVEL::C1, Fw32s, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 srcTemp1_1,srcTemp2_1;
					XMM128 srcTemp1_2, srcTemp2_2;

					__m128i signext = _mm_srai_epi16(r.src1[0].i, 15);
					r.src1[0].i = _mm_unpacklo_epi16( r.src1[0].i, signext );

					srcTemp1_1.d = _mm_cvtepi32_pd(r.src1[0].i);
					srcTemp2_1.d = _mm_cvtepi32_pd(r.dst[0].i);

					srcTemp1_2.i = _mm_srli_si128(r.src1[0].i, 8);
					srcTemp2_2.i = _mm_srli_si128(r.dst[0].i, 8);

					srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
					srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

					r.dst[0].d = _mm_add_pd(srcTemp1_1.d, srcTemp2_1.d);
					srcTemp1_2.d = _mm_add_pd(srcTemp1_2.d, srcTemp2_2.d);

					r.dst[0].i = CBL_SSE2::Pack64FTo32S(r.dst[0].d, srcTemp1_2.d);
				}      
				IV REFR(const Fw16s *s, Fw32s *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddI(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct Add_8u16u	: public AddI< Fw8u, Fw8u, Fw16u, OPT_LEVEL::C1 >
			{
				Add_8u16u(int scaleFactor) : OPT_LEVEL::AddI< Fw8u, Fw8u, Fw16u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_8u16u_unit(r.src1[0], r.src2[0], r.dst[0]);
				}      
			};

			struct Add_16s32f	: public AddI< Fw16s, Fw16s, Fw32f, OPT_LEVEL::C1 >
			{
				Add_16s32f(int scaleFactor) : OPT_LEVEL::AddI< Fw16s, Fw16s, Fw32f, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16s32f_unit(r.src1[0], r.src2[0], r.dst[0]);
				}      
			};
		}

		namespace C1M
		{
			struct Add_8u32f	: public fe3< Fw8u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					XMM128 mask1, mask2;

					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
				IV REFR(const Fw8u *s, const Fw8u *m, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF_m(s, OPT_LEVEL::C1, m, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_8s32f	: public fe3< Fw8s, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					XMM128 mask1, mask2;

					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,src_hi.i);

					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
				IV REFR(const Fw8s *s, const Fw8u *m, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF_m(s, OPT_LEVEL::C1, m, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_16u32f	: public fe3< Fw16u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2;
					XMM128 src_hi;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_cvtepi32_ps (r.src1[0].i);
					r.dst[0].f  = ::_mm_add_ps( r.dst[0].f, r.src1[0].f );
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
				IV REFR(const Fw16u *s, const Fw8u *m, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF_m(s, OPT_LEVEL::C1, m, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Add_32f	: public fe3< Fw32f, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2,src1_hi;

					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src1_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
				IV REFR(const Fw32f *s, const Fw8u *m, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF_m(s, OPT_LEVEL::C1, m, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};
		}


		namespace C3
		{
			template <int scaleType>
			struct Add_8u	: public AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				Add_8u(int scaleFactor) : OPT_LEVEL::AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADD::Init::C1::Add_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks);
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Add_16s	: public AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				Add_16s(int scaleFactor) : OPT_LEVEL::AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Add_16sc	: public AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				Add_16sc(int scaleFactor) : OPT_LEVEL::AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_16sc_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16sc_unit<scaleType>( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_16sc_unit<scaleType>( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_16sc_unit<scaleType>( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Add_32sc	: public AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				Add_32sc(int scaleFactor) : OPT_LEVEL::AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Add_32f	: public fe3< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src1[0].f, r.src2[0].f );
					r.dst[1].f = _mm_add_ps( r.src1[1].f, r.src2[1].f );
					r.dst[2].f = _mm_add_ps( r.src1[2].f, r.src2[2].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};

			struct Add_32fc	: public fe3< Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src1[0], r.src2[0], r.dst[0]);
					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src1[1], r.src2[1], r.dst[1]);
					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src1[2], r.src2[2], r.dst[2]);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct Add_8u	: public AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Add_8u(int scaleFactor) : OPT_LEVEL::AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADD::Init::AC4::Add_8u_setup<scaleType>(scaleFactor, scaleMasks, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src2[0], r.src1[0], temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Add_16s	: public AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Add_16s(int scaleFactor) : OPT_LEVEL::AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::AC4::Add_16s_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src2[0], r.src1[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Add_16sc	: public AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Add_16sc(int scaleFactor) : OPT_LEVEL::AddI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::AC4::Add_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADD::Unit::C1::Add_16sc_unit<scaleType>( r.src1[0], r.src2[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Add_32sc	: public AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Add_32sc(int scaleFactor) : OPT_LEVEL::AddI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::AC4::Add_32sc_setup(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src2[1], r.src1[1], temp, scaleMasks[0]);
					ADD_SSE2::ADD::Unit::C1::Add_32sc_unit( r.src2[0], r.src1[0], r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct Add_32f	: public fe3< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::AC4::Add_32f_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_add_ps( r.src1[0].f, r.src2[0].f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};

			struct Add_32fc	: public fe3< Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::AC4::Add_32fc_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;

					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src2[1], r.src1[1], temp);
					ADD_SSE2::ADD::Unit::C1::Add_32fc_unit( r.src2[0], r.src1[0], r.dst[0]);

					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct Add_8u	: public AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				Add_8u(int scaleFactor) : OPT_LEVEL::AddI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADD::Init::C1::Add_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct Add_16s	: public AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				Add_16s(int scaleFactor) : OPT_LEVEL::AddI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADD::Init::C1::Add_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADD::Unit::C1::Add_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Add_32f	: public fe3< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src1[0].f, r.src2[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					ADD_REF::ADD::AddF(s1, OPT_LEVEL::C4, s2, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

	namespace ADDC
	{
		namespace C1
		{
			template <int scaleType>
			struct AddC_8u	: public AddCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				AddC_8u(int scaleFactor, Fw8u valC) : OPT_LEVEL::AddCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADDC::Init::C1::AddC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct AddC_16s	: public AddCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				AddC_16s(int scaleFactor, Fw16s valC) : OPT_LEVEL::AddCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val, val);
				}      
			};

			template <int scaleType>
			struct AddC_16sc	: public AddCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				AddC_16sc(int scaleFactor, Fw16sc valC) : OPT_LEVEL::AddCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[0], val, val, r.dst[0], scaleMasks[0]);
				}      
			};

			struct AddC_32s	: public AddCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				AddC_32s(int scaleFactor, Fw32s valC) : OPT_LEVEL::AddCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					Fw32sc valC32sc = {valC, valC};
					ADD_SSE2::ADDC::Init::C1::AddC_32sc_setup(scaleFactor, scaleMasks[0], valC32sc, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct AddC_32sc	: public AddCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				AddC_32sc(int scaleFactor, Fw32sc valC) : OPT_LEVEL::AddCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_32sc_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct AddC_32f	: public AddCF_C1<Fw32f, Fw32f>
			{
				AddC_32f(Fw32f valC) : OPT_LEVEL::AddCF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src1[0].f, val.f );
				}      
			};

			struct AddC_64f	: public AddCF_C1<Fw64f, Fw64f>
			{
				AddC_64f(Fw64f valC) : OPT_LEVEL::AddCF_C1<Fw64f, Fw64f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_64f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_add_pd( r.src1[0].d, val.d );
				}      
			};

			struct AddC_32fc	: public AddCF_C1<Fw32fc, Fw32fc>
			{
				AddC_32fc(Fw32fc valC) : OPT_LEVEL::AddCF_C1<Fw32fc, Fw32fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_32fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};

			struct AddC_64fc	: public AddCF_C1<Fw64fc, Fw64fc>
			{
				AddC_64fc(Fw64fc valC) : OPT_LEVEL::AddCF_C1<Fw64fc, Fw64fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C1::AddC_64fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_64fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};
		};


		namespace C3
		{
			template <int scaleType>
			struct AddC_8u	: public AddCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				AddC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::AddCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADDC::Init::C3::AddC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[0], val0, val1, r.dst[0], scaleMasks);
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[1], val2, val0, r.dst[1], scaleMasks);
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[2], val1, val2, r.dst[2], scaleMasks);
				}      
			};

   			template <int scaleType>
			struct AddC_8u_scale_zero : public AddCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				AddC_8u_scale_zero(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::AddCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADDC::Init::C3::AddC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_scale_zero_unit(r.src1[0], val0, r.dst[0]);
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_scale_zero_unit(r.src1[1], val1, r.dst[1]);
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_scale_zero_unit(r.src1[2], val2, r.dst[2]);
				}      
			};

			template <int scaleType>
			struct AddC_16s	: public AddCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				AddC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::AddCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C3::AddC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
                    switch (scaleType)
                    {
                    case 1:
                    case -1:
					    ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val0, val1);
					    ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0], val2, val0);
					    ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0], val1, val2);
                    break;
                    default:
                        r.dst[0].i = _mm_adds_epi16(r.src1[0].i, val0.i);
                        r.dst[1].i = _mm_adds_epi16(r.src1[1].i, val1.i);
                        r.dst[2].i = _mm_adds_epi16(r.src1[2].i, val2.i);
                    }

				}      
			};

			template <int scaleType>
			struct AddC_16sc	: public AddCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				AddC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::AddCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C3::AddC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					switch (scaleType)
					{
					case 1:
					case -1:
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[0], val0, val1, r.dst[0], scaleMasks[0]);
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[1], val2, val0, r.dst[1], scaleMasks[0]);
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[2], val1, val2, r.dst[2], scaleMasks[0]);
						break;
					default:
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[0], val0, val0, r.dst[0], scaleMasks[0]);
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[1], val1, val1, r.dst[1], scaleMasks[0]);
						ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[2], val2, val2, r.dst[2], scaleMasks[0]);
					}
				}      
			};

			struct AddC_32sc	: public AddCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				AddC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::AddCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C3::AddC_32sc_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[0], val0, r.dst[0], scaleMasks[0]);
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[1], val1, r.dst[1], scaleMasks[0]);
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[2], val2, r.dst[2], scaleMasks[0]);
				}      
			};

			struct AddC_32f	: public AddCF_C3<Fw32f, Fw32f>
			{
				AddC_32f(const Fw32f valC[]) : OPT_LEVEL::AddCF_C3<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C3::AddC_32f_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src1[0].f, val0.f );
					r.dst[1].f = _mm_add_ps( r.src1[1].f, val1.f );
					r.dst[2].f = _mm_add_ps( r.src1[2].f, val2.f );
				}      
			};

			struct AddC_32fc	: public AddCF_C3<Fw32fc, Fw32fc>
			{
				AddC_32fc(const Fw32fc valC[]) : OPT_LEVEL::AddCF_C3<Fw32fc, Fw32fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C3::AddC_32fc_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[0], val0, r.dst[0]);
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[1], val1, r.dst[1]);
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[2], val2, r.dst[2]);
				}      
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct AddC_8u	: public AddCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				AddC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::AddCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[0], val, val, temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct AddC_16s	: public AddCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				AddC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::AddCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[0], temp, scaleMasks[0], val, val);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct AddC_16sc	: public AddCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, val1;
				AddC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::AddCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADDC::Unit::C1::AddC_16sc_unit<scaleType>( r.src1[0], val, val1, temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct AddC_32sc	: public AddCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				XMM128 val1;
				AddC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::AddCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_32sc_setup(scaleFactor, scaleMasks[0], valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[1], val1, temp,     scaleMasks[0]);
					ADD_SSE2::ADDC::Unit::C1::AddC_32sc_unit( r.src1[0], val,  r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct AddC_32f	: public AddCF_AC4<Fw32f, Fw32f>
			{
				AddC_32f(const Fw32f valC[]) : OPT_LEVEL::AddCF_AC4<Fw32f, Fw32f>(valC) {}
				
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_32f_setup(valC, val, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_add_ps( r.src1[0].f, val.f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
			};

			struct AddC_32fc	: public AddCF_AC4<Fw32fc, Fw32fc>
			{
				AddC_32fc(const Fw32fc valC[]) : OPT_LEVEL::AddCF_AC4<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 val1, mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::AC4::AddC_32fc_setup(valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[1], val1, temp		);
					ADD_SSE2::ADDC::Unit::C1::AddC_32fc_unit( r.src1[0], val,  r.dst[0]	);
					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct AddC_8u	: public AddCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				AddC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::AddCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					ADD_SSE2::ADDC::Init::C4::AddC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct AddC_16s	: public AddCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				AddC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::AddCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C4::AddC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					ADD_SSE2::ADDC::Unit::C1::AddC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], val, val);
				}      
			};

			struct AddC_32f	: public AddCF_C4<Fw32f, Fw32f>
			{
				AddC_32f(const Fw32f valC[]) : OPT_LEVEL::AddCF_C4<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					ADD_SSE2::ADDC::Init::C4::AddC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_add_ps( r.src1[0].f, val.f );
				} 
			};
		}
	}
	
	namespace ADDX
	{
		namespace C1
		{
			struct AddProduct_8u32f	: public AddProduct< Fw8u, Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi,src_lo;

					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src1_hi.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src2[0].i);
					src1_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src2[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src1_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
				}      
			};

			struct AddProduct_8s32f	: public AddProduct< Fw8s, Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi,src_lo;

					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack8STo16S(r.src2[0].i,src1_hi.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src2[0].i);
					src1_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src2[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src1_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
				}
			};

			struct AddProduct_16u32f	: public AddProduct< Fw16u, Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi,src2_hi;

					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src2_hi.i);

					r.src1[0].f =	_mm_cvtepi32_ps(r.src1[0].i);
					r.src2[0].f =	_mm_cvtepi32_ps(r.src2[0].i);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,r.src2[0].f);
					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);
				}      
			};

			struct AddProduct_32f	: public AddProduct< Fw32f, Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.src1[0].f	= _mm_mul_ps(r.src1[0].f, r.src2[0].f);
					r.dst[0].f	= _mm_add_ps(r.dst[0].f,  r.src1[0].f);
				}      
			};

			struct AddProduct_64f	: public AddProduct< Fw64f, Fw64f, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.src1[0].d	= _mm_mul_pd(r.src1[0].d, r.src2[0].d);
					r.dst[0].d	= _mm_add_pd(r.dst[0].d,  r.src1[0].d);
				}      
			};

			struct AddProduct_32fc	: public AddProduct< Fw32fc, Fw32fc, Fw32fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					__m128 src1 = r.src1[0].f;
					__m128 src2 = r.src2[0].f;

					__m128 prod = CBL_SSE2::Multiply_32fc(src1, src2);
					r.dst[0].f = _mm_add_ps(prod, r.dst[0].f);
				}      
			};

			struct AddProduct_64fc	: public AddProduct< Fw64fc, Fw64fc, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					__m128d src1 = r.src1[0].d;
					__m128d src2 = r.src2[0].d;

					__m128d prod = CBL_SSE2::Multiply_64fc(src1, src2);
					r.dst[0].d = _mm_add_pd(prod, r.dst[0].d);
				}      
			};

			struct AddProduct_16s	: public AddProductI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				AddProduct_16s(int scaleFactor) : AddProductI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_16s_setup(scaleFactor, scaleMasks[0]);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp_lo,temp_hi;

					temp_lo.i = _mm_mullo_epi16(r.src1[0].i,r.src2[0].i);	
					temp_hi.i = _mm_mulhi_epi16(r.src1[0].i,r.src2[0].i);	

					r.src1[0].i = _mm_unpacklo_epi16(temp_lo.i, temp_hi.i);
					r.src2[0].i = _mm_unpackhi_epi16(temp_lo.i, temp_hi.i);

					r.src1[0].f  = _mm_cvtepi32_ps  ( r.src1[0].i);
					r.src2[0].f  = _mm_cvtepi32_ps  ( r.src2[0].i);

					CBL_SSE2::Unpack16STo32S(r.dst[0].i,temp_hi.i);

					r.dst[0].f  = _mm_cvtepi32_ps  ( r.dst[0].i);
					temp_hi.f  = _mm_cvtepi32_ps  ( temp_hi.i);

					r.src1[0].f = _mm_add_ps( r.src1[0].f,r.dst[0].f);
					r.src2[0].f = _mm_add_ps(  r.src2[0].f,temp_hi.f);

					r.src1[0].f = _mm_mul_ps( r.src1[0].f , scaleMasks[0].f);
					r.src2[0].f = _mm_mul_ps( r.src2[0].f , scaleMasks[0].f);


					temp_hi.i = _mm_srli_si128(r.src1[0].i, 8); // Added this code bcause cvt ps to epi32 was not working properly at max 32S +ive values
					r.src1[0].d  = _mm_cvtps_pd(r.src1[0].f);			// So goto 64 bit and then use cvt pd to epi32
					temp_hi.d  = _mm_cvtps_pd(temp_hi.f);
					r.src1[0].i = CBL_SSE2::Pack64FTo32S(r.src1[0].d, temp_hi.d);

					temp_hi.i = _mm_srli_si128(r.src2[0].i, 8); 
					r.src2[0].d  = _mm_cvtps_pd(r.src2[0].f);
					temp_hi.d  = _mm_cvtps_pd(temp_hi.f);
					r.src2[0].i = CBL_SSE2::Pack64FTo32S(r.src2[0].d, temp_hi.d);

					r.dst[0].i = _mm_packs_epi32(r.src1[0].i,r.src2[0].i);
				}
			};

			struct AddProduct_32s	: public AddProductI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				AddProduct_32s(int scaleFactor) : AddProductI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32s_setup(scaleFactor, scaleMasks[0]);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 srcTemp1_1,srcTemp2_1;
					XMM128 srcTemp1_2, srcTemp2_2;

					srcTemp1_1.d = _mm_cvtepi32_pd(r.src1[0].i);
					srcTemp2_1.d = _mm_cvtepi32_pd(r.src2[0].i);

					srcTemp1_2.i = _mm_srli_si128(r.src1[0].i, 8);
					srcTemp2_2.i = _mm_srli_si128(r.src2[0].i, 8);

					srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
					srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

					r.src1[0].d = _mm_mul_pd(srcTemp1_1.d, srcTemp2_1.d);
					r.src2[0].d = _mm_mul_pd(srcTemp1_2.d, srcTemp2_2.d);


					srcTemp1_1.d = _mm_cvtepi32_pd(r.dst[0].i);
					srcTemp1_2.i = _mm_srli_si128(r.dst[0].i, 8);
					srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);

					r.src1[0].d = _mm_add_pd(r.src1[0].d, srcTemp1_1.d);
					r.src2[0].d = _mm_add_pd(r.src2[0].d , srcTemp1_2.d);


					r.dst[0].d = _mm_mul_pd(r.src1[0].d, scaleMasks[0].d);
					srcTemp1_2.d = _mm_mul_pd(r.src2[0].d, scaleMasks[0].d);

					r.dst[0].i = CBL_SSE2::Pack64FTo32S(r.dst[0].d, srcTemp1_2.d);
				}
			};

			struct AddProduct_16s32s	: public AddProductI< Fw16s, Fw16s, Fw32s, OPT_LEVEL::C1 >
			{
				AddProduct_16s32s(int scaleFactor) : AddProductI< Fw16s, Fw16s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32s_setup(scaleFactor, scaleMasks[0]);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp_lo,temp_hi;

					temp_lo.i = _mm_mullo_epi16(r.src1[0].i,r.src2[0].i);	
					temp_hi.i = _mm_mulhi_epi16(r.src1[0].i,r.src2[0].i);	

					r.src1[0].i = _mm_unpacklo_epi16(temp_lo.i, temp_hi.i);

					temp_lo.d = _mm_cvtepi32_pd(r.src1[0].i);
					r.src1[0].i = _mm_srli_si128(r.src1[0].i, 8);
					r.src1[0].d = _mm_cvtepi32_pd(r.src1[0].i);


					temp_hi.d = _mm_cvtepi32_pd(r.dst[0].i);
					r.dst[0].i = _mm_srli_si128(r.dst[0].i, 8);
					r.dst[0].d = _mm_cvtepi32_pd(r.dst[0].i);

					r.src1[0].d = _mm_add_pd(r.src1[0].d ,r.dst[0].d); // 
					r.dst[0].d= _mm_add_pd(temp_lo.d ,temp_hi.d);

					r.src1[0].d = _mm_mul_pd(r.src1[0].d ,scaleMasks[0].d); // 
					r.dst[0].d= _mm_mul_pd(r.dst[0].d ,scaleMasks[0].d);

					r.dst[0].i = CBL_SSE2::Pack64FTo32S (r.dst[0].d, r.src1[0].d);
				}
			};

			struct AddSquare_8u32f	: public AddSquare< Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi,src_lo;

					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src_hi.i);
					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src1[0].i);
					src_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src1[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
				}
			};

			struct AddSquare_8s32f	: public AddSquare< Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi,src_lo;

					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src_hi.i);
					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src1[0].i);
					src_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src1[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
				}
			};

			struct AddSquare_16u32f	: public AddSquare< Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					r.src1[0].f =	_mm_cvtepi32_ps  (r.src1[0].i);
					r.src1[0].f = _mm_mul_ps (r.src1[0].f,r.src1[0].f);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,r.src1[0].f);
				}
			};

			struct AddSquare_32f	: public AddSquare< Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,r.src1[0].f);
					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);
				}
			};


			struct AddWeighted_8u32f	: public AddWeighted< Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_8u32f(Fw32f alpha) : AddWeighted< Fw8u, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);
					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
				}
			};

			struct AddWeighted_8s32f	: public AddWeighted< Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_8s32f(Fw32f alpha) : AddWeighted< Fw8s, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);
					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
				}
			};

			struct AddWeighted_16u32f	: public AddWeighted< Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_16u32f(Fw32f alpha) : AddWeighted< Fw16u, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);
					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
				}
			};

			struct AddWeighted_32f	: public AddWeighted< Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_32f(Fw32f alpha) : AddWeighted< Fw32f, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
				}
			};



		}

		namespace C1M
		{
			struct AddProduct_8u32f	: public AddProduct_m< Fw8u, Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi,src_lo;
					XMM128 mask1, mask2;


					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src3[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src3[0].i,src1_hi.i);

					ADD_SSE2::ADDX::Setup_mask(mask1,mask2,r.src3[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src2[0].i);
					src1_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src2[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src1_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
			};

			struct AddProduct_8s32f	: public AddProduct_m< Fw8s, Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi,src_lo;
					XMM128 mask1, mask2;
					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack8STo16S(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src3[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src3[0].i,src1_hi.i);

					ADD_SSE2::ADDX::Setup_mask(mask1,mask2,r.src3[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src2[0].i);
					src1_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src2[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src1_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddProduct_16u32f	: public AddProduct_m< Fw16u, Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src1_hi;
					XMM128 mask1, mask2;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src3[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src3[0].i,src1_hi.i);

					ADD_SSE2::ADDX::Setup_mask(mask1,mask2,r.src3[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f =	_mm_cvtepi32_ps(r.src1[0].i);
					r.src2[0].f =	_mm_cvtepi32_ps(r.src2[0].i);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,r.src2[0].f);
					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
			};

			struct AddProduct_32f	: public AddProduct_m< Fw32f, Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2,src1_hi;

					CBL_SSE2::Unpack8UTo16U(r.src3[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src3[0].i,src1_hi.i);
					ADD_SSE2::ADDX::Setup_mask(mask1,mask2,r.src3[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_mul_ps(r.src1[0].f,r.src2[0].f);
					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}      
			};


			struct AddSquare_8u32f	: public AddSquare_m< Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi,src_lo;
					XMM128 mask1, mask2;

					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src1[0].i);
					src_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src1[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddSquare_8s32f	: public AddSquare_m< Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi,src_lo;
					XMM128 mask1, mask2;

					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					src_lo.i = _mm_mullo_epi16 (r.src1[0].i,r.src1[0].i);
					src_hi.i = _mm_mulhi_epi16 (r.src1[0].i,r.src1[0].i);
					src_lo.i  =	_mm_unpacklo_epi16(src_lo.i,src_hi.i ); // lower 4

					src_lo.f = 	_mm_cvtepi32_ps  (src_lo.i);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,src_lo.f);

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddSquare_16u32f	: public AddSquare_m< Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2;
					XMM128 src_hi;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f =	_mm_cvtepi32_ps  (r.src1[0].i);
					r.src1[0].f = _mm_mul_ps (r.src1[0].f,r.src1[0].f);
					r.dst[0].f =	_mm_add_ps  (r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddSquare_32f	: public AddSquare_m< Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2,src1_hi;

					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src1_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_mul_ps(r.src1[0].f,r.src1[0].f);
					r.dst[0].f =	_mm_add_ps(r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};


			struct AddWeighted_8u32f	: public AddWeighted_m< Fw8u, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_8u32f(Fw32f alpha) : AddWeighted_m< Fw8u, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					XMM128 mask1, mask2;
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);

					CBL_SSE2::Unpack8UTo16U(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddWeighted_8s32f	: public AddWeighted_m< Fw8s, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_8s32f(Fw32f alpha) : AddWeighted_m< Fw8s, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 src_hi;
					XMM128 mask1, mask2;
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);


					CBL_SSE2::Unpack8STo16S(r.src1[0].i,src_hi.i);
					CBL_SSE2::Unpack16STo32S(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);

					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddWeighted_16u32f	: public AddWeighted_m< Fw16u, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_16u32f(Fw32f alpha) : AddWeighted_m< Fw16u, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2;
					XMM128 src_hi;
					CBL_SSE2::Unpack16UTo32U(r.src1[0].i,src_hi.i);
					r.src1[0].f = _mm_cvtepi32_ps(r.src1[0].i);
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src_hi.i);
					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

			struct AddWeighted_32f	: public AddWeighted_m< Fw32f, Fw32f, OPT_LEVEL::C1 >
			{
				AddWeighted_32f(Fw32f alpha) : AddWeighted_m< Fw32f, Fw32f, OPT_LEVEL::C1 >(alpha) {}
				FE_SSE2_REF

				IV SSE2_Init() 
				{
					ADD_SSE2::ADDX::Init::AddX_32f_setup(alpha, xmmAlpha);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 mask1, mask2,src1_hi;
					CBL_SSE2::Unpack8UTo16U(r.src2[0].i,src1_hi.i);
					CBL_SSE2::Unpack16UTo32U(r.src2[0].i,src1_hi.i);

					ADD_SSE2::ADD::Setup_mask(mask1,mask2,r.src2[0]);
					mask1.i = _mm_and_si128(r.dst[0].i,mask1.i);

					r.src1[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					r.src1[0].f = _mm_mul_ps(r.src1[0].f,xmmAlpha.f);
					r.dst[0].f = _mm_sub_ps(r.dst[0].f,r.src1[0].f);
					mask2.i = _mm_and_si128(r.dst[0].i,mask2.i);
					r.dst[0].i = _mm_or_si128(mask1.i,mask2.i);
				}
			};

		}
	}
}

} // namespace OPT_LEVEL

#endif	// #ifdef __ADDIDEF_H__

