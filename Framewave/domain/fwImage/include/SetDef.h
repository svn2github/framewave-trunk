/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SETDEF_H__
#define __SETDEF_H__

#include "Set.h"

namespace OPT_LEVEL
{

// C1R
template<class TSD>
struct Set_C1R_Custom : fe1<TSD, C1>
{
	XMM128 value;
	TSD val;

	FE_CUSTOM_SSE2_REF

	Set_C1R_Custom(const TSD &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1R_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(TSD *d, U32 pixCount) 
		{			
			Set_C1R_C4R_Custom_SSE2_I<TSD, C1, ia>(d, pixCount, value);
		}
	IV REFR( TSD *d ) const								
		{			
			Set1<TSD>(d,val);
		}
};

struct Set_32f_C1R_Custom : fe1<A32F, C1>
{
	XMM128 value;
	A32F val;

	FE_CUSTOM_SSE2_REF

	Set_32f_C1R_Custom(const A32F &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1R_SETUP_I(value.f, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(A32F *d, U32 pixCount)
		{			
			Set_C1R_C4R_Custom_SSE2_I<A32F, C1, ia>(d, pixCount, value);
		}
	IV REFR(A32F *d ) const								
		{			
			Set1<A32F>(d,val);
		}
};

// C3R
template<class TSD>
struct Set_C3R_Custom : fe1<TSD, C3>
{
	XMM128 value1, value2, value3;
	const TSD *val;

	FE_CUSTOM_SSE2_REF

	Set_C3R_Custom(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C3R_SETUP_I(value1, value2, value3, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(TSD *d, U32 pixCount)
		{			
			Set_C3R_Custom_SSE2_I<TSD, ia>(d, pixCount, value1, value2, value3);
		}
	IV REFR( TSD *d ) const								
		{			
			Set<TSD, C3>(d,val);
		}
};

// C4R
template<class TSD>
struct Set_C4R_Custom : fe1<TSD, C4>
{
	XMM128 value;
	const TSD *val;

	FE_CUSTOM_SSE2_REF

	Set_C4R_Custom(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4R_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(TSD *d, U32 pixCount)		{			
			Set_C1R_C4R_Custom_SSE2_I<TSD, C4, ia>(d, pixCount, value);
		}
	IV REFR( TSD *d ) const								
		{			
			Set<TSD, C4>(d,val);
		}
};

struct Set_32f_C4R_Custom : fe1<A32F, C4>
{
	XMM128 value;
	const A32F *val;

	FE_CUSTOM_SSE2_REF

	Set_32f_C4R_Custom(const A32F *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4R_SETUP_I(value.f, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(A32F *d, U32 pixCount)
		{			
			Set_C1R_C4R_Custom_SSE2_I<A32F, C4, ia>(d, pixCount, value);
		}
	IV REFR(A32F *d ) const								
		{			
			Set<A32F, C4>(d,val);
		}
};

// AC4R
template<class TSD>
struct Set_AC4R : fe1<TSD, AC4>
{
	XMM128 mask, value;
	const TSD *val;

	FE_SSE2_REF
	
	Set_AC4R(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_AC4R_SETUP_I(mask.i, value, val);
		}
	IV SSE2( RegFile &reg ) const								
		{			
			Set_AC4R_SSE2_I(reg, mask.i, value.i);
		}
	IV REFR( TSD *d ) const								
		{			
			Set<TSD, C3>(d,val);
		}
};

// C1MR
template<class TSD>
struct Set_C1MR : fe2<A8U, C1, TSD, C1>
{
	TSD val;
	XMM128 value;

	FE_SSE2_REF
	
	Set_C1MR(const TSD &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1MR_SETUP_I(value.i, val);
		}
	IV SSE2( RegFile &reg ) const								
		{			
			Set_C1MR_SSE2_I(reg, value.i);
		}
	IV REFR(const A8U *m, TSD *d ) const								
		{			
			Set1M<TSD>(m, d, val);
		}
};

struct Set_16s_C1MR_Custom : fe2<A8U, C1, A16S, C1>
{
	XMM128 value;
	A16S val;

	FE_CUSTOM_SSE2_REF
	
	Set_16s_C1MR_Custom(const A16S &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1MR_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A16S *d, U32 pixCount) 
		{			
			Set_16s_C1MR_Custom_SSE2_I<ia>(m, d, pixCount, value);
		}
	IV REFR(const A8U *m, A16S *d ) const								
		{			
			Set1M<A16S>(m, d, val);
		}
};

struct Set_32s_C1MR_Custom : fe2<A8U, C1, A32S, C1>
{
	XMM128 value;
	XMM128 zero;
	A32S val;

	FE_CUSTOM_SSE2_REF
	
	Set_32s_C1MR_Custom(const A32S &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1MR_SETUP_I(value.i, zero.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32S *d, U32 pixCount) 
		{			
			Set_32_C1MR_Custom_SSE2_I<A32S, ia>(m, d, pixCount, value, zero.i);
		}
	IV REFR(const A8U *m, A32S *d ) const								
		{			
			Set1M<A32S>(m, d, val);
		}
};

struct Set_32f_C1MR_Custom : fe2<A8U, C1, A32F, C1>
{
	XMM128 value;
	XMM128 zero;
	A32F val;

	FE_CUSTOM_SSE2_REF
	
	Set_32f_C1MR_Custom(const A32F &v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C1MR_SETUP_I(value.f, zero.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32F *d, U32 pixCount) 
		{			
			Set_32_C1MR_Custom_SSE2_I<A32F, ia>(m, d, pixCount, value, zero.i);
		}
	IV REFR(const A8U *m, A32F *d ) const								
		{			
			Set1M<A32F>(m, d, val);
		}
};

// C3MR
template<class TSD>
struct Set_C3MR : fe2<A8U, C1, TSD, C3>
{
	const TSD *val;
	TSD temp;
	XMM128 value1, value2, value3, zero;

	FE_SSE2_REF
	
	Set_C3MR(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C3MR_SETUP_I(value1.i, value2.i, value3.i, zero.i, val);
		}
	IV SSE2( RegFile &reg ) const								
		{			
			Set_C3MR_SSE2_I(reg, value1.i, value2.i, value3.i, zero.i, temp);
		}
	IV REFR(const A8U *m, TSD *d ) const								
		{			
			SetM<TSD, C3>(m, d, val);
		}
};

struct Set_16s_C3MR_Custom : fe2<A8U, C1, A16S, C3>
{
	XMM128 value1, value2, value3, zero;
	const A16S *val;
	
	FE_CUSTOM_SSE2_REF
	
	Set_16s_C3MR_Custom(const A16S *v)
		{
			val = v;
		}
   	IV SSE2_Init()
		{
			Set_C3MR_SETUP_I(value1.i, value2.i, value3.i, zero.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A16S *d, U32 pixCount)
		{			
			Set_16s_C3MR_SSE2_I<ia> (m, d, pixCount, value1, value2, value3, zero);
		}
	IV REFR(const A8U *m, A16S *d ) const								
		{			
			SetM<A16S, C3>(m, d, val);
		}
};

struct Set_16s_C3MR_eq_Custom : fe2<A8U, C1, A16S, C3>
{
	XMM128 value1, value2, value3, zero;
	const A16S *val;
	
	FE_CUSTOM_SSE2_REF
	
	Set_16s_C3MR_eq_Custom(const A16S *v)
		{
			val = v;
		}
   	IV SSE2_Init()
		{
			Set_C3MR_SETUP_I(value1.i, value2.i, value3.i, zero.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A16S *d, U32 pixCount)
		{			
			Set_16s_C3MR_eq_SSE2_I<ia> (m, d, pixCount, value1, zero);
		}
	IV REFR(const A8U *m, A16S *d ) const								
		{			
			SetM<A16S, C3>(m, d, val);
		}
};

struct Set_32s_C3MR_Custom : fe2<A8U, C1, A32S, C3>
{
	XMM128 value;	
	const A32S *val;
	
	FE_CUSTOM_SSE2_REF
	
	Set_32s_C3MR_Custom(const A32S *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C3MR_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32S *d, U32 pixCount)
		{			
			Set_32_C3MR_SSE2_I<A32S, ia> (m, d, pixCount, value);
		}
	IV REFR(const A8U *m, A32S *d ) const								
		{			
			SetM<A32S, C3>(m, d, val);
		}
};

struct Set_32f_C3MR_Custom : fe2<A8U, C1, A32F, C3>
{
	XMM128 value;	
	const A32F *val;
	
	FE_CUSTOM_SSE2_REF
	
	Set_32f_C3MR_Custom(const A32F *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C3MR_SETUP_I(value.f, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32F *d, U32 pixCount)
		{			
			Set_32_C3MR_SSE2_I<A32F, ia> (m, d, pixCount, value);
		}
	IV REFR(const A8U *m, A32F *d ) const								
		{			
			SetM<A32F, C3>(m, d, val);
		}
};

// C4MR
template<class TSD>
struct Set_C4MR : fe2<A8U, C1, TSD, C4>
{
	const TSD *val;
	TSD temp;
	XMM128 value, zero;

	FE_SSE2_REF
	
	Set_C4MR(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4MR_SETUP_I(value.i, zero.i, val);
		}
	IV SSE2( RegFile &reg ) const								
		{			
			Set_C4MR_SSE2_I(reg, value.i, zero.i, temp);
		}
	IV REFR(const A8U *m, TSD *d ) const								
		{			
			SetM<TSD, C4>(m, d, val);
		}
};

struct Set_16s_C4MR_Custom : fe2<A8U, C1, A16S, C4>
{
	XMM128 value1, value2, value3;
	const A16S *val;
	FE_CUSTOM_SSE2_REF
	
	Set_16s_C4MR_Custom(const A16S *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4MR_SETUP_I(value1.i, value2.i, value3.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A16S *d, U32 pixCount)
		{			
			Set_16s_C4MR_Custom_SSE2_I<ia>(m, d, pixCount, value1, value2.i, value3.i);
		}
	IV REFR(const A8U *m, A16S *d ) const								
		{			
			SetM<A16S, C4>(m, d, val);
		}
};

struct Set_32s_C4MR_Custom : fe2<A8U, C1, A32S, C4>
{
	XMM128 value;
	const A32S *val;
	FE_CUSTOM_SSE2_REF
	
	Set_32s_C4MR_Custom(const A32S *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4MR_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32S *d, U32 pixCount)
		{			
			Set_32_C4MR_Custom_SSE2_I<A32S, ia>(m, d, pixCount, value);
		}
	IV REFR(const A8U *m, A32S *d ) const								
		{			
			SetM<A32S, C4>(m, d, val);
		}
};

struct Set_32f_C4MR_Custom : fe2<A8U, C1, A32F, C4>
{
	XMM128 value;
	const A32F *val;
	FE_CUSTOM_SSE2_REF
	
	Set_32f_C4MR_Custom(const A32F *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_C4MR_SETUP_I(value.f, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32F *d, U32 pixCount)
		{			
			Set_32_C4MR_Custom_SSE2_I<A32F, ia>(m, d, pixCount, value);
		}
	IV REFR(const A8U *m, A32F *d ) const								
		{			
			SetM<A32F, C4>(m, d, val);
		}
};

// AC4MR
template<class TSD>
struct Set_AC4MR : fe2<A8U, C1, TSD, AC4>
{
	const TSD *val;
	TSD temp;
	XMM128 value, mask, zero;

	FE_SSE2_REF
	
	Set_AC4MR(const TSD *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_AC4MR_SETUP_I(value.i, mask.i, zero.i, val);
		}
	IV SSE2( RegFile &reg ) const								
		{			
			Set_AC4MR_SSE2_I(reg, value.i, mask.i, zero.i, temp);
		}
	IV REFR(const A8U *m, TSD *d ) const								
		{			
			SetM<TSD, C3>(m, d, val);
		}
};

struct Set_16s_AC4MR_Custom : fe2<A8U, C1, A16S, AC4>
{
	XMM128 value1, value2, value3;
	const A16S *val;
	FE_CUSTOM_SSE2_REF
	
	Set_16s_AC4MR_Custom(const A16S *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_AC4MR_SETUP_I(value1.i, value2.i, value3.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A16S *d, U32 pixCount)
		{			
			Set_16s_AC4MR_Custom_SSE2_I<ia>(m, d, pixCount, value1.i, value2.i, value3.i);
		}
	IV REFR(const A8U *m, A16S *d ) const								
		{			
			SetM<A16S, C3>(m, d, val);
		}
};

struct Set_32s_AC4MR_Custom : fe2<A8U, C1, A32S, AC4>
{
	XMM128 value;
	const A32S *val;
	FE_CUSTOM_SSE2_REF
	
	Set_32s_AC4MR_Custom(const A32S *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_AC4MR_SETUP_I(value.i, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32S *d, U32 pixCount)
		{			
			Set_32_AC4MR_Custom_SSE2_I<A32S, ia>(m, d, pixCount, value.i);
		}
	IV REFR(const A8U *m, A32S *d ) const								
		{			
			SetM<A32S, C3>(m, d, val);
		}
};

struct Set_32f_AC4MR_Custom : fe2<A8U, C1, A32F, AC4>
{
	XMM128 value;
	const A32F *val;
	FE_CUSTOM_SSE2_REF
	
	Set_32f_AC4MR_Custom(const A32F *v)
		{
			val = v;
		}
	IV SSE2_Init()
		{
			Set_AC4MR_SETUP_I(value.f, val);
		}
    template<IsAlign ia>
	IV Custom1D_sse2(const A8U *m, A32F *d, U32 pixCount)
		{			
			Set_32_AC4MR_Custom_SSE2_I<A32F, ia>(m, d, pixCount, value.i);
		}
	IV REFR(const A8U *m, A32F *d ) const								
		{			
			SetM<A32F, C3>(m, d, val);
		}
};

// C3CR
template<class TSD>
struct Set_C3CR_Custom: fe1<TSD,C3>
{
	TSD val;
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
	
	Set_C3CR_Custom(TSD &v)
		{
			val = v;
		}
    template<IsAlign ia>
	IV Custom1D_sse2(TSD *d, U32 pixCount) const
		{			
			Set_C3CR_Custom_SSE2_I<TSD, ia>(d, pixCount, val);
		}
	IV REFR(TSD *d ) const								
		{			
			Set1<TSD>(d, val);
		}
};

// C4CR
template<class TSD>
struct Set_C4CR_Custom : fe1<TSD,C4>
{
	TSD val;
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
	
	Set_C4CR_Custom(TSD &v)
		{
			val = v;
		}
    template<IsAlign ia>
	IV Custom1D_sse2(TSD *d, U32 pixCount) const
		{			
			Set_C4CR_Custom_SSE2_I<TSD, ia>(d, pixCount, val);
		}
	IV REFR(TSD *d ) const								
		{			
			Set1<TSD>(d, val);
		}
};

} // OPT_LEVEL
#endif

