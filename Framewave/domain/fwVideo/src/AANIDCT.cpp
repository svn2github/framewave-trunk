/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*******************************************************/
#include "fwdev.h"
#include "FwSharedCode.h"
#include "FwSharedCode_SSE2.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

extern SYS_FORCEALIGN_16 const Fw16s idct_weighting[];
extern const float idct_coefficients[8][8];

static void Idct(const float c[8][8], const Fw16s *pSrc, Fw16s *pDst)
{
	int i, j, k;
	float partialProduct;
	float tmp[64];

	for (i=0; i<8; i++)
		for (j=0; j<8; j++)
		{
			partialProduct = 0.0;

			for (k=0; k<8; k++)
				partialProduct+= c[k][j]*pSrc[8*i+k];

			tmp[8*i+j] = partialProduct;
		}

		// Transpose operation is integrated into address mapping by switching 
		// loop order of i and j 

		for (j=0; j<8; j++)
			for (i=0; i<8; i++)
			{
				partialProduct = 0.0;

				for (k=0; k<8; k++)
					partialProduct+= c[k][i]*tmp[8*k+j];

				//v = CBL_LIBRARY::Limits<int>::Sat(floor(partialProduct+0.5));
				//pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(v);
				pDst[8*i+j] = (Fw16s) CBL_LIBRARY::Limits<Fw16s>::Sat(floor(partialProduct+0.5));
			}
}


/**
 * performs AAN IDCT using SSE2
 *
 * Assumes destination is aligned on a 16-byte boundary
 */
SYS_INLINE static FwStatus My_idct_SSE2(const Fw16s* pSrc, Fw16s* pDst, Fw32s count)
{
	count = count;//in the future we will use count. For the time being make the compiler happy
	__m128i xmm0,  xmm1,  xmm2,  xmm3,  xmm4,  xmm5,  xmm6,  xmm7;
	__m128i xmm8,  xmm9,  xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
	Fw16s *peax = (Fw16s*)pSrc;
	Fw16s *pedx = pDst;
	const Fw16s *pesi, *pecx;

	pesi = idct_weighting;
	pecx = idct_weighting+64;

	const bool bSrcIsAligned = FW_REF::IsAligned(peax, 16);
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
	pesi = idct_weighting + 96;

	if(bSrcIsAligned)
	{
		xmm12 = _mm_load_si128((__m128i *)(peax+8)); 
	}
	else
	{
		xmm12 = _mm_loadu_si128((__m128i *)(peax+8)); 
	}
	pecx = idct_weighting + 32;
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
	xmm1 = _mm_load_si128((__m128i *)(idct_weighting+144));
	xmm0 = _mm_mulhi_epi16(xmm0, xmm1);
	xmm5 = _mm_load_si128((__m128i *)(idct_weighting+128));
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
	xmm3 = _mm_load_si128((__m128i *)(idct_weighting+136));
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
	xmm0 = _mm_load_si128((__m128i *)(idct_weighting+152));
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

    SYS_FORCEALIGN_16 Fw16s eightxEightDst[64];
    Fw16s* pEightxEightDst = eightxEightDst;
    bool isAligned;
    __m128i reg;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:    
		My_idct_SSE2(pSrc, eightxEightDst, count);

        for(int i=0;i<8;i++)
            {
             reg = _mm_load_si128((__m128i*)pEightxEightDst);
             isAligned = FW_REF::IsAligned(pDst,16);
             if(isAligned)
                 _mm_store_si128((__m128i*)pDst, reg);
             else
                 _mm_storeu_si128((__m128i*)pDst, reg);
            pDst = pDst + dstStep/2;
            pEightxEightDst = pEightxEightDst + 8;
            }

		break;

    default:             
        Idct(idct_coefficients, pSrc, eightxEightDst);
        for(int i=0;i<8;i++)
            {
            for(int j=0;j<8;j++)
                pDst[j] = pEightxEightDst[j];
            pDst = pDst + dstStep/2;
            pEightxEightDst = pEightxEightDst + 8;
            }
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

    SYS_FORCEALIGN_16 Fw16s eightxEightDst[64];
    Fw16s* pEightxEightDst = eightxEightDst;
    XMM128 reg1, reg2;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		My_idct_SSE2(pSrc, eightxEightDst, count);
        for(int i=0;i<4;i++)
            {
             reg1.i = _mm_load_si128((__m128i*)pEightxEightDst);
             pEightxEightDst = pEightxEightDst + 8;
             reg2.i = _mm_load_si128((__m128i*)pEightxEightDst);
             pEightxEightDst = pEightxEightDst + 8;
             reg1.i = _mm_packs_epi16(reg1.i, reg2.i);
             *((Fw64s*)pDst) = reg1.s64[0];
             pDst = pDst + dstStep;
             *((Fw64s*)pDst) = reg1.s64[1];
             pDst = pDst + dstStep;
            }
		break;

    default:    
        Idct(idct_coefficients, pSrc, eightxEightDst);
        for(int i=0;i<8;i++)
            {
            for(int j=0;j<8;j++)
                pDst[j] = CBL_LIBRARY::Limits<Fw8u>::Sat(pEightxEightDst[j]);
            pDst = pDst + dstStep;
            pEightxEightDst = pEightxEightDst + 8;
            }
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

    SYS_FORCEALIGN_16 Fw16s eightxEightDstU[64];
    SYS_FORCEALIGN_16 Fw16s eightxEightDstV[64];
    Fw16s* pEightxEightDstU = eightxEightDstU;
    Fw16s* pEightxEightDstV = eightxEightDstV;
    __m128i reg1, reg2, reg3;
    bool isAligned;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		//perform the AAN IDCT twice, once for the U and once for the V components
		My_idct_SSE2(pSrcU, eightxEightDstU, countU);//U
		My_idct_SSE2(pSrcV, eightxEightDstV, countV);//V

        for(int i=0;i<8;i++)
            {
             reg1 = _mm_load_si128((__m128i*)pEightxEightDstU);
             reg2 = _mm_load_si128((__m128i*)pEightxEightDstV);
             reg3 = _mm_unpacklo_epi16(reg1, reg2);
             reg1 = _mm_unpackhi_epi16(reg1, reg2);

             isAligned = FW_REF::IsAligned(pDstUV,16);

             if(isAligned)
                 {
                 _mm_store_si128((__m128i*)pDstUV, reg3);
                 _mm_store_si128((((__m128i*)pDstUV) + 1), reg1);
                 }
             else
                 {
                 _mm_storeu_si128((__m128i*)pDstUV, reg3);
                 _mm_storeu_si128((((__m128i*)pDstUV) + 1), reg1);
                 }

            pDstUV = pDstUV + dstStep/2;
            pEightxEightDstU = pEightxEightDstU + 8;
            pEightxEightDstV = pEightxEightDstV + 8;
            }
		break;

    default:                
		Idct(idct_coefficients, pSrcU, eightxEightDstU);
		Idct(idct_coefficients, pSrcV, eightxEightDstV);
        for(int i=0;i<8;i++)
            {
            for(int j=0,k=0;j<8;j++,k+=2)
                {
                pDstUV[k]   = pEightxEightDstU[j];
                pDstUV[k+1] = pEightxEightDstV[j];
                }

            pDstUV = pDstUV + dstStep/2;
            pEightxEightDstU = pEightxEightDstU + 8;
            pEightxEightDstV = pEightxEightDstV + 8;
            }
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

    SYS_FORCEALIGN_16 Fw16s eightxEightDstU[64];
    SYS_FORCEALIGN_16 Fw16s eightxEightDstV[64];
    Fw16s* pEightxEightDstU = eightxEightDstU;
    Fw16s* pEightxEightDstV = eightxEightDstV;
    __m128i reg1, reg2, reg3;
    bool isAligned;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:     
		//perform the AAN IDCT twice, once for the U and once for the V components
		My_idct_SSE2(pSrcU, eightxEightDstU, countU);//U
		My_idct_SSE2(pSrcV, eightxEightDstV, countV);//V
        for(int i=0;i<8;i++)
            {
             reg1 = _mm_load_si128((__m128i*)pEightxEightDstU);
             reg2 = _mm_load_si128((__m128i*)pEightxEightDstV);
             reg3 = _mm_unpacklo_epi16(reg1, reg2);
             reg1 = _mm_unpackhi_epi16(reg1, reg2);
             reg3 = _mm_packs_epi16(reg3, reg1);

             isAligned = FW_REF::IsAligned(pDstUV,16);

             if(isAligned)
                 _mm_store_si128((__m128i*)pDstUV, reg3);
             else
                 _mm_storeu_si128((__m128i*)pDstUV, reg3);

             pDstUV = pDstUV + dstStep;
             pEightxEightDstU = pEightxEightDstU + 8;
             pEightxEightDstV = pEightxEightDstV + 8;
            }
		break;

	default:          
		Idct(idct_coefficients, pSrcU, eightxEightDstU);
		Idct(idct_coefficients, pSrcV, eightxEightDstV);
        for(int i=0;i<8;i++)
            {
            for(int j=0,k=0;j<8;j++,k+=2)
                {
                pDstUV[k]   = CBL_LIBRARY::Limits<Fw8u>::Sat(pEightxEightDstU[j]);
                pDstUV[k+1] = CBL_LIBRARY::Limits<Fw8u>::Sat(pEightxEightDstV[j]);
                }

            pDstUV = pDstUV + dstStep;
            pEightxEightDstU = pEightxEightDstU + 8;
            pEightxEightDstV = pEightxEightDstV + 8;
            }
	}

	return fwStsNoErr;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2

