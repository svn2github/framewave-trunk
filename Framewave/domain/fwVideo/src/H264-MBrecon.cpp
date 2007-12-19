/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
//#include "fwImage.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

#define D_CBP_1ST_LUMA_AC_BITPOS 1
#define COEF_POS_EOB 0x20			// end of block flag
#define COEF_POS_16BIT_COEF 0x10	// next coefficient is 16 bits flag

// tab16, indexed by 4x4 subblock
const Fw8u left_edge_tab16[16] 	= {1,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0};
const Fw8u top_edge_tab16[16]		= {1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0};
const Fw8u right_edge_tab16[16] 	= {0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,1};

// Table for 4x4 intra prediction to find if a subblock can use predictors
// from above right. Also used for motion vector prediction availability.
// JVT CD block order.
const Fw8u above_right_avail_4x4[16] = {1,1,1,0,1,1,1,0,1,1,1,0,1,0,1,0};

// Table for 4x4 intra prediction to find if a subblock can use predictors
// from below left. JVT CD block order.
const Fw8u intra4x4_below_left_avail[16] = {1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0};

#define CLAMP_BIAS  256 // Bias in clamping table
#define CLIP_RANGE  (CLAMP_BIAS + 256 + CLAMP_BIAS)

//extern "C" const static Fw8u  ClampTbl[CLIP_RANGE];	/////////// Init
SYS_FORCEALIGN_16  const Fw8u ClampTbl[CLIP_RANGE] =
{
     0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
    ,0x00 ,0x01 ,0x02 ,0x03 ,0x04 ,0x05 ,0x06 ,0x07
    ,0x08 ,0x09 ,0x0a ,0x0b ,0x0c ,0x0d ,0x0e ,0x0f
    ,0x10 ,0x11 ,0x12 ,0x13 ,0x14 ,0x15 ,0x16 ,0x17
    ,0x18 ,0x19 ,0x1a ,0x1b ,0x1c ,0x1d ,0x1e ,0x1f
    ,0x20 ,0x21 ,0x22 ,0x23 ,0x24 ,0x25 ,0x26 ,0x27
    ,0x28 ,0x29 ,0x2a ,0x2b ,0x2c ,0x2d ,0x2e ,0x2f
    ,0x30 ,0x31 ,0x32 ,0x33 ,0x34 ,0x35 ,0x36 ,0x37
    ,0x38 ,0x39 ,0x3a ,0x3b ,0x3c ,0x3d ,0x3e ,0x3f
    ,0x40 ,0x41 ,0x42 ,0x43 ,0x44 ,0x45 ,0x46 ,0x47
    ,0x48 ,0x49 ,0x4a ,0x4b ,0x4c ,0x4d ,0x4e ,0x4f
    ,0x50 ,0x51 ,0x52 ,0x53 ,0x54 ,0x55 ,0x56 ,0x57
    ,0x58 ,0x59 ,0x5a ,0x5b ,0x5c ,0x5d ,0x5e ,0x5f
    ,0x60 ,0x61 ,0x62 ,0x63 ,0x64 ,0x65 ,0x66 ,0x67
    ,0x68 ,0x69 ,0x6a ,0x6b ,0x6c ,0x6d ,0x6e ,0x6f
    ,0x70 ,0x71 ,0x72 ,0x73 ,0x74 ,0x75 ,0x76 ,0x77
    ,0x78 ,0x79 ,0x7a ,0x7b ,0x7c ,0x7d ,0x7e ,0x7f
    ,0x80 ,0x81 ,0x82 ,0x83 ,0x84 ,0x85 ,0x86 ,0x87
    ,0x88 ,0x89 ,0x8a ,0x8b ,0x8c ,0x8d ,0x8e ,0x8f
    ,0x90 ,0x91 ,0x92 ,0x93 ,0x94 ,0x95 ,0x96 ,0x97
    ,0x98 ,0x99 ,0x9a ,0x9b ,0x9c ,0x9d ,0x9e ,0x9f
    ,0xa0 ,0xa1 ,0xa2 ,0xa3 ,0xa4 ,0xa5 ,0xa6 ,0xa7
    ,0xa8 ,0xa9 ,0xaa ,0xab ,0xac ,0xad ,0xae ,0xaf
    ,0xb0 ,0xb1 ,0xb2 ,0xb3 ,0xb4 ,0xb5 ,0xb6 ,0xb7
    ,0xb8 ,0xb9 ,0xba ,0xbb ,0xbc ,0xbd ,0xbe ,0xbf
    ,0xc0 ,0xc1 ,0xc2 ,0xc3 ,0xc4 ,0xc5 ,0xc6 ,0xc7
    ,0xc8 ,0xc9 ,0xca ,0xcb ,0xcc ,0xcd ,0xce ,0xcf
    ,0xd0 ,0xd1 ,0xd2 ,0xd3 ,0xd4 ,0xd5 ,0xd6 ,0xd7
    ,0xd8 ,0xd9 ,0xda ,0xdb ,0xdc ,0xdd ,0xde ,0xdf
    ,0xe0 ,0xe1 ,0xe2 ,0xe3 ,0xe4 ,0xe5 ,0xe6 ,0xe7
    ,0xe8 ,0xe9 ,0xea ,0xeb ,0xec ,0xed ,0xee ,0xef
    ,0xf0 ,0xf1 ,0xf2 ,0xf3 ,0xf4 ,0xf5 ,0xf6 ,0xf7
    ,0xf8 ,0xf9 ,0xfa ,0xfb ,0xfc ,0xfd ,0xfe ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
    ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff ,0xff
};
#define ClampVal(x)  (ClampTbl[CLAMP_BIAS + (x)])
#define ClampTblLookup(x, y) ClampVal((x) + (y))

FwStatus static DequantTransformResidual_idc_1(Fw16s* pSrcDst, Fw32s  QP, Fw16s *LevelScale);

void  static itrIDCTdequant4x4(int src[4][4], int dest[4][4], const int *dequantPtr,
                       int qp_per, int isDc, int dcValue)
{
  int tmp[4][4];
  int A, B, C, D, E, F;  
  int i;
  int deqc;

  /*
   *  a = A + B + C + (D>>1)
   *  b = A + (B>>1) - C - D
   *  c = A - (B>>1) - C + D
   *  d = A - B + C - (D>>1)
   *   =>
   *  E = A + C
   *  F = B +  (D>>1)
   *  a = E + F
   *  d = E - F
   *  E = A - C
   *  F = (B>>1) - D
   *  b = E + F
   *  c = E - F
   */

  A = dcValue;

  for (i = 0; i < 4; i++) {
    deqc = (*dequantPtr++) << qp_per;

    if (!isDc)
      A = src[i][0] * deqc;

    C = src[i][2] * deqc;

    deqc = (*dequantPtr++) << qp_per;

    B = src[i][1] * deqc;
    D = src[i][3] * deqc;

    E = A +  C;
    F = B + (D>>1);
    tmp[i][0] = E + F;
    tmp[i][3] = E - F;
    E =  A     - C;
    F = (B>>1) - D;
    tmp[i][1] = E + F;
    tmp[i][2] = E - F;

    isDc = 0;
  }

  for (i = 0; i < 4; i++) {
    E = tmp[0][i] +  tmp[2][i];
    F = tmp[1][i] + (tmp[3][i]>>1);
    dest[0][i] = E + F;
    dest[3][i] = E - F;
    E =  tmp[0][i]     - tmp[2][i];
    F = (tmp[1][i]>>1) - tmp[3][i];
    dest[1][i] = E + F;
    dest[2][i] = E - F;
  }
}


#define         FWVC_CBP_1ST_CHROMA_DC_BITPOS 17
#define         FWVC_CBP_1ST_CHROMA_AC_BITPOS 19
#define         FWVC_CBP_CHROMA_DC (0x3<<FWVC_CBP_1ST_CHROMA_DC_BITPOS)
#define         FWVC_CBP_CHROMA_AC (0xff<<FWVC_CBP_1ST_CHROMA_AC_BITPOS)
#define         FWVC_CBP_LUMA_AC (0xffff<<FWVC_CBP_1ST_LUMA_AC_BITPOS)
#define         FWVC_CBP_1ST_LUMA_AC_BITPOS 1
#define         FWVC_CBP_LUMA_DC 1

//FwStatus PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s QP);
//FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s  step, 
//													Fw16s* pDC, Fw32s  AC, Fw32s  QP);

#undef  MAX
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#undef  MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaInterMB_H264_16s8u_P2R)(
				Fw16s **ppSrcCoeff,
				Fw8u   *pSrcDstUPlane, 
				Fw8u   *pSrcDstVPlane, 
				const Fw32u srcDstStep, 
				const Fw32u cbp4x4, 
				const Fw32s ChromaQP)
{
	SYS_FORCEALIGN_16  Fw16s ChromaDCU[4];
	SYS_FORCEALIGN_16  Fw16s ChromaDCV[4];
	SYS_FORCEALIGN_16  Fw16s ACUplane[16];
	SYS_FORCEALIGN_16  Fw16s ACVplane[16];
	int i;
	Fw8u  *pSrcDstU = pSrcDstUPlane;
	Fw8u  *pSrcDstV = pSrcDstVPlane;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];

	Fw16s *pbs16       = (Fw16s *) *ppSrcCoeff;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

//printf("\n enter fwiReconstructChromaInterMB_H264_16s8u_P2R: pbs16=%ld, %x", pbs16, cbp4x4);

	if( ppSrcCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQP < 0 || ChromaQP > 39) return fwStsOutOfRangeErr;

	for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS))) 
	{
		Fw16s *psrc;
		psrc = pbs16;	// U point
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
	}

//printf("\n U: pbs16=%ld: %d %d %d %d", pbs16, ChromaDCU[0], ChromaDCU[1], ChromaDCU[2], ChromaDCU[3]);

	for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))	
	{
//printf("\n fwiReconstructChromaInterMB_H264_16s8u_P2R-1-1 pbs16=%ld", pbs16);

//		Fw16s *psrc;
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
	}

//printf("\n V: pbs16=%ld: %d %d %d %d", pbs16, ChromaDCV[0], ChromaDCV[1], ChromaDCV[2], ChromaDCV[3]);

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstStep);
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCU[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;

//printf("\n UAC=======: (DCCoeff=%d) ", DCCoeff);
//for(i=0;i<16;i++)printf(" %d", ACUplane[i]) ;

			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

//printf("\n U-AC: %d \n", uBlock);
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstStep;	
			}		
		}
	}	// for uBlock

//printf("\n fwiReconstructChromaInterMB_H264_16s8u_P2R-3: pbs16=%ld", pbs16);

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstStep);
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCV[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
//			Fw16s *psrc;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;

//printf("\n UAC+++++++: ");
//for(i=0;i<16;i++)printf(" %d", ACVplane[i]) ;

			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 0, ChromaQP);
		}

//printf("\n V-AC: %d \n", uBlock);
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//printf(" %d %d %d %d", ACVplane[i*4],ACVplane[i*4+1],ACVplane[i*4+2],ACVplane[i*4+3]);

				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstStep;	
			}
		}
	}	// for uBlock

	*ppSrcCoeff = pbs16;	// U point

//printf("\n End-fwiReconstructChromaInterMB_H264_16s8u_P2R: *ppSrcCoeff=%ld", *ppSrcCoeff);

	return fwStsNoErr;
}
#define  clip(min, max, val) (((val)<(min))? (min):(((val)>(max))? (max):(val)))
FwStatus static PredictIntraChroma8x8_H264_8u_C1IR(Fw8u* pSrcDst, 
												 Fw32s srcdstStep, 
												 FwIntraChromaPredMode_H264 predMode, 
												 Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 3) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
//	__m128i xmm1, xmm2, xmm4, xmm5, xmm6, xmm7;
	__m128i xmm7;

	if(FW_LEFT		  & availability) block_available_left = 1;
	if(FW_UPPER_LEFT & availability) block_available_up_left = 1;
	if(FW_UPPER	  & availability) block_available_up = 1;

//	int comp;
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D;
	int H, V, a, b, c;
	int i, j;
	int tmp; //, tmp2;

	switch (predMode) {

	  case FW_CHROMA_DC:

		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;

		 if(block_available_up_left==1)   block_available_left= block_available_up=1;
		  if (block_available_up) {
#if 0
			for (i = 0; i < 4; i++) {
			  S2 += recoPic[(1+i)*srcdstStep];
			  S3 += recoPic[(5+i)*srcdstStep];
			}
#else
			for( i = 0; i < 4; i++ )
			{
				S0 += pSrcDst[i   - srcdstStep];
				S1 += pSrcDst[i+4 - srcdstStep];
			}
#endif
		  }
		  if (block_available_left) {
#if 0
			for (i = 0; i < 4; i++) {
			  S0 += recoPic[1+i];
			  S1 += recoPic[5+i];
			}
#else
			for( i = 0; i < 4; i++ )
			{
				S2 += pSrcDst[i     * srcdstStep - 1];
				S3 += pSrcDst[(i+4) * srcdstStep - 1];
			}
#endif
		  }
//printf("\n A=%d B=%d C=%d D=%d", A,B,C,D);

		  if (block_available_up && block_available_left) {
			A = (S0 + S2 + 4)>>3;
			B = (S1 + 2)>>2;
			C = (S3 + 2)>>2;
			D = (S1 + S3 + 4)>>3;
		  }
		  else if (block_available_up) {
			A = B = (S0 + 2)>>2;
			C = D = (S1 + 2)>>2;
		  }
		  else if (block_available_left) {
			A = C = (S2 + 2)>>2;
			B = D = (S3 + 2)>>2;
		  }
		  else
			A = B = C = D = 128;
      
			if( Dispatch_Type == DT_SSE2)	{
				Fw32u val = (Fw32u)0x01010101;					// duplicate 4 DC value
				Fw32u vala, valb, valc, vald;					// duplicate 4 DC value
				vala = val * (Fw8u) A;
				valb = val * (Fw8u) B;
				valc = val * (Fw8u) C;
				vald = val * (Fw8u) D;

			  for (j = 0; j < 4; j++) {
				*(Fw32u*)&pSrcDst[j*srcdstStep]		= vala;	
				*(Fw32u*)&pSrcDst[j*srcdstStep+4]		= valb;	
				*(Fw32u*)&pSrcDst[(4+j)*srcdstStep]	= valc;	
				*(Fw32u*)&pSrcDst[(4+j)*srcdstStep+4]	= vald;	
			  }
			}
			else {
			  for (j = 0; j < 4; j++) {
				for (i = 0; i < 4; i++) {
				  pSrcDst[j    *srcdstStep+i  ] = (Fw8u) A;
				  pSrcDst[j    *srcdstStep+4+i] = (Fw8u) B;
				  pSrcDst[(4+j)*srcdstStep+i  ] = (Fw8u) C;
				  pSrcDst[(4+j)*srcdstStep+4+i] = (Fw8u) D;
				}
			  }
			}
		break;

	  case FW_CHROMA_VERT:

		/*
		 * Vertical prediction
		 */

//		if (!(block_available_left)) return fwStsLPCCalcErr;
		if (!(block_available_up)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
#if 0
		for (i = 0; i < 8; i++) {
			tmp = recoPic[1+i];
			for (j = 0; j < 8; j+=4) {
			  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
			  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
			}
		}
#else
			if( Dispatch_Type == DT_SSE2)	{
				xmm7 = _mm_loadl_epi64( (__m128i*)(recoPic-srcdstStep) );
				for (j = 0; j < 8; j++) {
					_mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 8 bytes
				}
			}
			else {
				for (i = 0; i < 8; i++) {
					tmp = recoPic[-srcdstStep+i];
					for (j = 0; j < 8; j+=4) {
					  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
					  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
					  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
					  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
					}
				}
			}
#endif
		break;

	  case FW_CHROMA_HOR:

		/*
		 * Horizontal prediction
		 */

//		if (!(block_available_up)) return fwStsLPCCalcErr;
		if (!(block_available_left)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;
#if 0
		for (j = 0; j < 8; j++) {
			tmp = recoPic[(1+j)*srcdstStep];
			for (i = 0; i < 8; i+=4) {
			  pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
			  pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
			}
printf(" %d", tmp); 
		}
#else
		if( Dispatch_Type==DT_SSE2)
		{
			for (j = 0; j < 8; j++) {
			  tmp = recoPic[-1+j*srcdstStep];
	  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
			  _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			for (j = 0; j < 8; j++) {
				tmp = recoPic[-1+j*srcdstStep];
				for (i = 0; i < 8; i+=4) {
				  pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
				}
			}
		}
#endif
		break;

	  case FW_CHROMA_PLANE:

		/*
		 * Plane Prediction
		 */
		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		H = V = 0;
/*
		for (i=1; i<=4; i++) {
			H += i*(recoPic[4+i] - recoPic[4-i]);
			V += i*(recoPic[(4+i)*srcdstStep] - recoPic[(4-i)*srcdstStep]);
		}
*/
		for( i = 0; i < 4; i++ )
		{
			H += (i+1) * ( pSrcDst[4+i-srcdstStep] -      pSrcDst[2-i-srcdstStep] );
			V += (i+1) * ( pSrcDst[-1+(i+4)*srcdstStep] - pSrcDst[-1+(2-i)*srcdstStep] );
		}

/*
		a = 16*(recoPic[8*srcdstStep] + recoPic[8]);
		b = (int)((17*((Fw32s)H)+16)>>5);
		c = (int)((17*((Fw32s)V)+16)>>5);
		tmp = a + c*(0-3) + 16;
*/
		a = 16 * ( pSrcDst[-1+7*srcdstStep] + pSrcDst[7-srcdstStep] );
		b = ( 17*H + 16 ) >> 5;
		c = ( 17*V + 16 ) >> 5;
		tmp = a -3*b -3*c + 16;
/*
		for (j = 0; j < 8; j++, tmp+=c) {
			tmp2 = tmp + b*(0-3);
			for (i = 0; i < 8; i++, tmp2+=b) {
			  pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
			}        
		}
*/
#if 0
		if( Dispatch_Type==DT_SSE2)
		{
			SYS_FORCEALIGN_16 const Fw16s c_b[8] = {0, 1, 2, 3, 4, 5, 6, 7};
			{
				xmm6  = _mm_load_si128( (__m128i*)(c_b) );	//[0, 7]
				xmm5  = _mm_set1_epi16((Fw16s)b);			// b
				xmm4  = _mm_set1_epi16((Fw16s)c);			// c
				xmm5  = _mm_mullo_epi16(xmm5, xmm6);		//b*[0,7]

				xmm2  = _mm_set1_epi16((Fw16s)(tmp));		// tmp
				xmm7  = _mm_set1_epi16((Fw16s)(0));		// 0

				for (j = 0; j < 8; j++) {
					xmm1 = xmm2;						// tmp
					xmm1 =	_mm_add_epi16(xmm1, xmm5);	// tmp+b*[0,7]
					xmm1 = _mm_srai_epi16(xmm1, 5);		// >>5
					xmm1 = _mm_unpacklo_epi64(xmm1, xmm7);	//clip(0,255,tmp2>>5);
					_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm4 ); // store 16 bytes
					xmm2  =	_mm_add_epi16(xmm2, xmm4);	//tmp+c
				}
			}
		}
		else
#endif
		{
			for(j=0; j<8; j++, tmp+=c)
			{
				for(i=0; i<8; i++ )
				{
					int pix;
					pix = (tmp+b*i) >> 5;
					pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,pix);
				}
			}
		}

		break;
	  default:
//printf("\n PredictIntraChroma8x8_H264_8u_C1IR------: default");
		  break;
	  }

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraMB_H264_16s8u_P2R)(
										Fw16s **ppSrcCoeff,
										Fw8u *pSrcDstUPlane, 
										Fw8u *pSrcDstVPlane, 
										const Fw32u srcDstUVStep, 
										const FwIntraChromaPredMode_H264 intra_chroma_mode, 
										const Fw32u cbp4x4, 
										const Fw32u ChromaQP, 
										const Fw8u edge_type)
{
    FwIntraChromaPredMode_H264 intra_mode = (FwIntraChromaPredMode_H264)intra_chroma_mode;
	FwStatus  res;
    Fw32s top		= (edge_type & D_TOP_EDGE);
    Fw32s left		= (edge_type & D_LEFT_EDGE);
	Fw32s top_left = (edge_type & D_TOP_LEFT_EDGE);
	Fw32s availability = (!top) | ((!left)<<1) | ((!top_left)<<5);

	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];
	Fw8u  *pSrcDstU = pSrcDstUPlane;
	Fw8u  *pSrcDstV = pSrcDstVPlane;

	Fw16s *pbs16       = (Fw16s *) *ppSrcCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQP < 0 || ChromaQP > 39) return fwStsOutOfRangeErr;

	availability = 0;
	if(left == 0) availability |= FW_LEFT;
	if(top == 0)  availability |= FW_UPPER;
	if(top_left == 0) availability |= FW_UPPER_LEFT;

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstUPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstVPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcCoeff;	// U point
//		for(i=0;i<4;i++) ChromaDCU[i] = psrc[i] ;
//		psrc = pbs16;	// U point
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
	}

	for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcCoeff;	// V point
//		for(i=0;i<4;i++) ChromaDCV[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;
		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
	}

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCU[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcCoeff;	// V point
//			for(i=0;i<16;i++) ACUplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[0])));
	//			pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[1])));
	//			pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[2])));
	//			pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[3])));
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]  +pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}		
		}


//		ppSrcCoeff++;	// next block
	}	// for uBlock


	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCV[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
	//		Fw16s *psrc;
	//		psrc = *ppSrcCoeff;	// V point
	//		for(i=0;i<16;i++) ACVplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 4x4
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 4x4
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[0])));
	//			pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[1])));
	//			pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[2])));
	//			pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[3])));
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]  +pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}
		}
//		ppSrcCoeff++;	// next block
	}	// for uBlock

	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}

const Fw8u Scan_Luma4x4[16] = {0,1,4,5,2,3,6,7,8,9,12,13,10,11,14,15};
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaInterMB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff, 
									Fw8u  *pSrcDstYPlane, 
									const Fw32u srcDstYStep, 
									const Fw32u cbp4x4,
									const Fw32s QP)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
//	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++) //, uCBPMask <<= 1)
	{
		uCBPMask = (1<<(uBlock+1));

//		pSrcCoeff = ppSrcCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;

			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);


			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
			pSrcDstYPlane2 = pSrcDstYPlane + (Scan_Luma4x4[uBlock]%4)*4 + 
											 (Scan_Luma4x4[uBlock]/4)*4*srcDstYStep;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
		//			for (j=0;j<4;j++)
		//			{	
		//				*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
		//			}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
//		else
//		{
//			for(i=0;i<16;i++) pDecSrcCoeff[i] = 0;
//		}
	}
	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}




/*
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, 
													Fw32s  step, 
													Fw16s* pDC, 
													Fw32s  AC, 
													Fw32s  QP);
*/
void static C_InverseQTA_Block(
	Fw32u QP,
	Fw8u **pCoeff,		// residual coeffs for block, update pointer on return
	Fw32s DCCoeff,		// input DC coefficient to support 16x16 intra, and chroma (zero in other cases)
	int bHasACCoeffs,	// only valid when DCCoeff is non-zero
	Fw8u *pDst,		// reconstructed data, prediction data here on entry
	Fw32u uPitch		// of Dst
)
{
	Fw16s tmpbuf[16];				// 4x4 work buffer for the coeffs
	Fw32u upos;
	bool bOnlyDCCoeff;			// currently for 16x16 intra, later for chroma?
	Fw8u *pU8Coeffs = *pCoeff;
	bool bMoreCoef;
	Fw8u *pRec;
	Fw16s *buf;
	int i;
	FwStatus  res;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if (DCCoeff)
		bOnlyDCCoeff = (bHasACCoeffs == 0);
	else
		bOnlyDCCoeff = false;

	if (!bOnlyDCCoeff)
	{
		// Fill tmpbuf from pCoeff.

		// initialize tmpbuf to zero
		memset(tmpbuf, 0, sizeof(tmpbuf));

		bMoreCoef = true;
		while (bMoreCoef)
		{
			upos = *pU8Coeffs & 0xf;
			bMoreCoef = (*pU8Coeffs & COEF_POS_EOB) == 0;
			if ((*pU8Coeffs & COEF_POS_16BIT_COEF) == 0)
			{
				tmpbuf[upos] = (Fw8s)(*(pU8Coeffs + 1));
				pU8Coeffs += 2;
			}
			else
			{
				tmpbuf[upos] = *(pU8Coeffs + 1) | ((Fw8s)(*(pU8Coeffs + 2)) << 8);
				pU8Coeffs += 3;
			}
			if (upos != 0) bHasACCoeffs = 1;

		}	// while
		*pCoeff = pU8Coeffs;
	}

	if (DCCoeff)
		tmpbuf[0] = (Fw16s)DCCoeff;

	// 4x4 inverse transform and add to prediction

	res = fwiDequantTransformResidual_H264_16s_C1I(tmpbuf, 8, 0,
												bHasACCoeffs, QP);
//	assert(res == fwStsNoErr);

	pRec = pDst;

	buf = tmpbuf;
	if( Dispatch_Type==DT_SSE2)
	{
		int *a;
	    xmm7  = _mm_set1_epi8(0); 
		for (i=0;i<4;i++)
		{	
			a    = (int *)pRec;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
			xmm1 = _mm_loadl_epi64 ( (__m128i*)(buf+i*4) );	// get 4 short
			xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
			*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
			pRec+=uPitch;	
		}		
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			pRec[0] = ClampTblLookup(pRec[0], *buf++);
			pRec[1] = ClampTblLookup(pRec[1], *buf++);
			pRec[2] = ClampTblLookup(pRec[2], *buf++);
			pRec[3] = ClampTblLookup(pRec[3], *buf++);
			pRec += uPitch;
		}
	}
}	// C_InverseQTA_Block


FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraMB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff, 
									Fw8u  *pSrcDstYPlane, 
									Fw32s srcDstYStep, 
									const FwIntra4x4PredMode_H264 *pMBIntraTypes, 
									const Fw32u cbp4x4, 
									const Fw32u QP, 
									const Fw8u  edgeType)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra4x4PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	Fw32s Luma4x4_idx;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

//printf("\n fwiReconstructLumaIntraMB: pbs16=%ld-%ld %d %x %d %x", pbs16, pSrcDstYPlane, 
//									srcDstYStep, cbp4x4, QP, edgeType);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

//        intra_mode =			pMBIntraTypes[Luma4x4_idx];
        intra_mode =			pMBIntraTypes[uBlock];

        top_edge_subblock =		top_edge_tab16[uBlock];
        left_edge_subblock =	left_edge_tab16[uBlock];
		right_edge_subblock =	right_edge_tab16[uBlock];

        top			=	top_edge_subblock   && (edgeType & D_TOP_EDGE);
        left		=	left_edge_subblock  && (edgeType & D_LEFT_EDGE);
		right		=	right_edge_subblock && (edgeType & D_RIGHT_EDGE);
		top_left	=	(top & left);
		top_right	=	(top & right) | (above_right_avail_4x4[uBlock] == 0);
//		top_left	=	(top_edge_subblock | left_edge_subblock)  &&  (edgeType & D_TOP_LEFT_EDGE);
//		top_right	=	(top_edge_subblock | right_edge_subblock) &&  (edgeType & D_TOP_RIGHT_EDGE);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);


		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

//printf("\n Luma4x4_idx=%d(%d), pSrcDstYPlane2=%ld, intra_mode=%d, pMBIntraTypes=%d",
//			Luma4x4_idx, uBlock, pSrcDstYPlane2, intra_mode, pMBIntraTypes); 
//printf("\n top=%d, left=%d, top_left=%d, top_right=%d", top,left,top_left,top_right);
//getchar();

		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );
//		res = fwiPredictIntra_4x4_H264_8u_C1IR ( (Fw8u*)pSrcDstYPlane2, (Fw32s) srcDstYStep, (FWIntra4x4PredMode_H264) intra_mode,(Fw32s) availability );

		pSrcCoeff = ppSrcCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);
//			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + (Luma4x4_idx/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//				for (j=0;j<4;j++)	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	*ppSrcCoeff = pbs16;	// U point

//printf("\n fwiReconstructLumaIntraMB exit: %ld", *ppSrcCoeff); 

	return fwStsNoErr;
}
#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraHalfMB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff, 
									Fw8u *pSrcDstYPlane, 
									Fw32s srcDstYStep, 
									FwIntra4x4PredMode_H264 *pMBIntraTypes, 
									Fw32u cbp4x2, 
									Fw32u QP, 
									Fw8u edgeType)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;
	FwIntra4x4PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;
	Fw32s     availability;
	FwStatus  res;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	Fw32s Luma4x4_idx;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<8; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = pMBIntraTypes[uBlock];

        top_edge_subblock = top_edge_tab16[uBlock];
        left_edge_subblock = left_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];

        top			= top_edge_subblock   && (edgeType & D_TOP_EDGE);
        left		= left_edge_subblock  && (edgeType & D_LEFT_EDGE);
		right		= right_edge_subblock && (edgeType & D_RIGHT_EDGE);
		top_left	= top & left;
		top_right	= (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);
 
		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

//		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );
		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcCoeff[uBlock];
		if( cbp4x2 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			Fw16s DCCoeff = pDecSrcCoeff[0];
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);

//			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//				for (j=0;j<4;j++)	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}
#endif

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaInter4x4MB_H264_16s8u_C1R)(
				Fw16s **ppSrcDstCoeff, 
				Fw8u *pSrcDstYPlane, 
				Fw32u srcDstYStep, 
				Fw32u cbp4x4, 
				Fw32s QP, 
				Fw16s *pQuantTable, 
				Fw8u bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;

	bypassFlag;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++, uCBPMask <<= 1)
	{
		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
			pSrcDstYPlane2 = pSrcDstYPlane + (Scan_Luma4x4[uBlock]%4)*4 + 
											 (Scan_Luma4x4[uBlock]/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));

					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
//		else
//		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
//		}
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// Y point

	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraHalf4x4MB_H264_16s8u_C1R)(
						Fw16s **ppSrcDstCoeff, 
						Fw8u *pSrcDstYPlane, 
						Fw32s srcDstYStep,
						FwIntra4x4PredMode_H264 *pMBIntraTypes, 
						Fw32u cbp4x2, 
						Fw32s QP, 
						Fw8u edgeType, 
						Fw16s *pQuantTable, 
						Fw8u bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra4x4PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;
	Fw32s Luma4x4_idx;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

bypassFlag;

	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<8; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = pMBIntraTypes[uBlock];

        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];

        top  = top_edge_subblock  && (edgeType & D_TOP_EDGE);
        left = left_edge_subblock && (edgeType & D_LEFT_EDGE);
		right = right_edge_subblock &&  (edgeType & D_RIGHT_EDGE);

		top_left = top & left;
		top_right = (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );
//		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp4x2 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
//			DequantTransformResidual_idc_1(pDecSrcCoeff, 8, &DCCoeff, 1, QP);
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
//			for (i=0;i<4;i++)
//			{
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
//				pSrcDstYPlane2+=srcDstYStep;	
//			}
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//printf("\n (%d) (%d) (%d) (%d)",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]  );
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}	
		}
#endif
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntra4x4MB_H264_16s8u_C1R)(
						Fw16s **ppSrcDstCoeff,
						Fw8u  *pSrcDstYPlane, 
						Fw32s srcDstYStep,
						FwIntra4x4PredMode_H264 *pMBIntraTypes, 
						Fw32u cbp4x4, 
						Fw32s QP, 
						Fw8u  edgeType, 
						Fw16s *pQuantTable, 
						Fw8u  bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra4x4PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;
	Fw32s Luma4x4_idx;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

bypassFlag;

	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = pMBIntraTypes[uBlock];
        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];
        top  = top_edge_subblock  && (edgeType & D_TOP_EDGE);
        left = left_edge_subblock && (edgeType & D_LEFT_EDGE);
		right = right_edge_subblock &&  (edgeType & D_RIGHT_EDGE);

		top_left = top & left;
		top_right = (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );
//		res = fwiPredictIntra_4x4_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
	//		for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			Fw16s DCCoeff = pDecSrcCoeff[0];
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
	//		DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
//			for (i=0;i<4;i++)
//			{
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
//				pSrcDstYPlane2+=srcDstYStep;	
//			}
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}
	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}
#endif

FwStatus static PredictIntraChroma4x8_H264_8u_C1IR(Fw8u* pSrcDst, 
												 Fw32s srcdstStep, 
												 FwIntraChromaPredMode_H264 predMode, 
												 Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 3) return fwStsOutOfRangeErr;

	int block_available_up=0;
	int block_available_left=0;
	int block_available_up_left=0;

	if(FW_LEFT		  & availability) block_available_left = 1;
	if(FW_UPPER_LEFT & availability) block_available_up_left = 1;
	if(FW_UPPER	  & availability) block_available_up = 1;

//	int comp;
	Fw8u *recoPic;
	int S0, S1, S2, S3;
	int A, B, C, D;
	int H, V, a, b, c;
	int i, j;
	int tmp, tmp2;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm1, xmm3, xmm4, xmm5, xmm6, xmm7;

	switch (predMode) {

	  case FW_CHROMA_DC:

		  recoPic = pSrcDst;
      	  S0 = S1 = S2 = S3 = 0;
      
		  if (block_available_up) {
			for (i = 0; i < 2; i++) {
			  S2 += recoPic[(1+i)*srcdstStep];
			  S3 += recoPic[(5+i)*srcdstStep];
			}
		  }
		  if (block_available_left) {
			for (i = 0; i < 4; i++) {
			  S0 += recoPic[1+i];
			  S1 += recoPic[5+i];
			}
		  }

		  if (block_available_up && block_available_left) {
			A = (S0 + S2 + 4)>>3;
			B = (S1 + 2)>>2;
			C = (S3 + 2)>>2;
			D = (S1 + S3 + 4)>>3;
		  }
		  else if (block_available_up) {
			A = B = (S2 + 2)>>2;
			C = D = (S3 + 2)>>2;
		  }
		  else if (block_available_left) {
			A = C = (S0 + 2)>>2;
			B = D = (S1 + 2)>>2;
		  }
		  else
			A = B = C = D = 128;
      
#if 0
		  for (j = 0; j < 2; j++) {
			for (i = 0; i < 4; i++) {
			  pSrcDst[j  *srcdstStep+i  ] = (Fw8u) A;		// A A A A B B B B 
			  pSrcDst[j  *srcdstStep+4+i] = (Fw8u) B;
//			  pSrcDst[(4+j)*srcdstStep+i  ] = (Fw8u) C;	// C C C C D D D D
//			  pSrcDst[(4+j)*srcdstStep+4+i] = (Fw8u) D;
			  pSrcDst[(2+j)*srcdstStep+i  ] = (Fw8u) C;	// C C C C D D D D
			  pSrcDst[(2+j)*srcdstStep+4+i] = (Fw8u) D;
			}
		  }
#else
		{
			Fw32u A_4 =(Fw32u)A*0x01010101;
			Fw32u B_4 =(Fw32u)B*0x01010101;
			Fw32u C_4 =(Fw32u)C*0x01010101;
			Fw32u D_4 =(Fw32u)D*0x01010101;
			*(Fw32u*)&pSrcDst[0*srcdstStep]   = A_4; 
			*(Fw32u*)&pSrcDst[0*srcdstStep+4] = B_4;
			*(Fw32u*)&pSrcDst[1*srcdstStep]   = A_4; 
			*(Fw32u*)&pSrcDst[1*srcdstStep+4] = B_4;
			*(Fw32u*)&pSrcDst[2*srcdstStep]   = C_4;
			*(Fw32u*)&pSrcDst[2*srcdstStep+4] = D_4;	
			*(Fw32u*)&pSrcDst[3*srcdstStep]   = C_4;
			*(Fw32u*)&pSrcDst[3*srcdstStep+4] = D_4;	
		}
#endif
		break;

	  case FW_CHROMA_VERT:

		/*
		 * Vertical prediction
		 */

		if (!(block_available_left)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		if( Dispatch_Type==DT_SSE2)
		{
			xmm7 = _mm_loadl_epi64( (__m128i*)(recoPic-srcdstStep) );
			_mm_storel_epi64( (__m128i*)(pSrcDst), xmm7 );				// store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+1*srcdstStep), xmm7 ); // store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+2*srcdstStep), xmm7 ); // store 8 bytes
			_mm_storel_epi64( (__m128i*)(pSrcDst+3*srcdstStep), xmm7 ); // store 8 bytes
		}
		else
		{
			for (i = 0; i < 8; i++) {	// correction: for (i = 0; i < 4; i++) {
				tmp = recoPic[-srcdstStep+i];  // tmp = recoPic[1+i];
				for (j = 0; j < 4; j+=4) { // for (j = 0; j < 8; j+=4) 
				  pSrcDst[(j+0)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+1)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+2)*srcdstStep+i] = (Fw8u) tmp;
				  pSrcDst[(j+3)*srcdstStep+i] = (Fw8u) tmp;
				}
			}
		}

		break;

	  case FW_CHROMA_HOR:

		/*
		 * Horizontal prediction
		 */

		if (!(block_available_up)) return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		if( Dispatch_Type==DT_SSE2)
		{
			recoPic = &pSrcDst[-1];
			for (j = 0; j < 4; j++) {
			  tmp = *recoPic;
			  recoPic += srcdstStep;
	  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
			  _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
			}
		}
		else
		{
			for (j = 0; j < 4; j++) {
//				tmp = recoPic[(1+j)*srcdstStep];
				tmp = recoPic[-1+j*srcdstStep];
				for (i = 0; i < 8; i+=4) {
				  pSrcDst[j*srcdstStep+i+0] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+1] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+2] = (Fw8u) tmp;
				  pSrcDst[j*srcdstStep+i+3] = (Fw8u) tmp;
				}
			}
		}
		break;

	  case FW_CHROMA_PLANE:

		/*
		 * Plane Prediction
		 */

		if (!(block_available_up) || !(block_available_left) || !(block_available_up_left)) 
			return fwStsLPCCalcErr;

		recoPic = pSrcDst;

		for (H = V = 0, i = 1; i <= 4; i++) {
			H += i*(recoPic[4+i] - recoPic[4-i]);
			V += i*(recoPic[(4+i)*srcdstStep] - recoPic[(4-i)*srcdstStep]);
		}

		a = 16*(recoPic[8*srcdstStep] + recoPic[8]);
		b = (int)((17*((Fw32s)H)+16)>>5);
		c = (int)((17*((Fw32s)V)+16)>>5);

		tmp = a + c*(0-3) + 16;
		if( Dispatch_Type==DT_SSE2)
		{
			SYS_FORCEALIGN_16 const Fw16s c_b[8] = {0, 1, 2, 3, 4, 5, 6, 7};
			{
				xmm6  = _mm_load_si128( (__m128i*)(c_b) );	//[0, 7]
				xmm5  = _mm_set1_epi16((Fw16s)b);	// b
				xmm4  = _mm_set1_epi16((Fw16s)c);	// c
				xmm3  = _mm_set1_epi16((Fw16s)tmp);	// tmp
				xmm7  =	_mm_set1_epi16(0);				// 0

				xmm5  = _mm_mullo_epi16(xmm5, xmm6); //b*[0, 7]
				xmm3  =	_mm_add_epi16(xmm3, xmm5);	//tmp+b*[0, 7]

/*
			for(j=0; j<8; j++, tmp+=c)			{
				for(i=0; i<8; i++ )				{
					int pix = (tmp+b*i) >> 5;
					pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,pix);
				}
			}
*/
				for (j = 0; j < 4; j++) {
					xmm1 = xmm3;
					xmm1 = _mm_srai_epi16(xmm1, 5);		//>>5
					xmm1 = _mm_unpacklo_epi64(xmm1, xmm7);	//clip(0,255,tmp2>>5);
				    _mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm1 ); // store 8 bytes

					xmm3  =	_mm_add_epi16(xmm3, xmm4);	//+c
				}
			}
		}
		else
		{
			for (j = 0; j < 4; j++, tmp+=c) {
				tmp2 = tmp + b*(0-3);
				for (i = 0; i < 8; i++, tmp2+=b) {
				  pSrcDst[j*srcdstStep+i] = (Fw8u) clip(0,255,tmp2>>5);
				}        
			}
		}

		break;
	  }

	return fwStsNoErr;

/*
Return Values
fwStsNoErr Indicates no error.
fwStsNullPtrErr Indicates an error condition if at least one of the specified pointers is
NULL.
fwStsOutOfRangeErr Indicates an error condition if predMode value falls outside [0,3].
fwStsStepErr Indicates an error condition if srcdstStep value is less than 8.
fwStsLPCCalcErr Indicates an error condition if predMode is not allowed for this block.
*/
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraHalvesMB_H264_16s8u_P2R)(
										Fw16s **ppSrcCoeff, 
										Fw8u *pSrcDstUPlane, 
										Fw8u *pSrcDstVPlane, 
										Fw32u srcDstUVStep, 
										FwIntraChromaPredMode_H264 intraChromaMode, 
										Fw32u cbp4x4,
										Fw32u ChromaQP, 
										Fw8u edgeTypeTop, 
										Fw8u edgeTypeBottom)
{
    FwIntraChromaPredMode_H264 intra_mode = (FwIntraChromaPredMode_H264)intraChromaMode;
	FwStatus  res;

    Fw32s top, left, top_left, availability;

	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw8u  *pSrcDstU, *pSrcDstV;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];

	Fw16s *pbs16       = (Fw16s *) *ppSrcCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQP < 0 || ChromaQP > 39) return fwStsOutOfRangeErr;

    top  = (edgeTypeTop & D_TOP_EDGE);
    left = (edgeTypeTop & D_LEFT_EDGE);
	top_left = top & left;
    availability = (!top) | ((!left)<<1) | ((!top_left)<<5);
	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstUPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstVPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

    top  = (edgeTypeBottom & D_TOP_EDGE);
    left = (edgeTypeBottom & D_LEFT_EDGE);
	top_left = top & left;
    availability = (!top) | ((!left)<<1) | ((!top_left)<<5);
	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstUPlane+srcDstUVStep*4,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstVPlane+srcDstUVStep*4,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);


	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcCoeff;	// U point
//		for(i=0;i<4;i++) ChromaDCU[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;

		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	}
//	*ppSrcCoeff++;

	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcCoeff;	// V point
//		for(i=0;i<4;i++) ChromaDCV[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;

		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	}
//	*ppSrcCoeff++;

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCU[uBlock];

		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcCoeff;	// V point
//			for(i=0;i<16;i++) ACUplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;
//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 1, ChromaQP);
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 16x16
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;

//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 0, ChromaQP);
			fwiDequantTransformResidual_H264_16s_C1I(ACUplane, // 16x16
														8, &DCCoeff, 0, ChromaQP);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[0])));
	//			pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[1])));
	//			pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[2])));
	//			pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[3])));
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}
		}
//		ppSrcCoeff++;	// next block
	}	// for uBlock

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		Fw16s DCCoeff = ChromaDCV[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcCoeff;	// V point
//			for(i=0;i<16;i++) ACVplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;
//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														8, &DCCoeff, 1, ChromaQP);
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 1, ChromaQP);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;
//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														8, &DCCoeff, 0, ChromaQP);
			fwiDequantTransformResidual_H264_16s_C1I(ACVplane, // 16x16
														8, &DCCoeff, 0, ChromaQP);
		}
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[0])));
	//			pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[1])));
	//			pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[2])));
	//			pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[3])));
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}
		}
//		ppSrcCoeff++;	// next block
	}	// for uBlock

	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}
#endif

FwStatus static TransformDequantChromaDC_idc_1(Fw16s* pSrcDst, Fw32s QP, Fw16s LevelScale)
{
	int qp_per = QP/6;

	Fw16s tmp0=*(pSrcDst);
	Fw16s tmp1=*(pSrcDst+1);
	Fw16s tmp2=*(pSrcDst+2);
	Fw16s tmp3=*(pSrcDst+3);

	pSrcDst[0]=(tmp0+tmp1+tmp2+tmp3);
    pSrcDst[1]=(tmp0-tmp1+tmp2-tmp3);
    pSrcDst[2]=(tmp0+tmp1-tmp2-tmp3);
    pSrcDst[3]=(tmp0-tmp1-tmp2+tmp3);

	// quantization
	//dcCij = ((fij* LevelScale(QPc%6,0,0))<<(QP'/6)) >> 5 with i,j = 0,1
#if 0 // whsu:
	for (i=0;i<4;i++)
  	  for (j=0;j<4;j++)
//	   *(pSrcDst+i*4+j) = (Fw16s)( (*(pSrcDst+i*4+j))*dequant_coef[qp_rem][i][j]<<qp_per)>>1;
	   *(pSrcDst+i*4+j) = (Fw16s)( ((*(pSrcDst+i*4+j))*LevelScale)<<qp_per)>>5;
#else
	pSrcDst[0]=(Fw16s)( ( pSrcDst[0] * LevelScale)<<qp_per)>>5;
	pSrcDst[1]=(Fw16s)( ( pSrcDst[1] * LevelScale)<<qp_per)>>5;
	pSrcDst[2]=(Fw16s)( ( pSrcDst[2] * LevelScale)<<qp_per)>>5;
	pSrcDst[3]=(Fw16s)( ( pSrcDst[3] * LevelScale)<<qp_per)>>5;
#endif
	return fwStsNoErr;
}
FwStatus static DequantTransformResidual_idc_1(Fw16s* pSrcDst, Fw32s  QP, Fw16s *LevelScale)
{
	int qp_per = (QP)/6;
	int i;
/*
– If qP is greater than or equal to 24, the scaled result shall be derived as follows
dij = (fij* LevelScale(QPc%6,i,j))<<(QP'/6-4) with i,j = 0,3
– Otherwise (qP is less than 24), the scaled result shall be derived as follows
dij = (fij* LevelScale(QPc%6,i,j)+2^(3-qp/6)>>(4-QP'/6) with i,j = 0,3
*/
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm4, xmm3, xmm5;

	if( QP < 24)
	{
		int qp_add = 1<<(3-qp_per);
		int qp_shft = 4-qp_per;
        __m128i qp_shft_x = _mm_cvtsi32_si128(qp_shft);
		Fw16s pSrcDst0 = pSrcDst[0];

		if( Dispatch_Type==DT_SSE2)
		{
				xmm0 = _mm_load_si128( (__m128i*)pSrcDst );
				xmm1 = _mm_load_si128( (__m128i*)LevelScale );
				xmm2  = _mm_set1_epi16((Fw16s)qp_add);	// qp_add
				xmm0  = _mm_mullo_epi16(xmm0, xmm1); // pSrcDst[i]*LevelScale[i]
				xmm0  =	_mm_add_epi16(xmm0, xmm2);	//  pSrcDst[i]*LevelScale[i] +qp_add
                xmm0 = _mm_sra_epi16(xmm0, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst), xmm0); // store 8 short

				xmm3 = _mm_load_si128( (__m128i*)pSrcDst+8 );
				xmm4 = _mm_load_si128( (__m128i*)LevelScale+8 );
				xmm3  = _mm_mullo_epi16(xmm3, xmm4); // pSrcDst[i]*LevelScale[i]
				xmm3  =	_mm_add_epi16(xmm3, xmm2);	//  pSrcDst[i]*LevelScale[i] +qp_add
				xmm3 = _mm_sra_epi16(xmm3, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst+8), xmm3); // store 8 short
		}
		else
		{
			for(int i=1;i<16;i++)
				pSrcDst[i]=(Fw16s)( (pSrcDst[i]*LevelScale[i]+qp_add ) >> qp_shft);
		}

		pSrcDst[0] = pSrcDst0;
	}
	else
	{
		int qp_shft = qp_per-4;
        __m128i qp_shft_x = _mm_cvtsi32_si128(qp_shft);
		Fw16s pSrcDst0 = pSrcDst[0];
		if( Dispatch_Type==DT_SSE2)
		{
				xmm0 = _mm_load_si128( (__m128i*)pSrcDst );
				xmm1 = _mm_load_si128( (__m128i*)LevelScale );
				xmm0  = _mm_mullo_epi16(xmm0, xmm1); // pSrcDst[i]*LevelScale[i]
				xmm0 = _mm_sll_epi16(xmm0, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst), xmm0); // store 8 short

				xmm3 = _mm_load_si128( (__m128i*)pSrcDst+8 );
				xmm4 = _mm_load_si128( (__m128i*)LevelScale+8 );
				xmm3  = _mm_mullo_epi16(xmm3, xmm4); // pSrcDst[i]*LevelScale[i]
				xmm3 = _mm_sll_epi16(xmm3, qp_shft_x);
				_mm_store_si128( (__m128i*)(pSrcDst+8), xmm3); // store 8 short
		}
		else
		{
			for(int i=1;i<16;i++)
				pSrcDst[i]=(Fw16s)( (pSrcDst[i]*LevelScale[i]) << qp_shft);
		}
		pSrcDst[0] = pSrcDst0;
	}

	if( Dispatch_Type==DT_SSE2)	////////// WHSU check
	{
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcDst );
		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4) );
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);
		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*2) );
		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcDst+4*3) );
		xmm2 = _mm_unpacklo_epi64(xmm2, xmm5);

	//  Horizontal transform
		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;

		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)

		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)

		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		// pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4			// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		// punpckhqdq	xmm3, xmm4			// xmm3 = [r1>>1-r3|r1+r3>>1]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		// pshufd		xmm2, xmm0, 0xe4
//		xmm2 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm2 = xmm0;

		// paddw		xmm0, xmm3			// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3			// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

	// Transpose
		//	pshufd	  xmm5, xmm0, 0xe4		// xmm5 = xmm0
//		xmm5 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm5 = xmm0;
		//	pshufd	  xmm2, xmm2, 0x4e		// xmm2 = [y3 |y2]
		xmm2 = _mm_shuffle_epi32(xmm2, 0x4e);
		//  punpcklwd xmm0, xmm2			// xmm0 = [y23|y03|y22|y02|y21|y01|y20|y00]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);
		//	punpckhwd xmm5, xmm2			// xmm5 = [y33|y13|y32|y12|y31|y11|y30|y10]
		xmm5 = _mm_unpackhi_epi16(xmm5, xmm2);
		//	movdqa	  xmm1, xmm0
		xmm1 = xmm0;
		//	punpcklwd xmm0, xmm5		// xmm0 = [y31|y21|y11|y01|y30|y20|y10|y00]=[y1|y0]
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm5);
		//	punpckhwd xmm1, xmm5		// xmm1 = [y33|y23|y13|y03|y32|y22|y12|y02]=[y3|y2]
		xmm1 = _mm_unpackhi_epi16(xmm1, xmm5);

	// vertical
		xmm2 = xmm1;

		xmm4 = xmm0;
		xmm5 = xmm2;
		xmm1 = xmm0;
		xmm3 = xmm2;
		xmm4 = _mm_srai_epi16(xmm4, 1);		// xmm4 = ([r1, r0]>>1)
		xmm5 = _mm_srai_epi16(xmm5, 1);		// xmm5 = ([r3, r2]>>1)
		xmm4 = _mm_srli_si128(xmm4, 8);		// xmm4 = ([r1]>>1)
		xmm5 = _mm_srli_si128(xmm5, 8);		// xmm5 = ([r3]>>1)
		xmm1 = _mm_unpacklo_epi64(xmm1, xmm4);	// xmm1 = ([r1]>>1), r0]	
		xmm3 = _mm_unpacklo_epi64(xmm3, xmm5);	// xmm3 = ([r3]>>1), r2]	

		//	pshufd	xmm4, xmm0, 0xe4	// xmm4 = xmm0
//		xmm4 = _mm_shuffle_epi32(xmm0, 0xe4);
//		xmm4 = xmm0;
		//	paddw		xmm0, xmm2			// xmm0 = [r1+r3>>1|r0+r2]
		xmm3 =	_mm_add_epi16(xmm3, xmm0);			// xmm3 = [r1+r3>>1|r0+r2] = [s3 s0]
		//	psubw		xmm4, xmm2			// xmm4 = [r1>>1-r3|r0-r2] 
		xmm1 =	_mm_sub_epi16(xmm1, xmm2);			// xmm1 = [r1>>1-r3|r0-r2] = [s2 s1]
		xmm0 = xmm3;
		xmm4 = xmm1;

		//	pshufd		xmm3, xmm0, 0xe4
//		xmm3 = _mm_shuffle_epi32(xmm0, 0xe4);
		xmm3 = xmm0;
		//	punpcklqdq  xmm0, xmm4		// xmm0 = [r0-r2|r0+r2]
		xmm0 = _mm_unpacklo_epi64(xmm0, xmm4);		
		//	punpckhqdq	xmm3, xmm4		// xmm3 = [r1-r3|r1+r3]
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm4);		
		//	movdqa		xmm2, xmm0
		xmm2 = xmm0;
		//	paddw		xmm0, xmm3		// xmm0 = [y1 |y0]
		xmm0 =	_mm_add_epi16(xmm0, xmm3);			
		//	psubw		xmm2, xmm3		// xmm2 = [y2 |y3]
		xmm2 =	_mm_sub_epi16(xmm2, xmm3);			

		xmm4 = _mm_shuffle_epi32(xmm2, 0x4e);	// xmm4 = [y3 |y2]

		xmm5  = _mm_set1_epi16((short)32);    // round;

		xmm0 =	_mm_add_epi16(xmm0, xmm5);			
		xmm4 =	_mm_add_epi16(xmm4, xmm5);			

		xmm0 = _mm_srai_epi16(xmm0, 6);		// pSrcDst[0*4 + i] = (s[0] + s[3] + 32) >> 6;
		xmm4 = _mm_srai_epi16(xmm4, 6);		// pSrcDst[0*4 + i] = (s[0] + s[3] + 32) >> 6;

		_mm_store_si128( (__m128i*)pSrcDst, xmm0 ); // store 16 bytes
		_mm_store_si128( (__m128i*)(pSrcDst+8), xmm4 ); // store 16 bytes
	}
	else
	{
		Fw16s s[4];
		for (i = 0; i < 4; i ++)
		{
			s[0] = pSrcDst[i*4 + 0] + pSrcDst[i*4 + 2];
			s[1] = pSrcDst[i*4 + 0] - pSrcDst[i*4 + 2];
			s[2] =(pSrcDst[i*4 + 1] >> 1) - pSrcDst[i*4 + 3];
			s[3] = pSrcDst[i*4 + 1] + (pSrcDst[i*4 + 3] >> 1);

			pSrcDst[i*4 + 0] = s[0] + s[3];
			pSrcDst[i*4 + 3] = s[0] - s[3];
			pSrcDst[i*4 + 1] = s[1] + s[2];
			pSrcDst[i*4 + 2] = s[1] - s[2];
		}

		for (i = 0; i < 4; i ++)
		{
			s[0] = pSrcDst[0*4 + i] + pSrcDst[2*4 + i];
			s[1] = pSrcDst[0*4 + i] - pSrcDst[2*4 + i];
			s[2] =(pSrcDst[1*4 + i] >> 1) - pSrcDst[3*4 + i];
			s[3] = pSrcDst[1*4 + i] + (pSrcDst[3*4 + i] >> 1);

			pSrcDst[0*4 + i] = (s[0] + s[3] + 32) >> 6;
			pSrcDst[3*4 + i] = (s[0] - s[3] + 32) >> 6;
			pSrcDst[1*4 + i] = (s[1] + s[2] + 32) >> 6;
			pSrcDst[2*4 + i] = (s[1] - s[2] + 32) >> 6;
		}
	}
	return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaInter4x4MB_H264_16s8u_P2R)(
			Fw16s **ppSrcDstCoeff, 
			Fw8u	*pSrcDstUPlane, 
			Fw8u	*pSrcDstVPlane, 
			Fw32u	 srcDstUVStep, 
			Fw32u	 cbp4x4, 
			Fw32s	 ChromaQPU, 
			Fw32u	 ChromaQPV, 
			Fw16s	*pQuantTableU, 
			Fw16s	*pQuantTableV, 
			Fw8u	 bypassFlag)
{
	bypassFlag;
	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw8u  *pSrcDstU = pSrcDstUPlane;
	Fw8u  *pSrcDstV = pSrcDstVPlane;
//	Fw16s *pACUplane = &ACUplane[0];
//	Fw16s *pACVplane = &ACVplane[0];

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16       = (Fw16s *) *ppSrcDstCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;


	if( ppSrcDstCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQPU < 0 || ChromaQPU > 39) return fwStsOutOfRangeErr;
	if( ChromaQPV < 0 || ChromaQPV > 39) return fwStsOutOfRangeErr;

//If chroma_format_idc is equal to 1, the scaled result is derived as
//dcCij = ((fij* LevelScale(QPc%6,0,0))<<(QP'/6)) >> 5 with i,j = 0,1
	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// U point
//		for(i=0;i<4;i++) ChromaDCU[i] = psrc[i] ;
		psrc = pbs16;	// U point
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQPU);
		TransformDequantChromaDC_idc_1(&ChromaDCU[0], ChromaQPU, pQuantTableU[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	}
//	*ppSrcDstCoeff++;

	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// V point
//		for(i=0;i<4;i++) ChromaDCV[i] = psrc[i] ;
//		Fw16s *psrc;
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQPV);
		TransformDequantChromaDC_idc_1(&ChromaDCV[0], ChromaQPV, pQuantTableV[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	}
//	*ppSrcDstCoeff++;
/*
Otherwise, the following applies.
– If qP is greater than or equal to 24, the scaled result shall be derived as follows
dij = (fij* LevelScale(QPc%6,i,j))<<(QP'/6-4) with i,j = 0,3
– Otherwise (qP is less than 24), the scaled result shall be derived as follows
dij = (fij* LevelScale(QPc%6,i,j)+2^(3-qp/6)>>(4-QP'/6) with i,j = 0,3

The bitstream shall not contain data that results in any element dij of d with i, j = 0..3 that exceeds the range of integer
values from –2(7 + bitDepth) to 2(7 + bitDepth) – 1, inclusive.
*/

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
//		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
									// change srcDstStep to srcDstUVStep
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		ACUplane[0] = ChromaDCU[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACUplane[i] = psrc[i] ;
//			Fw16s *psrc;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;

			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACUplane[i] = 0;

//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[0])));
	//			pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[1])));
	//			pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[2])));
	//			pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[3])));
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
//		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
								// change srcDstStep to srcDstUVStep
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		ACVplane[0] = ChromaDCV[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACVplane[i] = psrc[i] ;
//			Fw16s *psrc;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;

			ACVplane[0] = ChromaDCV[uBlock];
//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		else	// DC only
		{
			for(i=0;i<16;i++) ACVplane[i] = 0;

//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[0])));
	//			pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[1])));
	//			pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[2])));
	//			pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[3])));
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}		
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntra4x4MB_H264_16s8u_P2R)(
				Fw16s **ppSrcDstCoeff, 
				Fw8u *pSrcDstUPlane,	Fw8u *pSrcDstVPlane, 	Fw32u srcDstUVStep, 
				FwIntraChromaPredMode_H264 intraChromaMode, 
				Fw32u cbp4x4,
				Fw32s ChromaQPU,		Fw32u ChromaQPV, 
				Fw8u edgeType, 
				Fw16s *pQuantTableU, 	Fw16s *pQuantTableV, 
				Fw8u bypassFlag)
{
	bypassFlag;

    FwIntraChromaPredMode_H264 intra_mode = (FwIntraChromaPredMode_H264)intraChromaMode;
	FwStatus  res;

    Fw32s top  = (edgeType & D_TOP_EDGE);
    Fw32s left = (edgeType & D_LEFT_EDGE);
	Fw32s top_left = top & left;

	Fw32s availability = (!top) | ((!left)<<1) | ((!top_left)<<5);

	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw8u  *pSrcDstU, *pSrcDstV;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16       = (Fw16s *) *ppSrcDstCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	if( ppSrcDstCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQPU < 0 || ChromaQPU > 39) return fwStsOutOfRangeErr;
	if( ChromaQPV < 0 || ChromaQPV > 39) return fwStsOutOfRangeErr;

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstUPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma8x8_H264_8u_C1IR(pSrcDstVPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);


	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// U point
//		for(i=0;i<4;i++) ChromaDCU[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
		TransformDequantChromaDC_idc_1(&ChromaDCU[0], ChromaQPU, pQuantTableU[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCU[i] = 0 ;
	}
//	*ppSrcDstCoeff++;

	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// V point
//		for(i=0;i<4;i++) ChromaDCV[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
		TransformDequantChromaDC_idc_1(&ChromaDCV[0], ChromaQPV, pQuantTableV[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	}
//	*ppSrcDstCoeff++;

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACUplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 1, ChromaQP);
			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}
		else	// DC only
		{
//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 0, ChromaQP);
			for(i=0;i<16;i++) ACUplane[i] = 0;
			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[0])));
	//			pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[1])));
	//			pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[2])));
	//			pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[3])));
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACVplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														4, &DCCoeff, 1, ChromaQP);
			ACVplane[0] = ChromaDCV[uBlock];
//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		else	// DC only
		{
//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														4, &DCCoeff, 0, ChromaQP);
			for(i=0;i<16;i++) ACVplane[i] = 0;

			ACVplane[0] = ChromaDCV[uBlock];
//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[0])));
	//			pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[1])));
	//			pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[2])));
	//			pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[3])));
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}		
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraHalves4x4MB_H264_16s8u_P2R)(
				Fw16s **ppSrcDstCoeff, 
				Fw8u *pSrcDstUPlane, 
				Fw8u *pSrcDstVPlane, 
				Fw32u srcDstUVStep, 
				FwIntraChromaPredMode_H264 intraChromaMode, 
				Fw32u cbp4x4,
				Fw32s ChromaQPU, Fw32u ChromaQPV, 
				Fw8u edgeTypeTop, Fw8u edgeTypeBottom,
				Fw16s *pQuantTableU, 
				Fw16s *pQuantTableV, 
				Fw8u bypassFlag)
{
    FwIntraChromaPredMode_H264 intra_mode = (FwIntraChromaPredMode_H264)intraChromaMode;
	FwStatus  res;

    Fw32s top, left, top_left, availability;

	Fw16s ChromaDCU[4], ChromaDCV[4];
	Fw16s ACUplane[16], ACVplane[16];
	int i;
	Fw8u  *pSrcDstU, *pSrcDstV;
	Fw16s *pACUplane = &ACUplane[0];
	Fw16s *pACVplane = &ACVplane[0];

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16       = (Fw16s *) *ppSrcDstCoeff;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

	bypassFlag;

	if( ppSrcDstCoeff == NULL || pSrcDstUPlane == NULL || pSrcDstVPlane == NULL)
		return fwStsOutOfRangeErr;
	if( ChromaQPU < 0 || ChromaQPU > 39) return fwStsOutOfRangeErr;
	if( ChromaQPV < 0 || ChromaQPV > 39) return fwStsOutOfRangeErr;


    top  = (edgeTypeTop & D_TOP_EDGE);
    left = (edgeTypeTop & D_LEFT_EDGE);
	top_left = top & left;
    availability = (!top) | ((!left)<<1) | ((!top_left)<<5);
	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstUPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstVPlane,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

    top  = (edgeTypeBottom & D_TOP_EDGE);
    left = (edgeTypeBottom & D_LEFT_EDGE);
	top_left = top & left;
    availability = (!top) | ((!left)<<1) | ((!top_left)<<5);
	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstUPlane+srcDstUVStep*4,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	res = PredictIntraChroma4x8_H264_8u_C1IR(pSrcDstVPlane+srcDstUVStep*4,
                                           srcDstUVStep,
                                           intra_mode,
                                           availability);

	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// U point
//		for(i=0;i<4;i++) ChromaDCU[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCU[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCU[0], ChromaQP);
		TransformDequantChromaDC_idc_1(&ChromaDCU[0], ChromaQPU, pQuantTableU[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCU[i] = 0 ;

	}
//	*ppSrcDstCoeff++;

	if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_DC_BITPOS+1)))
	{
//		Fw16s *psrc;
//		psrc = *ppSrcDstCoeff;	// V point
//		for(i=0;i<4;i++) ChromaDCV[i] = psrc[i] ;
		for(i=0;i<4;i++) ChromaDCV[i] = *pbs16++ ;

//		fwiTransformDequantChromaDC_H264_16s_C1I(&ChromaDCV[0], ChromaQP);
		TransformDequantChromaDC_idc_1(&ChromaDCV[0], ChromaQPV, pQuantTableV[0]);
	}
	else
	{
		for(i=0;i<4;i++) ChromaDCV[i] = 0 ;
	}
//	*ppSrcDstCoeff++;

	// U AC block
	pSrcDstU = pSrcDstUPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACUplane = &ACUplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstU = pSrcDstUPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
//		Fw16s DCCoeff = ChromaDCU[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACUplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACUplane[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 1, ChromaQP);
			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}
		else	// DC only
		{
//			fwiDequantTransformResidual_H264_16s_C1I(pACUplane, // 16x16
//														8, &DCCoeff, 0, ChromaQP);
			for(i=0;i<16;i++) ACUplane[i] = 0;

			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACUplane, ChromaQPU, pQuantTableU);
			DequantTransformResidual_idc_1(ACUplane, ChromaQPU, pQuantTableU);
		}

		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstU;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACUplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstU+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[0])));
	//			pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[1])));
	//			pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[2])));
	//			pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(*(pACUplane++)+pSrcDstU[3])));
				pSrcDstU[0] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4]+pSrcDstU[0])));
				pSrcDstU[1] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+1]+pSrcDstU[1])));
				pSrcDstU[2] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+2]+pSrcDstU[2])));
				pSrcDstU[3] = (Fw8u)MAX(0,MIN(255,(ACUplane[i*4+3]+pSrcDstU[3])));

				pSrcDstU+=srcDstUVStep;	
			}
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// V AC block
	pSrcDstV = pSrcDstVPlane;
	for	(int uBlock=0; uBlock<4; uBlock++)
	{
		pACVplane = &ACVplane[0];
//		pACUplane += ((uBlock/2)*4 + (uBlock%2)*srcDstUVStep);
		pSrcDstV = pSrcDstVPlane + ( (uBlock/2)*4*srcDstUVStep + (uBlock%2)*4 );

		// dequantized DC coeff
//		Fw16s DCCoeff = ChromaDCV[uBlock];
		// if no coeffs, nothing to do
		if( cbp4x4 & (1<<(FWVC_CBP_1ST_CHROMA_AC_BITPOS+uBlock+4)))
		{
//			Fw16s *psrc;
//			psrc = *ppSrcDstCoeff;	// V point
//			for(i=0;i<16;i++) ACVplane[i] = psrc[i] ;
			for(i=0;i<16;i++) ACVplane[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														8, &DCCoeff, 1, ChromaQP);
			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		else	// DC only
		{
//			fwiDequantTransformResidual_H264_16s_C1I(pACVplane, // 16x16
//														8, &DCCoeff, 0, ChromaQP);
			for(i=0;i<16;i++) ACVplane[i] = 0;

			ACUplane[0] = ChromaDCU[uBlock];
//			DequantTransformResidual_idc_1(pACVplane, ChromaQPV, pQuantTableV);
			DequantTransformResidual_idc_1(ACVplane, ChromaQPV, pQuantTableV);
		}
		if( Dispatch_Type==DT_SSE2)
		{
			int *a;
  		    xmm7  = _mm_set1_epi8(0); 
			for (i=0;i<4;i++)
			{	
				a    = (int *)pSrcDstV;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm1 = _mm_loadl_epi64 ( (__m128i*)(ACVplane+i*4) );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				pSrcDstV+=srcDstUVStep;	
			}		
		}
		else
		{
			for (i=0;i<4;i++)
			{	
	//			pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[0])));
	//			pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[1])));
	//			pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[2])));
	//			pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(*(pACVplane++)+pSrcDstV[3])));
				pSrcDstV[0] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4]+pSrcDstV[0])));
				pSrcDstV[1] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+1]+pSrcDstV[1])));
				pSrcDstV[2] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+2]+pSrcDstV[2])));
				pSrcDstV[3] = (Fw8u)MAX(0,MIN(255,(ACVplane[i*4+3]+pSrcDstV[3])));

				pSrcDstV+=srcDstUVStep;	
			}
		}
//		ppSrcDstCoeff++;	// next block
	}	// for uBlock

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaInter8x8MB_H264_16s8u_C1R)(
				Fw16s **ppSrcDstCoeff, 
				Fw8u *pSrcDstYPlane, 
				Fw32u srcDstYStep, 
				Fw32u cbp8x8, 
				Fw32s QP, 
				Fw16s *pQuantTable, 
				Fw8u bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
//	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;

	bypassFlag;

	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<2; uBlock++, uCBPMask <<= 1)
	{
//		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp8x8 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;

//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
			pSrcDstYPlane2 = pSrcDstYPlane + (Scan_Luma4x4[uBlock]%4)*4 + 
											 (Scan_Luma4x4[uBlock]/4)*4*srcDstYStep;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}	
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
//		else
//		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
//		}
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// Y point

	return fwStsNoErr;
}
#endif

typedef enum
{
    FW_8x8_VERT     = 0,
    FW_8x8_HOR      = 1,
    FW_8x8_DC       = 2,
    FW_8x8_DIAG_DL  = 3,
    FW_8x8_DIAG_DR  = 4,
    FW_8x8_VR       = 5,
    FW_8x8_HD       = 6,
    FW_8x8_VL       = 7,
    FW_8x8_HU       = 8

} fwintra8x8PredMode_H264;
/*
 *************************************************************************************
 * \brief
 *    Prefiltering for Intra8x8 prediction
 *************************************************************************************
 */
 // Predictor array index definitions
#define P_Z (PredPel[0])
#define P_A (PredPel[1])
#define P_B (PredPel[2])
#define P_C (PredPel[3])
#define P_D (PredPel[4])
#define P_E (PredPel[5])
#define P_F (PredPel[6])
#define P_G (PredPel[7])
#define P_H (PredPel[8])
#define P_I (PredPel[9])
#define P_J (PredPel[10])
#define P_K (PredPel[11])
#define P_L (PredPel[12])
#define P_M (PredPel[13])
#define P_N (PredPel[14])
#define P_O (PredPel[15])
#define P_P (PredPel[16])
#define P_Q (PredPel[17])
#define P_R (PredPel[18])
#define P_S (PredPel[19])
#define P_T (PredPel[20])
#define P_U (PredPel[21])
#define P_V (PredPel[22])
#define P_W (PredPel[23])
#define P_X (PredPel[24])
void static LowPassForIntra8x8Pred(int *PredPel, int block_up_left, int block_up, int block_left)
{
  int i;
  int LoopArray[25];
 

  for(i = 0; i < 25; i++)
     LoopArray[i] = PredPel[i] ;

 	if(block_up)
	{
		if(block_up_left) 
		{
			LoopArray[1] = ((&P_Z)[0] + ((&P_Z)[1]<<1) + (&P_Z)[2] + 2)>>2;
		}
		else
			LoopArray[1] = ((&P_Z)[1] + ((&P_Z)[1]<<1) + (&P_Z)[2] + 2)>>2; 


		for(i = 2; i <16; i++)
		{
			LoopArray[i] = ((&P_Z)[i-1] + ((&P_Z)[i]<<1) + (&P_Z)[i+1] + 2)>>2;
		}
		LoopArray[16] = (P_P + (P_P<<1) + P_O + 2)>>2;
	}

	if(block_up_left) 
	{
		
		if(block_up && block_left)
		{
				LoopArray[0] = (P_Q + (P_Z<<1) + P_A +2)>>2;
		}
		else
		{
			if(block_up)
				LoopArray[0] = (P_Z + (P_Z<<1) + P_A +2)>>2;
			else
				if(block_left)
					LoopArray[0] = (P_Z + (P_Z<<1) + P_Q +2)>>2;
		}

	}

	if(block_left)
	{
		if(block_up_left)
			LoopArray[17] = (P_Z + (P_Q<<1) + P_R + 2)>>2; 
		else
			LoopArray[17] = (P_Q + (P_Q<<1) + P_R + 2)>>2;

		for(i = 18; i <24; i++)
		{
			LoopArray[i] = ((&P_Z)[i-1] + ((&P_Z)[i]<<1) + (&P_Z)[i+1] + 2)>>2;
		}
		LoopArray[24] = (P_W + (P_X<<1) + P_X + 2)>>2;
	}

  for(i = 0; i < 25; i++)
    PredPel[i] = LoopArray[i];
}
/*
; output: %1 = (t[n-1] + t[n]*2 + t[n+1] + 2) >> 2
%macro PRED8x8_LOWPASS0 6
    mov%6       %5, %2
    pavgb       %2, %3
    pxor        %3, %5
    mov%6       %1, %4
    pand        %3, [pb_1 GLOBAL]
    psubusb     %2, %3
    pavgb       %1, %2
*/

#define PRED8x8_LOWPASS(m1, m2, m3, m4, m5)	\
{	\
    m5 = m2;\
	m2 = _mm_avg_epu8(m2, m3);\
	m3 = _mm_xor_si128 (m3, m5);\
    m1 = m4;\
	m3 = _mm_and_si128(m3, _mm_set1_epi8(1) );\
	m2 = _mm_subs_epu8(m2, m3);\
	m1 = _mm_avg_epu8(m1, m2);\
}

FwStatus static iPredictIntra_8x8_H264_8u_C1IR(Fw8u* pSrcDst, 
											Fw32s srcdstStep,
											fwintra8x8PredMode_H264 predMode, 
											Fw32s availability)
{
	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if(srcdstStep < 4) return fwStsStepErr;
	if(predMode < 0 || predMode > 8) return fwStsOutOfRangeErr;

  int i,j;
  int s0;
  SYS_FORCEALIGN_16  int PredPel[25];  // array of predictor pels
//  imgpel **imgY = dec_picture->imgY;  // For MB level frame/field coding tools -- set default to imgY

//  PixelPos pix_a[8];
//  PixelPos pix_b, pix_c, pix_d;

  int block_available_up=0;
  int block_available_left=0;
  int block_available_up_left=0;
  int block_available_up_right=0;
//  int BLOCK_SIZE = 8;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7;

	if(FW_LEFT & availability)			block_available_left = 1;
	if(FW_UPPER_LEFT & availability)	block_available_up_left = 1;
	if(FW_UPPER & availability)		block_available_up = 1;
	if(FW_UPPER_RIGHT & availability)	block_available_up_right = 1;
/*
  int ioff = (b8%2)*8;
  int joff = (b8/2)*8;

  for (i=0;i<8;i++)
  {
    getNeighbour(mb_nr, ioff -1 , joff +i , 1, &pix_a[i]);
  }

  getNeighbour(mb_nr, ioff    , joff -1 , 1, &pix_b);
  getNeighbour(mb_nr, ioff +8 , joff -1 , 1, &pix_c);
  getNeighbour(mb_nr, ioff -1 , joff -1 , 1, &pix_d);
  
  pix_c.available = pix_c.available &&!(ioff == 8 && joff == 8);
  if (active_pps->constrained_intra_pred_flag)
  {
    for (i=0, block_available_left=1; i<8;i++)
      block_available_left  &= pix_a[i].available ? img->intra_block[pix_a[i].mb_addr]: 0;
    block_available_up       = pix_b.available ? img->intra_block [pix_b.mb_addr] : 0;
    block_available_up_right = pix_c.available ? img->intra_block [pix_c.mb_addr] : 0;
    block_available_up_left  = pix_d.available ? img->intra_block [pix_d.mb_addr] : 0;
  }
  else
  {
    block_available_left     = pix_a[0].available;
    block_available_up       = pix_b.available;
    block_available_up_right = pix_c.available;
    block_available_up_left  = pix_d.available;
  }
*/


//  *left_available = block_available_left;
//  *up_available   = block_available_up;
//  *all_available  = block_available_up && block_available_left && block_available_up_left;

  // form predictor pels
  // form predictor pels
  if (block_available_up)
  {
    P_A = pSrcDst[1];
    P_B = pSrcDst[2];
    P_C = pSrcDst[3];
    P_D = pSrcDst[4];
    P_E = pSrcDst[5];
    P_F = pSrcDst[6];
    P_G = pSrcDst[7];
    P_H = pSrcDst[8];
  }
  else
  {
    P_A = P_B = P_C = P_D = P_E = P_F = P_G = P_H = 128;
  }

  if (block_available_up_right)
  {
    P_I = pSrcDst[9+1];
    P_J = pSrcDst[9+2];
    P_K = pSrcDst[9+3];
    P_L = pSrcDst[9+4];
    P_M = pSrcDst[9+5];
    P_N = pSrcDst[9+6];
    P_O = pSrcDst[9+7];
    P_P = pSrcDst[9+8];

  }
  else
  {
    P_I = P_J = P_K = P_L = P_M = P_N = P_O = P_P = P_H;
  }

  if (block_available_left)
  {
    P_Q = pSrcDst[srcdstStep*1];
    P_R = pSrcDst[srcdstStep*2];
    P_S = pSrcDst[srcdstStep*3];
    P_T = pSrcDst[srcdstStep*4];
    P_U = pSrcDst[srcdstStep*5];
    P_V = pSrcDst[srcdstStep*6];
    P_W = pSrcDst[srcdstStep*7];
    P_X = pSrcDst[srcdstStep*8];
  }
  else
  {
    P_Q = P_R = P_S = P_T = P_U = P_V = P_W = P_X = 128;
  }

  if (block_available_up_left)
  {
    P_Z = pSrcDst[0];
  }
  else
  {
    P_Z = 128;
  }
  
  LowPassForIntra8x8Pred(&(P_Z), block_available_up_left, block_available_up, block_available_left);

//pSrcDst[x][y]
  switch(predMode)
  {
  case FW_8x8_DC:
    s0 = 0;
    if (block_available_up && block_available_left)
    {   
      // no edge
      s0 = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 8) >> 4;
    }
    else if (!block_available_up && block_available_left)
    {
      // upper edge
      s0 = (P_Q + P_R + P_S + P_T + P_U + P_V + P_W + P_X + 4) >> 3;             
    }
    else if (block_available_up && !block_available_left)
    {
      // left edge
      s0 = (P_A + P_B + P_C + P_D + P_E + P_F + P_G + P_H + 4) >> 3;             
    }
    else //if (!block_available_up && !block_available_left)
    {
      // top left corner, nothing to predict from
      s0 = 128; //img->dc_pred_value;                           
    }

	if( Dispatch_Type==DT_SSE2)
	{
		xmm7  = _mm_set1_epi8((Fw8u)s0); 
		for (j = 0; j < 8; j++) {
			_mm_storel_epi64( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else 
	{
//		for(i = 0; i < 2*BLOCK_SIZE; i++)
//		  for(j = 0; j < 2*BLOCK_SIZE; j++)
		for(i=0; i <8; i++)
		  for(j=0; j <8; j++)
			pSrcDst[i*srcdstStep+ j] = (Fw8u) s0;// pSrcDst[i*srcdstStep + j] = s0;
	}
    break;

  case FW_8x8_VERT:

	if( Dispatch_Type==DT_SSE2)
	{
		xmm7 = _mm_loadl_epi64( (__m128i*)(&PredPel[1]) );
		for (j = 0; j < 8; j++) {
			_mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else 
	{
		for (i=0; i < 8; i++) //    for (i=0; i < 2*BLOCK_SIZE; i++)
		{
		  pSrcDst[i*srcdstStep + 0] = 
		  pSrcDst[i*srcdstStep + 1] = 
		  pSrcDst[i*srcdstStep + 2] = 
		  pSrcDst[i*srcdstStep + 3] = 
		  pSrcDst[i*srcdstStep + 4] = 
		  pSrcDst[i*srcdstStep + 5] = 
		  pSrcDst[i*srcdstStep + 6] = 
		  pSrcDst[i*srcdstStep + 7] = (Fw8u) (&P_A)[i];
		}
	}
    break;
  case FW_8x8_HOR:
	if( Dispatch_Type==DT_SSE2)
	{
		Fw8u tmp;
		for (j = 0; j < 8; j++) {
		  tmp = (Fw8u) (&P_A)[j];
  		  xmm7  = _mm_set1_epi8((Fw8u)tmp); 
		  _mm_store_si128( (__m128i*)(pSrcDst+j*srcdstStep), xmm7 ); // store 16 bytes
		}
	}
	else
	{
		for (j=0; j < 8; j++)
		{
		  pSrcDst[0*srcdstStep + j]  = 
		  pSrcDst[1*srcdstStep + j]  = 
		  pSrcDst[2*srcdstStep + j]  = 
		  pSrcDst[3*srcdstStep + j]  = 
		  pSrcDst[4*srcdstStep + j]  = 
		  pSrcDst[5*srcdstStep + j]  = 
		  pSrcDst[6*srcdstStep + j]  = 
		  pSrcDst[7*srcdstStep + j]  = (Fw8u) (&P_Q)[j];
		}
	}
    break;

  case FW_8x8_DIAG_DL:
    // Mode DIAG_DOWN_LEFT_PRED
	if( Dispatch_Type==DT_SSE2)
	{	
//    movdqa      xmm3, [parm2q+16]
//    movdqu      xmm2, [parm2q+17]
//    movdqa      xmm1, xmm3
//    pslldq      xmm1, 1
//    PRED8x8_LOWPASS_XMM xmm0, xmm1, xmm2, xmm3, xmm4

		xmm3 = _mm_loadu_si128 ((__m128i*)(&PredPel[1])  );
		xmm2 = _mm_loadu_si128 ((__m128i*)(&PredPel[2])  );
		xmm1 = xmm3;
		xmm1 = _mm_slli_si128(xmm1, 1);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm3, xmm4);

//%assign Y 0
//%rep 8
//    psrldq      xmm0, 1
//    movq        [parm1q+Y*FDEC_STRIDE], xmm0
//%assign Y (Y+1)
		for(i=0;i<8;i++)
		{
			xmm0 = _mm_srli_si128(xmm0, 1);
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm0 );
		}
	}
	else
	{
		pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_A + P_C + 2*(P_B) + 2) >> 2);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 0] = (Fw8u) ((P_B + P_D + 2*(P_C) + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] =
		pSrcDst[1*srcdstStep + 1] =
		pSrcDst[2*srcdstStep + 0] = (Fw8u) ((P_C + P_E + 2*(P_D) + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 0] = (Fw8u) ((P_D + P_F + 2*(P_E) + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 0] = (Fw8u) ((P_E + P_G + 2*(P_F) + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 0] = (Fw8u) ((P_F + P_H + 2*(P_G) + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_G + P_I + 2*(P_H) + 2) >> 2);
		pSrcDst[0*srcdstStep + 7] = 
		pSrcDst[1*srcdstStep + 6] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_H + P_J + 2*(P_I) + 2) >> 2);
		pSrcDst[1*srcdstStep + 7] = 
		pSrcDst[2*srcdstStep + 6] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_I + P_K + 2*(P_J) + 2) >> 2);
		pSrcDst[2*srcdstStep + 7] = 
		pSrcDst[3*srcdstStep + 6] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_J + P_L + 2*(P_K) + 2) >> 2);
		pSrcDst[3*srcdstStep + 7] = 
		pSrcDst[4*srcdstStep + 6] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_K + P_M + 2*(P_L) + 2) >> 2);
		pSrcDst[4*srcdstStep + 7] = 
		pSrcDst[5*srcdstStep + 6] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_L + P_N + 2*(P_M) + 2) >> 2);
		pSrcDst[5*srcdstStep + 7] = 
		pSrcDst[6*srcdstStep + 6] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_M + P_O + 2*(P_N) + 2) >> 2);
		pSrcDst[6*srcdstStep + 7] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_N + P_P + 2*(P_O) + 2) >> 2);
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_O + 3*(P_P) + 2) >> 2);
	}
    break;

  case FW_8x8_VL:
	if( Dispatch_Type==DT_SSE2)
	{	
//    movdqa      xmm4, [parm2q+16]
//    movdqa      xmm2, xmm4
//    movdqa      xmm1, xmm4
//    movdqa      xmm3, xmm4
//    psrldq      xmm2, 1
//    pslldq      xmm1, 1
//    pavgb       xmm3, xmm2
//    PRED8x8_LOWPASS_XMM xmm0, xmm1, xmm2, xmm4, xmm5

		xmm4 = _mm_loadu_si128 ((__m128i*)(&PredPel[1])  );
		xmm2 = xmm4;
		xmm1 = xmm4;
		xmm3 = xmm4;
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm1 = _mm_slli_si128 (xmm1, 1);
		xmm3 = _mm_avg_epu8(xmm3, xmm2);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm4, xmm5);
//; xmm0: (t0 + 2*t1 + t2 + 2) >> 2
//; xmm3: (t0 + t1 + 1) >> 1

		xmm1 = xmm0;
		_mm_unpacklo_epi8 (xmm0, xmm3);	// LO
		_mm_unpackhi_epi8 (xmm1, xmm3);	// HI

//%assign Y 0
//%rep 3
//    psrldq      xmm0, 1
//    movq        [parm1q+ Y   *FDEC_STRIDE], xmm3
//    movq        [parm1q+(Y+1)*FDEC_STRIDE], xmm0
//    psrldq      xmm3, 1
//%assign Y (Y+2)
//%endrep
//    psrldq      xmm0, 1
//    movq        [parm1q+ Y   *FDEC_STRIDE], xmm3
//    movq        [parm1q+(Y+1)*FDEC_STRIDE], xmm0

		for(i=0;i<4;i++)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm0 );
			xmm0 = _mm_srli_si128(xmm0, 2);
		}
		for(;i<8;i++)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+i*srcdstStep), xmm1 );
			xmm1 = _mm_srli_si128(xmm1, 2);
		}
/*
		int y;
		for(y=0, i=0;i<3;i++, y+=2)
		{
			xmm0 = _mm_srli_si128(xmm0, 1);
			_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm3 );
			_mm_storel_epi64( (__m128i*)(pSrcDst+(y+1)*srcdstStep), xmm0 );
			xmm3 = _mm_srli_si128(xmm3, 1);
		}
		xmm0 = _mm_srli_si128(xmm0, 1);
		_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm3 );
		_mm_storel_epi64( (__m128i*)(pSrcDst+(y+1)*srcdstStep), xmm0 );
*/
	}
	else
	{
		pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_A + P_B + 1) >> 1);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[0*srcdstStep + 2] = (Fw8u) ((P_B + P_C + 1) >> 1);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[0*srcdstStep + 4] = (Fw8u) ((P_C + P_D + 1) >> 1);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[0*srcdstStep + 6] = (Fw8u) ((P_D + P_E + 1) >> 1);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 6] = (Fw8u) ((P_E + P_F + 1) >> 1);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 6] = (Fw8u) ((P_F + P_G + 1) >> 1);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 6] = (Fw8u) ((P_G + P_H + 1) >> 1);
		pSrcDst[7*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 6] = (Fw8u) ((P_H + P_I + 1) >> 1);
		pSrcDst[7*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 6] = (Fw8u) ((P_I + P_J + 1) >> 1);
		pSrcDst[7*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_J + P_K + 1) >> 1);
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_K + P_L + 1) >> 1);
		pSrcDst[0*srcdstStep + 1] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[0*srcdstStep + 3] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[0*srcdstStep + 5] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_F + P_H + 2*P_G + 2) >> 2);
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_G + P_I + 2*P_H + 2) >> 2);
		pSrcDst[7*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_H + P_J + 2*P_I + 2) >> 2);
		pSrcDst[7*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_I + P_K + 2*P_J + 2) >> 2);
		pSrcDst[7*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_J + P_L + 2*P_K + 2) >> 2);
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_K + P_M + 2*P_L + 2) >> 2);
	}
	break;
  case FW_8x8_DIAG_DR:
    // Mode DIAG_DOWN_RIGHT_PRED
	if( Dispatch_Type==DT_SSE2) 
	{	
//    movdqu      xmm3, [parm2q+8]
//    movdqu      xmm1, [parm2q+7]
//    movdqa      xmm2, xmm3
//    psrldq      xmm2, 1
//    PRED8x8_LOWPASS_XMM xmm0, xmm1, xmm2, xmm3, xmm4

	    SYS_FORCEALIGN_16  int PredPel2[25];  // array of predictor pels

		int y=0;
		for(i=8;i>=0;i--, y++) 	PredPel2[y] = PredPel[i];
		for(i=17;i<=24;i++, y++)PredPel2[y] = PredPel[i];

		xmm3 = _mm_loadu_si128 ((__m128i*)(&PredPel2[0])  );
		xmm1 = _mm_loadu_si128 ((__m128i*)(&PredPel2[1])  );
		xmm2 = xmm3;
		xmm2 = _mm_srli_si128(xmm2, 1);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm3, xmm4);
//    movdqa      xmm1, xmm0
//    psrldq      xmm1, 1
		xmm1 = xmm0;
		xmm1 = _mm_srli_si128(xmm1, 1);

//%assign Y 7
//%rep 3
//    movq        [parm1q+Y*FDEC_STRIDE], xmm0
//    movq        [parm1q+(Y-1)*FDEC_STRIDE], xmm1
//    psrldq      xmm0, 2
//    psrldq      xmm1, 2
//%assign Y (Y-2)
//%endrep
//    movq        [parm1q+1*FDEC_STRIDE], xmm0
//    movq        [parm1q+0*FDEC_STRIDE], xmm1
		for(y=7, i=0;i<3;i++, y-=2)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm0 );
			_mm_storel_epi64( (__m128i*)(pSrcDst+(y-1)*srcdstStep), xmm1 );
			xmm0 = _mm_srli_si128(xmm0, 2);
			xmm1 = _mm_srli_si128(xmm1, 2);
		}
		_mm_storel_epi64( (__m128i*)(pSrcDst+srcdstStep), xmm0 );
		_mm_storel_epi64( (__m128i*)(pSrcDst), xmm1 );

	}
	else
	{
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_X + P_V + 2*(P_W) + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = 
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_W + P_U + 2*(P_V) + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] = 
		pSrcDst[1*srcdstStep + 6] = 
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_V + P_T + 2*(P_U) + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] = 
		pSrcDst[1*srcdstStep + 5] = 
		pSrcDst[2*srcdstStep + 6] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_U + P_S + 2*(P_T) + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] = 
		pSrcDst[1*srcdstStep + 4] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 6] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_T + P_R + 2*(P_S) + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 6] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_S + P_Q + 2*(P_R) + 2) >> 2);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 6] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_R + P_Z + 2*(P_Q) + 2) >> 2);
		pSrcDst[0*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 6] = 
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_Q + P_A + 2*(P_Z) + 2) >> 2);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_Z + P_B + 2*(P_A) + 2) >> 2);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_A + P_C + 2*(P_B) + 2) >> 2);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_B + P_D + 2*(P_C) + 2) >> 2);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_C + P_E + 2*(P_D) + 2) >> 2);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_D + P_F + 2*(P_E) + 2) >> 2);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_E + P_G + 2*(P_F) + 2) >> 2);
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_F + P_H + 2*(P_G) + 2) >> 2);
	}
    break;

  case  FW_8x8_VR:/* diagonal prediction -22.5 deg to horizontal plane */
#if 0
	if( Dispatch_Type==DT_SSE2) ///////// ----------- working
	{	
//    predict_8x8_vr_core_mmxext( src, edge );

//    movq        mm2, [parm2q+16]
//    movq        mm3, [parm2q+15]
//    movq        mm1, [parm2q+14]
//    movq        mm4, mm3
//    pavgb       mm3, mm2
//    PRED8x8_LOWPASS mm0, mm1, mm2, mm4, mm7

		xmm2 = _mm_loadu_si128 ((__m128i*)(&PredPel[1])  );
		xmm3 = _mm_loadu_si128 ((__m128i*)(&PredPel[2])  );
		xmm1 = _mm_loadu_si128 ((__m128i*)(&PredPel[2])  );
		xmm4 = xmm3;
		xmm3 = _mm_avg_epu8(xmm3, xmm2);
		PRED8x8_LOWPASS( xmm0, xmm1, xmm2, xmm4, xmm7);

//%assign Y 0
//%rep 3
//    movq        [parm1q+ Y   *FDEC_STRIDE], mm3
//    movq        [parm1q+(Y+1)*FDEC_STRIDE], mm0
//    psllq       mm3, 8
//    psllq       mm0, 8
//%assign Y (Y+2)
//%endrep
//    movq        [parm1q+ Y   *FDEC_STRIDE], mm3
//    movq        [parm1q+(Y+1)*FDEC_STRIDE], mm0
		int y;
		for(y=0, i=0;i<3;i++, y+=2)
		{
			_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm3 );
			_mm_storel_epi64( (__m128i*)(pSrcDst+(y+1)*srcdstStep), xmm0 );
			xmm3 = _mm_srli_si128(xmm3, 8);
			xmm0 = _mm_srli_si128(xmm0, 8);
		}
		_mm_storel_epi64( (__m128i*)(pSrcDst+y*srcdstStep), xmm3 );
		_mm_storel_epi64( (__m128i*)(pSrcDst+(y+1)*srcdstStep), xmm0 );

/*
    predict_8x8_vr_core_mmxext( src, edge );
    {
        PREDICT_8x8_LOAD_TOPLEFT
        PREDICT_8x8_LOAD_LEFT
        SRC(0,2)=SRC(1,4)=SRC(2,6)= (l1 + 2*l0 + lt + 2) >> 2;
        SRC(0,3)=SRC(1,5)=SRC(2,7)= (l2 + 2*l1 + l0 + 2) >> 2;
        SRC(0,4)=SRC(1,6)= (l3 + 2*l2 + l1 + 2) >> 2;
        SRC(0,5)=SRC(1,7)= (l4 + 2*l3 + l2 + 2) >> 2;
        SRC(0,6)= (l5 + 2*l4 + l3 + 2) >> 2;
        SRC(0,7)= (l6 + 2*l5 + l4 + 2) >> 2;
    }
*/
	}
	else
#endif
	{
		pSrcDst[0*srcdstStep + 0] = 
		pSrcDst[1*srcdstStep + 2] = 
		pSrcDst[2*srcdstStep + 4] = 
		pSrcDst[3*srcdstStep + 6] = (Fw8u) ((P_Z + P_A + 1) >> 1);
		pSrcDst[1*srcdstStep + 0] = 
		pSrcDst[2*srcdstStep + 2] = 
		pSrcDst[3*srcdstStep + 4] = 
		pSrcDst[4*srcdstStep + 6] = (Fw8u) ((P_A + P_B + 1) >> 1);
		pSrcDst[2*srcdstStep + 0] = 
		pSrcDst[3*srcdstStep + 2] = 
		pSrcDst[4*srcdstStep + 4] = 
		pSrcDst[5*srcdstStep + 6] = (Fw8u) ((P_B + P_C + 1) >> 1);
		pSrcDst[3*srcdstStep + 0] = 
		pSrcDst[4*srcdstStep + 2] = 
		pSrcDst[5*srcdstStep + 4] = 
		pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_C + P_D + 1) >> 1);
		pSrcDst[4*srcdstStep + 0] = 
		pSrcDst[5*srcdstStep + 2] = 
		pSrcDst[6*srcdstStep + 4] = 
		pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_D + P_E + 1) >> 1);
		pSrcDst[5*srcdstStep + 0] = 
		pSrcDst[6*srcdstStep + 2] = 
		pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_E + P_F + 1) >> 1);
		pSrcDst[6*srcdstStep + 0] = 
		pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_F + P_G + 1) >> 1);
		pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_G + P_H + 1) >> 1);
		pSrcDst[0*srcdstStep + 1] = 
		pSrcDst[1*srcdstStep + 3] = 
		pSrcDst[2*srcdstStep + 5] = 
		pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_Q + P_A + 2*P_Z + 2) >> 2);
		pSrcDst[1*srcdstStep + 1] = 
		pSrcDst[2*srcdstStep + 3] = 
		pSrcDst[3*srcdstStep + 5] = 
		pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_Z + P_B + 2*P_A + 2) >> 2);
		pSrcDst[2*srcdstStep + 1] = 
		pSrcDst[3*srcdstStep + 3] = 
		pSrcDst[4*srcdstStep + 5] = 
		pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
		pSrcDst[3*srcdstStep + 1] = 
		pSrcDst[4*srcdstStep + 3] = 
		pSrcDst[5*srcdstStep + 5] = 
		pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
		pSrcDst[4*srcdstStep + 1] = 
		pSrcDst[5*srcdstStep + 3] = 
		pSrcDst[6*srcdstStep + 5] = 
		pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
		pSrcDst[5*srcdstStep + 1] = 
		pSrcDst[6*srcdstStep + 3] = 
		pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
		pSrcDst[6*srcdstStep + 1] = 
		pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
		pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_F + P_H + 2*P_G + 2) >> 2);
		pSrcDst[0*srcdstStep + 2] =
		pSrcDst[1*srcdstStep + 4] =
		pSrcDst[2*srcdstStep + 6] = (Fw8u) ((P_R + P_Z + 2*P_Q + 2) >> 2);
		pSrcDst[0*srcdstStep + 3] =
		pSrcDst[1*srcdstStep + 5] =
		pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_S + P_Q + 2*P_R + 2) >> 2);
		pSrcDst[0*srcdstStep + 4] =
		pSrcDst[1*srcdstStep + 6] = (Fw8u) ((P_T + P_R + 2*P_S + 2) >> 2);
		pSrcDst[0*srcdstStep + 5] =
		pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_U + P_S + 2*P_T + 2) >> 2);
		pSrcDst[0*srcdstStep + 6] = (Fw8u) ((P_V + P_T + 2*P_U + 2) >> 2);
		pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_W + P_U + 2*P_V + 2) >> 2);
	}
    break;

  case  FW_8x8_HD:/* diagonal prediction -22.5 deg to horizontal plane */
   
    pSrcDst[0*srcdstStep + 0] = 
    pSrcDst[2*srcdstStep + 1] = 
    pSrcDst[4*srcdstStep + 2] = 
    pSrcDst[6*srcdstStep + 3] = (Fw8u) ((P_Q + P_Z + 1) >> 1);
    pSrcDst[0*srcdstStep + 1] = 
    pSrcDst[2*srcdstStep + 2] = 
    pSrcDst[4*srcdstStep + 3] = 
    pSrcDst[6*srcdstStep + 4] = (Fw8u) ((P_R + P_Q + 1) >> 1);
    pSrcDst[0*srcdstStep + 2] = 
    pSrcDst[2*srcdstStep + 3] = 
    pSrcDst[4*srcdstStep + 4] = 
    pSrcDst[6*srcdstStep + 5] = (Fw8u) ((P_S + P_R + 1) >> 1);
    pSrcDst[0*srcdstStep + 3] = 
    pSrcDst[2*srcdstStep + 4] = 
    pSrcDst[4*srcdstStep + 5] = 
    pSrcDst[6*srcdstStep + 6] = (Fw8u) ((P_T + P_S + 1) >> 1);
    pSrcDst[0*srcdstStep + 4] = 
    pSrcDst[2*srcdstStep + 5] = 
    pSrcDst[4*srcdstStep + 6] = 
    pSrcDst[6*srcdstStep + 7] = (Fw8u) ((P_U + P_T + 1) >> 1);
    pSrcDst[0*srcdstStep + 5] = 
    pSrcDst[2*srcdstStep + 6] = 
    pSrcDst[4*srcdstStep + 7] = (Fw8u) ((P_V + P_U + 1) >> 1);
    pSrcDst[0*srcdstStep + 6] = 
    pSrcDst[2*srcdstStep + 7] = (Fw8u) ((P_W + P_V + 1) >> 1);
    pSrcDst[0*srcdstStep + 7] = (Fw8u) ((P_X + P_W + 1) >> 1);
    pSrcDst[1*srcdstStep + 0] =
    pSrcDst[3*srcdstStep + 1] =
    pSrcDst[5*srcdstStep + 2] =
    pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_Q + P_A + 2*P_Z + 2) >> 2);
    pSrcDst[1*srcdstStep + 1] =
    pSrcDst[3*srcdstStep + 2] =
    pSrcDst[5*srcdstStep + 3] =
    pSrcDst[7*srcdstStep + 4] = (Fw8u) ((P_Z + P_R + 2*P_Q + 2) >> 2);
    pSrcDst[1*srcdstStep + 2] =
    pSrcDst[3*srcdstStep + 3] =
    pSrcDst[5*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 5] = (Fw8u) ((P_Q + P_S + 2*P_R + 2) >> 2);
    pSrcDst[1*srcdstStep + 3] =
    pSrcDst[3*srcdstStep + 4] =
    pSrcDst[5*srcdstStep + 5] =
    pSrcDst[7*srcdstStep + 6] = (Fw8u) ((P_R + P_T + 2*P_S + 2) >> 2);
    pSrcDst[1*srcdstStep + 4] =
    pSrcDst[3*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 6] =
    pSrcDst[7*srcdstStep + 7] = (Fw8u) ((P_S + P_U + 2*P_T + 2) >> 2);
    pSrcDst[1*srcdstStep + 5] =
    pSrcDst[3*srcdstStep + 6] =
    pSrcDst[5*srcdstStep + 7] = (Fw8u) ((P_T + P_V + 2*P_U + 2) >> 2);
    pSrcDst[1*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 7] = (Fw8u) ((P_U + P_W + 2*P_V + 2) >> 2);
    pSrcDst[1*srcdstStep + 7] = (Fw8u) ((P_V + P_X + 2*P_W + 2) >> 2);
    pSrcDst[2*srcdstStep + 0] = 
    pSrcDst[4*srcdstStep + 1] = 
    pSrcDst[6*srcdstStep + 2] = (Fw8u) ((P_Z + P_B + 2*P_A + 2) >> 2);
    pSrcDst[3*srcdstStep + 0] = 
    pSrcDst[5*srcdstStep + 1] = 
    pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_A + P_C + 2*P_B + 2) >> 2);
    pSrcDst[4*srcdstStep + 0] = 
    pSrcDst[6*srcdstStep + 1] = (Fw8u) ((P_B + P_D + 2*P_C + 2) >> 2);
    pSrcDst[5*srcdstStep + 0] = 
    pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_C + P_E + 2*P_D + 2) >> 2);
    pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_D + P_F + 2*P_E + 2) >> 2);
    pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_E + P_G + 2*P_F + 2) >> 2);
    break;

  case  FW_8x8_HU:/* diagonal prediction -22.5 deg to horizontal plane */

    pSrcDst[0*srcdstStep + 0] = (Fw8u) ((P_Q + P_R + 1) >> 1);
    pSrcDst[0*srcdstStep + 1] =
    pSrcDst[2*srcdstStep + 0] = (Fw8u) ((P_R + P_S + 1) >> 1);
    pSrcDst[0*srcdstStep + 2] =
    pSrcDst[2*srcdstStep + 1] =
    pSrcDst[4*srcdstStep + 0] = (Fw8u) ((P_S + P_T + 1) >> 1);
    pSrcDst[0*srcdstStep + 3] =
    pSrcDst[2*srcdstStep + 2] =
    pSrcDst[4*srcdstStep + 1] =
    pSrcDst[6*srcdstStep + 0] = (Fw8u) ((P_T + P_U + 1) >> 1);
    pSrcDst[0*srcdstStep + 4] =
    pSrcDst[2*srcdstStep + 3] =
    pSrcDst[4*srcdstStep + 2] =
    pSrcDst[6*srcdstStep + 1] = (Fw8u) ((P_U + P_V + 1) >> 1);
    pSrcDst[0*srcdstStep + 5] =
    pSrcDst[2*srcdstStep + 4] =
    pSrcDst[4*srcdstStep + 3] =
    pSrcDst[6*srcdstStep + 2] = (Fw8u) ((P_V + P_W + 1) >> 1);
    pSrcDst[0*srcdstStep + 6] =
    pSrcDst[2*srcdstStep + 5] =
    pSrcDst[4*srcdstStep + 4] =
    pSrcDst[6*srcdstStep + 3] = (Fw8u) ((P_W + P_X + 1) >> 1);
    pSrcDst[6*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 4] =
    pSrcDst[4*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 5] =
    pSrcDst[6*srcdstStep + 5] =
    pSrcDst[7*srcdstStep + 5] =
    pSrcDst[2*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 6] =
    pSrcDst[4*srcdstStep + 6] =
    pSrcDst[5*srcdstStep + 6] =
    pSrcDst[6*srcdstStep + 6] =
    pSrcDst[7*srcdstStep + 6] =
    pSrcDst[0*srcdstStep + 7] =
    pSrcDst[1*srcdstStep + 7] =
    pSrcDst[2*srcdstStep + 7] =
    pSrcDst[3*srcdstStep + 7] =
    pSrcDst[4*srcdstStep + 7] =
    pSrcDst[5*srcdstStep + 7] =
    pSrcDst[6*srcdstStep + 7] =
    pSrcDst[7*srcdstStep + 7] = (Fw8u) P_X;
    pSrcDst[1*srcdstStep + 6] =
    pSrcDst[3*srcdstStep + 5] =
    pSrcDst[5*srcdstStep + 4] =
    pSrcDst[7*srcdstStep + 3] = (Fw8u) ((P_W + 3*P_X + 2) >> 2);
    pSrcDst[1*srcdstStep + 5] =
      pSrcDst[3*srcdstStep + 4] =
      pSrcDst[5*srcdstStep + 3] =
      pSrcDst[7*srcdstStep + 2] = (Fw8u) ((P_X + P_V + 2*P_W + 2) >> 2);
    pSrcDst[1*srcdstStep + 4] =
      pSrcDst[3*srcdstStep + 3] =
      pSrcDst[5*srcdstStep + 2] =
      pSrcDst[7*srcdstStep + 1] = (Fw8u) ((P_W + P_U + 2*P_V + 2) >> 2);
    pSrcDst[1*srcdstStep + 3] =
      pSrcDst[3*srcdstStep + 2] =
      pSrcDst[5*srcdstStep + 1] =
      pSrcDst[7*srcdstStep + 0] = (Fw8u) ((P_V + P_T + 2*P_U + 2) >> 2);
    pSrcDst[1*srcdstStep + 2] =
      pSrcDst[3*srcdstStep + 1] =
      pSrcDst[5*srcdstStep + 0] = (Fw8u) ((P_U + P_S + 2*P_T + 2) >> 2);
    pSrcDst[1*srcdstStep + 1] =
      pSrcDst[3*srcdstStep + 0] = (Fw8u) ((P_T + P_R + 2*P_S + 2) >> 2);
    pSrcDst[1*srcdstStep + 0] = (Fw8u) ((P_S + P_Q + 2*P_R + 2) >> 2);
    break;
    
  default:
    break;
  }
  return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntra8x8MB_H264_16s8u_C1R)(
			Fw16s **ppSrcDstCoeff, 
			Fw8u *pSrcDstYPlane, 
			Fw32s srcDstYStep, 
			fwintra8x8PredMode_H264 *pMBOIntraTypes, 
			Fw32u cbp8x8, 
			Fw32s QP, 
			Fw8u edgeType, 
			Fw16s *pQuantTable, 
			Fw8u bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	fwintra8x8PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;
	Fw32s Luma4x4_idx;

bypassFlag;
	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<4; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = pMBOIntraTypes[uBlock];	/// ------------ new
        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];
        top  = top_edge_subblock  && (edgeType & D_TOP_EDGE);
        left = left_edge_subblock && (edgeType & D_LEFT_EDGE);
		right = right_edge_subblock &&  (edgeType & D_RIGHT_EDGE);

		top_left = top & left;
		top_right = (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

//		res = iPredictIntra_8x8_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );
		res = iPredictIntra_8x8_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp8x8 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
	//		for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			Fw16s DCCoeff = pDecSrcCoeff[0];
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
	//		DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable); ///<<<<<<<<< changed to 8x8

			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable); ///<<<<<<<<< changed to 8x8

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
//			for (i=0;i<4;i++)
//			{
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
//				pSrcDstYPlane2+=srcDstYStep;	
//			}
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//printf("\n (%d) (%d) (%d) (%d)",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]  );
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}
#endif

#if 0
/***********************************************************************
 * \brief
 *    Inverse 8x8 transformation
 ************************************************************************/
void itrans8x8(struct img_par *img, //!< image parameters
              int ioff,            //!< index to 4x4 block
              int joff)            //!<
{
  int i,j;
  int m6[8][8];
  Boolean lossless_qpprime = ((img->qp + img->bitdepth_luma_qp_scale)==0 && img->lossless_qpprime_flag==1);

  int residue_transform_flag = img->residue_transform_flag;


  for( i=0; i<8 && !lossless_qpprime; i++)
  {
    int a[8], b[8];
    a[0] = img->m7[ioff + 0][joff + i] + img->m7[ioff + 4][joff + i];
    a[4] = img->m7[ioff + 0][joff + i] - img->m7[ioff + 4][joff + i];
    a[2] = (img->m7[ioff + 2][joff + i]>>1) - img->m7[ioff + 6][joff + i];
    a[6] = img->m7[ioff + 2][joff + i] + (img->m7[ioff + 6][joff + i]>>1);

    b[0] = a[0] + a[6];
    b[2] = a[4] + a[2];
    b[4] = a[4] - a[2];
    b[6] = a[0] - a[6];

    a[1] = -img->m7[ioff + 3][joff + i] + img->m7[ioff + 5][joff + i] - img->m7[ioff + 7][joff + i] - (img->m7[ioff + 7][joff + i]>>1);
    a[3] = img->m7[ioff + 1][joff + i] + img->m7[ioff + 7][joff + i] - img->m7[ioff + 3][joff + i] - (img->m7[ioff + 3][joff + i]>>1);
    a[5] = -img->m7[ioff + 1][joff + i] + img->m7[ioff + 7][joff + i] + img->m7[ioff + 5][joff + i] + (img->m7[ioff + 5][joff + i]>>1);
    a[7] = img->m7[ioff + 3][joff + i] + img->m7[ioff + 5][joff + i] + img->m7[ioff + 1][joff + i] + (img->m7[ioff + 1][joff + i]>>1);

    b[1] = a[1] + (a[7]>>2);
//    b[7] = -(a[1]>>2 + 0) + a[7];  KS: do we need to add zero?
    b[7] = -(a[1]>>2) + a[7];
    b[3] = a[3] + (a[5]>>2);
    b[5] = (a[3]>>2) - a[5];

    m6[0][i] = b[0] + b[7];
    m6[1][i] = b[2] + b[5];
    m6[2][i] = b[4] + b[3];
    m6[3][i] = b[6] + b[1];
    m6[4][i] = b[6] - b[1];
    m6[5][i] = b[4] - b[3];
    m6[6][i] = b[2] - b[5];
    m6[7][i] = b[0] - b[7];
  }
  for( i=0; i<8 && !lossless_qpprime; i++)
  {
    int a[8], b[8];
    a[0] = m6[i][0] + m6[i][4];
    a[4] = m6[i][0] - m6[i][4];
    a[2] = (m6[i][2]>>1) - m6[i][6];
    a[6] = m6[i][2] + (m6[i][6]>>1);

    b[0] = a[0] + a[6];
    b[2] = a[4] + a[2];
    b[4] = a[4] - a[2];
    b[6] = a[0] - a[6];

    a[1] = -m6[i][3] + m6[i][5] - m6[i][7] - (m6[i][7]>>1);
    a[3] = m6[i][1] + m6[i][7] - m6[i][3] - (m6[i][3]>>1);
    a[5] = -m6[i][1] + m6[i][7] + m6[i][5] + (m6[i][5]>>1);
    a[7] = m6[i][3] + m6[i][5] + m6[i][1] + (m6[i][1]>>1);

    b[1] = a[1] + (a[7]>>2);
    b[7] = -(a[1]>>2) + a[7];
    b[3] = a[3] + (a[5]>>2);
    b[5] = (a[3]>>2) - a[5];

    img->m7[ioff + i][joff + 0] = b[0] + b[7];
    img->m7[ioff + i][joff + 1] = b[2] + b[5];
    img->m7[ioff + i][joff + 2] = b[4] + b[3];
    img->m7[ioff + i][joff + 3] = b[6] + b[1];
    img->m7[ioff + i][joff + 4] = b[6] - b[1];
    img->m7[ioff + i][joff + 5] = b[4] - b[3];
    img->m7[ioff + i][joff + 6] = b[2] - b[5];
    img->m7[ioff + i][joff + 7] = b[0] - b[7];
  }
  for( i=0; i<8; i++)
  {
    for( j=0; j<8; j++)
    {
      // Residue Color Transform
      if(!residue_transform_flag)
      {
        if(lossless_qpprime)
          img->m7[i+ioff][j+joff] =min(img->max_imgpel_value,max(0,img->m7[ioff + i][joff + j]+(long)img->mpr[i+ioff][j+joff]));
        else
          img->m7[i+ioff][j+joff] =min(img->max_imgpel_value,max(0,(img->m7[ioff + i][joff + j]+((long)img->mpr[i+ioff][j+joff] << DQ_BITS_8)+DQ_ROUND_8)>>DQ_BITS_8));
      }
      else
      {
        if(lossless_qpprime)
          img->m7[i+ioff][j+joff] = img->m7[ioff + i][joff + j];
        else
          img->m7[i+ioff][j+joff] =(img->m7[ioff + i][joff + j]+DQ_ROUND_8)>>DQ_BITS_8;
      }
    }
  }
} 
#endif

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraHalf8x8MB_H264_16s8u_C1R)(
				Fw16s **ppSrcDstCoeff, 
				Fw8u  *pSrcDstYPlane, 
				Fw32s srcDstYStep,
				fwintra8x8PredMode_H264 *pMBOIntraTypes, 
				Fw32u cbp8x2, 
				Fw32s QP, 
				Fw8u  edgeType, 
				Fw16s *pQuantTable, 
				Fw8u  bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	fwintra8x8PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;
	Fw32s Luma4x4_idx;
bypassFlag;
	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<2; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = pMBOIntraTypes[uBlock];	
        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];
        top  = top_edge_subblock  && (edgeType & D_TOP_EDGE);
        left = left_edge_subblock && (edgeType & D_LEFT_EDGE);
		right = right_edge_subblock &&  (edgeType & D_RIGHT_EDGE);

		top_left = top & left;
		top_right = (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

//		res = iPredictIntra_8x8_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );
		res = iPredictIntra_8x8_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp8x2 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
		//	for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			Fw16s DCCoeff = pDecSrcCoeff[0];
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);//<<<<< need changed to 8x8

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
//			for (i=0;i<4;i++)
//			{
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
//				pSrcDstYPlane2+=srcDstYStep;	
//			}
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//printf("\n (%d) (%d) (%d) (%d)",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]  );
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntra16x16MB_H264_16s8u_C1R)(
									Fw16s **ppSrcCoeff,
									Fw8u *pSrcDstYPlane, 
									Fw32u srcDstYStep, 
									const FwIntra16x16PredMode_H264 intra_luma_mode, 
									const Fw32u cbp4x4, 
									const Fw32u QP, 
									const Fw8u edge_type)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra16x16PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	Fw16s *pbs16 = (Fw16s *) *ppSrcCoeff;
	Fw32s Luma4x4_idx;

	if( ppSrcCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = intra_luma_mode;	/// ------------ new

        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];
											// change edgetype to edge_type
        top			=	top_edge_subblock   && (edge_type & D_TOP_EDGE);
        left		=	left_edge_subblock  && (edge_type & D_LEFT_EDGE);
		right		=	right_edge_subblock && (edge_type & D_RIGHT_EDGE);
		top_left	=	(top & left);
		top_right	=	(top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

//		res = fwiPredictIntra_16x16_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );
		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;
		res = fwiPredictIntra_16x16_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
//			Fw16s DCCoeff = pDecSrcCoeff[0];
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 8, &DCCoeff, 1, QP);
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
//			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//printf("\n (%d) (%d) (%d) (%d)",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]  );
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	*ppSrcCoeff = pbs16;	// U point

	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntra_16x16MB_H264_16s8u_C1R)(
							Fw16s **ppSrcDstCoeff, 
							Fw8u *pSrcDstYPlane, 
							Fw32u srcDstYStep,
							FwIntra16x16PredMode_H264 intraLumaMode, 
							Fw32u cbp4x4, Fw32u QP, 
							Fw8u edgeType, 
							Fw16s *pQuantTable, 
							Fw8u bypassFlag)
{
	Fw16s DecSrcCoeff[16], *pDecSrcCoeff;
	Fw16s *pSrcCoeff;
	Fw8u  *pSrcDstYPlane2;

	FwIntra16x16PredMode_H264 intra_mode;
    Fw8u   left_edge_subblock, top_edge_subblock, right_edge_subblock;
    Fw32s	top, left, right, top_left, top_right;

	Fw32s     availability;
	FwStatus  res;

	// change ppSrcCoeff to ppSrcDstCoeff
	Fw16s *pbs16 = (Fw16s *) *ppSrcDstCoeff;
	Fw32s Luma4x4_idx;
bypassFlag;
	if( ppSrcDstCoeff == NULL || pSrcDstYPlane == NULL)
		return fwStsOutOfRangeErr;
	if( QP < 0 || QP > 51) return fwStsOutOfRangeErr;
	int uBlock, i;//, j;
	int uCBPMask = (1<<D_CBP_1ST_LUMA_AC_BITPOS);

	pDecSrcCoeff = &DecSrcCoeff[0];
	for	(uBlock=0; uBlock<16; uBlock++, uCBPMask <<= 1)
	{

		Luma4x4_idx = Scan_Luma4x4[uBlock];

        intra_mode = intraLumaMode;	/// ------------ new
        left_edge_subblock = left_edge_tab16[uBlock];
        top_edge_subblock = top_edge_tab16[uBlock];
		right_edge_subblock = right_edge_tab16[uBlock];
        top  = top_edge_subblock  && (edgeType & D_TOP_EDGE);
        left = left_edge_subblock && (edgeType & D_LEFT_EDGE);
		right = right_edge_subblock &&  (edgeType & D_RIGHT_EDGE);

		top_left = top & left;
		top_right = (top & right) | (above_right_avail_4x4[uBlock] == 0);

		availability = (!top) | ((!left)<<1) | ((!top_left)<<5) | ((!top_right)<<6);

		pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
										 (Luma4x4_idx/4)*4*srcDstYStep;

//		res = fwiPredictIntra_16x16_H264_8u_C1IR ( pSrcDstYPlane, srcDstYStep, intra_mode,availability );
		res = fwiPredictIntra_16x16_H264_8u_C1IR ( pSrcDstYPlane2, srcDstYStep, intra_mode,availability );

		pSrcCoeff = ppSrcDstCoeff[uBlock];
		if( cbp4x4 & uCBPMask )
		{
			//1.Performs scaling, integer inverse transformation, and shift for each 4x4 block 
			//	in the same order as is shown in Figure 6-6 of [JVTG050] in accordance 
			//  with 8.5.8 of [JVTG050] in the same way as DequantTransformResidual_H264 function.
	//		for(i=0;i<16;i++) *(pDecSrcCoeff+i) = *(pSrcCoeff+i);
			for(i=0;i<16;i++) pDecSrcCoeff[i] = *pbs16++ ;
			Fw16s DCCoeff = pDecSrcCoeff[0];
			DCCoeff = 0;
//			Fw16s DCCoeff = pDecSrcCoeff[0];
//			fwiDequantTransformResidual_H264_16s_C1I(pDecSrcCoeff, 4, &DCCoeff, 1, QP);
			DequantTransformResidual_idc_1(pDecSrcCoeff, QP, pQuantTable);

			// 2. Performs adding of a 16x16 inter prediction block and a 16x16 residual block in
			//	accordance with 8-247 of [JVTG050].
//			pSrcDstYPlane2 = pSrcDstYPlane + (uBlock%4)*4 + (uBlock/4)*4*srcDstYStep;
//			for (i=0;i<4;i++)
//			{
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
//				pSrcDstYPlane2+=srcDstYStep;	
//			}
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7;

			if( Dispatch_Type==DT_SSE2)
			{
				int *a;
  				xmm7  = _mm_set1_epi8(0); 
				for (i=0;i<4;i++)
				{	
					a    = (int *)pSrcDstYPlane2;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm1 = _mm_loadl_epi64 ( (__m128i*)(pDecSrcCoeff+i*4) );	// get 4 short
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
					pSrcDstYPlane2+=srcDstYStep;	
				}		
			}
			else
			{
				for (i=0;i<4;i++)
				{
	//				for (j=0;j<4;j++)
	//				{	
	//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
	//				}		
					pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4  ]+pSrcDstYPlane2[0]) ));
					pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+1]+pSrcDstYPlane2[1]) ));
					pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+2]+pSrcDstYPlane2[2]) ));
					pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pDecSrcCoeff[i*4+3]+pSrcDstYPlane2[3]) ));
					pSrcDstYPlane2+=srcDstYStep;	
				}
			}
		}
#if 0
		else
		{
//			for(i=0;i<16;i++) *(pDecSrcCoeff+i) = 0;
			pSrcDstYPlane2 = pSrcDstYPlane + (Luma4x4_idx%4)*4 + 
											 (Luma4x4_idx/4)*4*srcDstYStep;
			for (i=0;i<4;i++)
			{
//printf("\n (%d) (%d) (%d) (%d)",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]  );
//				for (j=0;j<4;j++)
//				{	
//					*(pSrcDstYPlane2+i) = (Fw8u)MAX(0,MIN(255,(*(pDecSrcCoeff++)+*(pSrcDstYPlane2+i))));
//				}		
				pSrcDstYPlane2[0] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[0]) ));
				pSrcDstYPlane2[1] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[1]) ));
				pSrcDstYPlane2[2] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[2]) ));
				pSrcDstYPlane2[3] = (Fw8u)MAX(0,MIN(255,(pSrcDstYPlane2[3]) ));
//printf("\n %d %d %d %d",pSrcDstYPlane2[0],pSrcDstYPlane2[1],pSrcDstYPlane2[2],pSrcDstYPlane2[3]);

				pSrcDstYPlane2+=srcDstYStep;	
			}		
		}
#endif
	}

	// change ppSrcCoeff to ppSrcDstCoeff
	*ppSrcDstCoeff = pbs16;	// U point

	return fwStsNoErr;
}
#endif

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
