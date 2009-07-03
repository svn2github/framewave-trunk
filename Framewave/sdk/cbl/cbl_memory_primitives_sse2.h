/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_MEMORY_PRIMITIVES_SSE2_H__
#define __CBL_MEMORY_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS

CBL_INLINE void CBL_SSE2::Load_1x16_8u( __m128i & variable, U8 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u8[0] = value;
	v.u8[1] = value;
	v.u8[2] = value;
	v.u8[3] = value;
	variable = _mm_shuffle_epi32( variable, _MM_SHUFFLE( 0,0,0,0 ) );
}

CBL_INLINE void CBL_SSE2::Load_1x16_8s( __m128i & variable, S8 value )
{
    Load_1x16_8u( variable, (U8)value );
}

CBL_INLINE void CBL_SSE2::Load_1x8_16u( __m128i & variable, U16 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u16[0] = value;
	v.u16[1] = value;
	variable = _mm_shuffle_epi32( variable, _MM_SHUFFLE( 0,0,0,0 ) );
}

CBL_INLINE void CBL_SSE2::Load_1x8_16s( __m128i & variable, S16 value )
{
    Load_1x8_16u( variable, (U16)value );
}

CBL_INLINE void CBL_SSE2::Load_2x4_16u( __m128i & variable, U16 value1, U16 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u16[0] = value1;
	v.u16[1] = value2;
	variable = _mm_shuffle_epi32( variable, _MM_SHUFFLE( 0,0,0,0 ) );
}

CBL_INLINE void CBL_SSE2::Load_2x4_16s( __m128i & variable, S16 value1, S16 value2 )
{
    Load_2x4_16u( variable, (U16)value1 , (U16)value2 );
}

CBL_INLINE void CBL_SSE2::Load_1x4_32u( __m128i & variable, U32 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u32[0] = value;
	variable = _mm_shuffle_epi32( variable, _MM_SHUFFLE( 0,0,0,0 ) );
}

CBL_INLINE void CBL_SSE2::Load_1x4_32s( __m128i & variable, S32 value )
{
    Load_1x4_32u( variable, (U32)value );
}

CBL_INLINE void CBL_SSE2::Load_1x4_32f( __m128 & variable, F32 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.f32[0] = value;
	v.i = _mm_shuffle_epi32( v.i, _MM_SHUFFLE( 0,0,0,0 ) );
}

CBL_INLINE void CBL_SSE2::Load_192( __m128i &rg_lo, __m128i &rg_hi, const __m128i* pMemory )
{
    const XMM128 & v1 = *reinterpret_cast<const XMM128*>(pMemory);
    const XMM128 & v2 = *reinterpret_cast<const XMM128*>(pMemory+1);
    XMM128 & r1 = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r2 = reinterpret_cast<XMM128&>(rg_hi);
	r1.d = _mm_loadl_pd		( r1.d,	&v1.f64[0]);		// lower  64-bit
	r1.d = _mm_loadh_pd		( r1.d,	&v1.f64[1]);		// middle 64-bit
	r2.d = _mm_loadl_pd		( r2.d,	&v2.f64[0]);		// higher 64-bit
}

CBL_INLINE void CBL_SSE2::Load_96( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.i = _mm_cvtsi32_si128	( v.s32[2] );			// upper 32-bit
	r.i = _mm_slli_si128		( r.i, 8 );			// shift by 8 bytes (64-bits)
	r.d = _mm_loadl_pd			( r.d, &v.f64[0] );	// lower 64-bit	
}

CBL_INLINE void CBL_SSE2::Load_64( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.d = _mm_loadl_pd( r.d, &v.f64[0] );
}

CBL_INLINE void CBL_SSE2::Load_32( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.i = _mm_cvtsi32_si128( v.s32[0] );
}

CBL_INLINE void CBL_SSE2::Store_192( __m128i* pMemory, __m128i &rg_lo, __m128i &rg_hi )
{
    XMM128 & r1 = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r2 = reinterpret_cast<XMM128&>(rg_hi);
	XMM128 & v1 = *reinterpret_cast<XMM128*>(pMemory);
	XMM128 & v2 = *reinterpret_cast<XMM128*>(pMemory+1);
	_mm_storel_pd( &v1.f64[0], r1.d );	// lower  64 bit 
	_mm_storeh_pd( &v1.f64[1], r1.d );	// middle 64 bit
	_mm_storel_pd( &v2.f64[0], r2.d );	// upper  64 bit
}

CBL_INLINE void CBL_SSE2::Store_96( __m128i* pMemory, __m128i &rg )
{
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
	_mm_storel_pd( &v.f64[0], r.d    ); // sequential store, 64 bits first time
	r.i = _mm_srli_si128( r.i, 8 );	
	_mm_store_ss ( &v.f32[2], r.f );	 // 32 bits store next time
}

CBL_INLINE void CBL_SSE2::Store_64( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	_mm_storel_pd( &v.f64[0], r.d ); 
}

CBL_INLINE void CBL_SSE2::Store_32( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	_mm_store_ss( &v.f32[0], r.f );
}

CBL_INLINE void CBL_SSE2::LUT_8u( __m128i & reg, U8* table )
{
	XMM128 & r = reinterpret_cast<XMM128&>(reg);
	U32 r0	= r.u32[0];
	U32 r1	= r.u32[1];
	U32 r2	= r.u32[2];
	U32 r3	= r.u32[3];

	r0			= 	(table[(U8) (r0 >> 24)] << 24)
				  |	(table[(U8) (r0 >> 16)] << 16)
				  |	(table[(U8) (r0 >>  8)] <<  8)
				  |	(table[(U8) (r0		)]		);

	r1			= 	(table[(U8) (r1 >> 24)] << 24)
				  |	(table[(U8) (r1 >> 16)] << 16)
				  |	(table[(U8) (r1 >>  8)] <<  8)
				  |	(table[(U8) (r1		)]		);

	r2			= 	(table[(U8) (r2 >> 24)] << 24)
				  |	(table[(U8) (r2 >> 16)] << 16)
				  |	(table[(U8) (r2 >>  8)] <<  8)
				  |	(table[(U8) (r2		)]		);

	r3			= 	(table[(U8) (r3 >> 24)] << 24)
				  |	(table[(U8) (r3 >> 16)] << 16)
				  |	(table[(U8) (r3 >>  8)] <<  8)
				  |	(table[(U8) (r3		)]		);

	r.i			= _mm_set_epi32(r3, r2, r1, r0);
}
/// @endcond

#endif // __CBL_MEMORY_PRIMITIVES_SSE2_H__
