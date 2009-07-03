/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/



#ifndef __CBL_MATH_PRIMITIVES_H__
#define __CBL_MATH_PRIMITIVES_H__

#include <math.h>

namespace CBL_Library_Internal
{
#if defined(CBL_MSVC) 
    static float cbrt_custom(float x) {
	    if (x == 0.0) return(0.0);
	    else {
		    if (x < 0.0)	return (float)(-pow((double)-x, (double)0.333333333333333333));
		    else			return (float)(pow((double)x, (double)0.333333333333333333));
	    }
    }

     static float inv_cbrt_custom(float x) {
        if (x == 0.0) {
            const U32 INF_32Fi = 0x7F800000;
            return *(float*) &INF_32Fi; 
        }
	    else {
		    if (x < 0.0)	return (float)(-pow((double)-x, (double)-0.333333333333333333));
		    else			return (float)(pow((double)x, (double)-0.333333333333333333));
	    }
    }
#else
    static float cbrt_custom(float x)
    {
	    return cbrtf(x);
    }
    static float inv_cbrt_custom(float x)
    {
	    return 1.0/cbrtf(x);
    }
#endif
}

/*!
 Equivalent of frexp in math.h, which splits a floating point number into a power of 2 and a mantissa.
 \par Parameters:
 \param[in] val - 128 bit variable containing two double precision floating point values to be split
 \param[out] exp - 128 bit variable to store the integer exponents
 \return the mantissa value m
 - val = 2^e * m, where 0.5 <= m <= 1
 */
CBL_INLINE __m128d CBL_REF::Frexp(const __m128d &val, __m128i &exp)
{
    XMM128 mantissa;
    
    const XMM128 &value = reinterpret_cast<const XMM128&>(val);
    XMM128 &e		= reinterpret_cast<XMM128&>(exp);
    
    e.s32[1] = 0;
    e.s32[3] = 0;
    mantissa.f64[0] = frexp(value.f64[0], &e.s32[0]);
    mantissa.f64[1] = frexp(value.f64[1], &e.s32[2]);

    return mantissa.d;
}
#pragma warning (push)
#pragma warning (disable : 4701)
/*!
 Equivalent of frexp in math.h, which splits a floating number into a power of 2 and a mantissa.
 \par Parameters:
 \param[in] val - 128 bit variable containing four single precision floating point values to be split
 \param[out] exp - 128 bit variable to store the integer exponents
 \return the mantissa value m
 - val = 2^e * m, where 0.5 <= m <= 1
 */
CBL_INLINE __m128 CBL_REF::Frexpf(const __m128 &val, __m128i &exp)
{
    XMM128 mantissa;

    const XMM128 &value = reinterpret_cast<const XMM128&>(val);
    XMM128 &e		= reinterpret_cast<XMM128&>(exp);

    for(int i = 0 ; i < 4 ; i++)
        mantissa.f32[i] = frexpf(value.f32[i], &e.s32[i]);
    return mantissa.f;
}
 
/*!
 Calculates square root with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_sqrt_A11().
 \return the square root of src
 - result = sqrt(src)
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_sqrt_A11()
 */
CBL_INLINE __m128 CBL_REF::sqrt_A11(const __m128 &src, const sqrt_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = sqrt(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates inverse square root with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_invSqrt_A11().
 \return the inverse square root of src
 - result = 1 / sqrt(src)
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_invSqrt_A11()
 */
CBL_INLINE __m128 CBL_REF::invSqrt_A11(const __m128 &src, const invSqrt_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = 1.0f/sqrt(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates cube root with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_cbrt_A11().
 \return the cube root of src
 - result = cube_root(src)
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_cbrt_A11()
 */
CBL_INLINE __m128 CBL_REF::cbrt_A11 (const __m128 &src, const cbrt_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = CBL_Library_Internal::cbrt_custom(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates inverse cube root with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_invCbrt_A11().
 \return the inverse cube root of src
 - result = 1 / cube_root(src)
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_invCbrt_A11()
 */
CBL_INLINE __m128 CBL_REF::invCbrt_A11(const __m128 &src, const invCbrt_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
        dst.f32[i] = CBL_Library_Internal::inv_cbrt_custom(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates the exponential with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_exp_A11().
 \return exponential of src
 - result = e<sup>src</sup>
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_exp_A11()
 */
CBL_INLINE __m128 CBL_REF::exp_A11  (const __m128 &src, const exp_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = expf(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates the natural logarithm (base 'e') with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_ln_A11().
 \return natural logarithm of src
 - result = ln( src )
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_ln_A11()
 */
CBL_INLINE __m128 CBL_REF::ln_A11  (const __m128 &src, const ln_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = log(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates the common logarithm (base 10) with 11 bits of precision
 \par Parameters:
 \param[in] src - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_log_A11().
 \return natural logarithm of src
 - result = log<sub>10</sub>( src )
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_log_A11()
 */
CBL_INLINE __m128 CBL_REF::log_A11  (const __m128 &src, const log_A11_Data & /*d*/)
{
    const XMM128 &tSrc = reinterpret_cast<const XMM128&>(src);
    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = log10(tSrc.f32[i]);
    return dst.f;
}

/*!
 Calculates the power with 11 bits of precision
 \par Parameters:
 \param[in] src1 - 128 bit variable containing four single precision floating point values
 \param[out] d - intermediate data obtained by a prior call to Prep_pow_A11().
 \return power of src1 to src2
 - result = src1<sup>src2</sup>
 \par NOTE:
 d is not used in the reference implementation
 \par See Also:
 Prep_pow_A11()
 */
CBL_INLINE __m128 CBL_REF::pow_A11  (const __m128 &src1, const __m128 &src2, const pow_A11_Data & /*d*/)
{
    const XMM128 &tSrc1 = reinterpret_cast<const XMM128&>(src1);
    const XMM128 &tSrc2 = reinterpret_cast<const XMM128&>(src2);

    XMM128 dst;
    for(int i=0 ; i < 4 ; i++)
       dst.f32[i] = powf(tSrc1.f32[i], tSrc2.f32[i]);
    return dst.f;
}
#pragma warning (pop)
#endif // __CBL_MATH_PRIMITIVES_H__

