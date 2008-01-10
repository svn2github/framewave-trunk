/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __MULDEF_H__
#define __MULDEF_H__

#include "MulImpl.h"
#include "fe.h"


namespace OPT_LEVEL
{

template<class TS1, class TS2, class TD, CH c>
struct MulI : public fe3< TS1, c, TS2, c, TD, c>   
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	MulI(int scaleFactor_)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MUL::MulI(s1, c, s2, c, d, c, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct MulCI_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	MulCI_C1(int scaleFactor_, TS valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCI_1(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct MulCI_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	MulCI_C3(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct MulCI_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	MulCI_AC4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCI(s, c, d, c, this->valC, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct MulCI_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	MulCI_C4(int scaleFactor_, const TD valC[]) : OPT_LEVEL::Common::InitConst::C4::ValC<TS, TD>(valC)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCI(s, c, d, c, this->valC, scaleFactor);
	} 
};


template<class TS, class TD>
struct MulCF_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	MulCF_C1(const TS valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCF_1(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct MulCF_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	MulCF_C3(const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3, this->valC);
	} 
};

template<class TS, class TD>
struct MulCF_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	MulCF_AC4(const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4, this->valC);
	} 
};

template<class TS, class TD>
struct MulCF_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	MulCF_C4(const TD valC[]) : Common::InitConst::C4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULC::MulCF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4, this->valC);
	} 
};

// MulScale

template<class TS1, class TS2, class TD, CH c>
struct MulScale : fe3 <TS1, c, TS2, c, TD, c>
{
	XMM128 xmmMax;
	IV REFR(const TS1 *s1, const TS2 *s2, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULXSCALE::MulScale(s1, c, s2, c, d, c);
	} 
};


// MulCScale

template<class TS, class TD>
struct MulCScale_C1 : Common::InitConst::C1::ValC<TS, TD>
{
	MulCScale_C1(const TD valC) : OPT_LEVEL::Common::InitConst::C1::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULXSCALE::MulCScale_1<const TS, TD>(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1, this->valC);
	} 
};

template<class TS, class TD>
struct MulCScale_C3 : Common::InitConst::C3::ValC<TS, TD>
{
	MulCScale_C3(const TD valC[]) : OPT_LEVEL::Common::InitConst::C3::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULXSCALE::MulCScale<const TS, TD>(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3, this->valC);
	} 
};

template<class TS, class TD>
struct MulCScale_AC4 : Common::InitConst::AC4::ValC<TS, TD>
{
	MulCScale_AC4(const TD valC[]) : OPT_LEVEL::Common::InitConst::AC4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULXSCALE::MulCScale(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4, this->valC);
	} 
};

template<class TS, class TD>
struct MulCScale_C4 : Common::InitConst::C4::ValC<TS, TD>
{
	XMM128 xmmMax;
	MulCScale_C4(const TD valC[]) : Common::InitConst::C4::ValC<TS, TD>(valC) {}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		MUL_REF::MULXSCALE::MulCScale(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4, this->valC);
	} 
};


namespace DEF_MUL
{
	namespace MUL
	{
		namespace C1
		{
			template <int scaleType>
			struct Mul_8u	: public MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				Mul_8u(int scaleFactor) : OPT_LEVEL::MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MUL::Init::C1::Mul_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src2[0], r.src1[0], r.dst[0], scaleMasks);
				}      
			};

            template<class TSD>
            struct Mul_8u_PosScale : fe3<TSD, OPT_LEVEL::C1, TSD, OPT_LEVEL::C1, TSD, OPT_LEVEL::C1>
                {
                XMM128 scaleMasks[4];
                int scale;

                    FE_CUSTOM_SSE2_REF
                    IV SSE2_Init()
	                {
		                MUL_SSE2::MUL::Init::C1::Mul_8u_setup<1>(scale, scaleMasks);
	                }
                    Mul_8u_PosScale(int scaleFactor)
                    {  
                        scale = scaleFactor;
                    }

                    template<IsAlign ia>
                    IV Custom1D_sse2(const TSD *s1, const TSD *s2, TSD *d, U32 pixCount) const 								
                    {			
                        Mul_8u_unit_PosScale_Custom<TSD, OPT_LEVEL::C1, ia>(s1, s2, d, pixCount, scale, scaleMasks);
                    }				
                    IV REFR(const TSD *s1, const TSD *s2, TSD *d) const		// REFR Pixel function
	                {
		                MUL_REF::MUL::MulI(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1, scale);
	                } 
                };


			template <int scaleType>
			struct Mul_16s	: public MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Mul_16s(int scaleFactor) : OPT_LEVEL::MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src2[0], r.src1[0], r.dst[0], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Mul_16sc	: public MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				XMM128 min32s, negbmask;
				Mul_16sc(int scaleFactor) : OPT_LEVEL::MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16sc_unit<scaleType>( r.src2[0], r.src1[0], r.dst[0], scaleMasks[0], min32s, negbmask);
				}      
			};

			struct Mul_32sc	: public MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				Mul_32sc(int scaleFactor) : OPT_LEVEL::MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Mul_32s	: public MulI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				Mul_32s(int scaleFactor) : OPT_LEVEL::MulI< Fw32s, Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					// since only the scale is initialized, we can use the 32sc setup
					MUL_SSE2::MUL::Init::C1::Mul_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32s_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};


			// Odd cases
			struct Mul_16u16s	: public MulI< Fw16u, Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				Mul_16u16s(int scaleFactor) : OPT_LEVEL::MulI< Fw16u, Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					// since only the scale is initialized, we can use the 16sc setup
					MUL_SSE2::MUL::Init::C1::Mul_16u16s_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16u16s_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};


			struct Mul_16s32s	: public MulI< Fw16s, Fw16s, Fw32s, OPT_LEVEL::C1 >
			{
				Mul_16s32s(int scaleFactor) : OPT_LEVEL::MulI< Fw16s, Fw16s, Fw32s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					// since only the scale is initialized, we can use the 32sc setup
					MUL_SSE2::MUL::Init::C1::Mul_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s32s_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Mul_32s32sc	: public MulI< Fw32s, Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				Mul_32s32sc(int scaleFactor) : OPT_LEVEL::MulI< Fw32s, Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					// since only the scale is initialized, we can use the 32scc setup
					MUL_SSE2::MUL::Init::C1::Mul_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32s32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}
			};

			struct Mul_8u16u	: public fe3< Fw8u, OPT_LEVEL::C1, Fw8u, OPT_LEVEL::C1, Fw16u, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 

				IV REFR(const Fw8u *s1, const Fw8u *s2, Fw16u *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  

				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_8u16u_unit( r.src1[0], r.src2[0], r.dst[0]);
				}
			};

			struct Mul_16s32f	: public fe3< Fw16s, OPT_LEVEL::C1, Fw16s, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				IV REFR(const Fw16s *s1, const Fw16s *s2, Fw32f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s32f_unit( r.src1[0], r.src2[0], r.dst[0]);
				}
			};



			struct Mul_32f	: public fe3<Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src2[0].f, r.src1[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Mul_64f	: public fe3< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_mul_pd( r.src1[0].d, r.src2[0].d );
				}      
				IV REFR(const Fw64f *s1, const Fw64f *s2, Fw64f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Mul_32fc	: public fe3< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				XMM128 negamask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_32fc_setup(negamask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src2[0], r.src1[0], r.dst[0], negamask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}
			};

			struct Mul_64fc	: public fe3< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_64fc_unit( r.src1[0], r.src2[0], r.dst[0]);
				}      
				IV REFR(const Fw64fc *s1, const Fw64fc *s2, Fw64fc *d) const	// REFR Pixel function
				{         
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C1, s2, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};
		};


		namespace C3
		{
			template <int scaleType>
			struct Mul_8u	: public MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				Mul_8u(int scaleFactor) : OPT_LEVEL::MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MUL::Init::C1::Mul_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks);
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks);
				}      
			};

            template<class TSD>
            struct Mul_8u_PosScale : fe3<TSD, OPT_LEVEL::C3, TSD, OPT_LEVEL::C3, TSD, OPT_LEVEL::C3>
                {
                XMM128 scaleMasks[4];
                int scale;

                    FE_CUSTOM_SSE2_REF
                    IV SSE2_Init()
	                {
		                MUL_SSE2::MUL::Init::C1::Mul_8u_setup<1>(scale, scaleMasks);
	                }
                    Mul_8u_PosScale(int scaleFactor)
                    {  
                        scale = scaleFactor;
                    }

                    template<IsAlign ia>
                    IV Custom1D_sse2(const TSD *s1, const TSD *s2, TSD *d, U32 pixCount) const 								
                    {			
                        Mul_8u_unit_PosScale_Custom<TSD, OPT_LEVEL::C3, ia>(s1, s2, d, pixCount, scale, scaleMasks);
                    }				
                    IV REFR(const TSD *s1, const TSD *s2, TSD *d) const		// REFR Pixel function
	                {
		                MUL_REF::MUL::MulI(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3, scale);
	                } 
                };

			template <int scaleType>
			struct Mul_16s	: public MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				Mul_16s(int scaleFactor) : OPT_LEVEL::MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Mul_16sc	: public MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				XMM128 min32s, negbmask;
				Mul_16sc(int scaleFactor) : OPT_LEVEL::MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16sc_unit<scaleType>( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0], min32s, negbmask);
					MUL_SSE2::MUL::Unit::C1::Mul_16sc_unit<scaleType>( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0], min32s, negbmask);
					MUL_SSE2::MUL::Unit::C1::Mul_16sc_unit<scaleType>( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0], min32s, negbmask);
				}      
			};

			struct Mul_32sc	: public MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				Mul_32sc(int scaleFactor) : OPT_LEVEL::MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_32sc_setup(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src1[1], r.src2[1], r.dst[1], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src1[2], r.src2[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Mul_32f	: public fe3< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, r.src2[0].f );
					r.dst[1].f = _mm_mul_ps( r.src1[1].f, r.src2[1].f );
					r.dst[2].f = _mm_mul_ps( r.src1[2].f, r.src2[2].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};

			struct Mul_32fc	: public fe3< Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3, Fw32fc, OPT_LEVEL::C3 >
			{
				XMM128 negamask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_32fc_setup(negamask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src1[0], r.src2[0], r.dst[0], negamask);
					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src1[1], r.src2[1], r.dst[1], negamask);
					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src1[2], r.src2[2], r.dst[2], negamask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{         
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C3, s2, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct Mul_8u	: public MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Mul_8u(int scaleFactor) : OPT_LEVEL::MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MUL::Init::AC4::Mul_8u_setup<scaleType>(scaleFactor, scaleMasks, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src2[0], r.src1[0], temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Mul_16s	: public MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Mul_16s(int scaleFactor) : OPT_LEVEL::MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::AC4::Mul_16s_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src2[0], r.src1[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Mul_16sc	: public MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, min32s, negbmask;
				Mul_16sc(int scaleFactor) : OPT_LEVEL::MulI< Fw16sc, Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::AC4::Mul_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], min32s, negbmask, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MUL::Unit::C1::Mul_16sc_unit<scaleType>( r.src1[0], r.src2[0], temp, scaleMasks[0], min32s, negbmask);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Mul_32sc	: public MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Mul_32sc(int scaleFactor) : OPT_LEVEL::MulI< Fw32sc, Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::AC4::Mul_32sc_setup(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src2[1], r.src1[1], temp, scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_32sc_unit( r.src2[0], r.src1[0], r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct Mul_32f	: public fe3< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::AC4::Mul_32f_setup(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_mul_ps( r.src1[0].f, r.src2[0].f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};

			struct Mul_32fc	: public fe3< Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4, Fw32fc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, negamask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::AC4::Mul_32fc_setup(negamask, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;

					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src2[1], r.src1[1], temp, negamask);
					MUL_SSE2::MUL::Unit::C1::Mul_32fc_unit( r.src2[0], r.src1[0], r.dst[0], negamask);

					Common::AC4::Apply::MaskF(r.dst[1], temp, mask);
				}      
				IV REFR(const Fw32fc *s1, const Fw32fc *s2, Fw32fc *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::AC4, s2, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct Mul_8u	: public MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				Mul_8u(int scaleFactor) : OPT_LEVEL::MulI< Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MUL::Init::C1::Mul_8u_setup<scaleType>(scaleFactor, scaleMasks);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_8u_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks);
				}      
			};
            template<class TSD>
            struct Mul_8u_PosScale : fe3<TSD, OPT_LEVEL::C4, TSD, OPT_LEVEL::C4, TSD, OPT_LEVEL::C4>
                {
                XMM128 scaleMasks[4];
                int scale;

                    FE_CUSTOM_SSE2_REF
                    IV SSE2_Init()
	                {
		                MUL_SSE2::MUL::Init::C1::Mul_8u_setup<1>(scale, scaleMasks);
	                }
                    Mul_8u_PosScale(int scaleFactor)
                    {  
                        scale = scaleFactor;
                    }

                    template<IsAlign ia>
                    IV Custom1D_sse2(const TSD *s1, const TSD *s2, TSD *d, U32 pixCount) const 								
                    {			
                        Mul_8u_unit_PosScale_Custom<TSD, OPT_LEVEL::C4, ia>(s1, s2, d, pixCount, scale, scaleMasks);
                    }				
                    IV REFR(const TSD *s1, const TSD *s2, TSD *d) const		// REFR Pixel function
	                {
		                MUL_REF::MUL::MulI(s1, OPT_LEVEL::C4, s2, OPT_LEVEL::C4, d, OPT_LEVEL::C4, scale);
	                } 
                };

			template <int scaleType>
			struct Mul_16s	: public MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				Mul_16s(int scaleFactor) : OPT_LEVEL::MulI< Fw16s, Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MUL::Init::C1::Mul_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[0], r.src2[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct Mul_32f	: public fe3< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, r.src2[0].f );
				}      
				IV REFR(const Fw32f *s1, const Fw32f *s2, Fw32f *d) const	// REFR Pixel function
				{
					MUL_REF::MUL::MulF(s1, OPT_LEVEL::C4, s2, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

	namespace MULC
	{
		namespace C1
		{
			template <int scaleType>
			struct MulC_8u	: public MulCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				MulC_8u(int scaleFactor, Fw8u valC) : OPT_LEVEL::MulCI_C1< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MULC::Init::C1::MulC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct MulC_16s	: public MulCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				MulC_16s(int scaleFactor, Fw16s valC) : OPT_LEVEL::MulCI_C1< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct MulC_16sc	: public MulCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				XMM128 min32s, negbmask;
				MulC_16sc(int scaleFactor, Fw16sc valC) : OPT_LEVEL::MulCI_C1< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_16sc_unit<scaleType>( r.src1[0], val, r.dst[0], scaleMasks[0], min32s, negbmask);
				}      
			};

			struct MulC_32s	: public MulCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >
			{
				MulC_32s(int scaleFactor, Fw32s valC) : OPT_LEVEL::MulCI_C1< Fw32s, Fw32s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32s_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_32s_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};


			struct MulC_32f16s : public MulCI_C1< Fw32f, Fw16s, OPT_LEVEL::C1 >
			{
				MulC_32f16s(int scaleFactor, Fw32f valC) : OPT_LEVEL::MulCI_C1< Fw32f, Fw16s, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32f16s_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = ::_mm_mul_ps( r.src1[0].f, val.f);
					r.dst[0].f = ::_mm_mul_ps( r.dst[0].f,  scaleMasks[0].f);
					MUL_SSE2::MULC::Pack32fto16S(r.dst[0], r.dst[0]);
				}
			};

			struct MulC_32sc	: public MulCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >
			{
				MulC_32sc(int scaleFactor, Fw32sc valC) : OPT_LEVEL::MulCI_C1< Fw32sc, Fw32sc, OPT_LEVEL::C1 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32sc_setup(scaleFactor, scaleMasks[0], valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[0], val, r.dst[0], scaleMasks[0]);
				}
			};

			struct MulC_32f	: public MulCF_C1<Fw32f, Fw32f>
			{
				MulC_32f(Fw32f valC) : OPT_LEVEL::MulCF_C1<Fw32f, Fw32f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, val.f );
				}      
			};

			struct MulC_Low_32f16s	: public MulCF_C1<Fw32f, Fw16s>
			{
				MulC_Low_32f16s(Fw32f valC) : OPT_LEVEL::MulCF_C1<Fw32f, Fw16s>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = ::_mm_mul_ps( r.src1[0].f, val.f);
					MUL_SSE2::MULC::Pack32fto16S(r.dst[0],r.dst[0]);
				}      
			};

			struct MulC_64f	: public MulCF_C1<Fw64f, Fw64f>
			{
				MulC_64f(Fw64f valC) : OPT_LEVEL::MulCF_C1<Fw64f, Fw64f>(valC) {}
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_64f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_mul_pd( r.src1[0].d, val.d );
				}      
			};

			struct MulC_32fc	: public MulCF_C1<Fw32fc, Fw32fc>
			{
				MulC_32fc(Fw32fc valC) : OPT_LEVEL::MulCF_C1<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 negamask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_32fc_setup(valC, val, negamask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[0], val, r.dst[0], negamask);
				}      
			};

			struct MulC_64fc	: public MulCF_C1<Fw64fc, Fw64fc>
			{
				MulC_64fc(Fw64fc valC) : OPT_LEVEL::MulCF_C1<Fw64fc, Fw64fc>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C1::MulC_64fc_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_64fc_unit( r.src1[0], val, r.dst[0]);
				}      
			};
		};


		namespace C3
		{
			template <int scaleType>
			struct MulC_8u	: public MulCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				MulC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::MulCI_C3< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MULC::Init::C3::MulC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val0, val1, val2);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[0], val0, val1, r.dst[0], scaleMasks);
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[1], val2, val0, r.dst[1], scaleMasks);
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[2], val1, val2, r.dst[2], scaleMasks);
				}      
			};

			template <int scaleType>
			struct MulC_16s	: public MulCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				MulC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::MulCI_C3< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C3::MulC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[0], val0, r.dst[0], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[1], val1, r.dst[1], scaleMasks[0]);
					MUL_SSE2::MUL::Unit::C1::Mul_16s_unit<scaleType>(r.src1[2], val2, r.dst[2], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct MulC_16sc	: public MulCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >
			{
				XMM128 min32s, negbmask;
				MulC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::MulCI_C3< Fw16sc, Fw16sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C3::MulC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val0, val1, val2, min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_16sc_unit<scaleType>( r.src1[0], val0, r.dst[0], scaleMasks[0], min32s, negbmask);
					MUL_SSE2::MULC::Unit::C1::MulC_16sc_unit<scaleType>( r.src1[1], val1, r.dst[1], scaleMasks[0], min32s, negbmask);
					MUL_SSE2::MULC::Unit::C1::MulC_16sc_unit<scaleType>( r.src1[2], val2, r.dst[2], scaleMasks[0], min32s, negbmask);
				}      
			};

			struct MulC_32sc	: public MulCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >
			{
				MulC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::MulCI_C3< Fw32sc, Fw32sc, OPT_LEVEL::C3 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C3::MulC_32sc_setup(scaleFactor, scaleMasks[0], valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[0], val0, r.dst[0], scaleMasks[0]);
					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[1], val1, r.dst[1], scaleMasks[0]);
					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[2], val2, r.dst[2], scaleMasks[0]);
				}      
			};

			struct MulC_32f	: public MulCF_C3<Fw32f, Fw32f>
			{
				MulC_32f(const Fw32f valC[]) : OPT_LEVEL::MulCF_C3<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C3::MulC_32f_setup(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, val0.f );
					r.dst[1].f = _mm_mul_ps( r.src1[1].f, val1.f );
					r.dst[2].f = _mm_mul_ps( r.src1[2].f, val2.f );
				}      
			};

			struct MulC_32fc	: public MulCF_C3<Fw32fc, Fw32fc>
			{
				MulC_32fc(const Fw32fc valC[]) : OPT_LEVEL::MulCF_C3<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 negamask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C3::MulC_32fc_setup(valC, val0, val1, val2, negamask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[0], val0, r.dst[0], negamask);
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[1], val1, r.dst[1], negamask);
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[2], val2, r.dst[2], negamask);
				}      
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct MulC_8u	: public MulCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				MulC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::MulCI_AC4< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MULC::Init::AC4::MulC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[0], val, val, temp, scaleMasks);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct MulC_16s	: public MulCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				MulC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::MulCI_AC4< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::AC4::MulC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULC::Unit::C1::MulC_16s_unit<scaleType>(r.src1[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct MulC_16sc	: public MulCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask, min32s, negbmask;
				MulC_16sc(int scaleFactor, const Fw16sc valC[]) : OPT_LEVEL::MulCI_AC4< Fw16sc, Fw16sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::AC4::MulC_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], valC, val, min32s, negbmask, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULC::Unit::C1::MulC_16sc_unit<scaleType>( r.src1[0], val, temp, scaleMasks[0], min32s, negbmask);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct MulC_32sc	: public MulCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				XMM128 val1;
				MulC_32sc(int scaleFactor, const Fw32sc valC[]) : OPT_LEVEL::MulCI_AC4< Fw32sc, Fw32sc, OPT_LEVEL::AC4 >(scaleFactor, valC) {};

				FE_SSE2_REF
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::AC4::MulC_32sc_setup(scaleFactor, scaleMasks[0], valC, val, val1, mask);
				}
				IV SSE2( RegFile & r ) const                        // SSE2 Pixel function
				{
					XMM128 temp;

					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[1], val1, temp,     scaleMasks[0]);
					MUL_SSE2::MULC::Unit::C1::MulC_32sc_unit( r.src1[0], val,  r.dst[0], scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[1], temp, mask);
				}      
			};

			struct MulC_32f	: public MulCF_AC4<Fw32f, Fw32f>
			{
				MulC_32f(const Fw32f valC[]) : OPT_LEVEL::MulCF_AC4<Fw32f, Fw32f>(valC) {}
				
				XMM128 mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::AC4::MulC_32f_setup(valC, val, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_mul_ps( r.src1[0].f, val.f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
			};

			struct MulC_32fc	: public MulCF_AC4<Fw32fc, Fw32fc>
			{
				MulC_32fc(const Fw32fc valC[]) : OPT_LEVEL::MulCF_AC4<Fw32fc, Fw32fc>(valC) {}
				
				XMM128 negamask, val1, mask;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::AC4::MulC_32fc_setup(valC, val, val1, negamask, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[1], val1, temp,	 negamask);
					MUL_SSE2::MULC::Unit::C1::MulC_32fc_unit( r.src1[0], val,  r.dst[0], negamask);
					r.dst[1].f = _mm_or_ps(_mm_andnot_ps(mask.f, r.dst[1].f), temp.f);
				}      
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct MulC_8u	: public MulCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				MulC_8u(int scaleFactor, const Fw8u valC[]) : OPT_LEVEL::MulCI_C4< Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					MUL_SSE2::MULC::Init::C4::MulC_8u_setup<scaleType>(scaleFactor, scaleMasks, valC, val);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_8u_unit<scaleType>(r.src1[0], val, val, r.dst[0], scaleMasks);
				}      
			};

			template <int scaleType>
			struct MulC_16s	: public MulCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				MulC_16s(int scaleFactor, const Fw16s valC[]) : OPT_LEVEL::MulCI_C4< Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor, valC) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C4::MulC_16s_setup<scaleType>(scaleFactor, scaleMasks[0], valC);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULC::Unit::C1::MulC_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
				}      
			};

			struct MulC_32f	: public MulCF_C4<Fw32f, Fw32f>
			{
				MulC_32f(const Fw32f valC[]) : OPT_LEVEL::MulCF_C4<Fw32f, Fw32f>(valC) {}
				
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULC::Init::C4::MulC_32f_setup(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps( r.src1[0].f, val.f );
				} 
			};
		}
	}

	namespace MULXSCALE
	{
		namespace C1
		{
			struct MulScale_8u : public MulScale<Fw8u, Fw8u, Fw8u, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};

			struct MulScale_16u : public MulScale<Fw16u, Fw16u, Fw16u, OPT_LEVEL::C1>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};

			struct MulCScale_8u : public MulCScale_C1<Fw8u, Fw8u>
			{
				MulCScale_8u(const Fw8u valC) : OPT_LEVEL::MulCScale_C1<Fw8u, Fw8u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C1::MulCScale_8u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],val, r.dst[0]);
				}      
			};

			struct MulCScale_16u : public MulCScale_C1<Fw16u, Fw16u>
			{
				MulCScale_16u(const Fw16u valC) : OPT_LEVEL::MulCScale_C1<Fw16u, Fw16u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C1::MulCScale_16u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Mul_16u(r.src1[0], val, r.dst[0]);
				}      
			};
		}
		namespace C3
		{
			struct MulScale_8u : public MulScale<Fw8u, Fw8u, Fw8u, OPT_LEVEL::C3>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[2],r.src2[2], r.dst[2]);
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[1],r.src2[1], r.dst[1]);
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};

			struct MulScale_16u : public MulScale<Fw16u, Fw16u, Fw16u, OPT_LEVEL::C3>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[2],r.src2[2], r.dst[2]);
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[1],r.src2[1], r.dst[1]);
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};

			struct MulCScale_8u : public MulCScale_C3<Fw8u, Fw8u>
			{
				MulCScale_8u(const Fw8u valC[]) : OPT_LEVEL::MulCScale_C3<Fw8u, Fw8u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C3::MulCScale_8u(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],val0, r.dst[0]);
                    MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[1],val1, r.dst[1]);
                    MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[2],val2, r.dst[2]);
				}      
			};

			struct MulCScale_16u : public MulCScale_C3<Fw16u, Fw16u>
			{
				MulCScale_16u(const Fw16u valC[]) : OPT_LEVEL::MulCScale_C3<Fw16u, Fw16u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C3::MulCScale_16u(valC, val0, val1, val2);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Mul_16u_C3(r.src1[0],	val0, val1, r.dst[0]);
					MUL_SSE2::MULXSCALE::MulScale_Mul_16u_C3(r.src1[1],	val2, val0, r.dst[1]);
					MUL_SSE2::MULXSCALE::MulScale_Mul_16u_C3(r.src1[2],	val1, val2, r.dst[2]);
				}      
			};

		}
		namespace AC4
		{
			struct MulScale_8u : public MulScale<Fw8u, Fw8u, Fw8u, OPT_LEVEL::AC4>
			{
				XMM128 mask;
				FE_SSE2_REF
				
				IV SSE2_Init() 
				{
					Common::AC4::Init::For8(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],r.src2[0], temp);
					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};
			struct MulScale_16u : public MulScale<Fw16u, Fw16u, Fw16u, OPT_LEVEL::AC4>
			{
				XMM128 mask;
				FE_SSE2_REF
				
				IV SSE2_Init() 
				{
					Common::AC4::Init::For16(mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[0],r.src2[0], temp);
					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct MulCScale_8u : public MulCScale_AC4<Fw8u, Fw8u>
			{
				MulCScale_8u(const Fw8u valC[]) : OPT_LEVEL::MulCScale_AC4<Fw8u, Fw8u>(valC) {}

				XMM128 mask;
				FE_SSE2_REF
				
				IV SSE2_Init() 
				{
					Common::AC4::Init::For8(mask);
					MUL_SSE2::MULXSCALE::Init::AC4::MulCScale_8u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],val, temp);
					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);


				}
			};
			struct MulCScale_16u : public MulCScale_AC4<Fw16u, Fw16u>
			{
				MulCScale_16u(const Fw16u valC[]) : OPT_LEVEL::MulCScale_AC4<Fw16u, Fw16u>(valC) {}

				XMM128 mask;
				FE_SSE2_REF
				IV SSE2_Init() 
				{
					Common::AC4::Init::For16(mask);
					MUL_SSE2::MULXSCALE::Init::AC4::MulCScale_16u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;

					MUL_SSE2::MULXSCALE::MulScale_Mul_16u(r.src1[0], val, temp);
					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}
			};
		}
		namespace C4
		{
			struct MulScale_8u : public MulScale<Fw8u, Fw8u, Fw8u, OPT_LEVEL::C4>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};
			struct MulScale_16u : public MulScale<Fw16u, Fw16u, Fw16u, OPT_LEVEL::C4>
			{
				FE_SSE2_REF 
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_32u(r.src1[0],r.src2[0], r.dst[0]);
				}      
			};

			struct MulCScale_8u : public MulCScale_C4<Fw8u, Fw8u>
			{
				MulCScale_8u(const Fw8u valC[]) : OPT_LEVEL::MulCScale_C4<Fw8u, Fw8u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C4::MulCScale_8u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					MUL_SSE2::MULXSCALE::MulScale_Div_16u(r.src1[0], val, r.dst[0]);
				}      
			};
			struct MulCScale_16u : public MulCScale_C4<Fw16u, Fw16u>
			{
				MulCScale_16u(const Fw16u valC[]) : OPT_LEVEL::MulCScale_C4<Fw16u, Fw16u>(valC) {}

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					MUL_SSE2::MULXSCALE::Init::C4::MulCScale_16u(valC, val);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
                    MUL_SSE2::MULXSCALE::MulScale_Mul_16u(r.src1[0], val, r.dst[0]);
				}      
			};
		}
	}
}

} // namespace OPT_LEVEL

#endif	// #ifdef __MULDEF_H__

