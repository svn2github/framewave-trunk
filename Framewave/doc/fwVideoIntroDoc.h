/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*#DataBlock

#Parameters

EdgePelCount - Specifies the minimum number of pairs of elements that differ by more than EdgePelDifference.
EdgePelDifference - Specifies the edge difference threshold between neighboring elements.
QP - Quantizer scale factor read from the bitstream.
count - Number of the last non-zero coefficient in zig-zag order. When a block contains no non-zero coefficients, the value is -1.
countU - Number of the last non-zero U coefficient in zig-zag order. When a block contains no non-zero coefficients, the value is -1.
countV - Number of the last non-zero V coefficient in zig-zag order. When a block contains no non-zero coefficients, the value is -1.
dstDiffStep - Destination block step size (width of the block in bytes).
dstPredictorStep - Destination predictor block step size (width of the block in bytes).
dstStep - Destination buffer step size (width of the buffer in bytes).
dstStep - Step of the destination array.
mcType - Specifies the type of prediction used for motion compensation.
mcTypeB - Specifies the type of prediction used for backward frame motion compensation.
mcTypeF - Specifies the type of prediction used for forward frame motion compensation.
pACTable - Pointer to a table containing run-level codes for all DCT coefficients other than the first one.
pDCPred - Pointer to a value to be added to the DC coefficient. The value is read from the standard table.
pDCSizeTable - Pointer to a table containing codes for the DC coefficient (the first of the DCT coefficients).
pDCTable - Pointer to a table containing codes for the DC coefficient (the first of the DCT coefficients).
pDecodeTable - Pointer to a decoding table.
pDiff - Pointer to an array of size 16 that contains the sums of 4X4 difference block coefficients.
pDst - Pointer to a location in a destination buffer, a location in a destination array or to a destination result.
pDstBlock - Pointer to a block of decoded elements.
pDstDiff - Pointer to a 16X16 block in the destination plane which contains differences between current and reference blocks.
pDstPredictor - Pointer to a destination predictor block.
pDstSAD - Pointer to a destination array of size 4 that stores SAD values.
pDstSize - Pointer to the position of the last non-zero block coefficient in scanning sequence.
pDstUV - Pointer to a UV destination array.
pFirst - Pointer to the first destination value.
pMean - Pointer to a computed mean of pixel values.
pOffset - Pointer to the offset between the bit that ppBitStream points to and the start of the code.
pPred - Pointer to a 16x16 predictor block in the reference plane.
pQPMatrix - Pointer to a standard or user-defined weighting matrix.
pRef - Pointer to a prefetch buffer which contains previously decoded bits.
pRefB - Pointer to a backward-reference block of specified size.
pRefF - Pointer to a forward-reference block of specified size. 
pRes - Pointer to a result value.
pSAD - Pointer to an SAD result.
pScanMatrix - Pointer to a matrix containing indices of elements in a scanning sequence.
pSecond - Pointer to a second destination value.
pSqrDiff - Pointer to a sum of square differences between the current and reference blocks.
pSrc - Pointer to a location in a source buffer.
pSrc - Pointer to a block of DCT coefficients.
pSrc1 - Pointer to a location in source buffer one.
pSrc2 - Pointer to a location in source buffer two.
pSrcCur - Pointer to a block in the current plane.
pSrcDst - Pointer to a location in a buffer that contains both the source and destination. 
pSrcDst - Pointer to the start of the block.
pSrcRef - Pointer to a block in the reference plane.
pSrcRefB - Pointer to a backward block in the reference plane. 
pSrcRefF - Pointer to a forward block in the reference plane. 
pSrcSum - Pointer to a sum of pixel values for the current block.
pSrcTable - Pointer to a source table.
pSrcU - Pointer to a block of DCT coefficients for U component.
pSrcV - Pointer to a block of DCT coefficients for V component.
pSrcYData - Pointer to a block of inverse DCT output data.
pSums - Pointer to an array of size 256 that contains a sequence of 4X4 residual blocks.
pVar - Pointer to a variance value.
ppBitStream - Double pointer to the current position in the bit stream.
ppDecodeTable - Double pointer to a decoding table.
ppDstSpec - Double pointer to a destination decoding table.
predStep - Reference plane step size (in bytes).
refStep - Reference block step size (width of the block in bytes).
refStepB - Backward reference frame reference block step size (width of the block in bytes).
refStepF - Forward reference frame reference block step size (width of the block in bytes).
roiSize - Specifies the height and width of an ROI.
roundControl - Specifies the type of rounding used for half-pixel approximation.
shiftDCVal - Integer value. The DC coefficient must be multiplied by 2shiftDCVal.
srcCurStep - Source current block step size (width of the block in bytes)
srcDstStep - Source and destination buffer step size in bytes (width of both buffers in bytes).
srcRefStep - Source reference block step size (width of the block in bytes)
srcRefStepB - Source backward reference block step size (width of the block in bytes)
srcRefStepF - Source forward reference block step size (width of the block in bytes)
srcStep - Source buffer step size (width of the buffer in bytes).
srcStepB - Source-aligned backward reference frame step size (width in bytes).
srcStepF - Source-aligned forward reference frame step size (width in bytes).
srcYDataStep - Source-aligned DCT output data block step size (width of the block in bytes). 

AC - AC compoment
ChromaQP - Chroma quantizer, must be within the range [0;39].
DstStep - Size of a row in bytes, the aligned destination frame width.
PredStep - Reference plane step size (in bytes).
StartPtr - Pointer to frame source data.
Switch - Flag that indicates picture switch (non-zero when a switch occurs, equal to zero otherwise).
availability - Flag that indicates availability of the samples used for prediction.
cbp4x2 - Coded block pattern. If cbp4x2 & (1<<(1+i))is not equal to 0 (0 < i < 8), i-th 4x4 AC. Luma block is not zero-filled and it exists in ppSrcCoeff.
cbp4x4 - Coded block pattern. If cbp4x4 & (1<<(1+i))is not equal to 0 (0 < i < 16), i-th 4x4 AC. Luma block is not zero-filled and it exists in ppSrcCoeff.
dx - Fractional part of the x-coordinate in a source image.
dy - Fractional part of the y-coordinate in a source image.
edgeType - Flag that indicates the availability of the macroblocks used for 16X16 intra-prediction. When the upper macroblock is not available, edgeType&D_TOP_EDGE are non-zero. When the left macroblock is not available, edgeType&D_LEFT_EDGE are non-zero. When the upper-left macroblock is not available, edgeType&D_TOP_LEFT_EDGE are non-zero.
edgeTypeBottom - Flag that indicates the availability of the upper half of a macroblock used for prediction. The upper and lower halves of a macroblock may have different prediction vectors. There is one flag for each half.
edgeTypeTop - Flag that indicates the availability of the lower half of a macroblock  used for prediction. The upper and lower halves of a macroblock may have different prediction vectors. There is one flag for each half.
edgeType - Specifies a vertical or horizontal edge.
height - Specifies the height of a block.
intraChromaMode - Specifies the operating mode of the intrachroma prediction process.
isSigned - Flag that indicates whether an output value must be decoded as signed.
nAlpha - A calculated deblock filter value.
nBeta - A calculated deblock filter value.
offset - Specifies an offset.
outPixels - The number of pixels by which the data specified by pSrc reaches over the frame top boundary.
pAlpha - Pointer to an array of size 2 of alpha thresholds (values for external and internal vertical edge).
pBS - Pointer to an array of size 16 of BS parameters (values for the left edge of each 4x4 block).
pBeta - Pointer to an array of size 2 of beta thresholds (values for external and internal vertical edge).
pBitOffset - Pointer to a bit position within the byte that ppBitStream points to. Valid within the range 0 to 7. The pointer is updated after motion vector decoding.
pDC - Pointer to an output coefficient.
pDCPredict - Pointer to an array (size 4) of the of inter-prediction DC samples for the current macroblock after inverse transform. Inverse transform of the inter prediction samples can be calculated using TransformPrediction_H264 function for each 4x4 block.
pMBIntraTypes - Pointer to an array of Intra_4x4 luma prediction modes for eight subblocks. pMBIntraTypes[i] is defined in the same way as in PredictIntra_4x4_H264 function (0 < i < 8 ).
pNumCoeff - Pointer to the output number of non-zero coefficients.
pPredictBlock - Pointer to array (size 16) of inter-prediction samples for the current macroblock after inverse transform. Samples are calculated using the TransformPrediction_H264 function.
pSrc2Dst - Pointer to the second source and result.
pSrcDstUPlane - Pointer to a macroblock that is reconstructed in the current U plane. The macroblock must contain inter-prediction samples.
pSrcDstVPlane - Pointer to a macroblock that is reconstructed in the current V plane. The macroblock must contain inter-prediction samples.
pSrcDstYPlane - Pointer to the current macroblock that is reconstructed in current Y-plane. This macroblock must contain inter prediction samples.
pTblCoeffToken - Pointer to a table of coefficient tokens.
pThresholds - Pointer to an array of size 16 of threshold (Tc0) values for the left edge of each 4x4 block.
pitch - Specifies memory pitch.
ppDstCoeffs - Double pointer to a 4x4 block of coefficients calculated by the function. The function shifts pointer *ppDstCoeffs on 16.
ppSrcCoeff - Double pointer to the order of 4x4 blocks of residual coefficients for a macroblock, the results of Huffman decoding (2x2 DC U-block, 2x2 DC V-block, 4x4 AC U-blocks, 4x4 AC V-blocks if the block is not zero-filled). The pointer is updated by the function and points to the blocks for the next macroblock.
ppTblCoeffToken - Double pointer to a CoeffToken table.
ppTblRunBefore - Double pointer to a RunBefore table.
ppTblTotalZeros - Double pointer to a TotalZeros table.
ppTblTotalZerosCR - Double pointer to a chroma DC TotalZeros table.
predMode - Specifies the prediction mode of the Intra_4x4 prediction process for luma samples.
qp - Quantization parameter (QPY in [JVTG050]). It must be within the range [0;51].
qs - Quantization parameter qs.
shift - Specifies shift size.
srcDstUVStep - Plane step size.
srcDstYStep - Y-plane step size.
step - Plane step
uFrameFieldFlag - Flag that indicates an expansion method for various image types: frame, top field, bottom field.
uFrameHeight - Frame height in pixels.
uFrameWidth - Frame width in pixels.
uMaxNumCoeff - Maximum number of coefficients in a block (16 for Intra 16x16, 15 for others).
uPels - Number of pixels filled to the right/left and up/down in process of expansion.
uPitch - Frame width in bytes.
uVLCSelect - Predictor on number of CoeffToken table.
weight1 - Weighting factor.
weight2 - Weighting factor.
width - Block width.
cubeDimensions[3] - dimensions of the 3D-LUT cube in x, y and z directions .(eg. (17,17,17), (33,33,33) etc)
cubeMax[3] - The maximum values that can occur in 3D-LUT cube in x, y and z directions.(eg. (1023,1023,1023),4095,4095,4095) for 10 bit and 12 bit outputs respectively)
pCube - Pointer to the 3d_LUT cube. The pixel values in the table should be filled along z-axis first, then along y-axis and finally along x-axis
pLUTSpec - pointer to the FwiLUTSpec structure
ppLUTSpec - pointer to a pointer to FwiLUTSpec structure

#DataStructures

FwiLUTSpec - Internal data structure for 3D-LUT table.

#Enumerators

fwintraChromaPredMode_H264 - Enumerates prediction modes for the Intra prediction process of chroma compoments.
fwintra16x16PredMode_H264 - Enumerates prediction modes for the Intra_16x16 prediction process of luma compoments.
fwintra4x4PredMode_H264 - Enumerates prediction modes for the Intra_4x4 prediction process of luma compoments.
FwvcFrameFieldFlag - Enumerates image type of the picture.

*/

/*#Documentation

<SectionHeading>Video Library</SectionHeading>

<Paragraph>Video Coding Library<SectDesc> functions perform video manipulation, encoding and decoding.</SectDesc></Paragraph>

<Paragraph>This section is organized as follows.</Paragraph>

<SectLinks></SectLinks>

<Paragraph>Within the section, the <Bold>Basic Concepts</Bold> chapter provides overview information related to the functions in the library, 
and subsequent chapters provide detailed descriptions of library functions that perform operations of the same kind.</Paragraph>
<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 
<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description.</Paragraph>

<RefLink></RefLink>

<Chapter>
<ChapterHeading>Basic Concepts</ChapterHeading>

<Paragraph>This <ChaptDesc> chapter provides an overview of the information contained in the functional descriptions.</ChaptDesc></Paragraph>

<Paragraph>Video Coding library functional descriptions include the following types of information.</Paragraph>

<Heading2>
<H2Name>Enumerators</H2Name>
<H2Link>video_enumerators</H2Link>
</Heading2>

<Paragraph>Video Coding library function definitions use the following enumerators.</Paragraph>

#PrintAllEnumerators

<Heading2>
<H2Name>Parameter Glossary</H2Name>
<H2Link>video_parameters</H2Link>
</Heading2>

<Paragraph>Video Coding library function definitions use the following parameters.</Paragraph>

#PrintAllParameters

</Chapter>

*/
