/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_SHUFFLE_PRIMITIVES_SSE2_H__
#define __CBL_SHUFFLE_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS
// ============================= CBL shuffle functions =============================
//	Please note:	a1a0 means that in the output vector, the "lower" bits are a0 
//					and "higher" bits are a1. Hence as an example,
//
//					Shuffle_b0a1_64f
//					will result in,
//					Output (high 64bits) = b (low 64 bits)
//					Output (low 64bits)  = a (high 64 bits)
//
//					When a and b are the same register, the register is effectively
//					swapped around a 64 bit pivot
//					
//
//					All the shuffle functions will follow the same convention
// =================================================================================

CBL_INLINE __m128 CBL_SSE2::Shuffle_b3a1b2a0_32f(const __m128 & a, const __m128 & b)
{	
    return _mm_unpacklo_ps(a, _mm_movehl_ps(b, b));
}

CBL_INLINE __m128 CBL_SSE2::Shuffle_b3b1a2a0_32f(const __m128 & a, const __m128 & b)
{	
    return _mm_shuffle_ps( a, b, _MM_SHUFFLE(3,1,2,0) );
}

CBL_INLINE __m128 CBL_SSE2::Shuffle_b1b0a1a0_32f(const __m128 & a, const __m128 & b)
{	
    return _mm_movelh_ps(a,b);
}

CBL_INLINE __m128 CBL_SSE2::Shuffle_a3a2b3b2_32f(const __m128 & a, const __m128 & b)
{	
    return _mm_movehl_ps(a,b);
}


CBL_INLINE __m128d CBL_SSE2::Shuffle_b0a0_64f(const __m128d & a, const __m128d & b)
{	
    return _mm_unpacklo_pd(a, b);
}

CBL_INLINE __m128d CBL_SSE2::Shuffle_b1a1_64f(const __m128d & a, const __m128d & b)
{	
    return	  _mm_unpackhi_pd(a, b);
}

CBL_INLINE __m128d CBL_SSE2::Shuffle_b1a0_64f(const __m128d & a, const __m128d & b)
{	
    return	  _mm_move_sd(b, a);
}

CBL_INLINE __m128d CBL_SSE2::Shuffle_a1b0_64f(const __m128d & a, const __m128d & b)
{	
    return	  _mm_move_sd(a, b);
}

CBL_INLINE __m128d CBL_SSE2::Shuffle_a1a1_64f(const __m128d & a)
{
   /* XMM128 t;
    const XMM128 & va = reinterpret_cast<const XMM128 &>(a);
	t.f = _mm_movehl_ps(va.f, va.f);
	return t.d;*/
	return _mm_unpackhi_pd(a, a);
}

CBL_INLINE __m128d CBL_SSE2::Shuffle_b0a1_64f(const __m128d & a, const __m128d & b)
{	
    return _mm_unpacklo_pd(Shuffle_a1a1_64f(a), b); 
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_b0a0_64s(const __m128i & a, const __m128i & b)
{	
    return _mm_unpacklo_epi64(a, b);
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_b1a1_64s(const __m128i & a, const __m128i & b)
{	
    return _mm_unpackhi_epi64(a, b);
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_b1a0_64s(const __m128i & a, const __m128i & b)
{	
    return	  _mm_unpacklo_epi64(a, _mm_srli_si128(b, 8));
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_b0a1_64s(const __m128i & a, const __m128i & b)
{	
    return	  _mm_unpacklo_epi64(_mm_srli_si128(a, 8), b);
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_3201_32s(const __m128i & a) 
{ 
    return _mm_shufflelo_epi16(a, _MM_SHUFFLE(1, 0, 3, 2));
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_2310_32s(const __m128i & a) 
{ 
    return _mm_shufflehi_epi16(a, _MM_SHUFFLE(1, 0, 3, 2));
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_2301_32s(const __m128i & a) 
{ 
    __m128i temp = _mm_shufflehi_epi16(a, _MM_SHUFFLE(1, 0, 3, 2));
    return _mm_shufflelo_epi16(temp, _MM_SHUFFLE(1, 0, 3, 2));
}

CBL_INLINE __m128i CBL_SSE2::Shuffle_3120_32s(const __m128i & a) 
{ 
    return _mm_unpacklo_epi32(a, _mm_srli_si128(a, 8));
}

/// @endcond

#endif // __CBL_SHUFFLE_PRIMITIVES_SSE2_H__
