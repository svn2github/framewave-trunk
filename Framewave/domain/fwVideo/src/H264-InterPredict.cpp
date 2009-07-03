/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.

The following interpolation routines are based on ITU-H.264 doc 3/2005.
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

#define avg16_sse2(pesi, pedi)	\
{\
	xmm0 = _mm_loadu_si128( (__m128i*)(pesi) );\
	xmm1 = _mm_loadu_si128( (__m128i*)(pedi) );\
	xmm0 = _mm_avg_epu8(xmm0, xmm1);\
	_mm_storeu_si128( (__m128i*)(pesi), xmm0 );\
}

// Based ITU-H.264 doc 8.4.2.2.1
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLuma_H264_8u_C1R)(
								const Fw8u* pSrc, Fw32s srcStep,
									  Fw8u* pDst, Fw32s dstStep, 
									  Fw32s dx,   Fw32s dy, 
									  FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;

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
				result = filter_h(pSrc+j*srcStep+i);	
				pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }
		  if ((dx&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
				pDst[j*dstStep+i] = (pDst[j*dstStep+i]+pSrc[j*srcStep+(i+dx/2)]+1)/2;
		  }
		}
    }
    else if (dx == 0) {  /* No horizontal interpolation */
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
				result = filter_v(pSrc+j*srcStep+i, srcStep);	
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));

			}
		  }

		  if ((dy&1) == 1) {
			for (j = 0; j < roiSize.height; j++)
			  for (i = 0; i < roiSize.width; i++)
				 pDst[j*dstStep+i] = (pDst[j*dstStep+i] + pSrc[(j+dy/2)*srcStep+i] +1 )/2;
		  }
		}
    }
    else if (dx == 2) {  /* Vertical & horizontal interpolation */
		{
		  for (j = -2; j < roiSize.height+3; j++) {
			for (i = 0; i < roiSize.width; i++)
			  for (tmp_res[j+2][i] = 0, x = -2; x < 4; x++)
				tmp_res[j+2][i] += pSrc[j*srcStep+i+x]*COEF[x+2];
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
				  result = (Fw8u)(max(0, min(255, (tmp_res[j+2+dy/2][i]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
    }
    else if (dy == 2) {  /* Horizontal & vertical interpolation */
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = -2; i <roiSize.width+3; i++)
			  for (tmp_res[j][i+2] = 0, y = -2; y < 4; y++)
				tmp_res[j][i+2] += pSrc[(j+y)*srcStep+i]*COEF[y+2];
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
				  result = (Fw8u)(max(0, min(255, (tmp_res[j][i+2+dx/2]+16)/32)));
				  result += (int)pDst[j*dstStep+i];
				  pDst[j*dstStep+i] = (Fw8u)((result+1)/2);
			  }
		  }
		}
    }
    else {  /* Diagonal interpolation */
		{
		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_y = dy == 1 ? j : j+1;
			  for (result = 0, x = -2; x < 4; x++)
				result += pSrc[pres_y*srcStep+i+x]*COEF[x+2];
			  pDst[j*dstStep+i] = (Fw8u)(max(0, min(255, (result+16)/32)));
			}
		  }

		  for (j = 0; j < roiSize.height; j++) {
			for (i = 0; i < roiSize.width; i++) {
			  pres_x = dx == 1 ? i : i+1;
			  for (result = 0, y = -2; y < 4; y++)
				result += pSrc[(j+y)*srcStep+pres_x]*COEF[y+2];
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
// Based ITU-H.264 doc 8.4.2.2.2
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

	int coef0, coef1, coef2, coef3, tmp;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7;

    /*Get chroma prediction  */
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
// Based ITU-H.264 doc 8.4.2.2.1
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaTop_H264_8u_C1R)(const Fw8u* pSrc, Fw32s srcStep,
			Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s outPixels, 
			FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;
	if( roiSize.width  != 4  && roiSize.width  != 8  && roiSize.width  != 16 &&
		roiSize.height != 4  &&	roiSize.height != 8  &&	roiSize.height != 16) return fwStsSizeErr;

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

// Based ITU-H.264 doc 8.4.2.2.1
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateLumaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
			srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s
			outPixels, FwiSize roiSize)
{
	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 16) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 3 || dy > 3) return fwStsBadArgErr;
	if( roiSize.width  != 4  && roiSize.width  != 8  && roiSize.width  != 16 &&
		roiSize.height != 4  &&	roiSize.height != 8  &&	roiSize.height != 16) return fwStsSizeErr;

        Fw8u *pTemp;
        Fw8u TemporalPixels[32*24];
        pTemp=CopyBlockFromBottom((Fw8u *)pSrc,TemporalPixels+32*4,srcStep,32,outPixels,dx,dy,roiSize);
        return fwiInterpolateLuma_H264_8u_C1R(pTemp,32,pDst,dstStep,dx,dy,roiSize);
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
    return fwiInterpolateChroma_H264_8u_C1R(pSrc,srcStep,pDst,dstStep,dx,dy,roiSize);
}
// Based ITU-H.264 doc 8.4.2.2.2
FwStatus PREFIX_OPT(OPT_PREFIX, fwiInterpolateChromaBottom_H264_8u_C1R)(const Fw8u* pSrc, Fw32s
			srcStep, Fw8u* pDst, Fw32s dstStep, Fw32s dx, Fw32s dy, Fw32s
			outPixels, FwiSize roiSize)
{

	if(pSrc == NULL) return fwStsNullPtrErr;
	if(srcStep < 8) return fwStsStepErr;
	if(dx < 0 || dy < 0 || dx > 7 || dy > 7) return fwStsBadArgErr;
	if( roiSize.width  != 2  && roiSize.width  != 4  && roiSize.width  != 8 &&
		roiSize.height != 2  &&	roiSize.height != 4  &&	roiSize.height != 8) return fwStsSizeErr;

	int coef0, coef1, coef2, coef3, tmp;
    /* Get chroma prediction    */
    /* Bilinear interpolation coefficients */
    coef0 = (8-(dx&7)) * (8-(dy&7));
    coef1 = (dx&7)     * (8-(dy&7));
    coef2 = (8-(dx&7)) * (dy&7);
    coef3 = (dx&7)     * (dy&7);

	if(outPixels > 0)
	{
	    int jj, BottomLine = roiSize.height-outPixels-1; 

		/* Chroma vectors have 1/8 chroma pel precision */
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
			   for (int i = 0; i < roiSize.width;  i++) {
				 tmp = coef0 * (int)pSrc[j*srcStep + i  ];
				 tmp+= coef1 * (int)pSrc[j*srcStep + i+1];
				 tmp+= coef2 * (int)pSrc[j*srcStep + i ];
				 tmp+= coef3 * (int)pSrc[j*srcStep + i+1];
				 pDst[j*dstStep+i] = (Fw8u) ((tmp+32)/64);
			  }
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

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
