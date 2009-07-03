/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_LOGICAL_PRIMITIVES_H__
#define __CBL_LOGICAL_PRIMITIVES_H__

#pragma warning( push )
#pragma warning( disable: 4127 4701 )
/*! 
    \param[in] src - 128 bit value containing 16 unsigned 8-bit values to be shifted
    \param[in] value - indicates the number of bits the src values to be shifted 
    \return the right shifted src value
	\par Notes:
    The function operates on 8-bit unsigned data values.
    The src is right shifted by value bits and the result is returned.
*/
CBL_INLINE __m128i CBL_REF::RShift_8u ( const __m128i & src, const __m128i & value )
{
	const XMM128 & srcT = reinterpret_cast<const XMM128&>(src);
    const XMM128 & valueT = reinterpret_cast<const XMM128&>(value);
    XMM128 result;
    int shift = valueT.s32[0];
    for( int n = 0; n < 16; n++ )
    {
        result.u8[n] = (U8)(srcT.u8[n] >> shift);
    }
	return result.i;
}

/*! 
    \param[in] src - 128 bit value containing 16 signed 8-bit values to be shifted
    \param[in] value - indicates the number of bits the src values to be shifted 
    \return the right shifted src value
	\par Notes:
    The function operates on 8-bit unsigned data values.
    The src is right shifted by value bits and the result is returned.
*/
CBL_INLINE __m128i CBL_REF::RShift_8s ( const __m128i & src, const __m128i & value )
{
	const XMM128 & srcT = reinterpret_cast<const XMM128&>(src);
    const XMM128 & valueT = reinterpret_cast<const XMM128&>(value);
    XMM128 result;
    int shift = valueT.s32[0];
    for( int n = 0; n < 16; n++ )
    {
        result.s8[n] = (S8)(srcT.s8[n] >> shift);
    }
	return result.i;
}

/*! 
    \param[in] src - 128 bit value containing 16 unsigned 8-bit values to be shifted
    \param[in] value - indicates the number of bits the src values to be shifted 
    \return the left shifted src value
	\par Notes:
    The function operates on 8-bit unsigned data values.
    The src is left shifted by value bits and the result is returned.
*/
CBL_INLINE __m128i CBL_REF::LShift_8u ( const __m128i & src, const __m128i & value )
{
	const XMM128 & srcT = reinterpret_cast<const XMM128&>(src);
    const XMM128 & valueT = reinterpret_cast<const XMM128&>(value);
    XMM128 result;
    int shift = valueT.s32[0];
    for( int n = 0; n < 16; n++ )
    {
        result.u8[n] = (U8)(srcT.u8[n] << shift);
    }
	return result.i;
}

/*! Rotates the 128 bit value by specified count to the left.
    \par Template parameters:
    \param shift - specifies the count of positions to shift the values to the left
    \par Parameters:
    \param[in] src - a 128 bit variable holding the value to be shifted
    \return - the result of the rotate operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100 and the shift value was 3,
    then the result (output) value will be 0xccbbaa99887766554433221100ffeedd
    \par See Also:
    RotateR()
 */
template< int shift >
CBL_INLINE __m128i CBL_REF::RotateL( const __m128i & src )
{
    int count = shift % 16;
    XMM128 result;
    const XMM128& source = reinterpret_cast<const XMM128 &>(src);
    if( shift != 0 )
    {
        int anchor = 16 - count;
        for( int n = 0; n < count; n++ )
        {
            result.u8[n] = source.u8[anchor + n];
        }
        for( int n = count; n < 16; n++ )
        {
            result.u8[n] = source.u8[n - count];
        }
    }
    else
    {
        for( int n = 0; n < 16; n++ )
        {
            result.u8[n] = source.u8[n];
        }
    }

    return result.i;
}
#pragma warning( pop )

/*! Rotates the 128 bit value by specified count to the right.
    \par Template parameters:
    \param shift - specifies the count of positions to shift the values to the right
    \par Parameters:
    \param[in] src - a 128 bit variable holding the value to be shifted
    \return - the result of the rotate operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100 and the shift value was 3,
    then the result (output) value will be 0x221100ffeeddccbbaa99887766554433
    \par See Also:
    RotateL()
 */
template< int shift >
CBL_INLINE __m128i CBL_REF::RotateR( const __m128i & src )
{
    return CBL_REF::RotateL< 16 - shift >( src );
}

/*! Reverses the order of the 8 bit values in the 128 bit variable
    \par Parameters:
    \param[in] values - a 128 bit variable holding the values to be reversed
    \return - the result of the reverse operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100, 
    then the result (output) value will be 0x00112233445566778899aabbccddeeff
    \par See Also:
    Reverse16(), Reverse32(), Reverse64()
 */
CBL_INLINE void CBL_REF::Reverse8( __m128i & values )
{
    U8 * pvalues = (U8*)&values;
    for( S32 n = 0; n < 8; n++ )
    {
        U8 tmp = pvalues[n];
        pvalues[n] = pvalues[15-n];
        pvalues[15-n] = tmp;
    }
}

/*! Reverses the order of the 16 bit values in the 128 bit variable
    \par Parameters:
    \param[in] values - a 128 bit variable holding the values to be reversed
    \return - the result of the reverse operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100, 
    then the result (output) value will be 0x11003322554477669988bbaaddccffee
    \par See Also:
    Reverse8(), Reverse32(), Reverse64()
 */
CBL_INLINE void CBL_REF::Reverse16( __m128i & values )
{
    U16 * pvalues = (U16*)&values;
    for( S32 n = 0; n < 4; n++ )
    {
        U16 tmp = pvalues[n];
        pvalues[n] = pvalues[7-n];
        pvalues[7-n] = tmp;
    }
}

/*! Reverses the order of the 32 bit values in the 128 bit variable
    \par Parameters:
    \param[in] values - a 128 bit variable holding the values to be reversed
    \return - the result of the reverse operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100, 
    then the result (output) value will be 0x3322110077665544bbaa9988ffeeddcc
    \par See Also:
    Reverse8(), Reverse16(), Reverse64()
 */
CBL_INLINE void CBL_REF::Reverse32( __m128i & values )
{
    U32 * pvalues = (U32*)&values;
    for( S32 n = 0; n < 2; n++ )
    {
        U32 tmp = pvalues[n];
        pvalues[n] = pvalues[3-n];
        pvalues[3-n] = tmp;
    }
}

/*! Reverses the order of the 32 bit values in the 128 bit variable
    \par Parameters:
    \param[in] values - a 128 bit variable holding the values to be reversed
    \return - the result of the reverse operation
    \par Example:
    if the initial (input) value was 0xffeeddccbbaa99887766554433221100, 
    then the result (output) value will be 0x7766554433221100ffeeddccbbaa9988
    \par See Also:
    Reverse8(), Reverse16(), Reverse32()
 */
CBL_INLINE void CBL_REF::Reverse64( __m128i & values )
{
    U64 * pvalues = (U64*)&values;
    U64 tmp = pvalues[0];
    pvalues[0] = pvalues[1];
    pvalues[1] = tmp;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 tmp = pb[0];
    for( S32 n = 0; n < 7; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
    }
    pa[7] = tmp;
    pb[7] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = c.16s[0]
    \n c.16s[0] = c.16s[1]
    \n c.16s[1] = c.16s[2]
    \n ...
    \n c.16s[7] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 * pc = (U16*)&c;
    U16 tmp1 = pb[0];
    U16 tmp2 = pc[0];
    for( S32 n = 0; n < 7; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
        pc[n] = pc[n+1];
    }
    pa[7] = tmp1;
    pb[7] = tmp2;
    pc[7] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = c.16s[0]
    \n c.16s[0] = c.16s[1]
    \n c.16s[1] = c.16s[2]
    \n ...
    \n c.16s[7] = d.16s[0]
    \n d.16s[0] = d.16s[1]
    \n d.16s[1] = d.16s[2]
    \n ...
    \n d.16s[7] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 * pc = (U16*)&c;
    U16 * pd = (U16*)&d;
    U16 tmp1 = pb[0];
    U16 tmp2 = pc[0];
    U16 tmp3 = pd[0];
    for( S32 n = 0; n < 7; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
        pc[n] = pc[n+1];
        pd[n] = pd[n+1];
    }
    pa[7] = tmp1;
    pb[7] = tmp2;
    pc[7] = tmp3;
    pd[7] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 tmp = pa[7];
    for( S32 n = 7; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
    }
    pb[0] = tmp;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n c.16s[7] = c.16s[6]
    \n c.16s[6] = c.16s[5]
    \n ...
    \n c.16s[1] = c.16s[0]
    \n c.16s[0] = b.16s[7]
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 * pc = (U16*)&c;
    U16 tmp1 = pa[7];
    U16 tmp2 = pb[7];
    for( S32 n = 7; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
        pc[n] = pc[n-1];
    }
    pc[0] = tmp2;
    pb[0] = tmp1;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - a 16 bit value
    \return none
    \n d.16s[7] = d.16s[6]
    \n d.16s[6] = d.16s[5]
    \n ...
    \n d.16s[1] = d.16s[0]
    \n d.16s[0] = c.16s[7]
    \n c.16s[7] = c.16s[6]
    \n c.16s[6] = c.16s[5]
    \n ...
    \n c.16s[1] = c.16s[0]
    \n c.16s[0] = b.16s[7]
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S16 value )
{
    U16 * pa = (U16*)&a;
    U16 * pb = (U16*)&b;
    U16 * pc = (U16*)&c;
    U16 * pd = (U16*)&d;
    U16 tmp1 = pa[7];
    U16 tmp2 = pb[7];
    U16 tmp3 = pc[7];
    for( S32 n = 7; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
        pc[n] = pc[n-1];
        pd[n] = pd[n-1];
    }
    pd[0] = tmp3;
    pc[0] = tmp2;
    pb[0] = tmp1;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = value.16s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_R_16s( a, b, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = c.16s[0]
    \n c.16s[0] = c.16s[1]
    \n c.16s[1] = c.16s[2]
    \n ...
    \n c.16s[7] = value.16s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_R_16s( a, b, c, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.16s[0] = a.16s[1]
    \n a.16s[1] = a.16s[2]
    \n ...
    \n a.16s[7] = b.16s[0]
    \n b.16s[0] = b.16s[1]
    \n b.16s[1] = b.16s[2]
    \n ...
    \n b.16s[7] = c.16s[0]
    \n c.16s[0] = c.16s[1]
    \n c.16s[1] = c.16s[2]
    \n ...
    \n c.16s[7] = d.16s[0]
    \n d.16s[0] = d.16s[1]
    \n d.16s[1] = d.16s[2]
    \n ...
    \n d.16s[7] = value.s16s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_R_16s( a, b, c, d, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.16s[0] = a.16s[3]
    \n a.16s[1] = a.16s[4]
    \n ...
    \n a.16s[7] = b.16s[2]
    \n b.16s[0] = b.16s[3]
    \n b.16s[1] = b.16s[4]
    \n ...
    \n b.16s[5] = value.16s[0]
    \n b.16s[6] = value.16s[1]
    \n b.16s[7] = value.16s[2]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_3R_16s( __m128i & a, __m128i & b, const U8 * pvalues )
{
    S16 * pa = (S16 *)&a;
    S16 * pb = (S16 *)&b;
    pa[0] = pa[3];
    pa[1] = pa[4];
    pa[2] = pa[5];
    pa[3] = pa[6];
    pa[4] = pa[7];
    pa[5] = pb[0];
    pa[6] = pb[1];
    pa[7] = pb[2];
    pb[0] = pb[3];
    pb[1] = pb[4];
    pb[2] = pb[5];
    pb[3] = pb[6];
    pb[4] = pb[7];
    pb[5] = (S16)pvalues[0];
    pb[6] = (S16)pvalues[1];
    pb[7] = (S16)pvalues[2];
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.16s[0] = a.16s[4]
    \n a.16s[1] = a.16s[5]
    \n ...
    \n a.16s[7] = b.16s[3]
    \n b.16s[0] = b.16s[4]
    \n b.16s[1] = b.16s[5]
    \n ...
    \n b.16s[4] = value.16s[0]
    \n b.16s[5] = value.16s[1]
    \n b.16s[6] = value.16s[2]
    \n b.16s[7] = value.16s[3]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_4R_16s( __m128i & a, __m128i & b, const U8 * pvalues )
{
    S16 * pa = (S16 *)&a;
    S16 * pb = (S16 *)&b;
    for( int n = 0; n < 4; n++ )
    {
        pa[n] = pa[n+4];
        pa[n+4] = pb[n];
        pb[n] = pb[n+4];
        pb[n+4] = (S16)pvalues[n];
    }
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding the value to be shifted in
    \return none
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value.16s[7]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_L_16s( a, b, pv[7] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding the value to be shifted in
    \return none
    \n c.16s[7] = c.16s[6]
    \n c.16s[6] = c.16s[5]
    \n ...
    \n c.16s[1] = c.16s[0]
    \n c.16s[0] = b.16s[7]
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value.16s[7]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_L_16s( a, b, c, pv[7] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding the value to be shifted in
    \return none
    \n d.16s[7] = d.16s[6]
    \n d.16s[6] = d.16s[5]
    \n ...
    \n d.16s[1] = d.16s[0]
    \n d.16s[0] = c.16s[7]
    \n c.16s[7] = c.16s[6]
    \n c.16s[6] = c.16s[5]
    \n ...
    \n c.16s[1] = c.16s[0]
    \n c.16s[0] = b.16s[7]
    \n b.16s[7] = b.16s[6]
    \n b.16s[6] = b.16s[5]
    \n ...
    \n b.16s[1] = b.16s[0]
    \n b.16s[0] = a.16s[7]
    \n a.16s[7] = a.16s[6]
    \n a.16s[6] = a.16s[5]
    \n ...
    \n a.16s[1] = a.16s[0]
    \n a.16s[0] = value.16s[7]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_16s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U16 * pv = (U16*)&value;
    ShiftAndCarry_L_16s( a, b, c, d, pv[7] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.32s[0] = a.16s[1]
    \n a.32s[1] = a.32s[2]
    \n a.32s[2] = a.32s[3]
    \n a.32s[3] = b.32s[0]
    \n b.32s[0] = b.32s[1]
    \n b.32s[1] = b.32s[2]
    \n b.32s[2] = b.32s[3]
    \n b.16s[3] = value.32s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 tmp = pb[0];
    for( S32 n = 0; n < 3; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
    }
    pa[3] = tmp;
    pb[3] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.32s[0] = a.16s[1]
    \n a.32s[1] = a.32s[2]
    \n a.32s[2] = a.32s[3]
    \n a.32s[3] = b.32s[0]
    \n b.32s[0] = b.32s[1]
    \n b.32s[1] = b.32s[2]
    \n b.32s[2] = b.32s[3]
    \n b.16s[3] = c.32s[0]
    \n c.32s[0] = c.32s[1]
    \n c.32s[1] = c.32s[2]
    \n c.32s[2] = c.32s[3]
    \n c.16s[3] = value.32s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 * pc = (U32*)&c;
    U32 tmp1 = pb[0];
    U32 tmp2 = pc[0];
    for( S32 n = 0; n < 3; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
        pc[n] = pc[n+1];
    }
    pa[3] = tmp1;
    pb[3] = tmp2;
    pc[3] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n a.32s[0] = a.16s[1]
    \n a.32s[1] = a.32s[2]
    \n a.32s[2] = a.32s[3]
    \n a.32s[3] = b.32s[0]
    \n b.32s[0] = b.32s[1]
    \n b.32s[1] = b.32s[2]
    \n b.32s[2] = b.32s[3]
    \n b.16s[3] = c.32s[0]
    \n c.32s[0] = c.32s[1]
    \n c.32s[1] = c.32s[2]
    \n c.32s[2] = c.32s[3]
    \n c.16s[3] = d.32s[0]
    \n d.32s[0] = d.32s[1]
    \n d.32s[1] = d.32s[2]
    \n d.32s[2] = d.32s[3]
    \n d.16s[3] = value.32s[0]
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 * pc = (U32*)&c;
    U32 * pd = (U32*)&d;
    U32 tmp1 = pb[0];
    U32 tmp2 = pc[0];
    U32 tmp3 = pd[0];
    for( S32 n = 0; n < 3; n++ )
    {
        pa[n] = pa[n+1];
        pb[n] = pb[n+1];
        pc[n] = pc[n+1];
        pd[n] = pd[n+1];
    }
    pa[3] = tmp1;
    pb[3] = tmp2;
    pc[3] = tmp3;
    pd[3] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in,out] e - a 128 bit variable holding the values to be shifted
    \param[in,out] f - a 128 bit variable holding the values to be shifted
    \param[in,out] g - a 128 bit variable holding the values to be shifted
    \param[in,out] h - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n shifts all values by 64 bits to the right, carring the low order bits 
    from one register to the high order position in another register.
    \n Sets the high order bits of register h to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value )
{
    F64 * pa = (F64 *)&a;
    F64 * pb = (F64 *)&b;
    F64 * pc = (F64 *)&c;
    F64 * pd = (F64 *)&d;
    F64 * pe = (F64 *)&e;
    F64 * pf = (F64 *)&f;
    F64 * pg = (F64 *)&g;
    F64 * ph = (F64 *)&h;
    pa[0] = pa[1];
    pa[1] = pb[0];
    pb[0] = pb[1];
    pb[1] = pc[0];
    pc[0] = pc[1];
    pc[1] = pd[0];
    pd[0] = pd[1];
    pd[1] = pe[0];
    pe[0] = pe[1];
    pe[1] = pf[0];
    pf[0] = pf[1];
    pf[1] = pg[0];
    pg[0] = pg[1];
    pg[1] = ph[0];
    ph[0] = ph[1];
    ph[1] = (F64)value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in,out] e - a 128 bit variable holding the values to be shifted
    \param[in,out] f - a 128 bit variable holding the values to be shifted
    \param[in,out] g - a 128 bit variable holding the values to be shifted
    \param[in,out] h - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n shifts all values by 192 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register h to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_3R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3 )
{
    F64 * pa = (F64 *)&a;
    F64 * pb = (F64 *)&b;
    F64 * pc = (F64 *)&c;
    F64 * pd = (F64 *)&d;
    F64 * pe = (F64 *)&e;
    F64 * pf = (F64 *)&f;
    F64 * pg = (F64 *)&g;
    F64 * ph = (F64 *)&h;
    pa[0] = pb[1];
    pa[1] = pc[0];
    pb[0] = pc[1];
    pb[1] = pd[0];
    pc[0] = pd[1];
    pc[1] = pe[0];
    pd[0] = pe[1];
    pd[1] = pf[0];
    pe[0] = pf[1];
    pe[1] = pg[0];
    pf[0] = pg[1];
    pf[1] = ph[0];
    pg[0] = ph[1];
    pg[1] = (F64)value1;
    ph[0] = (F64)value2;
    ph[1] = (F64)value3;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in,out] e - a 128 bit variable holding the values to be shifted
    \param[in,out] f - a 128 bit variable holding the values to be shifted
    \param[in,out] g - a 128 bit variable holding the values to be shifted
    \param[in,out] h - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n shifts all values by 256 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register h to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_4R_64f( __m128d & a, __m128d & b, __m128d & c, __m128d & d, __m128d & e, __m128d & f, __m128d & g, __m128d & h, S32 value1, S32 value2, S32 value3, S32 value4 )
{
    F64 * pa = (F64 *)&a;
    F64 * pb = (F64 *)&b;
    F64 * pc = (F64 *)&c;
    F64 * pd = (F64 *)&d;
    F64 * pe = (F64 *)&e;
    F64 * pf = (F64 *)&f;
    F64 * pg = (F64 *)&g;
    F64 * ph = (F64 *)&h;
    pa[0] = pc[0];
    pa[1] = pc[1];
    pb[0] = pd[0];
    pb[1] = pd[1];
    pc[0] = pe[0];
    pc[1] = pe[1];
    pd[0] = pf[0];
    pd[1] = pf[1];
    pe[0] = pg[0];
    pe[1] = pg[1];
    pf[0] = ph[0];
    pf[1] = ph[1];
    pg[0] = (F64)value1;
    pg[1] = (F64)value2;
    ph[0] = (F64)value3;
    ph[1] = (F64)value4;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32f( __m128 & a, __m128 & b, F32 value )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    pa[0] = pa[1];
    pa[1] = pa[2];
    pa[2] = pa[3];
    pa[3] = pb[0];
    pb[0] = pb[1];
    pb[1] = pb[2];
    pb[2] = pb[3];
    pb[3] = (F32)value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - a 128 bit variable holding value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, F32 value )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    F32 * pc = (F32 *)&c;
    F32 * pd = (F32 *)&d;
    pa[0] = pa[1];
    pa[1] = pa[2];
    pa[2] = pa[3];
    pa[3] = pb[0];
    pb[0] = pb[1];
    pb[1] = pb[2];
    pb[2] = pb[3];
    pb[3] = pc[0];
    pc[0] = pc[1];
    pc[1] = pc[2];
    pc[2] = pc[3];
    pc[3] = pd[0];
    pd[0] = pd[1];
    pd[1] = pd[2];
    pd[2] = pd[3];
    pd[3] = (F32)value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] pvalues - pointer to 3 element array holding values to be shifted in
    \return none
    \n shifts all values by 96 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    F32 * pc = (F32 *)&c;
    F32 * pd = (F32 *)&d;
    pa[0] = pa[3];
    pa[1] = pb[0];
    pa[2] = pb[1];
    pa[3] = pb[2];
    pb[0] = pb[3];
    pb[1] = pc[0];
    pb[2] = pc[1];
    pb[3] = pc[2];
    pc[0] = pc[3];
    pc[1] = pd[0];
    pc[2] = pd[1];
    pc[3] = pd[2];
    pd[0] = pd[3];
    pd[1] = (F32)pvalues[0];
    pd[2] = (F32)pvalues[1];
    pd[3] = (F32)pvalues[2];
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] pvalues - pointer to 4 element array holding values to be shifted in
    \return none
    \n shifts all values by 128 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, __m128 & c, __m128 & d, const U8 * pvalues )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    F32 * pc = (F32 *)&c;
    F32 * pd = (F32 *)&d;
    for( int n = 0; n < 4; n++ )
    {
        pa[n] = pb[n];
        pb[n] = pc[n];
        pc[n] = pd[n];
        pd[n] = (F32)pvalues[n];
    }
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] pvalues - pointer to 3 element array holding values to be shifted in
    \return none
    \n shifts all values by 96 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_3R_32f( __m128 & a, __m128 & b, const S16 * pvalues )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    pa[0] = pa[3];
    pa[1] = pb[0];
    pa[2] = pb[1];
    pa[3] = pb[2];
    pb[0] = pb[3];
    pb[1] = (F32)pvalues[0];
    pb[2] = (F32)pvalues[1];
    pb[3] = (F32)pvalues[2];
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] pvalues - pointer to 4 element array holding values to be shifted in
    \return none
    \n shifts all values by 128 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_4R_32f( __m128 & a, __m128 & b, const S16 * pvalues )
{
    F32 * pa = (F32 *)&a;
    F32 * pb = (F32 *)&b;
    for( int n = 0; n < 4; n++ )
    {
        pa[n] = pb[n];
        pb[n] = (F32)pvalues[n];
    }
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 tmp = pa[3];
    for( S32 n = 3; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
    }
    pb[0] = tmp;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register c to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 * pc = (U32*)&c;
    U32 tmp1 = pa[3];
    U32 tmp2 = pb[3];
    for( S32 n = 3; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
        pc[n] = pc[n-1];
    }
    pc[0] = tmp2;
    pb[0] = tmp1;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 value )
{
    U32 * pa = (U32*)&a;
    U32 * pb = (U32*)&b;
    U32 * pc = (U32*)&c;
    U32 * pd = (U32*)&d;
    U32 tmp1 = pa[3];
    U32 tmp2 = pb[3];
    U32 tmp3 = pc[3];
    for( S32 n = 3; n > 0; n-- )
    {
        pa[n] = pa[n-1];
        pb[n] = pb[n-1];
        pc[n] = pc[n-1];
        pd[n] = pd[n-1];
    }
    pd[0] = tmp3;
    pc[0] = tmp2;
    pb[0] = tmp1;
    pa[0] = value;
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_L_32s( a, b, pv[3] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register c to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_L_32s( a, b, c, pv[3] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_L_32s( a, b, c, d, pv[3] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_R_32s( a, b, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register c to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_R_32s( a, b, c, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] value - value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const __m128i & value )
{
    U32 * pv = (U32*)&value;
    ShiftAndCarry_R_32s( a, b, c, d, pv[0] );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the low order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, const S32 * pValue )
{
    ShiftAndCarry_L_32s( a, b, *pValue );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the low order bits of register c to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue )
{
    ShiftAndCarry_L_32s( a, b, c, *pValue );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the left, carring the shifted bits 
    from one register to another.
    \n Sets the low order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_L_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue )
{
    ShiftAndCarry_L_32s( a, b, c, d, *pValue );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register b to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, const S32 * pValue )
{
    ShiftAndCarry_R_32s( a, b, *pValue );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register c to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, const S32 * pValue )
{
    ShiftAndCarry_R_32s( a, b, c, *pValue );
}

/*! 
    \par Parameters:
    \param[in,out] a - a 128 bit variable holding the values to be shifted
    \param[in,out] b - a 128 bit variable holding the values to be shifted
    \param[in,out] c - a 128 bit variable holding the values to be shifted
    \param[in,out] d - a 128 bit variable holding the values to be shifted
    \param[in] pvalue - pointer to value to be shifted in
    \return none
    \n shifts all values by 32 bits to the right, carring the shifted bits 
    from one register to another.
    \n Sets the high order bits of register d to specified value.
 */
CBL_INLINE void CBL_REF::ShiftAndCarry_R_32s( __m128i & a, __m128i & b, __m128i & c, __m128i & d, const S32 * pValue )
{
    ShiftAndCarry_R_32s( a, b, c, d, *pValue );
}



//! Extract bit field from register.  Source index and length are encoded in second parameter.

CBL_INLINE void  CBL_REF::ExtractField( __m128i & srcdst, const __m128i & param )
/*!
Extracts a bit field at specified index (0-63) of specified length (0-63)
from the srcdst register and stores it at bit offset 0.  All remaining bits will be set to zero.
A length value of 0 will be interpreted as bit filed of 64 bits.
The index is encoded in bits 13:8 of the param register and the length is encoded in bits 5:0 of
the param register.
*/
{
    const XMM128 & len_ind = reinterpret_cast<const XMM128&>( param );
    XMM128 & dst = reinterpret_cast< XMM128&>( srcdst );

	int length = len_ind.s8[0];
	int index = len_ind.s8[1];

	dst.u64[0] = dst.u64[0] >> (S32)index;
	//U64 upper = dst.u64[1] >> index;
	int r_index = 64 - (S32)index;	
	dst.u64[1] = dst.u64[1] << r_index;
	dst.u64[0] = dst.u64[0] | dst.u64[1];
	dst.u64[1] = 0;
	int r_length = 64 - length;

	dst.u64[0] = dst.u64[0] << r_length;
	dst.u64[0] = dst.u64[0] >> r_length;	
}

//! Extract bit field from register.  Source index and length are immediate cosntants.
template < U8 index, U8 length >
CBL_INLINE void  CBL_REF::ExtractField( __m128i & srcdst )
/*!
Extracts a bit field at specified index (0-63) of specified length (0-63)
from the srcdst register and stores it at bit offset 0.  All remaining bits will be set to zero.
A length value of 0 will be interpreted as bit filed of 64 bits.
The index and length parameters are passed as template arguments because in the future versions
of the library they might be required to be compile time constants.
*/
{
    XMM128 & dst = reinterpret_cast< XMM128&>( srcdst );


	dst.u64[0] = dst.u64[0] >> (S32)index;
	//U64 upper = dst.u64[1] >> index;
	int r_index = 64 - (S32)index;	
	dst.u64[1] = dst.u64[1] << r_index;
	dst.u64[0] = dst.u64[0] | dst.u64[1];
	dst.u64[1] = 0;

	int r_length = 64 - length;

	dst.u64[0] = dst.u64[0] << r_length;
	dst.u64[0] = dst.u64[0] >> r_length;		
}


//! Insert a bit field into register.  Destination index and length are encoded in second register.

CBL_INLINE void  CBL_REF::InsertField( __m128i & dst, const __m128i & src )
/*!
Inserts bit field of specified length from src register at bit index 0 into the dst register at 
the specified index.
The index must be a value in the 0-63 range and is encoded in the bits 77:72 of the src register.
The length must be a value in the 0-63 range and is encoded in the bits 64:69 of the src register.  
    Length value of 0 will be interpreted as bit filed of 64 bits.
The remaining bits in dst register are not modified.
*/
{

    const XMM128 & len_ind = reinterpret_cast<const XMM128&>( src );

	int length = len_ind.s8[8];
	int index = len_ind.s8[9];

    XMM128 dst1;
    
	XMM128 & dst2 = reinterpret_cast< XMM128&>( dst );
	
	dst1.i = src;


	dst1.u64[0] = dst1.u64[0] >> (S32)index;
	//U64 upper = dst.u64[1] >> index;
	index = 64 - index;	
	dst1.u64[1] = dst1.u64[1] << (S32)index;
	dst1.u64[0] = dst1.u64[0] | dst1.u64[1];
	dst1.u64[1] = 0;
	int r_length = 64 - length;

	dst1.u64[0] = dst1.u64[0] << r_length;
	dst1.u64[0] = dst1.u64[0] >> r_length;	

	
	dst2.u64[0] = dst2.u64[0] >> length;
	dst2.u64[0] = dst2.u64[0] << length;

	dst2.u64[0] = dst2.u64[0] | dst1.u64[0];

}
//! Insert a bit field into register.  Destination index and length are immediate constants.
template< U8 index, U8 length >
CBL_INLINE void  CBL_REF::InsertField( __m128i & dst, const __m128i & src )
/*!
Inserts bit field of specified length from src register at bit index 0 into the dst register at 
the specified index.
The index must be a value in the 0-63 range.
The length must be a value in the 0-63 range.  Length of 0 will be interpreted as bit filed of 64 bits.
The remaining bits in dst register are not modified.
*/
{
    XMM128 dst1;
    
	XMM128 & dst2 = reinterpret_cast< XMM128&>( dst );
	
	dst1.i = src;


	dst1.u64[0] = dst1.u64[0] >> (S32)index;
	//U64 upper = dst.u64[1] >> index;
	int r_index = 64 - index;	
	dst1.u64[1] = dst1.u64[1] << r_index;
	dst1.u64[0] = dst1.u64[0] | dst1.u64[1];
	dst1.u64[1] = 0;
	int r_length = 64 - length;

	dst1.u64[0] = dst1.u64[0] << r_length;
	dst1.u64[0] = dst1.u64[0] >> r_length;	

	
	dst2.u64[0] = dst2.u64[0] >> length;
	dst2.u64[0] = dst2.u64[0] << length;

	dst2.u64[0] = dst2.u64[0] | dst1.u64[0];
}


#endif // __CBL_LOGICAL_PRIMITIVES_H__
