/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_SUPPORT_PRIMITIVES_SSE2_H__
#define __CBL_SUPPORT_PRIMITIVES_SSE2_H__

struct CBL_AlignedLoad
{
    CBL_INLINE static void Load( __m128i & r, const __m128i *p )
    {
        r = _mm_load_si128( p );
    }

    CBL_INLINE static void Load( __m128d & r, const double *p )
    {
        r = _mm_load_pd( p );
    }

    CBL_INLINE static void Load( __m128 & r, const float *p )
    {
        r = _mm_load_ps( p );
    }
};

struct CBL_AlignedStore
{
    CBL_INLINE static void Store( __m128i *p, __m128i a )
    {
        _mm_store_si128( p, a );
    }

    CBL_INLINE static void Store( double *p, __m128d a )
    {
        _mm_store_pd( p, a );
    }

    CBL_INLINE static void Store( float *p, __m128 a )
    {
        _mm_store_ps( p, a );
    }
};

struct CBL_UnalignedLoad
{
    CBL_INLINE static void Load( __m128i & r, const __m128i *p )
    {
        r = _mm_loadu_si128( p );
    }

    CBL_INLINE static void Load( __m128d & r, const double *p )
    {
        r = _mm_loadu_pd( p );
    }

    CBL_INLINE static void Load( __m128 & r, const float *p )
    {
        r = _mm_loadu_ps( p );
    }
};

struct CBL_UnalignedStore
{
    CBL_INLINE static void Store( __m128i *p, __m128i a )
    {
        _mm_storeu_si128( p, a );
    }

    CBL_INLINE static void Store( double *p, __m128d a )
    {
        _mm_storeu_pd( p, a );
    }

    CBL_INLINE static void Store( float *p, __m128 a )
    {
        _mm_storeu_ps( p, a );
    }
};

struct CBL_StreamingStore
{
    CBL_INLINE static void Store( __m128i *p, __m128i a )
    {
        _mm_stream_si128( p, a );
    }

    CBL_INLINE static void Store( double *p, __m128d a )
    {
        _mm_stream_pd( p, a );
    }

    CBL_INLINE static void Store( float *p, __m128 a )
    {
        _mm_stream_ps( p, a );
    }
};

#endif // __CBL_ARITHMETIC_PRIMITIVES_SSE2_H__


