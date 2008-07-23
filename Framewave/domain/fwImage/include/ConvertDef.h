/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __CONVERTDEF_H__
#define __CONVERTDEF_H__

#include "Convert.h"

namespace OPT_LEVEL
{

// C1R, C3R, C4R //common to many data types
template<class TS, CH cs, class TD, CH CD>
struct Convert_CR : fe2<TS, cs, TD, CD>
    {
    FE_REF

        IV REFR( const TS *s, TD *d ) const								
        {			
        Convert<TS, TD, cs>(s, d);
        }
    };

// AC4R // common to many data types
template<class TS, CH cs, class TD, CH CD>
struct Convert_AC4R : fe2<TS, cs, TD, CD>
    {
    FE_REF

        IV REFR( const TS *s, TD *d ) const								
        {			
        Convert<TS, TD, C3>(s, d);
        }
    };

// 8U16U, 8U16S C1R, C4R
template<class TS, CH cs, class TD, CH CD>
struct Convert_8u16_C14R : fe2<TS, cs, TD, CD>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_8u16_C1R_SSE2_I(reg, zero.i);
        }

    IV REFR( const TS *s, TD *d ) const								
        {			
        Convert<TS, TD, cs>(s, d);
        }
    };

// 8U16U, 8U16S C3R
template<class TS, class TD>
struct Convert_8u16_C3R : fe2<TS, C3, TD, C3>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_8u16_C3R_SSE2_I(reg, zero.i);
        }

    IV REFR( const TS *s, TD *d ) const								
        {			
        Convert<TS, TD, C3>(s, d);
        }
    };

// 8U32S C1R, C4R
//template<CH cs, CH CD>
//struct Convert_8u32s_C14R : fe2<A8U, cs, A32S, CD>
//    {
//    FE_SSE2_REF
//        XMM128 zero;
//
//    IV SSE2_Init()
//        {
//        Convert_zero_SETUP_I(zero.i);
//        }
//
//    IV SSE2( RegFile &reg ) const								
//        {			
//        Convert_8u32s_C1R_SSE2_I(reg, zero.i);
//        }
//
//    IV REFR( const A8U *s, A32S *d ) const								
//        {			
//        Convert<A8U, A32S, cs>(s, d);
//        }
//    };

// 8U32S C3R
//struct Convert_8u32s_C3R : fe2<A8U, C3, A32S, C3>
//    {
//    FE_SSE2_REF
//        XMM128 zero;
//
//    IV SSE2_Init()
//        {
//        Convert_zero_SETUP_I(zero.i);
//        }
//
//    IV SSE2( RegFile &reg ) const								
//        {			
//        Convert_8u32s_C3R_SSE2_I(reg, zero.i);
//        }
//
//    IV REFR( const A8U *s, A32S *d ) const								
//        {			
//        Convert<A8U, A32S, C3>(s, d);
//        }
//    };

template<CH cs, CH CD>
struct Convert_8u32s_C14R_Custom : fe2<A8U, cs, A32S, CD>
    {
    FE_CUSTOM_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *s1, A32S *d, U32 pixCount) const 		
        {			
        Convert_8u32s_C14R_Custom_SSE2<ia, A8U, CD>(s1, d, pixCount);
        }

    IV REFR( const A8U *s, A32S *d ) const								
        {			
        Convert<A8U, A32S, CD>(s, d);
        }
    };

template<CH cs, CH CD>
struct Convert_8s32s_C14R_Custom : fe2<A8S, cs, A32S, CD>
    {
    FE_CUSTOM_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8S *s1, A32S *d, U32 pixCount) const 		
        {			
        Convert_8s32s_C14R_Custom_SSE2<ia, A8S, CD>(s1, d, pixCount);
        }

    IV REFR( const A8S *s, A32S *d ) const								
        {			
        Convert<A8S, A32S, CD>(s, d);
        }
    };





template<CH cs, CH CD>
struct Convert_8u32f_C14R_Custom : fe2<A8U, cs, A32F, CD>
    {
    FE_CUSTOM_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8U *s1, A32F *d, U32 pixCount) const 		
        {			
        Convert_8u32f_C14R_Custom_SSE2<ia, A8U, CD>(s1, d, pixCount);
        }

    IV REFR( const A8U *s, A32F *d ) const								
        {			
        Convert<A8U, A32F, CD>(s, d);
        }
    };


template<CH cs, CH CD>
struct Convert_8s32f_C14R_Custom : fe2<A8S, cs, A32F, CD>
    {
    FE_CUSTOM_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        }
        template<IsAlign ia>
        IV Custom1D_sse2(const A8S *s1, A32F *d, U32 pixCount) const 		
        {			
        Convert_8s32f_C14R_Custom_SSE2<ia, A8S, CD>(s1, d, pixCount);
        }

    IV REFR( const A8S *s, A32F *d ) const								
        {			
        Convert<A8S, A32F, CD>(s, d);
        }
    };



// 16U32S C1R, C4R
template<CH cs, CH CD>
struct Convert_16u32s_C14R : fe2<A16U, cs, A32S, CD>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16u32s_C1R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16U *s, A32S *d ) const								
        {			
        Convert<A16U, A32S, cs>(s, d);
        }
    };

// 16U32S C3R
struct Convert_16u32s_C3R : fe2<A16U, C3, A32S, C3>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16u32s_C3R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16U *s, A32S *d ) const								
        {			
        Convert<A16U, A32S, C3>(s, d);
        }
    };

// 16U32F C1R, C4R
template<CH cs, CH CD>
struct Convert_16u32f_C14R : fe2<A16U, cs, A32F, CD>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16u32f_C1R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16U *s, A32F *d ) const								
        {			
        Convert<A16U, A32F, cs>(s, d);
        }
    };

// 16U32F C3R
struct Convert_16u32f_C3R : fe2<A16U, C3, A32F, C3>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16u32f_C3R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16U *s, A32F *d ) const								
        {			
        Convert<A16U, A32F, C3>(s, d);
        }
    };

// 16S32F C1R, C4R
template<CH cs, CH CD>
struct Convert_16s32f_C14R : fe2<A16S, cs, A32F, CD>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16s32f_C1R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16S *s, A32F *d ) const								
        {			
        Convert<A16S, A32F, cs>(s, d);
        }
    };

// 16S32F C3R
struct Convert_16s32f_C3R : fe2<A16S, C3, A32F, C3>
    {
    FE_SSE2_REF
        XMM128 zero;

    IV SSE2_Init()
        {
        Convert_zero_SETUP_I(zero.i);
        }

    IV SSE2( RegFile &reg ) const								
        {			
        Convert_16s32f_C3R_SSE2_I(reg, zero.i);
        }

    IV REFR( const A16S *s, A32F *d ) const								
        {			
        Convert<A16S, A32F, C3>(s, d);
        }
    };

// 16S8U C1R, C4R
template<class TS, class TD, CH CSD>
struct Convert_16s8u_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }

        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16s8u_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 16S8U C3R
template<class TS, class TD, CH CSD>
struct Convert_16s8u_C3R_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16s8u_C3R_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 16S8U AC4R
template<class TS, class TD, CH CSD>
struct Convert_16s8uAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF


        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16s8uAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };

// 16U8U C1R, C4R
template<class TS, class TD, CH CSD>
struct Convert_16u8u_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16u8u_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 16U8U C3R
template<class TS, class TD, CH CSD>
struct Convert_16u8u_C3R_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16u8u_C3R_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };


// 16U8U AC4R
template<class TS, class TD, CH CSD>
struct Convert_16u8uAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF


        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_16u8uAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };

// 32S8U C1R, C3R, C4R
template<class TS, class TD, CH CSD>
struct Convert_32s8u_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32s8u_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32S8U AC4R
template<class TS, class TD, CH CSD>
struct Convert_32s8uAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF


        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32s8uAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };

// 32S8S C1R, C3R, C4R
template<class TS, class TD, CH CSD>
struct Convert_32s8s_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32s8s_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32S8S AC4R
template<class TS, class TD, CH CSD>
struct Convert_32s8sAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
    FE_CUSTOM_SSE2_REF

        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32s8sAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };


template<class TS, class TD, CH CSD>
struct Convert_32f8u_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f8u_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32f8U AC4R
template<class TS, class TD, CH CSD>
struct Convert_32f8uAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f8uAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };





template<class TS, class TD, CH CSD>
struct Convert_32f8s_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f8s_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32f8s AC4R
template<class TS, class TD, CH CSD>
struct Convert_32f8sAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
        ConvertTo8u_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f8sAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };



template<class TS, class TD, CH CSD>
struct Convert_32f16s_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f16s_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32f8U AC4R
template<class TS, class TD, CH CSD>
struct Convert_32f16sAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
        ConvertTo16s_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f16sAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };

template<class TS, class TD, CH CSD>
struct Convert_32f16u_Custom : fe2<TS, CSD, TD, CSD>
    {
	FE_CUSTOM_SSE2_REF
    IV SSE2_Init() { }


        template<IsAlign ia>
        IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f16u_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,CSD>(s, d);
        }
    };

// 32f16u AC4R
template<class TS, class TD, CH CSD>
struct Convert_32f16uAC4_Custom : fe2<TS, CSD, TD, CSD>
    {
    XMM128 mask;
	FE_CUSTOM_SSE2_REF
        IV SSE2_Init()
        {
        ConvertTo16s_SETUP_I(mask.i);
        }
    template<IsAlign ia>
    IV Custom1D_sse2(const TS *s, TD *d, U32 pixCount)
        {			
        Convert_32f16uAC4_Custom_SSE2_I<TS, TD, CSD, ia> (s, d, pixCount, mask.i);
        }
    IV REFR(const TS *s, TD *d ) const								
        {			
        ConvertDown<TS,TD,C3>(s, d);
        }
    };



}; // OPT_LEVEL

#endif

