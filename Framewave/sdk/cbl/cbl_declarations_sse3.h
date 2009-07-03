/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


// cbl_declarations_sse3.h

/// @cond OPT_DETAILS
struct CBL_SSE3 : public CBL_SSE2
{
    ////////////////////////////////////////////////////////////////////////////
    // ARITHMETIC ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @addtogroup Group2
 *  @{
 */

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

//@}
/** @} */ // end of Group2
};

/// @endcond

