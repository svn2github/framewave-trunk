/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_ARITHMETIC_PRIMITIVES_H__
#define __CBL_ARITHMETIC_PRIMITIVES_H__

// scale routines

/*! 
    Initializes a scale mask array for subsequent calls to Scale_16s().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  scale the scale factor
    \param[out] scaleMasksp  mask array
    \return none
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Scale_16s()
*/
template <int scaleType>
CBL_INLINE void CBL_REF::Setup_ScaleMasks_16s(const int &scale, __m128i scaleMasksp[])
{
	XMM128 * scaleMasks = reinterpret_cast<XMM128 *>(scaleMasksp);

	U16 mask1, mask2;
	switch (scaleType)
	{
	case 1:
		mask1 = 1 << ((U16) scale - 1);
		mask2 = 1 << ((U16) scale);

		CBL_REF::Load_1x8_16u( scaleMasks[3].i, mask2 );

	    //scaleMasks[2]	= _mm_cmpeq_epi16(scaleMasks[3], scaleMasks[3]);	// all f's
	    //scaleMasks[2]	= _mm_srli_epi16(scaleMasks[2], 16 - scale - 1);
        for( int n = 0; n < 8; n++ )
        {
            scaleMasks[2].u16[n] = 0xFFFF;
            scaleMasks[2].u16[n] >>= 16 - scale - 1;
        }

		CBL_REF::Load_1x8_16u( scaleMasks[1].i, mask1 );
		//scaleMasks[0]	= _mm_set_epi32(0, 0, 0, scale);
        scaleMasks[0].s32[0] = scale;
        scaleMasks[0].s32[1] = 0;
        scaleMasks[0].s32[2] = 0;
        scaleMasks[0].s32[3] = 0;
		break;
	case -1:
		//scaleMasks[0]	= _mm_set_epi32(0, 0, 0, -scale);
        scaleMasks[0].s32[0] = -scale;
        scaleMasks[0].s32[1] = 0;
        scaleMasks[0].s32[2] = 0;
        scaleMasks[0].s32[3] = 0;
		break;
	default:
		break;
	}
}

/*! 
    Initializes a scale mask array for subsequent calls to Scale_32s().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  scale the scale factor
    \param[out] scaleMasksp  mask array
    \return none
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Scale_32s()
*/
template <int scaleType>
CBL_INLINE void CBL_REF::Setup_ScaleMasks_32s(const int &scale, __m128i scaleMasksp[])
{
	XMM128 * scaleMasks = reinterpret_cast<XMM128 *>(scaleMasksp);

	U32 mask1, mask2;

	switch (scaleType)
	{
	case 1:
		mask1 = 1 << ((U32) scale - 1);
		mask2 = 1 << ((U32) scale);

		CBL_REF::Load_1x4_32u( scaleMasks[3].i, mask2 );

		//scaleMasks[2]	= _mm_cmpeq_epi32(scaleMasks[3], scaleMasks[3]);	// all f's
		//scaleMasks[2]	= _mm_srli_epi32(scaleMasks[2], 32 - scale - 1);
        for( int n = 0; n < 4; n++ )
        {
            scaleMasks[2].u32[n] = 0xFFFFFFFF;
            scaleMasks[2].u32[n] >>= 32 - scale - 1;
        }

		CBL_REF::Load_1x4_32u( scaleMasks[1].i, mask1 );
		//scaleMasks[0]	= _mm_set_epi32(0, 0, 0, scale);
        scaleMasks[0].s32[0] = scale;
        scaleMasks[0].s32[1] = 0;
        scaleMasks[0].s32[2] = 0;
        scaleMasks[0].s32[3] = 0;
		break;
	case -1:
		//scaleMasks[0] = _mm_set_epi32(0, 0, 0, -scale);
        scaleMasks[0].s32[0] = -scale;
        scaleMasks[0].s32[1] = 0;
        scaleMasks[0].s32[2] = 0;
        scaleMasks[0].s32[3] = 0;
		break;
	default:
		break;
	}
}

/*! 
    Initializes a scale mask array for subsequent calls to Scale_32f().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  scale the scale factor
    \param[out] scaleMasksp  mask array
    \return none
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Scale_32f()
*/
template <int scaleType>
CBL_INLINE void CBL_REF::Setup_ScaleMasks_32f(const int &scale, __m128 scaleMasksp[])
{
	XMM128 * scaleMasks = reinterpret_cast<XMM128 *>(scaleMasksp);
	switch (scaleType)
	{
	case 1:
	case -1:
		//scaleMasks[0] = _mm_set1_ps(pow((float)2.0, -scale));
        for( int n = 0; n < 4; n++ )
        {
            scaleMasks[0].f32[n] = pow((float)2.0, -scale);
        }
		break;
	default:
		//scaleMasks[0] = _mm_set1_ps(1.0);
        for( int n = 0; n < 4; n++ )
        {
            scaleMasks[0].f32[n] = 1.0;
        }
		break;
	}
}

#pragma warning( push )
#pragma warning( disable : 4701 )

/*! 
    Performs scalling using a previously calculated caleMask obtained by a previous call to Setup_ScaleMasks_16s().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  value the value to be scaled
    \param[in] scaleMasksp  mask array
    \return scaling result obtained by using the following formula: result = value * 2 ^ ( - scale )
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Setup_ScaleMasks_16s()
*/
template <int scaleType>
CBL_INLINE __m128i CBL_REF::Scale_16s(const __m128i &value, const __m128i scaleMasksp[])
{
	// scaleMasks are,
	// scaleMasks[0]	= scale value
	// scaleMasks[1-3]	= scale masks used by Gongyuans's integer scaling algorithm

	const XMM128 * scaleMasks = reinterpret_cast<const XMM128 *>(scaleMasksp);
	const XMM128 & valueArray = reinterpret_cast<const XMM128 &>(value);
	S16 mask;
    XMM128 temp;

	switch (scaleType)
	{
	case 1:
		//temp	= _mm_add_epi16(value, scaleMasks[1]);
		//mask	= _mm_and_si128(temp,  scaleMasks[2]);
		//mask	= _mm_cmpeq_epi16(mask, scaleMasks[3]);
		//temp	= _mm_srl_epi16(temp, scaleMasks[0]);
		//return	  _mm_add_epi16(temp, mask);
        for( int n = 0; n < 8; n++ )
        {
            temp.s16[n] = valueArray.s16[n] + scaleMasks[1].s16[n];
            mask = temp.u16[n] & scaleMasks[2].u16[n];
            mask = ( mask == scaleMasks[3].u16[n] ) ? 0xFFFF : 0x0;
            temp.u16[n] = temp.u16[n] >> scaleMasks[0].s16[0];
            temp.s16[n] = temp.s16[n] + mask;
        }
        return temp.i;
	case -1:
		//return	  _mm_sll_epi16(value, scaleMasks[0]);
        for( int n = 0; n < 8; n++ )
        {
            temp.u16[n] = valueArray.u16[n] << scaleMasks[0].s16[0];
        }
        return temp.i;
	default:	// fall through to 0 scale otherwise
		return value;
	}
}

/*! 
    Performs scalling using a previously calculated caleMask obtained by a previous call to Setup_ScaleMasks_32s().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  value the value to be scaled
    \param[in] scaleMasksp  mask array
    \return scaling result obtained by using the following formula: result = value * 2 ^ ( - scale )
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Setup_ScaleMasks_32s()
*/
template <int scaleType>
CBL_INLINE __m128i CBL_REF::Scale_32s(const __m128i &value, const __m128i scaleMasksp[])
{
	const XMM128 * scaleMasks = reinterpret_cast<const XMM128 *>(scaleMasksp);
	const XMM128 & valueArray = reinterpret_cast<const XMM128 &>(value);
	S32 mask;
    XMM128 temp;

	switch (scaleType)
	{
	case 1:
		//temp	= _mm_add_epi32(value, scaleMasks[1]);
		//mask	= _mm_and_si128(temp,  scaleMasks[2]);
		//mask	= _mm_cmpeq_epi32(mask, scaleMasks[3]);
		//temp	= _mm_srl_epi32(temp, scaleMasks[0]);
		//return	  _mm_add_epi32(temp, mask);
        for( int n = 0; n < 4; n++ )
        {
            temp.s32[n] = valueArray.s32[n] + scaleMasks[1].s32[n];
            mask = temp.s32[n] & scaleMasks[2].s32[n];
            mask = ( mask == scaleMasks[3].s32[n] ) ? 0xFFFFFFFF : 0x0;
            temp.u32[n] = temp.u32[n] >> scaleMasks[0].s32[0];
            temp.s32[n] = temp.s32[n] + mask;
        }
        return temp.i;
	case -1:
		//return	  _mm_sll_epi32(value, scaleMasks[0]);
        for( int n = 0; n < 4; n++ )
        {
            temp.u32[n] = valueArray.u32[n] << scaleMasks[0].s32[0];
        }
        return temp.i;
	default:	// fall through to 0 scale otherwise
		return value;
	}
}

/*! 
    Performs scalling using a previously calculated caleMask obtained by a previous call to Setup_ScaleMasks_32f().
    \par Template Parameters:
    \e scaleType scaling mode; use 1 for positive scale, -1 for negative scale, 0 otherwise
    \param[in]  value the value to be scaled
    \param[in] scaleMasksp  mask array
    \return scaling result obtained by using the following formula: result = value * 2 ^ ( - scale )
	\par Notes:
    Always match the corret scale setup routine with the scale routine.
	Using the template parameter gaurantees that that the correct scaling
	will be performed, as long as the setup matches the scale function
	\par See also:
    Setup_ScaleMasks_32f()
*/
template <int scaleType>
CBL_INLINE __m128 CBL_REF::Scale_32f(const __m128 &value, const __m128 scaleMasksp[])
{
	const XMM128 * scaleMasks = reinterpret_cast<const XMM128 *>(scaleMasksp);
	const XMM128 & valueArray = reinterpret_cast<const XMM128 &>(value);
    XMM128 temp;

    switch (scaleType)
	{
	case 1:
	case -1:
		//return _mm_mul_ps(value, scaleMasks[0]);
        for( int n = 0; n < 4; n++ )
        {
            temp.f32[n] = valueArray.f32[n] * scaleMasks[0].f32[n];
        }
        return temp.f;
	default:	// fall through to 0 scale otherwise
		return value;
	}
}
#pragma warning( pop )

// division by a constant routines

#pragma warning( push )
#pragma warning( disable : 4701 )
/*!
    Performs division on signed 16 bit integer values using previously calculated constants m, s 
    and codePath obtained by a previous call to PrepDiv_16s_const().
    \par Parameters:
    \param[in] d dividend
    \param[in] mm precalculated constant obtained by calling PrepDiv_16s_const()
    \param[in] ss not used by the reference implementation
    \param[in] executePath constant value obtained by calling PrepDiv_16s_const()
    \return result (quotient) of the division by a constant passed to the PrepDiv_16s_const() call
    \n result[0] = dividend[0] / const
    \n result[1] = dividend[1] / const
    \n result[2] = dividend[2] / const
    \n result[3] = dividend[3] / const
    \n result[4] = dividend[4] / const
    \n result[5] = dividend[5] / const
    \n result[6] = dividend[6] / const
    \n result[7] = dividend[7] / const

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
CBL_INLINE __m128i CBL_REF::Divide_16s_const( __m128i & d, const __m128i & mm, __m128i, U8 executePath )
{
	XMM128 vd = reinterpret_cast<XMM128&>(d);
	XMM128 vc = reinterpret_cast<const XMM128&>(mm);
	XMM128 result;
	if( executePath == 0 )
	{
		for( int n = 0; n < 8; n++ )
			result.s16[n] = 0;
	}
	else
	{
		for( int n = 0; n < 8; n++ )
			result.s16[n] = vd.s16[n] / vc.s16[n];
	}
	return result.i;
}
#pragma warning( pop )

/*!
    Precalculates constants used during division on signed 16 bit integer values in subsequent call to Divide_16s_const.
    \par Parameters:
    \param[in] divisor - the divisor value
    \param[out] mm precalculated constant
    \param[out] ss not used in reference implementation
    \return codePath value indicating which algorithm to use in the Divide_16s_const() call.
    \par Notes:
    Always match the correct PrepDiv_16s_const() call with the Divide_16s_const() function.
    The reference implementation of the two functions uses a different algorithm than the SSE2 implementation.  
    Mixing of the reference and SSE2 versions of the two functions is not supported.
	\par See also:
    Divide_16s_const()
*/
CBL_INLINE U8 CBL_REF::PrepDiv_16s_const( __m128i & mm, __m128i & , S16 divisor )
{
	XMM128 * pmm = reinterpret_cast<XMM128*>(&mm);

	for( int n = 0; n < 8; n++ )
		pmm->s16[n] = divisor;

	if( divisor == 0 )
		return 0;
	else
		return 1;
}

#pragma warning( push )
#pragma warning( disable: 4701 )
/*!
    Perform fast 16 bit signed division by a constant
    \par Parameters:
    \param[in] dividend - the dividend value
    \param[in] operand - a precalculated constant obtained by a previous call to PrepFastDivide_16s_const().
    \return result of the signed 16 bit division by a constant passed to the PrepFastDivide_16s_const().
        \n result[0] = dividend[0] / const
        \n result[1] = dividend[1] / const
        \n result[2] = dividend[2] / const
        \n result[3] = dividend[3] / const
        \n result[4] = dividend[4] / const
        \n result[5] = dividend[5] / const
        \n result[6] = dividend[6] / const
        \n result[7] = dividend[7] / const

    \par Notes: 
    This implementation sacrafices accuracy for performance.  It performs the division 
      in just one 2 cycle instruction, but the results have a margin of error of +/- 1.5 units.
    \par See also: 
    PrepFastDivide_16s_const()
*/
CBL_INLINE __m128i CBL_REF::FastDivide_16s_const( const __m128i & dividend, const __m128i & operand )
{
    XMM128 result;
    const XMM128 & div = reinterpret_cast<const XMM128 &>( dividend );
    const XMM128 & op = reinterpret_cast<const XMM128 &>( operand );
    for( int n = 0; n < 8; n++ )
    {
        result.s16[n] = static_cast<S16>( ( static_cast<S32>(div.s16[n]) * static_cast<S32>(op.s16[n]) ) >> 16 );
    }
    return result.i;
}
#pragma warning( pop )

/*!
    Precalculates constant values used in fast 16 bit signed division by subsequent call to FastDivide_16s_const().
    \par Parameters:
    \param[out] operand - a precalculated constant to be used in a subsequent call to FastDivide_16s_const().
    \param[in] divisor - the divisor value
    \return none
    \par See also:
    FastDivide_16s_const()
*/
CBL_INLINE void CBL_REF::PrepFastDivide_16s_const( __m128i & operand, S16 divisor )
{
    XMM128 & op = reinterpret_cast<XMM128 &>( operand );
    for( int n = 0; n < 8; n++ )
    {
        op.s16[n] = static_cast<S16>(65535 / divisor);
    }
}

/*!
    Precalculates constant values used in fast 16 bit signed division by subsequent call to FastDivide_16s_const().
    \par Parameters:
    \param[out] operand - a precalculated constant to be used in a subsequent call to FastDivide_16s_const().
    \param[in] divisor - the divisor value
    \return none
    \par Notes:
    this version of the PrepFastDivide_16s_const() function allows subsequent division by 8 different constant values.
    Instead of a single divisor value, this function takes a 128 bit value containing eight different divisor values.
    The dividend values will be divided by the corresponding divisor values.
    \par See also:
    FastDivide_16s_const()
*/
CBL_INLINE void CBL_REF::PrepFastDivide_16s_const( __m128i & operand, const __m128i & divisor )
{
    const XMM128 & div = reinterpret_cast<const XMM128 &>( divisor );
    XMM128 & op = reinterpret_cast<XMM128 &>( operand );
    for( int n = 0; n < 8; n++ )
    {
        op.s16[n] = static_cast<S16>(65536 / div.s16[n]);
    }
}



/*!
    Calculates the magnitude of a 16 bit signed complex number and returns the magnitude in float
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \return the magnitude of 16 bit signed complex numbers; the result is floating point
        \n result[0] = magnitude[(src[0], src[1])]
        \n result[1] = magnitude[(src[2], src[3])]
        \n result[2] = magnitude[(src[4], src[5])]
        \n result[3] = magnitude[(src[6], src[7])]
*/
CBL_INLINE __m128	CBL_REF::Magnitude_16sc_32f(const __m128i & src, const __m128i & min32s)
{
    XMM128 r,r1;
    const XMM128 & val = reinterpret_cast<const XMM128&>( src );
    min32s;

	r.f32[0] = (F32)((S32) val.s16[0] * val.s16[0]) + (F32) ((S32) val.s16[1] * val.s16[1]);
	r.f32[1] = (F32)((S32) val.s16[2] * val.s16[2]) + (F32) ((S32) val.s16[3] * val.s16[3]);
	r.f32[2] = (F32)((S32) val.s16[4] * val.s16[4]) + (F32) ((S32) val.s16[5] * val.s16[5]);
	r.f32[3] = (F32)((S32) val.s16[6] * val.s16[6]) + (F32) ((S32) val.s16[7] * val.s16[7]);

	r1.f32[0] =  sqrt(r.f32[0]);
	r1.f32[1] =  sqrt(r.f32[1]);
	r1.f32[2] =  sqrt(r.f32[2]);
	r1.f32[3] =  sqrt(r.f32[3]);

	return r1.f;
}

/*!
    Multiplies four 16 bit complex values and returns the result in 32 bit integer complex
    \par Parameters:
    \param[in,out] src1DstLo - 128 bit operand containing 4 signed 16 bit complex values and
    \ contains the first two of the 32 bit complex values returned
        \n src1DstLo[0] = (src1DstLo[0] * src2[0]) [real]
        \n src1DstLo[1] = (src1DstLo[0] * src2[0]) [imaginary]
        \n src1DstLo[2] = (src1DstLo[1] * src2[1]) [real]
        \n src1DstLo[3] = (src1DstLo[1] * src2[1]) [imaginary]
    \param[in] src2   - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \param[in] negb   - 128 bit operand a special constant required for this function
    \return the third and the fourth of the 32 bit complex values returned
        \n result[0] = (src1DstLo[2] * src2[2]) [real]
        \n result[1] = (src1DstLo[2] * src2[2]) [imaginary]
        \n result[2] = (src1DstLo[3] * src2[3]) [real]
        \n result[3] = (src1DstLo[3] * src2[3]) [imaginary]
*/
CBL_INLINE __m128i CBL_REF::Multiply_16sc_32sc(__m128i & src1DstLo, const __m128i & src2, const __m128i & min32s, const __m128i & negb)
{
    XMM128 r1,r2;
    XMM128 & v1 = reinterpret_cast< XMM128&>( src1DstLo );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
	min32s;
	negb;
	
	r1.s32[0] = Limits<S32>::Sat(((S64)((S32)v1.s16[0] * v2.s16[0])) - ((S64)((S32)v1.s16[1] * v2.s16[1])));
	r1.s32[1] = Limits<S32>::Sat(((S64)((S32)v1.s16[0] * v2.s16[1])) + ((S64)((S32)v1.s16[1] * v2.s16[0])));
	r1.s32[2] = Limits<S32>::Sat(((S64)((S32)v1.s16[2] * v2.s16[2])) - ((S64)((S32)v1.s16[3] * v2.s16[3])));
	r1.s32[3] = Limits<S32>::Sat(((S64)((S32)v1.s16[2] * v2.s16[3])) + ((S64)((S32)v1.s16[3] * v2.s16[2])));

	r2.s32[0] = Limits<S32>::Sat(((S64)((S32)v1.s16[4] * v2.s16[4])) - ((S64)((S32)v1.s16[5] * v2.s16[5])));
	r2.s32[1] = Limits<S32>::Sat(((S64)((S32)v1.s16[4] * v2.s16[5])) + ((S64)((S32)v1.s16[5] * v2.s16[4])));
	r2.s32[2] = Limits<S32>::Sat(((S64)((S32)v1.s16[6] * v2.s16[6])) - ((S64)((S32)v1.s16[7] * v2.s16[7])));
	r2.s32[3] = Limits<S32>::Sat(((S64)((S32)v1.s16[6] * v2.s16[7])) + ((S64)((S32)v1.s16[7] * v2.s16[6])));

	v1 = r1;

	return r2.i;
}

/*!
    Multiplies four 16 bit complex values
    \par Parameters:
    \param[in] src1   - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] src2   - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \param[in] negb   - 128 bit operand a special constant required for this function
    \return the result of the complex multiplication between the 16 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
        \n result[2] = (src1[2] * src2[2])
        \n result[3] = (src1[3] * src2[3])
*/
CBL_INLINE __m128i CBL_REF::Multiply_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s, const __m128i & negb)
{
    XMM128 r1;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
	min32s;
	negb;
	
	r1.s16[0] = Limits<S16>::Sat(((S64)((S32)v1.s16[0] * v2.s16[0])) - ((S64)((S32)v1.s16[1] * v2.s16[1])));
	r1.s16[1] = Limits<S16>::Sat(((S64)((S32)v1.s16[0] * v2.s16[1])) + ((S64)((S32)v1.s16[1] * v2.s16[0])));
	r1.s16[2] = Limits<S16>::Sat(((S64)((S32)v1.s16[2] * v2.s16[2])) - ((S64)((S32)v1.s16[3] * v2.s16[3])));
	r1.s16[3] = Limits<S16>::Sat(((S64)((S32)v1.s16[2] * v2.s16[3])) + ((S64)((S32)v1.s16[3] * v2.s16[2])));

	r1.s16[4] = Limits<S16>::Sat(((S64)((S32)v1.s16[4] * v2.s16[4])) - ((S64)((S32)v1.s16[5] * v2.s16[5])));
	r1.s16[5] = Limits<S16>::Sat(((S64)((S32)v1.s16[4] * v2.s16[5])) + ((S64)((S32)v1.s16[5] * v2.s16[4])));
	r1.s16[6] = Limits<S16>::Sat(((S64)((S32)v1.s16[6] * v2.s16[6])) - ((S64)((S32)v1.s16[7] * v2.s16[7])));
	r1.s16[7] = Limits<S16>::Sat(((S64)((S32)v1.s16[6] * v2.s16[7])) + ((S64)((S32)v1.s16[7] * v2.s16[6])));

	return r1.i;
}

/*!
    Multiplies two 32 bit float complex values
    \par Parameters:
    \param[in] src1   - 128 bit operand containing two 32 bit complex values
    \param[in] src2   - 128 bit operand containing two 32 bit complex values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128 CBL_REF::Multiply_32fc( const __m128 & multiplicand, const __m128 & multiplier)
{
    XMM128 r1;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( multiplier );

	r1.f32[0] = (v1.f32[0] * v2.f32[0]) - (v1.f32[1] * v2.f32[1]);
	r1.f32[1] = (v1.f32[0] * v2.f32[1]) + (v1.f32[1] * v2.f32[0]);
	r1.f32[2] = (v1.f32[2] * v2.f32[2]) - (v1.f32[3] * v2.f32[3]);
	r1.f32[3] = (v1.f32[2] * v2.f32[3]) + (v1.f32[3] * v2.f32[2]);
	
	return r1.f;
}

/*!
    Multiplies one 64 bit float complex values
    \par Parameters:
    \param[in] src1   - 128 bit operand containing one 64 bit complex values
    \param[in] src2   - 128 bit operand containing one 64 bit complex values
    \return the result of the complex multiplication between the 64 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128d CBL_REF::Multiply_64fc( const __m128d & multiplicand, const __m128d & multiplier)
{
    XMM128 r1;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( multiplier );

	r1.f64[0] = (v1.f64[0] * v2.f64[0]) - (v1.f64[1] * v2.f64[1]);
	r1.f64[1] = (v1.f64[0] * v2.f64[1]) + (v1.f64[1] * v2.f64[0]);
	
	return r1.d;
}


// horizontal add and subtract routines
CBL_INLINE __m128 CBL_REF::HorizontalAdd_32f( const __m128 & src1, const __m128 & src2 )
/*!
    Horizontal addition of single precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal additon of the two operands
    \par
        \n result[0:31]    = src1[0:31]    + src1[32:63]
        \n result[32:63]   = src1[64:95]   + src1[96:127]
        \n result[64:95]   = src2[0:31]    + src2[32:63]
        \n result[96:127]  = src2[64:95]   + src2[96:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f32[0] = v1.f32[0] + v1.f32[1];
    r.f32[1] = v1.f32[2] + v1.f32[3];
    r.f32[2] = v2.f32[0] + v2.f32[1];
    r.f32[3] = v2.f32[2] + v2.f32[3];
    return r.f;
}

CBL_INLINE __m128 CBL_REF::HorizontalSubtract_32f( const __m128 & src1, const __m128 & src2 )
/*!
    Horizontal subtraction of single precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal subtraction of the two operands
    \par
    \n result[0:31]    = src1[0:31]    - src1[32:63]
    \n result[32:63]   = src1[64:95]   - src1[96:127]
    \n result[64:95]   = src2[0:31]    - src2[32:63]
    \n result[96:127]  = src2[64:95]   - src2[96:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f32[0] = v1.f32[0] - v1.f32[1];
    r.f32[1] = v1.f32[2] - v1.f32[3];
    r.f32[2] = v2.f32[0] - v2.f32[1];
    r.f32[3] = v2.f32[2] - v2.f32[3];
    return r.f;
}

CBL_INLINE __m128 CBL_REF::AddAndSubtract_32f( const __m128 & src1, const __m128 & src2 )
/*!
    Horizontal addition and subtraction of single precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal additon and subtraction of the two operands
    \par
    \n result[0:31]    = src1[0:31]    - src2[0:31]
    \n result[32:63]   = src1[32:63]   + src2[32:63]
    \n result[64:95]   = src1[64:95]   - src2[64:95]
    \n result[96:127]  = src1[96:127]  + src2[96:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f32[0] = v1.f32[0] - v2.f32[0];
    r.f32[1] = v1.f32[1] + v2.f32[1];
    r.f32[2] = v1.f32[2] - v2.f32[2];
    r.f32[3] = v1.f32[3] + v2.f32[3];
    return r.f;
}

CBL_INLINE __m128d CBL_REF::HorizontalAdd_64f( const __m128d & src1, const __m128d & src2 )
/*!
    Horizontal addition of double precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal additon of the two operands
    \par
    \n result[0:63]    = src1[0:63]    + src1[64:127]
    \n result[64:127]  = src2[0:63]    + src2[64:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f64[0] = v1.f64[0] + v1.f64[1];
    r.f64[1] = v2.f64[0] + v2.f64[1];
    return r.d;
}

CBL_INLINE __m128d CBL_REF::HorizontalSubtract_64f( const __m128d & src1, const __m128d & src2 )
/*!
    Horizontal subtraction of double precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal subtraction of the two operands
    \par
    \n result[0:63]    = src1[0:63]    - src1[64:127]
    \n result[64:127]  = src2[0:63]    - src2[64:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f64[0] = v1.f64[0] - v1.f64[1];
    r.f64[1] = v2.f64[0] - v2.f64[1];
    return r.d;
}

CBL_INLINE __m128d CBL_REF::AddAndSubtract_64f( const __m128d & src1, const __m128d & src2 )
/*!
    Horizontal addition and subtraction of double precision floating point values
    \par Parameters:
    \param[in] src1 - the first operand
    \param[in] src2 - the second operand
    \return result of the horizontal additon and subtraction of the two operands
    \par
    \n result[0:63]    = src1[0:63]    - src2[0:63]
    \n result[64:127]  = src1[64:127]  + src2[64:127]
*/
{
    XMM128 r;
    const XMM128 & v1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & v2 = reinterpret_cast<const XMM128&>( src2 );
    r.f64[0] = v1.f64[0] - v2.f64[0];
    r.f64[1] = v1.f64[1] + v2.f64[1];
    return r.d;
}

/*!
    Horizontal addition of 16 bit signed integer values in a single register
    \par Parameters:
    \param[in] values - a 128 bit variable containing the 8 16 bit operands
    \return result of the horizontal additon 16 bit signed integer values within the operand
    \par
    result[0:15]    = values[0:15]  + values[16:31] + values[32:47]  + values[48:63]
                    + values[64:79] + values[80:95] + values[96:111] + values[112:127]
*/
CBL_INLINE void CBL_REF::HorizontalAdd_16s( __m128i & values )
{
    
	XMM128 & pSrc = reinterpret_cast< XMM128&>( values );

    pSrc.s16[0] = pSrc.s16[0] + pSrc.s16[1] + pSrc.s16[2] + pSrc.s16[3] 
                  + pSrc.s16[4] + pSrc.s16[5] + pSrc.s16[6] + pSrc.s16[7];
}

/*!
    Horizontal addition of 16 bit signed integer values in two registers
    \par Parameters:
    \param[in] value1 - first operand
    \param[in] value2 - second operand
    \return result of the horizontal additon 16 bit signed integer values within the operands
    \par
    \n result[0:15]    =   value1[0:15] +   value1[16:31]
    \n result[16:31]   =  value1[32:47] +   value1[48:63]
    \n result[32:47]   =  value1[64:79] +   value1[80:95]
    \n result[48:63]   = value1[96:111] + value1[112:127]
    \n result[64:79]   =   value2[0:15] +   value2[16:31]
    \n result[80:95]   =  value2[32:47] +   value2[48:63]
    \n result[96:111]  =  value2[64:79] +   value2[80:95]
    \n result[112:127] = value2[96:111] + value2[112:127]
*/
CBL_INLINE void CBL_REF::HorizontalAdd_16s( __m128i & value1, const __m128i & value2 )
{

	XMM128 & pSrc1 = reinterpret_cast< XMM128&>( value1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( value2 );

    pSrc1.s16[0] = pSrc1.s16[0] + pSrc1.s16[1];
    pSrc1.s16[1] = pSrc1.s16[2] + pSrc1.s16[3];
    pSrc1.s16[2] = pSrc1.s16[4] + pSrc1.s16[5];
    pSrc1.s16[3] = pSrc1.s16[6] + pSrc1.s16[7];
    pSrc1.s16[4] = pSrc2.s16[0] + pSrc2.s16[1];
    pSrc1.s16[5] = pSrc2.s16[2] + pSrc2.s16[3];
    pSrc1.s16[6] = pSrc2.s16[4] + pSrc2.s16[5];
    pSrc1.s16[7] = pSrc2.s16[6] + pSrc2.s16[7];
}

/*!
    Horizontal subtraction of 16 bit signed integer values in two registers
    \par Parameters:
    \param[in] value1 - first operand
    \param[in] value2 - second operand
    \return result of the horizontal subtraction 16 bit signed integer values within the operands
    \par
    \n result[0:15]    =   value1[0:15] -   value1[16:31]
    \n result[16:31]   =  value1[32:47] -   value1[48:63]
    \n result[32:47]   =  value1[64:79] -   value1[80:95]
    \n result[48:63]   = value1[96:111] - value1[112:127]
    \n result[64:79]   =   value2[0:15] -   value2[16:31]
    \n result[80:95]   =  value2[32:47] -   value2[48:63]
    \n result[96:111]  =  value2[64:79] -   value2[80:95]
    \n result[112:127] = value2[96:111] - value2[112:127]
*/
CBL_INLINE void CBL_REF::HorizontalSub_16s( __m128i & value1, const __m128i & value2 )
{
	XMM128 & pSrc1 = reinterpret_cast< XMM128&>( value1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( value2 );

    pSrc1.s16[0] = pSrc1.s16[0] - pSrc1.s16[1];
    pSrc1.s16[1] = pSrc1.s16[2] - pSrc1.s16[3];
    pSrc1.s16[2] = pSrc1.s16[4] - pSrc1.s16[5];
    pSrc1.s16[3] = pSrc1.s16[6] - pSrc1.s16[7];
    pSrc1.s16[4] = pSrc2.s16[0] - pSrc2.s16[1];
    pSrc1.s16[5] = pSrc2.s16[2] - pSrc2.s16[3];
    pSrc1.s16[6] = pSrc2.s16[4] - pSrc2.s16[5];
    pSrc1.s16[7] = pSrc2.s16[6] - pSrc2.s16[7];
}

/*!
    Horizontal addition of 32 bit signed integer values in register
    \par Parameters:
    \param[in] values - a 128 bit variable containing the 4 32 bit operands
    \return result of the horizontal additon 32 bit signed integer values in argument
    \par
    \n result[0:31] = values[0:31] + values[32:63] + values[64:95] + values[96:127]
*/
CBL_INLINE void CBL_REF::HorizontalAdd_32s( __m128i & values )
{
	XMM128 & pSrc1 = reinterpret_cast< XMM128&>( values );
    pSrc1.s32[0] = pSrc1.s32[0] + pSrc1.s32[1] + pSrc1.s32[2] + pSrc1.s32[3];
}

/*!
    Horizontal addition of 32 bit signed integer values in two registers
    \par Parameters:
    \param[in] value1 - first operand
    \param[in] value2 - second operand
    \return result of the horizontal additon 32 bit signed integer values within the operands
    \par
    \n result[0:31]   = value1[0:31]  + value1[32:63]
    \n result[32:63]  = value1[64:95] + value1[96:127]
    \n result[64:95]  = value2[0:31]  + value2[32:63]
    \n result[96:127] = value2[64:95] + value2[96:127]
*/
CBL_INLINE void CBL_REF::HorizontalAdd_32s( __m128i & value1, const __m128i & value2 )
{
	XMM128 & pSrc1 = reinterpret_cast< XMM128&>( value1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( value2 );

    pSrc1.s32[0] = pSrc1.s32[0] + pSrc1.s32[1];
    pSrc1.s32[1] = pSrc1.s32[2] + pSrc1.s32[3];
    pSrc1.s32[2] = pSrc2.s32[0] + pSrc2.s32[1];
    pSrc1.s32[3] = pSrc2.s32[2] + pSrc2.s32[3];
}

/*!
    Horizontal subtraction of 32 bit signed integer values in two registers
    \par Parameters:
    \param[in] value1 - first operand
    \param[in] value2 - second operand
    \return result of the horizontal subtraction 32 bit signed integer values within the operands
    \par
    \n result[0:31]   = value1[0:31]  - value1[32:63]
    \n result[32:63]  = value1[64:95] - value1[96:127]
    \n result[64:95]  = value2[0:31]  - value2[32:63]
    \n result[96:127] = value2[64:95] - value2[96:127]
*/
CBL_INLINE void CBL_REF::HorizontalSub_32s( __m128i & value1, const __m128i & value2 )
{
	XMM128 & pSrc1 = reinterpret_cast< XMM128&>( value1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( value2 );

    pSrc1.s32[0] = pSrc1.s32[0] - pSrc1.s32[1];
    pSrc1.s32[1] = pSrc1.s32[2] - pSrc1.s32[3];
    pSrc1.s32[2] = pSrc2.s32[0] - pSrc2.s32[1];
    pSrc1.s32[3] = pSrc2.s32[2] - pSrc2.s32[3];
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S8toS16( const __m128i & src )
/*!
    Horizontal addition of 16 signed 8 bit integer values into 8 signed 16 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 signed 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src[0:7]     + src[8:15]
    \n result[16:31]   = src[16:23]   + src[24:31]
    \n result[32:47]   = src[32:39]   + src[40:47]
    \n result[48:63]   = src[48:55]   + src[56:63]
    \n result[64:79]   = src[64:71]   + src[72:79]
    \n result[80:95]   = src[80:87]   + src[88:95]
    \n result[96:111]  = src[96:103]  + src[104:111]
    \n result[112:127] = src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s16[0] = (S16)pSrc.s8[0] + pSrc.s8[1];
	pDst.s16[1] = (S16)pSrc.s8[2] + pSrc.s8[3];
	pDst.s16[2] = (S16)pSrc.s8[4] + pSrc.s8[5];
	pDst.s16[3] = (S16)pSrc.s8[6] + pSrc.s8[7];
	pDst.s16[4] = (S16)pSrc.s8[8] + pSrc.s8[9];
	pDst.s16[5] = (S16)pSrc.s8[10] + pSrc.s8[11];
	pDst.s16[6] = (S16)pSrc.s8[12] + pSrc.s8[13];
	pDst.s16[7] = (S16)pSrc.s8[14] + pSrc.s8[15];
	return pDst.i;
}


CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S8toS32( const __m128i & src )
/*!
    Horizontal addition of 16 signed 8 bit integer values into 4 signed 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 signed 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:7]    + src[8:15]    + src[16:23]   + src[24:31]
    \n result[32:63]   = src[32:39]  + src[40:47]   + src[48:55]   + src[56:63]
    \n result[64:95]   = src[64:71]  + src[72:79]   + src[80:87]   + src[88:95]
    \n result[96:127]  = src[96:103] + src[104:111] + src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i =  CBL_REF::HorizontalAdd_S8toS16(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_S16toS32(pDst.i);

	return pDst.i;
}



CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S8toS64( const __m128i & src )
/*!
    Horizontal addition of 16 signed 8 bit integer values into 2 signed 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 signed 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:7]    + src[8:15]    + src[16:23]   + src[24:31]   + src[32:39]  + src[40:47]   + src[48:55]   + src[56:63]
    \n result[64:127]  = src[64:71]  + src[72:79]   + src[80:87]   + src[88:95]   + src[96:103] + src[104:111] + src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i =  CBL_REF::HorizontalAdd_S8toS16(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_S16toS32(pDst.i);
	pDst.i = CBL_REF::HorizontalAdd_S32toS64(pDst.i);
	return pDst.i;
}


CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S16toS32( const __m128i & src )
/*!
    Horizontal addition of 8 signed 16 bit integer values into 4 signed 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:15]   + src[16:31]
    \n result[32:63]   = src[32:47]  + src[48:63]
    \n result[64:95]   = src[64:79]  + src[80:95]
    \n result[96:127]  = src[96:111] + src[112:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s32[0] = (S32)pSrc.s16[0] + pSrc.s16[1];
	pDst.s32[1] = (S32)pSrc.s16[2] + pSrc.s16[3];
	pDst.s32[2] = (S32)pSrc.s16[4] + pSrc.s16[5];
	pDst.s32[3] = (S32)pSrc.s16[6] + pSrc.s16[7];

	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S16toS64( const __m128i & src )
/*!
    Horizontal addition of 8 signed 16 bit integer values into 2 signed 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:15]   + src[16:31]  + src[32:47]  + src[48:63]
    \n result[64:127]  = src[64:79]  + src[80:95]  + src[96:111] + src[112:127]
*/
{

    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i = CBL_REF::HorizontalAdd_S16toS32(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_S32toS64(pDst.i);
	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_S32toS64( const __m128i & src )
/*!
    Horizontal addition of 4 signed 32 bit integer values into 2 signed 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 signed 32 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:31]  + src[32:63]
    \n result[64:127]  = src[64:95]  + src[96:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s64[0] = (S64)pSrc.s32[0] + pSrc.s32[1];
	pDst.s64[1] = (S64)pSrc.s32[2] + pSrc.s32[3];

	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U8toU16( const __m128i & src )
/*!
    Horizontal addition of 16 unsigned 8 bit integer values into 8 unsigned 16 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 unsigned 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src[0:7]     + src[8:15]
    \n result[16:31]   = src[16:23]   + src[24:31]
    \n result[32:47]   = src[32:39]   + src[40:47]
    \n result[48:63]   = src[48:55]   + src[56:63]
    \n result[64:79]   = src[64:71]   + src[72:79]
    \n result[80:95]   = src[80:87]   + src[88:95]
    \n result[96:111]  = src[96:103]  + src[104:111]
    \n result[112:127] = src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u16[0] = (U16)pSrc.u8[0] + 	 pSrc.u8[1];
	pDst.u16[1] = (U16)pSrc.u8[2] + 	 pSrc.u8[3];
	pDst.u16[2] = (U16)pSrc.u8[4] + 	 pSrc.u8[5];
	pDst.u16[3] = (U16)pSrc.u8[6] + 	 pSrc.u8[7];
	pDst.u16[4] = (U16)pSrc.u8[8] + 	 pSrc.u8[9];
	pDst.u16[5] = (U16)pSrc.u8[10] +  pSrc.u8[11];
	pDst.u16[6] = (U16)pSrc.u8[12] +  pSrc.u8[13];
	pDst.u16[7] = (U16)pSrc.u8[14] +  pSrc.u8[15];
	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U8toU32( const __m128i & src )
/*!
    Horizontal addition of 16 unsigned 8 bit integer values into 4 unsigned 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 unsigned 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:7]    + src[8:15]    + src[16:23]   + src[24:31]
    \n result[32:63]   = src[32:39]  + src[40:47]   + src[48:55]   + src[56:63]
    \n result[64:95]   = src[64:71]  + src[72:79]   + src[80:87]   + src[88:95]
    \n result[96:127]  = src[96:103] + src[104:111] + src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i =  CBL_REF::HorizontalAdd_U8toU16(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_U16toU32(pDst.i);

	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U8toU64( const __m128i & src )
/*!
    Horizontal addition of 16 unsigned 8 bit integer values into 2 unsigned 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 unsigned 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:7]    + src[8:15]    + src[16:23]   + src[24:31]   + src[32:39]  + src[40:47]   + src[48:55]   + src[56:63]
    \n result[64:127]  = src[64:71]  + src[72:79]   + src[80:87]   + src[88:95]   + src[96:103] + src[104:111] + src[112:119] + src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i =  CBL_REF::HorizontalAdd_U8toU16(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_U16toU32(pDst.i);
	pDst.i = CBL_REF::HorizontalAdd_U32toU64(pDst.i);
	return pDst.i;

}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U16toU32( const __m128i & src )
/*!
    Horizontal addition of 8 unsigned 16 bit integer values into 4 unsigned 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 unsigned 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:15]   + src[16:31]
    \n result[32:63]   = src[32:47]  + src[48:63]
    \n result[64:95]   = src[64:79]  + src[80:95]
    \n result[96:127]  = src[96:111] + src[112:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u32[0] = (U32)pSrc.u16[0] + pSrc.u16[1];
	pDst.u32[1] = (U32)pSrc.u16[2] + pSrc.u16[3];
	pDst.u32[2] = (U32)pSrc.u16[4] + pSrc.u16[5];
	pDst.u32[3] = (U32)pSrc.u16[6] + pSrc.u16[7];

	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U16toU64( const __m128i & src )
/*!
    Horizontal addition of 8 unsigned 16 bit integer values into 2 unsigned 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 unsigned 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:15]   + src[16:31]  + src[32:47]  + src[48:63]
    \n result[64:127]  = src[64:79]  + src[80:95]  + src[96:111] + src[112:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.i = CBL_REF::HorizontalAdd_U16toU32(pSrc.i);
	pDst.i = CBL_REF::HorizontalAdd_U32toU64(pDst.i);
	return pDst.i;
}

CBL_INLINE  __m128i CBL_REF::HorizontalAdd_U32toU64( const __m128i & src )
/*!
    Horizontal addition of 4 unsigned 32 bit integer values into 2 unsigned 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 unsigned 32 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:31]  + src[32:63]
    \n result[64:127]  = src[64:95]  + src[96:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u64[0] = (U64)pSrc.u32[0] + pSrc.u32[1];
	pDst.u64[1] = (U64)pSrc.u32[2] + pSrc.u32[3];

	return pDst.i;
}

CBL_INLINE __m128i CBL_REF::HorizontalSubtract_S8toS16( const __m128i & src )
/*!
    Horizontal subtraction of 16 signed 8 bit integer values into 8 signed 16 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 signed 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src[0:7]     - src[8:15]
    \n result[16:31]   = src[16:23]   - src[24:31]
    \n result[32:47]   = src[32:39]   - src[40:47]
    \n result[48:63]   = src[48:55]   - src[56:63]
    \n result[64:79]   = src[64:71]   - src[72:79]
    \n result[80:95]   = src[80:87]   - src[88:95]
    \n result[96:111]  = src[96:103]  - src[104:111]
    \n result[112:127] = src[112:119] - src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s16[0] = (S16)pSrc.s8[0] - 	 pSrc.s8[1];
	pDst.s16[1] = (S16)pSrc.s8[2] - 	 pSrc.s8[3];
	pDst.s16[2] = (S16)pSrc.s8[4] - 	 pSrc.s8[5];
	pDst.s16[3] = (S16)pSrc.s8[6] - 	 pSrc.s8[7];
	pDst.s16[4] = (S16)pSrc.s8[8] - 	 pSrc.s8[9];
	pDst.s16[5] = (S16)pSrc.s8[10] -  pSrc.s8[11];
	pDst.s16[6] = (S16)pSrc.s8[12] -  pSrc.s8[13];
	pDst.s16[7] = (S16)pSrc.s8[14] -  pSrc.s8[15];
	return pDst.i;

}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_S16toS32( const __m128i & src )
/*!
    Horizontal subtraction of 8 signed 16 bit integer values into 4 signed 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:15]   - src[16:31]
    \n result[32:63]   = src[32:47]  - src[48:63]
    \n result[64:95]   = src[64:79]  - src[80:95]
    \n result[96:127]  = src[96:111] - src[112:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s32[0] = (S32)pSrc.s16[0] - pSrc.s16[1];
	pDst.s32[1] = (S32)pSrc.s16[2] - pSrc.s16[3];
	pDst.s32[2] = (S32)pSrc.s16[4] - pSrc.s16[5];
	pDst.s32[3] = (S32)pSrc.s16[6] - pSrc.s16[7];

	return pDst.i;

}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_S32toS64( const __m128i & src )
/*!
    Horizontal subtraction of 4 signed 32 bit integer values into 2 signed 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 signed 32 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:31]  - src[32:63]
    \n result[64:127]  = src[64:95]  - src[96:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.s64[0] = (S64)pSrc.s32[0] - pSrc.s32[1];
	pDst.s64[1] = (S64)pSrc.s32[2] - pSrc.s32[3];

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_U8toU16( const __m128i & src )
/*!
    Horizontal subtraction of 16 unsigned 8 bit integer values into 8 unsigned 16 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 16 unsigned 8 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src[0:7]     - src[8:15]
    \n result[16:31]   = src[16:23]   - src[24:31]
    \n result[32:47]   = src[32:39]   - src[40:47]
    \n result[48:63]   = src[48:55]   - src[56:63]
    \n result[64:79]   = src[64:71]   - src[72:79]
    \n result[80:95]   = src[80:87]   - src[88:95]
    \n result[96:111]  = src[96:103]  - src[104:111]
    \n result[112:127] = src[112:119] - src[120:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u16[0] = (U16)pSrc.u8[0] - pSrc.u8[1];
	pDst.u16[1] = (U16)pSrc.u8[2] - pSrc.u8[3];
	pDst.u16[2] = (U16)pSrc.u8[4] - pSrc.u8[5];
	pDst.u16[3] = (U16)pSrc.u8[6] - pSrc.u8[7];
	pDst.u16[4] = (U16)pSrc.u8[8] - pSrc.u8[9];
	pDst.u16[5] = (U16)pSrc.u8[10] - pSrc.u8[11];
	pDst.u16[6] = (U16)pSrc.u8[12] - pSrc.u8[13];
	pDst.u16[7] = (U16)pSrc.u8[14] - pSrc.u8[15];
	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_U16toU32( const __m128i & src )
/*!
    Horizontal subtraction of 8 unsigned 16 bit integer values into 4 unsigned 32 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 8 unsigned 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:31]    = src[0:15]   - src[16:31]
    \n result[32:63]   = src[32:47]  - src[48:63]
    \n result[64:95]   = src[64:79]  - src[80:95]
    \n result[96:127]  = src[96:111] - src[112:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u32[0] = (U32)pSrc.u16[0] - pSrc.u16[1];
	pDst.u32[1] = (U32)pSrc.u16[2] - pSrc.u16[3];
	pDst.u32[2] = (U32)pSrc.u16[4] - pSrc.u16[5];
	pDst.u32[3] = (U32)pSrc.u16[6] - pSrc.u16[7];

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_U32toU64( const __m128i & src )
/*!
    Horizontal subtraction of 4 unsigned 32 bit integer values into 2 unsigned 64 bit integer values
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 unsigned 32 bit values
    \return result of the horizontal addition
    \par
    \n result[0:63]    = src[0:31]  - src[32:63]
    \n result[64:127]  = src[64:95]  - src[96:127]
*/
{
    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;

	pDst.u64[0] = (U64)pSrc.u32[0] - pSrc.u32[1];
	pDst.u64[1] = (U64)pSrc.u32[2] - pSrc.u32[3];

	return pDst.i;
}


CBL_INLINE __m128i CBL_REF::HorizontalAdd_S16( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal addition of 8 signed 16 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 8 signed 16 bit values
    \param[in] src2 - second 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src1[0:15]    + src1[16:31]
    \n result[16:31]   = src1[32:47]   + src1[48:63]
    \n result[32:47]   = src1[64:79]   + src1[80:95]
    \n result[48:63]   = src1[96:111]  + src1[112:127]
    \n result[64:79]   = src2[0:15]    + src2[16:31]
    \n result[80:95]   = src2[32:47]   + src2[48:63]
    \n result[96:111]  = src2[64:79]   + src2[80:95]
    \n result[112:127] = src2[96:111]  + src2[112:127]
    \par NOTE:
    The results are not saturated.  The results are truncated to the least significant 16 bits.
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s16[0] = pSrc1.s16[0] + pSrc1.s16[1];
	pDst.s16[1] = pSrc1.s16[2] + pSrc1.s16[3];
	pDst.s16[2] = pSrc1.s16[4] + pSrc1.s16[5];
	pDst.s16[3] = pSrc1.s16[6] + pSrc1.s16[7];
	pDst.s16[4] = pSrc2.s16[0] + pSrc2.s16[1];
	pDst.s16[5] = pSrc2.s16[2] + pSrc2.s16[3];
	pDst.s16[6] = pSrc2.s16[4] + pSrc2.s16[5];
	pDst.s16[7] = pSrc2.s16[6] + pSrc2.s16[7];

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalAddSat_S16( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal addition of 8 signed 16 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 8 signed 16 bit values
    \param[in] src2 - second 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal addition
    \par
    \n result[0:15]    = src1[0:15]    + src1[16:31]
    \n result[16:31]   = src1[32:47]   + src1[48:63]
    \n result[32:47]   = src1[64:79]   + src1[80:95]
    \n result[48:63]   = src1[96:111]  + src1[112:127]
    \n result[64:79]   = src2[0:15]    + src2[16:31]
    \n result[80:95]   = src2[32:47]   + src2[48:63]
    \n result[96:111]  = src2[64:79]   + src2[80:95]
    \n result[112:127] = src2[96:111]  + src2[112:127]
    \par NOTE: The results are saturated
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s16[0] = Limits<S16>::Sat((S32)pSrc1.s16[0] + pSrc1.s16[1]);
	pDst.s16[1] = Limits<S16>::Sat((S32)pSrc1.s16[2] + pSrc1.s16[3]);
	pDst.s16[2] = Limits<S16>::Sat((S32)pSrc1.s16[4] + pSrc1.s16[5]);
	pDst.s16[3] = Limits<S16>::Sat((S32)pSrc1.s16[6] + pSrc1.s16[7]);
	pDst.s16[4] = Limits<S16>::Sat((S32)pSrc2.s16[0] + pSrc2.s16[1]);
	pDst.s16[5] = Limits<S16>::Sat((S32)pSrc2.s16[2] + pSrc2.s16[3]);
	pDst.s16[6] = Limits<S16>::Sat((S32)pSrc2.s16[4] + pSrc2.s16[5]);
	pDst.s16[7] = Limits<S16>::Sat((S32)pSrc2.s16[6] + pSrc2.s16[7]);

	return pDst.i;

}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_S16( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal subtraction of 8 signed 16 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 8 signed 16 bit values
    \param[in] src2 - second 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal subtraction
    \par
    \n result[0:15]    = src1[0:15]    - src1[16:31]
    \n result[16:31]   = src1[32:47]   - src1[48:63]
    \n result[32:47]   = src1[64:79]   - src1[80:95]
    \n result[48:63]   = src1[96:111]  - src1[112:127]
    \n result[64:79]   = src2[0:15]    - src2[16:31]
    \n result[80:95]   = src2[32:47]   - src2[48:63]
    \n result[96:111]  = src2[64:79]   - src2[80:95]
    \n result[112:127] = src2[96:111]  - src2[112:127]
    \par NOTE: The results are not saturated.  The results are truncated to the least significant 16 bits.
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s16[0] = pSrc1.s16[0] - pSrc1.s16[1];
	pDst.s16[1] = pSrc1.s16[2] - pSrc1.s16[3];
	pDst.s16[2] = pSrc1.s16[4] - pSrc1.s16[5];
	pDst.s16[3] = pSrc1.s16[6] - pSrc1.s16[7];
	pDst.s16[4] = pSrc2.s16[0] - pSrc2.s16[1];
	pDst.s16[5] = pSrc2.s16[2] - pSrc2.s16[3];
	pDst.s16[6] = pSrc2.s16[4] - pSrc2.s16[5];
	pDst.s16[7] = pSrc2.s16[6] - pSrc2.s16[7];

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalSubtractSat_S16( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal subtraction  of 8 signed 16 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 8 signed 16 bit values
    \param[in] src2 - second 128 bit operand containing 8 signed 16 bit values
    \return result of the horizontal subtraction
    \par
    \n result[0:15]    = src1[0:15]    - src1[16:31]
    \n result[16:31]   = src1[32:47]   - src1[48:63]
    \n result[32:47]   = src1[64:79]   - src1[80:95]
    \n result[48:63]   = src1[96:111]  - src1[112:127]
    \n result[64:79]   = src2[0:15]    - src2[16:31]
    \n result[80:95]   = src2[32:47]   - src2[48:63]
    \n result[96:111]  = src2[64:79]   - src2[80:95]
    \n result[112:127] = src2[96:111]  - src2[112:127]
    \par NOTE: The results are saturated
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s16[0] = Limits<S16>::Sat((S32)pSrc1.s16[0] - pSrc1.s16[1]);
	pDst.s16[1] = Limits<S16>::Sat((S32)pSrc1.s16[2] - pSrc1.s16[3]);
	pDst.s16[2] = Limits<S16>::Sat((S32)pSrc1.s16[4] - pSrc1.s16[5]);
	pDst.s16[3] = Limits<S16>::Sat((S32)pSrc1.s16[6] - pSrc1.s16[7]);
	pDst.s16[4] = Limits<S16>::Sat((S32)pSrc2.s16[0] - pSrc2.s16[1]);
	pDst.s16[5] = Limits<S16>::Sat((S32)pSrc2.s16[2] - pSrc2.s16[3]);
	pDst.s16[6] = Limits<S16>::Sat((S32)pSrc2.s16[4] - pSrc2.s16[5]);
	pDst.s16[7] = Limits<S16>::Sat((S32)pSrc2.s16[6] - pSrc2.s16[7]);

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalAdd_S32( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal addition of 4 signed 32 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 signed 32 bit values
    \param[in] src2 - second 128 bit operand containing 4 signed 32 bit values
    \return result of the horizontal addition
    \par
    result[0:31]    = src1[0:31]    + src1[32:63]
    result[32:63]   = src1[64:95]   + src1[96:127]
    result[64:95]   = src2[0:31]    + src2[32:63]
    result[96:127]  = src2[64:95]   + src2[96:127]
    \par NOTE: The results are not saturated.  The results are truncated to the least significant 32 bits.
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s32[0] = pSrc1.s32[0] + pSrc1.s32[1];
	pDst.s32[1] = pSrc1.s32[2] + pSrc1.s32[3];
	pDst.s32[2] = pSrc2.s32[0] + pSrc2.s32[1];
	pDst.s32[3] = pSrc2.s32[2] + pSrc2.s32[3];

	return pDst.i;
}
CBL_INLINE __m128i CBL_REF::HorizontalSubtract_S32( const __m128i & src1, const __m128i & src2 )
/*!
    Horizontal subtraction of 4 signed 32 bit integer values in two registers
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 signed 32 bit values
    \param[in] src2 - second 128 bit operand containing 4 signed 32 bit values
    \return result of the horizontal subtraction
    \par
    \n result[0:31]    = src1[0:31]    - src1[32:63]
    \n result[32:63]   = src1[64:95]   - src1[96:127]
    \n result[64:95]   = src2[0:31]    - src2[32:63]
    \n result[96:127]  = src2[64:95]   - src2[96:127]
    \par NOTE: The results are not saturated.  The results are truncated to the least significant 32 bits.
*/
{
	const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
	const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 pDst;

	pDst.s32[0] = pSrc1.s32[0] - pSrc1.s32[1];
	pDst.s32[1] = pSrc1.s32[2] - pSrc1.s32[3];
	pDst.s32[2] = pSrc2.s32[0] - pSrc2.s32[1];
	pDst.s32[3] = pSrc2.s32[2] - pSrc2.s32[3];

	return pDst.i;
}


//// Absolute functions

/*!
    Calculate absolute value of 16 signed 8-bit integers
    \par Parameters:
    \param[in] src - a 128 bit variable containing 16 signed 8-bit integer values
    \return: absolute value of the values in src
*/
CBL_INLINE __m128i CBL_REF::Abs_S8( const __m128i &src )
{


    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;
	pDst.s32[0] = 0;

		for (int i=0 ; i < 16 ; i++)
		{
			S32 complement, temp,dst;
			temp = ((S32)pSrc.s8[i] >> 31);
			complement = (pSrc.s8[i]^temp);  //one's complement of src
			dst = (complement- temp);
			temp  = ((U32)dst >>  31);
			dst =   (dst- temp);
			if (dst >= 128)
				pDst.s8[i] = 127;
			else
				pDst.s8[i] = (S8)dst; 			

		}
	return pDst.i;
}

/*!
    Calculate absolute value of 8 signed 16-bit integers
    \par Parameters:
    \param[in] src - a 128 bit variable containing 8 signed 16-bit integer values
    \return: absolute value of the values in src
*/
CBL_INLINE __m128i CBL_REF::Abs_S16( const __m128i &src )
{

    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;	
	pDst.s32[0] = 0;

		for (int i=0 ; i < 8 ; i++)
		{
			S32 complement, temp,dst;
			temp = ((S32)pSrc.s16[i] >> 31);
			complement = (pSrc.s16[i]^temp);  //one's complement of src
			dst = (complement- temp);
			temp  = ((U32)dst >>  31);
			dst =   (dst- temp);
			if (dst >= 32768)
				pDst.s16[i] = 32767;
			else
				pDst.s16[i] = (S16)dst; 
		}
	return pDst.i;
}

/*!
    Calculate absolute value of 4 signed 32-bit integers
    \par Parameters:
    \param[in] src - a 128 bit variable containing 4 signed 32-bit integer values
    \return: absolute value of the values in src
*/
CBL_INLINE __m128i CBL_REF::Abs_S32( const __m128i &src )
{

    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;
	pDst.s32[0] = 0;
		for (int i=0 ; i < 4 ; i++)
		{
			S32 complement, temp,dst;
			temp = (pSrc.s32[i] >> 31);
			complement = (pSrc.s32[i]^temp);  //one's complement of src
			dst = (complement- temp);
			temp  = ((U32)dst >>  31);
			pDst.s32[i] =   (dst- temp);
		}
	return pDst.i;
}

/*!
    Calculate absolute value of 2 signed 64-bit integers
    \par Parameters:
    \param[in] src - a 128 bit variable containing 2 signed 64-bit integer values
    \return: absolute value of the values in src
*/
CBL_INLINE  __m128i CBL_REF::Abs_S64( const __m128i &src )
{

    const XMM128 & pSrc = reinterpret_cast<const XMM128&>( src );
    XMM128 pDst;
	pDst.s32[0] = 0;
		for (int i=0 ; i < 2 ; i++)
		{
			S64 complement, temp = 0,dst;
			S32 *p;
			S32 t1;
			p = (S32*)&pSrc.s64[i];
			t1 = (p[1] >> 31);
			p = (S32*)&temp;
			p[0] = t1;
			p[1] = t1;
			complement = (pSrc.s64[i]^temp);  //one's complement of src
			dst = (complement- temp);

			p = (S32*)&dst;
			t1  = ((U32)p[1] >>  31);
			p = (S32*)&temp;
			p[1] = 0;
			p[0] = t1;
			pDst.s64[i] =    (dst- temp);
		}
	return pDst.i;
}

//// End of Absolute functions



/*! 
    \param[in] src1 - 128 bit value containing 16 unsigned 8-bit values
    \param[in] src2 - 128 bit value containing 16 unsigned 8-bit values
    \return FF for each value if src1 < src2 else return 0
*/
CBL_INLINE __m128i CBL_REF::compare_lt_8u( const __m128i & src1, const __m128i & src2 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
	pDst.s32[0] = 0;
	for(int i = 0; i<16 ; i++)
	{
		pDst.u8[i] = (pSrc1.u8[i] < pSrc2.u8[i]) ? 0xFF : 0;
	}
	return pDst.i;
}

/*! 
    \param[in] src1 - 128 bit value containing 16 unsigned 8-bit values
    \param[in] src2 - 128 bit value containing 16 unsigned 8-bit values
    \return FF for each value if src1 <= src2 else return 0
*/
CBL_INLINE __m128i CBL_REF::compare_le_8u( const __m128i & src1, const __m128i & src2 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );

	for(int i = 0; i<16 ; i++)
	{
		pDst.u8[i] = (pSrc1.u8[i] <= pSrc2.u8[i]) ? 0xFF : 0;
	}
	return pDst.i;
}

/*! 
    \param[in] src1 - 128 bit value containing 16 unsigned 8-bit values
    \param[in] src2 - 128 bit value containing 16 unsigned 8-bit values
    \return FF for each value if src1 > src2 else return 0
*/
CBL_INLINE __m128i CBL_REF::compare_gt_8u( const __m128i & src1, const __m128i & src2 )
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );

	for(int i = 0; i<16 ; i++)
	{
		pDst.u8[i] = (pSrc1.u8[i] > pSrc2.u8[i]) ? 0xFF : 0;
	}
	return pDst.i;
}

/*! 
    \param[in] src1 - 128 bit value containing 16 unsigned 8-bit values
    \param[in] src2 - 128 bit value containing 16 unsigned 8-bit values
    \return FF for each value if src1 >= src2 else return 0
*/
CBL_INLINE __m128i CBL_REF::compare_ge_8u( const __m128i & src1, const __m128i & src2 )
{	
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );

	for(int i = 0; i<16 ; i++)
	{
		pDst.u8[i] = (pSrc1.u8[i] >= pSrc2.u8[i]) ? 0xFF : 0;
	}
	return pDst.i;

}

// end of compare routines.


CBL_INLINE __m128i  CBL_REF::MultiplyAccumulate_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    Multiply accumulate signed word to signed word
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 8 signed 16-bit integer values
    \return
    \n result[0:15]    = src1[0:15]    * src2[0:15]    + src3[0:15];
    \n result[16:31]   = src1[16:31]   * src2[16:31]   + src3[16:31];
    \n result[32:47]   = src1[32:47]   * src2[32:47]   + src3[32:47];
    \n result[48:63]   = src1[48:63]   * src2[48:63]   + src3[48:63];
    \n result[64:79]   = src1[64:79]   * src2[64:79]   + src3[64:79];
    \n result[80:95]   = src1[80:95]   * src2[80:95]   + src3[80:95];
    \n result[96:111]  = src1[96:111]  * src2[96:111]  + src3[96:111];
    \n result[112:127] = src1[112:127] * src2[112:127] + src3[112:127];
    \par NOTE:
    No saturation is performed.  High order bits of the multiplication result are ignored.  
    Only low order 16 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
	pDst.s16[0] = 0;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	for(int i = 0 ; i < 8 ; i++)
		pDst.s16[i] = (pSrc1.s16[i] * pSrc2.s16[i]) + pSrc3.s16[i];
	
	return pDst.i;
}

CBL_INLINE __m128i  CBL_REF::MultiplyAccumulateSat_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    Multiply accumulate signed word to signed word with saturation
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 8 signed 16-bit integer values
    \return
    \n result[0:15]    = src1[0:15]    * src2[0:15]    + src3[0:15];
    \n result[16:31]   = src1[16:31]   * src2[16:31]   + src3[16:31];
    \n result[32:47]   = src1[32:47]   * src2[32:47]   + src3[32:47];
    \n result[48:63]   = src1[48:63]   * src2[48:63]   + src3[48:63];
    \n result[64:79]   = src1[64:79]   * src2[64:79]   + src3[64:79];
    \n result[80:95]   = src1[80:95]   * src2[80:95]   + src3[80:95];
    \n result[96:111]  = src1[96:111]  * src2[96:111]  + src3[96:111];
    \n result[112:127] = src1[112:127] * src2[112:127] + src3[112:127];
*/
{
    XMM128 pDst;
	pDst.s16[0] = 0;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	for(int i = 0 ; i < 8 ; i++)
		pDst.s16[i] = Limits<S16>::Sat(((S32)pSrc1.s16[i] * pSrc2.s16[i]) + (S32)pSrc3.s16[i]);

	return pDst.i;
}

CBL_INLINE __m128i  CBL_REF::MultiplyAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    multiply the odd-numbered 16-bit signed values in src1 and src2 and adds 32-bit signed value in src3.
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]   = src1[0:15]   * src2[0:15]   + src3[0:31];
    \n result[32:63]  = src1[32:47]  * src2[32:47]  + src3[32:63];
    \n result[64:95]  = src1[64:79]  * src2[64:79]  + src3[64:95];
    \n result[96:127] = src1[96:111] * src2[96:111] + src3[96:127];
    \par NOTE:
    No saturation is performed.
    Only low order 32 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s32[0] = ((S32) pSrc1.s16[0] * pSrc2.s16[0]) + pSrc3.s32[0];
	pDst.s32[1] = ((S32) pSrc1.s16[2] * pSrc2.s16[2]) + pSrc3.s32[1];
	pDst.s32[2] = ((S32) pSrc1.s16[4] * pSrc2.s16[4]) + pSrc3.s32[2];
	pDst.s32[3] = ((S32) pSrc1.s16[6] * pSrc2.s16[6]) + pSrc3.s32[3];

	return pDst.i;
}


//! Multiply accumulate signed word to signed double word with saturation
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    multiply the odd-numbered 16-bit signed values in src1 and src2 and adds 32-bit signed value in src3.
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]   = src1[0:15]   * src2[0:15]   + src3[0:31];
    \n result[32:63]  = src1[32:47]  * src2[32:47]  + src3[32:63];
    \n result[64:95]  = src1[64:79]  * src2[64:79]  + src3[64:95];
    \n result[96:127] = src1[96:111] * src2[96:111] + src3[96:127];
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s32[0] = Limits<S32>::Sat(((S32) pSrc1.s16[0] * pSrc2.s16[0]) + (S64) pSrc3.s32[0]);
	pDst.s32[1] = Limits<S32>::Sat(((S32) pSrc1.s16[2] * pSrc2.s16[2]) + (S64) pSrc3.s32[1]);
	pDst.s32[2] = Limits<S32>::Sat(((S32) pSrc1.s16[4] * pSrc2.s16[4]) + (S64) pSrc3.s32[2]);
	pDst.s32[3] = Limits<S32>::Sat(((S32) pSrc1.s16[6] * pSrc2.s16[6]) + (S64) pSrc3.s32[3]);

	return pDst.i;
}

//! Multiply accumulate signed double word to signed double word
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulate_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]    = src1[0:31]    * src2[0:31]    + src3[0:31];
    \n result[32:63]   = src1[32:63]   * src2[32:63]   + src3[32:63];
    \n result[64:95]   = src1[64:95]   * src2[64:95]   + src3[64:95];
    \n result[96:127]  = src1[96:127]  * src2[96:127]  + src3[96:127];
    \par NOTE:
    No saturation is performed.  High order bits of the multiplication result are ignored.  
    Only low order 32 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s32[0] = ((S32) pSrc1.s32[0] * pSrc2.s32[0]) + pSrc3.s32[0];
	pDst.s32[1] = ((S32) pSrc1.s32[1] * pSrc2.s32[1]) + pSrc3.s32[1];
	pDst.s32[2] = ((S32) pSrc1.s32[2] * pSrc2.s32[2]);
	pDst.s32[2]+= pSrc3.s32[2];
	pDst.s32[3] = ((S32) pSrc1.s32[3] * pSrc2.s32[3]);
	pDst.s32[3] += pSrc3.s32[3];

	return pDst.i;
}

//! Multiply accumulate signed double word to signed double word with saturation
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulateSat_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]    = src1[0:31]    * src2[0:31]    + src3[0:31];
    \n result[32:63]   = src1[32:63]   * src2[32:63]   + src3[32:63];
    \n result[64:95]   = src1[64:95]   * src2[64:95]   + src3[64:95];
    \n result[96:127]  = src1[96:127]  * src2[96:127]  + src3[96:127];
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s32[0] = Limits<S32>::Sat(((S64) pSrc1.s32[0] * pSrc2.s32[0]) + (S64) pSrc3.s32[0]);
	pDst.s32[1] = Limits<S32>::Sat(((S64) pSrc1.s32[1] * pSrc2.s32[1]) + (S64) pSrc3.s32[1]);
	pDst.s32[2] = Limits<S32>::Sat(((S64) pSrc1.s32[2] * pSrc2.s32[2]) + (S64) pSrc3.s32[2]);
	pDst.s32[3] = Limits<S32>::Sat(((S64) pSrc1.s32[3] * pSrc2.s32[3]) + (S64) pSrc3.s32[3]);

	return pDst.i;

}
//! Multiply accumulate signed low double word to signed quad word
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulate_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 2 signed 64-bit integer values
    \return
    \n result[0:63]    = src1[0:31]    * src2[0:31]    + src3[0:63];
    \n result[64:127]  = src1[64:95]   * src2[64:95]   + src3[64:127];
    \par NOTE:
    No saturation is performed.
    Only low order 32 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = (pSrc1.s32[0] * pSrc2.s32[0]) + pSrc3.s64[0];
	pDst.s64[1] = (pSrc1.s32[2] * pSrc2.s32[2]) + pSrc3.s64[1];

	return pDst.i;
}

//! Multiply accumulate signed low double word to signed quad word with saturation
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulateSat_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 2 signed 64-bit integer values
    \return
    \n result[0:63]    = src1[0:31]    * src2[0:31]    + src3[0:63];
    \n result[64:127]  = src1[64:95]   * src2[64:95]   + src3[64:127];
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[0] * pSrc2.s32[0]), pSrc3.s64[0]);
	pDst.s64[1] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[2] * pSrc2.s32[2]), pSrc3.s64[1]);

	return pDst.i;
}

//! Multiply accumulate signed high double word to signed quad word
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulate_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 2 signed 64-bit integer values
    \return
    \n result[0:63]    = src1[32:63]    * src2[32:63]    + src3[0:63];
    \n result[64:127]  = src1[96:127]   * src2[96:127]   + src3[64:127];
    \par NOTE:
    No saturation is performed.
    Only low order 32 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = (pSrc1.s32[1] * pSrc2.s32[1]) + pSrc3.s64[0];
	pDst.s64[1] = (pSrc1.s32[3] * pSrc2.s32[3]) + pSrc3.s64[1];

	return pDst.i;
}

//! Multiply accumulate signed high double word to signed quad word with saturation
CBL_INLINE __m128i  CBL_REF::MultiplyAccumulateSat_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src2 - a 128 bit variable containing 4 signed 32-bit integer values
    \param[in] src3 - a 128 bit variable containing 2 signed 64-bit integer values
    \return
    \n result[0:63]    = src1[32:63]    * src2[32:63]    + src3[0:63];
    \n result[64:127]  = src1[96:127]   * src2[96:127]   + src3[64:127];
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s64[0] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[1] * pSrc2.s32[1]),pSrc3.s64[0]);
	pDst.s64[1] = CBL_Library_Internal::addSat64to128(((S64) pSrc1.s32[3] * pSrc2.s32[3]),pSrc3.s64[1]);

	return pDst.i;
}

//! Multiply, add and accumulate signed word to signed double word				   
CBL_INLINE __m128i  CBL_REF::MultiplyAddAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    multiply and add the 16-bit signed values in src1 and src2, then adds 32-bit signed value in src3.
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]   = src1[0:15]   * src2[0:15]   + src1[16:31]   * src2[16:31]   + src3[0:31];
    \n result[32:63]  = src1[32:47]  * src2[32:47]  + src1[48:63]   * src2[48:63]   + src3[32:63];
    \n result[64:95]  = src1[64:79]  * src2[64:79]  + src1[80:95]   * src2[80:95]   + src3[64:95];
    \n result[96:127] = src1[96:111] * src2[96:111] + src1[112:127] * src2[112:127] + src3[96:127];
    \par NOTE:
    No saturation is performed.
    Only low order 32 bits of the result are stored in the result register.
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	
	pDst.s32[0] = ((S32) pSrc1.s16[0] * pSrc2.s16[0]) + ((S32) pSrc1.s16[1] * pSrc2.s16[1]) +  pSrc3.s32[0];
	pDst.s32[1] = ((S32) pSrc1.s16[2] * pSrc2.s16[2]) + ((S32) pSrc1.s16[3] * pSrc2.s16[3]) +  pSrc3.s32[1];
	pDst.s32[2] = ((S32) pSrc1.s16[4] * pSrc2.s16[4]) + ((S32) pSrc1.s16[5] * pSrc2.s16[5]) +  pSrc3.s32[2];
	pDst.s32[3] = ((S32) pSrc1.s16[6] * pSrc2.s16[6]) + ((S32) pSrc1.s16[7] * pSrc2.s16[7]) +  pSrc3.s32[3];

	return pDst.i;
}

//! Multiply, add and accumulate signed word to signed double word with saturation
CBL_INLINE __m128i  CBL_REF::MultiplyAddAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 )
/*!
    multiply and add the 16-bit signed values in src1 and src2, then adds 32-bit signed value in src3.
    \par Parameters:
    \param[in] src1 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src2 - a 128 bit variable containing 8 signed 16-bit integer values
    \param[in] src3 - a 128 bit variable containing 4 signed 32-bit integer values
    \return
    \n result[0:31]   = src1[0:15]   * src2[0:15]   + src1[16:31]   * src2[16:31]   + src3[0:31];
    \n result[32:63]  = src1[32:47]  * src2[32:47]  + src1[48:63]   * src2[48:63]   + src3[32:63];
    \n result[64:95]  = src1[64:79]  * src2[64:79]  + src1[80:95]   * src2[80:95]   + src3[64:95];
    \n result[96:127] = src1[96:111] * src2[96:111] + src1[112:127] * src2[112:127] + src3[96:127];
*/
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

	pDst.s32[0] = Limits<S32>::Sat((S64)((S32) pSrc1.s16[0] * pSrc2.s16[0]) + (S64)((S32) pSrc1.s16[1] * pSrc2.s16[1]) +  pSrc3.s32[0]);
	pDst.s32[1] = Limits<S32>::Sat((S64)((S32) pSrc1.s16[2] * pSrc2.s16[2]) + (S64)((S32) pSrc1.s16[3] * pSrc2.s16[3]) +  pSrc3.s32[1]);
	pDst.s32[2] = Limits<S32>::Sat((S64)((S32) pSrc1.s16[4] * pSrc2.s16[4]) + (S64)((S32) pSrc1.s16[5] * pSrc2.s16[5]) +  pSrc3.s32[2]);
	pDst.s32[3] = Limits<S32>::Sat((S64)((S32) pSrc1.s16[6] * pSrc2.s16[6]) + (S64)((S32) pSrc1.s16[7] * pSrc2.s16[7]) +  pSrc3.s32[3]);

	return pDst.i;
}


/*!
    Multiplies four 32 bit integer values and returns the results in 32 bit float
    \par Parameters:
    \param[in] src1   - 128 bit operand containing 4 signed 32 bit integer values
    \param[in] src2   - 128 bit operand containing 4 signed 32 bit integer values
    \return the result of the complex multiplication between the 32 bit integers
    \with the result in floating point
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
        \n result[2] = (src1[2] * src2[2])
        \n result[3] = (src1[3] * src2[3])
*/
CBL_INLINE __m128  CBL_REF::Multiply_32s32f( const __m128i &s1, const __m128i& s2)
{
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( s1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( s2 );
    XMM128 pDst;

	pDst.f32[0] = ((F32)pSrc1.s32[0] * pSrc2.s32[0]);
	pDst.f32[1] = ((F32)pSrc1.s32[1] * pSrc2.s32[1]);
	pDst.f32[2] = ((F32)pSrc1.s32[2] * pSrc2.s32[2]);
	pDst.f32[3] = ((F32)pSrc1.s32[3] * pSrc2.s32[3]);
	return pDst.f;
}

/*!
    Multiplies two 32 bit complex values
    \par Parameters:
    \param[in] src1     - 128 bit operand containing two 32 bit complex values
    \param[in] src2     - 128 bit operand containing two 32 bit complex values
    \param[in] negamask - 128 bit operand containing four 32 bit values with only the MSB set for the 'a' values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
    \Note: This function is identical to the Multiply_32fc function but it is faster
*/
CBL_INLINE __m128 CBL_REF::Multiply_32fc_wm( const __m128 & multiplicand, const __m128 & multiplier, const __m128 negamask)
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );	
	negamask;

	pDst.f32[0] = (pSrc1.f32[0] * pSrc2.f32[0]) - (pSrc1.f32[1] * pSrc2.f32[1]);
	pDst.f32[1] = (pSrc1.f32[0] * pSrc2.f32[1]) + (pSrc1.f32[1] * pSrc2.f32[0]);
	pDst.f32[2] = (pSrc1.f32[2] * pSrc2.f32[2]) - (pSrc1.f32[3] * pSrc2.f32[3]);
	pDst.f32[3] = (pSrc1.f32[2] * pSrc2.f32[3]) + (pSrc1.f32[3] * pSrc2.f32[2]);

	return pDst.f;
}


/*!
    Multiplies four 16 bit complex values and returns the result in 32 bit floating point complex
    \par Parameters:
    \param[in] multiplicand - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] multiplier   - 128 bit operand containing 4 signed 16 bit complex values
    \param[out] dstLo   -
        \n dstLo[0] = (multiplicand[0] * multiplier[0]) [real]
        \n dstLo[1] = (multiplicand[0] * multiplier[0]) [imaginary]
        \n dstLo[2] = (multiplicand[1] * multiplier[1]) [real]
        \n dstLo[3] = (multiplicand[1] * multiplier[1]) [imaginary]
    \param[out] dstHi   -
        \n dstHi[0] = (multiplicand[2] * multiplier[2]) [real]
        \n dstHi[1] = (multiplicand[2] * multiplier[2]) [imaginary]
        \n dstHi[2] = (multiplicand[3] * multiplier[3]) [real]
        \n dstHi[3] = (multiplicand[3] * multiplier[3]) [imaginary]
    \ return none
*/
CBL_INLINE void CBL_REF::Multiply_16sc32fc(const __m128i &multiplicand,const __m128i &multiplier,__m128 &destLo, __m128 &destHi)
{

    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );
    XMM128 & pDstlo = reinterpret_cast<XMM128&>( destLo );
    XMM128 & pDsthi = reinterpret_cast<XMM128&>( destHi );

	pDstlo.f32[0] = ((F32)pSrc1.s16[0] * pSrc2.s16[0]) - ((F32)pSrc1.s16[1] * pSrc2.s16[1]);
	pDstlo.f32[1] = ((F32)pSrc1.s16[0] * pSrc2.s16[1]) + ((F32)pSrc1.s16[1] * pSrc2.s16[0]);
	pDstlo.f32[2] = ((F32)pSrc1.s16[2] * pSrc2.s16[2]) - ((F32)pSrc1.s16[3] * pSrc2.s16[3]);
	pDstlo.f32[3] = ((F32)pSrc1.s16[2] * pSrc2.s16[3]) + ((F32)pSrc1.s16[3] * pSrc2.s16[2]);

	pDsthi.f32[0] = ((F32)pSrc1.s16[4] * pSrc2.s16[4]) - ((F32)pSrc1.s16[5] * pSrc2.s16[5]);
	pDsthi.f32[1] = ((F32)pSrc1.s16[4] * pSrc2.s16[5]) + ((F32)pSrc1.s16[5] * pSrc2.s16[4]);
	pDsthi.f32[2] = ((F32)pSrc1.s16[6] * pSrc2.s16[6]) - ((F32)pSrc1.s16[7] * pSrc2.s16[7]);
	pDsthi.f32[3] = ((F32)pSrc1.s16[6] * pSrc2.s16[7]) + ((F32)pSrc1.s16[7] * pSrc2.s16[6]);
}

/*!
    Multiplies eight 16 bit complex values and returns the result in 32 bit integer
    \par Parameters:
    \param[in,out] multiplicand - 128 bit operand containing 8 signed 16 bit values and
    \ contains the first two of the 32 bit results returned
        \n src1DstLo[0] = (multiplicand[0] * multiplier[0])
        \n src1DstLo[1] = (multiplicand[1] * multiplier[1])
        \n src1DstLo[2] = (multiplicand[2] * multiplier[2])
        \n src1DstLo[3] = (multiplicand[3] * multiplier[3])
    \param[in] multiplier   - 128 bit operand containing 4 signed 16 bit complex values
    \return the third and the fourth of the 32 bit values returned
        \n result[0] = (multiplicand[4] * multiplier[4]) [real]
        \n result[1] = (multiplicand[5] * multiplier[5]) [imaginary]
        \n result[2] = (multiplicand[6] * multiplier[6]) [real]
        \n result[3] = (multiplicand[7] * multiplier[7]) [imaginary]
*/
CBL_INLINE __m128i CBL_REF::Multiply_16s_32s( __m128i & multiplicand, const __m128i & multiplier)
{
    XMM128 pDst,pDst1;
    XMM128 & pSrc1 = reinterpret_cast<XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );	

	pDst.s32[0] = ((S32)pSrc1.s16[0] * pSrc2.s16[0]);
	pDst.s32[1] = ((S32)pSrc1.s16[1] * pSrc2.s16[1]);
	pDst.s32[2] = ((S32)pSrc1.s16[2] * pSrc2.s16[2]);
	pDst.s32[3] = ((S32)pSrc1.s16[3] * pSrc2.s16[3]);

	pDst1.s32[0] = ((S32)pSrc1.s16[4] * pSrc2.s16[4]);
	pDst1.s32[1] = ((S32)pSrc1.s16[5] * pSrc2.s16[5]);
	pDst1.s32[2] = ((S32)pSrc1.s16[6] * pSrc2.s16[6]);
	pDst1.s32[3] = ((S32)pSrc1.s16[7] * pSrc2.s16[7]);

	pSrc1 = pDst;
	return pDst1.i;
}

/*!
    Multiplies four 32 bit integer values and returns the results in 64 bit float
    \par Parameters:
    \param[in] src1   - 128 bit operand containing 4 signed 32 bit values
    \param[in] src2   - 128 bit operand containing 4 signed 32 bit values
    \param[out] desLo   -
        \n desLo[0] = (multiplicand[0] * multiplier[0])
        \n desLo[1] = (multiplicand[1] * multiplier[2])
    \param[out] desHi   -
        \n desHi[0] = (multiplicand[2] * multiplier[2])
        \n desHi[1] = (multiplicand[3] * multiplier[3])
    \return none
*/
CBL_INLINE void CBL_REF::Multiply_32s64f( const __m128i & s1, const __m128i & s2,__m128d& desLo, __m128d  &desHi)
{
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( s1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( s2 );
    XMM128 & pDstlo = reinterpret_cast<XMM128&>( desLo );
    XMM128 & pDsthi = reinterpret_cast<XMM128&>( desHi );

	pDstlo.f64[0] = ((F64)pSrc1.s32[0] * pSrc2.s32[0]);
	pDstlo.f64[1] = ((F64)pSrc1.s32[1] * pSrc2.s32[1]);
	pDsthi.f64[0] = ((F64)pSrc1.s32[2] * pSrc2.s32[2]);
	pDsthi.f64[1] = ((F64)pSrc1.s32[3] * pSrc2.s32[3]);
}


/*
    Multiplies 16 unsigned 8 bit integer values
    \par Parameters:
    \param[in,out] multiplicand - 128 bit operand containing 16 unsigned 8 bit integer values
    \and the lower half of the result (in 16 bit signed)
        \n result[00] = (multiplicand[00] * multiplier[00])
        \n result[01] = (multiplicand[01] * multiplier[01])
        \n result[02] = (multiplicand[02] * multiplier[02])
        \n result[03] = (multiplicand[03] * multiplier[03])
        \n result[04] = (multiplicand[04] * multiplier[04])
        \n result[05] = (multiplicand[05] * multiplier[05])
        \n result[06] = (multiplicand[06] * multiplier[06])
        \n result[07] = (multiplicand[07] * multiplier[07])
    \param[in] multiplier       - 128 bit operand containing 16 unsigned 8 bit integer values
    \return the upper half of the result (in 16 bit signed)
        \n result[08] = (multiplicand[08] * multiplier[08])
        \n result[09] = (multiplicand[09] * multiplier[09])
        \n result[10] = (multiplicand[10] * multiplier[10])
        \n result[11] = (multiplicand[11] * multiplier[11])
        \n result[12] = (multiplicand[12] * multiplier[12])
        \n result[13] = (multiplicand[13] * multiplier[13])
        \n result[14] = (multiplicand[14] * multiplier[14])
        \n result[15] = (multiplicand[15] * multiplier[15])
*/
CBL_INLINE __m128i CBL_REF::Multiply_8u_16s_Lo(__m128i &multiplicand, const __m128i &multiplier)
{
    XMM128 pDst,pDst1;
    XMM128 & pSrc1 = reinterpret_cast<XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );

	pDst.s16[0] = (S16) pSrc1.u8[8] * pSrc2.u8[8];
	pDst.s16[1] = (S16) pSrc1.u8[9] * pSrc2.u8[9];
	pDst.s16[2] = (S16) pSrc1.u8[10] * pSrc2.u8[10];
	pDst.s16[3] = (S16) pSrc1.u8[11] * pSrc2.u8[11];
	pDst.s16[4] = (S16) pSrc1.u8[12] * pSrc2.u8[12];
	pDst.s16[5] = (S16) pSrc1.u8[13] * pSrc2.u8[13];
	pDst.s16[6] = (S16) pSrc1.u8[14] * pSrc2.u8[14];
	pDst.s16[7] = (S16) pSrc1.u8[15] * pSrc2.u8[15];

	pDst1.s16[0] = (S16) pSrc1.u8[0] * pSrc2.u8[0];
	pDst1.s16[1] = (S16) pSrc1.u8[1] * pSrc2.u8[1];
	pDst1.s16[2] = (S16) pSrc1.u8[2] * pSrc2.u8[2];
	pDst1.s16[3] = (S16) pSrc1.u8[3] * pSrc2.u8[3];
	pDst1.s16[4] = (S16) pSrc1.u8[4] * pSrc2.u8[4];
	pDst1.s16[5] = (S16) pSrc1.u8[5] * pSrc2.u8[5];
	pDst1.s16[6] = (S16) pSrc1.u8[6] * pSrc2.u8[6];
	pDst1.s16[7] = (S16) pSrc1.u8[7] * pSrc2.u8[7];

	pSrc1 = pDst1;

	return pDst.i;

}


/*
    Multiplies 16 unsigned 8 bit integer values with 8 signed 16 bit integer values
    \par Parameters:
    \param[in,out] multiplicand - 128 bit operand containing 16 unsigned 8 bit integer values
    \and the lower half of the result (in 16 bit signed)
        \n result[00] = (multiplicand[00] * multiplier[00])
        \n result[01] = (multiplicand[01] * multiplier[01])
        \n result[02] = (multiplicand[02] * multiplier[02])
        \n result[03] = (multiplicand[03] * multiplier[03])
        \n result[04] = (multiplicand[04] * multiplier[04])
        \n result[05] = (multiplicand[05] * multiplier[05])
        \n result[06] = (multiplicand[06] * multiplier[06])
        \n result[07] = (multiplicand[07] * multiplier[07])
    \param[in] multiplier       - 128 bit operand containing 8 signed 16 bit integer values
    \return the upper half of the result (in 16 bit signed)
        \n result[08] = (multiplicand[08] * multiplier[00])
        \n result[09] = (multiplicand[09] * multiplier[01])
        \n result[10] = (multiplicand[10] * multiplier[02])
        \n result[11] = (multiplicand[11] * multiplier[03])
        \n result[12] = (multiplicand[12] * multiplier[04])
        \n result[13] = (multiplicand[13] * multiplier[05])
        \n result[14] = (multiplicand[14] * multiplier[06])
        \n result[15] = (multiplicand[15] * multiplier[07])
*/
CBL_INLINE __m128i CBL_REF::Multiply_8u16s_Lo(__m128i &multiplicand, const __m128i &multiplier)
{
    XMM128 pDst,pDst1;
    XMM128 & pSrc1 = reinterpret_cast<XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );

	pDst.s16[0] = (S16) pSrc1.u8[8] *  pSrc2.s16[0];
	pDst.s16[1] = (S16) pSrc1.u8[9] *  pSrc2.s16[1];
	pDst.s16[2] = (S16) pSrc1.u8[10] * pSrc2.s16[2];
	pDst.s16[3] = (S16) pSrc1.u8[11] * pSrc2.s16[3];
	pDst.s16[4] = (S16) pSrc1.u8[12] * pSrc2.s16[4];
	pDst.s16[5] = (S16) pSrc1.u8[13] * pSrc2.s16[5];
	pDst.s16[6] = (S16) pSrc1.u8[14] * pSrc2.s16[6];
	pDst.s16[7] = (S16) pSrc1.u8[15] * pSrc2.s16[7];

	pDst1.s16[0] = (S16) pSrc1.u8[0] * pSrc2.s16[0];
	pDst1.s16[1] = (S16) pSrc1.u8[1] * pSrc2.s16[1];
	pDst1.s16[2] = (S16) pSrc1.u8[2] * pSrc2.s16[2];
	pDst1.s16[3] = (S16) pSrc1.u8[3] * pSrc2.s16[3];
	pDst1.s16[4] = (S16) pSrc1.u8[4] * pSrc2.s16[4];
	pDst1.s16[5] = (S16) pSrc1.u8[5] * pSrc2.s16[5];
	pDst1.s16[6] = (S16) pSrc1.u8[6] * pSrc2.s16[6];
	pDst1.s16[7] = (S16) pSrc1.u8[7] * pSrc2.s16[7];

	pSrc1 = pDst1;

	return pDst.i;

}

/*
    Multiplies 16 unsigned 8 bit integer values with 16 signed 16 bit integer values
    \par Parameters:
    \param[in,out] multiplicand - 128 bit operand containing 16 unsigned 8 bit integer values
    \and the lower half of the result (in 16 bit signed)
        \n result[00] = (multiplicand[00] * multiplier[00])
        \n result[01] = (multiplicand[01] * multiplier[01])
        \n result[02] = (multiplicand[02] * multiplier[02])
        \n result[03] = (multiplicand[03] * multiplier[03])
        \n result[04] = (multiplicand[04] * multiplier[04])
        \n result[05] = (multiplicand[05] * multiplier[05])
        \n result[06] = (multiplicand[06] * multiplier[06])
        \n result[07] = (multiplicand[07] * multiplier[07])
    \param[in] multiplier       - 128 bit operand containing 8 signed 16 bit integer values
    \param[in] multiplierHi     - 128 bit operand containing 8 signed 16 bit integer values
    \return the upper half of the result (in 16 bit signed)
        \n result[08] = (multiplicand[08] * multiplierHi[00])
        \n result[09] = (multiplicand[09] * multiplierHi[01])
        \n result[10] = (multiplicand[10] * multiplierHi[02])
        \n result[11] = (multiplicand[11] * multiplierHi[03])
        \n result[12] = (multiplicand[12] * multiplierHi[04])
        \n result[13] = (multiplicand[13] * multiplierHi[05])
        \n result[14] = (multiplicand[14] * multiplierHi[06])
        \n result[15] = (multiplicand[15] * multiplierHi[07])
*/
CBL_INLINE __m128i CBL_REF::Multiply_8u16s_Lo(__m128i &multiplicand, const __m128i &multiplierlo, const __m128i &multiplierhi)
{
    XMM128 pDst,pDst1;
    XMM128 & pSrc1 = reinterpret_cast<XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplierlo );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( multiplierhi );

	pDst.s16[0] = (S16) pSrc1.u8[8] *  pSrc3.s16[0];
	pDst.s16[1] = (S16) pSrc1.u8[9] *  pSrc3.s16[1];
	pDst.s16[2] = (S16) pSrc1.u8[10] * pSrc3.s16[2];
	pDst.s16[3] = (S16) pSrc1.u8[11] * pSrc3.s16[3];
	pDst.s16[4] = (S16) pSrc1.u8[12] * pSrc3.s16[4];
	pDst.s16[5] = (S16) pSrc1.u8[13] * pSrc3.s16[5];
	pDst.s16[6] = (S16) pSrc1.u8[14] * pSrc3.s16[6];
	pDst.s16[7] = (S16) pSrc1.u8[15] * pSrc3.s16[7];

	pDst1.s16[0] = (S16) pSrc1.u8[0] * pSrc2.s16[0];
	pDst1.s16[1] = (S16) pSrc1.u8[1] * pSrc2.s16[1];
	pDst1.s16[2] = (S16) pSrc1.u8[2] * pSrc2.s16[2];
	pDst1.s16[3] = (S16) pSrc1.u8[3] * pSrc2.s16[3];
	pDst1.s16[4] = (S16) pSrc1.u8[4] * pSrc2.s16[4];
	pDst1.s16[5] = (S16) pSrc1.u8[5] * pSrc2.s16[5];
	pDst1.s16[6] = (S16) pSrc1.u8[6] * pSrc2.s16[6];
	pDst1.s16[7] = (S16) pSrc1.u8[7] * pSrc2.s16[7];

	pSrc1 = pDst1;

	return pDst.i;
}


/*!
    Multiplies two 32 bit integer complex values and returns the result in 64 bit floating point
    \par Parameters:
    \param[in] multiplicand   - 128 bit operand containing two 32 bit integer complex values
    \param[in] multiplier     - 128 bit operand containing two 32 bit integer complex values
    \param[out] desLo         - Lower result of the complex multiplication of the lower complex value
        \n desLo[0] = (multiplicand[0] * multiplier[0])
    \param[out] desHi         - High result of the complex multiplication of the upper complex value
        \n desHi[1] = (multiplicand[1] * multiplier[1])
    \return the result of the complex multiplication between the 32 bit integer complex numbers;
    \only the upper value though
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE void CBL_REF::Multiply_32sc64fc( const __m128i& s1, const __m128i &s2,__m128d &desLo, __m128d  &desHi)
{
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( s1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( s2 );
    XMM128 & pDstlo = reinterpret_cast<XMM128&>( desLo );
    XMM128 & pDsthi = reinterpret_cast<XMM128&>( desHi );

	pDstlo.f64[0] = ((F64)pSrc1.s32[0] * pSrc2.s32[0]) - ((F64)pSrc1.s32[1] * pSrc2.s32[1]);
	pDstlo.f64[1] = ((F64)pSrc1.s32[0] * pSrc2.s32[1]) + ((F64)pSrc1.s32[1] * pSrc2.s32[0]);
	pDsthi.f64[0] = ((F64)pSrc1.s32[2] * pSrc2.s32[2]) - ((F64)pSrc1.s32[3] * pSrc2.s32[3]);
	pDsthi.f64[1] = ((F64)pSrc1.s32[2] * pSrc2.s32[3]) + ((F64)pSrc1.s32[3] * pSrc2.s32[2]);
}


/*!
    Multiplies two 32 bit integer complex values and returns the result in 32 bit floating point
    \par Parameters:
    \param[in] multiplicand   - 128 bit operand containing two 32 bit integer complex values
    \param[in] multiplier     - 128 bit operand containing two 32 bit integer complex values
    \return the result of the complex multiplication between the 32 bit integer complex numbers
    \except that the result is returned in 32 bit floating point complex
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128  CBL_REF::Multiply_32sc32fc(const __m128i & multiplicand, const __m128i & multiplier)
{
    XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );	

	pDst.f32[0] = ((F32)pSrc1.s32[0] * pSrc2.s32[0]) - ((F32)pSrc1.s32[1] * pSrc2.s32[1]);
	pDst.f32[1] = ((F32)pSrc1.s32[0] * pSrc2.s32[1]) + ((F32)pSrc1.s32[1] * pSrc2.s32[0]);
	pDst.f32[2] = ((F32)pSrc1.s32[2] * pSrc2.s32[2]) - ((F32)pSrc1.s32[3] * pSrc2.s32[3]);
	pDst.f32[3] = ((F32)pSrc1.s32[2] * pSrc2.s32[3]) + ((F32)pSrc1.s32[3] * pSrc2.s32[2]);

	return pDst.f;
}

/*!
    Multiplies two 32 bit integer complex values
    \par Parameters:
    \param[in] multiplicand   - 128 bit operand containing two 32 bit integer complex values
    \param[in] multiplier     - 128 bit operand containing two 32 bit integer complex values
    \return the result of the complex multiplication between the 32 bit integer complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128i CBL_REF::Multiply_32sc( const __m128i & multiplicand, const __m128i & multiplier)
{

	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( multiplicand );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( multiplier );

	pDst.s32[0] = Limits<S32>::Sat(((F64)pSrc1.s32[0] * pSrc2.s32[0]) - ((F64)pSrc1.s32[1] * pSrc2.s32[1]));
	pDst.s32[1] = Limits<S32>::Sat(((F64)pSrc1.s32[0] * pSrc2.s32[1]) + ((F64)pSrc1.s32[1] * pSrc2.s32[0]));
	pDst.s32[2] = Limits<S32>::Sat(((F64)pSrc1.s32[2] * pSrc2.s32[2]) - ((F64)pSrc1.s32[3] * pSrc2.s32[3]));
	pDst.s32[3] = Limits<S32>::Sat(((F64)pSrc1.s32[2] * pSrc2.s32[3]) + ((F64)pSrc1.s32[3] * pSrc2.s32[2]));
	return pDst.i;
}

/*!
    Calculates the magnitude of a 16 bit signed complex number and returns the magnitude in float
    \par Parameters:
    \param[in] src - 128 bit operand containing 4 signed 16 bit complex values
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \return the magnitude of 16 bit signed complex numbers; the result is floating point
        \n result[0] = magnitude[(src[0], src[1])]
        \n result[1] = magnitude[(src[2], src[3])]
        \n result[2] = magnitude[(src[4], src[5])]
        \n result[3] = magnitude[(src[6], src[7])]
    \Note: This function is identical to Magnitude_16sc_32f except that it is faster and the precision
    \is lower. This function operates of 12 bit precision for its SIMD versions
*/
CBL_INLINE __m128	CBL_REF::FastMagnitude_16sc_32f(const __m128i & src, const __m128i & min32s)
{
	XMM128 pDst;
//	XMM128 pDst1;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src );
	min32s;

	pDst.s32[0] = ((S32)pSrc1.s16[0] * pSrc1.s16[0]  +  (S32)pSrc1.s16[1] * pSrc1.s16[1]);
	pDst.s32[1] = ((S32)pSrc1.s16[2] * pSrc1.s16[2]  +  (S32)pSrc1.s16[3] * pSrc1.s16[3]);
	pDst.s32[2] = ((S32)pSrc1.s16[4] * pSrc1.s16[4]  +  (S32)pSrc1.s16[5] * pSrc1.s16[5]);
	pDst.s32[3] = ((S32)pSrc1.s16[6] * pSrc1.s16[6]  +  (S32)pSrc1.s16[7] * pSrc1.s16[7]);

	pDst.f32[0] = (sqrt((F32)pDst.s32[0]));
	pDst.f32[1] = (sqrt((F32)pDst.s32[1]));
	pDst.f32[2] = (sqrt((F32)pDst.s32[2]));
	pDst.f32[3] = (sqrt((F32)pDst.s32[3]));

	return pDst.f;
}


/*!
    Divides two 32 bit float complex values
    \par Parameters:
    \param[in] divident     - 128 bit operand containing two 32 bit complex values
    \param[in] divisor      - 128 bit operand containing two 32 bit complex values
    \param[in] negbmask - 128 bit operand containing four 32 bit values with only the MSB set for the 'b' values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
    \Note: This function is identical to the Divide_32fc_wm function but it is faster
    \at the cost of accuracy. The accuracy for the SIMD versions of this function
    \is 12 bits
*/
CBL_INLINE __m128 CBL_REF::FastDivide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask)
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( divisor );
	negbmask;
	F32 den;

	den =  (pSrc2.f32[0] * pSrc2.f32[0]  +  pSrc2.f32[1] * pSrc2.f32[1]);

	pDst.f32[0] = (pSrc1.f32[0] * pSrc2.f32[0]  +  pSrc1.f32[1] * pSrc2.f32[1])/den;
	pDst.f32[1] = (pSrc1.f32[1] * pSrc2.f32[0]  -  pSrc1.f32[0] * pSrc2.f32[1])/den;

	den =  (pSrc2.f32[2] * pSrc2.f32[2]  +  pSrc2.f32[3] * pSrc2.f32[3]);
	pDst.f32[2] = (pSrc1.f32[2] * pSrc2.f32[2]  +  pSrc1.f32[3] * pSrc2.f32[3])/den;
	pDst.f32[3] = (pSrc1.f32[3] * pSrc2.f32[2]  -  pSrc1.f32[2] * pSrc2.f32[3])/den;

	return pDst.f;
}

/*!
    Divides two 32 bit float complex values
    \par Parameters:
    \param[in] divident     - 128 bit operand containing two 32 bit complex values
    \param[in] divisor      - 128 bit operand containing two 32 bit complex values
    \param[in] negbmask - 128 bit operand containing four 32 bit values with only the MSB set for the 'b' values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128 CBL_REF::Divide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask)
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( divisor );
	negbmask;
	F32 den;

	den =  (pSrc2.f32[0] * pSrc2.f32[0]  +  pSrc2.f32[1] * pSrc2.f32[1]);

	pDst.f32[0] = (pSrc1.f32[0] * pSrc2.f32[0]  +  pSrc1.f32[1] * pSrc2.f32[1])/den;
	pDst.f32[1] = (pSrc1.f32[1] * pSrc2.f32[0]  -  pSrc1.f32[0] * pSrc2.f32[1])/den;

	den =  (pSrc2.f32[2] * pSrc2.f32[2]  +  pSrc2.f32[3] * pSrc2.f32[3]);
	pDst.f32[2] = (pSrc1.f32[2] * pSrc2.f32[2]  +  pSrc1.f32[3] * pSrc2.f32[3])/den;
	pDst.f32[3] = (pSrc1.f32[3] * pSrc2.f32[2]  -  pSrc1.f32[2] * pSrc2.f32[3])/den;

	return pDst.f;
}

/*!
    Divides two 32 bit float complex values
    \par Parameters:
    \param[in] divident     - 128 bit operand containing two 32 bit complex values
    \param[in] divisor      - 128 bit operand containing two 32 bit complex values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
    \Note: This function is identical to the Divide_32fc function but it is faster
    \at the cost of accuracy. The accuracy for the SIMD versions of this function
    \is 12 bits
*/
CBL_INLINE __m128 CBL_REF::FastDivide_32fc( const __m128 & divident, const __m128 & divisor)
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( divisor );
	F32 den;

	den =  (pSrc2.f32[0] * pSrc2.f32[0]  +  pSrc2.f32[1] * pSrc2.f32[1]);

	pDst.f32[0] = (pSrc1.f32[0] * pSrc2.f32[0]  +  pSrc1.f32[1] * pSrc2.f32[1])/den;
	pDst.f32[1] = (pSrc1.f32[1] * pSrc2.f32[0]  -  pSrc1.f32[0] * pSrc2.f32[1])/den;

	den =  (pSrc2.f32[2] * pSrc2.f32[2]  +  pSrc2.f32[3] * pSrc2.f32[3]);
	pDst.f32[2] = (pSrc1.f32[2] * pSrc2.f32[2]  +  pSrc1.f32[3] * pSrc2.f32[3])/den;
	pDst.f32[3] = (pSrc1.f32[3] * pSrc2.f32[2]  -  pSrc1.f32[2] * pSrc2.f32[3])/den;

	return pDst.f;
}


/*!
    Divides a 64 bit float complex value
    \par Parameters:
    \param[in] divident     - 128 bit operand containing a 64 bit complex value
    \param[in] divisor      - 128 bit operand containing two 32 bit complex values
    \param[out] zeromask    - 128 bit operand which will contain a 1's mask for every value where the denominator was zero
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
  
*/
CBL_INLINE __m128d CBL_REF::Divide_64fc( const __m128d & divident, const __m128d & divisor, __m128d & zeromask )
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( divisor );
	zeromask;

	F64 den;

	den =  (pSrc2.f64[0] * pSrc2.f64[0]  +  pSrc2.f64[1] * pSrc2.f64[1]);

	pDst.f64[0] = (pSrc1.f64[0] * pSrc2.f64[0]  +  pSrc1.f64[1] * pSrc2.f64[1])/den;
	pDst.f64[1] = (pSrc1.f64[1] * pSrc2.f64[0]  -  pSrc1.f64[0] * pSrc2.f64[1])/den;

	return pDst.d;
}


/*!
    Divides two 32 bit float complex values
    \par Parameters:
    \param[in] src1     - 128 bit operand containing two 32 bit complex values
    \param[in] src2     - 128 bit operand containing two 32 bit complex values
    \return the result of the complex multiplication between the 32 bit complex numbers
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
  
*/
CBL_INLINE __m128 CBL_REF::Divide_32fc( const __m128 & divident, const __m128 & divisor)
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( divisor );

	F32 den;

	den =  (pSrc2.f32[0] * pSrc2.f32[0]  +  pSrc2.f32[1] * pSrc2.f32[1]);

	pDst.f32[0] = (pSrc1.f32[0] * pSrc2.f32[0]  +  pSrc1.f32[1] * pSrc2.f32[1])/den;
	pDst.f32[1] = (pSrc1.f32[1] * pSrc2.f32[0]  -  pSrc1.f32[0] * pSrc2.f32[1])/den;

	den =  (pSrc2.f32[2] * pSrc2.f32[2]  +  pSrc2.f32[3] * pSrc2.f32[3]);
	pDst.f32[2] = (pSrc1.f32[2] * pSrc2.f32[2]  +  pSrc1.f32[3] * pSrc2.f32[3])/den;
	pDst.f32[3] = (pSrc1.f32[3] * pSrc2.f32[2]  -  pSrc1.f32[2] * pSrc2.f32[3])/den;

	return pDst.f;
}

/*!
    Divides four 16 bit complex values and returns the result in floating point
    \par Parameters:
    \param[in] src1   - 128 bit operand containing four 16 bit complex values
    \param[in] src2   - 128 bit operand containing four 16 bit complex values
    \param[out] dstLo - 128 bit operand which will contain the 3rd and 4th result of the complex multiplication
        \n dstLo[0] = (src1[2] * src2[2])
        \n dstLo[1] = (src1[3] * src2[3])
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \param[out] zeromask    - 128 bit operand which will contain a 1's mask for every value where the denominator was zero
    \return the 1st and the second result of the complex multiplication
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
*/
CBL_INLINE __m128 CBL_REF::Divide_16sc_32fc(const __m128i & src1, const __m128i & src2, __m128 & destLo, const __m128i & min32s, __m128i & zeromask)
{
	XMM128 pDst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    XMM128 & pDestLo = reinterpret_cast<XMM128&>( destLo );
    XMM128 & pZm = reinterpret_cast<XMM128&>( zeromask );
	min32s;

	F32 den;

	den =  ((F32)pSrc2.s16[0] * pSrc2.s16[0]  +  (F32)pSrc2.s16[1] * pSrc2.s16[1]);
	pZm.s32[0]  = den == 0.0F ? 0xFFFFFFFF : 0; 
	pDestLo.f32[0] = ((F32)pSrc1.s16[0] * pSrc2.s16[0]  +  (F32)pSrc1.s16[1] * pSrc2.s16[1])/den;
	pDestLo.f32[1] = ((F32)pSrc1.s16[1] * pSrc2.s16[0]  -  (F32)pSrc1.s16[0] * pSrc2.s16[1])/den;
	den =  ((F32)pSrc2.s16[2] * pSrc2.s16[2]  + (F32) pSrc2.s16[3] * pSrc2.s16[3]);
	pZm.s32[1]  = den == 0.0F ? 0xFFFFFFFF : 0; 
	pDestLo.f32[2] = ((F32)pSrc1.s16[2] * pSrc2.s16[2]  +  (F32)pSrc1.s16[3] * pSrc2.s16[3])/den;
	pDestLo.f32[3] = ((F32)pSrc1.s16[3] * pSrc2.s16[2]  -  (F32)pSrc1.s16[2] * pSrc2.s16[3])/den;

	den =  ((F32)pSrc2.s16[4] * pSrc2.s16[4]  +  (F32)pSrc2.s16[5] * pSrc2.s16[5]);
	pZm.s32[2]  = den == 0.0F ? 0xFFFFFFFF : 0; 
	pDst.f32[0] = ((F32)pSrc1.s16[4] * pSrc2.s16[4]  +  (F32)pSrc1.s16[5] * pSrc2.s16[5])/den;
	pDst.f32[1] = ((F32)pSrc1.s16[5] * pSrc2.s16[4]  -  (F32)pSrc1.s16[4] * pSrc2.s16[5])/den;
	den =  ((F32)pSrc2.s16[6] * pSrc2.s16[6]  +  (F32)pSrc2.s16[7] * pSrc2.s16[7]);
	pZm.s32[3]  = den == 0.0F ? 0xFFFFFFFF : 0; 
	pDst.f32[2] = ((F32)pSrc1.s16[6] * pSrc2.s16[6]  +  (F32)pSrc1.s16[7] * pSrc2.s16[7])/den;
	pDst.f32[3] = ((F32)pSrc1.s16[7] * pSrc2.s16[6]  -  (F32)pSrc1.s16[6] * pSrc2.s16[7])/den;

	return pDst.f;

}

/*!
    Divides four 16 bit complex values and returns the result in floating point
    \par Parameters:
    \param[in] src1   - 128 bit operand containing four 16 bit complex values
    \param[in] src2   - 128 bit operand containing four 16 bit complex values
    \param[in] min32s - 128 bit operand containing 4 signed 32 bit values containing negative max for 32 bit
    \return the result of the complex multiplication
        \n result[0] = (src1[0] * src2[0])
        \n result[1] = (src1[1] * src2[1])
        \n result[2] = (src1[2] * src2[2])
        \n result[3] = (src1[3] * src2[3])
*/
CBL_INLINE __m128i CBL_REF::Divide_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s)
{
	
	XMM128 pDst,pDst1,pDst2,temp;
	pDst2.f = Divide_16sc_32fc(src1,src2,pDst1.f,min32s,temp.i);
	pDst.s16[0] = Limits<S16>::Sat(FloatToInt(pDst1.f32[0]));
	pDst.s16[1] = Limits<S16>::Sat(FloatToInt(pDst1.f32[1]));
	pDst.s16[2] = Limits<S16>::Sat(FloatToInt(pDst1.f32[2]));
	pDst.s16[3] = Limits<S16>::Sat(FloatToInt(pDst1.f32[3]));
	pDst.s16[4] = Limits<S16>::Sat(FloatToInt(pDst2.f32[0]));
	pDst.s16[5] = Limits<S16>::Sat(FloatToInt(pDst2.f32[1]));
	pDst.s16[6] = Limits<S16>::Sat(FloatToInt(pDst2.f32[2]));
	pDst.s16[7] = Limits<S16>::Sat(FloatToInt(pDst2.f32[3]));

	return pDst.i;
}


/*!
    Divides two 32 bit float complex values by a constant
    \par Parameters:
    \param[in] divident     - 128 bit operand containing two 32 bit complex values
    \param[in] divisorC     - 128 bit operand containing two 32 bit complex values
    \param[in] divisorCRev  - 128 bit operand containing two 32 bit complex values which are a reverse of the
    \real and imaginary parts of the values in divisorC
    \return the result of the complex multiplication between the 32 bit complex numbers and the constant value
        \n result[0] = (divident[0] * divisorC)
        \n result[1] = (divident[1] * divisorC)
*/
CBL_INLINE __m128 CBL_REF::DivideC_32fc( const __m128 & divident, const __m128 & divisorC, const __m128 & divisorCRev)
{
	XMM128 pDst,pDst1,pDst2;
    const XMM128 & pDiv = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pDsr = reinterpret_cast<const XMM128&>( divisorC );
    const XMM128 & pDsrre = reinterpret_cast<const XMM128&>( divisorCRev );


	pDst1.f32[0] = pDiv.f32[0] * pDsr.f32[0];
	pDst1.f32[1] = pDiv.f32[1] * pDsr.f32[1];
	pDst1.f32[2] = pDiv.f32[2] * pDsr.f32[2];
	pDst1.f32[3] = pDiv.f32[3] * pDsr.f32[3];


	pDst2.f32[0] = pDiv.f32[0] * pDsrre.f32[0];
	pDst2.f32[1] = pDiv.f32[1] * pDsrre.f32[1];
	pDst2.f32[2] = pDiv.f32[2] * pDsrre.f32[2];
	pDst2.f32[3] = pDiv.f32[3] * pDsrre.f32[3];

	pDst.f32[0] = pDst1.f32[0] + pDst1.f32[1];
	pDst.f32[1] = pDst2.f32[0] + pDst2.f32[1];
	pDst.f32[2] = pDst1.f32[3] + pDst1.f32[2];
	pDst.f32[3] = pDst2.f32[3] + pDst2.f32[2];

	return pDst.f;

}

/*!
    Divides one 64 bit float complex value by a constant
    \par Parameters:
    \param[in] divident     - 128 bit operand containing one 64 bit complex values
    \param[in] divisorC     - 128 bit operand containing one 64 bit complex values
    \param[in] divisorCRev  - 128 bit operand containing one 64 bit complex values which is a reverse of the 
    \real and imaginary parts of the values in divisorC
    \return the result of the complex multiplication between the 64 bit complex number and the constant value
        \n result[0] = (divident[0] * divisorC)
*/
CBL_INLINE __m128d CBL_REF::DivideC_64fc( const __m128d & divident, const __m128d & divisorC, const __m128d & divisorCRev)
{
	XMM128 pDst,pDst1,pDst2;
    const XMM128 & pDiv = reinterpret_cast<const XMM128&>( divident );
    const XMM128 & pDsr = reinterpret_cast<const XMM128&>( divisorC );
    const XMM128 & pDsrre = reinterpret_cast<const XMM128&>( divisorCRev );


	pDst1.f64[0] = pDiv.f64[0] * pDsr.f64[0];
	pDst1.f64[1] = pDiv.f64[1] * pDsr.f64[1];

	pDst2.f64[0] = pDiv.f64[0] * pDsrre.f64[0];
	pDst2.f64[1] = pDiv.f64[1] * pDsrre.f64[1];

	pDst.f64[0] = pDst1.f64[0] + pDst1.f64[1];
	pDst.f64[1] = pDst2.f64[0] + pDst2.f64[1];

	return pDst.d;

}




#pragma warning( push )
#pragma warning( disable: 4701 )

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 single precision floating point values
    \param[in] src2 - second 128 bit operand containing 4 single precision floating point values
    \param[in] src3 - third 128 bit operand containing 4 single precision floating point values
    \return result = src1 * src2 + src3
*/
CBL_INLINE __m128 CBL_REF::MultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 4; n++ )
    {
        dst.f32[n] = pSrc1.f32[n] * pSrc2.f32[n] + pSrc3.f32[n];
    }

    return dst.f;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 2 double precision floating point values
    \param[in] src2 - second 128 bit operand containing 2 double precision floating point values
    \param[in] src3 - third 128 bit operand containing 2 double precision floating point values
    \return result = src1 * src2 + src3
*/
CBL_INLINE __m128d CBL_REF::MultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 2; n++ )
    {
        dst.f64[n] = pSrc1.f64[n] * pSrc2.f64[n] + pSrc3.f64[n];
    }

    return dst.d;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 single precision floating point values
    \param[in] src2 - second 128 bit operand containing 4 single precision floating point values
    \param[in] src3 - third 128 bit operand containing 4 single precision floating point values
    \return result = src1 * src2 - src3
*/
CBL_INLINE __m128 CBL_REF::MultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 4; n++ )
    {
        dst.f32[n] = pSrc1.f32[n] * pSrc2.f32[n] - pSrc3.f32[n];
    }

    return dst.f;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 2 double precision floating point values
    \param[in] src2 - second 128 bit operand containing 2 double precision floating point values
    \param[in] src3 - third 128 bit operand containing 2 double precision floating point values
    \return result = src1 * src2 - src3
*/
CBL_INLINE __m128d CBL_REF::MultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 2; n++ )
    {
        dst.f64[n] = pSrc1.f64[n] * pSrc2.f64[n] - pSrc3.f64[n];
    }

    return dst.d;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 single precision floating point values
    \param[in] src2 - second 128 bit operand containing 4 single precision floating point values
    \param[in] src3 - third 128 bit operand containing 4 single precision floating point values
    \return result = -(src1 * src2) + src3
*/
CBL_INLINE __m128 CBL_REF::NegMultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 4; n++ )
    {
        dst.f32[n] = - pSrc1.f32[n] * pSrc2.f32[n] + pSrc3.f32[n];
    }

    return dst.f;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 2 double precision floating point values
    \param[in] src2 - second 128 bit operand containing 2 double precision floating point values
    \param[in] src3 - third 128 bit operand containing 2 double precision floating point values
    \return result = -(src1 * src2) + src3
*/
CBL_INLINE __m128d CBL_REF::NegMultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 2; n++ )
    {
        dst.f64[n] = - pSrc1.f64[n] * pSrc2.f64[n] + pSrc3.f64[n];
    }

    return dst.d;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 4 single precision floating point values
    \param[in] src2 - second 128 bit operand containing 4 single precision floating point values
    \param[in] src3 - third 128 bit operand containing 4 single precision floating point values
    \return result = -(src1 * src2) - src3
*/
CBL_INLINE __m128 CBL_REF::NegMultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 4; n++ )
    {
        dst.f32[n] = - pSrc1.f32[n] * pSrc2.f32[n] - pSrc3.f32[n];
    }

    return dst.f;
}

/*!
    \par Parameters:
    \param[in] src1 - first 128 bit operand containing 2 double precision floating point values
    \param[in] src2 - second 128 bit operand containing 2 double precision floating point values
    \param[in] src3 - third 128 bit operand containing 2 double precision floating point values
    \return result = -(src1 * src2) - src3
*/
CBL_INLINE __m128d CBL_REF::NegMultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 )
{
    XMM128 dst;
    const XMM128 & pSrc1 = reinterpret_cast<const XMM128&>( src1 );
    const XMM128 & pSrc2 = reinterpret_cast<const XMM128&>( src2 );
    const XMM128 & pSrc3 = reinterpret_cast<const XMM128&>( src3 );

    for( int n = 0; n < 2; n++ )
    {
        dst.f64[n] = - pSrc1.f64[n] * pSrc2.f64[n] - pSrc3.f64[n];
    }

    return dst.d;
}

#pragma warning( pop )

#endif // __CBL_ARITHMETIC_PRIMITIVES_H__
