/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FORMATCONVERSION_H__
#define __FORMATCONVERSION_H__

#include "fwdev.h"
#include "colorModelConversion.h"
#include "cbl.h"
#include "fe.h"

namespace OPT_LEVEL
{
    namespace F_422To420
    {
        template <CMC::ChCount chCountS, class TS, CH cs, CMC::ChCount chCountD, class TD, CH cd>
        struct ChannelToPlanar : public fe1S3D <TS, cs, TD, cd, TD, cd, TD, cd>
        {
            int srcStep;
            int *pDstStep;
            FwiSize roiSize;
            int numBlocks, remWidth;

            ChannelToPlanar( int srcStep_, int *dstStep_, FwiSize roiSize_) 
                : srcStep(srcStep_), pDstStep(dstStep_), roiSize(roiSize_)
            { 
                numBlocks = roiSize.width/16; // Calculate number of blocks that can be processed by SSE2
                remWidth  = roiSize.width%16; // Calculate number of pixels that CANNOT be process by SSE2
            }

            FE_CUSTOM_NO_FB_SSE2_CUSTOM_NO_FB_REF
            IV SSE2_Init() { }

            template<IsAlign>
            IV Custom1D_refr( const TS* pSrc, TD* pDst_0, TD* pDst_1, TD* pDst_2, A32U count ) const
            {
                count;
                TS *pSrcRow1    = const_cast<TS*>( pSrc );
                TS *pSrcRow2    = FW_REF::Offset( pSrcRow1, srcStep );
                // Calculate the pointer to the end of the last even numbered pixel, 
                    // and don't process the last remaining odd pixel
                TS *pSrcRow1End = FW_REF::Offset( pSrcRow1, ( roiSize.width - 
                                                               (roiSize.width&0x1)) * sizeof(TS) * CH_COUNT(cs) );
                TD *pY_DstRow1  = pDst_0 ;
                TD *pY_DstRow2  = FW_REF::Offset( pY_DstRow1, pDstStep[0] );
                TD *pCb_DstRow  = pDst_1;
                TD *pCr_DstRow  = pDst_2;

                // For 422 chroma-sampled images, 2 Y's, 1 U, and 1V correspond to every 2 pixels of RGB.
                for( ; pSrcRow1<pSrcRow1End; pY_DstRow1+=2, pY_DstRow2+=2, 
                                             ++pCb_DstRow,  ++pCr_DstRow,   
                                             pSrcRow1+=4,   pSrcRow2+=4 )   
                {
                    if( CMC::CH_C2_422_BACA == chCountS )
                    {
                        //     422       TO       420
                        // ----------        ------  -----------  -----------
                        // |B A C A |   ---> |A A |  | (B+B)/2 |  | (C+C)/2 |
                        // |B A C A |        |A A |  -----------  -----------
                        // ----------        ------
                        *pY_DstRow1       = *(pSrcRow1 + 1);
                        *(pY_DstRow1 + 1) = *(pSrcRow1 + 3);
                        *pY_DstRow2       = *(pSrcRow2 + 1);
                        *(pY_DstRow2 + 1) = *(pSrcRow2 + 3);
                        *pCb_DstRow       = (*pSrcRow1 + *pSrcRow2) >> 1;             // Average of Cb values
                        *pCr_DstRow       = (*(pSrcRow1 + 2) + *(pSrcRow2 + 2)) >> 1; // Average of Cr values
                    }
                    else
                        ASSERT(false);
                }
            }
            template<IsAlign ia>
            IV Custom1D_sse2( const TS* pSrc, TD* pDst_0, TD* pDst_1, TD* pDst_2, A32U count ) const
            {
                count;
                TS *pSrcRow1    = const_cast<TS*>( pSrc );
                TS *pSrcRow2    = FW_REF::Offset( pSrcRow1, srcStep );
                TD *pY_DstRow1  = pDst_0;
                TD *pY_DstRow2  = FW_REF::Offset( pY_DstRow1, pDstStep[0] );
                TD *pCb_DstRow  = pDst_1;
                TD *pCr_DstRow  = pDst_2;
                XMM128 reg1, reg2, reg3, reg4, reg5, reg6, mask;
                mask.i = _mm_set1_epi32( 0x00FF00FF );
                // mask = {00,FF,00,FF,00,FF,00,FF,00,FF,00,FF,00,FF,00,FF}

                // Process 16 pixels at a time, and write to 16 Y pixels, and 8 Cb and Cr pixels.
                for( int i=0; i<numBlocks; ++i, FW_REF::Inc<TS>(pSrcRow1,   16 * CH_COUNT(cs) * sizeof(TS)), 
                                                FW_REF::Inc<TS>(pSrcRow2,   16 * CH_COUNT(cs) * sizeof(TS)), 
                                                FW_REF::Inc<TD>(pY_DstRow1, 16 * sizeof(TD)), 
                                                FW_REF::Inc<TD>(pY_DstRow2, 16 * sizeof(TD)), 
                                                FW_REF::Inc<TD>(pCb_DstRow,  8 * sizeof(TD)), 
                                                FW_REF::Inc<TD>(pCr_DstRow,  8 * sizeof(TD)) )
                {

                    // Load 1st Row
                    if( ALIGN_TRUE == ia )
                    {
                        reg1.i = _mm_load_si128( (__m128i*)pSrcRow1 );
                        reg2.i = _mm_load_si128( (__m128i*)(pSrcRow1+16) );
                    }
                    else
                    {
                        reg1.i = _mm_loadu_si128( (__m128i*)pSrcRow1 );
                        reg2.i = _mm_loadu_si128( (__m128i*)(pSrcRow1+16) );
                    }
                    // reg1 = {A7,C3,A6,B3,A5,C2,A4,B2,A3,C1,A2,B1,A1,C0,A0,B0}
                    // reg2 = {Af,C7,Ae,B7,Ad,C6,Ac,B6,Ab,C5,Aa,B5,A9,C4,A8,B4}

                    reg3.i = _mm_srli_si128  ( reg1.i, 1      ); // reg3 = {00,A7,C3,A6,B3,A5,C2,A4,B2,A3,C1,A2,B1,A1,C0,A0}
                    reg3.i = _mm_and_si128   ( reg3.i, mask.i ); // reg3 = {00,A7,00,A6,00,A5,00,A4,00,A3,00,A2,00,A1,00,A0}
                    reg4.i = _mm_srli_si128  ( reg2.i, 1      ); // reg4 = {00,Af,C7,Ae,B7,Ad,C6,Ac,B6,Ab,C5,Aa,B5,A9,C4,A8}
                    reg4.i = _mm_and_si128   ( reg4.i, mask.i ); // reg4 = {00,Af,00,Ae,00,Ad,00,Ac,00,Ab,00,Aa,00,A9,00,A8}
                    reg3.i = _mm_packus_epi16( reg3.i, reg4.i ); // reg3 = {Af,Ae,Ad,Ac,Ab,Aa,A9,A8,A7,A6,A5,A4,A3,A2,A1,A0}
                    // Store Y values contained in reg3
                    if( ALIGN_TRUE == ia )
                    {
                        _mm_stream_si128( (__m128i*)pY_DstRow1, reg3.i );
                    }
                    else
                    {
                        _mm_storeu_si128( (__m128i*)pY_DstRow1, reg3.i );
                    }

                    reg1.i = _mm_and_si128   ( reg1.i, mask.i ); // reg1 = {00,C3,00,B3,00,C2,00,B2,00,C1,00,B1,00,C0,00,B0}
                    reg2.i = _mm_and_si128   ( reg2.i, mask.i ); // reg2 = {00,C7,00,B7,00,C6,00,B6,00,C5,00,B5,00,C4,00,B4}
                    // reg3 and reg4 contain interleaved 16bit unsigned Cr & Cb values

                    // Load 2nd Row
                    if( ALIGN_TRUE == ia )
                    {
                        reg3.i = _mm_load_si128( (__m128i*)pSrcRow2 );
                        reg4.i = _mm_load_si128( (__m128i*)(pSrcRow2+16) );
                    }
                    else
                    {
                        reg3.i = _mm_loadu_si128( (__m128i*)pSrcRow2 );
                        reg4.i = _mm_loadu_si128( (__m128i*)(pSrcRow2+16) );
                    }
                    // reg3 = {A7,C3,A6,B3,A5,C2,A4,B2,A3,C1,A2,B1,A1,C0,A0,B0}
                    // reg4 = {Af,C7,Ae,B7,Ad,C6,Ac,B6,Ab,C5,Aa,B5,A9,C4,A8,B4}

                    reg5.i = _mm_srli_si128  ( reg3.i, 1      ); // reg5 = {00,A7,C3,A6,B3,A5,C2,A4,B2,A3,C1,A2,B1,A1,C0,A0}
                    reg5.i = _mm_and_si128   ( reg5.i, mask.i ); // reg5 = {00,A7,00,A6,00,A5,00,A4,00,A3,00,A2,00,A1,00,A0}
                    reg6.i = _mm_srli_si128  ( reg4.i, 1      ); // reg6 = {00,Af,C7,Ae,B7,Ad,C6,Ac,B6,Ab,C5,Aa,B5,A9,C4,A8}
                    reg6.i = _mm_and_si128   ( reg6.i, mask.i ); // reg6 = {00,Af,00,Ae,00,Ad,00,Ac,00,Ab,00,Aa,00,A9,00,A8}
                    reg5.i = _mm_packus_epi16( reg5.i, reg6.i ); // reg5 = {Af,Ae,Ad,Ac,Ab,Aa,A9,A8,A7,A6,A5,A4,A3,A2,A1,A0}
                    // Store Y values contained in reg5
                    if( ALIGN_TRUE == ia )
                    {
                        _mm_stream_si128( (__m128i*)pY_DstRow2, reg5.i );
                    }
                    else
                    {
                        _mm_storeu_si128( (__m128i*)pY_DstRow2, reg5.i );
                    }

                    reg3.i = _mm_and_si128   ( reg3.i, mask.i ); // reg3 = {00,C3,00,B3,00,C2,00,B2,00,C1,00,B1,00,C0,00,B0}
                    reg4.i = _mm_and_si128   ( reg4.i, mask.i ); // reg4 = {00,C7,00,B7,00,C6,00,B6,00,C5,00,B5,00,C4,00,B4}
                    // reg3 and reg4 contain interleaved 16bit unsigned Cr & Cb values
                      
                    reg3.i = _mm_avg_epu16      ( reg3.i, reg1.i               );
                    // reg3 = {00,C3,00,B3,00,C2,00,B2,00,C1,00,B1,00,C0,00,B0}
                    reg3.i = _mm_shufflelo_epi16( reg3.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg3 = {00,C3,00,B3,00,C2,00,B2,00,C1,00,C0,00,B1,00,B0}
                    reg3.i = _mm_shufflehi_epi16( reg3.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg3 = {00,C3,00,C2,00,B3,00,B2,00,C1,00,C0,00,B1,00,B0}
                    reg3.i = _mm_shuffle_epi32  ( reg3.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg3 = {00,C3,00,C2,00,C1,00,C0,00,B3,00,B2,00,B1,00,B0}
                    reg4.i = _mm_avg_epu16      ( reg4.i, reg2.i               );
                    // reg4 = {00,C7,00,B7,00,C6,00,B6,00,C5,00,B5,00,C4,00,B4}
                    reg4.i = _mm_shufflelo_epi16( reg4.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg4 = {00,C7,00,B7,00,C6,00,B6,00,C5,00,C4,00,B5,00,B4}
                    reg4.i = _mm_shufflehi_epi16( reg4.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg4 = {00,C7,00,C6,00,B7,00,B6,00,C5,00,C4,00,B5,00,B4}
                    reg4.i = _mm_shuffle_epi32  ( reg4.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg4 = {00,C7,00,C6,00,C5,00,C4,00,B7,00,B6,00,B5,00,B4}
                    reg3.i = _mm_packus_epi16   ( reg3.i, reg4.i               );
                    // reg3 = {C7,C6,C5,C4,B7,B6,B5,B4,C3,C2,C1,C0,B3,B2,B1,B0}

                    // Store 8 bytes of Cb
                    reg3.i = _mm_shuffle_epi32  ( reg3.i, _MM_SHUFFLE(3,1,2,0) );
                    // reg3 = {C7,C6,C5,C4,C3,C2,C1,C0,B7,B6,B5,B4,B3,B2,B1,B0}
                    _mm_storel_pd( (double*)pCb_DstRow, reg3.d );

                    // Store 8 bytes of Cr
                    reg4.i = _mm_srli_si128     ( reg3.i, 8                    );
                    // reg3 = {00,00,00,00,00,00,00,00,C7,C6,C5,C4,C3,C2,C1,C0}
                    _mm_storel_pd( (double*)pCr_DstRow, reg4.d );
                }
                if( remWidth>1 )
                {
                    // Calculate the pointer to the end of the last even numbered pixel, 
                    // and don't process the last remaining odd pixel
                    TS* pSrcRow1End = FW_REF::Offset( pSrcRow1, (remWidth-(remWidth&0x1))*sizeof(TS)*CH_COUNT(cs) );

                    for( ; pSrcRow1<pSrcRow1End; pY_DstRow1+=2, pY_DstRow2+=2, 
                                                 ++pCb_DstRow,  ++pCr_DstRow, 
                                                 pSrcRow1+=4,   pSrcRow2+=4 )
                    {
                        if( CMC::CH_C2_422_BACA == chCountS )
                        {
                            //     422       TO       420
                            // ----------        ------  -----------  -----------
                            // |B A C A |   ---> |A A |  | (B+B)/2 |  | (C+C)/2 |
                            // |B A C A |        |A A |  -----------  -----------
                            // ----------        ------
                            *pY_DstRow1       = *(pSrcRow1 + 1);
                            *(pY_DstRow1 + 1) = *(pSrcRow1 + 3);
                            *pY_DstRow2       = *(pSrcRow2 + 1);
                            *(pY_DstRow2 + 1) = *(pSrcRow2 + 3);
                            *pCb_DstRow       = (*pSrcRow1 + *pSrcRow2) / 2;
                            *pCr_DstRow       = (*(pSrcRow1 + 2) + *(pSrcRow2 + 2)) / 2;
                        }
                        else
                            ASSERT(false);
                    }
                }
                _mm_mfence();
            }
        }; // struct ChannelToPlanar
    } // namespace F_422To420
} // namespace OPT_LEVEL
#endif // __FORMATCONVERSION_H__
