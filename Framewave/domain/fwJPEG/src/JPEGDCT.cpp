/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include Combined DCT functions from JPEG Chapter
//	fwiDCTQuantFwd8x8_JPEG_16s_C1
//	fwiDCTQuantFwd8x8_JPEG_16s_C1I
//	fwiDCTQuantFwd8x8LS_JPEG_8u16s_C1R
//	fwiDCTQuantInv8x8_JPEG_16s_C1
//	fwiDCTQuantInv8x8_JPEG_16s_C1I
//	fwiDCTQuantInv8x8LS_JPEG_8u16s_C1R
//************************************************************************* 

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "fwJPEG.h"
//#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

//#define USE_C

static int  IdctQuant_SSE2(const Fw16s *pSrc, Fw16s *pDst, const Fw16u *pQuantFwdTable);
static void FdctQuant_SSE2(const Fw16s *pSrc, Fw16s *pDst, const Fw16u *pQuantFwdTable);
static int IdctQuant_LS_SSE2(const Fw16s *pSrc, Fw8u *pDst, int dstStp, const Fw16u *pQuantInvTable);
static void FdctQuant_LS_SSE2(const Fw8u *pSrc, int srcStp, Fw16s *pDst, const Fw16u *pQuantFwdTable);

extern const float idct_coefficients[8][8];
static void Idct(const float c[8][8], const Fw16s *pSrc, Fw16s *pDst);
static void C_faandct(const Fw16s *pSrc, Fw16s *pDst, float round);

#ifndef __JPEGDCT
#define __JPEGDCT
#define MYFW_EPSILON (float)0.0001 
#endif

//-----------------------------------------------------------------------
//This function performs forward DCT and quantization for an 8*8 block
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8_JPEG_16s_C1)(const Fw16s *pSrc, Fw16s *pDst, 
										 const Fw16u *pQuantFwdTable)
{
    if (pSrc == 0 || pDst == 0 || pQuantFwdTable == 0)
		return fwStsNullPtrErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
		FdctQuant_SSE2(pSrc, pDst, pQuantFwdTable);
		break;
	default:
		int q;
		C_faandct(pSrc, pDst, MYFW_EPSILON);

		for(int i=0; i<64; i++) {
			q = pQuantFwdTable[i];
			//*(pDst+i) = (Fw16s)(*(pDst+i)*q) >> 15 ;
			if (*(pDst+i) >0)
				*(pDst+i) = (Fw16s)((*(pDst+i)*q+16384)>>15);
			else 
				*(pDst+i) = -(Fw16s)((-(*(pDst+i))*q+16384)>>15);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8_JPEG_16s_C1I)(Fw16s *pDst, const Fw16u *pQuantFwdTable)
{
    if (pDst == 0 || pQuantFwdTable == 0)
		return fwStsNullPtrErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
		FdctQuant_SSE2(pDst, pDst, pQuantFwdTable);
		break;
	default:
		int q;
		C_faandct(pDst, pDst, MYFW_EPSILON);
		for(int i=0; i<64; i++) 
		{
			q = pQuantFwdTable[i];
			//*(pDst+i) = (Fw16s)(*(pDst+i)*q) >> 15 ;
			if (*(pDst+i) >0)
				*(pDst+i) = (Fw16s)((*(pDst+i)*q+16384)>>15);
			else 
				*(pDst+i) = -(Fw16s)((-(*(pDst+i))*q+16384)>>15);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This function performs level shift, forward DCT and quantization for 
//an 8*8 block
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8LS_JPEG_8u16s_C1R)(const Fw8u *pSrc, int srcStp, Fw16s *pDst, 
											  const Fw16u *pQuantFwdTable)
{
    if (pSrc == 0 || pDst == 0 || pQuantFwdTable == 0)
		return fwStsNullPtrErr;

	if (srcStp < 1) return fwStsStepErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
		FdctQuant_LS_SSE2(pSrc, srcStp, pDst, pQuantFwdTable);
		break;
	default:
		int i, q;
		for(i=0; i<8; i++) {
			for(int j=0; j<8; j++) {	
				*(pDst+i*8+j) = (Fw16s)pSrc[i*srcStp+j] - 128;
			}
		}

		C_faandct(pDst, pDst, MYFW_EPSILON);

		for(i=0; i<64; i++) {
			q = pQuantFwdTable[i];
			if (*(pDst+i) >0)
				*(pDst+i) = (Fw16s)((*(pDst+i)*q+16384)>>15);
			else 
				*(pDst+i) = -(Fw16s)((-(*(pDst+i))*q+16384)>>15);
			//*(pDst+i) = (Fw16s)((((int)(*(pDst+i)))*q) >> 15) ;
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This function performs dequantization and inverse DCT for an 8*8 block
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8_JPEG_16s_C1)(const Fw16s *pSrc, Fw16s *pDst, const Fw16u *pQuantInvTable)
{
    if (pSrc == 0 || pDst == 0 || pQuantInvTable == 0)
		return fwStsNullPtrErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
		IdctQuant_SSE2(pSrc, pDst, pQuantInvTable);
		break;
	default:
		int q;
        for(int i=0; i<64; i++) {
			q = pQuantInvTable[i];
			*(pDst+i) = (Fw16s)(*(pSrc+i)* q) ;
		}

		Idct(idct_coefficients, pDst, pDst);
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8_JPEG_16s_C1I)(Fw16s *pDst, const Fw16u *pQuantInvTable)
{
    if (pDst == 0 || pQuantInvTable == 0)
		return fwStsNullPtrErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
	   IdctQuant_SSE2(pDst, pDst, pQuantInvTable);
	   break;
	default:
		int q;
		for(int i=0; i<64; i++) {
			q = pQuantInvTable[i];
			*(pDst+i) = (Fw16s)(*(pDst+i)*q) ;
		}	
        
        Idct(idct_coefficients, pDst, pDst);
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This function performs dequantization, inverse DCT and level shift for 
//an 8*8 block
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8LS_JPEG_16s8u_C1R)(const Fw16s *pSrc, Fw8u *pDst, int dstStp, const Fw16u *pQuantInvTable)
{
    if (pSrc == 0 || pDst == 0 || pQuantInvTable == 0)
		return fwStsNullPtrErr;

	if (dstStp < 1) return fwStsStepErr;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE2:
	case DT_SSE3:
		IdctQuant_LS_SSE2(pSrc, pDst, dstStp, pQuantInvTable);
		break;
	default:
		int q, i, j;
	    Fw16s ppSrc[64];
		for(i=0; i<8; i++) {
		  for(j=0; j<8; j++) {
		 	q = pQuantInvTable[8*i+j];
			*(ppSrc+i*8+j) = (Fw16s)(*(pSrc+i*8+j)*q);
			}
		}

		Idct(idct_coefficients, ppSrc, ppSrc);

		for(i=0; i<8; i++) {
		  for(j=0; j<8; j++) {
			*(pDst+i*dstStp+j) = FW_REF::Limits<U8>::Sat(ppSrc[i*8+j] + 128);
		  }
		}
	}

	return fwStsNoErr;
}

extern const Fw16s idct_weighting[160];
//static SYS_FORCEALIGN_16 short idct_weighting[] = {
//	16384,	21407,	16384,	8867, 16384,	-8867,  16384, -21407,	//	w05 w04 w01 w00	w13 w12 w09 w08
//	16384,	8867,	-16384,	-21407,	-16384,	21407,  16384,  -8867,	//	w07 w06 w03 w02	w15 w14 w11 w10
//	22725,	19266,  19266,  -4520, 	12873,	-22725,	4520,	-12873,	//	w21 w20 w17 w16 w29 w28 w25 w24
//	12873,	4520,	-22725, -12873, 4520,	19266,  19266,	-22725, //	w23 w22 w19 w18 w31 w30 w27 w26
//
//	22725,  29692,  22725,  12299, 22725,	-12299, 22725,	-29692,
//	22725,  12299,	-22725, -29692,	-22725,	29692,  22725,	-12299,
//	31521,  26722,  26722,  -6270, 17855,	-31521, 6270,	-17855,
//	17855,  6270,	-31521, -17855,	6270,	26722,  26722,	-31521,
//
//	21407,  27969,  21407,  11585,  21407,	-11585, 21407,	-27969,
//	21407,  11585,	-21407, -27969,	-21407, 27969,  21407,	-11585,
//	29692,  25172,  25172,  -5906,  16819,	-29692, 5906,	-16819,
//	16819,  5906,	-29692, -16819,	5906,	25172,  25172,	-29692,
//
//	19266,  25172,  19266,  10426, 19266,	-10426, 19266,	-25172,	
//	19266,  10426,	-19266, -25172,	-19266, 25172,  19266,	-10426,	
//	26722,  22654,  22654,  -5315,  15137,	-26722, 5315,	-15137,	
//	15137,  5315,	-26722, -15137, 5315,	22654,  22654,	-26722,
//
//	13036,  13036,  13036,  13036,  13036,  13036,  13036,  13036, 
//	27146,  27146,  27146,  27146,  27146,  27146,  27146,  27146,
//	-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746, // tg * (2<<16) + 0.5
//	-19195, -19195, -19195, -19195,-19195, -19195, -19195, -19195   //cos * (2<<16) + 0.5
//}; 

int IdctQuant_SSE2(const Fw16s *pSrc, Fw16s *pDst, const Fw16u *pQuantInvTable)
{
	__m128i rxmm0,  rxmm1,  rxmm2,  rxmm3,  rxmm4,  rxmm5,  rxmm6,  rxmm7;
	__m128i rxmm8,  rxmm9,  rxmm10, rxmm11, rxmm12, rxmm13, rxmm14, rxmm15;
	__m128i quantCoef;
	Fw16s *peax = (Fw16s*)pSrc;
	Fw16s *pedx = (Fw16s*)pDst;
	Fw16s *pesi, *pecx;
	Fw16s *pedi = pDst;

	pesi = (Fw16s *)idct_weighting;
	pecx = (Fw16s *)(idct_weighting+64);

	rxmm0 = _mm_loadu_si128((__m128i *)(peax)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable));	// deQ
	rxmm0 = _mm_mullo_epi16(rxmm0, quantCoef);

	rxmm4 = _mm_loadu_si128((__m128i *)(peax+16)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+16));	// deQ
	rxmm4 = _mm_mullo_epi16(rxmm4, quantCoef);

	rxmm8 = _mm_loadu_si128((__m128i *)(peax+32)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+32));	// deQ
	rxmm8 = _mm_mullo_epi16(rxmm8, quantCoef);

	rxmm12 = _mm_loadu_si128((__m128i *)(peax+48)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+48));	// deQ
	rxmm12 = _mm_mullo_epi16(rxmm12, quantCoef);

//	DCT_8_INV_ROW	//Row 1, tab_i_04 and Row 3, tab_i_26
	rxmm0 = _mm_shufflelo_epi16(rxmm0, 0xD8);
//	DCT_8_INV_ROW	//Row 5, tab_i_04 and Row 7, tab_i_26
	rxmm8 = _mm_shufflelo_epi16(rxmm8, 0xD8);

	rxmm0 = _mm_shufflehi_epi16 (rxmm0, 0xD8);
	rxmm8 = _mm_shufflehi_epi16 (rxmm8, 0xD8);
	rxmm3 = _mm_shuffle_epi32 (rxmm0, 0x55) ;
	rxmm11 = _mm_shuffle_epi32 (rxmm8, 0x55) ;
	rxmm1 = _mm_shuffle_epi32 (rxmm0, 0) ;
	rxmm9 = _mm_shuffle_epi32 (rxmm8, 0) ;
	rxmm2 = _mm_shuffle_epi32 (rxmm0, 0xAA) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm8, 0xAA) ;
	rxmm0 = _mm_shuffle_epi32 (rxmm0, 0xFF) ;
	rxmm8 = _mm_shuffle_epi32 (rxmm8, 0xFF) ;
	rxmm1 = _mm_madd_epi16(rxmm1, _mm_load_si128((__m128i *)(pesi)));
	rxmm9 = _mm_madd_epi16(rxmm9, _mm_load_si128((__m128i *)(pesi)));
	rxmm2 = _mm_madd_epi16(rxmm2, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm10 = _mm_madd_epi16(rxmm10, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm3 = _mm_madd_epi16(rxmm3, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm11 = _mm_madd_epi16(rxmm11, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm0 = _mm_madd_epi16(rxmm0, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm8 = _mm_madd_epi16(rxmm8, _mm_load_si128((__m128i *)(pesi+24)));

	rxmm0 = _mm_add_epi32(rxmm0, rxmm3);
	rxmm8 = _mm_add_epi32(rxmm8, rxmm11);

	rxmm4 = _mm_shufflelo_epi16(rxmm4, 0xD8);
	rxmm12 = _mm_shufflelo_epi16(rxmm12, 0xD8);
	rxmm4 = _mm_shufflehi_epi16 (rxmm4, 0xD8);
	rxmm12 = _mm_shufflehi_epi16 (rxmm12, 0xD8);

	rxmm7 = _mm_set1_epi32 (0x800);	// rounding
	rxmm15 = _mm_set1_epi32 (0x800);	// rounding

	rxmm1 = _mm_add_epi32(rxmm1, rxmm7);
	rxmm9 = _mm_add_epi32(rxmm9, rxmm15);

	rxmm6 = _mm_shuffle_epi32 (rxmm4, 0xAA) ;
	rxmm14 = _mm_shuffle_epi32 (rxmm12, 0xAA) ;
	rxmm5 = _mm_shuffle_epi32 (rxmm4, 0) ;
	rxmm13 = _mm_shuffle_epi32 (rxmm12, 0) ;

	rxmm5 = _mm_madd_epi16(rxmm5, _mm_load_si128((__m128i *)(pecx)));
	rxmm13 = _mm_madd_epi16(rxmm13, _mm_load_si128((__m128i *)(pecx)));
	rxmm5 = _mm_add_epi32(rxmm5, rxmm7);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm15);
	rxmm6 = _mm_madd_epi16(rxmm6, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm14 = _mm_madd_epi16(rxmm14, _mm_load_si128((__m128i *)(pecx+8)));

	rxmm7 = _mm_shuffle_epi32 (rxmm4, 0x55) ;
	rxmm15 = _mm_shuffle_epi32 (rxmm12, 0x55) ;

	rxmm7 = _mm_madd_epi16(rxmm7, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm15 = _mm_madd_epi16(rxmm15, _mm_load_si128((__m128i *)(pecx+16)));

	rxmm4 = _mm_shuffle_epi32 (rxmm4, 0xFF) ;
	rxmm12 = _mm_shuffle_epi32 (rxmm12, 0xFF) ;

	rxmm4 = _mm_madd_epi16(rxmm4, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm12 = _mm_madd_epi16(rxmm12, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm1 = _mm_add_epi32(rxmm1, rxmm2);
	rxmm9 = _mm_add_epi32(rxmm9, rxmm10);
	
	rxmm2 = rxmm1 ;
	rxmm10 = rxmm9 ;
	
	rxmm2 = _mm_sub_epi32(rxmm2, rxmm0);
	rxmm10 = _mm_sub_epi32(rxmm10, rxmm8);
	
	rxmm2 = _mm_srai_epi32(rxmm2, 12);
	rxmm10 = _mm_srai_epi32(rxmm10, 12);
	
	rxmm2 = _mm_shuffle_epi32 (rxmm2, 0x1B) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm10, 0x1B) ;
	
	rxmm0 = _mm_add_epi32(rxmm0, rxmm1);
	rxmm8 = _mm_add_epi32(rxmm8, rxmm9);
	
	rxmm0 = _mm_srai_epi32(rxmm0, 12);
	rxmm8 = _mm_srai_epi32(rxmm8, 12);

	rxmm5 = _mm_add_epi32(rxmm5, rxmm6);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm14);

	rxmm0 = _mm_packs_epi32(rxmm0, rxmm2);
	rxmm8 = _mm_packs_epi32(rxmm8, rxmm10);

	rxmm4 = _mm_add_epi32(rxmm4, rxmm7);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm15);

	rxmm6 = rxmm5;
	rxmm14 = rxmm13;

	rxmm6 = _mm_sub_epi32(rxmm6, rxmm4); 
	rxmm14 = _mm_sub_epi32(rxmm14, rxmm12); 
	rxmm6 = _mm_srai_epi32(rxmm6, 12);
	rxmm14 = _mm_srai_epi32(rxmm14, 12);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm5); 
	rxmm12 = _mm_add_epi32(rxmm12, rxmm13); 
	rxmm4 = _mm_srai_epi32(rxmm4, 12); 
	rxmm12 = _mm_srai_epi32(rxmm12, 12); 

	rxmm6 = _mm_shuffle_epi32 (rxmm6, 0x1B) ;
	rxmm14 = _mm_shuffle_epi32 (rxmm14, 0x1B) ;
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm6);
	rxmm12 = _mm_packs_epi32(rxmm12, rxmm14);
	
	_mm_storeu_si128((__m128i *)pedx, rxmm0);
	_mm_storeu_si128((__m128i *)(pedx+16), rxmm4);
	_mm_storeu_si128((__m128i *)(pedx+32), rxmm8);
	_mm_storeu_si128((__m128i *)(pedx+48), rxmm12);

	rxmm8 = _mm_loadu_si128((__m128i *)(peax+24)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+24));	// deQ
	rxmm8 = _mm_mullo_epi16(rxmm8, quantCoef);

	pesi = (Fw16s *)(idct_weighting + 96);
	rxmm12 = _mm_loadu_si128((__m128i *)(peax+8)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+8));	// deQ
	rxmm12 = _mm_mullo_epi16(rxmm12, quantCoef);

	pecx = (Fw16s *)(idct_weighting + 32);
//	DCT_8_INV_ROW	//Row 4, tab_i_35 and Row 2, tab_i_17
	rxmm8 = _mm_shufflelo_epi16(rxmm8, 0xD8);
	rxmm8 = _mm_shufflehi_epi16 (rxmm8, 0xD8);
	rxmm11 = _mm_shuffle_epi32 (rxmm8, 0x55) ;
	rxmm9 = _mm_shuffle_epi32 (rxmm8, 0) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm8, 0xAA) ;
	rxmm8 = _mm_shuffle_epi32 (rxmm8, 0xFF) ;
	rxmm9 = _mm_madd_epi16(rxmm9, _mm_load_si128((__m128i *)(pesi)));
	rxmm10 = _mm_madd_epi16(rxmm10, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm11 = _mm_madd_epi16(rxmm11, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm8 = _mm_madd_epi16(rxmm8, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm8 = _mm_add_epi32(rxmm8, rxmm11);
	rxmm12 = _mm_shufflelo_epi16(rxmm12, 0xD8);
	rxmm12 = _mm_shufflehi_epi16 (rxmm12, 0xD8);
	rxmm15 = _mm_set1_epi32 (0x800);	// rounding

	rxmm9 = _mm_add_epi32(rxmm9, rxmm15);
	rxmm14 = _mm_shuffle_epi32 (rxmm12, 0xAA) ;
	rxmm13 = _mm_shuffle_epi32 (rxmm12, 0) ;
	rxmm13 = _mm_madd_epi16(rxmm13, _mm_load_si128((__m128i *)(pecx)));
	rxmm13 = _mm_add_epi32(rxmm13, rxmm15);
	rxmm14 = _mm_madd_epi16(rxmm14, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm15 = _mm_shuffle_epi32 (rxmm12, 0x55) ;
	rxmm15 = _mm_madd_epi16(rxmm15, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm12 = _mm_shuffle_epi32 (rxmm12, 0xFF) ;
	rxmm12 = _mm_madd_epi16(rxmm12, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm9 = _mm_add_epi32(rxmm9, rxmm10);
	rxmm10 = rxmm9 ;
	rxmm10 = _mm_sub_epi32(rxmm10, rxmm8);
	rxmm10 = _mm_srai_epi32(rxmm10, 12);
	rxmm10 = _mm_shuffle_epi32 (rxmm10, 0x1B) ;
	rxmm8 = _mm_add_epi32(rxmm8, rxmm9);
	rxmm8 = _mm_srai_epi32(rxmm8, 12);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm14);
	rxmm8 = _mm_packs_epi32(rxmm8, rxmm10);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm15);
	rxmm14 = rxmm13;
	rxmm14 = _mm_sub_epi32(rxmm14, rxmm12); 
	rxmm14 = _mm_srai_epi32(rxmm14, 12);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm13); 
	rxmm12 = _mm_srai_epi32(rxmm12, 12); 
	rxmm14 = _mm_shuffle_epi32 (rxmm14, 0x1B) ;
	rxmm12 = _mm_packs_epi32(rxmm12, rxmm14);

	_mm_storeu_si128((__m128i *)(pedx+24), rxmm8);
	rxmm0 = _mm_loadu_si128((__m128i *)(peax+40)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+40));	// deQ
	rxmm0 = _mm_mullo_epi16(rxmm0, quantCoef);

	_mm_storeu_si128((__m128i *)(pedx+8), rxmm12);
	rxmm4 = _mm_loadu_si128((__m128i *)(peax+56)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+56));	// deQ
	rxmm4 = _mm_mullo_epi16(rxmm4, quantCoef);


//	DCT_8_INV_ROW	//Row 6, tab_i_35 and Row 8, tab_i_17	
	rxmm0 = _mm_shufflelo_epi16(rxmm0, 0xD8);
	rxmm0 = _mm_shufflehi_epi16 (rxmm0, 0xD8);
	rxmm3 = _mm_shuffle_epi32 (rxmm0, 0x55) ;
	rxmm1 = _mm_shuffle_epi32 (rxmm0, 0) ;
	rxmm2 = _mm_shuffle_epi32 (rxmm0, 0xAA) ;
	rxmm0 = _mm_shuffle_epi32 (rxmm0, 0xFF) ;
	rxmm1 = _mm_madd_epi16(rxmm1, _mm_load_si128((__m128i *)(pesi)));
	rxmm2 = _mm_madd_epi16(rxmm2, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm3 = _mm_madd_epi16(rxmm3, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm0 = _mm_madd_epi16(rxmm0, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm0 = _mm_add_epi32(rxmm0, rxmm3);
	rxmm4 = _mm_shufflelo_epi16(rxmm4, 0xD8);
	rxmm4 = _mm_shufflehi_epi16 (rxmm4, 0xD8);
	rxmm7 = _mm_set1_epi32 (0x800);	// rounding

	rxmm1 = _mm_add_epi32(rxmm1, rxmm7);
	rxmm6 = _mm_shuffle_epi32 (rxmm4, 0xAA) ;
	rxmm5 = _mm_shuffle_epi32 (rxmm4, 0) ;
	rxmm5 = _mm_madd_epi16(rxmm5, _mm_load_si128((__m128i *)(pecx)));
	rxmm5 = _mm_add_epi32(rxmm5, rxmm7);
	rxmm6 = _mm_madd_epi16(rxmm6, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm7 = _mm_shuffle_epi32 (rxmm4, 0x55) ;
	rxmm7 = _mm_madd_epi16(rxmm7, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm4 = _mm_shuffle_epi32 (rxmm4, 0xFF) ;
	rxmm4 = _mm_madd_epi16(rxmm4, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm1 = _mm_add_epi32(rxmm1, rxmm2);
	rxmm2 = rxmm1 ;
	rxmm2 = _mm_sub_epi32(rxmm2, rxmm0);
	rxmm2 = _mm_srai_epi32(rxmm2, 12);
	rxmm2 = _mm_shuffle_epi32 (rxmm2, 0x1B) ;
	rxmm0 = _mm_add_epi32(rxmm0, rxmm1);
	rxmm0 = _mm_srai_epi32(rxmm0, 12);
	rxmm5 = _mm_add_epi32(rxmm5, rxmm6);
	rxmm0 = _mm_packs_epi32(rxmm0, rxmm2);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm7);
	rxmm6 = rxmm5;
	rxmm6 = _mm_sub_epi32(rxmm6, rxmm4); 
	rxmm6 = _mm_srai_epi32(rxmm6, 12);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm5); 
	rxmm4 = _mm_srai_epi32(rxmm4, 12); 
	rxmm6 = _mm_shuffle_epi32 (rxmm6, 0x1B) ;
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm6);

//	DCT_8_INV_COL_8
	rxmm6 = rxmm4;
	rxmm2 = rxmm0;
	rxmm3 = _mm_loadu_si128((__m128i *)(pedx+24));
	rxmm1 = _mm_load_si128((__m128i *)(idct_weighting+144));
	rxmm0 = _mm_mulhi_epi16(rxmm0, rxmm1);
	rxmm5 = _mm_load_si128((__m128i *)(idct_weighting+128));
	rxmm1 = _mm_mulhi_epi16(rxmm1, rxmm3);
	rxmm1 = _mm_adds_epi16(rxmm1, rxmm3);
	rxmm4 = _mm_mulhi_epi16(rxmm4, rxmm5);
	rxmm7 = _mm_loadu_si128((__m128i *)(pedx+48));
	rxmm5 = _mm_mulhi_epi16(rxmm5, _mm_loadu_si128((__m128i *)(pedx+8)));
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm6);
	rxmm6 = rxmm5;
	rxmm4 = _mm_adds_epi16(rxmm4, _mm_loadu_si128((__m128i *)(pedx+8)));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm2);
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm3);
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm1);
	rxmm1 = rxmm0;
	rxmm3 = _mm_load_si128((__m128i *)(idct_weighting+136));
	rxmm7 = _mm_mulhi_epi16(rxmm7, rxmm3);
	rxmm3 = _mm_mulhi_epi16(rxmm3, _mm_loadu_si128((__m128i *)(pedx+16)));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm4);
	rxmm4 = _mm_subs_epi16(rxmm4, rxmm1);
	rxmm0 = _mm_adds_epi16(rxmm0, _mm_set1_epi16 (1));

	_mm_storeu_si128((__m128i *)(pedx+56), rxmm0);
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm2);
	rxmm5 = _mm_adds_epi16(rxmm5, _mm_set1_epi16 (1));
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm2);
	_mm_storeu_si128((__m128i *)(pedx+24), rxmm6);
	rxmm1 = rxmm4;
	rxmm0 = _mm_load_si128((__m128i *)(idct_weighting+152));
	rxmm2 = rxmm0;
	rxmm4 = _mm_adds_epi16(rxmm4, rxmm5);
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm5);
	rxmm7 = _mm_adds_epi16(rxmm7, _mm_loadu_si128((__m128i *)(pedx+16)));
	rxmm3 = _mm_subs_epi16(rxmm3, _mm_loadu_si128((__m128i *)(pedx+48)));
	rxmm6 = _mm_loadu_si128((__m128i *)(pedx));
	rxmm0 = _mm_mulhi_epi16(rxmm0, rxmm1);
	rxmm5 = _mm_loadu_si128((__m128i *)(pedx+32));
	rxmm5 = _mm_adds_epi16(rxmm5, rxmm6);
	rxmm6 = _mm_subs_epi16(rxmm6, _mm_loadu_si128((__m128i *)(pedx+32)));
	rxmm2 = _mm_mulhi_epi16(rxmm2, rxmm4);
	rxmm4 = _mm_adds_epi16(rxmm4, rxmm2);
	rxmm2 = rxmm5;
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm7);
	rxmm4 = _mm_or_si128(rxmm4, _mm_set1_epi16 (1));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm1);
	rxmm0 = _mm_or_si128(rxmm0, _mm_set1_epi16 (1));
	rxmm5 = _mm_adds_epi16(rxmm5, rxmm7);
	rxmm5 = _mm_adds_epi16(rxmm5, _mm_set1_epi16 (16));
	rxmm1 = rxmm6;
	rxmm7 = _mm_loadu_si128((__m128i *)(pedx+56));

	rxmm7 = _mm_adds_epi16(rxmm7, rxmm5);
	rxmm7 = _mm_srai_epi16(rxmm7, 5);

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable));	// deQ
//	rxmm7 = _mm_mullo_epi16(rxmm7, quantCoef);				// (r0 * quantCoef) & 0xffff

	_mm_storeu_si128((__m128i *)(pedi), rxmm7);		// row 0

	rxmm6 = _mm_adds_epi16(rxmm6, rxmm3);
	rxmm6 = _mm_adds_epi16(rxmm6, _mm_set1_epi16 (16));
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm3);
	rxmm1 = _mm_adds_epi16(rxmm1, _mm_set1_epi16 (15));
	rxmm7 = rxmm1;
	rxmm3 = rxmm6;
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm4);
	rxmm2 = _mm_adds_epi16(rxmm2, _mm_set1_epi16 (15));
	rxmm6 = _mm_srai_epi16(rxmm6, 5);

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+8));	// deQ
//	rxmm6 = _mm_mullo_epi16(rxmm6, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+8), rxmm6);		// row 1

	rxmm1 = _mm_adds_epi16(rxmm1, rxmm0);
	rxmm1 = _mm_srai_epi16(rxmm1, 5);

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+16));	// deQ
//	rxmm1 = _mm_mullo_epi16(rxmm1, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+16), rxmm1);	// row 2

	rxmm1 = _mm_loadu_si128((__m128i *)(pedx+24));
	rxmm6 = rxmm1;
	rxmm7 = _mm_subs_epi16(rxmm7, rxmm0);
	rxmm7 = _mm_srai_epi16(rxmm7, 5);

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+40));	// deQ
//	rxmm7 = _mm_mullo_epi16(rxmm7, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+40), rxmm7);	// row 5

	rxmm5 = _mm_subs_epi16(rxmm5, _mm_loadu_si128((__m128i *)(pedx+56)));
	rxmm5 = _mm_srai_epi16(rxmm5, 5); 

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+56));	// deQ
//	rxmm5 = _mm_mullo_epi16(rxmm5, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+56), rxmm5);	// row 7

	rxmm3 = _mm_subs_epi16(rxmm3, rxmm4);
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm2);
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm1);
	rxmm6 = _mm_srai_epi16(rxmm6, 5);

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+24));	// deQ
//	rxmm6 = _mm_mullo_epi16(rxmm6, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+24), rxmm6);	// row 3

	rxmm2 = _mm_srai_epi16(rxmm2, 5); 

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+32));	// deQ
//	rxmm2 = _mm_mullo_epi16(rxmm2, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+32), rxmm2);	// row 4

	rxmm3 = _mm_srai_epi16(rxmm3, 5); 

//	quantCoef	 = _mm_load_si128((__m128i *)(pQuantInvTable+48));	// deQ
//	rxmm3 = _mm_mullo_epi16(rxmm3, quantCoef);

	_mm_storeu_si128((__m128i *)(pedi+48), rxmm3);	// row 6

	return(0);
}

int IdctQuant_LS_SSE2(const Fw16s *pSrc, Fw8u *pDst, int dstStp, const Fw16u *pQuantInvTable)
{
	__m128i rxmm0,  rxmm1,  rxmm2,  rxmm3,  rxmm4,  rxmm5,  rxmm6,  rxmm7;
	__m128i rxmm8,  rxmm9,  rxmm10, rxmm11, rxmm12, rxmm13, rxmm14, rxmm15;
	__m128i quantCoef;
	Fw16s *peax = (Fw16s*)pSrc;
	Fw16s *pedx;
	Fw16s *pesi, *pecx;
	Fw8u *pedi  = pDst;

    pedx = (Fw16s *) fwMalloc(128); //64 array of Fw16s type

	pesi = (Fw16s *)idct_weighting;
	pecx = (Fw16s *)(idct_weighting+64);

	rxmm0 = _mm_loadu_si128((__m128i *)(peax)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable));	// deQ
	rxmm0 = _mm_mullo_epi16(rxmm0, quantCoef);

	rxmm4 = _mm_loadu_si128((__m128i *)(peax+16)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+16));	// deQ
	rxmm4 = _mm_mullo_epi16(rxmm4, quantCoef);

	rxmm8 = _mm_loadu_si128((__m128i *)(peax+32)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+32));	// deQ
	rxmm8 = _mm_mullo_epi16(rxmm8, quantCoef);

	rxmm12 = _mm_loadu_si128((__m128i *)(peax+48)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+48));	// deQ
	rxmm12 = _mm_mullo_epi16(rxmm12, quantCoef);


//	DCT_8_INV_ROW	//Row 1, tab_i_04 and Row 3, tab_i_26
	rxmm0 = _mm_shufflelo_epi16(rxmm0, 0xD8);
//	DCT_8_INV_ROW	//Row 5, tab_i_04 and Row 7, tab_i_26
	rxmm8 = _mm_shufflelo_epi16(rxmm8, 0xD8);

	rxmm0 = _mm_shufflehi_epi16 (rxmm0, 0xD8);
	rxmm8 = _mm_shufflehi_epi16 (rxmm8, 0xD8);
	rxmm3 = _mm_shuffle_epi32 (rxmm0, 0x55) ;
	rxmm11 = _mm_shuffle_epi32 (rxmm8, 0x55) ;
	rxmm1 = _mm_shuffle_epi32 (rxmm0, 0) ;
	rxmm9 = _mm_shuffle_epi32 (rxmm8, 0) ;
	rxmm2 = _mm_shuffle_epi32 (rxmm0, 0xAA) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm8, 0xAA) ;
	rxmm0 = _mm_shuffle_epi32 (rxmm0, 0xFF) ;
	rxmm8 = _mm_shuffle_epi32 (rxmm8, 0xFF) ;
	rxmm1 = _mm_madd_epi16(rxmm1, _mm_load_si128((__m128i *)(pesi)));
    rxmm9 = _mm_madd_epi16(rxmm9, _mm_load_si128((__m128i *)(pesi)));
	rxmm2 = _mm_madd_epi16(rxmm2, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm10 = _mm_madd_epi16(rxmm10, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm3 = _mm_madd_epi16(rxmm3, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm11 = _mm_madd_epi16(rxmm11, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm0 = _mm_madd_epi16(rxmm0, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm8 = _mm_madd_epi16(rxmm8, _mm_load_si128((__m128i *)(pesi+24)));

	rxmm0 = _mm_add_epi32(rxmm0, rxmm3);
	rxmm8 = _mm_add_epi32(rxmm8, rxmm11);

	rxmm4 = _mm_shufflelo_epi16(rxmm4, 0xD8);
	rxmm12 = _mm_shufflelo_epi16(rxmm12, 0xD8);
	rxmm4 = _mm_shufflehi_epi16 (rxmm4, 0xD8);
	rxmm12 = _mm_shufflehi_epi16 (rxmm12, 0xD8);

	rxmm7 = _mm_set1_epi32 (0x800);	// rounding
	//rxmm15 = _mm_set1_epi32 (0x800);	// rounding

	rxmm1 = _mm_add_epi32(rxmm1, rxmm7);
	rxmm9 = _mm_add_epi32(rxmm9, rxmm7);

	rxmm6 = _mm_shuffle_epi32 (rxmm4, 0xAA) ;
	rxmm14 = _mm_shuffle_epi32 (rxmm12, 0xAA) ;
	rxmm5 = _mm_shuffle_epi32 (rxmm4, 0) ;
	rxmm13 = _mm_shuffle_epi32 (rxmm12, 0) ;

	rxmm5 = _mm_madd_epi16(rxmm5, _mm_load_si128((__m128i *)(pecx)));
	rxmm13 = _mm_madd_epi16(rxmm13, _mm_load_si128((__m128i *)(pecx)));
	rxmm5 = _mm_add_epi32(rxmm5, rxmm7);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm7);
	rxmm6 = _mm_madd_epi16(rxmm6, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm14 = _mm_madd_epi16(rxmm14, _mm_load_si128((__m128i *)(pecx+8)));

	rxmm7 = _mm_shuffle_epi32 (rxmm4, 0x55) ;
	rxmm15 = _mm_shuffle_epi32 (rxmm12, 0x55) ;

	rxmm7 = _mm_madd_epi16(rxmm7, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm15 = _mm_madd_epi16(rxmm15, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm4 = _mm_shuffle_epi32 (rxmm4, 0xFF) ;
	rxmm12 = _mm_shuffle_epi32 (rxmm12, 0xFF) ;

	rxmm4 = _mm_madd_epi16(rxmm4, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm12 = _mm_madd_epi16(rxmm12, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm1 = _mm_add_epi32(rxmm1, rxmm2);
	rxmm9 = _mm_add_epi32(rxmm9, rxmm10);
	
	rxmm2 = rxmm1 ;
	rxmm10 = rxmm9 ;
	
	rxmm2 = _mm_sub_epi32(rxmm2, rxmm0);
	rxmm10 = _mm_sub_epi32(rxmm10, rxmm8);
	
	rxmm2 = _mm_srai_epi32(rxmm2, 12);
	rxmm10 = _mm_srai_epi32(rxmm10, 12);
	
	rxmm2 = _mm_shuffle_epi32 (rxmm2, 0x1B) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm10, 0x1B) ;
	
	rxmm0 = _mm_add_epi32(rxmm0, rxmm1);
	rxmm8 = _mm_add_epi32(rxmm8, rxmm9);
	
	rxmm0 = _mm_srai_epi32(rxmm0, 12);
	rxmm8 = _mm_srai_epi32(rxmm8, 12);

	rxmm5 = _mm_add_epi32(rxmm5, rxmm6);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm14);

	rxmm0 = _mm_packs_epi32(rxmm0, rxmm2);
	rxmm8 = _mm_packs_epi32(rxmm8, rxmm10);

	rxmm4 = _mm_add_epi32(rxmm4, rxmm7);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm15);

	rxmm6 = rxmm5;
	rxmm14 = rxmm13;

	rxmm6 = _mm_sub_epi32(rxmm6, rxmm4); 
	rxmm14 = _mm_sub_epi32(rxmm14, rxmm12); 
	rxmm6 = _mm_srai_epi32(rxmm6, 12);
	rxmm14 = _mm_srai_epi32(rxmm14, 12);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm5); 
	rxmm12 = _mm_add_epi32(rxmm12, rxmm13); 
	rxmm4 = _mm_srai_epi32(rxmm4, 12); 
	rxmm12 = _mm_srai_epi32(rxmm12, 12); 

	rxmm6 = _mm_shuffle_epi32 (rxmm6, 0x1B) ;
	rxmm14 = _mm_shuffle_epi32 (rxmm14, 0x1B) ;
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm6);
	rxmm12 = _mm_packs_epi32(rxmm12, rxmm14);
	
	_mm_store_si128((__m128i *)pedx, rxmm0);
	_mm_store_si128((__m128i *)(pedx+16), rxmm4);
	_mm_store_si128((__m128i *)(pedx+32), rxmm8);
	_mm_store_si128((__m128i *)(pedx+48), rxmm12);

	rxmm8 = _mm_loadu_si128((__m128i *)(peax+24));
	
	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+24));	// deQ
	rxmm8 = _mm_mullo_epi16(rxmm8, quantCoef);

	pesi = (Fw16s *)(idct_weighting + 96);
	rxmm12 = _mm_loadu_si128((__m128i *)(peax+8)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+8));	// deQ
	rxmm12 = _mm_mullo_epi16(rxmm12, quantCoef);


	pecx = (Fw16s *)(idct_weighting + 32);
//	DCT_8_INV_ROW	//Row 4, tab_i_35 and Row 2, tab_i_17
	rxmm8 = _mm_shufflelo_epi16(rxmm8, 0xD8);
	rxmm8 = _mm_shufflehi_epi16 (rxmm8, 0xD8);
	rxmm11 = _mm_shuffle_epi32 (rxmm8, 0x55) ;
	rxmm9 = _mm_shuffle_epi32 (rxmm8, 0) ;
	rxmm10 = _mm_shuffle_epi32 (rxmm8, 0xAA) ;
	rxmm8 = _mm_shuffle_epi32 (rxmm8, 0xFF) ;
	rxmm9 = _mm_madd_epi16(rxmm9, _mm_load_si128((__m128i *)(pesi)));
	rxmm10 = _mm_madd_epi16(rxmm10, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm11 = _mm_madd_epi16(rxmm11, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm8 = _mm_madd_epi16(rxmm8, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm8 = _mm_add_epi32(rxmm8, rxmm11);
	rxmm12 = _mm_shufflelo_epi16(rxmm12, 0xD8);
	rxmm12 = _mm_shufflehi_epi16 (rxmm12, 0xD8);
	rxmm15 = _mm_set1_epi32 (0x800);	// rounding

	rxmm9 = _mm_add_epi32(rxmm9, rxmm15);
	rxmm14 = _mm_shuffle_epi32 (rxmm12, 0xAA) ;
	rxmm13 = _mm_shuffle_epi32 (rxmm12, 0) ;
	rxmm13 = _mm_madd_epi16(rxmm13, _mm_load_si128((__m128i *)(pecx)));
	rxmm13 = _mm_add_epi32(rxmm13, rxmm15);
	rxmm14 = _mm_madd_epi16(rxmm14, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm15 = _mm_shuffle_epi32 (rxmm12, 0x55) ;
	rxmm15 = _mm_madd_epi16(rxmm15, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm12 = _mm_shuffle_epi32 (rxmm12, 0xFF) ;
	rxmm12 = _mm_madd_epi16(rxmm12, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm9 = _mm_add_epi32(rxmm9, rxmm10);
	rxmm10 = rxmm9 ;
	rxmm10 = _mm_sub_epi32(rxmm10, rxmm8);
	rxmm10 = _mm_srai_epi32(rxmm10, 12);
	rxmm10 = _mm_shuffle_epi32 (rxmm10, 0x1B) ;
	rxmm8 = _mm_add_epi32(rxmm8, rxmm9);
	rxmm8 = _mm_srai_epi32(rxmm8, 12);
	rxmm13 = _mm_add_epi32(rxmm13, rxmm14);
	rxmm8 = _mm_packs_epi32(rxmm8, rxmm10);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm15);
	rxmm14 = rxmm13;
	rxmm14 = _mm_sub_epi32(rxmm14, rxmm12); 
	rxmm14 = _mm_srai_epi32(rxmm14, 12);
	rxmm12 = _mm_add_epi32(rxmm12, rxmm13); 
	rxmm12 = _mm_srai_epi32(rxmm12, 12); 
	rxmm14 = _mm_shuffle_epi32 (rxmm14, 0x1B) ;
	rxmm12 = _mm_packs_epi32(rxmm12, rxmm14);

	_mm_store_si128((__m128i *)(pedx+24), rxmm8);
	rxmm0 = _mm_loadu_si128((__m128i *)(peax+40)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+40));	// deQ
	rxmm0 = _mm_mullo_epi16(rxmm0, quantCoef);

	_mm_store_si128((__m128i *)(pedx+8), rxmm12);
	rxmm4 = _mm_loadu_si128((__m128i *)(peax+56)); 

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantInvTable+56));	// deQ
	rxmm4 = _mm_mullo_epi16(rxmm4, quantCoef);

//	DCT_8_INV_ROW	//Row 6, tab_i_35 and Row 8, tab_i_17	
	rxmm0 = _mm_shufflelo_epi16(rxmm0, 0xD8);
	rxmm0 = _mm_shufflehi_epi16 (rxmm0, 0xD8);
	rxmm3 = _mm_shuffle_epi32 (rxmm0, 0x55) ;
	rxmm1 = _mm_shuffle_epi32 (rxmm0, 0) ;
	rxmm2 = _mm_shuffle_epi32 (rxmm0, 0xAA) ;
	rxmm0 = _mm_shuffle_epi32 (rxmm0, 0xFF) ;
	rxmm1 = _mm_madd_epi16(rxmm1, _mm_load_si128((__m128i *)(pesi)));
	rxmm2 = _mm_madd_epi16(rxmm2, _mm_load_si128((__m128i *)(pesi+8)));
	rxmm3 = _mm_madd_epi16(rxmm3, _mm_load_si128((__m128i *)(pesi+16)));
	rxmm0 = _mm_madd_epi16(rxmm0, _mm_load_si128((__m128i *)(pesi+24)));
	rxmm0 = _mm_add_epi32(rxmm0, rxmm3);
	rxmm4 = _mm_shufflelo_epi16(rxmm4, 0xD8);
	rxmm4 = _mm_shufflehi_epi16 (rxmm4, 0xD8);
	rxmm7 = _mm_set1_epi32 (0x800);	// rounding

	rxmm1 = _mm_add_epi32(rxmm1, rxmm7);
	rxmm6 = _mm_shuffle_epi32 (rxmm4, 0xAA) ;
	rxmm5 = _mm_shuffle_epi32 (rxmm4, 0) ;
	rxmm5 = _mm_madd_epi16(rxmm5, _mm_load_si128((__m128i *)(pecx)));
	rxmm5 = _mm_add_epi32(rxmm5, rxmm7);
	rxmm6 = _mm_madd_epi16(rxmm6, _mm_load_si128((__m128i *)(pecx+8)));
	rxmm7 = _mm_shuffle_epi32 (rxmm4, 0x55) ;
	rxmm7 = _mm_madd_epi16(rxmm7, _mm_load_si128((__m128i *)(pecx+16)));
	rxmm4 = _mm_shuffle_epi32 (rxmm4, 0xFF) ;
	rxmm4 = _mm_madd_epi16(rxmm4, _mm_load_si128((__m128i *)(pecx+24)));
	rxmm1 = _mm_add_epi32(rxmm1, rxmm2);
	rxmm2 = rxmm1 ;
	rxmm2 = _mm_sub_epi32(rxmm2, rxmm0);
	rxmm2 = _mm_srai_epi32(rxmm2, 12);
	rxmm2 = _mm_shuffle_epi32 (rxmm2, 0x1B) ;
	rxmm0 = _mm_add_epi32(rxmm0, rxmm1);
	rxmm0 = _mm_srai_epi32(rxmm0, 12);
	rxmm5 = _mm_add_epi32(rxmm5, rxmm6);
	rxmm0 = _mm_packs_epi32(rxmm0, rxmm2);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm7);
	rxmm6 = rxmm5;
	rxmm6 = _mm_sub_epi32(rxmm6, rxmm4); 
	rxmm6 = _mm_srai_epi32(rxmm6, 12);
	rxmm4 = _mm_add_epi32(rxmm4, rxmm5); 
	rxmm4 = _mm_srai_epi32(rxmm4, 12); 
	rxmm6 = _mm_shuffle_epi32 (rxmm6, 0x1B) ;
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm6);

//	DCT_8_INV_COL_8
	rxmm6 = rxmm4;
	rxmm2 = rxmm0;
	rxmm3 = _mm_load_si128((__m128i *)(pedx+24));
	rxmm1 = _mm_load_si128((__m128i *)(idct_weighting+144));
	rxmm0 = _mm_mulhi_epi16(rxmm0, rxmm1);
	rxmm5 = _mm_load_si128((__m128i *)(idct_weighting+128));
	rxmm1 = _mm_mulhi_epi16(rxmm1, rxmm3);
	rxmm1 = _mm_adds_epi16(rxmm1, rxmm3);
	rxmm4 = _mm_mulhi_epi16(rxmm4, rxmm5);
	rxmm7 = _mm_load_si128((__m128i *)(pedx+48));
	rxmm5 = _mm_mulhi_epi16(rxmm5, _mm_load_si128((__m128i *)(pedx+8)));
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm6);
	rxmm6 = rxmm5;
	rxmm4 = _mm_adds_epi16(rxmm4, _mm_load_si128((__m128i *)(pedx+8)));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm2);
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm3);
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm1);
	rxmm1 = rxmm0;
	rxmm3 = _mm_load_si128((__m128i *)(idct_weighting+136));
	rxmm7 = _mm_mulhi_epi16(rxmm7, rxmm3);
	rxmm3 = _mm_mulhi_epi16(rxmm3, _mm_load_si128((__m128i *)(pedx+16)));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm4);
	rxmm4 = _mm_subs_epi16(rxmm4, rxmm1);
	rxmm0 = _mm_adds_epi16(rxmm0, _mm_set1_epi16 (1));

	_mm_store_si128((__m128i *)(pedx+56), rxmm0);
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm2);
	rxmm5 = _mm_adds_epi16(rxmm5, _mm_set1_epi16 (1));
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm2);
	_mm_store_si128((__m128i *)(pedx+24), rxmm6);
	rxmm1 = rxmm4;
	rxmm0 = _mm_load_si128((__m128i *)(idct_weighting+152));
	rxmm2 = rxmm0;
	rxmm4 = _mm_adds_epi16(rxmm4, rxmm5);
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm5);
	rxmm7 = _mm_adds_epi16(rxmm7, _mm_load_si128((__m128i *)(pedx+16)));
	rxmm3 = _mm_subs_epi16(rxmm3, _mm_load_si128((__m128i *)(pedx+48)));
	rxmm6 = _mm_load_si128((__m128i *)(pedx));
	rxmm0 = _mm_mulhi_epi16(rxmm0, rxmm1);
	rxmm5 = _mm_load_si128((__m128i *)(pedx+32));
	rxmm5 = _mm_adds_epi16(rxmm5, rxmm6);
	rxmm6 = _mm_subs_epi16(rxmm6, _mm_load_si128((__m128i *)(pedx+32)));
	rxmm2 = _mm_mulhi_epi16(rxmm2, rxmm4);
	rxmm4 = _mm_adds_epi16(rxmm4, rxmm2);
	rxmm2 = rxmm5;
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm7);
	rxmm4 = _mm_or_si128(rxmm4, _mm_set1_epi16 (1));
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm1);
	rxmm0 = _mm_or_si128(rxmm0, _mm_set1_epi16 (1));
	rxmm5 = _mm_adds_epi16(rxmm5, rxmm7);
	rxmm5 = _mm_adds_epi16(rxmm5, _mm_set1_epi16 (16));
	rxmm1 = rxmm6;
	rxmm7 = _mm_load_si128((__m128i *)(pedx+56));

	rxmm7 = _mm_adds_epi16(rxmm7, rxmm5);
	rxmm7 = _mm_srai_epi16(rxmm7, 5);

//	_mm_storeu_si128((__m128i *)(pedi), rxmm7);		// row 0
	rxmm7 = _mm_adds_epi16(rxmm7, _mm_set1_epi16 (128));	// Level Scale

	//rxmm7 = _mm_packs_epi16(rxmm7, rxmm7);
    rxmm7 = _mm_packus_epi16(rxmm7, rxmm7);
	_mm_storel_epi64((__m128i *)(pedi), rxmm7);		// row 0

	rxmm6 = _mm_adds_epi16(rxmm6, rxmm3);
	rxmm6 = _mm_adds_epi16(rxmm6, _mm_set1_epi16 (16));
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm3);
	rxmm1 = _mm_adds_epi16(rxmm1, _mm_set1_epi16 (15));
	rxmm7 = rxmm1;
	rxmm3 = rxmm6;
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm4);
	rxmm2 = _mm_adds_epi16(rxmm2, _mm_set1_epi16 (15));
	rxmm6 = _mm_srai_epi16(rxmm6, 5);

//	_mm_storeu_si128((__m128i *)(pedi+8), rxmm6);		// row 1
	rxmm6 = _mm_adds_epi16(rxmm6, _mm_set1_epi16 (128));	// Level Scale

	rxmm6 = _mm_packus_epi16(rxmm6, rxmm6);
	_mm_storel_epi64((__m128i *)(pedi+dstStp), rxmm6);		// row 1

	rxmm1 = _mm_adds_epi16(rxmm1, rxmm0);
	rxmm1 = _mm_srai_epi16(rxmm1, 5);

//	_mm_storeu_si128((__m128i *)(pedi+16), rxmm1);	// row 2
	rxmm1 = _mm_adds_epi16(rxmm1, _mm_set1_epi16 (128));	// Level Scale

	rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*2), rxmm1);	// row 2

	rxmm1 = _mm_load_si128((__m128i *)(pedx+24));
	rxmm6 = rxmm1;
	rxmm7 = _mm_subs_epi16(rxmm7, rxmm0);
	rxmm7 = _mm_srai_epi16(rxmm7, 5);

//	_mm_storeu_si128((__m128i *)(pedi+40), rxmm7);	// row 5
	rxmm7 = _mm_adds_epi16(rxmm7, _mm_set1_epi16 (128));	// Level Scale

	rxmm7 = _mm_packus_epi16(rxmm7, rxmm7);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*5), rxmm7);	// row 5

	rxmm5 = _mm_subs_epi16(rxmm5, _mm_load_si128((__m128i *)(pedx+56)));
	rxmm5 = _mm_srai_epi16(rxmm5, 5); 

//	_mm_storeu_si128((__m128i *)(pedi+56), rxmm5);	// row 7
	rxmm5 = _mm_adds_epi16(rxmm5, _mm_set1_epi16 (128));	// Level Scale

	rxmm5 = _mm_packus_epi16(rxmm5, rxmm5);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*7), rxmm5);	// row 7

	rxmm3 = _mm_subs_epi16(rxmm3, rxmm4);
	rxmm6 = _mm_adds_epi16(rxmm6, rxmm2);
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm1);
	rxmm6 = _mm_srai_epi16(rxmm6, 5);

//	_mm_storeu_si128((__m128i *)(pedi+24), rxmm6);	// row 3
	rxmm6 = _mm_adds_epi16(rxmm6, _mm_set1_epi16 (128));	// Level Scale

	rxmm6 = _mm_packus_epi16(rxmm6, rxmm6);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*3), rxmm6);	// row 3

	rxmm2 = _mm_srai_epi16(rxmm2, 5); 

//	_mm_storeu_si128((__m128i *)(pedi+32), rxmm2);	// row 4
	rxmm2 = _mm_adds_epi16(rxmm2, _mm_set1_epi16 (128));	// Level Scale

	rxmm2 = _mm_packus_epi16(rxmm2, rxmm2);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*4), rxmm2);	// row 4

	rxmm3 = _mm_srai_epi16(rxmm3, 5); 

//	_mm_storeu_si128((__m128i *)(pedi+48), rxmm3);	// row 6
	rxmm3 = _mm_adds_epi16(rxmm3, _mm_set1_epi16 (128));	// Level Scale
	rxmm3 = _mm_packus_epi16(rxmm3, rxmm3);
	_mm_storel_epi64((__m128i *)(pedi+dstStp*6), rxmm3);	// row 6

    fwFree(pedx);

	return(0);
}

extern const Fw16s fdct_weighting[160];
//SYS_FORCEALIGN_16 static const short fdct_weighting[]=
//{
//	16384,  16384,  22725,  19266,  21407,   8867,  19266,  -4520,   // 04
//	16384,  16384,  12873,   4520,	-8867, -21407, -22725, -12873,   
//	16384, -16384,  12873, -22725,	 8867, -21407,   4520, -12873,   
//	-16384,  16384,   4520,  19266,	21407,  -8867,  19266, -22725,   
//
//	22725,  22725,  31521,  26722,  29692,  12299,  26722,  -6270,   // 17
//	22725,  22725,  17855,   6270,  -12299, -29692, -31521, -17855,   
//	22725, -22725,  17855, -31521,  12299, -29692,   6270, -17855,   
//	-22725,  22725,   6270,  26722, 29692, -12299,  26722, -31521,   
//
//	21407,  21407,  29692,  25172,  27969,  11585,  25172,  -5906,   // 26
//	21407,  21407,  16819,   5906,  -11585, -27969, -29692, -16819,   
//	21407, -21407,  16819, -29692,  11585, -27969,   5906, -16819,   
//	-21407,  21407,   5906,  25172, 27969, -11585,  25172, -29692,   
//
//	19266,  19266,  26722,  22654,  25172,  10426,  22654,  -5315,   //35
//	19266,  19266,  15137,   5315,  -10426, -25172, -26722, -15137,   
//	19266, -19266,  15137, -26722,  10426, -25172,   5315, -15137,   
//	-19266,  19266,   5315,  22654, 25172, -10426,  22654, -26722,   
//
//	13036,  13036,  13036,  13036,  13036,  13036,  13036,  13036, 
//	27146,  27146,  27146,  27146,  27146,  27146,  27146,  27146,
//	-21746, -21746, -21746, -21746, -21746, -21746, -21746, -21746, // tg * (2<<16) + 0.5
//	23170,  23170,  23170,  23170,  23170,  23170,  23170,  23170   //cos * (2<<15) + 0.5
//}; 

void FdctQuant_SSE2(const Fw16s *pSrc, Fw16s *pDst, const Fw16u *pQuantFwdTable)
{
	__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7;
	__m128i rxmm8, rxmm9, rxmm10, rxmm11, rxmm12, rxmm13, rxmm14, rxmm15;
	__m128i short_one, weighting;
	__m128i quantCoef;
	Fw16s *peax = (Fw16s *)pSrc;
	Fw16s *pedx = pDst;

	rxmm0 = _mm_loadu_si128((__m128i *)(peax));
	rxmm1 = _mm_loadu_si128((__m128i *)(peax+8));
	rxmm2 = _mm_loadu_si128((__m128i *)(peax+16));
	rxmm3 = _mm_loadu_si128((__m128i *)(peax+24));
	rxmm4 = _mm_loadu_si128((__m128i *)(peax+32));
	rxmm5 = _mm_loadu_si128((__m128i *)(peax+40));
	rxmm6 = _mm_loadu_si128((__m128i *)(peax+48));
	rxmm7 = _mm_loadu_si128((__m128i *)(peax+56));
	
	peax = (Fw16s *) pQuantFwdTable;			// eax = pQuantFwdTable;

	rxmm8 = rxmm0;
	rxmm0 = _mm_subs_epi16(rxmm0, rxmm7);			//rxmm0 = 0-7
	rxmm8 = _mm_adds_epi16(rxmm8, rxmm7);			//rxmm7 = 0+7
	
	rxmm9 = rxmm1;
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm6);			//rxmm1 = 1-6
	rxmm9 = _mm_adds_epi16(rxmm9, rxmm6);			//rxmm6 = 1+6

	rxmm10 = rxmm2;
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm5);			//rxmm2 = 2-5
	rxmm10 = _mm_adds_epi16(rxmm10, rxmm5);		//rxmm5 = 2+5

	rxmm11 = rxmm3;
	rxmm3 = _mm_subs_epi16(rxmm3, rxmm4);			//rxmm3 = 3-4
	rxmm11 = _mm_adds_epi16(rxmm11, rxmm4);		//rxmm4 = 3+4

	// ================= Even ================================================ 

	rxmm8 = _mm_slli_epi16(rxmm8, 3);				
	rxmm15 = rxmm8;
	rxmm9 = _mm_slli_epi16(rxmm9, 3);				
	rxmm10 = _mm_slli_epi16(rxmm10, 3);				
	rxmm13 = rxmm10;
	rxmm11 = _mm_slli_epi16(rxmm11, 3);				
	//rxmm11 = rxmm11;

	rxmm4 = _mm_adds_epi16(rxmm9, rxmm13);		// rxmm4 = 1+6+2+5
	rxmm5 = _mm_adds_epi16(rxmm15, rxmm11);	// rxmm5 = 0+7+3+4

	rxmm8 = _mm_adds_epi16(rxmm5, rxmm4);
	rxmm12 = _mm_subs_epi16(rxmm5, rxmm4);
	
	rxmm6 = _mm_subs_epi16(rxmm9, rxmm13);		// rxmm6 = 1+6-(2+5)
	rxmm7 = _mm_subs_epi16(rxmm15, rxmm11);	// rxmm7 = 0+7-(3+4)

	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 136));

	rxmm4 = _mm_mulhi_epi16(rxmm6, weighting);	// rxmm4 = mul1625
	rxmm5 = _mm_mulhi_epi16(rxmm7, weighting);	// rxmm5 = mul0734

	rxmm4 = _mm_adds_epi16(rxmm4, rxmm7);
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm6);

	short_one = _mm_set1_epi16 (1);

	rxmm10 = _mm_or_si128(rxmm4, short_one);
	rxmm14 = _mm_or_si128(rxmm5, short_one);
	
//	process rxmm8 and rxmm12 and store them in dst+12 and dst+32;
	rxmm9  = _mm_shufflehi_epi16(rxmm8, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm12, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm8, rxmm12);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4  = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9  = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5  = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm8  = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6  = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm12 = _mm_unpackhi_epi64(rxmm11, rxmm11);	


	rxmm9  = _mm_load_si128((__m128i *)(fdct_weighting));	
	rxmm4  = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7  = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9  = _mm_load_si128((__m128i *)(fdct_weighting+8));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm4  = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+16));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+24));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
//	rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)pedx, rxmm4);				// row-0
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+32));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
//	rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+32), rxmm7);		// row-4

//	process rxmm10 and rxmm14 and store them in dst+16 and dst+48;
	rxmm9 = _mm_shufflehi_epi16(rxmm10, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm14, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm10, rxmm14);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm10 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm14 = _mm_unpackhi_epi64(rxmm11, rxmm11);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+64));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+72));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+80));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+88));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+16));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
//	rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+16), rxmm4);		// row-2
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+48));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
//	rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+48), rxmm7);		// row-6
	
	// ================= Odd ================================================ 

	rxmm8 = rxmm0;
	rxmm10 = rxmm1;
	rxmm12 = rxmm2;
	rxmm14 = rxmm3;

	rxmm8  = _mm_slli_epi16(rxmm8, 3);
	rxmm10 = _mm_slli_epi16(rxmm10, 4);
	rxmm12 = _mm_slli_epi16(rxmm12, 4);
	rxmm14 = _mm_slli_epi16(rxmm14, 3);

	rxmm0 = _mm_subs_epi16(rxmm10, rxmm12);		// rxmm0 = t2+t4
	rxmm10 = _mm_adds_epi16(rxmm10, rxmm12);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 152));
	rxmm10 = _mm_mulhi_epi16(rxmm10, weighting);
	rxmm0 = _mm_mulhi_epi16(rxmm0, weighting);
	
	rxmm1 = _mm_subs_epi16(rxmm14, rxmm0);		// rxmm1 = t6-rxmm0
	rxmm14 = _mm_adds_epi16(rxmm14, rxmm0);
	
	rxmm10 = _mm_or_si128(rxmm10, short_one);	// rxmm3 = t0-t2
	rxmm3 = _mm_subs_epi16(rxmm8, rxmm10);
	rxmm12 = _mm_adds_epi16(rxmm8, rxmm10);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 128) );
	rxmm8 = _mm_mulhi_epi16(weighting, rxmm14);
	rxmm8 = _mm_adds_epi16(rxmm8, rxmm12);
	rxmm12 = _mm_mulhi_epi16(rxmm12,weighting);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 144));
	rxmm0 = _mm_mulhi_epi16(weighting, rxmm1);	
	rxmm2 = _mm_mulhi_epi16(weighting, rxmm3);	// rxmm2 = tg_3_6 * rxmm1
	rxmm2 = _mm_adds_epi16(rxmm2, rxmm3);
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm1);
	
	rxmm11 = _mm_subs_epi16(rxmm3, rxmm0);
	rxmm13 = _mm_adds_epi16(rxmm2, rxmm1);
	
	rxmm9 = _mm_or_si128(rxmm8, short_one);
	rxmm15 = _mm_subs_epi16(rxmm12, rxmm14);

//	process rxmm9 and rxmm15 and store them in dst+8 and dst+56;
	rxmm8 = rxmm9;
	rxmm12 = rxmm15;
	rxmm10 = rxmm11;
	rxmm14 = rxmm13;

	rxmm9 = _mm_shufflehi_epi16(rxmm8, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm12, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm8, rxmm12);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm8 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm12 = _mm_unpackhi_epi64(rxmm11, rxmm11);	


	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+32));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+40));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+48));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+56));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+8));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
//	rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+8), rxmm4);			// row-1
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+56));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
//	rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+56), rxmm7);		// row-7

//	process rxmm11 and rxmm13 and store them in dst+24 and dst+40;
	rxmm9 = _mm_shufflehi_epi16(rxmm10, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm14, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm10, rxmm14);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm10 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm14 = _mm_unpackhi_epi64(rxmm11, rxmm11);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+96));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+104));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+112));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+120));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+24));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
//	rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+24), rxmm4);		// row-3
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+40));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
//	rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+40), rxmm7);		// row-5

}

void FdctQuant_LS_SSE2(const Fw8u *pSrc, int srcStp, Fw16s *pDst, const Fw16u *pQuantFwdTable)
{
	__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7;
	__m128i rxmm8, rxmm9, rxmm10, rxmm11, rxmm12, rxmm13, rxmm14, rxmm15;
	__m128i short_one, weighting;
	__m128i quantCoef;
	Fw8u  *pesi  = (Fw8u *)pSrc;
	Fw16s *peax;
	Fw16s *pedx = pDst;

	rxmm0 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm1 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm2 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm3 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm4 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm5 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm6 = _mm_loadl_epi64((__m128i *)(pesi));
	pesi += srcStp;
	rxmm7 = _mm_loadl_epi64((__m128i *)(pesi));

	// Level Scaling
	rxmm8 = _mm_setzero_si128();
	rxmm0 = _mm_unpacklo_epi8(rxmm0,rxmm8);
	rxmm1 = _mm_unpacklo_epi8(rxmm1,rxmm8);
	rxmm2 = _mm_unpacklo_epi8(rxmm2,rxmm8);
	rxmm3 = _mm_unpacklo_epi8(rxmm3,rxmm8);
	rxmm4 = _mm_unpacklo_epi8(rxmm4,rxmm8);
	rxmm5 = _mm_unpacklo_epi8(rxmm5,rxmm8);
	rxmm6 = _mm_unpacklo_epi8(rxmm6,rxmm8);
	rxmm7 = _mm_unpacklo_epi8(rxmm7,rxmm8);

	rxmm9 = _mm_set1_epi16 (128);
	rxmm0 = _mm_subs_epi16(rxmm0, rxmm9);			//rxmm0 -= 128
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm9);			//rxmm1 -= 128
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm9);			//rxmm2 -= 128
	rxmm3 = _mm_subs_epi16(rxmm3, rxmm9);			//rxmm3 -= 128
	rxmm4 = _mm_subs_epi16(rxmm4, rxmm9);			//rxmm4 -= 128
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm9);			//rxmm5 -= 128
	rxmm6 = _mm_subs_epi16(rxmm6, rxmm9);			//rxmm6 -= 128
	rxmm7 = _mm_subs_epi16(rxmm7, rxmm9);			//rxmm7 -= 128

	peax = (Fw16s *) pQuantFwdTable;			// eax = pQuantFwdTable;

	rxmm8 = rxmm0;
	rxmm0 = _mm_subs_epi16(rxmm0, rxmm7);			//rxmm0 = 0-7
	rxmm8 = _mm_adds_epi16(rxmm8, rxmm7);			//rxmm7 = 0+7
	
	rxmm9 = rxmm1;
	rxmm1 = _mm_subs_epi16(rxmm1, rxmm6);			//rxmm1 = 1-6
	rxmm9 = _mm_adds_epi16(rxmm9, rxmm6);			//rxmm6 = 1+6

	rxmm10 = rxmm2;
	rxmm2 = _mm_subs_epi16(rxmm2, rxmm5);			//rxmm2 = 2-5
	rxmm10 = _mm_adds_epi16(rxmm10, rxmm5);		//rxmm5 = 2+5

	rxmm11 = rxmm3;
	rxmm3 = _mm_subs_epi16(rxmm3, rxmm4);			//rxmm3 = 3-4
	rxmm11 = _mm_adds_epi16(rxmm11, rxmm4);		//rxmm4 = 3+4

	// ================= Even ================================================

	rxmm8 = _mm_slli_epi16(rxmm8, 3);				
	rxmm15 = rxmm8;
	rxmm9 = _mm_slli_epi16(rxmm9, 3);				
	rxmm10 = _mm_slli_epi16(rxmm10, 3);				
	rxmm13 = rxmm10;
	rxmm11 = _mm_slli_epi16(rxmm11, 3);				
	//rxmm11 = rxmm11;

	rxmm4 = _mm_adds_epi16(rxmm9, rxmm13);		// rxmm4 = 1+6+2+5
	rxmm5 = _mm_adds_epi16(rxmm15, rxmm11);	// rxmm5 = 0+7+3+4

	rxmm8 = _mm_adds_epi16(rxmm5, rxmm4);
	rxmm12 = _mm_subs_epi16(rxmm5, rxmm4);
	
	rxmm6 = _mm_subs_epi16(rxmm9, rxmm13);		// rxmm6 = 1+6-(2+5)
	rxmm7 = _mm_subs_epi16(rxmm15, rxmm11);	// rxmm7 = 0+7-(3+4)

	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 136));

	rxmm4 = _mm_mulhi_epi16(rxmm6, weighting);	// rxmm4 = mul1625
	rxmm5 = _mm_mulhi_epi16(rxmm7, weighting);	// rxmm5 = mul0734

	rxmm4 = _mm_adds_epi16(rxmm4, rxmm7);
	rxmm5 = _mm_subs_epi16(rxmm5, rxmm6);

	short_one = _mm_set1_epi16 (1);

	rxmm10 = _mm_or_si128(rxmm4, short_one);
	rxmm14 = _mm_or_si128(rxmm5, short_one);
	
//	process rxmm8 and rxmm12 and store them in dst+12 and dst+32;
	rxmm9  = _mm_shufflehi_epi16(rxmm8, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm12, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm8, rxmm12);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4  = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9  = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5  = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm8  = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6  = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm12 = _mm_unpackhi_epi64(rxmm11, rxmm11);	


	rxmm9  = _mm_load_si128((__m128i *)(fdct_weighting));	
	rxmm4  = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7  = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9  = _mm_load_si128((__m128i *)(fdct_weighting+8));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm4  = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+16));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+24));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));		
	//rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)pedx, rxmm4);				// row-0

	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+32));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
//	rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+32), rxmm7);		// row-4

//	process rxmm10 and rxmm14 and store them in dst+16 and dst+48;
	rxmm9 = _mm_shufflehi_epi16(rxmm10, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm14, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm10, rxmm14);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm10 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm14 = _mm_unpackhi_epi64(rxmm11, rxmm11);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+64));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+72));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+80));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+88));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+16));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
	//rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+16), rxmm4);		// row-2
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+48));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
	//rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+48), rxmm7);		// row-6
	
	// ================= Odd ================================================ 

	rxmm8 = rxmm0;
	rxmm10 = rxmm1;
	rxmm12 = rxmm2;
	rxmm14 = rxmm3;

	rxmm8  = _mm_slli_epi16(rxmm8, 3);
	rxmm10 = _mm_slli_epi16(rxmm10, 4);
	rxmm12 = _mm_slli_epi16(rxmm12, 4);
	rxmm14 = _mm_slli_epi16(rxmm14, 3);

	rxmm0 = _mm_subs_epi16(rxmm10, rxmm12);		// rxmm0 = t2+t4
	rxmm10 = _mm_adds_epi16(rxmm10, rxmm12);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 152));
	rxmm10 = _mm_mulhi_epi16(rxmm10, weighting);
	rxmm0 = _mm_mulhi_epi16(rxmm0, weighting);
	
	rxmm1 = _mm_subs_epi16(rxmm14, rxmm0);		// rxmm1 = t6-rxmm0
	rxmm14 = _mm_adds_epi16(rxmm14, rxmm0);
	
	rxmm10 = _mm_or_si128(rxmm10, short_one);	// rxmm3 = t0-t2
	rxmm3 = _mm_subs_epi16(rxmm8, rxmm10);
	rxmm12 = _mm_adds_epi16(rxmm8, rxmm10);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 128) );
	rxmm8 = _mm_mulhi_epi16(weighting, rxmm14);
	rxmm8 = _mm_adds_epi16(rxmm8, rxmm12);
	rxmm12 = _mm_mulhi_epi16(rxmm12,weighting);
	
	weighting = _mm_load_si128((__m128i *)(fdct_weighting + 144));
	rxmm0 = _mm_mulhi_epi16(weighting, rxmm1);	
	rxmm2 = _mm_mulhi_epi16(weighting, rxmm3);	// rxmm2 = tg_3_6 * rxmm1
	rxmm2 = _mm_adds_epi16(rxmm2, rxmm3);
	rxmm0 = _mm_adds_epi16(rxmm0, rxmm1);
	
	rxmm11 = _mm_subs_epi16(rxmm3, rxmm0);
	rxmm13 = _mm_adds_epi16(rxmm2, rxmm1);
	
	rxmm9 = _mm_or_si128(rxmm8, short_one);
	rxmm15 = _mm_subs_epi16(rxmm12, rxmm14);

//	process rxmm9 and rxmm15 and store them in dst+8 and dst+56;
	rxmm8 = rxmm9;
	rxmm12 = rxmm15;
	rxmm10 = rxmm11;
	rxmm14 = rxmm13;

	rxmm9 = _mm_shufflehi_epi16(rxmm8, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm12, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm8, rxmm12);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm8 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm12 = _mm_unpackhi_epi64(rxmm11, rxmm11);	


	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+32));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+40));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);
	

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+48));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+56));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm8);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm12);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+8));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
	//rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+8), rxmm4);			// row-1
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+56));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
	//rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+56), rxmm7);		// row-7

//	process rxmm11 and rxmm13 and store them in dst+24 and dst+40;
	rxmm9 = _mm_shufflehi_epi16(rxmm10, 0x1B); 
	rxmm11 = _mm_shufflehi_epi16(rxmm14, 0x1B); 
	
	rxmm13 = _mm_unpacklo_epi64(rxmm10, rxmm14);	
	rxmm15 = _mm_unpackhi_epi64(rxmm9, rxmm11);	
	
	rxmm4 = _mm_adds_epi16(rxmm13, rxmm15);		
	rxmm13 = _mm_subs_epi16(rxmm13, rxmm15);		
	
	rxmm9 = _mm_unpacklo_epi32(rxmm4, rxmm13);	
	rxmm5 = _mm_unpacklo_epi64(rxmm9, rxmm9);	
	rxmm10 = _mm_unpackhi_epi64(rxmm9, rxmm9);	
	
	rxmm11 = _mm_unpackhi_epi32(rxmm4, rxmm13);	
	rxmm6 = _mm_unpacklo_epi64(rxmm11, rxmm11);	
	rxmm14 = _mm_unpackhi_epi64(rxmm11, rxmm11);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+96));	
	rxmm4 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm7 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+104));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm4 = _mm_add_epi32(rxmm4, rxmm13);
	//rxmm11 = _mm_set1_epi32 (0x80000);	// rounding

	//rxmm4 = _mm_add_epi32(rxmm4, rxmm11);
	rxmm4 = _mm_srai_epi32(rxmm4, 20);
	
	rxmm7 = _mm_add_epi32(rxmm7, rxmm15);
	//rxmm7 = _mm_add_epi32(rxmm7, rxmm11);
	rxmm7 = _mm_srai_epi32(rxmm7, 20);

	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+112));	
	rxmm13 = _mm_madd_epi16(rxmm9, rxmm5);	
	rxmm15 = _mm_madd_epi16(rxmm9, rxmm6);	
	
	rxmm9 = _mm_load_si128((__m128i *)(fdct_weighting+120));	
	rxmm5 = _mm_madd_epi16(rxmm9, rxmm10);
	rxmm9 = _mm_madd_epi16(rxmm9, rxmm14);
	
	rxmm13 = _mm_add_epi32(rxmm13, rxmm5);
	//rxmm13 = _mm_add_epi32(rxmm13, rxmm11);
	rxmm13 = _mm_srai_epi32(rxmm13, 20);
	
	rxmm4 = _mm_packs_epi32(rxmm4, rxmm13);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+24));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm4 = _mm_mullo_epi16(rxmm4, _mm_set1_epi16 (4));							 
	//rxmm4 = _mm_add_epi16(rxmm4, short_one);					// rounding
	rxmm4 = _mm_mulhi_epi16(rxmm4, quantCoef);
	rxmm4 = _mm_add_epi16(rxmm4, short_one);
	rxmm4 = _mm_srai_epi16(rxmm4, 1);
	_mm_storeu_si128((__m128i *)(pedx+24), rxmm4);		// row-3
	
	rxmm15 = _mm_add_epi32(rxmm15, rxmm9);
	//rxmm15 = _mm_add_epi32(rxmm15, rxmm11);
	rxmm15 = _mm_srai_epi32(rxmm15, 20);
	
	rxmm7 = _mm_packs_epi32(rxmm7, rxmm15);

	quantCoef	 = _mm_loadu_si128((__m128i *)(pQuantFwdTable+40));	// Quant
	//quantCoef	 = _mm_srai_epi16(quantCoef, 1);
	rxmm7 = _mm_mullo_epi16(rxmm7, _mm_set1_epi16 (4));							 
	//rxmm7 = _mm_add_epi16(rxmm7, short_one);					// rounding
	rxmm7 = _mm_mulhi_epi16(rxmm7, quantCoef);
	rxmm7 = _mm_add_epi16(rxmm7, short_one);
	rxmm7 = _mm_srai_epi16(rxmm7, 1);
	_mm_storeu_si128((__m128i *)(pedx+40), rxmm7);		// row-5

}


//------------------------------------------------------------------------
void Idct(const float c[8][8], const Fw16s *pSrc, Fw16s *pDst)
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

//-----------------------------------------------------------------------------

static float one_pixel_dct(const Fw16s *pSrc, unsigned int u, unsigned int v)
{
    const float PI8 = 0.392699081f; //Pi/8

    unsigned int x, y;

    float sum = 0;
    for(x=0; x<8; x++)
    {
        for(y=0; y<8; y++)
        {
			 sum += (pSrc[x*8+y])*cos(PI8*(x+0.5f)*u)*cos(PI8*(y+0.5f)*v);
        }
    }

    return sum;
}

void C_faandct(const Fw16s *pSrc, Fw16s *pDst, float round)
{
    const float SQRT_1BY2 = 0.707106781f;

    const float NORM_FACTOR = 0.25f;

	float result;
    
    unsigned int u, v;

    for(u=0; u<8; u++)
    {
        const float C_U = ((u==0) ? SQRT_1BY2 : 1.0f);
        for(v=0; v<8; v++)
        {
            const float C_V = ((v==0) ? SQRT_1BY2 : 1.0f);

			result = C_U * C_V * NORM_FACTOR * one_pixel_dct(pSrc,u,v);
			pDst[u*8+v] = (Fw16s)((result >0) ? (result+round) : (result-round));
        }
    }
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
