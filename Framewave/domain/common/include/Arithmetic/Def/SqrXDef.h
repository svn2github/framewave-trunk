/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SQRXDEF_H__
#define __SQRXDEF_H__

#include "SqrXImpl.h"
#include "fe.h"


namespace OPT_LEVEL
{

template<class TS, class TD, CH c>
struct SqrtI : public fe2< TS, c, TD, c>   
{
	int scaleFactor;
	XMM128 scale;

	SqrtI(int scaleFactor_)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SQRX_REF::SQRT::SqrtI(s, c, d, c, scaleFactor);
	} 
};

template<class TS, class TD, CH c>
struct SqrI : public fe2< TS, c, TD, c>   
{
	int scaleFactor;
	XMM128 scaleMasks[4];

	SqrI(int scaleFactor_)
	{
		scaleFactor = scaleFactor_;
	}

	IV REFR(const TS *s, TD *d) const		// REFR Pixel function
	{
		SQRX_REF::SQR::SqrI(s, c, d, c, scaleFactor);
	} 
};



namespace DEF_SQRX
{
	namespace SQRT
	{
		namespace C1
		{
			template <int scaleType>
			struct Sqrt_8u	: public SqrtI< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				Sqrt_8u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_8u_setup<scaleType>(scaleFactor, scale);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[0], r.dst[0], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16s	: public SqrtI< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Sqrt_16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16s_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[0], r.dst[0], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16u	: public SqrtI< Fw16u, Fw16u, OPT_LEVEL::C1 >
			{
				
				Sqrt_16u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16u, Fw16u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16u_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[0], r.dst[0], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16sc	: public SqrtI< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				XMM128 min32s, half;
				Sqrt_16sc(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16sc_setup<scaleType>(scaleFactor, scale, min32s, half);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16sc_unit<scaleType>(r.src1[0], r.dst[0], scale, min32s, half);
				}      
			};


			struct Sqrt_32f	: public fe2< Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sqrt_ps( r.src1[0].f );
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sqrt_64f	: public fe2< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_sqrt_pd( r.src1[0].d );
				}      
				IV REFR(const Fw64f *s, Fw64f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sqrt_32fc	: public fe2< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				XMM128 negbmask, half;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_32fc_setup(negbmask, half);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_32fc_unit(r.src1[0], r.dst[0], negbmask, half);
				}      
				IV REFR(const Fw32fc *s, Fw32fc *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sqrt_64fc	: public fe2< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				XMM128 negbmask, half;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_64fc_setup(negbmask, half);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_64fc_unit(r.src1[0], r.dst[0], negbmask, half);
				}      
				IV REFR(const Fw64fc *s, Fw64fc *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};


			template <int scaleType>
			struct Sqrt_32s16s	: public SqrtI< Fw32s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Sqrt_32s16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw32s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_32s16s_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_32s16s_unit<scaleType>(r.src1[0], r.dst[0], scale);
				}      
			};


			struct Sqrt_64s	: public SqrtI< Fw64s, Fw64s, OPT_LEVEL::C1 >
			{
				Sqrt_64s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw64s, Fw64s, OPT_LEVEL::C1 >(scaleFactor) {};
				FE_REF
			};

			struct Sqrt_64s16s	: public SqrtI< Fw64s, Fw16s, OPT_LEVEL::C1 >
			{
				Sqrt_64s16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw64s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};
				FE_REF
			};
		}

		namespace C3
		{
			template <int scaleType>
			struct Sqrt_8u	: public SqrtI< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				Sqrt_8u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_8u_setup<scaleType>(scaleFactor, scale);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[2], r.dst[2], scale);
				}
			};

			template <int scaleType>
			struct Sqrt_16s	: public SqrtI< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				Sqrt_16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16s_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[2], r.dst[2], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16u	: public SqrtI< Fw16u, Fw16u, OPT_LEVEL::C3 >
			{
				
				Sqrt_16u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16u, Fw16u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16u_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[2], r.dst[2], scale);
				}      
			};

			struct Sqrt_32f	: public fe2< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sqrt_ps( r.src1[0].f );
					r.dst[1].f = _mm_sqrt_ps( r.src1[1].f );
					r.dst[2].f = _mm_sqrt_ps( r.src1[2].f );
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct Sqrt_8u	: public SqrtI< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sqrt_8u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQRT::Init::AC4::Sqrt_8u_setup<scaleType>(scaleFactor, scale, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[0], temp, scale);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sqrt_16s	: public SqrtI< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sqrt_16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::AC4::Sqrt_16s_setup<scaleType>(scaleFactor, scale, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>( r.src1[0], temp, scale);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sqrt_16u	: public SqrtI< Fw16u, Fw16u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sqrt_16u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16u, Fw16u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::AC4::Sqrt_16u_setup<scaleType>(scaleFactor, scale, mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>( r.src1[0], temp, scale);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Sqrt_32f	: public fe2< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::AC4::Sqrt_32f_setup(mask);

				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_sqrt_ps( r.src1[0].f );

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct Sqrt_8u	: public SqrtI< Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				Sqrt_8u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_8u_setup<scaleType>(scaleFactor, scale);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[2], r.dst[2], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_8u_unit<scaleType>(r.src1[3], r.dst[3], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16s	: public SqrtI< Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				Sqrt_16s(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16s_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[2], r.dst[2], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16s_unit<scaleType>(r.src1[3], r.dst[3], scale);
				}      
			};

			template <int scaleType>
			struct Sqrt_16u	: public SqrtI< Fw16u, Fw16u, OPT_LEVEL::C4 >
			{
				
				Sqrt_16u(int scaleFactor) : OPT_LEVEL::SqrtI< Fw16u, Fw16u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQRT::Init::C1::Sqrt_16u_setup<scaleType>(scaleFactor, scale);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[0], r.dst[0], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[1], r.dst[1], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[2], r.dst[2], scale);
					SQRX_SSE2::SQRT::Unit::C1::Sqrt_16u_unit<scaleType>(r.src1[3], r.dst[3], scale);
				}      
			};

			struct Sqrt_32f	: public fe2< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_sqrt_ps( r.src1[0].f );
					r.dst[1].f = _mm_sqrt_ps( r.src1[1].f );
					r.dst[2].f = _mm_sqrt_ps( r.src1[2].f );
					r.dst[3].f = _mm_sqrt_ps( r.src1[3].f );
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQRT::SqrtF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

	namespace SQR
	{
		namespace C1
		{
			template <int scaleType>
			struct Sqr_8u	: public SqrI< Fw8u, Fw8u, OPT_LEVEL::C1 >
			{
				XMM128 sqrCap;
				Sqr_8u(int scaleFactor) : OPT_LEVEL::SqrI< Fw8u, Fw8u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_8u_setup<scaleType>(scaleFactor, scaleMasks, sqrCap);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks, sqrCap);
				}      
			};

			template <int scaleType>
			struct Sqr_16s	: public SqrI< Fw16s, Fw16s, OPT_LEVEL::C1 >
			{
				
				Sqr_16s(int scaleFactor) : OPT_LEVEL::SqrI< Fw16s, Fw16s, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Sqr_16u	: public SqrI< Fw16u, Fw16u, OPT_LEVEL::C1 >
			{
				Sqr_16u(int scaleFactor) : OPT_LEVEL::SqrI< Fw16u, Fw16u, OPT_LEVEL::C1 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16u_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
				}      
			};
	
			struct Sqr_32f	: public fe2< Fw32f, OPT_LEVEL::C1, Fw32f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps (r.src1[0].f, r.src1[0].f);
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sqr_64f	: public fe2< Fw64f, OPT_LEVEL::C1, Fw64f, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].d = _mm_mul_pd (r.src1[0].d, r.src1[0].d);
				}      
				IV REFR(const Fw64f *s, Fw64f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			template <int scaleType>
			struct Sqr_16sc	: public SqrI< Fw16sc, Fw16sc, OPT_LEVEL::C1 >
			{
				Sqr_16sc(int scaleFactor) : OPT_LEVEL::SqrI< Fw16sc, Fw16sc, OPT_LEVEL::C1 >(scaleFactor) {};

				XMM128 negbmask, min32s;
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16sc_setup<scaleType>(scaleFactor, scaleMasks[0], min32s, negbmask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16sc_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0], min32s, negbmask);
				}      
			};

			struct Sqr_32fc	: public fe2< Fw32fc, OPT_LEVEL::C1, Fw32fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_32fc_unit(r.src1[0], r.dst[0]);
				}      
				IV REFR(const Fw32fc *s, Fw32fc *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

			struct Sqr_64fc	: public fe2< Fw64fc, OPT_LEVEL::C1, Fw64fc, OPT_LEVEL::C1 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_64fc_unit(r.src1[0], r.dst[0]);
				}      
				IV REFR(const Fw64fc *s, Fw64fc *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C1, d, OPT_LEVEL::C1);
				}  
			};

		}

		namespace C3
		{
			template <int scaleType>
			struct Sqr_8u	: public SqrI< Fw8u, Fw8u, OPT_LEVEL::C3 >
			{
				XMM128 sqrCap;
				Sqr_8u(int scaleFactor) : OPT_LEVEL::SqrI< Fw8u, Fw8u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_8u_setup<scaleType>(scaleFactor, scaleMasks, sqrCap);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks, sqrCap);
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks, sqrCap);
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks, sqrCap);
				}      
			};

			template <int scaleType>
			struct Sqr_16s	: public SqrI< Fw16s, Fw16s, OPT_LEVEL::C3 >
			{
				
				Sqr_16s(int scaleFactor) : OPT_LEVEL::SqrI< Fw16s, Fw16s, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Sqr_16u	: public SqrI< Fw16u, Fw16u, OPT_LEVEL::C3 >
			{
				
				Sqr_16u(int scaleFactor) : OPT_LEVEL::SqrI< Fw16u, Fw16u, OPT_LEVEL::C3 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16u_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0]);
				}      
			};

			struct Sqr_32f	: public fe2< Fw32f, OPT_LEVEL::C3, Fw32f, OPT_LEVEL::C3 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps (r.src1[0].f, r.src1[0].f);
					r.dst[1].f = _mm_mul_ps (r.src1[1].f, r.src1[1].f);
					r.dst[2].f = _mm_mul_ps (r.src1[2].f, r.src1[2].f);
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C3, d, OPT_LEVEL::C3);
				}  
			};
		}



		namespace AC4
		{
			template <int scaleType>
			struct Sqr_8u	: public SqrI< Fw8u, Fw8u, OPT_LEVEL::AC4 >
			{
				XMM128 mask, sqrCap;
				Sqr_8u(int scaleFactor) : OPT_LEVEL::SqrI< Fw8u, Fw8u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQR::Init::AC4::Sqr_8u_setup<scaleType>(scaleFactor, scaleMasks, sqrCap, mask);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[0], temp, scaleMasks, sqrCap);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sqr_16s	: public SqrI< Fw16s, Fw16s, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sqr_16s(int scaleFactor) : OPT_LEVEL::SqrI< Fw16s, Fw16s, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::AC4::Sqr_16s_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>( r.src1[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			template <int scaleType>
			struct Sqr_16u	: public SqrI< Fw16u, Fw16u, OPT_LEVEL::AC4 >
			{
				XMM128 mask;
				Sqr_16u(int scaleFactor) : OPT_LEVEL::SqrI< Fw16u, Fw16u, OPT_LEVEL::AC4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::AC4::Sqr_16u_setup<scaleType>(scaleFactor, scaleMasks[0], mask);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>( r.src1[0], temp, scaleMasks[0]);

					Common::AC4::Apply::MaskI(r.dst[0], temp, mask);
				}      
			};

			struct Sqr_32f	: public fe2< Fw32f, OPT_LEVEL::AC4, Fw32f, OPT_LEVEL::AC4 >
			{
				XMM128 mask;

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::AC4::Sqr_32f_setup(mask);

				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					XMM128 temp;
					temp.f = _mm_mul_ps (r.src1[0].f, r.src1[0].f);

					Common::AC4::Apply::MaskF(r.dst[0], temp, mask);
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::AC4, d, OPT_LEVEL::AC4);
				}  
			};
		}

		namespace C4
		{
			template <int scaleType>
			struct Sqr_8u	: public SqrI< Fw8u, Fw8u, OPT_LEVEL::C4 >
			{
				XMM128 sqrCap;
				Sqr_8u(int scaleFactor) : OPT_LEVEL::SqrI< Fw8u, Fw8u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init()
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_8u_setup<scaleType>(scaleFactor, scaleMasks, sqrCap);
				}

				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks, sqrCap);
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks, sqrCap);
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks, sqrCap);
					SQRX_SSE2::SQR::Unit::C1::Sqr_8u_unit<scaleType>(r.src1[3], r.dst[3], scaleMasks, sqrCap);
				}      
			};

			template <int scaleType>
			struct Sqr_16s	: public SqrI< Fw16s, Fw16s, OPT_LEVEL::C4 >
			{
				
				Sqr_16s(int scaleFactor) : OPT_LEVEL::SqrI< Fw16s, Fw16s, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16s_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16s_unit<scaleType>(r.src1[3], r.dst[3], scaleMasks[0]);
				}      
			};

			template <int scaleType>
			struct Sqr_16u	: public SqrI< Fw16u, Fw16u, OPT_LEVEL::C4 >
			{
				
				Sqr_16u(int scaleFactor) : OPT_LEVEL::SqrI< Fw16u, Fw16u, OPT_LEVEL::C4 >(scaleFactor) {};

				FE_SSE2_REF 
				IV SSE2_Init() 
				{
					SQRX_SSE2::SQR::Init::C1::Sqr_16u_setup<scaleType>(scaleFactor, scaleMasks[0]);
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[0], r.dst[0], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[1], r.dst[1], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[2], r.dst[2], scaleMasks[0]);
					SQRX_SSE2::SQR::Unit::C1::Sqr_16u_unit<scaleType>(r.src1[3], r.dst[3], scaleMasks[0]);
				}      
			};

			struct Sqr_32f	: public fe2< Fw32f, OPT_LEVEL::C4, Fw32f, OPT_LEVEL::C4 >
			{
				FE_SSE2_REF 
				IV SSE2_Init() 
				{
				}
				IV SSE2( RegFile & r ) const									// SSE2 Pixel function
				{
					r.dst[0].f = _mm_mul_ps (r.src1[0].f, r.src1[0].f);
					r.dst[1].f = _mm_mul_ps (r.src1[1].f, r.src1[1].f);
					r.dst[2].f = _mm_mul_ps (r.src1[2].f, r.src1[2].f);
					r.dst[3].f = _mm_mul_ps (r.src1[3].f, r.src1[3].f);
				}      
				IV REFR(const Fw32f *s, Fw32f *d) const	// REFR Pixel function
				{
					SQRX_REF::SQR::SqrF(s, OPT_LEVEL::C4, d, OPT_LEVEL::C4);
				}  
			};
		}
	}

}

} // namespace OPT_LEVEL

#endif	// #ifdef __SQRTDEF_H__

