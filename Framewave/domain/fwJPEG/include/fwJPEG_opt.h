/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef	__FWJPEG_OPT_H__
#define	__FWJPEG_OPT_H__

#include "buildnum.h"
#include "fwdev.h"
#include "fwJPEG.h"

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGBToY_JPEG_8u_P3C1R)(const Fw8u* pSrcRGB[3], int srcStep, 
							Fw8u* pDstY, int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGBToY_JPEG_8u_C3C1R)(const Fw8u* pSrcRGB, int srcStep,
							Fw8u* pDstY, int dstStep, FwiSize roiSize);


OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiBGRToY_JPEG_8u_C3C1R)(const Fw8u* pSrcBGR, int srcStep,
							Fw8u* pDstY, int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_JPEG_8u_P3R)(const Fw8u* pSrcRGB[3], int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_JPEG_8u_C3P3R)(const Fw8u* pSrcRGB, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_JPEG_8u_P3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw8u* pDstRGB[3], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_JPEG_8u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw8u* pDstRGB, int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGB565ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u* pSrcRGB, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRGB555ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u* pSrcRGB, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB565_JPEG_8u16u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw16u* pDstRGB, int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB555_JPEG_8u16u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw16u* pDstRGB, int dstStep, FwiSize roiSize);


OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr_JPEG_8u_C3P3R)(const Fw8u* pSrcBGR, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR_JPEG_8u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw8u* pDstBGR, int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiBGR565ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiBGR555ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw8u* pDstYCbCr[3], int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR565_JPEG_8u16u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw16u* pDstBGR, int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR555_JPEG_8u16u_P3C3R)(const Fw8u* pSrcYCbCr[3], int srcStep, 
							Fw16u* pDstBGR, int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiCMYKToYCCK_JPEG_8u_P4R)(const Fw8u* pSrcCMYK[4], int srcStep, 
							Fw8u* pDstYCCK[4], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiCMYKToYCCK_JPEG_8u_C4P4R)(const Fw8u* pSrcCMYK, int srcStep, 
							Fw8u* pDstYCCK[4], int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiYCCKToCMYK_JPEG_8u_P4R)(const Fw8u* pSrcYCCK[4], int srcStep, 
							Fw8u* pDstCMYK[4], int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiYCCKToCMYK_JPEG_8u_P4C4R)(const Fw8u* pSrcYCCK[4], int srcStep, 
							Fw8u* pDstCMYK, int dstStep, FwiSize roiSize);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiRGBToYCbCr444LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcRGB, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcRGB, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcRGB, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R)(const Fw8u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R)(const Fw16u* pSrcBGR, int srcStep, 
							Fw16s* pDstMCU[3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK444LS_MCU_8u16s_C4P4R)(const Fw8u* pSrcCMYK,
							int srcStep, Fw16s* pDstMCU[4]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R)(const Fw8u* pSrcCMYK,
							int srcStep, Fw16s* pDstMCU[4]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R)(const Fw8u* pSrcCMYK,
							int srcStep, Fw16s* pDstMCU[4]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToRGBLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstRGB, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGBLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstRGB, int dstStep);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstRGB, int dstStep);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw16u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw16u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw16u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw16u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw16u* pDstBGR, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R)(const Fw16s* pSrcMCU[3], Fw16u* pDstBGR, int dstStep);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCCK444ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s* pSrcMCU[4],
							Fw8u* pDstCMYK, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCCK422ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s* pSrcMCU[4],
							Fw8u* pDstCMYK, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiYCCK411ToCMYKLS_MCU_16s8u_P4C4R)(const Fw16s* pSrcMCU[4],
							Fw8u* pDstCMYK, int dstStep);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantFwdRawTableInit_JPEG_8u)(Fw8u* pQuantRawTable, int qualityFactor);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantFwdTableInit_JPEG_8u16u)(const Fw8u* pQuantRawTable, 
							Fw16u* pQuantFwdTable);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantFwd8x8_JPEG_16s_C1I)(Fw16s* pSrcDst, const Fw16u* pQuantFwdTable);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInvTableInit_JPEG_8u16u)(const Fw8u* pQuantRawTable, 
							Fw16u* pQuantInvTable);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInv8x8_JPEG_16s_C1I)(Fw16s* pSrcDst, const Fw16u*
							pQuantInvTable);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8_JPEG_16s_C1)( const Fw16s* pSrc,
							Fw16s* pDst, const Fw16u* pQuantFwdTable );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8_JPEG_16s_C1I)(Fw16s* pDst, 
							const Fw16u* pQuantFwdTable );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantFwd8x8LS_JPEG_8u16s_C1R)(const Fw8u* pSrc, 
							int srcStep, Fw16s* pDst, const Fw16u* pQuantFwdTable );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8_JPEG_16s_C1)(const Fw16s* pSrc, Fw16s* pDst, 
							const Fw16u* pQuantInvTable );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8_JPEG_16s_C1I)(Fw16s* pDst,  
							const Fw16u* pQuantInvTable );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCTQuantInv8x8LS_JPEG_16s8u_C1R)(const Fw16s* pSrc, 
							Fw8u* pDst, int dstStep,  const Fw16u* pQuantInvTable );

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiSub128_JPEG_8u16s_C1R)(const Fw8u* pSrc, int srcStep, 
							Fw16s* pDst, int dstStep, FwiSize roiSize);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX,  fwiAdd128_JPEG_16s8u_C1R)(const Fw16s* pSrc, int srcStep,
							Fw8u* pDst, int dstStep, FwiSize roiSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDownH2V1_JPEG_8u_C1R)(const Fw8u* pSrc, int srcStep,
							FwiSize srcSize, Fw8u* pDst, int dstStep, FwiSize dstSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDownH2V2_JPEG_8u_C1R)(const Fw8u* pSrc, int srcStep,
							FwiSize srcSize, Fw8u* pDst, int dstStep, FwiSize dstSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDownRowH2V1_Box_JPEG_8u_C1)(const Fw8u* pSrc,int srcWidth, 
							Fw8u* pDst);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDownRowH2V2_Box_JPEG_8u_C1)(const Fw8u* pSrc1, const Fw8u* pSrc2,
							int srcWidth, Fw8u* pDst);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUpH2V1_JPEG_8u_C1R)(const Fw8u* pSrc, int srcStep,
							FwiSize srcSize, Fw8u* pDst, int dstStep, FwiSize dstSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUpH2V2_JPEG_8u_C1R)(const Fw8u* pSrc, int srcStep,
							FwiSize srcSize, Fw8u* pDst, int dstStep, FwiSize dstSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUpRowH2V1_Triangle_JPEG_8u_C1)(const Fw8u* pSrc, 
							int srcWidth, Fw8u* pDst);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUpRowH2V2_Triangle_JPEG_8u_C1)(const Fw8u* pSrc1, 
							const Fw8u* pSrc2, int srcWidth, Fw8u* pDst);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDown444LS_MCU_8u16s_C3P3R)(const Fw8u* pSrc, 
							int srcStep, Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDown422LS_MCU_8u16s_C3P3R)(const Fw8u* pSrc, 
							int srcStep, Fw16s* pDstMCU[3]);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleDown411LS_MCU_8u16s_C3P3R)(const Fw8u* pSrc, 
							int srcStep, Fw16s* pDstMCU[3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUp444LS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDst, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUp422LS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDst, int dstStep);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSampleUp411LS_MCU_16s8u_P3C3R)(const Fw16s* pSrcMCU[3],
							Fw8u* pDst, int dstStep);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSplit422LS_MCU_8u16s_C2P3R)(const Fw8u* pSrc, int srcStep,
							Fw16s* pDstMCU[3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiJoin422LS_MCU_16s8u_P3C2R)(const Fw16s* pSrcMCU[3], 
							Fw8u*pDst, int dstStep);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanRawTableInit_JPEG_8u)(const int pStatistics[256],
							Fw8u* pListBits, Fw8u* pListVals);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanSpecGetBufSize_JPEG_8u)(int* pSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanSpecInit_JPEG_8u)(const Fw8u* pListBits,
							const Fw8u* pListVals,  FwiEncodeHuffmanSpec* pEncHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanSpecInitAlloc_JPEG_8u)(const Fw8u* pListBits, 
							const Fw8u* pListVals, FwiEncodeHuffmanSpec** pEncHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanSpecFree_JPEG_8u)(FwiEncodeHuffmanSpec* pEncHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanStateGetBufSize_JPEG_8u)(int* pSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanStateInit_JPEG_8u)(FwiEncodeHuffmanState* pEncHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanStateInitAlloc_JPEG_8u)(FwiEncodeHuffmanState** pEncHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffmanStateFree_JPEG_8u)(FwiEncodeHuffmanState* pEncHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_JPEG_16s1u_C1)(const Fw16s* pSrc, Fw8u* pDst, 
							int dstLenBytes, int* pDstCurrPos, Fw16s* pLastDC,
							const FwiEncodeHuffmanSpec* pDcTable, 
							const FwiEncodeHuffmanSpec* pAcTable, 
							FwiEncodeHuffmanState* pEncHuffState, int bFlushState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_Direct_JPEG_16s1u_C1)(const Fw16s* pSrc,
							Fw8u* pDst, int* pDstBitsLen, Fw16s* pLastDC,
							const FwiEncodeHuffmanSpec* pDcTable,
							const FwiEncodeHuffmanSpec* pAcTable);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1)(const Fw16s* pSrc, 
							Fw8u *pDst, int dstLenBytes, int* pDstCurrPos, 
							Fw16s* pLastDC, int Al, const FwiEncodeHuffmanSpec* pDcTable, 
							FwiEncodeHuffmanState* pEncHuffState, int bFlushState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1)(const Fw16s* pSrc,
							Fw8u* pDst, int dstLenBytes, int* pDstCurrPos, int Al,
							FwiEncodeHuffmanState* pEncHuffState, int bFlushState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1)(const Fw16s* pSrc, 
							Fw8u* pDst, int dstLenBytes, int* pDstCurrPos, int Ss, 
							int Se, int Al, const FwiEncodeHuffmanSpec* pAcTable, 
							FwiEncodeHuffmanState* pEncHuffState, int bFlushState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1)(const Fw16s* pSrc,
							Fw8u* pDst, int dstLenBytes, int* pDstCurrPos, int Ss, 
							int Se, int Al, const FwiEncodeHuffmanSpec* pAcTable, 
							FwiEncodeHuffmanState* pEncHuffState, int bFlushState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetHuffmanStatistics8x8_JPEG_16s_C1)(const Fw16s* pSrc,
							int pDcStatistics[256], int pAcStatistics[256], 
							Fw16s* pLastDC);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetHuffmanStatistics8x8_DCFirst_JPEG_16s_C1)(const Fw16s* pSrc,
							int pDcStatistics[256], Fw16s* pLastDC, int Al);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanSpecGetBufSize_JPEG_8u)(int* pSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanSpecInit_JPEG_8u)(const Fw8u* pListBits,
							const Fw8u* pListVals, FwiDecodeHuffmanSpec* pDecHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanSpecInitAlloc_JPEG_8u)(const Fw8u* pListBits, 
							const Fw8u* pListVals, FwiDecodeHuffmanSpec** pDecHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanSpecFree_JPEG_8u)(FwiDecodeHuffmanSpec* pDecHuffSpec);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanStateGetBufSize_JPEG_8u)(int* pSize);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanStateInit_JPEG_8u)(FwiDecodeHuffmanState* pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanStateInitAlloc_JPEG_8u)(FwiDecodeHuffmanState** pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanStateFree_JPEG_8u)(FwiDecodeHuffmanState* pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_JPEG_1u16s_C1)(const Fw8u *pSrc, 
							int	srcLenBytes, int *pSrcCurrPos, Fw16s *pDst, 
							Fw16s *pLastDC, int *pMarker, 
							const FwiDecodeHuffmanSpec *pDcTable, 
							const FwiDecodeHuffmanSpec *pAcTable, 
							FwiDecodeHuffmanState *pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_Direct_JPEG_1u16s_C1)(const Fw8u* pSrc,
							int* pSrcBitsLen, Fw16s* pDst, Fw16s* pLastDC, 
							int* pMarker, Fw32u* pPrefetchedBits, 
							int* pNumValidPrefetchedBits, 
							const FwiDecodeHuffmanSpec* pDcTable, 
							const FwiDecodeHuffmanSpec* pAcTable);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1)(const Fw8u* pSrc, 
							int srcLenBytes, int* pSrcCurrPos, Fw16s* pDst, 
							Fw16s* pLastDC, int* pMarker, int Al, 
							const FwiDecodeHuffmanSpec* pDcTable,
							FwiDecodeHuffmanState* pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1)(const Fw8u* pSrc,
							int srcLenBytes, int* pSrcCurrPos, Fw16s* pDst, 
							int* pMarker, int Al,
							FwiDecodeHuffmanState* pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1)(const Fw8u* pSrc, 
							int srcLenBytes, int* pSrcCurrPos, Fw16s* pDst, 
							int* pMarker, int Ss, int Se, int Al, 
							const FwiDecodeHuffmanSpec* pAcTable,
							FwiDecodeHuffmanState* pDecHuffState);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1)(const Fw8u* pSrc,
							int srcLenBytes, int* pSrcCurrPos, Fw16s* pDst, 
							int* pMarker, int Ss, int Se, int Al, 
							const FwiDecodeHuffmanSpec* pAcTable,
							FwiDecodeHuffmanState* pDecHuffState);

#endif	// __FWJPEG_OPT_H__
