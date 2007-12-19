/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*******************************************************/
#include "fwdev.h"
#include "FwSharedCode.h"
#include "FwSharedCode_SSE2.h"
#include "AANIDCT.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

/******************************************************************/
#define PRESHIFT 4	// left-shift input coefficient amount
#define ROW_STRIDE 16	// for 8x8 matrix transpose operation

/*******************************************************/
SYS_FORCEALIGN_16 static Fw16s My_idct_weighting[] = {
	16384,	21407,	16384,	8867, 16384,	-8867,  16384, -21407,	//	w05 w04 w01 w00	w13 w12 w09 w08
	16384,	8867,	-16384,	-21407,	-16384,	21407,  16384,  -8867,	//	w07 w06 w03 w02	w15 w14 w11 w10
	22725,	19266,  19266,  -4520, 	12873,	-22725,	4520,	-12873,	//	w21 w20 w17 w16 w29 w28 w25 w24
	12873,	4520,	-22725, -12873, 4520,	19266,  19266,	-22725, //	w23 w22 w19 w18 w31 w30 w27 w26

	22725,  29692,  22725,  12299, 22725,	-12299, 22725,	-29692,
	22725,  12299,	-22725, -29692,	-22725,	29692,  22725,	-12299,
	31521,  26722,  26722,  -6270, 17855,	-31521, 6270,	-17855,
	17855,  6270,	-31521, -17855,	6270,	26722,  26722,	-31521,

	21407,  27969,  21407,  11585,  21407,	-11585, 21407,	-27969,
	21407,  11585,	-21407, -27969,	-21407, 27969,  21407,	-11585,
	29692,  25172,  25172,  -5906,  16819,	-29692, 5906,	-16819,
	16819,  5906,	-29692, -16819,	5906,	25172,  25172,	-29692,

	19266,  25172,  19266,  10426, 19266,	-10426, 19266,	-25172,	
	19266,  10426,	-19266, -25172,	-19266, 25172,  19266,	-10426,	
	26722,  22654,  22654,  -5315,  15137,	-26722, 5315,	-15137,	
	15137,  5315,	-26722, -15137, 5315,	22654,  22654,	-26722,

	13036,  13036,  13036,  13036,  13036,  13036,  13036,  13036, 
    27146,  27146,  27146,  27146,  27146,  27146,  27146,  27146,
	-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746, // tg * (2<<16) + 0.5
    -19195, -19195, -19195, -19195,-19195, -19195, -19195, -19195}; //cos * (2<<16) + 0.5

const static Fw16s preSC[] = {
		16384, 22725, 21407, 19266,  16384, 12873, 8867,  4520,
        22725, 31521, 29692, 26722,  22725, 17855, 12299, 6270,
        21407, 29692, 27969, 25172,  21407, 16819, 11585, 5906,
        19266, 26722, 25172, 22654,  19266, 15137, 10426, 5315,
        16384, 22725, 21407, 19266,  16384, 12873, 8867,  4520,
        12873, 17855, 16819, 15137,  25746, 20228, 13933, 7103,
        17734, 24598, 23170, 20853,  17734, 13933, 9597,  4892,
        18081, 25080, 23624, 21261,  18081, 14206, 9785,  4988,
	};

SYS_FORCEALIGN_16 static Fw16s tempIdctOutputSSE2[NUM_ROWS][NUM_COLS];
SYS_FORCEALIGN_16 static Fw16s tempUVIdctOutputSSE2[NUM_ROWS][NUM_COLS];

/*******************************************************/
//Start internal function for DCT
#define PI 3.14159265358979323846
//static float c[8][8]; /* transform coefficients */
static void init_idct(float c[8][8])
{
	int i, j;
	float s;

	for (i=0; i<8; i++) {
		s = (float)((i==0) ? sqrt(0.125) : 0.5);

		for (j=0; j<8; j++)
			c[i][j] = s * (float)(cos((PI/8.0)*i*(j+0.5)));
	}

}

static void idct(float c[8][8], const Fw16s* pSrc, Fw16s* pDst)
{
	int i, j, k;
	float partial_product;
	float tmp[64];

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
		{
			partial_product = 0.0;

			for (k=0; k<8; k++)
				partial_product+= c[k][j]*pSrc[8*i+k];

			tmp[8*i+j] = partial_product;
		}

		/* Transpose operation is integrated into address mapping by switching 
		loop order of i and j */

		for (j=0; j<8; j++)
			for (i=0; i<8; i++)
			{
				partial_product = 0.0;

				for (k=0; k<8; k++)
					partial_product+= c[k][i]*tmp[8*k+j];

				//v = CBL_LIBRARY::Limits<int>::Sat(floor(partial_product+0.5));
				//pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(v);
				pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(floor(partial_product+0.5));
			}
}

/***************************************************************************/

template<typename T>
static void transposeBlock(T* dst, Pointer8x8Block16s src, Fw32s dstStep);
template<typename T>
static void transposeBlock(T* dst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep);
static void transposeBlockOptimized_16s_C1R(Fw16s *pDst, Pointer8x8Block16s pSrc, Fw32s dstStep = NUM_COLS*sizeof(Fw16s));
static void transposeBlockOptimized_16s8u_C1R(Fw8u *pDst, Pointer8x8Block16s pSrc, Fw32s dstStep = NUM_COLS*sizeof(Fw8u));
static void transposeBlockOptimized_16s_P2C2R(Fw16s* dst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep=2*NUM_COLS*sizeof(Fw16s));
static void transposeBlockOptimized_16s8u_P2C2R(Fw8u* dst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep=2*NUM_COLS*sizeof(Fw8u));

/*******************************************************/
/**
 * performs AAN IDCT using SSE2
 *
 * Assumes destination is aligned on a 16-byte boundary
 */
static FwStatus My_idct_SSE2(const Fw16s* pSrc, Fw16s* pDst, Fw32s count)
{
	count = count;//in the future we will use count. For the time being make the compiler happy
	__m128i xmm0,  xmm1,  xmm2,  xmm3,  xmm4,  xmm5,  xmm6,  xmm7;
	__m128i xmm8,  xmm9,  xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
	Fw16s *peax = (Fw16s*)pSrc;
	Fw16s *pedx = pDst;
	Fw16s *pesi, *pecx;

	pesi = My_idct_weighting;
	pecx = My_idct_weighting+64;

	const bool bSrcIsAligned = FW_REF::IsAligned(peax, NUM_COLS*sizeof(Fw16s));
	if(bSrcIsAligned)
	{
		xmm0 = _mm_load_si128((__m128i *)(peax)); 
		xmm4 = _mm_load_si128((__m128i *)(peax+16)); 
		xmm8 = _mm_load_si128((__m128i *)(peax+32)); 
		xmm12 = _mm_load_si128((__m128i *)(peax+48)); 
	}
	else
	{
		xmm0 = _mm_loadu_si128((__m128i *)(peax)); 
		xmm4 = _mm_loadu_si128((__m128i *)(peax+16)); 
		xmm8 = _mm_loadu_si128((__m128i *)(peax+32)); 
		xmm12 = _mm_loadu_si128((__m128i *)(peax+48)); 
	}

//	DCT_8_INV_ROW	//Row 1, tab_i_04 and Row 3, tab_i_26
	xmm0 = _mm_shufflelo_epi16(xmm0, 0xD8);
//	DCT_8_INV_ROW	//Row 5, tab_i_04 and Row 7, tab_i_26
	xmm8 = _mm_shufflelo_epi16(xmm8, 0xD8);

	xmm0 = _mm_shufflehi_epi16 (xmm0, 0xD8);
	xmm8 = _mm_shufflehi_epi16 (xmm8, 0xD8);
	xmm3 = _mm_shuffle_epi32 (xmm0, 0x55) ;
	xmm11 = _mm_shuffle_epi32 (xmm8, 0x55) ;
	xmm1 = _mm_shuffle_epi32 (xmm0, 0) ;
	xmm9 = _mm_shuffle_epi32 (xmm8, 0) ;
	xmm2 = _mm_shuffle_epi32 (xmm0, 0xAA) ;
	xmm10 = _mm_shuffle_epi32 (xmm8, 0xAA) ;
	xmm0 = _mm_shuffle_epi32 (xmm0, 0xFF) ;
	xmm8 = _mm_shuffle_epi32 (xmm8, 0xFF) ;
	xmm1 = _mm_madd_epi16(xmm1, _mm_load_si128((__m128i *)(pesi)));
	xmm9 = _mm_madd_epi16(xmm9, _mm_load_si128((__m128i *)(pesi)));
	xmm2 = _mm_madd_epi16(xmm2, _mm_load_si128((__m128i *)(pesi+8)));
	xmm10 = _mm_madd_epi16(xmm10, _mm_load_si128((__m128i *)(pesi+8)));
	xmm3 = _mm_madd_epi16(xmm3, _mm_load_si128((__m128i *)(pesi+16)));
	xmm11 = _mm_madd_epi16(xmm11, _mm_load_si128((__m128i *)(pesi+16)));
	xmm0 = _mm_madd_epi16(xmm0, _mm_load_si128((__m128i *)(pesi+24)));
	xmm8 = _mm_madd_epi16(xmm8, _mm_load_si128((__m128i *)(pesi+24)));

	xmm0 = _mm_add_epi32(xmm0, xmm3);
	xmm8 = _mm_add_epi32(xmm8, xmm11);

	xmm4 = _mm_shufflelo_epi16(xmm4, 0xD8);
	xmm12 = _mm_shufflelo_epi16(xmm12, 0xD8);
	xmm4 = _mm_shufflehi_epi16 (xmm4, 0xD8);
	xmm12 = _mm_shufflehi_epi16 (xmm12, 0xD8);

	xmm7 = _mm_set1_epi32 (0x800);	// rounding
	xmm15 = _mm_set1_epi32 (0x800);	// rounding

	xmm1 = _mm_add_epi32(xmm1, xmm7);
	xmm9 = _mm_add_epi32(xmm9, xmm15);

	xmm6 = _mm_shuffle_epi32 (xmm4, 0xAA) ;
	xmm14 = _mm_shuffle_epi32 (xmm12, 0xAA) ;
	xmm5 = _mm_shuffle_epi32 (xmm4, 0) ;
	xmm13 = _mm_shuffle_epi32 (xmm12, 0) ;

	xmm5 = _mm_madd_epi16(xmm5, _mm_load_si128((__m128i *)(pecx)));
	xmm13 = _mm_madd_epi16(xmm13, _mm_load_si128((__m128i *)(pecx)));
	xmm5 = _mm_add_epi32(xmm5, xmm7);
	xmm13 = _mm_add_epi32(xmm13, xmm15);
	xmm6 = _mm_madd_epi16(xmm6, _mm_load_si128((__m128i *)(pecx+8)));
	xmm14 = _mm_madd_epi16(xmm14, _mm_load_si128((__m128i *)(pecx+8)));

	xmm7 = _mm_shuffle_epi32 (xmm4, 0x55) ;
	xmm15 = _mm_shuffle_epi32 (xmm12, 0x55) ;

	xmm7 = _mm_madd_epi16(xmm7, _mm_load_si128((__m128i *)(pecx+16)));
	xmm15 = _mm_madd_epi16(xmm15, _mm_load_si128((__m128i *)(pecx+16)));

	xmm4 = _mm_shuffle_epi32 (xmm4, 0xFF) ;
	xmm12 = _mm_shuffle_epi32 (xmm12, 0xFF) ;

	xmm4 = _mm_madd_epi16(xmm4, _mm_load_si128((__m128i *)(pecx+24)));
	xmm12 = _mm_madd_epi16(xmm12, _mm_load_si128((__m128i *)(pecx+24)));
	xmm1 = _mm_add_epi32(xmm1, xmm2);
	xmm9 = _mm_add_epi32(xmm9, xmm10);
	
	xmm2 = xmm1 ;
	xmm10 = xmm9 ;
	
	xmm2 = _mm_sub_epi32(xmm2, xmm0);
	xmm10 = _mm_sub_epi32(xmm10, xmm8);
	
	xmm2 = _mm_srai_epi32(xmm2, 12);
	xmm10 = _mm_srai_epi32(xmm10, 12);
	
	xmm2 = _mm_shuffle_epi32 (xmm2, 0x1B) ;
	xmm10 = _mm_shuffle_epi32 (xmm10, 0x1B) ;
	
	xmm0 = _mm_add_epi32(xmm0, xmm1);
	xmm8 = _mm_add_epi32(xmm8, xmm9);
	
	xmm0 = _mm_srai_epi32(xmm0, 12);
	xmm8 = _mm_srai_epi32(xmm8, 12);

	xmm5 = _mm_add_epi32(xmm5, xmm6);
	xmm13 = _mm_add_epi32(xmm13, xmm14);

	xmm0 = _mm_packs_epi32(xmm0, xmm2);
	xmm8 = _mm_packs_epi32(xmm8, xmm10);

	xmm4 = _mm_add_epi32(xmm4, xmm7);
	xmm12 = _mm_add_epi32(xmm12, xmm15);

	xmm6 = xmm5;
	xmm14 = xmm13;

	xmm6 = _mm_sub_epi32(xmm6, xmm4); 
	xmm14 = _mm_sub_epi32(xmm14, xmm12); 
	xmm6 = _mm_srai_epi32(xmm6, 12);
	xmm14 = _mm_srai_epi32(xmm14, 12);
	xmm4 = _mm_add_epi32(xmm4, xmm5); 
	xmm12 = _mm_add_epi32(xmm12, xmm13); 
	xmm4 = _mm_srai_epi32(xmm4, 12); 
	xmm12 = _mm_srai_epi32(xmm12, 12); 

	xmm6 = _mm_shuffle_epi32 (xmm6, 0x1B) ;
	xmm14 = _mm_shuffle_epi32 (xmm14, 0x1B) ;
	xmm4 = _mm_packs_epi32(xmm4, xmm6);
	xmm12 = _mm_packs_epi32(xmm12, xmm14);
	
	_mm_store_si128((__m128i *)pedx, xmm0);
	_mm_store_si128((__m128i *)(pedx+16), xmm4);
	_mm_store_si128((__m128i *)(pedx+32), xmm8);
	_mm_store_si128((__m128i *)(pedx+48), xmm12);

	if(bSrcIsAligned)
	{
		xmm8 = _mm_load_si128((__m128i *)(peax+24)); 
	}
	else
	{
		xmm8 = _mm_loadu_si128((__m128i *)(peax+24)); 
	}
	pesi = My_idct_weighting + 96;

	if(bSrcIsAligned)
	{
		xmm12 = _mm_load_si128((__m128i *)(peax+8)); 
	}
	else
	{
		xmm12 = _mm_loadu_si128((__m128i *)(peax+8)); 
	}
	pecx = My_idct_weighting + 32;
//	DCT_8_INV_ROW	//Row 4, tab_i_35 and Row 2, tab_i_17
	xmm8 = _mm_shufflelo_epi16(xmm8, 0xD8);
	xmm8 = _mm_shufflehi_epi16 (xmm8, 0xD8);
	xmm11 = _mm_shuffle_epi32 (xmm8, 0x55) ;
	xmm9 = _mm_shuffle_epi32 (xmm8, 0) ;
	xmm10 = _mm_shuffle_epi32 (xmm8, 0xAA) ;
	xmm8 = _mm_shuffle_epi32 (xmm8, 0xFF) ;
	xmm9 = _mm_madd_epi16(xmm9, _mm_load_si128((__m128i *)(pesi)));
	xmm10 = _mm_madd_epi16(xmm10, _mm_load_si128((__m128i *)(pesi+8)));
	xmm11 = _mm_madd_epi16(xmm11, _mm_load_si128((__m128i *)(pesi+16)));
	xmm8 = _mm_madd_epi16(xmm8, _mm_load_si128((__m128i *)(pesi+24)));
	xmm8 = _mm_add_epi32(xmm8, xmm11);
	xmm12 = _mm_shufflelo_epi16(xmm12, 0xD8);
	xmm12 = _mm_shufflehi_epi16 (xmm12, 0xD8);
	xmm15 = _mm_set1_epi32 (0x800);	// rounding

	xmm9 = _mm_add_epi32(xmm9, xmm15);
	xmm14 = _mm_shuffle_epi32 (xmm12, 0xAA) ;
	xmm13 = _mm_shuffle_epi32 (xmm12, 0) ;
	xmm13 = _mm_madd_epi16(xmm13, _mm_load_si128((__m128i *)(pecx)));
	xmm13 = _mm_add_epi32(xmm13, xmm15);
	xmm14 = _mm_madd_epi16(xmm14, _mm_load_si128((__m128i *)(pecx+8)));
	xmm15 = _mm_shuffle_epi32 (xmm12, 0x55) ;
	xmm15 = _mm_madd_epi16(xmm15, _mm_load_si128((__m128i *)(pecx+16)));
	xmm12 = _mm_shuffle_epi32 (xmm12, 0xFF) ;
	xmm12 = _mm_madd_epi16(xmm12, _mm_load_si128((__m128i *)(pecx+24)));
	xmm9 = _mm_add_epi32(xmm9, xmm10);
	xmm10 = xmm9 ;
	xmm10 = _mm_sub_epi32(xmm10, xmm8);
	xmm10 = _mm_srai_epi32(xmm10, 12);
	xmm10 = _mm_shuffle_epi32 (xmm10, 0x1B) ;
	xmm8 = _mm_add_epi32(xmm8, xmm9);
	xmm8 = _mm_srai_epi32(xmm8, 12);
	xmm13 = _mm_add_epi32(xmm13, xmm14);
	xmm8 = _mm_packs_epi32(xmm8, xmm10);
	xmm12 = _mm_add_epi32(xmm12, xmm15);
	xmm14 = xmm13;
	xmm14 = _mm_sub_epi32(xmm14, xmm12); 
	xmm14 = _mm_srai_epi32(xmm14, 12);
	xmm12 = _mm_add_epi32(xmm12, xmm13); 
	xmm12 = _mm_srai_epi32(xmm12, 12); 
	xmm14 = _mm_shuffle_epi32 (xmm14, 0x1B) ;
	xmm12 = _mm_packs_epi32(xmm12, xmm14);

	_mm_store_si128((__m128i *)(pedx+24), xmm8);
	if(bSrcIsAligned)
	{
		xmm0 = _mm_load_si128((__m128i *)(peax+40));
	}
	else
	{
		xmm0 = _mm_loadu_si128((__m128i *)(peax+40));
	}
	_mm_store_si128((__m128i *)(pedx+8), xmm12);
	if(bSrcIsAligned)
	{	
		xmm4 = _mm_load_si128((__m128i *)(peax+56)); 
	}
	else
	{
		xmm4 = _mm_loadu_si128((__m128i *)(peax+56)); 
	}
//	DCT_8_INV_ROW	//Row 6, tab_i_35 and Row 8, tab_i_17	
	xmm0 = _mm_shufflelo_epi16(xmm0, 0xD8);
	xmm0 = _mm_shufflehi_epi16 (xmm0, 0xD8);
	xmm3 = _mm_shuffle_epi32 (xmm0, 0x55) ;
	xmm1 = _mm_shuffle_epi32 (xmm0, 0) ;
	xmm2 = _mm_shuffle_epi32 (xmm0, 0xAA) ;
	xmm0 = _mm_shuffle_epi32 (xmm0, 0xFF) ;
	xmm1 = _mm_madd_epi16(xmm1, _mm_load_si128((__m128i *)(pesi)));
	xmm2 = _mm_madd_epi16(xmm2, _mm_load_si128((__m128i *)(pesi+8)));
	xmm3 = _mm_madd_epi16(xmm3, _mm_load_si128((__m128i *)(pesi+16)));
	xmm0 = _mm_madd_epi16(xmm0, _mm_load_si128((__m128i *)(pesi+24)));
	xmm0 = _mm_add_epi32(xmm0, xmm3);
	xmm4 = _mm_shufflelo_epi16(xmm4, 0xD8);
	xmm4 = _mm_shufflehi_epi16 (xmm4, 0xD8);
	xmm7 = _mm_set1_epi32 (0x800);	// rounding

	xmm1 = _mm_add_epi32(xmm1, xmm7);
	xmm6 = _mm_shuffle_epi32 (xmm4, 0xAA) ;
	xmm5 = _mm_shuffle_epi32 (xmm4, 0) ;
	xmm5 = _mm_madd_epi16(xmm5, _mm_load_si128((__m128i *)(pecx)));
	xmm5 = _mm_add_epi32(xmm5, xmm7);
	xmm6 = _mm_madd_epi16(xmm6, _mm_load_si128((__m128i *)(pecx+8)));
	xmm7 = _mm_shuffle_epi32 (xmm4, 0x55) ;
	xmm7 = _mm_madd_epi16(xmm7, _mm_load_si128((__m128i *)(pecx+16)));
	xmm4 = _mm_shuffle_epi32 (xmm4, 0xFF) ;
	xmm4 = _mm_madd_epi16(xmm4, _mm_load_si128((__m128i *)(pecx+24)));
	xmm1 = _mm_add_epi32(xmm1, xmm2);
	xmm2 = xmm1 ;
	xmm2 = _mm_sub_epi32(xmm2, xmm0);
	xmm2 = _mm_srai_epi32(xmm2, 12);
	xmm2 = _mm_shuffle_epi32 (xmm2, 0x1B) ;
	xmm0 = _mm_add_epi32(xmm0, xmm1);
	xmm0 = _mm_srai_epi32(xmm0, 12);
	xmm5 = _mm_add_epi32(xmm5, xmm6);
	xmm0 = _mm_packs_epi32(xmm0, xmm2);
	xmm4 = _mm_add_epi32(xmm4, xmm7);
	xmm6 = xmm5;
	xmm6 = _mm_sub_epi32(xmm6, xmm4); 
	xmm6 = _mm_srai_epi32(xmm6, 12);
	xmm4 = _mm_add_epi32(xmm4, xmm5); 
	xmm4 = _mm_srai_epi32(xmm4, 12); 
	xmm6 = _mm_shuffle_epi32 (xmm6, 0x1B) ;
	xmm4 = _mm_packs_epi32(xmm4, xmm6);

//	DCT_8_INV_COL_8
	xmm6 = xmm4;
	xmm2 = xmm0;
	xmm3 = _mm_load_si128((__m128i *)(pedx+24));
	xmm1 = _mm_load_si128((__m128i *)(My_idct_weighting+144));
	xmm0 = _mm_mulhi_epi16(xmm0, xmm1);
	xmm5 = _mm_load_si128((__m128i *)(My_idct_weighting+128));
	xmm1 = _mm_mulhi_epi16(xmm1, xmm3);
	xmm1 = _mm_adds_epi16(xmm1, xmm3);
	xmm4 = _mm_mulhi_epi16(xmm4, xmm5);
	xmm7 = _mm_load_si128((__m128i *)(pedx+48));
	xmm5 = _mm_mulhi_epi16(xmm5, _mm_load_si128((__m128i *)(pedx+8)));
	xmm5 = _mm_subs_epi16(xmm5, xmm6);
	xmm6 = xmm5;
	xmm4 = _mm_adds_epi16(xmm4, _mm_load_si128((__m128i *)(pedx+8)));
	xmm0 = _mm_adds_epi16(xmm0, xmm2);
	xmm0 = _mm_adds_epi16(xmm0, xmm3);
	xmm2 = _mm_subs_epi16(xmm2, xmm1);
	xmm1 = xmm0;
	xmm3 = _mm_load_si128((__m128i *)(My_idct_weighting+136));
	xmm7 = _mm_mulhi_epi16(xmm7, xmm3);
	xmm3 = _mm_mulhi_epi16(xmm3, _mm_load_si128((__m128i *)(pedx+16)));
	xmm0 = _mm_adds_epi16(xmm0, xmm4);
	xmm4 = _mm_subs_epi16(xmm4, xmm1);
	xmm0 = _mm_adds_epi16(xmm0, _mm_set1_epi16 (1));

	_mm_store_si128((__m128i *)(pedx+56), xmm0);
	xmm5 = _mm_subs_epi16(xmm5, xmm2);
	xmm5 = _mm_adds_epi16(xmm5, _mm_set1_epi16 (1));
	xmm6 = _mm_adds_epi16(xmm6, xmm2);
	_mm_store_si128((__m128i *)(pedx+24), xmm6);
	xmm1 = xmm4;
	xmm0 = _mm_load_si128((__m128i *)(My_idct_weighting+152));
	xmm2 = xmm0;
	xmm4 = _mm_adds_epi16(xmm4, xmm5);
	xmm1 = _mm_subs_epi16(xmm1, xmm5);
	xmm7 = _mm_adds_epi16(xmm7, _mm_load_si128((__m128i *)(pedx+16)));
	xmm3 = _mm_subs_epi16(xmm3, _mm_load_si128((__m128i *)(pedx+48)));
	xmm6 = _mm_load_si128((__m128i *)(pedx));
	xmm0 = _mm_mulhi_epi16(xmm0, xmm1);
	xmm5 = _mm_load_si128((__m128i *)(pedx+32));
	xmm5 = _mm_adds_epi16(xmm5, xmm6);
	xmm6 = _mm_subs_epi16(xmm6, _mm_load_si128((__m128i *)(pedx+32)));
	xmm2 = _mm_mulhi_epi16(xmm2, xmm4);
	xmm4 = _mm_adds_epi16(xmm4, xmm2);
	xmm2 = xmm5;
	xmm2 = _mm_subs_epi16(xmm2, xmm7);
	xmm4 = _mm_or_si128(xmm4, _mm_set1_epi16 (1));
	xmm0 = _mm_adds_epi16(xmm0, xmm1);
	xmm0 = _mm_or_si128(xmm0, _mm_set1_epi16 (1));
	xmm5 = _mm_adds_epi16(xmm5, xmm7);
	xmm5 = _mm_adds_epi16(xmm5, _mm_set1_epi16 (16));
	xmm1 = xmm6;
	xmm7 = _mm_load_si128((__m128i *)(pedx+56));
	xmm7 = _mm_adds_epi16(xmm7, xmm5);
	xmm7 = _mm_srai_epi16(xmm7, 5);
	_mm_store_si128((__m128i *)(pedx), xmm7);
	xmm6 = _mm_adds_epi16(xmm6, xmm3);
	xmm6 = _mm_adds_epi16(xmm6, _mm_set1_epi16 (16));
	xmm1 = _mm_subs_epi16(xmm1, xmm3);
	xmm1 = _mm_adds_epi16(xmm1, _mm_set1_epi16 (15));
	xmm7 = xmm1;
	xmm3 = xmm6;
	xmm6 = _mm_adds_epi16(xmm6, xmm4);
	xmm2 = _mm_adds_epi16(xmm2, _mm_set1_epi16 (15));
	xmm6 = _mm_srai_epi16(xmm6, 5);
	_mm_store_si128((__m128i *)(pedx+8), xmm6);
	xmm1 = _mm_adds_epi16(xmm1, xmm0);
	xmm1 = _mm_srai_epi16(xmm1, 5);
	_mm_store_si128((__m128i *)(pedx+16), xmm1);
	xmm1 = _mm_load_si128((__m128i *)(pedx+24));
	xmm6 = xmm1;
	xmm7 = _mm_subs_epi16(xmm7, xmm0);
	xmm7 = _mm_srai_epi16(xmm7, 5);
	_mm_store_si128((__m128i *)(pedx+40), xmm7);
	xmm5 = _mm_subs_epi16(xmm5, _mm_load_si128((__m128i *)(pedx+56)));
	xmm5 = _mm_srai_epi16(xmm5, 5); 
	_mm_store_si128((__m128i *)(pedx+56), xmm5);
	xmm3 = _mm_subs_epi16(xmm3, xmm4);
	xmm6 = _mm_adds_epi16(xmm6, xmm2);
	xmm2 = _mm_subs_epi16(xmm2, xmm1);
	xmm6 = _mm_srai_epi16(xmm6, 5);
	_mm_store_si128((__m128i *)(pedx+24), xmm6);
	xmm2 = _mm_srai_epi16(xmm2, 5); 
	_mm_store_si128((__m128i *)(pedx+32), xmm2);
	xmm3 = _mm_srai_epi16(xmm3, 5); 
	_mm_store_si128((__m128i *)(pedx+48), xmm3);

	return(fwStsNoErr);
}

template<typename T>
static void transposeBlock(T* dst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep)
{
	for(unsigned int I = 0; I < NUM_ROWS; I++)
	{
		for(unsigned int J = 0; J < NUM_COLS; J++)
		{
			T ucomp = CBL_LIBRARY::Limits<T>::Sat((*srcU)[I][J]);
			T vcomp = CBL_LIBRARY::Limits<T>::Sat((*srcV)[I][J]);

			((T*)(((char*)dst) + J*dstStep))[2*I] = ucomp;
			((T*)(((char*)dst) + J*dstStep))[(2*I)+1] = vcomp;
		}
	}
}

template<typename T>
static void transposeBlock(T* dst, Pointer8x8Block16s src, Fw32s dstStep)
{
	for(unsigned int I = 0; I < NUM_ROWS; I++)
	{
		for(unsigned int J = 0; J < NUM_COLS; J++)
		{
			((T*)(((char*)dst) + I * dstStep))[J] = CBL_LIBRARY::Limits<T>::Sat((*src)[J][I]);
		}
	}
}

/**
 * Transpose an 8x8 block of Fw16s
 *
 * @param dst pointer to the location of memory where the transposed block will be written
 * @param src pointer to the location of memory where the block to be transposed is.
 * @param dstStep distance in bytes of two rows in the destination buffer
 *
 * Algorithm:
 *
 * Step 1: Shuffle low and high qwords of rows 0-4,1-5,2-6,3-7
 * Step 2: Shuffle words of (the NEW) rows 0-1,2-3,4-5,6-7 	
 * Step 3: Shuffle dwords of (the NEW) rows 0-2,1-3,4-6,5-7
 * Step 4: Shuffle qwords of (the NEW) rows 0-4,1-5,2-6,3-7
 * Write block to destination taking dstStep into account
 */
static void transposeBlockOptimized_16s_C1R(Fw16s* pDst, Pointer8x8Block16s pSrc, Fw32s dstStep)
{
	__m128i xmm0,  xmm1,  xmm2,  xmm3,  xmm4,  xmm5,  xmm6,  xmm7;
	__m128i xmm8,  xmm9;

#include "TransposeBlockOptimizedCode.hpp"
	const unsigned int OUTPUT_BYTE_STRIDE = dstStep;

	if(FW_REF::IsAligned(pDst, dstStep))
	{
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), xmm0);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), xmm1);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), xmm2);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), xmm3);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), xmm4);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), xmm5);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), xmm6);
		_mm_store_si128 ((__m128i *)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), xmm7);
	}
	else
	{
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), xmm0);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), xmm1);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), xmm2);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), xmm3);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), xmm4);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), xmm5);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), xmm6);
		_mm_storeu_si128 ((__m128i *)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), xmm7);
	}
}

/**
 * Transpose an 8x8 block of Fw16s and convert to Fw8u using saturation
 *
 * @param dst pointer to the location of memory where the transposed block will be written
 * @param src pointer to the location of memory where the block to be transposed is.
 * @param dstStep distance in bytes of two rows in the destination buffer
 *
 * Use same algorithm as in transposeBlockOptimized_16s_C1R() for transposing block
 * Finally, saturate to 8-bits and write block to destination taking dstStep into account
 */
static void transposeBlockOptimized_16s8u_C1R(Fw8u *pDst, Pointer8x8Block16s pSrc, Fw32s dstStep)
{
	__m128i xmm0,  xmm1,  xmm2,  xmm3,  xmm4,  xmm5,  xmm6,  xmm7;
	__m128i xmm8,  xmm9;

#include "TransposeBlockOptimizedCode.hpp"
	const unsigned int OUTPUT_BYTE_STRIDE = dstStep;
	xmm8 = _mm_packus_epi16(xmm0, xmm1);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), xmm8);
	xmm8 = _mm_srli_si128(xmm8, 8);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), xmm8);

	xmm8 = _mm_packus_epi16(xmm2, xmm3);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), xmm8);
	xmm8 = _mm_srli_si128(xmm8, 8);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), xmm8);

	xmm8 = _mm_packus_epi16(xmm4, xmm5);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), xmm8);
	xmm8 = _mm_srli_si128(xmm8, 8);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), xmm8);

	xmm8 = _mm_packus_epi16(xmm6, xmm7);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), xmm8);
	xmm8 = _mm_srli_si128(xmm8, 8);
	_mm_storel_epi64((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), xmm8);
}

/**
 * Transpose 8x8 U and V blocks of Fw16s and store to common UV block
 *
 * @param dst pointer to the location of memory where the transposed block will be written
 * @param srcU pointer to the location of memory where the U block to be transposed is.
 * @param srcV pointer to the location of memory where the V block to be transposed is.
 * @param dstStep distance in bytes of two rows in the destination buffer
 *
 * Use same algorithm as in transposeBlockOptimized_16s_C1R() for transposing blocks
 * Saturate to 8-bits and write blocks to common destination block taking dstStep into account
 */
static void transposeBlockOptimized_16s_P2C2R(Fw16s* pDst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep)
{
	Pointer8x8Block16s pSrc;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	__m128i xmm8, xmm9;

	pSrc = srcV;
#include "TransposeBlockOptimizedCode.hpp"
	//save the transposed block of the V components to tmp0-7
	__m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp0 = _mm_load_si128(&xmm0);
	tmp1 = _mm_load_si128(&xmm1);
	tmp2 = _mm_load_si128(&xmm2);
	tmp3 = _mm_load_si128(&xmm3);
	tmp4 = _mm_load_si128(&xmm4);
	tmp5 = _mm_load_si128(&xmm5);
	tmp6 = _mm_load_si128(&xmm6);
	tmp7 = _mm_load_si128(&xmm7);

	//save the transposed block of the U components to xmm0-7
	pSrc = srcU;
#include "TransposeBlockOptimizedCode.hpp"

	//store to memory
	__m128i tmpXmmLow, tmpXmmHigh;

	const unsigned int OUTPUT_BYTE_STRIDE = dstStep;
	const bool bDstIsAligned = FW_REF::IsAligned(pDst, dstStep);
	
	tmpXmmLow = _mm_unpacklo_epi16(xmm0, tmp0);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm0, tmp0);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm1, tmp1);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm1, tmp1);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm2, tmp2);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm2, tmp2);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm3, tmp3);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm3, tmp3);	
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm4, tmp4);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm4, tmp4);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm5, tmp5);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm5, tmp5);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm6, tmp6);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm6, tmp6);
	if(bDstIsAligned)
	{			
		_mm_store_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
		
	tmpXmmLow = _mm_unpacklo_epi16(xmm7, tmp7);
	tmpXmmHigh = _mm_unpackhi_epi16(xmm7, tmp7);
	if(bDstIsAligned)
	{			
		_mm_store_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), tmpXmmLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE + NUM_COLS * sizeof(Fw16s)), tmpXmmHigh);
	}
}
/*
 * Transpose 8x8 U and V blocks of Fw16s and store to common UV block of Fw8u using saturation.
 *
 * @param dst pointer to the location of memory where the transposed block will be written
 * @param srcU pointer to the location of memory where the U block to be transposed is.
 * @param srcV pointer to the location of memory where the V block to be transposed is.
 * @param dstStep distance in bytes of two rows in the destination buffer
 *
 * Use same algorithm as in transposeBlockOptimized_16s_C1R() for transposing blocks
 * Saturate to 8-bits and write blocks to common destination block taking dstStep into account
 */

static void transposeBlockOptimized_16s8u_P2C2R(Fw8u* pDst, Pointer8x8Block16s srcU, Pointer8x8Block16s srcV, Fw32s dstStep)
{
	Pointer8x8Block16s pSrc;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	__m128i xmm8, xmm9;

	pSrc = srcV;
#include "TransposeBlockOptimizedCode.hpp"
	//save the transposed block of the V components to tmp0-7
	__m128i tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	tmp0 = _mm_load_si128(&xmm0);
	tmp1 = _mm_load_si128(&xmm1);
	tmp2 = _mm_load_si128(&xmm2);
	tmp3 = _mm_load_si128(&xmm3);
	tmp4 = _mm_load_si128(&xmm4);
	tmp5 = _mm_load_si128(&xmm5);
	tmp6 = _mm_load_si128(&xmm6);
	tmp7 = _mm_load_si128(&xmm7);

	//save the transposed block of the U components to xmm0-7
	pSrc = srcU;
#include "TransposeBlockOptimizedCode.hpp"

	//saturate to 8 bits and store to memory
	const unsigned int OUTPUT_BYTE_STRIDE = dstStep;
	const bool bDstIsAligned = FW_REF::IsAligned(pDst, dstStep);
	
	__m128i tmpSat8Low, tmpSat8High;
	__m128i tmpStoreLow, tmpStoreHigh;

	tmpSat8Low = _mm_packus_epi16(xmm0, xmm1);
	tmpSat8High = _mm_packus_epi16(tmp0, tmp1);
	tmpStoreLow = _mm_unpacklo_epi8(tmpSat8Low, tmpSat8High);
	tmpStoreHigh = _mm_unpackhi_epi8(tmpSat8Low, tmpSat8High);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 0 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 1 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}

	tmpSat8Low = _mm_packus_epi16(xmm2, xmm3);
	tmpSat8High = _mm_packus_epi16(tmp2, tmp3);
	tmpStoreLow = _mm_unpacklo_epi8(tmpSat8Low, tmpSat8High);
	tmpStoreHigh = _mm_unpackhi_epi8(tmpSat8Low, tmpSat8High);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 2 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 3 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}

	tmpSat8Low = _mm_packus_epi16(xmm4, xmm5);
	tmpSat8High = _mm_packus_epi16(tmp4, tmp5);
	tmpStoreLow = _mm_unpacklo_epi8(tmpSat8Low, tmpSat8High);
	tmpStoreHigh = _mm_unpackhi_epi8(tmpSat8Low, tmpSat8High);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 4 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 5 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}

	tmpSat8Low = _mm_packus_epi16(xmm6, xmm7);
	tmpSat8High = _mm_packus_epi16(tmp6, tmp7);
	tmpStoreLow = _mm_unpacklo_epi8(tmpSat8Low, tmpSat8High);
	tmpStoreHigh = _mm_unpackhi_epi8(tmpSat8Low, tmpSat8High);
	if(bDstIsAligned)
	{
		_mm_store_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_store_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}
	else
	{
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 6 * OUTPUT_BYTE_STRIDE), tmpStoreLow);
		_mm_storeu_si128((__m128i*)(((char*)pDst) + 7 * OUTPUT_BYTE_STRIDE), tmpStoreHigh);
	}
}

/**
 * Performs inverse DCT on pre-transposed block.
 *
 * @param pSrc Pointer to the block of DCT coefficients.
 * @param pDst Pointer to the destination array.
 * @param dstStep Step of the destination array.
 * @param count Number of the last non-zero coefficient in zig-zag order. If the block contains
 * no non-zero coefficients, pass the value -1.
 * 
 * This function is declared in the fwVideo.h header file. The function fwiDCT8x8Inv_AANTransposed_16s_C1R 
 * is used for non-intra macroblocks and performs inverse DCT on a transposed block. The block is transposed 
 * during the rearranging stage of VLC decoding, using the transposed scanning matrix as scanMatrix argument 
 * of the functions ReconstructDCTBlock_MPEG1 and ReconstructDCTBlock_MPEG2. 
 *
 * @return fwStsNoErr Indicates no error. fwStsNullPtrErr Indicates an error when at least one input pointer is NULL.
 */
FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s_C1R)(const Fw16s* pSrc, Fw16s* pDst, Fw32s dstStep, Fw32s count)
{
	if(FW_REF::PtrNotOK(pSrc, pDst))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:    
		My_idct_SSE2(pSrc, &tempIdctOutputSSE2[0][0], count);
		transposeBlockOptimized_16s_C1R(pDst, (Pointer8x8Block16s)&tempIdctOutputSSE2, dstStep);
		break;
	case DT_REFR:
	default:             
		float c[8][8];
	    init_idct(c);
		idct(c, pSrc, &tempIdctOutputSSE2[0][0]);
		transposeBlock(pDst, (Pointer8x8Block16s)&tempIdctOutputSSE2, dstStep);
		break;
	}
	
	return fwStsNoErr;
}

/**
 * Performs inverse DCT on pre-transposed block and converts output to unsigned char format.
 * @param pSrc Pointer to the block of DCT coefficients.
 * @param pDst Pointer to the destination array.
 * @param dstStep Step of the destination array.
 * @param count Number of the last non-zero coefficient in zig-zag order. If the block contains
 * no non-zero coefficients, pass the value -1.
 * 
 *
 * This function is declared in the fwVideo.h header file. The function fwiDCT8x8Inv_AANTransposed_16s8u_C1R 
 * is used for intra macroblocks. The function performs inverse DCT on a transposed block and converts the 
 * output to unsigned char format. The block is transposed during the rearranging stage of VCL decoding, 
 * using the transposed scanning matrix as scanMatrix argument of the functions ReconstructDCTBlockIntra_MPEG1 
 * and ReconstructDCTBlockIntra_MPEG2.
 *
 * @return fwStsNoErr Indicates no error.fwStsNullPtrErr Indicates an error when at least one input pointer is NULL.
 */
FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s8u_C1R)(const Fw16s* pSrc, Fw8u* pDst, Fw32s dstStep, Fw32s count)
{
	if(FW_REF::PtrNotOK(pSrc, pDst))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		My_idct_SSE2(pSrc, &tempIdctOutputSSE2[0][0], count);
		transposeBlockOptimized_16s8u_C1R(pDst, (Pointer8x8Block16s)&tempIdctOutputSSE2, dstStep);
		break;
	case DT_REFR:
	default:    
		float c[8][8];
	    init_idct(c);
		idct(c, pSrc, &tempIdctOutputSSE2[0][0]);
		transposeBlock(pDst, (Pointer8x8Block16s)&tempIdctOutputSSE2, dstStep);
		break;
	}

	return fwStsNoErr;
}

/**
 * Performs inverse DCT on pre-transposed data of two input chroma blocks and joins the output data into one
 * array.
 * 
 * @param pSrcU Pointer to the block of DCT coefficients for U component.
 * @param pSrcV Pointer to the block of DCT coefficients for V component.
 * @param pDstUV Pointer to the destination array.
 * @param dstStep Step of the destination array.
 * @param countU Number of the last non-zero U coefficient in zig-zag order. If the block contains no non-zero 
 * coefficients, pass the value -1.
 * @param countV Number of the last non-zero V coefficient in zig-zag order. If the block contains no non-zero 
 * coefficients, pass the value -1.
 *
 * This function is declared in the fwVideo.h header file. The function fwiDCT8x8Inv_AANTransposed_16s_P2C2R is 
 * used for non-intra macroblocks. 
 * The function performs inverse DCT on a transposed U-block and a transposed V-block and joins the output data 
 * into one UV Block. The blocks are transposed during the rearranging stage of VCL decoding, using the 
 * transposed scanning matrix as scanMatrix argument of the functions ReconstructDCTBlock_MPEG1 and 
 * ReconstructDCTBlock_MPEG2.
 * @return fwStsNoErr Indicates no error. fwStsNullPtrErr Indicates an error when at least one input pointer is NULL.
 */

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s_P2C2R)(const Fw16s* pSrcU, const Fw16s* pSrcV, Fw16s* pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV)
{
	if(FW_REF::PtrNotOK(pSrcU, pSrcV, pDstUV))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		//perform the AAN IDCT twice, once for the U and once for the V components
		My_idct_SSE2(pSrcU, &tempIdctOutputSSE2[0][0], countU);//U
		My_idct_SSE2(pSrcV, &tempUVIdctOutputSSE2[0][0], countV);//V
		transposeBlockOptimized_16s_P2C2R(pDstUV, (Pointer8x8Block16s)&tempIdctOutputSSE2, 
			(Pointer8x8Block16s)&tempUVIdctOutputSSE2, dstStep);
		break;
	case DT_REFR:
	default:                
		float c[8][8];
	    init_idct(c);
		idct(c, pSrcU, &tempIdctOutputSSE2[0][0]);
		idct(c, pSrcV, &tempUVIdctOutputSSE2[0][0]);
		transposeBlock(pDstUV, (Pointer8x8Block16s)&tempIdctOutputSSE2, 
			(Pointer8x8Block16s)&tempUVIdctOutputSSE2, dstStep);

		break;
	}

	return fwStsNoErr;
}

/** 
 * Performs inverse DCT on pre-transposed data of two input chroma blocks and joins the output data into one
 * unsigned char array.
 * 
 * @param pSrcU Pointer to the block of DCT coefficients for U component.
 * @param pSrcV Pointer to the block of DCT coefficients for V component.
 * @param pDstUV Pointer to the destination array.
 * @param dstStep Step of the destination array.
 * @param countU Number of the last non-zero U coefficient in zig-zag order. If the block contains no non-zero 
 * coefficients, pass the value -1.
 * @param countV Number of the last non-zero V coefficient in zig-zag order. If the block contains no non-zero 
 * coefficients, pass the value -1.
 *
 * This function is declared in the fwVideo.h header file. The function fwiDCT8x8Inv_AANTransposed_16s8u_P2C2R is 
 * used for intra macroblocks. The function performs inverse DCT on a transposed U-block and a transposed V-block 
 * and joins the output data into one unsigned char UV Block. The blocks are transposed during the rearranging 
 * stage of VCL decoding, using the transposed scanning matrix as scanMatrix argument of the functions 
 * ReconstructDCTBlockIntra_MPEG1 and ReconstructDCTBlockIntra_MPEG2.
 * @return fwStsNoErr Indicates no error. fwStsNullPtrErr Indicates an error when at least one input pointer is NULL.
 */
FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s8u_P2C2R)(const Fw16s* pSrcU, const Fw16s* pSrcV, Fw8u* pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV)
{
	if(FW_REF::PtrNotOK(pSrcU, pSrcV, pDstUV))return fwStsNullPtrErr;
	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		//perform the AAN IDCT twice, once for the U and once for the V components
		My_idct_SSE2(pSrcU, &tempIdctOutputSSE2[0][0], countU);//U
		My_idct_SSE2(pSrcV, &tempUVIdctOutputSSE2[0][0], countV);//V
		transposeBlockOptimized_16s8u_P2C2R(pDstUV, (Pointer8x8Block16s)&tempIdctOutputSSE2,
			(Pointer8x8Block16s)&tempUVIdctOutputSSE2, dstStep);
		break;
	case DT_REFR:
	default:          
		float c[8][8];
	    init_idct(c);
		idct(c, pSrcU, &tempIdctOutputSSE2[0][0]);
		idct(c, pSrcV, &tempUVIdctOutputSSE2[0][0]);
		transposeBlock(pDstUV, (Pointer8x8Block16s)&tempIdctOutputSSE2,
			(Pointer8x8Block16s)&tempUVIdctOutputSSE2, dstStep);
		break;
	}

	return fwStsNoErr;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2

