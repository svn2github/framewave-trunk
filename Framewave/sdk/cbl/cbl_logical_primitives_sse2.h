/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_LOGICAL_PRIMITIVES_SSE2_H__
#define __CBL_LOGICAL_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS

//! Shifts the 16 unsigned 8-bit integers in a right by value bits while shifting in 0.
CBL_INLINE __m128i CBL_SSE2::RShift_8u ( const __m128i & src, const __m128i & value )
{
	__m128i dst1, dst2;
	dst1 = _mm_slli_epi16 ( src, 8);
	dst1 = _mm_srl_epi16 ( dst1, value);
	dst1 = _mm_srli_epi16 ( dst1, 8);
	dst2 = _mm_srl_epi16 ( src, value);
	dst2 = _mm_srli_epi16 ( dst2, 8);
	dst2 = _mm_slli_epi16 ( dst2, 8);
	return _mm_or_si128 ( dst1, dst2);
}

//! Shifts the 16 signed 8-bit integers in a right by value bits while shifting in the sign bit
CBL_INLINE __m128i CBL_SSE2::RShift_8s ( const __m128i & src, const __m128i & value )
{
	__m128i dst1,dst2;
	dst1 = _mm_slli_epi16 ( src, 8);
	dst1 = _mm_sra_epi16 ( dst1, value);
	dst1 = _mm_srli_epi16 ( dst1, 8);
	dst2 = _mm_sra_epi16 ( src, value);
	dst2 = _mm_srli_epi16 ( dst2, 8);
	dst2 = _mm_slli_epi16 ( dst2, 8);
	return _mm_or_si128 ( dst1, dst2);
}

//! Shifts the 16 unsigned 8-bit integers in a left by value bits while shifting in 0
CBL_INLINE __m128i CBL_SSE2::LShift_8u ( const __m128i & src, const __m128i & value )
{
	__m128i dst1, dst2;
	dst1 = _mm_slli_epi16 ( src, 8);
	dst1 = _mm_sll_epi16 ( dst1, value);
	dst1 = _mm_srli_epi16 ( dst1, 8);
	dst2 = _mm_srli_epi16 (src, 8);
	dst2 = _mm_slli_epi16 ( dst2, 8);
	dst2 = _mm_sll_epi16 ( dst2, value);
	return _mm_or_si128 ( dst1, dst2);
}

template< int count >
CBL_INLINE __m128i CBL_SSE2::RotateR( const __m128i & src )
{
    __m128i tmp = _mm_slli_si128( src, 16 - count );
    __m128i result = _mm_srli_si128( src, count );
    result = _mm_or_si128( result, tmp );
    return result;
}

template< int count >
CBL_INLINE __m128i CBL_SSE2::RotateL( const __m128i & src )
{
    return CBL_SSE2::RotateR< 16 - count >( src );
}

CBL_INLINE void CBL_SSE2::Reverse8( __m128i & values )
{
    __m128i tmp = _mm_slli_epi16( values, 8 );
    values = _mm_srli_epi16( values, 8 );
    values = _mm_or_si128( values, tmp );
    values = _mm_shufflelo_epi16( values, CBL_SHUF_IMM( 0, 1, 2, 3 ) );
    values = _mm_shufflehi_epi16( values, CBL_SHUF_IMM( 0, 1, 2, 3 ) );
    values = _mm_shuffle_epi32( values, CBL_SHUF_IMM( 1, 0, 3, 2 ) );
}

CBL_INLINE void CBL_SSE2::Reverse16( __m128i & values )
{
    values = _mm_shufflelo_epi16( values, CBL_SHUF_IMM( 0, 1, 2, 3 ) );
    values = _mm_shufflehi_epi16( values, CBL_SHUF_IMM( 0, 1, 2, 3 ) );
    values = _mm_shuffle_epi32( values, CBL_SHUF_IMM( 1, 0, 3, 2 ) );
}

CBL_INLINE void CBL_SSE2::Reverse32( __m128i & values )
{
    values = _mm_shuffle_epi32( values, CBL_SHUF_IMM( 0, 1, 2, 3 ) );
}

CBL_INLINE void CBL_SSE2::Reverse64( __m128i & values )
{
    values = _mm_shuffle_epi32( values, CBL_SHUF_IMM( 1, 0, 3, 2 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const __m128i & value )
{
    U32 tmp;
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    tmp = _mm_extract_epi16( value, 7 );
    a = _mm_insert_epi16( a, tmp, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U32 tmp;
    c = _mm_slli_si128( c, 2 );
    tmp = _mm_extract_epi16( b, 7 );
    c = _mm_insert_epi16( c, tmp, 0 );
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    tmp = _mm_extract_epi16( value, 7 );
    a = _mm_insert_epi16( a, tmp, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U32 tmp;
    d = _mm_slli_si128( d, 2 );
    tmp = _mm_extract_epi16( c, 7 );
    d = _mm_insert_epi16( d, tmp, 0 );
    c = _mm_slli_si128( c, 2 );
    tmp = _mm_extract_epi16( b, 7 );
    c = _mm_insert_epi16( c, tmp, 0 );
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    tmp = _mm_extract_epi16( value, 7 );
    a = _mm_insert_epi16( a, tmp, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const __m128i & value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    tmp = _mm_extract_epi16( value, 0 );
    b = _mm_insert_epi16( b, tmp, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    tmp = _mm_extract_epi16( c, 0 );
    b = _mm_insert_epi16( b, tmp, 7 );
    c = _mm_srli_si128( c, 2 );
    tmp = _mm_extract_epi16( value, 0 );
    c = _mm_insert_epi16( c, tmp, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    tmp = _mm_extract_epi16( c, 0 );
    b = _mm_insert_epi16( b, tmp, 7 );
    c = _mm_srli_si128( c, 2 );
    tmp = _mm_extract_epi16( d, 0 );
    c = _mm_insert_epi16( c, tmp, 7 );
    d = _mm_srli_si128( d, 2 );
    tmp = _mm_extract_epi16( value, 0 );
    d = _mm_insert_epi16( d, tmp, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const S16 value )
{
    U32 tmp;
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    a = _mm_insert_epi16( a, value, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const S16 value )
{
    U32 tmp;
    c = _mm_slli_si128( c, 2 );
    tmp = _mm_extract_epi16( b, 7 );
    c = _mm_insert_epi16( c, tmp, 0 );
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    a = _mm_insert_epi16( a, value, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 value )
{
    U32 tmp;
    d = _mm_slli_si128( d, 2 );
    tmp = _mm_extract_epi16( c, 7 );
    d = _mm_insert_epi16( d, tmp, 0 );
    c = _mm_slli_si128( c, 2 );
    tmp = _mm_extract_epi16( b, 7 );
    c = _mm_insert_epi16( c, tmp, 0 );
    b = _mm_slli_si128( b, 2 );
    tmp = _mm_extract_epi16( a, 7 );
    b = _mm_insert_epi16( b, tmp, 0 );
    a = _mm_slli_si128( a, 2 );
    a = _mm_insert_epi16( a, value, 0 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const S16 value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    b = _mm_insert_epi16( b, value, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const S16 value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    tmp = _mm_extract_epi16( c, 0 );
    b = _mm_insert_epi16( b, tmp, 7 );
    c = _mm_srli_si128( c, 2 );
    c = _mm_insert_epi16( c, value, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 value )
{
    U32 tmp;
    a = _mm_srli_si128( a, 2 );
    tmp = _mm_extract_epi16( b, 0 );
    a = _mm_insert_epi16( a, tmp, 7 );
    b = _mm_srli_si128( b, 2 );
    tmp = _mm_extract_epi16( c, 0 );
    b = _mm_insert_epi16( b, tmp, 7 );
    c = _mm_srli_si128( c, 2 );
    tmp = _mm_extract_epi16( d, 0 );
    c = _mm_insert_epi16( c, tmp, 7 );
    d = _mm_srli_si128( d, 2 );
    d = _mm_insert_epi16( d, value, 7 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_3R_16s( __m128i & a, __m128i & b, const U8 * pvalues )
{
    __m128i tmp;
    __m128i zero = _mm_setzero_si128();

    a = _mm_srli_si128( a, 6 );
    tmp = _mm_slli_si128( b, 10 );
    a = _mm_or_si128( a, tmp );
    
    b = _mm_srli_si128( b, 6 );

    // the _mm_cvtsi32_si128 should generate a MOVD instruction.  There are two variants:
    // 1. MOVD reg128, mem32
    // 2. MOVD reg32, mem32 
    //    MOVD reg128, reg32
    // we use the volatile keyword to trick the compiler into using variant 1 which is much faster
    tmp = _mm_cvtsi32_si128( *(volatile S32 *)(pvalues-1) ); 
    //d = _mm_load_ss( (float *)pvalues );
    tmp = _mm_unpacklo_epi8( tmp, zero );
    tmp = _mm_srli_si128( tmp, 2 );
    tmp = _mm_slli_si128( tmp, 10 );
    b = _mm_or_si128( b, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_4R_16s( __m128i & a, __m128i & b, const U8 * pvalues )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128i tmp;
    __m128i zero = _mm_setzero_si128();
    a = _mm_srli_si128( a, 8 );
    va.f = _mm_movelh_ps( va.f, vb.f );
    b = _mm_srli_si128( b, 8 );
    // the _mm_cvtsi32_si128 should generate a MOVD instruction.  There are two variants:
    // 1. MOVD reg128, mem32
    // 2. MOVD reg32, mem32 
    //    MOVD reg128, reg32
    // we use the volatile keyword to trick the compiler into using variant 1 which is much faster
    tmp = _mm_cvtsi32_si128( *(volatile S32 *)pvalues ); 
    //d = _mm_load_ss( (float *)pvalues );
    tmp = _mm_unpacklo_epi8( tmp, zero );
    tmp = _mm_slli_si128( tmp, 8 );
    b = _mm_or_si128( b, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128 tmp;
    tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vd.f = _mm_move_ss( vd.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, tmp );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, tmp );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(&value) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vd.f );
    vd.f = _mm_move_ss( vd.f, tmp );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32f( __m128 & a, __m128 & b, F32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128 tmp = _mm_load_ss( &value );
    a = _mm_move_ss( a, b );
    b = _mm_move_ss( b, tmp );
    va.i = _mm_shuffle_epi32( va.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    vb.i = _mm_shuffle_epi32( vb.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, F32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128 tmp = _mm_load_ss( &value );
    a = _mm_move_ss( a, b );
    b = _mm_move_ss( b, c );
    c = _mm_move_ss( c, d );
    d = _mm_move_ss( d, tmp );
    va.i = _mm_shuffle_epi32( va.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    vb.i = _mm_shuffle_epi32( vb.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    vc.i = _mm_shuffle_epi32( vc.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    vd.i = _mm_shuffle_epi32( vd.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    XMM128 tmp;
    __m128i signext;
    __m128i zero = _mm_setzero_si128();

    va.i = _mm_srli_si128( va.i, 12 );
    tmp.i = _mm_slli_si128( vb.i, 4 );
    va.i = _mm_or_si128( va.i, tmp.i );
    
    vb.i = _mm_srli_si128( vb.i, 12 );
    tmp.i = _mm_slli_si128( vc.i, 4 );
    vb.i = _mm_or_si128( vb.i, tmp.i );
    
    vc.i = _mm_srli_si128( vc.i, 12 );
    tmp.i = _mm_slli_si128( vd.i, 4 );
    vc.i = _mm_or_si128( vc.i, tmp.i );
    
    vd.i = _mm_srli_si128( vd.i, 12 );

    // the _mm_cvtsi32_si128 should generate a MOVD instruction.  There are two variants:
    // 1. MOVD reg128, mem32
    // 2. MOVD reg32, mem32 
    //    MOVD reg128, reg32
    // we use the volatile keyword to trick the compiler into using variant 1 which is much faster
    tmp.i = _mm_cvtsi32_si128( *(volatile S32 *)(pvalues-1) ); 
    //d = _mm_load_ss( (float *)pvalues );
    tmp.i = _mm_unpacklo_epi8( tmp.i, zero );
    signext = _mm_srai_epi16( tmp.i, 15 );
    tmp.i = _mm_unpacklo_epi16( tmp.i, signext );
    tmp.f = _mm_cvtepi32_ps( tmp.i );
    tmp.i = _mm_srli_si128( tmp.i, 4 );
    tmp.i = _mm_slli_si128( tmp.i, 4 );
    vd.i = _mm_or_si128( vd.i, tmp.i );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues )
{
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128i signext;
    __m128i zero = _mm_setzero_si128();
    a = b;
    b = c;
    c = d;
    // the _mm_cvtsi32_si128 should generate a MOVD instruction.  There are two variants:
    // 1. MOVD reg128, mem32
    // 2. MOVD reg32, mem32 
    //    MOVD reg128, reg32
    // we use the volatile keyword to trick the compiler into using variant 1 which is much faster
    vd.i = _mm_cvtsi32_si128( *(volatile S32 *)pvalues ); 
    //d = _mm_load_ss( (float *)pvalues );
    vd.i = _mm_unpacklo_epi8( vd.i, zero );
    signext = _mm_srai_epi16( vd.i, 15 );
    vd.i = _mm_unpacklo_epi16( vd.i, signext );
    d = _mm_cvtepi32_ps( vd.i );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, const S16 * pvalues )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
	__m128i signext;
    XMM128 tmp;

    va.i = _mm_srli_si128( va.i, 12 );
    tmp.i = _mm_slli_si128( vb.i, 4 );
    va.i = _mm_or_si128( va.i, tmp.i );
    
    vb.i = _mm_srli_si128( vb.i, 12 );

    tmp.d = _mm_loadl_pd( tmp.d, reinterpret_cast<const double*>(pvalues - 1) );
    signext = _mm_srai_epi16( tmp.i, 15 );
    tmp.i = _mm_unpacklo_epi16( tmp.i, signext );
    tmp.f = _mm_cvtepi32_ps( tmp.i );
    tmp.i = _mm_srli_si128( tmp.i, 4 );
    tmp.i = _mm_slli_si128( tmp.i, 4 );
    vb.i = _mm_or_si128( vb.i, tmp.i );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, const S16 * pvalues )
{
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
	__m128i signext;
    a = b;
    vb.d = _mm_loadl_pd( vb.d, reinterpret_cast<const double*>(pvalues) );
    signext = _mm_srai_epi16( vb.i, 15 );
    vb.i = _mm_unpacklo_epi16( vb.i, signext );
    b = _mm_cvtepi32_ps( vb.i );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    XMM128 & ve = reinterpret_cast<XMM128&>(e);
    XMM128 & vf = reinterpret_cast<XMM128&>(f);
    XMM128 & vg = reinterpret_cast<XMM128&>(g);
    XMM128 & vh = reinterpret_cast<XMM128&>(h);
    va.i = _mm_srli_si128( va.i, 8 );
    va.f = _mm_movelh_ps( va.f, vb.f );
    vb.i = _mm_srli_si128( vb.i, 8 );
    vb.f = _mm_movelh_ps( vb.f, vc.f );
    vc.i = _mm_srli_si128( vc.i, 8 );
    vc.f = _mm_movelh_ps( vc.f, vd.f );
    vd.i = _mm_srli_si128( vd.i, 8 );
    vd.f = _mm_movelh_ps( vd.f, ve.f );
    ve.i = _mm_srli_si128( ve.i, 8 );
    ve.f = _mm_movelh_ps( ve.f, vf.f );
    vf.i = _mm_srli_si128( vf.i, 8 );
    vf.f = _mm_movelh_ps( vf.f, vg.f );
    vg.i = _mm_srli_si128( vg.i, 8 );
    vg.f = _mm_movelh_ps( vg.f, vh.f );
    vh.d = _mm_cvtsi32_sd( vh.d, value );
    vh.i = _mm_shuffle_epi32( vh.i, _MM_SHUFFLE( 1, 0, 3, 2 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_3R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3 )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    XMM128 & ve = reinterpret_cast<XMM128&>(e);
    XMM128 & vf = reinterpret_cast<XMM128&>(f);
    XMM128 & vg = reinterpret_cast<XMM128&>(g);
    XMM128 & vh = reinterpret_cast<XMM128&>(h);
    va.f = _mm_movehl_ps( va.f, vb.f );
    va.f = _mm_movelh_ps( va.f, vc.f );
    vb.f = _mm_movehl_ps( vb.f, vc.f );
    vb.f = _mm_movelh_ps( vb.f, vd.f );
    vc.f = _mm_movehl_ps( vc.f, vd.f );
    vc.f = _mm_movelh_ps( vc.f, ve.f );
    vd.f = _mm_movehl_ps( vd.f, ve.f );
    vd.f = _mm_movelh_ps( vd.f, vf.f );
    ve.f = _mm_movehl_ps( ve.f, vf.f );
    ve.f = _mm_movelh_ps( ve.f, vg.f );
    vf.f = _mm_movehl_ps( vf.f, vg.f );
    vf.f = _mm_movelh_ps( vf.f, vh.f );
    g = _mm_cvtsi32_sd( g, value1 );
    vg.i = _mm_slli_si128( vg.i, 8 );
    vg.f = _mm_movehl_ps( vg.f, vh.f );
//    vg.i = _mm_shuffle_epi32( vg.i, _MM_SHUFFLE( 1, 0, 3, 2 ) );
    h = _mm_cvtsi32_sd( h, value3 );
    vh.i = _mm_slli_si128( vh.i, 8 );
    h = _mm_cvtsi32_sd( h, value2 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_4R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3, S32 value4 )
{
    XMM128 & vg = reinterpret_cast<XMM128&>(g);
    XMM128 & vh = reinterpret_cast<XMM128&>(h);
    a = c;
    b = d;
    c = e;
    d = f;
    e = g;
    f = h;
    g = _mm_cvtsi32_sd( g, value2 );
    vg.i = _mm_slli_si128( vg.i, 8 );
    g = _mm_cvtsi32_sd( g, value1 );
    h = _mm_cvtsi32_sd( h, value4 );
    vh.i = _mm_slli_si128( vh.i, 8 );
    h = _mm_cvtsi32_sd( h, value3 );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 tmp;
    tmp.i = _mm_srli_si128( value, 12 );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp.f );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 tmp;
    tmp.i = _mm_srli_si128( value, 12 );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp.f );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    XMM128 tmp;
    tmp.i = _mm_srli_si128( value, 12 );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vd.f = _mm_move_ss( vd.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp.f );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    const XMM128 & vv = reinterpret_cast<const XMM128&>(value);
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vv.f );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    const XMM128 & vv = reinterpret_cast<const XMM128&>(value);
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vv.f );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    const XMM128 & vv = reinterpret_cast<const XMM128&>(value);
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vd.f );
    vd.f = _mm_move_ss( vd.f, vv.f );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 2, 1, 0, 3 ) );
    vd.f = _mm_move_ss( vd.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vb.f );
    vb.f = _mm_move_ss( vb.f, va.f );
    va.f = _mm_move_ss( va.f, tmp );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, tmp );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, tmp );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

CBL_INLINE void CBL_SSE2::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue )
{
    XMM128 & va = reinterpret_cast<XMM128&>(a);
    XMM128 & vb = reinterpret_cast<XMM128&>(b);
    XMM128 & vc = reinterpret_cast<XMM128&>(c);
    XMM128 & vd = reinterpret_cast<XMM128&>(d);
    __m128 tmp = _mm_load_ss( reinterpret_cast<F32*>(const_cast<S32*>(pValue)) );
    va.f = _mm_move_ss( va.f, vb.f );
    vb.f = _mm_move_ss( vb.f, vc.f );
    vc.f = _mm_move_ss( vc.f, vd.f );
    vd.f = _mm_move_ss( vd.f, tmp );
    d = _mm_shuffle_epi32( d, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    c = _mm_shuffle_epi32( c, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    b = _mm_shuffle_epi32( b, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
    a = _mm_shuffle_epi32( a, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
}

//! Extract bit field from register.  Source index and length are encoded in second parameter.
CBL_INLINE void  CBL_SSE2::ExtractField( __m128i & srcdst, const __m128i & param )
/*!
Extracts a bit field at specified index (0-63) of specified length (0-63)
from the srcdst register and stores it at bit offset 0.  All remaining bits will be set to zero.
A length value of 0 will be interpreted as bit filed of 64 bits.
The index is encoded in bits 13:8 of the param register and the length is encoded in bits 5:0 of
the param register.
*/
{
    const XMM128 & len_ind = reinterpret_cast<const XMM128&>( param );

	int length = len_ind.s8[0];
	int index = len_ind.s8[1];

	__m128i dst;

	dst = _mm_srli_epi64( srcdst,(S32)index);
	dst = _mm_slli_si128(dst,8);
	int r_index = 64 - index;	
	srcdst = _mm_slli_epi64(srcdst,r_index);

	srcdst = _mm_or_si128(srcdst,dst);
	srcdst = _mm_srli_si128(srcdst,8);

	int r_length = 64 - length;

	srcdst = _mm_slli_epi64(srcdst,r_length);
	srcdst = _mm_srli_epi64(srcdst,r_length);		
}

//! Extract bit field from register.  Source index and length are immediate cosntants.
template< U8 index, U8 length >
CBL_INLINE void  CBL_SSE2::ExtractField( __m128i & srcdst )
/*!
Extracts a bit field at specified index (0-63) of specified length (0-63)
from the srcdst register and stores it at bit offset 0.  All remaining bits will be set to zero.
A length value of 0 will be interpreted as bit filed of 64 bits.
The index and length parameters are passed as template arguments because in the future versions
of the library they might be required to be compile time constants.
*/
{

	__m128i dst;

	dst = _mm_srli_epi64( srcdst,(S32)index);
	dst = _mm_slli_si128(dst,8);
	int r_index = 64 - index;	
	srcdst = _mm_slli_epi64(srcdst,r_index);

	srcdst = _mm_or_si128(srcdst,dst);
	srcdst = _mm_srli_si128(srcdst,8);

	int r_length = 64 - length;

	srcdst = _mm_slli_epi64(srcdst,r_length);
	srcdst = _mm_srli_epi64(srcdst,r_length);		
}


//! Insert a bit field into register.  Destination index and length are encoded in second register.
CBL_INLINE void  CBL_SSE2::InsertField( __m128i & dst, const __m128i & src )
/*!
Inserts bit field of specified length from src register at bit index 0 into the dst register at 
the specified index.
The index must be a value in the 0-63 range and is encoded in the bits 77:72 of the src register.
The length must be a value in the 0-63 range and is encoded in the bits 64:69 of the src register.  
    Length value of 0 will be interpreted as bit filed of 64 bits.
The remaining bits in dst register are not modified.
*/
{

    const XMM128 & len_ind = reinterpret_cast<const XMM128&>( src );

	int length = len_ind.s8[8];
	int index = len_ind.s8[9];

	__m128i dst1,srcdst;

	dst1 = _mm_srli_epi64( src,(S32)index);
	dst1 = _mm_slli_si128(dst1,8);
	index = 64 - index;	
	srcdst = _mm_slli_epi64(src,index);

	srcdst = _mm_or_si128(srcdst,dst1);
	srcdst = _mm_srli_si128(srcdst,8);

	int r_length = 64 - length;

	srcdst = _mm_slli_epi64(srcdst,r_length);
	srcdst = _mm_srli_epi64(srcdst,r_length);		

	dst1 = _mm_srli_epi64(dst,length);
	dst1 = _mm_slli_epi64(dst1,length);
	
	dst1 = _mm_or_si128(dst1,srcdst);

	dst1 = _mm_slli_si128(dst1,8);
	dst = _mm_unpackhi_epi64(dst1,dst);

}

//! Insert a bit field into register.  Destination index and length are immediate constants.
template< U8 index, U8 length >
CBL_INLINE void  CBL_SSE2::InsertField( __m128i & dst, const __m128i & src )
/*!
Inserts bit field of specified length from src register at bit index 0 into the dst register at 
the specified index.
The index must be a value in the 0-63 range.
The length must be a value in the 0-63 range.  Length of 0 will be interpreted as bit filed of 64 bits.
The remaining bits in dst register are not modified.
*/
{
	__m128i dst1,srcdst;

	dst1 = _mm_srli_epi64( src,(S32)index);
	dst1 = _mm_slli_si128(dst1,8);
	int r_index = 64 - index;	
	srcdst = _mm_slli_epi64(src,r_index);

	srcdst = _mm_or_si128(srcdst,dst1);
	srcdst = _mm_srli_si128(srcdst,8);

	int r_length = 64 - length;

	srcdst = _mm_slli_epi64(srcdst,r_length);
	srcdst = _mm_srli_epi64(srcdst,r_length);		

	dst1 = _mm_srli_epi64(dst,length);
	dst1 = _mm_slli_epi64(dst1,length);
	
	dst1 = _mm_or_si128(dst1,srcdst);

	dst1 = _mm_slli_si128(dst1,8);
	dst = _mm_unpackhi_epi64(dst1,dst);

}

/// @endcond

#endif // __CBL_LOGICAL_PRIMITIVES_SSE2_H__
