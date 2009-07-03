/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_TRANSPOSE_PRIMITIVES_SSE2_H__
#define __CBL_TRANSPOSE_PRIMITIVES_SSE2_H__

#pragma warning ( push )
#pragma warning ( disable : 4328 )

/// @cond OPT_DETAILS

// convert 3-channel RGB to 3-planar format

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_8bit(__m128i &rgb1,__m128i &rgb2,__m128i &rgb3)
{
		__m128i temp1, temp2;
	
		rgb2  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	// b7, g7, r7, b6, g6, r6, b5, g5,g10,r10, b9, g9, r9, b8, g8, r8
		temp1 = _mm_unpacklo_epi8(rgb1, rgb2);					//g10, g2,r10, r2, b9, b1, g9, g1, r9, r1, b8, b0, g8, g0, r8, r0
		temp2 = _mm_unpackhi_epi8(rgb2, rgb3);					//b15, b7,g15, g7,r15, r7,b14, b6,g14, g6,r14, r6,b13, b5,g13, g5
		rgb3  = _mm_slli_si128(rgb3, 8);						//r13,b12,g12,r12,b11,g11,r11,b10,  0,  0,  0,  0,  0,  0,  0,  0
		rgb2  = _mm_unpackhi_epi8(rgb1, rgb3);					//r13, r5,b12, b4,g12, g4,r12, r4,b11, b3,g11, g3,r11, r3,b10, b2
		
		rgb3  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	//b11, b3,g11, g3,r11, r3,b10, b2,r13, r5,b12, b4,g12, g4,r12, r4
		rgb1  = _mm_unpacklo_epi8(temp1, rgb3);					//r13, r9, r5, r1,b12, b8, b4, b0,g12, g8, g4, g0,r12, r8, r4, r0
		temp1 = _mm_srli_si128(temp1, 8);						//  0,  0,  0,  0,  0,  0,  0,  0,g10, g2,r10, r2, b9, b1, g9, g1
		temp1 = _mm_unpacklo_epi8(temp1, temp2);				//g14,g10, g6, g2,r14,r10, r6, r2,b13, b9, b5, b1,g13, g9, g5, g1
		temp2 = _mm_unpackhi_epi8(rgb3, temp2);					//b15,b11, b7, b3,g15,g11, g7, g3,r15,r11, r7, r3,b14,b10, b6, b2
	
		temp1 = _mm_shuffle_epi32(temp1, _MM_SHUFFLE(1,0,3,2)); //b13, b9, b5, b1,g13, g9, g5, g1,g14,g10, g6, g2,r14,r10, r6, r2
		rgb3  = _mm_unpackhi_epi8(temp1, temp2);				//b15,b13,b11, b9, b7, b5, b3, b1,g15,g13,g11, g9, g7, g5, g3, g1
		temp2 = _mm_slli_si128(temp2, 8);						//r15,r11, r7, r3,b14,b10, b6, b2,  0,  0,  0,  0,  0,  0,  0,  0
		temp2 = _mm_unpackhi_epi8(rgb1, temp2);					//r15,r13,r11, r9, r7, r5, r3, r1,b14,b12,b10, b8, b6, b4, b2, b0
		temp1 = _mm_unpacklo_epi8(rgb1, temp1);					//g14,g12,g10, g8, g6, g4, g2, g0,r14,r12,r10, r8, r6, r4, r2, r0
	
		temp2 = _mm_shuffle_epi32(temp2, _MM_SHUFFLE(1,0,3,2)); //b14,b12,b10, b8, b6, b4, b2, b0,r15,r13,r11, r9, r7, r5, r3, r1
		rgb1  = _mm_unpacklo_epi8(temp1, temp2);				//r15,r14,r13,r12,r11,r10, r9, r8, r7, r6, r5, r4, r3, r2, r1, r0	
		temp1 = _mm_srli_si128(temp1, 8);						//  0,  0,  0,  0,  0,  0,  0,  0,g14,g12,g10, g8, g6, g4, g2, g0
		rgb2  = _mm_unpacklo_epi8(temp1, rgb3);					//g15,g14,g13,g12,g11,g10, g9, g8, g7, g6, g5, g4, g3, g2, g1, g0	
		rgb3  = _mm_unpackhi_epi8(temp2,rgb3);					//b15,b14,b13,b12,b11,b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0	
}

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_16bit(__m128i &rgb1,__m128i &rgb2,__m128i &rgb3)
{
		__m128i temp1, temp2;

		rgb2  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	//b3,g3,r3,b2,r5,b4,g4,r4
		temp1 = _mm_unpacklo_epi16(rgb1, rgb2);					//r5,r1,b4,b0,g4,g0,r4,r0
		temp2 = _mm_unpackhi_epi16(rgb2, rgb3);					//b7,b3,g7,g3,r7,r3,b6,b2
		rgb3  = _mm_slli_si128(rgb3, 8);						//g6,r6,b5,g5, 0, 0, 0, 0
		rgb2  = _mm_unpackhi_epi16(rgb1, rgb3);					//g6,g2,r6,r2,b5,b1,g5,g1
		
		rgb3  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	//b5,b1,g5,g1,g6,g2,r6,r2
		rgb1  = _mm_unpacklo_epi16(temp1, rgb3);				//g6,g4,g2,g0,r6,r4,r2,r0
		temp1 = _mm_srli_si128(temp1, 8);						// 0, 0, 0, 0,r5,r1,b4,b0
		temp1 = _mm_unpacklo_epi16(temp1, temp2);				//r7,r5,r3,r1,b6,b4,b2,b0
		temp2 = _mm_unpackhi_epi16(rgb3, temp2);				//b7,b5,b3,b1,g7,g5,g3,g1

		temp1 = _mm_shuffle_epi32(temp1, _MM_SHUFFLE(1,0,3,2)); //b6,b4,b2,b0,r7,r5,r3,r1
		rgb3  = _mm_unpackhi_epi16(temp1, temp2);				//b7,b6,b5,b4,b3,b2,b1,b0				
		temp2 = _mm_slli_si128(temp2, 8);						//g7,g5,g3,g1, 0, 0, 0, 0
		rgb2  = _mm_unpackhi_epi16(rgb1, temp2);				//g7,g6,g5,g4,g3,g2,g1,g0				
		rgb1  = _mm_unpacklo_epi16(rgb1, temp1);				//r7,r6,r5,r4,r3,r2,r1,r0				
}

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_32bit(__m128i &rgb1,__m128i &rgb2,__m128i &rgb3)
{
		__m128i temp1, temp2;
		
		rgb2  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	//b1,g1,g2,r2
		temp1 = _mm_unpacklo_epi32(rgb1, rgb2);					//g2,g0,r2,r0
		temp2 = _mm_unpackhi_epi32(rgb2, rgb3);					//b3,b1,g3,g1
		rgb3  = _mm_slli_si128(rgb3, 8);						//r3,b2, 0, 0
		rgb2  = _mm_unpackhi_epi32(rgb1, rgb3);					//r3,r1,b2,b0
		
		rgb3  = _mm_shuffle_epi32(rgb2, _MM_SHUFFLE(1,0,3,2));	//b2,b0,r3,r1
		rgb1  = _mm_unpacklo_epi32(temp1, rgb3);				//r3,r2,r1,r0
		temp1 = _mm_srli_si128(temp1, 8);						// 0, 0,g2,g0
		rgb2  = _mm_unpacklo_epi32(temp1, temp2);				//g3,g2,g1,g0
		rgb3  = _mm_unpackhi_epi32(rgb3, temp2);				//b3,b2,b1,b0
}

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_8bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_8bit(r,g,b);
}

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_16bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_16bit(r,g,b);
}

CBL_INLINE void CBL_SSE2::Convert_3C_to_3P_32bit( const __m128i & rgb1, const __m128i & rgb2, const __m128i & rgb3, __m128i & r, __m128i & g, __m128i & b )
{
	r = rgb1;
	g = rgb2;
	b = rgb3;
	Convert_3C_to_3P_32bit(r,g,b);
}

namespace CBL_Library_Internal
{
    template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		CBL_SSE2::Convert_3C_to_3P_8bit( r, g, b);
    }

    template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		CBL_SSE2::Convert_3C_to_3P_16bit( r, g, b);
    }

	template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		CBL_SSE2::Convert_3C_to_3P_32bit( r, g, b);
    }

} // namespace CBL_Library_Internal

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_3C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_8bit< CBL_AlignedLoad >( pAlignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_3C_to_3P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_16bit< CBL_AlignedLoad >( pAlignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_3C_to_3P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_32bit< CBL_AlignedLoad >( pAlignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_3C_to_3P_8bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_8bit< CBL_UnalignedLoad >( pUnalignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_3C_to_3P_16bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_16bit< CBL_UnalignedLoad >( pUnalignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_3C_to_3P_32bit( const void * pUnalignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_3C_to_3P_32bit< CBL_UnalignedLoad >( pUnalignedData, r, g, b );
}


// convert 3-planar RGB to 3-channel format

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_8bit(__m128i &r,__m128i &g,__m128i &b)
{
		__m128i temp;

		temp = _mm_srli_si128(r, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15,r14,r13,r12,r11,r10, r9, r8
		r    = _mm_unpacklo_epi8(r, temp);		//r15, r7,r14, r6,r13, r5,r12, r4,r11, r3,r10, r2, r9, r1, r8, r0
		temp = _mm_srli_si128(r, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15, r7,r14, r6,r13, r5,r12, r4 	
		r    = _mm_unpacklo_epi8(r, temp);		//r15,r11, r7, r3,r14,r10, r6, r2,r13, r9, r5, r1,r12, r8, r4, r0
		temp = _mm_srli_si128(r, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15,r11, r7, r3,r14,r10, r6, r2
		r    = _mm_unpacklo_epi8(r, temp);		//r15,r13,r11, r9, r7, r5, r3, r1,r14,r12,r10, r8, r6, r4, r2, r0

		temp = _mm_srli_si128(g, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g15,g14,g13,g12,g11,g10, g9, g8
		g    = _mm_unpacklo_epi8(g, temp);		//g15, g7,g14, g6,g13, g5,g12, g4,g11, g3,g10, g2, g9, g1, g8, g0
		temp = _mm_srli_si128(g, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g15, g7,g14, g6,g13, g5,g12, g4 
		g    = _mm_unpacklo_epi8(g, temp);		//g15,g11, g7, g3,g14,g10, g6, g2,g13, g9, g5, g1,g12, g8, g4, g0
		temp = _mm_srli_si128(g, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g15,g11, g7, g3,g14,g10, g6, g2
		g    = _mm_unpacklo_epi8(g, temp);		//g15,g13,g11, g9, g7, g5, g3, g1,g14,g12,g10, g8, g6, g4, g2, g0

		temp = _mm_srli_si128(b, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,b15,b14,b13,b12,b11,b10, b9, b8
		b    = _mm_unpacklo_epi8(b, temp);		//b15, b7,b14, b6,b13, b5,b12, b4,b11, b3,b10, b2, b9, b1, b8, b0
		temp = _mm_srli_si128(b, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,b15, b7,b14, b6,b13, b5,b12, b4 
		b    = _mm_unpacklo_epi8(b, temp);		//b15,b11, b7, b3,b14,b10, b6, b2,b13, b9, b5, b1,b12, b8, b4, b0
		temp = _mm_srli_si128(b, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,b15,b11, b7, b3,b14,b10, b6, b2
		b    = _mm_unpacklo_epi8(b, temp);		//b15,b13,b11, b9, b7, b5, b3, b1,b14,b12,b10, b8, b6, b4, b2, b0

		temp = _mm_unpacklo_epi8(r, g);			//g14,r14,g12,r12,g10,r10, g8, r8, g6, r6, g4, r4, g2, r2, g0, r0
		g    = _mm_unpackhi_epi8(g, b);			//b15,g15,b13,g13,b11,g11, b9, g9, b7, g7, b5, g5, b3, g3, b1, g1
		r    = _mm_srli_si128(r, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15,r13,r11, r9, r7, r5, r3, r1
		b    = _mm_unpacklo_epi8(b,r);			//r15,b14,r13,b12,r11,b10, r9, b8, r7, b6, r5, b4, r3, b2, r1, b0

		r    = _mm_srli_si128(temp, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g14,r14,g12,r12,g10,r10, g8, r8
		temp = _mm_unpacklo_epi16(temp, r);		//g14,r14, g6, r6,g12,r12, g4, r4,g10,r10, g2, r2, g8, r8, g0, r0
		r    = _mm_srli_si128(temp, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g14,r14, g6, r6,g12,r12, g4, r4
		temp = _mm_unpacklo_epi16(temp, r);		//g14,g10,r14,r10, g6, g2, r6, r2,g12, g8,r12, r8, g4, g0, r4, r0

		r    = _mm_srli_si128(g, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,b15,g15,b13,g13,b11,g11, b9, g9
		g    = _mm_unpacklo_epi16(g, r);		//b15,g15, b7, g7,b13,g13, b5, g5,b11,g11, b3, g3, b9, g9, b1, g1
		r    = _mm_srli_si128(g, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,b15,g15, b7, g7,b13,g13, b5, g5
		g    = _mm_unpacklo_epi16(g, r);		//b15,g15,b11,g11, b7, g7, b3, g3,b13,g13, b9, g9, b5, g5, b1, g1

		r    = _mm_srli_si128(b, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15,b14,r13,b12,r11,b10, r9, b8
		b    = _mm_unpacklo_epi16(b, r);		//r15,b14, r7, b6,r13,b12, r5, b4,r11,b10, r3, b2, r9, b8, r1, b0
		r    = _mm_srli_si128(b, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,r15,b14, r7, b6,r13,b12, r5, b4
		b    = _mm_unpacklo_epi16(b, r);		//r15,b14,r11,b10, r7, b6, r3, b2,r13,b12, r9, b8, r5, b4, r1, b0

		r    = _mm_unpacklo_epi16(temp, g);		//b13,g13,g12,r12, b9, g9, g8, r8, b5, g5, g4, r4, b1, g1, g0, r0
		g    = _mm_unpackhi_epi16(b, g);		//b15,g15,r15,b14,b11,g11,r11,b10, b7, g7, r7, b6, b3, g3, r3, b2
		temp = _mm_srli_si128(temp, 8);			//  0,  0,  0,  0,  0,  0,  0,  0,g14,r14,g10,r10, g6, r6, g2, r2
		b    = _mm_unpacklo_epi16(b, temp);		//g14,r14,r13,b12,g10,r10, r9, b8, g6, r6, r5, b4, g2, r2, r1, b0

		temp = _mm_unpacklo_epi16(r, b);		// g6, r6, b5, g5, r5, b4, g4, r4, g2, r2, b1, g1, r1, b0, g0, r0
		b    = _mm_unpackhi_epi16(r, b);		//g14,r14,b13,g13,r13,b12,g12,r12,g10,r10, b9, g9, r9, b8, g8, r8

		r    = _mm_unpacklo_epi64(temp, b);		//g10,r10, b9, g9, r9, b8, g8, r8, g2, r2, b1, g1, r1, b0, g0, r0
		b    = _mm_unpackhi_epi32(g, b);		//g14,r14,b13,g13,b15,g15,r15,b14,r13,b12,g12,r12,b11,g11,r11,b10
		b    = _mm_shufflehi_epi16(b, _MM_SHUFFLE(1,0,3,2));//b15,g15,r15,b14,g14,r14,b13,g13,r13,b12,g12,r12,b11,g11,r11,b10

		temp = _mm_srli_si128(temp, 8);			//  0,  0,  0,  0,  0,  0,  0,  0, g6, r6, b5, g5, r5, b4, g4, r4
		temp = _mm_unpacklo_epi32(g, temp);		// g6, r6, b5, g5, b7, g7, r7, b6, r5, b4, g4, r4, b3, g3, r3, b2
		temp = _mm_shufflehi_epi16(temp, _MM_SHUFFLE(1,0,3,2));//b7,g7,r7,b6,g6,r6,b5,g5,r5,b4,g4,r4,b3,g3,r3,b2

		g    = _mm_unpackhi_epi64(temp, r);		//g10,r10, b9, g9, r9, b8, g8, r8, b7, g7, r7, b6, g6, r6, b5, g5
		r    = _mm_unpacklo_epi64(r, temp);		// r5, b4, g4, r4, b3, g3, r3, b2, g2, r2, b1, g1, r1, b0, g0, r0
}

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_16bit(__m128i &r,__m128i &g,__m128i &b)
{
		__m128i temp;

		temp = _mm_srli_si128(r, 8);			// 0, 0, 0, 0,r7,r6,r5,r4
		r    = _mm_unpacklo_epi16(r, temp);		//r7,r3,r6,r2,r5,r1,r4,r0
		temp = _mm_srli_si128(r, 8);			// 0, 0, 0, 0,r7,r3,r6,r2
		r    = _mm_unpacklo_epi16(r, temp);		//r7,r5,r3,r1,r6,r4,r2,r0

		temp = _mm_srli_si128(g, 8);			//g7,g3,g6,g2,g5,g1,g4,g0
		g    = _mm_unpacklo_epi16(g, temp);		// 0, 0, 0, 0,g7,g3,g6,g2
		temp = _mm_srli_si128(g, 8);			//g7,g5,g3,g1,g6,g4,g2,g0
		g    = _mm_unpacklo_epi16(g, temp);		//g7,g5,g3,g1,g6,g4,g2,g0

		temp = _mm_srli_si128(b, 8);			//b7,b3,b6,b2,b5,b1,b4,b0
		b    = _mm_unpacklo_epi16(b, temp);		// 0, 0, 0, 0,b7,b3,b6,b2
		temp = _mm_srli_si128(b, 8);			//b7,b5,b3,b1,b6,b4,b2,b0
		b    = _mm_unpacklo_epi16(b, temp);		//b7,b5,b3,b1,b6,b4,b2,b0

		temp = _mm_unpacklo_epi16(r, g);		//g6,r6,g4,r4,g2,r2,g0,r0
		r    = _mm_srli_si128(r , 8);			// 0, 0, 0, 0,r7,r5,r3,r1
		r    = _mm_unpacklo_epi16(b, r);		//r7,b6,r5,b4,r3,b2,r1,b0
		g    = _mm_unpackhi_epi16(g, b);		//b7,g7,b5,g5,b3,g3,b1,g1

		b    = _mm_srli_si128(r, 8);			// 0, 0, 0, 0,r7,b6,r5,b4
		r    = _mm_unpacklo_epi32(r, b);		//r7,b6,r3,b2,r5,b4,r1,b0
		b    = _mm_srli_si128(g, 8);			// 0, 0, 0, 0,b7,g7,b5,g5
		g    = _mm_unpacklo_epi32(g, b);		//b7,g7,b3,g3,b5,g5,b1,g1
		b    = _mm_srli_si128(temp, 8);			// 0, 0, 0, 0,g6,r6,g4,r4
		temp = _mm_unpacklo_epi32(temp, b);		//g6,r6,g2,r2,g4,r4,g0,r0

		b    = _mm_unpacklo_epi32(temp, g);		//b5,g5,g4,r4,b1,g1,g0,r0
		temp = _mm_srli_si128(temp, 8);			// 0, 0, 0, 0,g6,r6,g2,r2
		temp = _mm_unpacklo_epi32(r, temp);		//g6,r6,r5,b4,g2,r2,r1,b0
		g    = _mm_unpackhi_epi32(r, g);		//b7,g7,r7,b6,b3,g3,r3,b2
		
		r    = _mm_unpacklo_epi32(b, temp);		//g2,r2,b1,g1,r1,b0,g0,r0
		temp = _mm_unpackhi_epi32(b, temp);		//g6,r6,b5,g5,r5,b4,g4,b4
		b    = _mm_unpackhi_epi64(temp, g);		//b7,g7,r7,b6,g6,r6,b5,g5
		g    = _mm_unpacklo_epi64(g, temp);		//r5,b4,g4,r4,b3,g3,r3,b2
}

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_32bit(__m128i &r,__m128i &g,__m128i &b)
{
		__m128i temp;

		temp = _mm_srli_si128(r, 8);			// 0, 0,r3,r2
		r    = _mm_unpacklo_epi32(r, temp);		//r3,r1,r2,r0
		temp = _mm_srli_si128(g, 8);			// 0, 0,g3,g2
		g    = _mm_unpacklo_epi32(g, temp);		//g3,g1,g2,g0
		temp = _mm_srli_si128(b, 8);			// 0, 0,b3,b2
		b    = _mm_unpacklo_epi32(b, temp);		//b3,b1,b2,b0

		temp = _mm_unpacklo_epi32(r, g);		//g2,r2,g0,r0
		g    = _mm_unpackhi_epi32(g, b);		//b3,g3,b1,g1
		r    = _mm_srli_si128(r, 8);			// 0, 0,r3,r1
		b    = _mm_unpacklo_epi32(b, r);		//r3,b2,r1,b0

		r    = _mm_unpacklo_epi64(temp, b);		//r1,b0,g0,r0
		b    = _mm_unpackhi_epi64(b, g);		//b3,g3,r3,b2
		g    = _mm_slli_si128(g, 8);			//b1,g1, 0, 0
		g    = _mm_unpackhi_epi64(g, temp);		//g2,r2,b1,g1
}

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{ 
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_8bit( rgb1, rgb2, rgb3);
}

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_16bit( rgb1, rgb2, rgb3);
}

CBL_INLINE void CBL_SSE2::Convert_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, __m128i & rgb1, __m128i & rgb2, __m128i & rgb3 )
{
	rgb1 = r;
	rgb2 = g;
	rgb3 = b;
	Convert_3P_to_3C_32bit( rgb1, rgb2, rgb3);
}

namespace CBL_Library_Internal
{
    template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const void * pAlignedData )
    {
        __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;

		CBL_SSE2::Convert_3P_to_3C_8bit( temp_r, temp_g, temp_b);

        StoreClass::Store( pDst,   temp_r );    // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );    // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );    // store the third 16 bytes    
	}

    template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const void * pAlignedData )
    {
	    __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;

		CBL_SSE2::Convert_3P_to_3C_16bit( temp_r, temp_g, temp_b);

        StoreClass::Store( pDst,   temp_r );    // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );    // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );    // store the third 16 bytes  
    }

	template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const void * pAlignedData )
    {
	    __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;

		CBL_SSE2::Convert_3P_to_3C_32bit( temp_r, temp_g, temp_b);

        StoreClass::Store( pDst,   temp_r );    // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );    // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );    // store the third 16 bytes  
    }

} // namespace CBL_Library_Internal

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_8bit< CBL_AlignedStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_16bit< CBL_AlignedStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_32bit< CBL_AlignedStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
	CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_8bit< CBL_StreamingStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_16bit< CBL_StreamingStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_32bit< CBL_StreamingStore >( r, g, b, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_8bit< CBL_UnalignedStore >( r, g, b, pUnalignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_16bit< CBL_UnalignedStore >( r, g, b, pUnalignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, void * pUnalignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_3P_to_3C_32bit< CBL_UnalignedStore >( r, g, b, pUnalignedData );
}

// convert 4-channel RGBA to 4-planar format

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_8bit(__m128i &rgba1,__m128i &rgba2,__m128i &rgba3,__m128i &rgba4)
{
		__m128i temp1,temp2;

		temp1 = _mm_unpacklo_epi8(rgba1, rgba3);			// a9, a1, b9, b1, g9, g1, r9, r1, a8, a0, b8, b0, g8, g0, r8, r0
		rgba1 = _mm_unpackhi_epi8(rgba1, rgba3);			//a11, a3,b11, b3,g11, g3,r11, r3,a10, a2,b10, b2,g10, g2,r10, r2
		rgba3 = _mm_unpacklo_epi8(rgba2, rgba4);			//a13, a5,b13, b5,g13, g5,r13, r5,a12, a4,b12, b4,g12, g4,r12, r4
		temp2 = _mm_unpackhi_epi8(rgba2, rgba4);			//a15, a7,b15, b7,g15, g7,r15, r7,a14, a6,b14, b6,g14, g6,r14, r6

		rgba4 = _mm_unpackhi_epi8(rgba1, temp2);			//a15,a11, a7, a3,b15,b11, b7, b3,g15,g11, g7, g3,r15,r11, r7, r3
		rgba1 = _mm_unpacklo_epi8(rgba1, temp2);			//a14,a10, a6, a2,b14,b10, b6, b2,g14,g10, g6, g2,r14,r10, r6, r2
		rgba2 = _mm_unpacklo_epi8(temp1, rgba3);			//a12, a8, a4, a0,b12, b8, b4, b0,g12, g8, g4, g0,r12, r8, r4, r0
		rgba3 = _mm_unpackhi_epi8(temp1, rgba3);			//a13, a9, a5, a1,b13, b9, b5, b1,g13, g9, g5, g1,r13, r9, r5, r1

		temp1 = _mm_unpacklo_epi8(rgba3, rgba4);			//g15,g13,g11, g9, g7, g5, g3, g1,r15,r13,r11, r9, r7, r5, r3, r1
		rgba3 = _mm_unpackhi_epi8(rgba3, rgba4);			//a15,a13,a11, a9, a7, a5, a3, a1,b15,b13,b11, b9, b7, b5, b3, b1
		temp2 = _mm_unpackhi_epi8(rgba2, rgba1);			//a14,a12,a10, a8, a6, a4, a2, a0,b14,b12,b10, b8, b6, b4, b2, b0
		rgba2 = _mm_unpacklo_epi8(rgba2, rgba1);			//g14,g12,g10, g8, g6, g4, g2, g0,r14,r12,r10, r8, r6, r4, r2, r0

		rgba1 = _mm_unpacklo_epi8(rgba2, temp1);			//r15,r14,r13,r12,r11,r10, r9, r8, r7, r6, r5, r4, r3, r2, r1, r0
		rgba2 = _mm_unpackhi_epi8(rgba2, temp1);			//g15,g14,g13,g12,g11,g10, g9, g8, g7, g6, g5, g4, g3, g2, g1, g0
		rgba4 = _mm_unpackhi_epi8(temp2, rgba3);			//a15,a14,a13,a12,a11,a10, a9, a8, a7, a6, a5, a4, a3, a2, a1, a0
		rgba3 = _mm_unpacklo_epi8(temp2, rgba3);			//b15,b14,b13,b12,b11,b10, b9, b8, b7, b6, b5, b4, b3, b2, b1, b0
}

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_16bit(__m128i &rgba1,__m128i &rgba2,__m128i &rgba3,__m128i &rgba4)
{
		__m128i temp1, temp2;

		temp1  = _mm_unpacklo_epi16(rgba1, rgba3);			//a4,a0,b4,b0,g4,g0,r4,r0
		rgba1  = _mm_unpackhi_epi16(rgba1, rgba3);			//a5,a1,b5,b1,g5,g1,r5,r1
		rgba3  = _mm_unpacklo_epi16(rgba2, rgba4);			//a6,a2,b6,b2,g6,g2,r6,r2
		rgba2  = _mm_unpackhi_epi16(rgba2, rgba4);			//a7,a3,b7,b3,g7,g3,r7,r3

		rgba4  = _mm_unpackhi_epi16(rgba1, rgba2);			//a7,a5,a3,a1,b7,b5,b3,b1
		rgba1  = _mm_unpacklo_epi16(rgba1, rgba2);			//g7,g5,g3,g1,r7,r5,r3,r1
		temp2  = _mm_unpacklo_epi16(temp1, rgba3);			//g6,g4,g2,g0,r6,r4,r2,r0
		temp1  = _mm_unpackhi_epi16(temp1, rgba3);			//a6,a4,a2,a0,b6,b4,b2,b0

		rgba3  = _mm_unpacklo_epi16(temp1, rgba4);			//b7,b6,b5,b4,b3,b2,b1,b0
		rgba4  = _mm_unpackhi_epi16(temp1, rgba4);			//a7,a6,a5,a4,a3,a2,a1,a0
		rgba2  = _mm_unpackhi_epi16(temp2, rgba1);			//g7,g6,g5,g4,g3,g2,g1,g0
		rgba1  = _mm_unpacklo_epi16(temp2, rgba1);			//r7,r6,r5,r4,r3,r2,r1,r0
}

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_32bit(__m128i &rgba1,__m128i &rgba2,__m128i &rgba3, __m128i &rgba4)
{
		__m128i temp1, temp2;

		temp1  = _mm_unpacklo_epi32(rgba1, rgba3);			//g2,g0,r2,r0
		rgba1  = _mm_unpackhi_epi32(rgba1, rgba3);			//a2,a0,b2,b0
		temp2  = _mm_unpacklo_epi32(rgba2, rgba4);			//g3,g1,r3,r1
		rgba2  = _mm_unpackhi_epi32(rgba2, rgba4);			//a3,a1,b3,b1

		rgba4  = _mm_unpackhi_epi32(rgba1, rgba2);			//a3,a2,a1,a0
		rgba3  = _mm_unpacklo_epi32(rgba1, rgba2);			//b3,b2,b1,b0
		rgba1  = _mm_unpacklo_epi32(temp1, temp2);			//r3,r2,r1,r0
		rgba2  = _mm_unpackhi_epi32(temp1, temp2);			//g3,g2,g1,g0
}

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_8bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_8bit( r, g, b, a);
}

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_16bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_16bit( r, g, b, a);
}

CBL_INLINE void CBL_SSE2::Convert_4C_to_4P_32bit( const __m128i & rgba1, const __m128i & rgba2, const __m128i & rgba3, const __m128i & rgba4, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
	r = rgba1;
	g = rgba2;
	b = rgba3;
	a = rgba4;
	Convert_4C_to_4P_32bit( r, g, b, a);
}

namespace CBL_Library_Internal
{
    template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		LoadClass::Load( a, pSrc+3 );		// read the fourth 16 bytes

		CBL_SSE2::Convert_4C_to_4P_8bit( r, g, b, a);
    }

	template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		LoadClass::Load( a, pSrc+3 );		// read the fourth 16 bytes

		CBL_SSE2::Convert_4C_to_4P_16bit( r, g, b, a);
    }

	template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
    {
		__m128i * pSrc = (__m128i *)pAlignedData;

        LoadClass::Load( r, pSrc );			// read the first 16 bytes
        LoadClass::Load( g, pSrc+1 );		// read the second 16 bytes
        LoadClass::Load( b, pSrc+2 );		// read the third 16 bytes
		LoadClass::Load( a, pSrc+3 );		// read the fourth 16 bytes

		CBL_SSE2::Convert_4C_to_4P_32bit( r, g, b, a);
    }

} // namespace CBL_Library_Internal

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_4C_to_4P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_8bit< CBL_AlignedLoad >( pAlignedData, r, g, b, a );
}

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_4C_to_4P_16bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_16bit< CBL_AlignedLoad >( pAlignedData, r, g, b, a );
}

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_4C_to_4P_32bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_32bit< CBL_AlignedLoad >( pAlignedData, r, g, b, a );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_4C_to_4P_8bit( const void * pUnAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_8bit< CBL_UnalignedLoad >( pUnAlignedData, r, g, b, a );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_4C_to_4P_16bit( const void * pUnAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_16bit< CBL_UnalignedLoad >( pUnAlignedData, r, g, b, a );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_4C_to_4P_32bit( const void * pUnAlignedData, __m128i & r, __m128i & g, __m128i & b, __m128i & a )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_4P_32bit< CBL_UnalignedLoad >( pUnAlignedData, r, g, b, a );
}

// convert 4-planar RGBA to 4-channel format

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_8bit(__m128i &r,__m128i &g,__m128i &b,__m128i &a)
{
		__m128i temp1, temp2;

		temp1 = _mm_unpacklo_epi8(r, b);			// b7, r7, b6, r6, b5, r5, b4, r4, b3, r3, b2, r2, b1, r1, b0, r0
		r     = _mm_unpackhi_epi8(r, b);            //b15,r15,b14,r14,b13,r13,b12,r12,b11,r11,b10,r10, b9, r9, b8, r8
		temp2 = _mm_unpacklo_epi8(g, a);			// a7, g7, a6, g6, a5, g5, a4, g4, a3, g3, a2, g2, a1, g1, a0, g0
		g     = _mm_unpackhi_epi8(g, a);			//a15,g15,a14,g14,a13,g13,a12,g12,a11,g11,a10,g10, a9, g9, a8, g8

		b     = _mm_unpacklo_epi8(r, g);			//a11,b11,g11,r11,a10,b10,g10,r10, a9, b9, g9, r9, a8, b8, g8, r8
		a     = _mm_unpackhi_epi8(r, g);			//a16,b16,g16,r16,a15,b15,g15,r15,a14, b1,g14,r14,a13,b13,g12,r12
		r     = _mm_unpacklo_epi8(temp1, temp2);	// a3, b3, g3, r3, a2, b2, g2, r2, a1, b1, g1, r1, a0, b0, g0, r0
		g     = _mm_unpackhi_epi8(temp1, temp2);	// a7, b7, g7, r7, a6, b6, g6, r6, a5, b5, g5, r5, a4, b4, g4, r4
}

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_16bit(__m128i &r,__m128i &g,__m128i &b,__m128i &a)
{
		__m128i temp1, temp2;

		temp1 = _mm_unpacklo_epi16(r, b);			//b3,r3,b2,r2,b1,r1,b0,r0
		r     = _mm_unpackhi_epi16(r, b);           //b7,r7,b6,r6,b5,r5,b4,r4
		temp2 = _mm_unpacklo_epi16(g, a);			//a3,g3,a2,g2,a1,g1,a0,g0
		g     = _mm_unpackhi_epi16(g, a);			//a7,g7,a6,g6,a5,g5,a4,g4

		b     = _mm_unpacklo_epi16(r, g);			//a5,b5,g5,r5,a4,b4,g4,r4
		a     = _mm_unpackhi_epi16(r, g);			//a7,b7,g7,r7,a6,b6,g6,r6
		r     = _mm_unpacklo_epi16(temp1, temp2);	//a1,b1,g1,r1,a0,b0,g0,r0
		g     = _mm_unpackhi_epi16(temp1, temp2);	//a3,b3,g3,r3,a2,b2,g2,r2
}

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_32bit(__m128i &r,__m128i &g,__m128i &b,__m128i &a)
{
		__m128i temp1, temp2;

		temp1 = _mm_unpacklo_epi32(r, b);			//b1,r1,b0,r0
		r     = _mm_unpackhi_epi32(r, b);           //b3,r3,b2,r2
		temp2 = _mm_unpacklo_epi32(g, a);			//a1,g1,a0,g0
		g     = _mm_unpackhi_epi32(g, a);			//a3,g3,a2,g2

		b     = _mm_unpacklo_epi32(r, g);			//a2,b2,g2,r2
		a     = _mm_unpackhi_epi32(r, g);			//a3,b3,g3,r3
		r     = _mm_unpacklo_epi32(temp1, temp2);	//a0,b0,g0,r0
		g     = _mm_unpackhi_epi32(temp1, temp2);	//a1,b1,g1,r1
}

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_8bit( rgba1, rgba2, rgba3, rgba4);
}

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_16bit( rgba1, rgba2, rgba3, rgba4);
}

CBL_INLINE void CBL_SSE2::Convert_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, __m128i & rgba1, __m128i & rgba2, __m128i & rgba3, __m128i & rgba4 )
{
	rgba1 = r;
	rgba2 = g;
	rgba3 = b;
	rgba4 = a;
	Convert_4P_to_4C_32bit( rgba1, rgba2, rgba3, rgba4);
}

namespace CBL_Library_Internal
{
    template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    {
        __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;
		__m128i temp_a = a;

		CBL_SSE2::Convert_4P_to_4C_8bit( temp_r, temp_g, temp_b,temp_a);

        StoreClass::Store( pDst,   temp_r );        // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );        // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );        // store the third 16 bytes  
		StoreClass::Store( pDst+3, temp_a );        // store the fourth 16 bytes      
	}

    template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    {
        __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;
		__m128i temp_a = a;

		CBL_SSE2::Convert_4P_to_4C_16bit( temp_r, temp_g, temp_b,temp_a);

        StoreClass::Store( pDst,   temp_r );        // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );        // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );        // store the third 16 bytes  
		StoreClass::Store( pDst+3, temp_a );        // store the fourth 16 bytes 
	}
    template< typename StoreClass >
    CBL_INLINE void ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
    {
        __m128i * pDst = (__m128i *)pAlignedData;

		__m128i temp_r = r;
		__m128i temp_g = g;
		__m128i temp_b = b;
		__m128i temp_a = a;

		CBL_SSE2::Convert_4P_to_4C_32bit( temp_r, temp_g, temp_b,temp_a);

        StoreClass::Store( pDst,   temp_r );        // store the first 16 bytes
        StoreClass::Store( pDst+1, temp_g );        // store the second 16 bytes
        StoreClass::Store( pDst+2, temp_b );        // store the third 16 bytes  
		StoreClass::Store( pDst+3, temp_a );        // store the fourth 16 bytes 
	}

} // namespace CBL_Library_Internal

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_8bit< CBL_AlignedStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_16bit< CBL_AlignedStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_32bit< CBL_AlignedStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_8bit< CBL_StreamingStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_16bit< CBL_StreamingStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStream_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_32bit< CBL_StreamingStore >( r, g, b, a, pAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_8bit< CBL_UnalignedStore >( r, g, b, a, pUnAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_16bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_16bit< CBL_UnalignedStore >( r, g, b, a, pUnAlignedData );
}

CBL_INLINE void CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_32bit( const __m128i & r, const __m128i & g, const __m128i & b, const __m128i & a, void * pUnAlignedData )
{
    CBL_Library_Internal::ConvertAndStoreAligned_4P_to_4C_32bit< CBL_UnalignedStore >( r, g, b, a, pUnAlignedData );
}

namespace CBL_Library_Internal
{
    template< typename LoadClass >
    CBL_INLINE void LoadAlignedAndConvert_4C_to_3P_8bit( const void * pData, __m128i & r, __m128i & g, __m128i & b )
    {
	    __m128i s, t;
        static const __m128i cMask  = CONST_SET_32I( 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF );
        static const __m128i cZero  = CONST_SETZERO_8I();
        __m128i * pSrc = (__m128i*)pData;

    	
	    // read the first 16 bytes
        LoadClass::Load( r, pSrc );                                 // r = { R0,G0,B0,A0,R1,G1,B1,A1,R2,G2,B2,A2,R3,G3,B3,A3 }

	    //////////////////////////////////////////////////////////
        // extract BLUE components from the FIRST 16 bytes
        //////////////////////////////////////////////////////////
	    b = _mm_srli_si128		( r, 2 );						    // b = { B0,A0,R1,G1,B1,A1,R2,G2,B2,A2,R3,G3,B3,A3,__,__ }
	    b = _mm_and_si128		( b, cMask );					    // b = { B0,__,__,__,B1,__,__,__,B2,__,__,__,B3,__,__,__ }
        b = _mm_shufflelo_epi16	( b, _MM_SHUFFLE( 2, 0, 3, 1 ) );   // b = { __,__,__,__,B0,__,B1,__,B2,__,__,__,B3,__,__,__ }
        b = _mm_shufflehi_epi16	( b, _MM_SHUFFLE( 3, 1, 2, 0 ) );   // b = { __,__,__,__,B0,__,B1,__,B2,__,B3,__,__,__,__,__ }
        b = _mm_srli_si128		( b, 4 );						    // b = ( B0,__,B1,__,B2,__,B3,__,__,__,__,__,__,__,__,__ }
        b = _mm_packus_epi16	( b, cZero );					    // b = ( B0,B1,B2,B3,B4,__,__,__,__,__,__,__,__,__,__,__ }

	    //////////////////////////////////////////////////////////
        // extract GREEN components from the FIRST 16 bytes
        //////////////////////////////////////////////////////////
	    g = _mm_srli_si128		( r, 1 );						    // g = { G0,B0,A1,R1,G1,B1,A2,R2,G2,B2,A3,R3,G3,B3,A0,__ }
	    g = _mm_and_si128			( g, cMask );					// g = { G0,__,__,__,G1,__,__,__,G2,__,__,__,G3,__,__,__ }
        g = _mm_shufflelo_epi16	( g, _MM_SHUFFLE( 2, 0, 3, 1) );    // g = { __,__,__,__,G0,__,G1,__,G2,__,__,__,G3,__,__,__ }
        g = _mm_shufflehi_epi16	( g, _MM_SHUFFLE( 3, 1, 2, 0) );    // g = { __,__,__,__,G0,__,G1,__,G2,__,G3,__,__,__,__,__ }
        g = _mm_srli_si128		( g, 4 );						    // g = ( G0,__,G1,__,G2,__,G3,__,__,__,__,__,__,__,__,__ }
        g = _mm_packus_epi16		( g, cZero );					// g = ( G0,G1,G2,G3,G4,__,__,__,__,__,__,__,__,__,__,__ }

	    //////////////////////////////////////////////////////////
        // extract RED components from the FIRST 16 bytes
        //////////////////////////////////////////////////////////
	    r = _mm_and_si128			( r, cMask );					// r = { R0,__,__,__,R1,__,__,__,R2,__,__,__,R3,__,__,__ }
        r = _mm_shufflelo_epi16	( r, _MM_SHUFFLE( 2, 0, 3, 1) );	// r = { __,__,__,__,R0,__,R1,__,R2,__,__,__,R3,__,__,__ }
        r = _mm_shufflehi_epi16	( r, _MM_SHUFFLE( 3, 1, 2, 0) );	// r = { __,__,__,__,R0,__,R1,__,R2,__,R3,__,__,__,__,__ }
        r = _mm_srli_si128		( r, 4 );						    // r = ( R0,__,R1,__,R2,__,R3,__,__,__,__,__,__,__,__,__ }
        r = _mm_packus_epi16		( r, cZero );					// r = ( R0,R1,R2,R3,__,__,__,__,__,__,__,__,__,__,__,__ }

	    // read the second 16 bytes
        LoadClass::Load( s, pSrc+1 );                               // r = { R4,G4,B4,A4,R5,G5,B5,A5,R6,G6,B6,A6,R7,G7,B7,A7 }

	    //////////////////////////////////////////////////////////
        // extract RED components from the SECOND 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_and_si128			( s, cMask );					// r = { R4,__,__,__,R5,__,__,__,R6,__,__,__,R7,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// r = { __,__,__,__,R4,__,R5,__,R6,__,__,__,R7,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// r = { __,__,__,__,R4,__,R5,__,R6,__,R7,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // r = ( __,__,__,__,__,__,__,__,R4,__,R5,__,R6,__,R7,__ )
	    t = _mm_packus_epi16		( t, cZero );					// r = ( __,__,__,__,R4,R5,R6,R7,__,__,__,__,__,__,__,__ }
	    r = _mm_or_si128			( r, t );					    // r = ( R0,R1,R2,R3,R4,R5,R6,R7,__,__,__,__,__,__,__,__ }

	    //////////////////////////////////////////////////////////
        // extract GREEN components from the SECOND 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 1 );						    // t = { G4,B4,A4,R5,G5,B5,A5,R6,G6,B6,A6,R7,G7,B7,A7,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { G4,__,__,__,G5,__,__,__,G6,__,__,__,G7,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,G4,__,G5,__,G6,__,__,__,G7,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,G4,__,G5,__,G6,__,G7,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // t = ( __,__,__,__,__,__,__,__,G4,__,G5,__,G6,__,G7,__ )
	    t = _mm_packus_epi16		( t, cZero );					// t = ( __,__,__,__,G4,G5,G6,G7,__,__,__,__,__,__,__,__ }
	    g = _mm_or_si128			( g, t );					    // g = ( G0,G1,G2,G3,G4,G5,G6,G7,__,__,__,__,__,__,__,__ }

	    /////////////////////////////////////////////////////////
        // extract BLUE components from the SECOND 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 2 );						    // t = { B4,A4,R5,G5,B5,A5,R6,G6,B6,A6,R7,G7,B7,A7,__,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { G4,__,__,__,G5,__,__,__,G6,__,__,__,G7,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,G4,__,G5,__,G6,__,__,__,G7,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,G4,__,G5,__,G6,__,G7,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // t = ( __,__,__,__,__,__,__,__,G4,__,G5,__,G6,__,G7,__ )
	    t = _mm_packus_epi16		( t, cZero );					// t = ( __,__,__,__,G4,G5,G6,G7,__,__,__,__,__,__,__,__ }
	    b = _mm_or_si128			( b, t );					    // g = ( G0,G1,G2,G3,G4,G5,G6,G7,__,__,__,__,__,__,__,__ }
    	
	    // read the THIRD 16 bytes
        LoadClass::Load( s, pSrc+2 );                               // r = { R8,G8,B8,A8,R9,G9,B9,A9,Ra,Ga,Ba,Aa,Rb,Gb,Bb,Ab }

	    //////////////////////////////////////////////////////////
        // extract RED components from the THIRD 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_and_si128			( s, cMask );					// t = { R8,__,__,__,R9,__,__,__,Ra,__,__,__,Rb,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,R8,__,R9,__,Ra,__,__,__,Rb,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,R8,__,R9,__,Ra,__,Rb,__,__,__,__,__ }
	    t = _mm_srli_si128		( t, 4 );						    // t = ( R8,__,R9,__,Ra,__,Rb,__,__,__,__,__,__,__,__,__ )
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,R8,R9,Ra,Rb,__,__,__,__ }
	    r = _mm_or_si128			( r, t );					    // r = ( R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,Ra,Rb,__,__,__,__ }

	    //////////////////////////////////////////////////////////
        // extract GREEN components from the THIRD 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 1 );						    // t = { G8,B8,A8,R9,G9,B9,A9,Ra,Ga,Ba,Ab,Rb,Gb,Bb,Ab,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { G8,__,__,__,G9,__,__,__,Ga,__,__,__,Gb,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,G8,__,G9,__,Ga,__,__,__,Gb,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,G8,__,G9,__,Ga,__,Gb,__,__,__,__,__ }
	    t = _mm_srli_si128		( t, 4 );						    // t = ( G8,__,G9,__,Ga,__,Gb,__,__,__,__,__,__,__,__,__ )
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,G8,G9,Ga,Gb,__,__,__,__ }
	    g = _mm_or_si128			( g, t );					    // r = ( G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,Ga,Gb,__,__,__,__ }

	    //////////////////////////////////////////////////////////
        // extract BLUE components from the THIRD 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 2 );						    // t = { B8,A8,B9,G9,B9,A9,Ba,Ga,Ba,Aa,Bb,Gb,Bb,Ab,__,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { B8,__,__,__,B9,__,__,__,Ba,__,__,__,Bb,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,B8,__,B9,__,Ba,__,__,__,Bb,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,B8,__,B9,__,Ba,__,Bb,__,__,__,__,__ }
	    t = _mm_srli_si128		( t, 4 );						    // t = ( B8,__,B9,__,Ba,__,Bb,__,__,__,__,__,__,__,__,__ )
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,B8,B9,Ba,Bb,__,__,__,__ }
	    b = _mm_or_si128			( b, t );					    // r = ( B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,Ba,Bb,__,__,__,__ }


	    // read the FOURTH 16 bytes
	    LoadClass::Load( s, pSrc+3 );				                // r = { R8,G8,B8,A8,R9,G9,B9,A9,Ra,Ga,Ba,Aa,Rb,Gb,Bb,Ab }

	    //////////////////////////////////////////////////////////
        // extract RED components from the FOURTH 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_and_si128			( s, cMask );					// t = { Rc,__,__,__,Rd,__,__,__,Re,__,__,__,Rf,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,Rc,__,Rd,__,Re,__,__,__,Rf,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,Rc,__,Rd,__,Re,__,Rf,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // t = ( __,__,__,__,__,__,__,__,__,Rc,__,Rd,__,Re,__,Rf }
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,__,__,__,__,Rc,Rd,Re,Rf }
	    r = _mm_or_si128			( r, t );					    // g = ( R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,Ra,Rb,Rc,Rd,Re,Rf }

	    //////////////////////////////////////////////////////////
        // extract GREEN components from the FOURTH 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 1 );						    // t = { Gc,Bc,Ac,Bd,Gd,Bd,Ad,Be,Ge,Be,Ae,Bf,Gf,Bf,Af,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { Gc,__,__,__,Gd,__,__,__,Ge,__,__,__,Gf,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,Gc,__,Gd,__,Ge,__,__,__,Gf,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,Gc,__,Gd,__,Ge,__,Gf,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // t = ( __,__,__,__,__,__,__,__,__,Gc,__,Gd,__,Ge,__,Gf }
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,__,__,__,__,Gc,Gd,Ge,Gf }
	    g = _mm_or_si128			( g, t );					    // g = ( G0,G1,G2,G3,G4,G5,G6,G7,G8,G9,Ga,Gb,Gc,Gd,Ge,Gf }
    	
	    //////////////////////////////////////////////////////////
        // extract BLUE components from the FOURTH 16 bytes
        //////////////////////////////////////////////////////////
	    t = _mm_srli_si128		( s, 2 );						    // t = { Bc,Ac,Bd,Gd,Bd,Ad,Be,Ge,Be,Ae,Bf,Gf,Bf,Af,__,__ }
	    t = _mm_and_si128			( t, cMask );					// t = { Bc,__,__,__,Bd,__,__,__,Be,__,__,__,Bf,__,__,__ }
        t = _mm_shufflelo_epi16	( t, _MM_SHUFFLE( 2, 0, 3, 1) );	// t = { __,__,__,__,Bc,__,Bd,__,Be,__,__,__,Bf,__,__,__ }
        t = _mm_shufflehi_epi16	( t, _MM_SHUFFLE( 3, 1, 2, 0) );	// t = { __,__,__,__,Bc,__,Bd,__,Be,__,Bf,__,__,__,__,__ }
	    t = _mm_slli_si128		( t, 4 );						    // t = ( __,__,__,__,__,__,__,__,__,Bc,__,Bd,__,Be,__,Bf }
	    t = _mm_packus_epi16		( cZero, t );					// t = ( __,__,__,__,__,__,__,__,__,__,__,__,Bc,Bd,Be,Bf }
	    b = _mm_or_si128			( b, t );					    // g = ( B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,Ba,Bb,Bc,Bd,Be,Bf }	
    }
} // namespace CBL_Library_Internal

CBL_INLINE void CBL_SSE2::LoadAlignedAndConvert_4C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_3P_8bit< CBL_AlignedLoad >( pAlignedData, r, g, b );
}

CBL_INLINE void CBL_SSE2::LoadUnalignedAndConvert_4C_to_3P_8bit( const void * pAlignedData, __m128i & r, __m128i & g, __m128i & b )
{
    CBL_Library_Internal::LoadAlignedAndConvert_4C_to_3P_8bit< CBL_UnalignedLoad >( pAlignedData, r, g, b );
}

/// @endcond

#pragma warning ( pop )

#endif // __CBL_TRANSPOSE_PRIMITIVES_SSE2_H__
