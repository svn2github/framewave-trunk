/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SHUFFLE_H__
#define __SHUFFLE_H__

#include "fwBase.h"
#include "system.h"
#include "shortcuts.h"

namespace OPT_LEVEL
{

// ============================= _mm shuffle functions =============================
// Please note: a0a1 means the "lower" bits are a0 and "higher" bits are a1
//              The ordering though, for convenience has been kept from right
//              to left, right meaning the higher bit positions and left meaning
//              the lower bit positions
//
//              All the shuffle functions will follow the same convention
// =================================================================================

// --- <pd> ---
SYS_INLINE STATIC __m128d _mm_shufb0a0_pd(const __m128d &a, const __m128d &b)
{
    return _mm_unpacklo_pd(a, b);
}
SYS_INLINE STATIC __m128d _mm_shufb1a1_pd(const __m128d &a, const __m128d &b)
{
    return _mm_unpackhi_pd(a, b);
}
SYS_INLINE STATIC __m128d _mm_shufb1a0_pd(const __m128d &a, const __m128d &b)
{
    return CBL_OPT::Shuffle_b1a0_64f(a, b);
}
SYS_INLINE STATIC __m128d _mm_shufa1b0_pd(const __m128d &a, const __m128d &b)
{
    return CBL_OPT::Shuffle_a1b0_64f(a, b);
}

SYS_INLINE STATIC __m128d _mm_shufb0a1_pd(const __m128d &a, const __m128d &b)
{
//    XMM128 ai; ai.d = a;
    __m128d ai;
    ai   = _mm_unpackhi_pd(a,a);        // move a1 to a0
    return _mm_unpacklo_pd(ai, b);
}

// --- </pd> ---

// --- <ps> ---
//SYS_INLINE STATIC __m128 _mm_shuf1302_ps(__m128 a) { return _mm_unpacklo_ps(_mm_srli_si128(a, 8), a); }
//SYS_INLINE STATIC __m128 _mm_shuf3120_ps(__m128 a) { return _mm_unpacklo_ps(a, _mm_srli_si128(a, 8)); }
// --- </ps> ---


// --- <epi64> ---
SYS_INLINE STATIC __m128i _mm_shufb0a0_epi64( const __m128i &a, const __m128i &b)
{
    return _mm_unpacklo_epi64(a, b);
}
SYS_INLINE STATIC __m128i _mm_shufb1a1_epi64( const __m128i &a, const __m128i &b)
{
    return _mm_unpackhi_epi64(a, b);
}
SYS_INLINE STATIC __m128i _mm_shufb1a0_epi64( const __m128i &a, const __m128i &b)
{
    return _mm_unpacklo_epi64(a, _mm_srli_si128(b, 8));
}
SYS_INLINE STATIC __m128i _mm_shufb0a1_epi64( const __m128i &a, const  __m128i &b)
{
    return _mm_unpacklo_epi64(_mm_srli_si128(a, 8), b);
}
// --- </epi64> ---



// --- <epi32> ---
SYS_INLINE STATIC __m128i _mm_shuf3201_epi32(const __m128i &a) { return         _mm_shufflehi_epi16(a ,  _MM_SHUFFLE(1, 0, 3, 2)); }
SYS_INLINE STATIC __m128i _mm_shuf2310_epi32(const __m128i &a) { return         _mm_shufflelo_epi16(a ,  _MM_SHUFFLE(1, 0, 3, 2)); }
SYS_INLINE STATIC __m128i _mm_shuf2301_epi32(const __m128i &a) { __m128i temp = _mm_shufflehi_epi16(a ,  _MM_SHUFFLE(1, 0, 3, 2));
                                                                 return         _mm_shufflelo_epi16(temp,_MM_SHUFFLE(1, 0, 3, 2)); }
SYS_INLINE STATIC __m128i _mm_shuf3120_epi32(const __m128i &a) { return         _mm_unpacklo_epi32 (a, _mm_srli_si128(a, 8));  }
// --- </epi32> ---


// --- <epi16> ---
SYS_INLINE STATIC __m128i _mm_rlli_epi16(const __m128i &reg, const  int &by)
{
    __m128i temp2;
    __m128i temp    = _mm_srli_epi16(reg, 16 - by);
            temp2   = _mm_slli_epi16(reg, by);
    return            _mm_or_si128(temp2, temp);
}

SYS_INLINE STATIC __m128i _mm_rrli_epi16(const __m128i &reg, const  int &by)
{
    __m128i temp2;
    __m128i temp    = _mm_slli_epi16(reg, 16 - by);
            temp2   = _mm_srli_epi16(reg, by);
    return            _mm_or_si128(temp2, temp);
}

// --- </epi16> ---


// --- <epi8> ---
SYS_INLINE STATIC __m128i _mm_rlli_epi8(const __m128i &reg, const  int &by)
{
    static const __m128i maskl = CONST_SET1_32I((long) 0xFF00FF00);
    static const __m128i maskr = CONST_SET1_32I((long) 0x00FF00FF);

    __m128i left     = _mm_slli_epi16(reg, by);
    __m128i tmaskl   = _mm_srli_epi16(_mm_slli_epi16(maskl, 8 - by), 8 - by);
            left     = _mm_andnot_si128(tmaskl, left);

    __m128i right    = _mm_srli_epi16(reg, 8 - by);
    __m128i tmaskr   = _mm_slli_epi16(_mm_srli_epi16(maskr, by), by);
            right    = _mm_andnot_si128(tmaskr, right);
    
    return _mm_or_si128(left, right);
}

SYS_INLINE STATIC __m128i _mm_rrli_epi8(const __m128i &reg, const  int &by)
{
    static const __m128i maskl = CONST_SET1_32I((long) 0xFF00FF00);
    static const __m128i maskr = CONST_SET1_32I((long) 0x00FF00FF);

    __m128i left     = _mm_slli_epi16(reg, 8 - by);
    __m128i tmaskl   = _mm_srli_epi16(_mm_slli_epi16(maskl, by), by);
            left     = _mm_andnot_si128(tmaskl, left);

    __m128i  right   = _mm_srli_epi16(reg, by);
    __m128i tmaskr   = _mm_slli_epi16(_mm_srli_epi16(maskr, 8 - by), 8 - by);
            right    = _mm_andnot_si128(tmaskr, right);
    
    return _mm_or_si128(left, right);
}
// --- </epi8> ---


} // namespace OPT_LEVEL

#endif //    __SHUFFLE_H__



