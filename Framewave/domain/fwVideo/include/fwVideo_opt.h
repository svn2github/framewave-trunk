/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef	__FWVIDEO_OPT_H__
#define	__FWVIDEO_OPT_H__

#include "buildnum.h"
#include "fwdev.h"
#include "fwVideo.h"

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeCAVLCCoeffs_H264_1u16s)( Fw32u **ppBitStream, Fw32s *pBitOffset, Fw16s *pNumCoeff, Fw16s **ppDstCoeffs, Fw32u uVLCSelect, Fw16s uMaxNumCoeff, const Fw32s **ppTblCoeffToken,	const Fw32s **ppTblTotalZeros, const Fw32s **ppTblRunBefore, 	const Fw32s  *pScanMatrix);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeCAVLCChromaDcCoeffs_H264_1u16s)(	Fw32u **ppBitStream, Fw32s *pBitOffset, Fw16s *pNumCoeff, Fw16s **ppDstCoeffs, const Fw32s *pTblCoeffToken, const Fw32s **ppTblTotalZerosCR, const Fw32s **ppTblRunBefore);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeExpGolombOne_H264_1u16s)(Fw32u **ppBitStream, Fw32s *pBitOffset, Fw16s *pDst, Fw8u isSigned);


OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_VerEdge_H264_8u_C1IR)( Fw8u *pSrcDst, Fw32s  srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS);
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_HorEdge_H264_8u_C1IR)( Fw8u* pSrcDst, Fw32s srcDstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, Fw8u* pBS);
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_HorEdge_H264_8u_C1IR)( Fw8u* pSrcDst, Fw32s srcDstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, Fw8u* pBS);
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_VerEdge_H264_8u_C1IR)( Fw8u* pSrcDst, Fw32s srcDstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, Fw8u* pBS);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExpandPlane_H264_8u_C1R)(	Fw8u *StartPtr, 
	Fw32u uFrameWidth,	Fw32u uFrameHeight, Fw32u uPitch, 
	Fw32u uPels, FwvcFrameFieldFlag uFrameFieldFlag);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateLuma_H264_8u_C1R)(
	const Fw8u* pSrc, Fw32s srcStep, Fw8u* pDst, Fw32s dstStep, 
	Fw32s dx,   Fw32s dy, FwiSize roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaTop_H264_8u_C1R)(const Fw8u* pSrc, Fw32s srcStep,
	Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s outPixels, FwiSize roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
	srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s
			outPixels, FwiSize roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateChroma_H264_8u_C1R)(
	const Fw8u*	pSrc,	Fw32s	srcStep,  Fw8u*	pDst,	Fw32s	dstStep, 
	Fw32s	dx,		Fw32s	dy,  FwiSize	roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateChromaTop_H264_8u_C1R)(const Fw8u* pSrc, 
	Fw32s srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, 
	Fw32s outPixels, FwiSize roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateChromaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
			srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s outPixels, FwiSize roiSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiInterpolateBlock_H264_8u_P2P1R)(Fw8u *pSrc1, 
	 Fw8u *pSrc2, Fw8u *pDst,  Fw32u width, Fw32u height, Fw32u pitch);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWeightedAverage_H264_8u_C1IR)(const Fw8u* pSrc1, 
	Fw8u* pSrc2Dst, Fw32s srcDstStep, Fw32s weight1, Fw32s weight2, 
    Fw32s shift,Fw32s offset, FwiSize roiSize);


OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiPredictIntra_4x4_H264_8u_C1IR)(Fw8u* pSrcDst, 
		Fw32s srcDstStep,	FwIntra4x4PredMode_H264 predMode, 	Fw32s availability);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiPredictIntra_16x16_H264_8u_C1IR)(Fw8u* pSrcDst, 
	  Fw32s srcDstStep, FwIntra16x16PredMode_H264 predMode, Fw32s availability);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiPredictIntraChroma8x8_H264_8u_C1IR)(Fw8u* pSrcDst, 
	 Fw32s srcDstStep, FwIntraChromaPredMode_H264 predMode, Fw32s availability);


OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaInterMB_H264_16s8u_P2R)(
	Fw16s **ppSrcCoeff, Fw8u *pSrcDstUPlane, Fw8u *pSrcDstVPlane, 
	const Fw32u srcDstStep, const Fw32u cbp4x4, const Fw32s ChromaQP);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraHalvesMB_H264_16s8u_P2R)(
	Fw16s **ppSrcCoeff, Fw8u *pSrcDstUPlane, Fw8u *pSrcDstVPlane, Fw32u srcDstUVStep, 
	FwIntraChromaPredMode_H264 intraChromaMode, Fw32u cbp4x4,	Fw32u ChromaQP, Fw8u edgeTypeTop, Fw8u edgeTypeBottom);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructChromaIntraMB_H264_16s8u_P2R)(
	Fw16s **ppSrcCoeff, Fw8u *pSrcDstUPlane, Fw8u *pSrcDstVPlane, 
	const Fw32u srcDstUVStep, const FwIntraChromaPredMode_H264 intraChromaMode, const Fw32u cbp4x4, const Fw32u ChromaQP, const Fw8u edgeType);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaInterMB_H264_16s8u_C1R)(
	Fw16s **ppSrcCoeff, Fw8u  *pSrcDstYPlane, const Fw32u srcDstYStep, const Fw32u cbp4x4, const Fw32s QP);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraHalfMB_H264_16s8u_C1R)(
	Fw16s **ppSrcCoeff, Fw8u *pSrcDstYPlane, Fw32s srcDstYStep, FwIntra4x4PredMode_H264 *pMBIntraTypes, 
	Fw32u cbp4x2, Fw32u QP, Fw8u edgeType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructLumaIntraMB_H264_16s8u_C1R)(
	Fw16s **ppSrcCoeff, Fw8u  *pSrcDstYPlane, Fw32s srcDstYStep, 
	const FwIntra4x4PredMode_H264 *pMBIntraTypes, const Fw32u cbp4x4, const Fw32u QP, const Fw8u  edgeType);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiTransformDequantLumaDC_H264_16s_C1I)(
	Fw16s* pSrcDst, Fw32s QP);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_H264_16s_C1I)(Fw16s* pSrcDst, Fw32s QP);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_H264_16s_C1I)(Fw16s* pSrcDst, 
	Fw32s  step, Fw16s  *pDC, Fw32s  AC, Fw32s  QP);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidualAndAdd_H264_16s_C1I)(const  Fw8u  *pPred,
	 Fw16s *pSrcDst, Fw16s *pDC, Fw8u  *pDst, Fw32s  PredStep, Fw32s  DstStep, Fw32s  QP,  Fw32s  AC);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiTransformPrediction_H264_8u16s_C1)(Fw8u *pSrc, Fw32s step, Fw16s *pDst);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDequantTransformResidual_SISP_H264_16s_C1I)(	Fw16s* pSrcDst,
	Fw16s* pPredictBlock, Fw16s* pDC, Fw32s  AC, Fw32s  qp, Fw32s  qs, Fw32s Switch);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiTransformDequantChromaDC_SISP_H264_16s_C1I)(Fw16s* pSrcDst,
	Fw16s* pDCPredict, Fw32s qp, Fw32s qs, Fw32s Switch);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanTableInitAlloc_32s)(const Fw32s* pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanRunLevelTableInitAlloc_32s)(const Fw32s* pSrcTable, FwVCHuffmanSpec_32s** ppDstSpec);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanOne_1u32s)(Fw32u** ppBitStream, Fw32s* pOffset, Fw32s* pDst, const FwVCHuffmanSpec_32s *pDecodeTable);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDecodeHuffmanPair_1u16s)(Fw32u** ppBitStream, Fw32s* pOffset, const FwVCHuffmanSpec_32s *pDecodeTable, Fw8s *pFirst, Fw16s *pSecond);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiHuffmanTableFree_32s)(FwVCHuffmanSpec_32s** ppDecodeTable);


OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x16_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x8_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x16_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x8_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x4_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x8_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x4_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC2x4_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x2_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC2x2_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x4_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const Fw16s
*pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x8UV_8u_C1 )( const Fw8u *pSrcRef, Fw32s srcStep, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s mcType, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x16B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x8B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x16B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x8B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC8x4B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x8B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x4B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC2x4B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC4x2B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC2x2B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x4B_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMC16x8BUV_8u_C1 )( const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s
mcTypeF, const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, const
Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst, Fw32s dstStep, Fw32s roundControl );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x16_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep, const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor, Fw32s dstPredictorStep, Fw32s mcType, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x8_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep,const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor,Fw32s dstPredictorStep,  Fw32s mcType, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x8_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep,const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor,Fw32s dstPredictorStep, Fw32s mcType,  Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x16_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep, const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor,Fw32s dstPredictorStep, Fw32s mcType,	Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x4_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep, const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor,Fw32s dstPredictorStep, Fw32s mcType,	Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff4x4_8u16s_C1)(const Fw8u* pSrcCur, Fw32s srcCurStep, const Fw8u* pSrcRef, Fw32s srcRefStep, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw16s* pDstPredictor,Fw32s dstPredictorStep, Fw32s mcType,	Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x16B_8u16s_C1)(const Fw8u* pSrcCur,  Fw32s srcCurStep,const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x8B_8u16s_C1)(const Fw8u* pSrcCur,  Fw32s srcCurStep, const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x8B_8u16s_C1)(const Fw8u* pSrcCur,  Fw32s srcCurStep, const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x16B_8u16s_C1)(const Fw8u* pSrcCur,  Fw32s srcCurStep, const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x4B_8u16s_C1)(const Fw8u* pSrcCur,  Fw32s srcCurStep, const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, Fw16s* pDstDiff, Fw32s dstDiffStep, Fw32s roundControl);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrDiff16x16_8u32s)(const Fw8u* pSrc, Fw32s srcStep, const Fw8u* pRef, Fw32s refStep, Fw32s mcType, Fw32s* pSqrDiff);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrDiff16x16B_8u32s)(const Fw8u* pSrc,  Fw32s srcStep, const Fw8u* pRefF, Fw32s refStepF, Fw32s mcTypeF, const Fw8u* pRefB, Fw32s refStepB, Fw32s mcTypeB, Fw32s* pSqrDiff);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiVarMean8x8_8u32s_C1R)(const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar, Fw32s* pMean);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiVarMean8x8_16s32s_C1R)(const Fw16s* pSrc, Fw32s srcStep, Fw32s* pVar, Fw32s* pMean);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiVarMeanDiff16x16_8u32s_C1R)(const Fw8u* pSrc, Fw32s srcStep, const Fw8u* pRef, Fw32s refStep, Fw32s* pSrcSum, Fw32s* pVar, Fw32s* pMean, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiVarMeanDiff16x8_8u32s_C1R)(const Fw8u* pSrc, Fw32s srcStep, const Fw8u* pRef, Fw32s refStep, Fw32s* pSrcSum, Fw32s* pVar,  Fw32s* pMean, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiVariance16x16_8u32s)(const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiEdgesDetect16x16_8u_C1R)(const Fw8u *pSrc, Fw32u srcStep, Fw8u EdgePelDifference, Fw8u EdgePelCount, Fw8u *pRes);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSAD16x16_8u32s)(const Fw8u* pSrc, Fw32s srcStep, const Fw8u* pRef, Fw32s refStep, Fw32s* pSAD, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSAD8x8_8u32s_C1R)(const Fw8u* pSrcCur, int srcCurStep, const Fw8u* pSrcRef, Fw32s srcRefStep, Fw32s * pDst, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSAD4x4_8u32s)(const Fw8u *pSrc, Fw32s srcStep, Fw8u *pRef, Fw32s refStep, Fw32s *pSAD, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSAD16x16Blocks8x8_8u16u)(const Fw8u  *pSrc, Fw32s srcStep, Fw8u  *pRef, Fw32s refStep, Fw16u *pDstSAD, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSAD16x16Blocks4x4_8u16u)(const Fw8u *pSrc, Fw32s srcStep, Fw8u *pRef, Fw32s refStep, Fw16u *pDstSAD, Fw32s mcType);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumsDiff16x16Blocks4x4_8u16s_C1)(Fw8u * pSrc,  Fw32s srcStep,Fw8u * pPred, Fw32s predStep, Fw16s* pSums, Fw16s* pDiff);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumsDiff8x8Blocks4x4_8u16s_C1)(Fw8u* pSrc, Fw32s srcStep,Fw8u* pPred, Fw32s predStep, Fw16s* pSums, Fw16s* pDiff);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlock_MPEG1_32s)(Fw32u **ppBitStream, int *pOffset, const Fw32s *pDCSizeTable, const Fw32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDstBlock, Fw32s *pDstSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlockIntra_MPEG1_32s)(Fw32u **ppBitStream, int *pOffset, const Fw32s *pDCSizeTable, const Fw32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDCPred, Fw16s *pDstBlock, Fw32s *pDstSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlock_MPEG2_32s)(Fw32u **ppBitStream, int *pOffset, const FwVCHuffmanSpec_32s *pDCTable, const FwVCHuffmanSpec_32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDstBlock, Fw32s *pDstSize);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlockIntra_MPEG2_32s)(Fw32u **ppBitStream, int *pOffset, const FwVCHuffmanSpec_32s *pDCSizeTable, const FwVCHuffmanSpec_32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDCPred, Fw32s shiftDCVal, Fw16s *pDstBlock, Fw32s *pDstSize);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInvIntra_MPEG2_16s_C1I)(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiQuantInv_MPEG2_16s_C1I)(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s_C1R)(const Fw16s *pSrc, Fw16s *pDst, Fw32s dstStep, Fw32s count );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s8u_C1R)(const Fw16s *pSrc, Fw8u  *pDst, Fw32s dstStep, Fw32s count );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s_P2C2R)(const Fw16s *pSrcU, const Fw16s *pSrcV, Fw16s *pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_AANTransposed_16s8u_P2C2R)(const Fw16s *pSrcU, const Fw16s *pSrcV, Fw8u  *pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV );

#endif	// __FWVIDEO_OPT_H__
