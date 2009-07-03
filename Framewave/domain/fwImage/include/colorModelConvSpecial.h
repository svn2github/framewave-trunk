/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORMODELCONVSPECIAL_H__
#define __COLORMODELCONVSPECIAL_H__

#include "colorModelConversion.h"

#define CMC_CHECK_PARAMS    if( !pDst    || !pSrc )                         return fwStsNullPtrErr;    \
                            if( !dstStep || !srcStep )                      return fwStsStepErr;       \
                            if( roiSize.height<=0 || roiSize.width <=0 )    return fwStsSizeErr;


//
// CMC Loop Definitions for chroma sampled functions 
//    e.g. 422, 411, 420 sampled image fns, as well as Most Planar Functions
//

namespace OPT_LEVEL
{
    namespace CMC_SS //(CMC Sub Sampling) ColorModelConvSpecial
    {
        // Pixel format. This enum shows how many pixels and which pixels need to be processed.
        enum CMCProcPxl 
        {                                          // 422/420        411
            A,      // Pixel  A                    //  -----        ---------
            AB,     // Pixels A & B                //  |A|B|        |A|B|C|D|
            AC,     // Pixels A & C                //  |C|D|        ---------
            ABC,    // Pixels A, B, & C            //  -----
            ABCD    // Pixels A, B, C, & D
        };

        enum CMCStep
        {
            STEP_CHANNEL, // Step through the ROI in channel format (ie. RGBRGB...)
            STEP_PLANAR   // Step through the ROI in planar format. Each channel is in its own buffer
                          //   ie. pSrc[0] = RRR...  pSrc[1] = GGG...  pSrc[2] = BBB...
        };

        ///////////////////////////////////
        // Pixel-Processing Functions
        ///////////////////////////////////
        template<  class TS, class TD, pixel_fn pfn, CMC::ChCount chSrc, CMC::ChCount chDst, CMCProcPxl pxl >
        struct Pixel
        {
            SYS_INLINE static void Process( const TS *, void *, TD *, void *, A8U ) 
            { STATIC_ASSERT(false); }
        };

        // Process 3-Channel to 3-Channel - pDst must be A8U due to RGB* formats
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            A8U *pDst,       void *, A8U )
            {
                // Call pixel function and pass in the exact location of the source and destination pointers.
                pfn ( pSrc, C3, pDst, C3);
            }
        };

        // Process 3-Planar to 3-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3, CMC::CH_P3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst[3],             void *, A8U )
            {
                TS tempSrc[3];
                A8U tempDst[3];

                // Convert Planar to Channel format
                tempSrc[0] = *(pSrc[0]);
                tempSrc[1] = *(pSrc[1]);
                tempSrc[2] = *(pSrc[2]);

                pfn( tempSrc, C3, tempDst, C3 );

                // Convert Channel to Planar format
                *(pDst[0]) = tempDst[0];
                *(pDst[1]) = tempDst[1];
                *(pDst[2]) = tempDst[2];
            }
        };

        // Process 3-Planar to 3-Channel - pDst must be A8U due to RGB* formats
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *,
                                            A8U *pDst,                void *, A8U )
            {
                TS tempSrc[3];

                // Convert Planar to Channel format
                tempSrc[0] = *(pSrc[0]);
                tempSrc[1] = *(pSrc[1]);
                tempSrc[2] = *(pSrc[2]);

                pfn( tempSrc, C3, pDst, C3 );
            }
        };

        // Process 3-Channel to 3-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_P3, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst[3],    void *, A8U )
            {
                TD tempDst[3];

                pfn( pSrc, C3, tempDst, C3 );
                
                // Convert Planar to Channel format
                *(pDst[0]) = tempDst[0];
                *(pDst[1]) = tempDst[1];
                *(pDst[2]) = tempDst[2];
            }
        };

        // Process 4-Channel to 4-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_AC4, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U )
            {
                pfn( pSrc, C3, pDst, C3 );
                // Copy the alpha channel from the source
                *((A8U*)pDst + 3) = *(pSrc + 3);
            }
        };

        // Process 4-Planar to 4-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_AP4, CMC::CH_AP4, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[4], void *, 
                                            TD *pDst[4],             void *, A8U )
            {
                TS tempSrc[3];
                TD tempDst[3];

                // Convert Planar to Channel format
                tempSrc[0] = *(pSrc[0]);
                tempSrc[1] = *(pSrc[1]);
                tempSrc[2] = *(pSrc[2]);

                pfn( tempSrc, C3, tempDst, C3 );

                // Convert Channel to Planar format
                *(pDst[0]) = tempDst[0];
                *(pDst[1]) = tempDst[1];
                *(pDst[2]) = tempDst[2];
                // Copy the alpha channel from the source
                *(pDst[3]) = *(pSrc[3]);
            }
        };

        // Process 4-Planar to 4-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_AP4, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst,                void *, A8U )
            {
                TS tempSrc[3];

                // Convert Planar to Channel format
                tempSrc[0] = *(pSrc[0]);
                tempSrc[1] = *(pSrc[1]);
                tempSrc[2] = *(pSrc[2]);
                pfn( tempSrc, C3, (A8U *)pDst, C3 );
                // Copy the alpha channel from the source
                *((A8U *)pDst + 3) = *(pSrc[3]);
            }
        };

        // Process 4-Channel to 4-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_AC4, CMC::CH_AP4, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst[4],    void *, A8U )
            {
                TD tempDst[3];

                pfn( pSrc, C3, tempDst, C3 );
                
                // Convert Channel to Planar format
                *(pDst[0]) = tempDst[0];
                *(pDst[1]) = tempDst[1];
                *(pDst[2]) = tempDst[2];
                // Copy the alpha channel from the source
                *(pDst[3]) = *(pSrc + 3);
            }
        };

        // Process 3-Channel to 2-Channel 422 BACA
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_C2_422_BACA, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U )
            {
                const TS *ptrSrc = pSrc;
                A8U tempDst[3];

                if ((pxl == AB) || (pxl==ABCD)) // Compile time IF
                {
                    pfn( ptrSrc    , C1, ((A8U *)pDst + 1), C1 ); // A0
                    pfn( ptrSrc + 3, C1, ((A8U *)pDst + 3), C1 ); // A1
                    // Calculate the average of the two pixel values for 422 compression
                    TS srcData[3] = { (*ptrSrc       + *(ptrSrc + 3))>>1,   // R
                                      (*(ptrSrc + 1) + *(ptrSrc + 4))>>1,   // G
                                      (*(ptrSrc + 2) + *(ptrSrc + 5))>>1 }; // B
                    pfn( srcData, C3, tempDst, C3 );
                    *(A8U *)pDst       = tempDst[1]; // B0
                    *(A8U *)(pDst + 2) = tempDst[2]; // C0
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( ptrSrc , C2, tempDst, C2 );
                    *(A8U *)pDst       = tempDst[1]; // B0
                    *(A8U *)(pDst + 1) = tempDst[0]; // A0
                }
            }
        };

        // Process 2-Channel 422 BACA to 3-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C2_422_BACA, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U )
            {
                const TS *ptrSrc = pSrc;
                TS tempSrc[3];

                tempSrc[0] = ptrSrc[ 1 ]; // A0
                tempSrc[1] = ptrSrc[ 0 ]; // B0
                
                if( (pxl == AB) || (pxl==ABCD) ) // Process 2 pixels at a time
                {
                    tempSrc[2] = ptrSrc[ 2 ];       // C0

                    pfn( tempSrc, C3, pDst, C3 );   // U0 | V0 | W0

                    tempSrc[0] = ptrSrc[3];         // A1

                    if (GetType<TD>::Index() == GetType<A16U>::Index()) 
                        // Skip 2 bytes in 2 channel, 422's case because each pixel is 2 bytes
                        pfn( tempSrc, C3, pDst + 2, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 3, C3 ); // U1 | V1 | W1
                }
                else    // Process 1 pixel
                {
                    pfn( tempSrc, C3, pDst, C3 ); // U0 | V0 | W0
                }
            }
        };

        // Process 4-Channel to 2-Channel 422 BACA
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_AC4, CMC::CH_C2_422_BACA, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U )
            {
                A8U tempDst[3];
                const TS *ptrSrc = pSrc;
                
                if( (pxl == AB) || (pxl==ABCD) )
                {
                    pfn( ptrSrc    , C1, (pDst + 1 ), C1 );  // A0
                    pfn( ptrSrc + 4, C1, (pDst + 3 ), C1 );  // A1
                    // Calculate the average of the two pixel values for 422 compression
                    TS srcData[3] = { (*ptrSrc     + *(ptrSrc + 4))>>1,   // R
                                      (*(ptrSrc+1) + *(ptrSrc + 5))>>1,   // G
                                      (*(ptrSrc+2) + *(ptrSrc + 6))>>1 }; // B

                    pfn( srcData, C3, tempDst, C3 );

                    *pDst        = tempDst[1];              // B0
                    *(pDst + 2 ) = tempDst[2];              // C0
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( ptrSrc, C2, tempDst, C2 );
                    *pDst       = tempDst[1];               // B0
                    *(pDst + 1) = tempDst[0];               // A0
                }
            }
        };

        // Process 2-Channel 422 BACA to 4-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C2_422_BACA, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U alphaVal )
            {
                const TS *ptrSrc = (TS *)pSrc;
                TS tempSrc[3];

                tempSrc[0] = ptrSrc[ 1 ]; // A0
                tempSrc[1] = ptrSrc[ 0 ]; // B0

                if ((pxl==AB)||(pxl==ABCD))
                {
                    tempSrc[2] = ptrSrc[ 2 ]; // C0

                    pfn( tempSrc, C3, pDst, C3 ); // U0 | V0 | W0

                    *(pDst + 3) = alphaVal;      // Alpha-Channel

                    tempSrc[0] = ptrSrc[3]; // A1

                    pfn( tempSrc, C3, pDst + 4, C3 ); // U1 | V1 | W1

                    *(pDst + 7) = alphaVal;
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( tempSrc, C3, pDst, C3 ); // U0 | V0 | W0
                    *(pDst + 3) = alphaVal;       // Alpha-Channel
                }
            }
        };

        // Process 3-Channel to 2-Channel 422 ABAC
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_C2_422_ABAC, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst,       void *, A8U )
            {
                A8U tempDst[3];
                const TS *ptrSrc = pSrc;
                
                if ((pxl == AB)||(pxl==ABCD))
                {
                    pfn( ptrSrc  , C1, pDst       , C1 );    // A0
                    pfn( ptrSrc+3, C1, (pDst + 2 ), C1 );    // A1
                    // Calculate the average of the two pixel values for 422 compression
                    TS srcData[3] = { (*ptrSrc       + *(ptrSrc + 3))>>1, 
                                      (*(ptrSrc + 1) + *(ptrSrc + 4))>>1,
                                      (*(ptrSrc + 2) + *(ptrSrc + 5))>>1 };

                    pfn( srcData , C3, tempDst, C3 );

                    *(pDst + 1 ) = tempDst[1];              // B0
                    *(pDst + 3 ) = tempDst[2];              // C0
                    
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( ptrSrc, C2, pDst, C2 ); // A0 B0
                }
            }
        };

        // Process 2-Channel 422 ABAC to 3-Channel - pDst must be A8U due to RGB* formats
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C2_422_ABAC, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            A8U *pDst,       void *, A8U )
            {
                const TS *ptrSrc = pSrc;
                TS tempSrc[3];

                tempSrc[0] = ptrSrc[0]; // A0
                tempSrc[1] = ptrSrc[1]; // B0

                if ((pxl == AB)||(pxl==ABCD))    // Process 2 pixels
                {
                    tempSrc[2] = ptrSrc[3]; // C0
                    pfn( tempSrc, C3, pDst, C3 ); // U0 | V0 | W0

                    tempSrc[0] = ptrSrc[2]; // A1
                    if( GetType<TD>::Index() == GetType<A16U>::Index() ) 
                        // Skip 2 bytes in 2 channel, 422's case because each pixel is 2 bytes
                        pfn( tempSrc, C3, pDst + 2, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 3, C3 ); // U1 | V1 | W1
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( tempSrc, C3, pDst, C3 ); // U0 | V0 | W0
                }
            }
        };

        // Process 2-Channel 422 ABAC to 3-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C2_422_ABAC, CMC::CH_P3, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst[3],    void *, A8U )
            {
                const TS *ptrSrc = pSrc;
                TS tempSrc[3];
                A8U tempDst[3];

                tempSrc[0] = ptrSrc[ 0 ]; // A0
                tempSrc[1] = ptrSrc[ 1 ]; // B0

                if ((pxl == AB)||(pxl==ABCD))
                {
                    tempSrc[2] = ptrSrc[ 3 ]; // C0
                    pfn( tempSrc, C3, tempDst, C3); // U0 | V0 | W0

                    *(pDst[0]) = tempDst[0];
                    *(pDst[1]) = tempDst[1];
                    *(pDst[2]) = tempDst[2];

                    tempSrc[0] = ptrSrc[2]; // A1
                    pfn( tempSrc, C3, tempDst, C3 ); // U1 | V1 | W1
                    
                    *(pDst[0]+1) = tempDst[0];
                    *(pDst[1]+1) = tempDst[1];
                    *(pDst[2]+1) = tempDst[2];
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( tempSrc, C3, tempDst, C3); // U0 | V0 | W0

                    *(pDst[0]) = tempDst[0];
                    *(pDst[1]) = tempDst[1];
                    *(pDst[2]) = tempDst[2];
                }
            }
        };

        // Process 3-Planar to 2-Channel 422 ABAC
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3, CMC::CH_C2_422_ABAC, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst,                void *, A8U )
            {
                A8U tempDst[3];
                TS tempSrc[3] = { *(pSrc[0]), 
                                  *(pSrc[1]), 
                                  *(pSrc[2]) };

                if ( (pxl == AB)||(pxl==ABCD) )
                {
                    pfn( tempSrc, C1, pDst, C1 ); // A0
                    
                    tempDst[0] = *(pSrc[0] + 1);
                    tempDst[1] = *(pSrc[1] + 1);
                    tempDst[2] = *(pSrc[2] + 1);
                    pfn( tempDst, C1, (pDst + 2), C1 ); // A1
                    // Calculate the average of the two pixel values for 422 compression
                    tempSrc[0] = (tempSrc[0] + tempDst[0])>>1;
                    tempSrc[1] = (tempSrc[1] + tempDst[1])>>1;
                    tempSrc[2] = (tempSrc[2] + tempDst[2])>>1;
                
                    pfn( tempSrc, C3, tempDst, C3 );
                    *(pDst + 1) = tempDst[1]; // B0
                    *(pDst + 3) = tempDst[2]; // C0
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn( tempSrc, C2, pDst, C2 );
                }
            }
        };

        // Process 3-Planar to 3-Planar 422
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3, CMC::CH_P3_422, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst[3],             void *, A8U )
            {
                A8U tempDst[3];
                TS tempSrc[3] = {  *(pSrc[0]),
                                   *(pSrc[1]),
                                   *(pSrc[2]) };

                if ( (pxl == AB) || (pxl==ABCD) )
                {
                    pfn( tempSrc, C1, pDst[0], C1);   // A0
                    
                    tempDst[0] = *(pSrc[0] + 1);
                    tempDst[1] = *(pSrc[1] + 1);
                    tempDst[2] = *(pSrc[2] + 1);
                    pfn( tempDst, C1, (pDst[0] + 1), C1 ); // A1

                    // Calculate the average of the two pixel values for 422 compression
                    tempSrc[0] = (tempSrc[0] + tempDst[0])>>1;
                    tempSrc[1] = (tempSrc[1] + tempDst[1])>>1;
                    tempSrc[2] = (tempSrc[2] + tempDst[2])>>1;
                    pfn( tempSrc, C3, tempDst, C3);

                    *(pDst[1]) = tempDst[1]; // B0
                    *(pDst[2]) = tempDst[2]; // C0
                }
                else
                {
                    pfn( tempSrc, C3, tempDst, C3 );
                    *(pDst[0]) = tempDst[0]; // A0
                    *(pDst[1]) = tempDst[1]; // B0
                    *(pDst[2]) = tempDst[2]; // C0
                }
            }
        };

        // Process 3-Planar 422 to 3-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_422, CMC::CH_P3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst[3],             void *, A8U )
            {
                TS tempSrc[3];
                A8U tempDst[3];

                tempSrc[0] = *(pSrc[0]); // A0
                tempSrc[1] = *(pSrc[1]); // B0
                tempSrc[2] = *(pSrc[2]); // C0

                pfn( tempSrc, C3, tempDst, C3 );

                *(pDst[0]) = tempDst[0];
                *(pDst[1]) = tempDst[1];
                *(pDst[2]) = tempDst[2];

                if( (pxl == AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = (pSrc[0]+1); // A1

                    pfn( tempSrc, C3, tempDst, C3 );

                    *(pDst[0] + 1) = tempDst[0];
                    *(pDst[1] + 1) = tempDst[1];
                    *(pDst[2] + 1) = tempDst[2];
                }
            }
        };

        // Process 3-Channel to 3-Planar 422
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_P3_422, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *, 
                                            TD *pDst[3],    void *, A8U )
            {
                A8U tempDst[3];
                const TS *ptrSrc = pSrc;

                if( (pxl == AB) || (pxl==ABCD) )
                {
                    pfn( ptrSrc  , C1, pDst[0]    , C1 ); // A0
                    pfn( ptrSrc+3, C1, pDst[0] + 1, C1 ); // A1

                    // Calculate the average of the two pixel values for 422 compression
                    TS srcData[3] = { (*ptrSrc     + *(ptrSrc+3))>>1, 
                                      (*(ptrSrc+1) + *(ptrSrc+4))>>1,
                                      (*(ptrSrc+2) + *(ptrSrc+5))>>1 };
                    
                    pfn (srcData, C3, tempDst, C3);
                    *(pDst[1]) = tempDst[1]; // B0
                    *(pDst[2]) = tempDst[2]; // C0
                }
                else
                {
                    // Process the last odd numbered pixel of a row.
                    pfn (ptrSrc, C3, tempDst, C3);
                    *(pDst[0]) = tempDst[0]; // A0
                    *(pDst[1]) = tempDst[1]; // B0
                    *(pDst[2]) = tempDst[2]; // C0
                }
            }
        };

        // Process 3-Planar 422 to 3-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_422, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const void *pSrc, void *, 
                                            void *pDst,       void *, A8U )
            {
                TS tempSrc[3];

                tempSrc[0] = *(((TS **)pSrc)[0]); // A0
                tempSrc[1] = *(((TS **)pSrc)[1]); // B0
                tempSrc[2] = *(((TS **)pSrc)[2]); // C0

                pfn( tempSrc, C3, (A8U *)pDst, C3);

                if ((pxl == AB)||(pxl==ABCD))
                {
                    tempSrc[0] = *(((TS **)pSrc)[0] + 1); // A1

                    if (GetType<TD>::Index() == GetType<A16U>::Index()) 
                        // Skip 2 bytes in 2 channel, 422's case because each pixel is 2 bytes
                        pfn( tempSrc, C3, reinterpret_cast<A8U *>(pDst) + 2, C3); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, reinterpret_cast<A8U *>(pDst) + 3, C3);
                }
            }
        };

        // Process 3-Planar 422 to 4-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_422, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst,                void *, A8U alphaVal )
            {
                TS tempSrc[3];

                tempSrc[0] = pSrc[0][0]; // A0
                tempSrc[1] = pSrc[1][0]; // B0
                tempSrc[2] = pSrc[2][0]; // C0

                pfn( tempSrc, C3, reinterpret_cast<A8U *>(pDst), C3 );
                *(pDst + 3) = alphaVal;

                if( (pxl == AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = *(((TS **)pSrc)[0] + 1); // A1

                    pfn( tempSrc, C3, reinterpret_cast<A8U *>(pDst) + 4, C3 );
                    *(pDst + 7) = alphaVal;
                }
            }
        };

        // Process 3-Planar 411 to 3-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_411, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst,                void *, A8U )
            {
                // 411
                // -------------   ----   ----
                // |A0|A1|A2|A3|   |B0|   |C0|
                // -------------   ----   ----

                TS tempSrc[3];

                tempSrc[0] = *(pSrc[0]); // A0
                tempSrc[1] = *(pSrc[1]); // B0
                tempSrc[2] = *(pSrc[2]); // C0

                pfn( tempSrc, C3, pDst, C3 );

                if ( (pxl == AB) || (pxl == ABCD) )
                {
                    tempSrc[0] = *(pSrc[0] + 1); // A1

                    if (GetType<TD>::Index() == GetType<A16U>::Index()) 
                        pfn( tempSrc, C3, pDst + 2, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 3, C3 );
                }

                if ( (pxl == ABC) || (pxl == ABCD) )
                {
                    tempSrc[0] = *(pSrc[0] + 2); // A2

                    if (GetType<TD>::Index() == GetType<A16U>::Index()) 
                        pfn( tempSrc, C3, pDst + 4, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 6, C3 );
                }

                if ( pxl == ABCD )
                {
                    tempSrc[0] = *(pSrc[0] + 3); // A3

                    if (GetType<TD>::Index() == GetType<A16U>::Index())
                        pfn( tempSrc, C3, pDst + 6, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 9, C3 );
                }
            }
        };

        // Process 3-Planar 411 to 4-Channel
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_411, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *, 
                                            TD *pDst,                void *, A8U alphaVal )
            {
                // 411
                // -------------   ----   ----
                // |A0|A1|A2|A3|   |B0|   |C0|
                // -------------   ----   ----

                TS tempSrc[3];

                tempSrc[0] = *(pSrc[0]); // A0
                tempSrc[1] = *(pSrc[1]); // B0
                tempSrc[2] = *(pSrc[2]); // C0

                pfn( tempSrc, C3, pDst, C3);
                *(pDst + 3) = alphaVal;

                if ( (pxl==AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = *(pSrc[0] + 1); // A1

                    pfn( tempSrc, C3, pDst + 4, C3 );
                    *(pDst + 7) = alphaVal;
                }

                if ( (pxl==ABC) || (pxl==ABCD) )
                {
                    tempSrc[0] = *(pSrc[0] + 2); // A2

                    pfn( tempSrc, C3, pDst + 8, C3 );
                    *(pDst + 11) = alphaVal;
                }

                if ( pxl==ABCD )
                {
                    tempSrc[0] = *(pSrc[0] + 3); // A3

                    pfn( tempSrc, C3, pDst + 12, C3 );
                    *(pDst + 15) = alphaVal;
                }
            }
        };


        // Process 3-Planar to 3 Planar 420
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3, CMC::CH_P3_420, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *srcStep, 
                                            TD *pDst[3],             void *dstStep, A8U )
            {
                const TS *const src[3] = { pSrc[0],
                                           pSrc[1],
                                           pSrc[2] };
                TD *dst[3] = { pDst[0],
                               pDst[1],
                               pDst[2] };

                A8U tempDst[3];                                          // Temporary array for conversion result
                TS tempSrc[3] = { *(src[0]), *(src[1]), *(src[2]) };    // Src data

                switch (pxl)
                {
                case A:        // Process only 1 pxl
                    {                    
                        pfn( tempSrc, C3, tempDst, C3 );    
                        *(dst[0]) = tempDst[0];
                        *(dst[1]) = tempDst[1];
                        *(dst[2]) = tempDst[2];    
                        break;
                    }
                case AB:    // Process 2 consecutive pxls
                    {
                        pfn( tempSrc, C1, dst[0], C1 );     // A0 - Top-left
                        
                        tempDst[0] = *(src[0]+C1);
                        tempDst[1] = *(src[1]+C1);
                        tempDst[2] = *(src[2]+C1);
                        pfn( tempDst, C1, dst[0]+C1, C1 ); // A1 -Top-right
                            
                        tempSrc[0] = (tempSrc[0]+tempDst[0])>>1;
                        tempSrc[1] = (tempSrc[1]+tempDst[1])>>1;
                        tempSrc[2] = (tempSrc[2]+tempDst[2])>>1;
                        pfn(tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1];  // B0
                        *(dst[2]) = tempDst[2];  // C0
                        break;
                    }
                case AC:    // Process 2 top/down neighboring pxls
                    {
                        pfn( tempSrc, C1, dst[0], C1); // A0 - Top-left
                        
                        tempDst[0] = *(src[0]+((Fw32s*)srcStep)[0]);
                        tempDst[1] = *(src[1]+((Fw32s*)srcStep)[1]);
                        tempDst[2] = *(src[2]+((Fw32s*)srcStep)[2]);
                        pfn( tempDst, C1, dst[0]+((Fw32u *)dstStep)[0], C1);    // A1 - Bottom-left

                        tempSrc[0] = (tempSrc[0]+tempDst[0])>>1;
                        tempSrc[1] = (tempSrc[1]+tempDst[1])>>1;
                        tempSrc[2] = (tempSrc[2]+tempDst[2])>>1;
                        pfn(tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1];                        // B0
                        *(dst[2]) = tempDst[2];                        // C0
                    break;
                    }
                case ABCD:    // Process all 4 pxls
                    {
                        A64S srcCS[3];                        // Src data for chroma-subsampling
                        pfn( tempSrc, C1, dst[0], C1);        // A0 - Top-left

                        tempDst[0] = *(src[0]+C1);
                        tempDst[1] = *(src[1]+C1);
                        tempDst[2] = *(src[2]+C1);
                        pfn( tempDst, C1, dst[0]+C1, C1);    // A1 -Top-right
                        
                        srcCS[0] = tempSrc[0]+tempDst[0];
                        srcCS[1] = tempSrc[1]+tempDst[1];
                        srcCS[2] = tempSrc[2]+tempDst[2];

                        tempSrc[0] = *(src[0]+((Fw32s*)srcStep)[0]);
                        tempSrc[1] = *(src[1]+((Fw32s*)srcStep)[1]);
                        tempSrc[2] = *(src[2]+((Fw32s*)srcStep)[2]);
                        pfn( tempSrc, C1, dst[0]+((Fw32u *)dstStep)[0], C1);    // A1 - Bottom-left

                        tempDst[0] = *(src[0]+((Fw32s*)srcStep)[0]+C1);
                        tempDst[1] = *(src[1]+((Fw32s*)srcStep)[1]+C1);
                        tempDst[2] = *(src[2]+((Fw32s*)srcStep)[2]+C1);
                        pfn( tempDst, C1, dst[0]+((Fw32u *)dstStep)[0]+C1, C1); // A1 - Bottom-left

                        tempSrc[0] = FW_REF::Limits<TS>::Sat((tempSrc[0]+tempDst[0]+srcCS[0])>>2);
                        tempSrc[1] = FW_REF::Limits<TS>::Sat((tempSrc[1]+tempDst[1]+srcCS[1])>>2);
                        tempSrc[2] = FW_REF::Limits<TS>::Sat((tempSrc[2]+tempDst[2]+srcCS[2])>>2);
                        pfn( tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1];        // B0
                        *(dst[2]) = tempDst[2];        // C0

                        break;
                    }
                default: assert(false);
                }
            }
        };

        // Process 3-Channel to 3-Planar 420
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_C3, CMC::CH_P3_420, pxl>
        {
            SYS_INLINE static void Process( const TS *pSrc, void *srcStep, 
                                            TD *pDst[3],    void *dstStep, A8U )
            {
                const TS *ptrSrc = pSrc;
                TD *dst[3] = {  pDst[0],
                                pDst[1],
                                pDst[2] };

                A8U tempDst[3];                                // Temporary array for conversion result
                TS tempSrc[3] = { *ptrSrc, *(ptrSrc+1), *(ptrSrc+2) }; // Src data

                switch (pxl)
                {
                case A:        // Process only 1 pxl
                    {                    
                        pfn( tempSrc, C3, tempDst, C3 );    
                        *(dst[0]) = tempDst[0];
                        *(dst[1]) = tempDst[1];
                        *(dst[2]) = tempDst[2];    
                        break;
                    }
                case AB:    // Process 2 consecutive pxls
                    {
                        pfn( tempSrc, C1, dst[0], C1 );        // A0 - Top-left
                        
                        tempDst[0] = *(ptrSrc+C3);
                        tempDst[1] = *(ptrSrc+C3+1);
                        tempDst[2] = *(ptrSrc+C3+2);
                        pfn( tempDst, C1, dst[0]+C1, C1 );    // A1 -Top-right
                            
                        tempSrc[0] = (tempSrc[0]+tempDst[0])>>1;
                        tempSrc[1] = (tempSrc[1]+tempDst[1])>>1;
                        tempSrc[2] = (tempSrc[2]+tempDst[2])>>1;
                        pfn(tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1];                        // B0
                        *(dst[2]) = tempDst[2];                        // C0
                        break;
                    }
                case AC:    // Process 2 top/down neighboring pxls
                    {
                        pfn( tempSrc, C1, dst[0], C1);        // A0 - Top-left
                        
                        tempDst[0] = *(ptrSrc+*((Fw32s*)srcStep));
                        tempDst[1] = *(ptrSrc+*((Fw32s*)srcStep)+1);
                        tempDst[2] = *(ptrSrc+*((Fw32s*)srcStep)+2);
                        pfn( tempDst, C1, dst[0]+((Fw32u *)dstStep)[0], C1);    // A2 - Bottom-left

                        tempSrc[0] = (tempSrc[0]+tempDst[0])>>1;
                        tempSrc[1] = (tempSrc[1]+tempDst[1])>>1;
                        tempSrc[2] = (tempSrc[2]+tempDst[2])>>1;
                        pfn(tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1];                        // B0
                        *(dst[2]) = tempDst[2];                        // C0
                    break;
                    }
                case ABCD:    // Process all 4 pxls
                    {
                        A64S srcCS[3];                        // Src data for chroma-subsampling
                        pfn( tempSrc, C1, dst[0], C1);        // A0 - Top-left

                        tempDst[0] = *(ptrSrc+C3);
                        tempDst[1] = *(ptrSrc+C3+1);
                        tempDst[2] = *(ptrSrc+C3+2);
                        pfn( tempDst, C1, dst[0]+C1, C1);    // A1 -Top-right
                        
                        srcCS[0] = tempSrc[0]+tempDst[0];
                        srcCS[1] = tempSrc[1]+tempDst[1];
                        srcCS[2] = tempSrc[2]+tempDst[2];

                        tempSrc[0] = *(ptrSrc + *((Fw32s*)srcStep));
                        tempSrc[1] = *(ptrSrc + *((Fw32s*)srcStep) + 1);
                        tempSrc[2] = *(ptrSrc + *((Fw32s*)srcStep) + 2);
                        pfn( tempSrc, C1, dst[0]+((Fw32s*)dstStep)[0], C1);    // A2 - Bottom-left

                        tempDst[0] = *(ptrSrc+*((Fw32s*)srcStep) + C3);
                        tempDst[1] = *(ptrSrc+*((Fw32s*)srcStep) + C3 + 1);
                        tempDst[2] = *(ptrSrc+*((Fw32s*)srcStep) + C3 + 2);
                        pfn( tempDst, C1, dst[0] + ((Fw32s*)dstStep)[0] + C1, C1);    // A3 - Bottom-right

                        tempSrc[0] = FW_REF::Limits<TS>::Sat((tempSrc[0] + tempDst[0] + srcCS[0])>>2);
                        tempSrc[1] = FW_REF::Limits<TS>::Sat((tempSrc[1] + tempDst[1] + srcCS[1])>>2);
                        tempSrc[2] = FW_REF::Limits<TS>::Sat((tempSrc[2] + tempDst[2] + srcCS[2])>>2);
                        pfn( tempSrc, C3, tempDst, C3);

                        *(dst[1]) = tempDst[1]; // B0
                        *(dst[2]) = tempDst[2]; // C0

                        break;
                    }
                default: assert(false);
                }
            }
        };

        // Process 3-Planar 420 to 3-Planar
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_420, CMC::CH_P3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *srcStep, 
                                            TD *pDst[3],             void *dstStep, A8U )
            {
                TS tempSrc[3];
                A8U tempDst[3];

                tempSrc[0] = *(pSrc[0]); // A0
                tempSrc[1] = *(pSrc[1]); // B0
                tempSrc[2] = *(pSrc[2]); // C0

                pfn( tempSrc, C3, tempDst, C3 );

                *(pDst[0]) = tempDst[0]; // U0
                *(pDst[1]) = tempDst[1]; // V0
                *(pDst[2]) = tempDst[2]; // W0

                if ( (pxl==AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = *(pSrc[0]+1); // A1
                    pfn( tempSrc, C3, tempDst, C3 );

                    *(pDst[0]) = tempDst[0]; // U1
                    *(pDst[1]) = tempDst[1]; // V1
                    *(pDst[2]) = tempDst[2]; // W1
                }

                if ( (pxl==AC) || (pxl==ABCD) )
                {
                    tempSrc[0] = *(pSrc[0]+((Fw32s *)srcStep)[0]); // A2
                    pfn( tempSrc, C3, tempDst, C3 );

                    pDst[0][ ((Fw32s*)dstStep)[0] ] = tempDst[0]; // U2
                    pDst[1][ ((Fw32s*)dstStep)[1] ] = tempDst[1]; // V2
                    pDst[2][ ((Fw32s*)dstStep)[2] ] = tempDst[2]; // W2
                }

                if ( pxl==ABCD )
                {
                    tempSrc[0] = pSrc[0][ ((Fw32s *)srcStep)[0]+1 ]; // A3
                    pfn( tempSrc, C3, tempDst, C3 );

                    pDst[0][ ((Fw32s *)dstStep)[0] + 1 ] = tempDst[0]; // U3
                    pDst[1][ ((Fw32s *)dstStep)[1] + 1 ] = tempDst[1]; // V3
                    pDst[2][ ((Fw32s *)dstStep)[2] + 1 ] = tempDst[2]; // W3
                }
            }
        };

        // Process 3-Planar 420 to 3-Channel - Dst Needs to be A8U due to RGB* formats
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_420, CMC::CH_C3, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *srcStep, 
                                            A8U *pDst,               void *dstStep, A8U )
            {
                TS tempSrc[3] = { *(pSrc[0]),    // A0
                                   *(pSrc[1]),    // B0
                                   *(pSrc[2]) };  // C0

                pfn( tempSrc, C3, pDst, C3 );

                if ( (pxl==AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = pSrc[0][1]; // A1
                    if (GetType<TD>::Index() == GetType<A16U>::Index())
                        pfn( tempSrc, C3, pDst + 2, C3 ); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + 3, C3 );
                }

                if ( (pxl==AC) || (pxl==ABCD) )
                {
                    tempSrc[0] = pSrc[0][ ((Fw32s*)srcStep)[0] ]; // A2
                    pfn( tempSrc, C3, pDst + *(Fw32s*)dstStep, C3);
                }

                if (pxl==ABCD)
                {
                    tempSrc[0] = pSrc[0][ ((Fw32s *)srcStep)[0]+1 ]; // A3
                    if (GetType<TD>::Index() == GetType<A16U>::Index())
                        pfn( tempSrc, C3, pDst + *(Fw32s *)dstStep + 2, C3); // Pack (U1 | V1 | W1)
                    else
                        pfn( tempSrc, C3, pDst + *(Fw32s *)dstStep + 3, C3);
                }
            }
        };

        // Process 3-Planar 420 to 4-Channel - Dst Needs to be A8U due to RGB* formats
        template< class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Pixel <TS,TD,pfn, CMC::CH_P3_420, CMC::CH_AC4, pxl>
        {
            SYS_INLINE static void Process( const TS *const pSrc[3], void *srcStep, 
                                            A8U *pDst,               void *dstStep, A8U alphaVal )
            {
                TS tempSrc[3];

                tempSrc[0] = *(pSrc[0]); // A0
                tempSrc[1] = *(pSrc[1]); // B0
                tempSrc[2] = *(pSrc[2]); // C0

                pfn( tempSrc, C3, pDst, C3 );
                pDst[3] = alphaVal;

                if ( (pxl==AB) || (pxl==ABCD) )
                {
                    tempSrc[0] = pSrc[0][1]; // A1
                    pfn( tempSrc, C3, pDst + 4, C3 );
                    pDst[7] = alphaVal;
                }

                if ( (pxl==AC) || (pxl==ABCD) )
                {
                    tempSrc[0] = pSrc[0][ ((Fw32s*)srcStep)[0] ]; // A2
                    pfn( tempSrc, C3, pDst + *(Fw32s*)dstStep, C3 );
                    pDst[ *(Fw32s *)dstStep + 3 ] = alphaVal;
                }

                if (pxl==ABCD)
                {
                    tempSrc[0] = pSrc[0][ ((Fw32s*)srcStep)[0]+1 ]; // A3
                    pfn( tempSrc, C3, pDst + *(Fw32s*)dstStep + 4, C3 );
                    pDst[ *(Fw32s*)dstStep + 7 ] = alphaVal;
                }
            }
        };


        //////////////////////////////////////////////////////////////
        // Block Functions - For inside the inner loop (acts as a layer of 
        //                   overloaded functions with varying parameters for 
        //                   the underlying same pixel functions [img2img vs plnr2plnr)
        //                   Always pass in buffer pointers at UNSIGNED 8-BIT
        //////////////////////////////////////////////////////////////
        template<CMC::ChCount chSrc, CMC::ChCount chDst, class TS, class TD, pixel_fn pfn, 
                  CMCStep ChSrcStep, CMCStep ChDstStep, CMCProcPxl pxl>
        struct Block
        {
            SYS_INLINE static void ProcessBlock( void *, void *, const A8U *, A8U *, A8U ) 
            { 
                STATIC_ASSERT(false); 
            }
        };

        // Channel to Channel Conversion
        template< CMC::ChCount chSrc, CMC::ChCount chDst, class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Block < chSrc, chDst, TS, TD, pfn, STEP_CHANNEL, STEP_CHANNEL, pxl>
        {
            SYS_INLINE static void ProcessBlock( void *srcStep, void *dstStep, 
                                                 const A8U *pSrc,  A8U *pDst,    A8U alphaVal )
            {
                Pixel< TS, TD, pfn,chSrc, chDst, pxl>::Process( pSrc, srcStep, 
                                                                pDst, dstStep, alphaVal );
            }
        };

        // Planar To Planar Conversion
        template< CMC::ChCount chSrc, CMC::ChCount chDst, class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Block < chSrc, chDst, TS, TD, pfn, STEP_PLANAR, STEP_PLANAR, pxl>
        {
            SYS_INLINE static void ProcessBlock( void *srcStep, void *dstStep,
                                                 const A8U* const pSrc[4], A8U* pDst[4], A8U alphaVal )
            {
                Pixel< TS, TD, pfn,chSrc, chDst, pxl>::Process( pSrc, srcStep, 
                                                                pDst, dstStep, alphaVal );
            }
        };

        // Channel to Planar Conversion
        template< CMC::ChCount chSrc, CMC::ChCount chDst, class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Block < chSrc, chDst, TS, TD, pfn, STEP_CHANNEL, STEP_PLANAR, pxl>
        {
            SYS_INLINE static void ProcessBlock( void *srcStep, void *dstStep, 
                                                 const A8U *pSrc, A8U *pDst[3],  A8U alphaVal )
            {
                Pixel< TS, TD, pfn,chSrc, chDst, pxl>::Process( pSrc, srcStep, 
                                                                pDst, dstStep, alphaVal );
            }
        };

        // Planar to Channel Conversion
        template< CMC::ChCount chSrc, CMC::ChCount chDst, class TS, class TD, pixel_fn pfn, CMCProcPxl pxl >
        struct Block < chSrc, chDst, TS, TD, pfn, STEP_PLANAR, STEP_CHANNEL, pxl>
        {
            SYS_INLINE static void ProcessBlock( void *srcStep, void *dstStep, 
                                                 const A8U *const pSrc[3], A8U *pDst, A8U alphaVal )
            {
                Pixel< TS, TD, pfn,chSrc, chDst, pxl>::Process( pSrc, srcStep, 
                                                                pDst, dstStep, alphaVal );
            }
        };
        
        SYS_INLINE int CountCh( CMC::ChCount s)
        {
            switch (s)
            {
                case CMC::CH_AC4:         return 4;
                case CMC::CH_C3:          return 3;
                case CMC::CH_C2_422_ABAC:
                case CMC::CH_C2_422_BACA: return 2;
                case CMC::CH_P3:
                case CMC::CH_AP4:
                case CMC::CH_P3_422:
                case CMC::CH_P3_411:
                case CMC::CH_P3_420:      return 1;
                default:                  return 0;
            }
        }

        template< class TS >
        SYS_INLINE void CalcXOffset( CMC::ChCount ch, int x,  TS *offset[3] )
        {
            offset[0] += x;
            switch (ch)    // Compile-Time switch
            {
                case CMC::CH_P3_422:
                    offset[1] += x/2;   // H: Half-Steps        V: Full-Steps
                    offset[2] += x/2;
                    break;
                case CMC::CH_P3_411:
                    offset[1] += x/4;   // H: Quarter-Steps     V: Full-Steps
                    offset[2] += x/4;
                    break;
                case CMC::CH_P3_420: 
                    offset[1] += x/2;   // H: Half-Steps        V: Half-Steps
                    offset[2] += x/2;
                    break;
                default:
                    offset[1] += x;     // H: Full-Steps        V: Full-Steps
                    offset[2] += x;
            }
        }

        ///////////////////////////////////////////////////////////
        // "Special" Model Conversion Functions - 
        //            Planar and varied stepping functions
        ///////////////////////////////////////////////////////////
        namespace Model
        {
            // -- Channel to Channel --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U *pSrc, int srcStep, 
                                                     A8U *pDst,       int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                const A8U *pSrcRow, *pSrcRowEnd;
                A8U *pDstRow;
                int height = roiSize.height, 
                    width  = roiSize.width;

                // Round down to the nearest even column number;
                if ( (chDst == CMC::CH_C2_422_ABAC) || (chDst == CMC::CH_C2_422_BACA) ||
                     (chSrc == CMC::CH_C2_422_ABAC) || (chSrc == CMC::CH_C2_422_BACA) )
                    width = width - (width&0x1);
                
                // If Odd-number of pixels in a row, omits the last pixel in 4:2:2 and 4:2:0 because it omits every 
                // odd row (e.g. 1, 2, 3, 4 -> 2, 4    and    1, 2, 3, 4, 5 -> 2, 4)
                for( A32S y = 0; y < height; y += VertInterval )
                {
                    pSrcRow    = pSrc    + y * srcStep;
                    pDstRow    = pDst    + y * dstStep;
                    pSrcRowEnd = pSrcRow + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow < pSrcRowEnd; pSrcRow += SrcInterval, 
                                                 pDstRow += DstInterval  )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, AB>::ProcessBlock( &srcStep, &dstStep, 
                                                                                                  pSrcRow,  pDstRow, 
                                                                                                  alphaVal             );
                    }
                    // Process 1 col pixel (if odd number of columns in ROI)
                    if ( (chDst == CMC::CH_C2_422_ABAC) || (chDst == CMC::CH_C2_422_BACA) ||
                         (chSrc == CMC::CH_C2_422_ABAC) || (chSrc == CMC::CH_C2_422_BACA) )
                    {
                        if( roiSize.width&0x1 )    //Process the one column pixel
                        {
                            Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, A>::ProcessBlock( &srcStep, &dstStep, 
                                                                                                     pSrcRow,  pDstRow, 
                                                                                                     alphaVal             );
                        }
                    }
                }
                // Process 1 row (if odd number of rows in ROI)
                if ( (chDst == CMC::CH_C2_422_ABAC) || (chDst == CMC::CH_C2_422_BACA) ||
                     (chSrc == CMC::CH_C2_422_ABAC) || (chSrc == CMC::CH_C2_422_BACA) )
                {
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;
                }

                return fwStsNoErr;
            }

            // -- Planar To Planar : Multiple-srcStep, Multiple-dstStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U* const pSrc[3], int srcStep[3], 
                                                     A8U* pDst[3],             int dstStep[3], 
                                                     FwiSize roiSize,         A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if ( srcStep[0] <= 0 || srcStep[1] <= 0 || srcStep[2] <= 0) return fwStsStepErr;
                if ( dstStep[0] <= 0 || dstStep[1] <= 0 || dstStep[2] <= 0) return fwStsStepErr;

                int height = roiSize.height, 
                     width = roiSize.width;
                const A8U *pSrcRow[3];
                const A8U *pSrcRow1End;
                A8U *pDstRow[3];
                

                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow[0]  = pSrc[0] + y * srcStep[0];
                    pSrcRow[1]  = pSrc[1] + y * srcStep[1];
                    pSrcRow[2]  = pSrc[2] + y * srcStep[2];

                    pDstRow[0]  = pDst[0] + y * dstStep[0];
                    pDstRow[1]  = pDst[1] + y * dstStep[1];
                    pDstRow[2]  = pDst[2] + y * dstStep[2];

                    pSrcRow1End = pSrc[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                     CalcXOffset( chDst, DstInterval, pDstRow ) )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, ABCD>::ProcessBlock( &srcStep, &dstStep, 
                                                                                                    pSrcRow,  pDstRow, 
                                                                                                    alphaVal         );
                    }
                }

                return fwStsNoErr;
            }

            // -- Planar To Planar : Single-srcStep, Multiple-dstStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U* const pSrc[3], int srcStep, 
                                                     A8U* pDst[3],             int dstStep[3], 
                                                     FwiSize roiSize,         A8U alphaVal     )
            {
                CMC_CHECK_PARAMS
                if ( dstStep[0] <= 0 || dstStep[1] <= 0 || dstStep[2] <= 0) return fwStsStepErr;

                const A8U *pSrcRow[3];
                const A8U *pSrcRow1End;
                A8U *pDstRow[3];
                int height = roiSize.height, 
                    width  = roiSize.width;
                int tempSrcStep[3] = { srcStep, srcStep, srcStep };

                switch (chDst)
                {
                    case CMC::CH_P3_420:    
                        height = height - (height & 0x1); // Round down to the nearest even row.
                    case CMC::CH_P3_422:    
                        width  = width  - (width & 0x1);  // Round down to the nearest even column.
                }

                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow[0] = pSrc[0] + y * srcStep;
                    pSrcRow[1] = pSrc[1] + y * srcStep;
                    pSrcRow[2] = pSrc[2] + y * srcStep;

                    pDstRow[0] = pDst[0] + y * dstStep[0];

                    if( CMC::CH_P3_420 == chDst )
                    {
                        pDstRow[1] = pDst[1] + (y>>1) * dstStep[1];
                        pDstRow[2] = pDst[2] + (y>>1) * dstStep[2];
                    }
                    else
                    {
                        pDstRow[1] = pDst[1] + y * dstStep[1];
                        pDstRow[2] = pDst[2] + y * dstStep[2];
                    }

                    pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                     CalcXOffset( chDst, DstInterval, pDstRow ) )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, ABCD>::ProcessBlock( tempSrcStep, dstStep, 
                                                                                                    pSrcRow,     pDstRow, 
                                                                                                    alphaVal         );
                    }
                    // Process 1 col pixel (if odd number of columns in ROI)
                    switch (chDst)
                    {
                        case CMC::CH_P3_420:
                        case CMC::CH_P3_422:
                            if( roiSize.width&0x1 )     //Process the one column pixel
                            {
                                Block< chSrc, chDst, TS, TD, pfn,
                                       ChSrcStep, ChDstStep, AC>::ProcessBlock( tempSrcStep, dstStep, 
                                                                                pSrcRow,     pDstRow, 
                                                                                alphaVal         );
                            }
                    }
                }
                // Process 1 row
                if (chDst == CMC::CH_P3_420)
                {
                    if( roiSize.height&0x1 )
                    {
                        // Update the src & dst pointers, independent of the ROI
                        pSrcRow[0]  = pSrc[0] + height * srcStep;
                        pSrcRow[1]  = pSrc[1] + height * srcStep;
                        pSrcRow[2]  = pSrc[2] + height * srcStep;

                        pDstRow[0]  = pDst[0] + height * dstStep[0];
                        pDstRow[1]  = pDst[1] + (height>>1) * dstStep[1];
                        pDstRow[2]  = pDst[2] + (height>>1) * dstStep[2];

                        pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                        for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                         CalcXOffset( chDst, DstInterval, pDstRow ) )
                        {
                            Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, AB>::ProcessBlock( tempSrcStep, dstStep, 
                                                                                                      pSrcRow,     pDstRow, 
                                                                                                      alphaVal         );
                        }
                        // Process 1 col pixel (if odd number of columns in ROI)
                        if( roiSize.width&0x1 )    //Process the one column pixel
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, A >::ProcessBlock( tempSrcStep, dstStep, 
                                                                            pSrcRow,     pDstRow, alphaVal );
                            return fwStsDoubleSize;
                        }
                        return fwStsDoubleSize;
                    }
                }
                else if (chDst == CMC::CH_P3_422)
                {
                    if( roiSize.width&0x1 )
                        return fwStsDoubleSize;
                }
                return fwStsNoErr;
            }

            // -- Planar To Planar : Multiple-srcStep, Single-dstStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const TS* const pSrc[3], int srcStep[3], 
                                                     TD* pDst[3],             int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if ( srcStep[0] <= 0 || srcStep[1] <= 0 || srcStep[2] <= 0) return fwStsStepErr;

                const A8U *pSrcRow[3];
                const A8U *pSrcRow1End;
                A8U *pDstRow[3];
                int height = roiSize.height, 
                    width  = roiSize.width;
                int tempDstStep[3] = { dstStep, dstStep, dstStep };

                switch (chDst)
                {
                    case CMC::CH_P3_420:    
                        height = height - (height & 0x1); // Round down to the nearest even row.
                    case CMC::CH_P3_422:    
                        width  = width  - (width & 0x1);  // Round down to the nearest even column.
                }


                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow[0] = pSrc[0] + y * srcStep[0];
                    pSrcRow[1] = pSrc[1] + y * srcStep[1];
                    pSrcRow[2] = pSrc[2] + y * srcStep[2];

                    pDstRow[0] = pDst[0] + y * dstStep;
                    if( CMC::CH_P3_420 == chDst )
                    {
                        pDstRow[1] = pDst[1] + (y>>1) * dstStep;
                        pDstRow[2] = pDst[2] + (y>>1) * dstStep;
                    }
                    else
                    {
                        pDstRow[1] = pDst[1] + y * dstStep;
                        pDstRow[2] = pDst[2] + y * dstStep;
                    }

                    pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                     CalcXOffset( chDst, DstInterval, pDstRow ) )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD >::ProcessBlock( srcStep, tempDstStep, 
                                                                           pSrcRow, pDstRow, 
                                                                           alphaVal         );
                    }
                    if ( (chSrc == CMC::CH_P3_422) || (chSrc == CMC::CH_P3_420) )
                    {
                        if (roiSize.width&0x1)
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AC >::ProcessBlock( srcStep, tempDstStep, 
                                                                             pSrcRow, pDstRow, 
                                                                             alphaVal         );
                        }
                    }
                }
                if ( chSrc == CMC::CH_P3_420 )
                {
                    if( roiSize.height&0x1 )
                    {
                        // Update the src & dst pointers, independent of the ROI
                        pSrcRow[0]  = pSrc[0]    + height * srcStep[0];
                        pSrcRow[1]  = pSrc[1]    + (height>>1) * srcStep[1];
                        pSrcRow[2]  = pSrc[2]    + (height>>1) * srcStep[2];

                        pDstRow[0]  = pDst[0]    + height * dstStep;
                        pDstRow[1]  = pDst[1]    + height * dstStep;
                        pDstRow[2]  = pDst[2]    + height * dstStep;

                        pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                        for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                         CalcXOffset( chDst, DstInterval, pDstRow ) )
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AB >::ProcessBlock( srcStep, tempDstStep, 
                                                                             pSrcRow, pDstRow, 
                                                                             alphaVal         );
                        }
                        if( roiSize.width&0x1 )
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, A >::ProcessBlock( srcStep, tempDstStep, 
                                                                            pSrcRow, pDstRow, 
                                                                            alphaVal         );
                            return fwStsDoubleSize;
                        }
                        return fwStsDoubleSize;
                    }                                    
                }
                else if (chSrc == CMC::CH_P3_422)
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;
                    
                return fwStsNoErr;
            }

            // -- Planar To Planar : Single-srcStep, Single-dstStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U *const pSrc[4], int srcStep, 
                                                     A8U *pDst[4],             int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if ( srcStep <= 0 || dstStep <= 0 ) return fwStsStepErr;

                const A8U *pSrcRow[4];
                const A8U *pSrcRow1End;
                A8U *pDstRow[4];
                int height = roiSize.height, 
                    width  = roiSize.width;
                int tempSrcStep[4] = { srcStep, srcStep, srcStep, srcStep };
                int tempDstStep[4] = { dstStep, dstStep, dstStep, dstStep };

                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow[0]  = pSrc[0]    + y * srcStep;
                    pSrcRow[1]  = pSrc[1]    + y * srcStep;
                    pSrcRow[2]  = pSrc[2]    + y * srcStep;
                    pSrcRow[3]  = pSrc[3]    + y * srcStep;

                    pDstRow[0]  = pDst[0]    + y * dstStep;
                    pDstRow[1]  = pDst[1]    + y * dstStep;
                    pDstRow[2]  = pDst[2]    + y * dstStep;
                    pDstRow[3]  = pDst[3]    + y * dstStep;

                    pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                     CalcXOffset( chDst, DstInterval, pDstRow ),
                                                     pDstRow[3]+=DstInterval,
                                                     pSrcRow[3]+=SrcInterval)
                    {
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD>::ProcessBlock( tempSrcStep, tempDstStep, 
                                                                          pSrcRow,     pDstRow, 
                                                                          alphaVal              );
                    }
                }
                return fwStsNoErr;
            }

            // -- Channel To Planar : Multiple-dstStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U *pSrc, int srcStep, 
                                                     A8U *pDst[3],    int dstStep[3], FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if ( srcStep <= 0 || dstStep[0] <= 0 || dstStep[1] <= 0 || dstStep[2] <= 0) 
                    return fwStsStepErr;

                const A8U *pSrcRow;
                const A8U *pSrcRowEnd;
                A8U *pDstRow[3];
                int height = roiSize.height, 
                    width  = roiSize.width;

                switch (chDst)
                {
                    case CMC::CH_P3_420:    
                        height = height - (height&0x1);         // Round down to the nearest even row.
                    case CMC::CH_P3_422:
                        width  = width  - (width&0x1);    // Round down to the nearest even column number;
                }

                for( A32S y = 0; y < height; y += VertInterval )
                {
                    pSrcRow    = pSrc    + y * srcStep;

                    pDstRow[0] = pDst[0] + y * dstStep[0];
                    if( CMC::CH_P3_420 == chDst )
                    {
                        pDstRow[1] = pDst[1] + (y>>1) * dstStep[1];
                        pDstRow[2] = pDst[2] + (y>>1) * dstStep[2];
                    }
                    else
                    {
                        pDstRow[1] = pDst[1] + y * dstStep[1];
                        pDstRow[2] = pDst[2] + y * dstStep[2];
                    }
                    
                    pSrcRowEnd = pSrcRow + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow < pSrcRowEnd; pSrcRow += SrcInterval,    // RGB incr by 3*2 bytes, YUV incr by 1 pixel 
                                                 CalcXOffset( chDst, DstInterval, pDstRow )  )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD >::ProcessBlock( 
                                                                           &srcStep, dstStep, 
                                                                           pSrcRow,  pDstRow, 
                                                                           alphaVal         );
                    }
                    // Process 1 col pixel (if odd number of columns in ROI)
                    switch (chDst)
                    {
                        case CMC::CH_P3_420:
                        case CMC::CH_P3_422:
                            if( roiSize.width&0x1 )    //Process the one column pixel
                            {
                                Block< chSrc, chDst, TS, TD, pfn,
                                       ChSrcStep, ChDstStep, AC>::ProcessBlock( &srcStep, dstStep, 
                                                                                pSrcRow,  pDstRow, 
                                                                                alphaVal );
                            }
                    }
                }
                // Process 1 row (if odd number of rows in ROI)
                if (chDst == CMC::CH_P3_420)
                {
                    if (roiSize.height&0x1)
                    {
                        // Update the src & dst pointers, independent of the ROI
                        pSrcRow    = pSrc    + height * srcStep;
                        
                        pDstRow[0] = pDst[0] + height * dstStep[0];
                        pDstRow[1] = pDst[1] + (height>>1) * dstStep[1];
                        pDstRow[2] = pDst[2] + (height>>1) * dstStep[2];

                        pSrcRowEnd = pSrcRow + width * CMC_ChannelCount(chSrc);

                        for( ; pSrcRow < pSrcRowEnd; pSrcRow += SrcInterval,    // RGB incr by 3*2 bytes, YUV incr by 1 pixel 
                                                     CalcXOffset( chDst, DstInterval, pDstRow )  )
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AB >::ProcessBlock( &srcStep, dstStep, 
                                                                             pSrcRow,  pDstRow, 
                                                                             alphaVal         );
                        }
                        // Process 1 col pixel (if odd number of columns in ROI)
                        if (roiSize.width&0x1)    //Process the one column pixel
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, A >::ProcessBlock( &srcStep, dstStep, 
                                                                            pSrcRow,  pDstRow, 
                                                                            alphaVal         );
                            return fwStsDoubleSize;
                        }
                        return fwStsDoubleSize;
                    }
                }
                else if (chDst == CMC::CH_P3_422)
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;            

                return fwStsNoErr;
            }
            // -- Channel To Planar : Single-dstStep (Planar might be 4-channel) --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF( const A8U *pSrc, int srcStep, 
                                                    A8U *pDst[4],    int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if ( srcStep <= 0 || dstStep <= 0 ) return fwStsStepErr;

                const A8U *pSrcRow;
                const A8U *pSrcRowEnd;
                A8U *pDstRow[4];
                int height = roiSize.height, 
                    width  = roiSize.width;
                int tempDstStep[4] = { dstStep, dstStep, dstStep };

                if ( (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) )
                    width = width - (width&0x1);

                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow    = pSrc    + y * srcStep;

                    pDstRow[0] = pDst[0] + y * dstStep;
                    pDstRow[1] = pDst[1] + y * dstStep;
                    pDstRow[2] = pDst[2] + y * dstStep;
                    pDstRow[3] = pDst[3] + y * dstStep;

                    pSrcRowEnd = pSrcRow + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow < pSrcRowEnd; pSrcRow += SrcInterval,
                                                 pDstRow[0] += DstInterval,
                                                 pDstRow[1] += DstInterval,
                                                 pDstRow[2] += DstInterval,
                                                 pDstRow[3] += DstInterval  )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD>::ProcessBlock( &srcStep, tempDstStep, 
                                                                          pSrcRow,  pDstRow, 
                                                                          alphaVal             );
                    }
                    if ( (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) ) // Compile Time Switch
                    {
                        if (roiSize.width&0x1)
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AC >::ProcessBlock( &srcStep, tempDstStep, 
                                                                             pSrcRow,  pDstRow, 
                                                                             alphaVal           );
                        }
                    }
                }

                if ( (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) )
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;

                return fwStsNoErr;
            }

            // -- Planar To Channel : Multiple-srcStep --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const TS* const pSrc[3], int srcStep[3], 
                                                     A8U* pDst,               int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if( srcStep[0] <= 0 || srcStep[1] <= 0 || srcStep[2] <= 0 ) 
                {
                    return fwStsStepErr;
                }

                const TS *pSrcRow[3];
                const A8U *pSrcRow1End;
                A8U *pDstRow;
                int height = roiSize.height, 
                    width  = roiSize.width;

                if ( chSrc == CMC::CH_P3_411 )
                {
                    width = width - (width&0x3);
                }
                else if ( (chSrc == CMC::CH_P3_420) || (chSrc == CMC::CH_P3_422) )
                {
                    width = width - (width&0x1);
                }

                if ( chSrc == CMC::CH_P3_420 )
                {
                    height = height - (height&0x1);
                }
                
                for( A32S y = 0; y < height; y += VertInterval )
                {
                    pSrcRow[0] = pSrc[0] + y * srcStep[0];
                    if( CMC::CH_P3_420 == chSrc )
                    {
                        pSrcRow[1] = pSrc[1] + (y>>1) * srcStep[1];
                        pSrcRow[2] = pSrc[2] + (y>>1) * srcStep[2];
                    }
                    else
                    {
                        pSrcRow[1] = pSrc[1] + y * srcStep[1];
                        pSrcRow[2] = pSrc[2] + y * srcStep[2];
                    }

                    pDstRow    = pDst + y * dstStep;
                    
                    pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                     pDstRow += DstInterval )
                    {
                        // Processes 4 Pixels at once.
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD >::ProcessBlock( srcStep, &dstStep, 
                                                                           pSrcRow, pDstRow, 
                                                                           alphaVal         );
                    }
                    if( (chSrc == CMC::CH_P3_420) || (chSrc == CMC::CH_P3_422) || (chSrc == CMC::CH_P3_411) )
                    {
                        // if the roiSize.width is an odd number, process the one last pixel 
                        // (ie.  width = 5, process the 5th pixel)
                        if( width < roiSize.width )
                        {
                            if( (chSrc == CMC::CH_P3_420) || (chSrc == CMC::CH_P3_422) )
                                Block< chSrc, chDst, TS, TD, pfn,
                                       ChSrcStep, ChDstStep, AC >::ProcessBlock( srcStep, &dstStep, 
                                                                                 pSrcRow, pDstRow, 
                                                                                 alphaVal         );
                            else // chSrc == CMC::CH_P3_411
                            {
                                switch( roiSize.width&0x3 )
                                {
                                    case 1: 
                                        Block< chSrc, chDst, TS, TD, pfn,
                                               ChSrcStep, ChDstStep, A>::ProcessBlock( srcStep, &dstStep, 
                                                                                       pSrcRow, pDstRow, 
                                                                                       alphaVal ); 
                                        break;
                                    case 2: 
                                        Block< chSrc, chDst, TS, TD, pfn,
                                               ChSrcStep, ChDstStep, AB>::ProcessBlock( srcStep, &dstStep,
                                                                                        pSrcRow, pDstRow, 
                                                                                        alphaVal );  
                                        break;
                                    case 3: 
                                        Block< chSrc, chDst, TS, TD, pfn,
                                               ChSrcStep, ChDstStep, ABC>::ProcessBlock( srcStep, &dstStep,
                                                                                         pSrcRow, pDstRow, 
                                                                                         alphaVal );  
                                        break;
                                } // end switch
                            } // end else
                        } // end if
                    } // end if 
                } // end for loop

                if ( chSrc == CMC::CH_P3_420 )
                {
                    if (roiSize.height&0x1)
                    {
                        // Update the src & dst pointers, independent of the ROI
                        pDstRow     = pDst       + height * dstStep;
                        pSrcRow[0]  = pSrc[0]    + height * srcStep[0];
                        pSrcRow[1]  = pSrc[1]    + (height>>1) * srcStep[1];
                        pSrcRow[2]  = pSrc[2]    + (height>>1) * srcStep[2];
                        pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                        for( ; pSrcRow[0] < pSrcRow1End; CalcXOffset( chSrc, SrcInterval, pSrcRow ),
                                                         pDstRow += DstInterval )
                        {
                            // Processes 2 Pixels at once!
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AB >::ProcessBlock( srcStep, &dstStep, 
                                                                             pSrcRow, pDstRow, 
                                                                             alphaVal           );
                        }
                        if ( roiSize.width&0x1 )
                        {
                            // Processes 1 Pixels at once!
                            Block< chSrc, chDst, TS, TD, pfn,ChSrcStep, ChDstStep, A >::ProcessBlock( srcStep, &dstStep, 
                                                                                                      pSrcRow, pDstRow, 
                                                                                                      alphaVal           );
                            return fwStsDoubleSize;
                        }
                        return fwStsDoubleSize;
                    }
                }
                else if ( chSrc == CMC::CH_P3_411 )
                {
                    if (roiSize.width&0x3)
                        return fwStsDoubleSize;
                }
                else if ( chSrc == CMC::CH_P3_422 )
                {
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;
                }
                return fwStsNoErr;
            }

            // -- Planar To Channel : Single-srcStep (Planar might be 4-channel) --
            template <CMCStep ChSrcStep, CMCStep ChDstStep, Fw32u VertInterval, Fw32s SrcInterval, Fw32s DstInterval,
                            CMC::ChCount chSrc, CMC::ChCount chDst,
                            class TS, class TD, pixel_fn pfn>
            SYS_INLINE static FwStatus ConvertREF ( const A8U* const pSrc[4], int srcStep, 
                                                     A8U* pDst,                int dstStep, FwiSize roiSize, A8U alphaVal )
            {
                CMC_CHECK_PARAMS
                if( srcStep <= 0 || dstStep <= 0 ) 
                {
                    return fwStsStepErr;
                }

                const A8U *pSrcRow[4];
                const A8U *pSrcRow1End;
                A8U *pDstRow;
                int height = roiSize.height, 
                    width  = roiSize.width;
                A32S tempSrcStep[4] = {srcStep, srcStep, srcStep, srcStep};

                // Round down to the nearest even column number if 422 chroma-sampled
                if ( (chDst==CMC::CH_C2_422_ABAC) || (chDst==CMC::CH_C2_422_BACA) || 
                     (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) )
                    width = width - (width&0x1); 
                    
                for( A32S y = 0; y < height; y += VertInterval )
                {
                    // Update the src & dst pointers, independent of the ROI
                    pSrcRow[0]  = pSrc[0]    + y * srcStep;
                    pSrcRow[1]  = pSrc[1]    + y * srcStep;
                    pSrcRow[2]  = pSrc[2]    + y * srcStep;
                    pSrcRow[3]  = pSrc[3]    + y * srcStep;
                    pDstRow     = pDst       + y * dstStep;
                    pSrcRow1End = pSrcRow[0] + width * CMC_ChannelCount(chSrc);

                    for( ; pSrcRow[0] < pSrcRow1End; pSrcRow[0] += SrcInterval, 
                                                     pSrcRow[1] += SrcInterval,
                                                     pSrcRow[2] += SrcInterval,
                                                     pSrcRow[3] += SrcInterval,
                                                     pDstRow    += DstInterval )
                    {
                        Block< chSrc, chDst, TS, TD, pfn,
                               ChSrcStep, ChDstStep, ABCD >::ProcessBlock( tempSrcStep, &dstStep, 
                                                                           pSrcRow, pDstRow, 
                                                                           alphaVal             );
                    }
                    if ( (chDst==CMC::CH_C2_422_ABAC) || (chDst==CMC::CH_C2_422_BACA) ||
                         (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) )
                    {
                        if( roiSize.width&0x1 )
                        {
                            Block< chSrc, chDst, TS, TD, pfn,
                                   ChSrcStep, ChDstStep, AC >::ProcessBlock( tempSrcStep, &dstStep,  
                                                                             pSrcRow, pDstRow, 
                                                                             alphaVal             );
                        }
                    }
                }

                if ( (chDst==CMC::CH_C2_422_ABAC) || (chDst==CMC::CH_C2_422_BACA) ||
                     (chSrc==CMC::CH_C2_422_ABAC) || (chSrc==CMC::CH_C2_422_BACA) )
                {
                    if (roiSize.width&0x1)
                        return fwStsDoubleSize;
                }

                return fwStsNoErr;
            }
        }; // Model
    }; // CMC_SS
}; // OPT_LEVEL

#endif // __COLORMODELCONVSPECIAL_H__
