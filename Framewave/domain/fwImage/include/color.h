/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLOR_H__
#define __COLOR_H__
 
#include "fwdev.h"
#include "colorModelConversion.h"
#include <vector>

#pragma warning( disable: 4328 )
// warning C4328: indirection alignment of formal parameter 1 (16) is greater than the actual argument alignment (8)

#define BYTES(x)     x*sizeof(unsigned char)

namespace OPT_LEVEL
{
    // RGB C3 -> YCbCr P3
    static FwStatus ConvertRGBToYCbCr420_8u_C3P3R_REF_2D( const A8U *pSrc, int srcStep, 
                                                           A8U *pDst[3],    int dstStep[3], FwiSize roiSize  );
    static void      ConvertRGBToYCbCr420_8u_C3P3R_REF_1D( const A8U *pSrc, int srcStep, 
                                                           A8U *pDst[3],    int dstStep_Y, int width, int row );

    static void      ConvertRGBToYCbCr420_8u_C3P3R_SSE_1D( const A8U *pSrc, int srcStep, 
                                                           A8U *pDst[3],    int dstStep_Y, int width, int row );


    // BGR C3 -> YCbCr P3
    static FwStatus ConvertBGRToYCbCr420_8u_C3P3R_REF_2D( const A8U *pSrc, int srcStep, 
                                                           A8U *pDst[3],    int dstStep[3], FwiSize roiSize  );
    static void      ConvertBGRToYCbCr420_8u_C3P3R_REF_1D( const A8U *pSrc, int srcStep, 
                                                           A8U *pDst[3],    int dstStep_Y, int width, int row );

    // BGR AC4 -> YCbCr P3
    static FwStatus ConvertBGRToYCbCr420_8u_AC4P3R_REF_2D( const A8U *pSrc, int srcStep, 
                                                            A8U *pDst[3],    int dstStep[3], FwiSize roiSize  );
    static void      ConvertBGRToYCbCr420_8u_AC4P3R_REF_1D( const A8U *pSrc, int srcStep, 
                                                            A8U *pDst[3],    int dstStep_Y, int width, int row );

    /////////////////////////////////
    // RGB C3 -> YCbCr 4:2:0 P3
    /////////////////////////////////
    static SYS_INLINE FwStatus ConvertRGBToYCbCr420_8u_C3P3R_REF_2D( const A8U *pSrc,    int srcStep, 
                                                                      A8U *const pDst[3], int dstStep[3], FwiSize roiSize )
    {
        A8U *pSrcEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};
        int rem = roiSize.height&0x1;

        if (rem)
        {
            pSrcEnd = FW_REF::Offset( (A8U *)pSrc, (roiSize.height-1) * srcStep );
        }
        else
        {
            pSrcEnd = FW_REF::Offset( (A8U *)pSrc, roiSize.height * srcStep );
        }

        // Process every 2 rows at a time
        for ( ; pSrc < pSrcEnd; FW_REF::Inc(pSrc,   2 * srcStep), 
                                FW_REF::Inc(dst[0], 2 * dstStep[0]), 
                                FW_REF::Inc(dst[1], dstStep[1]),  
                                FW_REF::Inc(dst[2], dstStep[2]) )
        {
            switch( Dispatch::Type<DT_SSE2>() )
            {
                case DT_SSE3:
                case DT_SSE2:   ConvertRGBToYCbCr420_8u_C3P3R_SSE_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 2 ); break;
                	
                default:
                                ConvertRGBToYCbCr420_8u_C3P3R_REF_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 2 );
            	
            }
        }

        if ( (roiSize.width|roiSize.height)&0x1 )
        {
            return fwStsDoubleSize;
        }

        return fwStsNoErr;
    }

    static SYS_INLINE void ConvertRGBToYCbCr420_8u_C3P3R_REF_1D (const A8U *pSrc, int srcStep, 
                                                                 A8U *pDst[3],    int dstStep_Y, int width, int row)
    {
        A8U *pSrcRowEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};
        A8U *dstY = dst[0], *src = (A8U *)pSrc;
        double rSum = 0, gSum = 0, bSum = 0;                            // Running sum of RGB values per block

        int rem = width&0x1;
        pSrcRowEnd = FW_REF::Offset((A8U*)pSrc, (width-rem)*BYTES(3)); // rem = 0 or 1.

        if (row > 1) // Condition: 1) # of rows = 2
        {
            // 2x2px block process: 420 Compression
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                // Compute Y
                // Y0
                rSum = src[0]; 
                gSum = src[1]; 
                bSum = src[2];
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                FW_REF::Inc(src, BYTES(3));
                // Y1
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];
                
                // Next Row
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                // Y2
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];
                FW_REF::Inc(src, BYTES(3));

                // Y3
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 4 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 4 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
            if (rem) // Process the extra column if odd number of columns
            {
                rSum = src[0];
                gSum = src[1];
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
        }
        else // row = 1
        {
            // 1x2px block process: 420 Compression (MPEG-1)
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                rSum = src[0]; 
                gSum = src[1]; 
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                FW_REF::Inc(src, BYTES(3));
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
            if (rem) // Process the extra column if even number of columns
            {
                rSum = src[0]; 
                gSum = src[1]; 
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( (double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                     (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                     (double)(gSum) * CONST_RGB2YCbCr_CbG + 
                                                     (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( (double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                     (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                     (double)(bSum) * CONST_RGB2YCbCr_CrB +
                                                     (double)CONST_RGB2YCbCraddCr );
            }
        }
    }

  static SYS_INLINE void ConvertRGBToYCbCr420_8u_C3P3R_SSE_1D (const A8U *pSrc, int srcStep, 
                                                                 A8U *pDst[3],    int dstStep_Y, int width, int row)
    {
        A8U *pSrcRowEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};
        A8U *dstY = dst[0], *src = (A8U *)pSrc;
        double rSum = 0, gSum = 0, bSum = 0;                            // Running sum of RGB values per block

        int rem = width&0x1;
        pSrcRowEnd = FW_REF::Offset((A8U*)pSrc, (width-rem)*BYTES(3)); // rem = 0 or 1.

// Y 66	129	25	
//Cb 38	74	112	 9 19 28
//Cr 112 94 18   28 24 4 

        static const __m128i rgbtoYcbcr_Y = CONST_SET_16I(25,129,66,0,0,25,129,66);
        static const __m128i rgbtoYcbcr_crcb = CONST_SET_16I(-4,-24,28,0,0,28,-19,-9);
        static const __m128i const128 = CONST_SET_16I(0,0,0,128,128,0,0,0);
        static const __m128i const16 = CONST_SET_16I(0,16,0,16,0,16,0,16);
        static const __m128i addconst128 = CONST_SET_16I(0,0,0,128,0,0,0,128);
        static const __m128i zero = CONST_SET1_32I(0);

        if (row > 1) // Condition: 1) # of rows = 2
        {
            // 2x2px block process: 420 Compression
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                // Compute Y
                // Y0 Y1

                XMM128 imRes,imRes1,res1,rgbSum1,mSrc,mSrc1;
                XMM128 rgbSum;
                mSrc.i  = _mm_cvtsi32_si128(*((int*)(src)));
                mSrc1.i = _mm_cvtsi32_si128(*((int*)(src+2)));
                mSrc.i = _mm_unpacklo_epi32( mSrc.i, mSrc1.i );
	            mSrc.i = _mm_unpacklo_epi8( mSrc.i, zero ); 
                rgbSum =  mSrc;
                mSrc.i = _mm_mullo_epi16(mSrc.i,rgbtoYcbcr_Y);
                mSrc.i = _mm_or_si128(mSrc.i,const128);
                res1.i = _mm_srli_epi64(mSrc.i,32);
                mSrc.i = _mm_add_epi16(mSrc.i, res1.i);
                res1.i = _mm_srli_epi32(mSrc.i,16);
                imRes.i = _mm_add_epi16(mSrc.i, res1.i);

                    // Next Row
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                // Y2 Y3
                mSrc.i  = _mm_cvtsi32_si128(*((int*)(src)));
                mSrc1.i = _mm_cvtsi32_si128(*((int*)(src+2)));
                mSrc.i = _mm_unpacklo_epi32( mSrc.i, mSrc1.i );
	            mSrc.i = _mm_unpacklo_epi8( mSrc.i, zero ); 
                rgbSum.i = _mm_add_epi16(rgbSum.i,mSrc.i);
                mSrc.i = _mm_mullo_epi16(mSrc.i,rgbtoYcbcr_Y);
                mSrc.i = _mm_or_si128(mSrc.i,const128);
                res1.i = _mm_srli_epi64(mSrc.i,32);
                mSrc.i = _mm_add_epi16(mSrc.i, res1.i);
                res1.i = _mm_srli_epi32(mSrc.i,16);
                imRes1.i = _mm_add_epi16(mSrc.i, res1.i);

                imRes.f = _mm_shuffle_ps(imRes.f,imRes1.f,_MM_SHUFFLE(2,0,2,0));   
                imRes.i = _mm_srai_epi16(imRes.i,8);
                imRes.i = _mm_add_epi16(imRes.i, const16);

               *dstY = (U8) _mm_extract_epi16(imRes.i,0);
                *(dstY +1) = (U8)  _mm_extract_epi16(imRes.i,2);
                FW_REF::Inc(dstY, dstStep_Y);
                *dstY = (U8) _mm_extract_epi16(imRes.i,4);
                *(dstY+1) = (U8) _mm_extract_epi16(imRes.i,6);



                // Compute Cb, Cr
                rgbSum1.i = _mm_srli_si128(rgbSum.i,10);
                rgbSum.i = _mm_add_epi16(rgbSum.i, rgbSum1.i);
                rgbSum.i = _mm_shuffle_epi32(rgbSum.i,_MM_SHUFFLE(1,0,1,0));
                rgbSum.i = _mm_shufflehi_epi16(rgbSum.i,_MM_SHUFFLE(2,1,0,3));
                imRes.i = _mm_mullo_epi16(rgbSum.i,rgbtoYcbcr_crcb);
            //    imRes.i = _mm_or_si128(imRes.i,const128);
                res1.i = _mm_srli_epi64(imRes.i,32);
                imRes.i = _mm_add_epi16(imRes.i, res1.i);
                res1.i = _mm_srli_epi32(imRes.i,16);
                imRes.i = _mm_add_epi16(imRes.i, res1.i);
                imRes.i = _mm_srai_epi16(imRes.i,8);
                imRes.i = _mm_add_epi16(imRes.i, addconst128);

                *dst[1] = (U8) _mm_extract_epi16(imRes.i,0);
                *dst[2] = (U8)  _mm_extract_epi16(imRes.i,4); 

            }
            if (rem) // Process the extra column if odd number of columns
            {
                rSum = src[0];
                gSum = src[1];
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
        }
        else // row = 1
        {
            // 1x2px block process: 420 Compression (MPEG-1)
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                rSum = src[0]; 
                gSum = src[1]; 
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                FW_REF::Inc(src, BYTES(3));
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YB );
                rSum += src[0]; 
                gSum += src[1]; 
                bSum += src[2];
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
            if (rem) // Process the extra column if even number of columns
            {
                rSum = src[0]; 
                gSum = src[1]; 
                bSum = src[2];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( (double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                     (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                     (double)(gSum) * CONST_RGB2YCbCr_CbG + 
                                                     (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( (double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                     (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                     (double)(bSum) * CONST_RGB2YCbCr_CrB +
                                                     (double)CONST_RGB2YCbCraddCr );
            }
        }
    }

    static SYS_INLINE FwStatus ConvertBGRToYCbCr420_8u_C3P3R_REF_2D( const A8U *pSrc,    int srcStep, 
                                                                      A8U *const pDst[3], int dstStep[3], FwiSize roiSize )
    {
        A8U *pSrcEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};

        int rem = roiSize.height&0x1;
        pSrcEnd = FW_REF::Offset( (A8U *)pSrc, (roiSize.height-rem)*srcStep ); // rem = 0 or 1

        
        // Process every 2 rows at a time
        for ( ; pSrc < pSrcEnd; FW_REF::Inc(pSrc,   2*srcStep),    
                                FW_REF::Inc(dst[0], 2*dstStep[0]), 
                                FW_REF::Inc(dst[1], dstStep[1]), 
                                FW_REF::Inc(dst[2], dstStep[2]) )
        {
            ConvertBGRToYCbCr420_8u_C3P3R_REF_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 2 );
        }
        // Process the remaining 1 row, if needed
        if (rem)
        {
            ConvertBGRToYCbCr420_8u_C3P3R_REF_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 1 );
        }
        
        if ( (roiSize.width|roiSize.height)&0x1 )
        {
            return fwStsDoubleSize;
        }

        return fwStsNoErr;
    }

    static SYS_INLINE void ConvertBGRToYCbCr420_8u_C3P3R_REF_1D( const A8U *pSrc, int srcStep, 
                                                                 A8U *pDst[3],    int dstStep_Y, int width, int row )
    {
        A8U *pSrcRowEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};
        A8U *dstY = dst[0], *src = (A8U *)pSrc;
        double rSum = 0, gSum = 0, bSum = 0;                             // Running sum of RGB values per block
        int rem = width&0x1;

        pSrcRowEnd = FW_REF::Offset((A8U*)pSrc, (width-rem)*BYTES(3));  // rem = 0 or 1.

        if (row > 1) // Condition: 1) # of rows = 2
        {
            // 2x2px block process: 420 Compression
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                // Compute Y
                // Y0
                rSum = src[2]; 
                gSum = src[1];
                bSum = src[0];
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                FW_REF::Inc(src, BYTES(3));
                // Y1
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];
                
                // Next Row
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                // Y2
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];
                FW_REF::Inc(src, BYTES(3));

                // Y3
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 4 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 4 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }

            if (rem) // Process the extra column if odd number of columns
            {
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
        }
        else // row = 1
        {
            // 1x2px block process: 420 Compression (MPEG-1)
            for ( ; pSrc != pSrcRowEnd; FW_REF::Inc(pSrc,   BYTES(6)),  // 2 x 3Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),  // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),  // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),  // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                FW_REF::Inc(src, BYTES(3));
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1];
                bSum += src[0];
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }

            if (rem) // Process the extra column if odd number of columns
            {
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( (double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                     (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                     (double)(gSum) * CONST_RGB2YCbCr_CbG + 
                                                     (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( (double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                     (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                     (double)(bSum) * CONST_RGB2YCbCr_CrB +
                                                     (double)CONST_RGB2YCbCraddCr );
            }
        }
    }

    static SYS_INLINE FwStatus ConvertBGRToYCbCr420_8u_AC4P3R_REF_2D( const A8U *pSrc,    int srcStep, 
                                                                       A8U *const pDst[3], int dstStep[3], FwiSize roiSize )
    {
        A8U *pSrcEnd;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};

        int rem = roiSize.height&0x1;
        pSrcEnd = FW_REF::Offset( (A8U *)pSrc, (roiSize.height-rem)*srcStep );
        
        // Process every 2 rows at a time
        for ( ; pSrc < pSrcEnd; FW_REF::Inc(pSrc,   2 * srcStep),    
                                FW_REF::Inc(dst[0], 2 * dstStep[0]), 
                                FW_REF::Inc(dst[1], dstStep[1]), 
                                FW_REF::Inc(dst[2], dstStep[2]) )
        {
            ConvertBGRToYCbCr420_8u_AC4P3R_REF_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 2 );
        }

        // Process the remaining 1 row, if needed
        if (rem)
        {
            ConvertBGRToYCbCr420_8u_AC4P3R_REF_1D( pSrc, srcStep, dst, dstStep[0], roiSize.width, 1 );
        }

        if( roiSize.height&0x1 || roiSize.width&0x1 )
        {
            return fwStsDoubleSize;
        }

        return fwStsNoErr;
    }

    static SYS_INLINE void ConvertBGRToYCbCr420_8u_AC4P3R_REF_1D( const A8U *pSrc, int srcStep, 
                                                                  A8U *pDst[3],    int dstStep_Y, int width, int row )
    {
        A8U *pSrcEndRow;
        A8U *dst[3] = {pDst[0], pDst[1], pDst[2]};
        A8U *dstY = dst[0], *src = (A8U *)pSrc;
        double rSum = 0, gSum = 0, bSum = 0;                             // Running sum of RGB values per block
        int rem = width&0x1;

        pSrcEndRow = FW_REF::Offset((A8U*)pSrc, (width-rem)*BYTES(4));  // rem = 0 or 1.

        if (row > 1) // Condition: 1) # of rows = 2
        {
            // 2x2px block process: 420 Compression
            for ( ; pSrc != pSrcEndRow; FW_REF::Inc(pSrc,   BYTES(8)), // 2 x 4Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)), // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)), // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)), // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                // Compute Y
                // Y0
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                FW_REF::Inc(src, BYTES(4));

                // Y1
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];
                
                // Next Row
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                // Y2
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];
                FW_REF::Inc(src, BYTES(4));

                // Y3
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 4 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 4 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }

            if (rem) // Process the extra column if odd number of columns
            {
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                
                FW_REF::Inc(dstY, dstStep_Y);
                src = FW_REF::Offset((A8U *)pSrc, srcStep);

                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(src[2]) * CONST_RGB2YCbCr_YR +        
                                                   (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                   (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];

                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }
        }
        else // row = 1
        {
            // 1x2px block process: 420 Compression (MPEG-1)
            for ( ; pSrc != pSrcEndRow; FW_REF::Inc(pSrc,   BYTES(8)),    // 2 x 4Bpp
                                        FW_REF::Inc(dst[0], BYTES(2)),    // 2 x 1Bpp
                                        FW_REF::Inc(dst[1], BYTES(1)),    // 1 x 1Bpp
                                        FW_REF::Inc(dst[2], BYTES(1)),    // 1 x 1Bpp
                                        dstY = dst[0],
                                        src = (A8U *)pSrc )
            {
                rSum = src[2];
                gSum = src[1];
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                FW_REF::Inc(src, BYTES(4));
                *(dstY+1) = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                       (double)(src[2]) * CONST_RGB2YCbCr_YR +
                                                       (double)(src[1]) * CONST_RGB2YCbCr_YG + 
                                                       (double)(src[0]) * CONST_RGB2YCbCr_YB );
                rSum += src[2]; 
                gSum += src[1]; 
                bSum += src[0];
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( ((double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                      (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                      (double)(gSum) * CONST_RGB2YCbCr_CbG) / 2 + 
                                                      (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( ((double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                      (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                      (double)(bSum) * CONST_RGB2YCbCr_CrB) / 2 +
                                                      (double)CONST_RGB2YCbCraddCr );
            }

            if (rem) // Process the extra column if even number of columns
            {
                rSum = src[2]; 
                gSum = src[1]; 
                bSum = src[0];
                // Compute Y
                *dstY = FW_REF::Limits<A8U>::Sat( (double)CONST_RGB2YCbCraddY + 
                                                   (double)(rSum) * CONST_RGB2YCbCr_YR +        
                                                   (double)(gSum) * CONST_RGB2YCbCr_YG + 
                                                   (double)(bSum) * CONST_RGB2YCbCr_YB );
                
                // Compute Cb, Cr
                *dst[1] = FW_REF::Limits<A8U>::Sat( (double)(bSum) * CONST_RGB2YCbCr_CbB - 
                                                     (double)(rSum) * CONST_RGB2YCbCr_CbR - 
                                                     (double)(gSum) * CONST_RGB2YCbCr_CbG + 
                                                     (double)CONST_RGB2YCbCraddCb );
                *dst[2] = FW_REF::Limits<A8U>::Sat( (double)(rSum) * CONST_RGB2YCbCr_CrR -
                                                     (double)(gSum) * CONST_RGB2YCbCr_CrG - 
                                                     (double)(bSum) * CONST_RGB2YCbCr_CrB +
                                                     (double)CONST_RGB2YCbCraddCr );
            }
        }
    }
}; // OPT_LEVEL
#endif // __COLOR_H__
