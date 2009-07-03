/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_MATH_PRIMITIVES_SSE2_H__
#define __CBL_MATH_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS

#include "cbl_common_primitives.h"
#include "cbl_exp.h"
#include "cbl_ln.h"
#include "cbl_pow.h"
#include "cbl_sqrtCbrt.h"

/*
 * Equivalent of frexpf in math.h, which splits a normal float number into a power of 2 and a mantissa.
 * src = 2^exp * m, where 0.5 <= m <= 1
 * Input: single precision "src" 
 * Return value: mantissa m
 * Note: Inputs of INF and NAN are not handled
 */
CBL_INLINE __m128 CBL_SSE2::Frexpf(const __m128 &src, __m128i &exp)
{
	XMM128 tsrc ;
	tsrc.f = src ;
	__m128i eq_zero = _mm_cmpeq_epi32(tsrc.i, _mm_setzero_si128());

	__m128i sign = _mm_and_si128(tsrc.i, CBL_Library_Internal::__g_min_S32_vector);	
	tsrc.i = _mm_xor_si128(tsrc.i, sign);			//strip sign off
	
	__m128i mask = _mm_slli_epi32(_mm_srli_epi32(_mm_cmpeq_epi32(tsrc.i, tsrc.i), 26), 24);

	exp = _mm_sub_epi32(tsrc.i, mask);
	exp = _mm_srai_epi32(exp, 23);
	
	tsrc.i = _mm_srli_epi32(_mm_slli_epi32(tsrc.i, 9), 9);
	tsrc.i = _mm_or_si128(tsrc.i, mask);
	tsrc.i = _mm_or_si128(tsrc.i, sign);
	
	tsrc.i = _mm_andnot_si128(eq_zero, tsrc.i );
	exp = _mm_andnot_si128(eq_zero, exp );
		
	return tsrc.f ;
}

/*
 * Equivalent of frexp in math.h, which splits a normal float number into a power of 2 and a mantissa.
 * src = 2^e * m, where 0.5 <= m <= 1
 * Input: double precision "src" 
 * Return value: mantissa m
 * Note: Inputs of INF and NAN are not handled
 */
CBL_INLINE __m128d CBL_SSE2::Frexp(const __m128d &src, __m128i &exp)
{
	XMM128 tsrc, eq_zero ;
	tsrc.d = src ;	
	eq_zero.d = _mm_cmpeq_pd(tsrc.d, _mm_setzero_pd());

	__m128i sign = _mm_and_si128(tsrc.i, CBL_Library_Internal::__g_min_S64_vector);	
	tsrc.i = _mm_xor_si128(tsrc.i, sign);			//strip sign off
	
	__m128i mask = _mm_slli_epi64(_mm_srli_epi64(_mm_cmpeq_epi32(tsrc.i, tsrc.i), 55), 53);

	exp = _mm_sub_epi64(tsrc.i, mask);
	exp = _mm_srli_epi64(exp, 52);					//get power of 2 into exp
	
	tsrc.i = _mm_srli_epi64(_mm_slli_epi64(tsrc.i, 12), 12);
	tsrc.i = _mm_or_si128(tsrc.i, mask);
	tsrc.i = _mm_or_si128(tsrc.i, sign);			// get mantissa in tsrc.i
	
	tsrc.i = _mm_andnot_si128(eq_zero.i , tsrc.i );
	exp = _mm_andnot_si128(eq_zero.i, exp );		
		
	return tsrc.d ;
}

/*
 *   Prep functions corresponding to each _A11 functions
 */

CBL_INLINE void CBL_SSE2::Prep_sqrt_A11( sqrt_A11_Data &d)
{
    CBL_Library_Internal::invSqrtf_setup(d.half.f, d.three_by_2.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_invSqrt_A11( invSqrt_A11_Data &d)
{
    CBL_Library_Internal::invSqrtf_setup(d.half.f, d.three_by_2.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_cbrt_A11( cbrt_A11_Data &d)
{
    CBL_Library_Internal::cbrt_A11_setup(d.init_const.i, d.one_by_3.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_invCbrt_A11( invCbrt_A11_Data &d)
{
    CBL_Library_Internal::inv_cbrtf_setup(d.init_const.i, d.four_by_3.f, d.one_by_3.f, d.four_by_3D.d)	;		
}

CBL_INLINE void CBL_SSE2::Prep_exp_A11( exp_A11_Data &d)
{
    CBL_Library_Internal::exp_A11_setup(d.thirtytwo_by_ln2.f, d.ln2_by_32.f, d.A1_A2.f, d.max_min_arg.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_ln_A11( ln_A11_Data &d)
{
    CBL_Library_Internal::ln_A11_setup(d.half_sqhalf.f, d.p5.f, d.p6_p7.f, d.p8_p9.f, d.const1_const2.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_log_A11( log_A11_Data &d)
{
    CBL_Library_Internal::ln_A11_setup(d.half_sqhalf.f, d.p5.f, d.p6_p7.f, d.p8_p9.f, d.const1_const2.f)	;		
}

CBL_INLINE void CBL_SSE2::Prep_pow_A11( pow_A11_Data &d)
{
    CBL_Library_Internal::ln_32f_tang_setup(d.ln2_lead.f, d.ln2_trail.f, d.A1_A2.f);
}
/* END of Prep functions */

//! Calculate square root with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::sqrt_A11( const __m128 &src, const sqrt_A11_Data &d )
{   
    XMM128 tSrc, tDst;
    tSrc.f = src ;
    CBL_Library_Internal::Sqrt_A11_sse2(tSrc, tDst, d.half.f, d.three_by_2.f);
    return tDst.f ;
}

//! Calculate inverse square root with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::invSqrt_A11( const __m128 &src,  const invSqrt_A11_Data &d)
{
    XMM128 tSrc, tDst;
    tSrc.f = src ;
    CBL_Library_Internal::InvSqrt_A11_sse2(tSrc, tDst, d.half.f, d.three_by_2.f);
    return tDst.f ;
}

//! Calculate cube root with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::cbrt_A11( const __m128 &src,  const cbrt_A11_Data &d )
{
    XMM128 tSrc, tDst;
    tSrc.f = src;
    CBL_Library_Internal::Cbrt_A11_sse2(tSrc, tDst, d.init_const.i, d.one_by_3.f);
    return tDst.f ;
}

//! Calculate inverse Cube root with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::invCbrt_A11( const __m128 &src, const invCbrt_A11_Data &d )
{
    XMM128 tSrc, tDst;
    tSrc.f = src ;
    CBL_Library_Internal::InvCbrt_A11_sse2(tSrc, tDst, d.init_const.i, d.four_by_3.f, d.one_by_3.f);
    return tDst.f ;
}

//! Calculate exponential  with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::exp_A11( const __m128 &src, const exp_A11_Data &d )
{
    XMM128 tSrc, tDst;
    tSrc.f = src;
    CBL_Library_Internal::Exp_A11_sse(tSrc, tDst, d.thirtytwo_by_ln2.f, d.ln2_by_32.f, d.A1_A2.f, d.max_min_arg.f) ;   
    return tDst.f ;
}

//! Calculate natural logarithm ( base “e”) with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::ln_A11( const __m128 &src, const ln_A11_Data &d )
{
    XMM128 tSrc, tDst;
    tSrc.f = src;
    CBL_Library_Internal::Ln_A11_sse2(tSrc, tDst, d.half_sqhalf.f, d.p5.f, d.p6_p7.f, d.p8_p9.f,	d.const1_const2.f) ;   
    return tDst.f ;
}

//! Calculate common Logarithm (base 10) with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::log_A11( const __m128 &src, const log_A11_Data &d )
{
    XMM128 tSrc, tDst;
    tSrc.f = src;
    CBL_Library_Internal::Log10_A11_sse2(tSrc, tDst, d.half_sqhalf.f, d.p5.f, d.p6_p7.f, d.p8_p9.f, d.const1_const2.f) ;   
    return tDst.f ;
}

//! Calculate x**y with 11 bits of accuracy
CBL_INLINE __m128 CBL_SSE2::pow_A11( const __m128 &src1, const __m128 &y, const pow_A11_Data &d )
{
    XMM128 tSrc1, tSrc2, tDst;

    tSrc1.f = src1;    tSrc2.f = y;
    CBL_Library_Internal::Pow_A11_sse2(tSrc1, tSrc2, tDst, d.ln2_lead.f, d.ln2_trail.f, d.A1_A2.f) ;   
    return tDst.f ;
}

/// @endcond

#endif

