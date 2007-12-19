/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_MEMORY_PRIMITIVES_GH_H__
#define __CBL_MEMORY_PRIMITIVES_GH_H__

/// @cond OPT_DETAILS
CBL_INLINE void CBL_GH::Load_192( __m128i &rg_lo, __m128i &rg_hi, const __m128i* pMemory )
{
    const XMM128 & v1 = *reinterpret_cast<const XMM128*>(pMemory);
    const XMM128 & v2 = *reinterpret_cast<const XMM128*>(pMemory+1);
    XMM128 & r1 = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r2 = reinterpret_cast<XMM128&>(rg_hi);
	r1.d = _mm_loadu_pd		( &v1.f64[0]);		// lower  128-bit
	r2.d = _mm_load_sd		( &v2.f64[0]);		// higher 64-bit
}

CBL_INLINE void CBL_GH::Load_96( __m128i &rg, const __m128i* pMemory )
{
    XMM128 tmp;
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.i = _mm_cvtsi32_si128	( v.s32[2] );			// upper 32-bit
	r.i = _mm_slli_si128		( r.i, 8 );			// shift by 8 bytes (64-bits)
    tmp.d = _mm_load_sd			( &v.f64[0] );	// lower 64-bit	
	r.i = _mm_or_si128( r.i, tmp.i );
}

CBL_INLINE void CBL_GH::Load_64( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.d = _mm_load_sd( &v.f64[0] );
}

CBL_INLINE void CBL_GH::Store_192( __m128i* pMemory, __m128i &rg_lo, __m128i &rg_hi )
{
    XMM128 & r1 = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r2 = reinterpret_cast<XMM128&>(rg_hi);
	XMM128 & v1 = *reinterpret_cast<XMM128*>(pMemory);
	XMM128 & v2 = *reinterpret_cast<XMM128*>(pMemory+1);
	_mm_storeu_pd( &v1.f64[0], r1.d );	// lower  128 bit 
	_mm_store_sd( &v2.f64[0], r2.d );	// upper  64 bit
}

CBL_INLINE void CBL_GH::Store_96( __m128i* pMemory, __m128i &rg )
{
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
	_mm_store_sd( &v.f64[0], r.d    ); // sequential store, 64 bits first time
	r.i = _mm_srli_si128( r.i, 8 );	
	_mm_store_ss  ( &v.f32[2], r.f );	 // 32 bits store next time
}

CBL_INLINE void CBL_GH::Store_64( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	_mm_store_sd( &v.f64[0], r.d ); 
}

/// @endcond

#endif // __CBL_MEMORY_PRIMITIVES_GH_H__
