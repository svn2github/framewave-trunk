/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

void SYS_INLINE MySet_8u(Fw8u val, Fw8u* pDst, int len)
{
	for (int col=0; col<len; col++)	*(pDst+col) = val;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiExpandPlane_H264_8u_C1R)(	Fw8u *StartPtr, 
										Fw32u uFrameWidth,
										Fw32u uFrameHeight, 
										Fw32u uPitch, 
										Fw32u uPels, 
										FwvcFrameFieldFlag uFrameFieldFlag)
{
	if(StartPtr == NULL) return fwStsNullPtrErr;

    Fw32u i;
//	Fw32u col;
//    Fw8u *pSrc, *pSrcDst, *pDst;
    Fw8u *pSrcDst;
//	Fw8u uLeftFillVal;
//	Fw8u uRightFillVal;

    switch (uFrameFieldFlag)
    {
    case FWVC_FRAME:
#if 0
        /* bottom */
        pSrc = StartPtr + uPitch * (uFrameHeight - 1);
        pDst = pSrc + uPitch;
        for (i = 0; i < uPels*2; i++)
        {
			for (col=0; col<uFrameWidth; col++)	pDst[col] = pSrc[col];
            pDst += uPitch;
        }

        /* sides */
        pSrcDst = StartPtr;
        for (i = 0; i < uFrameHeight + uPels; i += 1)
        {
			uLeftFillVal = *pSrcDst;
			uRightFillVal = *(pSrcDst + uFrameWidth - 1);

			for (col=0; col<uPels; col++)
			{
				*(pSrcDst - uPels + col) = uLeftFillVal;
				*(pSrcDst + uFrameWidth + col) = uRightFillVal;
			}

            pSrcDst  += uPitch;
        }

        /* top */
        pSrc = StartPtr - uPels;
        pDst = pSrc - uPitch;

        for (i = 0; i < uPels*2; i++)
        {
			for (col=0; col<uFrameWidth + 2*uPels; col++)	pDst[col] = pSrc[col];

            pDst -= uPitch;
        }
#else	// refer um_h264_dec_defs_yuv.cpp
        pSrcDst = StartPtr;
            for (i = 0; i < uFrameHeight + uPels; i += 1)
            {
                /* set left site */
                MySet_8u(pSrcDst[0], pSrcDst - uPels, uPels);
                /* set right site */
                MySet_8u(pSrcDst[uFrameWidth - 1], pSrcDst + uFrameWidth, uPels);
                pSrcDst  += uPitch;
            }
#endif
        break;

    case FWVC_TOP_FIELD:

#if 0
        /* bottom */
        pSrc = StartPtr + uPitch * (uFrameHeight - 2);
        pDst = pSrc + uPitch * 2;
        for (i = 0; i < uPels; i++)
        {
			for (col=0; col<uFrameWidth; col++)	pDst[col] = pSrc[col];

            pDst += uPitch * 2;
        }

        /* Area (2) - sides */
        pSrcDst = StartPtr;

        for (i = 0; i < (uFrameHeight>>1)  + uPels; i++)
        {
			// get fill values from left and right columns of original frame
			uLeftFillVal  = *pSrcDst;
			uRightFillVal = *(pSrcDst + uFrameWidth - 1);

			for (col=0; col<uPels; col++)
			{
				*(pSrcDst - uPels + col) = uLeftFillVal;
				*(pSrcDst + uFrameWidth + col) = uRightFillVal;
			}

            pSrcDst  += uPitch * 2;
        }

       /* top */
        pSrc = StartPtr - uPels;
        pDst = pSrc - uPitch * 2;

        for (i = 0; i < uPels; i++)
        {
			for (col=0; col<uFrameWidth + 2*uPels; col++)	pDst[col] = pSrc[col];
            pDst -= uPitch * 2;
        }
#else	// refer um_h264_dec_defs_yuv.cpp
        pSrcDst = StartPtr;
        for (i = 0; i < (uFrameHeight>>1)  + uPels; i++)
        {
            /* set left site */
            MySet_8u(pSrcDst[0], pSrcDst - uPels, uPels);
            /* set right site */
            MySet_8u(pSrcDst[uFrameWidth - 1], pSrcDst + uFrameWidth, uPels);
            pSrcDst  += uPitch * 2;
        }
#endif
        break;

    case FWVC_BOTTOM_FIELD:
#if 0
        /* bottom */
        pSrc = StartPtr + uPitch * (uFrameHeight - 1);
        pDst = pSrc + uPitch * 2;

        for (i = 0; i < uPels ; i++)
        {
			for (col=0; col<uFrameWidth; col++) pDst[col] = pSrc[col];

            pDst += uPitch * 2;
        }

        /* sides */
        pSrcDst = StartPtr + uPitch;

        for (i = 0; i < (uFrameHeight>>1)  + uPels; i++)
        {
			uLeftFillVal  = *pSrcDst;
			uRightFillVal = *(pSrcDst + uFrameWidth - 1);

			// fill all bytes on both edges
			for (col=0; col<uPels; col++)
			{
				*(pSrcDst - uPels + col) = uLeftFillVal;
				*(pSrcDst + uFrameWidth + col) = uRightFillVal;
			}

            pSrcDst  += uPitch * 2;
        }
        /* top */
        pSrc = StartPtr - uPels + uPitch;
        pDst = pSrc - uPitch * 2;

        for (i = 0; i < uPels; i++)
        {
			for (col=0; col<uFrameWidth + 2*uPels; col++)	pDst[col] = pSrc[col];
            pDst -= uPitch * 2;
        }
#else	// refer um_h264_dec_defs_yuv.cpp
        pSrcDst = StartPtr + uPitch;

        for (i = 0; i < (uFrameHeight>>1)  + uPels; i++)
        {
            /* set left site */
            MySet_8u(pSrcDst[0], pSrcDst - uPels, uPels);
            /* set right site */
            MySet_8u(pSrcDst[uFrameWidth - 1], pSrcDst + uFrameWidth, uPels);
            pSrcDst  += uPitch * 2;
        }

#endif
        break;

    default:
        return fwStsBadArgErr;
    }


	return fwStsNoErr;
}
#endif

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
static SYS_INLINE Fw32s filter_h(const Fw8u* p)
{
    return p[-2]-5*p[-1]+20*p[0]+20*p[1]-5*p[2]+p[3];
}
static SYS_INLINE Fw32s filter_v(const Fw8u* p, Fw32s stride)
{
    return p[-2*stride]-5*p[-stride]+20*p[0]+20*p[stride]-5*p[2*stride]+p[3*stride];
}

#define interpolate_halfpel_h8_sse2(pesi, pedi)	\
{\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi-2) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi-1) );\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+1) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+2) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+3) );\
	xmm7  = _mm_set1_epi8(0); \
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm0 = _mm_add_epi16(xmm0, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm0 = _mm_mullo_epi16(xmm0, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm0 = _mm_add_epi16(xmm0, xmm4);\
	xmm0 = _mm_srai_epi16(xmm0, 5);\
	xmm0 = _mm_packus_epi16(xmm0, xmm0);\
	_mm_storel_epi64( (__m128i*)(pedi), xmm0 );\
}
#define interpolate_halfpel_v8_sse2(pesi, stride, pedi)	\
{\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi-2*stride) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi-1*stride) );\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+1*stride) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+2*stride) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+3*stride) );\
	xmm7  = _mm_set1_epi8(0); \
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm0 = _mm_add_epi16(xmm0, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm0 = _mm_mullo_epi16(xmm0, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm0 = _mm_add_epi16(xmm0, xmm4);\
	xmm0 = _mm_srai_epi16(xmm0, 5);\
	xmm0 = _mm_packus_epi16(xmm0, xmm0);\
	_mm_storel_epi64( (__m128i*)(pedi), xmm0 );\
}

#define avg8_sse2(pesi, pedi)	\
{\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pedi) );\
	xmm0 = _mm_avg_epu8(xmm0, xmm1);\
	_mm_storel_epi64( (__m128i*)(pesi), xmm0 );\
}

#define interpolate_halfpel_h16_sse2(pesi, pedi)	\
{\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi-2) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi-1) );\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+1) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+2) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+3) );\
	xmm7  = _mm_set1_epi8(0); \
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm0 = _mm_add_epi16(xmm0, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm0 = _mm_mullo_epi16(xmm0, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm0 = _mm_add_epi16(xmm0, xmm4);\
	xmm0 = _mm_srai_epi16(xmm0, 5);\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi+8-2) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi+8-1) );\
	xmm8 = _mm_loadl_epi64( (__m128i*)(pesi+8) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+8+1) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+8+2) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+8+3) );\
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm8 = _mm_add_epi16(xmm8, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm8 = _mm_mullo_epi16(xmm8, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm8 = _mm_add_epi16(xmm8, xmm4);\
	xmm8 = _mm_srai_epi16(xmm8, 5);\
	xmm0 = _mm_packus_epi16(xmm0, xmm8);\
	_mm_storeu_si128 ( (__m128i*)(pedi), xmm0 );\
}
#define interpolate_halfpel_v16_sse2(pesi, stride, pedi)	\
{\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi-2*stride) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi-1*stride) );\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+1*stride) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+2*stride) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+3*stride) );\
	xmm7  = _mm_set1_epi8(0); \
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm0 = _mm_add_epi16(xmm0, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm0 = _mm_mullo_epi16(xmm0, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm0 = _mm_add_epi16(xmm0, xmm4);\
	xmm0 = _mm_srai_epi16(xmm0, 5);\
	xmm4 = _mm_loadl_epi64( (__m128i*)(pesi+8-2*stride) );\
	xmm5 = _mm_loadl_epi64( (__m128i*)(pesi+8-1*stride) );\
	xmm8 = _mm_loadl_epi64( (__m128i*)(pesi+8) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+8+1*stride) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+8+2*stride) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+8+3*stride) );\
   	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);\
   	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);\
   	xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm6 = _mm_set1_epi16(16); \
	xmm4 = _mm_add_epi16(xmm4, xmm6);\
	xmm5 = _mm_add_epi16(xmm5, xmm2);\
	xmm8 = _mm_add_epi16(xmm8, xmm1);\
	xmm6 = _mm_set1_epi16(-5); \
	xmm5 = _mm_mullo_epi16(xmm5, xmm6);\
	xmm6 = _mm_set1_epi16(20); \
	xmm8 = _mm_mullo_epi16(xmm8, xmm6);\
	xmm4 = _mm_add_epi16(xmm4, xmm3);\
	xmm4 = _mm_add_epi16(xmm4, xmm5);\
	xmm8 = _mm_add_epi16(xmm8, xmm4);\
	xmm8 = _mm_srai_epi16(xmm8, 5);\
	xmm0 = _mm_packus_epi16(xmm0, xmm8);\
	_mm_storeu_si128 ( (__m128i*)(pedi), xmm0 );\
}
#if 0
#define avg16_sse2(pesi, pedi)	\
{\
	xmm0 = _mm_load_si128( (__m128i*)(pesi) );\
	xmm1 = _mm_load_si128( (__m128i*)(pedi) );\
	xmm0 = _mm_avg_epu8(xmm0, xmm1);\
	_mm_store_si128( (__m128i*)(pesi), xmm0 );\
}
#else
#define avg16_sse2(pesi, pedi)	\
{\
	xmm0 = _mm_loadu_si128( (__m128i*)(pesi) );\
	xmm1 = _mm_loadu_si128( (__m128i*)(pedi) );\
	xmm0 = _mm_avg_epu8(xmm0, xmm1);\
	_mm_storeu_si128( (__m128i*)(pesi), xmm0 );\
}
#endif

FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLuma_H264_8u_C1R)(
								const Fw8u* pSrc, Fw32s srcStep,
									  Fw8u* pDst, Fw32s dstStep, 
									  Fw32s dx,   Fw32s dy, 
									  FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;

//	if( roiSize.width  != 4  && roiSize.width  != 8  && roiSize.width  != 16 &&
//		roiSize.height != 4  &&	roiSize.height != 8  &&	roiSize.height != 16) return fwStsSizeErr;

	static const int COEF[6] = {1, -5, 20, 20, -5, 1 };
	int maxold_x,maxold_y, i, j, x, y;
    int result = 0;
    int pres_x;
    int pres_y; 
    int tmp_res[32][32];
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();


	maxold_x = roiSize.width-1;
	maxold_y = roiSize.height-1;

    if (dy == 0 && dx == 0) { /* No vertical interpolation */
	  for (j = 0; j < roiSize.height; j++) {
		for (i = 0; i < roiSize.width; i++) {
			pDst[j*dstStep+i] = pSrc[j*srcStep+i];
		}
	  }
	  return fwStsNoErr;
	}

    if (dy == 0) { /* No vertical interpolation */
		if( Dispatch_Type==DT_SSE2 && (roiSize.width==8 || roiSize.width==16))
		{
			if(roiSize.width==16)
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h16_sse2(pSrc+j*srcStep, pDst+j*dstStep);
			  }
			  if ((dx&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg16_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			} else {
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h8_sse2(pSrc+j*srcStep, pDst+j*dstStep);
			  }
			  if ((dx&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg8_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
		 }
		else
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
	#if 0
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
	#else
				result = filter_h(pSrc+j*srcStep+i);	
	#endif
				pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }
		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
	#if 0
				pDst[j*dstStep+i] = (pDst[j*dstStep+i] 
									 + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))]
									 + 1 )/2;
	#else
				pDst[j*dstStep+i] = (pDst[j*dstStep+i]+pSrc[j*srcStep+(i+dx/2)]+1)/2;
	#endif
		  }
		}
    }
    else if (dx == 0) {  /* No horizontal interpolation */
#if 0
		if( Dispatch_Type==DT_SSE2 && (roiSize.width==8 || roiSize.width==16))
		{
			if(roiSize.width==16)
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v16_sse2((pSrc+j*srcStep), srcStep, (pDst+j*dstStep));
			  }
			  if ((dy&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg16_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			} else {
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v8_sse2(pSrc+j*srcStep, srcStep, pDst+j*dstStep);
			  }
			  if ((dy&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg8_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
		}	
		else
#endif
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
	#if 0
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
	#else
				result = filter_v(pSrc+j*srcStep+i, srcStep);	
	#endif
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));

			}
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
	#if 0
				 pDst[j*dstStep+i] = (pDst[j*dstStep+i] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
	#else
				 pDst[j*dstStep+i] = (pDst[j*dstStep+i] + pSrc[(j+dy/2)*srcStep+i] +1 )/2;
	#endif
		  }
		}
    }
    else if (dx == 2) {  /* Vertical & horizontal interpolation */
#if 0
		if( Dispatch_Type==DT_SSE2 && (roiSize.width==8 || roiSize.width==16))
		{
				Fw8u tmp_buf[32*32];
				Fw8u *ptmp_buf=&tmp_buf[0];

			for (j=0; j<32*32; j++) tmp_buf[j]=0;

			if(roiSize.width==16)
			{
		  for (j = -2; j < roiSize.height+3; j++) {
				interpolate_halfpel_h16_sse2(pSrc+j*srcStep, ptmp_buf+j*32+64);
			  }
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v16_sse2(ptmp_buf+j*32+64, 32, pDst+j*dstStep);
			  }
			  if ((dy&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg16_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
			else
			{
		  for (j = -2; j < roiSize.height+3; j++) {
				interpolate_halfpel_h8_sse2(pSrc+j*srcStep, ptmp_buf+j*32+64);
			  }
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v8_sse2(ptmp_buf+j*32+64, 32, pDst+j*dstStep);
			  }
			  if ((dy&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg8_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
		}
		else
#endif
		{
		  for (j = -2; j < roiSize.height+3; j++) {
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[j+2][i] = 0, x = -2; x < 4; x++)
	#if 0
				tmp_res[j+2][i] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
	#else
				tmp_res[j+2][i] += pSrc[j*srcStep+i+x]*COEF[x+2];
	#endif
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += tmp_res[j+y+2][i]*COEF[y+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			} 
		  }
		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
	//             pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)) +1 )/2;
				  result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
    }
    else if (dy == 2) {  /* Horizontal & vertical interpolation */
#if 0
		if( Dispatch_Type==DT_SSE2 && (roiSize.width==8 || roiSize.width==16))
		{
			if(roiSize.width==16)
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v16_sse2(pSrc+j*srcStep, dstStep, pDst+j*dstStep);
			  }
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h16_sse2(pDst+j*dstStep, pDst+j*dstStep);
			  }
			  if ((dx&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg16_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
			else
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v8_sse2(pDst+j*dstStep, dstStep, pDst+j*dstStep);
			  }

			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h8_sse2(pSrc+j*srcStep, pDst+j*dstStep);
			  }
			  if ((dx&1) == 1) {
				for (j = 0; j < roiSize.height; j++)
					avg8_sse2( pDst+j*dstStep, pSrc+j*srcStep+dx/2 );
			  }
			}
		}
		else
#endif
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
	#if 0
				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
	#else
				tmp_res[j][i+2] += pSrc[(j+y)*srcStep+i]*COEF[y+2];
	#endif
		  }
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += tmp_res[j][i+x+2]*COEF[x+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			}
		  }
		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
	//            pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
				  result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
    }
    else {  /* Diagonal interpolation */
#if 0
		if( Dispatch_Type==DT_SSE2 && (roiSize.width==8 || roiSize.width==16))
		{
			if(roiSize.width==16)
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h16_sse2(pSrc+j*srcStep, pDst+j*dstStep);
			  }
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v16_sse2(pDst+j*dstStep, dstStep, pDst+j*dstStep);
			  }
			  for (j = 0; j < roiSize.height; j++)
					avg16_sse2( pDst+j*dstStep, pSrc+j*srcStep );
			}
			else
			{
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_h8_sse2(pDst+j*dstStep, pDst+j*dstStep);
			  }
			  for (j = 0; j < roiSize.height; j++) {
				interpolate_halfpel_v8_sse2(pSrc+j*srcStep, dstStep, pDst+j*dstStep);
			  }
			  for (j = 0; j < roiSize.height; j++)
			    avg8_sse2( pDst+j*dstStep, pSrc+j*srcStep );
			}
		}
		else
#endif
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? j : j+1;
	//          pres_y = max(0,min(maxold_y,pres_y));
			  for (result = 0, x = -2; x < 4; x++)
	#if 0
				result += pSrc[pres_y*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
	#else
				result += pSrc[pres_y*srcStep+i+x]*COEF[x+2];
	#endif
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
	//          pres_x = max(0,min(maxold_x,pres_x));
			  for (result = 0, y = -2; y < 4; y++)
	#if 0
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
	#else
				result += pSrc[(j+y)*srcStep+pres_x]*COEF[y+2];
	#endif
	//        pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (result+16)/32)) +1 ) / 2;
			  result = (Fw8u)(max(0, min(255, (result+16)/32)));
			  result += (int)pDst[j*dstStep+i];
			  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			}
		}
      }
    }

	return fwStsNoErr;
}

#define interpolate_Chroma_h8_sse2(pesi, srcStep, pedi, coef0, coef1, coef2, coef3)	\
{\
	xmm0 = _mm_loadl_epi64( (__m128i*)(pesi) );\
	xmm1 = _mm_loadl_epi64( (__m128i*)(pesi+1) );\
	xmm2 = _mm_loadl_epi64( (__m128i*)(pesi+srcStep) );\
	xmm3 = _mm_loadl_epi64( (__m128i*)(pesi+srcStep+1) );\
	xmm7  = _mm_set1_epi8(0); \
   	xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);\
   	xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);\
   	xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);\
   	xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);\
	xmm5 = _mm_set1_epi16((short)coef0); \
	xmm0 = _mm_mullo_epi16(xmm0, xmm5);\
	xmm6 = _mm_set1_epi16((short)coef1); \
	xmm1 = _mm_mullo_epi16(xmm1, xmm6);\
	xmm0 = _mm_add_epi16(xmm0, xmm1);\
	xmm5 = _mm_set1_epi16((short)coef2); \
	xmm2 = _mm_mullo_epi16(xmm2, xmm5);\
	xmm0 = _mm_add_epi16(xmm0, xmm2);\
	xmm6 = _mm_set1_epi16((short)coef3); \
	xmm3 = _mm_mullo_epi16(xmm3, xmm6);\
	xmm0 = _mm_add_epi16(xmm0, xmm3);\
	xmm5 = _mm_set1_epi16((short)32); \
	xmm0 = _mm_add_epi16(xmm0, xmm5);\
	xmm0 = _mm_srai_epi16(xmm0, 6);\
	xmm0 = _mm_packus_epi16(xmm0, xmm0);\
	_mm_storel_epi64( (__m128i*)(pedi), xmm0 );\
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateChroma_H264_8u_C1R)(
								const Fw8u*	pSrc,	Fw32s	srcStep,
									  Fw8u*	pDst,	Fw32s	dstStep, 
									  Fw32s	dx,		Fw32s	dy, 
									  FwiSize	roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 8) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 7 || dy > 7) return fwStsBadArgErr;
	if( roiSize.width  != 2  && roiSize.width  != 4  && roiSize.width  != 8 &&
		roiSize.height != 2  &&	roiSize.height != 4  &&	roiSize.height != 8) return fwStsSizeErr;

//	int coef0, coef1, coef2, coef3, xInt, yInt, tmp;
	int coef0, coef1, coef2, coef3, tmp;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7;

    /*
     * Get chroma prediction
     */

    /* Bilinear interpolation coefficients */
    coef0 = (8-(dx&7)) * (8-(dy&7));
    coef1 = (dx&7)     * (8-(dy&7));
    coef2 = (8-(dx&7)) * (dy&7);
    coef3 = (dx&7)     * (dy&7);

    /* Chroma vectors have 1/8 chroma pel precision */
	if( Dispatch_Type==DT_SSE2 && roiSize.width==8)
	{
		for (int j = 0; j < roiSize.height;  j++) {
		   interpolate_Chroma_h8_sse2((pSrc+j*srcStep), srcStep, (pDst+j*dstStep), coef0, coef1, coef2, coef3);
		}
	}
	else
	{
		for (int j = 0; j < roiSize.height;  j++) {
		   for (int i = 0; i < roiSize.width;  i++) {
			 tmp = (coef0 * pSrc[j*srcStep + i  ] +
					coef1 * pSrc[j*srcStep + i+1] +
					coef2 * pSrc[(j+1)*srcStep + i  ] +
					coef3 * pSrc[(j+1)*srcStep + i+1] + 32) >> 6;
			 pDst[j*dstStep+i] = (Fw8u) tmp;
		  }
		}
	}
	return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateBlock_H264_8u_P2P1R)(Fw8u *pSrc1, 
											 Fw8u *pSrc2, 
											 Fw8u *pDst,  
											 Fw32u width, Fw32u height, 
											 Fw32u pitch)
{
	if(pSrc1 == NULL || pSrc2 == NULL || pDst == NULL) return fwStsNullPtrErr;

	int tmp;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1;

	if(Dispatch_Type==DT_SSE2 && width > 8)
	{
		for (int j = 0; j < (int)height;  j++) {
			int i=0;
		   for (; i < (int)width;  i+=8) {
				xmm0 = _mm_loadl_epi64( (__m128i*)(pSrc1+j*width+i) );
				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrc2+j*width+i) );
				xmm0 = _mm_avg_epu8(xmm0, xmm1);
				_mm_storel_epi64( (__m128i*)(pSrc1+j*pitch+i), xmm0 );
		   }
		   for (; i < (int)width;  i++) {
			 tmp = (pSrc1[j*width+i] + pSrc2[j*width+i]+1)/2;
			 pDst[j*pitch+i] = (Fw8u) tmp;
		  }
		}
	}
	else
	{
		for (int j = 0; j < (int)height;  j++) {
		   for (int i = 0; i < (int)width;  i++) {
			 tmp = (pSrc1[j*width+i] + pSrc2[j*width+i]+1)/2;
			 pDst[j*pitch+i] = (Fw8u) tmp;
		  }
		}
	}
	return fwStsNoErr;
}


#define  clip1(val) (((val)<0)? 0:(((val)>255)? 255:(val)))
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWeightedAverage_H264_8u_C1IR)(const Fw8u* pSrc1, 
												 Fw8u* pSrc2Dst, 
												 Fw32s srcDstStep, 
												 Fw32s weight1, Fw32s weight2, 
												 Fw32s shift,
												 Fw32s offset, 
												 FwiSize roiSize)
{
	if(pSrc1 == NULL || pSrc2Dst == NULL) return fwStsNullPtrErr;
	if(srcDstStep <= roiSize.width) return fwStsStepErr;

	// Source2 [x,y]=
	//Clip1( weight1*Source1[x,y]+ weight2*Source2[x,y]+1+( shift-1)) >> shift + offset)
	int tmp;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7, xmm5, xmm6;

	if(Dispatch_Type==DT_SSE2 && roiSize.width > 8)
	{
		for (int j = 0; j < (int)roiSize.height;  j++) {
			int i=0;
		   for (; i < (int)roiSize.width;  i+=8) {
				xmm0 = _mm_loadl_epi64( (__m128i*)(pSrc1+j*roiSize.width+i) );
				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrc2Dst+j*srcDstStep+i) );
				xmm7  = _mm_set1_epi8(0); 
   				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);
   				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);

 				xmm5 = _mm_set1_epi16((short)weight1); 
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
				xmm6 = _mm_set1_epi16((short)weight2); 
				xmm1 = _mm_mullo_epi16(xmm1, xmm6);
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

                __m128i shift_x = _mm_cvtsi32_si128(shift);
				xmm0 = _mm_sra_epi16(xmm0, shift_x);		// >>shift

 				xmm5 = _mm_set1_epi16((short)offset);	// +offset
				xmm0 = _mm_add_epi16(xmm0, xmm5);

				xmm0 = _mm_packus_epi16(xmm0, xmm0);
				_mm_storel_epi64( (__m128i*)(pSrc2Dst+j*srcDstStep+i), xmm0 );
		   }
		   for (; i < roiSize.width;  i++) {
			 tmp = ((weight1*   pSrc1[j*roiSize.width+i]+
				     weight2*pSrc2Dst[j*srcDstStep+i]+1)>>shift) + offset;
			 pSrc2Dst[j*srcDstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}
	else
	{
		for (int j = 0; j < roiSize.height;  j++) {
		   for (int i = 0; i < roiSize.width;  i++) {
			 tmp = ((weight1*   pSrc1[j*roiSize.width+i]+
					 weight2*pSrc2Dst[j*srcDstStep+i]+1)>> shift) + offset;
			 pSrc2Dst[j*srcDstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}

	return fwStsNoErr;
}
#endif

FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaTop_H264_8u_C1R)(const Fw8u* pSrc, Fw32s srcStep,
			Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s outPixels, 
			FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;
	if( roiSize.width  != 4  && roiSize.width  != 8  && roiSize.width  != 16 &&
		roiSize.height != 4  &&	roiSize.height != 8  &&	roiSize.height != 16) return fwStsSizeErr;

#if 1

//pTempBuffer = CopyBlockFromTop(pSrc,srcStep, TempBuffer+32*4, 32, outPixels, 3, 3, roiSize);
    {
        const Fw8u *pRefPlane=pSrc;
		Fw8u TempBuffer[32*24];
        Fw8u *TemporalP=(TempBuffer+32*4);
        Fw32s Temp_Width=32;
        Fw32s xh=3;
        Fw32s yh=3;

        Fw8u *pOut;
        int i;

        pOut = TemporalP;
        Fw32s py = yh>0?3:0;
        Fw32s px = xh>0?3:0;

        pRefPlane += (srcStep*outPixels);

        Fw32s num_outs = outPixels+py;
        Fw32s EXheight = roiSize.height+py*2;

        pRefPlane-=px;
        pOut-=py*Temp_Width;
        pOut-=px;

        if (num_outs>EXheight) //starting point outside bottom boundary
        {
            num_outs = roiSize.height+2*py;
            for(i=0;i<num_outs;i++,pOut+=Temp_Width)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);
        }
        else
        {
            for(i=0;i<num_outs;i++,pOut+=Temp_Width)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);

            for(i=0;i<EXheight-num_outs;i++,pOut+=Temp_Width,pRefPlane+=srcStep)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);
        }

	pSrc=TemporalP;
    srcStep=32;
 }

    return fwiInterpolateLuma_H264_8u_C1R(pSrc, srcStep, pDst, dstStep, dx, dy, roiSize);

#else

	static const int COEF[6] = {1, -5, 20, 20, -5, 1 };
	int maxold_x,maxold_y, i, j, x, y;
//    int result = 0, result1, result2;
    int result = 0;
    int pres_x;
    int pres_y; 
//    int tmp_res[4][9];


    int tmp_res[32][25];

	maxold_x = roiSize.width-1;
	maxold_y = roiSize.height-1;

	// data actually used in interpixel interpolation at the top boundary when outPixels > 0
	if(outPixels > 0)	
	{
		int jj;

//printf("\n outPixels=%d, h=%d, w=%d, dx=%d dy=%d",outPixels, roiSize.height, roiSize.width, dx, dy);

	// skip when totally out of image area.

	if(outPixels >= roiSize.height)	
	{
			for (jj = 0; jj < outPixels; jj++) 
			{
                j = outPixels;;
				for (i = 0; i < roiSize.width; i++)
				{
				  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			      pDst[jj*dstStep+i] = (Fw8u)result;
				}
			}

		  return fwStsNoErr;
	}

		if (dy==0 && dx==0) { /* No vertical interpolation */
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[jj*dstStep+i] = (Fw8u)result;
			}
		  }
		  return fwStsNoErr;
		}

		if (dy == 0) { /* No vertical interpolation */
#if 1
//printf("<<<<< dy == 0");

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
	          pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }
		  if ((dx&1) == 1) {
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			  if(outPixels > j)	j = outPixels; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
	            pDst[jj*dstStep+i] = (pDst[jj*dstStep+i] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))] +1 )/2;
			}
		  }
#else
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[jj*dstStep+i] = (Fw8u)result;
			}
		  }
#endif

  		  return fwStsNoErr;
		}
		else if (dx == 0) {  /* No horizontal interpolation */

//printf("<<<<< dx == 0");

#if 1
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//		    j=jj;
//			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
			  {
//				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
			    j=jj+y;
				if(outPixels > j)	j = outPixels; // use top boundary
//				result += pSrc[max(outPixels,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
				result += pSrc[max(outPixels,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
			  }
              pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dy&1) == 1) {
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//			  j=jj;
//			  if(outPixels > j)	j = outPixels; // use top boundary
			  j=jj+dy/2;
			  if(outPixels > j)	j = outPixels; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
			  {
//	            pDst[jj*dstStep+i] = (pDst[jj*dstStep+i] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
	            pDst[jj*dstStep+i] = (pDst[jj*dstStep+i] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
			  }
			}
		  }
#else
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[jj*dstStep+i] = (Fw8u)result;
			}
		  }
#endif
		  return fwStsNoErr;
		}
		else if (dx == 2) {  /* Vertical & horizontal interpolation */

//printf("<<<<< dx == 2");

#if 1
		  for (jj = -2; jj < roiSize.height+3; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[jj+2][i] = 0, x = -2; x < 4; x++)
			  {
//				tmp_res[j+2][i] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
				tmp_res[jj+2][i] += pSrc[max(outPixels,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
			  }
		  }

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//		    j=jj;
//			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
			  {
			    j=jj+y+2;
				if(outPixels > j)	j = outPixels; // use top boundary
//				result += tmp_res[j+y+2][i]*COEF[y+2];
				result += tmp_res[j][i]*COEF[y+2];
			  }
		      pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			} 
		  }

		  if ((dy&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++) 
			{
//			  j=jj;
//			  if(outPixels > j)	j = outPixels; // use top boundary
			  j=jj+2+dy/2;
			  if(outPixels > j)	j = outPixels; // use top boundary

			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[i][j+2+dy/2]+16)/32)) +1 )/2;
	//	          result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
		          result = (Fw8u)(max(0, min(255, (tmp_res[j][i]+16)/32)));
				  result += (int)pDst[jj*dstStep+i];
				  pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			  }
			}
		  }
#else
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[jj*dstStep+i] = (Fw8u)result;
			}
		  }
#endif
		}
		else if (dy == 2) {  /* Horizontal & vertical interpolation */

//printf("<<<<< dy == 2");

#if 1
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//			j=jj;
//			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[jj][i+2] = 0, y = -2; y < 4; y++)
			  {
			j=jj+y;
			if(outPixels > j)	j = outPixels; // use top boundary
//				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
//				tmp_res[j][i+2] += pSrc[max(outPixels,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
				tmp_res[jj][i+2] += pSrc[max(outPixels,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
			  }
		  }

			for (jj = 0; jj < roiSize.height; jj++) 
			{
			  j=jj;
			  if(outPixels > j)	j = outPixels; // use top boundary
			  for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += tmp_res[j][i+x+2]*COEF[x+2];
			  pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			}
		  }

		  if ((dx&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++) 
			{
			  j=jj;
			  if(outPixels > j)	j = outPixels; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
				result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				result += (int)pDst[jj*dstStep+i];
				pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			  }
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}
		else {  /* Diagonal interpolation */

//printf("<<<<< else");

#if 1
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//			j=jj;
//			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? jj : jj+1;
			  pres_y = max(0,min(maxold_y,pres_y));

			if(outPixels > pres_y)	pres_y = outPixels; // use top boundary

			  for (result = 0, x = -2; x < 4; x++)
//				result += pSrc[pres_y*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
				result += pSrc[pres_y*srcStep+max(outPixels,min(maxold_x,i+x))]*COEF[x+2];
			  pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
//			j=jj;
//			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
			  pres_x = max(0,min(maxold_x,pres_x));
			  for (result = 0, y = -2; y < 4; y++)
			  {
				j=jj+y;
				if(outPixels > j)	j = outPixels; // use top boundary

//				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
//				result += pSrc[max(outPixels,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
				result += pSrc[max(outPixels,min(maxold_y,j))*srcStep+pres_x]*COEF[y+2];
			  }
//			  pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (result+16)/32)) +1 ) / 2;
			  result = (Fw8u)(max(0, min(255, (result+16)/32)));
			  result += (int)pDst[jj*dstStep+i];
			  pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}
	}
	else // 	if(outPixels <= 0)	
	{
    if (dy == 0 && dx == 0) { /* No vertical interpolation */
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[j*dstStep+i] = (Fw8u)result;
			}
		  }
		  return fwStsNoErr;
	}

		if (dy == 0) { /* No vertical interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[j*dstStep+i] = (pDst[j*dstStep+i] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))] +1 )/2;
		          result =  (int)pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))];
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dx == 0) {  /* No horizontal interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//			   pDst[j*dstStep+i] = (pDst[j*dstStep+i] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
		          result =  (int)pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))];
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dx == 2) {  /* Vertical & horizontal interpolation */

		  for (j = -2; j < roiSize.height+3; j++) {
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[j+2][i] = 0, x = -2; x < 4; x++)
				tmp_res[j+2][i] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += tmp_res[j+y+2][i]*COEF[y+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			} 
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)) +1 )/2;
		          result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dy == 2) {  /* Horizontal & vertical interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += tmp_res[j][i+x+2]*COEF[x+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			}
		  }

		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
		          result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else {  /* Diagonal interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? j : j+1;
			  pres_y = max(0,min(maxold_y,pres_y));
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[pres_y*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
			  pres_x = max(0,min(maxold_x,pres_x));
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
//			  pDst[j*dstStep+i] = (Fw8u)(pDst[j*dstStep+i] + max(0, min(255, (result+16)/32)) +1 ) / 2;
			  result = (Fw8u)(max(0, min(255, (result+16)/32)));
			  result += (int)pDst[j*dstStep+i];
			  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			}
		  }

		}
	}
	return fwStsNoErr;

#endif

}
static  Fw8u* CopyBlockFromBottom( Fw8u *pRefPlane,
        Fw8u *TemporalPixels,
        Fw32s pitch,
        Fw32s temp_pitch,
        Fw32s outPixels,
        Fw32s xh,
        Fw32s yh,
        FwiSize roi)
    {
        Fw8u *pOut;
        Fw8u *pOutReturn;
        int i;

        pOutReturn = pOut = TemporalPixels;

        Fw32s padded_y = yh>0?3:0;
        Fw32s padded_x = xh>0?3:0;
        Fw32s sbheight = roi.height+padded_y*2;

        Fw32s num_outs = outPixels+padded_y;
        pOut-=padded_y*temp_pitch;
        pOut-=padded_x;
        pRefPlane-=padded_x;

        if (num_outs>sbheight) //starting point outside bottom boundary
        {
            num_outs = sbheight;
            pRefPlane-=(outPixels-roi.height+1)*pitch;//get boundary pixel location
            for(i=0;i<num_outs;i++,pOut+=temp_pitch)
                memcpy(pOut,pRefPlane,roi.width+2*padded_x);

        }
        else
        {
            pRefPlane -=padded_y*pitch;
            for(i=0;i<sbheight-num_outs;i++,pOut+=temp_pitch,pRefPlane+=pitch)
                memcpy(pOut,pRefPlane,roi.width+2*padded_x);
            pRefPlane-=pitch;
            for(i=0;i<num_outs;i++,pOut+=temp_pitch)
                memcpy(pOut,pRefPlane,roi.width+2*padded_x);
        }
        return pOutReturn;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
			srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s
			outPixels, FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;
	if( roiSize.width  != 4  && roiSize.width  != 8  && roiSize.width  != 16 &&
		roiSize.height != 4  &&	roiSize.height != 8  &&	roiSize.height != 16) return fwStsSizeErr;
#if 1

        Fw8u *pTemp;
        Fw8u TemporalPixels[32*24];
        pTemp=CopyBlockFromBottom((Fw8u *)pSrc,TemporalPixels+32*4,srcStep,32,outPixels,dx,dy,roiSize);
        return fwiInterpolateLuma_H264_8u_C1R(pTemp,32,pDst,dstStep,dx,dy,roiSize);

#else

	static const int COEF[6] = {1, -5, 20, 20, -5, 1 };
	int maxold_x,maxold_y, i, j, x, y;
//    int result = 0, result1, result2;
    int result = 0;
    int pres_x;
    int pres_y; 
//    int tmp_res[4][9];
    int tmp_res[25][25];

	maxold_x = roiSize.width-1;
	maxold_y = roiSize.height-1;

	// data actually used in interpixel interpolation at the bottom boundary when outPixels > 0
	if(outPixels > 0) 
	{ 
	    int jj, BottomLine = roiSize.height-outPixels-1; 

    if (dy == 0 && dx == 0) { /* No vertical interpolation */
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[jj*dstStep+i] = (Fw8u)result;
			}
		  }
		  return fwStsNoErr;
	}

		if (dy == 0) { /* No vertical interpolation */
#if 0
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
		    j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
	          pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		  if ((dx&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++)
			{
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
//				pDst[i*dstStep+j] = (pDst[i*dstStep+j] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))] +1 )/2;
	            pDst[jj*dstStep+i] = (pDst[jj*dstStep+i] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))] +1 )/2;
			}
		  }
		}
		else if (dx == 0) {  /* No horizontal interpolation */
#if 0
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary

			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
              pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dy&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++)
			{
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
//			    pDst[i*dstStep+j] = (pDst[i*dstStep+j] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
	            pDst[jj*dstStep+i] = (pDst[jj*dstStep+i] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}
		else if (dx == 2) {  /* Vertical & horizontal interpolation */
#if 0
		  for (jj = -2; jj < roiSize.height+3; jj++) 
		  {
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[j+2][i] = 0, x = -2; x < 4; x++)
				tmp_res[j+2][i] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
		  }

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += tmp_res[i][j+y+2]*COEF[y+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+512)/1024)));
		      pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			} 
		  }

		  if ((dy&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++)
			{
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[i][j+2+dy/2]+16)/32)) +1 )/2;
		          result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
				  result += (int)pDst[jj*dstStep+i];
				  pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			  }
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}
		else if (dy == 2) {  /* Horizontal & vertical interpolation */
#if 0
		  for (jj = 0; jj < roiSize.height; jj++) {
			j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
		  }

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
			j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += tmp_res[j][i+x+2]*COEF[x+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			  pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			}
		  }

		  if ((dx&1) == 1) {
			for (jj = 0; jj < roiSize.height; jj++)
			{
				j=jj;
				if(j > BottomLine)	j = BottomLine; // use top boundary
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
				result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				result += (int)pDst[jj*dstStep+i];
				pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			  }
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}
		else {  /* Diagonal interpolation */
#if 0
		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
			j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? j : j+1;
			  pres_y = max(0,min(maxold_y,pres_y));
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[pres_y*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
//	  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
			  pDst[jj*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (jj = 0; jj < roiSize.height; jj++) 
		  {
			j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
			  pres_x = max(0,min(maxold_x,pres_x));
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
//	  pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (result+16)/32)) +1 ) / 2;
			  result = (Fw8u)(max(0, min(255, (result+16)/32)));
			  result += (int)pDst[jj*dstStep+i];
			  pDst[jj*dstStep+i] = (Fw8u)((result+1)/2);
			}
		  }
#else
		 for (jj = 0; jj < roiSize.height; jj++) 
		 {
		   j=jj;
			if(j > BottomLine)	j = BottomLine; // use top boundary
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
			  pDst[jj*dstStep+i] = (Fw8u)result;
			}
		 }
#endif
		}

	}
	else //	if(outPixels <= 0) 
	{

    if (dy == 0 && dx == 0) { /* No vertical interpolation */
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  result=pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i))];
	          pDst[j*dstStep+i] = (Fw8u)result;
			}
		  }
		  return fwStsNoErr;
	}

		if (dy == 0) { /* No vertical interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
//	  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (pDst[i*dstStep+j] + pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))] +1 )/2;
		          result =  (int)pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+dx/2))];
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dx == 0) {  /* No horizontal interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//			   pDst[i*dstStep+j] = (pDst[i*dstStep+j] + pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))] +1 )/2;
		          result =  (int)pSrc[max(0,min(maxold_y,j+dy/2))*srcStep+max(0,min(maxold_x,i))];
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dx == 2) {  /* Vertical & horizontal interpolation */

		  for (j = -2; j < roiSize.height+3; j++) {
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[j+2][i] = 0, x = -2; x < 4; x++)
//				tmp_res[i][j+2] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
				tmp_res[j+2][i] += pSrc[max(0,min(maxold_y,j))*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, y = -2; y < 4; y++)
				result += tmp_res[j+y+2][i]*COEF[y+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			} 
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[i][j+2+dy/2]+16)/32)) +1 )/2;
		          result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else if (dy == 2) {  /* Horizontal & vertical interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
//				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
				tmp_res[j][i+2] += pSrc[max(0,min(maxold_y,j+y))*srcStep+max(0,min(maxold_x,i))]*COEF[y+2];
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  for (result = 0, x = -2; x < 4; x++)
				result += tmp_res[j][i+x+2]*COEF[x+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+512)/1024)));
			}
		  }

		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
			  {
//				pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32))+1)/2;
		          result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
		else {  /* Diagonal interpolation */

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? j : j+1;
			  pres_y = max(0,min(maxold_y,pres_y));
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[pres_y*srcStep+max(0,min(maxold_x,i+x))]*COEF[x+2];
//			  pDst[i*dstStep+j] = (Fw8u)(max(0, min(255, (result+16)/32)));
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
			  pres_x = max(0,min(maxold_x,pres_x));
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[max(0,min(maxold_y,j+y))*srcStep+pres_x]*COEF[y+2];
//			  pDst[i*dstStep+j] = (Fw8u)(pDst[i*dstStep+j] + max(0, min(255, (result+16)/32)) +1 ) / 2;
			  result = (Fw8u)(max(0, min(255, (result+16)/32)));
			  result += (int)pDst[j*dstStep+i];
			  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			}
		  }

		}
	}
	return fwStsNoErr;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateChromaTop_H264_8u_C1R)(const Fw8u* pSrc, 
	Fw32s srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, 
	Fw32s outPixels, FwiSize roiSize)
{

	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 8) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 7 || dy > 7) return fwStsBadArgErr;
	if( roiSize.width  != 2  && roiSize.width  != 4  && roiSize.width  != 8 &&
		roiSize.height != 2  &&	roiSize.height != 4  &&	roiSize.height != 8) return fwStsSizeErr;

#if 1
//    Fw8u *pTemp;
  //  Fw8u TemporalPixels[32*24];
    //pTemp=CopyBlockFromTop((Fw8u *)pSrc,srcStep,TemporalPixels+32*4,32,outPixels,dx,dy,roiSize);
    {
        const Fw8u *pRefPlane=pSrc;
		Fw8u TempBuffer[32*24];
        Fw8u *TemporalP=(TempBuffer+32*4);
        Fw32s Temp_Width=32;
        Fw32s xh=dx;
        Fw32s yh=dy;

        Fw8u *pOut;
        int i;

        pOut = TemporalP;
        Fw32s py = yh>0?3:0;
        Fw32s px = xh>0?3:0;

        pRefPlane += (srcStep*outPixels);

        Fw32s num_outs = outPixels+py;
        Fw32s EXheight = roiSize.height+py*2;

        pRefPlane-=px;
        pOut-=py*Temp_Width;
        pOut-=px;

        if (num_outs>EXheight) //starting point outside bottom boundary
        {
            num_outs = roiSize.height+2*py;
            for(i=0;i<num_outs;i++,pOut+=Temp_Width)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);
        }
        else
        {
            for(i=0;i<num_outs;i++,pOut+=Temp_Width)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);

            for(i=0;i<EXheight-num_outs;i++,pOut+=Temp_Width,pRefPlane+=srcStep)
                memcpy(pOut,pRefPlane,roiSize.width+2*px);
        }

	pSrc=TemporalP;
    srcStep=32;
 }
//    return fwiInterpolateChroma_H264_8u_C1R(pTemp,32,pDst,dstStep,dx,dy,roiSize);
    return fwiInterpolateChroma_H264_8u_C1R(pSrc,srcStep,pDst,dstStep,dx,dy,roiSize);


#else

//	int coef0, coef1, coef2, coef3, xInt, yInt, tmp;
	int coef0, coef1, coef2, coef3, tmp;

    /*
     * Get chroma prediction
     */

    /* Bilinear interpolation coefficients */
    coef0 = (8-(dx&7)) * (8-(dy&7));
    coef1 = (dx&7)     * (8-(dy&7));
    coef2 = (8-(dx&7)) * (dy&7);
    coef3 = (dx&7)     * (dy&7);

	if(outPixels > 0)
	{
		int jj;
//		for (int j = 0; j < roiSize.height;  j++) {
		for (jj = 0; jj < roiSize.height; jj++) 
		{
		    int j=jj;
			if(outPixels > j)	j = outPixels; // use top boundary
			for (int i = 0; i < roiSize.width;  i++) {
			 tmp = (coef0 * pSrc[j*srcStep + i  ] +
					coef1 * pSrc[j*srcStep + i+1] +
					coef2 * pSrc[(j+1)*srcStep + i  ] +
					coef3 * pSrc[(j+1)*srcStep + i+1] + 32) >> 6;
			 pDst[jj*dstStep+i] = (Fw8u) tmp;
		  }
		}
	}
	else 
	{
		/* Chroma vectors have 1/8 chroma pel precision */
		for (int j = 0; j < roiSize.height;  j++) {
		   for (int i = 0; i < roiSize.width;  i++) {
			 tmp = (coef0 * pSrc[j*srcStep + i  ] +
					coef1 * pSrc[j*srcStep + i+1] +
					coef2 * pSrc[(j+1)*srcStep + i  ] +
					coef3 * pSrc[(j+1)*srcStep + i+1] + 32) >> 6;
			 pDst[j*dstStep+i] = (Fw8u) tmp;
		  }
		}
	}
	return fwStsNoErr;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateChromaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
			srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s
			outPixels, FwiSize roiSize)
{

	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 8) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 7 || dy > 7) return fwStsBadArgErr;
	if( roiSize.width  != 2  && roiSize.width  != 4  && roiSize.width  != 8 &&
		roiSize.height != 2  &&	roiSize.height != 4  &&	roiSize.height != 8) return fwStsSizeErr;

//	int coef0, coef1, coef2, coef3, xInt, yInt, tmp;
	int coef0, coef1, coef2, coef3, tmp;

    /*
     * Get chroma prediction
     */

    /* Bilinear interpolation coefficients */
    coef0 = (8-(dx&7)) * (8-(dy&7));
    coef1 = (dx&7)     * (8-(dy&7));
    coef2 = (8-(dx&7)) * (dy&7);
    coef3 = (dx&7)     * (dy&7);

	if(outPixels > 0)
	{
	    int jj, BottomLine = roiSize.height-outPixels-1; 

		/* Chroma vectors have 1/8 chroma pel precision */
//		for (int j = 0; j < roiSize.height;  j++) {
		for (jj = 0; jj < roiSize.height; jj++) 
		{
			int j=jj;
			if(j >= BottomLine){
				j = BottomLine; // use top boundary
				for (int i = 0; i < roiSize.width;  i++) {
					tmp = 	coef0 * (int)pSrc[j*srcStep + i  ];
					tmp+=	(coef1 * (int)pSrc[j*srcStep + i+1]);
					tmp+=	(coef2 * (int)pSrc[j*srcStep + i  ]);
					tmp+=	(coef3 * (int)pSrc[j*srcStep + i+1]);
					pDst[jj*dstStep+i] = (Fw8u) ((tmp+32)/64);
				}
			}
			else
			{
				for (int i = 0; i < roiSize.width;  i++) {
					tmp = 	coef0 * (int)pSrc[j*srcStep + i  ];
					tmp+=	(coef1 * (int)pSrc[j*srcStep + i+1]);
					tmp+=	(coef2 * (int)pSrc[(j+1)*srcStep + i  ]);
					tmp+=	(coef3 * (int)pSrc[(j+1)*srcStep + i+1]);
					pDst[jj*dstStep+i] = (Fw8u) ((tmp+32)/64);
				}
			}
		}
	}
	else 
	{
		/* Chroma vectors have 1/8 chroma pel precision */
		if(outPixels == 0)
		{
			int j;
			for (j = 0; j < roiSize.height-1;  j++) {
			   for (int i = 0; i < roiSize.width;  i++) {
				 tmp = coef0 * (int)pSrc[j*srcStep + i  ];
				 tmp+= coef1 * (int)pSrc[j*srcStep + i+1];
				 tmp+= coef2 * (int)pSrc[(j+1)*srcStep + i  ];
				 tmp+= coef3 * (int)pSrc[(j+1)*srcStep + i+1];
				 pDst[j*dstStep+i] = (Fw8u) ((tmp+32)/64);
			  }
			}
			// last line
//			for (int j = 0; j < roiSize.height;  j++) {
			   for (int i = 0; i < roiSize.width;  i++) {
				 tmp = coef0 * (int)pSrc[j*srcStep + i  ];
				 tmp+= coef1 * (int)pSrc[j*srcStep + i+1];
				 tmp+= coef2 * (int)pSrc[j*srcStep + i ];
				 tmp+= coef3 * (int)pSrc[j*srcStep + i+1];
				 pDst[j*dstStep+i] = (Fw8u) ((tmp+32)/64);
			  }
//			}
		}
		else
		{
			for (int j = 0; j < roiSize.height;  j++) {
			   for (int i = 0; i < roiSize.width;  i++) {
				 tmp = coef0 * (int)pSrc[j*srcStep + i  ];
				 tmp+= coef1 * (int)pSrc[j*srcStep + i+1];
				 tmp+= coef2 * (int)pSrc[(j+1)*srcStep + i  ];
				 tmp+= coef3 * (int)pSrc[(j+1)*srcStep + i+1];
				 pDst[j*dstStep+i] = (Fw8u) ((tmp+32)/64);
			  }
			}
		}
	}

	return fwStsNoErr;
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiUniDirWeightBlock_H264_8u_C1IR)(Fw8u *pSrcDst, Fw32u srcDstStep,
			Fw32u ulog2wd, Fw32s iWeight, Fw32s iOffset, FwiSize roi)
{
	int tmp;

	if(pSrcDst == NULL) return fwStsNullPtrErr;
	if((int)srcDstStep <= roi.width) return fwStsStepErr;

	if(ulog2wd >= 1)
	{
		for (int j = 0; j < roi.height;  j++) {
		   for (int i = 0; i < roi.width;  i++) {
			 tmp = (iWeight*pSrcDst[j*roi.width+i] + (1<<(ulog2wd-1)) ) >> ulog2wd;
			 pSrcDst[j*srcDstStep+i] = (Fw8u) clip1(tmp + iOffset);
		  }
		}
	}
	else
	{
		for (int j = 0; j < roi.height;  j++) {
		   for (int i = 0; i < roi.width;  i++) {
			 tmp = iWeight*pSrcDst[j*roi.width+i];
			 pSrcDst[j*srcDstStep+i] = (Fw8u) clip1(tmp + iOffset);
		  }
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBiDirWeightBlock_H264_8u_P2P1R)(Fw8u *pSrc1, Fw8u *pSrc2, Fw8u *pDst, 
				Fw32u srcStep, Fw32u dstStep, Fw32u ulog2wd, Fw32s iWeight1,
				Fw32s iOffset1, Fw32s iWeight2, Fw32s iOffset2, FwiSize roi)
{
	if(pSrc1 == NULL || pSrc2 == NULL || pDst == NULL) return fwStsNullPtrErr;
	if((int)srcStep <= roi.width) return fwStsStepErr;

	// Source2 [x,y]=
	//Clip1( weight1*Source1[x,y]+ weight2*Source2[x,y]+1+( shift-1)) >> shift + offset)
	int tmp;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7, xmm5, xmm6;

	if(Dispatch_Type==DT_SSE2 && roi.width > 8)
	{
		for (int j = 0; j < roi.height;  j++) {
			int i=0;
		   for (; i < (int)roi.width;  i+=8) {
				xmm0 = _mm_loadl_epi64( (__m128i*)(pSrc1+j*srcStep+i) );
				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrc2+j*srcStep+i) );
				xmm7  = _mm_set1_epi8(0); 
   				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);
   				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);

 				xmm5 = _mm_set1_epi16((short)iWeight1); 
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
				xmm6 = _mm_set1_epi16((short)iWeight2); 
				xmm1 = _mm_mullo_epi16(xmm1, xmm6);
				xmm0 = _mm_add_epi16(xmm0, xmm1);	// +

 				xmm5 = _mm_set1_epi16((short)(1<<(ulog2wd-1))); 
				xmm0 = _mm_add_epi16(xmm0, xmm5);	// add (1<<(ulog2wd-1))

                __m128i ulog2wd_x1 = _mm_cvtsi32_si128(ulog2wd+1);
				xmm0 = _mm_sra_epi16(xmm0, ulog2wd_x1);		// >>(ulog2wd+1)

 				xmm5 = _mm_set1_epi16((short)((iOffset1+iOffset2+1)>>2)); 
				xmm0 = _mm_add_epi16(xmm0, xmm5);	// add ((iOffset1+iOffset2+1)>>2)

				xmm0 = _mm_packus_epi16(xmm0, xmm0);

				_mm_storel_epi64( (__m128i*)(pDst+j*dstStep+i), xmm0 );
		   }
		   for (; i < roi.width;  i++) {
			 tmp = iWeight1*pSrc1[j*srcStep+i]+
				   iWeight2*pSrc2[j*srcStep+i]+(1<<(ulog2wd-1));
			 tmp = (tmp>>(ulog2wd+1)) + ((iOffset1+iOffset2+1)>>2);

			 pDst[j*dstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}
	else
	{
		for (int j = 0; j < roi.height;  j++) {
		   for (int i = 0; i < roi.width;  i++) {
			 tmp = iWeight1*pSrc1[j*srcStep+i]+
				   iWeight2*pSrc2[j*srcStep+i]+(1<<(ulog2wd-1));
			 tmp = (tmp>>(ulog2wd+1)) + ((iOffset1+iOffset2+1)>>2);

			 pDst[j*dstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBiDirWeightBlockImplicit_H264_8u_P2P1R)(Fw8u *pSrc1, Fw8u
*pSrc2, Fw8u *pDst, Fw32u srcStep, Fw32u dstStep, Fw32s iWeight1, Fw32s
iWeight2, FwiSize roi)
{
	if(pSrc1 == NULL || pSrc2 == NULL || pDst == NULL) return fwStsNullPtrErr;
	if((int)srcStep <= roi.width) return fwStsStepErr;

	// Source2 [x,y]=
	//Clip1( weight1*Source1[x,y]+ weight2*Source2[x,y]+1+( shift-1)) >> shift + offset)
	int tmp;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm7, xmm5, xmm6;

	if(Dispatch_Type==DT_SSE2 && roi.width > 8)
	{
		for (int j = 0; j < (int)roi.height;  j++) {
			int i=0;
		   for (; i < (int)roi.width;  i+=8) {
				xmm0 = _mm_loadl_epi64( (__m128i*)(pSrc1+j*srcStep+i) );
				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrc2+j*srcStep+i) );
				xmm7  = _mm_set1_epi8(0); 
   				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);
   				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);

 				xmm5 = _mm_set1_epi16((short)iWeight1); 
				xmm0 = _mm_mullo_epi16(xmm0, xmm5);
				xmm6 = _mm_set1_epi16((short)iWeight2); 
				xmm1 = _mm_mullo_epi16(xmm1, xmm6);
				xmm0 = _mm_add_epi16(xmm0, xmm1);	// +

 				xmm5 = _mm_set1_epi16((short)32); 
				xmm0 = _mm_add_epi16(xmm0, xmm5);	// add 32
				xmm0 = _mm_srai_epi16(xmm0, 6);		// >>6
				xmm0 = _mm_packus_epi16(xmm0, xmm0);
				_mm_storel_epi64( (__m128i*)(pDst+j*dstStep+i), xmm0 );
		   }
		   for (; i < roi.width;  i++) {
			 tmp = ((iWeight1*pSrc1[j*srcStep+i]+
				     iWeight2*pSrc2[j*srcStep+i]+32)>>6);
			 pDst[j*dstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}
	else
	{
		for (int j = 0; j < roi.height;  j++) {
		   for (int i = 0; i < roi.width;  i++) {
			 tmp = ((iWeight1*pSrc1[j*srcStep+i]+
				     iWeight2*pSrc2[j*srcStep+i]+32)>>6);
			 pDst[j*dstStep+i] = (Fw8u) clip1(tmp);
		  }
		}
	}
	return fwStsNoErr;
}
#endif

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
