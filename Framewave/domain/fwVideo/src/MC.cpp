/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
//#include "fwImage.h"
#include "fwVideo.h"
#include "system.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )

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

/*
static int Dispatch_Type = DT_REFR;
void Dispatch_MC_Type_func(int DT_type)
{
	Dispatch_Type = DT_type;
}
*/

/*
pSrcRef:   Pointer to the reference intra block.
srcStep:   Size of the row in bytes, specifying the aligned reference frame width.
pSrcYData: Pointer to the data obtained after inverse DCT.
srcYDataStep: Number of bytes, specifying the width of the aligned data obtained after iDCT.
pDs:       Pointer to the destination predicted block.
dstStep:   Size of the row in bytes, specifying the aligned destination frame width.
mcType:    MC type FWVC_MC_APX.
roundControl: type of rounding for half a pel approximation; may be 0 or 1.
*/
void static MC_FF_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j, diff;
	roundControl;
	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
			diff =  (*(pSrcRef+j) + *(pSrcYData+j));
			if(diff < 0)		diff =  0;
			else if(diff > 255) diff = 255;
			*(pDst+j) =  (Fw8u)diff;
		}
	}
}
typedef struct 
{
	const Fw8u *pSrcRef;
	Fw32s srcStep;
	const Fw16s *pSrcYData;
	Fw32s srcYDataStep;
	Fw8u  *pDst;
	Fw32s dstStep;
	int    width;	 
	int    height;
	Fw32s roundControl;
}	MC_FF_8u_C1_par;

void  static MC_FF_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					         Fw8u  *pDst,      Fw32s dstStep, 
						     int    width,		 int     height,	 
						     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm5  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	roundControl;
	switch (width)
	{
		case 16:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			srcYDataStep/=2;
			for(i=0;i<height/2;i++)
			{
//				for(j=0;j<width;j++)
//					*(pDst+j) =  (*(pSrcRef+j) + *(pSrcYData+j)) & 0xff;
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );

				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128( (__m128i*)pSrcYData );
				xmm3 = _mm_loadu_si128( (__m128i*)(pSrcYData+8) );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// 

//				xmm1    = xmm0;
//				xmm3    = xmm2;
//				xmm1    = _mm_cmpgt_epi16(xmm1, xmm7);		// < 0
//				xmm3    = _mm_cmpgt_epi16(xmm3, xmm7);	
//				xmm0	= _mm_and_si128(xmm0, xmm1);		// remove negative number
//				xmm2	= _mm_and_si128(xmm2, xmm3);		// remove negative number

				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;

				xmm8 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );

				xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);	// u8->u16
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128( (__m128i*)pSrcYData );
				xmm3 = _mm_loadu_si128( (__m128i*)(pSrcYData+8) );

				xmm8 = _mm_add_epi16(xmm8, xmm1);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// 

//				xmm1    = xmm8;
//				xmm3    = xmm2;
//				xmm1    = _mm_cmpgt_epi16(xmm1, xmm7);		// < 0
//				xmm3    = _mm_cmpgt_epi16(xmm3, xmm7);	
//				xmm8	= _mm_and_si128(xmm8, xmm1);		// remove negative number
//				xmm2	= _mm_and_si128(xmm2, xmm3);		// remove negative number

				xmm8 = _mm_packus_epi16 (xmm8, xmm2);			// 16bit to 8bit

			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			pDst+=dstStep;
				_mm_storeu_si128( (__m128i*)pDst, xmm8 ); // store 16 bytes
			pDst+=dstStep;
			}
		}
		else	// unaligned
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//				for(j=0;j<width;j++)
//					*(pDst+j) =  (*(pSrcRef+j) + *(pSrcYData+j)) & 0xff;
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
//				xmm0 = _mm_add_epi16(xmm0, xmm6);		// ROUND

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
//				xmm2 = _mm_add_epi16(xmm2, xmm6);		// ROUND

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
		}
			break;
		case 8:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			srcYDataStep/=2;
			for(i=0;i<height/2;i++)
			{
//				for(j=0;j<width;j++)
//					*(pDst+j) =  (*(pSrcRef+j) + *(pSrcYData+j)) & 0xff;

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
//				xmm0 = _mm_add_epi16(xmm0, xmm6);		// ROUND
#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif

				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;
			
				xmm8 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm8 = _mm_add_epi16(xmm8, xmm1);		// +
//				xmm8 = _mm_add_epi16(xmm8, xmm6);		// ROUND
#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm8 = _mm_and_si128(xmm8, xmm4);		// & 0xff
#endif
				xmm8 = _mm_packus_epi16 (xmm8, xmm8);			// 16bit to 8bit

			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			pDst+=dstStep;
				_mm_storel_epi64( (__m128i*)pDst, xmm8 ); // store 8 bytes
			pDst+=dstStep;
			
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//				for(j=0;j<width;j++)
//					*(pDst+j) =  (*(pSrcRef+j) + *(pSrcYData+j)) & 0xff;

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
//				xmm0 = _mm_add_epi16(xmm0, xmm6);		// ROUND
#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif

				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			
			}
		}
			break;
		case 4:
			int *a;
			srcYDataStep/=2;
			for(i=0;i<height/2;i++)
			{
//				for(j=0;j<width;j++)
//					*(pDst+j) =  (*(pSrcRef+j) + *(pSrcYData+j)) & 0xff;
				a    = (int *)pSrcRef;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
//				xmm0 = _mm_add_epi16(xmm0, xmm6);		// ROUND

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;
			
				a    = (int *)pSrcRef;
				xmm8 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);	// u8->u16

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

				xmm8 = _mm_add_epi16(xmm8, xmm1);		// +
//				xmm8 = _mm_add_epi16(xmm8, xmm6);		// ROUND

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm8 = _mm_and_si128(xmm8, xmm4);		// & 0xff
#endif

				xmm8 = _mm_packus_epi16 (xmm8, xmm8);			// 16bit to 8bit
				
			pSrcRef+=srcStep;
			pSrcYData+=srcYDataStep;

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
			pDst+=dstStep;
//				_mm_storel_epi64( (__m128i*)pDst, xmm8 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm8);			// get 4 bytes from src
			pDst+=dstStep;

			}
			break;
		default:
			MC_FF_8u_C1_C(pSrcRef,  srcStep, pSrcYData, srcYDataStep, 
					      pDst, dstStep, width, height, roundControl);
			break;
	}
}
void  static MC_FH_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j, summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
			summ  = (int)*(pSrcRef+j);
			summ += (int)*(pSrcRef+j+srcStep);
			summ += roundControl;
			summ /= 2;
			summ += (int)*(pSrcYData+j);

//			*(pDst+j) = (Fw8u) summ & 0xff;
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

//			*(pDst+j) = (Fw8u)((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
//											 + *(pSrcYData+j)) & 0xff;
		}
	}
}
void  static MC_FH_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					         Fw8u  *pDst,      Fw32s dstStep, 
						     int    width,		 int     height,	 
						     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	switch (width)
	{
		case 16:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			if(roundControl == 0)
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;
					
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
	//				xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
	//				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
//					xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
					xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
	//				xmm2 = _mm_avg_epu8 (xmm2, xmm5);		// +
	//				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

					xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

					xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

					_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;
					
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );

					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
	
					xmm2 = _mm_avg_epu8 (xmm2, xmm5);		// +
					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

					xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

					xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

					_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
				}
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
//													 + *(pSrcYData+j)) & 0xff;
				
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2



				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
		}
			break;
		case 8:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			if(roundControl == 0)
			{

				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;

					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;

					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
//					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
//					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
			}
		}
		else
		{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;

					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif

					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
		}
			break;
		case 4:
			int *a;
			if(roundControl == 0)
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;
					a    = (int *)pSrcRef;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					a    = (int *)(pSrcRef+srcStep);
					xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff

	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

	//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
					a    = (int *)pDst;
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
	//													 + *(pSrcYData+j)) & 0xff;
					a    = (int *)pSrcRef;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
//					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					a    = (int *)(pSrcRef+srcStep);
					xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
//					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

	//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
					a    = (int *)pDst;
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				}
			}

			break;
		default:
			MC_FH_8u_C1_C(pSrcRef,  srcStep, pSrcYData, srcYDataStep, 
					      pDst, dstStep, width, height, roundControl);
			break;
	}
}
void  static MC_HF_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//			*(pDst+j) =  (Fw8u)((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;
			summ =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2 +*(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;
		}
	}
}
void  static MC_HF_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					         Fw8u  *pDst,      Fw32s dstStep, 
						     int    width,		 int     height,	 
						     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	switch (width)
	{
		case 16:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			if(roundControl == 0)
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1+8) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
//					xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
					xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

					xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

					xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

					_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
//					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
//					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
//					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1+8) );
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
//					xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
//					xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
					xmm2 = _mm_avg_epu8 (xmm2, xmm5);		// +
					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

					xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

					xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

					xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

					_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
				}
			}
		}
		else
		{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

					xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
					xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1+8) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
					xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
					xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

					xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

					xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
					xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

					_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
				}
		}
			break;
		case 8:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			if(roundControl == 0)
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;

					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
//					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
//					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadu_si128( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
			}
		}
		else
		{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;

					xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

					_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				
				}
		}
			break;
		case 4:
			int *a;
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			if(roundControl == 0)
			{

				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					a    = (int *)pSrcRef;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					a    = (int *)(pSrcRef+1);
					xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

	//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
					a    = (int *)pDst;
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				}
			}
			else
			{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					a    = (int *)pSrcRef;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
//					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16
					a    = (int *)(pSrcRef+1);
					xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
//					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

//					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
//					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
//					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2
					xmm0 = _mm_avg_epu8 (xmm0, xmm5);		// +
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

	//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
					a    = (int *)pDst;
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				}
			}
		}
		else
		{
				for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
				{
	//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
	//													 +*(pSrcYData+j)) & 0xff;
					a    = (int *)pSrcRef;
					xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

					a    = (int *)(pSrcRef+1);
					xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
					xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

					xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
					xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
					xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

					xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

					xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

	#if 0
					xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
					xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
	#endif
					xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

	//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
					a    = (int *)pDst;
					*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
				}
		}
			break;
		default:
			MC_HF_8u_C1_C(pSrcRef,  srcStep, pSrcYData, srcYDataStep, 
					      pDst, dstStep, width, height, roundControl);
			break;
	}
}
void  static MC_HH_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//			*(pDst+j) =  (Fw8u)((*(pSrcRef+j)+*(pSrcRef+j+1)+ 
//						   *(pSrcRef+j+srcStep)+*(pSrcRef+j+1+srcStep)+roundControl*2)/4
//													 +*(pSrcYData+j)) & 0xff;
			summ = ((*(pSrcRef+j)+*(pSrcRef+j+1)+ 
						   *(pSrcRef+j+srcStep)+*(pSrcRef+j+1+srcStep)+roundControl*2)/4
													 +*(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

		}
	}
}
void  static MC_HH_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					         Fw8u  *pDst,      Fw32s dstStep, 
						     int    width,		 int     height,	 
						     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl*2);    // round;

	switch (width)
	{
		case 16:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+ 
//						   *(pSrcRef+j+srcStep)+*(pSrcRef+j+1+srcStep)+roundControl*2)/4
//													 +*(pSrcYData+j)) & 0xff;
//				
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm3);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+9) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+9) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm4);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +


				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+ 
//						   *(pSrcRef+j+srcStep)+*(pSrcRef+j+1+srcStep)+roundControl*2)/4
//													 +*(pSrcYData+j)) & 0xff;
//				
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm3);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+9) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+9) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm4);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );
				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
		}
			break;
		case 8:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm3);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;

				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+1) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm3 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+1) );
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm3);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
		}
			break;
		case 4:
			int *a;
			for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
//			*(pDst+j) =  ((*(pSrcRef+j)+*(pSrcRef+j+1)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;
				a    = (int *)pSrcRef;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				a    = (int *)(pSrcRef+1);
				xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				a    = (int *)(pSrcRef+srcStep);
				xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				a    = (int *)(pSrcRef+srcStep+1);
				xmm3 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm3 = _mm_unpacklo_epi8(xmm3, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm3);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short
				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src

			}
			break;
		default:
			MC_HH_8u_C1_C(pSrcRef,  srcStep, pSrcYData, srcYDataStep, 
					      pDst, dstStep, width, height, roundControl);
			break;
	}
}

FwStatus static iMC_8u_C1(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						      Fw8u  *pDst,      Fw32s dstStep, 
							  Fw32s  mcType,    Fw32s roundControl,
							  int    width,		 int     height)
{
	Fw8u  *ps;
	Fw16s *psY ;
	Fw8u  *pd;

	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	if (pSrcRef == NULL || pSrcYData == NULL || pDst == NULL ) return fwStsNullPtrErr;

	if(roundControl == 0) roundControl = 1;
	else				  roundControl = 0;

	ps  = (Fw8u *) pSrcRef;
	psY = (Fw16s *) pSrcYData;
	pd  = (Fw8u*) pDst;

	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type == DT_SSE2)
			{
			    MC_FF_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			else
			{
			    MC_FF_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type == DT_SSE2)
			{
			    MC_FH_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			else
			{
			    MC_FH_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type == DT_SSE2)
			{
			    MC_HF_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			else
			{
			    MC_HF_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type == DT_SSE2)
			{
			    MC_HH_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			else
			{
			    MC_HH_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			}
			break;
		default:
			break;
	}
	return fwStsNoErr;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x16_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
							 const Fw16s *pSrcYData, Fw32s srcYDataStep, 
							       Fw8u  *pDst,      Fw32s dstStep, 
							       Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 16 || srcYDataStep < 32 || dstStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;
	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,16,16) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x8_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
							const Fw16s *pSrcYData, Fw32s srcYDataStep, 
							       Fw8u  *pDst,      Fw32s dstStep, 
							       Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,16,8) ;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x16_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
							const Fw16s *pSrcYData, Fw32s srcYDataStep, 
							      Fw8u  *pDst,      Fw32s dstStep, 
							      Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 16 || srcYDataStep < 32 || dstStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,8,16) ;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x8_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,8,8) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x4_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,8,4) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x8_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,4,8) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x4_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,4,4) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC2x4_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,2,4) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x2_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
						         Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 2 || srcYDataStep < 4 || dstStep < 2) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,4,2) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC2x2_8u_C1 )(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						         Fw8u  *pDst,      Fw32s dstStep, 
							     Fw32s  mcType,    Fw32s roundControl)
{
	if(srcStep < 2 || srcYDataStep < 4 || dstStep < 2) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,2,2) ;
}


/*
Description
This function is declared in the fwvc.h header file. The function fwiMC16x8UV_8u_C1
calculates sum of 16x8 residual UV block and 16x8 predicted UV block for reconstruction of the
source UV block (See Figure 16-2). Prediction is calculated on the basis of the reference UV
block and mcType (see FWVC_MC_APX).
Half sample prediction is calculated taking into account structure of UV block. So (horizontally)
neighboring elements of U-block (or V-block) have indexes i and i+2 in UV block.
*/
/*
UV Block
UV block of size (2*H)xW combines U block of size HxW and V block of size HxW. The formula
for UV is as follows:
,
Figure 16-1 Predictions for 4x4 Reference Block
elements of reference block (FF prediction)
Elements of half sample predictions
FH HF HH
UV[2*i+1+j] =  V[i,j]
UV[2*i+j]   =  U[i,j]
where i=[0, H-1] and j=[0, W-1]. See the figure below for an example of a UV block.
*/
void  static MCUV_FF_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	roundControl;
	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
			summ = (*(pSrcRef+j)   + *(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;
			summ = (*(pSrcRef+j+1) + *(pSrcYData+j+1));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}
}
void  static MCUV_FF_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					     const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					           Fw8u  *pDst,      Fw32s dstStep, 
						       int    width,		 int     height,	 
						       Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	width;
	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2) {
//			*(pDst+j)   =  (*(pSrcRef+j)   + *(pSrcYData+j)) & 0xff;
//			*(pDst+j+1) =  (*(pSrcRef+j+1) + *(pSrcYData+j+1)) & 0xff;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

		xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

//		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
//		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
//		xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff

		xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes

	}
}
void  static MCUV_FH_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
			summ = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
													 + *(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;
			summ = ((*(pSrcRef+j+1)+*(pSrcRef+j+srcStep+1)+roundControl)/2
													 + *(pSrcYData+j+1));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}

}
void  static MCUV_FH_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					     const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					           Fw8u  *pDst,      Fw32s dstStep, 
						       int    width,		 int     height,	 
						       Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	width;
	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)		{
//			*(pDst+j)  = ((*(pSrcRef+j)+*(pSrcRef+j+srcStep)+roundControl)/2
//													 + *(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)= ((*(pSrcRef+j+1)+*(pSrcRef+j+srcStep+1)+roundControl)/2
//													 + *(pSrcYData+j+1)) & 0xff;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRef );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRef+8) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRef+srcStep+8) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

//		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
//		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
//		xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff

		xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
	}
}
void  static MCUV_HF_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
//			*(pDst+j)  =  (Fw8u)((*(pSrcRef+j)+*(pSrcRef+j+2)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=  (Fw8u)((*(pSrcRef+j+1)+*(pSrcRef+j+2+1)+roundControl)/2
//													 +*(pSrcYData+j+1)) & 0xff;

			summ = ((*(pSrcRef+j)+*(pSrcRef+j+2)+roundControl)/2 +*(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

			summ = ((*(pSrcRef+j+1)+*(pSrcRef+j+2+1)+roundControl)/2 +*(pSrcYData+j+1));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}
}
void  static MCUV_HF_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					     const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					           Fw8u  *pDst,      Fw32s dstStep, 
						       int    width,		 int     height,	 
						       Fw32s roundControl)
{
	int i;
	__m128i xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	xmm8  = _mm_set1_epi8(0);    // zero;
	width;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)		{
//			*(pDst+j)  =  ((*(pSrcRef+j)+*(pSrcRef+j+2)+roundControl)/2
//													 +*(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=  ((*(pSrcRef+j+1)+*(pSrcRef+j+2+1)+roundControl)/2
//													 +*(pSrcYData+j+1)) & 0xff;
//		}
		xmm5 = _mm_loadu_si128( (__m128i*)pSrcRef );
		xmm2 = xmm5;
		xmm5 = _mm_and_si128 (xmm5, xmm7);	// odd
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even
		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRef+2));
		xmm3 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm5 = _mm_add_epi16(xmm5, xmm1);		// +
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm4 = xmm5;
		xmm5 = _mm_unpacklo_epi16(xmm5, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );	// +Y
		xmm5 = _mm_add_epi16(xmm5, xmm1);		// +

		xmm1 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) ); // +Y
		xmm4 = _mm_add_epi16(xmm4, xmm1);		// +

//		xmm1  = _mm_set1_epi16((short)0xff);		   // 0x00FF
//		xmm4 = _mm_and_si128(xmm4, xmm1);		// & 0xff
//		xmm5 = _mm_and_si128(xmm5, xmm1);		// & 0xff

		xmm5 = _mm_packus_epi16 (xmm5, xmm4);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm5); // store 16 bytes

	}
}

void  static MCUV_HH_8u_C1_C( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					      Fw8u  *pDst,      Fw32s dstStep, 
						  int    width,		 int     height,	 
						  Fw32s roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
//			*(pDst+j)  =(Fw8u)((*(pSrcRef+j)          +*(pSrcRef+j+2)+ 
 //					             *(pSrcRef+j+srcStep)  +*(pSrcRef+j+2+srcStep)+roundControl*2)/4
//													   +*(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=(Fw8u)((*(pSrcRef+j+1)        +*(pSrcRef+j+3)+ 
//						         *(pSrcRef+j+1+srcStep)+*(pSrcRef+j+3+srcStep)+roundControl*2)/4
//													   +*(pSrcYData+j+1)) & 0xff;

			summ = ((*(pSrcRef+j)+*(pSrcRef+j+2)+ *(pSrcRef+j+srcStep) +
					 *(pSrcRef+j+2+srcStep)+roundControl*2)/4 +*(pSrcYData+j)) ;
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

			summ = ((*(pSrcRef+j+1)+*(pSrcRef+j+3)+*(pSrcRef+j+1+srcStep)+*(pSrcRef+j+3+srcStep)+
						roundControl*2)/4 +*(pSrcYData+j+1));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}
}
void  static MCUV_HH_8u_C1_SSE2( const Fw8u  *pSrcRef,   Fw32s srcStep, 
					     const Fw16s *pSrcYData, Fw32s srcYDataStep, 
					           Fw8u  *pDst,      Fw32s dstStep, 
						       int    width,		 int     height,	 
						       Fw32s roundControl)
{
	int i;
	__m128i xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm9;
	
	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	width;

	for(i=0;i<height;i++, pSrcRef+=srcStep, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)	{
//			*(pDst+j)  =  ((*(pSrcRef+j)+			*(pSrcRef+j+1)+ 
//						   *(pSrcRef+j+srcStep)+	*(pSrcRef+j+1+srcStep)+roundControl*2)/4
//												   +*(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=  ((*(pSrcRef+j+1)+			*(pSrcRef+j+3)+ 
//						    *(pSrcRef+j+1+srcStep)+	*(pSrcRef+j+3+srcStep)+roundControl*2)/4
//												   +*(pSrcYData+j+1)) & 0xff;
//		}
		xmm5 = _mm_loadu_si128( (__m128i*)pSrcRef );
		xmm2 = xmm5;
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm5 = _mm_and_si128 (xmm5, xmm7);	// odd
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRef+2));
		xmm3 = xmm1;
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm5 = _mm_add_epi16(xmm5, xmm1);	// + odd
		xmm2 = _mm_add_epi16(xmm2, xmm3);	// + even

		xmm3 = _mm_loadu_si128( (__m128i*)(pSrcRef+srcStep) );
		xmm4 = xmm3;
		xmm4 = _mm_srli_si128(xmm4, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// odd
		xmm4 = _mm_and_si128 (xmm4, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRef+srcStep+2));
		xmm9 = xmm1;
		xmm9 = _mm_srli_si128(xmm9, 1);
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm9 = _mm_and_si128 (xmm9, xmm7);	// even

		xmm5 = _mm_add_epi16(xmm5, xmm3);	// + odd
		xmm5 = _mm_add_epi16(xmm5, xmm1);	// + odd

		xmm5 = _mm_add_epi16(xmm5, xmm6);	// round
		xmm5 = _mm_add_epi16(xmm5, xmm6);	// round
		xmm5 = _mm_srai_epi16(xmm5, 2);		// /4

		xmm2 = _mm_add_epi16(xmm2, xmm4);	// + even
		xmm2 = _mm_add_epi16(xmm2, xmm9);	// + even

		xmm2 = _mm_add_epi16(xmm2, xmm6);	// round
		xmm2 = _mm_add_epi16(xmm2, xmm6);	// round
		xmm2 = _mm_srai_epi16(xmm2, 2);		// /4

		xmm4 = xmm5;
		xmm5 = _mm_unpacklo_epi16(xmm5, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );
		xmm1 = _mm_add_epi16(xmm1, xmm5);	// +

		xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );
		xmm3 = _mm_add_epi16(xmm3, xmm4);	// +

//		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
//		xmm1 = _mm_and_si128(xmm1, xmm4);		// & 0xff
//		xmm3 = _mm_and_si128(xmm3, xmm4);		// & 0xff

		xmm1 = _mm_packus_epi16 (xmm1, xmm3);// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm1 ); // store 16 bytes

	}
}
FwStatus  static iMCUV_8u_C1(const Fw8u  *pSrcRef,   Fw32s srcStep, 
						const Fw16s *pSrcYData, Fw32s srcYDataStep, 
						      Fw8u  *pDst,      Fw32s dstStep, 
							  Fw32s  mcType,    Fw32s roundControl,
						  int    width,		 int     height) 
{
	Fw8u  *ps;
	Fw16s *psY ;
	Fw8u  *pd;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	if (pSrcRef == NULL || pSrcYData == NULL || pDst == NULL ) return fwStsNullPtrErr;

	if(roundControl == 0) roundControl = 1;
	else				  roundControl = 0;

	ps  = (Fw8u *) pSrcRef;
	psY = (Fw16s *) pSrcYData;
	pd  = (Fw8u*) pDst;
	switch(mcType)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type == DT_SSE2)
			    MCUV_FF_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			else
			    MCUV_FF_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type == DT_SSE2)
			    MCUV_FH_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			else
			    MCUV_FH_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type == DT_SSE2)
			    MCUV_HF_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			else
			    MCUV_HF_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type == DT_SSE2)
				MCUV_HH_8u_C1_SSE2( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			else
				MCUV_HH_8u_C1_C( ps, srcStep, psY, srcYDataStep, pd, dstStep, width, height, roundControl);
			break;
		default:
			break;
	}
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x4_8u_C1 )(const Fw8u  *pSrcRef,  Fw32s srcStep, 
							const Fw16s *pSrcYData,Fw32s srcYDataStep, 
								  Fw8u *pDst,      Fw32s dstStep, 
								  Fw32s mcType,	Fw32s roundControl)
{
	if(srcStep < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMC_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,16,4) ;
//	return iMCUV_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
//						      pDst, dstStep, mcType, roundControl,16,4) ;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x8UV_8u_C1 )(const Fw8u *pSrcRef, Fw32s srcStep, 
							  const Fw16s *pSrcYData, Fw32s srcYDataStep, 
							  Fw8u *pDst, Fw32s dstStep, 
							  Fw32s mcType, Fw32s roundControl)
{
	if(srcStep < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcType!=FWVC_MC_APX_FF && mcType!=FWVC_MC_APX_FH &&
		mcType!=FWVC_MC_APX_HF && mcType!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCUV_8u_C1 (pSrcRef, srcStep, pSrcYData, srcYDataStep, 
						      pDst, dstStep, mcType, roundControl,16,8) ;
}


/*
Arguments
pSrcRefF: Pointer to the forward reference block.
srcStepF: Size of the row in bytes, specifying the aligned forward reference frame width.
mcTypeF: Forward MC type FWVC_MC_APX.
pSrcRefB: Pointer to the backward reference block.
srcStepB: Size of the ow in bytes, specifying the aligned backward reference frame width.
mcTypeB: Backward MC type FWVC_MC_APX.
pSrcYData: Pointer to the data obtained after inverse DCT.
srcYDataStep: Number of bytes, specifying the width of the aligned data obtained after iDCT.
pDst: Pointer to the destination predicted block.
dstStep: Size of the row in bytes, specifying the aligned destination frame width.
roundControl: Parameter that determines type of rounding for half a pel approximation; 0 or 1
*/
#if 0
void PredF_FF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
void PredF_FF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
#else
void  static MC_PredF_FF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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

void  static MC_PredF_FF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
		MC_PredF_FF_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}

#endif

#if 0
void PredF_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
void PredF_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
#else
void  static MC_PredF_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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

void  static MC_PredF_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
		MC_PredF_FH_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}
#endif

#if 0
void static PredF_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
								int height,		  int width,
								Fw32s roundControl);
void PredF_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
								int height,		  int width,
								Fw32s roundControl);
#else
void  static MC_PredF_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
void  static MC_PredF_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
		MC_PredF_HF_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);

		break;
	}
}
#endif

#if 0
void PredF_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
void PredF_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl);
#else
void  static MC_PredF_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
void  static MC_PredF_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
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
		MC_PredF_HH_8u16s_C1_C( pSrcCur,  srcCurStep,
 								pSrcRef,  srcRefStep,
								width,	height, roundControl);
		break;
	}
}
#endif

void  static MCB_FF_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j;
	int summ;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//			*(pDst+j) =  (Fw8u)((*(pSrcRefF+j)+*(pSrcRefB+j)+roundControl)/2 + *(pSrcYData+j)) & 0xff;
			summ = ((*(pSrcRefF+j)+*(pSrcRefB+j)+roundControl)/2 + *(pSrcYData+j)) ;
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

		}
	}
}

void  static MCB_FF_8u_C1_SSE2(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					   const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					   const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
					   	     Fw8u  *pDst,		  Fw32s dstStep, 
						     int    width,		 int     height,	 
  						     Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm4, xmm6, xmm7, xmm8;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF

//	printf("\n SSE2: %d-%d", width, height);
//Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));

	switch (width)
	{
		case 16:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			srcYDataStep/=2;
			for(i=0;i<height/2;i++)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#else
//				xmm1    = xmm0;
//				xmm3    = xmm2;
//				xmm1    = _mm_cmpgt_epi16(xmm1, xmm7);		// < 0
//				xmm3    = _mm_cmpgt_epi16(xmm3, xmm7);	
//				xmm0	= _mm_and_si128(xmm0, xmm1);		// remove negative number
//				xmm2	= _mm_and_si128(xmm2, xmm3);		// remove negative number
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				pSrcRefF+=srcStepF;
				pSrcRefB+=srcStepB;
				pSrcYData+=srcYDataStep;

				xmm8 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm8 = _mm_add_epi16(xmm8, xmm5);		// +
				xmm8 = _mm_add_epi16(xmm8, xmm6);		// round
				xmm8 = _mm_srai_epi16(xmm8, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm8 = _mm_add_epi16(xmm8, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm8 = _mm_packus_epi16 (xmm8, xmm2);			// 16bit to 8bit

				pSrcRefF+=srcStepF;
				pSrcRefB+=srcStepB;
				pSrcYData+=srcYDataStep;

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes				
				pDst+=dstStep;
				_mm_storeu_si128( (__m128i*)pDst, xmm8 ); // store 16 bytes				
				pDst+=dstStep;
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
		}
			break;
		case 8:
		if(((size_t)pSrcYData & 0xf) == 0)
		{
			srcYDataStep/=2;
			for(i=0;i<height/2;i++)
//			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				pSrcRefF+=srcStepF;
				pSrcRefB+=srcStepB;
				pSrcYData+=srcYDataStep;

				xmm8 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm8 = _mm_unpacklo_epi8(xmm8, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm8 = _mm_add_epi16(xmm8, xmm5);		// +
				xmm8 = _mm_add_epi16(xmm8, xmm6);		// round
				xmm8 = _mm_srai_epi16(xmm8, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm8 = _mm_add_epi16(xmm8, xmm1);		// +

#if 0
				xmm8 = _mm_and_si128(xmm8, xmm4);		// & 0xff
#endif
				xmm8 = _mm_packus_epi16 (xmm8, xmm0);			// 16bit to 8bit

				pSrcRefF+=srcStepF;
				pSrcRefB+=srcStepB;
				pSrcYData+=srcYDataStep;

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				pDst+=dstStep;
				_mm_storel_epi64( (__m128i*)pDst, xmm8 ); // store 8 bytes
				pDst+=dstStep;
			}
		}
		else
		{
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
		}
			break;
		case 4:
			int *a;
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				a    = (int *)pSrcRefF;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB);
				xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif

				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
			}
			break;
		default:
			MCB_FF_8u_C1_C(pSrcRefF,	 srcStepF, 
							pSrcRefB,	 srcStepB, 
							pSrcYData,   srcYDataStep, 
							pDst,		 dstStep, 
							width,		 height,	 
  							roundControl);
			break;
	}
}
void  static MCB_FH_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j;
	int summ;

	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//					*(pDst+j) = (Fw8u)( (*(pSrcRefF+j)+
//							(*(pSrcRefB+j)+*(pSrcRefB+j+srcStepB)+roundControl)/2
//							 +roundControl)/2 + *(pSrcYData+j) ) & 0xff;
			summ = ( (*(pSrcRefF+j)+
						(*(pSrcRefB+j)+*(pSrcRefB+j+srcStepB)+roundControl)/2
							 +roundControl)/2 + *(pSrcYData+j) );
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;
		}
	}

}
void  static MCB_FH_8u_C1_SSE2(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm5, xmm4, xmm6, xmm7;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

	switch (width)
	{
		case 16:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
			break;
		case 8:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
			break;
		case 4:
			int *a;
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				a    = (int *)pSrcRefF;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB);
				xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB+srcStepB);
				xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
			}
			break;
		default:
			MCB_FH_8u_C1_C(pSrcRefF,	srcStepF, 
							pSrcRefB,	srcStepB, 
							pSrcYData,  srcYDataStep, 
							pDst,		 dstStep, 
							width,		 height, roundControl);

			break;
	}
}
void  static MCB_HF_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j;
	int summ;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//					*(pDst+j) =  (Fw8u)( (*(pSrcRefF+j)+
//							(*(pSrcRefB+j)+*(pSrcRefB+j+1)+roundControl)/2
//							 +roundControl)/2 +*(pSrcYData+j)) & 0xff;
			summ = ( (*(pSrcRefF+j)+
							(*(pSrcRefB+j)+*(pSrcRefB+j+1)+roundControl)/2
							 +roundControl)/2 +*(pSrcYData+j));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

		}
	}
}
void  static MCB_HF_8u_C1_SSE2(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;

//	printf("\n SSE2: %d-%d", width, height);
//Fw16u* tempa = (Fw16u*) fwMalloc(8*sizeof(Fw16u));

	switch (width)
	{
		case 16:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit
/*
{
_mm_storeu_si128( (__m128i*)tempa, xmm0 ); // store 16 bytes
printf("\n< %d %d %d %d %d %d %d %d",tempa[0], tempa[1],tempa[2],tempa[3],
   tempa[4],tempa[5],tempa[6],tempa[7]);
}
{
_mm_storeu_si128( (__m128i*)tempa, xmm2 ); // store 16 bytes
printf(" %d %d %d %d %d %d %d %d",tempa[0], tempa[1],tempa[2],tempa[3],
   tempa[4],tempa[5],tempa[6],tempa[7]);
}
getchar();
*/
				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
			break;
		case 8:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
			break;
		case 4:
			int *a;
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				a    = (int *)pSrcRefF;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB);
				xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB+1);
				xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
				xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadl_epi64 ( (__m128i*)pSrcYData );	// get 4 short

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src
			}
			break;
		default:
			MCB_HF_8u_C1_C(pSrcRefF,	srcStepF, 
							pSrcRefB,	srcStepB, 
							pSrcYData,  srcYDataStep, 
							pDst,		 dstStep, 
							width,		 height, roundControl);
			break;
	}
}
void  static MCB_HH_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j;
	int summ;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j++)
		{
//			*(pDst+j) =  (Fw8u)( (*(pSrcRefF+j)+
//									(*(pSrcRefB+j)+*(pSrcRefB+j+1)+ 
//									 *(pSrcRefB+j+srcStepB)+*(pSrcRefB+j+1+srcStepB)+roundControl*2)/4
//						 +roundControl)/2 +*(pSrcYData+j)) & 0xff;

			summ = ( (*(pSrcRefF+j)+
									(*(pSrcRefB+j)+*(pSrcRefB+j+1)+ 
									 *(pSrcRefB+j+srcStepB)+*(pSrcRefB+j+1+srcStepB)+roundControl*2)/4
						 +roundControl)/2 +*(pSrcYData+j)) ;
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

		}
	}
}
void  static MCB_HH_8u_C1_SSE2(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	xmm8  = _mm_slli_epi16 (xmm6, 1);

	switch (width)
	{
		case 16:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcStepB) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm1);		// + 
				xmm5 = _mm_add_epi16(xmm5, xmm2);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm8);		// round
				xmm5 = _mm_srai_epi16(xmm5, 2);			// /4

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+8) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB+8));
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

				xmm3= _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcStepB+8));
				xmm3= _mm_unpacklo_epi8(xmm3, xmm7);	// pr u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm1);		// + 
				xmm5 = _mm_add_epi16(xmm5, xmm3);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm8);		// round
				xmm5 = _mm_srai_epi16(xmm5, 2);			// /4

				xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
				xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
				xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

				xmm3 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) );

				xmm2 = _mm_add_epi16(xmm2, xmm3);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
				xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

				_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
			}
			break;
		case 8:
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1) );
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				xmm1 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB) );
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

				xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+1+srcStepB) );
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm1);		// + 
				xmm5 = _mm_add_epi16(xmm5, xmm2);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm8);		// round
				xmm5 = _mm_srai_epi16(xmm5, 2);			// /4

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
			}
			break;
		case 4:
			int *a;
			for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
			{
				a    = (int *)pSrcRefF;
				xmm0 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

				a    = (int *)pSrcRefB;
				xmm5 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB+1);
				xmm4 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

				a    = (int *)(pSrcRefB+srcStepB);
				xmm1 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm1 = _mm_unpacklo_epi8(xmm1, xmm7);	// ps u8->u16

				a    = (int *)(pSrcRefB+1+srcStepB);
				xmm2 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src
				xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// pr u8->u16

				xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm1);		// + 
				xmm5 = _mm_add_epi16(xmm5, xmm2);		// +
				xmm5 = _mm_add_epi16(xmm5, xmm8);		// round
				xmm5 = _mm_srai_epi16(xmm5, 2);			// /4

				xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
				xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
				xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

				xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );

				xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

#if 0
				xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
				xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
#endif
				xmm0 = _mm_packus_epi16 (xmm0, xmm0);			// 16bit to 8bit

//				_mm_storel_epi64( (__m128i*)pDst, xmm0 ); // store 8 bytes
				a    = (int *)pDst;
				*a   = _mm_cvtsi128_si32 (xmm0);			// get 4 bytes from src

			}
			break;
		default:
			MCB_HH_8u_C1_C(pSrcRefF,	srcStepF, 
							pSrcRefB,	srcStepB, 
							pSrcYData,  srcYDataStep, 
							pDst,		 dstStep, 
							width,		 height, roundControl);

			break;
	}
}

FwStatus  static iMCB_8u_C1(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
						 const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
						 const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
							   Fw8u  *pDst,		Fw32s dstStep, 
							   Fw32s  roundControl,
						  int    width,		 int     height) 
{
	Fw8u  *ps,  *pr, *pm, *pd;
	Fw16s *psY ;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	Fw8u  SrcRefM[16*16+16];
	Fw32s srcStepM;

	if (pSrcRefF == NULL || pSrcRefB == NULL || pSrcYData == NULL || pDst == NULL ) 
		return fwStsNullPtrErr;

	if(roundControl == 0) roundControl = 1;
	else				  roundControl = 0;

//	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(height*width*sizeof(Fw8u));
	Fw8u* pSrcRefM = &SrcRefM[0];

//	if(((size_t)pSrcRefM % 0xf) != 0) pSrcRefM += (0xf - (size_t)pSrcRefM % 0xf); 
	if(((size_t)pSrcRefM & 0xf) != 0) pSrcRefM += (16 - ((size_t)pSrcRefM & 0xf)); 

	ps = (Fw8u *) pSrcRefF;
	pm = (Fw8u *) pSrcRefM;
	switch(mcTypeF)
	{
		case FWVC_MC_APX_FF:
//			if( Dispatch_Type != DT_SSE2)
//			    MC_PredF_FF_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
//			else
//			    MC_PredF_FF_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type != DT_SSE2)
			    MC_PredF_FH_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    MC_PredF_FH_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type != DT_SSE2)
			    MC_PredF_HF_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    MC_PredF_HF_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type != DT_SSE2)
			    MC_PredF_HH_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    MC_PredF_HH_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		default:
			break;
	}

	psY= (Fw16s *) pSrcYData;
	pr = (Fw8u  *) pSrcRefB;
	pd = (Fw8u  *) pDst;

	if(mcTypeF == FWVC_MC_APX_FF)
	{
		pm = (Fw8u *) pSrcRefF;
		srcStepM = srcStepF;
	}
	else
	{
		pm = (Fw8u  *) pSrcRefM;
		srcStepM = width;
	}

//	if(roundControl == 0) roundControl = 1;
//	else				  roundControl = 0;

	switch(mcTypeB)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type != DT_SSE2)
			    MCB_FF_8u_C1_C( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCB_FF_8u_C1_SSE2( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type != DT_SSE2)
			    MCB_FH_8u_C1_C( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCB_FH_8u_C1_SSE2( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type != DT_SSE2)
			    MCB_HF_8u_C1_C( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCB_HF_8u_C1_SSE2( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type != DT_SSE2)
			    MCB_HH_8u_C1_C( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCB_HH_8u_C1_SSE2( pm,	srcStepM, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		default:
			break;
	}

//	fwFree(pSrcRefM);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x16B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							  const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							  const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								    Fw8u  *pDst,		Fw32s dstStep, 
									Fw32s  roundControl)
{
	if(srcStepF < 16 || srcStepB < 16 || srcYDataStep < 32 || dstStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
						   pSrcRefB, srcStepB, mcTypeB, 
						   pSrcYData, srcYDataStep, 
						   pDst, dstStep, roundControl, 16,	16);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x8B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							 const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							 const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 8 || srcStepB < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 16,	8);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x16B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							 const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							 const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 16 || srcStepB < 16 || srcYDataStep < 32 || dstStep < 16) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 8,	16);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x8B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 8 || srcStepB < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 8,	8);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC8x4B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 8,	4);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x8B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 8 || srcStepB < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 4,	8);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x4B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 4 || srcStepB < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 4,	4);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC2x4B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 4 || srcStepB < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 2,	4);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC4x2B_8u_C1 )(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 2 || srcStepB < 2 || srcYDataStep < 4 || dstStep < 2) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 4,	2);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC2x2B_8u_C1)(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
						   const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
						   const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 2 || srcStepB < 2 || srcYDataStep < 4 || dstStep < 2) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 2,	2);
}

//////////////////////////////////////////////////////////////////////////////////////
void  static PredFUV_FF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								Fw8u* pSrcRef,  Fw32s srcRefStep,
						  int    width,		 int     height,	 
								Fw32s roundControl)
{
	int i, j;
	roundControl;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j+=2)
		{
			*(pSrcRef+j) =  *(pSrcCur+j);
			*(pSrcRef+j+1) =  *(pSrcCur+j+1);
		}
	}
}
void  static PredFUV_FF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								     Fw8u* pSrcRef,  Fw32s srcRefStep,
						             int    width,	   int    height,	 
								     Fw32s roundControl)
{
	int i;
	__m128i xmm0;
	roundControl;
	width;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
//		for(j=0;j<width;j+=2) {	*(pSrcRef+j)=*(pSrcCur+j); *(pSrcRef+j+1)=*(pSrcCur+j+1);}
		xmm0 = _mm_loadu_si128 ( (__m128i*)(pSrcCur) );
		_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 16 bytes
	}
}
void  static PredFUV_FH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								  Fw8u* pSrcRef,  Fw32s srcRefStep,
	 					          int    width,	   int     height,	 
								  Fw32s roundControl)
{
	int i, j;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j+=2)
		{
			*(pSrcRef+j)  = (Fw8u)((*(pSrcCur+j)  +*(pSrcCur+j+srcCurStep)  +roundControl)/2);
			*(pSrcRef+j+1)= (Fw8u)((*(pSrcCur+j+1)+*(pSrcCur+j+srcCurStep+1)+roundControl)/2);
		}
	}
}
void  static PredFUV_FH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								     Fw8u* pSrcRef,  Fw32s srcRefStep,
						             int    width,	   int    height,	 
								     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm2, xmm5, xmm6, xmm7;
	
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	roundControl;
	width;
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
//		for(j=0;j<width;j+=2) {
//			*(pSrcRef+j)  = (*(pSrcCur+j)  +*(pSrcCur+j+srcCurStep)  +roundControl)/2;
//			*(pSrcRef+j+1)= (*(pSrcCur+j+1)+*(pSrcCur+j+srcCurStep+1)+roundControl)/2;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcCur );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcCur+8) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcCur+srcCurStep+8) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2
#if 0
		xmm4  = _mm_set1_epi16((short)0xff);	 // 0x00FF
		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
		xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 16 bytes
	}
		}
void  static PredFUV_HF_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 							  	  Fw8u* pSrcRef,  Fw32s srcRefStep,
						          int    width,	   int    height,	 
								  Fw32s roundControl)
{
	int i, j;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j+=2)
		{
			*(pSrcRef+j)  = (Fw8u)((*(pSrcCur+j)  +*(pSrcCur+j+2)  +roundControl)/2);
			*(pSrcRef+j+1)= (Fw8u)((*(pSrcCur+j+1)+*(pSrcCur+j+2+1)+roundControl)/2);
		}
	}
}
void  static PredFUV_HF_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								     Fw8u* pSrcRef,  Fw32s srcRefStep,
						             int    width,	   int    height,	 
								     Fw32s roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm6, xmm7;
	roundControl;
	width;	
	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
//		for(j=0;j<width;j+=2) {
//			*(pSrcRef+j)  = (*(pSrcRef+j)  +*(pSrcRef+j+2)  +roundControl)/2;
//			*(pSrcRef+j+1)= (*(pSrcRef+j+1)+*(pSrcRef+j+2+1)+roundControl)/2;
//		}

		xmm0 = _mm_loadu_si128( (__m128i*)pSrcCur );
		xmm2 = xmm0;
		xmm0 = _mm_and_si128 (xmm0, xmm7);	// odd
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcCur+2));
		xmm3 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm4 = xmm0;
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even
#if 0
		xmm0 = _mm_and_si128(xmm0, xmm7);		// & 0xff
		xmm4 = _mm_and_si128(xmm4, xmm7);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm4);// 16bit to 8bit

//		xmm2 = _mm_slli_si128(xmm2, 1);
//		xmm0 = _mm_and_si128 (xmm0, xmm2);		// merge odd and even

		_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 16 bytes
	}
}
void  static PredFUV_HH_8u16s_C1_C( const Fw8u* pSrcCur,  Fw32s srcCurStep,
  								  Fw8u* pSrcRef,  Fw32s srcRefStep,
						          int    width,	   int    height,	 
								  Fw32s roundControl)
{
	int i, j, sum;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
		for(j=0;j<width;j+=2)
		{
			sum = (int)*(pSrcCur+j)  + 
				  (int)*(pSrcCur+j+2)+ 
				  (int)*(pSrcCur+j+srcCurStep)+ 
				  (int)*(pSrcCur+j+2+srcCurStep);
			*(pSrcRef+j)  = (Fw8u)((sum + roundControl*2)/4);

			sum = (int)*(pSrcCur+j+1)+ 
				  (int)*(pSrcCur+j+3)+ 
				  (int)*(pSrcCur+j+1+srcCurStep)+ 
				  (int)*(pSrcCur+j+3+srcCurStep);
			*(pSrcRef+j+1)= (Fw8u)((sum + roundControl*2)/4);
		}
	}
}
void  static PredFUV_HH_8u16s_C1_SSE2( const Fw8u* pSrcCur,  Fw32s srcCurStep,
 								     Fw8u* pSrcRef,  Fw32s srcRefStep,
						             int    width,	   int    height,	 
								     Fw32s roundControl)
{
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9;
	roundControl;
	width;
	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	int i;

	for(i=0;i<height;i++, pSrcCur+=srcCurStep, pSrcRef+=srcRefStep)
	{
//		for(j=0;j<width;j+=2) {
//			*(pSrcRef+j)  =  (*(pSrcCur+j)+   *(pSrcCur+j+1)+ 
//						      *(pSrcCur+j+srcCurStep)+  *(pSrcCur+j+1+srcCurStep)+roundControl*2)/4;
//			*(pSrcRef+j+1)=  (*(pSrcCur+j+1)+ *(pSrcCur+j+2+1)+ 
//						      *(pSrcCur+j+1+srcCurStep)+*(pSrcCur+j+2+1+srcCurStep)+roundControl*2)/4;
//		}
		xmm0 = _mm_loadu_si128( (__m128i*)pSrcCur );
		xmm2 = xmm0;
		xmm0 = _mm_and_si128 (xmm0, xmm7);	// odd
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcCur+2));
		xmm3 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm4 = _mm_loadu_si128( (__m128i*)(pSrcCur+srcCurStep) );
		xmm8 = xmm4;
		xmm4 = _mm_and_si128 (xmm4, xmm7);	// odd
		xmm8 = _mm_srli_si128(xmm8, 1);
		xmm8 = _mm_and_si128 (xmm8, xmm7);	// even

		xmm5 = _mm_loadu_si128( (__m128i*)(pSrcCur+srcCurStep+2));
		xmm9 = xmm5;
		xmm5 = _mm_and_si128 (xmm5, xmm7);	// odd
		xmm9 = _mm_srli_si128(xmm9, 1);
		xmm9 = _mm_and_si128 (xmm9, xmm7);	// even

		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm4);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 2);			// /4

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm8);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm9);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

//		xmm2 = _mm_slli_si128(xmm2, 1);
//		xmm0 = _mm_and_si128 (xmm0, xmm2);		// merge odd and even

		xmm4 = xmm0;
		xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even

#if 0
		xmm0 = _mm_and_si128(xmm0, xmm7);		// & 0xff
		xmm4 = _mm_and_si128(xmm4, xmm7);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm4);// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pSrcRef, xmm0 ); // store 16 bytes

	}

}


void  static MCBUV_FF_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j, summ;
	roundControl;
	width;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
			summ = (((int)*(pSrcRefF+j)+  
					 (int)*(pSrcRefB+j)+roundControl)/2 +   (int)*(pSrcYData+j)) ;
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;


			summ = (((int)*(pSrcRefF+j+1)+
					 (int)*(pSrcRefB+j+1)+roundControl)/2 + (int)*(pSrcYData+j+1));
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}

	}
}
void  static MCBUV_FF_8u_C1_SSE2(const Fw8u  *pSrcRefF,	Fw32s srcStepF, 
					     const Fw8u  *pSrcRefB,	Fw32s srcStepB, 
					     const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
					 	       Fw8u  *pDst,		Fw32s dstStep, 
						       int    width,		int     height,	 
  						       Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm5, xmm6, xmm7;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	width;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)
//		{
//			*(pDst+j)  =  ((*(pSrcRefF+j)+  *(pSrcRefB+j)+roundControl)/2 +   *(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=  ((*(pSrcRefF+j+1)+*(pSrcRefB+j+1)+roundControl)/2 + *(pSrcYData+j+1)) & 0xff;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );	// +Y
		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) ); // +Y
		xmm2 = _mm_add_epi16(xmm2, xmm1);		// +
#if 0
		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
		xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
	}
}
void  static MCBUV_FH_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j, summ;

	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
			summ = ( ((int)*(pSrcRefF+j)+
						((int)*(pSrcRefB+j)+(int)*(pSrcRefB+j+srcStepB)+roundControl)/2
							 +roundControl)/2 + (int)*(pSrcYData+j) );
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

			summ = ( ((int)*(pSrcRefF+j+1)+
							((int)*(pSrcRefB+j+1)+(int)*(pSrcRefB+j+1+srcStepB)+roundControl)/2
							 +roundControl)/2 + (int)*(pSrcYData+j+1) );
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}}
void  static MCBUV_FH_8u_C1_SSE2(const Fw8u  *pSrcRefF,	Fw32s srcStepF, 
					     const Fw8u  *pSrcRefB,	Fw32s srcStepB, 
					     const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
					 	       Fw8u  *pDst,		Fw32s dstStep, 
						       int    width,		int     height,	 
  						       Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm4, xmm5, xmm6, xmm7;

	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm6  = _mm_set1_epi16((short)roundControl);    // round;
	width;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)
//		{
//			*(pDst+j)  = (  (*(pSrcRefF+j)+
//							(*(pSrcRefB+j)+     *(pSrcRefB+j+srcStepB)+roundControl)/2
//							 +roundControl)/2 + *(pSrcYData+j) ) & 0xff;
//			*(pDst+j+1)= (  (*(pSrcRefF+j+1)+
//							(*(pSrcRefB+j+1)+   *(pSrcRefB+j+1+srcStepB)+roundControl)/2
//							 +roundControl)/2 + *(pSrcYData+j+1) ) & 0xff;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

		xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );	// +Y
		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm2 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
		xmm2 = _mm_unpacklo_epi8(xmm2, xmm7);	// u8->u16

		xmm5 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+8) );
		xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// u8->u16

		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefB+srcStepB+8) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// u8->u16

		xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm2 = _mm_add_epi16(xmm2, xmm5);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) ); // +Y
		xmm2 = _mm_add_epi16(xmm2, xmm1);		// +
#if 0
		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
		xmm2 = _mm_and_si128(xmm2, xmm4);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm2);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
	}
}
void  static MCBUV_HF_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
						  Fw8u  *pDst,		  Fw32s dstStep, 
						  int    width,		 int     height,	 
  						  Fw32s  roundControl)
{
	int i, j, summ;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{
			summ =  ((int)*(pSrcRefF+j)+
			       ((int)*(pSrcRefB+j)+(int)*(pSrcRefB+j+2)+roundControl)/2+
				    roundControl)/2;			             
			summ += (int)*(pSrcYData+j);
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;


			summ =  ((int)*(pSrcRefF+j+1)+
				   ((int)*(pSrcRefB+j+1)+*(pSrcRefB+j+1+2)+roundControl)/2+
					roundControl)/2;
			summ += (int)*(pSrcYData+j+1);
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}
}
void  static MCBUV_HF_8u_C1_SSE2(const Fw8u  *pSrcRefF,	Fw32s srcStepF, 
					     const Fw8u  *pSrcRefB,	Fw32s srcStepB, 
					     const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
					 	       Fw8u  *pDst,		Fw32s dstStep, 
						       int    width,		int     height,	 
  						       Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;
	
	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	xmm8  = _mm_set1_epi8(0);    // zero;
	width;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
//		for(j=0;j<width;j+=2)
//		{
//			*(pDst+j)  =  ( (*(pSrcRefF+j)+
//							(*(pSrcRefB+j)+*(pSrcRefB+j+2)+roundControl)/2
//							 +roundControl)/2 +*(pSrcYData+j)) & 0xff;
//			*(pDst+j+1)=  ( (*(pSrcRefF+j+1)+
//							(*(pSrcRefB+j+1)+*(pSrcRefB+j+1+2)+roundControl)/2
//							 +roundControl)/2 +*(pSrcYData+j+1)) & 0xff;
//		}
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm8);	// u8->u16

		xmm5 = _mm_loadu_si128( (__m128i*)pSrcRefB );
		xmm2 = xmm5;
		xmm5 = _mm_and_si128 (xmm5, xmm7);	// odd
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even
		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRefB+2));
		xmm3 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm5 = _mm_add_epi16(xmm5, xmm1);		// +
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// +
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 1);			// /2

		xmm4 = xmm5;
		xmm5 = _mm_unpacklo_epi16(xmm5, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even

		xmm0 = _mm_add_epi16(xmm0, xmm5);		// +
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );	// +Y
		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm5 = xmm4;

		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm8);	// u8->u16

		xmm5 = _mm_add_epi16(xmm5, xmm4);		// +
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) ); // +Y
		xmm5 = _mm_add_epi16(xmm5, xmm1);		// +
#if 0
		xmm4  = _mm_set1_epi16((short)0xff);		   // 0x00FF
		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
		xmm5 = _mm_and_si128(xmm5, xmm4);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm5);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
	}
}
void  static MCBUV_HH_8u_C1_C(const Fw8u  *pSrcRefF,	  Fw32s srcStepF, 
					  const Fw8u  *pSrcRefB,	  Fw32s srcStepB, 
					  const Fw16s *pSrcYData,    Fw32s srcYDataStep, 
						    Fw8u  *pDst,		  Fw32s dstStep, 
						    int    width,		  int     height,	 
  						    Fw32s  roundControl)
{
	int i, j, summ;

	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
		for(j=0;j<width;j+=2)
		{

			summ  =  ((int)*(pSrcRefB+j)         +(int)*(pSrcRefB+j+2)+ 
					 (int)*(pSrcRefB+j+srcStepB)+(int)*(pSrcRefB+j+2+srcStepB)+roundControl*2)/4;
			summ  =  ( (int)*(pSrcRefF+j) + summ + roundControl) /2 ;
			summ += (int)*(pSrcYData+j);
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j) =  (Fw8u)summ;

			summ =   ((int)*(pSrcRefB+j+1)		  +(int)*(pSrcRefB+j+3)+ 
					 (int)*(pSrcRefB+j+1+srcStepB)+(int)*(pSrcRefB+j+3+srcStepB)+roundControl*2)/4;
			summ =   ((int)*(pSrcRefF+j+1)+summ+roundControl)/2;
			summ += (int)*(pSrcYData+j+1);
			if(summ < 0)		summ =  0;
			else if(summ > 255) summ = 255;
			*(pDst+j+1) =  (Fw8u)summ;
		}
	}
}
void  static MCBUV_HH_8u_C1_SSE2(const Fw8u  *pSrcRefF,	Fw32s srcStepF, 
					     const Fw8u  *pSrcRefB,	Fw32s srcStepB, 
					     const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
					 	       Fw8u  *pDst,		Fw32s dstStep, 
						       int    width,		int     height,	 
  						       Fw32s  roundControl)
{
	int i;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9;
//printf("\n MCBUV_HF_8u_C1_SSE2: %d %d",width, height);
//Fw16s* tempa = (Fw16s*) fwMalloc(8*sizeof(Fw16s));

	xmm6  = _mm_set1_epi16((short)roundControl);   // round;
	xmm7  = _mm_set1_epi16((short)0xff);		   // 0x00FF
	xmm8  = _mm_set1_epi8(0);    // zero;
	width;
	for(i=0;i<height;i++, pSrcRefF+=srcStepF, pSrcRefB+=srcStepB, pSrcYData+=srcYDataStep/2, pDst+=dstStep)
	{
/*
		for(j=0;j<width;j+=2)
		{
			*(pDst+j)  =  ( (*(pSrcRefF+j)+
							(*(pSrcRefB+j)+*(pSrcRefB+j+2)+ 
						     *(pSrcRefB+j+srcStepB)+*(pSrcRefB+j+2+srcStepB)+roundControl*2)/4
							 +roundControl)/2 +*(pSrcYData+j)) & 0xff;
			*(pDst+j+1)=  ( (*(pSrcRefF+j+1)+
							(*(pSrcRefB+j+1)+*(pSrcRefB+j+3)+ 
						     *(pSrcRefB+j+1+srcStepB)+*(pSrcRefB+j+3+srcStepB)+roundControl*2)/4
							 +roundControl)/2 +*(pSrcYData+j+1)) & 0xff;
		}
*/
		xmm0 = _mm_loadl_epi64( (__m128i*)pSrcRefF );
		xmm0 = _mm_unpacklo_epi8(xmm0, xmm8);	// u8->u16

		xmm5 = _mm_loadu_si128( (__m128i*)pSrcRefB );
		xmm2 = xmm5;
		xmm5 = _mm_and_si128 (xmm5, xmm7);	// odd
		xmm2 = _mm_srli_si128(xmm2, 1);
		xmm2 = _mm_and_si128 (xmm2, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRefB+2));
		xmm3 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm3 = _mm_srli_si128(xmm3, 1);
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// even

		xmm5 = _mm_add_epi16(xmm5, xmm1);		// + odd
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round

		xmm2 = _mm_add_epi16(xmm2, xmm3);		// + even
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round

		xmm3 = _mm_loadu_si128( (__m128i*)(pSrcRefB+srcStepB) );
		xmm4 = xmm3;
		xmm3 = _mm_and_si128 (xmm3, xmm7);	// odd
		xmm4 = _mm_srli_si128(xmm4, 1);
		xmm4 = _mm_and_si128 (xmm4, xmm7);	// even

		xmm1 = _mm_loadu_si128( (__m128i*)(pSrcRefB+srcStepB+2));
		xmm9 = xmm1;
		xmm1 = _mm_and_si128 (xmm1, xmm7);	// odd
		xmm9 = _mm_srli_si128(xmm9, 1);
		xmm9 = _mm_and_si128 (xmm9, xmm7);	// even

		xmm1 = _mm_add_epi16(xmm1, xmm3);		// + odd
		xmm5 = _mm_add_epi16(xmm5, xmm1);		// + odd
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 2);			// /4

		xmm9 = _mm_add_epi16(xmm9, xmm4);		// + even
		xmm2 = _mm_add_epi16(xmm2, xmm9);		// + even
		xmm2 = _mm_add_epi16(xmm2, xmm6);		// round
		xmm2 = _mm_srai_epi16(xmm2, 2);			// /4

		xmm4 = xmm5;
		xmm5 = _mm_unpacklo_epi16(xmm5, xmm2);	// interleave odd & even
		xmm4 = _mm_unpackhi_epi16(xmm4, xmm2);	// interleave odd & even
/*
{
_mm_storeu_si128( (__m128i*)tempa, xmm5 ); // store 16 bytes
printf("\n %d %d %d %d %d %d %d %d",tempa[0], tempa[1],tempa[2],tempa[3],
	   tempa[4],tempa[5],tempa[6],tempa[7]);
_mm_storeu_si128( (__m128i*)tempa, xmm4 ); // store 16 bytes
printf(" %d %d %d %d %d %d %d %d",tempa[0], tempa[1],tempa[2],tempa[3],
	   tempa[4],tempa[5],tempa[6],tempa[7]);
}
*/
		xmm0 = _mm_add_epi16(xmm0, xmm5);		// + pSrcRefF
		xmm0 = _mm_add_epi16(xmm0, xmm6);		// round
		xmm0 = _mm_srai_epi16(xmm0, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)pSrcYData );	// +pSrcYData
		xmm0 = _mm_add_epi16(xmm0, xmm1);		// +

		xmm5 = xmm4;

		xmm4 = _mm_loadl_epi64( (__m128i*)(pSrcRefF+8) );
		xmm4 = _mm_unpacklo_epi8(xmm4, xmm8);	// u8->u16

		xmm5 = _mm_add_epi16(xmm5, xmm4);		// + pSrcRefF+8
		xmm5 = _mm_add_epi16(xmm5, xmm6);		// round
		xmm5 = _mm_srai_epi16(xmm5, 1);			// /2

		xmm1 = _mm_loadu_si128 ( (__m128i*)(pSrcYData+8) ); // +pSrcYData+8
		xmm5 = _mm_add_epi16(xmm5, xmm1);		// +
#if 0
		xmm4  = _mm_set1_epi16((short)0xff);	// 0x00FF
		xmm0 = _mm_and_si128(xmm0, xmm4);		// & 0xff
		xmm5 = _mm_and_si128(xmm5, xmm4);		// & 0xff
#endif
		xmm0 = _mm_packus_epi16 (xmm0, xmm5);			// 16bit to 8bit

		_mm_storeu_si128( (__m128i*)pDst, xmm0 ); // store 16 bytes
	}
}

FwStatus  static iMCBUV_8u_C1(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
						 const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
						 const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
							   Fw8u  *pDst,		Fw32s dstStep, 
							   Fw32s  roundControl,
						  int    width,		 int     height) 
{
	Fw8u  *ps,  *pr, *pm, *pd;
	Fw16s *psY ;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();
	Fw8u SrcRefM[16*16+16];

	if (pSrcRefF == NULL || pSrcRefB == NULL || pSrcYData == NULL || pDst == NULL ) 
		return fwStsNullPtrErr;

	if(roundControl == 0) roundControl = 1;
	else				  roundControl = 0;

//	Fw8u* pSrcRefM = (Fw8u*) fwMalloc(height*width*sizeof(Fw8u));
	Fw8u* pSrcRefM = &SrcRefM[0];
//	if(((size_t)pSrcRefM % 0xf) != 0) pSrcRefM += (0xf - (size_t)pSrcRefM % 0xf); 
	if(((size_t)pSrcRefM & 0xf) != 0) pSrcRefM += (16 - ((size_t)pSrcRefM & 0xf)); 

	ps = (Fw8u *) pSrcRefF;
	pm = (Fw8u *) pSrcRefM;
	switch(mcTypeF)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type != DT_SSE2)
			    PredFUV_FF_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    PredFUV_FF_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type != DT_SSE2)
			    PredFUV_FH_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    PredFUV_FH_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type != DT_SSE2)
				PredFUV_HF_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
				PredFUV_HF_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type != DT_SSE2)
			    PredFUV_HH_8u16s_C1_C( ps, srcStepF, pm, width, width, height, roundControl);
			else
			    PredFUV_HH_8u16s_C1_SSE2( ps, srcStepF, pm, width, width, height, roundControl);
			break;
		default:
			break;
	}

	psY= (Fw16s *) pSrcYData;
	pr = (Fw8u  *) pSrcRefB;
	pm = (Fw8u  *) pSrcRefM;
	pd = (Fw8u  *) pDst;

	switch(mcTypeB)
	{
		case FWVC_MC_APX_FF:
			if( Dispatch_Type != DT_SSE2)
				MCBUV_FF_8u_C1_C( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
				MCBUV_FF_8u_C1_SSE2( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_FH:
			if( Dispatch_Type != DT_SSE2)
			    MCBUV_FH_8u_C1_C( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCBUV_FH_8u_C1_SSE2( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_HF:
			if( Dispatch_Type != DT_SSE2)
			    MCBUV_HF_8u_C1_C( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCBUV_HF_8u_C1_SSE2( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		case FWVC_MC_APX_HH:
			if( Dispatch_Type != DT_SSE2)
			    MCBUV_HH_8u_C1_C( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			else
			    MCBUV_HH_8u_C1_SSE2( pm,	width, pr, srcStepB, psY, srcYDataStep, pd, dstStep, 
								width, height, roundControl);
			break;
		default:
			break;
	}

//	fwFree(pSrcRefM);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x4B_8u_C1)(const Fw8u  *pSrcRefF,	Fw32s srcStepF, Fw32s mcTypeF, 
							const Fw8u  *pSrcRefB,	Fw32s srcStepB, Fw32s mcTypeB, 
							const Fw16s *pSrcYData,  Fw32s srcYDataStep, 
								   Fw8u  *pDst,		Fw32s dstStep, 
								   Fw32s  roundControl)
{
	if(srcStepF < 4 || srcStepB < 4 || srcYDataStep < 8 || dstStep < 4) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

	return iMCB_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
						   pSrcRefB, srcStepB, mcTypeB, 
						   pSrcYData, srcYDataStep, 
						   pDst, dstStep, roundControl, 16,	4);
/*
	return fwiMCBUV_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 16,	4);
*/
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMC16x8BUV_8u_C1 )(const Fw8u *pSrcRefF, Fw32s srcStepF, Fw32s mcTypeF, 
							   const Fw8u *pSrcRefB, Fw32s srcStepB, Fw32s mcTypeB, 
							   const Fw16s *pSrcYData, Fw32s srcYDataStep, 
							   Fw8u *pDst, Fw32s dstStep, Fw32s roundControl)
{
	if(srcStepF < 8 || srcStepB < 8 || srcYDataStep < 16 || dstStep < 8) return fwStsSizeErr;
	if( roundControl < 0 || roundControl > 1) return fwStsBadArgErr;

	if(mcTypeF!=FWVC_MC_APX_FF && mcTypeF!=FWVC_MC_APX_FH &&
		mcTypeF!=FWVC_MC_APX_HF && mcTypeF!=FWVC_MC_APX_HH) return fwStsBadArgErr;
	if(mcTypeB!=FWVC_MC_APX_FF && mcTypeB!=FWVC_MC_APX_FH &&
		mcTypeB!=FWVC_MC_APX_HF && mcTypeB!=FWVC_MC_APX_HH) return fwStsBadArgErr;

		return iMCBUV_8u_C1 (pSrcRefF, srcStepF, mcTypeF, 
				   pSrcRefB, srcStepB, mcTypeB, 
				   pSrcYData, srcYDataStep, 
				   pDst, dstStep, roundControl, 16,	8);

}

#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
