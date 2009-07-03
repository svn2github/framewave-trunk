/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_ARITHMETIC_PRIMITIVES_SSE2_H__
#define __CBL_ARITHMETIC_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS
template <int scaleType>
CBL_INLINE void CBL_SSE2::Setup_ScaleMasks_16s(const int &scale, __m128i scaleMasksp[])
{
	__m128i * scaleMasks = scaleMasksp;

	U16 mask1;//, mask2;
	switch (scaleType)
	{
	case 1:
		mask1 = 1 << ((U16) scale - 1);
		CBL_SSE2::Load_1x8_16u( scaleMasks[1], mask1 );

		scaleMasks[2]	= _mm_cmpeq_epi16(scaleMasks[1], scaleMasks[1]);	// all f's
		scaleMasks[2]	= _mm_srl_epi16(scaleMasks[2], _mm_set_epi32(0,0,0,16 - scale - 1));

		scaleMasks[0]	= _mm_set_epi32(0, 0, 0, scale - 1);

		break;
	case -1:
		scaleMasks[0]	= _mm_set_epi32(0, 0, 0, -scale);
		break;
	default:
		break;
	}
}

template <int scaleType>
CBL_INLINE void CBL_SSE2::Setup_ScaleMasks_32s(const int &scale, __m128i scaleMasksp[])
{
	__m128i * scaleMasks = scaleMasksp;

	U32 mask1;//, mask2;

	switch (scaleType)
	{
	case 1:
		mask1 = 1 << ((U32) scale - 1);

		CBL_SSE2::Load_1x4_32u( scaleMasks[1], mask1 );

		scaleMasks[2]	= _mm_cmpeq_epi32(scaleMasks[1], scaleMasks[1]);	// all f's
		scaleMasks[2]	= _mm_srl_epi32(scaleMasks[2], _mm_set_epi32(0,0,0,32 - scale - 1));

		scaleMasks[0]	= _mm_set_epi32(0, 0, 0, scale - 1);
		break;
	case -1:
		scaleMasks[0] = _mm_set_epi32(0, 0, 0, -scale);
		break;
	default:
		break;
	}
}

template <int scaleType>
CBL_INLINE void CBL_SSE2::Setup_ScaleMasks_32f(const int &scale, __m128 scaleMasks[])
{
	switch (scaleType)
	{
	case 1:
	case -1:
		scaleMasks[0] = _mm_set1_ps(pow((float)2.0, -scale));
		break;
	default:
		scaleMasks[0] = _mm_set1_ps(1.0);
		break;
	}
}

template <int scaleType>
CBL_INLINE __m128i CBL_SSE2::Scale_16s(const __m128i &value, const __m128i scaleMasksp[])
{
	// scaleMasks are,
	// scaleMasks[0]	= scale value
	// scaleMasks[1-2]	= scale masks used by the new integer scaling algorithm

	const __m128i * scaleMasks = scaleMasksp;
	__m128i mask, temp;

	switch (scaleType)
	{
	case 1:
		mask	= _mm_and_si128(value,  scaleMasks[2]);		// 111 mask (for scale 2)
		mask	= _mm_cmpgt_epi16(mask, scaleMasks[1]);		// 010		(for scale 2)

		temp	= _mm_srl_epi16(value, scaleMasks[0]);		// rsh by scale - 1
		temp	= _mm_sub_epi16(temp, mask);				// i.e., if mask true, then +1

		return	  _mm_srli_epi16(temp, 1);					// rsh one more to make total rsh by scale
	case -1:
		return	  _mm_sll_epi16(value, scaleMasks[0]);
	default:	// fall through to 0 scale otherwise
		return value;
	}
}

template <int scaleType>
CBL_INLINE __m128i CBL_SSE2::Scale_32s(const __m128i &value, const __m128i scaleMasksp[])
{
	// scaleMasks are,
	// scaleMasks[0]	= scale value
	// scaleMasks[1-2]	= scale masks used by the new integer scaling algorithm

	const __m128i * scaleMasks = scaleMasksp;
	__m128i mask, temp;

	switch (scaleType)
	{
	case 1:
		mask	= _mm_and_si128(value,  scaleMasks[2]);		// 111 mask (for scale 2)
		mask	= _mm_cmpgt_epi16(mask, scaleMasks[1]);		// 010		(for scale 2)

		temp	= _mm_sra_epi16(value, scaleMasks[0]);		// rsh by scale - 1
		temp	= _mm_sub_epi16(temp, mask);				// i.e., if mask true, then +1

		return	  _mm_srai_epi16(temp, 1);					// rsh one more to make total rsh by scale
	case -1:
		return	  _mm_sll_epi32(value, scaleMasks[0]);
	default:	// fall through to 0 scale otherwise
		return value;
	}
}

template <int scaleType>
CBL_INLINE __m128 CBL_SSE2::Scale_32f(const __m128 &value, const __m128 scaleMasks[])
{
	switch (scaleType)
	{
	case 1:
	case -1:
		return _mm_mul_ps(value, scaleMasks[0]);
	default:	// fall through to 0 scale otherwise
		return value;
	}
}

//Compares the 16 signed 8-bit unsigned integers in src1 and the 16 signed 8-bit integers in src2 for less than
//If the condition is true the return value is set to 0xFF else 0
CBL_INLINE __m128i CBL_SSE2::compare_lt_8u( const __m128i & src1, const __m128i & src2 )
{
	return (_mm_xor_si128(_mm_cmpeq_epi8(_mm_max_epu8(src1, src2), src2), _mm_cmpeq_epi8(src1, src2)));
}

//Compares the 16 signed 8-bit unsigned integers in src1 and the 16 signed 8-bit integers in src2 for less than and equal
//If the condition is true the return value is set to 0xFF else 0
CBL_INLINE __m128i CBL_SSE2::compare_le_8u( const __m128i & src1, const __m128i & src2 )
{
	return (_mm_cmpeq_epi8(_mm_max_epu8(src1, src2), src2));
}

//Compares the 16 signed 8-bit unsigned integers in src1 and the 16 signed 8-bit integers in src2 for greater than
//If the condition is true the return value is set to 0xFF else 0
CBL_INLINE __m128i CBL_SSE2::compare_gt_8u( const __m128i & src1, const __m128i & src2 )
{
	return (_mm_xor_si128(_mm_cmpeq_epi8(_mm_min_epu8(src1, src2), src2), _mm_cmpeq_epi8(src1, src2)));
}

//Compares the 16 signed 8-bit unsigned integers in src1 and the 16 signed 8-bit integers in src2 for greater than and equal
//If the condition is true the return value is set to 0xFF else 0
CBL_INLINE __m128i CBL_SSE2::compare_ge_8u( const __m128i & src1, const __m128i & src2 )
{
	return (_mm_cmpeq_epi8(_mm_min_epu8(src1, src2), src2));
}


CBL_INLINE __m128i CBL_SSE2::Multiply_8u16s_Lo(__m128i &multiplicand, const __m128i &multiplier)
{
	__m128i multiplicandHi;

    CBL_SSE2::Unpack8UTo16S(multiplicand, multiplicandHi);

	// don't use the mul 16s function, since it will do both high and low multiplication
	// we need only low since MAX(8u) * MAX(8u) < MAX(16u)
	multiplicand	= _mm_mullo_epi16(multiplicand, multiplier);
	return			  _mm_mullo_epi16(multiplicandHi, multiplier);	// return the upper values
}

CBL_INLINE __m128i CBL_SSE2::Multiply_8u16s_Lo(__m128i &multiplicand, const __m128i &multiplierlo, const __m128i &multiplierhi)
{
	__m128i multiplicandHi;

	CBL_SSE2::Unpack8UTo16S(multiplicand, multiplicandHi);

	// don't use the mul 16s function, since it will do both high and low multiplication
	// we need only low since MAX(8u) * MAX(8u) < MAX(16u)
	multiplicand	= _mm_mullo_epi16(multiplicand, multiplierlo);
	return			  _mm_mullo_epi16(multiplicandHi, multiplierhi);	// return the upper values
}

CBL_INLINE __m128i CBL_SSE2::Multiply_8u_16s_Lo(__m128i &multiplicand, const __m128i &multiplier)
{
	__m128i multiplicandHi, multiplierlo = multiplier, multiplierHi;

	CBL_SSE2::Unpack8UTo16S(multiplicand, multiplicandHi);
	CBL_SSE2::Unpack8UTo16S(multiplierlo, multiplierHi);

	// don't use the mul 16s function, since it will do both high and low multiplication
	// we need only low since MAX(8u) * MAX(8u) < MAX(16u)
	multiplicand	= _mm_mullo_epi16(multiplicand, multiplierlo);
	return			  _mm_mullo_epi16(multiplicandHi, multiplierHi);	// return the upper values
}

CBL_INLINE __m128i CBL_SSE2::Multiply_16s_32s( __m128i & multiplicand, const __m128i & multiplier)
{
	__m128i temp1 = _mm_mullo_epi16(multiplicand, multiplier);
	__m128i temp2 = _mm_mulhi_epi16(multiplicand, multiplier);
	multiplicand  = _mm_unpacklo_epi16(temp1, temp2); 
	return		    _mm_unpackhi_epi16(temp1, temp2); 
}

CBL_INLINE __m128 CBL_SSE2::Multiply_32fc( const __m128 & multiplicand, const __m128 & multiplier)
{
	__m128	t1, t2, ts, imdt1, imdt2;

	t1	= _mm_mul_ps(multiplicand, multiplier);
	ts	= _mm_shuffle_ps(multiplier, multiplier, _MM_SHUFFLE(2, 3, 0, 1));
	t2	= _mm_mul_ps(multiplicand, ts);

	imdt1	= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(2, 0, 2, 0));
	imdt2	= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(3, 1, 3, 1));

	t1	= _mm_sub_ps(imdt1, imdt2);
	t2	= _mm_add_ps(imdt1, imdt2);

	return Shuffle_b3a1b2a0_32f(t1, t2);
}

CBL_INLINE __m128 CBL_SSE2::Multiply_32fc_wm( const __m128 & multiplicand, const __m128 & multiplier, const __m128 negamask)
{
	__m128 revcd = _mm_shuffle_ps(multiplier, multiplier, _MM_SHUFFLE(2, 3, 0, 1));

	__m128 a	= _mm_shuffle_ps(multiplicand, multiplicand, _MM_SHUFFLE(2, 2, 0, 0));	// a1 a0 a1 a0
	__m128 b	= _mm_shuffle_ps(multiplicand, multiplicand, _MM_SHUFFLE(3, 3, 1, 1));	// b1 b0 b1 b0

	__m128 t1	= _mm_mul_ps(multiplier, a);
	__m128 t2	= _mm_mul_ps(revcd, b);

	t2			= _mm_xor_ps(t2, negamask);			// bc -ad bc -ad

	return		  _mm_add_ps(t1, t2);
}

CBL_INLINE __m128d CBL_SSE2::Multiply_64fc( const __m128d & multiplicand, const __m128d & multiplier)
{
		__m128d temp1, temp2, b, a;
		temp1	= _mm_mul_pd(multiplicand, multiplier);

		temp2	= Shuffle_b0a1_64f(multiplier, multiplier);
		temp2	= _mm_mul_pd(multiplicand, temp2);

		a		= _mm_sub_sd(temp1, Shuffle_a1a1_64f(temp1));
		b		= _mm_add_sd(Shuffle_a1a1_64f(temp2), temp2);

		return _mm_unpacklo_pd(a, b);
}


CBL_INLINE __m128i CBL_SSE2::Multiply_32sc( const __m128i & multiplicand, const __m128i & multiplier)
{
	__m128d s1	= _mm_cvtepi32_pd(multiplicand);
	__m128d s2	= _mm_cvtepi32_pd(multiplier);

	__m128d r1	= CBL_SSE2::Multiply_64fc(s1, s2);

			s1	= _mm_cvtepi32_pd(_mm_srli_si128(multiplicand, 8));
			s2	= _mm_cvtepi32_pd(_mm_srli_si128(multiplier, 8));

	__m128d r2	= CBL_SSE2::Multiply_64fc(s1, s2);

    return CBL_SSE2::Pack64FTo32S(r1, r2);
}

CBL_INLINE __m128i CBL_SSE2::Multiply_16sc_32sc(__m128i & src1DstLo, const __m128i & src2, const __m128i & min32s, const __m128i & negb)
{
	__m128i rc, rd, s1, s2;
	// src1 = b a
	// src2 = d c

	// seperate d and c
	rc	= _mm_slli_epi32(src2, 16);
	rd	= _mm_srli_epi32(src2, 16);
	
	// negate d
	// can't do a sub(0 - src2) since -32768 will overflow and remain neg max
	// so multiply with neg(num + 1) == ~(num), i.e., -32768 becomes 32767
	// we'll just add b back at the end to take care of the difference
	s2	= _mm_xor_si128(src2, negb);

	// ac + b(-d)	- values for a
	s1	= _mm_madd_epi16(src1DstLo, s2);
	s1	= _mm_add_epi32(s1, _mm_srai_epi32(src1DstLo, 16));


	// fixup the only overflow value possible (-32768 * -32768 + -32768 * -32768)
	s1	= _mm_add_epi32(s1, _mm_cmpeq_epi32(s1, min32s));

	// reverse src2
	s2	= _mm_or_si128(rc, rd);

	// ad + bc		- values for b
	s2	= _mm_madd_epi16(src1DstLo, s2);

	// fixup overflow
	s2	= _mm_add_epi32(s2, _mm_cmpeq_epi32(s2, min32s));

	// interleave b's and a's
	src1DstLo	= _mm_unpacklo_epi32(s1, s2);	// set dst in lo
	return		  _mm_unpackhi_epi32(s1, s2);	// return hi
}


CBL_INLINE __m128i CBL_SSE2::Multiply_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s, const __m128i & negb)
{
	__m128i lo = src1, hi;
	hi = Multiply_16sc_32sc(lo, src2, min32s, negb);

	return	_mm_packs_epi32(lo, hi);
}


CBL_INLINE __m128 CBL_SSE2::Divide_32fc( const __m128 & divident, const __m128 & divisor)
{
	__m128	t1, t2, ts, imdt1, imdt2, denom;

	denom		= _mm_mul_ps(divisor, divisor);							// sqr(d) / sqr(c)
	denom		= _mm_add_ps(denom, _mm_shuffle_ps(denom, denom, _MM_SHUFFLE(2, 3, 0, 1)));
	__m128 cd	= _mm_div_ps(divisor, denom);

	t1			= _mm_mul_ps(divident, cd);
	ts			= _mm_shuffle_ps(cd, cd, _MM_SHUFFLE(2, 3, 0, 1));
	t2			= _mm_mul_ps(divident, ts);

	imdt1		= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(2, 0, 2, 0));
	imdt2		= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(3, 1, 3, 1));

	t1			= _mm_add_ps(imdt1, imdt2);
	t2			= _mm_sub_ps(imdt2, imdt1);

	return		  Shuffle_b3a1b2a0_32f(t1, t2);
}


CBL_INLINE __m128 CBL_SSE2::FastDivide_32fc( const __m128 & divident, const __m128 & divisor)
{
	__m128	t1, t2, ts, imdt1, imdt2, denom;

	denom		= _mm_mul_ps(divisor, divisor);							// sqr(d) / sqr(c)
	denom		= _mm_add_ps(denom, _mm_shuffle_ps(denom, denom, _MM_SHUFFLE(2, 3, 0, 1)));
	__m128 cd	= _mm_mul_ps(divisor, _mm_rcp_ps(denom));

	t1			= _mm_mul_ps(divident, cd);
	ts			= _mm_shuffle_ps(cd, cd, _MM_SHUFFLE(2, 3, 0, 1));
	t2			= _mm_mul_ps(divident, ts);

	imdt1		= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(2, 0, 2, 0));
	imdt2		= _mm_shuffle_ps(t1, t2, _MM_SHUFFLE(3, 1, 3, 1));

	t1			= _mm_add_ps(imdt1, imdt2);
	t2			= _mm_sub_ps(imdt2, imdt1);

	return		  Shuffle_b3a1b2a0_32f(t1, t2);
}


CBL_INLINE __m128 CBL_SSE2::Divide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask)
{
	__m128	denom	= _mm_mul_ps(divisor, divisor);							// sqr(d) / sqr(c)
	denom			= _mm_add_ps(denom, _mm_shuffle_ps(denom, denom, _MM_SHUFFLE(2, 3, 0, 1)));

	__m128 cd		= _mm_div_ps(divisor, denom);
	__m128 revcd	= _mm_shuffle_ps(cd, cd, _MM_SHUFFLE(2, 3, 0, 1));

	__m128 a		= _mm_shuffle_ps(divident, divident, _MM_SHUFFLE(2, 2, 0, 0));	// a1 a0 a1 a0
	__m128 b		= _mm_shuffle_ps(divident, divident, _MM_SHUFFLE(3, 3, 1, 1));	// b1 b0 b1 b0

	__m128 t1		= _mm_mul_ps(cd, a);
	__m128 t2		= _mm_mul_ps(revcd, b);
	t1				= _mm_xor_ps(t1, negbmask);			// bc -ad bc -ad

	return			  _mm_add_ps(t1, t2);
}


CBL_INLINE __m128 CBL_SSE2::FastDivide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask)
{
	__m128	denom	= _mm_mul_ps(divisor, divisor);							// sqr(d) / sqr(c)
	denom			= _mm_add_ps(denom, _mm_shuffle_ps(denom, denom, _MM_SHUFFLE(2, 3, 0, 1)));

	__m128 cd		= _mm_mul_ps(divisor, _mm_rcp_ps(denom));
	__m128 revcd	= _mm_shuffle_ps(cd, cd, _MM_SHUFFLE(2, 3, 0, 1));

	__m128 a		= _mm_shuffle_ps(divident, divident, _MM_SHUFFLE(2, 2, 0, 0));	// a1 a0 a1 a0
	__m128 b		= _mm_shuffle_ps(divident, divident, _MM_SHUFFLE(3, 3, 1, 1));	// b1 b0 b1 b0

	__m128 t1		= _mm_mul_ps(cd, a);
	__m128 t2		= _mm_mul_ps(revcd, b);
	t1				= _mm_xor_ps(t1, negbmask);			// bc -ad bc -ad

	return			  _mm_add_ps(t1, t2);
}


CBL_INLINE __m128d CBL_SSE2::Divide_64fc( const __m128d & divident, const __m128d & divisor, __m128d & zeromask )
{
	__m128d temp1, temp2, b, a, num, denom;
	temp1	= _mm_mul_pd(divident, divisor);

	temp2	= Shuffle_b0a1_64f(divisor, divisor);
	temp2	= _mm_mul_pd(divident, temp2);

	a		= _mm_add_sd(temp1, Shuffle_a1a1_64f(temp1));
	b		= _mm_sub_sd(Shuffle_a1a1_64f(temp2), temp2);

	num		= _mm_unpacklo_pd(a, b);
	denom	= _mm_mul_pd(divisor, divisor);
	denom	= _mm_add_sd(denom, Shuffle_a1a1_64f(denom));
	denom	= _mm_unpacklo_pd(denom, denom);

	zeromask	= _mm_cmpeq_sd(denom, _mm_setzero_pd());
	return	  _mm_div_pd(num, denom);
}


// Note: For the DivideC functions, divisorC	needs to be  (c + id)/(c^2 + d^2)
//									divisorCRev needs to be (-d + ic)/(c^2 + d^2)
CBL_INLINE __m128 CBL_SSE2::DivideC_32fc( const __m128 & divident, const __m128 & divisorC, const __m128 & divisorCRev)
{
		__m128 t1, t2, imdt1, imdt2;
		t1	= _mm_mul_ps(divident, divisorC);		//  bd |  ac
		t2	= _mm_mul_ps(divident, divisorCRev);	//  bc | -ad

		imdt1	= _mm_unpacklo_ps(t1, t2);
		imdt2	= _mm_unpackhi_ps(t1, t2);

		t1		= _mm_shuffle_ps(imdt1, imdt2, _MM_SHUFFLE(3, 2, 1, 0));
		t2		= _mm_shuffle_ps(imdt1, imdt2, _MM_SHUFFLE(1, 0, 3, 2));

		return	  _mm_add_ps(t1, t2);
}


CBL_INLINE __m128d CBL_SSE2::DivideC_64fc( const __m128d & divident, const __m128d & divisorC, const __m128d & divisorCRev)
{
		__m128d t1, t2, imdt1, imdt2;
		t1	= _mm_mul_pd(divident, divisorC);		//  bd |  ac
		t2	= _mm_mul_pd(divident, divisorCRev);	//  bc | -ad

		imdt1	= _mm_unpacklo_pd(t1, t2);
		imdt2	= _mm_unpackhi_pd(t1, t2);

		return	  _mm_add_pd(imdt1, imdt2);
}


CBL_INLINE __m128 CBL_SSE2::Divide_16sc_32fc(const __m128i & src1, const __m128i & src2, __m128 & destLo, const __m128i & min32s, __m128i & zeromask)
{
	__m128i rc, rd, s1, s2, denom;
	__m128	dstLof, dstHif, denomf, tdstLof, tdstHif;

	// src1 = b a
	// src2 = d c

	// seperate d and c
	rc	= _mm_slli_epi32(src2, 16);
	rd	= _mm_srli_epi32(_mm_xor_si128(src2, _mm_cmpeq_epi16(rc, rc)), 16);	// neg'd reverse position for d
	
	// ac + bd	- values for a
	s1	= _mm_madd_epi16(src1, src2);

	// fixup the only overflow value possible (-32768 * -32768 + -32768 * -32768)
	s1	= _mm_add_epi32(s1, _mm_cmpeq_epi32(s1, min32s));


	// reverse src2
	s2	= _mm_or_si128(rc, rd);

	// ac - bd		- values for b
	s2	= _mm_madd_epi16(src1, s2);
	s2	= _mm_add_epi32(s2, _mm_srai_epi32(_mm_slli_epi32(src1, 16), 16));

	// fixup overflow
	s2	= _mm_add_epi32(s2, _mm_cmpeq_epi32(s2, min32s));

	// interleave b's and a's
//	dstLo		= _mm_unpacklo_epi32(s1, s2);	// Don't interleave now
//	dstHi		= _mm_unpackhi_epi32(s1, s2);	// Divide first (since denom is in same order) and interleave after

	denom		= _mm_madd_epi16(src2, src2);
	denom		= _mm_add_epi32(denom, _mm_cmpeq_epi32(denom, min32s));

	denomf		= _mm_cvtepi32_ps(denom);

	tdstLof		= _mm_cvtepi32_ps(s1);
	tdstHif		= _mm_cvtepi32_ps(s2);

	zeromask	= _mm_cmpeq_epi32(denom, _mm_setzero_si128());

	tdstLof		= _mm_div_ps(tdstLof, denomf);
	tdstHif		= _mm_div_ps(tdstHif, denomf);

	dstLof		= _mm_unpacklo_ps(tdstLof, tdstHif);	// set dst in lo
	dstHif		= _mm_unpackhi_ps(tdstLof, tdstHif);	// return hi	(interleave now)

	destLo		= dstLof;
	return		  dstHif;
}


CBL_INLINE __m128i CBL_SSE2::Divide_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s)
{
	__m128   dstLo,  dstHi;
	__m128i idstLo, idstHi;
	__m128i zeromask = min32s;	// need to assign to some value to allow creation of register
								// optimization will make sure this assignment actually never takes place

	__m128i retval;

	dstHi	= Divide_16sc_32fc(src1, src2, dstLo, min32s, zeromask);

	idstLo	= _mm_cvtps_epi32(dstLo);
	idstHi	= _mm_cvtps_epi32(dstHi);

	retval	= _mm_packs_epi32(idstLo, idstHi);
	return	  _mm_andnot_si128(zeromask, retval);	// zeromask is set in epi32 for denom, = epi16 | epi16 masks per 32bit value
													// serves our purpose just fine without any further work
}


CBL_INLINE __m128	CBL_SSE2::FastMagnitude_16sc_32f(const __m128i & src, const __m128i & min32s)
{
	__m128i dst		= _mm_madd_epi16(src, src);
	dst				= _mm_add_epi32(dst, _mm_cmpeq_epi32(dst, min32s));
	__m128 old_dstf	= _mm_cvtepi32_ps(dst);
	__m128 dstf		= _mm_mul_ps(old_dstf, _mm_rsqrt_ps(old_dstf));
	return			  _mm_and_ps(dstf, _mm_cmpneq_ps(old_dstf, _mm_setzero_ps()));
}


CBL_INLINE __m128	CBL_SSE2::Magnitude_16sc_32f(const __m128i & src, const __m128i & min32s)
{
	__m128i dst		= _mm_madd_epi16(src, src);
	dst				= _mm_add_epi32(dst, _mm_cmpeq_epi32(dst, min32s));
	__m128 old_dstf	= _mm_cvtepi32_ps(dst);
	__m128 dstf		= _mm_sqrt_ps(old_dstf);
	return			  _mm_and_ps(dstf, _mm_cmpneq_ps(old_dstf, _mm_setzero_ps()));
}




CBL_INLINE void CBL_SSE2::Multiply_16sc32fc(const __m128i &multiplicand,const __m128i &multiplier,__m128 &destLo, __m128 &destHi)
{

	__m128i res1Lo,res1Hi,res2Lo,res2Hi;

	__m128i src1 = _mm_shufflelo_epi16(multiplicand,_MM_SHUFFLE(2, 2, 0, 0));				//  22 00 real parts
	res1Lo = _mm_shufflehi_epi16(src1,_MM_SHUFFLE(2, 2, 0, 0));       //  66 44 
	
	res1Hi = CBL_SSE2::Multiply_16s_32s(res1Lo,multiplier);

	__m128i src2  = _mm_shufflelo_epi16(multiplier ,_MM_SHUFFLE(2, 3, 0, 1));			
	src2 =_mm_shufflehi_epi16(src2 ,_MM_SHUFFLE(2, 3, 0, 1));

	src1 = _mm_shufflelo_epi16(multiplicand,_MM_SHUFFLE(3, 3, 1, 1));					// 33 11
	res2Lo = _mm_shufflehi_epi16(src1,_MM_SHUFFLE(3, 3, 1, 1));           // 77 55 

	res2Hi = CBL_SSE2::Multiply_16s_32s(res2Lo,src2);

	__m128 ires1Lof = _mm_cvtepi32_ps(res1Lo);
	__m128 ires1Hif = _mm_cvtepi32_ps(res1Hi);
	__m128 ires2Lof = _mm_cvtepi32_ps(res2Lo);
	__m128 ires2Hif = _mm_cvtepi32_ps(res2Hi);

	__m128 resLoRe = _mm_sub_ps(ires1Lof,ires2Lof);
	__m128 resHiRe = _mm_sub_ps(ires1Hif,ires2Hif);

	__m128 resLoIm= _mm_add_ps(ires1Lof,ires2Lof);
	__m128 resHiIm= _mm_add_ps(ires1Hif,ires2Hif);

	resLoRe = _mm_shuffle_ps(resLoRe,resLoIm,_MM_SHUFFLE(3, 1, 2, 0));
	destLo = _mm_shuffle_ps(resLoRe,resLoRe,_MM_SHUFFLE(3, 1, 2, 0));

	resHiIm = _mm_shuffle_ps(resHiRe,resHiIm,_MM_SHUFFLE(3, 1, 2, 0));
	destHi = _mm_shuffle_ps(resHiIm,resHiIm,_MM_SHUFFLE(3, 1, 2, 0));
}


CBL_INLINE __m128  CBL_SSE2::Multiply_32s32f( const __m128i &s1, const __m128i& s2)
{
	__m128 temp1,temp2;

	temp1 = _mm_cvtepi32_ps (s1);
	temp2 = _mm_cvtepi32_ps (s2);
	return  _mm_mul_ps( temp1, temp2 );
}


CBL_INLINE __m128  CBL_SSE2::Multiply_32sc32fc(const __m128i & multiplicand, const __m128i & multiplier)
{
	
		__m128i src1   = _mm_shuffle_epi32(multiplicand,_MM_SHUFFLE(2, 2, 0, 0));
		__m128i src1_1 = _mm_shuffle_epi32(multiplicand,_MM_SHUFFLE(3, 3, 1, 1));


		__m128i src2 = _mm_shuffle_epi32(multiplier,_MM_SHUFFLE(2, 3, 0, 1));
		
		__m128 temp1 = CBL_SSE2::Multiply_32s32f(src1,multiplier);		
		__m128 temp2 = CBL_SSE2::Multiply_32s32f(src1_1,src2);

		__m128 src1f = _mm_sub_ps(temp1,temp2);		
		__m128 src2f = _mm_add_ps(temp1,temp2);

		src1f = _mm_shuffle_ps(src1f,src2f,_MM_SHUFFLE(3, 1, 2, 0));
		return  _mm_shuffle_ps(src1f,src1f,_MM_SHUFFLE(3, 1, 2, 0));
}


CBL_INLINE void CBL_SSE2::Multiply_32s64f( const __m128i & s1, const __m128i & s2,__m128d& desLo, __m128d  &desHi)
{
	__m128d temp1,temp2,temp3,temp4;

	temp1 = _mm_cvtepi32_pd (s1);
	temp2 = _mm_cvtepi32_pd (s2);
	desLo = _mm_mul_pd( temp1, temp2 );

	__m128i is1 = _mm_unpackhi_epi64 (s1,s1);
	__m128i is2  = _mm_unpackhi_epi64 (s2,s2);
	temp3 = _mm_cvtepi32_pd (is1);
	temp4 = _mm_cvtepi32_pd (is2);
	desHi = _mm_mul_pd( temp3, temp4 );
}

CBL_INLINE void CBL_SSE2::Multiply_32sc64fc( const __m128i& s1, const __m128i &s2,__m128d &desLo, __m128d  &desHi)
{
		__m128d dst1_lo,dst1_hi, dst2_lo,dst2_hi ;

		__m128i src1 = _mm_shuffle_epi32(s1,_MM_SHUFFLE(2, 2, 0, 0));

		CBL_SSE2::Multiply_32s64f(src1,s2,dst1_lo,dst1_hi);
		
		__m128i src2 =_mm_shuffle_epi32(s2,_MM_SHUFFLE(2, 3, 0, 1));
		src1 = _mm_shuffle_epi32(s1,_MM_SHUFFLE(3, 3, 1, 1));
		
		CBL_SSE2::Multiply_32s64f(src1,src2,dst2_lo,dst2_hi);

		__m128d  destLoRe = _mm_sub_sd(dst1_lo,dst2_lo);
		__m128d  destHiRe = _mm_sub_sd(dst1_hi,dst2_hi);

		__m128d  destLoIm = _mm_add_pd(dst1_lo,dst2_lo);
		__m128d  destHiIm = _mm_add_pd(dst1_hi,dst2_hi);

		 desLo = _mm_move_sd(destLoIm,destLoRe);
		 desHi = _mm_move_sd(destHiIm,destHiRe);
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_1( __m128i d, const __m128i & m )
{
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_add_epi16( r, d );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_1_NEG( __m128i d, const __m128i & m )
{
	static const __m128i zero = CONST_SETZERO_32I();
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_add_epi16( r, d );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	r = _mm_sub_epi16( zero, r );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_2( __m128i d, const __m128i & m )
{
	__m128i r = _mm_mulhi_epi16( d, m );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_2_NEG( __m128i d, const __m128i & m )
{
	static const __m128i zero = CONST_SETZERO_32I();
	__m128i r = _mm_mulhi_epi16( d, m );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	r = _mm_sub_epi16( zero, r );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_1_S( __m128i d, const __m128i & m, const __m128i & s )
{
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_add_epi16( r, d );
	r = _mm_sra_epi16( r, s );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_1_S_NEG( __m128i d, const __m128i & m, const __m128i & s )
{
	static const __m128i zero = CONST_SETZERO_32I();
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_add_epi16( r, d );
	r = _mm_sra_epi16( r, s );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	r = _mm_sub_epi16( zero, r );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_2_S( __m128i d, const __m128i & m, const __m128i & s )
{
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_sra_epi16( r, s );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	return r;
}

CBL_INLINE __m128i CBL_SSE2::DIVISION16_SSE2_2_S_NEG( __m128i d, const __m128i & m, const __m128i & s )
{
	static const __m128i zero = CONST_SETZERO_32I();
	__m128i r = _mm_mulhi_epi16( d, m );
	r = _mm_sra_epi16( r, s );
	d = _mm_srli_epi16( d, 15 );
	r = _mm_add_epi16( r, d );
	r = _mm_sub_epi16( zero, r );
	return r;
}

/*!
    Performs division on signed 16 bit integer values using previously calculated constants mm, ss 
    and codePath obtained by a previous call to PrepDiv_16s_const().
    \par Parameters:
    \param[in] d dividend
    \param[in] mm precalculated constant obtained by calling PrepDiv_16s_const()
    \param[in] ss precalculated constant obtained by calling PrepDiv_16s_const()
    \param[in] executePath constant value obtained by calling PrepDiv_16s_const()
    \return result (quotient) of the division by a constant passed to the PrepDiv_16s_const() call
    - result[0] = dividend[0] / const
    - result[1] = dividend[1] / const
    - result[2] = dividend[2] / const
    - result[3] = dividend[3] / const
    - result[4] = dividend[4] / const
    - result[5] = dividend[5] / const
    - result[6] = dividend[6] / const
    - result[7] = dividend[7] / const

    \par Notes:
    This implementation of division by a constant uses a very fast algorithm that requires 
    the user to call the PrepDiv_16s_const() function first (to calculate some parameters used during 
    the actual division).  The PrepDiv_16s_const() function will also return an integer value indicating 
    the algorithm that should be used to perform the division.  The Divide_16s_const() function will 
    automatically choose the appropriate path based on the arguments.
    \par 
    Always match the correct PrepDiv_16s_const() call with the Divide_16s_const() function.
    The reference implementation of the two functions uses a different algorithm than the SSE2 implementation.  
    Mixing of the reference and SSE2 versions of the two functions is not supported.
	\par See also:
    PrepDiv_16s_const()

*/
CBL_INLINE __m128i CBL_SSE2::Divide_16s_const( __m128i & d, const __m128i & mm, __m128i ss, U8 executePath )
{
	switch( executePath )
	{
	case 1:
		return DIVISION16_SSE2_1_NEG( d, mm );
		break;
	case 2:
		return DIVISION16_SSE2_1( d, mm );
		break;
	case 3:
		return DIVISION16_SSE2_2_NEG( d, mm );
		break;
	case 4:
		return DIVISION16_SSE2_2( d, mm );
		break;
	case 5:
		return DIVISION16_SSE2_1_S_NEG( d, mm, ss );
		break;
	case 6:
		return DIVISION16_SSE2_1_S( d, mm, ss );
		break;
	case 7:
		return DIVISION16_SSE2_2_S_NEG( d, mm, ss );
		break;
	case 8:
		return DIVISION16_SSE2_2_S( d, mm, ss );
		break;
	default:
		return _mm_setzero_si128();
	}
}

/*!
    Precalculates constants used during division on signed 16 bit integer values in subsequent call to Divide_16s_const.
    \par Parameters:
    \param[in] divisor - the divisor value
    \param[out] mm precalculated constant
    \param[out] ss precalculated constant
    \return codePath value indicating which algorithm to use in the Divide_16s_const() call.
    \par Notes:
    Always match the correct PrepDiv_16s_const() call with the Divide_16s_const() function.
    The reference implementation of the two functions uses a different algorithm than the SSE2 implementation.  
    Mixing of the reference and SSE2 versions of the two functions is not supported.
	\par See also:
    Divide_16s_const()
*/
CBL_INLINE U8 CBL_SSE2::PrepDiv_16s_const( __m128i & mm, __m128i & ss, S16 divisor )
{
	long e;
	U32 d, l, s, m, a;
	U32 m_low, m_high, j, k;

	e = divisor;
	d = divisor;
	if( divisor < 0 )
	{
		d = -divisor;
	}
	if (d == 0) return 0;

	/* Determine algorithm (a), multiplier (m), and shift factor (s) for 32-bit
	signed integer division. Based on: Granlund, T.; Montgomery, P.L.:
	"Division by Invariant Integers using Multiplication". SIGPLAN Notices,
	Vol. 29, June 1994, page 61.
	*/

	{ // calculate l = log2(d)
		U32 i = d;
		l = 0;
		i = i >> 1;
		while (i) {
			i = i >> 1;
			l++;
		}
	}
	j = (((U32)(0x8000)) % ((U32)(d)));
	k = (((U32)(1)) << (16 + l)) / ((U32)(0x8000 - j));
	m_low = (((U64)(1)) << (16 + l)) / d;
	m_high = ((((U64)(1)) << (16 + l)) + k) / d;
	while (((m_low >> 1) < (m_high >> 1)) && (l > 0)) {
		m_low = m_low >> 1;
		m_high = m_high >> 1;
		l = l - 1;
	}
	m = ((U32)(m_high));
	s = l;
	a = (m_high >> 15) ? 1 : 0;

	mm = _mm_set1_epi16( (short)m );
	ss = _mm_set_epi32( 0, 0, 0, s );

	// choose the execution path
	U8 executePath = 1;
	if (a == 0) {
		executePath += 2;
	}
	if (divisor >= 0) 
	{
		executePath += 1;
	}
	if ( s != 0 )
	{
		executePath += 4;
	}

	return executePath;
}

CBL_INLINE __m128i CBL_SSE2::FastDivide_16s_const( const __m128i & dividend, const __m128i & operand )
{
    __m128i result = _mm_mulhi_epi16( dividend, operand );
    return result;
}

// horizontal add and subtract routines

CBL_INLINE __m128 CBL_SSE2::HorizontalAdd_32f( const __m128 & src1, const __m128 & src2 )
{
	__m128 res1 = _mm_shuffle_ps(src1,src2,_MM_SHUFFLE(2, 0, 2, 0)); // b2,b0,a2,a0
	__m128 res2 = _mm_shuffle_ps(src1,src2,_MM_SHUFFLE(3, 1, 3, 1)); // b3,b1,a3,a1
	return _mm_add_ps(res1,res2);
}

CBL_INLINE __m128 CBL_SSE2::HorizontalSubtract_32f( const __m128 & src1, const __m128 & src2 )
{
	__m128 res1 = _mm_shuffle_ps(src1,src2,_MM_SHUFFLE(2, 0, 2, 0)); // b2,b0,a2,a0 // not available in shuffle library
	__m128 res2 = _mm_shuffle_ps(src1,src2,_MM_SHUFFLE(3, 1, 3, 1)); // b3,b1,a3,a1
	return _mm_sub_ps(res1,res2);
}

CBL_INLINE __m128 CBL_SSE2::AddAndSubtract_32f( const __m128 & src1, const __m128 & src2 )
{
	__m128 res1 =  _mm_sub_ps(src1,src2); // A3 - B3, A2 - B2, A1 - B1, A0 - B0
	__m128 res2 =  _mm_add_ps(src1,src2); // A3 + B3, A2 + B2, A1 + B1, A0 + B0
	__m128 res3 = _mm_shuffle_ps(res1,res2,_MM_SHUFFLE(3, 1, 2, 0));	//A3+B3,A1+B1,A2-B2,A0-B0
	return  _mm_shuffle_ps(res3,res3,_MM_SHUFFLE(3, 1, 2, 0)); // A3+B3,A2-B2,A+B1,A0-B0

}

CBL_INLINE __m128d CBL_SSE2::HorizontalAdd_64f( const __m128d & src1, const __m128d & src2 )
{
	__m128d res1 = _mm_unpacklo_pd(src1,src2); // 	 b0,a0
	__m128d res2 = _mm_unpackhi_pd(src1,src2); //     b1,a1
	return  _mm_add_pd(res1,res2); // b0+b1, a0+a1
}

CBL_INLINE __m128d CBL_SSE2::HorizontalSubtract_64f( const __m128d & src1, const __m128d & src2 )
{
	__m128d res1 = _mm_unpacklo_pd(src1,src2); // 	 b0,a0
	__m128d res2 = _mm_unpackhi_pd(src1,src2); //     b1,a1
	return  _mm_sub_pd(res1,res2); // b0-b1, a0-a1
}

CBL_INLINE __m128d CBL_SSE2::AddAndSubtract_64f( const __m128d & src1, const __m128d & src2 )
{
    // src1 = A1, A0
    // src2 = B1, B0
    XMM128 t1, t2;
    t1.d = _mm_sub_pd( src1, src2 );    // A1 - B1, A0 - B0
    t2.d = _mm_add_pd( src1, src2 );    // A1 + B1, A0 + B0

    //t1.d = _mm_shuffle_pd( t1.d, t2.d, 2 ); // A1 + B1, A0 - B0
    t1.d = Shuffle_b1a0_64f( t1.d, t2.d );
    return t1.d;
}


CBL_INLINE void CBL_SSE2::HorizontalAdd_16s( __m128i & values )
{
	__m128i opr1;
	__m128i	opr2 = _mm_unpackhi_epi16(values,values);
			opr2 = _mm_add_epi16(values,opr2);
			opr1 = _mm_srli_si128(opr2,4);
			opr1 = _mm_add_epi16(opr1,opr2);
			opr2 = _mm_srli_si128(opr1,2);
			opr1 =  _mm_add_epi16(opr1,opr2);

			opr1 =_mm_srli_si128( _mm_slli_si128(opr1,14),14);
			values = _mm_slli_si128( _mm_srli_si128(values,2),2);
			values = _mm_or_si128(values,opr1);
}

CBL_INLINE void CBL_SSE2::HorizontalAdd_16s( __m128i & value1, const __m128i & value2 )
{
	__m128i t1 = _mm_unpacklo_epi16(value1,value2);
	__m128i t2 =  _mm_unpackhi_epi16(value1,value2);
	
	__m128i ts1 = _mm_srli_si128(t1,4);
	__m128i ts2 = _mm_srli_si128(t2,4);

	t1 = _mm_add_epi16(t1,ts1);
	t2 = _mm_add_epi16(t2,ts2);
	
	ts1 = _mm_unpacklo_epi16(t1,t2);
	ts2 = _mm_unpackhi_epi16(t1,t2);

	value1 = _mm_unpacklo_epi16(ts1,ts2);	

}

CBL_INLINE void CBL_SSE2::HorizontalSub_16s( __m128i & value1, const __m128i & value2 )
{
	__m128i t1 = _mm_unpacklo_epi16(value1,value2);
	__m128i t2 =  _mm_unpackhi_epi16(value1,value2);
	
	__m128i ts1 = _mm_srli_si128(t1,4);
	__m128i ts2 = _mm_srli_si128(t2,4);

	t1 = _mm_sub_epi16(t1,ts1);
	t2 = _mm_sub_epi16(t2,ts2);
	
	ts1 = _mm_unpacklo_epi16(t1,t2);
	ts2 = _mm_unpackhi_epi16(t1,t2);

	value1 = _mm_unpacklo_epi16(ts1,ts2);	
}

CBL_INLINE void CBL_SSE2::HorizontalAdd_32s( __m128i & values )
{
   __m128i  t1 = _mm_unpackhi_epi32(values,values);
	t1 = _mm_add_epi32(values,t1);
	__m128i t2 = _mm_srli_si128(t1,8);
	t1 = _mm_add_epi32(t2,t1);
	t2 = _mm_srli_si128(t1,4);
	t1 = _mm_add_epi32(t2,t1);			

	t1 =_mm_srli_si128( _mm_slli_si128(t1,12),12);
	values = _mm_slli_si128( _mm_srli_si128(values,4),4);
	values = _mm_or_si128(values,t1);
}

CBL_INLINE void CBL_SSE2::HorizontalAdd_32s( __m128i & value1, const __m128i & value2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi32(value1,value2); // b1,a1,b0,a0
	thi = _mm_unpackhi_epi32(value1,value2); // b3,a3,b2,a2

	t1lo = _mm_unpacklo_epi32(tlo,thi); // b2,b0,a2,a0
	t1hi = _mm_unpackhi_epi32(tlo,thi); // b3,b1,a3,a1

	value1 =  _mm_add_epi32(t1lo,t1hi);	
}

CBL_INLINE void CBL_SSE2::HorizontalSub_32s( __m128i & value1, const __m128i & value2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi32(value1,value2); // b1,a1,b0,a0
	thi = _mm_unpackhi_epi32(value1,value2); // b3,a3,b2,a2

	t1lo = _mm_unpacklo_epi32(tlo,thi); // b2,b0,a2,a0
	t1hi = _mm_unpackhi_epi32(tlo,thi); // b3,b1,a3,a1

	value1 =  _mm_sub_epi32(t1lo,t1hi);	
}



CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S8toS16( const __m128i & src )
{
	__m128i t1,t2;				//src = ....   a3,a2,a1,a0
	t1 = _mm_srai_epi16(src,8);  // ... .... sa5,a5,sa3,a3,sa1,a1	.. s -- sign bit
	t2 = _mm_slli_epi16(src,8);  // .... .....a4,0,a2,0,a0,0
	t2 = _mm_srai_epi16(t2,8);	// .... .... sa4,a4,sa2,a2,sa0,a0
	return _mm_add_epi16(t1,t2); // .... .... a4+a5,a2+a3,a0+a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S8toS32( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_S8toS16(src);
	return  CBL_SSE2::HorizontalAdd_S16toS32(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S8toS64( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_S8toS16(src);
	res1 =  CBL_SSE2::HorizontalAdd_S16toS32(res1);
	return CBL_SSE2::HorizontalAdd_S32toS64(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S16toS32( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srai_epi32(src,16);
	t2 = _mm_slli_epi32(src,16); 
	t2 = _mm_srai_epi32(t2,16);
	return _mm_add_epi32(t1,t2);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S16toS64( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_S16toS32(src);
	return  CBL_SSE2::HorizontalAdd_S32toS64(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S32toS64( const __m128i & src )
{
	__m128i t1,t2;
	__m128i sign = _mm_srai_epi32(src,31);
	__m128i sign1 = _mm_srli_epi64(sign,32);
	sign1 = _mm_slli_epi64(sign1,32);

	t1 = _mm_srli_epi64 (src,32);
	t1 = _mm_or_si128(t1,sign1);

	sign = _mm_slli_epi64(sign,32);

	t2 = _mm_slli_epi64 (src,32);
	t2 = _mm_srli_epi64 (t2,32);

	t2 = _mm_or_si128(t2,sign);



	return  _mm_add_epi64(t1,t2);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U8toU16( const __m128i & src )
{
	__m128i t1,t2;				//src = ....   a3,a2,a1,a0
	t1 = _mm_srli_epi16(src,8);  // ... .... 0,a5,0,a3,0,a1	
	t2 = _mm_slli_epi16(t1,8);  // .... .....a5,0,a3,0,a1,0
	t2 = _mm_xor_si128(src,t2);	// .... .... 0,a4,0,a2,0,a0
	return _mm_add_epi16(t1,t2); // .... .... a4+a5,a2+a3,a0+a1
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U8toU32( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_U8toU16(src);
	return  CBL_SSE2::HorizontalAdd_U16toU32(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U8toU64( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_U8toU16(src);
	res1 =  CBL_SSE2::HorizontalAdd_U16toU32(res1);
	return CBL_SSE2::HorizontalAdd_U32toU64(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U16toU32( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srli_epi32(src,16);
	t2 = _mm_slli_epi32(t1,16); 
	t2 = _mm_xor_si128(src,t2);
	return _mm_add_epi32(t1,t2);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U16toU64( const __m128i & src )
{
	__m128i res1;
	res1 =  CBL_SSE2::HorizontalAdd_U16toU32(src);
	return CBL_SSE2::HorizontalAdd_U32toU64(res1);
}

CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_U32toU64( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srli_epi64 (src,32);
	t2 = _mm_slli_epi64(t1,32); 
	t2 = _mm_xor_si128(src,t2);
	return  _mm_add_epi64(t1,t2);
}


CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_S8toS16( const __m128i & src )
{
	__m128i t1,t2;				//src = ....   a3,a2,a1,a0
	t1 = _mm_srai_epi16(src,8);  // ... .... sa5,a5,sa3,a3,sa1,a1	.. s -- sign bit
	t2 = _mm_slli_epi16(src,8);  // .... .....a4,0,a2,0,a0,0
	t2 = _mm_srai_epi16(t2,8);	// .... .... sa4,a4,sa2,a2,sa0,a0
	return _mm_sub_epi16(t2,t1); // .... .... a4+a5,a2+a3,a0+a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_S16toS32( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srai_epi32(src,16);
	t2 = _mm_slli_epi32(src,16); 
	t2 = _mm_srai_epi32(t2,16);
	return _mm_sub_epi32(t2,t1);
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_S32toS64( const __m128i & src )
{
	__m128i t1,t2;
	__m128i sign = _mm_srai_epi32(src,31);
	__m128i sign1 = _mm_srli_epi64(sign,32);
	sign1 = _mm_slli_epi64(sign1,32);

	t1 = _mm_srli_epi64 (src,32);
	t1 = _mm_or_si128(t1,sign1);

	sign = _mm_srai_epi32(src,31);
	sign = _mm_slli_epi64(sign,32);


	t2 = _mm_slli_epi64 (src,32);
	t2 = _mm_srli_epi64 (t2,32);

	t2 = _mm_or_si128(t2,sign);



	return  _mm_sub_epi64(t2,t1);
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_U8toU16( const __m128i & src )
{
	__m128i t1,t2;				//src = ....   a3,a2,a1,a0
	t1 = _mm_srli_epi16(src,8);  // ... .... 0,a5,0,a3,0,a1	
	t2 = _mm_slli_epi16(t1,8);  // .... .....a5,0,a3,0,a1,0
	t2 = _mm_xor_si128(src,t2);	// .... .... 0,a4,0,a2,0,a0
	return _mm_sub_epi16(t2,t1); // .... .... a4+a5,a2+a3,a0+a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_U16toU32( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srli_epi32(src,16);
	t2 = _mm_slli_epi32(t1,16); 
	t2 = _mm_xor_si128(src,t2);
	return _mm_sub_epi32(t2,t1);
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_U32toU64( const __m128i & src )
{
	__m128i t1,t2;
	t1 = _mm_srli_epi64 (src,32);
	t2 = _mm_slli_epi64(t1,32); 
	t2 = _mm_xor_si128(src,t2);
	return  _mm_sub_epi64(t2,t1);
}


CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S16( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi16(src1,src2); // b3,a3,b2,a2,b1,a1,b0,a0
	thi = _mm_unpackhi_epi16(src1,src2); // b7,a7,b6,a6,b5,a5,b4,a4

	t1lo = _mm_unpacklo_epi16(tlo,thi); // b5,b1,a5,a1,b4,b0,a4,a0
	t1hi = _mm_unpackhi_epi16(tlo,thi); // b7,b3,a7,a3,b6,b2,a6,a2
	
	tlo = _mm_srli_si128(t1lo,8);	//0,0,0,0,b5,b1,a5,a1,
	thi = _mm_srli_si128(t1hi,8);	//0,0,0,0,b7,b3,a7,a3
	
	tlo = _mm_add_epi16(t1lo,tlo); // X,X,X,X,b4+b5,b0+b1,a4+a5,a0+a1   X -> Dont Care.
	thi = _mm_add_epi16(t1hi,thi); // X,X,X,X,b6+b7,b2+b3,a6+a7,a2+a3

	return _mm_unpacklo_epi16(tlo,thi); // b6+b7,b4+b5,b2+b3,b0+b1,a6+a7,a4+a5,a2+a3,a0+a1

}
CBL_INLINE __m128i CBL_SSE2::HorizontalAddSat_S16( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi16(src1,src2); // b3,a3,b2,a2,b1,a1,b0,a0
	thi = _mm_unpackhi_epi16(src1,src2); // b7,a7,b6,a6,b5,a5,b4,a4

	t1lo = _mm_unpacklo_epi16(tlo,thi); // b5,b1,a5,a1,b4,b0,a4,a0
	t1hi = _mm_unpackhi_epi16(tlo,thi); // b7,b3,a7,a3,b6,b2,a6,a2
	
	tlo = _mm_srli_si128(t1lo,8);	//0,0,0,0,b5,b1,a5,a1,
	thi = _mm_srli_si128(t1hi,8);	//0,0,0,0,b7,b3,a7,a3
	
	tlo =  _mm_adds_epi16(t1lo,tlo); // X,X,X,X,S(b4+b5),S(b0+b1),S(a4+a5),S(a0+a1)   X -> Dont Care. S-> Saturate.
	thi =  _mm_adds_epi16(t1hi,thi); // X,X,X,X,S(b6+b7),S(b2+b3),S(a6+a7),S(a2+a3)

	return _mm_unpacklo_epi16(tlo,thi); // b6+b7,b4+b5,b2+b3,b0+b1,a6+a7,a4+a5,a2+a3,a0+a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_S16( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi16(src1,src2); // b3,a3,b2,a2,b1,a1,b0,a0
	thi = _mm_unpackhi_epi16(src1,src2); // b7,a7,b6,a6,b5,a5,b4,a4

	t1lo = _mm_unpacklo_epi16(tlo,thi); // b5,b1,a5,a1,b4,b0,a4,a0
	t1hi = _mm_unpackhi_epi16(tlo,thi); // b7,b3,a7,a3,b6,b2,a6,a2
	
	tlo = _mm_srli_si128(t1lo,8);	//0,0,0,0,b5,b1,a5,a1,
	thi = _mm_srli_si128(t1hi,8);	//0,0,0,0,b7,b3,a7,a3
	
	tlo = _mm_sub_epi16(t1lo,tlo); // X,X,X,X,b4-b5,b0-b1,a4-a5,a0-a1   X -> Dont Care.
	thi = _mm_sub_epi16(t1hi,thi); // X,X,X,X,b6-b7,b2-b3,a6-a7,a2-a3

	return _mm_unpacklo_epi16(tlo,thi); // b6-b7,b4-b5,b2-b3,b0-b1,a6-a7,a4-a5,a2-a3,a0-a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtractSat_S16( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi16(src1,src2); // b3,a3,b2,a2,b1,a1,b0,a0
	thi = _mm_unpackhi_epi16(src1,src2); // b7,a7,b6,a6,b5,a5,b4,a4

	t1lo = _mm_unpacklo_epi16(tlo,thi); // b5,b1,a5,a1,b4,b0,a4,a0
	t1hi = _mm_unpackhi_epi16(tlo,thi); // b7,b3,a7,a3,b6,b2,a6,a2
	
	tlo = _mm_srli_si128(t1lo,8);	//0,0,0,0,b5,b1,a5,a1,
	thi = _mm_srli_si128(t1hi,8);	//0,0,0,0,b7,b3,a7,a3
	
	tlo =  _mm_subs_epi16(t1lo,tlo); // X,X,X,X,S(b4-b5),S(b0-b1),S(a4-a5),S(a0-a1)   X -> Dont Care. S-> Saturate.
	thi =  _mm_subs_epi16(t1hi,thi); // X,X,X,X,S(b6-b7),S(b2-b3),S(a6-a7),S(a2-a3)

	return _mm_unpacklo_epi16(tlo,thi); // b6-b7,b4-b5,b2-b3,b0-b1,a6-a7,a4-a5,a2-a3,a0-a1
}
CBL_INLINE __m128i CBL_SSE2::HorizontalAdd_S32( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi32(src1,src2); // b1,a1,b0,a0
	thi = _mm_unpackhi_epi32(src1,src2); // b3,a3,b2,a2

	t1lo = _mm_unpacklo_epi32(tlo,thi); // b2,b0,a2,a0
	t1hi = _mm_unpackhi_epi32(tlo,thi); // b3,b1,a3,a1

	return _mm_add_epi32(t1lo,t1hi);
}
CBL_INLINE __m128i CBL_SSE2::HorizontalSubtract_S32( const __m128i & src1, const __m128i & src2 )
{
	__m128i tlo,thi,t1lo,t1hi;

	tlo = _mm_unpacklo_epi32(src1,src2); // b1,a1,b0,a0
	thi = _mm_unpackhi_epi32(src1,src2); // b3,a3,b2,a2

	t1lo = _mm_unpacklo_epi32(tlo,thi); // b2,b0,a2,a0
	t1hi = _mm_unpackhi_epi32(tlo,thi); // b3,b1,a3,a1

	return _mm_sub_epi32(t1lo,t1hi);
}

/// Absolute functions

CBL_INLINE __m128i CBL_SSE2::Abs_S8( const __m128i &src )
{
	__m128i complement, temp,dst,dst_lo,dst_hi;
	temp = _mm_cmplt_epi8(src,_mm_setzero_si128 ());
	complement = _mm_xor_si128(src, temp);  //one's complement of src
	dst = _mm_sub_epi8 (complement, temp);

	// need to optimize this block or think of other methods of implementing the function//
	//  temp  = _mm_srli_epi8(dst, 7);
	dst_lo = _mm_unpacklo_epi8(_mm_setzero_si128 (),dst); 
	dst_hi = _mm_unpackhi_epi8(_mm_setzero_si128 (),dst); 
	dst_lo  = _mm_srli_epi16(dst_lo, 15);
	dst_hi  = _mm_srli_epi16(dst_hi, 15);
	temp = _mm_packs_epi16(dst_lo,dst_hi);
	////
	return  _mm_sub_epi8(dst, temp);		
}

CBL_INLINE __m128i CBL_SSE2::Abs_S16( const __m128i &src )
{
	 /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
	__m128i complement, temp,dst ;
	temp = _mm_srai_epi16(src, 15);
	complement = _mm_xor_si128(src, temp);  //one's complement of src
	dst = _mm_sub_epi16 (complement, temp);
	temp  = _mm_srli_epi16(dst, 15);
	return  _mm_sub_epi16(dst, temp);
}
CBL_INLINE __m128i CBL_SSE2::Abs_S32( const __m128i &src )
{
	 /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
	__m128i complement, temp,dst;
	temp = _mm_srai_epi32(src, 31);
	complement = _mm_xor_si128(src, temp);  //one's complement of src
	dst = _mm_sub_epi32 (complement, temp);
	temp  = _mm_srli_epi32(dst, 31);
	return  _mm_sub_epi32(dst, temp);

}

CBL_INLINE __m128i CBL_SSE2::Abs_S64( const __m128i &src )
{
	__m128i complement, temp,dst;
	temp = _mm_cmplt_epi32(src,_mm_setzero_si128 ());
	temp = _mm_shuffle_epi32 (temp, _MM_SHUFFLE(3, 3, 1, 1));
	complement = _mm_xor_si128(src, temp);  //one's complement of src
	dst = _mm_sub_epi64 (complement, temp);
	temp  = _mm_srli_epi64(dst, 63);
	return  _mm_sub_epi64(dst, temp);	
}

/// End of Absolute functions

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulate_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i dst;
	dst =  _mm_mullo_epi16(src1,src2);
	return _mm_add_epi16(dst,src3);

}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulateSat_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i dstlo,dsthi,s3lo,s3hi,dst1;
	dstlo =  _mm_mullo_epi16(src1,src2);
	dsthi =  _mm_mulhi_epi16(src1,src2);
			
	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 
	
	s3lo = src3;
	CBL_SSE2::Unpack16STo32S(s3lo,s3hi);

	dstlo = _mm_add_epi32(dst1,s3lo);
	dsthi = _mm_add_epi32(dsthi,s3hi);
	
	return _mm_packs_epi32(dstlo,dsthi);
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i dstlo,dsthi,dst1;
	dstlo =  _mm_mullo_epi16(src1,src2);
	dsthi =  _mm_mulhi_epi16(src1,src2);
			
	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 

	dstlo =_mm_unpacklo_epi32(dst1,dsthi); 
	dsthi =_mm_unpackhi_epi32(dst1,dsthi); 
	
	dstlo = _mm_unpacklo_epi32(dstlo,dsthi);

	return _mm_add_epi32(dstlo,src3);	
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i dstlo,dsthi,dst1;
	dstlo =  _mm_mullo_epi16(src1,src2);
	dsthi =  _mm_mulhi_epi16(src1,src2);
			
	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 

	dstlo =_mm_unpacklo_epi32(dst1,dsthi); 
	dsthi =_mm_unpackhi_epi32(dst1,dsthi); 
	
	dstlo = _mm_unpacklo_epi32(dstlo,dsthi);

	XMM128 srcTemp1_1,srcTemp2_1;
	XMM128 srcTemp1_2, srcTemp2_2;

	srcTemp1_1.d = _mm_cvtepi32_pd(dstlo);
	srcTemp2_1.d = _mm_cvtepi32_pd(src3);

	srcTemp1_2.i = _mm_srli_si128(dstlo, 8);
	srcTemp2_2.i = _mm_srli_si128(src3, 8);

	srcTemp1_2.d = _mm_cvtepi32_pd(srcTemp1_2.i);
	srcTemp2_2.d = _mm_cvtepi32_pd(srcTemp2_2.i);

	srcTemp1_1.d = _mm_add_pd(srcTemp1_1.d, srcTemp2_1.d);
	srcTemp1_2.d = _mm_add_pd(srcTemp1_2.d, srcTemp2_2.d);

	return CBL_SSE2::Pack64FTo32S(srcTemp1_1.d,srcTemp1_2.d);
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulate_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i a,b,c,d,x,y,LO;
	__m128i dstlo,dsthi,dst1;

	a = _mm_srai_epi32(src1,16);
	b = _mm_slli_epi32(src1,16);
	b = _mm_srli_epi32(b,16);

	c = _mm_srai_epi32(src2,16);
	d = _mm_slli_epi32(src2,16);
	d = _mm_srli_epi32(d,16);

	dstlo =  _mm_mullo_epi16(b,d);
	dsthi =  _mm_mulhi_epu16(b,d);

	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 

	dstlo =_mm_unpacklo_epi32(dst1,dsthi); 
	dsthi =_mm_unpackhi_epi32(dst1,dsthi); 
	
	LO = _mm_unpacklo_epi32(dstlo,dsthi);

	dstlo =  _mm_mullo_epi16(a,d);
	dsthi =  _mm_mulhi_epi16(a,d);

	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 

	dstlo =_mm_unpacklo_epi32(dst1,dsthi); 
	dsthi =_mm_unpackhi_epi32(dst1,dsthi); 
	
	x = _mm_unpacklo_epi32(dstlo,dsthi);

	dstlo =  _mm_mullo_epi16(c,b);
	dsthi =  _mm_mulhi_epu16(c,b);

	dst1 =_mm_unpacklo_epi16(dstlo,dsthi); 
	dsthi =_mm_unpackhi_epi16(dstlo,dsthi); 

	dstlo =_mm_unpacklo_epi32(dst1,dsthi); 
	dsthi =_mm_unpackhi_epi32(dst1,dsthi); 
	
	y = _mm_unpacklo_epi32(dstlo,dsthi);

	x = _mm_add_epi32(x,y);

	y = _mm_srli_epi32(LO,16);

	y = _mm_add_epi32(y,x);
	
	y = _mm_slli_epi32(y,16);

	LO = _mm_slli_epi32(LO,16);
	LO = _mm_srli_epi32(LO,16);

	LO = _mm_or_si128(LO,y);

	return _mm_add_epi32(LO,src3);
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulateSat_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	XMM128 s1lo,s1hi,s2lo,s2hi,s3lo,s3hi;

	s1lo.d  =  _mm_cvtepi32_pd(src1);
	s1hi.d = _mm_cvtepi32_pd(_mm_srli_si128(src1,8)); 

	s2lo.d  =  _mm_cvtepi32_pd(src2);
	s2hi.d = _mm_cvtepi32_pd(_mm_srli_si128(src2,8)); 

	s1lo.d = _mm_mul_pd(s1lo.d,s2lo.d);
	s1hi.d = _mm_mul_pd(s1hi.d,s2hi.d);

	s3lo.d  =  _mm_cvtepi32_pd(src3);
	s3hi.d = _mm_cvtepi32_pd(_mm_srli_si128(src3,8)); 
	
	s1lo.d = _mm_add_pd(s1lo.d,s3lo.d);
	s1hi.d = _mm_add_pd(s1hi.d,s3hi.d);

	return CBL_SSE2::Pack64FTo32S(s1lo.d,s1hi.d);
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulate_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = (pSrc1.s32[0] * pSrc2.s32[0]) + pSrc3.s64[0];
	pDst.s64[1] = (pSrc1.s32[2] * pSrc2.s32[2]) + pSrc3.s64[1];

	return pDst.i;
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulateSat_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[0] * pSrc2.s32[0]),pSrc3.s64[0]);
	pDst.s64[1] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[2] * pSrc2.s32[2]),pSrc3.s64[1]);

	return pDst.i;

}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulate_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = (pSrc1.s32[1] * pSrc2.s32[1]) + pSrc3.s64[0];
	pDst.s64[1] = (pSrc1.s32[3] * pSrc2.s32[3]) + pSrc3.s64[1];

	return pDst.i;
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAccumulateSat_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[1] * pSrc2.s32[1]),pSrc3.s64[0]);
	pDst.s64[1] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[3] * pSrc2.s32[3]),pSrc3.s64[1]);

	return pDst.i;
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAddAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{
	__m128i mlo,mhi,rlo,rhi;

	mlo = _mm_mullo_epi16(src1,src2);
	mhi = _mm_mulhi_epi16(src1,src2);

	rlo = _mm_unpacklo_epi16(mlo,mhi);
	rhi = _mm_unpackhi_epi16(mlo,mhi);

	mlo =  _mm_unpacklo_epi32(rlo,rhi); 
	mhi =  _mm_unpackhi_epi32(rlo,rhi); 

	rlo =  _mm_unpacklo_epi32(mlo,mhi); 
	rhi =  _mm_unpackhi_epi32(mlo,mhi); 

	rlo = _mm_add_epi32(rlo,rhi);
	
	return _mm_add_epi32(rlo,src3);
}

CBL_INLINE __m128i CBL_SSE2::MultiplyAddAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
{

	__m128i mlo,mhi,rlo,rhi;
	XMM128 s1lo,s1hi,s2lo,s2hi,s3lo,s3hi;

	mlo = _mm_mullo_epi16(src1,src2);
	mhi = _mm_mulhi_epi16(src1,src2);

	rlo = _mm_unpacklo_epi16(mlo,mhi);
	rhi = _mm_unpackhi_epi16(mlo,mhi);

	mlo =  _mm_unpacklo_epi32(rlo,rhi); 
	mhi =  _mm_unpackhi_epi32(rlo,rhi); 

	rlo =  _mm_unpacklo_epi32(mlo,mhi); 
	rhi =  _mm_unpackhi_epi32(mlo,mhi); 


	s1lo.d  =  _mm_cvtepi32_pd(rlo);
	s1hi.d = _mm_cvtepi32_pd(_mm_srli_si128(rlo,8)); 

	s2lo.d  =  _mm_cvtepi32_pd(rhi);
	s2hi.d = _mm_cvtepi32_pd(_mm_srli_si128(rhi,8)); 

	s1lo.d = _mm_add_pd(s1lo.d,s2lo.d);
	s1hi.d = _mm_add_pd(s1hi.d,s2hi.d);

	s3lo.d  =  _mm_cvtepi32_pd(src3);
	s3hi.d = _mm_cvtepi32_pd(_mm_srli_si128(src3,8)); 

	s1lo.d = _mm_add_pd(s1lo.d,s3lo.d);
	s1hi.d = _mm_add_pd(s1hi.d,s3hi.d);
	
	return  CBL_SSE2::Pack64FTo32S(s1lo.d,s1hi.d);

}

/*!
    result = src1 * src2 + src3
*/
CBL_INLINE __m128 CBL_SSE2::MultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    __m128 dst;
    dst = _mm_mul_ps( src1, src2 );
    dst = _mm_add_ps( dst, src3 );
    return dst;
}

/*!
    result = src1 * src2 + src3
*/
CBL_INLINE __m128d CBL_SSE2::MultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    __m128d dst;
    dst = _mm_mul_pd( src1, src2 );
    dst = _mm_add_pd( dst, src3 );
    return dst;
}

/*!
    result = src1 * src2 - src3
*/
CBL_INLINE __m128 CBL_SSE2::MultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    __m128 dst;
    dst = _mm_mul_ps( src1, src2 );
    dst = _mm_sub_ps( dst, src3 );
    return dst;
}

/*!
    result = src1 * src2 - src3
*/
CBL_INLINE __m128d CBL_SSE2::MultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    __m128d dst;
    dst = _mm_mul_pd( src1, src2 );
    dst = _mm_sub_pd( dst, src3 );
    return dst;
}

/*!
    result = -(src1 * src2) + src3
*/
CBL_INLINE __m128 CBL_SSE2::NegMultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    __m128 dst = _mm_setzero_ps();
    dst = _mm_sub_ps( dst, src1 );
    dst = _mm_mul_ps( dst, src2 );
    dst = _mm_add_ps( dst, src3 );
    return dst;
}

/*!
    result = -(src1 * src2) + src3
*/
CBL_INLINE __m128d CBL_SSE2::NegMultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    __m128d dst = _mm_setzero_pd();
    dst = _mm_sub_pd( dst, src1 );
    dst = _mm_mul_pd( dst, src2 );
    dst = _mm_add_pd( dst, src3 );
    return dst;
}

/*!
    result = -(src1 * src2) - src3
*/
CBL_INLINE __m128 CBL_SSE2::NegMultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    __m128 dst = _mm_setzero_ps();
    dst = _mm_sub_ps( dst, src1 );
    dst = _mm_mul_ps( dst, src2 );
    dst = _mm_sub_ps( dst, src3 );
    return dst;
}

/*!
    result = -(src1 * src2) - src3
*/
CBL_INLINE __m128d CBL_SSE2::NegMultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    __m128d dst = _mm_setzero_pd();
    dst = _mm_sub_pd( dst, src1 );
    dst = _mm_mul_pd( dst, src2 );
    dst = _mm_sub_pd( dst, src3 );
    return dst;
}
/// @endcond

#endif // __CBL_ARITHMETIC_PRIMITIVES_SSE2_H__
