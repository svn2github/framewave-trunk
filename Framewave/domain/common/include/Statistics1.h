/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __STATISTICS1_H__
#define __STATISTICS1_H__

#include "fe.h"

namespace OPT_LEVEL
{

template< class TS, class TD >
SYS_INLINE STATIC void MaxEveryC1(const TS * s, TD * d)
{
    *d = (TD) (*s>*d?*s:*d);
}

template< class TS, class TD >
SYS_INLINE STATIC void MinEveryC1(const TS * s, TD * d)
{
    *d = (TD) (*s<*d?*s:*d);
}



SYS_INLINE STATIC void MaxEvery_sse2_16s(const __m128i &src,__m128i &dst)
{
    dst = _mm_max_epi16(src,dst);
}

SYS_INLINE STATIC void MaxEvery_sse2_32s(const __m128i &src,__m128i &dst)
{
    XMM128 inter_res, com_inter;
    inter_res.i = _mm_cmpgt_epi32 (src,dst);
    com_inter.i = _mm_andnot_si128(inter_res.i,dst);
    inter_res.i = _mm_and_si128(src, inter_res.i);
    dst         = _mm_or_si128(inter_res.i,com_inter.i);
}

SYS_INLINE STATIC void MaxEvery_sse2_32f(const __m128 &src,__m128 &dst)
{
    dst = _mm_max_ps(src,dst);
}

SYS_INLINE STATIC void MinEvery_sse2_16s(const __m128i &src,__m128i &dst)
{
    dst = _mm_min_epi16(src,dst);
}

SYS_INLINE STATIC void MinEvery_sse2_32s(const __m128i &src,__m128i &dst)
{
    XMM128 inter_res, com_inter;
    inter_res.i = _mm_cmplt_epi32 (src,dst);
    com_inter.i = _mm_andnot_si128(inter_res.i,dst);
    inter_res.i = _mm_and_si128(src, inter_res.i);
    dst         = _mm_or_si128(inter_res.i,com_inter.i);
}

SYS_INLINE STATIC void MinEvery_sse2_32f(const __m128 &src,__m128 &dst)
{
    dst = _mm_min_ps(src,dst);
}


} // namespace OPT_LEVEL

#endif // __STATISTICS1_H__

