/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORMODELCONVSPECIALDEF_H__
#define __COLORMODELCONVSPECIALDEF_H__

#include "colorModelConversion.h"
#include "fe.h"

namespace OPT_LEVEL
{
    enum RGB_Format
    {
        RGB555_FORMAT,
        RGB565_FORMAT,
        RGB444_FORMAT,
        BGR555_FORMAT,
        BGR565_FORMAT, 
        BGR444_FORMAT,
        RGB_FORMAT,
        BGR_FORMAT
    };

    template< class TS, class TD, RGB_Format rgbType >
    SYS_INLINE static void PackAndStore_REFR( TS &rVal, TS &gVal, TS &bVal, TD *dst )
    {
        switch (rgbType)
        {
        case RGB_FORMAT:            dst[0] = rVal;
                                    dst[1] = gVal;
                                    dst[2] = bVal;                                        break;
        case BGR_FORMAT:            dst[0] = bVal;
                                    dst[1] = gVal;
                                    dst[2] = rVal;                                        break;
        case RGB565_FORMAT:         *dst = FW_REF::Pack565<TS, TD> ( rVal, gVal, bVal ); break;
        case RGB555_FORMAT:         *dst = FW_REF::Pack555<TS, TD> ( rVal, gVal, bVal ); break;
        case RGB444_FORMAT:         *dst = FW_REF::Pack444<TS, TD> ( rVal, gVal, bVal ); break;
        case BGR565_FORMAT:         *dst = FW_REF::Pack565<TS, TD> ( bVal, gVal, rVal ); break;
        case BGR555_FORMAT:         *dst = FW_REF::Pack555<TS, TD> ( bVal, gVal, rVal ); break;
        case BGR444_FORMAT:         *dst = FW_REF::Pack444<TS, TD> ( bVal, gVal, rVal ); break;
        default:                    assert (false);
        }
    }

    template< RGB_Format rgbType, IsAlign ia >
    SYS_INLINE static void PackAndStore_SSE2 (XMM128 &rVal, XMM128 &gVal, XMM128 &bVal, __m128i *dst )
    {
        switch (rgbType)
        {
        case RGB565_FORMAT:
            rVal.i = _mm_srli_epi16 ( rVal.i, 3      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4,R3}
            gVal.i = _mm_srli_epi16 ( gVal.i, 2      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4,G3,G2}
            gVal.i = _mm_slli_epi16 ( gVal.i, 5      ); // gVal.s16[i]={__,__,__,__,__,G7,G6,G5,G4,G3,G2,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, gVal.i ); // rVal.s16[i]={__,__,__,__,__,G7,G6,G5,G4,G3,G2,R7,R6,R5,R4,R3}
            bVal.i = _mm_srli_epi16 ( bVal.i, 3      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4,B3}
            bVal.i = _mm_slli_epi16 ( bVal.i, 11     ); // bVal.s16[i]={B7,B6,B5,B4,B3,__,__,__,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={B7,B6,B5,B4,B3,G7,G6,G5,G4,G3,G2,R7,R6,R5,R4,R3}
            break;
        case RGB555_FORMAT: 
            rVal.i = _mm_srli_epi16 ( rVal.i, 3      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4,R3}
            gVal.i = _mm_srli_epi16 ( gVal.i, 3      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4,G3}
            gVal.i = _mm_slli_epi16 ( gVal.i, 5      ); // gVal.s16[i]={__,__,__,__,__,__,G7,G6,G5,G4,G3,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, gVal.i ); // rVal.s16[i]={__,__,__,__,__,__,G7,G6,G5,G4,G3,R7,R6,R5,R4,R3}
            bVal.i = _mm_srli_epi16 ( bVal.i, 3      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4,B3}
            bVal.i = _mm_slli_epi16 ( bVal.i, 10     ); // bVal.s16[i]={__,B7,B6,B5,B4,B3,__,__,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={__,B7,B6,B5,B4,B3,G7,G6,G5,G4,G3,R7,R6,R5,R4,R3}
            break;
        case RGB444_FORMAT:
            rVal.i = _mm_srli_epi16 ( rVal.i, 4      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4}
            gVal.i = _mm_srli_epi16 ( gVal.i, 4      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4}
            gVal.i = _mm_slli_epi16 ( gVal.i, 4      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,G7,G6,G5,G4,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, gVal.i ); // rVal.s16[i]={__,__,__,__,__,__,__,__,G7,G6,G5,G4,R7,R6,R5,R4}
            bVal.i = _mm_srli_epi16 ( bVal.i, 4      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4}
            bVal.i = _mm_slli_epi16 ( bVal.i, 8      ); // bVal.s16[i]={__,__,__,__,B7,B6,B5,B4,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={__,__,__,__,B7,B6,B5,B4,G7,G6,G5,G4,R7,R6,R5,R4}
            break;
        case BGR565_FORMAT:
            bVal.i = _mm_srli_epi16 ( bVal.i, 3      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4,B3}
            gVal.i = _mm_srli_epi16 ( gVal.i, 2      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4,G3,G2}
            gVal.i = _mm_slli_epi16 ( gVal.i, 5      ); // gVal.s16[i]={__,__,__,__,__,G7,G6,G5,G4,G3,G2,__,__,__,__,__}
            bVal.i = _mm_or_si128   ( bVal.i, gVal.i ); // bVal.s16[i]={__,__,__,__,__,G7,G6,G5,G4,G3,G2,B7,B6,B5,B4,B3}
            rVal.i = _mm_srli_epi16 ( rVal.i, 3      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4,R3}
            rVal.i = _mm_slli_epi16 ( rVal.i, 11     ); // rVal.s16[i]={R7,R6,R5,R4,R3,__,__,__,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={R7,R6,R5,R4,R3,G7,G6,G5,G4,G3,G2,B7,B6,B5,B4,B3}
            break;
        case BGR555_FORMAT:
            bVal.i = _mm_srli_epi16 ( bVal.i, 3      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4,B3}
            gVal.i = _mm_srli_epi16 ( gVal.i, 3      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4,G3}
            gVal.i = _mm_slli_epi16 ( gVal.i, 5      ); // gVal.s16[i]={__,__,__,__,__,__,G7,G6,G5,G4,G3,__,__,__,__,__}
            bVal.i = _mm_or_si128   ( bVal.i, gVal.i ); // bVal.s16[i]={__,__,__,__,__,__,G7,G6,G5,G4,G3,B7,B6,B5,B4,B3}
            rVal.i = _mm_srli_epi16 ( rVal.i, 3      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4,R3}
            rVal.i = _mm_slli_epi16 ( rVal.i, 10     ); // rVal.s16[i]={__,R7,R6,R5,R4,R3,__,__,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={__,R7,R6,R5,R4,R3,G7,G6,G5,G4,G3,B7,B6,B5,B4,B3}
            break;
        case BGR444_FORMAT: 
            bVal.i = _mm_srli_epi16 ( bVal.i, 4      ); // bVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,B7,B6,B5,B4}
            gVal.i = _mm_srli_epi16 ( gVal.i, 4      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,G7,G6,G5,G4}
            gVal.i = _mm_slli_epi16 ( gVal.i, 4      ); // gVal.s16[i]={__,__,__,__,__,__,__,__,G7,G6,G5,G4,__,__,__,__}
            bVal.i = _mm_or_si128   ( bVal.i, gVal.i ); // bVal.s16[i]={__,__,__,__,__,__,__,__,G7,G6,G5,G4,B7,B6,B5,B4}
            rVal.i = _mm_srli_epi16 ( rVal.i, 4      ); // rVal.s16[i]={__,__,__,__,__,__,__,__,__,__,__,__,R7,R6,R5,R4}
            rVal.i = _mm_slli_epi16 ( rVal.i, 8      ); // rVal.s16[i]={__,__,__,__,R7,R6,R5,R4,__,__,__,__,__,__,__,__}
            rVal.i = _mm_or_si128   ( rVal.i, bVal.i ); // rVal.s16[i]={__,__,__,__,R7,R6,R5,R4,G7,G6,G5,G4,B7,B6,B5,B4}
            break;
        default: 
            assert (false);
        }

        // Store to Memory
        LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rVal, (void*)dst);
    }

    namespace RGBToYUV_DEF // TODO
    {
        namespace Src444_To_Dst444
        {
        }; // namespace Src444_To_Dst444
        namespace Src444_To_Dst422
        {
        }; // namespace Src444_To_Dst422
        namespace Src444_To_Dst420
        {
        }; // namespace Src444_To_Dst420
    }; // namespace RGBToYUV_DEF


    namespace YUVToRGB_DEF
    {
        namespace Src422_To_Dst444
        {
            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd, RGB_Format rgbType>
            struct ChannelToChannel : public fe2 <TS, cs, TD, cd>
            {
                int srcStep;
                int dstStep;
                FwiSize roiSize;
                int numBlocks, remWidth;

                ChannelToChannel (const int srcStep_, const int dstStep_, const FwiSize roiSize_) 
                    : srcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; // Calculate number of blocks that CAN    be processed by SSE2
                    remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be processed by SSE2
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }
                IV SSE2( RegFile & ) const { assert(false); }
                
                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pSrcRow = (TS*)srcPtr;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pSrcRowEnd = pSrcRow+(roiSize.width - (roiSize.width&0x1))*sizeof(TS)*CH_COUNT(cs);
                    TD *pDstRow = dstPtr;
                    TS yuv[3], rgb[3];

                    if ( chCountD == CMC::CH_C3 )
                    {
                        A32S dstInc = CH_COUNT(cd);
                        // YUV 422 to RGB No Pack
                        for ( ; pSrcRow<pSrcRowEnd; pSrcRow += 4,
                                                  pDstRow += (2 * dstInc))
                        {
                            yuv[0] = pSrcRow[0];    // Y0 Value
                            yuv[1] = pSrcRow[1];    //  U Value
                            yuv[2] = pSrcRow[3];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow);

                            yuv[0] = pSrcRow[2];    // Y1 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow + dstInc);
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pSrcRow = (TS*)srcPtr;
                    TD *pDstRow = dstPtr;
                    XMM128 yHi, yLo, uVal, vVal, chVal, rLo, gLo, bLo, bVal;

                    if ( chCountD == CMC::CH_C3 )
                    {
                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pSrcRow, 16*sizeof(TS)*CH_COUNT(cs)), 
                                                        FW_REF::Inc<TD>(pDstRow, 16*sizeof(TD)*CH_COUNT(cd)) )

                        {
                            //  Read 32 bytes of data
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pSrcRow);
                            // yVal = {V3,Y7,U3,Y6,V2,Y5,U2,Y4,V1,Y3,U1,Y2,V0,Y1,U0,Y0}
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&uVal, (const void*)((__m128i*)pSrcRow+1));
                            // uVal = {V7,YF,U7,YE,V6,YD,U6,YC,V5,YB,U5,YA,V4,Y9,U4,Y8}

                            vVal.i = _mm_set1_epi16     ( 0x00ff                          ); 
                            // vVal = {__,ff,__,ff,__,ff,__,ff,__,ff,__,ff,__,ff,__,ff}
                            yHi.i  = _mm_and_si128      ( yLo.i,  vVal.i                  ); 
                            // yHi  = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}
                            chVal.i  = _mm_and_si128    ( vVal.i, uVal.i                  ); 
                            // tmp  = {__,YF,__,YE,__,YD,__,YC,__,YB,__,YA,__,Y9,__,Y8}
                            yHi.i  = _mm_packus_epi16   ( yHi.i,  chVal.i                 );
                            // yHi  = {YF,YE,YD,YC,YB,YA,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0}

                            vVal.i = _mm_slli_epi16     ( vVal.i, 8                       ); 
                            // vVal = {ff,__,ff,__,ff,__,ff,__,ff,__,ff,__,ff,__,ff,__}
                            yLo.i  = _mm_and_si128      ( yLo.i,  vVal.i                  ); 
                            // yLo  = {V3,__,U3,__,V2,__,U2,__,V1,__,U1,__,V0,__,U0,__}
                            uVal.i = _mm_and_si128      ( uVal.i, vVal.i                  );
                            // uVal = {V7,__,U7,__,V6,__,U6,__,V5,__,U5,__,V4,__,U4,__}
                            uVal.i = _mm_srli_epi16     ( uVal.i, 8                       );
                            // uVal = {__,V7,__,U7,__,V6,__,U6,__,V5,__,U5,__,V4,__,U4}	
                            uVal.i = _mm_shufflelo_epi16( uVal.i, _MM_SHUFFLE(3, 1, 2, 0) ); 
                            // uVal = {__,V7,__,U7,__,V6,__,U6,__,V5,__,V4,__,U5,__,U4}
                            uVal.i = _mm_shufflehi_epi16( uVal.i, _MM_SHUFFLE(3, 1, 2, 0) ); 
                            // uVal = {__,V7,__,V6,__,U7,__,U6,__,V5,__,V4,__,U5,__,U4}
                            uVal.i = _mm_shuffle_epi32  ( uVal.i, _MM_SHUFFLE(3, 1, 2, 0) );
                            // uVal = {__,V7,__,V6,__,V5,__,V4,__,U7,__,U6,__,U5,__,U4}
                            yLo.i  = _mm_srli_epi16     ( yLo.i, 8                        );
                            // yLo  = {__,V3,__,U3,__,V2,__,U2,__,V1,__,U1,__,V0,__,U0}	
                            yLo.i  = _mm_shufflelo_epi16( yLo.i, _MM_SHUFFLE(3, 1, 2, 0)  );
                            // yLo  = {__,V3,__,U3,__,V2,__,U2,__,V1,__,V0,__,U1,__,U0}
                            yLo.i  = _mm_shufflehi_epi16( yLo.i, _MM_SHUFFLE(3, 1, 2, 0)  );
                            // yLo  = {__,V3,__,V2,__,U3,__,U2,__,V1,__,V0,__,U1,__,U0}
                            yLo.i  = _mm_shuffle_epi32  ( yLo.i, _MM_SHUFFLE(3, 1, 2, 0)  );
                            // yLo  = {__,V3,__,V2,__,V1,__,V0,__,U3,__,U2,__,U1,__,U0}
                            uVal.i = _mm_packus_epi16   ( yLo.i, uVal.i                   );
                            // uVal = {V7,V6,V5,V4,U7,U6,U5,U4,V3,V2,V1,V0,U3,U2,U1,U0}
                            uVal.i = _mm_shuffle_epi32  ( uVal.i, _MM_SHUFFLE(3, 1, 2, 0) );
                            // uVal = {V7,V6,V5,V4,V3,V2,V1,V0,U7,U6,U5,U4,U3,U2,U1,U0}
                            yLo.i  = _mm_setzero_si128  (                                 );
                            // yLo  = {__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__}
                            vVal.i = _mm_unpackhi_epi8  ( uVal.i, yLo.i                   );
                            // vVal = {__,V7,__,V6,__,V5,__,V4,__,V3,__,V2,__,V1,__,V0}
                            uVal.i = _mm_unpacklo_epi8  ( uVal.i, yLo.i                   );
                            // uVal = {__,U7,__,U6,__,U5,__,U4,__,U3,__,U2,__,U1,__,U0} 
                            
                            //R = Y                 + 1.140*(V-128)
                            //G = Y - 0.394*(U-128) - 0.581*(V-128)
                            //B = Y - 2.032*(U-128)

                            // Because this is 422 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.

                            // Compute components involving channels U and V
                            chVal.i = _mm_set1_epi16 ( (short)(128)    );
                            vVal.i  = _mm_subs_epi16 ( vVal.i, chVal.i ); // vVal = v-128

                            rLo.i   = _mm_set1_epi16 ( (short)(73)     ); // r = ( 1.140*(2^8) )
                            rLo.i   = _mm_mullo_epi16( rLo.i, vVal.i   ); // r = ( 1.140*(V-128) )*(2^8)
                            rLo.i   = _mm_srai_epi16 ( rLo.i, 6        ); // r =   1.140*(V-128) 

                            gLo.i   = _mm_set1_epi16 ( (short)(-37)    ); // g = (-0.581*(2^8) )
                            gLo.i   = _mm_mullo_epi16( gLo.i, vVal.i   ); // g = (-0.581*(V-128) )*(2^8)

                            uVal.i = _mm_subs_epi16  ( uVal.i, chVal.i ); // uVal  = u-128

                            chVal.i = _mm_set1_epi16 ( (short)(-25)    ); // chVal = (-0.394*(2^8) )
                            chVal.i = _mm_mullo_epi16( chVal.i, uVal.i ); // b = (-0.394*(U-128))*(2^8)
                            gLo.i   = _mm_adds_epi16 ( gLo.i, chVal.i  ); // g = (-0.581*(V-128))*(2^8) + b 
                            gLo.i   = _mm_srai_epi16 ( gLo.i, 6        ); // g =  -0.394*(U-128) - 0.581*(V-128)

                            bLo.i   = _mm_set1_epi16 ( (short)(130)    ); // b = ( 2.032*(2^8) )
                            bLo.i   = _mm_mullo_epi16( bLo.i, uVal.i   ); // b = ( 2.032*(U-128) )*(2^8)
                            bLo.i   = _mm_srai_epi16 ( bLo.i, 6        ); // b =   2.032*(U-128)

                            // rLo =                 1.140(V-128)
                            // gLo = -0.394(U-128) - 0.581(V-128)
                            // bLo =  2.032(U-128)

                            //  ******************** ROW ************************
                            chVal.i = _mm_setzero_si128 ();
                            yLo.i   = _mm_unpacklo_epi8 ( yHi.i, chVal.i ); 
                            // yLo = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}
                            yHi.i   = _mm_unpackhi_epi8 ( yHi.i, chVal.i ); 
                            // yHi = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}

                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            uVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = Y + 1.140*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            vVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );

                            // **** PIXELS 8 to 15 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                            uVal.i  = _mm_packus_epi16  ( uVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                            // --Green--
                            chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            vVal.i  = _mm_packus_epi16  ( vVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                            // --Blue--
                            chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                            // Pack & Store
                            switch( rgbType )
                            {
                            case RGB_FORMAT:
                                if (ia == ALIGN_TRUE)
                                    CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( uVal.i, vVal.i, bVal.i, pDstRow );
                                else
                                    CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( uVal.i, vVal.i, bVal.i, pDstRow );
                                break;
                            case BGR_FORMAT:
                                if (ia == ALIGN_TRUE)
                                    CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i, vVal.i, uVal.i, pDstRow );
                                else
                                    CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i, vVal.i, uVal.i, pDstRow );
                                break;
                            default:
                                assert(false);
                            }
                        }
                        if( remWidth )
                        {
                            TS yuv[3];
                            // YUV 422 to RGB 
                            for ( int i=remWidth; i>1 ; i-=2, FW_REF::Inc<TS>(pSrcRow, 2*sizeof(TS)*CH_COUNT(cs)), 
                                                              FW_REF::Inc<TD>(pDstRow, 2*sizeof(TD)*CH_COUNT(cd)) )
                            {
                                yuv[0] = pSrcRow[0];    // Y0 Value
                                yuv[1] = pSrcRow[1];    //  U Value
                                yuv[2] = pSrcRow[3];    //  V Value
                                YUVToRGB<TS, TS>( yuv, C3, pDstRow, C3);    // Compute RGB from Y0, U0, V0

                                yuv[0] = pSrcRow[2];    // Y1 Value
                                // Compute RGB from Y1, U0, V0
                                YUVToRGB<TS, TS>( yuv, C3, FW_REF::Offset(pDstRow, sizeof(TD)*CH_COUNT(cd)), C3);    
                            }
                        }
                    }
                    else
                        assert (false);
                }
            };

            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd, RGB_Format rgbType>
            struct PlanarToChannel : public fe4 <TS, cs, TS, cs, TS, cs, TD, cd>
            {
                int *pSrcStep;
                int dstStep;
                FwiSize roiSize;
                int numBlocks, remWidth;

                PlanarToChannel (int *srcStep_, const int dstStep_, const FwiSize roiSize_) 
                    : pSrcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; // Calculate number of blocks that CAN    be processed by SSE2
                    remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be processed by SSE2
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }
                IV SSE2( RegFile & ) const { assert(false); }

                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1 = (TS*)srcPtr_0;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pY_Row1End = pY_Row1+(roiSize.width - (roiSize.width&0x1))*sizeof(TS);
                    TS *pU_Row  = (TS*)srcPtr_1;
                    TS *pV_Row  = (TS*)srcPtr_2;
                    TD *pDstRow1 = dstPtr;
                    TS yuv[3], rgb[3];

                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        A32S dstInc = CH_COUNT(cd);
                        // YUV 422 to RGB No Pack
                        for ( ; pY_Row1<pY_Row1End; pY_Row1+=2,   pDstRow1+=(2*dstInc),
                                                pU_Row +=1,   pV_Row +=1            )
                        {
                            yuv[0] = pY_Row1[0];    // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                            yuv[0] = pY_Row1[1];    // Y1 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1+dstInc);
                        }

                        // Process the last remaining odd-numbered pixel
                        if( roiSize.width&0x1 )
                        {
                            yuv[0] = pY_Row1[0];    // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1   = (TS*)srcPtr_0;
                    TS *pU_Row    = (TS*)srcPtr_1;
                    TS *pV_Row    = (TS*)srcPtr_2;
                    TD *pDstRow1 = (TD*)dstPtr;
                    XMM128 rLo,  gLo,  bLo, yLo, rVal, gVal, bVal, yHi, chVal;

                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pY_Row1,   16*sizeof(TS)), 
                                                        FW_REF::Inc<TD>(pDstRow1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TS>(pU_Row,     8*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pV_Row,     8*sizeof(TS) )   )
                        {
                            //
                            //R = Y                 + 1.140*(V-128)
                            //G = Y - 0.394*(U-128) - 0.581*(V-128)
                            //B = Y - 2.032*(U-128)

                            // Because this is 422 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.

                            // Compute components involving channels U and V
                            chVal.i = _mm_loadu_si128  ( (__m128i*)pV_Row ); 
                            // chVal = {Vf,Ve,Vd,Vc,Vb,Va,V9,V8,V7,V6,V5,V4,V3,V2,V1,V0}

                            yHi.i   = _mm_setzero_si128();
                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i );
                            // chVal = {__,V7,__,V6,__,V5,__,V4,__,V3,__,V2,__,V1,__,V0}
                            bLo.i   = _mm_set1_epi16   ( (short)(128)   );
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i );
                            // chVal = v-128

                            rLo.i   = _mm_set1_epi16   ( (short)(73)    ); // r = ( 1.140*(2^8) )
                            rLo.i   = _mm_mullo_epi16  ( rLo.i, chVal.i ); // r = ( 1.140*(V-128) )*(2^8)
                            rLo.i   = _mm_srai_epi16   ( rLo.i, 6       ); // r =   1.140*(V-128)

                            gLo.i   = _mm_set1_epi16   ( (short)(-37)   ); // g = (-0.581*(2^8) )
                            gLo.i   = _mm_mullo_epi16  ( gLo.i, chVal.i ); // g = (-0.581*(V-128) )*(2^8)

                            chVal.i = _mm_loadu_si128  ( (__m128i*)pU_Row ); // chVal = {Uf,Ue,Ud,...,U2,U1,U0}

                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i ); 
                            // chVal = {__,U7,__,U6,__,U5,__,U4,__,U3,__,U2,__,U1,__,U0}
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i ); // chVal = u-128

                            bLo.i   = _mm_set1_epi16   ( (short)(-25)   ); // b = (-0.394*(2^8))
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = (-0.394*(U-128) )*(2^8)
                            gLo.i   = _mm_adds_epi16   ( gLo.i, bLo.i   ); // g = (-0.581*(V-128) )*(2^8) + b
                            gLo.i   = _mm_srai_epi16   ( gLo.i, 6       ); // g =  -0.394*(U-128) - 0.581*(V-128)

                            bLo.i   = _mm_set1_epi16   ( (short)(130)   ); // b = ( 2.032*(2^8) )
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = ( 2.032*(U-128) )*(2^8)
                            bLo.i   = _mm_srai_epi16   ( bLo.i, 6       ); // b =   2.032*(U-128)

                            // rLo =                 1.140(V-128)
                            // gLo = -0.394(U-128) - 0.581(V-128)
                            // bLo =  2.032(U-128)

                            //  ******************** ROW ************************
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row1);
                            // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                            // Unpack Y
                            chVal.i = _mm_setzero_si128 ();
                            yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i    );
                            // yHi = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                            yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i    );
                            // yLo = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = Y + 1.140*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );

                            // **** PIXELS 8 to 15 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal = {R7,R7,R6,R6,R5,R5,R4,R4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                            rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                            // --Green--
                            chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                            // --Blue--
                            chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                            // Pack & Store
                            if( C3 == cd )
                            {
                                switch( rgbType )
                                {
                                case RGB_FORMAT:
                                    if (ia == ALIGN_TRUE)
                                        CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( rVal.i, gVal.i, bVal.i, pDstRow1 );
                                    else
                                        CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( rVal.i, gVal.i, bVal.i, pDstRow1 );
                                    break;
                                case BGR_FORMAT:
                                    if (ia == ALIGN_TRUE)
                                        CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i, gVal.i, rVal.i, pDstRow1 );
                                    else
                                        CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i, gVal.i, rVal.i, pDstRow1 );
                                    break;
                                default:
                                    assert(false);
                                }
                            }
                            else if( AC4 == cd )
                            {
                                switch( rgbType )
                                {
                                case RGB_FORMAT:
                                    if (ia == ALIGN_TRUE)
                                        CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( rVal.i, gVal.i, bVal.i, 
                                                                                          _mm_setzero_si128(), pDstRow1 );
                                    else
                                        CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( rVal.i, gVal.i, bVal.i, 
                                                                                          _mm_setzero_si128(), pDstRow1 );
                                    break;
                                case BGR_FORMAT:
                                    if (ia == ALIGN_TRUE)
                                        CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( bVal.i, gVal.i, rVal.i, 
                                                                                          _mm_setzero_si128(), pDstRow1 );
                                    else
                                        CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( bVal.i, gVal.i, rVal.i, 
                                                                                          _mm_setzero_si128(), pDstRow1 );
                                    break;
                                default:
                                    assert(false);
                                }
                            }
                            else
                                assert(false);
                        }

                        if( remWidth )
                        {
                            TS *pY_Row1End = pY_Row1+remWidth*sizeof(TS);
                            TS yuv[3], rgb[3];
                            // YUV 422 to RGB 
                            for ( int i=remWidth; pY_Row1<pY_Row1End; i-=2, 
                                                                  FW_REF::Inc<TS>(pY_Row1,   2*sizeof(TS)), 
                                                                  FW_REF::Inc<TD>(pDstRow1, 2*sizeof(TD)*CH_COUNT(cd)),
                                                                  FW_REF::Inc<TS>(pU_Row,    1*sizeof(TS)), 
                                                                  FW_REF::Inc<TS>(pV_Row,    1*sizeof(TS)) )

                            {
                                yuv[0] = pY_Row1[0];    // Y0 Value
                                yuv[1] =  pU_Row[0];    //  U Value
                                yuv[2] =  pV_Row[0];    //  V Value
                                YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                                PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                                if (i>1)
                                {
                                    yuv[0] = pY_Row1[1]; // Y1 Value
                                    // Compute RGB from Y1, U0, V0
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);
                                    PackAndStore_REFR<TS, TD, rgbType>( rgb[0], rgb[1], rgb[2], 
                                                                        FW_REF::Offset<TD>(pDstRow1, 
                                                                                            sizeof(TD)*CH_COUNT(cd)) );
                                }
                            } // for loop
                        } // if
                    }
                    else
                    {
                        assert (false);
                    }
                }
            }; // struct PlanarToChannel

            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd, RGB_Format rgbType>
            struct PlanarToPlanar : public fe3S3D <TS, cs, TS, cs, TS, cs, TD, cd, TD, cd, TD, cd>
            {
                const int *pSrcStep;
                const int dstStep;
                const FwiSize roiSize;
                int numBlocks, remWidth;

                PlanarToPlanar (const int *srcStep_, const int dstStep_, const FwiSize roiSize_)
                    : pSrcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; //  Calculate number of blocks that CAN    be processed by SSE2
                    remWidth  = roiSize.width%16; //  Calculate number of pixels that CANNOT be processed by SSE2
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }

                IV SSE2( RegFile & ) const { assert(false); }

                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, 
                    TD *dstPtr_0, TD *dstPtr_1, TD *dstPtr_2, A32U count ) const
                {
                    count;
                    TS *pY_Row1     = (TS*)srcPtr_0;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pY_Row1End  = pY_Row1+(roiSize.width - (roiSize.width&0x1))*sizeof(TS);
                    TS *pU_Row      = (TS*)srcPtr_1;
                    TS *pV_Row      = (TS*)srcPtr_2;
                    TD *pDstRow1_0 = dstPtr_0;
                    TD *pDstRow1_1 = dstPtr_1;
                    TD *pDstRow1_2 = dstPtr_2;
                    TS yuv[3], rgb[3];

                    if ( chCountD == CMC::CH_P3 )
                    {
                        A32S dstInc = CH_COUNT(cd);
                        // YUV 422 to RGB No Pack
                        for ( ; pY_Row1<pY_Row1End; pY_Row1+=2, pU_Row +=1, pV_Row +=1,
                                                pDstRow1_0+=(2*dstInc),
                                                pDstRow1_1+=(2*dstInc),
                                                pDstRow1_2+=(2*dstInc)       )
                        {
                            yuv[0] =  pY_Row1[0];   // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            *pDstRow1_0 = rgb[0];
                            *pDstRow1_1 = rgb[1];
                            *pDstRow1_2 = rgb[2];

                            yuv[0] = pY_Row1[1];    // Y1 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                            *(pDstRow1_0+dstInc) = rgb[0];
                            *(pDstRow1_1+dstInc) = rgb[1];
                            *(pDstRow1_2+dstInc) = rgb[2];
                        }

                        // Process the last remaining odd-numbered pixel
                        if( roiSize.width&0x1 )
                        {
                            yuv[0] = pY_Row1[0];    // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            *pDstRow1_0 = rgb[0];
                            *pDstRow1_1 = rgb[1];
                            *pDstRow1_2 = rgb[2];
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, 
                                        TD *dstPtr_0, TD *dstPtr_1, TD *dstPtr_2, A32U count ) const
                {
                    count;
                    TS *pY_Row1   = (TS*)srcPtr_0;
                    TS *pU_Row    = (TS*)srcPtr_1;
                    TS *pV_Row    = (TS*)srcPtr_2;
                    TD *pDstRow1_0 = dstPtr_0;
                    TD *pDstRow1_1 = dstPtr_1;
                    TD *pDstRow1_2 = dstPtr_2;
                    XMM128 rLo,  gLo,  bLo, yLo, 
                        rVal, gVal, bVal, yHi, chVal;

                    if ( chCountD == CMC::CH_P3 )
                    {
                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pY_Row1,    16*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pU_Row,      8*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pV_Row,      8*sizeof(TS)),
                                                        FW_REF::Inc<TD>(pDstRow1_0, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow1_1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow1_2, 16*sizeof(TD)*CH_COUNT(cd)) )
                        {
                            //
                            //R = Y                 + 1.140*(V-128)
                            //G = Y - 0.394*(U-128) - 0.581*(V-128)
                            //B = Y - 2.032*(U-128)

                            // Because this is 422 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.

                            // Compute components involving channels U and V
                            chVal.i = _mm_loadu_si128  ( (__m128i*)pV_Row ); // chVal = {Vf,Ve,Vd,...,V2,V1,V0}

                            yHi.i   = _mm_setzero_si128(                );
                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i ); 
                            // chVal = {__,V7,__,V6,__,V5,__,V4,__,V3,__,V2,__,V1,__,V0}
                            bLo.i   = _mm_set1_epi16   ( (short)(128)   );
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i ); // chVal = v-128

                            rLo.i   = _mm_set1_epi16   ( (short)(73)    ); // r = ( 1.140*(2^8) )
                            rLo.i   = _mm_mullo_epi16  ( rLo.i, chVal.i ); // r = ( 1.140*(V-128) )*(2^8)
                            rLo.i   = _mm_srai_epi16   ( rLo.i, 6       ); // r =   1.140*(V-128)

                            gLo.i   = _mm_set1_epi16   ( (short)(-37)   ); // g = (-0.581*(2^8) )
                            gLo.i   = _mm_mullo_epi16  ( gLo.i, chVal.i ); // g = (-0.581*(V-128) )*(2^8)

                            chVal.i = _mm_loadu_si128  ( (__m128i*)pU_Row ); // chVal = {Uf,Ue,Ud,...,U2,U1,U0}

                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i ); 
                            // chVal = {__,U7,__,U6,__,U5,__,U4,__,U3,__,U2,__,U1,__,U0}
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i ); // chVal = u-128

                            bLo.i   = _mm_set1_epi16   ( (short)(-25)   ); // b = ( -0.394*(2^8) )
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = ( -0.394*(U-128) )*(2^8)
                            gLo.i   = _mm_adds_epi16   ( gLo.i, bLo.i   ); // g = (-0.581*(V-128) )*(2^8) + b 
                            gLo.i   = _mm_srai_epi16   ( gLo.i, 6       ); // g =  -0.394*(U-128) - 0.581*(V-128)

                            bLo.i   = _mm_set1_epi16   ( (short)(130)   ); // b = ( 2.032*(2^8) )
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = ( 2.032*(U-128) )*(2^8)
                            bLo.i   = _mm_srai_epi16   ( bLo.i, 6       ); // b =   2.032*(U-128)

                            // rLo =                 1.140(V-128)
                            // gLo = -0.394(U-128) - 0.581(V-128)
                            // bLo =  2.032(U-128)

                            //  ******************** ROW ************************
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row1);        
                            // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                            // Unpack Y
                            chVal.i = _mm_setzero_si128 (                );
                            yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i ); 
                            // yHi  = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                            yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i ); 
                            // yLo  = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = Y + 1.140*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );

                            // **** PIXELS 8 to 15 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                            rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                            // --Green--
                            chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                            // --Blue--
                            chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                            bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                            // Store
                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rVal, (void*)pDstRow1_0);
                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&gVal, (void*)pDstRow1_1);
                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&bVal, (void*)pDstRow1_2);
                        }

                        if( remWidth )
                        {
                            TS *pY_Row1End = pY_Row1+remWidth*sizeof(TS);
                            TS yuv[3], rgb[3];
                            // YUV 422 to RGB 

                            for ( int i=remWidth; pY_Row1<pY_Row1End; i-=2, 
                                                                  FW_REF::Inc<TS>(pY_Row1, 2*sizeof(TS)), 
                                                                  FW_REF::Inc<TS>(pU_Row,  1*sizeof(TS)), 
                                                                  FW_REF::Inc<TS>(pV_Row,  1*sizeof(TS)),
                                                                  FW_REF::Inc<TD>(pDstRow1_0, 2*sizeof(TD)*CH_COUNT(cd)),
                                                                  FW_REF::Inc<TD>(pDstRow1_1, 2*sizeof(TD)*CH_COUNT(cd)),
                                                                  FW_REF::Inc<TD>(pDstRow1_2, 2*sizeof(TD)*CH_COUNT(cd)) )
                            {
                                yuv[0] = pY_Row1[0];    // Y0 Value
                                yuv[1] =  pU_Row[0];    //  U Value
                                yuv[2] =  pV_Row[0];    //  V Value
                                YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                                *pDstRow1_0 = rgb[0];
                                *pDstRow1_1 = rgb[1];
                                *pDstRow1_2 = rgb[2];

                                if (i>1)
                                {
                                    yuv[0] = pY_Row1[1];    // Y1 Value
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                                    *(pDstRow1_0+CH_COUNT(cd)) = rgb[0];
                                    *(pDstRow1_1+CH_COUNT(cd)) = rgb[1];
                                    *(pDstRow1_2+CH_COUNT(cd)) = rgb[2];
                                }
                            } // for loop
                        }
                    }
                    else
                    {
                        assert (false);
                    }
                }
            }; // struct PlanarToPlanar 
        }; // namespace Src422_To_Dst444
        namespace Src420_To_Dst444
        {
            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd, RGB_Format rgbType>
            struct PlanarToChannel : public fe4 <TS, cs, TS, cs, TS, cs, TD, cd>
            {
                mutable int rowsProcessed;
                int *pSrcStep;
                int dstStep;
                FwiSize roiSize;
                int numBlocks, remWidth;

                PlanarToChannel ( int *srcStep_,  const int dstStep_,  const FwiSize roiSize_) 
                    : pSrcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; // Calculate number of blocks that can be processed by SSE2
                    remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be process by SSE2
                    rowsProcessed = 0;
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }

                IV SSE2( RegFile & ) const { assert(false); }
                
                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1    = (TS*)srcPtr_0;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pY_Row1End = pY_Row1+(roiSize.width - (roiSize.width&0x1))*sizeof(TS);
                    TS *pU_Row     = (TS*)srcPtr_1;
                    TS *pV_Row     = (TS*)srcPtr_2;
                    TD *pDstRow1  = dstPtr;
                    TS yuv[3], rgb[3];

                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        TS *yRow2   = FW_REF::Offset<TS>( pY_Row1,  pSrcStep[0] );
                        TD *dstRow2 = FW_REF::Offset<TD>( pDstRow1, dstStep    );

                        A32S dstInc;
                        if( (rgbType == RGB_FORMAT) || (rgbType == BGR_FORMAT) )
                            dstInc = CH_COUNT(cd);
                        else
                            dstInc = 1;

                        // YUV 420 to RGB 565/555/444/No Pack
                        for ( ; pY_Row1<pY_Row1End; pY_Row1+=2,   yRow2+=2,
                                                pU_Row +=1,   pV_Row +=1,
                                                pDstRow1+=(2*dstInc), dstRow2+=(2*dstInc) )
                        {
                            yuv[0] = pY_Row1[0];    // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                            yuv[0] = pY_Row1[1];    // Y1 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1+dstInc);

                            yuv[0] = yRow2[0];    // Y2 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y2, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], dstRow2);

                            yuv[0] = yRow2[1];    // Y3 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y3, U0, V0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], dstRow2+dstInc);
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1   = (TS*)srcPtr_0;
                    TS *pU_Row    = (TS*)srcPtr_1;
                    TS *pV_Row    = (TS*)srcPtr_2;
                    TD *pDstRow1 = (TD*)dstPtr;
                    XMM128 rLo,  gLo,  bLo, yLo, 
                        rVal, gVal, bVal, yHi, chVal;

                    //if ( (chCountS == CMC::CH_P3_420) && (chCountD == CMC::CH_C3) )
                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        TS *yRow2 = FW_REF::Offset(pY_Row1, pSrcStep[0]);
                        TD *dstRow2 = FW_REF::Offset(pDstRow1, dstStep);

                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pY_Row1,   16*sizeof(TS)),    
                                                        FW_REF::Inc<TS>(yRow2,   16*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pU_Row,     8*sizeof(TS)),
                                                        FW_REF::Inc<TS>(pV_Row,     8*sizeof(TS)), 
                                                        FW_REF::Inc<TD>(pDstRow1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(dstRow2, 16*sizeof(TD)*CH_COUNT(cd)) )
                        {
                            //R = Y                 + 1.140*(V-128)
                            //G = Y - 0.394*(U-128) - 0.581*(V-128)
                            //B = Y - 2.032*(U-128)

                            // Because this is 420 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.

                            // Compute components involving channels U and V
                            chVal.i = _mm_loadu_si128  ( (__m128i*)pV_Row ); // chVal = {Vf,Ve,Vd,...,V2,V1,V0}

                            yHi.i   = _mm_setzero_si128();
                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i ); 
                            // chVal = {__,V7,__,V6,__,V5,__,V4,__,V3,__,V2,__,V1,__,V0}
                            bLo.i   = _mm_set1_epi16   ( (short)(128)   );
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i ); // chVal = v-128

                            rLo.i   = _mm_set1_epi16   ( (short)(73)    ); // r = ( 1.140*(2^8) )
                            rLo.i   = _mm_mullo_epi16  ( rLo.i, chVal.i ); // r = ( 1.140*(V-128) )*(2^8)
                            rLo.i   = _mm_srai_epi16   ( rLo.i, 6       ); // r =   1.140*(V-128)

                            gLo.i   = _mm_set1_epi16   ( (short)(-37)   ); // g = (-0.581*(2^8) )
                            gLo.i   = _mm_mullo_epi16  ( gLo.i, chVal.i ); // g = (-0.581*(V-128) )*(2^8)

                            chVal.i = _mm_loadu_si128  ( (__m128i*)pU_Row ); // chVal = {Uf,Ue,Ud,...,U2,U1,U0}

                            chVal.i = _mm_unpacklo_epi8( chVal.i, yHi.i ); 
                            // chVal = {__,U7,__,U6,__,U5,__,U4,__,U3,__,U2,__,U1,__,U0}
                            chVal.i = _mm_subs_epi16   ( chVal.i, bLo.i ); // chVal = u-128

                            bLo.i   = _mm_set1_epi16   ( (short)(-25)   ); // b = ( -0.394*(2^8) )
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = ( -0.394*(U-128) )*(2^8)
                            gLo.i   = _mm_adds_epi16   ( gLo.i, bLo.i   ); // g = (-0.581*(V-128) )*(2^8) + b
                            gLo.i   = _mm_srai_epi16   ( gLo.i, 6       ); // g =  -0.394*(U-128) - 0.581*(V-128)

                            bLo.i   = _mm_set1_epi16   ( (short)(130)   ); // b = ( 2.032*(2^8) )
                            bLo.i   = _mm_mullo_epi16  ( bLo.i, chVal.i ); // b = ( 2.032*(U-128) )*(2^8)
                            bLo.i   = _mm_srai_epi16   ( bLo.i, 6       ); // b =   2.032*(U-128)

                            // rLo =                 1.140(V-128)
                            // gLo = -0.394(U-128) - 0.581(V-128)
                            // bLo =  2.032(U-128)

                            //  ******************** TOP ROW ************************
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row1);
                            // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                            // Unpack Y
                            chVal.i = _mm_setzero_si128 ();
                            yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i    );
                            // yHi   = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                            yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i    );
                            // yLo   = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i      ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    ); // R = Y + 1.140*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i      ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    );
                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i      ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    );

                            if( (RGB_FORMAT == rgbType) || (BGR_FORMAT == rgbType) )
                            {
                                // **** PIXELS 8 to 15 in Row 1 ****
                                // --Red--
                                chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                                rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                                // --Green--
                                chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                                // --Blue--
                                chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                                if( C3 == cd )
                                {
                                    switch( rgbType )
                                    {
                                    case RGB_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( rVal.i, gVal.i, 
                                                                                              bVal.i, pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( rVal.i, gVal.i, 
                                                                                              bVal.i, pDstRow1 );
                                        break;
                                    case BGR_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i, gVal.i, 
                                                                                              rVal.i, pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i, gVal.i, 
                                                                                              rVal.i, pDstRow1 );
                                        break;
                                    default:
                                        assert(false);
                                    }
                                }
                                else if( AC4 == cd )
                                {
                                    switch( rgbType )
                                    {
                                    case RGB_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( rVal.i, 
                                                                                              gVal.i, bVal.i, 
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( rVal.i,
                                                                                              gVal.i,
                                                                                              bVal.i, 
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        break;
                                    case BGR_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( bVal.i,
                                                                                              gVal.i,
                                                                                              rVal.i,
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( bVal.i,
                                                                                              gVal.i,
                                                                                              rVal.i,
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        break;
                                    default:
                                        assert(false);
                                    }
                                }
                                else
                                    assert(false);
                            }
                            else
                            {
                                // Saturate value for 8U data type
                                chVal.i  = _mm_setzero_si128();
                                rVal.i = _mm_packus_epi16  ( rVal.i, rVal.i  ); 
                                // rVal = {R7,R6,R5,R4,R3,R2,R1,R0,R7,R6,R5,R4,R3,R2,R1,R0}
                                rVal.i = _mm_unpacklo_epi8 ( rVal.i, chVal.i );
                                // rVal = {__,R7,__,R6,__,R5,__,R4,__,R3,__,R2,__,R1,__,R0}
                                gVal.i = _mm_packus_epi16  ( gVal.i, gVal.i  ); 
                                // gVal = {G7,G6,G5,G4,G3,G2,G1,G0,G7,G6,G5,G4,G3,G2,G1,G0}
                                gVal.i = _mm_unpacklo_epi8 ( gVal.i, chVal.i ); 
                                // gVal = {__,G7,__,G6,__,G5,__,G4,__,G3,__,G2,__,G1,__,G0}
                                bVal.i = _mm_packus_epi16  ( bVal.i, bVal.i  );
                                // bVal = {B7,B6,B5,B4,B3,B2,B1,B0,B7,B6,B5,B4,B3,B2,B1,B0}
                                bVal.i = _mm_unpacklo_epi8 ( bVal.i, chVal.i );
                                // bVal = {__,B7,__,B6,__,B5,__,B4,__,B3,__,B2,__,B1,__,B0}

                                PackAndStore_SSE2<rgbType, ia> ( rVal, gVal, bVal, (__m128i*)pDstRow1);

                                // **** PIXELS 8 to 15 in Row 1 ****
                                // --Red--
                                chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i    ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                rVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  ); // R = Y + 1.140*V
                                // --Green--
                                chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i    ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                gVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  );
                                // --Blue--
                                chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i    ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                bVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  );

                                // Saturate value for 8U data type
                                chVal.i  = _mm_setzero_si128();
                                rVal.i = _mm_packus_epi16  ( rVal.i, rVal.i  ); 
                                // rVal = {Rf,Re,Rd,Rc,Rb,Ra,R9,R8,Rf,Re,Rd,Rc,Rb,Ra,R9,R0}
                                rVal.i = _mm_unpackhi_epi8 ( rVal.i, chVal.i ); 
                                // rVal = {__,Rf,__,Re,__,Rd,__,Rc,__,Rb,__,Ra,__,R9,__,R0}
                                gVal.i = _mm_packus_epi16  ( gVal.i, gVal.i  ); 
                                // gVal = {Gf,Ge,Gd,Gc,Gb,Ga,G9,G8,Gf,Ge,Gd,Gc,Gb,Ga,G9,G0}
                                gVal.i = _mm_unpackhi_epi8 ( gVal.i, chVal.i ); 
                                // gVal = {__,Gf,__,Ge,__,Gd,__,Gc,__,Gb,__,Ga,__,G9,__,G0}
                                bVal.i = _mm_packus_epi16  ( bVal.i, bVal.i  );
                                // bVal = {Bf,Be,Bd,Bc,Bb,Ba,B9,B8,Bf,Be,Bd,Bc,Bb,Ba,B9,B0}
                                bVal.i = _mm_unpackhi_epi8 ( bVal.i, chVal.i ); 
                                // bVal = {__,Bf,__,Be,__,Bd,__,Bc,__,Bb,__,Ba,__,B9,__,B0}

                                PackAndStore_SSE2<rgbType, ia> ( rVal, gVal, bVal, (__m128i*)(pDstRow1+(TD)8));
                            }

                            if ( (roiSize.height-rowsProcessed)>1 )
                            {
                                //  **************** ROW 2 ********************
                                if (ia == ALIGN_TRUE)
                                    yLo.i = _mm_load_si128  ( (__m128i*)yRow2 ); // chVal = {Yf,Ye,Yd,...,Y2,Y1,Y0]
                                else
                                    yLo.i = _mm_loadu_si128 ( (__m128i*)yRow2 ); // chVal = {Yf,Ye,Yd,...,Y2,Y1,Y0]

                                // Unpack Y
                                chVal.i = _mm_setzero_si128 (                 );
                                yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i  ); 
                                // yHi  = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                                yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i  ); 
                                // yLo  = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                                // **** PIXELS 0 to 7 in Row 2 ****
                                // --Red--
                                chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal= {R3,R3,R2,R2,R1,R1,R0,R0}
                                rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = Y + 1.140*V
                                // --Green--
                                chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal= {G3,G3,G2,G2,G1,G1,G0,G0}
                                gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                                // --Blue--
                                chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal= {B3,B3,B2,B2,B1,B1,B0,B0}
                                bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );

                                if( (RGB_FORMAT == rgbType) || (BGR_FORMAT == rgbType) )
                                {
                                    // Free: yLo, chVal
                                    // Used: yHi, rLo, gLo, bLo, rVal, gVal, bVal

                                    // **** PIXELS 8 to 15 in Row 1 ****
                                    // --Red--
                                    chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                                    rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                                    // --Green--
                                    chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                    gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                                    // --Blue--
                                    chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                    bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                                    //PackAndStore_SSE2<rgbType, ia> ( rVal, gVal, bVal, (__m128i*)dstRow2);
                                    if( C3 == cd )
                                    {
                                        switch( rgbType )
                                        {
                                        case RGB_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( rVal.i, 
                                                                                                  gVal.i,
                                                                                                  bVal.i,
                                                                                                  dstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( rVal.i,
                                                                                                  gVal.i,
                                                                                                  bVal.i,
                                                                                                  dstRow2 );
                                            break;
                                        case BGR_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i,
                                                                                                  gVal.i,
                                                                                                  rVal.i,
                                                                                                  dstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i,
                                                                                                  gVal.i,
                                                                                                  rVal.i,
                                                                                                  dstRow2 );
                                            break;
                                        default:
                                            assert(false);
                                        }
                                    }
                                    else if( AC4 == cd )
                                    {
                                        switch( rgbType )
                                        {
                                        case RGB_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( rVal.i, 
                                                                                                  gVal.i, 
                                                                                                  bVal.i,
                                                                                                  _mm_setzero_si128(), 
                                                                                                  dstRow2);
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( rVal.i, 
                                                                                                  gVal.i,
                                                                                                  bVal.i, 
                                                                                                  _mm_setzero_si128(),
                                                                                                  dstRow2 );
                                            break;
                                        case BGR_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( bVal.i,
                                                                                                  gVal.i, 
                                                                                                  rVal.i, 
                                                                                                  _mm_setzero_si128(),
                                                                                                  dstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( bVal.i, 
                                                                                                  gVal.i,
                                                                                                  rVal.i,
                                                                                                  _mm_setzero_si128(),
                                                                                                  dstRow2 );
                                            break;
                                        default:
                                            assert(false);
                                        }
                                    }
                                    else
                                        assert(false);
                                }
                                else
                                {
                                    // Saturate value for 8U data type
                                    chVal.i = _mm_setzero_si128 (                 );
                                    rVal.i  = _mm_packus_epi16  ( rVal.i, rVal.i  );
                                    // rVal = {R7,R6,R5,R4,R3,R2,R1,R0,R7,R6,R5,R4,R3,R2,R1,R0}
                                    rVal.i  = _mm_unpacklo_epi8 ( rVal.i, chVal.i );
                                    // rVal = {__,R7,__,R6,__,R5,__,R4,__,R3,__,R2,__,R1,__,R0}
                                    gVal.i  = _mm_packus_epi16  ( gVal.i, gVal.i  );
                                    // gVal = {G7,G6,G5,G4,G3,G2,G1,G0,G7,G6,G5,G4,G3,G2,G1,G0}
                                    gVal.i  = _mm_unpacklo_epi8 ( gVal.i, chVal.i );
                                    // gVal = {__,G7,__,G6,__,G5,__,G4,__,G3,__,G2,__,G1,__,G0}
                                    bVal.i  = _mm_packus_epi16  ( bVal.i, bVal.i  );
                                    // bVal = {B7,B6,B5,B4,B3,B2,B1,B0,B7,B6,B5,B4,B3,B2,B1,B0}
                                    bVal.i  = _mm_unpacklo_epi8 ( bVal.i, chVal.i );
                                    // bVal = {__,B7,__,B6,__,B5,__,B4,__,B3,__,B2,__,B1,__,B0}

                                    PackAndStore_SSE2<rgbType, ia> ( rVal, gVal, bVal, (__m128i*)dstRow2);

                                    // **** PIXELS 8 to 15 in Row 2 ****
                                    // --Red--
                                    chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i    ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                    rVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  ); // R = Y + 1.140*V
                                    // --Green--
                                    chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i    ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                    gVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  );
                                    // --Blue-- 
                                    chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i    ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                    bVal.i  = _mm_adds_epi16    ( yHi.i, chVal.i  );

                                    // Saturate value for 8U data type
                                    chVal.i  = _mm_setzero_si128(                 );
                                    rVal.i  = _mm_packus_epi16  ( rVal.i, rVal.i  ); 
                                    // rVal = {Rf,Re,Rd,Rc,Rb,Ra,R9,R8,Rf,Re,Rd,Rc,Rb,Ra,R9,R0}
                                    rVal.i  = _mm_unpackhi_epi8 ( rVal.i, chVal.i ); 
                                    // rVal = {__,Rf,__,Re,__,Rd,__,Rc,__,Rb,__,Ra,__,R9,__,R0}
                                    gVal.i  = _mm_packus_epi16  ( gVal.i, gVal.i  ); 
                                    // gVal = {Gf,Ge,Gd,Gc,Gb,Ga,G9,G8,Gf,Ge,Gd,Gc,Gb,Ga,G9,G0}
                                    gVal.i  = _mm_unpackhi_epi8 ( gVal.i, chVal.i ); 
                                    // gVal = {__,Gf,__,Ge,__,Gd,__,Gc,__,Gb,__,Ga,__,G9,__,G0}
                                    bVal.i  = _mm_packus_epi16  ( bVal.i, bVal.i  ); 
                                    // bVal = {Bf,Be,Bd,Bc,Bb,Ba,B9,B8,Bf,Be,Bd,Bc,Bb,Ba,B9,B0}
                                    bVal.i  = _mm_unpackhi_epi8 ( bVal.i, chVal.i ); 
                                    // bVal = {__,Bf,__,Be,__,Bd,__,Bc,__,Bb,__,Ba,__,B9,__,B0}
                                    PackAndStore_SSE2<rgbType, ia> ( rVal, gVal, bVal, (__m128i*)(dstRow2+(TD)8));
                                }
                            }
                        }

                        if (remWidth>1)
                        {
                            // Calculate the pointer to the end of the last even numbered pixel.
                            TS *pY_Row1End = pY_Row1+(remWidth-(remWidth&0x1))*sizeof(TS);
                            TS yuv[3], rgb[3];
                            // YUV 420 to RGB 565/555/444

                            for ( int i=remWidth; pY_Row1<pY_Row1End; i-=2, 
                                                                  FW_REF::Inc<TS>(pY_Row1,   2*sizeof(TS)), 
                                                                  FW_REF::Inc<TS>(yRow2,   2*sizeof(TS)),
                                                                  FW_REF::Inc<TS>(pU_Row,    1*sizeof(TS)), 
                                                                  FW_REF::Inc<TS>(pV_Row,    1*sizeof(TS)),
                                                                  FW_REF::Inc<TD>(pDstRow1, 2*sizeof(TD)*CH_COUNT(cd)), 
                                                                  FW_REF::Inc<TD>(dstRow2, 2*sizeof(TD)*CH_COUNT(cd)) )
                            {
                                yuv[0] = pY_Row1[0];    // Y0 Value
                                yuv[1] =  pU_Row[0];    //  U Value
                                yuv[2] =  pV_Row[0];    //  V Value
                                YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                                PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                                if (i>1)
                                {
                                    yuv[0] = pY_Row1[1];    // Y1 Value
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                                    PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], 
                                                                          rgb[1], 
                                                                          rgb[2], 
                                                                          FW_REF::Offset<TD>(pDstRow1, 
                                                                                              sizeof(TD)*CH_COUNT(cd)));
                                }

                                if ( (roiSize.height-rowsProcessed)>1 )
                                {
                                    yuv[0] = yRow2[0];    // Y2 Value
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y2, U0, V0
                                    PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], dstRow2);

                                    if (i>1)
                                    {
                                        yuv[0] = yRow2[1];    // Y3 Value
                                        YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y3, U0, V0
                                        PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], 
                                                                              rgb[1], 
                                                                              rgb[2], 
                                                                              FW_REF::Offset<TD>(dstRow2, 
                                                                                                  sizeof(TD)*CH_COUNT(cd)));
                                    }
                                }
                            }
                        }
                        rowsProcessed+=2;
                    }
                    else
                    {
                        assert (false);
                    }
                }
            };
            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd >
            struct PlanarToPlanar : public fe3S3D< TS, cs, TS, cs, TS, cs, TD, cd, TD, cd, TD, cd >
            {
                mutable int rowsProcessed;
                const int *pSrcStep;
                const int dstStep;
                const FwiSize roiSize;
                int numBlocks, remWidth;

                PlanarToPlanar (const int *srcStep_, const int dstStep_, const FwiSize roiSize_) 
                    : pSrcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; // Calculate number of blocks that can be processed by SSE2
                    remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be process by SSE2
                    rowsProcessed = 0;
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }

   
                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, 
                                        TD *dstPtr_0, TD *dstPtr_1, TD *dstPtr_2, A32U count ) const
                {
                    count;
                    TS *pY_Row1       = (TS*)srcPtr_0;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pY_Row1End    = pY_Row1+(roiSize.width - (roiSize.width&0x1))*sizeof(TS);
                    TS *pU_Row        = (TS*)srcPtr_1;
                    TS *pV_Row        = (TS*)srcPtr_2;
                    TD *pDstRow1[3]   = {dstPtr_0, dstPtr_1, dstPtr_2};
                    TS yuv[3], rgb[3];

                    if ( chCountD == CMC::CH_P3 )
                    {
                        TS *yRow2       =   FW_REF::Offset<TS>( pY_Row1,      pSrcStep[0] );
                        TD *dstRow2[3]  = { FW_REF::Offset<TD>( pDstRow1[0], dstStep ),
                            FW_REF::Offset<TD>( pDstRow1[1], dstStep ),
                            FW_REF::Offset<TD>( pDstRow1[2], dstStep )  };

                        //----
                        // YUV 420 to RGB No Pack
                        for ( ; pY_Row1<pY_Row1End; pY_Row1+=2,   yRow2+=2,
                                                pU_Row +=1,   pV_Row +=1,
                                                pDstRow1[0]+=2, pDstRow1[1]+=2, pDstRow1[2]+=2,
                                                dstRow2[0]+=2, dstRow2[1]+=2, dstRow2[2]+=2 )
                        {
                            yuv[0] =  pY_Row1[0];    // Y0 Value
                            yuv[1] =  pU_Row[0];    //  U Value
                            yuv[2] =  pV_Row[0];    //  V Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                            *pDstRow1[0] = rgb[0];
                            *pDstRow1[1] = rgb[1];
                            *pDstRow1[2] = rgb[2];

                            yuv[0] = pY_Row1[1];    // Y1 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                            *(pDstRow1[0]+1) = rgb[0];
                            *(pDstRow1[1]+1) = rgb[1];
                            *(pDstRow1[2]+1) = rgb[2];

                            yuv[0] = yRow2[0];    // Y2 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y2, U0, V0
                            *dstRow2[0] = rgb[0];
                            *dstRow2[1] = rgb[1];
                            *dstRow2[2] = rgb[2];

                            yuv[0] = yRow2[1];    // Y3 Value
                            YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y3, U0, V0
                            *(dstRow2[0]+1) = rgb[0];
                            *(dstRow2[1]+1) = rgb[1];
                            *(dstRow2[2]+1) = rgb[2];
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, 
                                        TD *dstPtr_0, TD *dstPtr_1, TD *dstPtr_2, A32U count ) const
                {
                    count;
                    TS *pY_Row1     = (TS*)srcPtr_0;
                    TS *pU_Row      = (TS*)srcPtr_1;
                    TS *pV_Row      = (TS*)srcPtr_2;
                    TD *pDstRow1_0  = (TD*)dstPtr_0;
                    TD *pDstRow1_1  = (TD*)dstPtr_1;
                    TD *pDstRow1_2  = (TD*)dstPtr_2;
                    XMM128 rLo,  gLo,  bLo, yLo, rVal, gVal, bVal, yHi, chVal;

                    if ( chCountD == CMC::CH_P3 )
                    {
                        TS *pYRow2     = FW_REF::Offset(pY_Row1, pSrcStep[0]);
                        TD *pDstRow2_0 = FW_REF::Offset(pDstRow1_0, dstStep);
                        TD *pDstRow2_1 = FW_REF::Offset(pDstRow1_1, dstStep);
                        TD *pDstRow2_2 = FW_REF::Offset(pDstRow1_2, dstStep);

                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pY_Row1,   16*sizeof(TS)),    
                                                        FW_REF::Inc<TS>(pYRow2,   16*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pU_Row,     8*sizeof(TS)),    
                                                        FW_REF::Inc<TS>(pV_Row,     8*sizeof(TS)), 
                                                        FW_REF::Inc<TD>(pDstRow1_0, 16*sizeof(TD)*CH_COUNT(cd)),    
                                                        FW_REF::Inc<TD>(pDstRow1_1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow1_2, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow2_0, 16*sizeof(TD)*CH_COUNT(cd)), 
                                                        FW_REF::Inc<TD>(pDstRow2_1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow2_2, 16*sizeof(TD)*CH_COUNT(cd)) )
                        {
                            //
                            //R = Y                 + 1.140*(V-128)
                            //G = Y - 0.394*(U-128) - 0.581*(V-128)
                            //B = Y - 2.032*(U-128)

                            // Because this is 420 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.
                            //

                            // Compute components involving channels U and V
                            chVal.i = _mm_loadu_si128   ( (__m128i*)pV_Row    );        
                            // chVal = {Vf,Ve,Vd,Vc,Vb,Va,V9,V8,V7,V6,V5,V4,V3,V2,V1,V0}

                            yHi.i   = _mm_setzero_si128 (                );
                            chVal.i = _mm_unpacklo_epi8 ( chVal.i, yHi.i );
                            // chVal = {__,V7,__,V6,__,V5,__,V4,__,V3,__,V2,__,V1,__,V0}
                            bLo.i   = _mm_set1_epi16    ( (short)(128)   );
                            chVal.i = _mm_subs_epi16    ( chVal.i, bLo.i ); // chVal = v-128

                            rLo.i   = _mm_set1_epi16    ( (short)(73)    ); // r = ( 1.140*(2^8) )
                            rLo.i   = _mm_mullo_epi16   ( rLo.i, chVal.i ); // r = ( 1.140*(V-128) )*(2^8)
                            rLo.i   = _mm_srai_epi16    ( rLo.i, 6       ); // r =   1.140*(V-128)

                            gLo.i   = _mm_set1_epi16    ( (short)(-37)   ); // g = (-0.581*(2^8) )
                            gLo.i   = _mm_mullo_epi16   ( gLo.i, chVal.i ); // g = (-0.581*(V-128) )*(2^8)

                            chVal.i = _mm_loadu_si128   ( (__m128i*)pU_Row ); 
                            // chVal = {Uf,Ue,Ud,Uc,Ub,Ua,U9,U8,U7,U6,U5,U4,U3,U2,U1,U0}

                            chVal.i = _mm_unpacklo_epi8 ( chVal.i, yHi.i ); 
                            // chVal = {__,U7,__,U6,__,U5,__,U4,__,U3,__,U2,__,U1,__,U0}
                            chVal.i = _mm_subs_epi16    ( chVal.i, bLo.i ); // chVal = u-128

                            bLo.i   = _mm_set1_epi16    ( (short)(-25)   ); // b = ( -0.394*(2^8) )
                            bLo.i   = _mm_mullo_epi16   ( bLo.i, chVal.i ); // b = ( -0.394*(U-128) )*(2^8)
                            gLo.i   = _mm_adds_epi16    ( gLo.i, bLo.i   ); // g = (-0.581*(V-128) )*(2^8) + b
                            gLo.i   = _mm_srai_epi16    ( gLo.i, 6       ); // g =  -0.394*(U-128) - 0.581*(V-128)

                            bLo.i   = _mm_set1_epi16    ( (short)(130)   ); // b     = ( 2.032*(2^8) )
                            bLo.i   = _mm_mullo_epi16   ( bLo.i, chVal.i ); // b     = ( 2.032*(U-128) )*(2^8)
                            bLo.i   = _mm_srai_epi16    ( bLo.i, 6       ); // b     =   2.032*(U-128)

                            // rLo =                 1.140(V-128)
                            // gLo = -0.394(U-128) - 0.581(V-128)
                            // bLo =  2.032(U-128)

                            //  ******************** TOP ROW ************************
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row1);
                            // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                            // Unpack Y
                            chVal.i = _mm_setzero_si128 (                   );
                            yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i    ); 
                            // yHi  = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                            yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i    );
                            // yLo   = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i      ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    ); // R = Y + 1.140*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i      ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    );
                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i      ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i    );

                            // **** PIXELS 8 to 15 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i      ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i    ); // R = Y + 1.140*V
                            rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i     ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                            // --Green--
                            chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i      ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i    );
                            gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i     ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                            // --Blue--
                            chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i      ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                            yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i    );
                            bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i     ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rVal, (void*)pDstRow1_0);
                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&gVal, (void*)pDstRow1_1);
                            LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&bVal, (void*)pDstRow1_2);

                            if ( (roiSize.height-rowsProcessed)>1 )
                            {
                                //  **************** ROW 2 ********************
                                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pYRow2);
                                // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                                // Unpack Y
                                chVal.i = _mm_setzero_si128 (                );
                                yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i );
                                // yHi  = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                                yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i ); 
                                // yLo  = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                                // **** PIXELS 0 to 7 in Row 2 ****
                                // --Red--
                                chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal= {R3,R3,R2,R2,R1,R1,R0,R0}
                                rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = Y + 1.140*V
                                // --Green--
                                chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal= {G3,G3,G2,G2,G1,G1,G0,G0}
                                gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                                // --Blue--
                                chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal= {B3,B3,B2,B2,B1,B1,B0,B0}
                                bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );

                                // **** PIXELS 8 to 15 in Row 1 ****
                                // --Red--
                                chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i ); // R = Y + 1.140*V
                                rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                                // --Green--
                                chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                                // --Blue--
                                chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&rVal, (void*)pDstRow2_0);
                                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&gVal, (void*)pDstRow2_1);
                                LoadStoreModules::STORE<16, DT_SSE2, ia, STREAM_FLSE>(&bVal, (void*)pDstRow2_2);
                            }
                        }

                        if (remWidth>1)
                        {
                            // Calculate the pointer to the end of the last even numbered pixel.
                            TS *pY_Row1End = pY_Row1+(remWidth-(remWidth&0x1))*sizeof(TS);
                            TS yuv[3], rgb[3];
                            // YUV 420 to RGB 565/555/444

                            for ( int i=remWidth; pY_Row1<pY_Row1End; i-=2, 
                                                                      FW_REF::Inc<TS>(pY_Row1, 2*sizeof(TS)), 
                                                                      FW_REF::Inc<TS>(pYRow2,   2*sizeof(TS)),
                                                                      FW_REF::Inc<TS>(pU_Row,  1*sizeof(TS)), 
                                                                      FW_REF::Inc<TS>(pV_Row,  1*sizeof(TS)),
                                                                      FW_REF::Inc<TD>(pDstRow1_0, 2*sizeof(TD)*CH_COUNT(cd)), 
                                                                      FW_REF::Inc<TD>(pDstRow1_1, 2*sizeof(TD)*CH_COUNT(cd)),
                                                                      FW_REF::Inc<TD>(pDstRow1_2, 2*sizeof(TD)*CH_COUNT(cd)),
                                                                      FW_REF::Inc<TD>(pDstRow2_0, 2*sizeof(TD)*CH_COUNT(cd)), 
                                                                      FW_REF::Inc<TD>(pDstRow2_1, 2*sizeof(TD)*CH_COUNT(cd)), 
                                                                      FW_REF::Inc<TD>(pDstRow2_2, 2*sizeof(TD)*CH_COUNT(cd))  )
                            {
                                yuv[0] = pY_Row1[0];    // Y0 Value
                                yuv[1] =  pU_Row[0];    //  U Value
                                yuv[2] =  pV_Row[0];    //  V Value
                                YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                                *pDstRow1_0 = rgb[0];
                                *pDstRow1_1 = rgb[1];
                                *pDstRow1_2 = rgb[2];

                                if (i>1)
                                {
                                    yuv[0] = pY_Row1[1];    // Y1 Value
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                                    *(pDstRow1_0+CH_COUNT(cd)) = rgb[0];
                                    *(pDstRow1_1+CH_COUNT(cd)) = rgb[1];
                                    *(pDstRow1_2+CH_COUNT(cd)) = rgb[2];
                                }

                                if ( (roiSize.height-rowsProcessed)>1 )
                                {
                                    yuv[0] = pYRow2[0];    // Y2 Value
                                    YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y2, U0, V0
                                    *pDstRow2_0 = rgb[0];
                                    *pDstRow2_1 = rgb[1];
                                    *pDstRow2_2 = rgb[2];

                                    if (i>1)
                                    {
                                        yuv[0] = pYRow2[1];    // Y3 Value
                                        YUVToRGB<TS, TS>( yuv, C3, rgb, C3);    // Compute RGB from Y3, U0, V0
                                        *(pDstRow2_0+CH_COUNT(cd)) = rgb[0];
                                        *(pDstRow2_1+CH_COUNT(cd)) = rgb[1];
                                        *(pDstRow2_2+CH_COUNT(cd)) = rgb[2];
                                    }
                                }
                            }
                        }
                        rowsProcessed+=2;
                    }
                    else
                    {
                        assert (false);
                    }
                }
            };
        };    
    };

    namespace YCbCrToRGB_DEF
    {
        namespace Src420_To_Dst444
        {
            template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd, RGB_Format rgbType>
            struct PlanarToChannel : public fe4 <TS, cs, TS, cs, TS, cs, TD, cd>
            {
                mutable int rowsProcessed;
                int *pSrcStep;
                int dstStep;
                FwiSize roiSize;
                int numBlocks, remWidth;

                PlanarToChannel ( int* srcStep_,  const int dstStep_,  const FwiSize roiSize_) 
                    : pSrcStep(srcStep_), dstStep(dstStep_), roiSize(roiSize_) 
                { 
                    numBlocks = roiSize.width/16; // Calculate number of blocks that can be processed by SSE2
                    remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be process by SSE2
                    rowsProcessed = 0;
                }

                FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
                IV SSE2_Init() { }

                IV SSE2( RegFile & ) const { assert(false); }

                template<IsAlign>
                IV Custom1D_refr( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1 = (TS*)srcPtr_0;
                    // Calculate the pointer to the end of the last even numbered pixel.
                    TS *pY_Row1End = pY_Row1+(roiSize.width - (roiSize.width&0x1))*sizeof(TS);
                    TS *pU_Row  = (TS*)srcPtr_1;
                    TS *pV_Row  = (TS*)srcPtr_2;
                    TD *pDstRow1 = dstPtr;
                    TS ycbcr[3], rgb[3];

                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        TS *yRow2   = FW_REF::Offset<TS>( pY_Row1,  pSrcStep[0] );
                        TD *dstRow2 = FW_REF::Offset<TD>( pDstRow1, dstStep    );

                        A32S dstInc;
                        if( (rgbType == RGB_FORMAT) || (rgbType == BGR_FORMAT) )
                            dstInc = CH_COUNT(cd);
                        else
                            dstInc = 1;

                        // YCbCr 420 to RGB 565/555/444/No Pack
                        for ( ; pY_Row1<pY_Row1End; pY_Row1+=2,   yRow2+=2,
                                                pU_Row +=1,   pV_Row +=1,
                                                pDstRow1+=(2*dstInc), dstRow2+=(2*dstInc) )
                        {
                            ycbcr[0] = pY_Row1[0];    // Y0 Value
                            ycbcr[1] =  pU_Row[0];    //  U Value
                            ycbcr[2] =  pV_Row[0];    //  V Value
                            YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y0, Cb0, Cr0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                            ycbcr[0] = pY_Row1[1];    // Y1 Value
                            YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y1, Cb0, Cr0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1+dstInc);

                            ycbcr[0] = yRow2[0];    // Y2 Value
                            YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y2, Cb0, Cr0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], dstRow2);

                            ycbcr[0] = yRow2[1];    // Y3 Value
                            YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y3, Cb0, Cr0
                            PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], dstRow2+dstInc);
                        }
                    }
                    else
                        assert (false);
                }
                template<IsAlign ia>
                IV Custom1D_sse2( const TS *srcPtr_0, const TS *srcPtr_1, const TS *srcPtr_2, TD *dstPtr, A32U count ) const
                {
                    count;
                    TS *pY_Row1  = (TS*)srcPtr_0;
                    TS *pU_Row   = (TS*)srcPtr_1;
                    TS *pV_Row   = (TS*)srcPtr_2;
                    TD *pDstRow1 = (TD*)dstPtr;
                    XMM128 rLo,  gLo,  bLo, yLo, 
                        rVal, gVal, bVal, yHi, chVal;

                    if ( (chCountD == CMC::CH_C3) || (chCountD == CMC::CH_AC4) )
                    {
                        TS *pY_Row2 = FW_REF::Offset(pY_Row1, pSrcStep[0]);
                        TD *pDstRow2 = FW_REF::Offset(pDstRow1, dstStep);

                        for (int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pY_Row1,   16*sizeof(TS)),    
                                                        FW_REF::Inc<TS>(pY_Row2,   16*sizeof(TS)), 
                                                        FW_REF::Inc<TS>(pU_Row,    8*sizeof(TS)),
                                                        FW_REF::Inc<TS>(pV_Row,    8*sizeof(TS)), 
                                                        FW_REF::Inc<TD>(pDstRow1, 16*sizeof(TD)*CH_COUNT(cd)),
                                                        FW_REF::Inc<TD>(pDstRow2, 16*sizeof(TD)*CH_COUNT(cd)) )
                        {
                            //R = 1.164*(Y-16)                  + 1.596*(Cr-128) 
                            //G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128) 
                            //B = 1.164*(Y-16) + 2.017*(Cb-128)

                            // Because this is 422 chroma sampled, components that involve channels U and V should be 
                            // precomputed before adding them to the Y channel.

                            // Compute components involving channels Cb and Cr
                            chVal.i = _mm_loadu_si128   ( (__m128i*)pV_Row    );
                            // chVal = {Cbf,Cbe,Cbd,Cbc,Cbb,Cba,Cb9,Cb8,Cb7,Cb6,Cb5,Cb4,Cb3,Cb2,Cb1,Cb0}

                            yHi.i   = _mm_setzero_si128 (                );
                            chVal.i = _mm_unpacklo_epi8 ( chVal.i, yHi.i ); 
                            // chVal = {__,Cb7,__,Cb6,__,Cb5,__,Cb4,__,Cb3,__,Cb2,__,Cb1,__,Cb0}
                            bLo.i   = _mm_set1_epi16    ( (short)(128)   );
                            chVal.i = _mm_subs_epi16    ( chVal.i, bLo.i ); // chVal = Cb-128

                            rLo.i   = _mm_set1_epi16    ( (short)(102)   ); // r     = ( 1.596*(2^6) )
                            rLo.i   = _mm_mullo_epi16   ( rLo.i, chVal.i ); // r     = ( 1.596*(Cb-128) )*(2^6)

                            gLo.i   = _mm_set1_epi16    ( (short)(-52)   ); // g     = (-0.813*(2^6) )
                            gLo.i   = _mm_mullo_epi16   ( gLo.i, chVal.i ); // g     = (-0.813*(V-128) )*(2^6)

                            chVal.i = _mm_loadu_si128   ( (__m128i*)pU_Row ); // chVal = {Crf,Cre,Crd,...,Cr2,Cr1,Cr0}

                            chVal.i = _mm_unpacklo_epi8 ( chVal.i, yHi.i ); 
                            // chVal = {__,Cr7,__,Cr6,__,Cr5,__,Cr4,__,Cr3,__,Cr2,__,Cr1,__,Cr0}
                            chVal.i = _mm_subs_epi16    ( chVal.i, bLo.i ); // chVal = Cr-128

                            bLo.i   = _mm_set1_epi16    ( (short)(-25)   ); // b     = ( -0.392*(2^6) )
                            bLo.i   = _mm_mullo_epi16   ( bLo.i, chVal.i ); // b     = ( -0.392*(Cr-128) )*(2^6)
                            gLo.i   = _mm_adds_epi16    ( gLo.i, bLo.i   ); // g     = (-0.813*(Cr-128) )*(2^6) + b 

                            bLo.i   = _mm_set1_epi16    ( (short)(129)   ); // b     = ( 2.017*(2^6) )
                            bLo.i   = _mm_mullo_epi16   ( bLo.i, chVal.i ); // b     = ( 2.017*(Cb-128) )*(2^6)

                            // rLo = (                   1.596*(Cr-128) )*(2^6)
                            // gLo = ( -0.392*(Cb-128) - 0.813*(Cr-128) )*(2^6)
                            // bLo = (  2.017*(Cb-128)                  )*(2^6)

                            //  ******************** TOP ROW ************************
                            LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row1);
                            // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                            // Unpack Y
                            chVal.i = _mm_setzero_si128 (                );
                            yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i ); 
                            // yHi   = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                            yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i ); 
                            // yLo   = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                            chVal.i = _mm_set1_epi16    ( (short)(16)    ); // chVal = 16
                            yLo.i   = _mm_subs_epi16    ( yLo.i, chVal.i ); // yLo   = yLo - 16
                            yHi.i   = _mm_subs_epi16    ( yHi.i, chVal.i ); // yHi   = yHi - 16

                            chVal.i = _mm_set1_epi16    ( (short)(75)    ); // chVal = ( 1.164*(2^6) )
                            yLo.i   = _mm_mullo_epi16   ( yLo.i, chVal.i ); // y     = ( 1.164*(Y-16) )*(2^6)
                            yHi.i   = _mm_mullo_epi16   ( yHi.i, chVal.i ); // y     = ( 1.164*(Y-16) )*(2^6)


                            // **** PIXELS 0 to 7 in Row 1 ****
                            // --Red--
                            chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                            rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // R = (1.164*(Y-16) + 1.596*V)*(2^6)
                            rVal.i  = _mm_srai_epi16    ( rVal.i, 6      ); // R = 1.164*(Y-16) + 1.596*V
                            // --Green--
                            chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                            gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); 
                            // G = (1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128))*(2^6)
                            gVal.i  = _mm_srai_epi16    ( gVal.i, 6      ); 
                            // G = 1.164*(Y-16) - 0.392*(Cb-128) - 0.813*(Cr-128)

                            // --Blue--
                            chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                            bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); 
                            // B = (1.164*(Y-16)+2.017*(Cb-128))*(2^6)
                            bVal.i  = _mm_srai_epi16    ( bVal.i, 6      ); 
                            // B = 1.164*(Y-16) + 2.017*(Cb-128)

                            if( (RGB_FORMAT == rgbType) || (BGR_FORMAT == rgbType) )
                            {
                                // **** PIXELS 8 to 15 in Row 1 ****
                                // --Red--
                                chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                                // --Green--
                                chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G3,G2,G1,G0}
                                // --Blue--
                                chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B3,B2,B1,B0}


                                if( C3 == cd )
                                {
                                    switch( rgbType )
                                    {
                                    case RGB_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( rVal.i, gVal.i, 
                                                                                              bVal.i, pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( rVal.i, gVal.i, 
                                                                                              bVal.i, pDstRow1 );
                                        break;
                                    case BGR_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i, gVal.i, 
                                                                                              rVal.i, pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i, gVal.i, 
                                                                                              rVal.i, pDstRow1 );
                                        break;
                                    default:
                                        assert(false);
                                    }
                                }
                                else if( AC4 == cd )
                                {
                                    switch( rgbType )
                                    {
                                    case RGB_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( rVal.i, 
                                                                                              gVal.i, 
                                                                                              bVal.i, 
                                                                                              _mm_setzero_si128(), 
                                                                                              pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( rVal.i, 
                                                                                              gVal.i, 
                                                                                              bVal.i,
                                                                                              _mm_setzero_si128(), 
                                                                                              pDstRow1 );
                                        break;
                                    case BGR_FORMAT:
                                        if (ia == ALIGN_TRUE)
                                            CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( bVal.i, 
                                                                                              gVal.i,
                                                                                              rVal.i,
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        else
                                            CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( bVal.i,
                                                                                              gVal.i, rVal.i,
                                                                                              _mm_setzero_si128(),
                                                                                              pDstRow1 );
                                        break;
                                    default:
                                        assert(false);
                                    }
                                }
                                else
                                    assert(false);
                            }
                            else
                                assert(false);

                            if ( (roiSize.height-rowsProcessed)>1 )
                            {
                                //  **************** ROW 2 ********************
                                LoadStoreModules::LOAD<16, DT_SSE2, ia, STREAM_FLSE>(&yLo, (const void*)pY_Row2);
                                // chVal = {Yf,Ye,Yd,Yc,Yb,Ya,Y9,Y8,Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0]

                                // Unpack Y
                                chVal.i = _mm_setzero_si128 (                );
                                yHi.i   = _mm_unpackhi_epi8 ( yLo.i, chVal.i );
                                // yHi  = {__,Yf,__,Ye,__,Yd,__,Yc,__,Yb,__,Ya,__,Y9,__,Y8}
                                yLo.i   = _mm_unpacklo_epi8 ( yLo.i, chVal.i );
                                // yLo  = {__,Y7,__,Y6,__,Y5,__,Y4,__,Y3,__,Y2,__,Y1,__,Y0}

                                chVal.i = _mm_set1_epi16    ( (short)(16)    ); // chVal = 16
                                yLo.i   = _mm_subs_epi16    ( yLo.i, chVal.i ); // yLo   = yLo - 16
                                yHi.i   = _mm_subs_epi16    ( yHi.i, chVal.i ); // yHi   = yHi - 16

                                chVal.i = _mm_set1_epi16    ( (short)(75)    ); // chVal = ( 1.164*(2^6) )
                                yLo.i   = _mm_mullo_epi16   ( yLo.i, chVal.i ); // y     = ( 1.164*(Y-16) )*(2^6)
                                yHi.i   = _mm_mullo_epi16   ( yHi.i, chVal.i ); // y     = ( 1.164*(Y-16) )*(2^6)

                                // **** PIXELS 0 to 7 in Row 2 ****
                                // --Red--
                                chVal.i = _mm_unpacklo_epi16( rLo.i, rLo.i   ); // chVal = {R3,R3,R2,R2,R1,R1,R0,R0}
                                rVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // 
                                rVal.i  = _mm_srai_epi16    ( rVal.i, 6      ); // R = {Rf,Re,Rd,...,R2,R1,R0}
                                // --Green--
                                chVal.i = _mm_unpacklo_epi16( gLo.i, gLo.i   ); // chVal = {G3,G3,G2,G2,G1,G1,G0,G0}
                                gVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i ); // 
                                gVal.i  = _mm_srai_epi16    ( gVal.i, 6      ); // G = {Gf,Ge,Gd,...,G2,G1,G0} 
                                // --Blue--
                                chVal.i = _mm_unpacklo_epi16( bLo.i, bLo.i   ); // chVal = {B3,B3,B2,B2,B1,B1,B0,B0}
                                bVal.i  = _mm_adds_epi16    ( yLo.i, chVal.i );
                                bVal.i  = _mm_srai_epi16    ( bVal.i, 6      ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                                if( (RGB_FORMAT == rgbType) || (BGR_FORMAT == rgbType) )
                                {
                                    // Free: yLo, chVal
                                    // Used: yHi, rLo, gLo, bLo, rVal, gVal, bVal

                                    // **** PIXELS 8 to 15 in Row 1 ****
                                    // --Red--
                                    chVal.i = _mm_unpackhi_epi16( rLo.i, rLo.i   ); // chVal= {R7,R7,R6,R6,R5,R5,R4,R4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                    yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                    rVal.i  = _mm_packus_epi16  ( rVal.i, yLo.i  );
                                    // --Green--
                                    chVal.i = _mm_unpackhi_epi16( gLo.i, gLo.i   ); // chVal= {G7,G7,G6,G6,G5,G5,G4,G4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                    yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                    gVal.i  = _mm_packus_epi16  ( gVal.i, yLo.i  ); // G = {Gf,Ge,Gd,...,G2,G1,G0}
                                    // --Blue--
                                    chVal.i = _mm_unpackhi_epi16( bLo.i, bLo.i   ); // chVal= {B7,B7,B6,B6,B5,B5,B4,B4}
                                    yLo.i   = _mm_adds_epi16    ( yHi.i, chVal.i );
                                    yLo.i   = _mm_srai_epi16    ( yLo.i, 6       );
                                    bVal.i  = _mm_packus_epi16  ( bVal.i, yLo.i  ); // B = {Bf,Be,Bd,...,B2,B1,B0}

                                    if( C3 == cd )
                                    {
                                        switch( rgbType )
                                        {
                                        case RGB_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( rVal.i, gVal.i, 
                                                                                                  bVal.i, pDstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( rVal.i, gVal.i, 
                                                                                                  bVal.i, pDstRow2 );
                                            break;
                                        case BGR_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_3P_to_3C_8bit  ( bVal.i, gVal.i, 
                                                                                                  rVal.i, pDstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_3P_to_3C_8bit( bVal.i, gVal.i, 
                                                                                                  rVal.i, pDstRow2 );
                                            break;
                                        default:
                                            assert(false);
                                        }
                                    }
                                    else if( AC4 == cd )
                                    {
                                        switch( rgbType )
                                        {
                                        case RGB_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( rVal.i, 
                                                                                                  gVal.i, 
                                                                                                  bVal.i,
                                                                                                  _mm_setzero_si128(), 
                                                                                                  pDstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( rVal.i, 
                                                                                                  gVal.i,
                                                                                                  bVal.i,
                                                                                                  _mm_setzero_si128(),
                                                                                                  pDstRow2 );
                                            break;
                                        case BGR_FORMAT:
                                            if (ia == ALIGN_TRUE)
                                                CBL_SSE2::ConvertAndStoreAligned_4P_to_4C_8bit  ( bVal.i,
                                                                                                  gVal.i,
                                                                                                  rVal.i,
                                                                                                  _mm_setzero_si128(),
                                                                                                  pDstRow2 );
                                            else
                                                CBL_SSE2::ConvertAndStoreUnaligned_4P_to_4C_8bit( bVal.i, 
                                                                                                  gVal.i, 
                                                                                                  rVal.i,
                                                                                                  _mm_setzero_si128(),
                                                                                                  pDstRow2 );
                                            break;
                                        default:
                                            assert(false);
                                        }
                                    }
                                    else
                                        assert(false);
                                }
                                else
                                    assert(false);
                            }
                        }
                        if (remWidth>1)
                        {
                            // Calculate the pointer to the end of the last even numbered pixel.
                            TS *pY_Row1End = pY_Row1+(remWidth-(remWidth&0x1))*sizeof(TS);
                            TS ycbcr[3], rgb[3];
                            // YCbCr 420 to RGB 565/555/444

                            for ( int i=remWidth; pY_Row1<pY_Row1End; i-=2, 
                                                                      FW_REF::Inc<TS>(pY_Row1,  2*sizeof(TS)), 
                                                                      FW_REF::Inc<TS>(pY_Row2,  2*sizeof(TS)),
                                                                      FW_REF::Inc<TS>(pU_Row,   1*sizeof(TS)),
                                                                      FW_REF::Inc<TS>(pV_Row,   1*sizeof(TS)),
                                                                      FW_REF::Inc<TD>(pDstRow1, 2*sizeof(TD)*CH_COUNT(cd)), 
                                                                      FW_REF::Inc<TD>(pDstRow2, 2*sizeof(TD)*CH_COUNT(cd)))
                            {
                                ycbcr[0] = pY_Row1[0];    // Y0 Value
                                ycbcr[1] =  pU_Row[0];    //  U Value
                                ycbcr[2] =  pV_Row[0];    //  V Value
                                YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y0, U0, V0
                                PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow1);

                                if (i>1)
                                {
                                    ycbcr[0] = pY_Row1[1];    // Y1 Value
                                    YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y1, U0, V0
                                    PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], 
                                                                          rgb[1], 
                                                                          rgb[2], 
                                                                          FW_REF::Offset<TD>(pDstRow1, 
                                                                                              sizeof(TD)*CH_COUNT(cd)) );
                                }

                                if ( (roiSize.height-rowsProcessed)>1 )
                                {
                                    ycbcr[0] = pY_Row2[0];    // Y2 Value
                                    YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);        // Compute RGB from Y2, U0, V0
                                    PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], rgb[1], rgb[2], pDstRow2);

                                    if (i>1)
                                    {
                                        ycbcr[0] = pY_Row2[1];    // Y3 Value
                                        YCbCrToRGB<TS, TS>( ycbcr, C3, rgb, C3);    // Compute RGB from Y3, U0, V0
                                        PackAndStore_REFR <TS, TD, rgbType> ( rgb[0], 
                                                                              rgb[1], 
                                                                              rgb[2], 
                                                                              FW_REF::Offset<TD>(pDstRow2, 
                                                                                                  sizeof(TD)*CH_COUNT(cd)) );
                                    }
                                }
                            }
                        }
                        rowsProcessed+=2;
                    }
                    else
                    {
                        assert (false);
                    }
                }
            };
        }; // Src420_To_Dst444
    };  // YCbCrToRGB_DEF
}; // OPT_LEVEL
#endif // __COLORMODELCONVSPECIALDEF_H__
