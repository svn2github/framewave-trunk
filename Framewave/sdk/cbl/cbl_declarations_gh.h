/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


// cbl_declarations_gh.h

/// @cond OPT_DETAILS
struct CBL_GH : public CBL_SSE3
{
    ////////////////////////////////////////////////////////////////////////////
    // MEMORY ROUTINES
    ////////////////////////////////////////////////////////////////////////////

    /** @addtogroup Group1
     *  @{
     */

    /** @name Fractional Load operations */
    //@{
        CBL_INLINE static void Load_192( __m128i &rg_lo, __m128i &rg_hi, const __m128i* pMemory );
        CBL_INLINE static void Load_96( __m128i &rg, const __m128i* pMemory );
        CBL_INLINE static void Load_64( __m128i &rg, const __m128i* pMemory );
    //@}

    /** @name Fractional Store operations */
    //@{
        CBL_INLINE static void Store_192( __m128i* pMemory, __m128i &rg_lo, __m128i &rg_hi );
        CBL_INLINE static void Store_96( __m128i* pMemory, __m128i &rg );
        CBL_INLINE static void Store_64( __m128i* pMemory, __m128i &rg );
    //@}

    /** @} */ // end of Group1

    ////////////////////////////////////////////////////////////////////////////
    // SHUFFLE ROUTINES
    ////////////////////////////////////////////////////////////////////////////

/** @defgroup Group6 Shuffle routines
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

};

/// @endcond

