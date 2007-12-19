/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __TRIGONOMETRICDEF_H__
#define __TRIGONOMETRICDEF_H__

#include "Trigonometric.h"
#include "Trigonometric_NETLIB.h"


namespace OPT_LEVEL
{

// Cos
struct Cos_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Cos_32f_A24(s, d);
		}
};

struct Cos_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Cos_64f_A53(s, d);
		}
};

// Sin
struct Sin_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Sin_32f_A24(s, d);
		}
};

struct Sin_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Sin_64f_A53(s, d);
		}
};

// SinCos // Needs multiple dest support from fe.
struct SinCos_32f_All : fe3 < A32F, C1, A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, const A32F *d1, A32F *d2 ) const // remove the const for d1 later								
		{													 // Added only for compilation
			SinCos_32f_A24(s, d1, d2);
		}
};
struct SinCos_64f_All : fe3 < A64F, C1, A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, const A64F *d1, A64F *d2 ) const// remove the const for d1 later																
		{													// Added only for compilation
			SinCos_64f_A53(s, d1, d2);
		}
};

// Tan
struct Tan_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Tan_32f_A24(s, d);
		}
};

struct Tan_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Tan_64f_A53(s, d);
		}
};

// Acos
struct Acos_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Acos_32f_A24(s, d);
		}
};

struct Acos_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Acos_64f_A53(s, d);
		}
};

// Asin
struct Asin_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Asin_32f_A24(s, d);
		}
};

struct Asin_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Asin_64f_A53(s, d);
		}
};

// Atan
struct Atan_32f_All : fe2 < A32F, C1, A32F, C1>
{
	Atan_DATA data;
	FE_SSE2_REF

	IV SSE2_Init()
		{
			Atan_32f_SETUP_S(data);
		}
	IV SSE2(RegFile &reg )const								
		{			
			Atan_32f_A24_SSE2_S(reg, data);
		}
	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Atan_32f_A24(s, d);
		}
};

struct Atan_64f_All : fe2 < A64F, C1, A64F, C1>
{
	Atan_DATA data;
	FE_SSE2_REF

	IV SSE2_Init()
		{
			Atan_64f_SETUP_S(data);
		}
	IV SSE2(RegFile &reg )const								
		{			
			Atan_64f_A53_SSE2_S(reg, data);
		}
	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Atan_64f_A53(s, d);
		}
};
struct Atan_32f_Inplace : fe1 < A32F, C1>
{
	Atan_DATA data;
	FE_SSE2_REF

	IV SSE2_Init()
		{
			Atan_32f_SETUP_S(data);
		}
	IV SSE2(RegFile &reg )const								
		{			
			Atan_32f_inplace_SSE2_S(reg, data);
		}
	IV REFR(A32F *d ) const								
		{			
			Atan_32f_A24_inplace(d);
		}
};

struct Atan_64f_Inplace : fe1 < A64F, C1>
{
	Atan_DATA data;
	FE_SSE2_REF

	IV SSE2_Init()
		{
			Atan_64f_SETUP_S(data);
		}
	IV SSE2(RegFile &reg )const								
		{			
			Atan_64f_inplace_SSE2_S(reg, data);
		}
	IV REFR( A64F *d ) const								
		{			
			Atan_64f_A53_inplace(d);
		}
};

// Atan2
struct Atan2_32f_All : fe3 < A32F, C1, A32F, C1, A32F, C1>
{
	FE_SSE2_REF

	IV SSE2(RegFile &reg )const								
		{			
			Atan2_32f_A24_SSE2_S(reg);
		}
	IV REFR( const A32F *s1, const A32F *s2, A32F *d ) const								
		{			
			Atan2_32f_A24(s1, s2, d);
		}
};

struct Atan2_64f_All : fe3 < A64F, C1, A64F, C1, A64F, C1>
{
	FE_SSE2_REF

	IV SSE2(RegFile &reg )const								
		{			
			Atan2_64f_A53_SSE2_S(reg);
		}
	IV REFR( const A64F *s1, const A64F *s2, A64F *d ) const								
		{			
			Atan2_64f_A53(s1, s2, d);
		}
};

// Cosh
struct Cosh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Cosh_32f_A24(s, d);
		}
};

struct Cosh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Cosh_64f_A53(s, d);
		}
};

// Sinh
struct Sinh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Sinh_32f_A24(s, d);
		}
};

struct Sinh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Sinh_64f_A53(s, d);
		}
};

// Tanh
struct Tanh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Tanh_32f_A24(s, d);
		}
};

struct Tanh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Tanh_64f_A53(s, d);
		}
};

// Acosh
struct Acosh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Acosh_32f_A24(s, d);
		}
};

struct Acosh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Acosh_64f_A53(s, d);
		}
};

// Asinh
struct Asinh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Asinh_32f_A24(s, d);
		}
};

struct Asinh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Asinh_64f_A53(s, d);
		}
};

// Atanh
struct Atanh_32f_All : fe2 < A32F, C1, A32F, C1>
{
	FE_REF

	IV REFR( const A32F *s, A32F *d ) const								
		{			
			Atanh_32f_A24(s, d);
		}
};

struct Atanh_64f_All : fe2 < A64F, C1, A64F, C1>
{
	FE_REF

	IV REFR( const A64F *s, A64F *d ) const								
		{			
			Atanh_64f_A53(s, d);
		}
};

} // namespace OPT_LEVEL
#endif
