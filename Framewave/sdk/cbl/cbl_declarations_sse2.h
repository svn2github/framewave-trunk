/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/
 

// cbl_declarations_sse2.h

/// @cond OPT_DETAILS
struct CBL_SSE2 : public CBL_REF
{
    ////////////////////////////////////////////////////////////////////////////
    // MEMORY ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group1
 *  @{
 */

/** @name SSE2 Load operations */
//@{
    CBL_INLINE static void Load_1x16_8u( __m128i & variable, U8 value );
    CBL_INLINE static void Load_1x16_8s( __m128i & variable, S8 value );
    CBL_INLINE static void Load_1x8_16u( __m128i & variable, U16 value );
    CBL_INLINE static void Load_1x8_16s( __m128i & variable, S16 value );
    CBL_INLINE static void Load_2x4_16u( __m128i & variable, U16 value1, U16 value2 );
    CBL_INLINE static void Load_2x4_16s( __m128i & variable, S16 value1, S16 value2 );
    CBL_INLINE static void Load_1x4_32u( __m128i & variable, U32 value );
    CBL_INLINE static void Load_1x4_32s( __m128i & variable, S32 value );
    CBL_INLINE static void Load_1x4_32f( __m128  & variable, F32 value );
//@}

/** @name Fractional Load operations */
//@{
    //! Load 192 bits of data
    CBL_INLINE static void Load_192( __m128i & rg_lo, __m128i & rg_hi, const __m128i* pMemory );
    //! Load 96 bits of data
    CBL_INLINE static void Load_96( __m128i & rg, const __m128i* pMemory );
    //! Load 64 bits of data
    CBL_INLINE static void Load_64( __m128i & rg, const __m128i* pMemory );
    //! Load 32 bits of data
    CBL_INLINE static void Load_32( __m128i & rg, const __m128i* pMemory );
//@}

/** @name Fractional Store operations */
//@{
    //! Store 192 bits of data
    CBL_INLINE static void Store_192( __m128i* pMemory, __m128i & rg_low, __m128i & rg_hi );
    //! Store 96 bits of data
    CBL_INLINE static void Store_96( __m128i* pMemory, __m128i & rg );
    //! Store 64 bits of data
    CBL_INLINE static void Store_64( __m128i* pMemory, __m128i & rg );
    //! Store 32 bits of data
    CBL_INLINE static void Store_32( __m128i* pMemory, __m128i & rg );
    //! Store 24 bits of data
//@}

/** @name Lookup-table operations */
//@{
	CBL_INLINE static void LUT_8u( __m128i & reg, U8* table );
//@}

/** @} */ // end of Group1

    ////////////////////////////////////////////////////////////////////////////
    // ARITHMETIC ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group2
 *  @{
 */

/** @name SSE2 Scale Routines */
//@{
	// always match the corret scale setup routine with the scale routine
	// Using the template parameter gaurantees that that the correct scaling
	// will be performed, as long as the setup matches the scale function
	template <int scaleType>
	CBL_INLINE static void Setup_ScaleMasks_16s(const int &scale, __m128i scaleMasksp[]);
	template <int scaleType>
	CBL_INLINE static void Setup_ScaleMasks_32s(const int &scale, __m128i scaleMasksp[]);
	template <int scaleType>
	CBL_INLINE static void Setup_ScaleMasks_32f(const int &scale, __m128 scaleMasksp[]);


	template <int scaleType>
	CBL_INLINE static __m128i Scale_16s(const __m128i &value, const __m128i scaleMasksp[]);
	template <int scaleType>
	CBL_INLINE static __m128i Scale_16s_lr1(const __m128i &value, const __m128i scaleMasksp[]);
	template <int scaleType>
	CBL_INLINE static __m128i Scale_16s_custom(const __m128i &value, const __m128i &scaleMasks0, 
																	const __m128i &scaleMasks1, 
																	const __m128i &scaleMasks2, 
																	const __m128i &scaleMasks3 );
	template <int scaleType>
	CBL_INLINE static __m128i Scale_32s(const __m128i &value, const __m128i scaleMasksp[]);
	template <int scaleType>
	CBL_INLINE static __m128 Scale_32f(const __m128 &value, const __m128 scaleMasksp[]);

//@}


/** @name Generic Compare */
//@{
    //! Compares 2 unsigned 8-bit values, less than operation
    CBL_INLINE static __m128i compare_lt_8u( const __m128i & src1, const __m128i & src2 );
	 //! Compares 2 unsigned 8-bit values, less than or equal to operation
    CBL_INLINE static __m128i compare_le_8u( const __m128i & src1, const __m128i & src2 );
	//! Compares 2 unsigned 8-bit values, greater than operation
    CBL_INLINE static __m128i compare_gt_8u( const __m128i & src1, const __m128i & src2 );
	//! Compares 2 unsigned 8-bit values, greater than or equl to operation
    CBL_INLINE static __m128i compare_ge_8u( const __m128i & src1, const __m128i & src2 );
//@}


/** @name SSE2 Multiplication */
//@{
    CBL_INLINE static __m128i	Multiply_8u16s_Lo( __m128i &multiplicand, const __m128i &multiplier );
	CBL_INLINE static __m128i	Multiply_8u16s_Lo(__m128i &multiplicand, const __m128i &multiplierlo, const __m128i &multiplierhi);
	CBL_INLINE static __m128i	Multiply_8u_16s_Lo( __m128i & multiplicand, const __m128i & multiplier );
	CBL_INLINE static __m128i	Multiply_16s_32s( __m128i & multiplicand, const __m128i & multiplier );

    CBL_INLINE static void		Multiply_32s_Lo( __m128i & multiplicand, const __m128i & multiplier );
    CBL_INLINE static void		Multiply_32s_Hi( __m128i & multiplicand, const __m128i & multiplier );
    CBL_INLINE static void		Multiply_32s( __m128i & multiplicand, const __m128i & multiplier );
    CBL_INLINE static void		Multiply_32s( __m128i & hiResult, __m128i & multiplicand, const __m128i & multiplier );
    CBL_INLINE static void		Multiply_64s_Lo( __m128i & multiplicand, const __m128i & multiplier );
    CBL_INLINE static void		Multiply_64s_Hi( __m128i & multiplicand, const __m128i & multiplier );
//@}


/** @name Generic MultiplyAccumulate  */
//@{
	//! Multiply two 16bit signed integers and add a 16bit signed integer without saturation.
	CBL_INLINE static __m128i MultiplyAccumulate_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply two 16bit signed integers and add a 16bit signed integer with saturation.
	CBL_INLINE static __m128i MultiplyAccumulateSat_S16toS16( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply two 16bit signed integers and add a 32bit signed integer without saturation.	
	CBL_INLINE static __m128i MultiplyAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply two 16bit signed integers and add a 32bit signed integer with saturation.
	CBL_INLINE static __m128i MultiplyAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply two 32bit signed integers and add a 32bit signed integer without saturation.
	CBL_INLINE static __m128i MultiplyAccumulate_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply two 32bit signed integers and add a 32bit signed integer with saturation.
	CBL_INLINE static __m128i MultiplyAccumulateSat_S32toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply accumulate signed low double word to signed quad word
	CBL_INLINE static __m128i MultiplyAccumulate_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply accumulate signed low double word to signed quad word with saturation
	CBL_INLINE static __m128i MultiplyAccumulateSat_S32LtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply accumulate signed high double word to signed quad word
	CBL_INLINE static __m128i MultiplyAccumulate_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply accumulate signed high double word to signed quad word with saturation
	CBL_INLINE static __m128i MultiplyAccumulateSat_S32HtoS64( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply, add and accumulate signed word to signed double word
	CBL_INLINE static __m128i MultiplyAddAccumulate_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
	//! Multiply, add and accumulate signed word to signed double word with saturation
	CBL_INLINE static __m128i MultiplyAddAccumulateSat_S16toS32( const __m128i &src1, const __m128i &src2, const __m128i &src3 );
//@}

/** @name Floating point multiply and add/subtract */
//@{
    //! Multiply and add single-precision floating point values
    CBL_INLINE static __m128 MultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 );
    //! Multiply and add double-precision floating point values
    CBL_INLINE static __m128d MultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 );
    //! Multiply and subtract single-precision floating point values
    CBL_INLINE static __m128 MultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 );
    //! Multiply and subtract double-precision floating point values
    CBL_INLINE static __m128d MultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 );
    //! Negative, multiply and add single-precision floating point values
    CBL_INLINE static __m128 NegMultiplyAndAdd_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 );
    //! Negative, multiply and add double-precision floating point values
    CBL_INLINE static __m128d NegMultiplyAndAdd_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 );
    //! Negative, multiply and subtract single-precision floating point values
    CBL_INLINE static __m128 NegMultiplyAndSubtract_F32( const __m128 &src1, const __m128 &src2, const __m128 &src3 );
    //! Negative, multiply and subtract double-precision floating point values
    CBL_INLINE static __m128d NegMultiplyAndSubtract_F64( const __m128d &src1, const __m128d &src2, const __m128d &src3 );
//@}


/** @name SSE2 Complex multiplication */
//@{
	CBL_INLINE static __m128	Multiply_32fc( const __m128 & multiplicand, const __m128 & multiplier );
	CBL_INLINE static __m128	Multiply_32fc_wm( const __m128 & multiplicand, const __m128 & multiplier, const __m128 negamask);

	CBL_INLINE static __m128d	Multiply_64fc( const __m128d & multiplicand, const __m128d & multiplier );
	CBL_INLINE static __m128i	Multiply_32sc( const __m128i & multiplicand, const __m128i & multiplier );
	CBL_INLINE static __m128i	Multiply_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s, const __m128i & negb);
	CBL_INLINE static __m128i	Multiply_16sc_32sc(__m128i & src1DstLo, const __m128i & src2, const __m128i & min32s, const __m128i & negb);

	CBL_INLINE static void Multiply_16sc32fc(const __m128i &multiplicand,const __m128i &multiplier,__m128 &destLo, __m128 &destHi);
	CBL_INLINE static __m128  Multiply_32s32f( const __m128i &s1, const __m128i& s2);
	CBL_INLINE static __m128  Multiply_32sc32fc(const __m128i & multiplicand, const __m128i & multiplier);
	CBL_INLINE static void  Multiply_32s64f( const __m128i & s1, const __m128i & s2,__m128d& desLo, __m128d  &desHi);
	CBL_INLINE static void  Multiply_32sc64fc( const __m128i& s1, const __m128i &s2,__m128d &desLo, __m128d  &desHi);

    CBL_INLINE static __m128 FastMagnitude_16sc_32f(const __m128i & src, const __m128i & min32s);
	CBL_INLINE static __m128 Magnitude_16sc_32f(const __m128i & src, const __m128i & min32s);
	
//@}

/** @name SSE2 Division by a Constant */
//@{
	CBL_INLINE static U8 PrepDiv_16s_const( __m128i & mm, __m128i & ss, S16 divisor );
	CBL_INLINE static __m128i Divide_16s_const( __m128i & d, const __m128i & mm, __m128i ss, U8 executePath );
	template< U8 codepath >
	CBL_INLINE static __m128i Divide_16s_const( __m128i & d, const __m128i & mm, __m128i ss )
	{
		return Divide_16s_const( d, mm, ss, codepath );
	}

private:
	CBL_INLINE static __m128i DIVISION16_SSE2_1( __m128i d, const __m128i & m );
	CBL_INLINE static __m128i DIVISION16_SSE2_1_NEG( __m128i d, const __m128i & m );
	CBL_INLINE static __m128i DIVISION16_SSE2_2( __m128i d, const __m128i & m );
	CBL_INLINE static __m128i DIVISION16_SSE2_2_NEG( __m128i d, const __m128i & m );
	CBL_INLINE static __m128i DIVISION16_SSE2_1_S( __m128i d, const __m128i & m, const __m128i & s );
	CBL_INLINE static __m128i DIVISION16_SSE2_1_S_NEG( __m128i d, const __m128i & m, const __m128i & s );
	CBL_INLINE static __m128i DIVISION16_SSE2_2_S( __m128i d, const __m128i & m, const __m128i & s );
	CBL_INLINE static __m128i DIVISION16_SSE2_2_S_NEG( __m128i d, const __m128i & m, const __m128i & s );

public:
    CBL_INLINE static __m128i FastDivide_16s_const( const __m128i & dividend, const __m128i & operand );
//@}

/** @name SSE2 Complex division */
//@{
	CBL_INLINE static __m128	Divide_32fc( const __m128 & divident, const __m128 & divisor);
	CBL_INLINE static __m128	Divide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask);

	CBL_INLINE static __m128	FastDivide_32fc( const __m128 & divident, const __m128 & divisor);
	CBL_INLINE static __m128	FastDivide_32fc_wm( const __m128 & divident, const __m128 & divisor, const __m128 negbmask);

	CBL_INLINE static __m128d	Divide_64fc( const __m128d & divident, const __m128d & divisor, __m128d & zeromask );
	CBL_INLINE static __m128i	Divide_16sc(const __m128i & src1, const __m128i & src2, const __m128i & min32s);
	CBL_INLINE static __m128	Divide_16sc_32fc(const __m128i & src1, const __m128i & src2, __m128 & destLo, const __m128i & min32s, __m128i & zeromask);
	CBL_INLINE static __m128	DivideC_32fc( const __m128 & divident, const __m128 & divisorC, const __m128 & divisorCRev);
	CBL_INLINE static __m128d	DivideC_64fc( const __m128d & divident, const __m128d & divisorC, const __m128d & divisorCRev);
//@}

/** @name SSE2 Horizontal Add and Subtract */
//@{
    //! Horizontal add pairs of single precision floating point values
    CBL_INLINE static __m128 HorizontalAdd_32f( const __m128 & src1, const __m128 & src2 );
    //! Horizontal subtract pairs of single precision floating point values
    CBL_INLINE static __m128 HorizontalSubtract_32f( const __m128 & src1, const __m128 & src2 );
    //! Add and subtract pairs of single precision floating point values
    CBL_INLINE static __m128 AddAndSubtract_32f( const __m128 & src1, const __m128 & src2 );
    //! Horizontal add pairs of double precision floating point values
    CBL_INLINE static __m128d HorizontalAdd_64f( const __m128d & src1, const __m128d & src2 );
    //! Horizontal subtract pairs of double precision floating point values
    CBL_INLINE static __m128d HorizontalSubtract_64f( const __m128d & src1, const __m128d & src2 );
    //! Add and subtract pairs of double precision floating point values
    CBL_INLINE static __m128d AddAndSubtract_64f( const __m128d & src1, const __m128d & src2 );

	//! Horizontal add consecutive signed 8bit numbers to a 16bit number.
	CBL_INLINE static __m128i HorizontalAdd_S8toS16( const __m128i & src );
	//! Horizontal add four consecutive signed 8bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalAdd_S8toS32( const __m128i & src );
	//! Horizontal add eight consecutive signed 8bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_S8toS64( const __m128i & src );
	//! Horizontal add  consecutive signed 16bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalAdd_S16toS32( const __m128i & src );
	//! Horizontal add  four consecutive signed 16bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_S16toS64( const __m128i & src );
	//! Horizontal add  consecutive signed 32bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_S32toS64( const __m128i & src );
	//! Horizontal add consecutive unsigned 8bit numbers to a 16bit number.
	CBL_INLINE static __m128i HorizontalAdd_U8toU16( const __m128i & src );
	//! Horizontal add four consecutive unsigned 8bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalAdd_U8toU32( const __m128i & src );
	//! Horizontal add eight consecutive unsigned 8bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_U8toU64( const __m128i & src );
	//! Horizontal add  consecutive unsigned 16bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalAdd_U16toU32( const __m128i & src );
	//! Horizontal add  four consecutive unsigned 16bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_U16toU64( const __m128i & src );
	//! Horizontal add  consecutive unsigned 32bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalAdd_U32toU64( const __m128i & src );

	//! Horizontal Subtract consecutive signed 8bit numbers to a 16bit number.
	CBL_INLINE static __m128i HorizontalSubtract_S8toS16( const __m128i & src );
	//! Horizontal Subtract consecutive signed 16bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalSubtract_S16toS32( const __m128i & src );
	//! Horizontal Subtract consecutive signed 32bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalSubtract_S32toS64( const __m128i & src );
	//! Horizontal Subtract consecutive unsigned 8bit numbers to a 16bit number.
	CBL_INLINE static __m128i HorizontalSubtract_U8toU16( const __m128i & src );
	//! Horizontal Subtract consecutive unsigned 16bit numbers to a 32bit number.
	CBL_INLINE static __m128i HorizontalSubtract_U16toU32( const __m128i & src );
	//! Horizontal Subtract consecutive unsigned 32bit numbers to a 64bit number.
	CBL_INLINE static __m128i HorizontalSubtract_U32toU64( const __m128i & src );

	//! Horizontal Add consecutive signed 16bit numbers to a 16bit number without saturation.
	CBL_INLINE static __m128i HorizontalAdd_S16( const __m128i & src1, const __m128i & src2 );
	//! Horizontal Add consecutive signed 16bit numbers to a 16bit number with saturation.
	CBL_INLINE static __m128i HorizontalAddSat_S16( const __m128i & src1, const __m128i & src2 );
	//! Horizontal Subtract consecutive signed 16bit numbers to a 16bit number without saturation.
	CBL_INLINE static __m128i HorizontalSubtract_S16( const __m128i & src1, const __m128i & src2 );
	//! Horizontal Subtract consecutive signed 16bit numbers to a 16bit number with saturation.
	CBL_INLINE static __m128i HorizontalSubtractSat_S16( const __m128i & src1, const __m128i & src2 );
	//! Horizontal Subtract consecutive signed 32bit numbers to a 32bit number without saturation.
	CBL_INLINE static __m128i HorizontalAdd_S32( const __m128i & src1, const __m128i & src2 );
	//! Horizontal Subtract consecutive signed 32bit numbers to a 32bit number with saturation.
	CBL_INLINE static __m128i HorizontalSubtract_S32( const __m128i & src1, const __m128i & src2 );

	//! Horizontal Add eight consecutive signed 16bit numbers to a 16bit number without saturation.
	CBL_INLINE static void HorizontalAdd_16s( __m128i & values );
	//! Horizontal Add consecutive signed 16bit numbers to a 16bit number without saturation.
	CBL_INLINE static void HorizontalAdd_16s( __m128i & value1, const __m128i & value2 );
	//! Horizontal Subtract eight consecutive signed 16bit numbers to a 16bit number without saturation.
	CBL_INLINE static void HorizontalSub_16s( __m128i & value1, const __m128i & value2 );
	//! Horizontal Add four consecutive signed 32bit numbers to a 32bit number without saturation.
	CBL_INLINE static void HorizontalAdd_32s( __m128i & values );
	//! Horizontal Add consecutive signed 32bit numbers to a 32bit number without saturation.
	CBL_INLINE static void HorizontalAdd_32s( __m128i & value1, const __m128i & value2 );
	//! Horizontal Subtract consecutive signed 32bit numbers to a 32bit number without saturation.
	CBL_INLINE static void HorizontalSub_32s( __m128i & value1, const __m128i & value2 );


//@}

/** @name Generic ABS routines */
//@{
	//! ABS of 8 bit values
	CBL_INLINE static __m128i Abs_S8( const __m128i &src );
	//! ABS of 16 bit values
	CBL_INLINE static __m128i Abs_S16( const __m128i &src );
	//! ABS of 32 bit values
	CBL_INLINE static __m128i Abs_S32( const __m128i &src );
	//! ABS of 64 bit values
	CBL_INLINE static __m128i Abs_S64( const __m128i &src );
//@}


/** @} */ // end of Group2

    ////////////////////////////////////////////////////////////////////////////
    // LOGICAL ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group3
 *  @{
 */

/** @name Generic Shifts */
//@{
   //! Right Shift 16 unsigned 8-bit integer by value bits and return the result
    CBL_INLINE static __m128i RShift_8u ( const __m128i & src, const __m128i & value );
    //! Right Shift 16 signed 8-bit integer by value bits and return the result
    CBL_INLINE static __m128i RShift_8s ( const __m128i & src, const __m128i & value );
	//! Left Shift 16 unsigned 8-bit integer by value bits and return the result
    CBL_INLINE static __m128i LShift_8u ( const __m128i & src, const __m128i & value );
//@}

/** @name SSE2 Rotate right */
//@{
    template< int count >
    CBL_INLINE static __m128i RotateR( const __m128i & src );
//@}

/** @name SSE2 Rotate left */
//@{
    template< int count >
    CBL_INLINE static __m128i RotateL( const __m128i & src );
//@}

/** @name SSE2 Reverse */
//@{
    CBL_INLINE static void Reverse8( __m128i & values );
    CBL_INLINE static void Reverse16( __m128i & values );
    CBL_INLINE static void Reverse32( __m128i & values );
    CBL_INLINE static void Reverse64( __m128i & values );
//@}

/** @name SSE2 Shift and Carry */
//@{
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const S16 pValue );
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const S16 pValue );
    CBL_INLINE static void ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 pValue );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const S16 pValue );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const S16 pValue );
    CBL_INLINE static void ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 pValue );

    CBL_INLINE static void ShiftAndCarry_3R_16s( __m128i & a, __m128i & b, const U8 * pValues );
    CBL_INLINE static void ShiftAndCarry_4R_16s( __m128i & a, __m128i & b, const U8 * pValues );

    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const S32 value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const S32 value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const S32 value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const S32 value );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 value );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const S32 * pValue );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue );
    CBL_INLINE static void ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const S32 * pValue );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue );
    CBL_INLINE static void ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue );

    CBL_INLINE static void ShiftAndCarry_R_32f( __m128 & a, __m128 & b, F32 value );
    CBL_INLINE static void ShiftAndCarry_R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, F32 value );
    CBL_INLINE static void ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues );
    CBL_INLINE static void ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues );
    CBL_INLINE static void ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, const S16 * pvalues );
    CBL_INLINE static void ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, const S16 * pvalues );

    CBL_INLINE static void ShiftAndCarry_R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value );
    CBL_INLINE static void ShiftAndCarry_3R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3 );
    CBL_INLINE static void ShiftAndCarry_4R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3, S32 value4 );
//@}
/** @} */ // end of Group3

    ////////////////////////////////////////////////////////////////////////////
    // TRANSPOSE ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group4
 *  @{
 */

/** @name SSE2 Convert 3-channel to 3-planar Inplace*/
//@{
    // convert 3-channel RGB to 3-planar format
    CBL_INLINE static void Convert_3C_to_3P_8bit(  __m128i & rgb1, __m128i & rgb2, __m128i & rgb3);
    CBL_INLINE static void Convert_3C_to_3P_16bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3);
    CBL_INLINE static void Convert_3C_to_3P_32bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3);
//@}

/** @name SSE2 Convert 3-channel to 3-planar */
//@{
    // convert 3-channel RGB to 3-planar format
    CBL_INLINE static void Convert_3C_to_3P_8bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void Convert_3C_to_3P_16bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void Convert_3C_to_3P_32bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadAlignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadAlignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadAlignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadUnalignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadUnalignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadUnalignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//@}

/** @name SSE2 Convert 3-planar to 3-channel Inplace*/
//@{
    // convert 3-planar RGB to 3-channel format
    CBL_INLINE static void Convert_3P_to_3C_8bit( __m128i & r, __m128i & g, __m128i & b);
    CBL_INLINE static void Convert_3P_to_3C_16bit( __m128i & r, __m128i & g, __m128i & b);
    CBL_INLINE static void Convert_3P_to_3C_32bit( __m128i & r, __m128i & g, __m128i & b);
//@}

/** @name SSE2 Convert 3-planar to 3-channel */
//@{
    // convert 3-planar RGB to 3-channel format
    CBL_INLINE static void Convert_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 );
    CBL_INLINE static void Convert_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 );
    CBL_INLINE static void Convert_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 );
    CBL_INLINE static void ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
	CBL_INLINE static void ConvertAndStream_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
	CBL_INLINE static void ConvertAndStream_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
	CBL_INLINE static void ConvertAndStream_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
//@}

/** @name SSE2 Convert 4-channel to 4-planar Inplace*/
//@{
    // convert 4-channel RGBA to 4-planar format
    CBL_INLINE static void Convert_4C_to_4P_8bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4);
    CBL_INLINE static void Convert_4C_to_4P_16bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4);
    CBL_INLINE static void Convert_4C_to_4P_32bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4);
//@}

/** @name SSE2 Convert 4-channel to 4-planar */
//@{
    // convert 4-channel RGBA to 4-planar format
    CBL_INLINE static void Convert_4C_to_4P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void Convert_4C_to_4P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void Convert_4C_to_4P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadAlignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadAlignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadAlignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a );
//@}

/** @name SSE2 Convert 4-planar to 4-channel Inplace*/
//@{
    // convert 4-planar RGBA to 4-channel format
    CBL_INLINE static void Convert_4P_to_4C_8bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a);
    CBL_INLINE static void Convert_4P_to_4C_16bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a);
    CBL_INLINE static void Convert_4P_to_4C_32bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a);
//@}

/** @name SSE2 Convert 4-planar to 4-channel */
//@{
    // convert 4-planar RGBA to 4-channel format
    CBL_INLINE static void Convert_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3, __m128i & rgb4 );
    CBL_INLINE static void Convert_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3, __m128i & rgb4 );
    CBL_INLINE static void Convert_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3, __m128i & rgb4 );
    CBL_INLINE static void ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStream_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStream_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStream_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
	CBL_INLINE static void ConvertAndStoreUnaligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreUnaligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
    CBL_INLINE static void ConvertAndStoreUnaligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData );
//@}

/** @name SSE2 Convert 4-channel to 3-planar */
//@{
    // convert 4-channel RGBA to 3-channel RGB planar format
//    CBL_INLINE static void Convert_4C_to_3P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void Convert_4C_to_3P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void Convert_4C_to_3P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadAlignedAndConvert_4C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void LoadAlignedAndConvert_4C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void LoadAlignedAndConvert_4C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//    CBL_INLINE static void LoadUnalignedAndConvert_4C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b );
//@}

/*-* -@-name SSE2 Convert 3-planar to 4-channel */
//-@-{

    // convert 3-planar RGB to 4-channel RGBA format
//    CBL_INLINE static void Convert_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 );
//    CBL_INLINE static void Convert_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 );
//    CBL_INLINE static void Convert_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 );
//    CBL_INLINE static void ConvertAndStoreAligned_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
//    CBL_INLINE static void ConvertAndStoreAligned_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
//    CBL_INLINE static void ConvertAndStoreAligned_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData );
//    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData );
//    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData );
//    CBL_INLINE static void ConvertAndStoreUnaligned_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData );

//-@-}

/** @} */ // end of Group4

	 ////////////////////////////////////////////////////////////////////////////
    // MATH ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group5
 *  @{
 */

/** @name Floating point operations  */
//@{

    typedef struct {
        XMM128 half         ;
        XMM128 three_by_2	;		
    }sqrt_A11_Data;

    typedef struct {
        XMM128 half         ;
        XMM128 three_by_2	;		
    }invSqrt_A11_Data;

    typedef struct {
        XMM128 init_const;
        XMM128 one_by_3	;		
    }cbrt_A11_Data;

    typedef struct {
        XMM128 init_const;
        XMM128 four_by_3;
        XMM128 one_by_3;
        XMM128 four_by_3D;
    }invCbrt_A11_Data;
    
    typedef struct {
        XMM128 thirtytwo_by_ln2 ;
        XMM128 ln2_by_32		;
        XMM128 A1_A2			;       				
        XMM128 max_min_arg      ;
    }exp_A11_Data;

    typedef struct {
        XMM128 half_sqhalf;
        XMM128 p5;
        XMM128 p6_p7;
        XMM128 p8_p9;
        XMM128 const1_const2;
    }ln_A11_Data;

    typedef struct {
        XMM128 half_sqhalf;
        XMM128 p5;
        XMM128 p6_p7;
        XMM128 p8_p9;
        XMM128 const1_const2;
    }log_A11_Data;

    typedef struct {
        XMM128 ln2_lead; 
        XMM128 ln2_trail; 
        XMM128 A1_A2 ; 
    }pow_A11_Data;

     //! Preparation function 
    CBL_INLINE static void Prep_sqrt_A11( sqrt_A11_Data &e);
    CBL_INLINE static void Prep_invSqrt_A11( invSqrt_A11_Data &e);
    CBL_INLINE static void Prep_cbrt_A11( cbrt_A11_Data &e);
    CBL_INLINE static void Prep_invCbrt_A11( invCbrt_A11_Data &e);
    CBL_INLINE static void Prep_exp_A11( exp_A11_Data &e);
    CBL_INLINE static void Prep_ln_A11( ln_A11_Data &e);
    CBL_INLINE static void Prep_log_A11( log_A11_Data &e);
    CBL_INLINE static void Prep_pow_A11( pow_A11_Data &e);

    //! Calculate square root with 11 bits of accuracy
    CBL_INLINE static __m128 sqrt_A11 (const __m128 &src, const sqrt_A11_Data &d);
    //! Calculate inverse square root with 11 bits of accuracy
    CBL_INLINE static __m128 invSqrt_A11(const __m128 &src, const invSqrt_A11_Data &d);
    //! Calculate cube root with 11 bits of accuracy
    CBL_INLINE static __m128 cbrt_A11 (const __m128 &src, const cbrt_A11_Data &d);
    //! Calculate inverse Cube root with 11 bits of accuracy
    CBL_INLINE static __m128 invCbrt_A11(const __m128 &src, const invCbrt_A11_Data &d);
    //! Calculate exponential  with 11 bits of accuracy
	CBL_INLINE static __m128 exp_A11  (const __m128 &src, const exp_A11_Data &d);
    //! Calculate natural logarithm ( base “e”) with 11 bits of accuracy
    CBL_INLINE static __m128 ln_A11  (const __m128 &src, const ln_A11_Data &d);
    //! Calculate common Logarithm (base 10) with 11 bits of accuracy
    CBL_INLINE static __m128 log_A11  (const __m128 &src, const log_A11_Data &d);
    //! Calculate src**y with 11 bits of accuracy
    CBL_INLINE static __m128 pow_A11  (const __m128 &src1, const __m128 &src2, const pow_A11_Data &d);

    //! Equivalent of frexp in math library. Separates mantissa and exponent and returns mantissa
    CBL_INLINE static __m128d Frexp(const __m128d &val, __m128i &exp);
    //! Equivalent of frexpf in math library. Separates mantissa and exponent and returns mantissa
    CBL_INLINE static __m128 Frexpf(const __m128 &val, __m128i &exp);
	
//@}
/** @} */ // end of Group5

	 ////////////////////////////////////////////////////////////////////////////
    // SHUFFLE ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group6
 *  @{
 */

/** @name Single precision floating point shuffle operations  */
//@{
    //! 32-bit shuffle: b3a1b2a0, single precision floating point
	CBL_INLINE static __m128 Shuffle_b3a1b2a0_32f(const __m128 & a, const __m128 & b);
    //! 32-bit shuffle: b3b1a2a0, single precision floating point
	CBL_INLINE static __m128 Shuffle_b3b1a2a0_32f(const __m128 & a, const __m128 & b);
    //! 32-bit shuffle: b1b0a1a0, single precision floating point
	CBL_INLINE static __m128 Shuffle_b1b0a1a0_32f(const __m128 & a, const __m128 & b);
    //! 32-bit shuffle: a3a2b3b2, single precision floating point
	CBL_INLINE static __m128 Shuffle_a3a2b3b2_32f(const __m128 & a, const __m128 & b);
//@}

/** @name Double precision floating point shuffle operations  */
//@{
    //! 64-bit shuffle: b0a0, double precision floating point
	CBL_INLINE static __m128d Shuffle_b0a0_64f(const __m128d & a, const __m128d & b);
    //! 64-bit shuffle: b1a1, double precision floating point
	CBL_INLINE static __m128d Shuffle_b1a1_64f(const __m128d & a, const __m128d & b);
    //! 64-bit shuffle: b1a0, double precision floating point
	CBL_INLINE static __m128d Shuffle_b1a0_64f(const __m128d & a, const __m128d & b);
    //! 64-bit shuffle: a1b0, double precision floating point
	CBL_INLINE static __m128d Shuffle_a1b0_64f(const __m128d & a, const __m128d & b);
    //! 64-bit shuffle: a1a1, double precision floating point
	CBL_INLINE static __m128d Shuffle_a1a1_64f(const __m128d & a);
    //! 64-bit shuffle: b0a1, double precision floating point
	CBL_INLINE static __m128d Shuffle_b0a1_64f(const __m128d & a, const __m128d & b);
//@}

/** @name Integer shuffle operations  */
//@{
    //! 64-bit shuffle: b0a0, integer
	CBL_INLINE static __m128i Shuffle_b0a0_64s(const __m128i & a, const __m128i & b);
    //! 64-bit shuffle: b1a1, integer
	CBL_INLINE static __m128i Shuffle_b1a1_64s(const __m128i & a, const __m128i & b);
    //! 64-bit shuffle: b1a0, integer
	CBL_INLINE static __m128i Shuffle_b1a0_64s(const __m128i & a, const __m128i & b);
    //! 64-bit shuffle: b0a1, integer
	CBL_INLINE static __m128i Shuffle_b0a1_64s(const __m128i & a, const __m128i & b);

    //! 32-bit shuffle: 3201, integer
	CBL_INLINE static __m128i Shuffle_3201_32s(const __m128i & a);
    //! 32-bit shuffle: 2310, integer
	CBL_INLINE static __m128i Shuffle_2310_32s(const __m128i & a);
    //! 32-bit shuffle: 2301, integer
	CBL_INLINE static __m128i Shuffle_2301_32s(const __m128i & a);
    //! 32-bit shuffle: 3120, integer
	CBL_INLINE static __m128i Shuffle_3120_32s(const __m128i & a);
//@}

/** @} */ // end of Group6

	 ////////////////////////////////////////////////////////////////////////////
    // PACK & UNPACK ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group7
 *  @{
 */

/** @name pack operations  */
//@{
	//! Packs and saturates two 2x64 bit double precision floating point values into 4x32 bit signed integers
	CBL_INLINE static __m128i Pack64FTo32S( const __m128d & srcLo, const __m128d & srcHi );
	//! Packs and saturates 2x64 bit double precision floating point values into 2x16 bit signed integers
	CBL_INLINE static __m128i Pack64FTo16S( const __m128d & src );
	//! Packs and saturates 2x64 bit non-negative double precision floating point values into 2x16 bit signed integers
	CBL_INLINE static __m128i Pack64FTo16S_nonegative( const __m128d & src );
	//! Packs and saturates 2x64 bit non-negative double precision floating point values into 2x32 bit signed integers
	CBL_INLINE static __m128i Pack64FTo32SLo( const __m128d &srcLo, const __m128i &src );
	//! Packs and saturates 2x32 bit non-negative float precision floating point values into 2x16 bit signed integers
	CBL_INLINE static __m128i Pack32FTo16S(const __m128 &src1,const __m128 &src2);
	//! Packs and saturates 2x64 bit integer values into 2x32 bit signed integers
	CBL_INLINE static __m128i Pack64STo32S( const __m128i &srcLo, const __m128i &srcHi );
	//! Packs and saturates 2x64 bit integer values into 2x32 non-negative float precision floating point values
	CBL_INLINE static __m128 Pack64STo32F( const __m128i &srcLo, const __m128i &srcHi );

//@}

/** @name unpack operations  */
//@{
	//!	Convert 4 x 32 bit signed integers into 2 x 64 bit wide integers in 2 registers
	CBL_INLINE static void Unpack32STo64S( __m128i & srcDstLo, __m128i & dstHi );
	//!	Convert 4 x 32 bit unsigned integers into 2 x 64 bit wide integers in 2 registers
	CBL_INLINE static void Unpack32UTo64U( __m128i & srcDstLo, __m128i & dstHi );
	//!	Convert 4 x 32 bit signed integers into 2 x 64 bit doubles in 2 registers
	CBL_INLINE static void Unpack32STo64F( const __m128i & src, __m128d & dstLo, __m128d & dstHi );
	//!	Convert 8 x 16 bit signed integers into 4 x 32 bit wide integers in 2 registers
	CBL_INLINE static void Unpack16STo32S( __m128i & srcDstLo, __m128i & dstHi );
	//!	Convert 8 x 16 bit unsigned integers into 4 x 32 bit wide integers in 2 registers
	CBL_INLINE static void Unpack16UTo32U( __m128i &srcDstLo, __m128i & dstHi );
	//! Convert 16 x 8 bit unsigned integers into 8 x 16 bit signed integers in 2 registers
	CBL_INLINE static void Unpack8UTo16U( __m128i & srcDstLo, __m128i & dstHi );
	//! Convert 16 x 8 bit signed integers into 8 x 16 bit signed integers in 2 registers
	CBL_INLINE static void Unpack8STo16S( __m128i & srcDstLo, __m128i & dstHi );
	//! Convert 16 x 8 bit unsigned integers into 8 x 16 bit signed integers in 2 registers
	CBL_INLINE static void Unpack8UTo16S( __m128i &srcDstLo, __m128i &dstHi );

//@}


/** @name Bitfield extract and insert  */
//@{

	CBL_INLINE static void ExtractField( __m128i & srcdst, const __m128i & param );

	template < U8 index, U8 length >
	CBL_INLINE static void ExtractField( __m128i & srcdst );


	CBL_INLINE static void InsertField( __m128i & dst, const __m128i & src );

	template < U8 index, U8 length >	
	CBL_INLINE static void InsertField( __m128i & dst, const __m128i & src );
	


//@}

/** @} */ // end of Group7

}; // struct CBL_SSE2

/// @endcond

