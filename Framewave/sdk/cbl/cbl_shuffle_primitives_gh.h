/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_SHUFFLE_PRIMITIVES_GH_H__
#define __CBL_SHUFFLE_PRIMITIVES_GH_H__

/// @cond OPT_DETAILS

CBL_INLINE __m128 CBL_GH::Shuffle_b3a1b2a0_32f( const __m128 & a, const __m128 & b)
/*!
    result.f32[0] = a.f32[0];
    result.f32[1] = b.f32[2];
    result.f32[2] = a.f32[1];
    result.f32[3] = b.f32[3];
*/
{
    return _mm_unpacklo_ps(a, _mm_movehl_ps(b, b));
}

CBL_INLINE __m128 CBL_GH::Shuffle_b3b1a2a0_32f( const __m128 & a, const __m128 & b)
/*!
    result.f32[0] = a.f32[0];
    result.f32[1] = a.f32[2];
    result.f32[2] = b.f32[1];
    result.f32[3] = b.f32[3];
*/
{
    return _mm_shuffle_ps( a, b, _MM_SHUFFLE(3, 1, 2, 0) );
}

CBL_INLINE __m128d CBL_GH::Shuffle_b0a0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[0];
    result.f64[1] = b.f64[0];
*/
{
    return _mm_shuffle_pd( a, b, _MM_SHUFFLE2(0, 0) );
}

CBL_INLINE __m128d CBL_GH::Shuffle_b1a1_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = b.f64[1];
*/
{
    return _mm_shuffle_pd( a, b, _MM_SHUFFLE2(1, 1) );
}

CBL_INLINE __m128d CBL_GH::Shuffle_b1a0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[0];
    result.f64[1] = b.f64[1];
*/
{
    return _mm_shuffle_pd( a, b, _MM_SHUFFLE2(1, 0) );
}

CBL_INLINE __m128d CBL_GH::Shuffle_a1b0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = b.f64[0];
    result.f64[1] = a.f64[1];
*/
{
    return _mm_shuffle_pd( b, a, _MM_SHUFFLE2(1, 0) );
}

CBL_INLINE __m128d CBL_GH::Shuffle_a1a1_64f( const __m128d & a)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = a.f64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.i = _mm_shuffle_epi32( va.i, _MM_SHUFFLE(3, 2, 3, 2) );
    return result.d;
}

CBL_INLINE __m128d CBL_GH::Shuffle_b0a1_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = b.f64[0];
*/
{
    return _mm_shuffle_pd( a, b, _MM_SHUFFLE2(0, 1) );
}

CBL_INLINE __m128i CBL_GH::Shuffle_b0a0_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[0];
    result.s64[1] = b.s64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.d = _mm_shuffle_pd( va.d, vb.d, _MM_SHUFFLE2(0, 0) );
    return result.i;
}

CBL_INLINE __m128i CBL_GH::Shuffle_b1a1_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[1];
    result.s64[1] = b.s64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.d = _mm_shuffle_pd( va.d, vb.d, _MM_SHUFFLE2(1, 1) );
    return result.i;
}

CBL_INLINE __m128i CBL_GH::Shuffle_b1a0_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[0];
    result.s64[1] = b.s64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.d = _mm_shuffle_pd( va.d, vb.d, _MM_SHUFFLE2(1, 0) );
    return result.i;
}

CBL_INLINE __m128i CBL_GH::Shuffle_b0a1_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[1];
    result.s64[1] = b.s64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.d = _mm_shuffle_pd( va.d, vb.d, _MM_SHUFFLE2(0, 1) );
    return result.i;
}

CBL_INLINE __m128i CBL_GH::Shuffle_3201_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[1];
    result.s32[1] = a.s32[0];
    result.s32[2] = a.s32[2];
    result.s32[3] = a.s32[3];
*/
{
    return _mm_shuffle_epi32( a, _MM_SHUFFLE(3, 2, 0, 1) );
}

CBL_INLINE __m128i CBL_GH::Shuffle_2310_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[0];
    result.s32[1] = a.s32[1];
    result.s32[2] = a.s32[3];
    result.s32[3] = a.s32[2];
*/
{
    return _mm_shuffle_epi32( a, _MM_SHUFFLE(2, 3, 1, 0) );
}

CBL_INLINE __m128i CBL_GH::Shuffle_2301_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[1];
    result.s32[1] = a.s32[0];
    result.s32[2] = a.s32[3];
    result.s32[3] = a.s32[2];
*/
{
    return _mm_shuffle_epi32( a, _MM_SHUFFLE(2, 3, 0, 1) );
}

CBL_INLINE __m128i CBL_GH::Shuffle_3120_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[0];
    result.s32[1] = a.s32[2];
    result.s32[2] = a.s32[1];
    result.s32[3] = a.s32[3];
*/
{
    return _mm_shuffle_epi32( a, _MM_SHUFFLE(3, 1, 2, 0) );
}

CBL_INLINE __m128 CBL_GH::Shuffle_b1b0a1a0_32f(const __m128 & a, const __m128 & b)
{	
    return  _mm_shuffle_ps(a,b,_MM_SHUFFLE(1, 0, 1, 0) );
}

CBL_INLINE __m128 CBL_GH::Shuffle_a3a2b3b2_32f(const __m128 & a, const __m128 & b)
{	
    return _mm_shuffle_ps(b,a, _MM_SHUFFLE(3, 2, 3, 2) );
}

/// @endcond

#endif // __CBL_SHUFFLE_PRIMITIVES_GH_H__
