/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_MEMORY_PRIMITIVES_H__
#define __CBL_MEMORY_PRIMITIVES_H__

/*! Sets the 16 unsigned, 8-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 8 bit unsigned integer value
    \return none
    \par
      \n variable.u8[0] = value
      \n variable.u8[1] = value
      - ...
      \n variable.u8[15] = value
 */
CBL_INLINE void CBL_REF::Load_1x16_8u( __m128i & variable, U8 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
    for( int n = 0; n < 16; n++ )
    {
	    v.u8[n] = value;
    }
}

/*! Sets the 16 signed, 8-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 8 bit signed integer value
    \return none
    \par
      \n variable.s8[0] = value
      \n variable.s8[1] = value
      - ...
      \n variable.s8[15] = value
 */
CBL_INLINE void CBL_REF::Load_1x16_8s( __m128i & variable, S8 value )
{
    Load_1x16_8u( variable, (U8)( value ) );
}

/*! Sets the 8 unsigned, 16-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 16 bit unsigned integer value
    \return none
    \par
      \n variable.u16[0] = value
      \n variable.u16[1] = value
      - ...
      \n variable.u16[7] = value
 */
CBL_INLINE void CBL_REF::Load_1x8_16u( __m128i & variable, U16 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
    for( int n = 0; n < 8; n++ )
    {
	    v.u16[n] = value;
    }
}

/*! Sets the 8 signed, 16-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 16 bit signed integer value
    \return none
    \par
      \n variable.s16[0] = value
      \n variable.s16[1] = value
      - ...
      \n variable.s16[7] = value
 */
CBL_INLINE void CBL_REF::Load_1x8_16s( __m128i & variable, S16 value )
{
    Load_1x8_16u( variable, (U16)value );
}

/*! Sets the 4 pairs of unsigned, 16-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 16 bit unsigned integer value 1
    \param[in]  value2 - a 16 bit unsigned integer value 2
    \return none
    \par
      \n variable.u16[0] = value1
      \n variable.u16[1] = value2
      \n variable.u16[3] = value1
      \n variable.u16[4] = value2
      - ...
      \n variable.u16[6] = value1
      \n variable.u16[7] = value2
 */
CBL_INLINE void CBL_REF::Load_2x4_16u( __m128i & variable, U16 value1, U16 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
    for( int n = 0; n < 8; n+=2 )
    {
	    v.u16[n] = value1;
	    v.u16[n+1] = value2;
    }
}

/*! Sets the 4 pairs of signed, 16-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 16 bit signed integer value 1
    \param[in]  value2 - a 16 bit signed integer value 2
    \return none
    \par
      \n variable.s16[0] = value1
      \n variable.s16[1] = value2
      \n variable.s16[3] = value1
      \n variable.s16[4] = value2
      - ...
      \n variable.s16[6] = value1
      \n variable.s16[7] = value2
 */
CBL_INLINE void CBL_REF::Load_2x4_16s( __m128i & variable, S16 value1, S16 value2 )
{
    Load_2x4_16u( variable, (U16)value1, (U16)value2 );
}

/*! Sets the 4 unsigned, 32-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 32 bit unsigned integer value
    \return none
    \par
      \n variable.u32[0] = value
      \n variable.u32[1] = value
      \n variable.u32[2] = value
      \n variable.u32[3] = value
 */
CBL_INLINE void CBL_REF::Load_1x4_32u( __m128i & variable, U32 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u32[0] = value;
	v.u32[1] = value;
	v.u32[2] = value;
	v.u32[3] = value;
}

/*! Sets the 4 signed, 32-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 32 bit signed integer value
    \return none
    \par
      \n variable.s32[0] = value
      \n variable.s32[1] = value
      \n variable.s32[2] = value
      \n variable.s32[3] = value
 */
CBL_INLINE void CBL_REF::Load_1x4_32s( __m128i & variable, S32 value )
{
    Load_1x4_32u( variable, (U32)value );
}

/*! Sets the 2 pairs of unsigned, 32-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 32 bit unsigned integer value 1
    \param[in]  value2 - a 32 bit unsigned integer value 2
    \return none
    \par
      \n variable.u32[0] = value1
      \n variable.u32[1] = value2
      \n variable.u32[3] = value1
      \n variable.u32[4] = value2
 */
CBL_INLINE void CBL_REF::Load_2x2_32u( __m128i & variable, U32 value1, U32 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u32[0] = value1;
	v.u32[1] = value2;
	v.u32[2] = value1;
	v.u32[3] = value2;
}

/*! Sets the 2 pairs of signed, 32-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 32 bit signed integer value 1
    \param[in]  value2 - a 32 bit signed integer value 2
    \return none
    \par
      \n variable.s32[0] = value1
      \n variable.s32[1] = value2
      \n variable.s32[3] = value1
      \n variable.s32[4] = value2
 */
CBL_INLINE void CBL_REF::Load_2x2_32s( __m128i & variable, S32 value1, S32 value2 )
{
    Load_2x2_32u( variable, (U32)value1, (U32)value2 );
}

/*! Sets the 2 unsigned, 64-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 64 bit unsigned integer value
    \return none
    \par
      \n variable.u64[0] = value
      \n variable.u64[1] = value
 */
CBL_INLINE void CBL_REF::Load_1x2_64u( __m128i & variable, U64 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u64[0] = value;
	v.u64[1] = value;
}

/*! Sets the 2 signed, 64-bit values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a 64 bit signed integer value
    \return none
    \par
      \n variable.s64[0] = value
      \n variable.s64[1] = value
 */
CBL_INLINE void CBL_REF::Load_1x2_64s( __m128i & variable, S64 value )
{
    Load_1x2_64u( variable, (U64)value );
}

/*! Sets the pair of unsigned, 64-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 64 bit unsigned integer value 1
    \param[in]  value2 - a 64 bit unsigned integer value 2
    \return none
    \par
      \n variable.u64[0] = value1
      \n variable.u64[1] = value2
 */
CBL_INLINE void CBL_REF::Load_2x1_64u( __m128i & variable, U64 value1, U64 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.u64[0] = value1;
	v.u64[1] = value2;
}

/*! Sets the pair of signed, 64-bit values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a 64 bit signed integer value 1
    \param[in]  value2 - a 64 bit signed integer value 2
    \return none
    \par
      \n variable.s64[0] = value1
      \n variable.s64[1] = value2
 */
CBL_INLINE void CBL_REF::Load_2x1_64s( __m128i & variable, S64 value1, S64 value2 )
{
    Load_2x1_64u( variable, (U64)value1, (U64)value2 );
}

/*! Sets the 4 single precision floating point values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a single precision floating point value
    \return none
    \par
      \n variable.f32[0] = value
      \n variable.f32[1] = value
      \n variable.f32[2] = value
      \n variable.f32[3] = value
 */
CBL_INLINE void CBL_REF::Load_1x4_32f( __m128 & variable, F32 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.f32[0] = value;
	v.f32[1] = value;
	v.f32[2] = value;
	v.f32[3] = value;
}

/*! Sets the 2 pairs of single precision floating point values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a single precision floating point value 1
    \param[in]  value2 - a single precision floating point value 2
    \return none
    \par
      \n variable.f32[0] = value1
      \n variable.f32[1] = value2
      \n variable.f32[3] = value1
      \n variable.f32[4] = value2
 */
CBL_INLINE void CBL_REF::Load_2x2_32f( __m128 & variable, F32 value1, F32 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.f32[0] = value1;
	v.f32[1] = value2;
	v.f32[2] = value1;
	v.f32[3] = value2;
}

/*! Sets the 2 double precision floating point values in variable to specified value: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value - a double precision floating point value
    \return none
    \par
      \n variable.f64[0] = value
      \n variable.f64[1] = value
 */
CBL_INLINE void CBL_REF::Load_1x2_64f( __m128d & variable, F64 value )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.f64[0] = value;
	v.f64[1] = value;
}

/*! Sets the pair of double precision floating point values in variable to specified values: 
    \par Parameters:
    \param[out] variable - a 128 bit variable to hold the result
    \param[in]  value1 - a double precision floating point value 1
    \param[in]  value2 - a double precision floating point value 2
    \return none
    \par
      \n variable.f64[0] = value1
      \n variable.f64[1] = value2
 */
CBL_INLINE void CBL_REF::Load_2x1_64f( __m128d & variable, F64 value1, F64 value2 )
{
    XMM128 & v = reinterpret_cast<XMM128&>(variable);
	v.f64[0] = value1;
	v.f64[1] = value2;
}

/*! Loads 192 bits of data at address pMemory into the two XMM registers: 
    \par Parameters:
    \param[out] rg_lo - a 128 bit variable to hold the low order bits of the result
    \param[out] rg_hi - a 128 bit variable to hold the high order bits of the result
    \param[in]  pMemory - pointer to memory location from which 192 bits of data will be read
    \return none
    \par
      \n rg_lo.s64[0] = bits 0..63
	  \n rg_lo.s64[1] = bits 64..127
	  \n rg_hi.s64[0] = bits 128..191
	  \n rg_hi.s64[1] = undefined
 */
CBL_INLINE void CBL_REF::Load_192( __m128i &rg_lo, __m128i &rg_hi, const __m128i* pMemory )
{
    const XMM128 & v1 = *reinterpret_cast<const XMM128*>(pMemory);
    const XMM128 & v2 = *reinterpret_cast<const XMM128*>(pMemory+1);
    XMM128 & r_lo = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r_hi = reinterpret_cast<XMM128&>(rg_hi);
	r_lo.u64[0] = v1.u64[0];
	r_lo.u64[1] = v1.u64[1];
	r_hi.u64[0] = v2.u64[0];
}

/*! Loads 96 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 96 bits of data will be read
    \return none
    \par
      \n rg.s32[0] = bits 0..31
	  \n rg.s32[1] = bits 32..63
	  \n rg.s32[2] = bits 64..95
	  \n rg.s32[3] = undefined
 */
CBL_INLINE void CBL_REF::Load_96( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u64[0] = v.u64[0];
	r.u32[2] = v.u32[2];
}

/*! Loads 64 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 64 bits of data will be read
    \return none
    \par
      \n rg.s64[0] = bits 0..63
	  \n rg.s32[1..3] = undefined
 */
CBL_INLINE void CBL_REF::Load_64( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u64[0] = v.u64[0];
}

/*! Loads 32 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 32 bits of data will be read
    \return none
    \par
      \n rg.s32[0] = bits 0..31
	  \n rg.s32[1..3] = undefined
 */
CBL_INLINE void CBL_REF::Load_32( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u32[0] = v.u32[0];
}

/*! Loads 24 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 24 bits of data will be read
    \return none
    \par
      \n rg.u8[0] = bits 0..7
      \n rg.u8[1] = bits 8..15
      \n rg.u8[2] = bits 16..23
	  \n rg.u8[3..15] = undefined
 */
CBL_INLINE void CBL_REF::Load_24( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u16[0] = v.u16[0];
	r.u8[2] = v.u8[2];
}

/*! Loads 16 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 16 bits of data will be read
    \return none
    \par
      \n rg.s16[0] = bits 0..15
	  \n rg.s16[1..7] = undefined
 */
CBL_INLINE void CBL_REF::Load_16( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u16[0] = v.u16[0];
}

/*! Loads 8 bits of data at address pMemory into the the XMM register: 
    \par Parameters:
    \param[out] rg - a 128 bit variable to hold the result
    \param[in]  pMemory - pointer to memory location from which 8 bits of data will be read
    \return none
    \par
      \n rg.u8[0] = bits 0..7
	  \n rg.u8[1..15] = undefined
 */
CBL_INLINE void CBL_REF::Load_8( __m128i &rg, const __m128i* pMemory )
{
    const XMM128 & v = *reinterpret_cast<const XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	r.u8[0] = v.u8[0];
}

/*! Stores 192 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 192 bits of data will be written
    \param[in] rg_lo - a 128 bit variable to holding the low order bits of data to be written
    \param[in] rg_hi - a 128 bit variable to holding the high order bits of data to be written
    \return none
    \par
	  \n memory bits 0..63 = rg_lo.s64[0]
	  \n memory bits 64..127 = rg_lo.s64[1]
	  \n memory bits 128..192 = rg_hi.s64[0]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_192( __m128i* pMemory, __m128i &rg_lo, __m128i &rg_hi )
{
    XMM128 & v1 = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & v2 = *reinterpret_cast<XMM128*>(pMemory+1);
    XMM128 & r_lo = reinterpret_cast<XMM128&>(rg_lo);
    XMM128 & r_hi = reinterpret_cast<XMM128&>(rg_hi);
	v1.u64[0] = r_lo.u64[0];
	v1.u64[1] = r_lo.u64[1];
	v2.u64[0] = r_hi.u64[0];
}

/*! Stores 96 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 96 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..31 = rg.s32[0]
	  \n memory bits 32..63 = rg.s32[1]
	  \n memory bits 64..95 = rg.s32[2]
	\par NOTE: This function might modify the input register!
 */
CBL_INLINE void CBL_REF::Store_96( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u64[0] = r.u64[0];
	v.u32[2] = r.u32[2];
}

/*! Stores 64 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 64 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..63 = rg.s64[0]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_64( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u64[0] = r.u64[0];
}

/*! Stores 32 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 32 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..32 = rg.s32[0]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_32( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u32[0] = r.u32[0];
}

/*! Stores 24 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 24 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..7 = rg.s8[0]
	  \n memory bits 8..15 = rg.s8[1]
	  \n memory bits 16..23 = rg.s8[2]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_24( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u16[0] = r.u16[0];
	v.u8[2] = r.u8[2];
}

/*! Stores 16 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 16 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..15 = rg.s16[0]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_16( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u16[0] = r.u16[0];
}

/*! Stores 8 bits of data in the two XMM registers to address specified in pMemory:
    \par Parameters:
    \param[out] pMemory - pointer to memory location where 8 bits of data will be written
    \param[in] rg - a 128 bit variable to holding the data to be written
    \return none
    \par
	  \n memory bits 0..7 = rg.u8[0]
	\par NOTE: This function might modify the input registers!
 */
CBL_INLINE void CBL_REF::Store_8( __m128i* pMemory, __m128i &rg )
{
    XMM128 & v = *reinterpret_cast<XMM128*>(pMemory);
    XMM128 & r = reinterpret_cast<XMM128&>(rg);
	v.u8[0] = r.u8[0];
}

/*! Sets each 8 bit value in the register to its corresponding table lookup value
    \par Parameters:
    \param[out] reg - a 128 bit variable to hold the result
    \param[in] table - a 16 element lookup table
    \return none
    \par
      \n reg.u8[0] = table[reg.u8[0]]
      \n reg.u8[1] = table[reg.u8[1]]
	  - ...
      \n reg.u8[15] = table[reg.u8[15]]

    \par NOTE: table needs to be a valid pointer to at least 256 bytes of memory, otherwise this function will crash
 */
CBL_INLINE void CBL_REF::LUT_8u( __m128i & reg, U8* table )
{
    XMM128 & r = reinterpret_cast<XMM128&>(reg);
	for (int i = 0; i < 16; i++)
		r.u8[i] = table[r.u8[i]];
}

#endif // __CBL_MEMORY_PRIMITIVES_H__
