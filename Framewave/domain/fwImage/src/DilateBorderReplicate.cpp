/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "Morphology.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )
#define MAXTHREADNUM 4

#ifdef Hai_DEBUG
#include <stdio.h>
#endif

typedef struct 
{
	const Fw8u* pSrc;
	int srcStep;
	Fw8u* pDst;
	int dstStep;
	FwiSize roiSize;
	int yBegin;
	int yEnd;
	int channel;
	Fw8u* pMask;
	FwiSize maskSize;
	FwiPoint anchor;
	Fw8u* pMask_pad;
}	fwiDilateBorderReplicate_8u_Not_In_Place;

#ifdef Hai_DEBUG
void printout_dilateBorder(Fw8u* pSrc, int width, int height, int channel)
{
	printf("_______________________________________________\n");

	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width*channel; i++)
		{
			printf("%d	", *(pSrc+ j * width*channel + i));
		}
		printf("\n");
	}
	printf("_______________________________________________\n");
}
#endif

//void setBufferLineZero_DilateBorderReplicate(Fw8u* pDst, int dstStep, int channel)
//{
//	Fw8u* pd = pDst;
//	int xEnd = dstStep - (int)((Fw64u)(pDst + dstStep) % 16);
//	int x = 0/*, k = 0*/;
//	channel = channel;
//
//	for(x = 0; x < xEnd; x+=16, pd += 16)
//	{
//		*(__m128i *)(pd) = _mm_setzero_si128();
//	}
//
//	//Deal with the remain part
//	if(xEnd != dstStep)
//	{
//		for(x = xEnd; x < dstStep; x++)
//		{
//				*(pDst+x)= 0;
//		}
//	}
//}

void setBufferLineZero_DilateBorderReplicate(Fw8u* pDst, int dstStep, int channel)
{
	Fw8u* pd  = pDst;
	int xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
	int xEnd = dstStep * channel - (int)((Fw64u)(pd + dstStep * channel) % 16);
	//bool bothAligned = false;
	int x = 0, k = 0;
	__m128i xmm00 = _mm_setzero_si128();

	//Copy the beginning part back to the source image
	if(xBegin != 0)
	{
		for(x = 0; x < xBegin/channel+1; x++)
		{
			for (k=0; k<channel; k++)
			{
				*(pDst+x*channel+k)=0;
			}
		}
	}

	//ps += xBegin;
	pd += xBegin;

	//if(bothAligned)
	//{
		for(x = xBegin; x < xEnd; x+=16, /*ps += 16, */pd += 16)
		{
			*(__m128i*)(pd) = xmm00;
		}
	//}
	//else
	//{
	//	for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
	//	{
	//		//xmm00 = _mm_loadu_si128( (__m128i*)(ps));
	//		_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
	//	}
	//}

	//Deal with the remain part
	if(xEnd != dstStep * channel)
	{
		for(x = xEnd/channel; x < dstStep; x++)
		{
			for (k=0; k<channel; k++)
			{
				*(pDst+x*channel+k)=0;
			}
		}
	}
}

/*
DilateBorderReplicate
Image dilation with replicated border
*/
template< class TS, CH chSrc, DispatchType disp >
FwStatus MyFW_DilateBorderReplicate(const TS* pSrc, int srcStep, TS* pDst,
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState)
{
	if (pSrc == NULL || pDst == NULL || pState == NULL) return fwStsNullPtrErr;
	if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiWidth) return fwStsSizeErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);

	if(srcStep < pState->roiWidth || dstStep < pState->roiWidth) return fwStsStepErr;

	if(sizeof(TS) == 4) //floating point image
	{
		if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;
	}

	border = fwBorderRepl;
	if(border != fwBorderRepl) return fwStsBadArgErr;

	//Here, other vendors might not check if all pMask elements are zero or not. But in the Dilate, they did.
	//Check if all mask values are equal to zero.
	/*
	for(x = 0; x < pState->maskSize.width * pState->maskSize.height; x++)
		if(*(pState->pMask+x) != 0)
			//Whenever found a nonzero value, I will set the x out of bound. So they can go out of the loop.
			x = pState->maskSize.width * pState->maskSize.height + 1;

	if(x != pState->maskSize.width * pState->maskSize.height + 1)
		return fwStsZeroMaskValuesErr;
	*/
	TS* srcAddr;
	TS* dstAddr;

	//Because I already initialized and "reflect" the pMask. Now, I do not need to do the "reflect" again.
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				//if(k == 3)
				//{
				//	srcAddr = (TS*)((Fw8u*)pSrc+y*srcStep+x*channel+k);
				//	dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

				//	*dstAddr = *srcAddr;
				//}
				//else
				//{
					dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*dstAddr = 0;

					for(int n = -pState->anchor.y; n < pState->maskSize.height-pState->anchor.y; n++)
					{
						for(int m = -pState->anchor.x; m < pState->maskSize.width-pState->anchor.x; m++)
						{
							//Ignore the pixels out of the boundary.
							//The ignorance will have equal result with border replicate.
							if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
								continue;

							if((pState->pMask[(n+pState->anchor.y)*(pState->maskSize.width) + (m+pState->anchor.x)]) > 0)
							{
								srcAddr = (TS*)((Fw8u*)pSrc+(y+n)*srcStep+(x+m)*channel+k);
								dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

								if(*srcAddr > *dstAddr)
								{
									*dstAddr = *srcAddr;
								}
							}
						}
					}
				//}
			}
		}
	}

	return fwStsNoErr;
}

void padMask_16_C1R_borderReplicate(Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
{
	int counter = 0;
	int x = 0, y = 0;

	for(y = 0; y < maskSize.height; y++)
	{
		for(x = 0; x < pMask_padWidth; x++)
		{
			if(pMask[y*maskSize.width + counter] > 0)
				pMask_pad[y*pMask_padWidth + x] = 255;			//11111111 in HEX
			else
				pMask_pad[y*pMask_padWidth + x] = 0;

			counter++;

			if(counter >= maskSize.width)
				counter -= maskSize.width;
		}

		counter = 0;
	}
}

//16 < maskSize.width <= 32
//Different than the padMask_16_C1R, this function will not circulate the mask.
void padMask_32_C1R_borderReplicate(Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
{
	for(int y = 0; y < maskSize.height; y++)
	{
		for(int x = 0; x < pMask_padWidth; x++)
		{
			if((x < maskSize.width) && (pMask[y*maskSize.width + x] > 0))
			{
				pMask_pad[y*pMask_padWidth + x] = 255;			//11111111 in HEX
			}
			else
			{
				pMask_pad[y*pMask_padWidth + x] = 0;
			}
		}
	}
}

void padMask_32_C3R_borderReplicate(Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
{
	for(int y = 0; y < maskSize.height; y++)
	{
		for(int x = 0; x < pMask_padWidth; x++)
		{
			if((x < maskSize.width) && (pMask[y*maskSize.width + x] > 0))
			{
				pMask_pad[y*pMask_padWidth + x] = 255;			//11111111 in HEX
			}
			else
			{
				pMask_pad[y*pMask_padWidth + x] = 0;
			}
		}
	}
}

void padMask_16_C3R_borderReplicate(Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
{
	for(int y = 0; y < maskSize.height; y++)
	{
		for(int x = 0; x < pMask_padWidth; x++)
		{
			pMask_pad[y*pMask_padWidth + x] = 0;
		}

		for(int z = 0; z < maskSize.width; z++)
		{
			if(pMask[y*maskSize.width + z] > 0)
			{
				pMask_pad[(y*pMask_padWidth + z*3)] = 255;
				pMask_pad[(y*pMask_padWidth + z*3)+1] = 255;
				pMask_pad[(y*pMask_padWidth + z*3)+2] = 255;
			}
		}
	}
}

void padMask_16_C4R_borderReplicate(Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
{
	for(int y = 0; y < maskSize.height; y++)
	{
		for(int x = 0; x < pMask_padWidth; x++)
		{
			pMask_pad[y*pMask_padWidth + x] = 0;
		}

		for(int z = 0; z < maskSize.width; z++)
		{
			if(pMask[y*maskSize.width + z] > 0)
			{
				pMask_pad[(y*pMask_padWidth + z*4)] = 255;
				pMask_pad[(y*pMask_padWidth + z*4)+1] = 255;
				pMask_pad[(y*pMask_padWidth + z*4)+2] = 255;
				pMask_pad[(y*pMask_padWidth + z*4)+3] = 255;
			}
		}
	}
}

void initResultMask_C1R_BorderReplicate(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize)
{
	for(int i = 0; i < padMaskWidth; i++)
	{
		if((i % maskSize.width == 0) && (i < padMaskWidth - maskSize.width))
		{
			resultMask[i] = 255;
		}
		else
		{
			resultMask[i] = 0;
		}
	}
}

void initResultMask_C3R_BorderReplicate(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize, int channel)
{
	for(int i = 0; i < padMaskWidth; i+=channel)
	{
		if((i % (maskSize.width*channel) == 0) && (i < (padMaskWidth - maskSize.width*channel)))
		{
			resultMask[i] = 255;
			resultMask[i+1] = 255;
			resultMask[i+2] = 255;
		}
		else
		{
			resultMask[i] = 0;
			resultMask[i+1] = 0;
			resultMask[i+2] = 0;
		}
	}
}

void initResultMask_C4R_BorderReplicate(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize, int channel)
{
	for(int i = 0; i < padMaskWidth; i+=channel)
	{
		if((i % (maskSize.width*channel) == 0) && (i < (padMaskWidth - maskSize.width*channel)))
		{
			resultMask[i] = 255;
			resultMask[i+1] = 255;
			resultMask[i+2] = 255;
			resultMask[i+3] = 255;
		}
		else
		{
			resultMask[i] = 0;
			resultMask[i+1] = 0;
			resultMask[i+2] = 0;
			resultMask[i+3] = 0;
		}
	}
}

void dilateBorderReplicate_8u_ref(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					  Fw8u* pMask_reflect, FwiSize maskSize, FwiPoint anchor, int channel, 
					   int xBegin, int xEnd, int yBegin, int yEnd)
{
	//Dilate with specified SE.
	for(int y = yBegin; y < yEnd; y++)
	{
		for(int x = xBegin; x < xEnd; x++)
		{
			for (int k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				//if(k == 3)
				//{
				//	*(pDst+y*dstStep+x*channel+k) = *(pSrc+y*srcStep+x*channel+k);
				//}
				//else
				//{
					*(pDst+y*dstStep+x*channel+k) = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might not use this way.
							//It simply use ROI without the boundary region.
							if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
								continue;

							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*(pSrc+(y+n)*srcStep+(x+m)*channel+k) > *(pDst+y*dstStep+x*channel+k)))
								{
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+(y+n)*srcStep+(x+m)*channel+k);
								}
						}
					}
				//}
			}
		}
	}
}

/*
For duplicated border pixel dilation:
1. Use Ref code to process regions at:
	1)(0,0) to (anchor.x, roiSize.height)
	2)(roiSize.width-anchor.x, 0)  to (roiSize.width - anchor.x, 
*/
void dilateBorderReplicate_C1R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 1;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
		  Fw8u* pd3 = pDst;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i xmm02;	//mask
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc += (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	/*
	For example, if the mask size is 5x3(w x h), then the result mask will be 100001000010000x.
	If the mask size is 7x5 (w x h), then the result mask will be 10000001000000xx.
	*/
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C1R_BorderReplicate(resultMask, pMask_pad_width, maskSize);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//initialize the pDst
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x), 1);

		for(m = anchor.x; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				xmm01 = _mm_loadu_si128((__m128i*)ps3);				//load data
				xmm02 = *(__m128i*)(pMask);							//load mask
				xmm00 = _mm_and_si128(xmm01, xmm02);				//masking

				ps3 += srcStep; pMask += pMask_pad_width;

				//Use a temporary buffer, load all maskSize.height data into the memory,
				//then use shift (rather than loadu) to process the data.
				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
					xmm02 = *(__m128i*)(pMask);						//load mask
					xmm01 = _mm_and_si128(xmm01, xmm02);			//masking
					xmm00 = _mm_max_epu8(xmm00, xmm01);				//find the max of rows
				}

				//find the max of the row max
				xmm01 = xmm00;

				for(z = 1; z < maskSize.width; z++)
				{
					xmm01 = _mm_srli_si128(xmm01, 1);
					xmm00 = _mm_max_epu8(xmm00, xmm01);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm01 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm01, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

//Dilate (C1R), maskSize.width <= 32 && maskSize.width > 16.
void dilateBorderReplicate_C1R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 1;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
		  Fw8u* pd3 = pDst;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm010,/*first 16-byte data*/ xmm011; /*Remain data*/
	__m128i xmm020, /*first 16-byte mask*/xmm021; /*Remain mask*/
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc += (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	/*
	When 16 < maskSize.width <= 32, the result mask will be 1000000000000000
	*/
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	resultMask[0] = 255;
	for(x = 1; x < 16; x++)
	{
		resultMask[x] = 0;
	}
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//set the values in pDst to zero
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x), 1);

		for(m = anchor.x; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			//Deal with the first 16 bytes.
			for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				xmm010 = _mm_loadu_si128((__m128i*)ps3);			//load first 16-byte data
				xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));		//load remained data
				xmm020 = *(__m128i*)(pMask);						//load first 16-byte mask
				xmm021 = *(__m128i*)(pMask+16);						//load remained mask
				xmm010 = _mm_and_si128(xmm010, xmm020);				//masking
				xmm011 = _mm_and_si128(xmm011, xmm021);				//masking
				xmm00 = _mm_and_si128(xmm010, xmm011);				//masking

				ps3 += srcStep; pMask += pMask_pad_width;

				//Use a temporary buffer, load all maskSize.height data into the memory,
				//then use shift (rather than loadu) to process the data.
				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm010 = _mm_loadu_si128((__m128i*)ps3);			//load first 16-byte data
					xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));		//load remained data
					xmm020 = *(__m128i*)(pMask);						//load first 16-byte mask
					xmm021 = *(__m128i*)(pMask+16);						//load remained mask
					xmm010 = _mm_and_si128(xmm010, xmm020);				//masking
					xmm011 = _mm_and_si128(xmm011, xmm021);				//masking
					xmm010 = _mm_max_epu8(xmm010, xmm011);				//masking
					xmm00 = _mm_max_epu8(xmm00, xmm010);				//find the max of rows
				}

				//find the max of the row max
				xmm010 = xmm00;

				for(z = 1; z < 16; z++)
				{
					xmm010 = _mm_srli_si128(xmm010, 1);
					xmm00 = _mm_max_epu8(xmm00, xmm010);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm010 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm010, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

//Dilate (C3R), maskSize.width <= 5.
void dilateBorderReplicate_C3R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 3;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
		  Fw8u* pd3 = pDst;		//temp pointer

	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i xmm02;	//mask

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc += (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C3R_BorderReplicate(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//initialize the pDst
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x*channel), 1);

		for(m = anchor.x*channel; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				//_mm_prefetch((char*)(ps3), _MM_HINT_T1);

				xmm01 = _mm_loadu_si128((__m128i*)(ps3));			//load data
				xmm02 = *(__m128i*)(pMask);							//load mask
				xmm00 = _mm_and_si128(xmm01, xmm02);				//masking

				ps3 += srcStep; pMask += pMask_pad_width;

				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
					xmm02 = *(__m128i*)(pMask);						//load mask
					xmm01 = _mm_and_si128(xmm01, xmm02);			//masking
					xmm00 = _mm_max_epu8(xmm00, xmm01);				//find the max of rows
				}

				//find the max of the row max
				xmm01 = xmm00;

				for(z = 1; z < maskSize.width; z++)
				{
					xmm01 = _mm_srli_si128(xmm01, 3);
					xmm00 = _mm_max_epu8(xmm00, xmm01);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm01 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm01, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

//Dilate (C3R), 6 <= maskSize.width <= 10
void dilateBorderReplicate_C3R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 3;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
	const Fw8u* pd3 = pSrc;		//temp pointer
	
	__m128i xmm00;	//result
	__m128i xmm010,/*first 16-byte data*/ xmm011; /*Remain data*/
	__m128i xmm020, /*first 16-byte mask*/xmm021; /*Remain mask*/

	int jumpStep = (maskSize.width * channel) * (pMask_pad_width / (maskSize.width * channel));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc -= (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	/*
	When 6 <= maskSize.width <= 10, the result mask will be 1110000000000000
	*/
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	resultMask[0] = 255; resultMask[1] = 255; resultMask[2] = 255;
	for(x = 3; x < 16; x++)
	{
		resultMask[x] = 0;
	}
	__m128i rmask = *(__m128i*)(resultMask);
	//printout(pMask, pMask_pad_width, maskSize.height, 1);

	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//initialize the pDst
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x*channel), 1);

		for(m = anchor.x*channel; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				_mm_prefetch((char*)(ps3), _MM_HINT_T1);
				_mm_prefetch((char*)(ps3+16), _MM_HINT_T1);

				xmm010 = _mm_loadu_si128((__m128i*)ps3);			//load first 16-byte data
				xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));		//load remained data
				xmm020 = *(__m128i*)(pMask);						//load first 16-byte mask
				xmm021 = *(__m128i*)(pMask+16);						//load remained mask
				xmm010 = _mm_and_si128(xmm010, xmm020);				//masking
				xmm011 = _mm_and_si128(xmm011, xmm021);				//masking
				xmm020 = _mm_srli_si128(xmm010, 15);				//shifting
				xmm021 = _mm_slli_si128(xmm011, 1);					//shifting
				xmm020 = _mm_or_si128(xmm020, xmm021);				//Obtain the red byte back
				xmm00 = _mm_max_epu8(xmm020, xmm010);				//find the max of rows

				/*
				Because of the maskSize.width >= 6, so the whole mask (3 channels) will go over the 16 byte
				register. For example: if the maskSize.width = 7, then the original image (one line) will be:
					|							     |								   |
				...	|r g b r g b r g b r g b r g b r | g b r g b r g b r g b r g b ? ? |...
					|							   | |								   |
	 											   | 16								  16
												   ^
												   this byte has to be shift to the next register to guarantee
												   data correctness.
			   ??: the last 2 bytes of the 32 bytes will not be used in the dilation process.
				*/
				//xmm00 = _mm_and_si128(xmm020, xmm010);				//masking

				ps3 += srcStep; pMask += pMask_pad_width;

				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm010 = _mm_loadu_si128((__m128i*)ps3);		//load first 16-byte data
					xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));	//load remained data
					xmm020 = *(__m128i*)(pMask);					//load first 16-byte mask
					xmm021 = *(__m128i*)(pMask+16);					//load remained mask
					xmm010 = _mm_and_si128(xmm010, xmm020);			//masking
					xmm011 = _mm_and_si128(xmm011, xmm021);			//masking
					xmm020 = _mm_srli_si128(xmm010, 15);			//shifting
					xmm021 = _mm_slli_si128(xmm011, 1);				//shifting
					xmm020 = _mm_or_si128(xmm020, xmm021);			//Obtain the red byte back
					xmm010 = _mm_max_epu8(xmm020, xmm010);			//masking
					xmm00 = _mm_max_epu8(xmm00, xmm010);			//find the max of rows
				}

				//find the max of the row max
				xmm010 = xmm00;

				for(z = 1; z <= maskSize.width; z++)
				{
					xmm010 = _mm_srli_si128(xmm010, 3);
					xmm00 = _mm_max_epu8(xmm00, xmm010);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm010 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm010, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

//Dilate (C4R), maskSize.width <= 4.
void dilateBorderReplicate_C4R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 4;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
		  Fw8u* pd3 = pDst;		//temp pointer

	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i xmm02;	//mask

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc += (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C4R_BorderReplicate(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//initialize the pDst
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x*channel), 1);

		for(m = anchor.x*channel; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				//_mm_prefetch((char*)(ps3), _MM_HINT_T1);

				xmm01 = _mm_loadu_si128((__m128i*)(ps3));			//load data
				xmm02 = *(__m128i*)(pMask);							//load mask
				xmm00 = _mm_and_si128(xmm01, xmm02);				//masking

				ps3 += srcStep; pMask += pMask_pad_width;

				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
					xmm02 = *(__m128i*)(pMask);						//load mask
					xmm01 = _mm_and_si128(xmm01, xmm02);			//masking
					xmm00 = _mm_max_epu8(xmm00, xmm01);				//find the max of rows
				}

				//find the max of the row max
				xmm01 = xmm00;

				for(z = 1; z < maskSize.width; z++)
				{
					xmm01 = _mm_srli_si128(xmm01, 4);
					xmm00 = _mm_max_epu8(xmm00, xmm01);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm01 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm01, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

//Dilate (C4R), 4 < maskSize.width <= 8
void dilateBorderReplicate_C4R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 4;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
	const Fw8u* pd3 = pSrc;		//temp pointer
	
	__m128i xmm00;	//result
	__m128i xmm010,/*first 16-byte data*/ xmm011; /*Remain data*/
	__m128i xmm020, /*first 16-byte mask*/xmm021; /*Remain mask*/

	int jumpStep = (maskSize.width * channel) * (pMask_pad_width / (maskSize.width * channel));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	pd = pDst;
	for(y = 0; y < anchor.y; y++, pd += dstStep)
	{
		setBufferLineZero_DilateBorderReplicate(pd, dstStep, 1);
	}

	//pSrc -= (anchor.y * srcStep + anchor.x * channel);
	pDst += (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	//resultMask[0] = 255; resultMask[1] = 255; resultMask[2] = 255; resultMask[3] = 255;

	for(x = 0; x < channel; x++)
	{
		resultMask[x] = 255;
	}

	for(x = channel; x < 16; x++)
	{
		resultMask[x] = 0;
	}

	__m128i rmask = *(__m128i*)(resultMask);
	//printout(pMask, pMask_pad_width, maskSize.height, 1);

	for(y = yBegin; y <= yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		//initialize the pDst
		setBufferLineZero_DilateBorderReplicate(pDst, (mEnd-anchor.x*channel), 1);

		for(m = anchor.x*channel; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				//_mm_prefetch((char*)(ps3), _MM_HINT_T1);
				//_mm_prefetch((char*)(ps3+16), _MM_HINT_T1);

				xmm010 = _mm_loadu_si128((__m128i*)ps3);			//load first 16-byte data
				xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));		//load remained data
				xmm020 = *(__m128i*)(pMask);						//load first 16-byte mask
				xmm021 = *(__m128i*)(pMask+16);						//load remained mask
				xmm010 = _mm_and_si128(xmm010, xmm020);				//masking
				xmm011 = _mm_and_si128(xmm011, xmm021);				//masking
				xmm00 = _mm_max_epu8(xmm010, xmm011);				//find the max of rows

				ps3 += srcStep; pMask += pMask_pad_width;

				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
				{
					xmm010 = _mm_loadu_si128((__m128i*)ps3);		//load first 16-byte data
					xmm011 = _mm_loadu_si128((__m128i*)(ps3+16));	//load remained data
					xmm020 = *(__m128i*)(pMask);					//load first 16-byte mask
					xmm021 = *(__m128i*)(pMask+16);					//load remained mask
					xmm010 = _mm_and_si128(xmm010, xmm020);			//masking
					xmm011 = _mm_and_si128(xmm011, xmm021);			//masking
					xmm010 = _mm_max_epu8(xmm010, xmm011);			//masking
					xmm00 = _mm_max_epu8(xmm00, xmm010);			//find the max of rows
				}

				//find the max of the row max
				xmm010 = xmm00;

				for(z = 1; z <= maskSize.width; z++)
				{
					xmm010 = _mm_srli_si128(xmm010, 4);
					xmm00 = _mm_max_epu8(xmm00, xmm010);
				}

				xmm00 = _mm_and_si128(xmm00, rmask);
				xmm010 = _mm_loadu_si128((__m128i*)pd3);
				xmm00 = _mm_or_si128(xmm010, xmm00);
				_mm_storeu_si128((__m128i *)(pd3), xmm00);

				ps3 = ps2;
			}
		}
	}

	fwFree(resultMask);
}

void My_fwiDilateBorderReplicate_8u_C1R(void* param)
{
	fwiDilateBorderReplicate_8u_Not_In_Place *dilate_par = (fwiDilateBorderReplicate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	Fw8u* pMask = dilate_par->pMask;
	FwiSize maskSize = dilate_par->maskSize;
	FwiPoint anchor = dilate_par->anchor;

	int pMask_pad_width = 0;

	if(maskSize.width <= 16)
		pMask_pad_width = 16;
	else if (maskSize.width > 16 && maskSize.width <= 32)
		pMask_pad_width = 32;
	else if (maskSize.width > 32 && maskSize.width <= 64)
		pMask_pad_width = 64;
	else if (maskSize.width > 64 && maskSize.width <= 128)
		pMask_pad_width = 128;
	else
		pMask_pad_width = 128;

	//pad the mask with 0's
	Fw8u* pMask_pad = (Fw8u*) fwMalloc(sizeof(Fw8u) * pMask_pad_width * channel * maskSize.height);

	if(maskSize.width <= 16)
	{
		padMask_16_C1R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilateBorderReplicate_C1R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 16 && maskSize.width <= 32)
	{
		padMask_32_C1R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilateBorderReplicate_C1R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 32 && maskSize.width <= 64)
		pMask_pad_width = 64;
	else if (maskSize.width > 64 && maskSize.width <= 128)
		pMask_pad_width = 128;
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilateBorderReplicate_8u_C3R(void* param)
{
	fwiDilateBorderReplicate_8u_Not_In_Place *dilate_par = (fwiDilateBorderReplicate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	Fw8u* pMask = dilate_par->pMask;
	FwiSize maskSize = dilate_par->maskSize;
	FwiPoint anchor = dilate_par->anchor;

	int pMask_pad_width = 0;

	if(maskSize.width <= 5)
		pMask_pad_width = 16;
	else if (maskSize.width > 5 && maskSize.width <= 10)
		pMask_pad_width = 32;
	else if (maskSize.width > 10 && maskSize.width <= 16)
		pMask_pad_width = 48;
	else
		pMask_pad_width = 128;

	//pad the mask with 0's
	Fw8u* pMask_pad = (Fw8u*) fwMalloc(sizeof(Fw8u) * pMask_pad_width * maskSize.height);

	if(maskSize.width <= 5)
	{
		//Circulate padding the mask to fit into 16 bytes SSE2 register
		padMask_16_C3R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout_dilateBorder(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilateBorderReplicate_C3R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 5 && maskSize.width <= 10)
	{
		padMask_16_C3R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilateBorderReplicate_C3R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 10 && maskSize.width <= 16)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilateBorderReplicate_8u_C4R(void* param)
{
	fwiDilateBorderReplicate_8u_Not_In_Place *dilate_par = (fwiDilateBorderReplicate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	Fw8u* pMask = dilate_par->pMask;
	FwiSize maskSize = dilate_par->maskSize;
	FwiPoint anchor = dilate_par->anchor;

	int pMask_pad_width = 0;

	if(maskSize.width <= 4)
		pMask_pad_width = 16;
	else if (maskSize.width > 4 && maskSize.width <= 8)
		pMask_pad_width = 32;
	else if (maskSize.width > 8 && maskSize.width <= 12)
		pMask_pad_width = 48;
	else if (maskSize.width > 12 && maskSize.width <= 16)
		pMask_pad_width = 64;
	else
		pMask_pad_width = 128;

	//pad the mask with 0's
	Fw8u* pMask_pad = (Fw8u*) fwMalloc(sizeof(Fw8u) * pMask_pad_width * maskSize.height);

	if(maskSize.width <= 4)
	{
		//Circulate padding the mask to fit into 16 bytes SSE2 register
		padMask_16_C4R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilateBorderReplicate_C4R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 4 && maskSize.width <= 8)
	{
		padMask_16_C4R_borderReplicate(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilateBorderReplicate_C4R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 8 && maskSize.width <= 12)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL || pState == NULL) return fwStsNullPtrErr;
			if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiWidth) return fwStsSizeErr;

			int channel = 1;

			if(srcStep < pState->roiWidth || dstStep < pState->roiWidth) return fwStsStepErr;

			if(sizeof(Fw8u) == 4) //floating point image
			{
				if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;
			}

			border = fwBorderRepl;
			if(border != fwBorderRepl) return fwStsBadArgErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= pState->maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			//Already do the mask reflection in the MorphologyInit or MorphologyInitAlloc
			//char* pMask_reflect = (char*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilateBorderReplicate_8u_Not_In_Place dilateBorderReplicate_par[MAXTHREADNUM];

//#ifdef Hai_DEBUG
//			total_thread_num = 1;
//#endif

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num);

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height-(pState->maskSize.height - pState->anchor.y)-1;
					}

					//if(thread_num > 0)
					//{
					//	//Because of the "analysis window", in each strip, the starting address 
					//	//should go back a little.
					//	y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					//}

					dilateBorderReplicate_par[thread_num].pSrc = pSrc;
					dilateBorderReplicate_par[thread_num].srcStep = srcStep;
					dilateBorderReplicate_par[thread_num].pDst = pDst;
					dilateBorderReplicate_par[thread_num].dstStep = dstStep;
					dilateBorderReplicate_par[thread_num].roiSize = roiSize;
					dilateBorderReplicate_par[thread_num].yBegin = y_bgn;
					dilateBorderReplicate_par[thread_num].yEnd = y_end;
					dilateBorderReplicate_par[thread_num].channel = channel;
					dilateBorderReplicate_par[thread_num].pMask = pState->pMask;
					dilateBorderReplicate_par[thread_num].maskSize = pState->maskSize;
					dilateBorderReplicate_par[thread_num].anchor = pState->anchor;

					ThreadPool::Run(My_fwiDilateBorderReplicate_8u_C1R, (void *) &dilateBorderReplicate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);
			int pMask_pad_width = 0;

			if(pState->maskSize.width <= 16)
				pMask_pad_width = 16;
			else if (pState->maskSize.width > 16 && pState->maskSize.width <= 32)
				pMask_pad_width = 32;
			else if (pState->maskSize.width > 32 && pState->maskSize.width <= 64)
				pMask_pad_width = 64;
			else if (pState->maskSize.width > 64 && pState->maskSize.width <= 128)
				pMask_pad_width = 128;
			else
				pMask_pad_width = 128;

			int jumpStep = pState->maskSize.width * (pMask_pad_width / pState->maskSize.width);
			int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 0, roiSize.width, 0,  pState->anchor.y);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0,  pState->anchor.x * channel, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				mEnd/channel, roiSize.width, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0, roiSize.width, roiSize.height - (pState->maskSize.height - pState->anchor.y), roiSize.height);

			return fwStsNoErr;
		}
		default:
			return MyFW_DilateBorderReplicate<Fw8u, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL || pState == NULL) return fwStsNullPtrErr;
			if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiWidth) return fwStsSizeErr;

			int channel = 3;

			if(srcStep < pState->roiWidth || dstStep < pState->roiWidth) return fwStsStepErr;

			if(sizeof(Fw8u) == 4) //floating point image
			{
				if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;
			}

			border = fwBorderRepl;
			if(border != fwBorderRepl) return fwStsBadArgErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= pState->maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			//Already do the mask reflection in the MorphologyInit or MorphologyInitAlloc
			//char* pMask_reflect = (char*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilateBorderReplicate_8u_Not_In_Place dilateBorderReplicate_par[MAXTHREADNUM];

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

//#ifdef Hai_DEBUG
//				total_thread_num = 1;
//#endif

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num);

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height-(pState->maskSize.height - pState->anchor.y)-2;
					}

					//if(thread_num > 0)
					//{
					//	//Because of the "analysis window", in each strip, the starting address 
					//	//should go back a little.
					//	y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					//}

					dilateBorderReplicate_par[thread_num].pSrc = pSrc;
					dilateBorderReplicate_par[thread_num].srcStep = srcStep;
					dilateBorderReplicate_par[thread_num].pDst = pDst;
					dilateBorderReplicate_par[thread_num].dstStep = dstStep;
					dilateBorderReplicate_par[thread_num].roiSize = roiSize;
					dilateBorderReplicate_par[thread_num].yBegin = y_bgn;
					dilateBorderReplicate_par[thread_num].yEnd = y_end;
					dilateBorderReplicate_par[thread_num].channel = channel;
					dilateBorderReplicate_par[thread_num].pMask = pState->pMask;
					dilateBorderReplicate_par[thread_num].maskSize = pState->maskSize;
					dilateBorderReplicate_par[thread_num].anchor = pState->anchor;

					ThreadPool::Run(My_fwiDilateBorderReplicate_8u_C3R, (void *) &dilateBorderReplicate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);

			int pMask_pad_width = 0;

			if(pState->maskSize.width <= 16)
				pMask_pad_width = 16;
			else if (pState->maskSize.width > 16 && pState->maskSize.width <= 32)
				pMask_pad_width = 32;
			else if (pState->maskSize.width > 32 && pState->maskSize.width <= 64)
				pMask_pad_width = 64;
			else if (pState->maskSize.width > 64 && pState->maskSize.width <= 128)
				pMask_pad_width = 128;
			else
				pMask_pad_width = 128;

			int jumpStep = pState->maskSize.width * (pMask_pad_width / pState->maskSize.width);
			int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 0, roiSize.width, 0,  pState->anchor.y);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0,  pState->anchor.x * channel, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				(mEnd/channel)*channel, roiSize.width, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0, roiSize.width, roiSize.height - (pState->maskSize.height - pState->anchor.y)-2, roiSize.height);

			return fwStsNoErr;
		}
		default:
			return MyFW_DilateBorderReplicate<Fw8u, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL || pState == NULL) return fwStsNullPtrErr;
			if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiWidth) return fwStsSizeErr;

			int channel = 4;

			if(srcStep < pState->roiWidth || dstStep < pState->roiWidth) return fwStsStepErr;

			if(sizeof(Fw8u) == 4) //floating point image
			{
				if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;
			}

			border = fwBorderRepl;
			if(border != fwBorderRepl) return fwStsBadArgErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= pState->maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			//Already do the mask reflection in the MorphologyInit or MorphologyInitAlloc
			//char* pMask_reflect = (char*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilateBorderReplicate_8u_Not_In_Place dilateBorderReplicate_par[MAXTHREADNUM];

//#ifdef Hai_DEBUG
//			total_thread_num = 1;
//#endif

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num);

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height-(pState->maskSize.height - pState->anchor.y)-2;
					}

					//if(thread_num > 0)
					//{
					//	//Because of the "analysis window", in each strip, the starting address 
					//	//should go back a little.
					//	y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					//}

					dilateBorderReplicate_par[thread_num].pSrc = pSrc;
					dilateBorderReplicate_par[thread_num].srcStep = srcStep;
					dilateBorderReplicate_par[thread_num].pDst = pDst;
					dilateBorderReplicate_par[thread_num].dstStep = dstStep;
					dilateBorderReplicate_par[thread_num].roiSize = roiSize;
					dilateBorderReplicate_par[thread_num].yBegin = y_bgn;
					dilateBorderReplicate_par[thread_num].yEnd = y_end;
					dilateBorderReplicate_par[thread_num].channel = channel;
					dilateBorderReplicate_par[thread_num].pMask = pState->pMask;
					dilateBorderReplicate_par[thread_num].maskSize = pState->maskSize;
					dilateBorderReplicate_par[thread_num].anchor = pState->anchor;

					ThreadPool::Run(My_fwiDilateBorderReplicate_8u_C4R, (void *) &dilateBorderReplicate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);
			int pMask_pad_width = 0;

			if(pState->maskSize.width <= 4)
				pMask_pad_width = 16;
			else if (pState->maskSize.width > 4 && pState->maskSize.width <= 8)
				pMask_pad_width = 32;
			else if (pState->maskSize.width > 8 && pState->maskSize.width <= 12)
				pMask_pad_width = 48;
			else if (pState->maskSize.width > 12 && pState->maskSize.width <= 16)
				pMask_pad_width = 64;
			else
				pMask_pad_width = 128;

			int jumpStep = pState->maskSize.width * (pMask_pad_width / pState->maskSize.width);
			int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 0, roiSize.width, 0,  pState->anchor.y);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0,  pState->anchor.x * channel, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				(mEnd/channel)*channel, roiSize.width, 0, roiSize.height);

			dilateBorderReplicate_8u_ref(pSrc, srcStep, pDst, dstStep, roiSize, 
				pState->pMask, pState->maskSize, pState->anchor, channel, 
				0, roiSize.width, roiSize.height - (pState->maskSize.height - pState->anchor.y)-2, roiSize.height);

			return fwStsNoErr;
		}
		default:
			return MyFW_DilateBorderReplicate<Fw8u, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
	return MyFW_DilateBorderReplicate<Fw32f, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
	return MyFW_DilateBorderReplicate<Fw32f, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, 
									  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState) 
{
	return MyFW_DilateBorderReplicate<Fw32f, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}


#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
