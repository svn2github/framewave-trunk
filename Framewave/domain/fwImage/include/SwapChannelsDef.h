/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SWAPCHANNELSDEF_H__
#define __SWAPCHANNELSDEF_H__

#include "SwapChannels.h"

namespace OPT_LEVEL
{

// A8U C3R
struct SwapChannels_8u_C3R012 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_8u_C3R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R012_SSE2_I(reg);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_C3R021 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R021_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_C3R102 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R102_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_C3R120 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskgb;

    FE_SSE2_REF

        SwapChannels_8u_C3R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R120_SETUP_I( maskr.i, maskgb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R120_SSE2_I(reg, maskr.i, maskgb.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_C3R201 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskrg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R201_SETUP_I( maskrg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R201_SSE2_I(reg, maskrg.i, maskb.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_C3R210 : fe2_96<A8U, C3, A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3R210_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

// A16U C3R
struct SwapChannels_16u_C3R012 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R012_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_C3R021 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R021_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_C3R102 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R102_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_C3R120 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R120_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_C3R201 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R201_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_C3R210 : fe2<A16U, C3, A16U, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF

        SwapChannels_16u_C3R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_C3R210_SSE2_I(reg);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

// A32S And A32F C3R
template<class TSD>
struct SwapChannels_32_C3R012 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

		FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R012_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template<class TSD>
struct SwapChannels_32_C3R021 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

	    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R021_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template<class TSD>
struct SwapChannels_32_C3R102 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R102_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template<class TSD>
struct SwapChannels_32_C3R120 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R120_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template<class TSD>
struct SwapChannels_32_C3R201 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R201_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template<class TSD>
struct SwapChannels_32_C3R210 : fe2<TSD, C3, TSD, C3>
    {
    const int * dstOrder;

    FE_SSE2_REF
		IV SSE2_Init() {}

        SwapChannels_32_C3R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_C3R210_SSE2_I(reg);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

// A8U AC4R
struct SwapChannels_8u_AC4R012 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R012_SETUP_I( maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R012_SSE2_I(reg, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_AC4R021 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb, maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R_SETUP_I( maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R021_SSE2_I(reg, maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_AC4R102 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb, maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R_SETUP_I( maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R102_SSE2_I(reg, maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_AC4R120 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maskr, maskgb, maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R120_SETUP_I( maskr.i, maskgb.i, maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R120_SSE2_I(reg, maskr.i, maskgb.i, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_AC4R201 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maskrg, maskb, maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R201_SETUP_I( maskrg.i, maskb.i, maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R201_SSE2_I(reg, maskrg.i, maskb.i, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_8u_AC4R210 : fe2<A8U, AC4, A8U, AC4>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb, maska;

    FE_SSE2_REF

        SwapChannels_8u_AC4R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_AC4R_SETUP_I( maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_AC4R210_SSE2_I(reg, maskr.i, maskg.i, maskb.i, maska.i);
        }
    IV REFR( const A8U *s, A8U *d ) const								
        {			
        Swap<A8U, C3> (s, d, dstOrder);
        }
    };

// A16U AC4R
struct SwapChannels_16u_AC4R012 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R012_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_AC4R021 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R021_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_AC4R102 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R102_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_AC4R120 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R120_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_AC4R201 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R201_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

struct SwapChannels_16u_AC4R210 : fe2<A16U, AC4, A16U, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_16u_AC4R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_16u_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_16u_AC4R210_SSE2_I(reg, maska.i);
        }
    IV REFR( const A16U *s, A16U *d ) const								
        {			
        Swap<A16U, C3> (s, d, dstOrder);
        }
    };

// A32S And A32F AC4R
template< class TSD>
struct SwapChannels_32_AC4R012 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R012(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R012_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template< class TSD>
struct SwapChannels_32_AC4R021 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R021_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template< class TSD>
struct SwapChannels_32_AC4R102 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R102_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template< class TSD>
struct SwapChannels_32_AC4R120 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R120_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template< class TSD>
struct SwapChannels_32_AC4R201 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R201_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

template< class TSD>
struct SwapChannels_32_AC4R210 : fe2<TSD, AC4, TSD, AC4>
    {
    const int * dstOrder;
    XMM128 maska;

    FE_SSE2_REF

        SwapChannels_32_AC4R210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_32_AC4R_SETUP_I(maska.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_32_AC4R210_SSE2_I(reg, maska.i);
        }
    IV REFR( const TSD *s, TSD *d ) const								
        {			
        Swap<TSD, C3> (s, d, dstOrder);
        }
    };

// A8U C3IR
struct SwapChannels_8u_C3IR021 : fe1_96<A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3IR021(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3IR021_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( A8U *d ) const								
        {			
        SwapI<A8U, C3> (d, dstOrder);
        }
    };

struct SwapChannels_8u_C3IR102 : fe1_96<A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3IR102(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3IR102_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( A8U *d ) const								
        {			
        SwapI<A8U, C3> (d, dstOrder);
        }
    };

struct SwapChannels_8u_C3IR120 : fe1_96<A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskgb;

    FE_SSE2_REF

        SwapChannels_8u_C3IR120(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R120_SETUP_I( maskr.i, maskgb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3IR120_SSE2_I(reg, maskr.i, maskgb.i);
        }
    IV REFR( A8U *d ) const								
        {			
        SwapI<A8U, C3> (d, dstOrder);
        }
    };

struct SwapChannels_8u_C3IR201 : fe1_96<A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskrg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3IR201(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R201_SETUP_I( maskrg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3IR201_SSE2_I(reg, maskrg.i, maskb.i);
        }
    IV REFR( A8U *d ) const								
        {			
        SwapI<A8U, C3> (d, dstOrder);
        }
    };

struct SwapChannels_8u_C3IR210 : fe1_96<A8U, C3>
    {
    const int * dstOrder;
    XMM128 maskr, maskg, maskb;

    FE_SSE2_REF

        SwapChannels_8u_C3IR210(const int *Order)
        {
        dstOrder = Order;
        }
    IV SSE2_Init()
        {
        SwapChannels_8u_C3R_SETUP_I( maskr.i, maskg.i, maskb.i);
        }
    IV SSE2( RegFile &reg ) const								
        {			
        SwapChannels_8u_C3IR210_SSE2_I(reg, maskr.i, maskg.i, maskb.i);
        }
    IV REFR( A8U *d ) const								
        {			
        SwapI<A8U, C3> (d, dstOrder);
        }
    };

} // OPT_LEVEL

#endif

