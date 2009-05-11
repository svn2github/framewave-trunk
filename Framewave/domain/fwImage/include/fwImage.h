/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FWIMAGE_H__
#define __FWIMAGE_H__

#include "fwBase.h"

typedef struct LUTSpec FwiLUTSpec;

//
// Enumerations
//
enum {
    FWI_INTER_NN       = 1,            // Nearest neighbor
    FWI_INTER_LINEAR   = 2,            // Linera Interpolation
    FWI_INTER_CUBIC    = 4,            // Bicubic Interpolation
    FWI_INTER_SUPER    = 8,            // Supersampling
    FWI_INTER_LANCZOS  = 16,           // 3-lobe Lanczos Interpolation
    FWI_SMOOTH_EDGE    = ( 1<<31 )     // Edge Smooth Feature
};

#if BUILD_NUM_AT_LEAST( 9999 )
//Data Structure define model
typedef struct fwcvMorphState      FwiMorphState;
typedef struct fwcvMorphAdvState   FwiMorphAdvState;
typedef struct fwcvMorphGrayState  FwcvMorphGrayState;
#endif

typedef enum {
    fwWinBartlett,
    fwWinBlackman,
    fwWinHamming,
    fwWinHann,
    fwWinRect,
    fwWinKaiser
} FwWinType;

typedef enum {
    fwAlphaOver,
    fwAlphaIn,
    fwAlphaOut,
    fwAlphaATop,
    fwAlphaXor,
    fwAlphaPlus,
    fwAlphaOverPremul,
    fwAlphaInPremul,
    fwAlphaOutPremul,
    fwAlphaATopPremul,
    fwAlphaXorPremul,
    fwAlphaPlusPremul
} FwiAlphaType;

typedef enum {
    fwDitherNone,
    fwDitherFS,
    fwDitherJJN,
    fwDitherStucki,
    fwDitherBayer
} FwiDitherType;

#ifdef __cplusplus
extern "C" {
#endif


/*#HintBlock
AutoOff
*/

/*#IncludeBlock
#includeGlobal "../../../doc/fwIntroDoc.h"
#include "../../../doc/fwImageIntroDoc.h"
*/

#if BUILD_NUM_AT_LEAST( 100 )

/*#Documentation

<Chapter>
<ChapterHeading>Library Version</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes the function that provides library version information.</ChaptDesc></Paragraph> 
*/

/*#FunctionBlock - GetLibVersion
TODO:Publish
#Technologies - REF
#Short - Get library version
#Long - <Text>This function returns a pointer to the <Bold>FwLibraryVersion</Bold> structure that contains FW Library version information.</Text>
*/

const FwLibraryVersion* STDCALL fwiGetLibVersion ();

/*#Documentation
</Chapter>
*/

#endif

/*#Documentation

<Chapter>
<ChapterHeading>Support Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes FW functions that support other FW functions.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - Malloc
todo:publish
#Technologies - REF
#Short - Memory allocation
#Long - <Text>These functions allocate memory for image processing. Every row is aligned to 32-bit boundaries. Zero-padding is used to force alignment.</Text>
*/
Fw8u * STDCALL fwiMalloc_8u_C1             ( int widthPixels, int heightPixels, int *pStepBytes );
Fw8u * STDCALL fwiMalloc_8u_C2             ( int widthPixels, int heightPixels, int *pStepBytes );
Fw8u * STDCALL fwiMalloc_8u_C3             ( int widthPixels, int heightPixels, int *pStepBytes );
Fw8u * STDCALL fwiMalloc_8u_C4             ( int widthPixels, int heightPixels, int *pStepBytes );
Fw8u * STDCALL fwiMalloc_8u_AC4            ( int widthPixels, int heightPixels, int *pStepBytes );

Fw16u * STDCALL fwiMalloc_16u_C1           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16u * STDCALL fwiMalloc_16u_C2           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16u * STDCALL fwiMalloc_16u_C3           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16u * STDCALL fwiMalloc_16u_C4           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16u * STDCALL fwiMalloc_16u_AC4          ( int widthPixels, int heightPixels, int *pStepBytes );

Fw16s* STDCALL fwiMalloc_16s_C1           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16s* STDCALL fwiMalloc_16s_C2           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16s* STDCALL fwiMalloc_16s_C3           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16s* STDCALL fwiMalloc_16s_C4           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw16s* STDCALL fwiMalloc_16s_AC4          ( int widthPixels, int heightPixels, int *pStepBytes );

Fw32s* STDCALL fwiMalloc_32s_C1           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32s* STDCALL fwiMalloc_32s_C2           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32s* STDCALL fwiMalloc_32s_C3           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32s* STDCALL fwiMalloc_32s_C4           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32s* STDCALL fwiMalloc_32s_AC4          ( int widthPixels, int heightPixels, int *pStepBytes );

Fw32f* STDCALL fwiMalloc_32f_C1           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32f* STDCALL fwiMalloc_32f_C2           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32f* STDCALL fwiMalloc_32f_C3           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32f* STDCALL fwiMalloc_32f_C4           ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32f* STDCALL fwiMalloc_32f_AC4          ( int widthPixels, int heightPixels, int *pStepBytes );

Fw32sc* STDCALL fwiMalloc_32sc_C1         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32sc* STDCALL fwiMalloc_32sc_C2         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32sc* STDCALL fwiMalloc_32sc_C3         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32sc* STDCALL fwiMalloc_32sc_C4         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32sc* STDCALL fwiMalloc_32sc_AC4        ( int widthPixels, int heightPixels, int *pStepBytes );

Fw32fc* STDCALL fwiMalloc_32fc_C1         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32fc* STDCALL fwiMalloc_32fc_C2         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32fc* STDCALL fwiMalloc_32fc_C3         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32fc* STDCALL fwiMalloc_32fc_C4         ( int widthPixels, int heightPixels, int *pStepBytes );
Fw32fc* STDCALL fwiMalloc_32fc_AC4        ( int widthPixels, int heightPixels, int *pStepBytes );

/*#FunctionBlock - Free
todo:publish
#Technologies - REF
#Short - Free allocated memory
#Long - <Text>These functions free memory allocated by the <FunctionName>Malloc</FunctionName> functions.</Text>
*/

void STDCALL fwiFree ( void *ptr );

/*#Documentation
</Chapter>
<Chapter>
<ChapterHeading>Image Data Exchange and Initialization Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that set the initial value of an image data buffer, copy data from one buffer to another, convert data type, and scale image data.</ChaptDesc></Paragraph> 

<Note>All the data exchange and initialization functions support negative step sizes. Negative step sizes can be used to "flip" data copied from a source buffer to a destination buffer. 
This is a common requirement when displaying a BMP image. To flip an image horizontally, call the copy function with <ParameterName>pSrc</ParameterName> pointing to the last row 
of the source buffer, a negative <ParameterName>srcStep</ParameterName>, <ParameterName>pDst</ParameterName> pointing to the start of the destination buffer, and a positive 
<ParameterName>dstStep</ParameterName>.
</Note>
*/


/*#FunctionBlock - Convert
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert data from one type to another
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data to another data type, and write the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiConvert_8u16u_C1R     ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16u_C3R     ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16u_C4R     ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16u_AC4R    ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_8u16s_C1R     ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16s_C3R     ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16s_C4R     ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u16s_AC4R    ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_8u32s_C1R     ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32s_C3R     ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32s_C4R     ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32s_AC4R    ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_8s32s_C1R     ( const Fw8s  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32s_C3R     ( const Fw8s  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32s_C4R     ( const Fw8s  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32s_AC4R    ( const Fw8s  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_16u32s_C1R    ( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32s_C3R    ( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32s_C4R    ( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32s_AC4R   ( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_8u32f_C1R     ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32f_C3R     ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32f_C4R     ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8u32f_AC4R    ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_8s32f_C1R     ( const Fw8s  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32f_C3R     ( const Fw8s  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32f_C4R     ( const Fw8s  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_8s32f_AC4R    ( const Fw8s  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_16u32f_C1R    ( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32f_C3R    ( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32f_C4R    ( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u32f_AC4R   ( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_16s32f_C1R    ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s32f_C3R    ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s32f_C4R    ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s32f_AC4R   ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

//Reduced bit depth
FwStatus STDCALL fwiConvert_16u8u_C1R     ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u8u_C3R     ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u8u_C4R     ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16u8u_AC4R    ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_16s8u_C1R     ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s8u_C3R     ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s8u_C4R     ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_16s8u_AC4R    ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32s8u_C1R     ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8u_C3R     ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8u_C4R     ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8u_AC4R    ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32s8s_C1R     ( const Fw32s *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8s_C3R     ( const Fw32s *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8s_C4R     ( const Fw32s *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32s8s_AC4R    ( const Fw32s *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32f8u_C1R     ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8u_C3R     ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8u_C4R     ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8u_AC4R    ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32f8s_C1R     ( const Fw32f *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8s_C3R     ( const Fw32f *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8s_C4R     ( const Fw32f *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f8s_AC4R    ( const Fw32f *pSrc, int srcStep, Fw8s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32f16s_C1R     ( const Fw32f *pSrc, int srcStep, Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16s_C3R     ( const Fw32f *pSrc, int srcStep, Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16s_C4R     ( const Fw32f *pSrc, int srcStep, Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16s_AC4R    ( const Fw32f *pSrc, int srcStep, Fw16s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiConvert_32f16u_C1R     ( const Fw32f *pSrc, int srcStep, Fw16u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16u_C3R     ( const Fw32f *pSrc, int srcStep, Fw16u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16u_C4R     ( const Fw32f *pSrc, int srcStep, Fw16u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiConvert_32f16u_AC4R    ( const Fw32f *pSrc, int srcStep, Fw16u  *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - Copy
TODO: publish
#Short - Copy values
#Technologies - REF, MT, SSE2, F10H
#Long - <Text>These functions step through an ROI in a source buffer and copy the source data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/


FwStatus STDCALL fwiCopy_8u_C1R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C3R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_AC4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C3AC4R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_AC4C3R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_8u_C3CR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C4CR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C3C1R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C4C1R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C1C3R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C1C4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_16s_C1R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C3R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C4R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_AC4R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C3AC4R       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_AC4C3R       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_16s_C3CR         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C4CR         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C3C1R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C4C1R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C1C3R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C1C4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32s_C1R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C3R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C4R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_AC4R         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C3AC4R       ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_AC4C3R       ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32s_C3CR         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C4CR         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C3C1R        ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C4C1R        ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C1C3R        ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C1C4R        ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32f_C1R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C3R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_AC4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C3AC4R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_AC4C3R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32f_C3CR         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C4CR         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C3C1R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C4C1R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C1C3R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C1C4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_8u_C3P3R         ( const Fw8u  *       pSrc   , int srcStep, Fw8u  * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_C4P4R         ( const Fw8u  *       pSrc   , int srcStep, Fw8u  * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_P3C3R         ( const Fw8u  * const pSrc[3], int srcStep, Fw8u  *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_8u_P4C4R         ( const Fw8u  * const pSrc[4], int srcStep, Fw8u  *       pDst   , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_16s_C3P3R        ( const Fw16s *       pSrc   , int srcStep, Fw16s * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_C4P4R        ( const Fw16s *       pSrc   , int srcStep, Fw16s * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_P3C3R        ( const Fw16s * const pSrc[3], int srcStep, Fw16s *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_16s_P4C4R        ( const Fw16s * const pSrc[4], int srcStep, Fw16s *       pDst   , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32s_C3P3R        ( const Fw32s *       pSrc   , int srcStep, Fw32s * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_C4P4R        ( const Fw32s *       pSrc   , int srcStep, Fw32s * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_P3C3R        ( const Fw32s * const pSrc[3], int srcStep, Fw32s *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32s_P4C4R        ( const Fw32s * const pSrc[4], int srcStep, Fw32s *       pDst   , int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiCopy_32f_C3P3R        ( const Fw32f *       pSrc   , int srcStep, Fw32f * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_C4P4R        ( const Fw32f *       pSrc   , int srcStep, Fw32f * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_P3C3R        ( const Fw32f * const pSrc[3], int srcStep, Fw32f *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiCopy_32f_P4C4R        ( const Fw32f * const pSrc[4], int srcStep, Fw32f *       pDst   , int dstStep, FwiSize roiSize );


FwStatus STDCALL fwiCopy_8u_C1MR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_8u_C3MR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_8u_C4MR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_8u_AC4MR         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
                                                                                     
FwStatus STDCALL fwiCopy_16s_C1MR         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_16s_C3MR         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_16s_C4MR         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_16s_AC4MR        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );

FwStatus STDCALL fwiCopy_32s_C1MR         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32s_C3MR         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32s_C4MR         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32s_AC4MR        ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );

FwStatus STDCALL fwiCopy_32f_C1MR         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32f_C3MR         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32f_C4MR         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
FwStatus STDCALL fwiCopy_32f_AC4MR        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );

/*#FunctionBlock - Swap
TODO: publish
#Technologies - REF, MT, SSE2, F10H
#Short - Change channel order
#Long - <Text>These functions step through an ROI in a source buffer, swap the source buffer channels as specified by the destination order, and write the channel data to the destination buffer.</Text>
<Text>For example, dstOrder[1] = 0 places channel 0 in the source buffer into channel 1 of the destination buffer.</Text>
#ReturnValues - fwStsNoErr ,fwStsNullPtrErr ,fwStsSizeErr,fwStsStepErr,fwStsChannelOrderErr
*/

FwStatus STDCALL fwiSwapChannels_8u_C3R   ( const Fw8u  *pSrc,    int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_8u_AC4R  ( const Fw8u  *pSrc,    int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_16u_C3R  ( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_16u_AC4R ( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_32s_C3R  ( const Fw32s *pSrc,    int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_32s_AC4R ( const Fw32s *pSrc,    int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_32f_C3R  ( const Fw32f *pSrc,    int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_32f_AC4R ( const Fw32f *pSrc,    int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3] );
FwStatus STDCALL fwiSwapChannels_8u_C3IR  (       Fw8u  *pSrcDst, int srcDstStep,                         FwiSize roiSize,const int dstOrder[3] );

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - Ramp
todo:publish
#Technologies - REF, MT, SSE2
#Short - Create ramped test image
#Long - <Text>These functions create an intensity-ramped test image in a destination buffer using calculated values.</Text>
<Text>Pixel values that exceed the image data range are saturated to the data range limits.</Text>
<Text>There are functions for one-channel and three-channel images.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiImageRamp_8u_C1R      ( Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_8u_C3R      ( Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_8s_C1R      ( Fw8s  *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_8s_C3R      ( Fw8s  *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_16u_C1R     ( Fw16u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_16u_C3R     ( Fw16u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_16s_C1R     ( Fw16s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_16s_C3R     ( Fw16s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_32s_C1R     ( Fw32s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_32s_C3R     ( Fw32s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_32f_C1R     ( Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
FwStatus STDCALL fwiImageRamp_32f_C3R     ( Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );

/*#FunctionBlock - Jaehne
todo:publish
#Technologies - REF, MT, SSE2
#Short - Create Jahne test image
#Long - <Text>These functions create a radial Jaehne test image in a destination buffer using calculated values.</Text>
<Text>There are functions for one-channel, three-channel, and four-channel images.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiImageJaehne_8u_C1R    ( Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8u_C3R    ( Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8u_C4R    ( Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8u_AC4R   ( Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiImageJaehne_8s_C1R    ( Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8s_C3R    ( Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8s_C4R    ( Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_8s_AC4R   ( Fw8s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiImageJaehne_16u_C1R   ( Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16u_C3R   ( Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16u_C4R   ( Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16u_AC4R  ( Fw16u *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiImageJaehne_16s_C1R   ( Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16s_C3R   ( Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16s_C4R   ( Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_16s_AC4R  ( Fw16s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiImageJaehne_32s_C1R   ( Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32s_C3R   ( Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32s_C4R   ( Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32s_AC4R  ( Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiImageJaehne_32f_C1R   ( Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32f_C3R   ( Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32f_C4R   ( Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiImageJaehne_32f_AC4R  ( Fw32f *pDst, int dstStep, FwiSize roiSize );
#endif

/*#FunctionBlock - ZigZag
todo:publish
#Technologies - REF, MT
#Short - Convert to and from zigzag order
#Long - <Text>These functions convert to and from zigzag order.</Text>
<Text>The forward version of the function converts an 8X8 image block from conventional order to zigzag order.</Text>
<Text>The inverse version of the function converts an 8X8 image block from zigzag order to conventional order.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiZigzagFwd8x8_16s_C1   ( const Fw16s *pSrc, Fw16s *pDst );
FwStatus STDCALL fwiZigzagInv8x8_16s_C1   ( const Fw16s *pSrc, Fw16s *pDst );


/*#FunctionBlock - Set
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Set buffer to a value
#Long - <Text>These functions write a specified value to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiSet_8u_C1R            (       Fw8u  value,    Fw8u   *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_8u_C3R            ( const Fw8u  value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_8u_AC4R           ( const Fw8u  value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_8u_C4R            ( const Fw8u  value[4], Fw8u   *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_8u_C3CR           (       Fw8u  value,    Fw8u   *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_8u_C4CR           (       Fw8u  value,    Fw8u   *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiSet_16s_C1R           (       Fw16s value,    Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_16s_C3R           ( const Fw16s value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_16s_AC4R          ( const Fw16s value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_16s_C4R           ( const Fw16s value[4], Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_16s_C3CR          (       Fw16s value,    Fw16s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_16s_C4CR          (       Fw16s value,    Fw16s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiSet_32s_C1R           (       Fw32s value,    Fw32s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32s_C3R           ( const Fw32s value[3], Fw32s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32s_AC4R          ( const Fw32s value[3], Fw32s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32s_C4R           ( const Fw32s value[4], Fw32s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32s_C3CR          (       Fw32s value,    Fw32s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32s_C4CR          (       Fw32s value,    Fw32s  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiSet_32f_C1R           (       Fw32f value,    Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32f_C3R           ( const Fw32f value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32f_AC4R          ( const Fw32f value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32f_C4R           ( const Fw32f value[4], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32f_C3CR          (       Fw32f value,    Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSet_32f_C4CR          (       Fw32f value,    Fw32f  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiSet_8u_C1MR           (       Fw8u  value,    Fw8u   *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_8u_C3MR           ( const Fw8u  value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_8u_AC4MR          ( const Fw8u  value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_8u_C4MR           ( const Fw8u  value[4], Fw8u   *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );

FwStatus STDCALL fwiSet_16s_C1MR          (       Fw16s value,    Fw16s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_16s_C3MR          ( const Fw16s value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_16s_AC4MR         ( const Fw16s value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_16s_C4MR          ( const Fw16s value[4], Fw16s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );

FwStatus STDCALL fwiSet_32s_C1MR          (       Fw32s value,    Fw32s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32s_C3MR          ( const Fw32s value[3], Fw32s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32s_AC4MR         ( const Fw32s value[3], Fw32s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32s_C4MR          ( const Fw32s value[4], Fw32s  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );

FwStatus STDCALL fwiSet_32f_C1MR          (       Fw32f value,    Fw32f  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32f_C3MR          ( const Fw32f value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32f_AC4MR         ( const Fw32f value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );
FwStatus STDCALL fwiSet_32f_C4MR          ( const Fw32f value[4], Fw32f  *pDst, int dstStep, FwiSize roiSize, const Fw8u  *pMask, int maskStep );


/*#FunctionBlock - Scale
TODO:Publish
#Technologies - REF, MT
#Short - Scale buffer data
#Long - <Text>These functions step through an ROI in a source buffer, scale the source data, and write the scaled data to a destination buffer.</Text>
<Text>The output is scaled so that the median value in the source range is the median value in the destination range.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiScale_8u16u_C1R       ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16u_C4R       ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16s_C1R       ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16s_C3R       ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16s_C4R       ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u16s_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u32s_C1R       ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u32s_C3R       ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u32s_C4R       ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiScale_8u32s_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiScale_16u8u_C1R       ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16u8u_C3R       ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16u8u_C4R       ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16u8u_AC4R      ( const Fw16u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16s8u_C1R       ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16s8u_C3R       ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16s8u_C4R       ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_16s8u_AC4R      ( const Fw16s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_32s8u_C1R       ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_32s8u_C3R       ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_32s8u_C4R       ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
FwStatus STDCALL fwiScale_32s8u_AC4R      ( const Fw32s *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );

FwStatus STDCALL fwiScale_8u32f_C1R       ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_8u32f_C3R       ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_8u32f_C4R       ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_8u32f_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_32f8u_C1R       ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_32f8u_C3R       ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_32f8u_C4R       ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
FwStatus STDCALL fwiScale_32f8u_AC4R      ( const Fw32f *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );


/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Arithmetic and Logic Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes general-purpose mathematic functions and functions that perform specific mathematic operations related to image processing.</ChaptDesc></Paragraph> 

<Note>All the arithmetic and logic functions support negative step sizes. Negative step sizes can be used to "flip" data copied from a source buffer to a destination buffer. 
This is a common requirement when displaying a BMP image. To flip an image horizontally, call the copy function with <ParameterName>pSrc</ParameterName> pointing to the last row 
of the source buffer, a negative <ParameterName>srcStep</ParameterName>, <ParameterName>pDst</ParameterName> pointing to the start of the destination buffer, and a positive 
<ParameterName>dstStep</ParameterName>.
</Note>*/

/*#FunctionBlock - Abs
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Get absolute value
#Long - <Text>These functions step through an ROI in a source buffer and calculate the absolute value of the source data.</Text>
<Text>The results can be written to a destination buffer, or back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiAbs_16s_C1IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_C3IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_C4IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_AC4IR         ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C1IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C3IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C4IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_AC4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAbs_16s_C1R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_C3R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_C4R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_16s_AC4R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_C4R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbs_32f_AC4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - AbsDiff
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Absolute difference of two source values
#Long - <Text>These functions step through ROIs in two source buffers, calculate the absolute value of the difference between the data in source buffer 1 and the data in source buffer 2, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiAbsDiff_8u_C1R        ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbsDiff_16u_C1R       ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAbsDiff_32f_C1R       ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - AbsDiffC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Absolute difference of a source value and a constant
#Long - <Text>These functions step through an ROI in a source buffer, calculate the absolute value of the difference between the source data and a specified constant, and write the result to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAbsDiffC_8u_C1R       ( const Fw8u  *pSrc, int srcStep,  Fw8u  *pDst, int dstStep, FwiSize roiSize, int    value );
FwStatus STDCALL fwiAbsDiffC_16u_C1R      ( const Fw16u *pSrc, int srcStep,  Fw16u *pDst, int dstStep, FwiSize roiSize, int    value );
FwStatus STDCALL fwiAbsDiffC_32f_C1R      ( const Fw32f *pSrc, int srcStep,  Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f value );

/*#FunctionBlock - Add
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Add
#Long - <Text>These functions step through ROIs in two source buffers and add the data in buffer 2 to the data in buffer 1.</Text>
<Text>The sum can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results. Other versions add signed and
unsigned integers and write 32-bit floating point results; variations of these functions can also perform a masking operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAdd_8u_C1IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_C3IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_C4IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_AC4IRSfs       ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C1IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C3IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C4IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_AC4IRSfs      ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_C1IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_C3IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_AC4IRSfs     ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_C1IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_C3IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_AC4IRSfs     ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32f_C1IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_C3IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_C4IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_AC4IR         ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_C1IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_C3IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_AC4IR        ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_8u32f_C1IR        ( const Fw8u   *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_8s32f_C1IR        ( const Fw8s   *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_16u32f_C1IR       ( const Fw16u  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAdd_8u32f_C1IMR       ( const Fw8u   *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_8s32f_C1IMR       ( const Fw8s   *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_16u32f_C1IMR      ( const Fw16u  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_C1IMR         ( const Fw32f  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAdd_8u_C1RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_C3RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_C4RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_8u_AC4RSfs        ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C1RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C3RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_C4RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16s_AC4RSfs       ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_C1RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_C3RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_16sc_AC4RSfs      ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_C1RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_C3RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32sc_AC4RSfs      ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAdd_32f_C1R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_C3R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_C4R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32f_AC4R          ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_C1R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_C3R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAdd_32fc_AC4R         ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - AddC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Add constant
#Long - <Text>These functions step through an ROI in a source buffer and add a specified constant value to the source data.</Text>
<Text>The sum can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAddC_8u_C1IRSfs       (       Fw8u   value   , Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_C3IRSfs       ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_AC4IRSfs      ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_C4IRSfs       ( const Fw8u   value[4], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C1IRSfs      (       Fw16s  value   , Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C3IRSfs      ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_AC4IRSfs     ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C4IRSfs      ( const Fw16s  value[4], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_C1IRSfs     (       Fw16sc value   , Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_C3IRSfs     ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_AC4IRSfs    ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_C1IRSfs     (       Fw32sc value   , Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_C3IRSfs     ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_AC4IRSfs    ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32f_C1IR         (       Fw32f  value   , Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_C3IR         ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_AC4IR        ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_C4IR         ( const Fw32f  value[4], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_C1IR        (       Fw32fc value   , Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_C3IR        ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_AC4IR       ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAddC_8u_C1RSfs        ( const Fw8u   *pSrc, int srcStep,       Fw8u   value   , Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_C3RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_AC4RSfs       ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_8u_C4RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[4], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C1RSfs       ( const Fw16s  *pSrc, int srcStep,       Fw16s  value   , Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C3RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_AC4RSfs      ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16s_C4RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[4], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_C1RSfs      ( const Fw16sc *pSrc, int srcStep,       Fw16sc value   , Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_C3RSfs      ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_16sc_AC4RSfs     ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_C1RSfs      ( const Fw32sc *pSrc, int srcStep,       Fw32sc value   , Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_C3RSfs      ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32sc_AC4RSfs     ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiAddC_32f_C1R          ( const Fw32f  *pSrc, int srcStep,       Fw32f  value   , Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_C3R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_AC4R         ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32f_C4R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[4], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_C1R         ( const Fw32fc *pSrc, int srcStep,       Fw32fc value   , Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_C3R         ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddC_32fc_AC4R        ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - AddProduct
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Add product
#Long - <Text>These functions step through ROIs in two source buffers, add the product of the data in buffer 1 and the data in buffer 2 to the data in buffer 1, and write the sum back to the source location.</Text>
<Text>There are versions of the functions that perform a masking operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAddProduct_8u32f_C1IR     ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_8s32f_C1IR     ( const Fw8s  *pSrc1, int src1Step, const Fw8s  *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_16u32f_C1IR    ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_32f_C1IR       ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAddProduct_8u32f_C1IMR    ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_8s32f_C1IMR    ( const Fw8s  *pSrc1, int src1Step, const Fw8s  *pSrc2, int src2Step, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_16u32f_C1IMR   ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddProduct_32f_C1IMR      ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

/*#FunctionBlock - AddSquare
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Add square
#Long - <Text>These functions step through ROIs in two source buffers, add the square of the data in buffer 1 to the data in buffer 2, and write the sum back to the source location.</Text>
<Text>There are versions of the functions that perform a masking operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAddSquare_8u32f_C1IR      ( const Fw8u  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_8s32f_C1IR      ( const Fw8s  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_16u32f_C1IR     ( const Fw16u *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_32f_C1IR        ( const Fw32f *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAddSquare_8u32f_C1IMR     ( const Fw8u  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_8s32f_C1IMR     ( const Fw8s  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_16u32f_C1IMR    ( const Fw16u *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAddSquare_32f_C1IMR       ( const Fw32f *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

/*#FunctionBlock - AddWeighted
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Add weighted
#Long - <Text>These functions step through ROIs in two source buffers, multiply the data in buffer 1 by an alpha scaling factor, multiply the data in buffer 2 by a scaling factor of 1 minus alpha, add the two products, and write the sum back to the source location.</Text>
<Text>There are versions of the functions that perform a masking operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAddWeighted_8u32f_C1IR    ( const Fw8u  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_8s32f_C1IR    ( const Fw8s  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_16u32f_C1IR   ( const Fw16u *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_32f_C1IR      ( const Fw32f *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );

FwStatus STDCALL fwiAddWeighted_8u32f_C1IMR   ( const Fw8u  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_8s32f_C1IMR   ( const Fw8s  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_16u32f_C1IMR  ( const Fw16u *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );
FwStatus STDCALL fwiAddWeighted_32f_C1IMR     ( const Fw32f *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha );

/*#FunctionBlock - Div
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Divide
#Long - <Text>These functions step through ROIs in two source buffers and divide the data in buffer 2 by the data in buffer 1.</Text>
<Text>The quotient can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiDiv_8u_C1IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_8u_C3IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16s_C1IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16s_C3IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_C1IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_C3IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_AC4IRSfs     ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_C1IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_C3IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_AC4IRSfs     ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32f_C1IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_C3IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_C4IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_AC4IR         ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_C1IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_C3IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_AC4IR        ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiDiv_8u_C1RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_8u_C3RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16s_C1RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16s_C3RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_C1RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_C3RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_16sc_AC4RSfs      ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_C1RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_C3RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32sc_AC4RSfs      ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDiv_32f_C1R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_C3R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_C4R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32f_AC4R          ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_C1R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_C3R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDiv_32fc_AC4R         ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - DivC
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Divide by a constant
#Long - <Text>These functions step through an ROI in a source buffer and divide the source data by a constant value.</Text>
<Text>The quotient can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiDivC_8u_C1IRSfs       (       Fw8u   value   , Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_8u_C3IRSfs       ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_8u_AC4IRSfs      ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_C1IRSfs      (       Fw16s  value   , Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_C3IRSfs      ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_AC4IRSfs     ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_C1IRSfs     (       Fw16sc value   , Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_C3IRSfs     ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_AC4IRSfs    ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_C1IRSfs     (       Fw32sc value   , Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_C3IRSfs     ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_AC4IRSfs    ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32f_C1IR         (       Fw32f  value   , Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32f_C3IR         ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32f_AC4IR        ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_C1IR        (       Fw32fc value   , Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_C3IR        ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_AC4IR       ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiDivC_8u_C1RSfs        ( const Fw8u   *pSrc, int srcStep,       Fw8u   value   , Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_8u_C3RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_8u_AC4RSfs       ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_C1RSfs       ( const Fw16s  *pSrc, int srcStep,       Fw16s  value   , Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_C3RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16s_AC4RSfs      ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_C1RSfs      ( const Fw16sc *pSrc, int srcStep,       Fw16sc value   , Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_C3RSfs      ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_16sc_AC4RSfs     ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_C1RSfs      ( const Fw32sc *pSrc, int srcStep,       Fw32sc value,    Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_C3RSfs      ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32sc_AC4RSfs     ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiDivC_32f_C1R          ( const Fw32f  *pSrc, int srcStep,       Fw32f  value   , Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32f_C3R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32f_AC4R         ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_C1R         ( const Fw32fc *pSrc, int srcStep,       Fw32fc value   , Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_C3R         ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDivC_32fc_AC4R        ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Exp
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Raise base e to a power
#Long - <Text>These functions step through an ROI in a source buffer and raise the base e to the power specified by the source data.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned and 16-bit signed versions of the functions perform an integer scaling operation before writing the results.</Text>
<Text>These functions are the inverses of the <FunctionName>Ln</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiExp_8u_C1IRSfs        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_8u_C3IRSfs        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_16s_C1IRSfs       ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_16s_C3IRSfs       ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_32f_C1IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiExp_32f_C3IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiExp_8u_C1RSfs         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_8u_C3RSfs         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_16s_C1RSfs        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_16s_C3RSfs        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiExp_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiExp_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Ln
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Natural logarithm
#Long - <Text>These functions step through an ROI in a source buffer and calculate the natural logarithms of the source data.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned and 16-bit signed versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiLn_8u_C1IRSfs         ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_8u_C3IRSfs         ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_16s_C1IRSfs        ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_16s_C3IRSfs        ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_32f_C1IR           ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLn_32f_C3IR           ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiLn_8u_C1RSfs          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_8u_C3RSfs          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_16s_C1RSfs         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_16s_C3RSfs         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiLn_32f_C1R            ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLn_32f_C3R            ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Mul
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Multiply
#Long - <Text>These functions step through ROIs in two source buffers and multiply the data in buffer 1 by the data in buffer 2.</Text>
<Text>The product can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMul_8u_C1IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_C3IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_C4IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_AC4IRSfs       ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C1IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C3IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C4IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_AC4IRSfs      ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_C1IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_C3IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_AC4IRSfs     ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_C1IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_C3IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_AC4IRSfs     ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32f_C1IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_C3IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_C4IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_AC4IR         ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_C1IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_C3IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_AC4IR        ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiMul_8u_C1RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_C3RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_C4RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_8u_AC4RSfs        ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C1RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C3RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_C4RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16s_AC4RSfs       ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_C1RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_C3RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_16sc_AC4RSfs      ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_C1RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_C3RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32sc_AC4RSfs      ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMul_32f_C1R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_C3R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_C4R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32f_AC4R          ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_C1R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_C3R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMul_32fc_AC4R         ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - MulC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Multiply by a constant
#Long - <Text>These functions step through an ROI in a source buffer and multiply the source data by a specified constant value.</Text>
<Text>The product can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMulC_8u_C1IRSfs       (       Fw8u   value   , Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_C3IRSfs       ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_AC4IRSfs      ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_C4IRSfs       ( const Fw8u   value[4], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C1IRSfs      (       Fw16s  value   , Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C3IRSfs      ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_AC4IRSfs     ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C4IRSfs      ( const Fw16s  value[4], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_C1IRSfs     (       Fw16sc value   , Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_C3IRSfs     ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_AC4IRSfs    ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_C1IRSfs     (       Fw32sc value   , Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_C3IRSfs     ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_AC4IRSfs    ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32f_C1IR         (       Fw32f  value   , Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_C3IR         ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_AC4IR        ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_C4IR         ( const Fw32f  value[4], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_C1IR        (       Fw32fc value   , Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_C3IR        ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_AC4IR       ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiMulC_8u_C1RSfs        ( const Fw8u   *pSrc, int srcStep,       Fw8u   value   , Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_C3RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_AC4RSfs       ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_8u_C4RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[4], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C1RSfs       ( const Fw16s  *pSrc, int srcStep,       Fw16s  value   , Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C3RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_AC4RSfs      ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16s_C4RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[4], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_C1RSfs      ( const Fw16sc *pSrc, int srcStep,       Fw16sc value   , Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_C3RSfs      ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_16sc_AC4RSfs     ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_C1RSfs      ( const Fw32sc *pSrc, int srcStep,       Fw32sc value   , Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_C3RSfs      ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32sc_AC4RSfs     ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiMulC_32f_C1R          ( const Fw32f  *pSrc, int srcStep,       Fw32f  value   , Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_C3R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_AC4R         ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32f_C4R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[4], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_C1R         ( const Fw32fc *pSrc, int srcStep,       Fw32fc value   , Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_C3R         ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulC_32fc_AC4R        ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - MulScale
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Multiply with scaling
#Long - <Text>These functions step through ROIs in two source buffers, multiply the data in buffer 1 by the data in buffer 2, then divide the product by a scaling factor equal to the upper bound of the range of values that can be represented by the source data type.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMulScale_8u_C1IR      ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_C3IR      ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_C4IR      ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_AC4IR     ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C1IR     ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C3IR     ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C4IR     ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_AC4IR    ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiMulScale_8u_C1R       ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_C3R       ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_C4R       ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_8u_AC4R      ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C1R      ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C3R      ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_C4R      ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulScale_16u_AC4R     ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - MulCScale
TODO:Publish
#Technologies - REF, MT, SSE2
#Short - Multiply by constant with scaling
#Long - <Text>These functions step through an ROI in a source buffer, multiply the source data by a specified constant value, then divide the product by a scaling factor equal to the upper bound of the range of values that can be represented by the source data type.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMulCScale_8u_C1IR     (       Fw8u  value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_C3IR     ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_AC4IR    ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_C4IR     ( const Fw8u  value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C1IR    (       Fw16u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C3IR    ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_AC4IR   ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C4IR    ( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiMulCScale_8u_C1R      ( const Fw8u  *pSrc, int srcStep,       Fw8u  value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_C3R      ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_AC4R     ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_8u_C4R      ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C1R     ( const Fw16u *pSrc, int srcStep,       Fw16u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C3R     ( const Fw16u *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_AC4R    ( const Fw16u *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiMulCScale_16u_C4R     ( const Fw16u *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - Sqr
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Square
#Long - <Text>These functions step through an ROI in a source buffer and calculate the square of the source data.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit unsigned, and 16-bit signed versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSqr_8u_C1IRSfs        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_C3IRSfs        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_C4IRSfs        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_AC4IRSfs       ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C1IRSfs       ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C3IRSfs       ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C4IRSfs       ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_AC4IRSfs      ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C1IRSfs       ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C3IRSfs       ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C4IRSfs       ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_AC4IRSfs      ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_32f_C1IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqr_32f_C3IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqr_32f_C4IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqr_32f_AC4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiSqr_8u_C1RSfs         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_C3RSfs         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_C4RSfs         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_8u_AC4RSfs        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C1RSfs        ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C3RSfs        ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_C4RSfs        ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16u_AC4RSfs       ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C1RSfs        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C3RSfs        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_C4RSfs        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_16s_AC4RSfs       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqr_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqr_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqr_32f_AC4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Sqrt
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Square root
#Long - <Text>These functions step through an ROI in a source buffer and calculate the square root of the source data.</Text>
<Text>Results can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit unsigned, and 16-bit signed versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSqrt_8u_C1IRSfs       ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_8u_C3IRSfs       ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_8u_AC4IRSfs      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_C1IRSfs      ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_C3IRSfs      ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_AC4IRSfs     ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_C1IRSfs      ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_C3IRSfs      ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_AC4IRSfs     ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_32f_C1IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqrt_32f_C3IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqrt_32f_C4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqrt_32f_AC4IR        ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiSqrt_8u_C1RSfs        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_8u_C3RSfs        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_8u_AC4RSfs       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_C1RSfs       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_C3RSfs       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16s_AC4RSfs      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_C1RSfs       ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_C3RSfs       ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_16u_AC4RSfs      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSqrt_32f_C1R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqrt_32f_C3R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSqrt_32f_AC4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Sub
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Subtract
#Long - <Text>These functions step through ROIs in two source buffers and subtract the data in buffer 1 from the data in buffer 2.</Text>
<Text>The difference can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSub_8u_C1IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_C3IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_C4IRSfs        ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_AC4IRSfs       ( const Fw8u   *pSrc, int srcStep, Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C1IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C3IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C4IRSfs       ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_AC4IRSfs      ( const Fw16s  *pSrc, int srcStep, Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_C1IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_C3IRSfs      ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_AC4IRSfs     ( const Fw16sc *pSrc, int srcStep, Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_C1IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_C3IRSfs      ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_AC4IRSfs     ( const Fw32sc *pSrc, int srcStep, Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32f_C1IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_C3IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_C4IR          ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_AC4IR         ( const Fw32f  *pSrc, int srcStep, Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_C1IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_C3IR         ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_AC4IR        ( const Fw32fc *pSrc, int srcStep, Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiSub_8u_C1RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_C3RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_C4RSfs         ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_8u_AC4RSfs        ( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2, int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C1RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C3RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_C4RSfs        ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16s_AC4RSfs       ( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2, int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_C1RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_C3RSfs       ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_16sc_AC4RSfs      ( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2, int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_C1RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_C3RSfs       ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32sc_AC4RSfs      ( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2, int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSub_32f_C1R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_C3R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_C4R           ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32f_AC4R          ( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2, int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_C1R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_C3R          ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSub_32fc_AC4R         ( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2, int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - SubC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Subtract a constant value
#Long - <Text>These functions step through an ROI in a source buffer and subtract a specified constant value from the source data.</Text>
<Text>The difference can be written back to the source location or to a destination buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiSubC_8u_C1IRSfs       (       Fw8u   value   , Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_C3IRSfs       ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_AC4IRSfs      ( const Fw8u   value[3], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_C4IRSfs       ( const Fw8u   value[4], Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C1IRSfs      (       Fw16s  value   , Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C3IRSfs      ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_AC4IRSfs     ( const Fw16s  value[3], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C4IRSfs      ( const Fw16s  value[4], Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_C1IRSfs     (       Fw16sc value   , Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_C3IRSfs     ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_AC4IRSfs    ( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_C1IRSfs     (       Fw32sc value   , Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_C3IRSfs     ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_AC4IRSfs    ( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32f_C1IR         (       Fw32f  value   , Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_C3IR         ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_AC4IR        ( const Fw32f  value[3], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_C4IR         ( const Fw32f  value[4], Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_C1IR        (       Fw32fc value   , Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_C3IR        ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_AC4IR       ( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiSubC_8u_C1RSfs        ( const Fw8u   *pSrc, int srcStep,       Fw8u   value   , Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_C3RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_AC4RSfs       ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[3], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_8u_C4RSfs        ( const Fw8u   *pSrc, int srcStep, const Fw8u   value[4], Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C1RSfs       ( const Fw16s  *pSrc, int srcStep,       Fw16s  value   , Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C3RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_AC4RSfs      ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[3], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16s_C4RSfs       ( const Fw16s  *pSrc, int srcStep, const Fw16s  value[4], Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_C1RSfs      ( const Fw16sc *pSrc, int srcStep,       Fw16sc value   , Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_C3RSfs      ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_16sc_AC4RSfs     ( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_C1RSfs      ( const Fw32sc *pSrc, int srcStep,       Fw32sc value   , Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_C3RSfs      ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32sc_AC4RSfs     ( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
FwStatus STDCALL fwiSubC_32f_C1R          ( const Fw32f  *pSrc, int srcStep,       Fw32f  value   , Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_C3R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_AC4R         ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[3], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32f_C4R          ( const Fw32f  *pSrc, int srcStep, const Fw32f  value[4], Fw32f  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_C1R         ( const Fw32fc *pSrc, int srcStep,       Fw32fc value   , Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_C3R         ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiSubC_32fc_AC4R        ( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - And
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - AND
#Long - <Text>These functions step through ROIs in two source buffers and perform a bitwise logical AND of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAnd_8u_C1IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_C3IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_C4IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_AC4IR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C1IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C3IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C4IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_AC4IR         ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C1IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C3IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C4IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_AC4IR         ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAnd_8u_C1R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_C3R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_C4R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_8u_AC4R           ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C1R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C3R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_C4R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_16u_AC4R          ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C1R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C3R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_C4R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAnd_32s_AC4R          ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - AndC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - AND with constant
#Long - <Text>These functions step through an ROI in a source buffer and perform a bitwise logical AND of the source data and a specified constant.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiAndC_8u_C1IR          (       Fw8u  value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_C3IR          ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_AC4IR         ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_C4IR          ( const Fw8u  value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C1IR         (       Fw16u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C3IR         ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_AC4IR        ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C4IR         ( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C1IR         (       Fw32s value   , Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C3IR         ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_AC4IR        ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C4IR         ( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiAndC_8u_C1R           ( const Fw8u   *pSrc, int srcStep,       Fw8u  value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_C3R           ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_AC4R          ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_8u_C4R           ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C1R          ( const Fw16u  *pSrc, int srcStep,       Fw16u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C3R          ( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_AC4R         ( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_16u_C4R          ( const Fw16u  *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C1R          ( const Fw32s  *pSrc, int srcStep,       Fw32s value   , Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C3R          ( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_AC4R         ( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiAndC_32s_C4R          ( const Fw32s  *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - Comp
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Complement
#Long - <Text>This function steps through an ROI in a source buffer, performs a logical complementation of the source data, and writes the results back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiComplement_32s_C1IR   ( Fw32s *pSrcDst, int srcDstStep,FwiSize roiSize );

/*#FunctionBlock - Not
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - NOT
#Long - <Text>These functions step through an ROI in a source buffer and perform a bitwise negation of the source data.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiNot_8u_C1IR           ( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_C3IR           ( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_AC4IR          ( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_C4IR           ( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);

FwStatus STDCALL fwiNot_8u_C1R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_C3R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_AC4R           ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiNot_8u_C4R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize);


/*#FunctionBlock - Or
todo:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - OR
#Long - <Text>These functions step through ROIs in two source buffers and perform a bitwise inclusive logical OR of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiOr_8u_C1IR            ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_C3IR            ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_C4IR            ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_AC4IR           ( const Fw8u  *pSrc,  int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C1IR           ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C3IR           ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C4IR           ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_AC4IR          ( const Fw16u *pSrc,  int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C1IR           ( const Fw32s *pSrc,  int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C3IR           ( const Fw32s *pSrc,  int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C4IR           ( const Fw32s *pSrc,  int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_AC4IR          ( const Fw32s *pSrc,  int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiOr_8u_C1R             ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_C3R             ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_C4R             ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_8u_AC4R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C1R            ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C3R            ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_C4R            ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_16u_AC4R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C1R            ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C3R            ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_C4R            ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOr_32s_AC4R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - OrC
todo:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - OR with constant
#Long - <Text>These functions step through an ROI in a source buffer and perform a bitwise inclusive logical OR of the source data and a specified constant.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiOrC_8u_C1IR           (       Fw8u  value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_C3IR           ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_AC4IR          ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_C4IR           ( const Fw8u  value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C1IR          (       Fw16u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C3IR          ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_AC4IR         ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C4IR          ( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C1IR          (       Fw32s value   , Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C3IR          ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_AC4IR         ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C4IR          ( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiOrC_8u_C1R            ( const Fw8u   *pSrc, int srcStep,       Fw8u  value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_C3R            ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_AC4R           ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_8u_C4R            ( const Fw8u   *pSrc, int srcStep, const Fw8u  value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C1R           ( const Fw16u  *pSrc, int srcStep,       Fw16u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C3R           ( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_AC4R          ( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_16u_C4R           ( const Fw16u  *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C1R           ( const Fw32s  *pSrc, int srcStep,       Fw32s value   , Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C3R           ( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_AC4R          ( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiOrC_32s_C4R           ( const Fw32s  *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - LShiftC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Left shift by a constant value
#Long - <Text>These functions step through an ROI in a source buffer and shift the bits of the source data a specified number of bit positions to the left.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
<Text>Left-shifting is equivalent to multiplying by 2 to the constant power.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiLShiftC_8u_C1IR       (       Fw32u value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_C3IR       ( const Fw32u value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_AC4IR      ( const Fw32u value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_C4IR       ( const Fw32u value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C1IR      (       Fw32u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C3IR      ( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_AC4IR     ( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C4IR      ( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C1IR      (       Fw32u value   , Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C3IR      ( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_AC4IR     ( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C4IR      ( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiLShiftC_8u_C1R        ( const Fw8u  *pSrc, int srcStep,       Fw32u value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_C3R        ( const Fw8u  *pSrc, int srcStep, const Fw32u value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_AC4R       ( const Fw8u  *pSrc, int srcStep, const Fw32u value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_8u_C4R        ( const Fw8u  *pSrc, int srcStep, const Fw32u value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C1R       ( const Fw16u *pSrc, int srcStep,       Fw32u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C3R       ( const Fw16u *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_AC4R      ( const Fw16u *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_16u_C4R       ( const Fw16u *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C1R       ( const Fw32s *pSrc, int srcStep,       Fw32u value   , Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C3R       ( const Fw32s *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_AC4R      ( const Fw32s *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLShiftC_32s_C4R       ( const Fw32s *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - RShiftC
todo:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Right shift by a constant value
#Long - <Text>These functions step through an ROI in a source buffer and shift the bits of the source data a specified number of bit positions to the right.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRShiftC_8u_C1IR       (       Fw32u value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_C3IR       ( const Fw32u value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_AC4IR      ( const Fw32u value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_C4IR       ( const Fw32u value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C1IR       (       Fw32u value   , Fw8s  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C3IR       ( const Fw32u value[3], Fw8s  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_AC4IR      ( const Fw32u value[3], Fw8s  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C4IR       ( const Fw32u value[4], Fw8s  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C1IR      (       Fw32u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C3IR      ( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_AC4IR     ( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C4IR      ( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C1IR      (       Fw32u value   , Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C3IR      ( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_AC4IR     ( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C4IR      ( const Fw32u value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C1IR      (       Fw32u value   , Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C3IR      ( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_AC4IR     ( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C4IR      ( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiRShiftC_8u_C1R        ( const Fw8u   *pSrc, int srcStep,       Fw32u value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_C3R        ( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_AC4R       ( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8u_C4R        ( const Fw8u   *pSrc, int srcStep, const Fw32u value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C1R        ( const Fw8s   *pSrc, int srcStep,       Fw32u value   , Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C3R        ( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_AC4R       ( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_8s_C4R        ( const Fw8s   *pSrc, int srcStep, const Fw32u value[4], Fw8s  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C1R       ( const Fw16u  *pSrc, int srcStep,       Fw32u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C3R       ( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_AC4R      ( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16u_C4R       ( const Fw16u  *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C1R       ( const Fw16s  *pSrc, int srcStep,       Fw32u value   , Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C3R       ( const Fw16s  *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_AC4R      ( const Fw16s  *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_16s_C4R       ( const Fw16s  *pSrc, int srcStep, const Fw32u value[4], Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C1R       ( const Fw32s  *pSrc, int srcStep,       Fw32u value   , Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C3R       ( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_AC4R      ( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRShiftC_32s_C4R       ( const Fw32s  *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Xor
todo:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - XOR
#Long - <Text>These functions step through ROIs in two source buffers and perform a bitwise exclusive logical OR (XOR) of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiXor_8u_C1IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_C3IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_C4IR           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_AC4IR          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C1IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C3IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C4IR          ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_AC4IR         ( const Fw16u *pSrc, int srcStep, Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C1IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C3IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C4IR          ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_AC4IR         ( const Fw32s *pSrc, int srcStep, Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiXor_8u_C1R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_C3R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_C4R            ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_8u_AC4R           ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C1R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C3R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_C4R           ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_16u_AC4R          ( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C1R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C3R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_C4R           ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXor_32s_AC4R          ( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2, int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - XorC
todo:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - XOR with constant
#Long - <Text>These functions step through an ROI in a source buffer and perform a bitwise exclusive logical OR (XOR) of the source data and a specified constant.</Text>
<Text>The results can be written back to the source location or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiXorC_8u_C1IR          (       Fw8u  value   , Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_C3IR          ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_AC4IR         ( const Fw8u  value[3], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_C4IR          ( const Fw8u  value[4], Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C1IR         (       Fw16u value   , Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C3IR         ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_AC4IR        ( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C4IR         ( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C1IR         (       Fw32s value   , Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C3IR         ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_AC4IR        ( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C4IR         ( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiXorC_8u_C1R           ( const Fw8u  *pSrc, int srcStep,       Fw8u  value   , Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_C3R           ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_AC4R          ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_8u_C4R           ( const Fw8u  *pSrc, int srcStep, const Fw8u  value[4], Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C1R          ( const Fw16u *pSrc, int srcStep,       Fw16u value   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C3R          ( const Fw16u *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_AC4R         ( const Fw16u *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_16u_C4R          ( const Fw16u *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C1R          ( const Fw32s *pSrc, int srcStep,       Fw32s value   , Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C3R          ( const Fw32s *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_AC4R         ( const Fw32s *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXorC_32s_C4R          ( const Fw32s *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );

/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Color Model Conversion Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that convert image data from one color model or space to another.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - RGBToYUV
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from RGB to YUV
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the YUV color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a YUV image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used: </Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 U = -0.147*R + 0.289*G + 0.436*B
 V = 0.615*R - 0.515*G - 0.100*B
Where:

The input RGB contains the range [0 to 255].
The output Y contains the range [0 to 255].
The output U contains the range [16 to 240], with 128 corresponding to zero.
The output V ranges from [-157 to 157].
</Pseudo>

<Text>In order to fit the range [0 to 255], a constant value of 128 is added and the values are saturated to the range [0 to 255].</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToYUV_8u_C3R       ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV_8u_AC4R      ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV_8u_P3R       ( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV_8u_C3P3R     ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );


/*#FunctionBlock - YUVToRGB
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YUV to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YUV color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>A YUV image (<ParameterName>pSrc</ParameterName>) declared in the ROI (roiSize) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYUVToRGB_8u_C3R       ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUVToRGB_8u_AC4R      ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUVToRGB_8u_P3R       ( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUVToRGB_8u_P3C3R     ( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst   , int dstStep, FwiSize roiSize );


/*#FunctionBlock - RGBToYUV422
todo:publish
#Technologies - REF
#Short - Convert from RGB to YUV with 4:2:2 sampling
#Long - <Text>These functions convert data in a source buffer from the RGB color model to the YUV color space with 4:2:2 chroma subsampling, and write the converted data to a destination buffer.</Text>
<Text>The repeating versions of the functions step through an ROI in a source buffer and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) is converted to a YUV image with 4:2:2 chroma subsampling.</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 U = -0.147*R + 0.289*G + 0.436*B
 V = 0.615*R - 0.515*G - 0.100*B
Where:

The input RGB contains the range [0 to 255].
The output Y contains the range [0 to 255].
The output U contains the range [16 to 240], with 128 corresponding to zero.
The output V ranges from [-157 to 157].
</Pseudo>

<Text>Versions of the functions that don't specify an ROI apply the conversion to the whole image buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiRGBToYUV422_8u_C3C2R  ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst   , int dstStep   , FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV422_8u_P3R    ( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst[3], int dstStep[3], FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV422_8u_P3     ( const Fw8u * const pSrc[3],              Fw8u *pDst[3],                 FwiSize imgSize );
FwStatus STDCALL fwiRGBToYUV422_8u_C3P3R  ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst[3], int dstStep[3], FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV422_8u_C3P3   ( const Fw8u *       pSrc   ,              Fw8u *pDst[3],                 FwiSize imgSize );

/*#FunctionBlock - YUV422ToRGB
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YUV with 4:2:2 sampling to RGB
#Long - <Text>These functions convert data in a source buffer from the YUV color space with 4:2:2 chroma subsampling to the RGB model, and write the converted data to a destination buffer.</Text>
<Text>The repeating versions of the functions step through an ROI in a source buffer and write the converted data to a destination buffer.</Text>
<Text>A YUV image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>Versions of the functions that don't specify an ROI apply the conversion to the whole image buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/

FwStatus STDCALL fwiYUV422ToRGB_8u_C2C3R  ( const Fw8u *       pSrc   , int srcStep,    Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV422ToRGB_8u_P3R    ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV422ToRGB_8u_P3     ( const Fw8u * const pSrc[3],                 Fw8u * const pDst[3],              FwiSize imgSize );
FwStatus STDCALL fwiYUV422ToRGB_8u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV422ToRGB_8u_P3AC4R ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV422ToRGB_8u_P3C3   ( const Fw8u * const pSrc[3],                 Fw8u *       pDst   ,              FwiSize imgSize );


/*#FunctionBlock - RGBToYUV420
todo:publish
#Technologies - REF
#Short - Convert from RGB to YUV  with 4:2:0 sampling
#Long - <Text>These functions convert the data in a source buffer from the RGB color model to the YUV color space with 4:2:0 chroma subsampling, and write the converted data to a destination buffer.</Text>
<Text>The repeating versions of the functions step through an ROI in a source buffer and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) is converted to a YUV image with 4:2:0 chroma subsampling.</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 Y = 0.299*R + 0.587*G + 0.114*B
 U = -0.147*R + 0.289*G + 0.436*B
 V = 0.615*R - 0.515*G - 0.100*B
Where:

The input RGB contains the range [0 to 255].
The output Y contains the range [0 to 255].
The output U contains the range [16 to 240], with 128 corresponding to zero.
The output V ranges from [-157 to 157].
</Pseudo>

<Text>Versions of the functions that don't specify an ROI apply the conversion to the whole image buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiRGBToYUV420_8u_P3R    ( const Fw8u * const pSrc[3], int srcStep, Fw8u * const pDst[3], int dstStep[3], FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV420_8u_P3     ( const Fw8u * const pSrc[3],              Fw8u * const pDst[3],                 FwiSize imgSize );
FwStatus STDCALL fwiRGBToYUV420_8u_C3P3R  ( const Fw8u *       pSrc   , int srcStep, Fw8u * const pDst[3], int dstStep[3], FwiSize roiSize );
FwStatus STDCALL fwiRGBToYUV420_8u_C3P3   ( const Fw8u *       pSrc   ,              Fw8u * const pDst[3],                 FwiSize imgSize );


/*#FunctionBlock - YUV420ToRGB
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YUV with 4:2:0 sampling to RGB
#Long - <Text>These functions convert data in an ROI in a source buffer from the YUV color space with 4:2:0 chroma subsampling to the RGB model, and write the converted data to a destination buffer.</Text>
<Text>The repeating versions of the functions step through an ROI in a source buffer and write the converted data to a destination buffer.</Text>
<Text>A YUV image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>Versions of the functions that don't specify an ROI apply the conversion to the whole image buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/

FwStatus STDCALL fwiYUV420ToRGB_8u_P3R    ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB_8u_P3     ( const Fw8u * const pSrc[3],                 Fw8u * const pDst[3],              FwiSize imgSize );
FwStatus STDCALL fwiYUV420ToRGB_8u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB_8u_P3AC4R ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB_8u_P3C3   ( const Fw8u * const pSrc[3],                 Fw8u *       pDst   ,              FwiSize imgSize );


/*#FunctionBlock - YUV420ToBGR
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Converts  from YUV with 4:2:0 sampling to BGR
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YUV color space with 4:2:0 chroma subsampling to the BGR model, and writes the converted data to a destination buffer.</Text>
<Text>A YUV image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYUV420ToBGR_8u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - YUV420ToRGB*
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YUV with 4:2:0 sampling to 16-bit per pixel RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YUV color space with 4:2:0 chroma subsampling to the 16-bit RGB model, and write the converted data to a destination buffer.</Text>
<Text>A YUV image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYUV420ToRGB565_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB555_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB444_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );


#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YUV420ToRGB*Dither
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YUV with 4:2:0 sampling to 16-bit RGB with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YUV color space with 4:2:0 chroma sub-sampling to the 16-bit RGB model with dithering, and write the converted data to a destination buffer.</Text>
<Text>*The function can use 565, 555, or 444 bit-per-color encoding for the destination RGB.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYUV420ToRGB565Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB555Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToRGB444Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif


/*#FunctionBlock - YUV420ToBGR*
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YUV with 4:2:0 sampling to 16-bit per pixel BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YUV color space with 4:2:0 chroma subsampling to the 16-bit BGR model, and write the converted data to a destination buffer.</Text>
<Text>A YUV image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYUV420ToBGR565_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToBGR555_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToBGR444_8u16u_P3C3R        ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YUV420ToBGR*Dither
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YUV with 4:2:0 sampling to 16-bit BGR with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YUV color space with 4:2:0 chroma sub-sampling to the 16-bit BGR model with dithering, and write the converted data to a destination buffer.</Text>
       <Text>*The function can use 565, 555, or 444 bit-per-color encoding for the destination BGR.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYUV420ToBGR565Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToBGR555Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYUV420ToBGR444Dither_8u16u_P3C3R  ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif


/*#FunctionBlock - RGBToYCbCr
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from RGB to YCbCr
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the YCbCr color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI (roiSize) is converted to a YCbCr image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToYCbCr_8u_C3R                 ( const Fw8u *       pSrc   , int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCbCr_8u_AC4R                ( const Fw8u *       pSrc   , int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCbCr_8u_P3R                 ( const Fw8u * const pSrc[3], int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );


/*#FunctionBlock - YCbCrToRGB
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from YCbCr to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the RGB model, and write the converted data to a destination buffer.</Text>
<Text>A YUV image (<ParameterName>pSrc</ParameterName>) declared in the ROI (roiSize) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCrToRGB_8u_C3R                 ( const Fw8u *       pSrc   , int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB_8u_AC4R                ( const Fw8u *       pSrc   , int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB_8u_P3R                 ( const Fw8u * const pSrc[3], int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB_8u_P3C3R               ( const Fw8u * const pSrc[3], int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCrToRGB*
todo:publish
#Technologies - REF, MT
#Short - Convert from YCbCr to 16-bit per pixel RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the 16-bit RGB model, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>
<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYCbCrToRGB565_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB555_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB444_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB565_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB555_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB444_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCrToRGB*Dither
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YCbCr to 16-bit RGB with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the 16-bit RGB model with dithering, and write the converted data to a destination buffer</Text>
       <Text>*The function can use 565, 555, or 444 bit-per-color encoding for the destination RGB.</Text>


#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYCbCrToRGB565Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB555Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB444Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB565Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB555Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToRGB444Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif


/*#FunctionBlock - YCbCrToBGR*
todo:publish
#Technologies - REF, MT
#Short - Convert from YCbCr to 16-bit per pixel BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the 16-bit BGR model, and write the converted data to a destination buffer.</Text>
<Text> A YCbCr image (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYCbCrToBGR565_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR555_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR444_8u16u_C3R           ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR565_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR555_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR444_8u16u_P3C3R         ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );


#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCrToBGR*Dither
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YCbCr to 16-bit BGR with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space to the 16-bit BGR model with dithering, and write the converted data to a destination buffer</Text>
       <Text>*The function can use 565, 555, or 444 bit-per-color encoding for the destination BGR.</Text>


#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCrToBGR565Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR555Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR444Dither_8u16u_C3R     ( const Fw8u *       pSrc   , int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR565Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR555Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCrToBGR444Dither_8u16u_P3C3R   ( const Fw8u * const pSrc[3], int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif


/*#FunctionBlock - RGBToYCbCr422
todo:publish
#Technologies - REF
#Short - Convert from RGB to 16-bit per pixel YCbCr with 4:2:2 sampling
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the YCbCr color space with 4:2:2 chroma subsampling, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) is converted to a 16-bit per pixel YCbCr image with 4:2:2 chroma subsampling.</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiRGBToYCbCr422_8u_C3C2R            ( const Fw8u *       pSrc   , int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCbCr422_8u_P3C2R            ( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCr422ToRGB
todo:publish
#Technologies - REF
#Short - Convert 16-bit per pixel YCbCr with 4:2:2 sampling to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:2 chroma subsampling to the RGB model, and write the converted data to a destination buffer.</Text>
<Text>A 16-bit per pixel YCbCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/

FwStatus STDCALL fwiYCbCr422ToRGB_8u_C2C3R            ( const Fw8u *       pSrc   , int srcStep   , Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB_8u_C2P3R            ( const Fw8u *       pSrc   , int srcStep   , Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB_8u_P3C3R            ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *       pDst   , int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToCbYCr422*
todo:publish
#Technologies - REF
#Short - Convert from RGB to 16-bit per pixel CbYCr with 4:2:2 sampling
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the CbYCr color space with 4:2:2 chroma subsampling, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) is converted to a 16-bit per pixel CbYCr image with 4:2:2 chroma subsampling.</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>

<Text>*A version of this function supports CbYCr gamma correction.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiRGBToCbYCr422_8u_C3C2R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToCbYCr422Gamma_8u_C3C2R       ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - CbYCr422ToRGB
todo:publish
#Technologies - REF
#Short - Convert 16-bit per pixel CbYCr with 4:2:2 sampling to RGB
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the CbYCr color space with 4:2:2 chroma subsampling to the RGB model, and writes the converted data to a destination buffer.</Text>
<Text>A 16-bit per pixel CbYCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiCbYCr422ToRGB_8u_C2C3R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - BGRToCbYCr422
todo:publish
#Technologies - REF
#Short - Convert from BGR to 16-bit per pixel CbYCr with 4:2:2 sampling
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the BGR color model to the CbYCr color space with 4:2:2 chroma subsampling, and writes the converted data to a destination buffer.</Text>
<Text>A gamma-corrected BGR image (<ParameterName>pSrc</ParameterName>) is converted to a 16-bit per pixel CbYCr image with 4:2:2 chroma subsampling.</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiBGRToCbYCr422_8u_AC4C2R           ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - CbYCr422ToBGR
todo:publish
#Technologies - REF
#Short - Convert 16-bit per pixel CbYCr with 4:2:2 sampling to BGR
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the CbYCr color space with 4:2:2 chroma subsampling to the BGR model, and writes the converted data to a destination buffer.</Text>
<Text>A 16-bit per pixel CbYCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiCbYCr422ToBGR_8u_C2C4R            ( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u alpha );

/*#FunctionBlock - YCbCr422ToRGB*
todo:publish
#Technologies - REF
#Short - Convert from 16-bit per pixel YCbCr with 4:2:2 sampling to 16-bit per pixel RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:2 chroma subsampling to the 16-bit RGB model, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr422ToRGB565_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB555_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB444_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB565_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB555_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB444_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCr422ToRGB*Dither
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YCbCr with 4:2:2 sampling to 16-bit RGB with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:2 chroma sub-sampling to the 16-bit RGB model with dithering, and write the converted data to a destination buffer.</Text>
       <Text>*The functions can use 565, 555, or 444 bit-per-color encoding for the destination RGB.</Text>


#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCr422ToRGB565Dither_8u16u_C2C3R( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB555Dither_8u16u_C2C3R( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB444Dither_8u16u_C2C3R( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB565Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB555Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToRGB444Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif

/*#FunctionBlock - YCbCr422ToBGR*
todo:publish
#Technologies - REF
#Short - Convert from 16-bit per pixel YCbCr with 4:2:2 sampling to 16-bit per pixel BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:2 chroma subsampling to the 16-bit BGR model, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr422ToBGR565_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR555_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR444_8u16u_C2C3R      ( const Fw8u *       pSrc   , int srcStep   , Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR565_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR555_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR444_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCr422ToBGR*Dither
TODO:publish
#Technologies - REF, MT, SSE2
   #Short - Convert from YCbCr with 4:2:2 sampling to 16-bit BGR with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:2 chroma subsampling to the 16-bit BGR model with dithering, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:2 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCr422ToBGR565Dither_8u16u_C2C3R( const Fw8u *          pSrc, int    srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR555Dither_8u16u_C2C3R( const Fw8u *          pSrc, int    srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR444Dither_8u16u_C2C3R( const Fw8u *          pSrc, int    srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR565Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR555Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr422ToBGR444Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif

/*#FunctionBlock - RGBToYCbCr420
todo:publish
#Technologies - REF, SSE2
#Short - Convert from RGB to YCbCr with 4:2:0 sampling
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the RGB color model to the YCbCr color space with 4:2:0 chroma subsampling, and writes the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) is converted to a YCbCr image with 4:2:0 chroma subsampling.</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiRGBToYCbCr420_8u_C3P3R            ( const Fw8u *pSrc, int srcStep, Fw8u * const pDst[3], int dstStep[3], FwiSize roiSize );


/*#FunctionBlock - BGRToYCbCr420
todo:publish
#Technologies - REF
#Short - Convert from BGR to YCbCr with 4:2:0 sampling
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the BGR color model to the YCbCr color space with 4:2:0 chroma subsampling, and writes the converted data to a destination buffer.</Text>
<Text>A gamma-corrected BGR image (<ParameterName>pSrc</ParameterName>) is converted to a YCbCr image with 4:2:0 chroma subsampling.</Text>
<Pseudo>
 Y = 0.257*R + 0.504*G + 0.098*B + 16
 Cb = -0.148*R - 0.291*G + 0.439*B + 128
 Cr = 0.439*R - 0.368*G - 0.071*B + 128
</Pseudo>
<Text>The input RGB contains the range [0 to 255].</Text>
<Text>The output Y nominally contains the range [16 to 235].</Text>
<Text>The output Cb and Cr contains the range [16 to 240], with 128 corresponding to zero.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiBGRToYCbCr420_8u_C3P3R            ( const Fw8u *pSrc, int srcStep, Fw8u * const pDst[3], int dstStep[3], FwiSize roiSize);
FwStatus STDCALL fwiBGRToYCbCr420_8u_AC4P3R           ( const Fw8u *pSrc, int srcStep, Fw8u * const pDst[3], int dstStep[3], FwiSize roiSize);

/*#FunctionBlock - YCbCr420ToRGB
todo:publish
#Technologies - REF, MT, SSE2
#Short - Convert from YCbCr with 4:2:0 sampling to RGB
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YCbCr color space with 4:2:0 chroma subsampling to the RGB color model, and writes the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr420ToRGB_8u_P3C3R            ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u  *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - YCbCr422ToYCbCr420
todo:publish
#Technologies - REF
#Short - Convert from YCbCr with 4:2:2 sampling to YCbCr with 4:2:0 sampling
#Long - <Text>These functions step through an ROI in a source buffer or buffers, convert the source data from the YCbCr color space with 4:2:2 chroma subsampling to YCbCr color space with 4:2:0 chroma subsampling, and writes the converted data to a destination buffer or buffers.</Text>
<Text>The source buffer can be two-channel or three-planar data. The destination is planar data with two or three planes.</Text>
<Text> The format of the two-channel data is Y1, Cb1, Y2, Cr1, Y3, Cb2, Y4, Cr2... etc</Text>
<Text> The format of the two-plane data is Y1, Y2, Y3, Y4... etc in the first plane and Cb1, Cr1, Cb2, Cr2... etc in the second plane.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL  fwiYCbCr422ToYCbCr420_8u_P3R ( const Fw8u* pSrc[3], int srcStep[3], Fw8u* pDst[3], int dstStep[3], FwiSize roiSize);
FwStatus STDCALL  fwiYCbCr422ToYCbCr420_8u_C2P3R(const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize);


/*#FunctionBlock - YCbCr420ToRGB*
todo:publish
#Technologies - REF
#Short - Convert from YCbCr with 4:2:0 sampling to 16-bit per pixel RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:0 chroma subsampling to the 16-bit RGB color model, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
 R = Y + 1.140*V
 G = Y - 0.394*U - 0.581*V
 B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
 RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
 RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
 RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr420ToRGB565_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToRGB555_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToRGB444_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );


#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCr420ToRGB*Dither
TODO:publish
#Technologies - REF, MT, SSE2
   #Short - Convert from YCbCr with 4:2:0 sampling to 16-bit RGB with dithering

#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:2:0 chroma subsampling to the 16-bit RGB color model with dithering, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following conversion formulas are used:</Text>
<Pseudo>
R = Y + 1.140*V
G = Y - 0.394*U - 0.581*V
B = Y + 2.032*U
</Pseudo>
<Text>*The destination image is a packed RGB 16-bit per pixel image with reduced bit depth. The three channel intensities are packed into two consecutive bytes. After the conversion is performed, the bit reduction discards the least significant bits in the image.</Text>

<Text>There are 3 possible packed formats:</Text>
<Pseudo>
RGB565 - 5 bits for Red, 6 bits for Green, 5 bits for Blue
RGB555 - 5 bits for Red, 5 bits for Green, 5 bits for Blue
RGB444 - 4 bits for Red, 4 bits for Green, 4 bits for Blue
</Pseudo>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCr420ToRGB565Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToRGB555Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToRGB444Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif

/*#FunctionBlock - YCbCr420ToBGR
todo:publish
#Technologies - REF
#Short - Convert from YCbCr with 4:2:0 sampling to BGR
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YCbCr color space with 4:2:0 chroma subsampling to the BGR color model with dithering, and writes the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr420ToBGR_8u_P3C3R            ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - YCbCr420ToBGR*
todo:publish
#Technologies - REF
#Short - Convert from YCbCr with 4:2:0 sampling to 16-bit BGR
#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YCbCr color space with 4:2:0 chroma subsampling to the BGR color model, and writes the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr420ToBGR565_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToBGR555_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToBGR444_8u16u_P3C3R      ( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );


#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - YCbCr420ToBGR*
TODO:publish
#Technologies - REF, MT, SSE2
   #Short - Convert from YCbCr with 4:2:0 sampling to 16-bit BGR with dithering

#Long - <Text>This function steps through an ROI in a source buffer, converts the source data from the YCbCr color space with 4:2:0 chroma subsampling to the BGR color model with dithering, and writes the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:2:0 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
R = 1.164*(Y-16) + 1.596*(Cr-128)
G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCbCr420ToBGR565Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToBGR555Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr420ToBGR444Dither_8u16u_P3C3R( const Fw8u * const pSrc[3], int srcStep[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
#endif


/*#FunctionBlock - YCbCr411ToBGR
todo:publish
#Technologies - REF
#Short - Convert from YCbCr with 4:1:1 sampling to BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCbCr color space with 4:1:1 chroma subsampling to the BGR color model, and write the converted data to a destination buffer.</Text>
<Text>A YCbCr image with 4:1:1 chroma subsampling (<ParameterName>pSrc</ParameterName>) is converted to a gamma-corrected BGR image (<ParameterName>pDst</ParameterName>).</Text>
<Text> The following conversion formulas are used:</Text>
<Pseudo>
 R = 1.164*(Y-16) + 1.596*(Cr-128)
 G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)
 B = 1.164*(Y-16) + 2.017*(Cb-128)
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDoubleSize
*/
FwStatus STDCALL fwiYCbCr411ToBGR_8u_P3C3R            ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCbCr411ToBGR_8u_P3C4R            ( const Fw8u * const pSrc[3], int srcStep[3], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u alpha );


/*#FunctionBlock - RGBToXYZ
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from RGB to XYZ
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the XYZ color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a CIE XYZ image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following formulas are used:</Text>
<Pseudo>
 X = 0.412453*R + 0.35758 *G + 0.180423*B
 Y = 0.212671*R + 0.71516 *G + 0.072169*B
 Z = 0.019334*R + 0.119193*G + 0.950227*B
</Pseudo>
<Text>The formulas assume that R, G, and B values are normalized to [0 to 1] for integer data types.</Text>
<Text>For floating point data types, the data must already be in the range [0 to 1]</Text>
<Text>For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range</Text>
<Text>For floating point data type, data is saturated to [0 to 1].</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToXYZ_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToXYZ_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - XYZToRGB
todo:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from XYZ to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the XYZ color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>A CIE XYZ image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>).</Text>
<Text>The following formulas are used:</Text>
<Pseudo>
 R =  3.240479*R - 1.537150*G - 0.498535*B
 G = -0.969256*R + 1.875991*G + 0.041556*B
 B =  0.055648*R - 0.204043*G + 1.057311*B

 The formulas assume that R, G, and B values are normalized to [0 to 1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.
 For floating point data type, data is saturated to [0 to 1].
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiXYZToRGB_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiXYZToRGB_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - RGBToLUV
todo:publish
#Technologies - REF, MT, SSE2
#Short - Convert from RGB to LUV
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the LUV color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a CIE LUV image (<ParameterName>pDst</ParameterName>) in two steps.</Text>
<Pseudo>
1. Convert( ) RGBToXYZ( ) using the following formulas.

 X = 0.412453*R + 0.35758 *G + 0.180423*B
 Y = 0.212671*R + 0.71516 *G + 0.072169*B
 Z = 0.019334*R + 0.119193*G + 0.950227*B

 The formulas assume that R, G, and B values are normalized to [0 to 1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.
 For floating point data type, data is saturated to [0 to 1].

2. Convert( ) XYZ to LUV as follows.

D65 white point:

 CIE chromaticity coordinates:

  xn = 0.312713
  yn = 0.329016

 CIE luminance:

  Yn = 1.0

 un = 4*xn / (-2*xn + 12*yn + 3)
 vn = 9*yn / (-2*xn + 12*yn + 3)
 u = 4*X / (X + 15*Y + 3*Z)
 v = 9*Y / (X + 15*Y + 3*Z)
 L = 116 * (Y/Yn)^(1/3) - 16
 U = 13*L*(u-un)
 V = 13*L*(v-vn)

 Computed L component values are in the range [0 to 100].
 Computed U component values are in the range [-124 to 220].
 Computed V component values are in the range [-140 to 116].

 The formulas assume that R, G, and B values are normalized to [0 to1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.

Scaling is performed as follows.

8U data type:

 L = L * FW_MAX_8U / 100
 U = (U + 134) * FW_MAX_8U / 354
 V = (V + 140) * FW_MAX_8U / 256

16U data type:

 L = L * FW_MAX_16U / 100
 U = (U + 134) * FW_MAX_16U / 354
 V = (V + 140) * FW_MAX_16U / 256

16S data type:

 L = L * FW_MAX_16U / 100 + FW_MIN_16S
 U = (U + 134) * FW_MAX_16U / 354 + FW_MIN_16S
 V = (V + 140) * FW_MAX_16U / 256 + FW_MIN_16S

32F data type:

 No conversion is applied, so the components remain in their ranges.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToLUV_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToLUV_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - LUVToRGB
todo:publish
#Technologies - REF, MT
#Short - Convert from LUV to RGB
#Long - <Text>A CIE LUV image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) in two steps.</Text>
<Pseudo>
1. Convert( ) CIE LUV to XYZ as follows.

Scale( ) data to the range [0 to 1]

8U data type:

 L = L * 100 / FW_MAX_8U
 U = (U * 354 / FW_MAX_8U) - 134
 V = (V * 256 / FW_MAX_8U) - 140

16U data type:

 L = L * 100 / FW_MAX_16U
 U = (U * 354 / FW_MAX_16U) - 134
 V = (V * 256 / FW_MAX_16U) - 140

16S data type:

 U = ((U - FW_MIN_16S) * 354 / FW_MAX_16U) - 134
 V = ((V - FW_MIN_16S) * 256 / FW_MAX_16U) - 140

32F data type:

 L, U, and V must already be in the range [0...1]

D65 white point:

 CIE chromaticity coordinates:

  xn = 0.312713
  yn = 0.329016

 CIE luminance:

  Yn = 1.0

 un = 4*xn / (-2*xn + 12*yn + 3)
 vn = 9*yn / (-2*xn + 12*yn + 3)
 u = U / (14 * L) + un
 v = V / (13 * L) + vn
 Y = Yn * ((L + 16) / 116) ^ 3
 X = -9 * Y * u / ((u - 4) * v - u * v) = (9 / 4) * Y * u / v
 Z = (9 * Y - 15 * v * Y - v * X) / (3 * v)

2.  Convert( ) XYZ to RGB using the following formulas:

 X = 3.240479*X - 1.53715 *Y - 0.498535*Z
 Y = -0.969256*X + 1.875991*Y + 0.041556*Z
 Z = 0.055648*X - 0.204043*Y + 1.057311*Z

 The formulas assume that X, Y, and Z values are normalized to [0 to 1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.
 For floating point data type, data is saturated to [0 to 1].
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiLUVToRGB_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLUVToRGB_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - BGRToLab
todo:publish
#Technologies - REF, MT
#Short - Convert from BGR to Lab
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the BGR color model to the Lab color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a CIE Lab image (<ParameterName>pDst</ParameterName>) in two steps.</Text>
<Pseudo>
1. Normalize the data to [0 to 1] and convert RGB to XYZ using the following formulas.

 X = 0.412453*R + 0.35758 *G + 0.180423*B
 Y = 0.212671*R + 0.71516 *G + 0.072169*B
 Z = 0.019334*R + 0.119193*G + 0.950227*B

 The formulas assume that R, G, and B values are normalized to [0 to 1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.
 For floating point data type, data is saturated to [0 to 1].

2. Convert( ) XYZ To Lab as follows.

D65 white point:

 Xn = 0.950455
 Yn = 1.0
 Zn = 1.088753

 CIE chromaticity coordinates:

  xn = 0.312713
  yn = 0.329016

 if ((Y / Yn) > 0.008856)
 L = 116 * (Y/Yn)^(1/3) - 16 else L = 903.3 * (Y/Yn)^(1/3)
 a = 500 * [f(X/Xn) - f(Y/Yn)]
 b = 200 * [f(Y/Yn) - f(Z/Zn)]

  where if (t > 0.008856) f(t) = t^(1/3) - 16 else f(t) = 7.787*t + 16/116

 The computed L, a, and b are in the range [0 to 100], [-128 to 127], and [-128b to 127], respectively.

 The computed image data is quantized and scaled to fit the 8-bit or 16-bit range.

8U data type:

 L = L * 255/100
 a = a + 128
 b = b + 128

16u data type:

 L = L * 65535 / 100
 a = (a+128) * 255
 b = (b+128) * 255
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiBGRToLab_8u_C3R       ( const Fw8u *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToLab_8u16u_C3R    ( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - LabToBGR
todo:publish
#Technologies - REF, MT
#Short - Convert from Lab to BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the Lab color space to the BGR color model, and write the converted data to a destination buffer.</Text>
<Text>A CIE Lab image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) in two steps.</Text>
<Pseudo>
1. Transform the data to the correct range as follows.

8U data type:

 L = L * 100 / 255
 a = a - 128
 b = b - 128

16u data type:

 L = L * 100 / 65535
 a = a / 255 - 128
 b = b / 255 - 128

Convert to XYZ:

 Y = Yn  *p^3
 X = Xn * (P + a/500)^3
 Z = Zn * (P - b/200)^2

 where P = (L+16)/116

D65 white point:

 Xn = 0.950455
 Yn = 1.0 Zn = 1.088753

2.  Convert( ) XYZ to BGR using the following formulas:

 X = 3.240479*X - 1.53715 *Y - 0.498535*Z
 Y = -0.969256*X + 1.875991*Y + 0.041556*Z
 Z = 0.055648*X - 0.204043*Y + 1.057311*Z

 The formulas assume that X, Y, and Z values are normalized to [0 to 1] for integer data types.
 For floating point data types, the data must already be in the range [0 to 1].
 For integer data types, the converted image data is saturated to [0 to 1] and scaled to the data type range.
 For floating point data type, data is saturated to [0 to 1].
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiLabToBGR_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiLabToBGR_16u8u_C3R    ( const Fw16u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToYCC
todo:publish
#Technologies - REF, MT
#Short - Convert from RGB to YCC
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the YCC color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a Photo YCC image (<ParameterName>pDst</ParameterName>) in four steps.</Text>
<Pseudo>
1.  Normalize the input to the range [0 to 1] for all integer data types. Floating-point data must already be in the range [0...1].

2.  Apply the following formulas:

 Y = 0.299*R + 0.587*G + 0.114*B
 C1 = -0.299*R - 0.587*G + 0.886*B
 C2 = 0.701*R - 0.587*G - 0.114*B

3. Quantize the color model and convert to the Photo YCC model. Data is in the range [0 to 1].

 Y = (1/1.402) * Y
 C1 = (111.4/255) * C1 + 156/255
 C2 = (135.64/255) * C2 + 137/255

4. For integer destination data types, the Photo YCC color model is scaled to the full range.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToYCC_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToYCC_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - YCCToRGB
todo:publish
#Technologies - REF, MT
#Short - Convert from YCC to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the YCC color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>A Photo YCC image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
Restore normal YCC data:

 Y = 1.3584 * Y
 C1 = 2.2179 * (C1 - 156/255)
 C2 = 1.8215 * (C2 - 137/255)

 The formulas assume that Y, C1, and C2 are normalized to [0 to 1].
 For floating point data type, the input must already be in the range [0 to 1].
 For integer data types, an internal conversion is performed, then the restored YCC data is transformed to RGB.

The following conversion formulas are used:

 R = Y + C2
 G = Y - 0.194*C1 - 0.509*C2
 B = Y + C1

 For integer destination data types, the result image data is scaled to the full range of the destination data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiYCCToRGB_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiYCCToRGB_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToHLS
todo:publish
#Technologies - REF, MT
#Short - Convert from RGB to HLS
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the HLS color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to an HLS image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
// Lightness:
M1 = max(R, G, B)
M2 = min(R, G, B)
L = (M1+M2)/2

// Saturation:
If (M1 == M2)
 S = H = 0
else // chromatics case

{
 if (L <= 0.5)
  S = (M1-M2) / (M1+M2)
 else
  S = (M1-M2) / (2-M1-M2)

 //Hue:
 Cr = (M1-R) / (M1-M2)
 Cg = (M1-G) / (M1-M2)
 Cb = (M1-B) / (M1-M2)
 if (M1 == R)
  H = Cb - Cg
 if (M1 == G)
  H = 2 + Cr - Cb
 if (M1 == B)
  H = 4 + Cg - Cr
  H = H*60
 if (H < 0)
  H = H + 360
}

The  formulas assume that R, G, and B values are in the range [0 to 1].
For integer destination data type, the values are scaled to the full range of the data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToHLS_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHLS_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - HLSToRGB
todo:publish
#Technologies - REF, MT
#Short - Convert from HLS to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the HLS color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>An HLS image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
if ( L <= 0.5)
 M2 = L * (1+S)
else
 M2 = L + S - L*S
 M1 = 2*L - M2
if (S == 0)
 R = G = B = L
else
{
 h = H + 120
 if (h > 360)
  h = h - 360
 if (h < 60)
  R = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  R = M2
 else if (h < 240)
  R = M1 + (M2 - M1)*(240 - h)/60
 else
  R = M1
 h = H
 if (h < 60)
  G = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  G = M2
 else if (h < 240)
  G = M1 + (M2 - M1)*(240 - h)/60
 else
  G = M1
 h = H - 120
 if (h < 60)
  B = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  B = M2
 else if (h < 240)
  B = M1 + (M2 - M1)*(240 - h)/60
 else
  B = M1
}

The formulas assume that H, L, and S values are in the range [0 to 1].
For integer destination data type, the values are scaled to the full range of the data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiHLSToRGB_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToRGB_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );


/*#FunctionBlock - BGRToHLS
todo:publish
#Technologies - REF, MT
#Short - Convert from BGR to HLS
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the BGR color model to the HLS color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected BGR image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to an HLS image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
// Lightness:
M1 = max(R, G, B)
M2 = min(R, G, B)
L = (M1+M2)/2
// Saturation:
If (M1 == M2)
 S = H = 0
else // chromatics case
{
 if (L <= 0.5)
  S = (M1-M2) / (M1+M2)
 else
  S = (M1-M2) / (2-M1-M2)
 //Hue:
 Cr = (M1-R) / (M1-M2)
 Cg = (M1-G) / (M1-M2)
 Cb = (M1-B) / (M1-M2)
 if (M1 == R)
  H = Cb - Cg
 if (M1 == G)
  H = 2 + Cr - Cb
 if (M1 == B)
  H = 4 + Cg - Cr
  H = H * 60
 if (H < 0)
  H = H + 360
}

The formulas assume that R, G, and B values are in the range [0 to 1].
For integer destination data type, the values are scaled to the full range of the data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiBGRToHLS_8u_AC4R      ( const Fw8u *       pSrc   , int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_C3P3R     ( const Fw8u *       pSrc   , int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_AC4P4R    ( const Fw8u *       pSrc   , int srcStep, Fw8u * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_AP4R      ( const Fw8u * const pSrc[4], int srcStep, Fw8u * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_P3R       ( const Fw8u * const pSrc[3], int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_AP4C4R    ( const Fw8u * const pSrc[4], int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiBGRToHLS_8u_P3C3R     ( const Fw8u * const pSrc[3], int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );


/*#FunctionBlock - HLSToBGR
todo:publish
#Technologies - REF, MT
#Short - Convert from HLS to BGR
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the HLS color space to the BGR color model, and write the converted data to a destination buffer.</Text>
<Text>An HLS image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
if ( L <= 0.5)
 M2 = L * (1+S)
else
 M2 = L + S - L*S
 M1 = 2*L - M2
if (S == 0)
 R = G = B = L
else
{
 h = H + 120
 if (h > 360)
  h = h - 360
 if (h < 60)
  R = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  R = M2
 else if (h < 240)
  R = M1 + (M2 - M1)*(240 - h)/60
 else
  R = M1
  h = H
 if (h < 60)
  G = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  G = M2
 else if (h < 240)
  G = M1 + (M2 - M1)*(240 - h)/60
 else
  G = M1
  h = H - 120
 if (h < 60)
  B = (M1 + (M2 - M1)*h/60)
 else if (h < 180)
  B = M2
 else if (h < 240)
  B = M1 + (M2 - M1)*(240 - h)/60
 else
  B = M1
}

The formulas assume that H, L, and S values are in the range [0 to 1].
For integer destination data type, the values are scaled to the full range of the data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiHLSToBGR_8u_C3P3R     ( const Fw8u *       pSrc   , int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToBGR_8u_AC4P4R    ( const Fw8u *       pSrc   , int srcStep, Fw8u * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToBGR_8u_AP4R      ( const Fw8u * const pSrc[4], int srcStep, Fw8u * const pDst[4], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToBGR_8u_P3R       ( const Fw8u * const pSrc[3], int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToBGR_8u_AP4C4R    ( const Fw8u * const pSrc[4], int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHLSToBGR_8u_P3C3R     ( const Fw8u * const pSrc[3], int srcStep, Fw8u *       pDst   , int dstStep, FwiSize roiSize );

/*#FunctionBlock - RGBToHSV
todo:publish
#Technologies - REF, MT
#Short - Convert from RGB to HSV
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the RGB color model to the HSV color space, and write the converted data to a destination buffer.</Text>
<Text>A gamma-corrected RGB image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to an HSV image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
Value:

V = max(R, G, B)

Saturation:

temp = min = (R, G, B)
if (V == 0)
 achromatics case
 S = 0
else
 chromatics case
 S = (V-temp)/V
Hue:

Cr = (V - R) / (V - temp)
Cg = (V - G) / (V - temp)
Cb = (V - B) / (V - temp)
if (R==V)
 H = Cb - Cg
if (G==V)
 H = 2 + Cr - Cb
if (B==V)
 H = 4 + Cg - Cr
 H = H * 60
if (H < 0)
 H = H + 360

The formulas assume R, G, and B values are in the range [0 to 1].
For integer data types, the computed image data are scaled to the full range of the destination data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToHSV_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHSV_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHSV_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiRGBToHSV_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - HSVToRGB
todo:publish
#Technologies - REF, MT
#Short - Convert from HSV to RGB
#Long - <Text>These functions step through an ROI in a source buffer, convert the source data from the HSV color space to the RGB color model, and write the converted data to a destination buffer.</Text>
<Text>An HSV image (<ParameterName>pSrc</ParameterName>) declared in the ROI is converted to a gamma-corrected RGB image (<ParameterName>pDst</ParameterName>) as follows.</Text>
<Pseudo>
if (S==0)
 R = G = B = V
else
{
 if (H == 360)
  H = 0
 else
  H = H/60
 I = floor(H)
 F = H - I
 M = V * (1 - S)
 N = V * (1 - S * F)
 K = V * (1 - s * (1 - F))
 if (I == 0)
  R = V
  G = K
  B = M
 if (I == 1)
  R = N
  G = V
  B = M
 if (I == 2)
  R = M
  G = V
  B = K
 if (I == 3)
  R = M
  G = N
  B = V
 if (I == 4)
  R = K
  G = M
  B = V
 if (I == 5)
  R = V
  G = M
  B = V
}

The computed image data is then scaled to the full range of the destination data type.
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiHSVToRGB_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHSVToRGB_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHSVToRGB_16u_C3R      ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiHSVToRGB_16u_AC4R     ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );




/*#FunctionBlock - ColorToGray
todo:publish
#Technologies - REF, SSE2
#Short - Convert a three-channel color image to gray scale using custom coefficients
#Long - <Text>The C3C1 version of the function steps through an ROI in a source buffer, converts a three-channel color image to a gray scale image and writes the converted data to a destination buffer.</Text>
<Text>The AC4C1 version of the function steps through an ROI in a source buffer, converts a four-channel color image to a gray scale image and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiColorToGray_8u_C3C1R(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_16u_C3C1R(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_16s_C3C1R(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_32f_C3C1R(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_8u_AC4C1R(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_16u_AC4C1R(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_16s_AC4C1R(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
FwStatus STDCALL fwiColorToGray_32f_AC4C1R(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);


/*#FunctionBlock - RGBToGray
todo:publish
#Technologies - REF, SSE2
#Short - Convert a three-channel RGB color image to gray scale using fixed coefficients
#Long - <Text>The C3C1 version of the function steps through an ROI in a source buffer, converts a three-channel RGB color image to a gray scale image and writes the converted data to a destination buffer.</Text>
<Text>The AC4C1 version of the function steps through an ROI in a source buffer, converts a four-channel color image to a gray scale image and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiRGBToGray_8u_C3C1R(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_16u_C3C1R(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_16s_C3C1R(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_32f_C3C1R(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_8u_AC4C1R(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_16u_AC4C1R(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_16s_AC4C1R(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize);
FwStatus STDCALL fwiRGBToGray_32f_AC4C1R(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize);



/*#FunctionBlock - YCbCr422
todo:publish
#Technologies - REF, MT
#Short - Convert 16-bit per pixel YCbCr with 4:2:2 sampling between 2-channel and 3-plane formats
#Long - <Text>The C2P3R version of the function steps through an ROI in a source buffer, converts a 2-channel YCbCr color image with 4:2:2 chroma subsampling to to a 3-plane image, and writes the converted data to a destination buffer.</Text>
<Text>The P3C2R version of the function steps through an ROI in a source buffer, converts a 3-plane YCbCr color image with 4:2:2 chroma subsampling to to a 2-channel image, and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiYCbCr422_8u_C2P3R     (const Fw8u *       pSrc   , int srcStep   , Fw8u *pDst[3], int dstStep[3], FwiSize roiSize);
FwStatus STDCALL fwiYCbCr422_8u_P3C2R     (const Fw8u * const pSrc[3], int srcStep[3], Fw8u *pDst   , int dstStep   , FwiSize roiSize);


/*#FunctionBlock - CbYCr422ToYCbCr
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert from CbYCr 4:2:2 to YCbCr 4:2:0
#Long - <Text>This function steps through an ROI in a source buffer, converts a 2-channel CbYCr color image with 4:2:2 chroma subsampling to a 2-channel YCbCr color image with 4:2:0 chroma subsampling, and writes the converted data to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiCbYCr422ToYCbCr420_8u_C2P3R       ( const Fw8u *pSrc, int srcStep, Fw8u *pDst[3], int dstStep[3], FwiSize roiSize );

/*#Documentation
</Chapter>

<Chapter>
<ChapterHeading>Statistical Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes FW functions which perform statistical operations on image data.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - Sum
todo:publish
#Technologies - REF, SSE2
#Short - Sum image data
#Long - <Text>These functions step through an ROI in a source buffer, sum the image data, and write the sum to a destination buffer.</Text>
*/

FwStatus STDCALL fwiSum_8u_C1R (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f* pSum);
FwStatus STDCALL fwiSum_16s_C1R (const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f* pSum);
FwStatus STDCALL fwiSum_8u_C3R(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
FwStatus STDCALL fwiSum_16s_C3R (const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
FwStatus STDCALL fwiSum_8u_AC4R(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
FwStatus STDCALL fwiSum_16s_AC4R(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
FwStatus STDCALL fwiSum_8u_C4R(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[4]);
FwStatus STDCALL fwiSum_16s_C4R(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[4]);
FwStatus STDCALL fwiSum_32f_C3R (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3], FwHintAlgorithm hint);
FwStatus STDCALL fwiSum_32f_AC4R (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3], FwHintAlgorithm hint);
FwStatus STDCALL fwiSum_32f_C4R (const Fw32f* pSrc, int srcStep, FwiSize roiSize, Fw64f sum[4], FwHintAlgorithm hint);


/*#FunctionBlock - Min
todo:publish
#Technologies - REF, SSE2
#Short - Min of image data
#Long - <Text>These functions step through an ROI in a source buffer and finds the minimum value in the buffer</Text>
*/


FwStatus STDCALL fwiMin_8u_C1R(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMin);
FwStatus STDCALL fwiMin_16s_C1R(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMin);
FwStatus STDCALL fwiMin_32f_C1R(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMin);



/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Threshold and Compare Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that compare image data and manipulate image data based on compare operations.</ChaptDesc></Paragraph> 
<Note>All the threshold and compare functions support negative step sizes. Negative step sizes can be used to "flip" data copied from a source buffer to a destination buffer. 
This is a common requirement when displaying a BMP image. To flip an image horizontally, call the copy function with <ParameterName>pSrc</ParameterName> pointing to the last row 
of the source buffer, a negative <ParameterName>srcStep</ParameterName>, <ParameterName>pDst</ParameterName> pointing to the start of the destination buffer, and a positive 
<ParameterName>dstStep</ParameterName>.</Note>
*/

/*#FunctionBlock - Threshold
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold, replace with threshold value (General)
#Long - <Text>These functions step through an ROI in a source buffer and compare the source data to a specified threshold value using a specified compare operation.</Text>
<Text>The compare operation can be "less than", "less than or equal", "equal", "greater than or equal" or "greater than".</Text>
<Text>When the comparison evaluates as true, the output data is set to the threshold value.</Text>
<Text>When the comparison evaluates as false, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/

FwStatus STDCALL fwiThreshold_8u_C1IR                 ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_8u_C3IR                 ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_8u_AC4IR                ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_C1IR                ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_C3IR                ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_AC4IR               ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_C1IR                ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_C3IR                ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_AC4IR               ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);

FwStatus STDCALL fwiThreshold_8u_C1R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_8u_C3R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_8u_AC4R                 ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_C1R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_C3R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_16s_AC4R                ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_C1R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_C3R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_32f_AC4R                ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);

/*#FunctionBlock - Threshold_GT
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold, replace with threshold value (Greater Than)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is greater than the threshold value, the output data is set to the threshold value.</Text>
<Text>When the source data is less than or equal to the threshold value, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiThreshold_GT_8u_C1IR              ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   );
FwStatus STDCALL fwiThreshold_GT_8u_C3IR              ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_GT_8u_AC4IR             ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_GT_16s_C1IR             ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   );
FwStatus STDCALL fwiThreshold_GT_16s_C3IR             ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_GT_16s_AC4IR            ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_GT_32f_C1IR             ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   );
FwStatus STDCALL fwiThreshold_GT_32f_C3IR             ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
FwStatus STDCALL fwiThreshold_GT_32f_AC4IR            ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);

FwStatus STDCALL fwiThreshold_GT_8u_C1R               ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold   );
FwStatus STDCALL fwiThreshold_GT_8u_C3R               ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_GT_8u_AC4R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_GT_16s_C1R              ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold   );
FwStatus STDCALL fwiThreshold_GT_16s_C3R              ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_GT_16s_AC4R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_GT_32f_C1R              ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold   );
FwStatus STDCALL fwiThreshold_GT_32f_C3R              ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
FwStatus STDCALL fwiThreshold_GT_32f_AC4R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);

/*#FunctionBlock - Threshold_LT
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold, replace with threshold value (Less Than)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is less than the threshold value, the output data is set to the threshold value.</Text>
<Text>When the source data is greater than or equal to the threshold value, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiThreshold_LT_8u_C1IR              ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   );
FwStatus STDCALL fwiThreshold_LT_8u_C3IR              ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_LT_8u_AC4IR             ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_LT_16s_C1IR             ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   );
FwStatus STDCALL fwiThreshold_LT_16s_C3IR             ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_LT_16s_AC4IR            ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_LT_32f_C1IR             ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   );
FwStatus STDCALL fwiThreshold_LT_32f_C3IR             ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
FwStatus STDCALL fwiThreshold_LT_32f_AC4IR            ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);

FwStatus STDCALL fwiThreshold_LT_8u_C1R               ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold   );
FwStatus STDCALL fwiThreshold_LT_8u_C3R               ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_LT_8u_AC4R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3]);
FwStatus STDCALL fwiThreshold_LT_16s_C1R              ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold   );
FwStatus STDCALL fwiThreshold_LT_16s_C3R              ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_LT_16s_AC4R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
FwStatus STDCALL fwiThreshold_LT_32f_C1R              ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold   );
FwStatus STDCALL fwiThreshold_LT_32f_C3R              ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
FwStatus STDCALL fwiThreshold_LT_32f_AC4R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);

/*#FunctionBlock - Threshold_Val
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short -  Compare to a threshold, replace with specified value (General)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to a specified threshold value using a specified compare operation.</Text>
<Text>The compare operation can be "less than", "less than or equal", "equal", "greater than or equal" or "greater than".</Text>
<Text>When the comparison evaluates as true, the output data is set to a specified value.</Text>
<Text>When the comparison evaluates as false, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiThreshold_Val_8u_C1IR             ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   ,       Fw8u  value,    FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_8u_C3IR             ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_8u_AC4IR            ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_C1IR            ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   ,       Fw16s value,    FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_C3IR            ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_AC4IR           ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_C1IR            ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   ,       Fw32f value,    FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_C3IR            ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_AC4IR           ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);

FwStatus STDCALL fwiThreshold_Val_8u_C1R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold,          Fw8u  value   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_8u_C3R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_8u_AC4R             ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_C1R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold,          Fw16s value   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_C3R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_16s_AC4R            ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_C1R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold,          Fw32f value   , FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_C3R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
FwStatus STDCALL fwiThreshold_Val_32f_AC4R            ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);

/*#FunctionBlock - Threshold_GTVal
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold, replace with specified value (Greater Than)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is greater than the threshold value, the output data is set to a specified value.</Text>
<Text>When the source data is less than or equal to the threshold value, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiThreshold_GTVal_8u_C1IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   ,       Fw8u  value   );
FwStatus STDCALL fwiThreshold_GTVal_8u_C3IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_GTVal_8u_AC4IR          ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_GTVal_8u_C4IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[4], const Fw8u  value[4]);
FwStatus STDCALL fwiThreshold_GTVal_16s_C1IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   ,       Fw16s value   );
FwStatus STDCALL fwiThreshold_GTVal_16s_C3IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_GTVal_16s_AC4IR         ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_GTVal_16s_C4IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
FwStatus STDCALL fwiThreshold_GTVal_32f_C1IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   ,       Fw32f value   );
FwStatus STDCALL fwiThreshold_GTVal_32f_C3IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_GTVal_32f_AC4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_GTVal_32f_C4IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);

FwStatus STDCALL fwiThreshold_GTVal_8u_C1R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold   ,       Fw8u  value   );
FwStatus STDCALL fwiThreshold_GTVal_8u_C3R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_GTVal_8u_AC4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_GTVal_8u_C4R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[4], const Fw8u  value[4]);
FwStatus STDCALL fwiThreshold_GTVal_16s_C1R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold   ,       Fw16s value   );
FwStatus STDCALL fwiThreshold_GTVal_16s_C3R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_GTVal_16s_AC4R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_GTVal_16s_C4R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
FwStatus STDCALL fwiThreshold_GTVal_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold   ,       Fw32f value   );
FwStatus STDCALL fwiThreshold_GTVal_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_GTVal_32f_AC4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_GTVal_32f_C4R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);

/*#FunctionBlock - Threshold_LTVal
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold, replace with specified value (Less Than)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is less than the threshold value, the output data is set to a specified value.</Text>
<Text>When the source data is greater than or equal to the threshold value, the output data is set to the same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiThreshold_LTVal_8u_C1IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  threshold   ,       Fw8u  value   );
FwStatus STDCALL fwiThreshold_LTVal_8u_C3IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_LTVal_8u_AC4IR          ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_LTVal_8u_C4IR           ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  threshold[4], const Fw8u  value[4]);
FwStatus STDCALL fwiThreshold_LTVal_16s_C1IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s threshold   ,       Fw16s value   );
FwStatus STDCALL fwiThreshold_LTVal_16s_C3IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_LTVal_16s_AC4IR         ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_LTVal_16s_C4IR          ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
FwStatus STDCALL fwiThreshold_LTVal_32f_C1IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f threshold   ,       Fw32f value   );
FwStatus STDCALL fwiThreshold_LTVal_32f_C3IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_LTVal_32f_AC4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_LTVal_32f_C4IR          ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);

FwStatus STDCALL fwiThreshold_LTVal_8u_C1R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  threshold,          Fw8u  value   );
FwStatus STDCALL fwiThreshold_LTVal_8u_C3R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_LTVal_8u_AC4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[3], const Fw8u  value[3]);
FwStatus STDCALL fwiThreshold_LTVal_8u_C4R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  threshold[4], const Fw8u  value[4]);
FwStatus STDCALL fwiThreshold_LTVal_16s_C1R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s threshold,          Fw16s value   );
FwStatus STDCALL fwiThreshold_LTVal_16s_C3R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_LTVal_16s_AC4R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
FwStatus STDCALL fwiThreshold_LTVal_16s_C4R           ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
FwStatus STDCALL fwiThreshold_LTVal_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f threshold,          Fw32f value   );
FwStatus STDCALL fwiThreshold_LTVal_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_LTVal_32f_AC4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
FwStatus STDCALL fwiThreshold_LTVal_32f_C4R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);

/*#FunctionBlock - Threshold_LTValGTVal
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to double threshold, replace with specified value (Less Than/Greater Than)
#Long - <Text>These functions step through an ROI in a source buffer and compare source data to two specified threshold values.</Text>
<Text>The value of thresholdLT must be less than the value of thresholdGT.</Text>
<Text>When the source data is less than the value specified by thresholdLT, the output data is set to the value specified by valueLT.</Text>
<Text>When the source data is greater than the value specified by thresholdGT, the output data is set to the value specified by valueGT.</Text>
<Text>When the source data is within the range defined by thresholdLT and thresholdGT, the output data is set to same value as the source data.</Text>
<Text>Output data is written back to the same buffer for in-place operation or to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsThresholdErr
*/

FwStatus STDCALL fwiThreshold_LTValGTVal_8u_C1IR      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw8u  thresholdLT   ,       Fw8u  valueLT,          Fw8u  thresholdGT   ,       Fw8u  valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_8u_C3IR      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  thresholdLT[3], const Fw8u  valueLT[3], const Fw8u  thresholdGT[3], const Fw8u  valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_8u_AC4IR     ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u  thresholdLT[3], const Fw8u  valueLT[3], const Fw8u  thresholdGT[3], const Fw8u  valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_C1IR     ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw16s thresholdLT   ,       Fw16s valueLT,          Fw16s thresholdGT   ,       Fw16s valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_C3IR     ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_AC4IR    ( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_C1IR     ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize,       Fw32f thresholdLT   ,       Fw32f valueLT,          Fw32f thresholdGT   ,       Fw32f valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_C3IR     ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_AC4IR    ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);

FwStatus STDCALL fwiThreshold_LTValGTVal_8u_C1R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize,       Fw8u  thresholdLT   ,       Fw8u  valueLT   ,       Fw8u  thresholdGT   ,       Fw8u  valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  thresholdLT[3], const Fw8u  valueLT[3], const Fw8u  thresholdGT[3], const Fw8u  valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u  thresholdLT[3], const Fw8u  valueLT[3], const Fw8u  thresholdGT[3], const Fw8u  valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_C1R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize,       Fw16s thresholdLT   ,       Fw16s valueLT   ,       Fw16s thresholdGT   ,       Fw16s valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_C3R      ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_16s_AC4R     ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_C1R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,       Fw32f thresholdLT   ,       Fw32f valueLT   ,       Fw32f thresholdGT   ,       Fw32f valueGT   );
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
FwStatus STDCALL fwiThreshold_LTValGTVal_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);

/*#FunctionBlock - Compare
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare source data
#Long - <Text>These functions step through ROIs in two source buffers and compare the data in buffer 1 to the data in buffer 2 using a specified compare operation.</Text>
<Text>The compare operation can be "less than", "less than or equal, "equal", "greater than or equal" or "greater than".</Text>
<Text>When the comparison evaluates as true, the output data is set to all ones.</Text>
<Text>When the comparison evaluates as false, the output data is set to all zeroes.</Text>
<Text>Output data is written to a 1-channel Fw8u destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiCompare_8u_C1R                    ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_8u_C3R                    ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_8u_C4R                    ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_8u_AC4R                   ( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_16s_C1R                   ( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_16s_C3R                   ( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_16s_C4R                   ( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_16s_AC4R                  ( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_32f_C1R                   ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_32f_C3R                   ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_32f_C4R                   ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompare_32f_AC4R                  ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );

/*#FunctionBlock - CompareC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a constant
#Long - <Text>These functions step through an ROI in a source buffer and compare the source data to a specified constant value using a specified compare operation.</Text>
<Text>The compare operation can be "less than", "less than or equal, "equal", "greater than or equal" or "greater than".</Text>
<Text>When the comparison evaluates as true, the output data is set to all ones.</Text>
<Text>When the comparison evaluates as false, the output data is set to all zeroes.</Text>
<Text>Output data is written to a 1-channel Fw8u destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiCompareC_8u_C1R                   ( const Fw8u  *pSrc, int srcStep,       Fw8u value    , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_8u_C3R                   ( const Fw8u  *pSrc, int srcStep, const Fw8u value[3] , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_8u_AC4R                  ( const Fw8u  *pSrc, int srcStep, const Fw8u value[3] , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_8u_C4R                   ( const Fw8u  *pSrc, int srcStep, const Fw8u value[4] , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_16s_C1R                  ( const Fw16s *pSrc, int srcStep,       Fw16s value   , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_16s_C3R                  ( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_16s_AC4R                 ( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_16s_C4R                  ( const Fw16s *pSrc, int srcStep, const Fw16s value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_32f_C1R                  ( const Fw32f *pSrc, int srcStep,       Fw32f value   , Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_32f_C3R                  ( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_32f_AC4R                 ( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
FwStatus STDCALL fwiCompareC_32f_C4R                  ( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );

/*#FunctionBlock - CompareEqualEps
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare floating-point data for equality
#Long - <Text>These functions step through ROIs in two floating-point source buffers and compare the data in buffer 1 to the data in buffer 2 for equality within a specified tolerance.</Text>
<Text>When the difference between the values is less than or equal to the specified tolerance, the output data is set to all ones.</Text>
<Text>When the difference between the floating-point value and the constant is greater than the specified tolerance, the output data is set to all zeroes.</Text>
<Text>Output data is written to a 1-channel Fw8u destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsEpsValErr
*/
FwStatus STDCALL fwiCompareEqualEps_32f_C1R           ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEps_32f_C3R           ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEps_32f_C4R           ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEps_32f_AC4R          ( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);


/*#FunctionBlock - CompareEqualEpsC
TODO:Publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare floating-point data to a constant
#Long - <Text>These functions step through an ROI in a source buffer and compare the source floating-point data for equality within a specified tolerance.</Text>
<Text>When the difference between the floating-point value and the constant is less than or equal to the specified tolerance, the output data is set to all ones.</Text>
<Text>When the difference between the floating-point value and the constant is greater than the specified tolerance, the output data is set to all zeroes.</Text>
<Text>Output data is written to a 1-channel Fw8u destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsEpsValErr
*/
FwStatus STDCALL fwiCompareEqualEpsC_32f_C1R          ( const Fw32f *pSrc, int srcStep,       Fw32f value   , Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEpsC_32f_C3R          ( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEpsC_32f_AC4R         ( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
FwStatus STDCALL fwiCompareEqualEpsC_32f_C4R          ( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);

#if BUILD_NUM_AT_LEAST( 101 )
/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Morphological Operations</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that warp, shear, resize, mirror, and rotate images.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - Dilate3X3
todo:publish
#Technologies - REF, MT, SSE2
#Short - Dilate with 3X3 mask
#Long - <Text>These functions step through an ROI in a source buffer and perform a morphological dilation of the source data using a 3x3 symmetric mask.</Text>
<Text>The functions read the values of the source pixel and the eight surrounding pixels in the mask area, and set the output pixel value to the largest of the source pixel values.</Text>
<Text>Output can be written to a destination buffer or back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiDilate3x3_8u_C1R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
#endif
#if BUILD_NUM_AT_LEAST( 9999 )
FwStatus STDCALL fwiDilate3x3_8u_C3R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_8u_C4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_8u_AC4R     ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiDilate3x3_32f_C1R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_C3R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_C4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_AC4R    ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

FwStatus STDCALL fwiDilate3x3_8u_C1IR     ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_8u_C3IR     ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_8u_C4IR     ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_8u_AC4IR    ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiDilate3x3_32f_C1IR    ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_C3IR    ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_C4IR    ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiDilate3x3_32f_AC4IR   ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

/*#FunctionBlock - Erode3X3
todo:publish
#Technologies - REF, MT, SSE2
#Short - Erode with 3X3 mask
#Long - <Text>These functions step through an ROI in a source buffer and perform a morphological erosion of the source data using a 3x3 symmetric mask.</Text>
<Text>The functions read the values of the source pixel and the eight surrounding pixels in the mask area, and set the output pixel value to the smallest of the source pixel values.</Text>
<Text>Output can be written to a destination buffer or back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwiErode3x3_8u_C1IR      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_C3IR      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_C4IR      ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_AC4IR     ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C1IR     ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C3IR     ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C4IR     ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_AC4IR    ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );

FwStatus STDCALL fwiErode3x3_8u_C1R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_C4R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_8u_AC4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C1R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_C4R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
FwStatus STDCALL fwiErode3x3_32f_AC4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );

/*#FunctionBlock - Dilate
todo:publish
#Technologies - REF, MT, SSE2
#Short - Dilate with specified mask
#Long - <Text>These functions step through an ROI in a source buffer and perform a morphological dilation of the source data using a user-specified mask.</Text>
<Text>The mask consists of zero and non-zero values. The functions read the values of the source pixel and the surrounding pixels in the mask area, and set the output pixel value to the largest of the source pixel values that corresponds to a non-zero mask value.</Text>
<Text>Output can be written to a destination buffer or back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsZeroMaskValuesErr
*/
FwStatus STDCALL fwiDilate_8u_C1IR        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_8u_C3IR        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_8u_AC4IR       ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_C1IR       ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_C3IR       ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_AC4IR      ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiDilate_8u_C1R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_8u_C3R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_8u_C4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_8u_AC4R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_C1R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_C3R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_C4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiDilate_32f_AC4R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - Erode
todo:publish
#Technologies - REF, MT, SSE2
#Short - Erode with specified mask
#Long - <Text>These functions step through an ROI in a source buffer and perform a morphological erosion of the source data using a user-specified mask.</Text>
<Text>The mask consists of zero and non-zero values. The functions read the values of the source pixel and the surrounding pixels in the mask area, and set the output pixel value to the smallest of the source pixel values that corresponds to a non-zero mask value.</Text>
<Text>Output can be written to a destination buffer or back to the source location.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsZeroMaskValuesErr
*/

FwStatus STDCALL fwiErode_8u_C1IR         ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_8u_C3IR         ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_8u_AC4IR        ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_C1IR        ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_C3IR        ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_AC4IR       ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiErode_8u_C1R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_8u_C4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_8u_AC4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_C1R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_C3R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_C4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiErode_32f_AC4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor );
#endif

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - MorphologyGetSize
todo:publish
#Technologies - REF
#Short - Get state structure size for dilation and erosion
#Long - <Text>These functions compute the morphology state structure (<ParameterName>pState</ParameterName>) size.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMorphologyGetSize_8u_C1R          ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphologyGetSize_8u_C3R          ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphologyGetSize_8u_C4R          ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphologyGetSize_32f_C1R         ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphologyGetSize_32f_C3R         ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphologyGetSize_32f_C4R         ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );

/*#FunctionBlock - MorphologyAdvGetSize
todo:publish
#Technologies - REF
#Short - Get state structure size for advanced operations
#Long - <Text>These functions compute the advanced morphology state structure (<ParameterName>pState</ParameterName>) size.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwiMorphAdvGetSize_8u_C1R            ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphAdvGetSize_8u_C3R            ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphAdvGetSize_8u_C4R            ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphAdvGetSize_32f_C1R           ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphAdvGetSize_32f_C3R           ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );
FwStatus STDCALL fwiMorphAdvGetSize_32f_C4R           ( int roiWidth, const Fw8u  *pMask, FwiSize maskSize, int *pSize );

/*#FunctionBlock - MorphologyInit
todo:publish
#Technologies - REF
#Short - Initialize state structure for dilation and erosion functions
#Long - <Text>These functions initialize the state structure for morphologic dilation and erosion.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiMorphologyInit_8u_C1R             ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );
FwStatus STDCALL fwiMorphologyInit_8u_C3R             ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );
FwStatus STDCALL fwiMorphologyInit_8u_C4R             ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );
FwStatus STDCALL fwiMorphologyInit_32f_C1R            ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );
FwStatus STDCALL fwiMorphologyInit_32f_C3R            ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );
FwStatus STDCALL fwiMorphologyInit_32f_C4R            ( int roiWidth, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState *pState );

/*#FunctionBlock - MorphAdvInitAlloc
todo:publish
#Technologies - REF
#Short - Initialize state structure for advanced functions
#Long - <Text>These functions initialize the state structure for advanced morphologic operations such as <FunctionName>fwiMorphCloseBorder</FunctionName>,
<FunctionName>fwiMorphOpenBorder</FunctionName>, <FunctionName>fwiMorphTophatBorder</FunctionName>, <FunctionName>fwiMorphBlackhatBorder</FunctionName>, and <FunctionName>fwiMorphGradientBorder</FunctionName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiMorphAdvInitAlloc_8u_C1R          ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiMorphAdvInitAlloc_8u_C3R          ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiMorphAdvInitAlloc_8u_C4R          ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiMorphAdvInitAlloc_32f_C1R         ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiMorphAdvInitAlloc_32f_C3R         ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiMorphAdvInitAlloc_32f_C4R         ( FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u  *pMask, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - DilateBorderReplicate
todo:publish
#Technologies - REF
#Short - Dilate with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform dilation using the state structure defined by the <FunctionName>MorphologyInit</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiDilateBorderReplicate_8u_C1R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiDilateBorderReplicate_8u_C3R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiDilateBorderReplicate_8u_C4R      ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiDilateBorderReplicate_32f_C1R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiDilateBorderReplicate_32f_C3R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiDilateBorderReplicate_32f_C4R     ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );

/*#FunctionBlock - ErodeBorderReplicate
todo:publish
#Technologies - REF
#Short - Erode with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform erosion using the state structure defined by the <FunctionName>MorphologyInit</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiErodeBorderReplicate_8u_C1R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiErodeBorderReplicate_8u_C3R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiErodeBorderReplicate_8u_C4R       ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiErodeBorderReplicate_32f_C1R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiErodeBorderReplicate_32f_C3R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );
FwStatus STDCALL fwiErodeBorderReplicate_32f_C4R      ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState *pState );

/*#FunctionBlock - MorphOpenBorder
todo:publish
#Technologies - REF
#Short - Open with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform a morphologic open using the state structure defined by the <FunctionName>MorphAdvInitAlloc</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiMorphOpenBorder_8u_C1R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphOpenBorder_8u_C3R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphOpenBorder_8u_C4R            ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphOpenBorder_32f_C1R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphOpenBorder_32f_C3R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphOpenBorder_32f_C4R           ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );

/*#FunctionBlock - MorphCloseBorder
todo:publish
#Technologies - REF
#Short - Close with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform a morphologic close using the state structure defined by the <FunctionName>MorphAdvInitAlloc</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiMorphCloseBorder_8u_C1R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphCloseBorder_8u_C3R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphCloseBorder_8u_C4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphCloseBorder_32f_C1R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphCloseBorder_32f_C3R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphCloseBorder_32f_C4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );

/*#FunctionBlock - MorphTophatBorder
todo:publish
#Technologies - REF
#Short - Tophat with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform a morphologic tophat using the state structure defined by the <FunctionName>MorphAdvInitAlloc</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiMorphTophatBorder_8u_C1R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphTophatBorder_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphTophatBorder_8u_C4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphTophatBorder_32f_C1R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphTophatBorder_32f_C3R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphTophatBorder_32f_C4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );

/*#FunctionBlock - MorphBlackhatBorder
todo:publish
#Technologies - REF
#Short - Blackhat with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform a morphologic blackhat using the state structure defined by the <FunctionName>MorphAdvInitAlloc</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiMorphBlackhatBorder_8u_C1R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphBlackhatBorder_8u_C3R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphBlackhatBorder_8u_C4R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphBlackhatBorder_32f_C1R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphBlackhatBorder_32f_C3R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphBlackhatBorder_32f_C4R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );

/*#FunctionBlock - MorphGradientBorder
todo:publish
#Technologies - REF
#Short - Gradient with border replication
#Long - <Text>These functions step through an ROI in a source buffer, perform a morphologic gradient using the state structure defined by the <FunctionName>MorphAdvInitAlloc</FunctionName> function, and write the morphed data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified boundary type. In the current version, only the <Bold>fwBorderRepl</Bold> type is implemented.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsNotEvenStepErr, fwStsBadArgErr
*/
FwStatus STDCALL fwiMorphGradientBorder_8u_C1R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphGradientBorder_8u_C3R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphGradientBorder_8u_C4R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphGradientBorder_32f_C1R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphGradientBorder_32f_C3R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );
FwStatus STDCALL fwiMorphGradientBorder_32f_C4R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState );

/*#FunctionBlock - MorphologyFree
todo:publish
#Technologies - REF
#Short - Release state structure memory
#Long - <Text>This function releases memory allocated to state structures (<ParameterName>pState</ParameterName>) for dilation and erosion functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiMorphologyFree                    ( FwiMorphState *pState );

/*#FunctionBlock - MorphAdvFree
todo:publish
#Technologies - REF
#Short - Release advanced function state structure memory
#Long - <Text>This function releases memory allocated to state structures (<ParameterName>pState</ParameterName>) for advanced morphologic functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiMorphAdvFree                      ( FwiMorphState *pState );
#endif

#ifndef __SUNPRO_CC
#if BUILD_NUM_AT_LEAST( 100 )

/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Digital Filter Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that alter frequency-related visual properties of images such as sharpness and contrast.</ChaptDesc></Paragraph>
*/


/*#FunctionBlock - Sharpen
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Sharpen
#Long - <Text>These functions step through an ROI in a source buffer, apply a sharpening filter to the source data, and write the result to a destination buffer.</Text>
<Text>The functions use a fixed 3X3 mask.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterSharpen_8u_C1R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_8u_C3R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_8u_C4R              ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_8u_AC4R             ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_16s_C1R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_16s_C3R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_16s_C4R             ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_16s_AC4R            ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_32f_C1R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_32f_C3R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_32f_C4R             ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSharpen_32f_AC4R            ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );


/*#FunctionBlock - FilterBox
TODO:publish
#Technologies - REF, MT
#Short - Blur
#Long - <Text>These functions step through an ROI in a source buffer, apply a box blur filter to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterBox_8u_C1R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_C3R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_C4R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_AC4R                 ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C1R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C3R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C4R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_AC4R                ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C1R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C3R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C4R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_AC4R                ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - FilterBoxInplace
TODO:publish
#Technologies - REF, MT
#Short - Blur in place
#Long - <Text>These functions step through an ROI in a source buffer, apply a box blur filter to the source data, and write the filtered data back to the same location.</Text>
<Text>The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiFilterBox_8u_C1IR                 ( Fw8u  *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_C3IR                 ( Fw8u  *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_C4IR                 ( Fw8u  *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_8u_AC4IR                ( Fw8u  *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C1IR                ( Fw16s *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C3IR                ( Fw16s *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_C4IR                ( Fw16s *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_16s_AC4IR               ( Fw16s *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C1IR                ( Fw32f *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C3IR                ( Fw32f *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_C4IR                ( Fw32f *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterBox_32f_AC4IR               ( Fw32f *pSrcDst, int srcDstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - FilterMin
TODO:publish
#Technologies - REF, MT
#Short - Filter with minimum value in mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the smallest pixel value in the area defined by the mask size, and write the filtered data to a destination buffer.</Text>
<Text>The functions decrease image contrast. The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterMin_8u_C1R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_8u_C3R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_8u_C4R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_8u_AC4R                 ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_16s_C1R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_16s_C3R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_16s_C4R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_16s_AC4R                ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_32f_C1R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_32f_C3R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_32f_C4R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMin_32f_AC4R                ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - FilterMax
TODO:publish
#Technologies - REF, MT
#Short - Filter with maximum value in mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the largest pixel value in the area defined by the mask size, and write the filtered data to a destination buffer.</Text>
<Text>The functions increase image contrast.</Text>
<Text>The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterMax_8u_C1R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_8u_C3R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_8u_C4R                  ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_8u_AC4R                 ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_16s_C1R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_16s_C3R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_16s_C4R                 ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_16s_AC4R                ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_32f_C1R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_32f_C3R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_32f_C4R                 ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
FwStatus STDCALL fwiFilterMax_32f_AC4R                ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );

/*#FunctionBlock - SumWindowRow
TODO:publish
#Technologies - REF, MT
#Short - Sum pixel values in row mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the sum of the values of all pixels in the area defined by the row mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions can use arbitrary horizontal row mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiSumWindowRow_8u32f_C1R            ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowRow_8u32f_C3R            ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowRow_8u32f_C4R            ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowRow_16s32f_C1R           ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowRow_16s32f_C3R           ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowRow_16s32f_C4R           ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );

/*#FunctionBlock - SumWindow Column
TODO:publish
#Technologies - REF, MT
#Short - Sum pixel values in column mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the sum of the values of all pixels in the area defined by the column mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions can use arbitrary horizontal row mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiSumWindowColumn_8u32f_C1R         ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowColumn_8u32f_C3R         ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowColumn_8u32f_C4R         ( const Fw8u  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowColumn_16s32f_C1R        ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowColumn_16s32f_C3R        ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );
FwStatus STDCALL fwiSumWindowColumn_16s32f_C4R        ( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int maskSize, int anchor );

#endif

#if BUILD_NUM_AT_LEAST( 9999 )
/*#FunctionBlock - FilterMinGetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Get buffer size for <FunctionName>FilterMinBorderReplicate</FunctionName> Function
#Long - <Text>These functions step through an ROI in a source buffer and compute the size of the working buffer for the <FunctionName>FilterMinBorderReplicate</FunctionName> function.</Text>
<Text>A function must be called to determine the required buffer size, then the buffer must be allocated and passed to <FunctionName>FilterMinBorderReplicate</FunctionName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterMinGetBufferSize_8u_C1R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMinGetBufferSize_8u_C3R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMinGetBufferSize_8u_C4R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMinGetBufferSize_32f_C1R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMinGetBufferSize_32f_C3R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMinGetBufferSize_32f_C4R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );

/*#FunctionBlock - FilterMinBorderReplicate
TODO:publish
#Technologies - REF, MT
#Short - Filter with minimum value in mask and replicate border pixels
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the smallest pixel value in the area defined by the mask size, calculate the values of border pixels, and write the filtered data to a destination buffer.</Text>
<Text>The functions decrease image contrast.</Text>
<Text>The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/

FwStatus STDCALL fwiFilterMinBorderReplicate_8u_C1R   ( const Fw8u *pSrc, int srcStep, 
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize, 
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMinBorderReplicate_8u_C3R   ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMinBorderReplicate_8u_C4R   ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMinBorderReplicate_32f_C1R  ( const Fw32f *pSrc, int srcStep,
                                                                Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMinBorderReplicate_32f_C3R  ( const Fw32f *pSrc, int srcStep,
                                                                Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize,  FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMinBorderReplicate_32f_C4R  ( const Fw32f *pSrc, int srcStep,
                                                                Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize,  FwiPoint anchor, Fw8u *pBuffer );

/*#FunctionBlock - FilterMaxGetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Compute buffer size for <FunctionName>FilterMaxBorderReplicate</FunctionName> Function
#Long - <Text>These functions step through an ROI in a source buffer and compute the size of the working buffer for the <FunctionName>FilterMaxBorderReplicate</FunctionName> function.</Text>
<Text>A function must be called to determine the required buffer size, then the buffer must be allocated and passed to <FunctionName>FilterMaxBorderReplicate</FunctionName>.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterMaxGetBufferSize_8u_C1R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMaxGetBufferSize_8u_C3R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMaxGetBufferSize_8u_C4R     ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMaxGetBufferSize_32f_C1R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMaxGetBufferSize_32f_C3R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );
FwStatus STDCALL fwiFilterMaxGetBufferSize_32f_C4R    ( int roiWidth, FwiSize maskSize, int *pBufferSize );

/*#FunctionBlock - FilterMaxBorderReplicate
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with maximum value in mask and replicate border pixels
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the largest pixel value in the area defined by the mask size, calculate the values of border pixels, and write the filtered data to a destination buffer.</Text>
<Text>The functions increase image contrast.</Text>
<Text>The functions can use arbitrary mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/

FwStatus STDCALL fwiFilterMaxBorderReplicate_8u_C1R   ( const Fw8u  *pSrc, int srcStep, 
                                                                Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMaxBorderReplicate_8u_C3R   ( const Fw8u  *pSrc, int srcStep, 
                                                                Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMaxBorderReplicate_8u_C4R   ( const Fw8u  *pSrc, int srcStep, 
                                                                Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMaxBorderReplicate_32f_C1R  ( const Fw32f  *pSrc, int srcStep,
                                                                Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMaxBorderReplicate_32f_C3R  ( const Fw32f  *pSrc, int srcStep,
                                                                Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterMaxBorderReplicate_32f_C4R  ( const Fw32f  *pSrc, int srcStep,
                                                                Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor, Fw8u *pBuffer );

#endif // BUILD_NUM_AT_LEAST

#if BUILD_NUM_AT_LEAST( 100 )


/*#FunctionBlock - FilterMedian
TODO:publish
#Technologies - REF, MT
#Short - Filter with median value in mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the median value of all pixels in the area defined by the mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions remove noise without decreasing image brightness, as averaging filters do.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiFilterMedian_8u_C1R               ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_8u_C3R               ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_8u_C4R               ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_8u_AC4R              ( const Fw8u *pSrc, int srcStep,
                                                                Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_16s_C1R              ( const Fw16s *pSrc, int srcStep,
                                                                Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_16s_C3R              ( const Fw16s *pSrc, int srcStep,
                                                                Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_16s_C4R              ( const Fw16s *pSrc, int srcStep,
                                                                Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );

FwStatus STDCALL fwiFilterMedian_16s_AC4R             ( const Fw16s *pSrc, int srcStep,
                                                                Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                FwiSize maskSize, FwiPoint anchor );


/*#FunctionBlock - FilterMedianHoriz
TODO:publish
#Technologies - REF, MT
#Short - Filter with median value in horizontal mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the median value of all pixels in the area defined by the horizontal mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions remove noise without decreasing image brightness, as averaging filters do.</Text>
<Text>The functions can use arbitrary horizontal mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiFilterMedianHoriz_8u_C1R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_8u_C4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_8u_AC4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_16s_C1R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_16s_C3R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_16s_C4R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianHoriz_16s_AC4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

/*#FunctionBlock - FilterMedianVert
TODO:publish
#Technologies - REF, MT
#Short - Filter with median value in vertical mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the median value of all pixels in the area defined by the vertical mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions remove noise without decreasing image brightness, as averaging filters do.</Text>
<Text>The functions can use arbitrary vertical mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiFilterMedianVert_8u_C1R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_8u_C3R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_8u_C4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_8u_AC4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_16s_C1R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_16s_C3R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_16s_C4R          ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianVert_16s_AC4R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

/*#FunctionBlock - FilterMedianCross
TODO:publish
#Technologies - REF, MT
#Short - Filter with median value in cross mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the median value of all pixels in the area defined by the cross mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions remove noise without decreasing image brightness, as averaging filters do.</Text>
<Text>The functions can use arbitrary cross mask sizes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr, fwStsMemAllocErr
*/
FwStatus STDCALL fwiFilterMedianCross_8u_C1R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianCross_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianCross_8u_AC4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianCross_16s_C1R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianCross_16s_C3R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianCross_16s_AC4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );


/*#FunctionBlock - FilterMedianColor
TODO:publish
#Technologies - REF, MT
#Short - Filter color with median value in mask
#Long - <Text>These functions step through an ROI in a source buffer, replace each source pixel value with the value of the pixel nearest to the source pixel in the area defined by the mask, and write the filtered data to a destination buffer.</Text>
<Text>The functions remove noise without decreasing image brightness, as averaging filters do, while preserving the correlation between color components.</Text>

<Text>The distance between the source and mask pixels is calculated as a sum of absolute values using the following formula:</Text>

<Pseudo> abs(R(i)-R(j)) + abs(G(i)-G(j)) + abs(B(i)-B(j)).</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterMedianColor_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianColor_8u_AC4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianColor_16s_C3R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianColor_16s_AC4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianColor_32f_C3R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterMedianColor_32f_AC4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );



/*#FunctionBlock - Filter
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with an integer rectangular kernel
#Long - <Text>These functions step through an ROI in a source buffer, calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate, then divide by the divisor.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The kernel is an array of signed 32-bit integer values; the anchor coordinate location is relative to the bottom right corner of the kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilter_8u_C1R             ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_8u_C3R             ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_8u_C4R             ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s  *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_8u_AC4R            ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_16s_C1R            ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_16s_C3R            ( const Fw16s *pSrc,  int srcStep,
                                                        Fw16s *pDst,  int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize,FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_16s_C4R            ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_16s_AC4R           ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, FwiSize kernelSize, FwiPoint anchor, int divisor );

FwStatus STDCALL fwiFilter_32f_C1R            ( const Fw32f *pSrc, int srcStep,
                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter_32f_C3R            ( const Fw32f *pSrc, int srcStep,
                                                        Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter_32f_C4R            ( const Fw32f *pSrc, int srcStep,
                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter_32f_AC4R           ( const Fw32f *pSrc, int srcStep,
                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

/*#FunctionBlock - Filter32f
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter integer values with a floating-point rectangular kernel
#Long - <Text>These functions step through an ROI in a source buffer, and calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The kernel is an array of 32-bit single-precision floating point values; the anchor coordinate location is relative to the bottom right corner of the kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilter32f_8u_C1R          ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f  *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_8u_C3R          ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_8u_C4R          ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_8u_AC4R         ( const Fw8u  *pSrc, int srcStep,
                                                        Fw8u  *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_16s_C1R         ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_16s_C3R         ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_16s_C4R         ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

FwStatus STDCALL fwiFilter32f_16s_AC4R        ( const Fw16s *pSrc, int srcStep,
                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, FwiSize kernelSize, FwiPoint anchor );

/*#FunctionBlock - FilterColumn
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with an integer vertical column kernel
#Long - <Text>These functions step through an ROI in a source buffer, calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate, then divide by the divisor.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The vertical column kernel is an array of signed 32-bit integer values; the anchor coordinate location is relative to the bottom of the kernel.</Text>
<Text>Functions that operate on 8-bit unsigned data assume that the kernel values are small (in the -4096 to 4096 range).</Text>
<Text>If the results of the intermediate calculations exceed 24 bits, calculation errors may occur.</Text>
<Text>For very small kernel values, the functions use very fast code, at the expense of accuracy (+/- 1.5).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterColumn_8u_C1R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_8u_C3R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_8u_C4R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_8u_AC4R      ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_16s_C1R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_16s_C3R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_16s_C4R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_16s_AC4R     ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int yAnchor, int divisor );

FwStatus STDCALL fwiFilterColumn_32f_C1R      ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn_32f_C3R      ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn_32f_C4R      ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn_32f_AC4R     ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

/*#FunctionBlock - FilterColumn32f
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter integer values with a floating-point vertical column kernel
#Long - <Text>These functions step through an ROI in a source buffer, and calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The vertical column kernel is an array of 32-bit single-precision floating point values; the anchor coordinate location is relative to the bottom of the kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterColumn32f_8u_C1R    ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_8u_C3R    ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_8u_C4R    ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_8u_AC4R   ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_16s_C1R   ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_16s_C3R   ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_16s_C4R   ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

FwStatus STDCALL fwiFilterColumn32f_16s_AC4R  ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int yAnchor );

/*#FunctionBlock - FilterRow
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with an integer horizontal row kernel
#Long - <Text>These functions step through an ROI in a source buffer, calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate, then divide by the divisor.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The horizontal row kernel is an array of signed 32-bit integer values; the anchor coordinate location is relative to the right side of the kernel.</Text>
<Text>Functions that operate on 8-bit unsigned data assume that the kernel values are small.</Text>
<Text>If the results of intermediate calculations exceed 24 bits, calculation errors may occur.</Text>
<Text>For very small kernel values, the functions use very fast code, at the expense of accuracy (+/- 1.5).</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterRow_8u_C1R          ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_8u_C3R          ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_8u_C4R          ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_8u_AC4R         ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_16s_C1R         ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_16s_C3R         ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_16s_C4R         ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_16s_AC4R        ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32s *pKernel, int kernelSize, int xAnchor, int divisor );

FwStatus STDCALL fwiFilterRow_32f_C1R         ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow_32f_C3R         ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow_32f_C4R         ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow_32f_AC4R        ( const Fw32f *pSrc,    int srcStep,
                                                        Fw32f *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

/*#FunctionBlock - FilterRow32f
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter integer values with a floating-point horizontal row kernel
#Long - <Text>These functions step through an ROI in a source buffer, and calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate.</Text>
<Text>The filtered data is written to a destination buffer.</Text>
<Text>The horizontal row kernel is an array of 32-bit single-precision floating point values; the anchor coordinate location is relative to the right side of the kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/
FwStatus STDCALL fwiFilterRow32f_8u_C1R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_8u_C3R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_8u_C4R       ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_8u_AC4R      ( const Fw8u  *pSrc,    int srcStep,
                                                        Fw8u  *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_16s_C1R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_16s_C3R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_16s_C4R      ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

FwStatus STDCALL fwiFilterRow32f_16s_AC4R     ( const Fw16s *pSrc,    int srcStep,
                                                        Fw16s *pDst,    int dstStep, FwiSize dstRoiSize,
                                                  const Fw32f *pKernel, int kernelSize, int xAnchor );

#endif

#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - FilterRowBorderPipelineGetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Get buffer size for <FunctionName>FilterRowBorderPipeline</FunctionName> function
#Long - <Text>These functions compute the size of the working buffer in bytes.</Text>
<Text>A buffer of the specified size must be allocated and the value must be passed to the <FunctionName>FilterRowBorderPipeline</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_8u16s_C1R        ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_8u16s_C3R        ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_16s_C1R          ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_16s_C3R          ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_32f_C1R          ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_32f_C3R          ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_Low_8u16s_C1R    ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_Low_8u16s_C3R    ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_Low_16s_C1R      ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterRowBorderPipelineGetBufferSize_Low_16s_C3R      ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );

/*#FunctionBlock - FilterColumnBorderPipelineGetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Get buffer size for <FunctionName>FilterColumnBorderPipeline</FunctionName> function
#Long - <Text>These functions compute the size of the working buffer in bytes.</Text>
<Text>A buffer of the specified size must be allocated and the value must be passed to the <FunctionName>FilterColumnBorderPipeline</FunctionName> function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_8u16s_C1R     ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_8u16s_C3R     ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_16s_C1R       ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_16s_C3R       ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_32f_C1R       ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_32f_C3R       ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_Low_8u16s_C1R ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_Low_8u16s_C3R ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_Low_16s_C1R   ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );
FwStatus STDCALL fwiFilterColumnBorderPipelineGetBufferSize_Low_16s_C3R   ( FwiSize dstRoiSize, int kernelSize, int *pBufferSize );

/*#FunctionBlock - FilterRowBorderPipeline
TODO:publish
#Technologies - REF, MT
#Short - Filter row with border replication and pass to pipeline buffer
#Long - <Text>These functions step through an ROI in a source buffer, calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate, then divide by the divisor.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>These functions use a non-continuous destination memory buffer. A destination pointer array is used instead of a single destination pointer. Each array element points to a different memory region and contains exactly one row of data.</Text>
<Text>The row kernel is an array of signed 32-bit integer values.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/

FwStatus STDCALL fwiFilterRowBorderPipeline_8u16s_C1R         ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue, int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_8u16s_C3R         ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_16s_C1R           ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_16s_C3R           ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_32f_C1R           ( const Fw32f  *pSrc, int srcStep,
                                                                        Fw32f **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32f  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw32f borderValue[3], Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_32f_C3R           ( const Fw32f  *pSrc, int srcStep,
                                                                        Fw32f **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32f  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw32f borderValue[3], Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_Low_8u16s_C1R     ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_Low_8u16s_C3R     ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_Low_16s_C1R       ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterRowBorderPipeline_Low_16s_C3R       ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );


/*#FunctionBlock - FilterColumnBorderPipeline
TODO:publish
#Technologies - REF, MT
#Short - Filter column with border replication and pass to pipeline buffer
#Long - <Text>These functions step through an ROI in a source buffer, calculate the sum of products of the kernel values and the corresponding pixel values in an area defined by the kernel size and anchor coordinate, then divide by the divisor.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>These functions use a non-continuous destination memory buffer. A destination pointer array is used instead of a single destination pointer. Each array element points to a different memory region and contains exactly one column of data.</Text>
<Text>The column kernel is an array of signed 32-bit integer values.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsMaskSizeErr, fwStsAnchorErr
*/

FwStatus STDCALL fwiFilterColumnBorderPipeline_8u16s_C1R      ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue,int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_8u16s_C3R      ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3],int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_16s_C1R        ( const Fw16s   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_16s_C3R        ( const Fw16s   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_32f_C1R        ( const Fw32f   *pSrc, int srcStep,
                                                                        Fw32f **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32f  *pKernel, int kernelSize,int xAnchor,
                                                                        FwiBorderType borderType, Fw32f borderValue[3], Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_32f_C3R        ( const Fw32f   *pSrc, int srcStep,
                                                                        Fw32f **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32f  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw32f borderValue[3], Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_Low_8u16s_C1R  ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_Low_8u16s_C3R  ( const Fw8u   *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw8u borderValue[3], int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_Low_16s_C1R    ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterColumnBorderPipeline_Low_16s_C3R    ( const Fw16s  *pSrc, int srcStep,
                                                                        Fw16s **ppDst, FwiSize dstRoiSize,
                                                                  const Fw32s  *pKernel, int kernelSize, int xAnchor,
                                                                        FwiBorderType borderType, Fw16s borderValue, int divisor, Fw8u *pBuffer );

#endif // BUILD_NUM_AT_LEAST

#if BUILD_NUM_AT_LEAST( 100 )

/*#FunctionBlock - FilterPrewittHoriz
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a horizontal Prewitt kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Prewitt operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions enhance the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiFilterPrewittHoriz_8u_C1R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_8u_C3R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_8u_C4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_8u_AC4R        ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_16s_C1R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_16s_C3R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_16s_C4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_16s_AC4R       ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_32f_C1R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_32f_C3R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_32f_C4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittHoriz_32f_AC4R       ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FiltePrewittVert
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a vertical Prewitt kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Prewitt operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions enhance the vertical edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterPrewittVert_8u_C1R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_8u_C3R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_8u_C4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_8u_AC4R         ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_16s_C1R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_16s_C3R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_16s_C4R         ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_16s_AC4R        ( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_32f_C1R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_32f_C3R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_32f_C4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterPrewittVert_32f_AC4R        ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FilterScharrHoriz
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a horizontal Scharr kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Scharr operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterScharrHoriz_8u16s_C1R       ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterScharrHoriz_8s16s_C1R       ( const Fw8s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterScharrHoriz_32f_C1R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FilterScharrVert
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a vertical Scharr kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Scharr operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the vertical edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiFilterScharrVert_8u16s_C1R        ( const Fw8u  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterScharrVert_8s16s_C1R        ( const Fw8s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterScharrVert_32f_C1R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FilterSobelHoriz
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a horizontal Sobel kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiFilterSobelHoriz_8u_C1R           ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_8u_C3R           ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_8u_C4R           ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_8u_AC4R          ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_16s_C1R          ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_16s_C3R          ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_16s_C4R          ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_16s_AC4R         ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_32f_C1R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_32f_C3R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_32f_C4R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_32f_AC4R         ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelHoriz_8u16s_C1R        ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelHoriz_8s16s_C1R        ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelHorizMask_32f_C1R      ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

/*#FunctionBlock - FilterSobelVert
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a vertical Sobel kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiFilterSobelVert_8u_C1R            ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_8u_C3R            ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_8u_C4R            ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_8u_AC4R           ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_16s_C1R           ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_16s_C3R           ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_16s_C4R           ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_16s_AC4R          ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_32f_C1R           ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_32f_C3R           ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_32f_C4R           ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_32f_AC4R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterSobelVert_8u16s_C1R         ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelVert_8s16s_C1R         ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelVertMask_32f_C1R       ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

/*#FunctionBlock - FilterSobelHorizSecond
todo:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a second derivative horizontal Sobel kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a second derivative horizontal Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterSobelHorizSecond_32f_C1R    ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelHorizSecond_8s16s_C1R  ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelHorizSecond_8u16s_C1R  ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

/*#FunctionBlock - FilterSobelVertSecond
todo:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a second derivative vertical Sobel kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a second derivative vertical Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterSobelVertSecond_32f_C1R     ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelVertSecond_8s16s_C1R   ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelVertSecond_8u16s_C1R   ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );


/*#FunctionBlock - FilterSobelCross

TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a second cross derivative Sobel kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a second cross derivative Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions operate on either a 3X3 or 5X5 mask.</Text>
<Text>These functions enhance and smooth the horizontal edges of an image.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterSobelCross_8u16s_C1R        ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelCross_8s16s_C1R        ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
FwStatus STDCALL fwiFilterSobelCross_32f_C1R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

/*#FunctionBlock - FilterRobertsDown
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a horizontal Roberts kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Roberts operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions provide a gross approximation of the horizontal pixel gradient.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/


FwStatus STDCALL fwiFilterRobertsDown_8u_C1R          ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_8u_C3R          ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_8u_AC4R         ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_16s_C1R         ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_16s_C3R         ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_16s_AC4R        ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_32f_C1R         ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_32f_C3R         ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsDown_32f_AC4R        ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FilterRobertsUp
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a vertical Roberts kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Roberts operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>These functions provide a gross approximation of the vertical pixel gradient.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwiFilterRobertsUp_8u_C1R            ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_8u_C3R            ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_8u_AC4R           ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_16s_C1R           ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_16s_C3R           ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_16s_AC4R          ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_32f_C1R           ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_32f_C3R           ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );
FwStatus STDCALL fwiFilterRobertsUp_32f_AC4R          ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize );

/*#FunctionBlock - FilterLaplace
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a Laplace kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a high-pass Lfwacian operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions use either a 3X3 or a 5X5 kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterLaplace_8u_C1R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_8u_C3R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_8u_C4R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_8u_AC4R             ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_16s_C1R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_16s_C3R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_16s_C4R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_16s_AC4R            ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_32f_C1R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_32f_C3R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_32f_C4R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_32f_AC4R            ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_8u16s_C1R           ( const Fw8u   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLaplace_8s16s_C1R           ( const Fw8s   *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

/*#FunctionBlock - FilterGauss
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a Gauss kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a low-pass Gaussian operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions use either a 3X3 or a 5X5 kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/


FwStatus STDCALL fwiFilterGauss_8u_C1R                ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_8u_C3R                ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_8u_C4R                ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_8u_AC4R               ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_16s_C1R               ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_16s_C3R               ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_16s_C4R               ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_16s_AC4R              ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_32f_C1R               ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_32f_C3R               ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_32f_C4R               ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterGauss_32f_AC4R              ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

/*#FunctionBlock - FilterHipass
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a high-pass kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a high-pass operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions use either a 3X3 or a 5X5 kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterHipass_8u_C1R               ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_8u_C3R               ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_8u_C4R               ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_8u_AC4R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_16s_C1R              ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_16s_C3R              ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_16s_C4R              ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_16s_AC4R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_32f_C1R              ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_32f_C3R              ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_32f_C4R              ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterHipass_32f_AC4R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

/*#FunctionBlock - FilterLowpass
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Filter with a low-pass kernel
#Long - <Text>These functions step through an ROI in a source buffer, apply a low-pass operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>The functions use either a 3X3 or a 5X5 kernel.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterLowpass_8u_C1R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_8u_C3R              ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_8u_AC4R             ( const Fw8u   *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_16s_C1R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_16s_C3R             ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_16s_AC4R            ( const Fw16s  *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_32f_C1R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_32f_C3R             ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
FwStatus STDCALL fwiFilterLowpass_32f_AC4R            ( const Fw32f  *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

#endif

#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - Filter*GetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Get filter function buffer size
#Long - <Text>These functions calculate the size of the memory buffer (in bytes) required by a specific border replication filter function.</Text>
<Text>* Represents the name of the filter function.</Text>
<Text>A buffer of the specified size must be allocated and the value must be passed to the filter function.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwiFilterScharrHorizGetBufferSize_8u16s_C1R              ( FwiSize roiSize, int *pBufferSize );
FwStatus STDCALL fwiFilterScharrHorizGetBufferSize_32f_C1R                ( FwiSize roiSize, int *pBufferSize );
FwStatus STDCALL fwiFilterScharrVertGetBufferSize_8u16s_C1R               ( FwiSize roiSize, int *pBufferSize );
FwStatus STDCALL fwiFilterScharrVertGetBufferSize_32f_C1R                 ( FwiSize roiSize, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelHorizGetBufferSize_8u16s_C1R               ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelHorizGetBufferSize_32f_C1R                 ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelVertGetBufferSize_8u16s_C1R                ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelVertGetBufferSize_32f_C1R                  ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelHorizSecondGetBufferSize_8u16s_C1R         ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelHorizSecondGetBufferSize_32f_C1R           ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelVertSecondGetBufferSize_8u16s_C1R          ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelVertSecondGetBufferSize_32f_C1R            ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelCrossGetBufferSize_8u16s_C1R               ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterSobelCrossGetBufferSize_32f_C1R                 ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterLfwacianGetBufferSize_8u16s_C1R                ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterLfwacianGetBufferSize_32f_C1R                  ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterLowpassGetBufferSize_8u_C1R                     ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );
FwStatus STDCALL fwiFilterLowpassGetBufferSize_32f_C1R                    ( FwiSize roiSize, FwiMaskSize mask, int *pBufferSize );

/*#FunctionBlock - FilterScharrHorizBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a horizontal Scharr kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Scharr operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr
*/

FwStatus STDCALL fwiFilterScharrHorizBorder_8u16s_C1R         ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterScharrHorizBorder_32f_C1R           ( const Fw32f  *pSrc, int srcStep,
                                                                        Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterScharrVertBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a vertical Scharr kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Scharr operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr
*/

FwStatus STDCALL fwiFilterScharrVertBorder_8u16s_C1R          ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterScharrVertBorder_32f_C1R            ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterSobelHorizBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a horizontal Sobel kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a horizontal Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterSobelHorizBorder_8u16s_C1R          ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize, 
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelHorizBorder_32f_C1R            ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterSobelVertBorder, FilterSobelNegVertBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a vertical Sobel kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a vertical Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterSobelVertBorder_8u16s_C1R           ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelVertBorder_32f_C1R             ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelNegVertBorder_8u16s_C1R        ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelNegVertBorder_32f_C1R          ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterSobelHorizSecondBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a second derivative horizontal Sobel kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a second derivative horizontal Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterSobelHorizSecondBorder_8u16s_C1R    ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelHorizSecondBorder_32f_C1R      ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterSobelVertSecondBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a second derivative vertical Sobel kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a second derivative vertical Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterSobelVertSecondBorder_8u16s_C1R     ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelVertSecondBorder_32f_C1R       ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterSobelCrossBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a second derivative cross Sobel kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a second derivative cross Sobel operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterSobelCrossBorder_8u16s_C1R          ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize, 
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterSobelCrossBorder_32f_C1R            ( const Fw32f  *pSrc, int srcStep,
                                                                        Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterLfwacianBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a Laplace kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a second Laplace operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterLfwacianBorder_8u16s_C1R           ( const Fw8u  *pSrc, int srcStep,
                                                                        Fw16s *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterLfwacianBorder_32f_C1R             ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );

/*#FunctionBlock - FilterLowpassBorder
TODO:publish
#Technologies - REF, MT
#Short - Filter with a low-pass kernel and replicate border
#Long - <Text>These functions step through an ROI in a source buffer, apply a low-pass operator to the source data, and write the filtered data to a destination buffer.</Text>
<Text>Border pixels are replicated according to the specified border type.</Text>
<Text>The function can use a 3X3 or 5X5 kernel size as specified by the mask parameter.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus STDCALL fwiFilterLowpassBorder_8u_C1R                ( const Fw8u *pSrc, int srcStep,
                                                                        Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw8u borderValue, Fw8u *pBuffer );

FwStatus STDCALL fwiFilterLowpassBorder_32f_C1R               ( const Fw32f *pSrc, int srcStep,
                                                                        Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
                                                                        FwiMaskSize mask, FwiBorderType borderType, Fw32f borderValue, Fw8u *pBuffer );
#endif // BUILD_NUM_AT_LEAST

#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - FilterWienerGetBufferSize
TODO:publish
#Technologies - REF, MT
#Short - Get buffer size for <FunctionName>FilterWiener</FunctionName> function
#Long - <Text>This function calculates the memory buffer size for the <FunctionName>FilterWiener</FunctionName> function in bytes.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsMaskSizeErr, FwStsNoiseRangeErr
*/

FwStatus fwiFilterWienerGetBufferSize     ( FwiSize dstRoiSize, FwiSize maskSize, int channels, int *pBufferSize );

/*#FunctionBlock - FilterWiener
TODO:publish
#Technologies - REF, MT
#Short - Remove Blur (Wiener)
#Long - <Text>These functions step through an ROI in a source buffer, apply a Wiener filter to the source data, and write the result to a destination buffer.</Text>
<Text>The functions use random Gaussian noise to perform deconvolution.</Text>
<Text>If no noise value is specified (noise = 0), the functions automatically calculate a noise value and write it to the provide noise buffer of the calling routine.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsBorderErr, fwStsNotEvenStepErr, fwStsMaskErr
*/

FwStatus fwiFilterWiener_8u_C1R           ( const Fw8u   *pSrc, int srcStep, 
                                                    Fw8u   *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[1], Fw8u *pBuffer );

FwStatus fwiFilterWiener_16s_C1R          ( const Fw16s  *pSrc, int srcStep, 
                                                    Fw16s  *pDst, int dstStep,FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[1], Fw8u *pBuffer );

FwStatus fwiFilterWiener_32f_C1R          ( const Fw32f  *pSrc, int srcStep,
                                                    Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[1], Fw8u *pBuffer );

FwStatus fwiFilterWiener_8u_C3R           ( const Fw8u   *pSrc, int srcStep,
                                                    Fw8u   *pDst, int dstStep,FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

FwStatus fwiFilterWiener_16s_C3R          ( const Fw16s  *pSrc, int srcStep,
                                                    Fw16s  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

FwStatus fwiFilterWiener_32f_C3R          ( const Fw32f  *pSrc, int srcStep,
                                                    Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

FwStatus fwiFilterWiener_8u_C4R           ( const Fw8u   *pSrc, int srcStep,
                                                    Fw8u   *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[4], Fw8u *pBuffer );

FwStatus fwiFilterWiener_16s_C4R          ( const Fw16s  *pSrc, int srcStep,
                                                    Fw16s  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[4], Fw8u *pBuffer );

FwStatus fwiFilterWiener_32f_C4R          ( const Fw32f  *pSrc, int srcStep,
                                                    Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[4], Fw8u *pBuffer );

FwStatus fwiFilterWiener_8u_AC4R          ( const Fw8u   *pSrc, int srcStep,
                                                    Fw8u   *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

FwStatus fwiFilterWiener_16s_AC4R         ( const Fw16s  *pSrc, int srcStep,
                                                    Fw16s  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

FwStatus fwiFilterWiener_32f_AC4R         ( const Fw32f  *pSrc, int srcStep,
                                                    Fw32f  *pDst, int dstStep, FwiSize dstRoiSize,
                                                    FwiSize maskSize, FwiPoint anchor, Fw32f noise[3], Fw8u *pBuffer );

#endif // BUILD_NUM_AT_LEAST

#endif // __SUNPRO_CC

#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - ConvFull
TODO:publish
#Technologies - REF, MT
#Short - Convolution (2D Full)
#Long - <Text>These functions step through ROIs in two source buffers, perform full two-dimensional finite linear convolution using the source data, and write the convolved data to a single destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsDivisorErr, fwStsMemAllocErr
*/

FwStatus STDCALL fwiConvFull_8u_C1R       ( const Fw8u  *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw8u  *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw8u  *pDst,  int dstStep,  int divisor );

FwStatus STDCALL fwiConvFull_8u_C3R       ( const Fw8u  *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw8u  *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw8u  *pDst,  int dstStep,  int divisor );

FwStatus STDCALL fwiConvFull_16s_C1R      ( const Fw16s *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw16s *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw16s *pDst,  int dstStep,  int divisor );

FwStatus STDCALL fwiConvFull_16s_C3R      ( const Fw16s *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw16s *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw16s *pDst,  int dstStep, int divisor );

FwStatus STDCALL fwiConvFull_32f_C1R      ( const Fw32f *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw32f *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw32f *pDst,  int dstStep );

FwStatus STDCALL fwiConvFull_32f_C3R      ( const Fw32f *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw32f *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw32f *pDst,  int dstStep );
/*#FunctionBlock - ConvValid
TODO:publish
#Technologies - REF, MT
#Short - Convolution (2D Valid)
#Long - <Text>These functions step through ROIs in two source buffers, perform valid two-dimensional finite linear convolution using the source data, and write the convolved data to a single destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr, fwStsDivisorErr, fwStsMemAllocErr
*/

FwStatus STDCALL fwiConvValid_8u_C1R      ( const Fw8u  *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw8u  *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw8u  *pDst,  int dstStep, int divisor );

FwStatus STDCALL fwiConvValid_8u_C3R      ( const Fw8u  *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw8u  *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw8u  *pDst,  int dstStep, int divisor );

FwStatus STDCALL fwiConvValid_16s_C1R     ( const Fw16s *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw16s *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw16s *pDst,  int dstStep, int divisor );

FwStatus STDCALL fwiConvValid_16s_C3R     ( const Fw16s *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw16s *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw16s *pDst,  int dstStep, int divisor );

FwStatus STDCALL fwiConvValid_32f_C1R     ( const Fw32f *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw32f *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw32f *pDst,  int dstStep );

FwStatus STDCALL fwiConvValid_32f_C3R     ( const Fw32f *pSrc1, int srcStep1, FwiSize srcSize1,
                                              const Fw32f *pSrc2, int srcStep2, FwiSize srcSize2,
                                                    Fw32f *pDst,  int dstStep );

#endif // BUILD_NUM_AT_LEAST

#if BUILD_NUM_AT_LEAST( 102 )
/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Linear Transformation Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions for Discrete Cosine Transform (DCT).</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - DCT8X8
todo:publish
#Technologies - REF, SSE2
#Short - Compute data block DCT
#Long - <Text>These functions compute the forward and inverse DCT coefficients of an 8X8 block of image data.</Text>
<Text>Results can be written to a destination buffer or back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiDCT8x8Fwd_16s_C1      (const Fw16s *pSrc, Fw16s *pDst   );
FwStatus STDCALL fwiDCT8x8Fwd_16s_C1I     (      Fw16s*               pSrcDst);
FwStatus STDCALL fwiDCT8x8Inv_16s_C1      (const Fw16s *pSrc, Fw16s *pDst   );
FwStatus STDCALL fwiDCT8x8Inv_16s_C1I     (      Fw16s*               pSrcDst);

#endif // BUILD_NUM_AT_LEAST

/*#Documentation

</Chapter>

<Chapter>
<ChapterHeading>Geometric Transform Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that warp, shear, resize, mirror, and rotate images.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - Resize
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Resize an image
#Long - <Text>These functions step through an ROI in a source buffer and map the source data into a destination ROI, using x-y scaling factors and a specified method of interpolation.</Text>
<Text>A super-sampling interpolation option can be used when both the x and y scaling factors are less than 1.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_SUPER: supersampling
 FWI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
<Text>Use the super-sampling interpolation option when both the x and y scaling factors are less than 1.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsResizeFactorErr,
fwStsInterpolationErr, fwStsWrongIntersectROI
*/
FwStatus STDCALL fwiResize_8u_C1R         ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_8u_C3R         ( const  Fw8u *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                  int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_8u_C4R         ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_8u_AC4R        ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_C1R        ( const  Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_C3R        ( const  Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_C4R        ( const  Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_AC4R       ( const  Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_C1R        ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_C3R        ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_C4R        ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_AC4R       ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_8u_P3R         ( const  Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_P3R        ( const  Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u * const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_P3R        ( const  Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_8u_P4R         ( const  Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[4], int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_16u_P4R        ( const  Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u * const pDst[4],                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

FwStatus STDCALL fwiResize_32f_P4R        ( const  Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[4],                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

/*#FunctionBlock - ResizeCenter
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Resize image relative to center pixel
#Long - <Text>These functions step through an ROI in a source buffer and map the source data into a destination ROI using x-y scaling factors and a specified method of interpolation.</Text>
<Text>The coordinates of the center pixel in the destination image are the same as the center coordinates of the center pixel in the source image.</Text>
<Text>The <FunctionName>Resize</FunctionName> function is a special case of this function with the image center set at (0,0).</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_SUPER: supersampling
 FWI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
<Text>Use the super-sampling interpolation option when both the x and y scaling factors are less than 1.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsResizeFactorErr,
fwStsInterpolationErr, fwStsWrongIntersectROI
*/
FwStatus STDCALL fwiResizeCenter_8u_C1R   ( const  Fw8u    *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u    *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_8u_C3R   ( const  Fw8u    *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u    *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_8u_C4R   ( const  Fw8u    *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u    *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_8u_AC4R  ( const  Fw8u    *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u    *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_C1R  ( const  Fw16u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_C3R  ( const  Fw16u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_C4R  ( const  Fw16u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_AC4R ( const  Fw16u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_C1R  ( const  Fw32f   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_C3R  ( const  Fw32f   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_C4R  ( const  Fw32f   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_AC4R ( const  Fw32f   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f   *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_8u_P3R   ( const  Fw8u *   const pSrc[3] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *   const pDst[3] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_P3R  ( const  Fw16u *  const pSrc[3] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *  const pDst[3] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_P3R  ( const  Fw32f * const pSrc[3] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f * const pDst[3] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_8u_P4R   ( const  Fw8u *  const pSrc[4] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *  const pDst[4] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_16u_P4R  ( const  Fw16u * const pSrc[4] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u * const pDst[4] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiResizeCenter_32f_P4R  ( const  Fw32f * const pSrc[4] , FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f * const pDst[4] ,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

/*#FunctionBlock - GetResizeFract
TODO:publish
#Technologies - REF
#Short - Get resizing fraction
#Long - <Text>This function calculates the inverses of x and y scaling factors and checks whether parameters are correctly defined. It is typically used with the <FunctionName>ResizeShift</FunctionName> functions.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsResizeFactorErr, fwStsInterpolationErr,
fwStsWrongIntersectROI
*/
FwStatus STDCALL fwiGetResizeFract        (FwiSize srcSize, FwiRect srcRoi, double xFactor, double yFactor,
                                             double* xFr, double* yFr, int interpolation );

/*#FunctionBlock - ResizeShift
TODO:publish
#Technologies - REF
#Short -  Resize and shift an image
#Long - <Text>These functions step through an ROI in a source buffer, scale the source data using inverse x-y scaling factors and a specified method of interpolation, then shift the data to a destination ROI.</Text>
<Text>The functions use the following equivalent mapping formulas.</Text>
<Pseudo>
 x=(x'-xShift)*xFr  or  x'=x*xFactor+xShift
 y=(y'-yShift)*yFr    y'=y*yFactor+yShift
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsResizeFactorErr,
 fwStsInterpolationErr, fwStsWrongIntersectROI
*/
FwStatus STDCALL fwiResizeShift_8u_C1R    ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_8u_C3R    ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *  pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_8u_C4R    ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_8u_AC4R   ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_C1R   ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_C3R   ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_C4R   ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_AC4R  ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_C1R   ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_C3R   ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_C4R   ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_AC4R  ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_8u_P3R    ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_P3R   ( const Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_P3R   ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[3],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_8u_P4R    ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[4],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_16u_P4R   ( const Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[4],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiResizeShift_32f_P4R   ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[4],                   int dstStep, FwiSize dstRoiSize,
                                              double xFr, double yFr, double xShift, double yShift, int interpolation );

/*#FunctionBlock - ResizeSqrPixelGetBufSize
TODO:publish
#Technologies - REF
#Short -  Calculate external buffer size for <FunctionName>ResizeSqrPixel</FunctionName> function
#Long - <Text>This function is used before a <FunctionName>ResizeSqrPixel</FunctionName> function. It calculates external buffer size, acts as a
buffer space placeholder, and checks whether parameters used by the <FunctionName>ResizeSqrPixel</FunctionName> function are defined correctly.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsNumChannelErr, fwStsInterpolationErr
*/
FwStatus STDCALL fwiResizeSqrPixelGetBufSize  (FwiSize dstSize, int nChannel,int interpolation, int *pBufferSize);

/*#FunctionBlock - ResizeSqrPixel
TODO:publish
#Technologies - REF, SSE2
#Short - Resize and shift an image
#Long - <Text>These functions step through an ROI in a source buffer, scale the source data using x-y scaling factors and a specified method of interpolation, then shift the data to a destination ROI.</Text>
<Text>Unlike the <FunctionName>ResizeShift</FunctionName> function, the <FunctionName>ResizeSqrPixel</FunctionName> functions map the source ROI to a specified destination ROI and use noninverted x-y scaling factors.</Text>
<Text>The functions use the following mapping formulas.</Text>
<Pseudo>
 x'=x*xFactor+xShift
 y'=y*yFactor+yShift
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_SUPER: supersampling
 PLI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
<Text>Use the super-sampling interpolation option when both the x and y scaling factors are less than 1.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsResizeFactorErr,
fwStsInterpolationErr, fwStsWrongIntersectROI
*/
FwStatus STDCALL fwiResizeSqrPixel_8u_C1R             ( const  Fw8u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u   *pDst,                   int dstStep, FwiRect dstRoi, 
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_8u_C3R             ( const  Fw8u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u   *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_8u_C4R             ( const  Fw8u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u   *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_8u_AC4R            ( const  Fw8u   *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u   *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_C1R            ( const  Fw16u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_C3R            ( const  Fw16u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_C4R            ( const  Fw16u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_AC4R           ( const  Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_C1R            ( const  Fw32f  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_C3R            ( const  Fw32f  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_C4R            ( const  Fw32f  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_AC4R           ( const  Fw32f  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_8u_P3R             ( const  Fw8u  * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u  * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_8u_P4R             ( const  Fw8u  * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw8u  * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor,  double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_P3R            ( const  Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_16u_P4R            ( const  Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw16u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_P3R            ( const  Fw32f * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

FwStatus STDCALL fwiResizeSqrPixel_32f_P4R            ( const  Fw32f * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                 Fw32f * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          double xFactor, double yFactor, double xShift, double yShift, int interpolation, Fw8u *pBuffer);

/*#FunctionBlock - ResizeYUV422
TODO:publish
#Technologies - REF
#Short -  Resize a YUV422 image
#Long - <Text>This function steps through an ROI in a source buffer, scales the source data using x-y scaling factors and a specified method of interpolation, then shifts the data to a destination ROI.</Text>
<Text>The function is specialized for YUV color space data with 4:2:2 chroma sub-sampling. The source data has two channels in 4:2:2 sampled format with decoupled luminance and chrominance components.</Text>
<Text>For example, the format could be in the form of alternating YUYVYUYV...</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
 FWI_INTER_SUPER: supersampling
 FWI_INTER_LANCZOS: interpolation with Lanczos window function
</Pseudo>
<Text>Use the super-sampling interpolation option when both the x and y scaling factors are less than 1.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsResizeNoOperationErr,
fwStsResizeFactorErr, fwStsInterpolationErr
*/
FwStatus STDCALL fwiResizeYUV422_8u_C2R   ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiSize dstRoiSize,
                                              double xFactor, double yFactor, int interpolation );

/*#FunctionBlock - Mirror
TODO:publish
#Technologies - REF, SSE2
#Short - Mirror an image
#Long - <Text>These functions step through an ROI in a source buffer and mirror the source data about the vertical axis, the horizontal axis, or both axes.</Text>
<Text>The mirrored data can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsMirrorFlipErr
*/
FwStatus STDCALL fwiMirror_8u_C1R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_C3R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_C4R           ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_AC4R          ( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C1R          ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C3R          ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C4R          ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_AC4R         ( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C1R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C3R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C4R          ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_AC4R         ( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C1R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C3R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C4R          ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_AC4R         ( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);

FwStatus STDCALL fwiMirror_8u_C1IR          ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_C3IR          ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_C4IR          ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_8u_AC4IR         ( Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C1IR         ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C3IR         ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_C4IR         ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_16u_AC4IR        ( Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C1IR         ( Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C3IR         ( Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_C4IR         ( Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32s_AC4IR        ( Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C1IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C3IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_C4IR         ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
FwStatus STDCALL fwiMirror_32f_AC4IR        ( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);

/*#FunctionBlock - Remap
TODO:publish
#Technologies - REF
#Short - Remap an image using table look-up
#Long - <Text>These functions step through an ROI in a source buffer, remap the source data using look-up tables for the x and y coordinates of each pixel and a specified method of interpolation, then write the data into a destination ROI.</Text>
<Text>The following mapping formula is used.</Text>
<Pseudo>
 dst_pixel[x,y] = src_pixel[pxMap[x,y], pyMap[x,y]]

Where:
 pxMap[x,y] is the x-coordinate look up table
 pyMap[x,y] is the y-coordinate look-up table
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr
*/
FwStatus STDCALL fwiRemap_8u_C1R          ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u  *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_8u_C3R          ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u  *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_8u_C4R          ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u  *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_8u_AC4R         ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u  *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_C1R         ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_C3R         ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_C4R         ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_AC4R        ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f *pDst,  int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_8u_P3R          ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u * const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_8u_P4R          ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw8u * const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_P3R         ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f* const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation );

FwStatus STDCALL fwiRemap_32f_P4R         ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                              const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
                                                    Fw32f* const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation );

/*#FunctionBlock - Rotate
todo:publish
#Technologies - REF, SSE2
#Short -  Rotate an image
#Long - <Text>These functions step through an ROI in a source buffer, rotate the source data around (0,0) by a specified angle, shift the x-y coordinates according to the x and y shift values, and map the data into a destination ROI using a specified method of interpolation.</Text>
<Text>The FWI_SMOOTH_EDGE option is available when the destination ROI contains the transformed image.</Text>
<Text>Depending on the x-y coordinate values and the amount of rotation, the rotated image may be clipped by the destination ROI boundary.</Text>
<Text>A positive angle value indicates counterclockwise rotation when source data is ordered downward for height.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsRectErr,
fwStsInterpolationErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiRotate_8u_C1R         ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_8u_C3R         ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_8u_C4R         ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_8u_AC4R        ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_C1R        ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_C3R        ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_C4R        ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_AC4R       ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_C1R        ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_C3R        ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_C4R        ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_AC4R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_8u_P3R         ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_8u_P4R         ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_P3R        ( const Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_16u_P4R        ( const Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_P3R        ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiRotate_32f_P4R        ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              double angle, double xShift, double yShift, int interpolation );

/*#FunctionBlock - GetRotateShift
TODO:publish
#Technologies - REF
#Short - Get rotation shift values
#Long - <Text>This function computes shift values for the <FunctionName>RotateCenter</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiGetRotateShift        (double xCenter, double yCenter,
                                             double angle, double* xShift, double* yShift);

/*#FunctionBlock - AddRotateShift
TODO:publish
#Technologies - REF
#Short - Add new rotation shift values to existing shift values
#Long - <Text>This function adds specified shift values to existing shift values for the <FunctionName>RotateCenter</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr
*/
FwStatus STDCALL fwiAddRotateShift        (double xCenter, double yCenter,
                                             double angle, double* xShift, double* yShift);

/*#FunctionBlock - GetRotateQuad
TODO:publish
#Technologies - REF
#Short - Get rotation quadrangle vertices
#Long - <Text>This function is used with the <FunctionName>GetRotateBound</FunctionName> function. It computes the vertex coordinates of the quadrangular destination ROI for the rotate functions.</Text>
<Text>Quad[0] contains the top right x and y coordinates</Text>
<Text>Quad[1] contains the top left x and y coordinates</Text>
<Text>Quad[2] contains the bottom right x and y coordinates</Text>
<Text>Quad[3] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetRotateQuad         (FwiRect srcRoi, double quad[4][2],
                                             double angle, double xShift, double yShift);

/*#FunctionBlock - GetRotateBound
TODO:publish
#Technologies - REF
#Short - Get rotation boundary
#Long - <Text>This function is used with the <FunctionName>GetRotateQuad</FunctionName> function. It computes the boundaries of the source ROI for the rotate functions.</Text>

<Text>Bound[0] contains the top right x and y coordinates</Text>
<Text>Bound[1] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetRotateBound        (FwiRect srcRoi, double bound[2][2],
                                             double angle, double xShift, double yShift);

/*#FunctionBlock - RotateCenter
TODO:publish
#Technologies - REF
#Short - Rotate around a specific center
#Long - <Text>These functions step through a source ROI, rotate the source data around a specified center by a specified angle, apply a specified method of interpolation, then write the rotated data to a destination ROI.</Text>
<Text>Positive angle parameter values specify counterclockwise rotation.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsRectErr,
fwStsInterpolationErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiRotateCenter_8u_C1R   ( const  Fw8u  *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u  *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_8u_C3R   ( const  Fw8u  *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u  *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_8u_C4R   ( const  Fw8u  *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u  *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_8u_AC4R  ( const  Fw8u  *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u  *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_C1R  ( const  Fw16u *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_C3R  ( const  Fw16u *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_C4R  ( const  Fw16u *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_AC4R ( const  Fw16u *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_C1R  ( const  Fw32f *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_C3R  ( const  Fw32f *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_C4R  ( const  Fw32f *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle, double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_AC4R ( const  Fw32f *pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_8u_P3R   ( const  Fw8u * const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[3],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_8u_P4R   ( const  Fw8u * const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[4],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_P3R  ( const  Fw16u * const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u * const pDst[3],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_16u_P4R  ( const  Fw16u * const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw16u * const pDst[4],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_P3R  ( const  Fw32f* const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[3],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

FwStatus STDCALL fwiRotateCenter_32f_P4R  ( const  Fw32f* const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[4],                  int dstStep, FwiRect dstRoi,
                                              double angle,  double xCenter, double yCenter, int interpolation );

/*#FunctionBlock - Shear
TODO:publish
#Technologies - REF
#Short - Perform shear transform
#Long - <Text>These functions step through a source ROI, perform the shear transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>The following shear transform formula is used.</Text>
<Pseudo>
 x' = xShear * y + x + xShift
 y' = yShear * x + y + yShift

Where x' and y' are the destination coordinates
</Pseudo>
<Text>The shear transform is a special case of the warp affine transform.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsCoeffErr,
fwStsInterpolationErr, fwStsWrongIntersectQuad, fwStsRectErr
*/
FwStatus STDCALL fwiShear_8u_C1R          ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiRect dstRoi, 
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_8u_C3R          ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_8u_C4R          ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_8u_AC4R         ( const  Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_32f_C1R         ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_32f_C3R         ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_32f_C4R         ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_32f_AC4R        ( const  Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation );

FwStatus STDCALL fwiShear_8u_P3R          ( const  Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation);

FwStatus STDCALL fwiShear_8u_P4R          ( const  Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation);

FwStatus STDCALL fwiShear_32f_P3R         ( const  Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation);

FwStatus STDCALL fwiShear_32f_P4R         ( const  Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                     Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              double xShear, double yShear, double xShift, double yShift, int interpolation);

/*#FunctionBlock - GetShearQuad
TODO:publish
#Technologies - REF
#Short - Get shear quadrangle vertices
#Long - <Text>This function is used with the <FunctionName>GetShearBound</FunctionName> function. It computes the vertex coordinates of the quadrangular destination ROI for the shear functions.</Text>
<Text>Quad[0] contains the top right x and y coordinates</Text>
<Text>Quad[1] contains the top left x and y coordinates</Text>
<Text>Quad[2] contains the bottom right x and y coordinates</Text>
<Text>Quad[3] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetShearQuad          (FwiRect srcRoi, double quad[4][2],
                                             double xShear, double yShear, double xShift, double yShift);

/*#FunctionBlock - GetShearBound
TODO:publish
#Technologies - REF
#Short - Get shear boundary
#Long - <Text>This function is used with the <FunctionName>GetShearQuad</FunctionName> function. It computes the boundaries of the source ROI for the shear functions.</Text>
<Text>Bound[0] contains the top right x and y coordinates</Text>
<Text>Bound[1] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetShearBound         (FwiRect srcRoi, double bound[2][2],
                                             double xShear, double yShear, double xShift, double yShift);

/*#FunctionBlock - WarpAffine
TODO:publish
#Technologies - REF
#Short - Perform warp affine transform
#Long - <Text>These functions step through a source ROI, perform the warp affine transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 x' = c00*x + c01*y + c02
 y' = c10*x + c11*y + c12

Where:
 x' and y' are the destination coordinates
 Cij are the warp affine transform coefficients that are passed into the array
</Pseudo>
<Text>The warp affine transform is a general linear transformation that maps parallel lines to parallel lines.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad, fwStsRectErr
*/
FwStatus STDCALL fwiWarpAffine_8u_C1R     ( const Fw8u *pSrc, FwiSize srcSize, int srcStep,FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_8u_C3R     ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_8u_C4R     ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_8u_AC4R    ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_C1R    ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_C3R    ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_C4R    ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_AC4R   ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_C1R    ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_C3R    ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_C4R    ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_AC4R   ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_8u_P3R     ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_8u_P4R     ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_P3R    ( const Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_16u_P4R    ( const Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw16u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_P3R    ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffine_32f_P4R    ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][3], int interpolation );

/*#FunctionBlock - WarpAffineBack
TODO:publish
#Technologies - REF
#Short - Perform inverse warp affine transform
#Long - <Text>These functions step through a source ROI, perform an inverse warp affine transform on the source data, apply a specified method of
interpolation, and write the data to a destination ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 c00*x' + c01*y' + c02 = x
 c10*x' + c11*y' + c12 = y

Where cij represents the transform coefficients
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiWarpAffineBack_8u_C1R             ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_8u_C3R             ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_8u_C4R             ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_8u_AC4R            ( const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_C1R            ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_C3R            ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_C4R            ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_AC4R           ( const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_C1R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_C3R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_C4R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_AC4R           ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_8u_P3R             ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_8u_P4R             ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_P3R            ( const Fw16u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_16u_P4R            ( const Fw16u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw16u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_P3R            ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

FwStatus STDCALL fwiWarpAffineBack_32f_P4R            ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][3], int interpolation );

/*#FunctionBlock - WarpAffineQuad
TODO:publish
#Technologies - REF
#Short - Perform quadrangular warp affine transform
#Long - <Text>These functions step through a quadrangular source ROI, perform the warp affine transform on the source data, and map the transformed data into a quadrangular destination ROI using a specified method of interpolation.</Text>
<Text>The warp affine coefficients are calculated using the quadrangle vertices. If the coefficients cannot be resolved consistently, the function returns errors.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsQuadErr, fwStsWrongIntersectQuad, fwStsRectErr
*/
FwStatus STDCALL fwiWarpAffineQuad_8u_C1R             ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_8u_C3R             ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_8u_C4R             ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_8u_AC4R            ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_C1R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_C3R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_C4R            ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_AC4R           ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_8u_P3R             ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_8u_P4R             ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_P3R            ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2], 
                                                          int interpolation );

FwStatus STDCALL fwiWarpAffineQuad_32f_P4R            ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

/*#FunctionBlock - GetAffineQuad
TODO:publish
#Technologies - REF
#Short - Get affine quadrangle vertices
#Long - <Text>This function is used with the <FunctionName>GetAffineBound</FunctionName> function. It computes the vertex coordinates of
the quadrangular destination ROI for the <FunctionName>WarpAffineQuad</FunctionName> functions.</Text>
<Text>Quad[0] contains the top right x and y coordinates</Text>
<Text>Quad[1] contains the top left x and y coordinates</Text>
<Text>Quad[2] contains the bottom right x and y coordinates</Text>
<Text>Quad[3] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetAffineQuad         (FwiRect srcRoi, double quad[4][2], const double coeffs[2][3]);

/*#FunctionBlock - GetAffineBound
TODO:publish
#Technologies - REF
#Short - Get affine boundary
#Long - <Text>This function is used with the <FunctionName>GetAffineQuad</FunctionName> function. It computes the boundaries of the source ROI for the <FunctionName>WarpAffineQuad</FunctionName> functions.</Text>
<Text>Bound[0] contains the top right x and y coordinates</Text>
<Text>Bound[1] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsCoeffErr, fwStsSizeErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetAffineBound        (FwiRect srcRoi, double bound[2][2], const double coeffs[2][3]);

/*#FunctionBlock - GetAffineTransform
TODO:publish
#Technologies - REF
#Short -  Get affine transform coefficients
#Long - <Text>This function computes affine transform coefficients for the <FunctionName>WarpAffineQuad</FunctionName> functions.</Text>
<Text>Quad[0] corresponds to the top right coordinates of the source ROI</Text>
<Text>Quad[1] corresponds to the top left coordinates of the source ROI</Text>
<Text>Quad[2] corresponds to the bottom right coordinates of the source ROI</Text>
<Text>Quad[3] corresponds to the bottom left coordinates of the source ROI</Text>

<Text>The function returns fwstsCoeffErr if the mapping is incorrect.</Text>
#ReturnValues - fwStsNoErr, fwStsRectErr, fwStsCoeffErr, fwStsSizeErr
*/
FwStatus STDCALL fwiGetAffineTransform    (FwiRect srcRoi, const double quad[4][2],double coeffs[2][3]);

/*#FunctionBlock - WarpPerspective
TODO:publish
#Technologies - REF
#Short -  Perform warp perspective transform
#Long - <Text>These functions step through a source ROI, perform the warp perspective transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 x' = (c00*x + c01*y + c02)/(c20*x + c21*y + c22)
 y' = (c10*x + c11*y + c12)/(c20*x + c21*y + c22)

Where:
 x' and y' are the destination coordinates
 Cij are transform coefficients that are passed into the array
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsRectErr,
fwStsInterpolationErr, fwStsCoeffErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiWarpPerspective_8u_C1R            ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_8u_C3R            ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_8u_C4R            ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_8u_AC4R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_C1R           ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_C3R           ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_C4R           ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_AC4R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_8u_P3R            ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_8u_P4R            ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_P3R           ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspective_32f_P4R           ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

/*#FunctionBlock - WarpPerspectiveBack
TODO:publish
#Technologies - REF
#Short - Perform inverse warp perspective transform
#Long - <Text>These functions step through a source ROI, perform an inverse warp perspective transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>Edge smoothing interpolation is applicable only when the destination quadrangle is completely within the destination image ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 (c00*x' + c01*y' + c02)/(c20*x' + c21*y' + c22) = x
 (c10*x' + c11*y' + c12)/(c20*x' + c21*y' + c22) = y

Where cij represents the transform coefficients
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiWarpPerspectiveBack_8u_C1R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_8u_C3R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_8u_C4R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_8u_AC4R       ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_C1R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep,FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_C3R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_C4R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_AC4R      ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_8u_P3R        ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_8u_P4R        ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_P3R       ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

FwStatus STDCALL fwiWarpPerspectiveBack_32f_P4R       ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[3][3], int interpolation );

/*#FunctionBlock - WarpPerspectiveQuad
TODO:publish
#Technologies - REF
#Short -  Perform quadrangular warp perspective transform
#Long - <Text>These functions step through a quadrangular source ROI, perform the warp perspective transform on the source data, and map the transformed data into a quadrangular destination ROI using a specified method of interpolation.</Text>
<Text>Edge smoothing interpolation is applicable only when the destination quadrangle is completely within the destination ROI.</Text>
<Text>The warp perspective coefficients are calculated using the quadrangle vertices. If the coefficients cannot be resolved consistently, the function returns errors.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad, fwStsRectErr
*/
FwStatus STDCALL fwiWarpPerspectiveQuad_8u_C1R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2], 
                                                          int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_8u_C3R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_8u_C4R        ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_8u_AC4R       ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_32f_C1R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_32f_C3R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_32f_C4R       ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_32f_AC4R      ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_8u_P3R        ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_8u_P4R        ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation );

FwStatus STDCALL fwiWarpPerspectiveQuad_32f_P3R       ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );


FwStatus STDCALL fwiWarpPerspectiveQuad_32f_P4R       ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                         int interpolation );

/*#FunctionBlock - GetPerspectiveQuad
TODO:publish
#Technologies - REF
#Short - Get perspective quadrangle vertices
#Long - <Text>This function is used with the <FunctionName>GetPerspectiveBound</FunctionName> function. It computes the vertex coordinates of the quadrangular destination ROI for the <FunctionName>WarpPerspectiveQuad</FunctionName> functions.</Text>

<Text>Quad[0] contains the top right x and y coordinates</Text>
<Text>Quad[1] contains the top left x and y coordinates</Text>
<Text>Quad[2] contains the bottom right x and y coordinates</Text>
<Text>Quad[3] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetPerspectiveQuad                (FwiRect srcRoi, double quad[4][2], const double coeffs[3][3]);

/*#FunctionBlock - GetPerspectiveBound
TODO:publish
#Technologies - REF
#Short - Get perspective boundary
#Long - <Text>This function is used with the <FunctionName>GetPerspectiveQuad</FunctionName> function. It computes the boundaries of the source ROI for the <FunctionName>WarpPerspectiveQuad</FunctionName> functions.</Text>
<Text>Bound[0] contains the top right x and y coordinates</Text>
<Text>Bound[1] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetPerspectiveBound               (FwiRect srcRoi, double bound[2][2], const double coeffs[3][3]);

/*#FunctionBlock - GetPerspectiveTransform
TODO:publish
#Technologies - REF
#Short - Get the perspective transform coefficients
#Long - <Text>This function computes perspective transform coefficients for the <FunctionName>WarpPerspectiveQuad</FunctionName> functions.</Text>
<Text>Quad[0] corresponds to the top right coordinates of the source ROI</Text>
<Text>Quad[1] corresponds to the top left coordinates of the source ROI</Text>
<Text>Quad[2] corresponds to the bottom right coordinates of the source ROI</Text>
<Text>Quad[3] corresponds to the bottom left coordinates of the source ROI</Text>
<Text>The function returns fwstsCoeffErr if the mapping is incorrect.</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetPerspectiveTransform           (FwiRect srcRoi, const double quad[4][2],double coeffs[3][3]);

/*#FunctionBlock - WarpBilinear
TODO:publish
#Technologies - REF
#Short - Perform warp bilinear transform
#Long - <Text>These functions step through a source ROI, perform the warp bilinear transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 x' = c00*xy + c01*x + c02*y + c03
 y' = c10*xy + c11*x + c12*y + c13

Where:
 x' and y' are the destination coordinates
 Cij are transform coefficients that are passed into the array
</Pseudo>
<Text>The warp bilinear transform is not a true linear transform. It does not preserve distance relationships.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsRectErr,
fwStsInterpolationErr, fwStsCoeffErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiWarpBilinear_8u_C1R   ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_8u_C3R   ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep,FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_8u_C4R   ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_8u_AC4R  ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_C1R  ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_C3R  ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_C4R  ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_AC4R ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_8u_P3R   ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_8u_P4R   ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_P3R  ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinear_32f_P4R  ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                    Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                              const double coeffs[2][4], int interpolation);

/*#FunctionBlock - WarpBilinearBack
TODO:publish
#Technologies - REF
#Short - Perform inverse warp bilinear transform
#Long - <Text>These functions step through a source ROI, perform an inverse warp bilinear transform on the source data, apply a specified method of interpolation, and write the data to a destination ROI.</Text>
<Text>Edge smoothing interpolation is applicable only when the destination quadrangle is completely within the destination image ROI.</Text>
<Text>The following transformation formula is used.</Text>
<Pseudo>
 c00*x'y' + c01*x' + c02*y' + c03 = x
 c10*x'y' + c11*x' + c12*y' + c13 = y

Where cij represents the transform coefficients
</Pseudo>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad
*/
FwStatus STDCALL fwiWarpBilinearBack_8u_C1R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_8u_C3R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_8u_C4R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_8u_AC4R          ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_C1R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_C3R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_C4R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_AC4R         ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_8u_P3R           ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_8u_P4R           ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_P3R          ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

FwStatus STDCALL fwiWarpBilinearBack_32f_P4R          ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi,
                                                          const double coeffs[2][4], int interpolation);

/*#FunctionBlock - WarpBilinearQuad
TODO:publish
#Technologies - REF
#Short -  Perform quadrangular warp bilinear transform
#Long - <Text>These functions step through a quadrangular source ROI, perform the warp bilinear transform on the source data, and map the transformed data into a quadrangular destination ROI using a specified method of interpolation.</Text>
<Text>Edge smoothing interpolation is applicable only when the destination quadrangle is completely within the destination ROI.</Text>
<Text>The warp perspective coefficients are calculated using the quadrangle vertices. If the coefficients cannot be resolved consistently, the function returns errors.</Text>
<Text>The following interpolation modes are available.</Text>
<Pseudo>
 FWI_INTER_NN: nearest neighbor interpolation
 FWI_INTER_LINEAR: linear interpolation
 FWI_INTER_CUBIC: cubic interpolation
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsInterpolationErr,
fwStsCoeffErr, fwStsWrongIntersectQuad, fwStsRectErr
*/
FwStatus STDCALL fwiWarpBilinearQuad_8u_C1R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_8u_C3R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_8u_C4R           ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_8u_AC4R          ( const Fw8u  *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,  const double srcQuad[4][2],
                                                                Fw8u  *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_C1R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_C3R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_C4R          ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2], 
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_AC4R         ( const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f *pDst,                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_8u_P3R           ( const Fw8u * const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2], 
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_8u_P4R           ( const Fw8u * const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw8u * const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_P3R          ( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[3],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

FwStatus STDCALL fwiWarpBilinearQuad_32f_P4R          ( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const double srcQuad[4][2],
                                                                Fw32f* const pDst[4],                   int dstStep, FwiRect dstRoi, const double dstQuad[4][2],
                                                          int interpolation);

/*#FunctionBlock - GetBilinearQuad
TODO:publish
#Technologies - REF
#Short - Get bilinear quadrangle vertices
#Long - <Text>This function is used with the <FunctionName>GetBilinearBound</FunctionName> function. It computes the vertex coordinates of the quadrangular destination ROI for the <FunctionName>WarpBilinearQuad</FunctionName> functions.</Text>
<Text>Quad[0] contains the top right x and y coordinates</Text>
<Text>Quad[1] contains the top left x and y coordinates</Text>
<Text>Quad[2] contains the bottom right x and y coordinates</Text>
<Text>Quad[3] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetBilinearQuad       ( FwiRect srcRoi, double quad[4][2], const double coeffs[2][4]);

/*#FunctionBlock - GetBilinearBound
TODO:publish
#Technologies - REF
#Short - Get bilinear boundary
#Long - <Text>This function is used with the <FunctionName>GetBilinearQuad</FunctionName> function. It computes the boundaries of the source ROI for the <FunctionName>WarpBilinearQuad</FunctionName> functions.</Text>
<Text>Bound[0] contains the top right x and y coordinates</Text>
<Text>Bound[1] contains the bottom left x and y coordinates</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetBilinearBound      ( FwiRect srcRoi, double bound[2][2], const double coeffs[2][4]);

/*#FunctionBlock - GetBilinearTransform
TODO:publish
#Technologies - REF
#Short - Get the bilinear transform coefficients
#Long - <Text>This function computes bilinear transform coefficients for the <FunctionName>WarpBilinearQuad</FunctionName> functions.</Text>
<Text>Quad[0] corresponds to the top right coordinates of the source ROI</Text>
<Text>Quad[1] corresponds to the top left coordinates of the source ROI</Text>
<Text>Quad[2] corresponds to the bottom right coordinates of the source ROI</Text>
<Text>Quad[3] corresponds to the bottom left coordinates of the source ROI</Text>

<Text>The function returns fwstsCoeffErr if the mapping is incorrect.</Text>
#ReturnValues - fwStsNoErr, fwStsSizeErr, fwStsCoeffErr, fwStsRectErr
*/
FwStatus STDCALL fwiGetBilinearTransform  ( FwiRect srcRoi, const double quad[4][2],double coeffs[2][4]);

/*#Documentation

</Chapter>

/*#Documentation
<Chapter>
<ChapterHeading>3D Look-up functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions for 3D look-up with trilinear interpolation.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - LookUp3DSpecInitAlloc
TODO:edit
#Technologies - REF, SSE2
#Short - initialises the FwiLUTSpec structure
#Long - <Text>The function allocates memory for the <Bold>FwiLUTSpec</Bold> structure and initialises it.</Text>
#ReturnValues - fwStsNullPtrErr, fwStsNoErr
*/

FwStatus STDCALL fwiLookUp3DSpecInitAlloc(const Fw16u cubeDimensions, const Fw16u cubeMax, FwiLUTSpec** ppLUTSpec);

/*#FunctionBlock - LookUp3DSpecInitAlloc
TODO:edit
#Technologies - REF, SSE2
#Short - Convert the image from one color space to another by using 3D-Lookup with trilinear interpolation
#Long - <Text>The functions run through all the pixels in an ROI and and convert them from one color space to another. The R, G and B values 
of each pixel are taken up and a 3D look-up is performed using the <Bold>pCube</Bold> table to find it's 8 neighbouring vertices in the new color space. 
Then a trilinear interpolation is done using the R, G and B values of the 8 vertices(in the new color space) to get the R, G and B values for the current 
pixel in the new color space. <Bold>pCube</Bold>points to a 3 dimensional cube. The contents of the table are the R, G and B values(in new color space) of each vertex in the cube.</Text>
#ReturnValues - fwStsNullPtrErr, fwStsNoErr
*/

FwStatus STDCALL fwiLookUp3D_16u_C3R(const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec);
FwStatus STDCALL fwiLookUp3D_16u_C3IR(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec);

/*#FunctionBlock - LookUp3DSpecFree 
TODO:edit
#Technologies - REF, SSE2
#Short - free the memory allocated to <Bold>FwiLUTSpec</Bold> structure
#Long - The function frees the memory allocated to <Bold>FwiLUTSpec</Bold> structure
#ReturnValues - fwStsNullPtrErr, fwStsNoErr
*/

FwStatus STDCALL fwiLookUp3DSpecFree(FwiLUTSpec* pLUTSpec);

/*#Documentation

</Chapter>

*/

#ifdef __cplusplus
}
#endif


#endif // __FWIMAGE_H__
