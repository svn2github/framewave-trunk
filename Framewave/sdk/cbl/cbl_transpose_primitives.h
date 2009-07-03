/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_TRANSPOSE_PRIMITIVES_H__
#define __CBL_TRANSPOSE_PRIMITIVES_H__

// convert 3-channel RGB to 3-planar format

/*! 
    \param[in,out] rgb1 contains first 16 values of rgb data as input. After the function returns, it contains 16 values of r-channel
    \param[in,out] rgb2 contains second 16 values of rgb data as input. After the function returns, it contains 16 values of g-channel
    \param[in,out] rgb3 contains third 16 values of rgb data as input. After the function returns, it contains 16 values of b-channel
    \return none
	\par Notes:
    The function operates on 8-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    The output is contained in the same input registers.
    \par Input:
    \n  rgb1 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 ,g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g10,r10,b9 ,g9 ,r9 ,b8 ,g8 ,r8 ,b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \n  rgb3 = (b15,g15,r15,b14,g14,r14,b13,g13,r13,b12,g12,r12,b11,g11,r11,b10)
    \par Output:
    \n  rgb1 = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  rgb2 = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  rgb3 = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_8bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_8bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
{
	const __m128i temp_rgb1 = rgb1;
	const __m128i temp_rgb2 = rgb2;
	const __m128i temp_rgb3 = rgb3;

    const S8 * prgb1 = (const S8 *)&temp_rgb1;
    const S8 * prgb2 = (const S8 *)&temp_rgb2;
    const S8 * prgb3 = (const S8 *)&temp_rgb3;

    S8 * pr = (S8 *)&rgb1;
    S8 * pg = (S8 *)&rgb2;
    S8 * pb = (S8 *)&rgb3;

    pr[0] = prgb1[0];
    pr[1] = prgb1[3];
    pr[2] = prgb1[6];
    pr[3] = prgb1[9];
    pr[4] = prgb1[12];
    pr[5] = prgb1[15];
    pr[6] = prgb2[2];
    pr[7] = prgb2[5];
    pr[8] = prgb2[8];
    pr[9] = prgb2[11];
    pr[10] = prgb2[14];
    pr[11] = prgb3[1];
    pr[12] = prgb3[4];
    pr[13] = prgb3[7];
    pr[14] = prgb3[10];
    pr[15] = prgb3[13];

    pg[0] = prgb1[1];
    pg[1] = prgb1[4];
    pg[2] = prgb1[7];
    pg[3] = prgb1[10];
    pg[4] = prgb1[13];
    pg[5] = prgb2[0];
    pg[6] = prgb2[3];
    pg[7] = prgb2[6];
    pg[8] = prgb2[9];
    pg[9] = prgb2[12];
    pg[10] = prgb2[15];
    pg[11] = prgb3[2];
    pg[12] = prgb3[5];
    pg[13] = prgb3[8];
    pg[14] = prgb3[11];
    pg[15] = prgb3[14];

    pb[0] = prgb1[2];
    pb[1] = prgb1[5];
    pb[2] = prgb1[8];
    pb[3] = prgb1[11];
    pb[4] = prgb1[14];
    pb[5] = prgb2[1];
    pb[6] = prgb2[4];
    pb[7] = prgb2[7];
    pb[8] = prgb2[10];
    pb[9] = prgb2[13];
    pb[10] = prgb3[0];
    pb[11] = prgb3[3];
    pb[12] = prgb3[6];
    pb[13] = prgb3[9];
    pb[14] = prgb3[12];
    pb[15] = prgb3[15];
}

/*! 
    \param[in,out] rgb1 contains first 8 values of rgb data as input. After the function returns, it contains 8 values of r-channel
    \param[in,out] rgb2 contains second 8 values of rgb data as input. After the function returns, it contains 8 values of g-channel
    \param[in,out] rgb3 contains third 8 values of rgb data as input. After the function returns, it contains 8 values of b-channel
    \return none
	\par Notes:
    The function operates on 16-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    The output is contained in the same input registers.
    \par Input:
    \n  rgb1 = (g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 )
    \n  rgb3 = (b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \par Output:
    \n  rgb1 = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  rgb2 = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  rgb3 = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_16bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_16bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
{
	const __m128i temp_rgb1 = rgb1;
	const __m128i temp_rgb2 = rgb2;
	const __m128i temp_rgb3 = rgb3;

    const S16 * prgb1 = (const S16 *)&temp_rgb1;
    const S16 * prgb2 = (const S16 *)&temp_rgb2;
    const S16 * prgb3 = (const S16 *)&temp_rgb3;

    S16 * pr = (S16 *)&rgb1;
    S16 * pg = (S16 *)&rgb2;
    S16 * pb = (S16 *)&rgb3;

    pr[0] = prgb1[0];
    pr[1] = prgb1[3];
    pr[2] = prgb1[6];
    pr[3] = prgb2[1];
    pr[4] = prgb2[4];
    pr[5] = prgb2[7];
    pr[6] = prgb3[2];
    pr[7] = prgb3[5];

    pg[0] = prgb1[1];
    pg[1] = prgb1[4];
    pg[2] = prgb1[7];
    pg[3] = prgb2[2];
    pg[4] = prgb2[5];
    pg[5] = prgb3[0];
    pg[6] = prgb3[3];
    pg[7] = prgb3[6];

    pb[0] = prgb1[2];
    pb[1] = prgb1[5];
    pb[2] = prgb2[0];
    pb[3] = prgb2[3];
    pb[4] = prgb2[6];
    pb[5] = prgb3[1];
    pb[6] = prgb3[4];
    pb[7] = prgb3[7];
}

/*! 
    \param[in,out] rgb1 contains first 4 values of rgb data as input. After the function returns, it contains 4 values of r-channel
    \param[in,out] rgb2 contains second 4 values of rgb data as input. After the function returns, it contains 4 values of g-channel
    \param[in,out] rgb3 contains third 4 values of rgb data as input. After the function returns, it contains 4 values of b-channel
    \return none
	\par Notes:
    The function operates on 32-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    The output is contained in the same input registers.
    \par Input:
    \n  rgb1 = (r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g2 ,r2 ,b1 ,g1 )
    \n  rgb3 = (b3 ,g3 ,r3 ,b2 )
    \par Output:
    \n  rgb1 = (r3 ,r2 ,r1 ,r0 )
    \n  rgb2 = (g3 ,g2 ,g1 ,g0 )
    \n  rgb3 = (b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_32bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_32bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
{
	const __m128i temp_rgb1 = rgb1;
	const __m128i temp_rgb2 = rgb2;
	const __m128i temp_rgb3 = rgb3;

    const S32 * prgb1 = (const S32 *)&temp_rgb1;
    const S32 * prgb2 = (const S32 *)&temp_rgb2;
    const S32 * prgb3 = (const S32 *)&temp_rgb3;

    S32 * pr = (S32 *)&rgb1;
    S32 * pg = (S32 *)&rgb2;
    S32 * pb = (S32 *)&rgb3;

    pr[0] = prgb1[0];
    pr[1] = prgb1[3];
    pr[2] = prgb2[2];
    pr[3] = prgb3[1];

    pg[0] = prgb1[1];
    pg[1] = prgb2[0];
    pg[2] = prgb2[3];
    pg[3] = prgb3[2];

    pb[0] = prgb1[2];
    pb[1] = prgb2[1];
    pb[2] = prgb3[0];
    pb[3] = prgb3[3];
}

/*! 
    \param[in] rgb1 contains first 16 values of rgb data as input 
    \param[in] rgb2 contains second 16 values of rgb data as input 
    \param[in] rgb3 contains third 16 values of rgb data as input 
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \return none
	\par Notes:
    The function operates on 8-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  rgb1 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 ,g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g10,r10,b9 ,g9 ,r9 ,b8 ,g8 ,r8 ,b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \n  rgb3 = (b15,g15,r15,b14,g14,r14,b13,g13,r13,b12,g12,r12,b11,g11,r11,b10)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_8bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_8bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_8bit(r,g,b);
}

/*! 
    \param[in] rgb1 contains first 8 values of rgb data as input
    \param[in] rgb2 contains second 8 values of rgb data as input
    \param[in] rgb3 contains third 8 values of rgb data as input
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \return none
	\par Notes:
    The function operates on 16-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  rgb1 = (g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 )
    \n  rgb3 = (b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_16bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_16bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_16bit(r,g,b);
}

/*! 
    \param[in] rgb1 contains first 4 values of rgb data as input 
    \param[in] rgb2 contains second 4 values of rgb data as input
    \param[in] rgb3 contains third 4 values of rgb data as input 
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \return none
	\par Notes:
    The function operates on 32-bit rgb data.
    It separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  rgb1 = (r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g2 ,r2 ,b1 ,g1 )
    \n  rgb3 = (b3 ,g3 ,r3 ,b2 )
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
	\par See also:
    Convert_3C_to_3P_32bit( __m128i & rgb1, __m128i & rgb2, __m128i & rgb3)
*/
CBL_INLINE void CBL_REF::Convert_3C_to_3P_32bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_32bit(r,g,b);
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgb buffer
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \return none
	\par Notes:
    The function operates on aligned 8-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadUnalignedAndConvert_3C_to_3P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    S8 * pr = (S8 *)&r;
    S8 * pg = (S8 *)&g;
    S8 * pb = (S8 *)&b;
    const S8 * prgb = (const S8*)pAlignedData;

    for( int n = 0; n < 16; n++ )
    {
        pr[n] = prgb[3 * n + 0];
        pg[n] = prgb[3 * n + 1];
        pb[n] = prgb[3 * n + 2];
    }
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgb buffer
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \return none
	\par Notes:
    The function operates on aligned 16-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadUnalignedAndConvert_3C_to_3P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    const S16 * prgb = (const S16 *)pAlignedData;
    S16 * pr = (S16 *)&r;
    S16 * pg = (S16 *)&g;
    S16 * pb = (S16 *)&b;

    for( int n = 0; n < 8; n++ )
    {
        pr[n] = prgb[3 * n + 0];
        pg[n] = prgb[3 * n + 1];
        pb[n] = prgb[3 * n + 2];
    }
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgb buffer
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \return none
	\par Notes:
    The function operates on aligned 32-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadUnalignedAndConvert_3C_to_3P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    const S32 * prgb = (const S32 *)pAlignedData;
    S32 * pr = (S32 *)&r;
    S32 * pg = (S32 *)&g;
    S32 * pb = (S32 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        pr[n] = prgb[3 * n + 0];
        pg[n] = prgb[3 * n + 1];
        pb[n] = prgb[3 * n + 2];
    }
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgb buffer
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \return none
	\par Notes:
    The function operates on unaligned 8-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadAlignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_3C_to_3P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_3C_to_3P_8bit( pUnalignedData, r, g, b );
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgb buffer
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \return none
	\par Notes:
    The function operates on unaligned 16-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadAlignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_3C_to_3P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_3C_to_3P_16bit( pUnalignedData, r, g, b );
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgb buffer
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \return none
	\par Notes:
    The function operates on unaligned 32-bit rgb data. It takes care of loading the rgb data.
    And then separates the rgb data into three different planar data channels r, g and b.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
	\par See also:
    LoadAlignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_3C_to_3P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_3C_to_3P_32bit( pUnalignedData, r, g, b );
}

// convert 3-planar RGB to 3-channel format

/*! 
    \param[in,out] r contains 16 values of r-channel data as input. After the function returns, it contains first 16 values of rgb data
    \param[in,out] g contains 16 values of g-channel data as input. After the function returns, it contains second 16 values of rgb data
    \param[in,out] b contains 16 values of b-channel data as input. After the function returns, it contains third 16 values of rgb data
    \return none
	\par Notes:
    The function operates on 8-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  r = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 ,g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  g = (g10,r10,b9 ,g9 ,r9 ,b8 ,g8 ,r8 ,b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \n  b = (b15,g15,r15,b14,g14,r14,b13,g13,r13,b12,g12,r12,b11,g11,r11,b10)
	\par See also:
    Convert_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_8bit( __m128i & r, __m128i & g, __m128i & b)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;

    S8 * prgb1 = (S8 *)&r;
    S8 * prgb2 = (S8 *)&g;
    S8 * prgb3 = (S8 *)&b;

    const S8 * pr = (const S8 *)&temp_r;
    const S8 * pg = (const S8 *)&temp_g;
    const S8 * pb = (const S8 *)&temp_b;

    prgb1[0] = pr[0];
    prgb1[3] = pr[1];
    prgb1[6] = pr[2];
    prgb1[9] = pr[3];
    prgb1[12] = pr[4];
    prgb1[15] = pr[5];
    prgb2[2] = pr[6];
    prgb2[5] = pr[7];
    prgb2[8] = pr[8];
    prgb2[11] = pr[9];
    prgb2[14] = pr[10];
    prgb3[1] = pr[11];
    prgb3[4] = pr[12];
    prgb3[7] = pr[13];
    prgb3[10] = pr[14];
    prgb3[13] = pr[15];

    prgb1[1] = pg[0];
    prgb1[4] = pg[1];
    prgb1[7] = pg[2];
    prgb1[10] = pg[3];
    prgb1[13] = pg[4];
    prgb2[0] = pg[5];
    prgb2[3] = pg[6];
    prgb2[6] = pg[7];
    prgb2[9] = pg[8];
    prgb2[12] = pg[9];
    prgb2[15] = pg[10];
    prgb3[2] = pg[11];
    prgb3[5] = pg[12];
    prgb3[8] = pg[13];
    prgb3[11] = pg[14];
    prgb3[14] = pg[15];

    prgb1[2] = pb[0];
    prgb1[5] = pb[1];
    prgb1[8] = pb[2];
    prgb1[11] = pb[3];
    prgb1[14] = pb[4];
    prgb2[1] = pb[5];
    prgb2[4] = pb[6];
    prgb2[7] = pb[7];
    prgb2[10] = pb[8];
    prgb2[13] = pb[9];
    prgb3[0] = pb[10];
    prgb3[3] = pb[11];
    prgb3[6] = pb[12];
    prgb3[9] = pb[13];
    prgb3[12] = pb[14];
    prgb3[15] = pb[15];
}

/*! 
    \param[in,out] r contains 8 values of r-channel data as input. After the function returns, it contains first 8 values of rgb data
    \param[in,out] g contains 8 values of g-channel data as input. After the function returns, it contains second 8 values of rgb data
    \param[in,out] b contains 8 values of b-channel data as input. After the function returns, it contains third 8 values of rgb data
    \return none
	\par Notes:
    The function operates on 16-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  r = (g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  g = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 )
    \n  b = (b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
	\par See also:
    Convert_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_16bit( __m128i & r, __m128i & g, __m128i & b)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;

    S16 * prgb1 = (S16 *)&r;
    S16 * prgb2 = (S16 *)&g;
    S16 * prgb3 = (S16 *)&b;

    const S16 * pr = (const S16 *)&temp_r;
    const S16 * pg = (const S16 *)&temp_g;
    const S16 * pb = (const S16 *)&temp_b;

    prgb1[0] = pr[0];
    prgb1[3] = pr[1];
    prgb1[6] = pr[2];
    prgb2[1] = pr[3];
    prgb2[4] = pr[4];
    prgb2[7] = pr[5];
    prgb3[2] = pr[6];
    prgb3[5] = pr[7];

    prgb1[1] = pg[0];
    prgb1[4] = pg[1];
    prgb1[7] = pg[2];
    prgb2[2] = pg[3];
    prgb2[5] = pg[4];
    prgb3[0] = pg[5];
    prgb3[3] = pg[6];
    prgb3[6] = pg[7];

    prgb1[2] = pb[0];
    prgb1[5] = pb[1];
    prgb2[0] = pb[2];
    prgb2[3] = pb[3];
    prgb2[6] = pb[4];
    prgb3[1] = pb[5];
    prgb3[4] = pb[6];
    prgb3[7] = pb[7];
}

/*! 
    \param[in,out] r contains 4 values of r-channel data as input. After the function returns, it contains first 4 values of rgb data
    \param[in,out] g contains 4 values of g-channel data as input. After the function returns, it contains second 4 values of rgb data
    \param[in,out] b contains 4 values of b-channel data as input. After the function returns, it contains third 4 values of rgb data
    \return none
	\par Notes:
    The function operates on 32-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  r = (r1 ,b0 ,g0 ,r0 )
    \n  g = (g2 ,r2 ,b1 ,g1 )
    \n  b = (b3 ,g3 ,r3 ,b2 )
	\par See also:
    Convert_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_32bit( __m128i & r, __m128i & g, __m128i & b)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;

	S32 * prgb1 = (S32 *)&r;
    S32 * prgb2 = (S32 *)&g;
    S32 * prgb3 = (S32 *)&b;

    const S32 * pr = (const S32 *)&temp_r;
    const S32 * pg = (const S32 *)&temp_g;
    const S32 * pb = (const S32 *)&temp_b;

    prgb1[0] = pr[0];
    prgb1[3] = pr[1];
    prgb2[2] = pr[2];
    prgb3[1] = pr[3];

    prgb1[1] = pg[0];
    prgb2[0] = pg[1];
    prgb2[3] = pg[2];
    prgb3[2] = pg[3];

    prgb1[2] = pb[0];
    prgb2[1] = pb[1];
    prgb3[0] = pb[2];
    prgb3[3] = pb[3];
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[out] rgb1 After the function returns, it contains first 16 values of rgb data
    \param[out] rgb2 After the function returns, it contains second 16 values of rgb data
    \param[out] rgb3 After the function returns, it contains third 16 values of rgb data
    \return none
	\par Notes:
    The function operates on 8-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  rgb1 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 ,g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g10,r10,b9 ,g9 ,r9 ,b8 ,g8 ,r8 ,b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
    \n  rgb3 = (b15,g15,r15,b14,g14,r14,b13,g13,r13,b12,g12,r12,b11,g11,r11,b10)
	\par See also:
    Convert_3P_to_3C_8bit( __m128i & r, __m128i & g, __m128i & b)
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_8bit(rgb1, rgb2, rgb3);
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[out] rgb1 After the function returns, it contains first 8 values of rgb data
    \param[out] rgb2 After the function returns, it contains second 8 values of rgb data
    \param[out] rgb3 After the function returns, it contains third 8 values of rgb data 
    \return none
	\par Notes:
    The function operates on 16-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  rgb1 = (g2 ,r2 ,b1 ,g1 ,r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (r5 ,b4 ,g4 ,r4 ,b3 ,g3 ,r3 ,b2 )
    \n  rgb3 = (b7 ,g7 ,r7 ,b6 ,g6 ,r6 ,b5 ,g5 )
	\par See also:
    Convert_3P_to_3C_16bit( __m128i & r, __m128i & g, __m128i & b)
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_16bit(rgb1, rgb2, rgb3);
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[out] rgb1 After the function returns, it contains first 4 values of rgb data
    \param[out] rgb2 After the function returns, it contains second 4 values of rgb data
    \param[out] rgb3 After the function returns, it contains third 4 values of rgb data 
    \return none
	\par Notes:
    The function operates on 32-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \par Output:
    \n  rgb1 = (r1 ,b0 ,g0 ,r0 )
    \n  rgb2 = (g2 ,r2 ,b1 ,g1 )
    \n  rgb3 = (b3 ,g3 ,r3 ,b2 )
	\par See also:
    Convert_3P_to_3C_32bit( __m128i & r, __m128i & g, __m128i & b)
*/
CBL_INLINE void CBL_REF::Convert_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_32bit(rgb1, rgb2, rgb3);
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r15,g15,b15)
	\par See also:
    \n ConvertAndStream_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    const S8 * pr = (S8 *)&r;
    const S8 * pg = (S8 *)&g;
    const S8 * pb = (S8 *)&b;
    S8 * prgb = (S8*)pAlignedData;

    for( int n = 0; n < 16; n++ )
    {
        prgb[3 * n + 0] = pr[n];
        prgb[3 * n + 1] = pg[n];
        prgb[3 * n + 2] = pb[n];
    }
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r7,g7,b7)
	\par See also:
    \n ConvertAndStream_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    S16 * prgb = (S16 *)pAlignedData;
    const S16 * pr = (const S16 *)&r;
    const S16 * pg = (const S16 *)&g;
    const S16 * pb = (const S16 *)&b;

    for( int n = 0; n < 8; n++ )
    {
        prgb[3 * n + 0] = pr[n];
        prgb[3 * n + 1] = pg[n];
        prgb[3 * n + 2] = pb[n];
    }
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2, r3, g3, b3)
	\par See also:
    \n ConvertAndStream_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    S32 * prgb = (S32 *)pAlignedData;
    const S32 * pr = (const S32 *)&r;
    const S32 * pg = (const S32 *)&g;
    const S32 * pb = (const S32 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        prgb[3 * n + 0] = pr[n];
        prgb[3 * n + 1] = pg[n];
        prgb[3 * n + 2] = pb[n];
    }
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r15,g15,b15)
	\par See also:
    \n ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    ConvertAndStoreAligned_3P_to_3C_8bit( r, g, b, pAlignedData );
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r7,g7,b7)
	\par See also:
    \n ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    ConvertAndStoreAligned_3P_to_3C_16bit( r, g, b, pAlignedData );
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, r1, g1, b1, r2, g2, r3, g3, b3)
	\par See also:
    \n ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreUnaligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    ConvertAndStoreAligned_3P_to_3C_32bit( r, g, b, pAlignedData );
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r15,g15,b15)
	\par See also:
    \n ConvertAndStream_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_3C_8bit( r, g, b, pUnalignedData );
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2, g2,......r7,g7,b7)
	\par See also:
    \n ConvertAndStream_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_3C_16bit( r, g, b, pUnalignedData );
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g, and b channels.
    It combines the three separate channels r, g, and b into a three channel rgb data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2, g2, r3, g3, b3)
	\par See also:
    \n ConvertAndStream_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
    \n ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_3C_32bit( r, g, b, pUnalignedData );
}

// convert 4-channel RGBA to 4-planar format

/*! 
    \param[in,out] rgba1 contains first 16 values of rgba data as input. After the function returns, it contains 16 values of r-channel
    \param[in,out] rgba2 contains second 16 values of rgba data as input. After the function returns, it contains 16 values of g-channel
    \param[in,out] rgba3 contains third 16 values of rgba data as input. After the function returns, it contains 16 values of b-channel
    \param[in,out] rgba4 contains fourth 16 values of rgba data as input. After the function returns, it contains 16 values of a-channel
    \return none
	\par Notes:
    The function operates on 8-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    The output is contained in the same input registers.
    \par Input:
    \n  rgba1 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 ,a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 ,a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba3 = (a11,b11,g11,r11,a10,b10,g10,r10,a9 ,b9 ,g9 ,r9 ,a8 ,b8 ,g8 ,r8 )
    \n  rgba4 = (a15,b15,g15,r15,a14,b14,g14,r14,a13,b13,g13,r13,a12,b12,g12,r12)
    \par Output:
    \n  rgba1 = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  rgba2 = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  rgba3 = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  rgba4 = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_8bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
{
	const __m128i temp_rgba1 = rgba1;
	const __m128i temp_rgba2 = rgba2;
	const __m128i temp_rgba3 = rgba3;
	const __m128i temp_rgba4 = rgba4;

    const S8 * prgba1 = (const S8 *)&temp_rgba1;
    const S8 * prgba2 = (const S8 *)&temp_rgba2;
    const S8 * prgba3 = (const S8 *)&temp_rgba3;
    const S8 * prgba4 = (const S8 *)&temp_rgba4;

    S8 * pr = (S8 *)&rgba1;
    S8 * pg = (S8 *)&rgba2;
    S8 * pb = (S8 *)&rgba3;
    S8 * pa = (S8 *)&rgba4;

    for( int n = 0; n < 4; n++ )
    {
        pr[0+n] = prgba1[4*n];
        pr[4+n] = prgba2[4*n];
        pr[8+n] = prgba3[4*n];
        pr[12+n] = prgba4[4*n];

        pg[0+n] = prgba1[4*n+1];
        pg[4+n] = prgba2[4*n+1];
        pg[8+n] = prgba3[4*n+1];
        pg[12+n] = prgba4[4*n+1];

        pb[0+n] = prgba1[4*n+2];
        pb[4+n] = prgba2[4*n+2];
        pb[8+n] = prgba3[4*n+2];
        pb[12+n] = prgba4[4*n+2];

        pa[0+n] = prgba1[4*n+3];
        pa[4+n] = prgba2[4*n+3];
        pa[8+n] = prgba3[4*n+3];
        pa[12+n] = prgba4[4*n+3];
    }
}

/*! 
    \param[in,out] rgba1 contains first 8 values of rgba data as input. After the function returns, it contains 8 values of r-channel
    \param[in,out] rgba2 contains second 8 values of rgba data as input. After the function returns, it contains 8 values of g-channel
    \param[in,out] rgba3 contains third 8 values of rgba data as input. After the function returns, it contains 8 values of b-channel
    \param[in,out] rgba4 contains fourth 8 values of rgba data as input. After the function returns, it contains 8 values of a-channel
    \return none
	\par Notes:
    The function operates on 16-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    The output is contained in the same input registers.
    \par Input:
    \n  rgba1 = (a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 )
    \n  rgba3 = (a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba4 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 )
    \par Output:
    \n  rgba1 = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  rgba2 = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  rgba3 = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  rgba4 = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_16bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
{
	const __m128i temp_rgba1 = rgba1;
	const __m128i temp_rgba2 = rgba2;
	const __m128i temp_rgba3 = rgba3;
	const __m128i temp_rgba4 = rgba4;

    const S16 * prgba1 = (const S16 *)&temp_rgba1;
    const S16 * prgba2 = (const S16 *)&temp_rgba2;
    const S16 * prgba3 = (const S16 *)&temp_rgba3;
    const S16 * prgba4 = (const S16 *)&temp_rgba4;

    S16 * pr = (S16 *)&rgba1;
    S16 * pg = (S16 *)&rgba2;
    S16 * pb = (S16 *)&rgba3;
    S16 * pa = (S16 *)&rgba4;

    for( int n = 0; n < 2; n++ )
    {
        pr[0+n] = prgba1[4*n];
        pr[2+n] = prgba2[4*n];
        pr[4+n] = prgba3[4*n];
        pr[6+n] = prgba4[4*n];

        pg[0+n] = prgba1[4*n+1];
        pg[2+n] = prgba2[4*n+1];
        pg[4+n] = prgba3[4*n+1];
        pg[6+n] = prgba4[4*n+1];

        pb[0+n] = prgba1[4*n+2];
        pb[2+n] = prgba2[4*n+2];
        pb[4+n] = prgba3[4*n+2];
        pb[6+n] = prgba4[4*n+2];

        pa[0+n] = prgba1[4*n+3];
        pa[2+n] = prgba2[4*n+3];
        pa[4+n] = prgba3[4*n+3];
        pa[6+n] = prgba4[4*n+3];
    }
}

/*! 
    \param[in,out] rgba1 contains first 4 values of rgba data as input. After the function returns, it contains 4 values of r-channel
    \param[in,out] rgba2 contains second 4 values of rgba data as input. After the function returns, it contains 4 values of g-channel
    \param[in,out] rgba3 contains third 4 values of rgba data as input. After the function returns, it contains 4 values of b-channel
    \param[in,out] rgba4 contains fourth 4 values of rgba data as input. After the function returns, it contains 4 values of a-channel
    \return none
	\par Notes:
    The function operates on 32-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    The output is contained in the same input registers.
    \par Input:
    \n  rgba1 = (a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a1 ,b1 ,g1 ,r1 )
    \n  rgba3 = (a2 ,b2 ,g2 ,r2 )
    \n  rgba4 = (a3 ,b3 ,g3 ,r3 )
    \par Output:
    \n  rgba1 = (r3 ,r2 ,r1 ,r0 )
    \n  rgba2 = (g3 ,g2 ,g1 ,g0 )
    \n  rgba3 = (b3 ,b2 ,b1 ,b0 )
    \n  rgba4 = (a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_32bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
{
	const __m128i temp_rgba1 = rgba1;
	const __m128i temp_rgba2 = rgba2;
	const __m128i temp_rgba3 = rgba3;
	const __m128i temp_rgba4 = rgba4;

    const S32 * prgba1 = (const S32 *)&temp_rgba1;
    const S32 * prgba2 = (const S32 *)&temp_rgba2;
    const S32 * prgba3 = (const S32 *)&temp_rgba3;
    const S32 * prgba4 = (const S32 *)&temp_rgba4;

    S32 * pr = (S32 *)&rgba1;
    S32 * pg = (S32 *)&rgba2;
    S32 * pb = (S32 *)&rgba3;
    S32 * pa = (S32 *)&rgba4;

    pr[0] = prgba1[0];
    pr[1] = prgba2[0];
    pr[2] = prgba3[0];
    pr[3] = prgba4[0];

    pg[0] = prgba1[1];
    pg[1] = prgba2[1];
    pg[2] = prgba3[1];
    pg[3] = prgba4[1];

    pb[0] = prgba1[2];
    pb[1] = prgba2[2];
    pb[2] = prgba3[2];
    pb[3] = prgba4[2];

    pa[0] = prgba1[3];
    pa[1] = prgba2[3];
    pa[2] = prgba3[3];
    pa[3] = prgba4[3];
}

/*! 
    \param[in] rgba1 contains first 16 values of rgba data as input 
    \param[in] rgba2 contains second 16 values of rgba data as input 
    \param[in] rgba3 contains third 16 values of rgba data as input 
    \param[in] rgba4 contains fourth 16 values of rgba data as input 
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \param[out] a After the function returns, it contains 16 values of a-channel
    \return none
	\par Notes:
    The function operates on 8-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  rgba1 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 ,a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 ,a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba3 = (a11,b11,g11,r11,a10,b10,g10,r10,a9 ,b9 ,g9 ,r9 ,a8 ,b8 ,g8 ,r8 )
    \n  rgba4 = (a15,b15,g15,r15,a14,b14,g14,r14,a13,b13,g13,r13,a12,b12,g12,r12)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_8bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_8bit( r, g, b, a);
}

/*! 
    \param[in] rgba1 contains first 8 values of rgba data as input 
    \param[in] rgba2 contains second 8 values of rgba data as input 
    \param[in] rgba3 contains third 8 values of rgba data as input 
    \param[in] rgba4 contains fourth 8 values of rgba data as input 
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \param[out] a After the function returns, it contains 8 values of a-channel
    \return none
	\par Notes:
    The function operates on 16-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  rgba1 = (a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 )
    \n  rgba3 = (a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba4 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 )
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_16bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_16bit( r, g, b, a);
}

/*! 
    \param[in] rgba1 contains first 4 values of rgba data as input 
    \param[in] rgba2 contains second 4 values of rgba data as input 
    \param[in] rgba3 contains third 4 values of rgba data as input 
    \param[in] rgba4 contains fourth 4 values of rgba data as input 
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \param[out] a After the function returns, it contains 4 values of a-channel
    \return none
	\par Notes:
    The function operates on 16-bit rgba data.
    It separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  rgba1 = (a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a1 ,b1 ,g1 ,r1 )
    \n  rgba3 = (a2 ,b2 ,g2 ,r2 )
    \n  rgba4 = (a3 ,b3 ,g3 ,r3 )
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
	\par See also:
    Convert_4C_to_4P_32bit( __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4)
*/
CBL_INLINE void CBL_REF::Convert_4C_to_4P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_32bit( r, g, b, a);
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgba buffer
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \param[out] a After the function returns, it contains 16 values of a-channel
    \return none
	\par Notes:
    The function operates on aligned 8-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadUnalignedAndConvert_4C_to_4P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    const S8 * pRGBA = (const S8*)pAlignedData;
    S8 * pr = (S8 *)&r;
    S8 * pg = (S8 *)&g;
    S8 * pb = (S8 *)&b;
    S8 * pa = (S8 *)&a;

    for( int n = 0; n < 16; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
        pa[n] = pRGBA[4*n+3];
    }
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgba buffer
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \param[out] a After the function returns, it contains 8 values of a-channel
    \return none
	\par Notes:
    The function operates on aligned 16-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadUnalignedAndConvert_4C_to_4P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    const S16 * pRGBA = (const S16*)pAlignedData;
    S16 * pr = (S16 *)&r;
    S16 * pg = (S16 *)&g;
    S16 * pb = (S16 *)&b;
    S16 * pa = (S16 *)&a;

    for( int n = 0; n < 8; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
        pa[n] = pRGBA[4*n+3];
    }
}

/*! 
    \param[in] pAlignedData void pointer to aligned rgba buffer
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \param[out] a After the function returns, it contains 4 values of a-channel
    \return none
	\par Notes:
    The function operates on aligned 32-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pAlignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadUnalignedAndConvert_4C_to_4P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    const S32 * pRGBA = (const S32*)pAlignedData;
    S32 * pr = (S32 *)&r;
    S32 * pg = (S32 *)&g;
    S32 * pb = (S32 *)&b;
    S32 * pa = (S32 *)&a;

    for( int n = 0; n < 4; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
        pa[n] = pRGBA[4*n+3];
    }
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgba buffer
    \param[out] r After the function returns, it contains 16 values of r-channel
    \param[out] g After the function returns, it contains 16 values of g-channel
    \param[out] b After the function returns, it contains 16 values of b-channel
    \param[out] a After the function returns, it contains 16 values of a-channel
    \return none
	\par Notes:
    The function operates on unaligned 8-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadAlignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_4P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    LoadAlignedAndConvert_4C_to_4P_8bit( pUnalignedData, r, g, b, a );
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgba buffer
    \param[out] r After the function returns, it contains 8 values of r-channel
    \param[out] g After the function returns, it contains 8 values of g-channel
    \param[out] b After the function returns, it contains 8 values of b-channel
    \param[out] a After the function returns, it contains 8 values of a-channel
    \return none
	\par Notes:
    The function operates on unaligned 16-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadAlignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_4P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    LoadAlignedAndConvert_4C_to_4P_16bit( pUnalignedData, r, g, b, a );
}

/*! 
    \param[in] pUnalignedData void pointer to unaligned rgba buffer
    \param[out] r After the function returns, it contains 4 values of r-channel
    \param[out] g After the function returns, it contains 4 values of g-channel
    \param[out] b After the function returns, it contains 4 values of b-channel
    \param[out] a After the function returns, it contains 4 values of a-channel
    \return none
	\par Notes:
    The function operates on unaligned 32-bit rgba data. It takes care of loading the rgba data.
    And then separates the rgba data into four different planar data channels r, g, b and a.
    \par Input:
    \n  *pUnalignedData = (r0, g0, b0, r1, g1, b1, r2,....)
    \par Output:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
	\par See also:
    LoadAlignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
*/
CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_4P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    LoadAlignedAndConvert_4C_to_4P_32bit( pUnalignedData, r, g, b, a );
}

// convert 4-planar RGBA to 4-channel format

/*! 
    \param[in,out] r contains 16 values of r-channel data as input. After the function returns, it contains first 16 values of rgba data
    \param[in,out] g contains 16 values of g-channel data as input. After the function returns, it contains second 16 values of rgba data
    \param[in,out] b contains 16 values of b-channel data as input. After the function returns, it contains third 16 values of rgba data
    \param[in,out] a contains 16 values of a-channel data as input. After the function returns, it contains fourth 16 values of rgba data
    \return none
	\par Notes:
    The function operates on 8-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  r = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 ,a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  g = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 ,a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  b = (a11,b11,g11,r11,a10,b10,g10,r10,a9 ,b9 ,g9 ,r9 ,a8 ,b8 ,g8 ,r8 )
    \n  a = (a15,b15,g15,r15,a14,b14,g14,r14,a13,b13,g13,r13,a12,b12,g12,r12)
	\par See also:
    Convert_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_8bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;
	const __m128i temp_a = a;

    S8 * prgba1 = (S8 *)&r;
    S8 * prgba2 = (S8 *)&g;
    S8 * prgba3 = (S8 *)&b;
    S8 * prgba4 = (S8 *)&a;

    const S8 * pr = (const S8 *)&temp_r;
    const S8 * pg = (const S8 *)&temp_g;
    const S8 * pb = (const S8 *)&temp_b;
    const S8 * pa = (const S8 *)&temp_a;

    for( int n = 0; n < 4; n++ )
    {
        prgba1[4*n] = pr[0+n];
        prgba2[4*n] = pr[4+n];
        prgba3[4*n] = pr[8+n];
        prgba4[4*n] = pr[12+n];

        prgba1[4*n+1] = pg[0+n];
        prgba2[4*n+1] = pg[4+n];
        prgba3[4*n+1] = pg[8+n];
        prgba4[4*n+1] = pg[12+n];

        prgba1[4*n+2] = pb[0+n];
        prgba2[4*n+2] = pb[4+n];
        prgba3[4*n+2] = pb[8+n];
        prgba4[4*n+2] = pb[12+n];

        prgba1[4*n+3] = pa[0+n];
        prgba2[4*n+3] = pa[4+n];
        prgba3[4*n+3] = pa[8+n];
        prgba4[4*n+3] = pa[12+n];
    }
}

/*! 
    \param[in,out] r contains 8 values of r-channel data as input. After the function returns, it contains first 8 values of rgba data
    \param[in,out] g contains 8 values of g-channel data as input. After the function returns, it contains second 8 values of rgba data
    \param[in,out] b contains 8 values of b-channel data as input. After the function returns, it contains third 8 values of rgba data
    \param[in,out] a contains 8 values of a-channel data as input. After the function returns, it contains fourth 8 values of rgba data
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  r = (a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  g = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 )
    \n  b = (a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  a = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 )
	\par See also:
    Convert_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_16bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;
	const __m128i temp_a = a;

    S16 * prgba1 = (S16 *)&r;
    S16 * prgba2 = (S16 *)&g;
    S16 * prgba3 = (S16 *)&b;
    S16 * prgba4 = (S16 *)&a;

    const S16 * pr = (const S16 *)&temp_r;
    const S16 * pg = (const S16 *)&temp_g;
    const S16 * pb = (const S16 *)&temp_b;
    const S16 * pa = (const S16 *)&temp_a;

    for( int n = 0; n < 2; n++ )
    {
        prgba1[4*n] = pr[0+n];
        prgba2[4*n] = pr[2+n];
        prgba3[4*n] = pr[4+n];
        prgba4[4*n] = pr[6+n];

        prgba1[4*n+1] = pg[0+n];
        prgba2[4*n+1] = pg[2+n];
        prgba3[4*n+1] = pg[4+n];
        prgba4[4*n+1] = pg[6+n];

        prgba1[4*n+2] = pb[0+n];
        prgba2[4*n+2] = pb[2+n];
        prgba3[4*n+2] = pb[4+n];
        prgba4[4*n+2] = pb[6+n];

        prgba1[4*n+3] = pa[0+n];
        prgba2[4*n+3] = pa[2+n];
        prgba3[4*n+3] = pa[4+n];
        prgba4[4*n+3] = pa[6+n];
    }
}

/*! 
    \param[in,out] r contains 4 values of r-channel data as input. After the function returns, it contains first 4 values of rgba data
    \param[in,out] g contains 4 values of g-channel data as input. After the function returns, it contains second 4 values of rgba data
    \param[in,out] b contains 4 values of b-channel data as input. After the function returns, it contains third 4 values of rgba data
    \param[in,out] a contains 4 values of a-channel data as input. After the function returns, it contains fourth 4 values of rgba data
    \return none
	\par Notes:
    The function operates on 32-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    The output is contained in the same input registers.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  r = (a0 ,b0 ,g0 ,r0 )
    \n  g = (a1 ,b1 ,g1 ,r1 )
    \n  b = (a2 ,b2 ,g2 ,r2 )
    \n  a = (a3 ,b3 ,g3 ,r3 )
	\par See also:
    Convert_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_32bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
{
	const __m128i temp_r = r;
	const __m128i temp_g = g;
	const __m128i temp_b = b;
	const __m128i temp_a = a;

    S32 * prgba1 = (S32 *)&r;
    S32 * prgba2 = (S32 *)&g;
    S32 * prgba3 = (S32 *)&b;
    S32 * prgba4 = (S32 *)&a;

    const S32 * pr = (const S32 *)&temp_r;
    const S32 * pg = (const S32 *)&temp_g;
    const S32 * pb = (const S32 *)&temp_b;
    const S32 * pa = (const S32 *)&temp_a;

    prgba1[0] = pr[0];
    prgba2[0] = pr[1];
    prgba3[0] = pr[2];
    prgba4[0] = pr[3];

    prgba1[1] = pg[0];
    prgba2[1] = pg[1];
    prgba3[1] = pg[2];
    prgba4[1] = pg[3];

    prgba1[2] = pb[0];
    prgba2[2] = pb[1];
    prgba3[2] = pb[2];
    prgba4[2] = pb[3];

    prgba1[3] = pa[0];
    prgba2[3] = pa[1];
    prgba3[3] = pa[2];
    prgba4[3] = pa[3];
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[in] a contains 16 values of a-channel data as input
    \param[out] rgba1 After the function returns, it contains first 16 values of rgba data
    \param[out] rgba2 After the function returns, it contains second 16 values of rgba data
    \param[out] rgba3 After the function returns, it contains third 16 values of rgba data
    \param[out] rgba4 After the function returns, it contains fourth 16 values of rgba data
    \return none
	\par Notes:
    The function operates on 8-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  rgba1 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 ,a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 ,a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba3 = (a11,b11,g11,r11,a10,b10,g10,r10,a9 ,b9 ,g9 ,r9 ,a8 ,b8 ,g8 ,r8 )
    \n  rgba4 = (a15,b15,g15,r15,a14,b14,g14,r14,a13,b13,g13,r13,a12,b12,g12,r12)
	\par See also:
    Convert_4P_to_4C_8bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_8bit( rgba1, rgba2, rgba3, rgba4);
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[in] a contains 8 values of a-channel data as input
    \param[out] rgba1 After the function returns, it contains first 8 values of rgba data
    \param[out] rgba2 After the function returns, it contains second 8 values of rgba data
    \param[out] rgba3 After the function returns, it contains third 8 values of rgba data
    \param[out] rgba4 After the function returns, it contains fourth 8 values of rgba data
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  rgba1 = (a1 ,b1 ,g1 ,r1 ,a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a3 ,b3 ,g3 ,r3 ,a2 ,b2 ,g2 ,r2 )
    \n  rgba3 = (a5 ,b5 ,g5 ,r5 ,a4 ,b4 ,g4 ,r4 )
    \n  rgba4 = (a7 ,b7 ,g7 ,r7 ,a6 ,b6 ,g6 ,r6 )
	\par See also:
    Convert_4P_to_4C_16bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_16bit( rgba1, rgba2, rgba3, rgba4);
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[in] a contains 4 values of a-channel data as input
    \param[out] rgba1 After the function returns, it contains first 4 values of rgba data
    \param[out] rgba2 After the function returns, it contains second 4 values of rgba data
    \param[out] rgba3 After the function returns, it contains third 4 values of rgba data
    \param[out] rgba4 After the function returns, it contains fourth 4 values of rgba data
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
    \par Output:
    \n  rgba1 = (a0 ,b0 ,g0 ,r0 )
    \n  rgba2 = (a1 ,b1 ,g1 ,r1 )
    \n  rgba3 = (a2 ,b2 ,g2 ,r2 )
    \n  rgba4 = (a3 ,b3 ,g3 ,r3 )
	\par See also:
    Convert_4P_to_4C_32bit( __m128i & r, __m128i & g, __m128i & b, __m128i & a)
*/
CBL_INLINE void CBL_REF::Convert_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_32bit( rgba1, rgba2, rgba3, rgba4);
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[in] a contains 16 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1, r2, g2,......r15,g15,b15, a15)
	\par See also:
    \n ConvertAndStoreUnaligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
    \n ConvertAndStream_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    S8 * prgba = (S8 *)pAlignedData;
    const S8 * pr = (const S8 *)&r;
    const S8 * pg = (const S8 *)&g;
    const S8 * pb = (const S8 *)&b;
    const S8 * pa = (const S8 *)&a;

    for( int n = 0; n < 16; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
        prgba[4*n+3] = pa[n];
    }
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[in] a contains 8 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1, r2, g2,......r7,g7,b7, a7)
	\par See also:
    \n ConvertAndStream_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStoreUnaligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    S16 * prgba = (S16 *)pAlignedData;
    const S16 * pr = (const S16 *)&r;
    const S16 * pg = (const S16 *)&g;
    const S16 * pb = (const S16 *)&b;
    const S16 * pa = (const S16 *)&a;

    for( int n = 0; n < 8; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
        prgba[4*n+3] = pa[n];
    }
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[in] a contains 4 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1, r2, g2, r3, g3, b3, a3)
	\par See also:
    \n ConvertAndStream_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStoreUnaligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    S32 * prgba = (S32 *)pAlignedData;
    const S32 * pr = (const S32 *)&r;
    const S32 * pg = (const S32 *)&g;
    const S32 * pb = (const S32 *)&b;
    const S32 * pa = (const S32 *)&a;

    for( int n = 0; n < 4; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
        prgba[4*n+3] = pa[n];
    }
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[in] a contains 16 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1,......r15,g15,b15,a15)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStoreUnaligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    ConvertAndStoreAligned_4P_to_4C_8bit( r, g, b, a, pAlignedData );
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[in] a contains 8 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1,......r7,g7,b7,a7)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStoreUnaligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    ConvertAndStoreAligned_4P_to_4C_16bit( r, g, b, a, pAlignedData );
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[in] a contains 4 values of a-channel data as input
    \param[out] pAlignedData void pointer to an aligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pAlignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pAlignedData = (r0, g0, b0, a0, r1, g1, b1, a1,......r3,g3,b3,a3)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStoreUnaligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStream_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    ConvertAndStoreAligned_4P_to_4C_32bit( r, g, b, a, pAlignedData );
}

/*! 
    \param[in] r contains 16 values of r-channel data as input
    \param[in] g contains 16 values of g-channel data as input
    \param[in] b contains 16 values of b-channel data as input
    \param[in] a contains 16 values of a-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 8-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r15,r14,r13,r12,r11,r10,r9 ,r8 ,r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g15,g14,g13,g12,g11,g10,g9 ,g8 ,g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b15,b14,b13,b12,b11,b10,b9 ,b8 ,b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a15,a14,a13,a12,a11,a10,a9 ,a8 ,a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, a0, r1, g1, b1, a1, r2, g2,......r15,g15,b15,a15)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStream_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
{
    ConvertAndStoreAligned_4P_to_4C_8bit( r, g, b, a, pUnalignedData );
}

/*! 
    \param[in] r contains 8 values of r-channel data as input
    \param[in] g contains 8 values of g-channel data as input
    \param[in] b contains 8 values of b-channel data as input
    \param[in] a contains 8 values of a-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 16-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r7 ,r6 ,r5 ,r4 ,r3 ,r2 ,r1 ,r0 )
    \n  g = (g7 ,g6 ,g5 ,g4 ,g3 ,g2 ,g1 ,g0 )
    \n  b = (b7 ,b6 ,b5 ,b4 ,b3 ,b2 ,b1 ,b0 )
    \n  a = (a7 ,a6 ,a5 ,a4 ,a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, a0, r1, g1, b1, a1, r2, g2,......r7,g7,b7,a7)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStream_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
{
    ConvertAndStoreAligned_4P_to_4C_16bit( r, g, b, a, pUnalignedData );
}

/*! 
    \param[in] r contains 4 values of r-channel data as input
    \param[in] g contains 4 values of g-channel data as input
    \param[in] b contains 4 values of b-channel data as input
    \param[in] a contains 4 values of a-channel data as input
    \param[out] pUnalignedData void pointer to an unaligned memory location 
    \return none
	\par Notes:
    The function operates on 32-bit r,g,b and a channels.
    It combines the four separate channels r, g, b and a into a four channel rgba data and stores 
    it at the memory location pointed by pUnalignedData.
    \par Input:
    \n  r = (r3 ,r2 ,r1 ,r0 )
    \n  g = (g3 ,g2 ,g1 ,g0 )
    \n  b = (b3 ,b2 ,b1 ,b0 )
    \n  a = (a3 ,a2 ,a1 ,a0 )
    \par Output:
    *pUnalignedData = (r0, g0, b0, a0,r1, g1, b1, a1, r2, g2, b2, a2, r3, g3, b3, a3)
	\par See also:
    \n ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    \n ConvertAndStream_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
*/
CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnalignedData )
{
    ConvertAndStoreAligned_4P_to_4C_32bit( r, g, b, a, pUnalignedData );
}

CBL_INLINE void CBL_REF::Convert_4C_to_3P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b )
{
    const S8 * prgba1 = (const S8 *)&rgba1;
    const S8 * prgba2 = (const S8 *)&rgba2;
    const S8 * prgba3 = (const S8 *)&rgba3;
    const S8 * prgba4 = (const S8 *)&rgba4;
    S8 * pr = (S8 *)&r;
    S8 * pg = (S8 *)&g;
    S8 * pb = (S8 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        pr[0+n] = prgba1[4*n];
        pr[4+n] = prgba2[4*n];
        pr[8+n] = prgba3[4*n];
        pr[12+n] = prgba4[4*n];

        pg[0+n] = prgba1[4*n+1];
        pg[4+n] = prgba2[4*n+1];
        pg[8+n] = prgba3[4*n+1];
        pg[12+n] = prgba4[4*n+1];

        pb[0+n] = prgba1[4*n+2];
        pb[4+n] = prgba2[4*n+2];
        pb[8+n] = prgba3[4*n+2];
        pb[12+n] = prgba4[4*n+2];
    }
}

CBL_INLINE void CBL_REF::Convert_4C_to_3P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b )
{
    const S16 * prgba1 = (const S16 *)&rgba1;
    const S16 * prgba2 = (const S16 *)&rgba2;
    const S16 * prgba3 = (const S16 *)&rgba3;
    const S16 * prgba4 = (const S16 *)&rgba4;
    S16 * pr = (S16 *)&r;
    S16 * pg = (S16 *)&g;
    S16 * pb = (S16 *)&b;

    for( int n = 0; n < 2; n++ )
    {
        pr[0+n] = prgba1[4*n];
        pr[2+n] = prgba2[4*n];
        pr[4+n] = prgba3[4*n];
        pr[6+n] = prgba4[4*n];

        pg[0+n] = prgba1[4*n+1];
        pg[2+n] = prgba2[4*n+1];
        pg[4+n] = prgba3[4*n+1];
        pg[6+n] = prgba4[4*n+1];

        pb[0+n] = prgba1[4*n+2];
        pb[2+n] = prgba2[4*n+2];
        pb[4+n] = prgba3[4*n+2];
        pb[6+n] = prgba4[4*n+2];
    }
}

CBL_INLINE void CBL_REF::Convert_4C_to_3P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b )
{
    const S32 * prgba1 = (const S32 *)&rgba1;
    const S32 * prgba2 = (const S32 *)&rgba2;
    const S32 * prgba3 = (const S32 *)&rgba3;
    const S32 * prgba4 = (const S32 *)&rgba4;
    S32 * pr = (S32 *)&r;
    S32 * pg = (S32 *)&g;
    S32 * pb = (S32 *)&b;

    pr[0] = prgba1[0];
    pr[1] = prgba2[0];
    pr[2] = prgba3[0];
    pr[3] = prgba4[0];

    pg[0] = prgba1[1];
    pg[1] = prgba2[1];
    pg[2] = prgba3[1];
    pg[3] = prgba4[1];

    pb[0] = prgba1[2];
    pb[1] = prgba2[2];
    pb[2] = prgba3[2];
    pb[3] = prgba4[2];
}

CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    const S8 * pRGBA = (const S8*)pAlignedData;
    S8 * pr = (S8 *)&r;
    S8 * pg = (S8 *)&g;
    S8 * pb = (S8 *)&b;

    for( int n = 0; n < 16; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
    }
}

CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    const S16 * pRGBA = (const S16*)pAlignedData;
    S16 * pr = (S16 *)&r;
    S16 * pg = (S16 *)&g;
    S16 * pb = (S16 *)&b;

    for( int n = 0; n < 8; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
    }
}

CBL_INLINE void CBL_REF::LoadAlignedAndConvert_4C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    const S16 * pRGBA = (const S16*)pAlignedData;
    S32 * pr = (S32 *)&r;
    S32 * pg = (S32 *)&g;
    S32 * pb = (S32 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        pr[n] = pRGBA[4*n+0];
        pg[n] = pRGBA[4*n+1];
        pb[n] = pRGBA[4*n+2];
    }
}

CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_3P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_4C_to_3P_8bit( pUnalignedData, r, g, b );
}

CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_3P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_4C_to_3P_16bit( pUnalignedData, r, g, b );
}

CBL_INLINE void CBL_REF::LoadUnalignedAndConvert_4C_to_3P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    LoadAlignedAndConvert_4C_to_3P_32bit( pUnalignedData, r, g, b );
}

// convert 3-planar RGB to 4-channel RGBA format

CBL_INLINE void CBL_REF::Convert_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
    S8 * prgba1 = (S8 *)&rgba1;
    S8 * prgba2 = (S8 *)&rgba2;
    S8 * prgba3 = (S8 *)&rgba3;
    S8 * prgba4 = (S8 *)&rgba4;
    const S8 * pr = (const S8 *)&r;
    const S8 * pg = (const S8 *)&g;
    const S8 * pb = (const S8 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        prgba1[4*n] = pr[0+n];
        prgba2[4*n] = pr[4+n];
        prgba3[4*n] = pr[8+n];
        prgba4[4*n] = pr[12+n];

        prgba1[4*n+1] = pg[0+n];
        prgba2[4*n+1] = pg[4+n];
        prgba3[4*n+1] = pg[8+n];
        prgba4[4*n+1] = pg[12+n];

        prgba1[4*n+2] = pb[0+n];
        prgba2[4*n+2] = pb[4+n];
        prgba3[4*n+2] = pb[8+n];
        prgba4[4*n+2] = pb[12+n];
    }
}

CBL_INLINE void CBL_REF::Convert_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
    S16 * prgba1 = (S16 *)&rgba1;
    S16 * prgba2 = (S16 *)&rgba2;
    S16 * prgba3 = (S16 *)&rgba3;
    S16 * prgba4 = (S16 *)&rgba4;
    const S16 * pr = (const S16 *)&r;
    const S16 * pg = (const S16 *)&g;
    const S16 * pb = (const S16 *)&b;

    for( int n = 0; n < 2; n++ )
    {
        prgba1[4*n] = pr[0+n];
        prgba2[4*n] = pr[2+n];
        prgba3[4*n] = pr[4+n];
        prgba4[4*n] = pr[6+n];

        prgba1[4*n+1] = pg[0+n];
        prgba2[4*n+1] = pg[2+n];
        prgba3[4*n+1] = pg[4+n];
        prgba4[4*n+1] = pg[6+n];

        prgba1[4*n+2] = pb[0+n];
        prgba2[4*n+2] = pb[2+n];
        prgba3[4*n+2] = pb[4+n];
        prgba4[4*n+2] = pb[6+n];
    }
}

CBL_INLINE void CBL_REF::Convert_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
    S32 * prgba1 = (S32 *)&rgba1;
    S32 * prgba2 = (S32 *)&rgba2;
    S32 * prgba3 = (S32 *)&rgba3;
    S32 * prgba4 = (S32 *)&rgba4;
    const S32 * pr = (const S32 *)&r;
    const S32 * pg = (const S32 *)&g;
    const S32 * pb = (const S32 *)&b;

    prgba1[0] = pr[0];
    prgba2[0] = pr[1];
    prgba3[0] = pr[2];
    prgba4[0] = pr[3];

    prgba1[1] = pg[0];
    prgba2[1] = pg[1];
    prgba3[1] = pg[2];
    prgba4[1] = pg[3];

    prgba1[2] = pb[0];
    prgba2[2] = pb[1];
    prgba3[2] = pb[2];
    prgba4[2] = pb[3];
}

CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    S8 * prgba = (S8 *)pAlignedData;
    const S8 * pr = (const S8 *)&r;
    const S8 * pg = (const S8 *)&g;
    const S8 * pb = (const S8 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
    }
}

CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    S16 * prgba = (S16 *)pAlignedData;
    const S16 * pr = (const S16 *)&r;
    const S16 * pg = (const S16 *)&g;
    const S16 * pb = (const S16 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
    }
}

CBL_INLINE void CBL_REF::ConvertAndStoreAligned_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    S32 * prgba = (S32 *)pAlignedData;
    const S32 * pr = (const S32 *)&r;
    const S32 * pg = (const S32 *)&g;
    const S32 * pb = (const S32 *)&b;

    for( int n = 0; n < 4; n++ )
    {
        prgba[4*n+0] = pr[n];
        prgba[4*n+1] = pg[n];
        prgba[4*n+2] = pb[n];
    }
}

CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_4C_8bit( r, g, b, pUnalignedData );
}

CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_4C_16bit( r, g, b, pUnalignedData );
}

CBL_INLINE void CBL_REF::ConvertAndStoreUnaligned_3P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    ConvertAndStoreAligned_3P_to_4C_32bit( r, g, b, pUnalignedData );
}

#endif // __CBL_TRANSPOSE_PRIMITIVES_H__
