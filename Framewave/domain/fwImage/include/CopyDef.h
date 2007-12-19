/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COPYDEF_H__
#define __COPYDEF_H__

#include "Copy.h"

namespace OPT_LEVEL
{

// C1R, C3R, C4R --  For All Data Types
template<class TSD,CH CSD> 
struct Copy_CR_Custom : fe2<TSD, CSD, TSD, CSD>
	{
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

		template<IsAlign ia>
		IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 
		{ 
			Copy_CR_Custom_SSE2_I<TSD, CSD, ia>(s, d, pixCount);
		}

		IV REFR(const TSD *s, TSD *d ) const								
		{
			Copy<TSD, CSD>(s, d);
		}
	};

template<class TSD> 
struct Copy_C3R_Custom : fe2<TSD, C3, TSD, C3>
	{
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

		template<IsAlign ia>
		IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 
		{ 
			Copy_C3R_Custom_SSE2_I<TSD, ia>(s, d, pixCount);
		}

		IV REFR(const TSD *s, TSD *d ) const								
		{
			Copy<TSD, C3>(s, d);
		}
	};

// AC4R -- For All Data Types
template<class TSD>
struct Copy_AC4R : fe2<TSD, AC4, TSD, AC4>
    {
    XMM128 mask;
    TSD temp;

    FE_SSE2_REF
        IV SSE2_Init()
        {
        Copy_AC4R_SETUP_I(mask.i, temp);
        }
    IV SSE2(RegFile &reg )const								
        {			
        Copy_AC4R_SSE2_I(reg, mask.i);
        }				
    IV REFR(const TSD *s, TSD *d ) const								
        {			
        Copy<TSD, C3>(s, d);
        }
    };

// C1MR 
template<class TSD>
struct Copy_C1MR : fe3<A8U, C1, TSD, C1, TSD, C1>
    {
    XMM128 mask;
    TSD temp;

    FE_SSE2_REF
        IV SSE2_Init()
        {
        Copy_CMR_SETUP_I(mask.i);
        }
    IV SSE2(RegFile &reg )const								
        {			
        Copy_C1MR_SSE2_I(reg, mask.i, temp);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<A8U, C1>(m, s, d);
        }	
    };

struct Copy_16s_C1MR_Custom : fe3<A8U, C1, A16S, C1, A16S, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const A16S *s, A16S *d, U32 pixCount) const 								
        {			
        Copy_16s_C1MR_Custom_SSE2_I<ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const A16S *s, A16S *d ) const								
        {			
        CopyM<A16S, C1>(m, s, d);
        }	
    };

template<class TSD>
struct Copy_32_C1MR_Custom : fe3<A8U, C1, TSD, C1, TSD, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const TSD *s, TSD *d, U32 pixCount) const 								
        {			
        Copy_32_C1MR_Custom_SSE2_I<TSD, ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C1>(m, s, d);
        }	
    };

 //C3MR 
template<class TSD>
struct Copy_C3MR : fe3<A8U, C1, TSD, C3, TSD, C3>
    {
    XMM128 mask;
    TSD temp;

    FE_SSE2_REF
        IV SSE2_Init()
        {
        Copy_CMR_SETUP_I(mask.i);
        }
    IV SSE2(RegFile &reg )const								
        {			
        Copy_C3MR_SSE2_I(reg, mask.i, temp);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C3>(m, s, d);
        }	
    };

template<class TSD>
struct Copy_C3MR_custom : fe3<A8U, C1, TSD, C3, TSD, C3>
    {
    XMM128 mask;
    TSD temp;

        FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
           
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const TSD *s, TSD *d, U32 pixCount) const 								
        {			
        Copy_C3MR_Custom_SSE2_I<TSD, ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C3>(m, s, d);
        }	
    };

struct Copy_C3MR_16s_custom : fe3<A8U, C1, A16S, C3, A16S, C3>
    {
    XMM128 mask;
    A16S temp;

        FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
           
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const A16S *s, A16S *d, U32 pixCount) const 								
        {			
        Copy_C3MR_16s_Custom_SSE2_I<ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const A16S *s, A16S *d ) const								
        {			
        CopyM<A16S, C3>(m, s, d);
        }	
    };


// C4MR
template<class TSD>
struct Copy_C4MR : fe3<A8U, C1, TSD, C4, TSD, C4>
    {
    XMM128 mask;
    TSD temp;

    FE_SSE2_REF
        IV SSE2_Init()
        {
        Copy_CMR_SETUP_I(mask.i);
        }
    IV SSE2(RegFile &reg )const								
        {			
        Copy_C4MR_SSE2_I(reg, mask.i, temp);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C4>(m, s, d);
        }
    };

struct Copy_16s_C4MR_Custom : fe3<A8U, C1, A16S, C4, A16S, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const A16S *s, A16S *d, U32 pixCount) const 								
        {			
        Copy_16s_C4MR_Custom_SSE2_I<ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const A16S *s, A16S *d ) const								
        {			
        CopyM<A16S, C4>(m, s, d);
        }
    };

template<class TSD>
struct Copy_32_C4MR_Custom : fe3<A8U, C1, TSD, C4, TSD, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const TSD *s, TSD *d, U32 pixCount) const 								
        {			
        Copy_32_C4MR_Custom_SSE2_I<TSD, ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C4>(m, s, d);
        }
    };

// AC4MR
template<class TSD>
struct Copy_AC4MR : fe3<A8U, C1, TSD, AC4, TSD, AC4>
    {
    XMM128 AC4_mask, zero;
    TSD temp;

    FE_SSE2_REF
        IV SSE2_Init()
        {
        Copy_AC4MR_SETUP_I(AC4_mask.i, zero.i, temp);
        }
    IV SSE2(RegFile &reg )const								
        {			
        Copy_AC4MR_SSE2_I(reg, AC4_mask.i, zero.i, temp);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C3>(m, s, d);
        }
    };

struct Copy_16s_AC4MR_Custom : fe3<A8U, C1, A16S, AC4, A16S, AC4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const A16S *s, A16S *d, U32 pixCount) const 								
        {			
        Copy_16s_AC4MR_Custom_SSE2_I<ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const A16S *s, A16S *d ) const								
        {			
        CopyM<A16S, C3>(m, s, d);
        }
    };

template<class TSD>
struct Copy_32_AC4MR_Custom : fe3<A8U, C1, TSD, AC4, TSD, AC4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }
        
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *m, const TSD *s, TSD *d, U32 pixCount) const 		
        {			
        Copy_32_AC4MR_Custom_SSE2_I<TSD, ia>(m, s, d, pixCount);
        }				
    IV REFR(const A8U *m, const TSD *s, TSD *d ) const								
        {			
        CopyM<TSD, C3>(m, s, d);
        }
    };

// C3CR
template<class TSD>
struct Copy_C3CR_Custom : fe2<TSD, C3, TSD, C3>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 		
        {			
        Copy_C3CR_Custom_SSE2_I<TSD, ia>(s, d, pixCount);
        }				
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy1<TSD>(s, d);
        }
    };

// C4CR
template<class TSD>
struct Copy_C4CR_Custom : fe2<TSD, C4, TSD, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 		
        {			
        Copy_C4CR_Custom_SSE2_I<TSD, ia>(s, d, pixCount);
        }				
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy1<TSD>(s, d);
        }
    };

// C3C1R
template<class TSD>
struct Copy_C3C1R : fe2<TSD, C3, TSD, C1>
    {
    TSD temp;
    FE_SSE2_REF
		IV SSE2_Init() {}

        IV SSE2( RegFile &reg ) const								
        {			
        Copy_C3C1R_SSE2_I(reg, temp);
        }				
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        CopyNto1<TSD>(s, d);
        }
    };

// C4C1R
template<class TSD>
struct Copy_C4C1R : fe2<TSD, C4, TSD, C1>
    {
    FE_REF

        IV REFR( const TSD *s, TSD *d ) const								
        {			
        CopyNto1<TSD>(s, d);
        }
    };

struct Copy_8u_C4C1R_Custom : fe2<A8U, C4, A8U, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *s, A8U *d, U32 pixCount) const 		
        {			
        Copy_8u_C4C1R_Custom_SSE2_I<ia>(s, d, pixCount);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        CopyNto1<A8U>(s, d);
        }
    };

struct Copy_16s_C4C1R_Custom : fe2<A16S, C4, A16S, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const A16S *s, A16S *d, U32 pixCount) const 		
        {			
        Copy_16s_C4C1R_Custom_SSE2_I<ia>(s, d, pixCount);
        }
    IV REFR( const A16S *s, A16S *d ) const								
        {			
        CopyNto1<A16S>(s, d);
        }
    };

// C1C3R
template<class TSD>
struct Copy_C1C3R : fe2<TSD, C1, TSD, C3>
    {
    XMM128 mask1, mask2, mask3;
    TSD temp;

    FE_SSE2_REF

        IV SSE2_Init()
        {
        Copy_C1C3R_SETUP_I(mask1.i, mask2.i, mask3.i, temp);
        }	
    IV SSE2( RegFile &reg ) const								
        {			
        Copy_C1C3R_SSE2_I(reg, mask1.i, mask2.i, mask3.i, temp);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy1toN<TSD, C1>(s, d);
        }
    };

template<class TSD>
struct Copy_32_C1C3R_Custom : fe2<TSD, C1, TSD, C3>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 		
        {			
        Copy_32_C1C3R_Custom_SSE2_I<TSD, ia>(s, d, pixCount);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy1toN<TSD, C1>(s, d);
        }
    };

// C1C4R
template<class TSD>
struct Copy_C1C4R_Custom : fe2<TSD, C1, TSD, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d, U32 pixCount) const 		
        {			
        Copy_C1C4R_Custom_SSE2_I<TSD, ia>(s, d, pixCount);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy1toN<TSD, C1>(s, d);
        }
    };

// C3AC4R
template<class TSD>
struct Copy_C3AC4R : fe2<TSD, C3, TSD, AC4>
    {
    FE_REF

        IV REFR( const TSD *s, TSD *d ) const								
        {			
        Copy_C3AC4(s, d);
        }
    };

// AC4C3R
struct Copy_8U_AC4C3R : fe2<A8U, AC4, A8U, C3>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *s, A8U *d, U32 pixCount) const 		
        {			
        Copy_8u_AC4C3R_Custom_SSE2_I(s, d, pixCount);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Copy<A8U, C3>(s, d);
        }
    };

struct Copy_16S_AC4C3R : fe2<A16S, AC4, A16S, C3>
    {
    XMM128 mask2, notmask2;

    FE_SSE2_REF

        IV SSE2_Init()
        {
        Copy_16s_AC4C3R_SETUP_I(mask2.i, notmask2.i);
        }	
    IV SSE2( RegFile &reg ) const								
        {			
        Copy_16s_AC4C3R_SSE2_I(reg, mask2.i, notmask2.i);
        }
    IV REFR( const A16S *s, A16S *d ) const								
        {			
        Copy<A16S, C3>(s, d);
        }
    };

struct Copy_32S_AC4C3R : fe2<A32S, AC4, A32S, C3>
    {
    FE_SSE2_REF

        IV SSE2( RegFile &reg ) const								
        {			
        Copy_32S_AC4C3R_SSE2_I(reg);
        }
    IV REFR( const A32S *s, A32S *d ) const								
        {			
        Copy<A32S, C3>(s, d);
        }
    };

struct Copy_32F_AC4C3R : fe2<A32F, AC4, A32F, C3>
    {
    FE_SSE2_REF

        IV SSE2( RegFile &reg ) const								
        {			
        Copy_32F_AC4C3R_SSE2_I(reg);
        }
    IV REFR( const A32F *s, A32F *d ) const								
        {			
        Copy<A32F, C3>(s, d);
        }
    };

// P3C3R
template<class TSD>
struct Copy_P3C3R_Custom : fe4<TSD, C1, TSD, C1, TSD, C1, TSD, C3>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s1, const TSD *s2, const TSD *s3, TSD *d, U32 pixCount) const 		
        {			
        Copy_P3C3R_Custom_SSE2_I<ia>(s1, s2, s3, d, pixCount);
        }
    IV REFR( const TSD *s1, const TSD *s2, const TSD *s3, TSD *d ) const								
        {			
        Copy3to1<TSD>(s1, s2, s3, d);
        }
    };

template<class TSD>
struct Copy_32_P3C3R_Custom : fe4<TSD, C1, TSD, C1, TSD, C1, TSD, C3>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s1, const TSD *s2, const TSD *s3, TSD *d, U32 pixCount) const 		
        {			
        Copy_P3C3R_Custom_SSE2_I<TSD, ia>(s1, s2, s3, d, pixCount);
        }
    IV REFR( const TSD *s1, const TSD *s2, const TSD *s3, TSD *d ) const								
        {			
        Copy3to1<TSD>(s1, s2, s3, d);
        }
    };

// P4C4R
template<class TSD>
struct Copy_P4C4R_Custom : fe4S1D<TSD, C1, TSD, C1, TSD, C1, TSD, C1, TSD, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d, U32 pixCount) const 		
        {			
        Copy_P4C4R_Custom_SSE2_I<ia>(s1, s2, s3, s4, d, pixCount);
        }
    IV REFR( const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d ) const								
        {			
        Copy4to1<TSD>(s1, s2, s3, s4, d);
        }
    };

template<class TSD>
struct Copy_32_P4C4R_Custom : fe4S1D<TSD, C1, TSD, C1, TSD, C1, TSD, C1, TSD, C4>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d, U32 pixCount) const 		
        {			
        Copy_P4C4R_Custom_SSE2_I<TSD, ia>(s1, s2, s3, s4, d, pixCount);
        }
    IV REFR( const TSD *s1, const TSD *s2, const TSD *s3, const TSD *s4, TSD *d ) const								
        {			
        Copy4to1<TSD>(s1, s2, s3, s4, d);
        }
    };

// C3P3R
template<class TSD>
struct Copy_C3P3R_Custom : fe1S3D<TSD, C3, TSD, C1, TSD, C1, TSD, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d1, TSD *d2, TSD *d3, U32 pixCount) const 		
        {			
        Copy_C3P3R_Custom_SSE2_I<ia>(s, d1, d2, d3, pixCount);
        }
    IV REFR( const TSD *s, TSD *d1, TSD *d2, TSD *d3) const								
        {			
        Copy1to3<TSD>(s, d1, d2, d3);
        }
    };

template<class TSD>
struct Copy_32_C3P3R_Custom : fe1S3D<TSD, C3, TSD, C1, TSD, C1, TSD, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d1, TSD *d2, TSD *d3, U32 pixCount) const 		
        {			
        Copy_C3P3R_Custom_SSE2_I<TSD, ia>(s, d1, d2, d3, pixCount);
        }
    IV REFR( const TSD *s, TSD *d1, TSD *d2, TSD *d3) const								
        {			
        Copy1to3<TSD>(s, d1, d2, d3);
        }
    };

// C4P4R
template<class TSD>
struct Copy_C4P4R_Custom : fe1S4D<TSD, C4, TSD, C1, TSD, C1, TSD, C1, TSD, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d1, TSD *d2, TSD *d3, TSD *d4, U32 pixCount) const 		
        {			
        Copy_C4P4R_Custom_SSE2_I<ia>(s, d1, d2, d3, d4, pixCount);
        }
    IV REFR( const TSD *s, TSD *d1, TSD *d2, TSD *d3, TSD *d4) const								
        {			
        Copy1to4<TSD>(s, d1, d2, d3, d4);
        }
    };

template<class TSD>
struct Copy_32_C4P4R_Custom : fe1S4D<TSD, C4, TSD, C1, TSD, C1, TSD, C1, TSD, C1>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TSD *s, TSD *d1, TSD *d2, TSD *d3, TSD *d4, U32 pixCount) const 		
        {			
        Copy_C4P4R_Custom_SSE2_I<TSD, ia>(s, d1, d2, d3, d4, pixCount);
        }
    IV REFR( const TSD *s, TSD *d1, TSD *d2, TSD *d3, TSD *d4) const								
        {			
        Copy1to4<TSD>(s, d1, d2, d3, d4);
        }
    };
}; // OPT_LEVEL

#endif

