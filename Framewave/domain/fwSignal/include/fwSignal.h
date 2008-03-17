/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FWSIGNAL_H__
#define __FWSIGNAL_H__

#include "fwBase.h"


#ifdef __cplusplus
extern "C" {
#endif

/*#HintBlock
AutoOff
*/

/*#IncludeBlock
#includeGlobal "../../../doc/fwIntroDoc.h"
#include "../../../doc/fwSignalIntroDoc.h"
*/

/*#Documentation
<Chapter>
<ChapterHeading>Library Version</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes the FW function that provides library version information.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - GetLibVersion
TODO:publish
#Technologies - REF
#Short - Get library version
#Long - <Text>This function returns a pointer to the <Bold>FwLibraryVersion</Bold> structure that contains FW Library version information.</Text>
*/
const FwLibraryVersion* STDCALL fwsGetLibVersion ();

/*#Documentation
</Chapter>
*/

/*#Documentation
<Chapter>
<ChapterHeading>Essential Vector Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that perform vector calculations.</ChaptDesc></Paragraph> 
*/

/*#FunctionBlock - Add
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Add
#Long - <Text>These functions step through vector elements in two source buffers and add the data in buffer 2 to the data in buffer 1.</Text>
<Text>The sum can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results. Other versions add signed and
unsigned integers and write 32-bit floating point results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAdd_8u16u             ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw16u  *pDst, int len );
FwStatus STDCALL fwsAdd_32u               ( const Fw32u  *pSrc1, const Fw32u  *pSrc2  , Fw32u  *pDst, int len );
FwStatus STDCALL fwsAdd_32f               ( const Fw32f  *pSrc1, const Fw32f  *pSrc2  , Fw32f  *pDst, int len );
FwStatus STDCALL fwsAdd_64f               ( const Fw64f  *pSrc1, const Fw64f  *pSrc2  , Fw64f  *pDst, int len );
FwStatus STDCALL fwsAdd_32fc              ( const Fw32fc *pSrc1, const Fw32fc *pSrc2  , Fw32fc *pDst, int len );
FwStatus STDCALL fwsAdd_64fc              ( const Fw64fc *pSrc1, const Fw64fc *pSrc2  , Fw64fc *pDst, int len );
FwStatus STDCALL fwsAdd_16s32f            ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw32f  *pDst, int len );

FwStatus STDCALL fwsAdd_16s_I             ( const Fw16s  *pSrc ,       Fw16s  *pSrcDst,                int len );
FwStatus STDCALL fwsAdd_16s32s_I          ( const Fw16s  *pSrc ,       Fw32s  *pSrcDst,                int len );
FwStatus STDCALL fwsAdd_32f_I             ( const Fw32f  *pSrc ,       Fw32f  *pSrcDst,                int len );
FwStatus STDCALL fwsAdd_64f_I             ( const Fw64f  *pSrc ,       Fw64f  *pSrcDst,                int len );
FwStatus STDCALL fwsAdd_32fc_I            ( const Fw32fc *pSrc ,       Fw32fc *pSrcDst,                int len );
FwStatus STDCALL fwsAdd_64fc_I            ( const Fw64fc *pSrc ,       Fw64fc *pSrcDst,                int len );

FwStatus STDCALL fwsAdd_8u_Sfs            ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsAdd_16s_Sfs           ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsAdd_32s_Sfs           ( const Fw32s  *pSrc1, const Fw32s  *pSrc2  , Fw32s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsAdd_16sc_Sfs          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2  , Fw16sc *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsAdd_32sc_Sfs          ( const Fw32sc *pSrc1, const Fw32sc *pSrc2  , Fw32sc *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsAdd_8u_ISfs           ( const Fw8u   *pSrc ,       Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsAdd_16s_ISfs          ( const Fw16s  *pSrc ,       Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsAdd_32s_ISfs          ( const Fw32s  *pSrc ,       Fw32s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsAdd_16sc_ISfs         ( const Fw16sc *pSrc ,       Fw16sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsAdd_32sc_ISfs         ( const Fw32sc *pSrc ,       Fw32sc *pSrcDst,                int len, int scaleFactor );


/*#FunctionBlock - AddC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Add constant
#Long - <Text>These functions step through vector elements in a source buffer and add a specified constant value to the source data.</Text>
<Text>The sum can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAddC_32f              ( const Fw32f  *pSrc, Fw32f  val, Fw32f  *pDst   , int len );
FwStatus STDCALL fwsAddC_64f              ( const Fw64f  *pSrc, Fw64f  val, Fw64f  *pDst   , int len );
FwStatus STDCALL fwsAddC_32fc             ( const Fw32fc *pSrc, Fw32fc val, Fw32fc *pDst   , int len );
FwStatus STDCALL fwsAddC_64fc             ( const Fw64fc *pSrc, Fw64fc val, Fw64fc *pDst   , int len );

FwStatus STDCALL fwsAddC_16s_I            (                      Fw16s  val, Fw16s  *pSrcDst, int len );
FwStatus STDCALL fwsAddC_32f_I            (                      Fw32f  val, Fw32f  *pSrcDst, int len );
FwStatus STDCALL fwsAddC_64f_I            (                      Fw64f  val, Fw64f  *pSrcDst, int len );
FwStatus STDCALL fwsAddC_32fc_I           (                      Fw32fc val, Fw32fc *pSrcDst, int len );
FwStatus STDCALL fwsAddC_64fc_I           (                      Fw64fc val, Fw64fc *pSrcDst, int len );

FwStatus STDCALL fwsAddC_8u_Sfs           ( const Fw8u   *pSrc, Fw8u   val, Fw8u   *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsAddC_16s_Sfs          ( const Fw16s  *pSrc, Fw16s  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsAddC_32s_Sfs          ( const Fw32s  *pSrc, Fw32s  val, Fw32s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsAddC_16sc_Sfs         ( const Fw16sc *pSrc, Fw16sc val, Fw16sc *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsAddC_32sc_Sfs         ( const Fw32sc *pSrc, Fw32sc val, Fw32sc *pDst   , int len, int scaleFactor );

FwStatus STDCALL fwsAddC_8u_ISfs          (                      Fw8u   val, Fw8u   *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddC_16s_ISfs         (                      Fw16s  val, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddC_32s_ISfs         (                      Fw32s  val, Fw32s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddC_16sc_ISfs        (                      Fw16sc val, Fw16sc *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddC_32sc_ISfs        (                      Fw32sc val, Fw32sc *pSrcDst, int len, int scaleFactor );


/*#FunctionBlock - Sub
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Subtract
#Long - <Text>These functions step through vector elements in two source buffers and subtract the data in buffer 1 from the data in buffer 2.</Text>
<Text>The difference can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSub_16s               ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len );
FwStatus STDCALL fwsSub_32f               ( const Fw32f  *pSrc1, const Fw32f  *pSrc2  , Fw32f  *pDst, int len );
FwStatus STDCALL fwsSub_64f               ( const Fw64f  *pSrc1, const Fw64f  *pSrc2  , Fw64f  *pDst, int len );
FwStatus STDCALL fwsSub_32fc              ( const Fw32fc *pSrc1, const Fw32fc *pSrc2  , Fw32fc *pDst, int len );
FwStatus STDCALL fwsSub_64fc              ( const Fw64fc *pSrc1, const Fw64fc *pSrc2  , Fw64fc *pDst, int len );
FwStatus STDCALL fwsSub_16s32f            ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw32f  *pDst, int len );

FwStatus STDCALL fwsSub_16s_I             ( const Fw16s  *pSrc ,       Fw16s  *pSrcDst,                int len );
FwStatus STDCALL fwsSub_32f_I             ( const Fw32f  *pSrc ,       Fw32f  *pSrcDst,                int len );
FwStatus STDCALL fwsSub_64f_I             ( const Fw64f  *pSrc ,       Fw64f  *pSrcDst,                int len );
FwStatus STDCALL fwsSub_32fc_I            ( const Fw32fc *pSrc ,       Fw32fc *pSrcDst,                int len );
FwStatus STDCALL fwsSub_64fc_I            ( const Fw64fc *pSrc ,       Fw64fc *pSrcDst,                int len );

FwStatus STDCALL fwsSub_8u_Sfs            ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSub_16s_Sfs           ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSub_32s_Sfs           ( const Fw32s  *pSrc1, const Fw32s  *pSrc2  , Fw32s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSub_16sc_Sfs          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2  , Fw16sc *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSub_32sc_Sfs          ( const Fw32sc *pSrc1, const Fw32sc *pSrc2  , Fw32sc *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsSub_8u_ISfs           ( const Fw8u   *pSrc ,       Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSub_16s_ISfs          ( const Fw16s  *pSrc ,       Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSub_32s_ISfs          ( const Fw32s  *pSrc ,       Fw32s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSub_16sc_ISfs         ( const Fw16sc *pSrc ,       Fw16sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSub_32sc_ISfs         ( const Fw32sc *pSrc ,       Fw32sc *pSrcDst,                int len, int scaleFactor );

/*#FunctionBlock - SubC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Subtract a constant
#Long - <Text>These functions step through vector elements in a source buffer and subtract a specified constant value from the source data.</Text>
<Text>The difference can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSubC_32f              ( const Fw32f  *pSrc, Fw32f  val, Fw32f  *pDst   , int len );
FwStatus STDCALL fwsSubC_64f              ( const Fw64f  *pSrc, Fw64f  val, Fw64f  *pDst   , int len );
FwStatus STDCALL fwsSubC_32fc             ( const Fw32fc *pSrc, Fw32fc val, Fw32fc *pDst   , int len );
FwStatus STDCALL fwsSubC_64fc             ( const Fw64fc *pSrc, Fw64fc val, Fw64fc *pDst   , int len );

FwStatus STDCALL fwsSubC_16s_I            (                      Fw16s  val, Fw16s  *pSrcDst, int len );
FwStatus STDCALL fwsSubC_32f_I            (                      Fw32f  val, Fw32f  *pSrcDst, int len );
FwStatus STDCALL fwsSubC_64f_I            (                      Fw64f  val, Fw64f  *pSrcDst, int len );
FwStatus STDCALL fwsSubC_32fc_I           (                      Fw32fc val, Fw32fc *pSrcDst, int len );
FwStatus STDCALL fwsSubC_64fc_I           (                      Fw64fc val, Fw64fc *pSrcDst, int len );

FwStatus STDCALL fwsSubC_8u_Sfs           ( const Fw8u   *pSrc, Fw8u   val, Fw8u   *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubC_16s_Sfs          ( const Fw16s  *pSrc, Fw16s  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubC_32s_Sfs          ( const Fw32s  *pSrc, Fw32s  val, Fw32s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubC_16sc_Sfs         ( const Fw16sc *pSrc, Fw16sc val, Fw16sc *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubC_32sc_Sfs         ( const Fw32sc *pSrc, Fw32sc val, Fw32sc *pDst   , int len, int scaleFactor );

FwStatus STDCALL fwsSubC_8u_ISfs          (                      Fw8u   val, Fw8u   *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubC_16s_ISfs         (                      Fw16s  val, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubC_32s_ISfs         (                      Fw32s  val, Fw32s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubC_16sc_ISfs        (                      Fw16sc val, Fw16sc *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubC_32sc_ISfs        (                      Fw32sc val, Fw32sc *pSrcDst, int len, int scaleFactor );


/*#FunctionBlock - SubCRev
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Subtract from a constant
#Long - <Text>These functions step through vector elements in a source buffer and subtract source data from a specified constant value.</Text>
<Text>The difference can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSubCRev_32f           ( const Fw32f  *pSrc, Fw32f  val, Fw32f  *pDst   , int len );
FwStatus STDCALL fwsSubCRev_64f           ( const Fw64f  *pSrc, Fw64f  val, Fw64f  *pDst   , int len );
FwStatus STDCALL fwsSubCRev_32fc          ( const Fw32fc *pSrc, Fw32fc val, Fw32fc *pDst   , int len );
FwStatus STDCALL fwsSubCRev_64fc          ( const Fw64fc *pSrc, Fw64fc val, Fw64fc *pDst   , int len );

FwStatus STDCALL fwsSubCRev_32f_I         (                      Fw32f  val, Fw32f  *pSrcDst, int len );
FwStatus STDCALL fwsSubCRev_64f_I         (                      Fw64f  val, Fw64f  *pSrcDst, int len );
FwStatus STDCALL fwsSubCRev_32fc_I        (                      Fw32fc val, Fw32fc *pSrcDst, int len );
FwStatus STDCALL fwsSubCRev_64fc_I        (                      Fw64fc val, Fw64fc *pSrcDst, int len );

FwStatus STDCALL fwsSubCRev_8u_Sfs        ( const Fw8u   *pSrc, Fw8u   val, Fw8u   *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_16s_Sfs       ( const Fw16s  *pSrc, Fw16s  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_32s_Sfs       ( const Fw32s  *pSrc, Fw32s  val, Fw32s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_16sc_Sfs      ( const Fw16sc *pSrc, Fw16sc val, Fw16sc *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_32sc_Sfs      ( const Fw32sc *pSrc, Fw32sc val, Fw32sc *pDst   , int len, int scaleFactor );

FwStatus STDCALL fwsSubCRev_8u_ISfs       (                      Fw8u   val, Fw8u   *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_16s_ISfs      (                      Fw16s  val, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_32s_ISfs      (                      Fw32s  val, Fw32s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_16sc_ISfs     (                      Fw16sc val, Fw16sc *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsSubCRev_32sc_ISfs     (                      Fw32sc val, Fw32sc *pSrcDst, int len, int scaleFactor );


/*#FunctionBlock - AddProduct
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Add product
#Long - <Text>These functions step through vector elements in two source buffers, add the product of the data in buffer 1 and the data in buffer 2 to the data in buffer 1, and write the sum back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAddProduct_32f        ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, Fw32f  *pSrcDst, int len );
FwStatus STDCALL fwsAddProduct_64f        ( const Fw64f  *pSrc1, const Fw64f  *pSrc2, Fw64f  *pSrcDst, int len );
FwStatus STDCALL fwsAddProduct_32fc       ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, Fw32fc *pSrcDst, int len );
FwStatus STDCALL fwsAddProduct_64fc       ( const Fw64fc *pSrc1, const Fw64fc *pSrc2, Fw64fc *pSrcDst, int len );

FwStatus STDCALL fwsAddProduct_16s_Sfs    ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddProduct_32s_Sfs    ( const Fw32s  *pSrc1, const Fw32s  *pSrc2, Fw32s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsAddProduct_16s32s_Sfs ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, Fw32s  *pSrcDst, int len, int scaleFactor );


/*#FunctionBlock - And
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - AND
#Long - <Text>These functions step through vector elements in two source buffers and perform a bitwise logical AND of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAnd_8u                ( const Fw8u  *pSrc1, const Fw8u  *pSrc2  , Fw8u  *pDst, int len );
FwStatus STDCALL fwsAnd_16u               ( const Fw16u *pSrc1, const Fw16u *pSrc2  , Fw16u *pDst, int len );
FwStatus STDCALL fwsAnd_32u               ( const Fw32u *pSrc1, const Fw32u *pSrc2  , Fw32u *pDst, int len );

FwStatus STDCALL fwsAnd_8u_I              ( const Fw8u  *pSrc ,       Fw8u  *pSrcDst,               int len );
FwStatus STDCALL fwsAnd_16u_I             ( const Fw16u *pSrc ,       Fw16u *pSrcDst,               int len );
FwStatus STDCALL fwsAnd_32u_I             ( const Fw32u *pSrc ,       Fw32u *pSrcDst,               int len );


/*#FunctionBlock - Or
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Or
#Long - <Text>These functions step through vector elements in two source buffers and perform a bitwise logical Or of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsOr_8u                 ( const Fw8u  *pSrc1, const Fw8u  *pSrc2  , Fw8u  *pDst, int len );
FwStatus STDCALL fwsOr_16u                ( const Fw16u *pSrc1, const Fw16u *pSrc2  , Fw16u *pDst, int len );
FwStatus STDCALL fwsOr_32u                ( const Fw32u *pSrc1, const Fw32u *pSrc2  , Fw32u *pDst, int len );

FwStatus STDCALL fwsOr_8u_I               ( const Fw8u  *pSrc ,       Fw8u  *pSrcDst,               int len );
FwStatus STDCALL fwsOr_16u_I              ( const Fw16u *pSrc ,       Fw16u *pSrcDst,               int len );
FwStatus STDCALL fwsOr_32u_I              ( const Fw32u *pSrc ,       Fw32u *pSrcDst,               int len );

/*#FunctionBlock - Xor
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Xor
#Long - <Text>These functions step through vector elements in two source buffers and perform a bitwise logical XOR of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsXor_8u                ( const Fw8u  *pSrc1, const Fw8u  *pSrc2  , Fw8u  *pDst, int len );
FwStatus STDCALL fwsXor_16u               ( const Fw16u *pSrc1, const Fw16u *pSrc2  , Fw16u *pDst, int len );
FwStatus STDCALL fwsXor_32u               ( const Fw32u *pSrc1, const Fw32u *pSrc2  , Fw32u *pDst, int len );

FwStatus STDCALL fwsXor_8u_I              ( const Fw8u  *pSrc ,       Fw8u  *pSrcDst,               int len );
FwStatus STDCALL fwsXor_16u_I             ( const Fw16u *pSrc ,       Fw16u *pSrcDst,               int len );
FwStatus STDCALL fwsXor_32u_I             ( const Fw32u *pSrc ,       Fw32u *pSrcDst,               int len );

/*#FunctionBlock - AndC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - AND with constant
#Long - <Text>These functions step through vector elements in a source buffer and perform a bitwise logical AND of the source data and a specified constant.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAndC_8u               ( const Fw8u  *pSrc, Fw8u  val, Fw8u  *pDst   , int len );
FwStatus STDCALL fwsAndC_16u              ( const Fw16u *pSrc, Fw16u val, Fw16u *pDst   , int len );
FwStatus STDCALL fwsAndC_32u              ( const Fw32u *pSrc, Fw32u val, Fw32u *pDst   , int len );

FwStatus STDCALL fwsAndC_8u_I             (                     Fw8u  val, Fw8u  *pSrcDst, int len );
FwStatus STDCALL fwsAndC_16u_I            (                     Fw16u val, Fw16u *pSrcDst, int len );
FwStatus STDCALL fwsAndC_32u_I            (                     Fw32u val, Fw32u *pSrcDst, int len );

/*#FunctionBlock - OrC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - OR with constant
#Long - <Text>These functions step through vector elements in a source buffer and perform a bitwise logical OR of the source data and a specified constant.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsOrC_8u                ( const Fw8u  *pSrc, Fw8u  val, Fw8u  *pDst   , int len );
FwStatus STDCALL fwsOrC_16u               ( const Fw16u *pSrc, Fw16u val, Fw16u *pDst   , int len );
FwStatus STDCALL fwsOrC_32u               ( const Fw32u *pSrc, Fw32u val, Fw32u *pDst   , int len );

FwStatus STDCALL fwsOrC_8u_I              (                     Fw8u  val, Fw8u  *pSrcDst, int len );
FwStatus STDCALL fwsOrC_16u_I             (                     Fw16u val, Fw16u *pSrcDst, int len );
FwStatus STDCALL fwsOrC_32u_I             (                     Fw32u val, Fw32u *pSrcDst, int len );

/*#FunctionBlock - XorC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Xor with constant
#Long - <Text>These functions step through vector elements in a source buffer and perform a bitwise logical XOR of the source data and a specified constant.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsXorC_8u               ( const Fw8u  *pSrc, Fw8u  val, Fw8u  *pDst   , int len );
FwStatus STDCALL fwsXorC_16u              ( const Fw16u *pSrc, Fw16u val, Fw16u *pDst   , int len );
FwStatus STDCALL fwsXorC_32u              ( const Fw32u *pSrc, Fw32u val, Fw32u *pDst   , int len );

FwStatus STDCALL fwsXorC_8u_I             (                     Fw8u  val, Fw8u  *pSrcDst, int len );
FwStatus STDCALL fwsXorC_16u_I            (                     Fw16u val, Fw16u *pSrcDst, int len );
FwStatus STDCALL fwsXorC_32u_I            (                     Fw32u val, Fw32u *pSrcDst, int len );


/*#FunctionBlock - Not
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Not
#Long - <Text>These functions step through vector elements in two source buffers and perform a bitwise logical NOT of the data in buffer 1 and the data in buffer 2.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNot_8u                ( const Fw8u  *pSrc   , Fw8u  *pDst, int len );
FwStatus STDCALL fwsNot_16u               ( const Fw16u *pSrc   , Fw16u *pDst, int len );
FwStatus STDCALL fwsNot_32u               ( const Fw32u *pSrc   , Fw32u *pDst, int len );

FwStatus STDCALL fwsNot_8u_I              (       Fw8u  *pSrcDst,               int len );
FwStatus STDCALL fwsNot_16u_I             (       Fw16u *pSrcDst,               int len );
FwStatus STDCALL fwsNot_32u_I             (       Fw32u *pSrcDst,               int len );

/*#FunctionBlock - LShiftC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Left shift by a constant value
#Long - <Text>These functions step through vector elements in a source buffer and shift the bits of the source data a specified number of bit positions to the left.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>Left-shifting is equivalent to multiplying by 2 to the constant power.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsLShiftC_8u            ( const Fw8u  *pSrc, int val, Fw8u  *pDst   , int len );
FwStatus STDCALL fwsLShiftC_16s           ( const Fw16s *pSrc, int val, Fw16s *pDst   , int len );
FwStatus STDCALL fwsLShiftC_16u           ( const Fw16u *pSrc, int val, Fw16u *pDst   , int len );
FwStatus STDCALL fwsLShiftC_32s           ( const Fw32s *pSrc, int val, Fw32s *pDst   , int len );

FwStatus STDCALL fwsLShiftC_8u_I          (                     int val, Fw8u  *pSrcDst, int len );
FwStatus STDCALL fwsLShiftC_16u_I         (                     int val, Fw16u *pSrcDst, int len );
FwStatus STDCALL fwsLShiftC_16s_I         (                     int val, Fw16s *pSrcDst, int len );
FwStatus STDCALL fwsLShiftC_32s_I         (                     int val, Fw32s *pSrcDst, int len );


/*#FunctionBlock - RShiftC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Right shift by a constant value
#Long - <Text>These functions step through vector elements in a source buffer and shift the bits of the source data a specified number of bit positions to the right.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsRShiftC_8u            ( const Fw8u  *pSrc, int val, Fw8u  *pDst   , int len );
FwStatus STDCALL fwsRShiftC_16s           ( const Fw16s *pSrc, int val, Fw16s *pDst   , int len );
FwStatus STDCALL fwsRShiftC_16u           ( const Fw16u *pSrc, int val, Fw16u *pDst   , int len );
FwStatus STDCALL fwsRShiftC_32s           ( const Fw32s *pSrc, int val, Fw32s *pDst   , int len );

FwStatus STDCALL fwsRShiftC_8u_I          (                     int val, Fw8u  *pSrcDst, int len );
FwStatus STDCALL fwsRShiftC_16u_I         (                     int val, Fw16u *pSrcDst, int len );
FwStatus STDCALL fwsRShiftC_16s_I         (                     int val, Fw16s *pSrcDst, int len );
FwStatus STDCALL fwsRShiftC_32s_I         (                     int val, Fw32s *pSrcDst, int len );


/*#FunctionBlock - Mul
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Multiply
#Long - <Text>These functions step through vector elements in two source buffers and multiply the data in buffer 1 by the data in buffer 2.</Text>
<Text>The product can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwsMul_16s               ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len );
FwStatus STDCALL fwsMul_32f               ( const Fw32f  *pSrc1, const Fw32f  *pSrc2  , Fw32f  *pDst, int len );
FwStatus STDCALL fwsMul_64f               ( const Fw64f  *pSrc1, const Fw64f  *pSrc2  , Fw64f  *pDst, int len );
FwStatus STDCALL fwsMul_32fc              ( const Fw32fc *pSrc1, const Fw32fc *pSrc2  , Fw32fc *pDst, int len );
FwStatus STDCALL fwsMul_64fc              ( const Fw64fc *pSrc1, const Fw64fc *pSrc2  , Fw64fc *pDst, int len );

FwStatus STDCALL fwsMul_8u16u             ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw16u  *pDst, int len );
FwStatus STDCALL fwsMul_16s32f            ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw32f  *pDst, int len );

FwStatus STDCALL fwsMul_16s_I             ( const Fw16s  *pSrc,        Fw16s  *pSrcDst,                int len );
FwStatus STDCALL fwsMul_32f_I             ( const Fw32f  *pSrc,        Fw32f  *pSrcDst,                int len );
FwStatus STDCALL fwsMul_64f_I             ( const Fw64f  *pSrc,        Fw64f  *pSrcDst,                int len );
FwStatus STDCALL fwsMul_32fc_I            ( const Fw32fc *pSrc,        Fw32fc *pSrcDst,                int len );
FwStatus STDCALL fwsMul_64fc_I            ( const Fw64fc *pSrc,        Fw64fc *pSrcDst,                int len );

FwStatus STDCALL fwsMul_8u_Sfs            ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_16s_Sfs           ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_32s_Sfs           ( const Fw32s  *pSrc1, const Fw32s  *pSrc2  , Fw32s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_16sc_Sfs          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2  , Fw16sc *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_32sc_Sfs          ( const Fw32sc *pSrc1, const Fw32sc *pSrc2  , Fw32sc *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsMul_16u16s_Sfs        ( const Fw16u  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_16s32s_Sfs        ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw32s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMul_32s32sc_Sfs       ( const Fw32s  *pSrc1, const Fw32sc *pSrc2  , Fw32sc *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsMul_8u_ISfs           ( const Fw8u   *pSrc,        Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsMul_16s_ISfs          ( const Fw16s  *pSrc,        Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsMul_32s_ISfs          ( const Fw32s  *pSrc,        Fw32s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsMul_16sc_ISfs         ( const Fw16sc *pSrc,        Fw16sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsMul_32sc_ISfs         ( const Fw32sc *pSrc,        Fw32sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsMul_32s32sc_ISfs      ( const Fw32s  *pSrc,        Fw32sc *pSrcDst,                int len, int scaleFactor );


/*#FunctionBlock - MulC
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Multiply by a constant
#Long - <Text>These functions step through vector elements in a source buffer and multiply the source data by a specified constant value.</Text>
<Text>The product can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMulC_32f              ( const Fw32f  *pSrc, Fw32f  val, Fw32f  *pDst   , int len );
FwStatus STDCALL fwsMulC_64f              ( const Fw64f  *pSrc, Fw64f  val, Fw64f  *pDst   , int len );
FwStatus STDCALL fwsMulC_32fc             ( const Fw32fc *pSrc, Fw32fc val, Fw32fc *pDst   , int len );
FwStatus STDCALL fwsMulC_64fc             ( const Fw64fc *pSrc, Fw64fc val, Fw64fc *pDst   , int len );

FwStatus STDCALL fwsMulC_16s_I            (                      Fw16s  val, Fw16s  *pSrcDst, int len );
FwStatus STDCALL fwsMulC_32f_I            (                      Fw32f  val, Fw32f  *pSrcDst, int len );
FwStatus STDCALL fwsMulC_64f_I            (                      Fw64f  val, Fw64f  *pSrcDst, int len );
FwStatus STDCALL fwsMulC_32fc_I           (                      Fw32fc val, Fw32fc *pSrcDst, int len );
FwStatus STDCALL fwsMulC_64fc_I           (                      Fw64fc val, Fw64fc *pSrcDst, int len );

FwStatus STDCALL fwsMulC_8u_Sfs           ( const Fw8u   *pSrc, Fw8u   val, Fw8u   *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsMulC_16s_Sfs          ( const Fw16s  *pSrc, Fw16s  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsMulC_32s_Sfs          ( const Fw32s  *pSrc, Fw32s  val, Fw32s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsMulC_16sc_Sfs         ( const Fw16sc *pSrc, Fw16sc val, Fw16sc *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsMulC_32sc_Sfs         ( const Fw32sc *pSrc, Fw32sc val, Fw32sc *pDst   , int len, int scaleFactor );

FwStatus STDCALL fwsMulC_8u_ISfs          (                      Fw8u   val, Fw8u   *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsMulC_16s_ISfs         (                      Fw16s  val, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsMulC_32s_ISfs         (                      Fw32s  val, Fw32s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsMulC_16sc_ISfs        (                      Fw16sc val, Fw16sc *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL fwsMulC_32sc_ISfs        (                      Fw32sc val, Fw32sc *pSrcDst, int len, int scaleFactor );

FwStatus STDCALL fwsMulC_32f16s_Sfs       ( const Fw32f  *pSrc, Fw32f  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL fwsMulC_Low_32f16s       ( const Fw32f  *pSrc, Fw32f  val, Fw16s  *pDst   , int len );


/*#FunctionBlock - Abs
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Get absolute value
#Long - <Text>These functions step through vector elements in a source buffer and calculate the absolute value of the source data.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/

FwStatus STDCALL fwsAbs_16s               ( const Fw16s *pSrc   ,Fw16s *pDst, int len );
FwStatus STDCALL fwsAbs_32s               ( const Fw32s *pSrc   ,Fw32s *pDst, int len );
FwStatus STDCALL fwsAbs_32f               ( const Fw32f *pSrc   ,Fw32f *pDst, int len );
FwStatus STDCALL fwsAbs_64f               ( const Fw64f *pSrc   ,Fw64f *pDst, int len );

FwStatus STDCALL fwsAbs_16s_I             (       Fw16s *pSrcDst,              int len );
FwStatus STDCALL fwsAbs_32s_I             (       Fw32s *pSrcDst,              int len );
FwStatus STDCALL fwsAbs_32f_I             (       Fw32f *pSrcDst,              int len );
FwStatus STDCALL fwsAbs_64f_I             (       Fw64f *pSrcDst,              int len );


/*#FunctionBlock - Sqrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Square root
#Long - <Text>These functions step through vector elements in a source buffer and calculate the square root of the source data.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit unsigned, and 16-bit signed versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSqrt_32f              ( const Fw32f  *pSrc   , Fw32f  *pDst, int len );
FwStatus STDCALL fwsSqrt_64f              ( const Fw64f  *pSrc   , Fw64f  *pDst, int len );
FwStatus STDCALL fwsSqrt_32fc             ( const Fw32fc *pSrc   , Fw32fc *pDst, int len );
FwStatus STDCALL fwsSqrt_64fc             ( const Fw64fc *pSrc   , Fw64fc *pDst, int len );

FwStatus STDCALL fwsSqrt_32f_I            (       Fw32f  *pSrcDst,                int len );
FwStatus STDCALL fwsSqrt_64f_I            (       Fw64f  *pSrcDst,                int len );
FwStatus STDCALL fwsSqrt_32fc_I           (       Fw32fc *pSrcDst,                int len );
FwStatus STDCALL fwsSqrt_64fc_I           (       Fw64fc *pSrcDst,                int len );

FwStatus STDCALL fwsSqrt_8u_Sfs           ( const Fw8u   *pSrc   , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16s_Sfs          ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16u_Sfs          ( const Fw16u  *pSrc   , Fw16u  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_32s16s_Sfs       ( const Fw32s  *pSrc   , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_64s16s_Sfs       ( const Fw64s  *pSrc   , Fw16s  *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsSqrt_8u_ISfs          (       Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16s_ISfs         (       Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16u_ISfs         (       Fw16u  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_64s_Sfs          ( const Fw64s  *pSrc   , Fw64s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_64s_ISfs         (       Fw64s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16sc_ISfs        (       Fw16sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqrt_16sc_Sfs         ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, int scaleFactor );


/*#FunctionBlock - Sqr
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Square
#Long - <Text>These functions step through vector elements in a source buffer and calculate the square of the source data.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit unsigned, and 16-bit signed versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSqr_32f               ( const Fw32f  *pSrc   , Fw32f  *pDst, int len );
FwStatus STDCALL fwsSqr_64f               ( const Fw64f  *pSrc   , Fw64f  *pDst, int len );
FwStatus STDCALL fwsSqr_32fc              ( const Fw32fc *pSrc   , Fw32fc *pDst, int len );
FwStatus STDCALL fwsSqr_64fc              ( const Fw64fc *pSrc   , Fw64fc *pDst, int len );

FwStatus STDCALL fwsSqr_32f_I             (       Fw32f  *pSrcDst,                int len );
FwStatus STDCALL fwsSqr_64f_I             (       Fw64f  *pSrcDst,                int len );
FwStatus STDCALL fwsSqr_32fc_I            (       Fw32fc *pSrcDst,                int len );
FwStatus STDCALL fwsSqr_64fc_I            (       Fw64fc *pSrcDst,                int len );

FwStatus STDCALL fwsSqr_8u_Sfs            ( const Fw8u   *pSrc   , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqr_16s_Sfs           ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsSqr_16u_Sfs           ( const Fw16u  *pSrc   , Fw16u  *pDst, int len, int scaleFactor );
FwStatus STDCALL  fwsSqr_16sc_Sfs         ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsSqr_8u_ISfs           (       Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqr_16s_ISfs          (       Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL fwsSqr_16u_ISfs          (       Fw16u  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL  fwsSqr_16sc_ISfs        (       Fw16sc *pSrcDst,                int len, int scaleFactor );

/*#FunctionBlock - Normalize
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Normalize
#Long - <Text> These functions step through vector elements in a source buffer, subtract <ParameterName>vsub</ParameterName> from each element, and divide the differences by <ParameterName>vdiv</ParameterName>.</Text>
<Text>The results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 16-bit unsigned and 16-bit signed versions perform an integer scaling operation before writing the results.</Text>
<Text>The normalize operation is performed as follows.</Text>
<Pseudo> Dst = (Src - vsub) / vdiv</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr ,fwStsDivByZeroErr
*/

FwStatus STDCALL  fwsNormalize_32f        ( const Fw32f  *pSrc, Fw32f  *pDst, int len, Fw32f  vsub, Fw32f vdiv );
FwStatus STDCALL  fwsNormalize_64f        ( const Fw64f  *pSrc, Fw64f  *pDst, int len, Fw64f  vsub, Fw64f vdiv );
FwStatus STDCALL  fwsNormalize_32fc       ( const Fw32fc *pSrc, Fw32fc *pDst, int len, Fw32fc vsub, Fw32f vdiv );
FwStatus STDCALL  fwsNormalize_64fc       ( const Fw64fc *pSrc, Fw64fc *pDst, int len, Fw64fc vsub, Fw64f vdiv );

FwStatus STDCALL  fwsNormalize_16s_Sfs    ( const Fw16s  *pSrc, Fw16s  *pDst, int len, Fw16s  vsub, int vdiv, int scaleFactor );
FwStatus STDCALL  fwsNormalize_16sc_Sfs   ( const Fw16sc *pSrc, Fw16sc *pDst, int len, Fw16sc vsub, int vdiv, int scaleFactor );

/*#FunctionBlock - Div
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Divide
#Long - <Text>These functions step through vector elements in two source buffers and divide the data in buffer 2 by the data in buffer 1.</Text>
<Text>The quotient can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL  fwsDiv_32f              ( const Fw32f  *pSrc1, const Fw32f  *pSrc2  , Fw32f  *pDst, int len );
FwStatus STDCALL  fwsDiv_64f              ( const Fw64f  *pSrc1, const Fw64f  *pSrc2  , Fw64f  *pDst, int len );
FwStatus STDCALL  fwsDiv_32fc             ( const Fw32fc *pSrc1, const Fw32fc *pSrc2  , Fw32fc *pDst, int len );
FwStatus STDCALL  fwsDiv_64fc             ( const Fw64fc *pSrc1, const Fw64fc *pSrc2  , Fw64fc *pDst, int len );

FwStatus STDCALL  fwsDiv_32f_I            ( const Fw32f  *pSrc ,       Fw32f  *pSrcDst,                int len );
FwStatus STDCALL  fwsDiv_64f_I            ( const Fw64f  *pSrc ,       Fw64f  *pSrcDst,                int len );
FwStatus STDCALL  fwsDiv_32fc_I           ( const Fw32fc *pSrc ,       Fw32fc *pSrcDst,                int len );
FwStatus STDCALL  fwsDiv_64fc_I           ( const Fw64fc *pSrc ,       Fw64fc *pSrcDst,                int len );

FwStatus STDCALL  fwsDiv_8u_Sfs           ( const Fw8u   *pSrc1, const Fw8u   *pSrc2  , Fw8u   *pDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_16s_Sfs          ( const Fw16s  *pSrc1, const Fw16s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_32s_Sfs          ( const Fw32s  *pSrc1, const Fw32s  *pSrc2  , Fw32s  *pDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_16sc_Sfs         ( const Fw16sc *pSrc1, const Fw16sc *pSrc2  , Fw16sc *pDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_32s16s_Sfs       ( const Fw16s  *pSrc1, const Fw32s  *pSrc2  , Fw16s  *pDst, int len, int scaleFactor );

FwStatus STDCALL  fwsDiv_8u_ISfs          ( const Fw8u   *pSrc ,       Fw8u   *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_16s_ISfs         ( const Fw16s  *pSrc ,       Fw16s  *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_16sc_ISfs        ( const Fw16sc *pSrc ,       Fw16sc *pSrcDst,                int len, int scaleFactor );
FwStatus STDCALL  fwsDiv_32s_ISfs         ( const Fw32s  *pSrc ,       Fw32s  *pSrcDst,                int len, int scaleFactor );

/*#FunctionBlock - DivC
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Divide by a constant
#Long - <Text>These functions step through vector elements in a source buffer and divide the source data by a constant value.</Text>
<Text>The quotient can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL  fwsDivC_32f             ( const Fw32f  *pSrc, Fw32f  val, Fw32f  *pDst   , int len );
FwStatus STDCALL  fwsDivC_64f             ( const Fw64f  *pSrc, Fw64f  val, Fw64f  *pDst   , int len );
FwStatus STDCALL  fwsDivC_32fc            ( const Fw32fc *pSrc, Fw32fc val, Fw32fc *pDst   , int len );
FwStatus STDCALL  fwsDivC_64fc            ( const Fw64fc *pSrc, Fw64fc val, Fw64fc *pDst   , int len );

FwStatus STDCALL  fwsDivC_32f_I           (                      Fw32f  val, Fw32f  *pSrcDst, int len );
FwStatus STDCALL  fwsDivC_64f_I           (                      Fw64f  val, Fw64f  *pSrcDst, int len );
FwStatus STDCALL  fwsDivC_32fc_I          (                      Fw32fc val, Fw32fc *pSrcDst, int len );
FwStatus STDCALL  fwsDivC_64fc_I          (                      Fw64fc val, Fw64fc *pSrcDst, int len );

FwStatus STDCALL  fwsDivC_8u_Sfs          ( const Fw8u   *pSrc, Fw8u   val, Fw8u   *pDst   , int len, int scaleFactor );
FwStatus STDCALL  fwsDivC_16s_Sfs         ( const Fw16s  *pSrc, Fw16s  val, Fw16s  *pDst   , int len, int scaleFactor );
FwStatus STDCALL  fwsDivC_16sc_Sfs        ( const Fw16sc *pSrc, Fw16sc val, Fw16sc *pDst   , int len, int scaleFactor );

FwStatus STDCALL  fwsDivC_8u_ISfs         (                      Fw8u   val, Fw8u   *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDivC_16s_ISfs        (                      Fw16s  val, Fw16s  *pSrcDst, int len, int scaleFactor );
FwStatus STDCALL  fwsDivC_16sc_ISfs       (                      Fw16sc val, Fw16sc *pSrcDst, int len, int scaleFactor );

/*#FunctionBlock - DivCRev
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Divide a constant
#Long - <Text>These functions step through vector elements in a source buffer and divide a constant value by the source data.</Text>
<Text>The quotient can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 8-bit unsigned, 16-bit signed, 16-bit signed complex, and 32-bit signed complex versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL  fwsDivCRev_16u          ( const Fw16u *pSrc, Fw16u val, Fw16u *pDst   , int len );
FwStatus STDCALL  fwsDivCRev_32f          ( const Fw32f *pSrc, Fw32f val, Fw32f *pDst   , int len );

FwStatus STDCALL  fwsDivCRev_16u_I        (                     Fw16u val, Fw16u *pSrcDst, int len );
FwStatus STDCALL  fwsDivCRev_32f_I        (                     Fw32f val, Fw32f *pSrcDst, int len );


/*#FunctionBlock - Cubrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Cube root
#Long - <Text>These functions step through vector elements in a source buffer, calculate the cube root of each element, and write the result to a destination buffer.</Text>
<Text>The 32-bit signed version of the functions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsCubrt_32f             ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCubrt_32s16s_Sfs      ( const Fw32s *pSrc, Fw16s *pDst, int len, int scaleFactor );

/*#FunctionBlock - Exp
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Raise base e to a specified power
#Long - <Text>These functions step through vector elements in a source buffer and calculate e to the power specified by each element.</Text>
<Text>Results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 16-bit and 32-bit signed versions perform an integer scaling operation before writing the results.</Text>
<Text>These functions are the inverses of the <FunctionName>Ln</FunctionName> functions.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsExp_32f               ( const Fw32f *pSrc   , Fw32f *pDst, int len );
FwStatus STDCALL fwsExp_64f               ( const Fw64f *pSrc   , Fw64f *pDst, int len );
FwStatus STDCALL fwsExp_32f64f            ( const Fw32f *pSrc   , Fw64f *pDst, int len );

FwStatus STDCALL fwsExp_32f_I             (       Fw32f *pSrcDst,               int len );
FwStatus STDCALL fwsExp_64f_I             (       Fw64f *pSrcDst,               int len );

FwStatus STDCALL fwsExp_16s_Sfs           ( const Fw16s *pSrc   , Fw16s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsExp_32s_Sfs           ( const Fw32s *pSrc   , Fw32s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsExp_64s_Sfs           ( const Fw64s *pSrc   , Fw64s *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsExp_16s_ISfs          (       Fw16s *pSrcDst,               int len, int scaleFactor );
FwStatus STDCALL fwsExp_32s_ISfs          (       Fw32s *pSrcDst,               int len, int scaleFactor );
FwStatus STDCALL fwsExp_64s_ISfs          (       Fw64s *pSrcDst,               int len, int scaleFactor );


/*#FunctionBlock - Ln
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Natural logarithm
#Long - <Text>These functions step through vector elements in a source buffer and calculate the natural logarithm of each element.</Text>
<Text>Results can be written to a destination buffer or written back to the source buffer.</Text>
<Text>The 16-bit and 32-bit signed versions perform an integer scaling operation before writing the results.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsLnZeroArg, fwStsLnNegArg

*/
FwStatus STDCALL fwsLn_32f                ( const Fw32f *pSrc   , Fw32f *pDst, int len );
FwStatus STDCALL fwsLn_64f                ( const Fw64f *pSrc   , Fw64f *pDst, int len );
FwStatus STDCALL fwsLn_64f32f             ( const Fw64f *pSrc   , Fw32f *pDst, int len );

FwStatus STDCALL fwsLn_32f_I              (       Fw32f *pSrcDst,               int len );
FwStatus STDCALL fwsLn_64f_I              (       Fw64f *pSrcDst,               int len );

FwStatus STDCALL fwsLn_16s_Sfs            ( const Fw16s *pSrc   , Fw16s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsLn_32s_Sfs            ( const Fw32s *pSrc   , Fw32s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsLn_32s16s_Sfs         ( const Fw32s *pSrc   , Fw16s *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsLn_16s_ISfs           (       Fw16s *pSrcDst,               int len, int scaleFactor );
FwStatus STDCALL fwsLn_32s_ISfs           (       Fw32s *pSrcDst,               int len, int scaleFactor );

/*#FunctionBlock - 10Log10
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Ten times log to the base 10
#Long - <Text>These functions step through vector elements in a source buffer and calculate ten times the common logarithm of each element.</Text>
<Text>The results are integer scaled, then written to a destination buffer.</Text>
<Text>This function is particularly useful for calculating logarithmic scale values, such as decibels.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsLnZeroArg, fwStsLnNegArg
*/
FwStatus STDCALL fws10Log10_32s_Sfs       ( const Fw32s *pSrc   ,Fw32s *pDst, int len, int scaleFactor );
FwStatus STDCALL fws10Log10_32s_ISfs      (       Fw32s *pSrcDst,              int len, int scaleFactor );

/*#FunctionBlock - Arctan
TODO: publish
#Technologies - REF, MT, SSE2
#Short - Arc Tangent
#Long - <Text>These functions step through vector elements in a source buffer and calculate the inverse tangent of each element.</Text>
<Text>Results can be written to a destination buffer or written back to the source buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsArctan_32f            ( const Fw32f *pSrc   , Fw32f *pDst, int len );
FwStatus STDCALL fwsArctan_64f            ( const Fw64f *pSrc   , Fw64f *pDst, int len );
FwStatus STDCALL fwsArctan_32f_I          (       Fw32f *pSrcDst,               int len );
FwStatus STDCALL fwsArctan_64f_I          (       Fw64f *pSrcDst,               int len );

/*#FunctionBlock - Threshold_LTVal
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value, replace with a specified value (Less Than)
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is less than the threshold value, the function writes a specified value.</Text>
<Text>When the source data is greater than or equal to the threshold value, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
        |value,  pSrc[n] < level|
For fwThreshold_LTVal  pDst[n] = |pSrc[n], pSrc[n] >= level |   |
        |   |

        | value,  abs(pSrc[n]) < level|
For complex variant  pDst[n] = |pSrc[n], pSrc[n] >= level |         |
        |         |
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_LTVal_16s   ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, Fw16s level, Fw16s  value );
FwStatus STDCALL fwsThreshold_LTVal_32f   ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level, Fw32f  value );
FwStatus STDCALL fwsThreshold_LTVal_64f   ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level, Fw64f  value );
FwStatus STDCALL fwsThreshold_LTVal_16sc  ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, Fw16s level, Fw16sc value );
FwStatus STDCALL fwsThreshold_LTVal_32fc  ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level, Fw32fc value );
FwStatus STDCALL fwsThreshold_LTVal_64fc  ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level, Fw64fc value );

FwStatus STDCALL fwsThreshold_LTVal_16s_I (       Fw16s  *pSrcDst,                int len, Fw16s level, Fw16s  value );
FwStatus STDCALL fwsThreshold_LTVal_32f_I (       Fw32f  *pSrcDst,                int len, Fw32f level, Fw32f  value );
FwStatus STDCALL fwsThreshold_LTVal_64f_I (       Fw64f  *pSrcDst,                int len, Fw64f level, Fw64f  value );
FwStatus STDCALL fwsThreshold_LTVal_16sc_I(       Fw16sc *pSrcDst,                int len, Fw16s level, Fw16sc value );
FwStatus STDCALL fwsThreshold_LTVal_32fc_I(       Fw32fc *pSrcDst,                int len, Fw32f level, Fw32fc value );
FwStatus STDCALL fwsThreshold_LTVal_64fc_I(       Fw64fc *pSrcDst,                int len, Fw64f level, Fw64fc value );

/*#FunctionBlock - Threshold_GTVal
TODO: publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value, replace with a specified value (Greater Than)
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is greater than the threshold value, the function writes a specified value.</Text>
<Text>When the source data is less than or equal to the threshold value, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
        |value,  pSrc[n] > level|
For fwThreshold_GTVal:  pDst[n] = |pSrc[n], pSrc[n] <= level |   |
        |   |

        | value,  abs(pSrc[n]) > level|
For complex variant:  pDst[n] = |pSrc[n], pSrc[n] <= level |         |
        |         |
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_GTVal_16s   ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, Fw16s level, Fw16s  value );
FwStatus STDCALL fwsThreshold_GTVal_32f   ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level, Fw32f  value );
FwStatus STDCALL fwsThreshold_GTVal_64f   ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level, Fw64f  value );
FwStatus STDCALL fwsThreshold_GTVal_16sc  ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, Fw16s level, Fw16sc value );
FwStatus STDCALL fwsThreshold_GTVal_32fc  ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level, Fw32fc value );
FwStatus STDCALL fwsThreshold_GTVal_64fc  ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level, Fw64fc value );

FwStatus STDCALL fwsThreshold_GTVal_16s_I (       Fw16s  *pSrcDst,                int len, Fw16s level, Fw16s  value );
FwStatus STDCALL fwsThreshold_GTVal_32f_I (       Fw32f  *pSrcDst,                int len, Fw32f level, Fw32f  value );
FwStatus STDCALL fwsThreshold_GTVal_64f_I (       Fw64f  *pSrcDst,                int len, Fw64f level, Fw64f  value );
FwStatus STDCALL fwsThreshold_GTVal_16sc_I(       Fw16sc *pSrcDst,                int len, Fw16s level, Fw16sc value );
FwStatus STDCALL fwsThreshold_GTVal_32fc_I(       Fw32fc *pSrcDst,                int len, Fw32f level, Fw32fc value );
FwStatus STDCALL fwsThreshold_GTVal_64fc_I(       Fw64fc *pSrcDst,                int len, Fw64f level, Fw64fc value );

/*#FunctionBlock - Threshold_LTValGTVal
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to upper and lower threshold values, replace with a specified value
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to two specified threshold values.</Text>
<Text>The <ParameterName>levelLT</ParameterName> parameter must be less than the <ParameterName>levelGT</ParameterName> parameter.</Text>
<Text>When the source data is less than <ParameterName>levelLT</ParameterName>, the function writes the value specified by <ParameterName>valueLT</ParameterName>.</Text>
<Text>When the source data is greater than the value specified by <ParameterName>levelGT</ParameterName>, the function writes the value specified by <ParameterName>valueGT</ParameterName>.</Text>
<Text>When the source data is within the range defined by <ParameterName>levelLT</ParameterName> and <ParameterName>levelGT</ParameterName>, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
           |valueLT, pSrc[n] < levelLT|
For fwThreshold_LTValGTVal:  pDst[n] = |pSrc[n], levelLT <= pSrc[n] <= levelGT  |      |
           |valueGT, pSrc[n] > levelGT|
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr, fwStsThresholdErr
*/
FwStatus STDCALL fwsThreshold_LTValGTVal_16s  ( const Fw16s *pSrc   , Fw16s *pDst, int len, Fw16s levelLT, Fw16s valueLT, Fw16s levelGT, Fw16s valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_32s  ( const Fw32s *pSrc   , Fw32s *pDst, int len, Fw32s levelLT, Fw32s valueLT, Fw32s levelGT, Fw32s valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_32f  ( const Fw32f *pSrc   , Fw32f *pDst, int len, Fw32f levelLT, Fw32f valueLT, Fw32f levelGT, Fw32f valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_64f  ( const Fw64f *pSrc   , Fw64f *pDst, int len, Fw64f levelLT, Fw64f valueLT, Fw64f levelGT, Fw64f valueGT );

FwStatus STDCALL fwsThreshold_LTValGTVal_16s_I(       Fw16s *pSrcDst,               int len, Fw16s levelLT, Fw16s valueLT, Fw16s levelGT, Fw16s valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_32s_I(       Fw32s *pSrcDst,               int len, Fw32s levelLT, Fw32s valueLT, Fw32s levelGT, Fw32s valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_32f_I(       Fw32f *pSrcDst,               int len, Fw32f levelLT, Fw32f valueLT, Fw32f levelGT, Fw32f valueGT );
FwStatus STDCALL fwsThreshold_LTValGTVal_64f_I(       Fw64f *pSrcDst,               int len, Fw64f levelLT, Fw64f valueLT, Fw64f levelGT, Fw64f valueGT );

/*#FunctionBlock - Threshold_LT
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value (Less Than)
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is less than the threshold value, the function writes the threshold value.</Text>
<Text>When the source data is greater than or equal to the threshold value, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
        |level,  pSrc[n] < level|
For fwThreshold_LT: pDst[n] = |pSrc[n], pSrc[n] >= level |   |
        |   |


        |(pSrc[n] * level)/ abs(pSrc[n],  abs(pSrc[n]) < level|
For complex variant:  pDst[n] = |pSrc[n], pSrc[n] >= level |            |
        |            |
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_LT_16s      ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, Fw16s level );
FwStatus STDCALL fwsThreshold_LT_32s      ( const Fw32s  *pSrc   , Fw32s  *pDst, int len, Fw32s level );
FwStatus STDCALL fwsThreshold_LT_32f      ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LT_64f      ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LT_32fc     ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LT_64fc     ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LT_16sc     ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, Fw16s level );

FwStatus STDCALL fwsThreshold_LT_16s_I    (       Fw16s  *pSrcDst,                int len, Fw16s level );
FwStatus STDCALL fwsThreshold_LT_32s_I    (       Fw32s  *pSrcDst,                int len, Fw32s level );
FwStatus STDCALL fwsThreshold_LT_32f_I    (       Fw32f  *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LT_64f_I    (       Fw64f  *pSrcDst,                int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LT_32fc_I   (       Fw32fc *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LT_64fc_I   (       Fw64fc *pSrcDst,                int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LT_16sc_I   (       Fw16sc *pSrcDst,                int len, Fw16s level );

/*#FunctionBlock - Threshold_GT
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value (Greater Than)
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is greater than the threshold value, the function writes the threshold value.</Text>
<Text>When the source data is less than or equal to the threshold value, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
      |level, pSrc[n] > level   |
For fwThreshold_LT: pDst[n] = |pSrc[n], pSrc[n] <= level|
      |       |


       |(pSrc[n] * level)/ abs(pSrc[n], abs(pSrc[n]) > level|
For complex variant:  pDst[n] = |pSrc[n], pSrc[n] <= level    |
       |       |
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_GT_16s      ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, Fw16s level );
FwStatus STDCALL fwsThreshold_GT_32s      ( const Fw32s  *pSrc   , Fw32s  *pDst, int len, Fw32s level );
FwStatus STDCALL fwsThreshold_GT_32f      ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_GT_64f      ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level );
FwStatus STDCALL fwsThreshold_GT_32fc     ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_GT_64fc     ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level );
FwStatus STDCALL fwsThreshold_GT_16sc     ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, Fw16s level );

FwStatus STDCALL fwsThreshold_GT_16s_I    (       Fw16s  *pSrcDst,                int len, Fw16s level );
FwStatus STDCALL fwsThreshold_GT_32s_I    (       Fw32s  *pSrcDst,                int len, Fw32s level );
FwStatus STDCALL fwsThreshold_GT_32f_I    (       Fw32f  *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_GT_64f_I    (       Fw64f  *pSrcDst,                int len, Fw64f level );
FwStatus STDCALL fwsThreshold_GT_32fc_I   (       Fw32fc *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_GT_64fc_I   (       Fw64fc *pSrcDst,                int len, Fw64f level );
FwStatus STDCALL fwsThreshold_GT_16sc_I   (       Fw16sc *pSrcDst,                int len, Fw16s level );

/*#FunctionBlock - Threshold
TODO: publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value, replace with threshold value (General)

#Long - <Text>These functions step through vector elements in a source buffer and compare the source data to a specified threshold value using a specified compare operation.</Text>
<Text>The compare operation can be "less than", "less than or equal", "equal", "greater than or equal" or "greater than".</Text>
<Text>When the comparison evaluates as true, the function writes the threshold value.</Text>
<Text>When the comparison evaluates as false, the function writes the value of the source data.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
      |level, pSrc[n] < level   |
For fwCmpLess:  pDst[n] = |pSrc[n], pSrc[n] >= level|
      |        |


    |(pSrc[n] * level)/ abs(pSrc[n], abs(pSrc[n]) < level|
For complex variant:  pDst[n] = |pSrc[n], pSrc[n] >= level        |
    |           |


    |level,  pSrc[n] > level  |
For fwCmpGreater:  pDst[n] = |pSrc[n], pSrc[n] <= level|
    |     |


    |(pSrc[n] * level)/ abs(pSrc[n], abs(pSrc[n]) > level|
For complex variant:  pDst[n] = |pSrc[n], pSrc[n] <= level        |
    |           |
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_16s         ( const Fw16s  *pSrc   , Fw16s  *pDst, int len, Fw16s level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_32f         ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_64f         ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_32fc        ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_64fc        ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_16sc        ( const Fw16sc *pSrc   , Fw16sc *pDst, int len, Fw16s level, FwCmpOp relOp );

FwStatus STDCALL fwsThreshold_16s_I       (       Fw16s  *pSrcDst,                int len, Fw16s level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_32f_I       (       Fw32f  *pSrcDst,                int len, Fw32f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_64f_I       (       Fw64f  *pSrcDst,                int len, Fw64f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_32fc_I      (       Fw32fc *pSrcDst,                int len, Fw32f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_64fc_I      (       Fw64fc *pSrcDst,                int len, Fw64f level, FwCmpOp relOp );
FwStatus STDCALL fwsThreshold_16sc_I      (       Fw16sc *pSrcDst,                int len, Fw16s level, FwCmpOp relOp );

/*#FunctionBlock - Threshold_LTInv
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Compare to a threshold value, replace with inverted threshold value
#Long - <Text>These functions step through vector elements in a source buffer and compare source data to a specified threshold value.</Text>
<Text>When the source data is less than the threshold value, the function writes the inverse of the threshold value.</Text>
<Text>When the source data is greater than or equal to the threshold value, the function writes the inverse of the source value.</Text>
<Text>Output data can be written to a destination buffer or back to the source buffer for in-place operation.</Text>
<Text>The following evaluation formulas are used.</Text>
<Pseudo>
         |1/level, abs(pSrc[n])= 0                     |
For fwsThreshold_LTInv:  pDst[n] =  |abs(pSrc[n])/(pSrc[n] * level), 0<abs(pSrc[n])<level |
                |1/pSrc[n], abs(pSrc[n])>level or abs(pSrc[n])<0    |

    |Infinity, abs(pSrc[n])= 0  |
if level = 0:  pDst[n] = |       |
    |1/pSrc[n], abs(pSrc[n]) > 0|
</Pseudo>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsStepErr
*/
FwStatus STDCALL fwsThreshold_LTInv_32f   ( const Fw32f  *pSrc   , Fw32f  *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LTInv_64f   ( const Fw64f  *pSrc   , Fw64f  *pDst, int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LTInv_32fc  ( const Fw32fc *pSrc   , Fw32fc *pDst, int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LTInv_64fc  ( const Fw64fc *pSrc   , Fw64fc *pDst, int len, Fw64f level );

FwStatus STDCALL fwsThreshold_LTInv_32f_I (       Fw32f  *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LTInv_64f_I (       Fw64f  *pSrcDst,                int len, Fw64f level );
FwStatus STDCALL fwsThreshold_LTInv_32fc_I(       Fw32fc *pSrcDst,                int len, Fw32f level );
FwStatus STDCALL fwsThreshold_LTInv_64fc_I(       Fw64fc *pSrcDst,                int len, Fw64f level );

/*#FunctionBlock - Magnitude
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Complex vector magnitude
#Long - <Text>These functions step through elements of a complex vector, calculate the magnitude of each element, and write the result to a destination buffer.</Text>
<Text>The following evaluation formula is used.</Text>
<Pseudo> Magnitude = sqrt( (re * re) + (im * im) )</Pseudo>
<Text>There are versions of the function that operate on one source buffer containing a complex vector and versions that operate on two source buffers containing real and imaginary component values.</Text>
<Text>There are single-vector 16-bit signed complex and single-vector 32-bit signed complex versions that perform integer scaling before writing the results.</Text>

#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMagnitude_32f         ( const Fw32f  *pSrcRe, const Fw32f *pSrcIm, Fw32f *pDst, int len );
FwStatus STDCALL fwsMagnitude_64f         ( const Fw64f  *pSrcRe, const Fw64f *pSrcIm, Fw64f *pDst, int len );
FwStatus STDCALL fwsMagnitude_32fc        ( const Fw32fc *pSrc  ,       Fw32f *pDst  ,               int len );
FwStatus STDCALL fwsMagnitude_64fc        ( const Fw64fc *pSrc  ,       Fw64f *pDst  ,               int len );
FwStatus STDCALL fwsMagnitude_16s32f      ( const Fw16s  *pSrcRe, const Fw16s *pSrcIm, Fw32f *pDst, int len );
FwStatus STDCALL fwsMagnitude_16sc32f     ( const Fw16sc *pSrc  ,       Fw32f *pDst  ,               int len );

FwStatus STDCALL fwsMagnitude_16s_Sfs     ( const Fw16s  *pSrcRe, const Fw16s *pSrcIm, Fw16s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsMagnitude_16sc_Sfs    ( const Fw16sc *pSrc  ,       Fw16s *pDst  ,               int len, int scaleFactor );
FwStatus STDCALL fwsMagnitude_32sc_Sfs    ( const Fw32sc *pSrc  ,       Fw32s *pDst  ,               int len, int scaleFactor );

/*#FunctionBlock - Convert
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Convert vector data from one type to another

#Long - <Text>These functions step through vector elements in a source buffer, convert the source data to another data type, and write the converted data to a destination buffer.</Text>
<Text>There are versions of the 16-bit signed-to-floating-point, 32-bit signed-to-floating-point, and 32-bit signed to 16-bit signed functions that perform an integer scaling operation on the results.</Text>
<Text>There are floating-point versions of the functions that also take a rounding mode argument. Values can be truncated toward zero or rounded to the nearest integer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsConvert_8s16s         ( const Fw8s  *pSrc, Fw16s *pDst, int len );
FwStatus STDCALL fwsConvert_8s32f         ( const Fw8s  *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsConvert_8u32f         ( const Fw8u  *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsConvert_16s32s        ( const Fw16s *pSrc, Fw32s *pDst, int len );
FwStatus STDCALL fwsConvert_16s32f        ( const Fw16s *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsConvert_16u32f        ( const Fw16u *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsConvert_32s16s        ( const Fw32s *pSrc, Fw16s *pDst, int len );
FwStatus STDCALL fwsConvert_32s32f        ( const Fw32s *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsConvert_32s64f        ( const Fw32s *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsConvert_32f64f        ( const Fw32f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsConvert_64f32f        ( const Fw64f *pSrc, Fw32f *pDst, int len );

FwStatus STDCALL fwsConvert_16s32f_Sfs    ( const Fw16s *pSrc, Fw32f *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsConvert_16s64f_Sfs    ( const Fw16s *pSrc, Fw64f *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsConvert_32s16s_Sfs    ( const Fw32s *pSrc, Fw16s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsConvert_32s32f_Sfs    ( const Fw32s *pSrc, Fw32f *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsConvert_32s64f_Sfs    ( const Fw32s *pSrc, Fw64f *pDst, int len, int scaleFactor );

FwStatus STDCALL fwsConvert_32f8s_Sfs     ( const Fw32f *pSrc, Fw8s  *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_32f8u_Sfs     ( const Fw32f *pSrc, Fw8u  *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_32f16s_Sfs    ( const Fw32f *pSrc, Fw16s *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_32f16u_Sfs    ( const Fw32f *pSrc, Fw16u *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_32f32s_Sfs    ( const Fw32f *pSrc, Fw32s *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_64s32s_Sfs    ( const Fw64s *pSrc, Fw32s *pDst, int len, FwRoundMode rndMode, int scaleFactor );
FwStatus STDCALL fwsConvert_64f32s_Sfs    ( const Fw64f *pSrc, Fw32s *pDst, int len, FwRoundMode rndMode, int scaleFactor );


#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - Phase
todo:publish
#Technologies - REF, MT
#Short - Complex vector phase
#Long - <Text>These functions step through vector elements in a source buffer, calculate the phase angle of each element (ATan(y/x)), and write the results to a destination buffer.</Text>
<Text>There are versions of the function that operate on one source buffer containing a complex vector and versions that operate on two source buffers containing real and imaginary component values.</Text>
<Text>There are single-vector 16-bit signed complex, single-vector 32-bit signed complex, and dual-vector 16-bit signed versions that perform integer scaling before writing the results.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsPhase_32f             ( const Fw32f  *pSrcRe, const Fw32f *pSrcIm, Fw32f *pDst, int len );
FwStatus STDCALL fwsPhase_64f             ( const Fw64f  *pSrcRe, const Fw64f *pSrcIm, Fw64f *pDst, int len );
FwStatus STDCALL fwsPhase_32fc            ( const Fw32fc *pSrc  ,       Fw32f *pDst  ,               int len );
FwStatus STDCALL fwsPhase_64fc            ( const Fw64fc *pSrc  ,       Fw64f *pDst  ,               int len );
FwStatus STDCALL fwsPhase_16s32f          ( const Fw16s  *pSrcRe, const Fw16s *pSrcIm, Fw32f *pDst, int len );
FwStatus STDCALL fwsPhase_16sc32f         ( const Fw16sc *pSrc  ,       Fw32f *pDst  ,               int len );

FwStatus STDCALL fwsPhase_16s_Sfs         ( const Fw16s  *pSrcRe, const Fw16s *pSrcIm, Fw16s *pDst, int len, int scaleFactor );
FwStatus STDCALL fwsPhase_16sc_Sfs        ( const Fw16sc *pSrc  ,       Fw16s *pDst  ,               int len, int scaleFactor );
FwStatus STDCALL fwsPhase_32sc_Sfs        ( const Fw32sc *pSrc  ,       Fw32s *pDst  ,               int len, int scaleFactor );

#endif

/*#FunctionBlock - Max
TODO:publish
#Technologies - REF, SSE2
#Short - Maximum
#Long - <Text>These functions step through vector elements in a source buffer, find the maximum value, and write the value to a location specified by a pointer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMax_16s               ( const Fw16s *pSrc, int len, Fw16s *pMax );
FwStatus STDCALL fwsMax_32s               ( const Fw32s *pSrc, int len, Fw32s *pMax );
FwStatus STDCALL fwsMax_32f               ( const Fw32f *pSrc, int len, Fw32f *pMax );
FwStatus STDCALL fwsMax_64f               ( const Fw64f *pSrc, int len, Fw64f *pMax );

/*#FunctionBlock - MaxIndx
TODO:publish
#Technologies - REF, SSE2
#Short - Maximum with index
#Long - <Text>These functions step through vector elements in a source buffer, find the maximum value and its index.</Text>
<Text>The maximum value is written to a maximum value buffer and the index is written to a location specified by a pointer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMaxIndx_16s           ( const Fw16s *pSrc, int len, Fw16s *pMax, int *pIndx );
FwStatus STDCALL fwsMaxIndx_32s           ( const Fw32s *pSrc, int len, Fw32s *pMax, int *pIndx );
FwStatus STDCALL fwsMaxIndx_32f           ( const Fw32f *pSrc, int len, Fw32f *pMax, int *pIndx );
FwStatus STDCALL fwsMaxIndx_64f           ( const Fw64f *pSrc, int len, Fw64f *pMax, int *pIndx );

/*#FunctionBlock - MaxAbs
TODO:publish
#Technologies - REF, SSE2
#Short - Maximum absolute value
#Long - <Text>These functions step through vector elements in a source buffer, find the maximum absolute value, and write the value to a location specified by a pointer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMaxAbs_16s            ( const Fw16s *pSrc, int len, Fw16s *pMaxAbs );
FwStatus STDCALL fwsMaxAbs_32s            ( const Fw32s *pSrc, int len, Fw32s *pMaxAbs );

/*#FunctionBlock - MaxAbsIndx
TODO:publish
#Technologies - REF, SSE2
#Short - Maximum absolute value with index
#Long - <Text>These functions step through vector elements in a source buffer, find the maximum absolute value and its index.</Text>
<Text>The maximum absolute value and the index are written to locations specified by pointers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMaxAbsIndx_16s        ( const Fw16s *pSrc, int len, Fw16s *pMaxAbs, int *pIndx );
FwStatus STDCALL fwsMaxAbsIndx_32s        ( const Fw32s *pSrc, int len, Fw32s *pMaxAbs, int *pIndx );


/*#FunctionBlock - Min
TODO:publish
#Technologies - REF, SSE2
#Short - Minimum
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum value, and write the value to a location specified by a pointer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMin_16s               ( const Fw16s *pSrc, int len, Fw16s *pMin );
FwStatus STDCALL fwsMin_32s               ( const Fw32s *pSrc, int len, Fw32s *pMin );
FwStatus STDCALL fwsMin_32f               ( const Fw32f *pSrc, int len, Fw32f *pMin );
FwStatus STDCALL fwsMin_64f               ( const Fw64f *pSrc, int len, Fw64f *pMin );

/*#FunctionBlock - MinIndx
TODO:publish
#Technologies - REF, SSE2
#Short - Minimum with index
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum value and its index.</Text>
<Text>The minimum value and the index are written to locations specified by pointers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinIndx_16s           ( const Fw16s *pSrc, int len, Fw16s *pMin, int *pIndx );
FwStatus STDCALL fwsMinIndx_32s           ( const Fw32s *pSrc, int len, Fw32s *pMin, int *pIndx );
FwStatus STDCALL fwsMinIndx_32f           ( const Fw32f *pSrc, int len, Fw32f *pMin, int *pIndx );
FwStatus STDCALL fwsMinIndx_64f           ( const Fw64f *pSrc, int len, Fw64f *pMin, int *pIndx );


/*#FunctionBlock - MinAbs
TODO:publish
#Technologies - REF, SSE2
#Short - Minimum absolute value
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum absolute value, and write the value to a location specified by a pointer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinAbs_16s            ( const Fw16s *pSrc, int len, Fw16s *pMinAbs );
FwStatus STDCALL fwsMinAbs_32s            ( const Fw32s *pSrc, int len, Fw32s *pMinAbs );


/*#FunctionBlock - MinAbsIndx
TODO:publish
#Technologies - REF
#Short - Minimum absolute value with index
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum absolute value and its index.</Text>
<Text>The minimum absolute value and the index are written to locations specified by pointers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinAbsIndx_16s        ( const Fw16s *pSrc, int len, Fw16s *pMinAbs, int *pIndx );
FwStatus STDCALL fwsMinAbsIndx_32s        ( const Fw32s *pSrc, int len, Fw32s *pMinAbs, int *pIndx );


/*#FunctionBlock - MinMax
TODO:publish
#Technologies - REF, SSE2
#Short - Minimum and maximum
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum and maximum values, and write the values to locations specified by pointers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinMax_8u             ( const Fw8u  *pSrc, int len, Fw8u  *pMin, Fw8u  *pMax );
FwStatus STDCALL fwsMinMax_16u            ( const Fw16u *pSrc, int len, Fw16u *pMin, Fw16u *pMax );
FwStatus STDCALL fwsMinMax_16s            ( const Fw16s *pSrc, int len, Fw16s *pMin, Fw16s *pMax );
FwStatus STDCALL fwsMinMax_32u            ( const Fw32u *pSrc, int len, Fw32u *pMin, Fw32u *pMax );
FwStatus STDCALL fwsMinMax_32s            ( const Fw32s *pSrc, int len, Fw32s *pMin, Fw32s *pMax );
FwStatus STDCALL fwsMinMax_32f            ( const Fw32f *pSrc, int len, Fw32f *pMin, Fw32f *pMax );
FwStatus STDCALL fwsMinMax_64f            ( const Fw64f *pSrc, int len, Fw64f *pMin, Fw64f *pMax );


/*#FunctionBlock - MinMaxIndx
TODO:publish
#Technologies - REF
#Short - Minimum and maximum with index
#Long - <Text>These functions step through vector elements in a source buffer, find the minimum and maximum values and the corresponding indices.</Text>
<Text>The minimum value, the maximum value, and the indices are written locations specified by pointers.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinMaxIndx_8u         ( const Fw8u  *pSrc, int len, Fw8u  *pMin, int *pMinIndx, Fw8u  *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_16u        ( const Fw16u *pSrc, int len, Fw16u *pMin, int *pMinIndx, Fw16u *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_16s        ( const Fw16s *pSrc, int len, Fw16s *pMin, int *pMinIndx, Fw16s *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_32u        ( const Fw32u *pSrc, int len, Fw32u *pMin, int *pMinIndx, Fw32u *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_32s        ( const Fw32s *pSrc, int len, Fw32s *pMin, int *pMinIndx, Fw32s *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_32f        ( const Fw32f *pSrc, int len, Fw32f *pMin, int *pMinIndx, Fw32f *pMax, int *pMaxIndx );
FwStatus STDCALL fwsMinMaxIndx_64f        ( const Fw64f *pSrc, int len, Fw64f *pMin, int *pMinIndx, Fw64f *pMax, int *pMaxIndx );

/*#FunctionBlock - Norm_Inf
TODO:publish
#Technologies - REF, SSE2
#Short - Norm C
#Long - <Text>These functions step through vector elements in a source buffer, calculate the C Norm, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> C Norm =((n = 0) - (len-1)) max (|pSrc[n]|)</Pseudo>

<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation before the result is written.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNorm_Inf_32f          ( const Fw32f  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_Inf_64f          ( const Fw64f  *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_Inf_16s32f       ( const Fw16s  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_Inf_32fc32f      ( const Fw32fc *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_Inf_64fc64f      ( const Fw64fc *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_Inf_16s32s_Sfs   ( const Fw16s  *pSrc, int len, Fw32s *pNorm,int scaleFactor );


/*#FunctionBlock - Norm_L1
TODO:publish
#Technologies - REF, SSE2
#Short - Norm L1
#Long - <Text>These functions step through vector elements in a source buffer, calculate the L1 Norm, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> L1 Norm =((n = 0) - (len-1)) Sum (|pSrc[n]|)</Pseudo>
<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNorm_L1_32f           ( const Fw32f  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_L1_64f           ( const Fw64f  *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L1_16s32f        ( const Fw16s  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_L1_32fc64f       ( const Fw32fc *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L1_64fc64f       ( const Fw64fc *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L1_16s32s_Sfs    ( const Fw16s  *pSrc, int len, Fw32s *pNorm,int scaleFactor );

/*#FunctionBlock - Norm_L2
TODO:publish
#Technologies - REF, SSE2
#Short - Norm L2
#Long - <Text>These functions step through vector elements in a source buffer, calculate the L2 Norm, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> L2 Norm =Sqrt(((n = 0) - (len-1)) Sum(Square(|pSrc[n]|))</Pseudo>
<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNorm_L2_32f           ( const Fw32f  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_L2_64f           ( const Fw64f  *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L2_16s32f        ( const Fw16s  *pSrc, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNorm_L2_32fc64f       ( const Fw32fc *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L2_64fc64f       ( const Fw64fc *pSrc, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNorm_L2_16s32s_Sfs    ( const Fw16s  *pSrc, int len, Fw32s *pNorm, int scaleFactor );

/*#FunctionBlock - NormDiff_Inf
TODO:publish
#Technologies - REF, SSE2
#Short - Norm of difference C
#Long - <Text>These functions step through vector elements in two source buffers, calculate the C Norm of the difference between the elements in buffer 1 and the elements in buffer 2, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> C NormDiff = ((n = 0) - (len-1)) max (|pSrc1[n] - pSrc2[n]|)</Pseudo>
<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNormDiff_Inf_32f          ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_Inf_64f          ( const Fw64f  *pSrc1, const Fw64f  *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_Inf_16s32f       ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_Inf_32fc32f      ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_Inf_64fc64f      ( const Fw64fc *pSrc1, const Fw64fc *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_Inf_16s32s_Sfs   ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32s *pNorm, int scaleFactor );

/*#FunctionBlock - NormDiff_L1
TODO:publish
#Technologies - REF, SSE2
#Short - Norm of difference L1
#Long - <Text>These functions step through vector elements in two source buffers, calculate the L1 Norm of the difference between the elements in buffer 1 and the elements in buffer 2, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> NormDiff = ((n = 0) - (len-1)) Sum (|pSrc1[n] - pSrc2[n]|)</Pseudo>
<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNormDiff_L1_32f       ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_L1_64f       ( const Fw64f  *pSrc1, const Fw64f  *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L1_16s32f    ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_L1_32fc64f   ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L1_64fc64f   ( const Fw64fc *pSrc1, const Fw64fc *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L1_16s32s_Sfs( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32s *pNorm, int scaleFactor );

/*#FunctionBlock - NormDiff_L2
TODO:publish
#Technologies - REF, SSE2
#Short - Norm of difference L2
#Long - <Text>These functions step through vector elements in two source buffers, calculate the L2 Norm of the difference between the elements in buffer 1 and the elements in buffer 2, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> NormDiff = Sqrt(((n = 0) - (len-1)) Sum(Square(|pSrc1[n] - pSrc2[n]|))</Pseudo>
<Text>The 16-bit signed to 32-bit signed version of the function performs an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsNormDiff_L2_32f       ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_L2_64f       ( const Fw64f  *pSrc1, const Fw64f  *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L2_16s32f    ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32f *pNorm );
FwStatus STDCALL fwsNormDiff_L2_32fc64f   ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L2_64fc64f   ( const Fw64fc *pSrc1, const Fw64fc *pSrc2, int len, Fw64f *pNorm );
FwStatus STDCALL fwsNormDiff_L2_16s32s_Sfs( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32s *pNorm, int scaleFactor );


/*#FunctionBlock - Mean
TODO:publish
#Technologies - REF, SSE2
#Short - Mean
#Long - <Text>These functions step through vector elements in a source buffer, calculate the arithmetic mean, and write the value to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> Mean = 1/len ((n = 0) - (len-1)) Sum(pSrc[n])</Pseudo>
<Text>The 16-bit signed and 16-bit signed complex versions perform an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMean_32f              ( const Fw32f  *pSrc, int len, Fw32f  *pMean,FwHintAlgorithm hint );
FwStatus STDCALL fwsMean_32fc             ( const Fw32fc *pSrc, int len, Fw32fc *pMean,FwHintAlgorithm hint );

FwStatus STDCALL fwsMean_64f              ( const Fw64f  *pSrc, int len, Fw64f  *pMean );
FwStatus STDCALL fwsMean_64fc             ( const Fw64fc *pSrc, int len, Fw64fc *pMean );

FwStatus STDCALL fwsMean_16s_Sfs          ( const Fw16s  *pSrc, int len, Fw16s  *pMean, int scaleFactor );
FwStatus STDCALL fwsMean_16sc_Sfs         ( const Fw16sc *pSrc, int len, Fw16sc *pMean, int scaleFactor );

/*#FunctionBlock - MaxEvery
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Maximum (element pair)
#Long - <Text>These functions step through vector elements in two buffers and compare corresponding elements in the source and destination buffers.</Text>
<Text>The maximum value in the comparison is written to the destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMaxEvery_16s_I        ( const Fw16s *pSrc, Fw16s *pSrcDst, int len );
FwStatus STDCALL fwsMaxEvery_32s_I        ( const Fw32s *pSrc, Fw32s *pSrcDst, int len );
FwStatus STDCALL fwsMaxEvery_32f_I        ( const Fw32f *pSrc, Fw32f *pSrcDst, int len );

/*#FunctionBlock - MinEvery
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Minimum (element pair)
#Long - <Text>These functions step through vector elements in two buffers and compare corresponding elements in the source and destination buffers.</Text>
<Text>The minimum value in the comparison is written to the destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMinEvery_16s_I        ( const Fw16s *pSrc, Fw16s *pSrcDst, int len );
FwStatus STDCALL fwsMinEvery_32s_I        ( const Fw32s *pSrc, Fw32s *pSrcDst, int len );
FwStatus STDCALL fwsMinEvery_32f_I        ( const Fw32f *pSrc, Fw32f *pSrcDst, int len );


/*#FunctionBlock - DotProd
TODO:publish
#Technologies - REF, SSE2
#Short - Dot Product
#Long - <Text>These functions step through vector elements in two buffers, calculate the dot product of the vectors, and write the result to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> DotProduct = ((n = 0) - (len-1)) Sum(pSrc1[n]  *pSrc2[n])</Pseudo>
<Text>There are 16-bit and 32-bit signed and signed complex versions that perform an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsDotProd_16s32f            ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32f  *pDp );
FwStatus STDCALL fwsDotProd_16s64s            ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw64s  *pDp );
FwStatus STDCALL fwsDotProd_16s16sc64sc       ( const Fw16s  *pSrc1, const Fw16sc *pSrc2, int len, Fw64sc *pDp );
FwStatus STDCALL fwsDotProd_16s16sc32fc       ( const Fw16s  *pSrc1, const Fw16sc *pSrc2, int len, Fw32fc *pDp );
FwStatus STDCALL fwsDotProd_32f               ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, int len, Fw32f  *pDp );
FwStatus STDCALL fwsDotProd_32f64f            ( const Fw32f  *pSrc1, const Fw32f  *pSrc2, int len, Fw64f  *pDp );
FwStatus STDCALL fwsDotProd_32f32fc           ( const Fw32f  *pSrc1, const Fw32fc *pSrc2, int len, Fw32fc *pDp );
FwStatus STDCALL fwsDotProd_32f32fc64fc       ( const Fw32f  *pSrc1, const Fw32fc *pSrc2, int len, Fw64fc *pDp );
FwStatus STDCALL fwsDotProd_64f               ( const Fw64f  *pSrc1, const Fw64f  *pSrc2, int len, Fw64f  *pDp );
FwStatus STDCALL fwsDotProd_64f64fc           ( const Fw64f  *pSrc1, const Fw64fc *pSrc2, int len, Fw64fc *pDp );
FwStatus STDCALL fwsDotProd_16sc64sc          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2, int len, Fw64sc *pDp );
FwStatus STDCALL fwsDotProd_16sc32fc          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2, int len, Fw32fc *pDp );
FwStatus STDCALL fwsDotProd_32fc              ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, int len, Fw32fc *pDp );
FwStatus STDCALL fwsDotProd_32fc64fc          ( const Fw32fc *pSrc1, const Fw32fc *pSrc2, int len, Fw64fc *pDp );
FwStatus STDCALL fwsDotProd_64fc              ( const Fw64fc *pSrc1, const Fw64fc *pSrc2, int len, Fw64fc *pDp );

FwStatus STDCALL fwsDotProd_16s_Sfs           ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw16s  *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16s32s_Sfs        ( const Fw16s  *pSrc1, const Fw16s  *pSrc2, int len, Fw32s  *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16s16sc32sc_Sfs   ( const Fw16s  *pSrc1, const Fw16sc *pSrc2, int len, Fw32sc *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16s32s32s_Sfs     ( const Fw16s  *pSrc1, const Fw32s  *pSrc2, int len, Fw32s  *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16s16sc_Sfs       ( const Fw16s  *pSrc1, const Fw16sc *pSrc2, int len, Fw16sc *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_32s_Sfs           ( const Fw32s  *pSrc1, const Fw32s  *pSrc2, int len, Fw32s  *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_32s32sc_Sfs       ( const Fw32s  *pSrc1, const Fw32sc *pSrc2, int len, Fw32sc *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16sc_Sfs          ( const Fw16sc *pSrc1, const Fw16sc *pSrc2, int len, Fw16sc *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_16sc32sc_Sfs      ( const Fw16sc *pSrc1, const Fw16sc *pSrc2, int len, Fw32sc *pDp, int scaleFactor );
FwStatus STDCALL fwsDotProd_32sc_Sfs          ( const Fw32sc *pSrc1, const Fw32sc *pSrc2, int len, Fw32sc *pDp, int scaleFactor );



/*#FunctionBlock - Sum
TODO:publish
#Technologies - REF, SSE2
#Short - Sum
#Long - <Text>These functions step through vector elements in a source buffer, calculate the sum of the elements, and write the result to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> Sum = ((n = 0) - (len-1)) Sum(pSrc[n])</Pseudo>
<Text>The 16-bit signed and signed complex and 32-bit signed versions perform an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSum_32f               ( const Fw32f  *pSrc, int len, Fw32f  *pSum, FwHintAlgorithm hint );
FwStatus STDCALL fwsSum_32fc              ( const Fw32fc *pSrc, int len, Fw32fc *pSum, FwHintAlgorithm hint );

FwStatus STDCALL fwsSum_64f               ( const Fw64f  *pSrc, int len, Fw64f  *pSum );
FwStatus STDCALL fwsSum_64fc              ( const Fw64fc *pSrc, int len, Fw64fc *pSum );

FwStatus STDCALL fwsSum_16s_Sfs           ( const Fw16s  *pSrc, int len, Fw16s  *pSum, int scaleFactor );
FwStatus STDCALL fwsSum_32s_Sfs           ( const Fw32s  *pSrc, int len, Fw32s  *pSum, int scaleFactor );
FwStatus STDCALL fwsSum_16s32s_Sfs        ( const Fw16s  *pSrc, int len, Fw32s  *pSum, int scaleFactor );
FwStatus STDCALL fwsSum_16sc_Sfs          ( const Fw16sc *pSrc, int len, Fw16sc *pSum, int scaleFactor );
FwStatus STDCALL fwsSum_16sc32sc_Sfs      ( const Fw16sc *pSrc, int len, Fw32sc *pSum, int scaleFactor );


/*#FunctionBlock - StdDev
TODO:publish
#Technologies - REF, SSE2
#Short - Standard deviation
#Long - <Text>These functions step through vector elements in a source buffer, calculate the standard deviation of the elements, and write the result to a location specified by a pointer.</Text>
<Text>The following formula is used.</Text>
<Pseudo> StdDev = Sqrt(((n = 0) - (len-1)) Sum(Square(pSrc[n]-pMean))/len-1)</Pseudo>
<Text>The 16-bit signed versions perform an integer scaling operation on the result.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsStepErr, fwStsSizeErr
*/
FwStatus STDCALL fwsStdDev_32f            ( const Fw32f *pSrc, int len, Fw32f *pStdDev, FwHintAlgorithm hint );

FwStatus STDCALL fwsStdDev_64f            ( const Fw64f *pSrc, int len, Fw64f *pStdDev );

FwStatus STDCALL fwsStdDev_16s32s_Sfs     ( const Fw16s *pSrc, int len, Fw32s *pStdDev, int scaleFactor );
FwStatus STDCALL fwsStdDev_16s_Sfs        ( const Fw16s *pSrc, int len, Fw16s *pStdDev, int scaleFactor );


/*#Documentation
</Chapter>
*/

//Fixed accuracy arithmetic

/*#Documentation
<Chapter>
<ChapterHeading>Fixed Accuracy Arithmetic Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that perform arithmetic operations at a chosen level of accuracy.</ChaptDesc></Paragraph>

<Paragraph>These functions provide flexible specification of accuracy and support IEEE-754 standards. Choice of accuracy level is based on practical experience and application requirements.</Paragraph>
<Paragraph>Options are specified by a function name suffix:</Paragraph>
<ParagraphIndent>The options for the single-precision data format are A11, A21, and A24.</ParagraphIndent>
<ParagraphIndent>The options for the double-precision data format are A50 and A53.</ParagraphIndent>
<Paragraph>For single precision data:</Paragraph>
<ParagraphIndent>Suffix_A11 guarantees 11 correctly rounded bits of significand, or at least three exact decimal digits.</ParagraphIndent>
<ParagraphIndent>Suffix_A21 guarantees 21 correctly rounded bits of significand, or four ulps, or approximately six exact decimal digits.</ParagraphIndent>
<ParagraphIndent>Suffix_A24 guarantees 24 correctly rounded bits of significand, including the implied bit, with the maximum guaranteed error within one ulp.</ParagraphIndent>
<Paragraph>For double precision data:</Paragraph>
<ParagraphIndent>Suffix_A50 guarantees 50 correctly rounded bits of significand, or four ulps, or approximately 15 exact decimal digits.</ParagraphIndent>
<ParagraphIndent>Suffix_A53 guarantees 53 correctly rounded bits of significand, including the implied bit, with the maximum guaranteed error within one ulp.</ParagraphIndent>
*/

/*#FunctionBlock - Inv
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Inverse
#Long - <Text>These functions step through a source buffer, calculate the inverse of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsSingularity
*/

FwStatus STDCALL fwsInv_32f_A11           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInv_32f_A21           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInv_32f_A24           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInv_64f_A50           ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsInv_64f_A53           ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Div
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Divide
#Long - <Text>These functions step through vector elements in two source buffers, divide the elements in buffer 1 by the elements in buffer 2, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsSingularity
*/

FwStatus STDCALL fwsDiv_32f_A11           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsDiv_32f_A21           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsDiv_32f_A24           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsDiv_64f_A50           ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );
FwStatus STDCALL fwsDiv_64f_A53           ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );

/*#FunctionBlock - Sqrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Square root
#Long - <Text>These functions step through vector elements in a source buffer, calculate the square root of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/

FwStatus STDCALL fwsSqrt_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSqrt_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSqrt_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSqrt_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsSqrt_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - InvSqrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Inverse Square root
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse square root of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/

FwStatus STDCALL fwsInvSqrt_32f_A11       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvSqrt_32f_A21       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvSqrt_32f_A24       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvSqrt_64f_A50       ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsInvSqrt_64f_A53       ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Cbrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Cube root
#Long - <Text>These functions step through vector elements in a source buffer, calculate the cube root of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/

FwStatus STDCALL fwsCbrt_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCbrt_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCbrt_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCbrt_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsCbrt_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - InvCbrt
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Inverse cube root
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse cube root of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsSingularity
*/

FwStatus STDCALL fwsInvCbrt_32f_A11       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvCbrt_32f_A21       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvCbrt_32f_A24       ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsInvCbrt_64f_A50       ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsInvCbrt_64f_A53       ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Pow
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Power
#Long - <Text>These functions step through vector elements in two source buffers, raise each element of buffer 1 to the power specified by the corresponding element of buffer 2, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/

FwStatus STDCALL fwsPow_32f_A11           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsPow_32f_A21           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsPow_32f_A24           ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsPow_64f_A50           ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );
FwStatus STDCALL fwsPow_64f_A53           ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );

/*#FunctionBlock - Powx
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Constant power
#Long - <Text>These functions step through vector elements in a source buffer, raise each element to a power specified by a constant, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/

FwStatus STDCALL fwsPowx_32f_A11          ( const Fw32f *pSrc1, const Fw32f val, Fw32f *pDst, int len );
FwStatus STDCALL fwsPowx_32f_A21          ( const Fw32f *pSrc1, const Fw32f val, Fw32f *pDst, int len );
FwStatus STDCALL fwsPowx_32f_A24          ( const Fw32f *pSrc1, const Fw32f val, Fw32f *pDst, int len );
FwStatus STDCALL fwsPowx_64f_A50          ( const Fw64f *pSrc1, const Fw64f val, Fw64f *pDst, int len );
FwStatus STDCALL fwsPowx_64f_A53          ( const Fw64f *pSrc1, const Fw64f val, Fw64f *pDst, int len );

/*#FunctionBlock - Exp
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Exponential
#Long - <Text>These functions step through vector elements in a source buffer, raise e to the power specified by each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsOverflow, fwStsUnderflow
*/

FwStatus STDCALL fwsExp_32f_A11           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsExp_32f_A21           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsExp_32f_A24           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsExp_64f_A50           ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsExp_64f_A53           ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Ln
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Natural Logarithm
#Long - <Text>These functions step through vector elements in a source buffer, calculate the natural logarithm of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/

FwStatus STDCALL fwsLn_32f_A11            ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLn_32f_A21            ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLn_32f_A24            ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLn_64f_A50            ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsLn_64f_A53            ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Log10
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Common Logarithm
#Long - <Text>These functions step through vector elements in a source buffer, calculate the common logarithm of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/

FwStatus STDCALL fwsLog10_32f_A11         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLog10_32f_A21         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLog10_32f_A24         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsLog10_64f_A50         ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsLog10_64f_A53         ( const Fw64f *pSrc, Fw64f *pDst, int len );

//Trigonometric functions

/*#FunctionBlock - Cos
TODO:publish
#Technologies - REF, MT
#Short - Cosine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the cosine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsCos_32f_A11           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCos_32f_A21           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCos_32f_A24           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCos_64f_A50           ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsCos_64f_A53           ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Sin
TODO: publish
#Technologies - REF, MT
#Short - Sine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the sine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsSin_32f_A11           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSin_32f_A21           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSin_32f_A24           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSin_64f_A50           ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsSin_64f_A53           ( const Fw64f *pSrc, Fw64f *pDst, int len );

#if BUILD_NUM_AT_LEAST( 9999 )

/*#FunctionBlock - SinCos
TODO: publish
#Technologies - REF, MT
#Short - Sine and Cosine
#Long - <Text>These functions step through vector elements in a source buffer and calculate the sine and the cosine of each element.</Text>
<Text>Sine results are written to destination buffer 1 and cosine results are written to destination buffer 2.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/

FwStatus STDCALL fwsSinCos_32f_A11        ( const Fw32f *pSrc, Fw32f *pDst1, Fw32f *pDst2, int len );
FwStatus STDCALL fwsSinCos_32f_A21        ( const Fw32f *pSrc, Fw32f *pDst1, Fw32f *pDst2, int len );
FwStatus STDCALL fwsSinCos_32f_A24        ( const Fw32f *pSrc, Fw32f *pDst1, Fw32f *pDst2, int len );
FwStatus STDCALL fwsSinCos_64f_A50        ( const Fw64f *pSrc, Fw64f *pDst1, Fw64f *pDst2, int len );
FwStatus STDCALL fwsSinCos_64f_A53        ( const Fw64f *pSrc, Fw64f *pDst1, Fw64f *pDst2, int len );

#endif
/*#FunctionBlock - Tan
TODO:publish
#Technologies - REF, MT
#Short - Tangent
#Long - <Text>These functions step through vector elements in a source buffer, calculate the tangent of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsTan_32f_A11           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTan_32f_A21           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTan_32f_A24           ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTan_64f_A50           ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsTan_64f_A53           ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Acos
TODO:publish
#Technologies - REF, MT
#Short - Inverse cosine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse cosine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsAcos_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcos_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcos_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcos_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAcos_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Asin
TODO:publish
#Technologies - REF, MT
#Short - Inverse sine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse sine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsAsin_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsin_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsin_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsin_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAsin_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Atan
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Inverse tangent
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse tangent of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAtan_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAtan_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Atan2
TODO:publish
#Technologies - REF, MT, SSE2
#Short - Inverse tangent (four quadrant)
#Long - <Text>These functions step through vector elements in two input buffers, calculate the inverse tangent of the y and x buffer elements, and write the results to a destination buffer.</Text>
<Text>Buffer 1 contains y values and buffer 2 contains x values.</Text>
<Text>The operation of this function is similar to calculating the arctangent of y/x, except that the signs of both arguments determine the quadrant of the result.</Text>
<Text>Results are in the range (-pi, pi).</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAtan2_32f_A11         ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan2_32f_A21         ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan2_32f_A24         ( const Fw32f *pSrc1, const Fw32f *pSrc2, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtan2_64f_A50         ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );
FwStatus STDCALL fwsAtan2_64f_A53         ( const Fw64f *pSrc1, const Fw64f *pSrc2, Fw64f *pDst, int len );

//Hyperbolic Functions

/*#FunctionBlock - Cosh
TODO:publish
#Technologies - REF, MT
#Short - Hyperbolic cosine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the hyperbolic cosine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsOverflow
*/
FwStatus STDCALL fwsCosh_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCosh_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCosh_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsCosh_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsCosh_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Sinh
TODO:publish
#Technologies - REF, MT
#Short - Hyperbolic sine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the hyperbolic sine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsOverflow
*/
FwStatus STDCALL fwsSinh_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSinh_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSinh_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsSinh_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsSinh_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Tanh
TODO:publish
#Technologies - REF, MT
#Short - Hyperbolic tangent
#Long - <Text>These functions step through vector elements in a source buffer, calculate the hyperbolic tangent of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsTanh_32f_A11          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTanh_32f_A21          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTanh_32f_A24          ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsTanh_64f_A50          ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsTanh_64f_A53          ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Acosh
TODO:publish
#Technologies - REF, MT
#Short - Inverse hyperbolic cosine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse hyperbolic cosine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain
*/
FwStatus STDCALL fwsAcosh_32f_A11         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcosh_32f_A21         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcosh_32f_A24         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAcosh_64f_A50         ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAcosh_64f_A53         ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Asinh
TODO:publish
#Technologies - REF, MT
#Short - Inverse hyperbolic sine
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse hyperbolic sine of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsAsinh_32f_A11         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsinh_32f_A21         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsinh_32f_A24         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAsinh_64f_A50         ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAsinh_64f_A53         ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#FunctionBlock - Atanh
TODO:publish
#Technologies - REF, MT
#Short - Inverse hyperbolic tangent
#Long - <Text>These functions step through vector elements in a source buffer, calculate the inverse hyperbolic tangent of each element, and write the results to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/
FwStatus STDCALL fwsAtanh_32f_A11         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtanh_32f_A21         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtanh_32f_A24         ( const Fw32f *pSrc, Fw32f *pDst, int len );
FwStatus STDCALL fwsAtanh_64f_A50         ( const Fw64f *pSrc, Fw64f *pDst, int len );
FwStatus STDCALL fwsAtanh_64f_A53         ( const Fw64f *pSrc, Fw64f *pDst, int len );

/*#Documentation
</Chapter>
*/

/*#Documentation
<Chapter>
<ChapterHeading>Vector Initialization Functions</ChapterHeading>
<Paragraph>This<ChaptDesc> chapter describes functions that perform vector initialization tasks.</ChaptDesc></Paragraph>
*/

/*#FunctionBlock - Copy
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Copy vector
#Long - <Text>These functions copy the vector elements in a source buffer to a destination buffer.</Text>
#ReturnValues - fwStsNoErr , fwStsNullPtrErr, fwStsSizeErr, fwStsDomain, fwStsSingularity
*/
FwStatus STDCALL fwsCopy_8u               ( const Fw8u   *pSrc, Fw8u   *pDst, int len );
FwStatus STDCALL fwsCopy_16s              ( const Fw16s  *pSrc, Fw16s  *pDst, int len );
FwStatus STDCALL fwsCopy_32f              ( const Fw32f  *pSrc, Fw32f  *pDst, int len );
FwStatus STDCALL fwsCopy_64f              ( const Fw64f  *pSrc, Fw64f  *pDst, int len );
FwStatus STDCALL fwsCopy_16sc             ( const Fw16sc *pSrc, Fw16sc *pDst, int len );
FwStatus STDCALL fwsCopy_32fc             ( const Fw32fc *pSrc, Fw32fc *pDst, int len );
FwStatus STDCALL fwsCopy_64fc             ( const Fw64fc *pSrc, Fw64fc *pDst, int len );

/*#FunctionBlock - Move
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Move vector
#Long - <Text>These functions move the vector elements in a source buffer to a destination buffer.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsMove_8u               ( const Fw8u   *pSrc, Fw8u   *pDst, int len );
FwStatus STDCALL fwsMove_16s              ( const Fw16s  *pSrc, Fw16s  *pDst, int len );
FwStatus STDCALL fwsMove_32f              ( const Fw32f  *pSrc, Fw32f  *pDst, int len );
FwStatus STDCALL fwsMove_64f              ( const Fw64f  *pSrc, Fw64f  *pDst, int len );
FwStatus STDCALL fwsMove_16sc             ( const Fw16sc *pSrc, Fw16sc *pDst, int len );
FwStatus STDCALL fwsMove_32fc             ( const Fw32fc *pSrc, Fw32fc *pDst, int len );
FwStatus STDCALL fwsMove_64fc             ( const Fw64fc *pSrc, Fw64fc *pDst, int len );

/*#FunctionBlock - Set
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Set vector
#Long - <Text>These functions set elements of a vector in a destination buffer to a specified value.</Text>
<Text>A length parameter specifies the number of elements that are set.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsSet_8u                ( Fw8u   val, Fw8u   *pDst, int len );
FwStatus STDCALL fwsSet_16s               ( Fw16s  val, Fw16s  *pDst, int len );
FwStatus STDCALL fwsSet_32s               ( Fw32s  val, Fw32s  *pDst, int len );
FwStatus STDCALL fwsSet_32f               ( Fw32f  val, Fw32f  *pDst, int len );
FwStatus STDCALL fwsSet_64s               ( Fw64s  val, Fw64s  *pDst, int len );
FwStatus STDCALL fwsSet_64f               ( Fw64f  val, Fw64f  *pDst, int len );
FwStatus STDCALL fwsSet_16sc              ( Fw16sc val, Fw16sc *pDst, int len );
FwStatus STDCALL fwsSet_32sc              ( Fw32sc val, Fw32sc *pDst, int len );
FwStatus STDCALL fwsSet_32fc              ( Fw32fc val, Fw32fc *pDst, int len );
FwStatus STDCALL fwsSet_64sc              ( Fw64sc val, Fw64sc *pDst, int len );
FwStatus STDCALL fwsSet_64fc              ( Fw64fc val, Fw64fc *pDst, int len );

/*#FunctionBlock - Zero
TODO:publish
#Technologies - REF, MT, SSE2, F10H
#Short - Clear vector
#Long - <Text>These functions clear elements of a vector in a destination buffer to zero.</Text>
<Text>A length parameter specifies the number of elements that are cleared.</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsSizeErr
*/
FwStatus STDCALL fwsZero_8u               ( Fw8u   *pDst, int len );
FwStatus STDCALL fwsZero_16s              ( Fw16s  *pDst, int len );
FwStatus STDCALL fwsZero_32f              ( Fw32f  *pDst, int len );
FwStatus STDCALL fwsZero_64f              ( Fw64f  *pDst, int len );
FwStatus STDCALL fwsZero_16sc             ( Fw16sc *pDst, int len );
FwStatus STDCALL fwsZero_32fc             ( Fw32fc *pDst, int len );



/*#FunctionBlock - Find Sbubstring
TODO:publish
#Technologies - REF
#Short - Find substring in a string
#Long - <Text>Find substring in a string</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsLengthErr
*/

FwStatus fwsFind_8u(const Fw8u* pSrc, int len, const Fw8u* pFind, int lenFind, int* pIndex);
FwStatus fwsFind_16u(const Fw16u* pSrc, int len, const Fw16u* pFind, int lenFind, int* pIndex);
FwStatus fwsFindRev_8u(const Fw8u* pSrc, int len, const Fw8u* pFind, int lenFind, int* pIndex);
FwStatus fwsFindRev_16u(const Fw16u* pSrc, int len, const Fw16u* pFind, int lenFind, int* pIndex);

/*#FunctionBlock - Find Value
TODO:publish
#Technologies - REF
#Short - Find the specified element in a given buffer
#Long - <Text>Find the specified element in a given buffer</Text>
#ReturnValues - fwStsNoErr, fwStsNullPtrErr, fwStsLengthErr
*/

FwStatus fwsFindC_8u(const Fw8u* pSrc, int len, Fw8u valFind, int* pIndex);
FwStatus fwsFindC_16u(const Fw16u* pSrc, int len, Fw16u valFind, int* pIndex);
FwStatus fwsFindRevC_8u(const Fw8u* pSrc, int len, Fw8u valFind, int* pIndex);
FwStatus fwsFindRevC_16u(const Fw16u* pSrc, int len, Fw16u valFind, int* pIndex);

/*#Documentation
</Chapter>
*/

#ifdef __cplusplus
}
#endif


#endif // __FWSIGNAL_H__
