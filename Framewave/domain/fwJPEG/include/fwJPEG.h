/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FWJPEG_H__
#define __FWJPEG_H__

#include "fwBase.h"

//Data Structure define model
typedef struct EncodeHuffmanSpec  FwiEncodeHuffmanSpec;
typedef struct EncodeHuffmanState FwiEncodeHuffmanState;
typedef struct DecodeHuffmanSpec  FwiDecodeHuffmanSpec;
typedef struct DecodeHuffmanState FwiDecodeHuffmanState;

#ifdef __cplusplus
extern "C" {
#endif


/*#HintBlock
AutoOff
*/

/*#IncludeBlock
#includeGlobal "../../../doc/fwIntroDoc.h"
#include "../../../doc/fwJPEGIntroDoc.h"
*/

#if BUILD_NUM_AT_LEAST( 100 )

/*#Documentation

<Chapter>
<ChapterHeading>Library Version</ChapterHeading>
<Paragraph>This chapter describes the FW function that provides library version information.</Paragraph>
*/

/*#FunctionBlock - GetLibVersion
TODO:Publish
#Technologies - REF
#Short - Get library version
#Long - <Text>This function returns a pointer to the <Bold>FwLibraryVersion</Bold> structure that contains FW Library version information.</Text>
*/

const FwLibraryVersion* STDCALL fwjGetLibVersion ();

/*#Documentation
</Chapter>
*/

#endif

#if BUILD_NUM_AT_LEAST(102)

/*#Documentation

<Chapter>
<ChapterHeading>Image Compression Functions</ChapterHeading>
<Paragraph>This chapter describes functions that are used in JPEG image compression.</Paragraph>
*/

/*#FunctionBlock - RGBToY_JPEG
TODO:publish
#Technologies - REF
#Short - Convert RGB to grayscale
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from RGB to grayscale luminance values, and write the converted data to a destination buffer.</Text>
<Text>The following formula is used to convert red, green, and blue values to luminance values.</Text>
<Pseudo> Y = 0.299*R + 0.587*G + 0.114*B</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiRGBToY_JPEG_8u_P3C1R              ( const Fw8u *pSrcRGB[3], int srcStep,
                                                                Fw8u *pDstY     , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiRGBToY_JPEG_8u_C3C1R              ( const Fw8u *pSrcRGB   , int srcStep,
                                                                Fw8u *pDstY     , int dstStep, FwiSize roiSize );

/*#FunctionBlock - BGRToY_JPEG
TODO:publish
#Technologies - REF
#Short - Convert BGR to grayscale (JPEG)
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from BGR to grayscale luminance values, and writes the converted data to a destination buffer.</Text>
<Text>The following formula is used to convert blue, green, and red values to luminance values.</Text>
<Pseudo> Y = 0.114*B + 0.587*G + 0.299*R</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiBGRToY_JPEG_8u_C3C1R              ( const Fw8u *pSrcBGR, int srcStep,
                                                                Fw8u *pDstY  , int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToYCbCr_JPEG
TODO:publish
#Technologies - REF
#Short - Convert RGB to YCbCr (JPEG)
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the YCbCr color space, and write the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiRGBToYCbCr_JPEG_8u_P3R            ( const Fw8u *pSrcRGB[3]  , int srcStep,
                                                                Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiRGBToYCbCr_JPEG_8u_C3P3R          ( const Fw8u *pSrcRGB     , int srcStep,
                                                                Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCrToRGB_JPEG
TODO:publish
#Technologies - REF
#Short - Convert YCbCr to RGB (JPEG)
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to red, green, and blue values.</Text>
<Pseudo>
 R = Y + 1.402*Cr - 179.456
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 B = Y + 1.772*Cb - 226.816
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCrToRGB_JPEG_8u_P3R            ( const Fw8u *pSrcYCbCr[3], int srcStep,
                                                                Fw8u *pDstRGB[3]  , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiYCbCrToRGB_JPEG_8u_P3C3R          ( const Fw8u *pSrcYCbCr[3], int srcStep,
                                                                Fw8u *pDstRGB     , int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGB5X5ToYCbCr_JPEG
TODO:publish
#Technologies - REF
#Short - Convert 16-bit RGB to YCbCr (JPEG)
#Long - <Text>These functions step through an ROI in a source buffer, convert 16-bit source data from the RGB color model to the YCbCr color space, and write the converted data to a destination buffer.</Text>
<Text>There are separate functions for 555 and 565 bit-per-channel RGB source encoding.</Text>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiRGB565ToYCbCr_JPEG_16u8u_C3P3R    ( const Fw16u *pSrcRGB     , int srcStep,
                                                                Fw8u  *pDstYCbCr[3], int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiRGB555ToYCbCr_JPEG_16u8u_C3P3R    ( const Fw16u *pSrcRGB     , int srcStep,
                                                                Fw8u  *pDstYCbCr[3], int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCrToRGB5X5_JPEG
TODO:publish
#Technologies - REF
#Short - Convert YCbCr to 16-bit RGB (JPEG)
#Long - <Text>These function step through an ROI in a source buffer, convert source data from the YCbCr color space to the 16-bit RGB color model, and write the converted data to a destination buffer.</Text>
<Text>There are separate functions for 555 and 565 bit-per-channel RGB destination encoding.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to red, green, and blue values.</Text>
<Pseudo>
 R = Y + 1.402*Cr - 179.456
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 B = Y + 1.772*Cb - 226.816
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCrToRGB565_JPEG_8u16u_P3C3R    ( const Fw8u  *pSrcYCbCr[3], int srcStep,
                                                                Fw16u *pDstRGB     , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiYCbCrToRGB555_JPEG_8u16u_P3C3R    ( const Fw8u  *pSrcYCbCr[3], int srcStep,
                                                                Fw16u *pDstRGB     , int dstStep, FwiSize roiSize );

/*#FunctionBlock - BGRToYCbCr_JPEG
TODO:publish
#Technologies - REF
#Short - Convert BGR to YCbCr (JPEG)
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the BGR color model to the YCbCr color space, and writes the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert blue, green, and red values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.114*B + 0.587*G + 0.299*R
 Cb = 0.5*B + 128 - 0.33126*G - 0.16874*R
 Cr = - 0.08131*B + 128 - 0.41869*G + 0.5*R
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiBGRToYCbCr_JPEG_8u_C3P3R          ( const Fw8u *pSrcBGR     , int srcStep,
                                                                Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCrToBGR_JPEG
TODO:publish
#Technologies - REF
#Short - Convert YCbCr to BGR (JPEG)
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YCbCr color space to the BGR color model, and writes the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to blue, green, and red values.</Text>
<Pseudo>
 B = Y + 1.772*Cb - 226.816
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 R = Y + 1.402*Cr - 179.456
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCrToBGR_JPEG_8u_P3C3R          ( const Fw8u *pSrcYCbCr[3], int srcStep,
                                                                Fw8u *pDstBGR     , int dstStep, FwiSize roiSize );

/*#FunctionBlock - BGR5X5ToYCbCr_JPEG
TODO:publish
#Technologies - REF
#Short - Convert 16-bit BGR to YCbCr (JPEG)
#Long - <Text>These functions step through an ROI in a source buffer, convert 16-bit source data from the BGR color model to the YCbCr color space, and write the converted data to a destination buffer.</Text>
<Text>There are separate functions for 555 and 565 bit-per-channel BGR source encoding.</Text>
<Text>The following formulas are used to convert blue, green, and red values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.114*B + 0.587*G + 0.299*R
 Cb = 0.5*B + 128 - 0.33126*G - 0.16874*R
 Cr = - 0.08131*B + 128 - 0.41869*G + 0.5*R
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiBGR565ToYCbCr_JPEG_16u8u_C3P3R    ( const Fw16u *pSrcBGR     , int srcStep,
                                                                Fw8u  *pDstYCbCr[3], int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiBGR555ToYCbCr_JPEG_16u8u_C3P3R    ( const Fw16u *pSrcBGR     , int srcStep,
                                                                Fw8u  *pDstYCbCr[3], int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCrToBGR5X5_JPEG
TODO:publish
#Technologies - REF
#Short - Convert YCbCr to 16-bit BGR (JPEG)
#Long - <Text>These function step through an ROI in a source buffer, convert source data from the YCbCr color space to the 16-bit BGR color model, and write the converted data to a destination buffer.</Text>
<Text>There are separate functions for 555 and 565 bit-per-channel BGR destination encoding.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to blue, green, and red values.</Text>
<Pseudo>
 B = Y + 1.772*Cb - 226.816
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 R = Y + 1.402*Cr - 179.456
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCrToBGR565_JPEG_8u16u_P3C3R    ( const Fw8u  *pSrcYCbCr[3], int srcStep,
                                                                Fw16u *pDstBGR     , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiYCbCrToBGR555_JPEG_8u16u_P3C3R    ( const Fw8u  *pSrcYCbCr[3], int srcStep,
                                                                Fw16u *pDstBGR     , int dstStep, FwiSize roiSize );

/*#FunctionBlock - CMYKToYCCK_JPEG
TODO:publish
#Technologies - REF
#Short - Convert CMYK to YCCK (JPEG)
#Long - <Text>These function step through an ROI in a source buffer, convert source data from the CMYK color model to the YCCK color space, and write the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert cyan, magenta, and yellow values to red, green, and blue values.</Text>
<Pseudo>
 R = 255 - C
 G = 255 - M
 B = 255 - Y
</Pseudo>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiCMYKToYCCK_JPEG_8u_P4R           ( const Fw8u *pSrcCMYK[4], int srcStep,
                                                                Fw8u *pDstYCCK[4], int dstStep, FwiSize roiSize );

FwStatus STDCALL  fwiCMYKToYCCK_JPEG_8u_C4P4R         ( const Fw8u *pSrcCMYK   , int srcStep,
                                                                Fw8u *pDstYCCK[4], int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCCKToCMYK_JPEG
TODO:publish
#Technologies - REF
#Short - Convert YCCK to CMYK (JPEG)
#Long - <Text>These function step through an ROI in a source buffer, convert source data from the YCCK color space to the CMYK color model, and write the converted data to a destination buffer.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to red, green, and blue values.</Text>
<Pseudo>
 R = Y + 1.402*Cr - 179.456
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 B = Y + 1.772*Cb - 226.816
</Pseudo>
<Text>The following formulas are used to convert red, green, and blue values to cyan, magenta, and yellow values.</Text>
<Pseudo>
 C = 255 -R
 M = 255 -G
 Y = 255 -B
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiYCCKToCMYK_JPEG_8u_P4R           ( const Fw8u *pSrcYCCK[4], int srcStep,
                                                                Fw8u *pDstCMYK[4], int dstStep, FwiSize roiSize );

FwStatus STDCALL  fwiYCCKToCMYK_JPEG_8u_P4C4R         ( const Fw8u *pSrcYCCK[4], int srcStep,
                                                                Fw8u *pDstCMYK   , int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToYCbCr444LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert RGB data block to 444 YCbCr MCU
#Long - <Text>This function steps through an 8X8 data block in a source buffer, converts the source data from the RGB color model to the YCbCr color space, level-shifts the unsigned values to signed values, creates a 444 sampled minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiRGBToYCbCr444LS_MCU_8u16s_C3P3R  ( const Fw8u  *pSrcRGB, int srcStep,
                                                                Fw16s *pDstMCU[3] );

/*#FunctionBlock - RGBToYCbCr422LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert RGB data block to 4:2:2 YCbCr MCU
#Long - <Text>This function steps through a 16X8 data block in a source buffer, converts the source data from the RGB color model to the YCbCr color space with 4:2:2 chroma sub-sampling, level-shifts the unsigned values to signed values, creates a minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiRGBToYCbCr422LS_MCU_8u16s_C3P3R  ( const Fw8u  *pSrcRGB, int srcStep,
                                                                Fw16s *pDstMCU[3] );

/*#FunctionBlock - RGBToYCbCr411LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert RGB data block to 4:1:1 YCbCr MCU
#Long - <Text>This function steps through a 16X16 data block in a source buffer, converts the source data from the RGB color model to the YCbCr color space with 4:1:1 chroma sub-sampling, level-shifts the unsigned values to signed values, creates a minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiRGBToYCbCr411LS_MCU_8u16s_C3P3R  ( const Fw8u  *pSrcRGB, int srcStep,
                                                                Fw16s *pDstMCU[3] );

/*#FunctionBlock - BGRToYCbCr444LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert BGR data block to 4:4:4 YCbCr MCU
#Long - <Text>These functions step through an 8X8 data block in a source buffer, convert the source data from the BGR color model to the YCbCr color space with 4:4:4 chroma sub-sampling, level-shift the unsigned values to signed values, create a minimum coded unit, and write the data to a destination buffer.</Text>
<Text>The following formulas are used to convert blue, green, and red values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.114*B + 0.587*G + 0.299*R
 Cb = 0.5*B + 128 - 0.33126*G - 0.16874*R
 Cr = - 0.08131*B + 128 - 0.41869*G + 0.5*R
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiBGRToYCbCr444LS_MCU_8u16s_C3P3R      ( const Fw8u  *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR565ToYCbCr444LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR555ToYCbCr444LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

/*#FunctionBlock - BGRToYCbCr422LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert BGR data block to 4:2:2 YCbCr MCU
#Long - <Text>These functions step through a 16X8 data block in a source buffer, convert the source data from the BGR color model to the YCbCr color space with 4:2:2 chroma sub-sampling, level-shift the unsigned values to signed values, create a minimum coded unit, and write the data to a destination buffer.</Text>
<Text>The following formulas are used to convert blue, green, and red values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.114*B + 0.587*G + 0.299*R
 Cb = 0.5*B + 128 - 0.33126*G - 0.16874*R
 Cr = - 0.08131*B + 128 - 0.41869*G + 0.5*R
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiBGRToYCbCr422LS_MCU_8u16s_C3P3R      ( const Fw8u  *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR565ToYCbCr422LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR555ToYCbCr422LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );


/*#FunctionBlock - BGRToYCbCr411LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert BGR data block to 4:1:1 YCbCr MCU
#Long - <Text>These functions step through a 16X16 data block in a source buffer, convert the source data from the BGR color model to the YCbCr color space with 4:1:1 chroma sub-sampling, level-shift the unsigned values to signed values, create a minimum coded unit, and write the data to a destination buffer.</Text>
<Text>The following formulas are used to convert blue, green, and red values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.114*B + 0.587*G + 0.299*R
 Cb = 0.5*B + 128 - 0.33126*G - 0.16874*R
 Cr = - 0.08131*B + 128 - 0.41869*G + 0.5*R
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiBGRToYCbCr411LS_MCU_8u16s_C3P3R      ( const Fw8u  *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR565ToYCbCr411LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL  fwiBGR555ToYCbCr411LS_MCU_16u16s_C3P3R  ( const Fw16u *pSrcBGR, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

/*#FunctionBlock - CMYKToYCCK444LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert CMYK data block to 4:4:4 YCCK MCU
#Long - <Text>This function steps through an 8X8 data block in a source buffer, converts the source data from the CMYK color model to the YCCK color space with 4:4:4 chroma sub-sampling, level-shifts the unsigned values to signed values, creates a minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert cyan, magenta, and yellow values to red, green, and blue values.</Text>
<Pseudo>
 R = 255 -C
 G = 255 -M
 B = 255 -Y
</Pseudo>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiCMYKToYCCK444LS_MCU_8u16s_C4P4R       ( const Fw8u  *pSrcCMYK, int srcStep,
                                                                    Fw16s *pDstMCU[4] );

/*#FunctionBlock - CMYKToYCCK422LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert CMYK data block to 4:2:2 YCCK MCU
#Long - <Text>This function steps through a 16X8 data block in a source buffer, converts the source data from the CMYK color model to the YCCK color space with 4:2: chroma sub-sampling, level-shifts the unsigned values to signed values, creates a minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert cyan, magenta, and yellow values to red, green, and blue values.</Text>
<Pseudo>
 R = 255 -C
 G = 255 -M
 B = 255 -Y
</Pseudo>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiCMYKToYCCK422LS_MCU_8u16s_C4P4R       ( const Fw8u  *pSrcCMYK, int srcStep,
                                                                    Fw16s *pDstMCU[4] );

/*#FunctionBlock - CMYKToYCCK411LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert CMYK data block to 4:1:1 YCCK MCU
#Long - <Text>This function steps through a 16X16 data block in a source buffer, converts the source data from the CMYK color model to the YCCK color space with 4:1:1 chroma sub-sampling, level-shifts the unsigned values to signed values, creates a minimum coded unit, and writes the data to a destination buffer.</Text>
<Text>The following formulas are used to convert cyan, magenta, and yellow values to red, green, and blue values.</Text>
<Pseudo>
 R = 255 -C
 G = 255 -M
 B = 255 -Y
</Pseudo>
<Text>The following formulas are used to convert red, green, and blue values to luminance and chrominance values.</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
 Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiCMYKToYCCK411LS_MCU_8u16s_C4P4R       ( const Fw8u  *pSrcCMYK, int srcStep,
                                                                    Fw16s *pDstMCU[4] );

/*#FunctionBlock - YCbCr4XXToRGBLS_MCU
TODO:publish
#Technologies - REF
#Short - Convert YCbCr MCU to RGB
#Long - <Text>These functions step through a minimum coded unit in a source buffer, convert the data from the YCbCr color space to the RGB color model, level-shift the signed values to unsigned 8-bit values, and write the converted data to a destination buffer.</Text>
<Text>There are functions to convert minimum coded units with 4:4:4, 4:2:2, and 4:1:1 chroma sub-sampling.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to red, green, and blue values.</Text>
<Pseudo>
 R = Y + 1.402*Cr - 179.456
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 B = Y + 1.772*Cb - 226.816
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCr444ToRGBLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u *pDstRGB, int dstStep );

FwStatus STDCALL fwiYCbCr422ToRGBLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u *pDstRGB, int dstStep );

FwStatus STDCALL fwiYCbCr411ToRGBLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                     Fw8u *pDstRGB, int dstStep );

/*#FunctionBlock - YCbCr4XXLS_MCUToBGR
TODO:publish
#Technologies - REF
#Short - Convert YCbCr MCU to BGR
#Long - <Text>These functions step through a minimum coded unit in a source buffer, convert the data from the YCbCr color space to the BGR color model, level-shift the signed values to unsigned values, and write the converted data to a destination buffer.</Text>
<Text>There are functions to convert minimum coded units with 4:4:4, 4:2:2, and 4:1:1 chroma sampling to 8-bit BGR or to 16-bit BGR with 555 or 565 bit-per-channel encoding.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to blue, green, and red values.</Text>
<Pseudo>
 B = Y + 1.772*Cb - 226.816
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 R = Y + 1.402*Cr - 179.456
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCbCr444ToBGRLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr444ToBGR565LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr444ToBGR555LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr422ToBGRLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr422ToBGR565LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr422ToBGR555LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr411ToBGRLS_MCU_16s8u_P3C3R       ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr411ToBGR565LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

FwStatus STDCALL fwiYCbCr411ToBGR555LS_MCU_16s16u_P3C3R   ( const Fw16s *pSrcMCU[3],
                                                                    Fw16u *pDstBGR   , int dstStep );

/*#FunctionBlock - YCCK4XXToCMYKLS_MCU
TODO:publish
#Technologies - REF
#Short - Convert YCCK MCU to CMYK
#Long - <Text>These functions step through a minimum coded unit in a source buffer, convert the data from the YCCK color space to the CMYK color model, level-shift the signed values to 8-bit unsigned values, and write the converted data to a destination buffer.</Text>
<Text>There are functions to convert minimum coded units with 4:4:4, 4:2:2, and 4:1:1 sampling.</Text>
<Text>The following formulas are used to convert luminance and chrominance values to red, green, and blue values.</Text>
<Pseudo>
 R = Y + 1.402*Cr - 179.456
 G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
 B = Y + 1.772*Cb - 226.816
</Pseudo>

<Text>The following formulas are used to convert red, green, and blue values to cyan, magenta, and yellow values.</Text>
<Pseudo>
 C = 255 - R
 M = 255 - G
 Y = 255 - B
</Pseudo>
<Text>The black (K) channel value remains the same during conversion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiYCCK444ToCMYKLS_MCU_16s8u_P4C4R       ( const Fw16s *pSrcMCU[4],
                                                                    Fw8u  *pDstCMYK  , int dstStep );

FwStatus STDCALL fwiYCCK422ToCMYKLS_MCU_16s8u_P4C4R       ( const Fw16s *pSrcMCU[4],
                                                                    Fw8u  *pDstCMYK  , int dstStep );

FwStatus STDCALL fwiYCCK411ToCMYKLS_MCU_16s8u_P4C4R       ( const Fw16s *pSrcMCU[4],
                                                                    Fw8u  *pDstCMYK  , int dstStep );

/*#FunctionBlock - QuantFwdRawTableInit_JPEG
TODO:publish
#Technologies - REF
#Short - Apply quality factor to the raw quantization table
#Long - <Text>This function applies a specified quality factor value between 1 and 100 to the initial raw quantization table.</Text>
<Text>If a factor value less than or equal to 0 is specified, a value of 1 is used; if a value equal to or greater than 100 is specified, a value of 100 is used.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantFwdRawTableInit_JPEG_8u          ( Fw8u *pQuantRawTable, int qualityFactor );

/*#FunctionBlock - QuantFwdTableInit_JPEG
TODO:publish
#Technologies - REF
#Short - Change quantization table order for fast encode
#Long - <Text>This function reads the raw quantization table in zigzag order, scales the values by 15 bits, and writes a new table in left-to-right, top-to-bottom order.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantFwdTableInit_JPEG_8u16u          ( const Fw8u  *pQuantRawTable, Fw16u *pQuantFwdTable );

/*#FunctionBlock - QuantFwd8X8_JPEG
TODO:publish
#Technologies - REF
#Short - Quantize data block
#Long - <Text>This function reads the FDCT output coefficients of an 8X8 data block, quantizes the values, and writes the quantized values back to the same location.</Text>
<Text>Quantization is performed using the following formula.</Text>
<Pseudo> sq[vu] = round (s[vu]/Q[vu])</Pseudo>

<Text>Nearest integer rounding is used.</Text>
<Text>The <ParameterName>pQuantFwdTable</ParameterName> pointer is generated by the <FunctionName>fwiQuantFwdTableInit_JPEG_8u16u</FunctionName> function, so the final result for sq[vu] is shifted 15 bits.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantFwd8x8_JPEG_16s_C1I              ( Fw16s *pSrcDst, const Fw16u *pQuantFwdTable );

/*#FunctionBlock - QuantInvTableInit_JPEG
TODO:publish
#Technologies - REF
#Short - Change quantization table order for fast decode
#Long - <Text>This function reads the raw quantization table in zigzag order, scales the values by 15 bits, and writes a new table in left-to-right, top-to-bottom order.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantInvTableInit_JPEG_8u16u          ( const Fw8u *pQuantRawTable, Fw16u *pQuantInvTable );

/*#FunctionBlock - QuantInv8X8_JPEG
TODO:publish
#Technologies - REF
#Short - Dequantize data block
#Long - <Text>This function reads a block of uncompressed image data, dequantizes the values, and writes an 8X8 data block of DCT coefficients to the same location.</Text>
<Text>Dequantization is performed using the following formula.</Text>
<Pseudo> s[vu] = (sq[vu]*Q[vu])</Pseudo>

<Text>Because quantization is lossy, some of the data may be saturated and require up-sampling.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiQuantInv8x8_JPEG_16s_C1I              ( Fw16s *pSrcDst, const Fw16u *pQuantInvTable );

/*#FunctionBlock - DCTQuantFwd8X8_JPEG
TODO:publish
#Technologies - REF, SSE2
#Short - Transform and quantize
#Long - <Text>These functions read an 8X8 data block of 16-bit signed data in a source buffer, perform forward DCT and quantization, then write the output data block. The data block can be written to a destination buffer or to the same location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCTQuantFwd8x8_JPEG_16s_C1            ( const Fw16s *pSrc,
                                                                    Fw16s *pDst, const Fw16u *pQuantFwdTable );

FwStatus STDCALL fwiDCTQuantFwd8x8_JPEG_16s_C1I           (       Fw16s *pDst, const Fw16u *pQuantFwdTable );

/*#FunctionBlock - DCTQuantFwd8X8LS_JPEG
TODO:publish
#Technologies - REF, SSE2
#Short - Transform and quantize with level shift
#Long - <Text>This function reads an 8X8 data block of 8-bit unsigned data in a source buffer, level-shifts the unsigned values to signed values, performs forward DCT and quantization, then writes the output data block to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCTQuantFwd8x8LS_JPEG_8u16s_C1R       ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDst, const Fw16u *pQuantFwdTable );

/*#FunctionBlock - DCTQuantInv8X8_JPEG
TODO:publish
#Technologies - REF, SSE2
#Short - Dequantize and transform
#Long - <Text>These functions read a block of uncompressed image data in a source buffer, perform dequantization and inverse DCT, then write an 8X8 data block. The data block can be written to a destination buffer or to the same location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCTQuantInv8x8_JPEG_16s_C1            ( const Fw16s *pSrc,
                                                                    Fw16s *pDst, const Fw16u *pQuantInvTable );

FwStatus STDCALL fwiDCTQuantInv8x8_JPEG_16s_C1I           (       Fw16s *pDst, const Fw16u *pQuantInvTable );

/*#FunctionBlock - DCTQuantInv8X8LS_JPEG
TODO:publish
#Technologies - REF, SSE2
#Short - Dequantize and transform with level shift
#Long - <Text>This function reads a block of uncompressed image data in a source buffer, performs dequantization and inverse DCT, level-shifts the signed values to unsigned values, then writes an 8X8 data block to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCTQuantInv8x8LS_JPEG_16s8u_C1R       ( const Fw16s *pSrc,
                                                                    Fw8u  *pDst, int dstStep, const Fw16u *pQuantInvTable );

/*#FunctionBlock - Sub128_JPEG
TODO:publish
#Technologies - REF
#Short - Level shift from unsigned 8u to signed 16s
#Long - <Text>This function steps through a block of unsigned 8-bit data in a source buffer, converts the source data to signed values in the range [-128, 127], and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiSub128_JPEG_8u16s_C1R                ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Add128_JPEG
TODO:publish
#Technologies - REF
#Short - Level shift from signed 16s to unsigned 8u
#Long - <Text>This function steps through a block of signed 16-bit data with values in the range [-128, 127] in a source buffer, converts the source data to 8-bit unsigned values in the range [0, 255], and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL  fwiAdd128_JPEG_16s8u_C1R                ( const Fw16s *pSrc, int srcStep,
                                                                    Fw8u  *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - SampleDownH2V1_JPEG
TODO:publish
#Technologies - REF
#Short - Downsample block (H2V1)
#Long - <Text> This function steps through a block of pixel data in a source buffer, performs 2:1 horizontal and 1:1 vertical (4:2:2) chrominance downsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleDownH2V1_JPEG_8u_C1R            ( const Fw8u *pSrc, int srcStep, FwiSize srcSize,
                                                                    Fw8u *pDst, int dstStep, FwiSize dstSize );

/*#FunctionBlock - SampleDownH2V2_JPEG
TODO:publish
#Technologies - REF
#Short - Downsample block (H2V2)
#Long - <Text> This function steps through a block of pixel data in a source buffer, performs 2:1 horizontal and 2:1 vertical (4:1:1) chrominance downsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleDownH2V2_JPEG_8u_C1R            ( const Fw8u *pSrc, int srcStep, FwiSize srcSize,
                                                                    Fw8u *pDst, int dstStep, FwiSize dstSize );

/*#FunctionBlock - SampleDownRowH2V1_JPEG
TODO:publish
#Technologies - REF
#Short - Downsample row (H2V1)
#Long - <Text> This function steps through a row of pixel data in a source buffer, performs 2:1 horizontal and 1:1 vertical (4:2:2) chrominance downsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>Row ends are defined by the two source pointers. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSampleDownRowH2V1_Box_JPEG_8u_C1      ( const Fw8u *pSrc,int srcWidth,
                                                                    Fw8u *pDst );

/*#FunctionBlock - SampleDownRowH2V2_JPEG
TODO:publish
#Technologies - REF
#Short - Downsample row (H2V2)
#Long - <Text> This function steps through a row of pixel data in a source buffer, performs 2:1 horizontal and 2:1 vertical (4:1:1) chrominance downsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>Row ends are defined by the two source pointers. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSampleDownRowH2V2_Box_JPEG_8u_C1      ( const Fw8u *pSrc1, const Fw8u *pSrc2,int srcWidth,
                                                                    Fw8u *pDst );

/*#FunctionBlock - SampleUpH2V1_JPEG
TODO:publish
#Technologies - REF
#Short - Upsample block (H2V1)
#Long - <Text> This function steps through a block of pixel data in a source buffer, performs 2:1 horizontal and 1:1 vertical (4:2:2) chrominance upsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>A "triangular" linear interpolation algorithm is used for upsampling; the interpolated pixels are 1/4 and 3/4 of the distance between the source pixels. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleUpH2V1_JPEG_8u_C1R              ( const Fw8u *pSrc, int srcStep, FwiSize srcSize,
                                                                    Fw8u *pDst, int dstStep, FwiSize dstSize );

/*#FunctionBlock - SampleUpH2V2_JPEG
TODO:publish
#Technologies - REF
#Short - Upsample block (H2V2)
#Long - <Text> This function steps through a block of pixel data in a source buffer, performs 2:1 horizontal and 2:1 vertical (4:1:1) chrominance upsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>A "triangular" linear interpolation algorithm is used for upsampling; the interpolated pixels are 1/4 and 3/4 of the distance between the source pixels. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleUpH2V2_JPEG_8u_C1R              ( const Fw8u *pSrc, int srcStep, FwiSize srcSize,
                                                                    Fw8u *pDst, int dstStep, FwiSize dstSize );

/*#FunctionBlock - SampleUpRowH2V1_JPEG
TODO:publish
#Technologies - REF
#Short - Upsample row (H2V1)
#Long - <Text> This function steps through a row of pixel data in a source buffer, performs 2:1 horizontal and 1:1 vertical (4:2:2) chrominance upsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>A "triangular" linear interpolation algorithm is used for upsampling; the interpolated pixels are 1/4 and 3/4 of the distance between the source pixels. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSampleUpRowH2V1_Triangle_JPEG_8u_C1   ( const Fw8u *pSrc, int srcWidth,
                                                                    Fw8u *pDst );

/*#FunctionBlock - SampleUpRowH2V2_JPEG
TODO:publish
#Technologies - REF
#Short - Upsample row H2V2
#Long - <Text> This function steps through a row of pixel data in a source buffer, performs 2:1 horizontal and 2:1 vertical (4:1:1) chrominance upsampling without smoothing, and writes the output data to a destination buffer.</Text>
<Text>A "triangular" linear interpolation algorithm is used for upsampling; the interpolated pixels are 1/4 and 3/4 of the distance between the source pixels. Ordered dither rounding is used (0.5 is rounded up or down at alternate pixel locations).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSampleUpRowH2V2_Triangle_JPEG_8u_C1   ( const Fw8u *pSrc1, const Fw8u *pSrc2, int srcWidth,
                                                                    Fw8u *pDst );

/*#FunctionBlock - SampleDown4xxLS_MCU
TODO:publish
#Technologies - REF
#Short - Downsample to 4XX MCU
#Long - <Text> These functions step through a block of pixel data in a source buffer, perform chrominance downsampling, level-shift the source data to 16-bit signed values, create a minimum coded unit, and write the output data to a destination buffer.</Text>
<Text>There are functions for 4:4:4, 4:2:2, and 4:1:1 MCUs.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleDown444LS_MCU_8u16s_C3P3R       ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL fwiSampleDown422LS_MCU_8u16s_C3P3R       ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

FwStatus STDCALL fwiSampleDown411LS_MCU_8u16s_C3P3R       ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

/*#FunctionBlock - SampleUp4xxLS_MCU
TODO:publish
#Technologies - REF
#Short - Upsample from 4XX MCU
#Long - <Text> These functions step through an minimum coded unit in a source buffer, perform chrominance upsampling, level-shift the source data to 8-bit unsigned values, and write interleaved pixel-order output data to a destination buffer.</Text>
<Text>There are functions for 4:4:4, 4:2:2, and 4:1:1 MCUs.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr
*/
FwStatus STDCALL fwiSampleUp444LS_MCU_16s8u_P3C3R         ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDst      , int dstStep );

FwStatus STDCALL fwiSampleUp422LS_MCU_16s8u_P3C3R         ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDst      , int dstStep );

FwStatus STDCALL fwiSampleUp411LS_MCU_16s8u_P3C3R         ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDst      , int dstStep );

/*#FunctionBlock - Split422LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert 422 MCU to 422 interleaved data
#Long - <Text>This function steps through a 4:2:2 chrominance sub-sampled MCU in a source buffer, splits the source MCU into two blocks of interleaved 4:2:2 chrominance sub-sampled pixel data, level-shifts the source data to unsigned values, and writes the output data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr
*/

FwStatus STDCALL fwiSplit422LS_MCU_8u16s_C2P3R            ( const Fw8u  *pSrc, int srcStep,
                                                                    Fw16s *pDstMCU[3] );

/*#FunctionBlock - Join422LS_MCU
TODO:publish
#Technologies - REF
#Short - Convert 422 interleaved data to 422 MCU
#Long - <Text>This function steps through two blocks of interleaved 4:2:2 chrominance sub-sampled pixel data in a source buffer, combines the two channels into a 4:2:2 chrominance sub-sampled MCU, level-shifts the source data to signed values, and writes the output data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr

*/
FwStatus STDCALL fwiJoin422LS_MCU_16s8u_P3C2R             ( const Fw16s *pSrcMCU[3],
                                                                    Fw8u  *pDst      , int dstStep );

/*#FunctionBlock - EncodeHuffmanRawTableInit
TODO:publish
#Technologies - REF
#Short - Create raw Huffman encode table
#Long - <Text>This function creates a raw Huffman table with symbol statistics.</Text>
<Text>The function definition follows CCITT Rec. T.81(1992 E), Figure B.7, page 40, and Annex K.2.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanRawTableInit_JPEG_8u     ( const int pStatistics[256],
                                                                    Fw8u *pListBits, Fw8u *pListVals );

/*#FunctionBlock - EncodeHuffmanSpecGetBufSize
TODO:publish
#Technologies - REF
#Short - Get Huffman encode table buffer size
#Long - <Text>This function returns the Huffman encode table buffer size, in bytes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanSpecGetBufSize_JPEG_8u   ( int *pSize );

/*#FunctionBlock - EncodeHuffmanSpecInit
TODO:publish
#Technologies - REF
#Short - Initialize Huffman encode table
#Long - <Text>This function creates and initializes a Huffman encode table.</Text>
<Text>The function definition follows CCITT Rec. T.81(1992 E), Annex C.2.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanSpecInit_JPEG_8u         ( const Fw8u *pListBits,
                                                              const Fw8u *pListVals, FwiEncodeHuffmanSpec *pEncHuffSpec );

/*#FunctionBlock - EncodeHuffmanSpecInitAlloc
TODO:publish
#Technologies - REF
#Short - Allocate memory and initialize Huffman encode table
#Long - <Text>This function allocates memory for creation of a Huffman encode table.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanSpecInitAlloc_JPEG_8u    ( const Fw8u *pListBits,
                                                              const Fw8u *pListVals, FwiEncodeHuffmanSpec* *pEncHuffSpec );

/*#FunctionBlock - EncodeHuffmanSpecFree
TODO:publish
#Technologies - REF
#Short - Free Huffman encode table memory
#Long - <Text>This function frees memory allocated by the <FunctionName>EncodeHuffmanSpecInitAlloc</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanSpecFree_JPEG_8u         ( FwiEncodeHuffmanSpec *pEncHuffSpec );

/*#FunctionBlock - EncodeHuffmanStateGetBufSize
TODO:publish
#Technologies - REF
#Short - Get Huffman encode state structure buffer size
#Long - <Text>This function returns the Huffman encode state structure buffer size, in bytes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanStateGetBufSize_JPEG_8u  ( int *pSize );

/*#FunctionBlock - EncodeHuffmanStateInit
TODO:publish
#Technologies - REF
#Short - Initialize Huffman encode state structure.
#Long - <Text>This function initializes the Huffman encode state structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanStateInit_JPEG_8u        ( FwiEncodeHuffmanState *pEncHuffState );

/*#FunctionBlock - EncodeHuffmanStateInitAlloc
TODO:publish
#Technologies - REF
#Short - Allocate memory and initialize Huffman encode state structure
#Long - <Text>This function allocates memory for the Huffman encode state structure and initializes the structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanStateInitAlloc_JPEG_8u   ( FwiEncodeHuffmanState* *pEncHuffState );

/*#FunctionBlock - EncodeHuffmanStateFree
TODO:publish
#Technologies - REF
#Short - Free Huffman encode state structure memory
#Long - <Text>This function frees memory allocated by the <FunctionName>EncodeHuffmanStateInitAlloc</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffmanStateFree_JPEG_8u        ( FwiEncodeHuffmanState *pEncHuffState );

/*#FunctionBlock - EncodeHuffman8X8
TODO:publish
#Technologies - REF
#Short - Encode data block with bit accumulation
#Long - <Text>This function performs Huffman baseline encoding of an 8X8 data block of quantized DCT coefficients using the DC and AC encoding tables.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex F.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks in a stream are accumulated in the Huffman encode state structure.</Text>
<Text>To place all the accumulated bits in the buffer, set the <Bold>bFlashState</Bold> bit to 1 before processing the last 8X8 block in the stream, or restart the encoded interval.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_JPEG_16s1u_C1        ( const Fw16s *pSrc, Fw8u *pDst,
                                                              int dstLenBytes, int *pDstCurrPos, Fw16s *pLastDC,
                                                              const FwiEncodeHuffmanSpec *pDcTable,
                                                              const FwiEncodeHuffmanSpec *pAcTable, FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - EncodeHuffman8X8_Direct
TODO:publish
#Technologies - REF
#Short - Encode data block without bit accumulation
#Long - <Text>This function performs direct Huffman baseline encoding of an 8X8 data block of quantized DCT coefficients using the DC and AC encoding tables.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex F.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks are not accumulated in the Huffman encode state structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_Direct_JPEG_16s1u_C1 ( const Fw16s *pSrc,
                                                                    Fw8u  *pDst, int *pDstBitsLen, Fw16s *pLastDC,
                                                              const FwiEncodeHuffmanSpec *pDcTable,
                                                              const FwiEncodeHuffmanSpec *pAcTable );

/*#FunctionBlock - EncodeHuffman8X8_DCFirst
TODO:publish
#Technologies - REF
#Short - Encode progressive DC initial
#Long - <Text>This function performs the first scan for progressive encoding of the DC coefficient of an 8X8 data block.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex G.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks in a stream are accumulated in the Huffman encode state structure.</Text>
<Text>To place all the accumulated bits in the buffer, set the <Bold>bFlashState</Bold> bit to 1 before processing the last 8X8 block in the stream, or restart the encoded interval.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_DCFirst_JPEG_16s1u_C1( const Fw16s *pSrc,
                                                                    Fw8u  *pDst, int dstLenBytes, int *pDstCurrPos,
                                                                    Fw16s *pLastDC, int Al, const FwiEncodeHuffmanSpec *pDcTable, FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - EncodeHuffman8X8_DCRefine
TODO:publish
#Technologies - REF
#Short -  Encode progressive DC subsequent
#Long - <Text>This function performs a subsequent scan for progressive encoding of the DC coefficient of an 8X8 data block.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex G.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks in a stream are accumulated in the Huffman encode state structure.</Text>
<Text>To place all the accumulated bits in the buffer, set the <Bold>bFlashState</Bold> bit to 1 before processing the last 8X8 block in the stream, or restart the encoded interval.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_DCRefine_JPEG_16s1u_C1 ( const Fw16s *pSrc,
                                                                      Fw8u  *pDst, int dstLenBytes, int *pDstCurrPos, int Al,
                                                                FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - EncodeHuffman8X8_ACFirst
TODO:publish
#Technologies - REF
#Short - Encode progressive AC initial
#Long - <Text>This function performs the first scan for progressive encoding of the AC coefficients of an 8X8 data block.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex G.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks in a stream are accumulated in the Huffman encode state structure.</Text>
<Text>To place all the accumulated bits in the buffer, set the <Bold>bFlashState</Bold> bit to 1 before processing the last 8X8 block in the stream, or restart the encoded interval.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_ACFirst_JPEG_16s1u_C1( const Fw16s *pSrc,
                                                                    Fw8u  *pDst, int dstLenBytes, int *pDstCurrPos, int Ss,
                                                              int Se, int Al, const FwiEncodeHuffmanSpec  *pAcTable,
                                                                                    FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - EncodeHuffman8X8_ACRefine
TODO:publish
#Technologies - REF
#Short - Encode progressive AC subsequent
#Long - <Text>This function performs a subsequent scan for progressive encoding of the AC coefficients of an 8X8 data block.</Text>
<Text>The encoding process follows CCITT Rec. T.81, Annex G.1.2. The function writes only full bytes to the output buffer.</Text>
<Text>Incomplete bits from data blocks in a stream are accumulated in the Huffman encode state structure.</Text>
<Text>To place all the accumulated bits in the buffer, set the <Bold>bFlashState</Bold> bit to 1 before processing the last 8X8 block in the stream, or restart the encoded interval.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiEncodeHuffman8x8_ACRefine_JPEG_16s1u_C1 ( const Fw16s *pSrc,
                                                                      Fw8u  *pDst, int dstLenBytes, int *pDstCurrPos, int Ss,
                                                                int Se, int Al, const FwiEncodeHuffmanSpec  *pAcTable,
                                                                                      FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - GetHuffmanStatistics8X8
TODO:publish
#Technologies - REF
#Short - Get encoding statistics for data block
#Long - <Text>This function computes statistics for encoding an 8X8 data block of DCT coefficients.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetHuffmanStatistics8x8_JPEG_16s_C1   ( const Fw16s *pSrc,
                                                              int pDcStatistics[256], int pAcStatistics[256],
                                                                    Fw16s *pLastDC );

/*#FunctionBlock - GetHuffmanStatistics8X8_DCFirst
TODO:publish
#Technologies - REF
#Short - Get encoding statistics progressive DC initial
#Long - <Text>This function computes statistics for the first scan of the DC coefficient in progressivce encoding of a data block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetHuffmanStatistics8x8_DCFirst_JPEG_16s_C1   ( const Fw16s *pSrc,
                                                                      int pDcStatistics[256], Fw16s *pLastDC, int Al );

#if BUILD_NUM_AT_LEAST(9999)
/*#FunctionBlock - GetHuffmanStatistics8X8_ACFirst
TODO:publish
#Technologies - REF
#Short - Get encoding statistics progressive AC initial
#Long - <Text>This function computes statistics for the first scan of the AC coefficients in progressivce encoding of a data block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetHuffmanStatistics8x8_ACFirst_JPEG_16s_C1   ( const Fw16s *pSrc,
                                                                      int pAcStatistics[256], int Ss, int Se, int Al,
                                                                      FwiEncodeHuffmanState *pEncHuffState, int bFlushState );

/*#FunctionBlock - GetHuffmanStatistics8X8_ACRefine
TODO:publish
#Technologies - REF
#Short - Get encoding statistics progressive AC subsequent
#Long - <Text>This function computes statistics for a subsequent scan of the AC coefficients in progressive encoding of a data block.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetHuffmanStatistics8x8_ACRefine_JPEG_16s_C1  ( const Fw16s *pSrc,
                                                                      int pAcStatistics[256], int Ss, int Se, int Al,
                                                                      FwiEncodeHuffmanState *pEncHuffState, int bFlushState );
#endif

/*#FunctionBlock - DecodeHuffmanSpecGetBufSize
TODO:publish
#Technologies - REF
#Short - Get Huffman decode buffer size
#Long - <Text>This function returns the Huffman decoder buffer size in bytes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanSpecGetBufSize_JPEG_8u ( int *pSize );

/*#FunctionBlock - DecodeHuffmanSpecInit
TODO:publish
#Technologies - REF
#Short - Initialize Huffman decode table
#Long - <Text>This function creates a Huffman table and initializes it for decode.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanSpecInit_JPEG_8u         ( const Fw8u *pListBits,
                                                              const Fw8u *pListVals, FwiDecodeHuffmanSpec *pDecHuffSpec );

/*#FunctionBlock - DecodeHuffmanSpecInitAlloc
TODO:publish
#Technologies - REF
#Short - Allocate memory and initialize Huffman decode table
#Long - <Text>This function allocates memory for a Huffman decode table, creates the table, and initializes it for decode.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanSpecInitAlloc_JPEG_8u    ( const Fw8u *pListBits,
                                                              const Fw8u *pListVals, FwiDecodeHuffmanSpec* *pDecHuffSpec );

/*#FunctionBlock - DecodeHuffmanSpecFree
TODO:publish
#Technologies - REF
#Short - Free Huffman decode table memory
#Long - <Text>This function frees the memory allocated for a Huffman decode table.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanSpecFree_JPEG_8u         ( FwiDecodeHuffmanSpec *pDecHuffSpec );

/*#FunctionBlock - DecodeHuffmanStateGetBufSize
TODO:publish
#Technologies - REF
#Short - Get Huffman decode state structure buffer size
#Long - <Text>This function returns the Huffman decode state structure buffer size, in bytes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanStateGetBufSize_JPEG_8u  ( int *pSize );

/*#FunctionBlock - DecodeHuffmanStateInit
TODO:publish
#Technologies - REF
#Short - Initialize Huffman decode state structure
#Long - <Text>This function initializes the Huffman decode state structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanStateInit_JPEG_8u        ( FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffmanStateInitAlloc
TODO:publish
#Technologies - REF
#Short - Allocate memory and initialize Huffman decode state structure
#Long - <Text>This function allocates memory and initializes a Huffman state structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanStateInitAlloc_JPEG_8u   ( FwiDecodeHuffmanState* *pDecHuffState );

/*#FunctionBlock - DecodeHuffmanStateFree
TODO:publish
#Technologies - REF
#Short - Free Huffman decode state structure memory
#Long - <Text>This function frees the memory allocated to the Huffman decode state structure.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffmanStateFree_JPEG_8u        ( FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffman8X8
TODO:publish
#Technologies - REF
#Short - Decode data block
#Long - <Text>This function decodes an 8X8 data block of Huffman-encoded quantized DCT coefficients.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex F.2.2.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_JPEG_1u16s_C1        ( const Fw8u *pSrc , int srcLenBytes  , int *pSrcCurrPos,
                                                                    Fw16s *pDst, Fw16s   *pLastDC, int *pMarker,
                                                              const FwiDecodeHuffmanSpec  *pDcTable, const FwiDecodeHuffmanSpec  *pAcTable, FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffman8X8_Direct
TODO:publish
#Technologies - REF
#Short - Decode data block without bit accumulation
#Long - <Text>This function performs direct decoding of an 8X8 data block of Huffman-encoded quantized DC and AC DCT coefficients.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex F.2.2.</Text>
<Text>The function writes only full bytes to the destination buffer. Incomplete bits from data blocks are not accumulated in the Huffman decode state structure.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
<Text>During function initialization, make sure that <ParameterName>pLastDC</ParameterName>, <ParameterName>pMarker</ParameterName>, and <ParameterName>pNumValidPrefetchedBits</ParameterName> are cleared to all zeroes.</Text>
<Text>After each restart interval, make sure that <ParameterName>pLastDC</ParameterName> and <ParameterName>pNumValidPrefetchedBits</ParameterName> are cleared to all zeroes.</Text>
<Text>After each found marker has been processed, make sure that <ParameterName>pMarker</ParameterName> and <ParameterName>pNumValidPrefetchedBits</ParameterName> are cleared to all zeroes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_Direct_JPEG_1u16s_C1 ( const Fw8u  *pSrc, int *pSrcBitsLen,
                                                                    Fw16s *pDst, Fw16s  *pLastDC, int *pMarker, Fw32u *pPrefetchedBits, int *pNumValidPrefetchedBits,
                                                              const FwiDecodeHuffmanSpec *pDcTable, const FwiDecodeHuffmanSpec *pAcTable );

/*#FunctionBlock - DecodeHuffman8X8_DCFirst
TODO:publish
#Technologies - REF
#Short - Decode progressive DC initial
#Long - <Text>This function performs the first scan for progressive decoding of the DC coefficient of an 8X8 data block.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex G.2.2.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_DCFirst_JPEG_1u16s_C1( const Fw8u  *pSrc,int srcLenBytes , int *pSrcCurrPos,
                                                                    Fw16s *pDst, Fw16s *pLastDC, int *pMarker, int Al,
                                                              const FwiDecodeHuffmanSpec  *pDcTable, FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffman8X8_DCRefine
TODO:publish
#Technologies - REF
#Short - Decode progressive DC initial
#Long - <Text>This function performs a subsequent scan for progressive decoding of the DC coefficient of an 8X8 data block.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex G.2.2.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_DCRefine_JPEG_1u16s_C1 ( const Fw8u *pSrc, int srcLenBytes, int *pSrcCurrPos,
                                                                      Fw16s *pDst, int *pMarker, int Al,
                                                                FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffman8X8_ACFirst
TODO:publish
#Technologies - REF
#Short - Decode progressive AC initial
#Long - <Text>This function performs the first scan for progressive decoding of the AC coefficients of an 8X8 data block.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex G.2.2.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_ACFirst_JPEG_1u16s_C1 ( const Fw8u  *pSrc, int srcLenBytes, int *pSrcCurrPos,
                                                                     Fw16s *pDst, int *pMarker, int Ss, int Se, int Al,
                                                               const FwiDecodeHuffmanSpec *pAcTable, FwiDecodeHuffmanState *pDecHuffState );

/*#FunctionBlock - DecodeHuffman8X8_ACRefine
TODO:publish
#Technologies - REF
#Short - Decode progressive AC subsequent
#Long - <Text>This function performs a subsequent scan for progressive decoding of the AC coefficients of an 8X8 data block.</Text>
<Text>The decoding process follows CCITT Rec. T.81, Annex G.2.2.</Text>
<Text>When the function detects a JPEG marker, it stops decoding and writes the marker to the location specified by <ParameterName>pMarker</ParameterName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDecodeHuffman8x8_ACRefine_JPEG_1u16s_C1 ( const Fw8u  *pSrc, int srcLenBytes, int *pSrcCurrPos,
                                                                      Fw16s *pDst, int *pMarker, int Ss, int Se, int Al,
                                                                const FwiDecodeHuffmanSpec *pAcTable, FwiDecodeHuffmanState *pDecHuffState );

#endif // FW_BUILD_NUM

/*#Documentation

</Chapter>

*/

#ifdef __cplusplus
}
#endif

#endif // __FWJPEG_H__
