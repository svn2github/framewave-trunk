/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
//#include "fwImage.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(100)

/*
The first descriptors FF, FH, HF, HH show whether prediction is accurate to 
full pel (F) or to half a pel (H). The first letter indicates the accuracy in 
horizontal direction and the second in vertical direction.
Video Coding 16 16-3
Most of general functions use parameter mcType (see enumeration FWVC_MC_APX). 
It is used for calculating prediction on the basis of block in the reference frame. 
See Figure 16-1. If mcType = FWVC_MC_APX_FF, the reference block is used as the 
prediction block. If mcType = FWVC_MC_APX_FH or FWVC_MC_APX_HF, each element of 
the prediction block is calculated as average of two elements of the reference block.
If mcType = FWVC_MC_APX_HH, each element of the prediction block is calculated as
average of four elements of the reference block.
*/
typedef enum _FWVC_MC_APX{
FWVC_MC_APX_FF = 0x0,
FWVC_MC_APX_FH = 0x4,
FWVC_MC_APX_HF = 0x8,
FWVC_MC_APX_HH = 0x0c
}FWVC_MC_APX;

/*
enum DispatchType
{
	DT_REFR,
	DT_SSE1,
	DT_SSE2,
	DT_SSE3,
	DT_COUNT
};
*/
static int Dispatch_Type = DT_REFR;
void static Dispatch_ME_Type_func(int DT_type)
{
	Dispatch_Type = DT_type;
}

///Evaluation of difference between current predicted and reference blocks

/*
Description:
The function fwiGetDiff16x16_8u16s_C1 evaluates the difference between the current 
block of specified size and the reference one. The result is stored in blocks pDstDiff 
and pDstPredictor. The latter stores some additional information about the coding block.
This information is used for encoding next blocks that refer to the current one. 
This method helps to decrease the number of encoding errors. Encoding is performed 
accurate to half a pel and rounding must be specified.
*/

void  static Diff_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
					  const Fw8u* pSrcRef,  Fw32s srcRefStep,
					        Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height)
{
	int i, j;
	Fw16s* pd = pDstDiff;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  (Fw16s)(*(pSrcCur+j) - *(pSrcRef+j));
		}
	}
}

void  static Diff_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
						 const Fw8u* pSrcRef,  Fw32s srcRefStep,
						       Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7;
	Fw16s* pd = pDstDiff;
	
	xmm7  = _mm_set1_epi8(0);    // zero;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_sub_epi16(xmm2, xmm3);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}

		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
	Diff_8u16s_C1_C( pSrcCur,  srcCurStep,
					 pSrcRef,  srcRefStep,
					 pDstDiff, dstDiffStep, 
						width,	height);
		break;
	} 
}

void  static Pred_FF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
				 			Fw32s roundControl)
{
	int i, j; 
	Fw16s* pd = pDstPredictor;

	roundControl;
	pSrcCur;
	srcCurStep;
	for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  *(pSrcRef+j);
		} 
	}
}

void  static Pred_FF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm2, xmm7;
	Fw16s* pd = pDstPredictor;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	roundControl;
//	pSrcRef;
//	srcRefStep;
	pSrcCur;
	srcCurStep;
	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
	
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16
		
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
		
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			a    = (int *)pSrcRef;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
		
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_FF_8u16s_C1_C( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);
		break;
	}
}

void  static Pred_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;
	pSrcCur;
	srcCurStep;
	for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) = (Fw16s)(*(pSrcRef+j) + *(pSrcRef+j+srcRefStep) + roundControl)/2;
		}
	}
}

void  static Pred_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7, xmm6;
	Fw16s* pd = pDstPredictor;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
//	pSrcRef;
//	srcRefStep;
	pSrcCur;
	srcCurStep;
	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcRefStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8+srcRefStep) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcRefStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			a    = (int *)pSrcRef;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRef+srcRefStep);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_FH_8u16s_C1_C( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);
		break;
	}
}

void static Pred_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;

	pSrcCur;
	srcCurStep;
	for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  (Fw16s)(*(pSrcRef+j) + *(pSrcRef+j+1) + roundControl)/2;
		}
	}
}

void  static Pred_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7, xmm6;
	Fw16s* pd = pDstPredictor;

	pSrcCur;
	srcCurStep;
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8+1) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			a    = (int *)pSrcRef;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRef+1);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_HF_8u16s_C1_C( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);
		break;
	}
}

void  static Pred_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						  int    width,		 int     height,
							Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;

	pSrcCur;
	srcCurStep;
	for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  (Fw16s)(*(pSrcRef+j) + 
								*(pSrcRef+j+1) + 
								*(pSrcRef+j+srcRefStep)  + 
								*(pSrcRef+j+1+srcRefStep) + roundControl*2)/4;
		}
	}
}


void  static Pred_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
						    int    width,		 int     height,
							Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6;
	Fw16s* pd = pDstPredictor;

	pSrcCur;
	srcCurStep;
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcRefStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1+srcRefStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			
			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+9) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcRefStep+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+9+srcRefStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm4);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm5);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcRefStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1+srcRefStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2)
		{
			a    = (int *)pSrcRef;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRef+1);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			a    = (int *)(pSrcRef+srcRefStep);
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRef+srcRefStep+1);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_HH_8u16s_C1_C( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);

		break;
	}
}

FwStatus  static iGetDiff_8u16s_C1(const Fw8u* pSrcCur,      Fw32s srcCurStep,
								const Fw8u* pSrcRef,	   Fw32s srcRefStep, 
								Fw16s*		 pDstDiff,	   Fw32s dstDiffStep, 
								Fw16s*		 pDstPredictor,Fw32s dstPredictorStep, 
								Fw32s		 mcType,	   Fw32s roundControl,
						  int    width,		 int     height)
{
//	int i, j;
	Fw8u  *ps,  *pr;
	Fw16s *pdd, *pdp ;

	if (pSrcCur == NULL || pSrcRef == NULL || pDstDiff == NULL ) return fwStsNullPtrErr;

	ps = (Fw8u *) pSrcCur;
	pr = (Fw8u *) pSrcRef;
	pdd= (Fw16s*) pDstDiff;
	pdp= (Fw16s*) pDstPredictor;


	switch( Dispatch_Type )
	{
		case DT_SSE2:    	    
			Diff_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdd, dstDiffStep, width, height);
			break;
		default:
			Diff_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdd, dstDiffStep, width, height);
			break;
	}

	if(pdp != NULL)
	{
		ps = (Fw8u *) pSrcCur;
		pdp= (Fw16s*) pDstPredictor;
		switch(mcType)
		{
			case FWVC_MC_APX_FF:
				if( Dispatch_Type == DT_SSE2)
				    Pred_FF_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				else
				    Pred_FF_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_FH:
				if( Dispatch_Type == DT_SSE2)
				    Pred_FH_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				else
				    Pred_FH_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_HF:
				if( Dispatch_Type == DT_SSE2)
				    Pred_HF_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				else
				    Pred_HF_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_HH:
				if( Dispatch_Type == DT_SSE2)
				    Pred_HH_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				else
				    Pred_HH_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdp, dstPredictorStep, width, height, roundControl);
				break;
			default:
				break;
		}
	}
	return fwStsNoErr;
}


void  static Pred_FF_8u16s_C1_C_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						    Fw16s* pDstDiff,Fw32s dstDiffStep, 
							Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							int    width,		 int     height,
							Fw32s roundControl)
{
	int i, j; 
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf = pDstDiff;

	roundControl;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  *(pSrcCur+j);
			*(pdf+j) =  (Fw16s)(*(pSrcCur+j) - *(pSrcRef+j));
		} 
	}
}
void  static Pred_FF_8u16s_C1_SSE2_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
							  const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7;
	Fw16s* pdf = pDstDiff;
	Fw16s* pd = pDstPredictor;

	xmm7  = _mm_set1_epi8(0);    // zero;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pdf+=dstDiffStep/2, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)pdf, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits

			xmm2 = _mm_sub_epi16(xmm2, xmm3);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)(pdf+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pdf+=dstDiffStep/2, pd+=dstPredictorStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storeu_si128( (__m128i*)pdf, xmm0 ); // store 64 bits
		}

		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pdf+=dstDiffStep/2, pd+=dstPredictorStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits

			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
			
			_mm_storel_epi64( (__m128i*)pdf, xmm0 ); // store 64 bits
		}
		break;
	default:
	Pred_FF_8u16s_C1_C_comb( pSrcCur,  srcCurStep,
					 pSrcRef,  srcRefStep,
					 pDstDiff, dstDiffStep, 
					 pDstPredictor, dstPredictorStep, 
						width,	height, roundControl);

		break;
	} 
}
void  static Pred_FH_8u16s_C1_C_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) = (Fw16s)(*(pSrcCur+j) + *(pSrcCur+j+srcCurStep) + roundControl)/2;
			*(pdf+j) =  (Fw16s)(*(pSrcCur+j) - *(pSrcRef+j));
		}
	}
}
void  static Pred_FH_8u16s_C1_SSE2_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	pSrcRef;
	srcRefStep;
	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)pdf, xmm4 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef+8 );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf+8), xmm4 ); // store 64 bits diff

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8+srcCurStep) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf), xmm4 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			a    = (int *)pSrcRef;
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf), xmm4 ); // store 64 bits diff

			a    = (int *)(pSrcCur+srcCurStep);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_FH_8u16s_C1_C_comb( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
							pDstDiff, dstDiffStep, 
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);
		break;
	}
}
void  static Pred_HF_8u16s_C1_C_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  (Fw16s)(*(pSrcCur+j) + *(pSrcCur+j+1) + roundControl)/2;
			*(pdf+j) =  (Fw16s)(*(pSrcCur+j) - *(pSrcRef+j));
		}
	}
}

void  static Pred_HF_8u16s_C1_SSE2_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)pdf, xmm4 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm4 = xmm2;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef+8 );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf+8), xmm4 ); // store 64 bits diff

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8+1) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
			
			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			xmm5 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf), xmm4 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm4 = xmm0;
			a    = (int *)pSrcRef;
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16
			xmm4 = _mm_sub_epi16(xmm4, xmm5);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf), xmm4 ); // store 64 bits diff

			a    = (int *)(pSrcCur+1);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_HF_8u16s_C1_C_comb( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
						    pDstDiff, dstDiffStep, 
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);
		break;
	}
}

void  static Pred_HH_8u16s_C1_C_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i, j;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pd+j) =  (Fw16s)(*(pSrcCur+j) + 
								*(pSrcCur+j+1) + 
								*(pSrcCur+j+srcCurStep)  + 
								*(pSrcCur+j+1+srcCurStep) + roundControl*2)/4;
			*(pdf+j) =  (Fw16s)(*(pSrcCur+j) - *(pSrcRef+j));
		}
	}
}


void  static Pred_HH_8u16s_C1_SSE2_comb( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							const Fw8u* pSrcRef,  Fw32s srcRefStep,
						      Fw16s* pDstDiff,Fw32s dstDiffStep, 
							  Fw16s* pDstPredictor, Fw32s dstPredictorStep, 
							  int    width,		 int     height,
							  Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6, xmm8, xmm9;
	Fw16s* pd = pDstPredictor;
	Fw16s* pdf= pDstDiff;
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm8 = xmm0;
			xmm9 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm9 = _mm_unpacklo_epi8(xmm9, xmm7);	// pr u8->u16
			xmm8 = _mm_sub_epi16(xmm8, xmm9);		// ps-pr
			_mm_storeu_si128( (__m128i*)pdf, xmm8 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4
			
			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
			
			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm8 = xmm2;
			xmm9 = _mm_loadl_epi64( (__m128i*)pSrcRef+8 );
			xmm9 = _mm_unpacklo_epi8(xmm9, xmm7);	// pr u8->u16
			xmm8 = _mm_sub_epi16(xmm8, xmm9);		// ps-pr
			_mm_storeu_si128( (__m128i*)(pdf+8), xmm8 ); // store 64 bits diff


			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+9) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+9+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm4);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm5);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

			_mm_storeu_si128( (__m128i*)(pd+8), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm8 = xmm0;
			xmm9 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm9 = _mm_unpacklo_epi8(xmm9, xmm7);	// pr u8->u16
			xmm8 = _mm_sub_epi16(xmm8, xmm9);		// ps-pr
			_mm_storeu_si128( (__m128i*)pdf, xmm8 ); // store 64 bits diff

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

			_mm_storeu_si128( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, pd+=dstPredictorStep/2, pdf+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm8 = xmm0;
			a    = (int *)pSrcRef;
			xmm9 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm9 = _mm_unpacklo_epi8(xmm9, xmm7);	// ps u8->u16
			xmm8 = _mm_sub_epi16(xmm8, xmm9);		// ps-pr
			_mm_storeu_si128( (__m128i*)pdf, xmm8 ); // store 64 bits diff

			a    = (int *)(pSrcCur+1);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			a    = (int *)(pSrcCur+srcCurStep);
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcCur+srcCurStep+1);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4
			
			_mm_storel_epi64( (__m128i*)pd, xmm0 ); // store 64 bits
		}
		break;
	default:
		Pred_HH_8u16s_C1_C_comb( pSrcCur, srcCurStep,
 							pSrcRef, srcRefStep,
						    pDstDiff, dstDiffStep, 
							pDstPredictor, dstPredictorStep, 
						    width,	height,
				 			roundControl);

		break;
	}
}
FwStatus  static iGetDiff_8u16s_C1_comb(const Fw8u* pSrcCur,      Fw32s srcCurStep,
								const Fw8u* pSrcRef,	   Fw32s srcRefStep, 
								Fw16s*		 pDstDiff,	   Fw32s dstDiffStep, 
								Fw16s*		 pDstPredictor,Fw32s dstPredictorStep, 
								Fw32s		 mcType,	   Fw32s roundControl,
						  int    width,		 int     height)
{
//	int i, j;
	Fw8u  *ps,  *pr;
	Fw16s *pdd, *pdp ;

	if (pSrcCur == NULL || pSrcRef == NULL || pDstDiff == NULL ) return fwStsNullPtrErr;

	ps = (Fw8u *) pSrcCur;
	pr = (Fw8u *) pSrcRef;
	pdd= (Fw16s*) pDstDiff;
	pdp= (Fw16s*) pDstPredictor;

	if(pdp != NULL)
	{
		switch(mcType)
		{
			case FWVC_MC_APX_FF:
				if( Dispatch_Type == DT_SSE2)
					Pred_FF_8u16s_C1_SSE2_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				else
					Pred_FF_8u16s_C1_C_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_FH:
				if( Dispatch_Type == DT_SSE2)
					Pred_FH_8u16s_C1_SSE2_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				else
					Pred_FH_8u16s_C1_C_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_HF:
				if( Dispatch_Type == DT_SSE2)
					Pred_HF_8u16s_C1_SSE2_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				else
					Pred_HF_8u16s_C1_C_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				break;
			case FWVC_MC_APX_HH:
				if( Dispatch_Type == DT_SSE2)
					Pred_HH_8u16s_C1_SSE2_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				else
					Pred_HH_8u16s_C1_C_comb( ps, srcCurStep, pr, srcRefStep, pdd,dstDiffStep, pdp,dstPredictorStep, width, height, roundControl);
				break;
			default:
				break;
		}
	}
	else
	{
		switch( Dispatch_Type )
		{
			case DT_SSE2:    	    
				Diff_8u16s_C1_SSE2( ps, srcCurStep, pr, srcRefStep, pdd, dstDiffStep, width, height);
				break;
			default:
				Diff_8u16s_C1_C( ps, srcCurStep, pr, srcRefStep, pdd, dstDiffStep, width, height);
				break;
		}
	}
	return fwStsNoErr;
}




FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x16_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{

	if(srcCurStep < 16 || srcRefStep < 16 || dstDiffStep < 32 || dstPredictorStep < 32) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;


#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 16, 16) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 16, 16) ;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x8_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{
	if(srcCurStep < 16 || srcRefStep < 16 || dstDiffStep < 32 || dstPredictorStep < 32) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 16, 8) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 16, 8) ;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x8_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStep < 8 || dstDiffStep < 16 || dstPredictorStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 8) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 8) ;
#endif
}
 

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x16_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStep < 8 || dstDiffStep < 16 || dstPredictorStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 16) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 16) ;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x4_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStep < 8 || dstDiffStep < 16 || dstPredictorStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 4) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 4) ;
#endif
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff4x4_8u16s_C1 )(const Fw8u* pSrcCur, Fw32s srcCurStep,
									 const Fw8u* pSrcRef, Fw32s srcRefStep, 
									 Fw16s* pDstDiff,	   Fw32s dstDiffStep, 
									 Fw16s* pDstPredictor,Fw32s dstPredictorStep, 
									 Fw32s mcType,		   Fw32s roundControl)
{
	if(srcCurStep < 4 || srcRefStep < 4 || dstDiffStep < 8 || dstPredictorStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

#if 0
	return iGetDiff_8u16s_C1_comb (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 8, 4) ;
#else
	return iGetDiff_8u16s_C1 (pSrcCur, srcCurStep,
								 pSrcRef, srcRefStep, 
								 pDstDiff, dstDiffStep, 
								 pDstPredictor,dstPredictorStep, 
								 mcType, roundControl, 4, 4) ;
#endif
}
/*
Description
This function is declared in the fwvc.h header file. The function fwiGetDiff16x16B_8u16s_C1
evaluates the difference between the current block and the mean of two reference 
blocks of specified size. One of the reference blocks is called forward and belongs 
to the previous frame in accordance with the type of motion compensation. The other
block is called backward and belongs to one of the following frames. The result is 
stored in block pDstDiff. Encoding is performed accurate to half a pel and rounding 
must be specified.
*/
void  static PredF_FF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								Fw32s roundControl)
{
	int i, j;
	roundControl;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j++)
		{
			*(pSrcRef+j) =  *(pSrcCur+j);
		}
	}
}

void  static PredF_FF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1;
	roundControl;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));

			xmm0 = _mm_unpacklo_epi64 (xmm0, xmm1);

			_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
		
			_mm_storel_epi64( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a, *b;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			a    = (int *)pSrcCur;
			b    = (int *)pSrcRef;
			*a   = *b;			// get 4 bytes from src
		}
		break;
	default:
		PredF_FF_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}
void  static PredF_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int   width,		 int     height,
								   Fw32s roundControl)
{
	int i, j, summ;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j++)
		{
			summ = (int) *(pSrcCur+j);
			summ+= (int) *(pSrcCur+j+srcCurStep);
			summ+= roundControl;
			summ/= 2;
			*(pSrcRef+j) =  (Fw8u)summ;
//			*(pSrcRef+j) = (Fw8u)(*(pSrcCur+j) + *(pSrcCur+j+srcCurStep) + roundControl)/2;
		}
	}
}

void  static PredF_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						    int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7, xmm6;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	
	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
		
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8+srcCurStep) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr

			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

			xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit
	
			_mm_store_si128( (__m128i*)(pSrcRef), xmm0 ); // store 64 bits			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
		
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
		
			_mm_storel_epi64( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a, *b, c;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			a    = (int *)pSrcCur;
			b    = (int *)(pSrcCur+srcCurStep);
			xmm0 = _mm_cvtsi32_si128 (*a);
			xmm1 = _mm_cvtsi32_si128 (*b);

			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
		
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			c = _mm_cvtsi128_si32 (xmm0);

			b    = (int *)pSrcRef;
			*b   = c;			// get 4 bytes from src
		}
		break;
	default:
		PredF_FH_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}



void  static PredF_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i, j, summ;
 	pSrcRef;
	srcRefStep;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=width)
	{
		for(j=0;j<width;j++)
		{
			summ = (int) *(pSrcCur+j);
			summ+= (int) *(pSrcCur+j+1);
			summ+= roundControl;
			summ/= 2;
			*(pSrcRef+j) =  (Fw8u)summ;
//			*(pSrcRef+j) =  (Fw8u)(*(pSrcCur+j) + *(pSrcCur+j+1) + roundControl)/2;
		}
	}
}
void  static PredF_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm7, xmm6;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	
	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
		
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8+1) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr

			xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

			xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit
	
			_mm_store_si128( (__m128i*)(pSrcRef), xmm0 ); // store 64 bits			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );

			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
		
			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit
		
			_mm_storel_epi64( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a, *b;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			a    = (int *)pSrcCur;
			b    = (int *)(pSrcCur+1);
			xmm0 = _mm_cvtsi32_si128 (*a);
			xmm1 = _mm_cvtsi32_si128 (*b);

			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr

			xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
	
			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			b    = (int *)pSrcRef;
			*b   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
		}
		break;
	default:
		PredF_HF_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}
void  static PredF_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i, j, summ;
 	pSrcRef;
	srcRefStep;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j++)
		{
			summ = (int)*(pSrcCur+j); 
			summ+= (int)*(pSrcCur+j+1); 
			summ+= (int)*(pSrcCur+j+srcCurStep); 
			summ+= (int)*(pSrcCur+j+1+srcCurStep);
			summ+= roundControl*2;
			summ/=4;
			*(pSrcRef+j) =  (Fw8u)summ;
		}
	}
}
void  static PredF_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl*2);    // round;

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4
			
			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcCur+9) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+9+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm2 = _mm_add_epi16(xmm2, xmm3);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm6);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm4);		// ps-pr
			xmm2 = _mm_add_epi16(xmm2, xmm5);		// ps-pr

			xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

			xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

			_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+1+srcCurStep) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr

			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			_mm_storel_epi64( (__m128i*)pSrcRef, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a, *b;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)(pSrcCur+1);
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			a    = (int *)(pSrcCur+srcCurStep);
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcCur+srcCurStep+1);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm0 = _mm_add_epi16(xmm0, xmm1);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm4);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm5);		// ps-pr
			xmm0 = _mm_add_epi16(xmm0, xmm6);		// ps-pr
			
			xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

			xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			b    = (int *)pSrcRef;
			*b   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
		}
		break;
	default:
		PredF_HH_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);
		break;
	}
}

void  static PredB_FF_8u16s_C1_C( Fw8u* pSrcCur,  Fw32s srcCurStep,
 						  Fw8u* pSrcRef,  Fw32s srcRefStep,
						  Fw8u* pSrcRefB, Fw32s srcRefStepB, 
						  Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								Fw32s roundControl)
{
	int i, j, summ, diff;
/*
			for(i=0;i<height;i++, pr+=srcRefStepB, ps+=srcCurStep, pm+=width, pdd+=dstDiffStep/2)
			{
				for(j=0;j<width;j++)
				{
					*(pdd+j) =  *(ps+j)-(*(pm+j)+*(pr+j)+roundControl)/2;
				}
			}
*/
	for(i=0;i<height;i++, pSrcCur +=srcCurStep,  pSrcRef +=srcRefStep, 
						  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			summ  = (int)*(pSrcRef+j);
			summ += (int)*(pSrcRefB+j);
			summ += roundControl;
			summ /=2;
			diff  =  (int) *(pSrcCur+j);
			diff -= summ;
			*(pDstDiff+j) =  (Fw16s)diff;
//			*(pDstDiff+j) =  (Fw16s)(*(pSrcCur+j)-(*(pSrcRef+j)+*(pSrcRefB+j)+roundControl)/2);
		}
	}
}

void  static PredB_FF_8u16s_C1_SSE2( Fw8u* pSrcCur,  Fw32s srcCurStep,
 							 Fw8u* pSrcRef,  Fw32s srcRefStep,
						     Fw8u* pSrcRefB, Fw32s srcRefStepB, 
							 Fw16s* pDstDiff,Fw32s dstDiffStep, 
						     int    width,		 int     height,
							 Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm8  = _mm_slli_epi16 (xmm6, 1);

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur +=srcCurStep,  pSrcRef +=srcRefStep,
		                      pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			xmm2 = _mm_loadl_epi64( (__m128i*)pSrcRefB );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm1 = _mm_add_epi16(xmm1, xmm2);		// pr
			xmm1 = _mm_add_epi16(xmm1, xmm6);		// prb
			xmm1 = _mm_srai_epi16(xmm1, 1);			// round
			
			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
			
			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8));
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// ps u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pr
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// prb
			xmm4 = _mm_srai_epi16(xmm4, 1);		    // round

			xmm2 = _mm_sub_epi16(xmm2, xmm4);		// ps-pr

			_mm_storeu_si128( (__m128i*)(pDstDiff+8), xmm2 ); // store 64 bits	
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)pSrcRef );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			xmm2 = _mm_loadl_epi64( (__m128i*)pSrcRefB );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm1 = _mm_add_epi16(xmm1, xmm2);		// pr
			xmm1 = _mm_add_epi16(xmm1, xmm6);		// prb
			xmm1 = _mm_srai_epi16(xmm1, 1);			// round
			
			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			a    = (int *)pSrcRefB;
			xmm2 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm1 = _mm_add_epi16(xmm1, xmm2);		// ps-pr
			xmm1 = _mm_add_epi16(xmm1, xmm6);		// ps-pr
			xmm1 = _mm_srai_epi16(xmm1, 1);			/// ps-pr
			
			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr

			_mm_storel_epi64( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
		}
		break;
	default:
		PredB_FF_8u16s_C1_C( pSrcCur,  srcCurStep,
 							 pSrcRef,  srcRefStep,
						     pSrcRefB, srcRefStepB, 
						     pDstDiff, dstDiffStep, 
						     width,	   height,
							 roundControl);
		break;
	}
}

void  static PredB_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							    Fw8u* pSrcRef,  Fw32s srcRefStep,
						  const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
								Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								Fw32s roundControl)
{
	int i, j, summ, diff;



	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, 
						  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			summ  = (int)*(pSrcRefB+j);
			summ += (int)*(pSrcRefB+j+srcRefStepB);
			summ += roundControl;
			summ /=2;
			summ += (int)*(pSrcRef+j);
			summ += roundControl;
			summ /=2;
			diff  =  (int) *(pSrcCur+j);
			diff -= summ;
			*(pDstDiff+j) =  (Fw16s)diff;

//			*(pDstDiff+j) =  (Fw16s)(*(pSrcCur+j)-(*(pSrcRef+j)+
//											(*(pSrcRefB+j)+*(pSrcRefB+j+srcRefStepB)+roundControl)/2
//											+roundControl)/2);
		}
	}
}

void  static PredB_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						     const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
							  	   Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm8  = _mm_slli_epi16 (xmm6, 1);

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcRefStepB) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)pSrcCur+8 );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8+srcRefStepB) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm2 = _mm_sub_epi16(xmm2, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)(pDstDiff+16), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcRefStepB) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			a    = (int *)pSrcRefB;
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRefB+srcRefStepB);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// ps u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storel_epi64( (__m128i*)pDstDiff, xmm0 ); // store 64 bits

		}
		break;
	default:
		PredB_FH_8u16s_C1_C( pSrcCur,  srcCurStep,
 							 pSrcRef,  srcRefStep,
						     pSrcRefB, srcRefStepB, 
						     pDstDiff, dstDiffStep, 
						     width,	   height,
							 roundControl);

		break;
	}
}
void  static PredB_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							    Fw8u* pSrcRef,  Fw32s srcRefStep,
						  const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
								Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								Fw32s roundControl)
{
	int i, j;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, 
						  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pDstDiff+j) =  (Fw16s)(*(pSrcCur+j)-(*(pSrcRef+j)+
											(*(pSrcRefB+j)+*(pSrcRefB+j+1)+roundControl)/2
											+roundControl)/2);
		}
	}
}

void  static PredB_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						     const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
							  	   Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm8  = _mm_slli_epi16 (xmm6, 1);

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
			

			xmm2 = _mm_loadl_epi64( (__m128i*)pSrcCur+8 );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8+1) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm2 = _mm_sub_epi16(xmm2, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)(pDstDiff+16), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			a    = (int *)pSrcRefB;
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRefB+1);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// ps u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storel_epi64( (__m128i*)pDstDiff, xmm0 ); // store 64 bits

		}
		break;
	default:
		PredB_HF_8u16s_C1_C( pSrcCur,  srcCurStep,
 							 pSrcRef,  srcRefStep,
						     pSrcRefB, srcRefStepB, 
						     pDstDiff, dstDiffStep, 
						     width,	   height,
							 roundControl);

		break;
	}
}

void  static PredB_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							    Fw8u* pSrcRef,  Fw32s srcRefStep,
						  const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
								Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,			 int    height,
								Fw32s roundControl)
{
	int i, j;
/*
			for(i=0;i<height;i++, pr+=srcRefStepB, ps+=srcCurStep, pm+=width, pdd+=dstDiffStep/2)
			{
				for(j=0;j<width;j++)
				{
					*(pdd+j) =  *(ps+j)-(*(pm+j)+(*(pr+j)+*(pr+j+1)+*(pr+j+srcRefStepB)+ 
										 *(pr+j+1+srcRefStepB)+roundControl*2)/4+roundControl)/2;
				}
			}
*/
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep, 
						  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
	{
		for(j=0;j<width;j++)
		{
			*(pDstDiff+j) =  (Fw16s)(*(pSrcCur+j)-(*(pSrcRef+j)+
											(*(pSrcRefB+j)+*(pSrcRefB+j+1)+
											 *(pSrcRefB+j+srcRefStepB)+*(pSrcRefB+j+1+srcRefStepB)+
												roundControl*2)/4
											+roundControl)/2);
		}
	}
}

void  static PredB_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								   Fw8u* pSrcRef,  Fw32s srcRefStep,
						     const Fw8u* pSrcRefB, Fw32s srcRefStepB, 
							  	   Fw16s* pDstDiff,Fw32s dstDiffStep, 
						  int    width,		 int     height,
								   Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm8  = _mm_slli_epi16 (xmm6, 1);

	switch (width)
	{
	case 16:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							  pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcRefStepB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcRefStepB) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm2);		// pSrcRefB 
			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm8);		// round
			xmm4 = _mm_srai_epi16(xmm4, 2);			// /4

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
			

			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur+8 );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+8) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcRefStepB+8) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcRefStepB+8) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm2);		// pSrcRefB 
			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm8);		// round
			xmm4 = _mm_srai_epi16(xmm4, 2);			// /4

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)(pDstDiff+16), xmm2 ); // store 64 bits
			
		}
		break;
	case 8:
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRef) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr u8->u16

			xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

			xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcRefStepB) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcRefStepB) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm2);		// pSrcRefB 
			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm8);		// round
			xmm4 = _mm_srai_epi16(xmm4, 2);			// /4

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2
/// can use _mm_avg_epu16 (__m128i a, __m128i b);

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storeu_si128( (__m128i*)pDstDiff, xmm0 ); // store 64 bits
		}
		break;
	case 4:
//		int tmp;
		int *a;
		for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep,
							                       pSrcRefB+=srcRefStepB, pDstDiff+=dstDiffStep/2)
		{
			a    = (int *)pSrcCur;
			xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps u8->u16

			a    = (int *)pSrcRef;
			xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

			a    = (int *)pSrcRefB;
			xmm2 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRefB+1);
			xmm3 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRefB+srcRefStepB);
			xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps u8->u16

			a    = (int *)(pSrcRefB+1+srcRefStepB);
			xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr u8->u16

			xmm4 = _mm_add_epi16(xmm4, xmm5);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm2);		// pSrcRefB 
			xmm4 = _mm_add_epi16(xmm4, xmm3);		// pSrcRefB
			xmm4 = _mm_add_epi16(xmm4, xmm8);		// round
			xmm4 = _mm_srai_epi16(xmm4, 2);			// /4

			xmm4 = _mm_add_epi16(xmm4, xmm1);		// pSrcRef
			xmm4 = _mm_add_epi16(xmm4, xmm6);		// round
			xmm4 = _mm_srai_epi16(xmm4, 1);			// /2

			xmm0 = _mm_sub_epi16(xmm0, xmm4);		// pSrcCur-pr

			_mm_storel_epi64( (__m128i*)pDstDiff, xmm0 ); // store 64 bits

		}
		break;
	default:
		PredB_HH_8u16s_C1_C( pSrcCur,  srcCurStep,
 							 pSrcRef,  srcRefStep,
						     pSrcRefB, srcRefStepB, 
						     pDstDiff, dstDiffStep, 
						     width,	   height,
							 roundControl);

		break;
	}
}

FwStatus  static iGetDiffB_8u16s_C1(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl,
						  int    width,		 int     height)
{
	Fw8u  *ps,  *pr, *pm;
	Fw16s *pdd ;

	if (pSrcCur == NULL || pSrcRefF == NULL || pSrcRefB == NULL || pDstDiff == NULL ) 
		return fwStsNullPtrErr;

	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(height*width*sizeof(Fw8u));

	ps = (Fw8u *) pSrcRefF;
	pm = (Fw8u *) pSrcRefM;
	switch(mcTypeF)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type == DT_SSE2)
			    PredF_FF_8u16s_C1_SSE2( ps, srcRefStepF, pm, width, width, height, roundControl);
			else
			    PredF_FF_8u16s_C1_C( ps, srcRefStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type == DT_SSE2)
			    PredF_FH_8u16s_C1_SSE2( ps, srcRefStepF, pm, width, width, height, roundControl);
			else
			    PredF_FH_8u16s_C1_C( ps, srcRefStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type == DT_SSE2)
			    PredF_HF_8u16s_C1_SSE2( ps, srcRefStepF, pm, width, width, height, roundControl);
			else
			    PredF_HF_8u16s_C1_C( ps, srcRefStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type == DT_SSE2)
			    PredF_HH_8u16s_C1_SSE2( ps, srcRefStepF, pm, width, width, height, roundControl);
			else
			    PredF_HH_8u16s_C1_C( ps, srcRefStepF, pm, width, width, height, roundControl);
			break;
		default:
			break;
	}
	ps = (Fw8u *) pSrcCur;
	pr = (Fw8u *) pSrcRefB;
	pm = (Fw8u *) pSrcRefM;
	pdd= (Fw16s*) pDstDiff;
	switch(mcTypeB)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type == DT_SSE2)
				PredB_FF_8u16s_C1_SSE2(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);

			else
				PredB_FF_8u16s_C1_C(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type == DT_SSE2)
				PredB_FH_8u16s_C1_SSE2(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			else
				PredB_FH_8u16s_C1_C(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type == DT_SSE2)
				PredB_HF_8u16s_C1_SSE2(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			else
				PredB_HF_8u16s_C1_C(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type == DT_SSE2)
				PredB_HH_8u16s_C1_SSE2(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);
			else
				PredB_HH_8u16s_C1_C(ps,		srcCurStep,
 									pm,		width,
									pr,		srcRefStepB, 
									pdd,	dstDiffStep, 
									width, height,	roundControl);

			break;
		default:
			break;
	}

	fwFree(pSrcRefM);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x16B_8u16s_C1 )(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl)
{
	if(srcCurStep < 16 || srcRefStepF < 16 || srcRefStepB < 16  || dstDiffStep < 32) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iGetDiffB_8u16s_C1 (pSrcCur,  srcCurStep,
									  pSrcRefF, srcRefStepF, mcTypeF, 
									  pSrcRefB, srcRefStepB, mcTypeB, 
									  pDstDiff,	dstDiffStep, roundControl,
									  16, 16);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff16x8B_8u16s_C1 )(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl)
{
	if(srcCurStep < 16 || srcRefStepF < 16 || srcRefStepB < 16  || dstDiffStep < 32) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iGetDiffB_8u16s_C1 (pSrcCur,  srcCurStep,
									  pSrcRefF, srcRefStepF, mcTypeF, 
									  pSrcRefB, srcRefStepB, mcTypeB, 
									  pDstDiff,	dstDiffStep, roundControl,
									  16, 8);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x8B_8u16s_C1 )(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStepF < 8 || srcRefStepB < 8  || dstDiffStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iGetDiffB_8u16s_C1 (pSrcCur,  srcCurStep,
									  pSrcRefF, srcRefStepF, mcTypeF, 
									  pSrcRefB, srcRefStepB, mcTypeB, 
									  pDstDiff,	dstDiffStep, roundControl,
									  8, 8);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x16B_8u16s_C1 )(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStepF < 8 || srcRefStepB < 8  || dstDiffStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iGetDiffB_8u16s_C1 (pSrcCur,  srcCurStep,
									  pSrcRefF, srcRefStepF, mcTypeF, 
									  pSrcRefB, srcRefStepB, mcTypeB, 
									  pDstDiff,	dstDiffStep, roundControl,
									  8, 16);

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetDiff8x4B_8u16s_C1 )(const Fw8u* pSrcCur,  Fw32s srcCurStep,
									  const Fw8u* pSrcRefF, Fw32s srcRefStepF, Fw32s mcTypeF, 
									  const Fw8u* pSrcRefB, Fw32s srcRefStepB, Fw32s mcTypeB, 
									  Fw16s* pDstDiff,		 Fw32s dstDiffStep, 
									  Fw32s roundControl)
{
	if(srcCurStep < 8 || srcRefStepF < 8 || srcRefStepB < 8  || dstDiffStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iGetDiffB_8u16s_C1 (pSrcCur,  srcCurStep,
									  pSrcRefF, srcRefStepF, mcTypeF, 
									  pSrcRefB, srcRefStepB, mcTypeB, 
									  pDstDiff,	dstDiffStep, roundControl,
									  8, 4);

}

/// Sum of Squares of Differences Evaluation
/*
Description
This function is declared in the fwvc.h header file. The function fwiSqrDiff16x16_8u32s
evaluates the sum of square difference between all the elements in the current block and
corresponding elements in the reference block. The result is stored in integer *pSqrDiff.
Let us denote the pel which lies at the crossing of ith row and jth column of the current block as
block[i,j] and the pel which lies at the crossing of ith row and jth column of the reference
block as ref_block[i,j]. Then
.
*pSqrDiff block i j , [ ] ref_
*/

FwStatus  static SqrDiff16x16_8u32s_C (const Fw8u* pSrc, Fw32s srcStep, 
								  const Fw8u* pRef, Fw32s refStep, 
								  Fw32s mcType,	 Fw32s* pSqrDiff)
{
//	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(8*4*sizeof(Fw8u));
	int i, j, sqr, avg;
	Fw8u  *ps,  *pr;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	ps = (Fw8u *) pSrc;
	pr = (Fw8u *) pRef;

	*pSqrDiff = 0;
	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					sqr = (int)*(ps+j);
					sqr-= (int)*(pr+j);
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					avg = (int)*(pr+j);
					avg+= (int)*(pr+j+refStep);
					avg/=2;
					sqr = (int)*(ps+j)-avg;
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					avg = (int)*(pr+j);
					avg+= (int)*(pr+j+1);
					avg/=2;
					sqr = (int)*(ps+j)-avg;
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					avg = *(pr+j);
					avg+= *(pr+j+1);
					avg+= *(pr+j+refStep);
					avg+= *(pr+j+1+refStep);
					avg/=4;
					sqr = (int)*(ps+j)-avg;
//					sqr = *(ps+j)-(*(pr+j) + *(pr+j+1)+*(pr+j+refStep)+*(pr+j+1+refStep))/4;
					*pSqrDiff += sqr*sqr;

				}
			}
			break;
		default:
			break;
	}

	return fwStsNoErr;
}

FwStatus  static SqrDiff16x16_8u32s_SSE2 (const Fw8u* pSrc, Fw32s srcStep, 
								   const Fw8u* pRef, Fw32s refStep, 
								   Fw32s mcType,	  Fw32s* pSqrDiff)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	
	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	Fw8u* prf = (Fw8u*) pRef;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;

//	Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));
//	Fw16s* tempb = (Fw16s*) fwMalloc(8*sizeof(Fw16s));

	*pSqrDiff = 0;
	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrc );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps 

				xmm1 = _mm_loadl_epi64( (__m128i*)pRef );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrc+8));
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(pRef+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm2 = _mm_sub_epi16(xmm2, xmm3);		// ps-pr
				xmm2 = _mm_madd_epi16 (xmm2, xmm2);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm2);		// accu to 32bit array			
			}

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 
	
			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);

			break;
		case FWVC_MC_APX_FH:
//			int j, sqr;
//			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
//			{
//				for(j=0;j<16;j++)
//				{
//					sqr = *(pSrc+j)-(*(pRef+j) + *(pRef+j+refStep))/2;
//					*pSqrDiff += sqr*sqr;
//				}
//			}
			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrc );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps 

				xmm1 = _mm_loadl_epi64( (__m128i*)pRef );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

				xmm2 = _mm_loadl_epi64( (__m128i*)(pRef+refStep) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//				xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
				xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);		// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrc+8));
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(pRef+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(pRef+refStep+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

//				xmm3 = _mm_avg_epu16 (xmm3, xmm4);		// average
				xmm3 = _mm_add_epi16 (xmm3, xmm4);		// average
				xmm3 = _mm_srai_epi16(xmm3, 1);			// /2

				xmm2 = _mm_sub_epi16(xmm2, xmm3);		// ps-pr
				xmm2 = _mm_madd_epi16 (xmm2, xmm2);		// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm2);		// accu to 32bit array				
			}

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);

			break;
		case FWVC_MC_APX_HF:
//			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
//			{
//				for(j=0;j<16;j++)
//				{
//					sqr = *(ps+j)-(*(pr+j) + *(pr+j+1))/2;
//					*pSqrDiff += sqr*sqr;
//				}
//			}
			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrc );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps 

				xmm1 = _mm_loadl_epi64( (__m128i*)pRef );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

				xmm2 = _mm_loadl_epi64( (__m128i*)(pRef+1) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//				xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
				xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-pr
				xmm0 =_mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu
			

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrc+8));
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(pRef+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(pRef+1+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

//				xmm3 = _mm_avg_epu16 (xmm3, xmm4);		// average
				xmm3 = _mm_add_epi16 (xmm3, xmm4);		// average
				xmm3 = _mm_srai_epi16(xmm3, 1);			// /2

				xmm2 = _mm_sub_epi16(xmm2, xmm3);		// ps-pr
				xmm2 = _mm_madd_epi16 (xmm2, xmm2);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm2);		// accu to 32bit array
				
			}

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);
			break;
		case FWVC_MC_APX_HH:
/*
			int j, sqr;
			*pSqrDiff = 0;
			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
			{
				for(j=0;j<16;j++)
				{
					sqr  = (int)*(pRef+j);
					sqr += (int)*(pRef+j+1);
					sqr += (int)*(pRef+j+refStep);
					sqr += (int)*(pRef+j+1+refStep);
					sqr /=4;
					sqr = (int)*(pSrc+j)-sqr;
					*pSqrDiff += sqr*sqr;
				}
			}
return;
*/
			for(i=0;i<16;i++, pSrc+=srcStep, pRef+=refStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrc );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps 

				prf = (Fw8u*) pRef;
				xmm1 = _mm_loadl_epi64( (__m128i*)prf );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

				xmm2 = _mm_loadl_epi64( (__m128i*)(prf+1) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				prf += refStep;
				xmm3 = _mm_loadl_epi64( (__m128i*)(prf) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(prf+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

				xmm1 = _mm_add_epi16 (xmm1, xmm2);		// sum
				xmm1 = _mm_add_epi16 (xmm1, xmm3);		// sum
				xmm1 = _mm_add_epi16 (xmm1, xmm4);		// sum
				xmm1 = _mm_srai_epi16(xmm1, 2);			// /4

				xmm0 = _mm_sub_epi16 (xmm0, xmm1);		// ps-pr
				xmm0 = _mm_madd_epi16(xmm0, xmm0);		// sum of sqr
				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				
				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrc+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// ps 

				prf  = (Fw8u*) pRef+8;
				xmm1 = _mm_loadl_epi64( (__m128i*)prf );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

				xmm2 = _mm_loadl_epi64( (__m128i*)(prf+1) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				prf += refStep;
				xmm3 = _mm_loadl_epi64( (__m128i*)(prf) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(prf+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

				xmm1 = _mm_add_epi16 (xmm1, xmm2);		// sum
				xmm1 = _mm_add_epi16 (xmm1, xmm3);		// sum
				xmm1 = _mm_add_epi16 (xmm1, xmm4);		// sum
				xmm1 = _mm_srai_epi16(xmm1, 2);			// /4

				xmm5 = _mm_sub_epi16 (xmm5, xmm1);		// ps-pr
				xmm5 = _mm_madd_epi16(xmm5, xmm5);		// sum of sqr
				xmm6 = _mm_add_epi32(xmm6, xmm5);		// accu				
			}

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);
			break;
		default:
			break;
	}

//printf("\n %d %d %d %d %d %d %d %d",tempa[0], tempa[1],tempa[2],tempa[3],
//	   tempa[4],tempa[5],tempa[6],tempa[7]);
//printf(" %d %d %d %d %d %d %d %d",tempb[0], tempb[1],tempb[2],tempb[3],
//	   tempb[4],tempb[5],tempb[6],tempb[7]);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqrDiff16x16_8u32s )(const Fw8u* pSrc, Fw32s srcStep, 
								  const Fw8u* pRef, Fw32s refStep, 
								  Fw32s mcType,	 Fw32s* pSqrDiff)
{
	if(srcStep < 16 || refStep < 16) return fwStsSizeErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	if( Dispatch_Type == DT_SSE2)
		return SqrDiff16x16_8u32s_SSE2 (pSrc, srcStep, pRef, refStep, mcType, pSqrDiff);
	else
		return SqrDiff16x16_8u32s_C (pSrc, srcStep, pRef, refStep, mcType, pSqrDiff);
}


FwStatus  static SqrDiff16x16B_8u32s_C(const Fw8u* pSrc,  Fw32s srcStep, 
								  const Fw8u* pRefF, Fw32s refStepF, Fw32s mcTypeF, 
								  const Fw8u* pRefB, Fw32s refStepB, Fw32s mcTypeB, Fw32s* pSqrDiff)
{
	int i, j, sqr, avg;
	Fw8u *ps, *pr, *pm ;

	if (pSrc == NULL || pRefF == NULL || pRefB == NULL) 
		return fwStsNullPtrErr;

	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(16*16*sizeof(Fw8u));

	ps = (Fw8u *) pRefF;
	pm = (Fw8u *) pSrcRefM;
	switch(mcTypeF)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					*(pm+j) =  *(ps+j);
				}
			}
			break;
		case FWVC_MC_APX_FH:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					avg = (int)*(ps+j);
					avg+= (int)*(ps+j+refStepF);
					avg/= 2;
					*(pm+j) = (Fw8u) avg;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				for(j=0;j<16;j++)
				{
//					*(pm+j) =  (*(ps+j) + *(ps+j+1))/2;
					avg = (int)*(ps+j);
					avg+= (int)*(ps+j+1);
					avg/= 2;
					*(pm+j) = (Fw8u) avg;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				for(j=0;j<16;j++)
				{
//					*(pm+j) = (*(ps+j)+*(ps+j+1)+*(ps+j+refStepF)+*(ps+j+1+refStepF))/4;
					avg = (int)*(ps+j);
					avg+= (int)*(ps+j+1);
					avg+= (int)*(ps+j+refStepF);
					avg+= (int)*(ps+j+1+refStepF);
					avg/= 4;
					*(pm+j) = (Fw8u) avg;

				}
			}
			break;
		default:
			break;
	}

	ps = (Fw8u *) pSrc;
	pr = (Fw8u *) pRefB;
	pm = (Fw8u *) pSrcRefM;
	* pSqrDiff = 0;
	switch(mcTypeB)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					sqr = (int)*(ps+j)-((int)*(pm+j)+(int)*(pr+j))/2;
					*pSqrDiff += (sqr*sqr);
				}
			}
			break;
		case FWVC_MC_APX_FH:
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					sqr = *(ps+j)-(*(pm+j)+(*(pr+j)+*(pr+j+refStepB))/2)/2;
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					sqr = *(ps+j)-(*(pm+j)+(*(pr+j)+*(pr+j+1))/2)/2;
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				for(j=0;j<16;j++)
				{
					sqr = (int)*(ps+j)-((int)*(pm+j)+
						((int)*(pr+j)+(int)*(pr+j+1)+
						 (int)*(pr+j+refStepB)+(int)*(pr+j+1+refStepB))/4)/2;
					*pSqrDiff += sqr*sqr;
				}
			}
			break;
		default:
			break;
	}

	fwFree(pSrcRefM);
	return fwStsNoErr;
}


FwStatus  static SqrDiff16x16B_8u32s_SSE2(const Fw8u* pSrc,  Fw32s srcStep, 
								   const Fw8u* pRefF, Fw32s refStepF, Fw32s mcTypeF, 
								   const Fw8u* pRefB, Fw32s refStepB, Fw32s mcTypeB, 
								   Fw32s* pSqrDiff)
{
	int i;
	Fw8u  *ps,  *pr, *pm ;
//	Fw32s sqr = 0;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm7, xmm6;

	if (pSrc == NULL || pRefF == NULL || pRefB == NULL) 
		return fwStsNullPtrErr;

	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(16*16*sizeof(Fw8u));

	xmm7  = _mm_set1_epi8(0);    // zero;

	ps = (Fw8u *) pRefF;
	pm = (Fw8u *) pSrcRefM;
	switch(mcTypeF)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
				_mm_storeu_si128( (__m128i*)pm, xmm0 ); // store 64 bits
			}
			break;
		case FWVC_MC_APX_FH:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(ps+refStepF) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps

//				xmm0 = _mm_avg_epu8  (xmm0, xmm1);
				xmm0 = _mm_add_epi16 (xmm0, xmm1);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm2 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(ps+refStepF+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// ps

//				xmm0 = _mm_avg_epu8  (xmm0, xmm1);
				xmm3 = _mm_add_epi16 (xmm3, xmm2);		// round
				xmm3 = _mm_srai_epi16(xmm3, 1);			// /2

				xmm0 = _mm_packus_epi16 (xmm0, xmm3);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pm, xmm0 ); // store 64 bits
			}
			break;
		case FWVC_MC_APX_HF:
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(ps+1) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps

//				xmm0 = _mm_avg_epu8  (xmm0, xmm1);
				xmm0 = _mm_add_epi16 (xmm0, xmm1);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm2 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(ps+1+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// ps

//				xmm0 = _mm_avg_epu8  (xmm0, xmm1);
				xmm3 = _mm_add_epi16 (xmm3, xmm2);		// round
				xmm3 = _mm_srai_epi16(xmm3, 1);			// /2

				xmm0 = _mm_packus_epi16 (xmm0, xmm3);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pm, xmm0 ); // store 64 bits
			}
			break;
		case FWVC_MC_APX_HH:
/*
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				for(j=0;j<16;j++)
					*(pm+j) =  (*(ps+j)+*(ps+j+1)+*(ps+j+refStepF)+*(ps+j+1+refStepF))/4;
			}
*/
			for(i=0;i<16;i++, ps+=refStepF, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(ps+1) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps

				xmm2 = _mm_loadl_epi64( (__m128i*)(ps+refStepF) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(ps+refStepF+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// ps

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// round
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// round
				xmm0 = _mm_add_epi16(xmm0, xmm2);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /2

				xmm4 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps

				xmm5 = _mm_loadl_epi64( (__m128i*)(ps+9) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// ps

				xmm6 = _mm_loadl_epi64( (__m128i*)(ps+8+refStepF) );
				xmm6 = _mm_unpacklo_epi8(xmm6, xmm7);	// ps

				xmm3 = _mm_loadl_epi64( (__m128i*)(ps+refStepF+9) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// ps

				xmm4 = _mm_add_epi16(xmm4, xmm5);		// round
				xmm3 = _mm_add_epi16(xmm3, xmm6);		// round
				xmm4 = _mm_add_epi16(xmm4, xmm3);		// round
				xmm4 = _mm_srai_epi16(xmm4, 2);			// /2

				xmm0 = _mm_packus_epi16 (xmm0, xmm4);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pm, xmm0 ); // store 64 bits
			}
			break;
		default:
			break;
	}

	ps = (Fw8u *) pSrc;
	pr = (Fw8u *) pRefB;
	pm = (Fw8u *) pSrcRefM;
	* pSqrDiff = 0;
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	switch(mcTypeB)
	{
		case FWVC_MC_APX_FF:
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)pm );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)pr );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16(xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				xmm0 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(pm+8) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)(pr+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu
			}

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);
			break;
		case FWVC_MC_APX_FH:
#if 0
//			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)			{
//				for(j=0;j<16;j++)				{
//					sqr = *(ps+j)-(*(pm+j)+(*(pr+j)+*(pr+j+refStepB))/2)/2;
//					*pSqrDiff += sqr*sqr;
//				}
//			}
#else
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)pm );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)pr );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+refStepB) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

//				xmm2 = _mm_avg_epu16(xmm2, xmm3);		//  avg
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				xmm0 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(pm+8) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)(pr+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+8+refStepB) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

//				xmm2 = _mm_avg_epu16(xmm2, xmm3);		//  avg
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu
			}
			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);			
#endif
			break;
		case FWVC_MC_APX_HF:
//			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16) 			{
//				for(j=0;j<16;j++)				{
//					sqr = *(ps+j)-(*(pm+j)+(*(pr+j)+*(pr+j+1))/2)/2;
//					*pSqrDiff += sqr*sqr;
//				}
//			}
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)pm );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)pr );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

//				xmm2 = _mm_avg_epu16(xmm2, xmm3);		//  avg
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2


				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

				xmm0 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(pm+8) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)(pr+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+1+8) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

//				xmm2 = _mm_avg_epu16(xmm2, xmm3);		//  avg
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
//				xmm1 = _mm_avg_epu16(xmm1, xmm2);		//  avg
				xmm1 = _mm_add_epi16(xmm1, xmm2);		// round
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu
			}
			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);			break;
		case FWVC_MC_APX_HH:
#if 0
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
				for(j=0;j<16;j++)				{
					sqr = (int)*(ps+j)-((int)*(pm+j)+
						((int)*(pr+j)+(int)*(pr+j+1)+(int)*(pr+j+refStepB)+ 
										 (int)*(pr+j+1+refStepB))/4)/2;
					*pSqrDiff += sqr*sqr;
				}
#else
			for(i=0;i<16;i++, pr+=refStepB, ps+=srcStep, pm+=16)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)ps );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)pm );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)pr );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(pr+refStepB) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

				xmm5 = _mm_loadl_epi64( (__m128i*)(pr+1+refStepB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// pr
				xmm4 = _mm_add_epi16(xmm4, xmm5);		// pr
				xmm2 = _mm_add_epi16(xmm2, xmm4);		// pr
				xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

				xmm1 = _mm_add_epi16(xmm1, xmm2);
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);		// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu



				xmm0 = _mm_loadl_epi64( (__m128i*)(ps+8) );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

				xmm1 = _mm_loadl_epi64( (__m128i*)(pm+8) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pm

				xmm2 = _mm_loadl_epi64( (__m128i*)(pr+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

				xmm3 = _mm_loadl_epi64( (__m128i*)(pr+8+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr

				xmm4 = _mm_loadl_epi64( (__m128i*)(pr+refStepB+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

				xmm5 = _mm_loadl_epi64( (__m128i*)(pr+1+8+refStepB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// pr
				xmm4 = _mm_add_epi16(xmm4, xmm5);		// pr
				xmm2 = _mm_add_epi16(xmm2, xmm4);		// pr
				xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

				xmm1 = _mm_add_epi16(xmm1, xmm2);		// pr+pm
				xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

				xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-avg
				xmm0 = _mm_madd_epi16 (xmm0, xmm0);		// sum of sqr

				xmm6 = _mm_add_epi32(xmm6, xmm0);		// accu

			}
			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 8);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			xmm0 = xmm6;
			xmm6 = _mm_srli_si128 (xmm6, 4);
			xmm6 = _mm_add_epi32(xmm6, xmm0); 

			*pSqrDiff = (Fw32s) _mm_cvtsi128_si32 (xmm6);			
#endif
			break;
		default:
			break;
	}
	fwFree(pSrcRefM);
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqrDiff16x16B_8u32s)(const Fw8u* pSrc,  Fw32s srcStep, 
								  const Fw8u* pRefF, Fw32s refStepF, Fw32s mcTypeF, 
								  const Fw8u* pRefB, Fw32s refStepB, Fw32s mcTypeB, Fw32s* pSqrDiff)
{
	if(srcStep < 16 || refStepF < 16 || refStepB < 16) return fwStsSizeErr;
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;


	if( Dispatch_Type == DT_SSE2)
		return SqrDiff16x16B_8u32s_SSE2(pSrc, srcStep, pRefF, refStepF, mcTypeF, pRefB, refStepB, mcTypeB, pSqrDiff);
	else
		return SqrDiff16x16B_8u32s_C(pSrc, srcStep, pRefF, refStepF, mcTypeF, pRefB, refStepB, mcTypeB, pSqrDiff);
}

FwStatus  static VarMean8x8_8u32s_C1R_C (const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar, Fw32s* pMean)
{
	int i, j, sqr;
	Fw8u  *ps;

	if (pSrc == NULL) return fwStsNullPtrErr;

	*pMean = 0;
	ps = (Fw8u*)pSrc;
	for(i=0;i<8;i++, ps+=srcStep) for(j=0;j<8;j++) *pMean +=  *(ps+j);
	*pMean /=  64;

	*pVar = 0;
	ps = (Fw8u*)pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
		for(j=0;j<8;j++)
		{
			sqr =  *(ps+j)-*pMean;
			*pVar +=  sqr*sqr;
		}
	*pVar /=  64;
	return fwStsNoErr;
}

FwStatus  static VarMean8x8_8u32s_C1R_SSE2 (const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar, Fw32s* pMean)
{
	int i;
	Fw8u  *ps;
	__m128i xmm0, xmm4, xmm6, xmm7;
	
	if (pSrc == NULL) return fwStsNullPtrErr;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	*pMean= 0;
	ps    = (Fw8u*)pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_sad_epu8 (xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0);

	*pMean = (Fw32s) _mm_cvtsi128_si32 (xmm6);
	*pMean /= 64;

	xmm4  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	*pVar = 0;
	ps    = (Fw8u*)pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-*pMean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm0 = _mm_sub_epi16 (xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16(xmm0, xmm0);			// sum of sqr

		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);
	*pVar /=  64;
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiVarMean8x8_8u32s_C1R )(const Fw8u* pSrc, Fw32s srcStep, 
									Fw32s* pVar, Fw32s* pMean)
{
	if(srcStep < 8) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return VarMean8x8_8u32s_C1R_SSE2 (pSrc, srcStep, pVar, pMean);
	else
		return VarMean8x8_8u32s_C1R_C (pSrc, srcStep, pVar, pMean);
}


FwStatus  static VarMean8x8_16s32s_C1R_C(const Fw16s* pSrc, Fw32s srcStep, 
									 Fw32s* pVar, Fw32s* pMean)
{
	int i, j, sqr;
	Fw16s  *ps;

	if (pSrc == NULL) return fwStsNullPtrErr;

srcStep/=2; // for 16s increment

	*pVar = *pMean = 0;
	ps = (Fw16s *) pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
		for(j=0;j<8;j++)
		{
			*pMean +=  *(ps+j);
		}
	}
	*pMean /=  64;
	ps = (Fw16s *) pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
		for(j=0;j<8;j++)
		{
			sqr =  *(ps+j)-*pMean;
			*pVar +=  sqr*sqr;
		}
	}
	*pVar /=  64;
	return fwStsNoErr;
}
FwStatus  static VarMean8x8_16s32s_C1R_SSE2  (const Fw16s* pSrc, Fw32s srcStep, 
									 Fw32s* pVar, Fw32s* pMean)
{
	int i;
	Fw16s  *ps;

	if (pSrc == NULL) return fwStsNullPtrErr;

	__m128i xmm0, xmm4, xmm6, xmm7;

srcStep/=2; // for 16s increment

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;

	*pVar = *pMean = 0;
	ps = (Fw16s *) pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	*pMean =  (Fw32s) _mm_cvtsi128_si32 (xmm6);
	*pMean /=  64;

	xmm4  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	ps = (Fw16s *) pSrc;
	for(i=0;i<8;i++, ps+=srcStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-*pMean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadu_si128 ( (__m128i*)ps );

		xmm0 = _mm_sub_epi16(xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);

	*pVar /=  64;
	return fwStsNoErr;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiVarMean8x8_16s32s_C1R )(const Fw16s* pSrc, Fw32s srcStep, 
									 Fw32s* pVar, Fw32s* pMean)
{
	if(srcStep < 16) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return VarMean8x8_16s32s_C1R_SSE2 (pSrc, srcStep, pVar, pMean);
	else
		return VarMean8x8_16s32s_C1R_C (pSrc, srcStep, pVar, pMean);
}


void  static VarMeanDiff8x8_FF_SSE2 (const Fw8u*  pSrc, Fw32s srcStep,
							 const Fw8u*  pRef, Fw32s refStep, 
							       Fw32s* pSrcSum, 
							       Fw32s* pVar,  
							       Fw32s* pMean)
{
	int i;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm4, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm5  = _mm_set1_epi8(0);    // zero;

	*pVar = *pMean = *pSrcSum = 0;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm5 = _mm_add_epi16 (xmm5, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	*pSrcSum =  (Fw32s) _mm_cvtsi128_si32 (xmm6);
	
	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 8);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 4);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 2);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// change 16bit to 32 bit

	*pMean =  (Fw32s) _mm_cvtsi128_si32 (xmm5);
	*pMean  =  *pSrcSum - *pMean;

	*pMean /=  64;

	xmm4  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm7  = _mm_set1_epi8(0);    // zero;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//	for(j=0;j<8;j++){ sqr = ((int)*(ps+j)-(int)*(pr+j))-*Mean;	
//		*Var +=  sqr*sqr;	}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

		xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-p2

		xmm0 = _mm_sub_epi16(xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);

	*pVar /=  64;

	return;
}

void  static VarMeanDiff8x8_FH_SSE2 (const Fw8u* pSrc, Fw32s srcStep,
								  const Fw8u* pRef, Fw32s refStep, 
								  Fw32s* pSrcSum, 
								  Fw32s* pVar,  
								  Fw32s* pMean)
{
	int i;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm5  = _mm_set1_epi8(0);    // zero;

	*pVar = *pMean = *pSrcSum = 0;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+refStep) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//		xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

		xmm5 = _mm_add_epi16 (xmm5, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	*pSrcSum =  (Fw32s) _mm_cvtsi128_si32 (xmm6);
	
	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 8);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 4);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 2);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// change 16bit to 32 bit

	*pMean =  (Fw32s) _mm_cvtsi128_si32 (xmm5);
	*pMean  =  *pSrcSum - *pMean;
	*pMean /=  64;

	xmm4  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm7  = _mm_set1_epi8(0);    // zero;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;

	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-*pMean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+refStep) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//		xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

		xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-p2

		xmm0 = _mm_sub_epi16(xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);
	*pVar /=  64;

	return;
}

void  static VarMeanDiff8x8_HF_SSE2 (const Fw8u* pSrc, Fw32s srcStep,
							  const Fw8u* pRef, Fw32s refStep, 
							  Fw32s* pSrcSum, 
							  Fw32s* pVar,  
							  Fw32s* pMean)
{
	int i;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm5  = _mm_set1_epi8(0);    // zero;

	*pVar = *pMean = *pSrcSum = 0;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+1) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//		xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

		xmm5 = _mm_add_epi16 (xmm5, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	*pSrcSum =  (Fw32s) _mm_cvtsi128_si32 (xmm6);
	
	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 8);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 4);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 2);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// change 16bit to 32 bit

	*pMean =  (Fw32s) _mm_cvtsi128_si32 (xmm5);
	*pMean  =  *pSrcSum - *pMean;

	*pMean /=  64;

	xmm4  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	ps = (const Fw8u *) pSrc;
	pr = (const Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-*pMean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+1) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr

//		xmm1 = _mm_avg_epu16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_srai_epi16(xmm1, 1);			// /2

		xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-p2

		xmm0 = _mm_sub_epi16(xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr

		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);

	*pVar /=  64;
	return;
}

void  static VarMeanDiff8x8_HH_SSE2 (const Fw8u* pSrc, Fw32s srcStep,
								  const Fw8u* pRef, Fw32s refStep, 
								  Fw32s* pSrcSum, 
								  Fw32s* pVar,  
								  Fw32s* pMean)
{
	int i;
	Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm5  = _mm_set1_epi8(0);    // zero;

	*pVar = *pMean = *pSrcSum = 0;
	ps = (Fw8u *) pSrc;
	pr = (Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++)*pMean +=  *(ps+j);
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+1) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr
		xmm3 = _mm_loadl_epi64( (__m128i*)(pr+refStep) );
		xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr
		xmm4 = _mm_loadl_epi64( (__m128i*)(pr+1+refStep) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

		xmm3 = _mm_add_epi16 (xmm3, xmm4);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm3);		// average
		xmm1 = _mm_srai_epi16(xmm1, 2);			// /4

		xmm5 = _mm_add_epi16 (xmm5, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	*pSrcSum =  (Fw32s) _mm_cvtsi128_si32 (xmm6);
	
	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 8);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 4);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm1 = xmm5;
	xmm5 = _mm_srli_si128 (xmm5, 2);
	xmm5 = _mm_add_epi16(xmm5, xmm1); 

	xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// change 16bit to 32 bit

	*pMean =  (Fw32s) _mm_cvtsi128_si32 (xmm5);
	*pMean  =  *pSrcSum - *pMean;
	*pMean /=  64;

	xmm5  = _mm_set1_epi16 ((short)(*pMean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	xmm7  = _mm_set1_epi8(0);    // zero;
	ps = (Fw8u *) pSrc;
	pr = (Fw8u *) pRef;
	for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-*pMean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		xmm2 = _mm_loadl_epi64( (__m128i*)(pr+1) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr
		xmm3 = _mm_loadl_epi64( (__m128i*)(pr+refStep) );
		xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr
		xmm4 = _mm_loadl_epi64( (__m128i*)(pr+1+refStep) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// pr

		xmm3 = _mm_add_epi16 (xmm3, xmm4);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm2);		// average
		xmm1 = _mm_add_epi16 (xmm1, xmm3);		// average
		xmm1 = _mm_srai_epi16(xmm1, 2);			// /4

		xmm0 = _mm_sub_epi16(xmm0, xmm1);		// ps-p2
		xmm0 = _mm_sub_epi16(xmm0, xmm5);		// ps-mean

		xmm0 = _mm_madd_epi16 (xmm0, xmm0);		// sum of sqr

		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);
	*pVar /=  64;

	return;
}

FwStatus  static VarMeanDiff16x16_8u32s_C1R_SSE2 (const Fw8u* pSrc, Fw32s srcStep,
										   const Fw8u* pRef, Fw32s refStep, 
										   Fw32s* pSrcSum, 
										   Fw32s* pVar,  
										   Fw32s* pMean, Fw32s mcType)
{
	int ii, jj;
	const Fw8u   *ps, *pr;
	Fw32s  SrcSum, Var, Mean;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_FF_SSE2 (ps, srcStep,
 											pr, refStep, 
											&SrcSum, 
										    &Var,  
										    &Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_FH_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
											&Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_HF_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
											&Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;

				}
			}
			break;
		case FWVC_MC_APX_HH:
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_HH_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
										    &Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}	
			break;
		default:
			break;
	}

	return fwStsNoErr;
}
FwStatus  static VarMeanDiff16x16_8u32s_C1R_C (const Fw8u* pSrc, Fw32s srcStep,
										const Fw8u* pRef, Fw32s refStep, 
										Fw32s* pSrcSum, 
										Fw32s* pVar,  
										Fw32s* pMean, Fw32s mcType)
{
	int ii, jj, i, j, sqr, pr_mc;
	const Fw8u   *ps, *pr;
	Fw32s  *SrcSum, *Var, *Mean;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							*Mean   +=  (int)*(pr+j);
							*SrcSum +=  (int)*(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							sqr   =  ((int)*(ps+j)-(int)*(pr+j))-(*Mean);	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;

					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							*Mean   +=  ((int)*(pr+j)+(int)*(pr+j+refStep))/2;
							*SrcSum +=  (int)*(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							sqr   =  ((int)*(pr+j)+(int)*(pr+j+refStep))/2;	
							sqr   =  (int)*(ps+j)-sqr-*Mean;	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;;

					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc    =  (*(pr+j)+*(pr+j+1))/2;
							*Mean   +=  pr_mc;
							*SrcSum +=  *(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc    =  (*(pr+j)+*(pr+j+1))/2;
							sqr   =  *(ps+j)-pr_mc-*Mean;	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;
					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<16;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc = (*(pr+j)+*(pr+j+1)+*(pr+j+refStep)+ *(pr+j+1+refStep))/4;

							*Mean   +=  pr_mc;
							*SrcSum +=  *(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc = (*(pr+j)+*(pr+j+1)+*(pr+j+refStep)+ *(pr+j+1+refStep))/4;
							sqr   =  *(ps+j)-pr_mc-*Mean;
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;
					Var++;
					Mean++;
					SrcSum++;
				}
			}	
			break;
		default:
			break;
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiVarMeanDiff16x16_8u32s_C1R )(const Fw8u* pSrc, Fw32s srcStep,
										  const Fw8u* pRef, Fw32s refStep, 
										  Fw32s* pSrcSum, 
										  Fw32s* pVar,  
										  Fw32s* pMean, Fw32s mcType)
{
	if(srcStep < 8 || refStep < 8) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return VarMeanDiff16x16_8u32s_C1R_SSE2 (pSrc, srcStep, pRef, refStep, 
											   pSrcSum, pVar, pMean, mcType);
	else
		return VarMeanDiff16x16_8u32s_C1R_C (pSrc, srcStep, pRef, refStep, 
											   pSrcSum, pVar, pMean, mcType);
}

FwStatus  static VarMeanDiff16x8_8u32s_C1R_SSE2 (const Fw8u* pSrc, Fw32s srcStep,
										 const Fw8u* pRef, Fw32s refStep, 
										 Fw32s* pSrcSum, 
										 Fw32s* pVar, 
										 Fw32s* pMean, Fw32s mcType)
{
	int ii, jj;
	const Fw8u   *ps, *pr;
	Fw32s  SrcSum, Var, Mean;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_FF_SSE2 (ps, srcStep,
 											pr, refStep, 
											&SrcSum, 
										    &Var,  
										    &Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_FH_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
											&Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_HF_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
											&Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;

				}
			}
			break;
		case FWVC_MC_APX_HH:
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					VarMeanDiff8x8_HH_SSE2 (ps, srcStep,
											pr, refStep, 
											&SrcSum, 
											&Var,  
										    &Mean);
					*(pVar++) = Var;
					*(pMean++) = Mean;
					*(pSrcSum++) = SrcSum;
				}
			}	
			break;
		default:
			break;
	}

	return fwStsNoErr;
}

FwStatus  static VarMeanDiff16x8_8u32s_C1R_C (const Fw8u* pSrc, Fw32s srcStep,
										 const Fw8u* pRef, Fw32s refStep, 
										 Fw32s* pSrcSum, 
										 Fw32s* pVar, 
										 Fw32s* pMean, Fw32s mcType)
{
	int ii, jj, i, j, sqr, pr_mc;
	const Fw8u   *ps, *pr;
	Fw32s  *SrcSum, *Var, *Mean;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							*Mean   +=  (int)*(pr+j);
							*SrcSum +=  (int)*(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;

					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							sqr   =  ((int)*(ps+j)-(int)*(pr+j))-(*Mean);	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;

					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							*Mean   +=  ((int)*(pr+j)+(int)*(pr+j+refStep))/2;
							*SrcSum +=  (int)*(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							sqr   =  ((int)*(pr+j)+(int)*(pr+j+refStep))/2;	
							sqr   =  (int)*(ps+j)-sqr-*Mean;	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;;

					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc    =  (*(pr+j)+*(pr+j+1))/2;
							*Mean   +=  pr_mc;
							*SrcSum +=  *(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc    =  (*(pr+j)+*(pr+j+1))/2;
							sqr   =  *(ps+j)-pr_mc-*Mean;	
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;
					Var++;
					Mean++;
					SrcSum++;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			Var=pVar; Mean=pMean; SrcSum=pSrcSum;
			for(ii=0;ii<8;ii+=8)
			{
				for(jj=0;jj<16;jj+=8)
				{
					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Mean=0; *SrcSum=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc = (*(pr+j)+*(pr+j+1)+*(pr+j+refStep)+ *(pr+j+1+refStep))/4;

							*Mean   +=  pr_mc;
							*SrcSum +=  *(ps+j);
						}
					}
					*Mean  =  *SrcSum - *Mean;
					*Mean /=  64;

					ps = pSrc + ii*srcStep + jj;
					pr = pRef + ii*refStep + jj;
					*Var=0;
					for(i=0;i<8;i++, ps+=srcStep, pr+= refStep)
					{
						for(j=0;j<8;j++)
						{
							pr_mc = (*(pr+j)+*(pr+j+1)+*(pr+j+refStep)+ *(pr+j+1+refStep))/4;
							sqr   =  *(ps+j)-pr_mc-*Mean;
							*Var +=  sqr*sqr;
						}
					}
					*Var /=  64;
					Var++;
					Mean++;
					SrcSum++;
				}
			}	
			break;
		default:
			break;
	}


	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiVarMeanDiff16x8_8u32s_C1R )(const Fw8u* pSrc, Fw32s srcStep,
										 const Fw8u* pRef, Fw32s refStep, 
										 Fw32s* pSrcSum, 
										 Fw32s* pVar, 
								
										 Fw32s* pMean, Fw32s mcType)
{
	if(srcStep < 8 || refStep < 8) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return VarMeanDiff16x8_8u32s_C1R_SSE2 (pSrc, srcStep, pRef, refStep, pSrcSum, pVar, pMean, mcType);
	else
		return VarMeanDiff16x8_8u32s_C1R_C (pSrc, srcStep, pRef, refStep, pSrcSum, pVar, pMean, mcType);

}

FwStatus  static Variance16x16_8u32s_SSE2 (const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar)
{
	int i;
	const Fw8u  *ps;
	Fw32s  Mean;

	if (pSrc == NULL) return fwStsNullPtrErr;

	__m128i xmm0, xmm1, xmm4, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi8(0);    // zero;

	*pVar = Mean = 0;
	ps = pSrc;
	for(i=0;i<16;i++, ps+=srcStep)
	{
//		for(j=0;j<16;j++)	Mean +=  *(ps+j);		
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)(ps+8) );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps
		xmm6 = _mm_add_epi16 (xmm6, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 2);
	xmm6 = _mm_add_epi16(xmm6, xmm0); 

	xmm6 = _mm_unpacklo_epi16(xmm6, xmm7);	// change 16bit to 32 bit
	Mean =  (Fw32s) _mm_cvtsi128_si32 (xmm6);

	Mean /=  256;

	xmm4  = _mm_set1_epi16 ((short)(Mean));    // mean;
	xmm6  = _mm_set1_epi8(0);    // zero;
	ps = pSrc;
	for(i=0;i<16;i++, ps+=srcStep)
	{
//		for(j=0;j<8;j++) {	sqr =  *(ps+j)-Mean;	*pVar +=  sqr*sqr;		}
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps

		xmm0 = _mm_sub_epi16(xmm0, xmm4);		// ps-mean
		xmm0 = _mm_madd_epi16 (xmm0, xmm0);			// sum of sqr
		xmm6 = _mm_add_epi32 (xmm6, xmm0);		// sum

		xmm1 = _mm_loadl_epi64( (__m128i*)(ps+8) );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps

		xmm1 = _mm_sub_epi16(xmm1, xmm4);		// ps-mean
		xmm1 = _mm_madd_epi16 (xmm1, xmm1);			// sum of sqr
		xmm6 = _mm_add_epi32 (xmm6, xmm1);		// sum
	}
	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 8);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	xmm0 = xmm6;
	xmm6 = _mm_srli_si128 (xmm6, 4);
	xmm6 = _mm_add_epi32(xmm6, xmm0); 

	*pVar = (Fw32s) _mm_cvtsi128_si32 (xmm6);

	*pVar /=  256;
	return fwStsNoErr;
}

FwStatus  static Variance16x16_8u32s_C (const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar)
{
	int i, j, sqr;
	const Fw8u   *ps;
	Fw32s  Mean;

	if (pSrc == NULL) return fwStsNullPtrErr;

	ps = pSrc;
	Mean=0;
	for(i=0;i<16;i++, ps+=srcStep)
		for(j=0;j<16;j++)	Mean +=  *(ps+j);		
	Mean /= 256;

	ps = pSrc;
	*pVar=0;
	for(i=0;i<16;i++, ps+=srcStep)	{
		for(j=0;j<16;j++)		{
			sqr   =  *(ps+j)-Mean;	
			*pVar +=  sqr*sqr;
		}
	}
	*pVar /= 256;

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiVariance16x16_8u32s )(const Fw8u* pSrc, Fw32s srcStep, Fw32s* pVar)
{
	if(srcStep < 16) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return Variance16x16_8u32s_SSE2 (pSrc, srcStep, pVar);
	else
		return Variance16x16_8u32s_C (pSrc, srcStep, pVar);
}



/*
Arguments
	pSrc --- Pointer to a 16x16 block in the current plan.
	srcStep --- Step of the current block, specifying width of the plane in bytes.
	EdgePelDifference --- The value for estimation of difference between neighboring elements.
	This value must be within the range of [0, 128].
	EdgePelCount --- The value for estimation of number of pairs of elements with ‘big
	difference’. This value must be within the range of [0, 128].
	pRes --- Pointer to output value. (*pRes) is equal to 1, if edges are detected, 
	and is equal to 0 if edges are not detected.
*/
FwStatus  static EdgesDetect16x16_SSE2 (const Fw8u *pSrc, Fw32u srcStep, 
									   Fw8u EdgePelDifference, Fw8u EdgePelCount, Fw8u *pRes)
{
	int i, j, count;
	const Fw8u   *ps;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm7, xmm6;
	Fw16u* p = (Fw16u*) fwMalloc(16);

	if (pSrc == NULL) return fwStsNullPtrErr;

	xmm6  = _mm_set1_epi16((Fw16u)EdgePelDifference);    // threshold
	xmm7  = _mm_set1_epi8(0);    // 1
	ps = pSrc;
	count = *pRes = 0;
	for(i=0;i<15;i++, ps+=srcStep)
	{
//		for(j=0;j<15;j++)
//			if ( (*ps - *(ps+1)) > EdgePelDifference ||
//				 (*ps - *(ps+srcStep)) > EdgePelDifference) count++;
		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm4 = xmm0;

		xmm1 = _mm_loadl_epi64( (__m128i*)(ps+1) );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps1

		xmm2 = _mm_loadu_si128 ( (__m128i*)(ps+srcStep) );	// ps+srcStep 
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// psr

		xmm0 = _mm_sub_epi16  (xmm0, xmm1);  //ps-(ps+1)
		xmm4 = _mm_sub_epi16  (xmm4, xmm2);  //ps-(psr)

		xmm0 = _mm_cmpgt_epi8(xmm0, xmm6);	 //ps-(ps+1) > threshold
		xmm4 = _mm_cmpgt_epi8(xmm4, xmm6);	 //ps-(psr)  > threshold

		xmm0 = _mm_or_si128(xmm0, xmm4);	//ps-(ps+1) > EdgePelDifference or

		_mm_storeu_si128 ((__m128i*)p, xmm0);

		for(j=0;j<8;j++) count+=(int)p[j];

		xmm0 = _mm_loadl_epi64( (__m128i*)(ps+8) );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
		xmm4 = xmm0;

		xmm1 = _mm_loadl_epi64( (__m128i*)(ps+9) );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps1

		xmm2 = _mm_loadu_si128 ( (__m128i*)(ps+8+srcStep) );	// ps+srcStep 
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// psr

		xmm0 = _mm_sub_epi16  (xmm0, xmm1);  //ps-(ps+1)
		xmm4 = _mm_sub_epi16  (xmm4, xmm2);  //ps-(psr)

		xmm0 = _mm_cmpgt_epi8(xmm0, xmm6);	 //ps-(ps+1) > threshold
		xmm4 = _mm_cmpgt_epi8(xmm4, xmm6);	 //ps-(psr)  > threshold

		xmm0 = _mm_or_si128(xmm0, xmm4);	//ps-(ps+1) > EdgePelDifference or

		_mm_storeu_si128 ((__m128i*)p, xmm0);

		for(j=0;j<7;j++) count+=(int)p[j];
	}
	count /= 255;
	if (count > EdgePelCount) *pRes = 1;

	fwFree(p);
	return fwStsNoErr;

}
FwStatus  static EdgesDetect16x16_C (const Fw8u *pSrc, Fw32u srcStep, 
									   Fw8u EdgePelDifference, Fw8u EdgePelCount, Fw8u *pRes)
{
/*
Description
	This function is declared in the fwvc.h header file. The function
	fwiEdgesDetect16x16_8u_C1R detects edges inside a 16x16 block: finds pair of
	neighboring (horizontal and vertical) elements with difference greater than
	EdgePelDifference.

	If the number of pairs is greater than EdgePelCount, edges are detected and flag (*pRes) is set
	to 1. Otherwise, edges are not detected ((*pRes) is set to 0).
.
res 0 = count 0 = row 0 14 [ , ] ¡Ê col 0 14 [ , ] ¡Ê
if  ((Src[row][col] - Src[row][col+1]) > EdgePelDifference)
OR																==> count++
	((Src[row][col] - Src[row+1][col]) > EdgePelDifference)

if (count > EdgePelCount) -> res = 1
*/
	int i, j, count, diff1, diff2;
	const Fw8u   *ps;

	if (pSrc == NULL) return fwStsNullPtrErr;
	ps = pSrc;
	count = *pRes = 0;
	for(i=0;i<15;i++, ps+=srcStep)
	{
		for(j=0;j<15;j++)
		{
			diff1 = (int)*(ps+j)-(int)*(ps+1+j);
			diff2 = (int)*(ps+j)-(int)*(ps+j+srcStep);
			if ( diff1 > EdgePelDifference || diff2 > EdgePelDifference) count++;
		}
	}
	if (count > EdgePelCount) *pRes = 1;

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiEdgesDetect16x16_8u_C1R )(const Fw8u *pSrc, Fw32u srcStep, 
									   Fw8u EdgePelDifference, Fw8u EdgePelCount, Fw8u *pRes)
{
	if(srcStep < 16) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return EdgesDetect16x16_SSE2(pSrc, srcStep, EdgePelDifference, EdgePelCount, pRes);
	else
		return EdgesDetect16x16_C(pSrc, srcStep, EdgePelDifference, EdgePelCount, pRes);

}
FwStatus  static SAD16x16_SSE2 (const Fw8u* pSrc, Fw32s srcStep, 
						 const Fw8u* pRef, Fw32s refStep, 
						       Fw32s* pSAD, Fw32s mcType)
{
	int i;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm7;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	xmm7  = _mm_set1_epi8(0);    // zero;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
				xmm1 = _mm_loadu_si128 ( (__m128i*)pr );
				xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
				xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
			}
			xmm1 = xmm7;
			xmm1 = _mm_srli_si128(xmm1, 8);   // move high 64bit down
			xmm7 = _mm_add_epi64 (xmm7, xmm1); // add high/low 64bit

			*pSAD = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.
			break;
		case FWVC_MC_APX_FH:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
				xmm1 = _mm_loadu_si128 ( (__m128i*)pr );
				xmm2 = _mm_loadu_si128 ( (__m128i*)(pr+refStep) );

				xmm1 = _mm_avg_epu8  (xmm1, xmm2);

				xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
				xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
			}
			xmm1 = xmm7;
			xmm1 = _mm_srli_si128(xmm1, 8);   // move high 64bit down
			xmm7 = _mm_add_epi64 (xmm7, xmm1); // add high/low 64bit

			*pSAD = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.
			break;
		case FWVC_MC_APX_HF:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
				xmm1 = _mm_loadu_si128 ( (__m128i*)pr );
				xmm2 = _mm_loadu_si128 ( (__m128i*)(pr+1) );

				xmm1 = _mm_avg_epu8  (xmm1, xmm2);

				xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
				xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
			}
			xmm1 = xmm7;
			xmm1 = _mm_srli_si128(xmm1, 8);   // move high 64bit down
			xmm7 = _mm_add_epi64 (xmm7, xmm1); // add high/low 64bit

			*pSAD = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.

			break;
		case FWVC_MC_APX_HH:

			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				xmm0 = _mm_loadu_si128 ( (__m128i*)ps );
				xmm1 = _mm_loadu_si128 ( (__m128i*)pr );
				xmm2 = _mm_loadu_si128 ( (__m128i*)(pr+1) );
				xmm1 = _mm_avg_epu8  (xmm1, xmm2);

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pr+refStep) );
				xmm4 = _mm_loadu_si128 ( (__m128i*)(pr+refStep+1) );
				xmm3 = _mm_avg_epu8  (xmm3, xmm4);

				xmm1 = _mm_avg_epu8  (xmm1, xmm3);

				xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
				xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
			}
			xmm1 = xmm7;
			xmm1 = _mm_srli_si128(xmm1, 8);   // move high 64bit down
			xmm7 = _mm_add_epi64 (xmm7, xmm1); // add high/low 64bit

			*pSAD = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.
			break;
		default:
			break;
	}

	return fwStsNoErr;
}
FwStatus  static SAD16x16_C (const Fw8u* pSrc, Fw32s srcStep, 
							  const Fw8u* pRef, Fw32s refStep, 
							  Fw32s* pSAD, Fw32s mcType)
{
	int i, j;
	const Fw8u   *ps, *pr;
	Fw32s  diff;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					diff =  *(pr+j)-*(ps+j);
					if(diff < 0) diff = -diff;
					*pSAD += diff;
				}
			}
			break;
		case FWVC_MC_APX_FH:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					diff =  ((int)*(pr+j)+(int)*(pr+j+refStep)+1)/2-*(ps+j);
					if(diff < 0) diff = -diff;
					*pSAD += diff;
				}
			}
			break;
		case FWVC_MC_APX_HF:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					diff =  (*(pr+j)+*(pr+j+1)+1)/2-*(ps+j);
					if(diff < 0) diff = -diff;
					*pSAD += diff;
				}
			}
			break;
		case FWVC_MC_APX_HH:
			ps = pSrc;
			pr = pRef;
			*pSAD=0;
			for(i=0;i<16;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<16;j++)
				{
					diff =  ((*(pr+j)+*(pr+j+1)+1)/2+
						     (*(pr+j+refStep)+*(pr+j+1+refStep)+1)/2+1)/2-*(ps+j);
					if(diff < 0) diff = -diff;
					*pSAD += diff;
				}
			}
			break;
		default:
			break;
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSAD16x16_8u32s )(const Fw8u* pSrc, Fw32s srcStep, 
							  const Fw8u* pRef, Fw32s refStep, 
							  Fw32s* pSAD, Fw32s mcType)
{
	if(srcStep < 16 || refStep < 16) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return SAD16x16_SSE2 (pSrc, srcStep, pRef, refStep, pSAD, mcType);
	else
		return SAD16x16_C (pSrc, srcStep, pRef, refStep, pSAD, mcType);
}
FwStatus  static SAD8x8_SSE2 (const Fw8u* pSrcCur, int srcCurStep, 
								const Fw8u* pSrcRef, Fw32s srcRefStep, 
								Fw32s * pDst, Fw32s mcType)
{
	int i;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm7;

	if (pSrcCur == NULL || pSrcRef == NULL) return fwStsNullPtrErr;
	mcType;
	xmm7  = _mm_set1_epi8(0);    // zero;
	ps = pSrcCur;
	pr = pSrcRef;
	*pDst=0;
	for(i=0;i<8;i++, ps+=srcCurStep, pr+=srcRefStep)
	{
		xmm0 = _mm_loadl_epi64 ( (__m128i*)ps );
		xmm1 = _mm_loadl_epi64 ( (__m128i*)pr );
		xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
		xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
	}

	*pDst = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.

	return fwStsNoErr;
}
FwStatus  static SAD8x8_C (const Fw8u* pSrcCur, int srcCurStep, 
								const Fw8u* pSrcRef, Fw32s srcRefStep, 
								Fw32s * pDst, Fw32s mcType)
{
	int i, j;
	const Fw8u   *ps, *pr;
	Fw32s  diff;

	if (pSrcCur == NULL || pSrcRef == NULL) return fwStsNullPtrErr;
	mcType;
	ps = pSrcCur;
	pr = pSrcRef;
	*pDst=0;
	for(i=0;i<8;i++, ps+=srcCurStep, pr+=srcRefStep)
	{
		for(j=0;j<8;j++)
		{
			diff =  *(pr+j)-*(ps+j);
			if(diff < 0) diff = -diff;
			*pDst += diff;
		}
	}
	return fwStsNoErr;
}

//mcType Reserved and must be 0.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSAD8x8_8u32s_C1R )(const Fw8u* pSrcCur, int srcCurStep, 
								const Fw8u* pSrcRef, Fw32s srcRefStep, 
								Fw32s * pDst, Fw32s mcType)
{
	mcType;
	if(srcCurStep < 8 || srcRefStep < 8) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return SAD8x8_SSE2 (pSrcCur, srcCurStep, pSrcRef, srcRefStep, pDst, 0);
	else
		return SAD8x8_C (pSrcCur, srcCurStep, pSrcRef, srcRefStep, pDst, 0);

}
//mcType Reserved and must be 0.
FwStatus  static SAD4x4_C (const Fw8u *pSrc, Fw32s srcStep, 
							Fw8u *pRef, Fw32s refStep, 
							Fw32s *pSAD, Fw32s mcType)
{
	int i, j;
	const Fw8u   *ps, *pr;
	Fw32s  diff;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;
	mcType;
	ps = pSrc;
	pr = pRef;
	*pSAD=0;
	for(i=0;i<4;i++, ps+=srcStep, pr+=refStep)
	{
		for(j=0;j<4;j++)
		{
			diff =  *(pr+j)-*(ps+j);
			if(diff < 0) diff = -diff;
			*pSAD += diff;
		}
	}
	return fwStsNoErr;
}
FwStatus  static SAD4x4_SSE2 (const Fw8u *pSrc, Fw32s srcStep, 
							Fw8u *pRef, Fw32s refStep, 
							Fw32s *pSAD, Fw32s mcType)
{
	int i, *a;
	const Fw8u   *ps, *pr;
	__m128i xmm0, xmm1, xmm7;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;
	mcType;
	xmm7  = _mm_set1_epi8(0);    // zero;
	ps = pSrc;
	pr = pRef;
	*pSAD=0;
	for(i=0;i<4;i++, ps+=srcStep, pr+=refStep)
	{
		a    = (int *)ps;
		xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
		a    = (int *)pr;
		xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
		xmm0 = _mm_sad_epu8(xmm0, xmm1); // SAD
		xmm7 = _mm_add_epi64 (xmm7, xmm0); // cumulate
	}

	*pSAD = (Fw32s)_mm_cvtsi128_si32 (xmm7); // convert to 32bit.

	return fwStsNoErr;
}

//mcType Reserved and must be 0.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSAD4x4_8u32s )(const Fw8u *pSrc, Fw32s srcStep, 
							Fw8u *pRef, Fw32s refStep, 
							Fw32s *pSAD, Fw32s mcType)
{
	mcType;
	if(srcStep < 4 || refStep < 4) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return SAD4x4_SSE2 (pSrc, srcStep, pRef, refStep, pSAD, 0);
	else
		return SAD4x4_C (pSrc, srcStep, pRef, refStep, pSAD, 0);
}

//mcType Reserved and must be 0.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSAD16x16Blocks8x8_8u16u )(const Fw8u  *pSrc, Fw32s srcStep,
											 Fw8u  *pRef, Fw32s refStep, 
											 Fw16u *pDstSAD, Fw32s mcType)
{
/*
This function is declared in the fwvc.h header file. The function
fwiSAD16x16Blocks8x8_8u16u evaluates the four partial sums of absolute differences of all
the elements in current 16x16 block and the corresponding elements in reference 16x16 block. 
The result is stored in pRes.
*/
	int ii, jj;
	const Fw8u   *ps;
	Fw8u		  *pr;
	Fw32s		  SAD;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;
	if(srcStep < 16 || refStep < 16) return fwStsSizeErr;

	ps = pSrc;
	pr = pRef;
	for(ii=0;ii<16;ii+=8)
	{
		for(jj=0;jj<16;jj+=8)
		{
			ps = pSrc + ii*srcStep + jj;
			pr = pRef + ii*refStep + jj;
/*
			*pDstSAD=0;
			for(i=0;i<8;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<8;j++)
				{
					diff =  *(pr+j)-*(ps+j);
					if(diff < 0) diff = -diff;
					*pDstSAD += diff;
				}
			}
*/
			fwiSAD8x8_8u32s_C1R (ps, srcStep, pr, refStep, &SAD, mcType);
			*pDstSAD=(Fw16u)SAD;
			pDstSAD++;
		}
	}			
	return fwStsNoErr;
}



//mcType Reserved and must be 0.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSAD16x16Blocks4x4_8u16u )(const Fw8u *pSrc, Fw32s srcStep, 
											 Fw8u *pRef, Fw32s refStep, 
											 Fw16u *pDstSAD, Fw32s mcType)
{
	int ii, jj;
	const Fw8u  *ps;
	Fw8u *pr;
	Fw32s	SAD;

	if (pSrc == NULL || pRef == NULL) return fwStsNullPtrErr;
	if(srcStep < 16 || refStep < 16) return fwStsSizeErr;

	ps = pSrc;
	pr = pRef;
	for(ii=0;ii<16;ii+=4)
	{
		for(jj=0;jj<16;jj+=4)
		{
			ps = pSrc + ii*srcStep + jj;
			pr = pRef + ii*refStep + jj;
/*
			*pDstSAD=0;
			for(i=0;i<4;i++, ps+=srcStep, pr+=refStep)
			{
				for(j=0;j<4;j++)
				{
					diff =  *(pr+j)-*(ps+j);
					if(diff < 0) diff = -diff;
					*pDstSAD += diff;
				}
			}
*/
			fwiSAD4x4_8u32s (ps, srcStep, pr, refStep, &SAD, mcType);
			*pDstSAD=(Fw16u)SAD;
			pDstSAD++;
		}
	}			
	return fwStsNoErr;
}

FwStatus  static SumsDiff16x16Blocks4x4_SSE2 (Fw8u*  pSrc,  Fw32s  srcStep,
									   Fw8u*  pPred, Fw32s  predStep, 
									   Fw16s* pSums, Fw16s* pDiff)
{
/*
The function fwiSumsDiff16x16Blocks4x4_8u16s_C1 evaluates difference between current and
reference 4x4 blocks and calculates sums of 4x4 residual blocks.

pDiff[(k*4+l)*16+i*4+j] = pSrc[(k*4+i)*srcStep+(l*4+j)]-pPred[(k*4+i)*predStep+(l*4+j)]

where  k,l,i,j in [0,3]
			
		    15
pSums[n] = SUM pDiff[n*16+i],		where n in [0, 15]
		    i=0
*/
	int k, l;
	Fw8u   *ps, *pr;
	Fw16s  *pd;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	
	if (pSrc == NULL || pPred == NULL) return fwStsNullPtrErr;

// Fw16s* p = (Fw16s*) fwMalloc(16);
// Fw8u*  pb= (Fw8u*) fwMalloc(16);

	xmm7  = _mm_set1_epi8(0);    // zero;

	pd = pDiff;
	for(k=0;k<16;k+=4)
	{
		for(l=0;l<16;l+=8)
		{
			ps   = pSrc  + k*srcStep  + l;
			pr   = pPred + k*predStep + l;

			xmm0 = _mm_loadl_epi64( (__m128i*)ps );
			xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
				
			xmm1 = _mm_loadl_epi64( (__m128i*)pr );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
			
			xmm0 = _mm_sub_epi16(xmm0, xmm1);		// pSrcCur-pr <- line 1

			xmm2 = _mm_loadl_epi64( (__m128i*)(ps+srcStep) );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps
				
			xmm3 = _mm_loadl_epi64( (__m128i*)(pr+predStep) );
			xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr
			
			xmm2 = _mm_sub_epi16(xmm2, xmm3);		// pSrcCur-pr <- line 2

			xmm3 = xmm0;

			xmm0 = _mm_unpacklo_epi64(xmm0, xmm2);	// for block-1
			xmm3 = _mm_unpackhi_epi64(xmm3, xmm2);  // for block-2

/////////////////////////////////////////////////////////////
			xmm4 = _mm_loadl_epi64( (__m128i*)(ps+srcStep*2) );
			xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps
				
			xmm1 = _mm_loadl_epi64( (__m128i*)(pr+predStep*2) );
			xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
			
			xmm4 = _mm_sub_epi16(xmm4, xmm1);		// pSrcCur-pr <- line 1

			xmm2 = _mm_loadl_epi64( (__m128i*)(ps+srcStep*3) );
			xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps
				
			xmm5 = _mm_loadl_epi64( (__m128i*)(pr+predStep*3) );
			xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr
			
			xmm2 = _mm_sub_epi16(xmm2, xmm5);		// pSrcCur-pr <- line 2

			xmm6 = xmm4;

			xmm4 = _mm_unpacklo_epi64(xmm4, xmm2);	// for block-1
			xmm6 = _mm_unpackhi_epi64(xmm6, xmm2);  // for block-2

			_mm_storeu_si128( (__m128i*)pd, xmm0 );	// block-1
			pd += 8;
			_mm_storeu_si128( (__m128i*)pd, xmm4 ); // block-1
			pd += 8;
			_mm_storeu_si128( (__m128i*)pd, xmm3 ); // block-2 
			pd += 8;
			_mm_storeu_si128( (__m128i*)pd, xmm6 ); // block-2
			pd += 8;
/////////////////////////////////////////////////////////////

			xmm0 = _mm_add_epi16(xmm0, xmm4);		// pSrcCur-pr <- block-1
			xmm5 = xmm0;
			xmm5 = _mm_srli_si128 (xmm5, 8);
			xmm5 = _mm_add_epi16(xmm5, xmm0); 
			xmm0 = xmm5;
			xmm5 = _mm_srli_si128 (xmm5, 4);
			xmm5 = _mm_add_epi16(xmm5, xmm0);
			xmm0 = xmm5;
			xmm5 = _mm_srli_si128 (xmm5, 2);
			xmm5 = _mm_add_epi16(xmm5, xmm0);

			xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// convert shot to int

			*pSums = (Fw16s) _mm_cvtsi128_si32 (xmm5);

			pSums++;

			xmm3 = _mm_add_epi16(xmm3, xmm6);		// pSrcCur-pr <- block-1
			xmm5 = xmm3;
			xmm5 = _mm_srli_si128 (xmm5, 8);
			xmm5 = _mm_add_epi16(xmm5, xmm3); 
			xmm3 = xmm5;
			xmm5 = _mm_srli_si128 (xmm5, 4);
			xmm5 = _mm_add_epi16(xmm5, xmm3);
			xmm3 = xmm5;
			xmm5 = _mm_srli_si128 (xmm5, 2);
			xmm5 = _mm_add_epi16(xmm5, xmm3);

			xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// convert shot to int

			*pSums = (Fw16s) _mm_cvtsi128_si32 (xmm5);

			pSums++;

		}
	}
/*
int n,i;
	for(n=0;n<16;n++)	{
		int sum=0;
printf("\n S-");
		for(i=0; i<16; i++) sum += pDiff[n*16+i];
		pSums[n] = (Fw16s)sum;
printf("%d ",pSums[n]);
	}
*/

	return fwStsNoErr;
}

FwStatus  static SumsDiff16x16Blocks4x4_C (Fw8u*  pSrc,  Fw32s srcStep,
									Fw8u*  pPred, Fw32s predStep, 
									Fw16s* pSums, Fw16s* pDiff)
{
/*
The function fwiSumsDiff16x16Blocks4x4_8u16s_C1 evaluates difference between current and
reference 4x4 blocks and calculates sums of 4x4 residual blocks.

pDiff[(k*4+l)*16+i*4+j] = pSrc[(k*4+i)*srcStep +(l*4+j)]-
                         pPred[(k*4+i)*predStep+(l*4+j)]

where  k,l,i,j in [0,3]
			
		    15
pSums[n] = SUM pDiff[n*16+i],		where n in [0, 15]
		    i=0
*/
	int i, j, k, l, n;

	if (pSrc == NULL || pPred == NULL) return fwStsNullPtrErr;

	for(k=0;k<4;k++)
	{
		for(l=0;l<4;l++)
		{
			for(i=0;i<4;i++)
			{
				for(j=0; j<4;j++)
				{
					pDiff[(k*4+l)*16+ i*4+j]=
						 pSrc[(k*4+i)*srcStep  + (l*4+j)] - 
						pPred[(k*4+i)*predStep + (l*4+j)];
				}
			}
		}
	}

	for(n=0;n<16;n++)	{
		int sum=0;
		for(i=0; i<16; i++)	sum += pDiff[n*16+i];
		pSums[n] = (Fw16s)sum;
	}
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumsDiff16x16Blocks4x4_8u16s_C1 )(Fw8u * pSrc,  Fw32s srcStep,
											   Fw8u * pPred, Fw32s predStep, 
											   Fw16s* pSums, Fw16s* pDiff)
{
	if(srcStep < 16 || predStep < 16) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return SumsDiff16x16Blocks4x4_SSE2 (pSrc, srcStep, pPred, predStep, pSums, pDiff);
	else
		return SumsDiff16x16Blocks4x4_C (pSrc, srcStep, pPred, predStep, pSums, pDiff);
}
FwStatus static SumsDiff8x8Blocks4x4_SSE2 (Fw8u*  pSrc,  Fw32s  srcStep,
									 Fw8u*  pPred, Fw32s  predStep, 
									 Fw16s* pSums, Fw16s* pDiff)
{
	int k;
	Fw8u   *ps, *pr;
	Fw16s  *pd;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	
	if (pSrc == NULL || pPred == NULL) return fwStsNullPtrErr;

	xmm7  = _mm_set1_epi8(0);    // zero;
	pd = pDiff;
//	for(k=0;k<2;k++)
	for(k=0;k<8;k+=4)
	{
// pDiff[(k*2+l)*16+i*4+j]=
// pSrc[(k*4+i)*srcStep + (l*4+j)] - pPred[k*4+i)*predStep + (l*4+j)]
// where  i,j in [0,3], k,l in [0,1]
		ps = pSrc  + k*srcStep;
		pr = pPred + k*predStep;

		xmm0 = _mm_loadl_epi64( (__m128i*)ps );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// ps
			
		xmm1 = _mm_loadl_epi64( (__m128i*)pr );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
		
		xmm0 = _mm_sub_epi16(xmm0, xmm1);		// pSrcCur-pr <- line 1

		xmm2 = _mm_loadl_epi64( (__m128i*)(ps+srcStep) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps
			
		xmm3 = _mm_loadl_epi64( (__m128i*)(pr+predStep) );
		xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// pr
		
		xmm2 = _mm_sub_epi16(xmm2, xmm3);		// pSrcCur-pr <- line 2

		xmm3 = xmm0;

		xmm0 = _mm_unpacklo_epi64(xmm0, xmm2);	// for block-1
		xmm3 = _mm_unpackhi_epi64(xmm3, xmm2);  // for block-2

////////////////////////////////////////////////////////////
		xmm4 = _mm_loadl_epi64( (__m128i*)(ps+srcStep*2) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// ps
				
		xmm1 = _mm_loadl_epi64( (__m128i*)(pr+predStep*2) );
		xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// pr
			
		xmm4 = _mm_sub_epi16(xmm4, xmm1);		// pSrcCur-pr <- line 1

		xmm2 = _mm_loadl_epi64( (__m128i*)(ps+srcStep*3) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// ps
				
		xmm5 = _mm_loadl_epi64( (__m128i*)(pr+predStep*3) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// pr
			
		xmm2 = _mm_sub_epi16(xmm2, xmm5);		// pSrcCur-pr <- line 2

		xmm6 = xmm4;

		xmm4 = _mm_unpacklo_epi64(xmm4, xmm2);	// for block-1
		xmm6 = _mm_unpackhi_epi64(xmm6, xmm2);  // for block-2

		_mm_storeu_si128( (__m128i*)pd, xmm0 );	// block-1
		pd += 8;
		_mm_storeu_si128( (__m128i*)pd, xmm4 ); // block-1
		pd += 8;
		_mm_storeu_si128( (__m128i*)pd, xmm3 ); // block-2 
		pd += 8;
		_mm_storeu_si128( (__m128i*)pd, xmm6 ); // block-2
		pd += 8;
//		     15
//pSums[n] = SUM pDiff[n*16+i],	where n in [0, 3]
// 		     i=0
		xmm0 = _mm_add_epi16(xmm0, xmm4);		// pSrcCur-pr <- block-1
		xmm5 = xmm0;
		xmm5 = _mm_srli_si128 (xmm5, 8);
		xmm5 = _mm_add_epi16(xmm5, xmm0); 
		xmm0 = xmm5;
		xmm5 = _mm_srli_si128 (xmm5, 4);
		xmm5 = _mm_add_epi16(xmm5, xmm0);
		xmm0 = xmm5;
		xmm5 = _mm_srli_si128 (xmm5, 2);
		xmm5 = _mm_add_epi16(xmm5, xmm0);

		xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// convert shot to int
		*pSums = (Fw16s) _mm_cvtsi128_si32 (xmm5);
		pSums++;

		xmm3 = _mm_add_epi16(xmm3, xmm6);		// pSrcCur-pr <- block-1
		xmm5 = xmm3;
		xmm5 = _mm_srli_si128 (xmm5, 8);
		xmm5 = _mm_add_epi16(xmm5, xmm3); 
		xmm3 = xmm5;
		xmm5 = _mm_srli_si128 (xmm5, 4);
		xmm5 = _mm_add_epi16(xmm5, xmm3);
		xmm3 = xmm5;
		xmm5 = _mm_srli_si128 (xmm5, 2);
		xmm5 = _mm_add_epi16(xmm5, xmm3);

		xmm5 = _mm_unpacklo_epi16(xmm5, xmm7);	// convert shot to int
		*pSums = (Fw16s) _mm_cvtsi128_si32 (xmm5);
		pSums++;
	}
/*
int n, i;
	for(n=0;n<4;n++)	{
		int sum=0;
		for(i=0; i<16; i++)		{
			sum += pDiff[n*16+i];
		}
		pSums[n] = (Fw16s)sum;
	}
*/
	return fwStsNoErr;
}

FwStatus static SumsDiff8x8Blocks4x4_C (Fw8u* pSrc, Fw32s srcStep,
											Fw8u* pPred, Fw32s predStep, 
											Fw16s* pSums, Fw16s* pDiff)
{
	int i, j, k, l, n;
//	Fw8u   *ps, *pr;
//	Fw32s  diff;

	if (pSrc == NULL || pPred == NULL) return fwStsNullPtrErr;

/*
The function fwiSumsDiff8x8Blocks4x4_8u16s_C1 evaluates difference between current and 
reference 4x4 blocks and calculates sums of 4x4 residual blocks elements 

pDiff[(k*2+l)*16+i*4+j]=
		pSrc[(k*4+i)*srcStep+ (l*4+j)] - 
		pPred[k*4+i)*predStep+(l*4+j)]
where  i,j in [0,3], k,l in [0,1]
		    15
pSums[n] = SUM pDiff[n*16+i],	where n in [0, 3]
		    i=0
*/
	for(k=0;k<2;k++)
	{
		for(l=0;l<2;l++)
		{
			for(i=0;i<4;i++)
			{
				for(j=0; j<4;j++)
				{
					pDiff[(k*2+l)*16+i*4+j]=
						 pSrc[(k*4+i)*srcStep +(l*4+j)] - 
						pPred[(k*4+i)*predStep+(l*4+j)];
				}
			}
		}
	}
	for(n=0;n<4;n++)	{
		int sum=0;
		for(i=0; i<16; i++)		{
			sum += pDiff[n*16+i];
		}
		pSums[n] = (Fw16s)sum;
	}
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumsDiff8x8Blocks4x4_8u16s_C1 )(Fw8u* pSrc, Fw32s srcStep,
											Fw8u* pPred, Fw32s predStep, 
											Fw16s* pSums, Fw16s* pDiff)
{
	if(srcStep < 8 || predStep < 8) return fwStsSizeErr;

	if( Dispatch_Type == DT_SSE2)
		return SumsDiff8x8Blocks4x4_SSE2 (pSrc, srcStep, pPred, predStep, pSums, pDiff);
	else
		return SumsDiff8x8Blocks4x4_C (pSrc, srcStep, pPred, predStep, pSums, pDiff);
}


#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
