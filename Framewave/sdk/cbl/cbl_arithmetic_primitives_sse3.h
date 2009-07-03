/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_ARITHMETIC_PRIMITIVES_SSE3_H__
#define __CBL_ARITHMETIC_PRIMITIVES_SSE3_H__

/// @cond OPT_DETAILS
// horizontal add and subtract routines

CBL_INLINE __m128 CBL_SSE3::HorizontalAdd_32f( const __m128 & src1, const __m128 & src2 )
{
    return _mm_hadd_ps( src1, src2 );
}

CBL_INLINE __m128 CBL_SSE3::HorizontalSubtract_32f( const __m128 & src1, const __m128 & src2 )
{
    return _mm_hsub_ps( src1, src2 );
}

CBL_INLINE __m128 CBL_SSE3::AddAndSubtract_32f( const __m128 & src1, const __m128 & src2 )
{
    return _mm_addsub_ps( src1, src2 );
}

CBL_INLINE __m128d CBL_SSE3::HorizontalAdd_64f( const __m128d & src1, const __m128d & src2 )
{
    return _mm_hadd_pd( src1, src2 );
}

CBL_INLINE __m128d CBL_SSE3::HorizontalSubtract_64f( const __m128d & src1, const __m128d & src2 )
{
    return _mm_hsub_pd( src1, src2 );
}

CBL_INLINE __m128d CBL_SSE3::AddAndSubtract_64f( const __m128d & src1, const __m128d & src2 )
{
    return _mm_addsub_pd( src1, src2 );
}
/// @endcond

#endif // __CBL_ARITHMETIC_PRIMITIVES_SSE3_H__
