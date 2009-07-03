/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_SHUFFLE_PRIMITIVES_H__
#define __CBL_SHUFFLE_PRIMITIVES_H__

CBL_INLINE __m128 CBL_REF::Shuffle_b3a1b2a0_32f( const __m128 & a, const __m128 & b)
/*!
    result.f32[0] = a.f32[0];
    result.f32[1] = b.f32[2];
    result.f32[2] = a.f32[1];
    result.f32[3] = b.f32[3];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f32[0] = va.f32[0];
    result.f32[1] = vb.f32[2];
    result.f32[2] = va.f32[1];
    result.f32[3] = vb.f32[3];
    return result.f;
}

CBL_INLINE __m128 CBL_REF::Shuffle_b3b1a2a0_32f( const __m128 & a, const __m128 & b)
/*!
    result.f32[0] = a.f32[0];
    result.f32[1] = a.f32[2];
    result.f32[2] = b.f32[1];
    result.f32[3] = b.f32[3];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f32[0] = va.f32[0];
    result.f32[1] = va.f32[2];
    result.f32[2] = vb.f32[1];
    result.f32[3] = vb.f32[3];
    return result.f;
}

CBL_INLINE __m128d CBL_REF::Shuffle_b0a0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[0];
    result.f64[1] = b.f64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f64[0] = va.f64[0];
    result.f64[1] = vb.f64[0];
    return result.d;
}

CBL_INLINE __m128d CBL_REF::Shuffle_b1a1_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = b.f64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f64[0] = va.f64[1];
    result.f64[1] = vb.f64[1];
    return result.d;
}

CBL_INLINE __m128d CBL_REF::Shuffle_b1a0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[0];
    result.f64[1] = b.f64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f64[0] = va.f64[0];
    result.f64[1] = vb.f64[1];
    return result.d;
}

CBL_INLINE __m128d CBL_REF::Shuffle_a1b0_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = b.f64[0];
    result.f64[1] = a.f64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f64[0] = vb.f64[0];
    result.f64[1] = va.f64[1];
    return result.d;
}

CBL_INLINE __m128d CBL_REF::Shuffle_a1a1_64f( const __m128d & a)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = a.f64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.f64[0] = va.f64[1];
    result.f64[1] = va.f64[1];
    return result.d;
}

CBL_INLINE __m128d CBL_REF::Shuffle_b0a1_64f( const __m128d & a, const __m128d & b)
/*!
    result.f64[0] = a.f64[1];
    result.f64[1] = b.f64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.f64[0] = va.f64[1];
    result.f64[1] = vb.f64[0];
    return result.d;
}

CBL_INLINE __m128i CBL_REF::Shuffle_b0a0_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[0];
    result.s64[1] = b.s64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.s64[0] = va.s64[0];
    result.s64[1] = vb.s64[0];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_b1a1_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[1];
    result.s64[1] = b.s64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.s64[0] = va.s64[1];
    result.s64[1] = vb.s64[1];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_b1a0_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[0];
    result.s64[1] = b.s64[1];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.s64[0] = va.s64[0];
    result.s64[1] = vb.s64[1];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_b0a1_64s( const __m128i & a, const __m128i & b)
/*!
    result.s64[0] = a.s64[1];
    result.s64[1] = b.s64[0];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;
    result.s64[0] = va.s64[1];
    result.s64[1] = vb.s64[0];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_3201_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[1];
    result.s32[1] = a.s32[0];
    result.s32[2] = a.s32[2];
    result.s32[3] = a.s32[3];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.s32[0] = va.s32[1];
    result.s32[1] = va.s32[0];
    result.s32[2] = va.s32[2];
    result.s32[3] = va.s32[3];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_2310_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[0];
    result.s32[1] = a.s32[1];
    result.s32[2] = a.s32[3];
    result.s32[3] = a.s32[2];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.s32[0] = va.s32[0];
    result.s32[1] = va.s32[1];
    result.s32[2] = va.s32[3];
    result.s32[3] = va.s32[2];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_2301_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[1];
    result.s32[1] = a.s32[0];
    result.s32[2] = a.s32[3];
    result.s32[3] = a.s32[2];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.s32[0] = va.s32[1];
    result.s32[1] = va.s32[0];
    result.s32[2] = va.s32[3];
    result.s32[3] = va.s32[2];
    return result.i;
}

CBL_INLINE __m128i CBL_REF::Shuffle_3120_32s( const __m128i & a)
/*!
    result.s32[0] = a.s32[0];
    result.s32[1] = a.s32[2];
    result.s32[2] = a.s32[1];
    result.s32[3] = a.s32[3];
*/
{
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    XMM128 result;
    result.s32[0] = va.s32[0];
    result.s32[1] = va.s32[2];
    result.s32[2] = va.s32[1];
    result.s32[3] = va.s32[3];
    return result.i;
}


CBL_INLINE __m128 CBL_REF::Shuffle_b1b0a1a0_32f(const __m128 & a, const __m128 & b)
{	
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;

    result.f32[0] = va.f32[0];
    result.f32[1] = va.f32[1];
    result.f32[2] = vb.f32[0];
    result.f32[3] = vb.f32[1];
    return result.f;
}

CBL_INLINE __m128 CBL_REF::Shuffle_a3a2b3b2_32f(const __m128 & a, const __m128 & b)
{	
    const XMM128 & va = reinterpret_cast<const XMM128 &>( a );
    const XMM128 & vb = reinterpret_cast<const XMM128 &>( b );
    XMM128 result;

    result.f32[0] = vb.f32[2];
    result.f32[1] = vb.f32[3];
    result.f32[2] = va.f32[2];
    result.f32[3] = va.f32[3];
    return result.f;
}


#endif // __CBL_SHUFFLE_PRIMITIVES_H__
