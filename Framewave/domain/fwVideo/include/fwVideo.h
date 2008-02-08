/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FWVIDEO_H__
#define __FWVIDEO_H__

#include "fwBase.h"

//
// Enumerations
//
// H264-IntraPredict
enum {
    FW_UPPER        = 1  ,
    FW_LEFT         = 2  ,
    FW_CENTER       = 4  ,
    FW_RIGHT        = 8  ,
    FW_LOWER        = 16 ,
    FW_UPPER_LEFT   = 32 ,
    FW_UPPER_RIGHT  = 64 ,
    FW_LOWER_LEFT   = 128,
    FW_LOWER_RIGHT  = 256
};

// H264-MB Recon
typedef enum {
               FW_CHROMA_DC    = 0,
               FW_CHROMA_HOR   = 1,
               FW_CHROMA_VERT  = 2,
               FW_CHROMA_PLANE = 3
             } FwIntraChromaPredMode_H264;

typedef enum {
               FW_16X16_VERT   = 0,
               FW_16X16_HOR    = 1,
               FW_16X16_DC     = 2,
               FW_16X16_PLANE  = 3
             } FwIntra16x16PredMode_H264;

typedef enum {
               FW_4x4_VERT     = 0,
               FW_4x4_HOR      = 1,
               FW_4x4_DC       = 2,
               FW_4x4_DIAG_DL  = 3,
               FW_4x4_DIAG_DR  = 4,
               FW_4x4_VR       = 5,
               FW_4x4_HD       = 6,
               FW_4x4_VL       = 7,
               FW_4x4_HU       = 8
             } FwIntra4x4PredMode_H264;

#define        D_LEFT_EDGE      0x1
#define        D_RIGHT_EDGE     0x2
#define        D_TOP_EDGE       0x4
#define        D_BOTTOM_EDGE    0x8
#define        D_TOP_LEFT_EDGE  0x10
#define        D_TOP_RIGHT_EDGE 0x20

// H264-InterPredict
typedef enum _FWVC_FRAME_FIELD_FLAG
{
             FWVC_FRAME         = 0x0,
             FWVC_TOP_FIELD     = 0X1,
             FWVC_BOTTOM_FIELD  = 0x2
} FWVC_FRAME_FIELD_FLAG;

#define FwvcFrameFieldFlag FWVC_FRAME_FIELD_FLAG

//MPEG-2 Decoder
typedef Fw32s FwVCHuffmanSpec_32s;

#ifdef __cplusplus
extern "C" {
#endif


/*#HintBlock
AutoOff
*/

/*#IncludeBlock
#includeGlobal "../../../doc/fwIntroDoc.h"
#include "../../../doc/fwVideoIntroDoc.h"
*/

#if BUILD_NUM_AT_LEAST( 101 )

/*#Documentation

<Chapter>
<ChapterHeading>Library Version</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes the FW function that provides library version information.</ChaptDesc></Paragraph> 
*/

/*#FunctionBlock - GetLibVersion
TODO:Publish
#Technologies - REF
#Short - Get library version
#Long - <Text>This function returns a pointer to the <Bold>FwLibraryVersion</Bold> structure that contains FW Library version information.</Text>
*/

const FwLibraryVersion* STDCALL fwvGetLibVersion ();

/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Video Coding Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that encode and decode video data according to the MPEG-1 (ISO11172), MPEG-2 (ISO13818), MPEG-4 (ISO14496A), DV (IEC61834), H.263 (ITUH263) and H.264 (JVTG050) standards.</ChaptDesc></Paragraph>
*/

// ------------------------------------------------
//  H.264 decoder
// ------------------------------------------------

// ------------------------------------------------
//  H.264 CAVLC
// ------------------------------------------------
/*#FunctionBlock - DecodeCAVLCCoeffs_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Decode CAVLC bitstream
#Long - <Text>This function parses and decodes a source stream of MPEG-4/AVC Chroma AC and Luma video syntax elements in JVTG050-compliant context-adaptive variable-length coding (CAVLC) format.
Result coefficients are written to a series of 4X4 destination blocks.</Text>

<Text>Each element of pointer array <ParameterName>pTblCoeffToken</ParameterName> points to a table that contains codes, the number of trailing one transform coefficients and the total number
of non-zero transform coefficients, in accordance with JVTG050 Table 9-5. The following values are used.</Text>

<Text>Element <ParameterName>pTblCoeffToken[0]</ParameterName>, 0 < <ParameterName>uVLCSelect</ParameterName> < 2</Text>
<Text>Element <ParameterName>pTblCoeffToken[1]</ParameterName>, 2 < <ParameterName>uVLCSelect</ParameterName> < 4</Text>
<Text>Element <ParameterName>pTblCoeffToken[2]</ParameterName>, 4 < <ParameterName>uVLCSelect</ParameterName> < 8</Text>
<Text>Element <ParameterName>pTblCoeffToken[3]</ParameterName>, <ParameterName>uVLCSelect</ParameterName> = -1 (used only for <FunctionName>DecodeCAVLCChromaDcCoeffs_H264</FunctionName>).</Text>

<Text>The <FunctionName>HuffmanRunLevelTableInitAlloc</FunctionName> function must be used to create <ParameterName>pTblCoeffToken</ParameterName> tables.
When the value of uVLCSelect is greater than 8, the function uses its own table of transform coefficients.</Text>

<Text>Each element of pointer array <ParameterName>ppTblTotalZeros</ParameterName> except for <ParameterName>ppTblTotalZeros[0]</ParameterName> points to a table that contains codes and values (JVTG050 <Bold>total_zeros</Bold>) in
accordance with JVTG050 Tables 9-7 and 9-8. Element <ParameterName>ppTblTotalZeros[0]</ParameterName> is not used.</Text>
<Text>Each element <ParameterName>ppTblTotalZeros[i]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>TotalCoeff</Bold> = i, 0 < i < 16.</Text>

<Text>Each element of pointer array <ParameterName>ppTblRunBefore</ParameterName> except for <ParameterName>ppTblRunBefore[0]</ParameterName> points to
a table that contains codes and values (JVTG050 <Bold>run_before</Bold>) in
accordance with JVTG050 Table 9-10. Element <ParameterName>ppTblRunBefore[0]</ParameterName> is not used.</Text>
<Text><ParameterName>ppTblRunBefore[i]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>zerosLeft</Bold> = i, 0 < i < 7.</Text>
<Text><ParameterName>ppTblRunBefore[7]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>zerosLeft</Bold> > 6.</Text>
<Text>The <FunctionName>HuffmanTableInitAlloc</FunctionName> function must be used to create <ParameterName>ppTblTotalZeros</ParameterName> and <ParameterName>ppTblRunBefore</ParameterName> tables.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeCAVLCCoeffs_H264_1u16s                (       Fw32u **ppBitStream, Fw32s *pBitOffset, Fw16s *pNumCoeff  ,
                                                                          Fw16s **ppDstCoeffs, Fw32u  uVLCSelect, Fw16s uMaxNumCoeff,
                                                                    const Fw32s **ppTblCoeffToken,
                                                                    const Fw32s **ppTblTotalZeros,
                                                                    const Fw32s **ppTblRunBefore ,
                                                                    const Fw32s  *pScanMatrix );

/*#FunctionBlock - DecodeCAVLCChromaDcCoeffs_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Decode Chroma CAVLC bitstream
#Long - <Text>This function parses and decodes a source stream of MPEG-4/AVC Chroma DC video syntax elements in CAVLC format.
Result coefficients are written to a series of 2X2 destination blocks.</Text>
Parameter <ParameterName>pTblCoeffToken</ParameterName> is  = -1, equivalent to element <ParameterName>pTblCoeffToken[3]</ParameterName> of the <FunctionName>DecodeCAVLCCoeffs_H264</FunctionName> function.
<Text>Parameter <ParameterName>pTblCoeffToken</ParameterName> points to a table that contains codes, the number of trailing one transform coefficients and the total number
of non-zero transform coefficients, in accordance with JVTG050 Table 9-5.</Text>

<Text>Each element of pointer array <ParameterName>ppTblRunBefore</ParameterName> except for <ParameterName>ppTblRunBefore[0]</ParameterName> points to
a table that contains codes and values (JVTG050 <Bold>run_before</Bold>) in accordance with JVTG050 Table 9-10. Element <ParameterName>ppTblRunBefore[0]</ParameterName> is not used.</Text>
<Text><ParameterName>ppTblRunBefore[i]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>zerosLeft</Bold> =i, 0 < i < 7.</Text>
<Text><ParameterName>ppTblRunBefore[7]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>zerosLeft</Bold> > 6.</Text>

<Text>Each element of pointer array <ParameterName>ppTblTotalZeros</ParameterName> except for <ParameterName>ppTblTotalZeros[0]</ParameterName> points to a table that contains codes and values (JVTG050 <Bold>total_zeros</Bold>) in
accordance with JVTG050 Table 9-9. Element <ParameterName>ppTblTotalZeros[0]</ParameterName> is not used.</Text>
<Text>Each element <ParameterName>ppTblTotalZeros[i]</ParameterName> contains codes and values that correspond to JVTG050 <Bold>TotalCoeff</Bold> = i, 0 < i < 4.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeCAVLCChromaDcCoeffs_H264_1u16s        (       Fw32u **ppBitStream, Fw32s *pBitOffset  ,
                                                                          Fw16s  *pNumCoeff  , Fw16s **ppDstCoeffs,
                                                                    const Fw32s  *pTblCoeffToken   ,
                                                                    const Fw32s **ppTblTotalZerosCR, 
                                                                    const Fw32s **ppTblRunBefore );

/*#FunctionBlock - DecodeExpGolombOne_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Decode ExpGolomb code
#Long - <Text>This function decodes a block of MPEG-4/AVC video syntax elements in Exp-Golumb format located in a bitstream. The result is written to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeExpGolombOne_H264_1u16s               ( Fw32u **ppBitStream, Fw32s *pBitOffset, Fw16s *pDst, Fw8u isSigned );

// ------------------------------------------------
//  H.264 Deblocking Filtering
// ------------------------------------------------
/*#FunctionBlock - FilterDeblockingLuma_VerEdge_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Deblock luma macroblock vertical edges
#Long - <Text>This function steps through a source 16X16 luma macroblock and filters artificial discontinuities at block boundaries caused by quantization and other encoding artifacts. It operates on internal or external vertical edges of the macroblock.
Results are written back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingLuma_VerEdge_H264_8u_C1IR   ( Fw8u *pSrcDst    , Fw32s srcDstStep,
                                                                    Fw8u *pAlpha     , Fw8u *pBeta     ,
                                                                    Fw8u *pThresholds, Fw8u *pBS );

/*#FunctionBlock - FilterDeblockingLuma_HorEdge_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Deblock luma macroblock horizontal edges
#Long - <Text>This function steps through a source 16X16 luma macroblock and filters artificial discontinuities at block boundaries caused by quantization and other encoding artifacts. It operates on internal or external horizontal edges of the macroblock.
Results are written back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingLuma_HorEdge_H264_8u_C1IR   ( Fw8u *pSrcDst    , Fw32s srcDstStep,
                                                                    Fw8u *pAlpha     , Fw8u *pBeta     ,
                                                                    Fw8u *pThresholds, Fw8u *pBS );

/*#FunctionBlock - FilterDeblockingChroma_HorEdge_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Deblock chroma macroblock horizontal edges
#Long - <Text>This function steps through a source 8X8 chroma macroblock and filters artificial discontinuities at block boundaries caused by quantization and other encoding artifacts. It operates on internal or external horizontal edges of the macroblock.
Results are written back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingChroma_HorEdge_H264_8u_C1IR ( Fw8u *pSrcDst    , Fw32s srcDstStep,
                                                                    Fw8u *pAlpha     , Fw8u *pBeta     ,
                                                                    Fw8u *pThresholds, Fw8u *pBS );

/*#FunctionBlock - FilterDeblockingChroma_VerEdge_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Deblock chroma macroblock vertical edges
#Long - <Text>This function steps through a source 8X8 chroma macroblock and filters artificial discontinuities at block boundaries caused by quantization and other encoding artifacts. It operates on internal or external vertical edges of the macroblock.
Results are written back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingChroma_VerEdge_H264_8u_C1IR ( Fw8u *pSrcDst    , Fw32s srcDstStep,
                                                                    Fw8u *pAlpha     , Fw8u *pBeta     ,
                                                                    Fw8u *pThresholds, Fw8u *pBS );

// ------------------------------------------------
//  H.264 Inter-predict
// ------------------------------------------------

/*#FunctionBlock - InterpolateLuma_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate luma component
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by stepping through a defined region of a source reference frame and performing quarter-pixel luma-component interpolation.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>. </Text>
<Text>To assure that all samples used for interpolation are within the boundaries of the source frame, either enlarge the source frame by using boundary samples or use the <FunctionName>InterpolateLumaTop_H264</FunctionName> or
<FunctionName>InterpolateLumaBottom_H264</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateLuma_H264_8u_C1R                 (const Fw8u *pSrc, Fw32s srcStep,
                                                                         Fw8u *pDst, Fw32s dstStep,
                                                                         Fw32s dx  , Fw32s dy, FwiSize roiSize );

/*#FunctionBlock - InterpolateLumaTop_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate luma component at top frame boundary
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by performing quarter-pixel luma-component interpolation near the top boundary of a defined region of a source reference frame.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>.
When the source region overlaps a frame boundary, the region of interpolation is adjusted according to the value specified by <ParameterName>outPixels</ParameterName> to assure that
the closest line of pixel data in the frame is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateLumaTop_H264_8u_C1R              ( const Fw8u *pSrc, Fw32s srcStep,
                                                                          Fw8u *pDst, Fw32s dstStep,
                                                                          Fw32s dx  , Fw32s dy, Fw32s outPixels, FwiSize roiSize );

/*#FunctionBlock - InterpolateLumaBottom_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate luma component at bottom frame boundary
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by performing quarter-pixel luma-component interpolation near the bottom boundary of a defined region of a source reference frame.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>.
When the source region overlaps a frame boundary, the region of interpolation is adjusted according to the value specified by <ParameterName>outPixels</ParameterName> to assure that
the closest line of pixel data in the frame is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateLumaBottom_H264_8u_C1R           ( const Fw8u *pSrc, Fw32s srcStep, 
                                                                          Fw8u *pDst, Fw32s dstStep,
                                                                          Fw32s dx  , Fw32s dy, Fw32s outPixels, FwiSize roiSize );

/*#FunctionBlock - InterpolateChroma_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate chroma component
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by stepping through a defined region of a source reference frame and performing eighth-pixel chroma-component interpolation.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>. </Text>
<Text>To assure that all samples used for interpolation are within the boundaries of the source frame, either enlarge the source frame by using boundary samples or use the <FunctionName>InterpolateChromaTop_H264</FunctionName> or
<FunctionName>InterpolateChromaBottom_H264</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateChroma_H264_8u_C1R               ( const Fw8u *pSrc, Fw32s srcStep,
                                                                          Fw8u *pDst, Fw32s dstStep,
                                                                          Fw32s dx  , Fw32s dy, FwiSize roiSize );

/*#FunctionBlock - InterpolateChromaTop_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate chroma component at top frame boundary
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by performing eighth-pixel chroma-component interpolation near the top boundary of a defined region of a source reference frame.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>.
When the source region overlaps a frame boundary, the region of interpolation is adjusted according to the value specified by <ParameterName>outPixels</ParameterName> to assure that
the closest line of pixel data in the frame is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateChromaTop_H264_8u_C1R            ( const Fw8u *pSrc, Fw32s srcStep,
                                                                          Fw8u *pDst, Fw32s dstStep,
                                                                          Fw32s dx  , Fw32s dy, Fw32s outPixels, FwiSize roiSize );

/*#FunctionBlock - InterpolateChromaBottom_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Interpolate chroma component at bottom frame boundary
#Long - <Text>This function increases the accuracy of inter-frame motion prediction by performing eighth-pixel chroma-component interpolation near the bottom boundary of a defined region of a source reference frame.
The interpolation method is convolution with a 6X6 kernel, in accordance with JVTG050 8.4.2.2.1. Results are written to a defined ROI in a destination buffer.</Text>
<Text>The function uses only the fractional portion of the complex motion vector to perform interpolation. The integer portion of the vector is used to define the source location <ParameterName>pSrc</ParameterName>.
When the source region overlaps a frame boundary, the region of interpolation is adjusted according to the value specified by <ParameterName>outPixels</ParameterName> to assure that
the closest line of pixel data in the frame is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsBadArgErr, fwStsSizeErr
*/
FwStatus STDCALL fwiInterpolateChromaBottom_H264_8u_C1R         ( const Fw8u *pSrc, Fw32s srcStep,
                                                                          Fw8u *pDst, Fw32s dstStep,
                                                                          Fw32s dx  , Fw32s dy, Fw32s outPixels, FwiSize roiSize );



// ------------------------------------------------
//  H.264 Intra-predict
// ------------------------------------------------
/*#FunctionBlock - PredictIntra_4x4_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Predict motion of luma block intra-frame
#Long - <Text>This function performs intra-frame motion prediction by comparing a 4X4 luma block to previously-encoded neighboring 4X4 luma blocks within the same frame.
Results are written back to the source location. Comparison is done in accordance with JVTG050 8.3.1.2., using one of the following prediction modes specified by parameter <ParameterName>predMode</ParameterName>.</Text>
<Pseudo> Name of Constant  Prediction Mode   JVTG050 Reference</Pseudo>
<Pseudo>
   FW_4x4_VERT        Intra_4x4_Vertical      8.3.1.2.1
   FW_4x4_HOR        Intra_4x4_Horizontal      8.3.1.2.2
   FW_4x4_DC        Intra_4x4_D       8.3.1.2.3
   FW_4x4_DIAG_DL       Intra_4x4_Diagonal_Down_Left     8.3.1.2.4
   FW_4x4_DIAG_DR       Intra_4x4_Diagonal_Down_Right     8.3.1.2.5
   FW_4x4_VR        Intra_4x4_Vertical_Right      8.3.1.2.6
   FW_4x4_HD        Intra_4x4_Horizontal_Down      8.3.1.2.7
   FW_4x4_VL        Intra_4x4_Vertical_Left      8.3.1.2.8
   FW_4x4_H        Intra_4x4_Horizontal_Up      8.3.1.2.9
</Pseudo>

<Text>The <ParameterName>availability</ParameterName> parameter is calculated as follows.</Text>
<Pseudo> B1*FW_LEFT + B2*FW_UPPER_LEFT + B3*FW_UPPER + B4*FW_UPPER_RIGHT</Pseudo>
<Text>The constants FW_LEFT, FW_UPPER_LEFT, FW_UPPER, FW_UPPER_RIGHT are from <ParameterName>FwLayoutFlag</ParameterName>.</Text>
<Text>The variables B1, B2, B3, B4 take the following values.</Text>
<Pseudo>
 0 - when a block is not available for prediction
 1 - when a block is available for prediction
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsOutOfRangeErr, fwStsResFloor
*/
FwStatus STDCALL fwiPredictIntra_4x4_H264_8u_C1IR               ( Fw8u *pSrcDst, Fw32s srcDstStep,
                                                                    FwIntra4x4PredMode_H264 predMode, Fw32s availability );

/*#FunctionBlock - PredictIntra_16x16_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Predict motion of luma macroblock intra-frame
#Long - <Text>This function performs intra-frame motion prediction by comparing a 16X16 luma macroblock to previously-encoded neighboring 16X16 luma macroblocks within the same frame.
Results are written back to the source location. Comparison is done in accordance with JVTG050 8.3.2., using one of the following prediction modes specified by parameter <ParameterName>predMode</ParameterName>.</Text>
<Pseudo> Name of Constant  Prediction Mode   JVTG050 Reference</Pseudo>
<Pseudo>
   FW_16x16_VERT       Intra_16x16_Vertical       8.3.2.1
   FW_16x16_HOR        Intra_16x16_Horizontal       8.3.2.2
   FW_16x16_DC        Intra_16x16_DC        8.3.2.3
   FW_16x16_PLANE       Intra_16x16_Plane        8.3.2.4
</Pseudo>

<Text>The <ParameterName>availability</ParameterName> parameter is calculated as follows.</Text>
<Pseudo> B1*FW_LEFT + B2*FW_UPPER_LEFT + B3*FW_UPPER</Pseudo>
<Text>The constants FW_LEFT, FW_UPPER_LEFT, and FW_UPPER are from <ParameterName>FwLayoutFlag</ParameterName>.</Text>
<Text>The variables B1, B2, and B3 take the following values.</Text>
<Pseudo>
 0 - when a macroblock is not available for prediction
 1 - when a macroblock is available for prediction
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsOutOfRangeErr, fwStsResFloor
*/
FwStatus STDCALL fwiPredictIntra_16x16_H264_8u_C1IR             ( Fw8u *pSrcDst, Fw32s srcDstStep  ,
                                                                    FwIntra16x16PredMode_H264 predMode, Fw32s availability );

/*#FunctionBlock - PredictIntraChroma8x8_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Predict motion of chroma block intra-frame
#Long - <Text>This function performs intra-frame motion prediction by comparing an 8X8 chroma block to previously-encoded neighboring 8X8  chroma blocks within the same frame.
Results are written back to the source location. Comparison is done in accordance with JVTG050 8.3.2., using one of the following prediction modes specified by parameter <ParameterName>predMode</ParameterName>.</Text>
<Pseudo> Name of Constant  Prediction Mode   JVTG050 Reference</Pseudo>
<Pseudo>
   FW_8x8_VERT        Intra_8x8_Vertical       8.3.3.1
   FW_8x8_HOR        Intra_8x8_Horizontal       8.3.3.2
   FW_8x8_DC        Intra_8x8_DC        8.3.3.3
   FW_8x8_PLANE        Intra_8x8_Plane        8.3.3.4
</Pseudo>

<Text>The <ParameterName>availability</ParameterName> parameter is calculated as follows.</Text>
<Pseudo> B1*FW_LEFT + B2*FW_UPPER_LEFT + B3*FW_UPPER</Pseudo>
<Text>The constants FW_LEFT, FW_UPPER_LEFT, and FW_UPPER are from <ParameterName>FwLayoutFlag</ParameterName>.</Text>
<Text>The variables B1, B2, and B3 take the following values.</Text>
<Pseudo>
 0 - when a block is not available for prediction
 1 - when a block is available for prediction
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsOutOfRangeErr, fwStsResFloor
*/
FwStatus STDCALL fwiPredictIntraChroma8x8_H264_8u_C1IR          ( Fw8u *pSrcDst, Fw32s srcDstStep   ,
                                                                    FwIntraChromaPredMode_H264 predMode, Fw32s availability );

// ------------------------------------------------
//  H.264 Reconstruction
// ------------------------------------------------
/*#FunctionBlock - ReconstructChromaInterMB_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Reconstruct chroma macroblock inter-frame
#Long - <Text>This function steps through source U and V plane buffers and reconstructs inter-frame predicted 8X8 U and 8X8 V chroma macroblocks as follows.</Text>
<Text>Performs an integer inverse transformation and dequantizes 2X2 U and V DC coefficients in accordance with JVTG050 8.5.7.</Text>
<Text>Scales, performs an integer inverse transformation, and shifts 4X4 AC U blocks in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 8-7.</Text>
<Text>Shifts 4X4 AC V blocks in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 8-7.</Text>
<Text>Adds 8X8 inter-frame prediction blocks and 8X8 residual blocks in accordance with JVTG050, Figure 8-247.</Text>
<Text>Results are written back to the source plane buffers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiReconstructChromaInterMB_H264_16s8u_P2R     (       Fw16s **ppSrcCoeff  ,       Fw8u  *pSrcDstUPlane,
                                                                          Fw8u  *pSrcDstVPlane, const Fw32u srcDstStep    ,
                                                                    const Fw32u cbp4x4        , const Fw32s ChromaQP );


/*#FunctionBlock - ReconstructChromaIntraMB_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Reconstruct chroma macroblock intra-frame
#Long - <Text>This function steps through source U and V plane buffers and reconstructs intra-frame predicted 8X8 U and 8X8 V chroma macroblocks as follows.</Text>
<Text>Performs an integer inverse transformation and dequantizes 2X2 U and V DC coefficients in accordance with JVTG050 8.5.7.</Text>
<Text>Scales, performs an integer inverse transformation, and shifts 4X4 AC U blocks in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 8-7.</Text>
<Text>Shifts 4X4 AC V blocks in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 8-7.</Text>
<Text>Adds 8X8 intra-frame prediction blocks and 8X8 residual blocks in accordance with JVTG050, Figure 8-247.</Text>
<Text>Results are written back to the source plane buffers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiReconstructChromaIntraMB_H264_16s8u_P2R     ( Fw16s **ppSrcCoeff   ,       Fw8u *pSrcDstUPlane,
                                                                    Fw8u  *pSrcDstVPlane , const Fw32u srcDstUVStep ,
                                                                    const FwIntraChromaPredMode_H264 intraChromaMode ,
                                                                    const Fw32u cbp4x4   , const Fw32u ChromaQP     ,
                                                                    const Fw8u edgeType );

/*#FunctionBlock - ReconstructLumaInterMB_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Reconstruct luma macroblock inter-frame
#Long - <Text>This functions steps through a source Y plane buffer and reconstructs intra-frame predicted 4X4 luma macroblocks as follows.</Text>
<Text>Scales, performs an integer inverse transformation, and shifts 4X4 blocks in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 6-6.</Text>
<Text>Adds 16X16 inter-frame prediction blocks and 16X16 residual blocks in accordance with JVTG050, Figure 8-247.</Text>
<Text>Results are written back to the source plane buffers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/

FwStatus STDCALL fwiReconstructLumaInterMB_H264_16s8u_C1R       (       Fw16s **ppSrcCoeff, Fw8u *pSrcDstYPlane,
                                                                    const Fw32u srcDstYStep ,
                                                                    const Fw32u cbp4x4      ,
                                                                    const Fw32s QP );

/*#FunctionBlock - ReconstructLumaIntraMB_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Reconstruct luma macroblock intra-frame
#Long - <Text>This functions steps through a source Y plane buffer and reconstructs intra-frame predicted 4X4 luma macroblocks as follows.</Text>
<Text>Scales, performs an integer inverse transformation, and shifts each 4X4 block in accordance with JVTG050 8.5.8., in the order shown in JVTG050, Figure 6-6.</Text>
<Text>Performs intra-frame prediction for each 4X4 block in accordance with JVTG050 8.3.1.2.</Text>
<Text>Adds 4X4 inter-frame prediction blocks and 4X4 residual blocks in accordance with JVTG050, Figure 8-247.</Text>
<Text>Results are written back to the source plane buffers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiReconstructLumaIntraMB_H264_16s8u_C1R       (       Fw16s **ppSrcCoeff, Fw8u *pSrcDstYPlane,
                                                                          Fw32s srcDstYStep ,
                                                                    const FwIntra4x4PredMode_H264 *pMBIntraTypes  ,
                                                                    const Fw32u cbp4x4      ,
                                                                    const Fw32u QP          ,
                                                                    const Fw8u edgeType );

#endif

#if BUILD_NUM_AT_LEAST( 101 )


// ------------------------------------------------
// H.264 Inverse Transform
// ------------------------------------------------
/*#FunctionBlock - TransformDequantChromaDC_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Inverse transform chroma DC
#Long - <Text>This function steps through a source buffer, performs integer inverse transformation and dequantization of 2X2 chroma DC coefficients in accordance with JVTG050 8.5.7., and writes the results back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiTransformDequantChromaDC_H264_16s_C1I       ( Fw16s *pSrcDst, Fw32s QP );

/*#FunctionBlock - DequantTransformResidual_H264
#Technologies - REF, SSE2
#Short - Inverse transform residual
#Long - <Text>This function steps through a source buffer, performs scaling, inverse transformation, and shift of a residual 4X4 block in accordance with JVTG050 8.5.7., and writes the result back to the source buffer.</Text>
<Text>For chroma block transforms, call this function after <FunctionName>TransformDequantChromaDC_H264</FunctionName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiDequantTransformResidual_H264_16s_C1I       ( Fw16s *pSrcDst, Fw32s step, Fw16s *pDC, Fw32s AC, Fw32s QP );

#endif

#if BUILD_NUM_AT_LEAST( 101 )

/*#FunctionBlock - HuffmanTableInitAlloc
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Allocate memory and initialize one-to-one Huffman decode table
#Long - <Text>This function allocates memory and initializes the structure for a Huffman decode table.
The table is used to decode bitstreams encoded with variable-length coding in which one code corresponds to one value.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiHuffmanTableInitAlloc_32s                   ( const Fw32s *pSrcTable, FwVCHuffmanSpec_32s **ppDstSpec );


/*#FunctionBlock - HuffmanRunLevelTableInitAlloc
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Allocate memory and initialize one-to-two Huffman decode table
#Long - <Text>This function allocates memory and initializes the structure for a Huffman decode table.
The table is used to decode bitstreams encoded with variable-length coding in which one code corresponds to two values.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiHuffmanRunLevelTableInitAlloc_32s           ( const Fw32s *pSrcTable, FwVCHuffmanSpec_32s **ppDstSpec );

/*#FunctionBlock - DecodeHuffmanOne
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Decode Huffman one-to-one
#Long - <Text>This function decodes a single code from a source bitstream using a specified table, writes a single-value result to a destination buffer,
and sets the bitstream pointers to new positions. The function uses the table initialized by the <FunctionName>HuffmanTableInitAlloc</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiDecodeHuffmanOne_1u32s                      ( Fw32u **ppBitStream, Fw32s *pOffset, Fw32s *pDst, const FwVCHuffmanSpec_32s *pDecodeTable );

/*#FunctionBlock - DecodeHuffmanPair
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Decode Huffman one-to-two
#Long - <Text>This function decodes a single code from a source bitstream using a specified table, writes the two resulting values result to separate destination buffers,
and sets the bitstream pointers to new positions. The function uses the table initialized by the <FunctionName>HuffmanRunLevelInitAlloc</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr and fwStsH263VLCCodeErr
*/
FwStatus STDCALL fwiDecodeHuffmanPair_1u16s                     ( Fw32u **ppBitStream, Fw32s *pOffset, const FwVCHuffmanSpec_32s *pDecodeTable, Fw8s *pFirst, Fw16s *pSecond );

/*#FunctionBlock - HuffmanTableFree
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Free memory allocated to Huffman decode table
#Long - <Text>This function frees the memory at the location pointed to by <ParameterName>ppDecodeTable</ParameterName> allocated to a Huffman decode table.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiHuffmanTableFree_32s                        ( FwVCHuffmanSpec_32s **ppDecodeTable );

#endif


#if BUILD_NUM_AT_LEAST( 100 )

// ------------------------------------------------
//  Motion Compensation
// ------------------------------------------------
/*#FunctionBlock - MC16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 16X16 block
#Long - <Text>This function adds a 16X16 predicted block to a 16X16 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x16_8u_C1         ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC16x8
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 16X8 block
#Long - <Text>This function adds a 16X8 predicted block to a 16X8 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x8_8u_C1          ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC8x16
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 8X16 block
#Long - <Text>This function adds an 8X16 predicted block to an 8X16 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x16_8u_C1          ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC8x8
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 8X8 block
#Long - <Text>This function adds an 8X8 predicted block to an 8X8 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x8_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC8x4
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 8X4 block
#Long - <Text>This function adds an 8X4 predicted block to an 8X4 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x4_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC4x8
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 4X8 block
#Long - <Text>This function adds a 4X8 predicted block to a 4X8 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x8_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 4X4 block
#Long - <Text>This function adds a 4X4 predicted block to a 4X4 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x4_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC2x4
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 2X4 block
#Long - <Text>This function adds a 2X4 predicted block to a 2X4 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC2x4_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC4x2
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 4X2 block
#Long - <Text>This function adds a 4X2 predicted block to a 4X2 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x2_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC2x2
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 2X2 block
#Long - <Text>This function adds a 2X2 predicted block to a 2X2 decoded residual block to reconstruct a source block.
Prediction is based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC2x2_8u_C1           ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC16x4
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 16X4 UV block
#Long - <Text>This function adds a 16X4 predicted UV block to a 16X4 decoded residual UV block to reconstruct a source UV block.
Prediction is based on the values in a reference UV block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x4_8u_C1          ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC16x8UV
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for predicted 16X8 UV block
#Long - <Text>This function adds a 16X8 predicted UV block to a 16X8 decoded residual UV block to reconstruct a source UV block.
Prediction is based on the values in a reference UV block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x8UV_8u_C1        ( const Fw8u  *pSrcRef  , Fw32s srcStep     ,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep,
                                                    Fw8u  *pDst     , Fw32s dstStep     ,
                                                    Fw32s  mcType   , Fw32s roundControl );

/*#FunctionBlock - MC16x16B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 16X16 block
#Long - <Text>This function adds a 16X16 bi-predicted block to a 16X16 decoded residual block to reconstruct a source block.
Prediction is the average of a 16X16 F-prediction and a 16X16 backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x16B_8u_C1        ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC16x8B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 16X8 block
#Long - <Text>This function adds a 16X8 bi-predicted block to a 16X8 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x8B_8u_C1         ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC8x16B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 8X16 block
#Long - <Text>This function adds an 8X16 bi-predicted block to an 8X16 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x16B_8u_C1         ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst   ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC8x8B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 8X8 block
#Long - <Text>This function adds an 8X8 bi-predicted block to an 8X8 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x8B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst   ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC8x4B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 8X4 block
#Long - <Text>This function adds an 8X4 bi-predicted block to an 8X4 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC8x4B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst   ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC4x8B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 4X8 block
#Long - <Text>This function adds a 4X8 bi-predicted block to a 4X8 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x8B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u *pDst   ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC4x4B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 4X4 block
#Long - <Text>This function adds a 4X4 bi-predicted block to a 4X4 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x4B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC2x4B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 2X4 block
#Long - <Text>This function adds a 2X4 bi-predicted block to a 2X4 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC2x4B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC4x2B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 4X2 block
#Long - <Text>This function adds a 4X2 bi-predicted block to a 4X2 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC4x2B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC2x2B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 2X2 block
#Long - <Text>This function adds a 2X2 bi-predicted block to a 2X2 decoded residual block to reconstruct a source block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC2x2B_8u_C1          ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC16x4B
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 16X4 UV block
#Long - <Text>This function adds a 16X4 bi-predicted UV block to a 16X4 decoded residual UV block to reconstruct a source UV block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference UV block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x4B_8u_C1         ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

/*#FunctionBlock - MC16x8BUV
TODO:publish
#Technologies - REF, SSE2
#Short - Motion compensation for bi-predicted 16X8 UV block
#Long - <Text>This function adds a 16X8 bi-predicted UV block to a 16X8 decoded residual UV block to reconstruct a source UV block.
Prediction is the average of a forward prediction and a backward prediction based on the values in a reference UV block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the prediction block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the prediction block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the prediction block is the average of four reference block elements.</Text>
<Text>The half-sample prediction calculation allows for UV block structure. Neighboring horizontal elements of the U and V blocks have indexes i and i+2 in the UV block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiMC16x8BUV_8u_C1       ( const Fw8u  *pSrcRefF , Fw32s srcStepF    , Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB , Fw32s srcStepB    , Fw32s mcTypeB,
                                              const Fw16s *pSrcYData, Fw32s srcYDataStep, Fw8u  *pDst  ,
                                                    Fw32s  dstStep  , Fw32s roundControl );

// ------------------------------------------------
//  Motion Estimation
// ------------------------------------------------
/*#FunctionBlock - GetDiff16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 16X16 block
#Long - <Text>This function compares a 16X16 source block to a 16X16 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff16x16_8u16s_C1 ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );

/*#FunctionBlock - GetDiff16x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 16X8 block
#Long - <Text>This function compares a 16X8 source block to a 16X8 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff16x8_8u16s_C1  ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );


/*#FunctionBlock - GetDiff8x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 8X8 block
#Long - <Text>This function compares an 8X8 source block to an 8X8 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x8_8u16s_C1   ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );

/*#FunctionBlock - GetDiff8x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 8X16 block
#Long - <Text>This function compares an 8X16 source block to an 8X16 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x16_8u16s_C1  ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );

/*#FunctionBlock - GetDiff8x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 8X4 block
#Long - <Text>This function compares an 8X4 source block to an 8X4 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x4_8u16s_C1   ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );

/*#FunctionBlock - GetDiff4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate predicted 4X4 block
#Long - <Text>This function compares a 4X4 source block to an 4X4 reference block and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Values in the predictor block are based on the values in a reference block, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff4x4_8u16s_C1   ( const Fw8u  *pSrcCur      , Fw32s srcCurStep      ,
                                              const Fw8u  *pSrcRef      , Fw32s srcRefStep      ,
                                                    Fw16s *pDstDiff     , Fw32s dstDiffStep     ,
                                                    Fw16s *pDstPredictor, Fw32s dstPredictorStep,
                                                    Fw32s  mcType       , Fw32s roundControl );

/*#FunctionBlock - GetDiff16x16B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate bi-predicted 16X16 block
#Long - <Text>This function compares a 16X16 source block to two 16X16 reference blocks and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Prediction is the mean of a forward prediction and a backward prediction based on the values in two reference blocks, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff16x16B_8u16s_C1 ( const Fw8u  *pSrcCur , Fw32s srcCurStep ,
                                               const Fw8u  *pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF,
                                               const Fw8u  *pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
                                                     Fw16s *pDstDiff, Fw32s dstDiffStep, Fw32s roundControl );

/*#FunctionBlock - GetDiff16x8B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate bi-predicted 16X8 block
#Long - <Text>This function compares a 16X8 source block to two 16X8 reference blocks and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Prediction is the mean of a forward prediction and a backward prediction based on the values in two reference blocks, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff16x8B_8u16s_C1 ( const Fw8u  *pSrcCur , Fw32s srcCurStep ,
                                              const Fw8u  *pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
                                                    Fw16s *pDstDiff, Fw32s dstDiffStep, Fw32s roundControl );

/*#FunctionBlock - GetDiff8x8B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate bi-predicted 8X8 block
#Long - <Text>This function compares an 8X8 source block to two 8X8 reference blocks and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Prediction is the mean of a forward prediction and a backward prediction based on the values in two reference blocks, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x8B_8u16s_C1  ( const Fw8u  *pSrcCur , Fw32s srcCurStep ,
                                              const Fw8u  *pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
                                                    Fw16s *pDstDiff, Fw32s dstDiffStep, Fw32s roundControl );

/*#FunctionBlock - GetDiff8x16B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate bi-predicted 8X16 block
#Long - <Text>This function compares an 8X16 source block to two 8X16 reference blocks and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Prediction is the mean of a forward prediction and a backward prediction based on the values in two reference blocks, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x16B_8u16s_C1 ( const Fw8u  *pSrcCur , Fw32s srcCurStep ,
                                              const Fw8u  *pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB,
                                                    Fw16s *pDstDiff, Fw32s dstDiffStep, Fw32s roundControl );

/*#FunctionBlock - GetDiff8x4B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate bi-predicted 8X4 block
#Long - <Text>This function compares an 8X4 source block to two 8X4 reference blocks and writes results to a difference block and a predictor block in a destination plane.
The results are used to reduce error when subsequent blocks that refer to the source block are encoded.</Text>
<Text>Prediction is the mean of a forward prediction and a backward prediction based on the values in two reference blocks, using a compensation type specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, the reference block is used as the predictor block.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of the predictor block is the average of two reference block elements.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, each element of the predictor block is the average of four reference block elements.</Text>
<Text>Encoding has half-pixel accuracy, and a rounding method must be specified.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetDiff8x4B_8u16s_C1  ( const Fw8u  *pSrcCur , Fw32s srcCurStep ,
                                              const Fw8u  *pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF,
                                              const Fw8u  *pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB,
                                                    Fw16s *pDstDiff, Fw32s dstDiffStep, Fw32s roundControl );

/*#FunctionBlock - SqrDiff16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sum of squares, 16X16 block
#Long - <Text>This function evaluates the sum of squares variance between all the elements of a 16X16 source block and the corresponding elements of a 16X16 reference block.
Integer results are written to a location specified by <ParameterName>pSqrDiff</ParameterName>.</Text>
<Text>A compensation type for the reference block is specified by the <ParameterName>mcType</ParameterName> parameter.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, each element of the reference block is used.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, the average of two reference block elements is used.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, the average of four reference block elements is used.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSqrDiff16x16_8u32s    ( const Fw8u *pSrc  , Fw32s srcStep,
                                              const Fw8u *pRef  , Fw32s refStep,
                                                    Fw32s mcType, Fw32s *pSqrDiff );

/*#FunctionBlock - SqrDiff16x16B
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sum of squares, bi-predicted 16X16 block
#Long - <Text>This function evaluates the sum of squares variance between all the elements of a bi-predicted 16X16 source block
and the corresponding elements of a 16X16 forward reference block and a 16X16 backward reference block.
Integer results are written to a location specified by <ParameterName>pSqrDiff</ParameterName>.</Text>
<Text>Compensation types for the forward and backward reference blocks are specified by the <ParameterName>mcTypeF</ParameterName> and <ParameterName>mcTypeB</ParameterName> parameters.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FF, each element of the reference block is used.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_FH or FWVC_MC_APX_HF, the average of two reference block elements is used.</Text>
<Text>When <ParameterName>mcType</ParameterName> = FWVC_MC_APX_HH, the average of four reference block elements is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSqrDiff16x16B_8u32s   ( const Fw8u  *pSrc , Fw32s srcStep ,
                                              const Fw8u  *pRefF, Fw32s refStepF, Fw32s mcTypeF,
                                              const Fw8u  *pRefB, Fw32s refStepB, Fw32s mcTypeB,
                                                    Fw32s *pSqrDiff );

/*#FunctionBlock - VarMean8x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate mean and variance, 8x8 block
#Long - <Text>The unsigned 8-bit version of this function evaluates the mean and variance of an 8x8 block of unsigned char values.</Text>
<Text>The signed 16-bit version evaluates the mean and variance of an 8x8 block of shor integer values.</Text>
<Text>Results are written to locations specified by <ParameterName>pVar</ParameterName> and <ParameterName>pMean</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiVarMean8x8_8u32s_C1R  ( const Fw8u  *pSrc, Fw32s srcStep, Fw32s *pVar, Fw32s *pMean );
FwStatus STDCALL fwiVarMean8x8_16s32s_C1R ( const Fw16s *pSrc, Fw32s srcStep, Fw32s *pVar, Fw32s *pMean );

/*#FunctionBlock - VarMeanDiff16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate means and variances, four 8X8 blocks of difference between two 16X16 blocks
#Long - <Text>This function evaluates the means and variances of two 8x8 blocks of difference between two 16x16 blocks of unsigned elements.</Text>
<Text>Parameter <ParameterName>pSrcSum</ParameterName> is used to avoid excessive computation as the function is repeatedly called for the same current block.
<ParameterName>pSrcSum</ParameterName> must point to the array of four 8x8 blocks that make up the current block.
Parameters <ParameterName>pVar</ParameterName> and <ParameterName>pMean</ParameterName> must point to four-element arrays
because variance and mean are calculated for each 8x8 block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiVarMeanDiff16x16_8u32s_C1R      ( const Fw8u  *pSrc , Fw32s srcStep,
                                                        const Fw8u  *pRef , Fw32s refStep, Fw32s *pSrcSum,
                                                              Fw32s *pVar , Fw32s *pMean , Fw32s  mcType );

/*#FunctionBlock - VarMeanDiff16x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate means and variances, two 8X8 blocks of difference between two 16X8 blocks
#Long - <Text>This function evaluates the means and variances of two 8x8 blocks of difference between 16x8 blocks of unsigned elements.</Text>
<Text>Parameter <ParameterName>pSrcSum</ParameterName> is used to avoid excessive computation as the function is repeatedly called for the same current block.
<ParameterName>pSrcSum</ParameterName> must point to the array of four 8x8 blocks that make up the current block.
Parameters <ParameterName>pVar</ParameterName> and <ParameterName>pMean</ParameterName> must point to four-element arrays
because variance and mean are calculated for each 8x8 block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiVarMeanDiff16x8_8u32s_C1R       ( const Fw8u  *pSrc , Fw32s srcStep,
                                                        const Fw8u  *pRef , Fw32s refStep, Fw32s *pSrcSum,
                                                              Fw32s *pVar , Fw32s *pMean , Fw32s  mcType );

/*#FunctionBlock - Variance16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate variance, 16X16 block
#Long - <Text>This function evaluates the variance of a 16X16 block of unsigned elements.
The result is stored in integer <ParameterName>Var</ParameterName>.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiVariance16x16_8u32s             ( const Fw8u *pSrc, Fw32s srcStep, Fw32s *pVar );

/*#FunctionBlock - EdgesDetect16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Detect edges within 16X16 block
#Long - <Text>This function detects edges within a 16X16 block by finding pairs of adjacent horizontal and vertical
elements with a difference greater than the value specified by <ParameterName>EdgePelDifference</ParameterName>.</Text>
<Text>When the number of detected pairs is greater than <ParameterName>EdgePelCount</ParameterName>, <ParameterName>pRes</ParameterName>) = 1.</Text>
<Text>When the number of detected pairs is less than or equal to <ParameterName>EdgePelCount</ParameterName>, <ParameterName>pRes</ParameterName>)= 0.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEdgesDetect16x16_8u_C1R         ( const Fw8u *pSrc, Fw32u srcStep, Fw8u EdgePelDifference, Fw8u EdgePelCount, Fw8u *pRes );

/*#FunctionBlock - SAD16x16
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sum of absolute difference, 16X16 block
#Long - <Text>This function evaluates the sum of absolute difference between the elements of a 16X16 block and the corresponding elements of a reference block.
The integer result is stored in the location pointed to by <ParameterName>pSAD</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSAD16x16_8u32s                  ( const Fw8u  *pSrc, Fw32s srcStep,
                                                        const Fw8u  *pRef, Fw32s refStep,
                                                              Fw32s *pSAD, Fw32s mcType );

/*#FunctionBlock - SAD8x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sum of absolute difference, 8x8 block
#Long - <Text>This function evaluates the sum of absolute difference between the elements of an 8X8 block and the corresponding elements of a reference block.
The integer result is stored in the location pointed to by <ParameterName>pSAD</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSAD8x8_8u32s_C1R                ( const Fw8u  *pSrcCur, int srcCurStep, 
                                                        const Fw8u  *pSrcRef, Fw32s srcRefStep,
                                                              Fw32s *pDst   , Fw32s mcType );

/*#FunctionBlock - SAD4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sum of absolute difference, 4x4 block
#Long - <Text>This function evaluates the sum of absolute difference between the elements of a 4X4 block and the corresponding elements of a reference block.
The integer result is stored in the location pointed to by <ParameterName>pSAD</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSAD4x4_8u32s                    ( const Fw8u  *pSrc, Fw32s srcStep,
                                                              Fw8u  *pRef, Fw32s refStep, 
                                                              Fw32s *pSAD, Fw32s mcType );

/*#FunctionBlock - SAD16x16Blocks8x8
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate four partial sums of absolute differences, 16x16 block
#Long - <Text>This function evaluates four partial sums of absolute differences between a 16X16 source block divided into four 8X8 blocks of elements and a 16X16 reference block divided
into four corresponding 8X8 blocks of elements. The four results are written to a four-element array pointed to by <ParameterName>pDstSAD</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSAD16x16Blocks8x8_8u16u         ( const Fw8u  *pSrc   , Fw32s srcStep, 
                                                              Fw8u  *pRef   , Fw32s refStep, 
                                                              Fw16u *pDstSAD, Fw32s mcType );

/*#FunctionBlock - SAD16x16Blocks4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sixteen partial sums of absolute differences, 16x16 block
#Long - <Text>This function evaluates sixteen partial sums of absolute differences between a 16X16 source block divided into sixteen 4X4 blocks of elements and a 16X16 reference block divided
into sixteen corresponding 4X4 blocks of elements. The sixteen results are written to a sixteen-element array pointed to by <ParameterName>pDstSAD</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSAD16x16Blocks4x4_8u16u         ( const Fw8u  *pSrc   , Fw32s srcStep,
                                                              Fw8u  *pRef   , Fw32s refStep,
                                                              Fw16u *pDstSAD, Fw32s mcType );

/*#FunctionBlock - SumsDiff16x16Blocks4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate sixteen sums of differences, 16X16
#Long - <Text>This function evaluates sixteen differences between a 16X16 source block divided into sixteen 4X4 blocks of elements and a 16X16 reference block divided
into sixteen corresponding 4X4 blocks of elements, then sums the results with sixteen 4X4 residual block elements pointed to by <ParameterName>pDiff</ParameterName>.
The sixteen results are written to a sixteen-element array pointed to by <ParameterName>pSums</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSumsDiff16x16Blocks4x4_8u16s_C1 ( Fw8u  *pSrc , Fw32s srcStep ,
                                                        Fw8u  *pPred, Fw32s predStep,
                                                        Fw16s *pSums, Fw16s *pDiff );

/*#FunctionBlock - SumsDiff8x8Blocks4x4
TODO:publish
#Technologies - REF, SSE2
#Short - Evaluate four sums of differences, 16X16 block
#Long - <Text>This function evaluates four differences between a 16X16 source block divided into four 8X8 blocks of elements and a 16X16 reference block divided
into four corresponding 8X8 blocks of elements, then sums the results with four 8X8 residual block elements pointed to by <ParameterName>pDiff</ParameterName>.
The four results are written to a four-element array pointed to by <ParameterName>pSums</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiSumsDiff8x8Blocks4x4_8u16s_C1   ( Fw8u  *pSrc , Fw32s srcStep ,
                                                        Fw8u  *pPred, Fw32s predStep,
                                                        Fw16s *pSums, Fw16s *pDiff );


#endif // FW_BUILD_NUM

/*#Documentation

</Chapter>

*/

#if BUILD_NUM_AT_LEAST(9999)
/*#FunctionBlock - DeblockingLuma_VerEdge_MBAFF_H264
TODO:edit
#Technologies - REF, SSE2
#Short - To Do
#Long - To Do
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingLuma_VerEdge_MBAFF_H264_8u_C1IR   ( Fw8u  *pSrcDst    , Fw32s srcDstStep,
                                                                          Fw32u nAlpha      , Fw32u nBeta     ,
                                                                          Fw8u  *pThresholds, Fw8u  *pBS );

/*#FunctionBlock - DeblockingChroma_VerEdge_MBAFF_H264
TODO:edit
#Technologies - REF, SSE2
#Short - To Do
#Long - To Do
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiFilterDeblockingChroma_VerEdge_MBAFF_H264_8u_C1IR ( Fw8u  *pSrcDst    , Fw32s srcDstStep,
                                                                          Fw32u nAlpha      , Fw32u nBeta,
                                                                          Fw8u  *pThresholds, Fw8u  *pBS );

/*#FunctionBlock - ExpandPlane_H264
TODO:edit
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiExpandPlane_H264_8u_C1R                       ( Fw8u  *StartPtr   , Fw32u uFrameWidth,
                                                                      Fw32u uFrameHeight, Fw32u uPitch,
                                                                      Fw32u uPels       , FwvcFrameFieldFlag uFrameFieldFlag );

/*#FunctionBlock - InterpolateBlock_H264
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiInterpolateBlock_H264_8u_P2P1R                ( Fw8u  *pSrc1, Fw8u  *pSrc2,
                                                                      Fw8u  *pDst , Fw32u width ,
                                                                      Fw32u height, Fw32u pitch );

/*#FunctionBlock - WeightedAverage_H264
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiWeightedAverage_H264_8u_C1IR                  ( const Fw8u  *pSrc1    , Fw8u  *pSrc2Dst,
                                                                            Fw32s srcDstStep, Fw32s weight1  , Fw32s weight2,
                                                                            Fw32s shift     , Fw32s offset   , FwiSize roiSize );
///*
//FwStatus STDCALL fwiUniDirWeightBlock_H264_8u_C1IR(Fw8u *pSrcDst, Fw32u srcDstStep,
//   Fw32u ulog2wd, Fw32s iWeight, Fw32s iOffset, FwiSize roi);
//FwStatus STDCALL fwiBiDirWeightBlock_H264_8u_P2P1R(Fw8u *pSrc1, Fw8u *pSrc2, Fw8u *pDst,
//    Fw32u srcStep, Fw32u dstStep, Fw32u ulog2wd, Fw32s iWeight1,
//    Fw32s iOffset1, Fw32s iWeight2, Fw32s iOffset2, FwiSize roi);
//FwStatus STDCALL fwiBiDirWeightBlockImplicit_H264_8u_P2P1R(Fw8u *pSrc1, Fw8u *pSrc2,
//    Fw8u *pDst, Fw32u srcStep, Fw32u dstStep, Fw32s iWeight1, Fw32s iWeight2, FwiSize roi);
//*/

FwStatus STDCALL fwiReconstructLumaIntraHalfMB_H264_16s8u_C1R     ( Fw16s **ppSrcCoeff, Fw8u *pSrcDstYPlane,
                                                                      Fw32s srcDstYStep , FwIntra4x4PredMode_H264 *pMBIntraTypes,
                                                                      Fw32u cbp4x2 , Fw32u QP, Fw8u edgeType );

/*#FunctionBlock - ReconstructChromaIntraHalvesMB_H264
TODO:edit
#Technologies - REF, SSE2
#Short - Reconstruct video
#Long - <Text>These functions reconstruct intra-block and inter-block images by performing integer inverse transforms, scaling, prediction, and adding to residual blocks.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsOutOfRangeErr
*/
FwStatus STDCALL fwiReconstructChromaIntraHalvesMB_H264_16s8u_P2R ( Fw16s **ppSrcCoeff  , Fw8u  *pSrcDstUPlane,
                                                                      Fw8u  *pSrcDstVPlane, Fw32u srcDstUVStep  ,
                                                                      FwIntraChromaPredMode_H264 intraChromaMode ,
                                                                      Fw32u cbp4x4, Fw32u ChromaQP, Fw8u edgeTypeTop, Fw8u edgeTypeBottom );
///*
//FwStatus STDCALL fwiReconstructLumaInter4x4MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32u srcDstYStep,
// Fw32u cbp4x4, Fw32s QP, Fw16s *pQuantTable, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructLumaIntraHalf4x4MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32s srcDstYStep, FwIntra4x4PredMode_H264 *pMBIntraTypes,
// Fw32u cbp4x2, Fw32s QP, Fw8u edgeType, Fw16s *pQuantTable, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructLumaIntra4x4MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u  *pSrcDstYPlane, Fw32s srcDstYStep,FwIntra4x4PredMode_H264 *pMBIntraTypes,
// Fw32u cbp4x4, Fw32s QP, Fw8u  edgeType, Fw16s *pQuantTable, Fw8u  bypassFlag);
//FwStatus STDCALL fwiReconstructLumaInter8x8MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32u srcDstYStep, Fw32u cbp8x8,
// Fw32s QP, Fw16s *pQuantTable, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructLumaIntraHalf8x8MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32s srcDstYStep,
// fwintra8x8PredMode_H264 *pMBOIntraTypes, Fw32u cbp8x2, Fw32s QP,
// Fw8u  edgeType, Fw16s *pQuantTable, Fw8u  bypassFlag);
//FwStatus STDCALL fwiReconstructLumaIntra8x8MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32s srcDstYStep,
// fwintra8x8PredMode_H264 *pMBOIntraTypes, Fw32u cbp8x8, Fw32s QP, Fw8u edgeType, Fw16s *pQuantTable, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructLumaIntra16x16MB_H264_16s8u_C1R(
// Fw16s **ppSrcCoeff, Fw8u *pSrcDstYPlane, Fw32u srcDstYStep,
// const FwIntra16x16PredMode_H264 intra_luma_mode,
// const Fw32u cbp4x4, const Fw32u QP, const Fw8u edgeType);
//FwStatus STDCALL fwiReconstructLumaIntra_16x16MB_H264_16s8u_C1R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstYPlane, Fw32u srcDstYStep,
// FwIntra16x16PredMode_H264 intraLumaMode, Fw32u cbp4x4, Fw32u QP,
// Fw8u edgeType, Fw16s *pQuantTable, Fw8u bypassFlag);
//*/
///*
//FwStatus STDCALL fwiReconstructChromaInter4x4MB_H264_16s8u_P2R(
// Fw16s **ppSrcDstCoeff, Fw8u*pSrcDstUPlane, Fw8u *pSrcDstVPlane,
// Fw32u srcDstUVStep, Fw32u cbp4x4, Fw32s ChromaQPU, Fw32u ChromaQPV, Fw16s *pQuantTableU, Fw16s *pQuantTableV, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructChromaIntraHalves4x4MB_H264_16s8u_P2R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstUPlane, Fw8u *pSrcDstVPlane, Fw32u srcDstUVStep,
// FwIntraChromaPredMode_H264 intraChromaMode, Fw32u cbp4x4, Fw32s ChromaQPU, Fw32u ChromaQPV,
// Fw8u edgeTypeTop, Fw8u edgeTypeBottom, Fw16s *pQuantTableU, Fw16s *pQuantTableV, Fw8u bypassFlag);
//FwStatus STDCALL fwiReconstructChromaIntra4x4MB_H264_16s8u_P2R(
// Fw16s **ppSrcDstCoeff, Fw8u *pSrcDstUPlane, Fw8u *pSrcDstVPlane, Fw32u srcDstUVStep,
// FwIntraChromaPredMode_H264 intraChromaMode, Fw32u cbp4x4, Fw32s ChromaQPU, Fw32u ChromaQPV,
// Fw8u edgeType, Fw16s *pQuantTableU, Fw16s *pQuantTableV, Fw8u bypassFlag);
//*/

/*#FunctionBlock - DequantTransformResidualAndAdd_H264
TODO:edit
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiDequantTransformResidualAndAdd_H264_16s_C1I   ( const Fw8u  *pPred  , Fw16s *pSrcDst,
                                                                            Fw16s *pDC    , Fw8u  *pDst   ,
                                                                            Fw32s PredStep, Fw32s DstStep,
                                                                            Fw32s QP      , Fw32s AC );

/*#FunctionBlock - DequantTransformResidual_SISP_H264
TODO:edit
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiDequantTransformResidual_SISP_H264_16s_C1I    ( Fw16s *pSrcDst, Fw16s *pPredictBlock,
                                                                      Fw16s *pDC    , Fw32s AC            ,
                                                                      Fw32s qp      , Fw32s qs, Fw32s Switch );



/*#FunctionBlock - TransformDequantChromaDC_SISP_H264
TODO:edit
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiTransformDequantChromaDC_SISP_H264_16s_C1I    ( Fw16s *pSrcDst, Fw16s *pDCPredict,
                                                                      Fw32s qp      , Fw32s qs, Fw32s Switch );
/*#FunctionBlock - TransformPrediction_H264
TODO:edit
#Technologies - REF, SSE2
#Short - TODO
#Long - TODO
#ReturnValues - TODO
*/
FwStatus STDCALL fwiTransformPrediction_H264_8u16s_C1             ( Fw8u *pSrc, Fw32s step, Fw16s *pDst );

/*#FunctionBlock - TransformDequantLumaDC_H264
TODO:publish
#Technologies - REF, SSE2
#Short - Inverse transform luma DC
#Long - <Text>This function steps through a source buffer, performs integer inverse transformation and dequantization of 4X4 luma DC coefficients in accordance with JVTG050 8.5.6., and writes the results back to the source buffer.</Text>
#ReturnValues - TODO
*/
FwStatus STDCALL fwiTransformDequantLumaDC_H264_16s_C1I           ( Fw16s *pSrcDst, Fw32s QP );

#endif // FW_BUILD_NUM


/*#FunctionBlock - QuantInv_MPEG2
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Inverse inter frame quantization (MPEG-2)
#Long - <Text>This function performs inverse quantization of inter frames in accordance with the MPEG-2 standard.</Text> 
<Text>The 8x8 matrix of DCT coefficients in <ParameterName>pSrcDst</ParameterName> are first multiplied by <ParameterName>Qp</ParameterName>, the quantizing scale factor, and then by 
the corresponding elements of the 8x8 <ParameterName>pQPMatrix</ParameterName>. The function also performs saturation and mismatch control in accordance with the MPEG-2 standard.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantInv_MPEG2_16s_C1I                        ( Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix );


/*#FunctionBlock - QuantInvIntra_MPEG2
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Inverse intra frame quantization (MPEG-2)
#Long - <Text>This function performs inverse quantization of intra frames in accordance with the MPEG-2 standard.</Text> 
<Text>The 8x8 matrix of DCT coefficients in <ParameterName>pSrcDst</ParameterName> are first multiplied by <ParameterName>Qp</ParameterName>, the quantizing scale factor, and then by 
the corresponding elements of the 8x8 <ParameterName>pQPMatrix</ParameterName>. The function also performs saturation and mismatch control in accordance with the MPEG-2 standard.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantInvIntra_MPEG2_16s_C1I                   ( Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix );


/*#FunctionBlock - ReconstructDCTBlock_MPEG1
TODO:publish
#Technologies - REF
#Short - Reconstruct inter frame DCT block (MPEG-1)
#Long - <Text>This function reconstructs an 8x8 DCT block for inter frames in accordance with the MPEG-1 standard.</Text>
<Text>The function first decodes 64 quantised DCT coefficients from the bitstream using the run-level tables of the MPEG-1 standard. 
The coefficients are then rearranged as an 8x8 block as specified by <ParameterName>pScanMatrix</ParameterName> and inverse quantization is performed.
The function also performs saturation and mismatch control in accordance with the MPEG-1 standard.</Text> 
<Text>The <ParameterName>pDCSizeTable</ParameterName> is used to decode the DC coefficient and the <ParameterName>pACTable</ParameterName> 
is used to decode the AC coefficients. The quantizing scale factor <ParameterName>Qp</ParameterName> and <ParameterName>pQPMatrix</ParameterName> are used to perform inverse quantization. 
The reconstructed 8x8 block of DCT coefficients is written to <ParamterName>pDstBlock</ParamterName>. Pointer <ParameterName>*pDstSize</ParamterName> indicates the position of the last non-zero coefficient. 
The pointers <ParamterName>ppBitStream</ParamterName> and <ParamterName>pOffset</ParamterName> are updated to the position in the bitstream where decoding of the next 8x8 block begins.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsH263VLCCodeErr
*/
FwStatus STDCALL fwiReconstructDCTBlock_MPEG1_32s                 (      Fw32u **ppBitStream, int *pOffset,
                                                                     const Fw32s *pDCSizeTable,
                                                                     const Fw32s *pACTable    ,
                                                                           Fw32s *pScanMatrix , int QP, Fw16s *pQPMatrix,
                                                                           Fw16s *pDstBlock   ,         Fw32s *pDstSize );


/*#FunctionBlock - ReconstructDCTBlockIntra_MPEG1
TODO:publish
#Technologies - REF
#Short - Reconstruct intra frame DCT block (MPEG-1)
#Long - <Text>This function reconstructs an 8x8 DCT block for intra frames in accordance with the MPEG-1 standard.</Text>
<Text>The function first decodes 64 quantised DCT coefficients from the bitstream using the run-level tables of the MPEG-1 standard. 
The coefficients are then rearranged as an 8x8 block as specified by <ParameterName>pScanMatrix</ParameterName> and inverse quantization is performed.
The function also performs saturation and mismatch control in accordance with the MPEG-1 standard.</Text> 
<Text>The <ParameterName>pDCSizeTable</ParameterName> is used to decode the DC coefficient and the <ParameterName>pACTable</ParameterName> 
is used to decode the AC coefficients. The quantizing scale factor <ParameterName>Qp</ParameterName> and <ParameterName>pQPMatrix</ParameterName> are used to perform inverse quantization. 
The reconstructed 8x8 block of DCT coefficients is written to <ParamterName>pDstBlock</ParamterName>. Pointer <ParameterName>*pDstSize</ParamterName> indicates the position of the last non-zero coefficient. 
The pointers <ParamterName>ppBitStream</ParamterName> and <ParamterName>pOffset</ParamterName> are updated to the position in the bitstream where decoding of the next 8x8 block begins.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsH263VLCCodeErr
*/

FwStatus STDCALL fwiReconstructDCTBlockIntra_MPEG1_32s            (      Fw32u **ppBitStream, int *pOffset,
                                                                     const Fw32s *pDCSizeTable,
                                                                     const Fw32s *pACTable    ,
                                                                           Fw32s *pScanMatrix , int QP, Fw16s *pQPMatrix,
                                                                           Fw16s *pDCPred     ,         Fw16s *pDstBlock, Fw32s *pDstSize );


/*#FunctionBlock - ReconstructDCTBlock_MPEG2
TODO:publish
#Technologies - REF
#Short - Reconstruct inter feame DCT block (MPEG-2)
#Long - <Text>This function reconstructs an 8x8 DCT block for inter frames in accordance with the MPEG-2 standard.</Text> 
<Text>The function first decodes 64 quantized DCT coefficients from the bitstream using the run-level tables of the MPEG-2 standard. 
The coefficients are then rearranged as an 8x8 block as specified by <ParameterName>pScanMatrix</ParameterName> and inverse quantization is performed.
The function also performs saturation and mismatch control in accordance with the MPEG-2 standard.</Text>
<Text>The <ParameterName>pDCTable</ParameterName> is used to decode the DC coefficientand the <ParameterName>pACTable</ParameterName> is used to decode the AC coefficients.
The quantizing scale factor <ParameterName>Qp</ParameterName> and <ParameterName>pQPMatrix</ParameterName> are used to perform inverse quantization. 
The reconstructed 8x8 block of DCT coefficients is written to <ParamterName>pDstBlock</ParamterName>. 
Pointer <ParameterName>*pDstSize</ParameterName> indicates the position of the last non-zero coefficient. 
The pointers <ParamterName>ppBitStream</ParamterName> and <ParamterName>pOffset</ParameterName> are updated to the position in the bitstream where decoding of the next 8x8 block begins.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsH263VLCCodeErr
*/
FwStatus STDCALL fwiReconstructDCTBlock_MPEG2_32s                 (       Fw32u **ppBitStream, int *pOffset,
                                                                      const FwVCHuffmanSpec_32s *pDCTable,
                                                                      const FwVCHuffmanSpec_32s *pACTable,
                                                                            Fw32s *pScanMatrix , int QP, Fw16s *pQPMatrix,
                                                                            Fw16s *pDstBlock   , Fw32s *pDstSize );


/*#FunctionBlock - ReconstructDCTBlockIntra_MPEG2
TODO:publish
#Technologies - REF
#Short - Reconstruct intra frame DCT block (MPEG-2)
#Long - <Text>This function reconstructs an 8x8 DCT block for intra frames in accordance with the MPEG-2 standard.</Text> 
<Text>The function first decodes 64 quantized DCT coefficients from the bitstream using the run-level tables of the MPEG-2 standard. 
The coefficients are then rearranged as an 8x8 block as specified by <ParameterName>pScanMatrix</ParameterName> and inverse quantization is performed.
The function also performs saturation and mismatch control in accordance with the MPEG-2 standard.</Text>
<Text>The <ParameterName>pDCTable</ParameterName> is used to decode the DC coefficientand the <ParameterName>pACTable</ParameterName> is used to decode the AC coefficients.
The quantizing scale factor <ParameterName>Qp</ParameterName> and <ParameterName>pQPMatrix</ParameterName> are used to perform inverse quantization. 
The reconstructed 8x8 block of DCT coefficients is written to <ParamterName>pDstBlock</ParamterName>. 
Pointer <ParameterName>*pDstSize</ParameterName> indicates the position of the last non-zero coefficient. 
The pointers <ParamterName>ppBitStream</ParamterName> and <ParamterName>pOffset</ParameterName> are updated to the position in the bitstream where decoding of the next 8x8 block begins.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsH263VLCCodeErr
*/

FwStatus STDCALL fwiReconstructDCTBlockIntra_MPEG2_32s            (       Fw32u **ppBitStream, int *pOffset,
                                                                      const FwVCHuffmanSpec_32s *pDCSizeTable,
                                                                      const FwVCHuffmanSpec_32s *pACTable    ,
                                                                            Fw32s *pScanMatrix , int QP, Fw16s *pQPMatrix, 
                                                                            Fw16s *pDCPred     , Fw32s shiftDCVal, 
                                                                            Fw16s *pDstBlock   , Fw32s *pDstSize );


/*#FunctionBlock - DCT8x8Inv_AANTransposed_Channel
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Perform AAN IDCT on an 8X8 block
#Long - <Text>These functions step through a transposed 8X8 block of DCT coefficients in a source buffer, perform an IDCT based on the AAN algorithm, and write the result to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCT8x8Inv_AANTransposed_16s_C1R               ( const Fw16s *pSrc, Fw16s *pDst, Fw32s dstStep, Fw32s count );
FwStatus STDCALL fwiDCT8x8Inv_AANTransposed_16s8u_C1R             ( const Fw16s *pSrc, Fw8u  *pDst, Fw32s dstStep, Fw32s count );

/*#FunctionBlock - DCT8x8Inv_AANTransposed_Plane
TODO:publish
#Technologies - REF, SSE2, SSE3
#Short - Perform AAN IDCT on 8X8 U and V blocks
#Long - <Text>These functions step through transposed 8X8 U and V block of DCT coefficients in two source buffers, perform an IDCT based on the AAN algorithm, and write the result to a joined UV block in a sestination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCT8x8Inv_AANTransposed_16s_P2C2R             ( const Fw16s *pSrcU, const Fw16s *pSrcV, Fw16s *pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV );
FwStatus STDCALL fwiDCT8x8Inv_AANTransposed_16s8u_P2C2R           ( const Fw16s *pSrcU, const Fw16s *pSrcV, Fw8u  *pDstUV, Fw32s dstStep, Fw32s countU, Fw32s countV );

#ifdef __cplusplus
}
#endif


#endif // __FWVIDEO_H__
