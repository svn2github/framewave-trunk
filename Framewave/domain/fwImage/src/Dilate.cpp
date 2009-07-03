/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#if 0
#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"

#if BUILD_NUM_AT_LEAST( 9999 )
#define MAXTHREADNUM 4

#ifdef Hai_DEBUG
#include <stdio.h>
#endif

using namespace OPT_LEVEL;

//Todo: In-place operation should use temporary buffer to store the results.

/*
Current idea to do the dilate with arbitrary shape of SE
1. Load all data into buffer; (done)
2. Padding the mask for future processing; (done but without using SSE2)
3. Process the data in the buffer;
4.1.	For In-Place operation, directly save the results to destination image
4.2		For Not-In-Place operation, temporary save the results to a buffer and the buffer back to original (source
			image) when they original pixels will not be touch any more;

Note: 
1. In the first tryout, I will only loadu rather than load (which means I will load 16-byte not-aligned data rather
than l6-byte aligned data).
1. I should try to process the image vertically, then I will just get ONE line of processed data. After that, I just
need to get the horizontal results;
OR
2. I can svae the line to a row, which might save some time to compute the max(min) value without shifting.
*/

//const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, 
//FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor
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
	const Fw8u* pMask;
	FwiSize maskSize;
	FwiPoint anchor;
	Fw8u* pMask_pad;
}	fwiDilate_8u_Not_In_Place;

typedef struct 
{
	Fw8u* pSrc;
	int srcStep;
	Fw8u* pDst;
	int dstStep;
	FwiSize roiSize;
	int yBegin;
	int yEnd;
	int channel;
	const Fw8u* pMask;
	FwiSize maskSize;
	FwiPoint anchor;
	Fw8u* pMask_pad;
	Fw8u* remainTopLines;
	Fw8u* remainBottomLines;
}	fwiDilate_8u_In_Place;

#ifdef Hai_DEBUG
void printout(Fw8u* pSrc, int width, int height, int channel)
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

void printout(Fw8u** pSrc, int width, int height, int channel)
{
	printf("_______________________________________________\n");

	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width*channel; i++)
		{
			//printf("%d	", *(pSrc+ j * width*channel + i));
			printf("%d	", *(pSrc[j] + i));
		}
		printf("\n");
	}
	printf("_______________________________________________\n");
}

void printout(const Fw8u* pSrc, int width, int height, int channel)
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

void printout(char* pSrc, int width, int height, int channel)
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

void printout(const char* pSrc, int width, int height, int channel)
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


inline __m128i _My_mm_slli_si128(__m128i xmm01, int imd)
{
	/*if(imd != 0)
	{
		for(int i = 0; i < imd; i++)
		{
			xmm01 = _mm_slli_si128(xmm01, 1);
		}
	}*/
	imd = imd%16;

	switch(imd)
	{
	case 1:
		xmm01 = _mm_slli_si128(xmm01,1);
	case 2:
		xmm01 = _mm_slli_si128(xmm01,2);
	case 3:
		xmm01 = _mm_slli_si128(xmm01,3);
	case 4:
		xmm01 = _mm_slli_si128(xmm01,4);
	case 5:
		xmm01 = _mm_slli_si128(xmm01,5);
	case 6:
		xmm01 = _mm_slli_si128(xmm01,6);
	case 7:
		xmm01 = _mm_slli_si128(xmm01,7);
	case 8:
		xmm01 = _mm_slli_si128(xmm01,8);
	case 9:
		xmm01 = _mm_slli_si128(xmm01,9);
	case 10:
		xmm01 = _mm_slli_si128(xmm01,10);
	case 11:
		xmm01 = _mm_slli_si128(xmm01,11);
	case 12:
		xmm01 = _mm_slli_si128(xmm01,12);
	case 13:
		xmm01 = _mm_slli_si128(xmm01,13);
	case 14:
		xmm01 = _mm_slli_si128(xmm01,14);
	case 15:
		xmm01 = _mm_slli_si128(xmm01,15);
	default:
		xmm01 = xmm01;
	}

	return xmm01;
}
#endif

__m128i _My_mm_srli_si128(__m128i xmm01, __m128i* xmm031, int imd)
{
	xmm031 = xmm031;

	if(imd != 0)
	{
		for(int i = 0; i < imd; i++)
		{
			xmm01 = _mm_slli_si128(xmm01, 1);
		}
	}

	return xmm01;
}

__m128i _My_mm_srli_si128(__m128i xmm01, int imd)
{
	if(imd != 0)
	{
		for(int i = 0; i < imd; i++)
		{
			xmm01 = _mm_srli_si128(xmm01, 1);
		}
	}

	return xmm01;
}

//The dilation reference code
void dilate_ref(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					   const Fw8u* pMask_reflect, FwiSize maskSize, FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	for(int y = 0; y <= yEnd - yBegin; y++)
	{
		for(int x = 0; x < roiSize.width; x++)
		{
			for (int k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					*(pDst+y*dstStep+x*channel+k) = *(pSrc+y*srcStep+x*channel+k);
				}
				else
				{
					*(pDst+y*dstStep+x*channel+k) = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*(pSrc+(y+n)*srcStep+(x+m)*channel+k) > *(pDst+y*dstStep+x*channel+k)))
								{
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+(y+n)*srcStep+(x+m)*channel+k);
								}
						}
					}
				}
			}
		}
	}
}

//The dilation reference code
void dilate_ref(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					  const Fw8u* pMask_reflect, FwiSize maskSize, FwiPoint anchor, int channel, 
					   int xBegin, int xEnd, int yBegin, int yEnd)
{
	roiSize = roiSize;

	//Dilate with specified SE.
	for(int y = 0; y <= yEnd - yBegin; y++)
	{
		for(int x = xBegin; x < xEnd; x++)
		{
			for (int k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					*(pDst+y*dstStep+x*channel+k) = *(pSrc+y*srcStep+x*channel+k);
				}
				else
				{
					*(pDst+y*dstStep+x*channel+k) = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*(pSrc+(y+n)*srcStep+(x+m)*channel+k) > *(pDst+y*dstStep+x*channel+k)))
								{
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+(y+n)*srcStep+(x+m)*channel+k);
								}
						}
					}
				}
			}
		}
	}
}

//Dilation ref in the bufferImage
//void dilate_ref(Fw8u** pSrc, Fw8u* pDst, int dstStep, const Fw8u* pMask_reflect, 
//				FwiSize maskSize, FwiPoint anchor, int channel, 
//					   int xBegin, int xEnd, int yBegin, int yEnd)
//{
//	//Dilate with specified SE.
//	for(int y = 0; y <= yEnd - yBegin; y++)
//	{
//		for(int x = xBegin; x < xEnd + anchor.x; x++)
//		{
//			for (int k=0; k<channel; k++)
//			{
//				//In the four-channel imamge, the alpha channel is not processed.
//				if(k == 3)
//				{
//					*(pDst+y*dstStep+(x-anchor.x)*channel+k ) = *(pSrc[y + anchor.y]+x*channel+k);
//				}
//				else
//				{
//					*(pDst+y*dstStep+x*channel+k - anchor.x) = 0;
//
//					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
//					{
//						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
//						{
//							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
//							//This will give me the same outcome as Matlab. But other vendors might not use this way.
//							//It simply use ROI without the boundary region.
//							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
//							//	continue;
//
//							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
//								(*(pSrc[(y+n)+ anchor.y]+(x+m)*channel+k) > *(pDst+y*dstStep+(x-anchor.x)*channel+k)))
//								{
//									*(pDst+y*dstStep+(x-anchor.x)*channel+k) = *(pSrc[(y+n)+ anchor.y]+(x+m)*channel+k);
//								}
//						}
//					}
//				}
//			}
//		}
//	}
//}

void dilate_ref(Fw8u** pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					  const Fw8u* pMask_reflect, FwiSize maskSize, FwiPoint anchor, int channel, 
					   int xBegin, int xEnd, int yBegin, int yEnd)
{
	roiSize = roiSize;
	srcStep = srcStep;

	//Dilate with specified SE.
	for(int y = 0; y <= yEnd - yBegin; y++)
	{
		for(int x = xBegin; x < xEnd + anchor.x; x++)
		{
			for (int k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					*(pDst+y*dstStep+(x-anchor.x)*channel+k ) = *(pSrc[y + anchor.y]+x*channel+k);
				}
				else
				{
					*(pDst+y*dstStep+x*channel+k - anchor.x) = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might does not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*(pSrc[(y+n)+ anchor.y]+(x+m)*channel+k) > *(pDst+y*dstStep+(x-anchor.x)*channel+k)))
								{
									*(pDst+y*dstStep+(x-anchor.x)*channel+k) = *(pSrc[(y+n)+ anchor.y]+(x+m)*channel+k);
								}
						}
					}
				}
			}
		}
	}
}

//Dilation ref in the bufferImage
void dilate_ref_C1IR(Fw8u** pSrc, Fw8u* pDst, int dstStep, const Fw8u* pMask_reflect, 
				FwiSize maskSize, FwiPoint anchor, int xBegin, int xEnd, int yBegin, int yEnd)
{
	//Dilate with specified SE.
	for(int y = 0; y <= yEnd - yBegin; y++)
	{
		for(int x = xBegin; x < xEnd + anchor.x; x++)
		{
			*(pDst+y*dstStep+x - anchor.x) = 0;

			for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
			{
				for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
				{
					if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
						(*(pSrc[(y+n)+ anchor.y]+(x+m)) > *(pDst+y*dstStep+(x-anchor.x))))
						{
							*(pDst+y*dstStep+(x-anchor.x)) = *(pSrc[(y+n)+ anchor.y]+(x+m));
						}
				}
			}
		}
	}
}

void initResultMask_C1R(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize)
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

//Fill the empty byte in 16 bytes SSE2 register
void initDataMask_C1IR(Fw8u* resultMask, int padMaskWidth, int jumpStep)
{
	for(int i = 0; i < padMaskWidth; i++)
	{
		if(i < jumpStep)
		{
			resultMask[i] = 255;
		}
		else
		{
			resultMask[i] = 0;
		}
	}
}


void initResultMask_C3R(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize, int channel)
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

void initResultMask_C4R(Fw8u* resultMask, int padMaskWidth, FwiSize maskSize, int channel)
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

//Load data from Source (pSrc) to Destination (pDst)
//The starting address might not be 16-byte aligned
//The border address might not be 16-byte aligned
//void copyOneLine(Fw8u* pSrc, Fw8u* pDst, int width, int channel)
//{
//	Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
//	int xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
//	int xEnd = width * channel - (int)((Fw64u)(pd + width * channel) % 16);
//	bool bothAligned = false;
//	int x = 0, k = 0;
//	__m128i xmm00;
//
//	if((16 - (int)((Fw64u)(ps))%16)%16 == xBegin)
//		bothAligned = true;
//	else
//		bothAligned = false;
//
//	//Copy the beginning part back to the source image
//	if(xBegin != 0)
//	{
//		for(x = 0; x < xBegin/channel+1; x++)
//		{
//			for (k=0; k<channel; k++)
//			{
//				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
//			}
//		}
//	}
//
//	ps += xBegin;
//	pd += xBegin;
//
//	if(bothAligned)
//	{
//		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
//		{
//			xmm00 = _mm_load_si128( (__m128i*)(ps));
//			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
//			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
//		}
//	}
//	else
//	{
//		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
//		{
//			xmm00 = _mm_loadu_si128( (__m128i*)(ps));
//			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
//			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
//		}
//	}
//
//	//Deal with the remain part
//	if(xEnd != width * channel)
//	{
//		for(x = xEnd/channel; x < width; x++)
//		{
//			for (k=0; k<channel; k++)
//			{
//				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
//			}
//		}
//	}
//}

//void copyOneLine_Dilate(const Fw8u* pSrc, Fw8u* pDst, int width, int channel)
//{
//	const Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
//	int xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
//	int xEnd = width * channel - (int)((Fw64u)(pd + width * channel) % 16);
//	bool bothAligned = false;
//	int x = 0, k = 0;
//	__m128i xmm00;
//
//	if((16 - (int)((Fw64u)(ps))%16)%16 == xBegin)
//		bothAligned = true;
//	else
//		bothAligned = false;
//
//	//Copy the beginning part back to the source image
//	if(xBegin != 0)
//	{
//		for(x = 0; x < xBegin/channel+1; x++)
//		{
//			for (k=0; k<channel; k++)
//			{
//				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
//			}
//		}
//	}
//
//	ps += xBegin;
//	pd += xBegin;
//
//	if(bothAligned)
//	{
//		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
//		{
//			xmm00 = _mm_load_si128( (__m128i*)(ps));
//			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
//			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
//		}
//	}
//	else
//	{
//		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
//		{
//			xmm00 = _mm_loadu_si128( (__m128i*)(ps));
//			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
//			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
//		}
//	}
//
//	//Deal with the remain part
//	if(xEnd != width * channel)
//	{
//		for(x = xEnd/channel; x < width; x++)
//		{
//			for (k=0; k<channel; k++)
//			{
//				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
//			}
//		}
//	}
//}

//Copy the whole line of data from pSrc to pDst.
void copyOneLine_Dilate(Fw8u* pSrc, Fw8u* pDst, int width, int channel)
{
	const Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
	int xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
	int xEnd = width * channel - (int)((Fw64u)(pd + width * channel) % 16);
	bool bothAligned = false;
	int x = 0, k = 0;
	__m128i xmm00;

	if((16 - (int)((Fw64u)(ps))%16)%16 == xBegin)
		bothAligned = true;
	else
		bothAligned = false;

	//Copy the beginning part back to the source image
	if(xBegin != 0)
	{
		for(x = 0; x < xBegin/channel+1; x++)
		{
			for (k=0; k<channel; k++)
			{
				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
			}
		}
	}

	ps += xBegin;
	pd += xBegin;

	if(bothAligned)
	{
		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
		{
			//_mm_prefetch((char*)(ps), _MM_HINT_T1);

			//xmm00 = _mm_load_si128( (__m128i*)(ps));
			////_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
			//_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream

			*(__m128i*)(pd) = *(__m128i *)(ps);
		}
	}
	else
	{
		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
		{
			//_mm_prefetch((char*)(ps), _MM_HINT_T1);

			xmm00 = _mm_loadu_si128( (__m128i*)(ps));
			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
		}
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		for(x = xEnd/channel; x < width; x++)
		{
			for (k=0; k<channel; k++)
			{
				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
			}
		}
	}
}

void copyOneLine_Dilate(Fw8u* pSrc, Fw8u* pDst, int width, int channel, int jumpStep)
{
	const Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
	int xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
	int xEnd = width * channel - (int)((Fw64u)(pd + width * channel) % 16);
	bool bothAligned = false;
	int x = 0, k = 0;
	__m128i xmm00;

	if((16 - (int)((Fw64u)(ps))%16)%16 == xBegin)
		bothAligned = true;
	else
		bothAligned = false;

	//Copy the beginning part back to the source image
	//if(xBegin != 0)
	//{
	//	for(x = 0; x < xBegin/channel+1; x++)
	//	{
	//		for (k=0; k<channel; k++)
	//		{
	//			*(pDst+x*channel+k)=*(pSrc+x*channel+k);
	//		}
	//	}
	//}

	//ps += xBegin;
	//pd += xBegin;

	for(x = xBegin; x < xEnd; x+=16, ps += jumpStep, pd += 16)
	{
		xmm00 = _mm_loadu_si128( (__m128i*)(ps));
		_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		for(x = xEnd/channel; x < width; x++)
		{
			for (k=0; k<channel; k++)
			{
				*(pDst+x*channel+k)=*(pSrc+x*channel+k);
			}
		}
	}
}

//This data loading function is written specificlly for In-Place operation.
//The data is from pSrc with width = jumpstep into 16 bytes (there will be some empty bytes)
//pSrc might not be 16-byte aligned, but pDst is 16-byte aligned.
//The pSrc unit is 16 bytes
//The pDst unit is jumpStep bytes
//The dmask is mask which set the remain parts in the 16 bytes SSE2 register to zero.
void loadOneLineData_C1IR(Fw8u* pSrc, int srcStep, Fw8u* pDst, int jumpStep, int xEnd, __m128i dmask)
{
	Fw8u* ps  = pSrc; 
	Fw8u* pd  = pDst;
	int x = 0;
	__m128i xmm00;
	//dmask = dmask;
	//xEnd = xEnd;

	//for(x = 0; x < xEnd; x+=16, ps += jumpStep, pd += 16)
	for(x = 0; x < srcStep; x+=jumpStep, ps += jumpStep, pd += 16)
	{
		xmm00 = _mm_loadu_si128( (__m128i*)(ps));
		xmm00 = _mm_and_si128( xmm00, dmask);
		_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
	}

	pd -= 16;

	xmm00 = _mm_setzero_si128();
	_mm_store_si128((__m128i *)(pd), xmm00);

	//Deal with the remain part
	for(x = xEnd; x < srcStep; x++, pd++)
	{
		*(pd)=*(pSrc+x);
	}
}

//Store the processed data to pDst
//pDst might not be 16-byte aligned, but pSrc is 16-byte aligned.
//The pSrc unit is jumpStep bytes
//The pDst unit is 16 bytes
void saveOneLineData_C1IR(Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, int xEnd, int jumpStep)
{
	Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
	int x = 0;
	//int xEnd = width - (int)((Fw64u)(pd + width) % 16);
	__m128i xmm00;

	for(x = 0; x < srcStep-16; x+=16, ps += 16, pd += jumpStep)
	{
		xmm00 = _mm_load_si128( (__m128i*)(ps));
		_mm_storeu_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
	}

	ps -= 16;
	pd -= jumpStep;

	//Deal with the remain part
	for(x = xEnd; x < dstStep; x++)
	{
		*(pDst+x)=*(pSrc+x);
	}
}

//shift the data one line up
void shiftUpOneLine(Fw8u** bufferImage, /*int width, */int height/*, int channel*/)
{
	Fw8u* tmp = bufferImage[0];

	for(int i = 1; i < height; i++)
	{
		bufferImage[i-1] = bufferImage[i];
	}

	bufferImage[height-1] = tmp;
}

//void bufferDilate_C1IR_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
//					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
//					 __m128i rmask, FwiPoint anchor, int channel, int yBegin, int yEnd)
//{
//	channel = 1;
//	int x = 0, y = 0, z = 0, m = 0;
//	const Fw8u* ps = pSrc;
//		  Fw8u* pd = pDst;
//		  Fw8u* pMask = pMask_pad;
//	const Fw8u* ps2 = pSrc;		//temp pointer
//	const Fw8u* ps3 = pSrc;		//temp pointer
//	const Fw8u* pd3 = pSrc;		//temp pointer
//	__m128i xmm00;	//result
//	__m128i xmm01;	//data
//	__m128i xmm02;	//mask
//	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
//	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;
//
//	pSrc += (anchor.y * srcStep + anchor.x * channel);
//
//	//use reference code to deal with the remain part
//	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
//			maskSize, anchor, channel, mEnd + jumpStep,
//			roiSize.width, yBegin, yEnd);
//
//	pSrc -= (anchor.y * srcStep + anchor.x * channel);
//
//	//Create a mask just for save the dilation result to the pDst
//	/*
//	For example, if the mask size is 5x3(w x h), then the result mask will be 100001000010000x.
//	If the mask size is 7x5 (w x h), then the result mask will be 10000001000000xx.
//	*/
//	//Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
//	//initResultMask_C1R(resultMask, pMask_pad_width, maskSize);
//	//__m128i rmask = *(__m128i*)(resultMask);
//
//	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
//	{
//		ps = pSrc;  pd = pDst;
//
//		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
//		{
//			ps3 = ps; pd3 = pd;
//
//			for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
//			{
//				pMask = pMask_pad;
//				ps2 = ps3;
//
//				xmm01 = _mm_loadu_si128((__m128i*)ps3);				//load data
//				xmm02 = *(__m128i*)(pMask);							//load mask
//				xmm00 = _mm_and_si128(xmm01, xmm02);				//masking
//
//				ps3 += srcStep; pMask += pMask_pad_width;
//
//				//Use a temporary buffer, load all maskSize.height data into the memory,
//				//then use shift (rather than loadu) to process the data.
//				for(z = 1; z < maskSize.height; z++, ps3 += srcStep, pMask += pMask_pad_width)	//16 might change
//				{
//					xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
//					xmm02 = *(__m128i*)(pMask);						//load mask
//					xmm01 = _mm_and_si128(xmm01, xmm02);			//masking
//					xmm00 = _mm_max_epu8(xmm00, xmm01);				//find the max of rows
//				}
//
//				//find the max of the row max
//				xmm01 = xmm00;
//
//				for(z = 1; z < maskSize.width; z++)
//				{
//					xmm01 = _mm_srli_si128(xmm01, 1);
//					xmm00 = _mm_max_epu8(xmm00, xmm01);
//				}
//
//				xmm00 = _mm_and_si128(xmm00, rmask);
//				xmm01 = _mm_loadu_si128((__m128i*)pd3);
//				xmm00 = _mm_or_si128(xmm01, xmm00);
//				_mm_storeu_si128((__m128i *)(pd3), xmm00);
//
//				ps3 = ps2;
//			}
//		}
//	}
//
//	//fwFree(resultMask);
//}

void setBufferLineZero(Fw8u* pDst, int dstStep, int channel)
{
	Fw8u* pd = pDst;
	int xEnd = dstStep - (int)((Fw64u)(pDst + dstStep) % 16);
	int x = 0/*, k = 0*/;
	//__m128i xmm00 = _mm_setzero_si128();
	channel = channel;

	for(x = 0; x < xEnd; x+=16, pd += 16)
	{
		//_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
		*(__m128i *)(pd) = _mm_setzero_si128();
	}

	//Deal with the remain part
	if(xEnd != dstStep)
	{
		for(x = xEnd; x < dstStep; x++)
		{
			//for (k=0; k<channel; k++)
			//{
				*(pDst+x)= 0;
			//}
		}
	}
}

__m128i loadDatabyShift_C1(__m128i x, __m128i y)
{
	__m128i xmm00 = _mm_srli_si128(x, 1);
	__m128i xmm01 = _mm_slli_si128(y, 15);

	return _mm_or_si128(xmm00, xmm01);
}

/*
pSrc and pDst are both 16-byte aligned. The pSrc are 0-padded to fill a 16-byte SSE2 register.

bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], bufferResultWidth, maskSize, rmask, channel, xmm02, xmm03, xEnd);
*/
//void bufferDilate_C1IR_16(Fw8u** pSrc, Fw8u* pDst, int dstStep, FwiSize maskSize,
//						  __m128i rmask, int channel, __m128i* xmm02, __m128i* xmm03/*0, __m128i* xmm031*/, 
//						  int mEnd, int jumpStep)
//{
//	channel = 1;
//	int x = 0, y = 0, z = 0, m = 0;
//	const Fw8u* ps = pSrc[0];
//		  Fw8u* pd = pDst;
//	const Fw8u* ps2 = pSrc[0];		//temp pointer
//	const Fw8u* ps3 = pSrc[0];		//temp pointer
//		  Fw8u* pd3 = pDst;		//temp pointer
//	__m128i xmm00;					//result
//	__m128i xmm01;					//data
//	//__m128i xmm04;
//	jumpStep = jumpStep;
//	//Fw8u* tempAddr;
//
//	//Initialize the pDst
//	setBufferLineZero(pDst, dstStep, channel);
//	xmm00 = _mm_setzero_si128();
//
//	ps = pSrc[0];  pd = pDst;
//
//	for(m = 0; m <= mEnd; m += 16, ps += 16, pd += 16)
//	{
//		ps3 = ps; pd3 = pd;
//
//		_mm_prefetch((char*)(pSrc[y]+m), _MM_HINT_T1);
//
//		for(y = 0; y < maskSize.height; y++)
//		{
//			//tempAddr = pSrc[y]+m;
//			//*(xmm03+y) = *(__m128i*)(tempAddr);				//load data from bufferImage
//			*(xmm03+y) = *(__m128i*)(pSrc[y]+m);
//		}
//
//		for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
//		{
//			ps2 = ps3;
//
//			//Use a temporary buffer, load all maskSize.height data into the memory,
//			//then use shift (rather than loadu) to process the data.
//			for(z = 0; z < maskSize.height; z++)				//16 might change
//			{
//				if(x-m == 0)
//					xmm01 = xmm03[z];
//				else if(x-m == 1)
//					xmm01 = _mm_slli_si128(xmm03[z], 1);
//				else if(x-m == 2)
//					xmm01 = _mm_slli_si128(xmm03[z], 2);
//				else if(x-m == 3)
//					xmm01 = _mm_slli_si128(xmm03[z], 3);
//				else if(x-m == 4)
//					xmm01 = _mm_slli_si128(xmm03[z], 4);
//				else if(x-m == 5)
//					xmm01 = _mm_slli_si128(xmm03[z], 5);
//				else if(x-m == 6)
//					xmm01 = _mm_slli_si128(xmm03[z], 6);
//				else if(x-m == 7)
//					xmm01 = _mm_slli_si128(xmm03[z], 7);
//				else if(x-m == 8)
//					xmm01 = _mm_slli_si128(xmm03[z], 8);
//				else if(x-m == 9)
//					xmm01 = _mm_slli_si128(xmm03[z], 9);
//				else if(x-m == 10)
//					xmm01 = _mm_slli_si128(xmm03[z], 10);
//				else if(x-m == 11)
//					xmm01 = _mm_slli_si128(xmm03[z], 11);
//				else if(x-m == 12)
//					xmm01 = _mm_slli_si128(xmm03[z], 12);
//				else if(x-m == 13)
//					xmm01 = _mm_slli_si128(xmm03[z], 13);
//				else if(x-m == 14)
//					xmm01 = _mm_slli_si128(xmm03[z], 14);
//				else
//					xmm01 = _mm_slli_si128(xmm03[z],15);
//
//				xmm01 = _mm_and_si128(xmm01, *(xmm02 + z));		//masking
//				xmm00 = _mm_max_epu8(xmm00, xmm01);				//find the max of rows
//			}
//
//			//find the max of the row max
//			xmm01 = xmm00;
//
//			for(z = 1; z < maskSize.width; z++)
//			{
//				xmm01 = _mm_srli_si128(xmm01, 1);
//				xmm00 = _mm_max_epu8(xmm00, xmm01);
//			}
//
//			xmm00 = _mm_and_si128(xmm00, rmask);
//			xmm01 = *(__m128i*)(pd);							//load already processed data to avoid of overwritten
//			//xmm01 = _mm_loadu_si128((__m128i*)pd3);
//			xmm00 = _mm_or_si128(xmm01, xmm00);
//			_mm_store_si128((__m128i *)(pd), xmm00);
//			//_mm_storeu_si128((__m128i *)(pd3), xmm00);
//
//			ps3 = ps2;
//		}
//	}
//}

void bufferDilate_C1IR_16(Fw8u** pSrc, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 __m128i rmask, int channel/*, int yBegin, int yEnd*/)
{
	channel = 1;
	int x = 0, /*y = 0,*/ z = 0, m = 0;
	const Fw8u* ps = pSrc[0];
		  Fw8u* pd = pDst;
	const Fw8u* ps2 = pSrc[0];		//temp pointer
	const Fw8u* ps3 = pSrc[0];		//temp pointer
	const Fw8u* pd3 = pDst;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i* xmm02 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize	.height);	//mask

	//Initialize the mask
	for(x = 0; x < maskSize.height; x++, pMask_pad += pMask_pad_width)
	{
		xmm02[x] = *(__m128i*)(pMask_pad);
	}

	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	//Initialize the pDst
	setBufferLineZero(pDst, dstStep, channel);

	//ps = pSrc[0];  pd = pDst;

	for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
	{
		ps3 = ps; pd3 = pd;

		//_mm_prefetch((char*)(ps3), _MM_HINT_T1);
		//_mm_prefetch((char*)(pd3), _MM_HINT_T2);

		for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
		{
			ps2 = ps3;

			xmm01 = _mm_loadu_si128((__m128i*)ps3);				//load data
			xmm00 = _mm_and_si128(xmm01, xmm02[0]);				//masking

			//Use a temporary buffer, load all maskSize.height data into the memory,
			//then use shift (rather than loadu) to process the data.
			for(z = 1; z < maskSize.height; z++)				//16 might change
			{
				ps3 = *(pSrc + z) + x;							//Here, I assume Fw8u data only
				xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
				xmm01 = _mm_and_si128(xmm01, *(xmm02 + z));		//masking
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

	fwFree(xmm02);
}

void bufferDilate_C3IR_16(Fw8u** pSrc, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 __m128i rmask, int channel/*, int yBegin, int yEnd*/)
{
	channel = 3;
	int x = 0, /*y = 0,*/ z = 0, m = 0;
	const Fw8u* ps = pSrc[0];
		  Fw8u* pd = pDst;
	const Fw8u* ps2 = pSrc[0];		//temp pointer
	const Fw8u* ps3 = pSrc[0];		//temp pointer
	const Fw8u* pd3 = pDst;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i* xmm02 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize	.height);	//mask
	//__m128i* xmm03 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height * 2);//data

	//Initialize the mask
	for(x = 0; x < maskSize.height; x++, pMask_pad += pMask_pad_width)
	{
		xmm02[x] = *(__m128i*)(pMask_pad);
	}

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width*channel - jumpStep) / jumpStep) * jumpStep;

	//Initialize the pDst
	setBufferLineZero(pDst, dstStep, channel);

	//ps = pSrc[0];  pd = pDst;

	for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
	{
		ps3 = ps; pd3 = pd;

		//for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
		for(x = m; x < m + maskSize.width; x++, ps3+=channel, pd3+=channel)
		{
			ps2 = ps3;

			xmm01 = _mm_loadu_si128((__m128i*)ps3);				//load data
			xmm00 = _mm_and_si128(xmm01, xmm02[0]);				//masking

			//Use a temporary buffer, load all maskSize.height data into the memory,
			//then use shift (rather than loadu) to process the data.
			for(z = 1; z < maskSize.height; z++)				//16 might change
			{
				//ps3 = *(pSrc + z) + x*channel;					//Here, I assume Fw8u data only
				ps3 = *(pSrc + z) + m + (x-m)*channel;					//Here, I assume Fw8u data only
				xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
				xmm01 = _mm_and_si128(xmm01, *(xmm02 + z));		//masking
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

	fwFree(xmm02);
}

void bufferDilate_AC4IR_16(Fw8u** pSrc, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 __m128i rmask, int channel/*, int yBegin, int yEnd*/)
{
	channel = 4;
	int x = 0, /*y = 0,*/ z = 0, m = 0;
	const Fw8u* ps = pSrc[0];
		  Fw8u* pd = pDst;
	const Fw8u* ps2 = pSrc[0];		//temp pointer
	const Fw8u* ps3 = pSrc[0];		//temp pointer
	const Fw8u* pd3 = pDst;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i* xmm02 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize	.height);	//mask
	//__m128i* xmm03 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height * 2);//data

	//Initialize the mask
	for(x = 0; x < maskSize.height; x++, pMask_pad += pMask_pad_width)
	{
		xmm02[x] = *(__m128i*)(pMask_pad);
	}

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width*channel - jumpStep) / jumpStep) * jumpStep;

	//Initialize the pDst
	setBufferLineZero(pDst, dstStep, channel);

	//ps = pSrc[0];  pd = pDst;

	for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
	{
		ps3 = ps; pd3 = pd;

		//for(x = m; x < m + maskSize.width; x++, ps3++, pd3++)
		for(x = m; x < m + maskSize.width; x++, ps3+=channel, pd3+=channel)
		{
			ps2 = ps3;

			xmm01 = _mm_loadu_si128((__m128i*)ps3);				//load data
			xmm00 = _mm_and_si128(xmm01, xmm02[0]);				//masking

			//Use a temporary buffer, load all maskSize.height data into the memory,
			//then use shift (rather than loadu) to process the data.
			for(z = 1; z < maskSize.height; z++)				//16 might change
			{
				//ps3 = *(pSrc + z) + x*channel;					//Here, I assume Fw8u data only
				ps3 = *(pSrc + z) + m + (x-m)*channel;					//Here, I assume Fw8u data only
				xmm01 = _mm_loadu_si128((__m128i*)ps3);			//load data
				xmm01 = _mm_and_si128(xmm01, *(xmm02 + z));		//masking
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

	fwFree(xmm02);
}

//Dilate (C1R), maskSize.width <= 16.
void dilate_C1R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 1;
	int x = 0, y = 0, z = 0, m = 0;
	const Fw8u* ps = pSrc;
		  Fw8u* pd = pDst;
		  Fw8u* pMask = pMask_pad;
	const Fw8u* ps2 = pSrc;		//temp pointer
	const Fw8u* ps3 = pSrc;		//temp pointer
	const Fw8u* pd3 = pSrc;		//temp pointer
	__m128i xmm00;	//result
	__m128i xmm01;	//data
	__m128i xmm02;	//mask
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, mEnd + jumpStep,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	/*
	For example, if the mask size is 5x3(w x h), then the result mask will be 100001000010000x.
	If the mask size is 7x5 (w x h), then the result mask will be 10000001000000xx.
	*/
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C1R(resultMask, pMask_pad_width, maskSize);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
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
void dilate_C1R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 1;
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
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, mEnd + jumpStep,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

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

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;	/*pm = pMask_pad;*/

		//for(m = 0; m < roiSize.width - maskSize.width; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
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

//Dilate (C1IR), maskSize.width <= 16.
/*
To do the In-Place operation with high speed, the main idea is to create 2 buffers:
1. bufferImage: the data buffer which temporary store the data from the pSrc
2. bufferResult: the result buffer which temporary store the dilation results
Then copy some data from pSrc to bufferImage, do the morphology process, and save the result to bufferResult.
To implement the in-place operation, the processed result will not be directly saved back to pSrc until I am sure
a block of pSrc will not be used any more.
To gurantee it, I will load one line of data, save it to the bottom line of bufferImage. Then shift this line up,
and keep this process till the whole bufferImage is filled up.
Then I will do the morphology process, save the result to the bottom line of bufferResult. Shift this line up,
load another data line to bufferImage, process, and save the result to the bottom line of bufferResult till the
bufferResult is filled up.
Now, I will copy the top line of bufferResult back to pSrc where I am sure it will not touched again.

The main idea of in-place operation is kind like the stack FIFO. But I am put in a line of data, rather than single data.
And the line shift operation is also very important. Because the data and the result are always save to the bottom
line of these 2 buffers.
*/

/*
Algorithm update:
1. To avoid of using loadu, I will create the bufferImage whose size is larger than roiSize.width. In the 
center part, the bufferImage is 16 byte aligned (might be some extra byte empty to make a whole 16 byte SSE2
register). But ths speed will increase about 25%
2. Do the same trick to bufferResult.
*/

/*
The last update:
1. Compute the jumpStep;
2. Load all jumpStep of data into bufferImage which only store the 16-byte aligned data
3. Dilation process the data in the bufferImage. Whenever I need some new data, I use SHIFT rather than LOADU to
obtain the data;
4. For the remain data, I will use Ref code to process it. The data will be obtain from pSrc, rather than bufferImage;
5. For the last (maskSize.height - anchor.y) lines of results, do not save them directly. I should save them back to 
pSrc after the dilation process is done.
*/
/*
Following function runs fast, but the result is not correct. The major problem is the:
1. To make the algorithm faster, we have to load the data efficiently;
2. When you want to load data in the next "row", which I did not put in the same 16 byte SSE2 register.
I tried to use "SHIFT" operation, but I just cannot make it.
3. The original loadu algorithm can give me a correct result but slow performance.
*/
//void dilate_C1IR_SSE2_16(Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
//					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
//					 FwiPoint anchor, int channel, int yBegin, int yEnd)
//{
//	channel = 1;
//	int x = 0, y = 0;
//	dstStep = dstStep;
//	pDst = pDst;
//	//move the pSrc pointer to the top-left position of the ROI.
//	pSrc -= (anchor.y * srcStep + anchor.x * channel);
//	pMask_reflect = pMask_reflect;
//
//	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
//	int numJump = (int)((roiSize.width + maskSize.width - 1) * 1.0 / jumpStep);
//	int xEnd = ((int)(roiSize.width + maskSize.width - 1) / jumpStep) * jumpStep;
//	int xRemainPixel = (roiSize.width + maskSize.width - 1) - xEnd;
//
//	__m128i* xmm02 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height);	//mask
//	//__m128i* xmm030 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height);	//data left
//	//__m128i* xmm031 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height);	//data right
//	__m128i* xmm03 = (__m128i*)fwMalloc(sizeof(__m128i) * maskSize.height);	//data
//
//	//Initialize the mask register
//	for(x = 0; x < maskSize.height; x++, pMask_pad += pMask_pad_width)
//	{
//		xmm02[x] = *(__m128i*)(pMask_pad);			//load padded mask data to SSE2 registers
//	}
//
//	//mask to obtain the data
//	Fw8u* dataMask = (Fw8u*)fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
//	initDataMask_C1IR(dataMask, pMask_pad_width, jumpStep);
//	__m128i dmask = *(__m128i*)(dataMask);
//
//	//mask to obtain the final result
//	Fw8u* resultMask = (Fw8u*)fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
//	initResultMask_C1R(resultMask, pMask_pad_width, maskSize);
//	__m128i rmask = *(__m128i*)(resultMask);
//
//	//1. create a temporary bufferImage
//	Fw8u** bufferImage = (Fw8u**) fwMalloc(sizeof(int) * maskSize.height);
//	//int bufferImageWidth = ((int)((roiSize.width + maskSize.width - 1)  * 1.0 * channel / jumpStep + 1)) * 16;
//	//int bufferImageWidth = ((int)((roiSize.width + maskSize.width - 1)  * 1.0 * channel / jumpStep)) * 16;
//	int bufferImageWidth = (numJump+1) * 16;
//
//	for(x = 0; x < maskSize.height; x++)
//	{
//		bufferImage[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
//	}
//
//	//2. create a temporary bufferResult
//	int bufferResultHeight = anchor.y + 1;
//	int bufferResultWidth = 16 * numJump + xRemainPixel;
//	//The remained pixels will be processed separately.
//
//	Fw8u** bufferResult = (Fw8u**) fwMalloc(sizeof(int) * bufferResultHeight);
//
//	for(x = 0; x < bufferResultHeight; x++)
//	{
//		bufferResult[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferResultWidth * channel);
//	}
//
//	for(y = yBegin; y < yBegin + maskSize.height - 1; y++, pSrc += srcStep)
//	{
//		//copy the data to the bottom of the bufferImage.
//		loadOneLineData_C1IR(pSrc, (roiSize.width + maskSize.width - 1),
//			bufferImage[maskSize.height - 1], jumpStep, xEnd, dmask);
//
//		//printout(pSrc, roiSize.width + maskSize.width - 1, 1, 1);
//		//printout(bufferImage[maskSize.height - 1], bufferImageWidth, 1, 1);
//
//		//Move one line up
//		shiftUpOneLine(bufferImage, maskSize.height);
//	}
//
//	for(y = yBegin + maskSize.height; y < yBegin + maskSize.height + bufferResultHeight - 1; y++, pSrc += srcStep)
//	{
//		loadOneLineData_C1IR(pSrc, (roiSize.width + maskSize.width - 1),
//			bufferImage[maskSize.height - 1], jumpStep, xEnd, dmask);
//
//		//dilation SSE2
//		bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], 
//			bufferResultWidth, maskSize, rmask, channel, xmm02, xmm03/*0, xmm031*/, xEnd, jumpStep);
//
//		//printout(bufferImage, bufferImageWidth, 1, 1);
//		//printout(bufferResult[bufferResultHeight-1], bufferResultWidth, 1, 1);
//		//dilation ref
//		//dilate_ref_C1IR(bufferImage, bufferResult[bufferResultHeight-1], roiSize.width, pMask_reflect, maskSize, anchor, xEnd + jumpStep, roiSize.width, y, y);
//
//		//printout(pSrc, roiSize.width + maskSize.width - 1, 1, 1);
//		//printout(bufferImage[maskSize.height - 1], bufferImageWidth, 1, 1);
//
//		shiftUpOneLine(bufferResult, bufferResultHeight);
//		shiftUpOneLine(bufferImage, maskSize.height);
//	}
//
//	loadOneLineData_C1IR(pSrc, (roiSize.width + maskSize.width - 1),
//		bufferImage[maskSize.height - 1], jumpStep, xEnd, dmask);
//
//	//dilate SSE2
//	//printout(bufferImage, bufferImageWidth, maskSize.height, 1);
//	bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], 
//		roiSize.width * channel, maskSize, rmask, channel, xmm02, xmm03/*0, xmm031*/, xEnd, jumpStep);
//	//dilation ref
//	//dilate_ref_C1IR(bufferImage, bufferResult[bufferResultHeight-1], roiSize.width, pMask_reflect, maskSize, anchor, xEnd + jumpStep, roiSize.width, y, y);
//
//	int backOffset = srcStep * (maskSize.height - 1 + bufferResultHeight - 1)
//						- (anchor.y * srcStep + anchor.x * channel);
//
//	for(y = yBegin + maskSize.height + bufferResultHeight; y < yEnd + maskSize.height + anchor.y; y++, pSrc += srcStep)
//	{
//		//copy the result to the pSrc where the data will not be used any more.
//		//printout(bufferResult[0], bufferResultWidth, 1, 1);
//
//		saveOneLineData_C1IR(bufferResult[0], bufferImageWidth, pSrc - backOffset, bufferResultWidth, xEnd, jumpStep);
//		//printout(pSrc-backOffset, roiSize.width, 1, 1);
//
//		//printout(bufferResult, bufferResultWidth, bufferResultHeight, 1);
//
//		shiftUpOneLine(bufferResult, bufferResultHeight);
//		shiftUpOneLine(bufferImage, maskSize.height);
//		//copy the data from the pSrc to the bufferImage.
//		loadOneLineData_C1IR(pSrc + srcStep, (roiSize.width + maskSize.width - 1),
//			bufferImage[maskSize.height - 1], jumpStep, xEnd, dmask);
//
//		//dilation SSE2
//		bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1],
//			roiSize.width * channel, maskSize, rmask, channel, xmm02, xmm03/*0, xmm031*/, xEnd, jumpStep);
//		//dilation ref
//		//dilate_ref_C1IR(bufferImage, bufferResult[bufferResultHeight-1], roiSize.width, pMask_reflect, maskSize, anchor, xEnd + jumpStep, roiSize.width, y, y);
//	}
//
//	for(y = 0; y < anchor.y; y++, pSrc += srcStep)
//	{
//		saveOneLineData_C1IR(bufferResult[0], bufferImageWidth, pSrc - backOffset, bufferResultWidth, xEnd, jumpStep);
//		shiftUpOneLine(bufferResult, bufferResultHeight);
//	}
//
//	fwFree(resultMask);
//
//	for(x = 0; x < bufferResultHeight; x++)
//	{
//		fwFree(bufferResult[x]);
//	}
//
//	for(x = 0; x < maskSize.height; x++)
//	{
//		fwFree(bufferImage[x]);
//	}
//
//	fwFree(xmm02);
//	//fwFree(xmm030);
//	//fwFree(xmm031);
//	fwFree(xmm03);
//	fwFree(dataMask);
//}

void dilate_C1IR_SSE2_16(Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd,
					 Fw8u* remainBottomLines, Fw8u* remainTopLines)
{
	channel = 1;
	int x = 0, y = 0;
	dstStep = dstStep;
	pDst = pDst;
	pSrc -= (anchor.y * srcStep + anchor.x * channel);
	remainTopLines = remainTopLines;
	remainBottomLines = remainBottomLines;

	//use reference code to deal with the remain part
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C1R(resultMask, pMask_pad_width, maskSize);
	__m128i rmask = *(__m128i*)(resultMask);

	//1. create a temporary bufferImage whose size is (roiSize.width + maskSize.width) * channel * maskSize.height to store data to be processed;
	Fw8u** bufferImage = (Fw8u**) fwMalloc(sizeof(int) * maskSize.height);
	int bufferImageWidth = roiSize.width + maskSize.width - 1;

	for(x = 0; x < maskSize.height; x++)
	{
		bufferImage[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	//2. create a temporary bufferResult whose size is roiSize.width * channel * anchor.y to store result data;
	int bufferResultHeight = anchor.y + 1;
	Fw8u** bufferResult = (Fw8u**) fwMalloc(sizeof(int) * bufferResultHeight);

	for(x = 0; x < bufferResultHeight; x++)
	{
		bufferResult[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	for(y = yBegin; y < yBegin + maskSize.height - 1; y++)
	{
		//copy the data to the bottom of the bufferImage.
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);

		//Move one line up
		shiftUpOneLine(bufferImage, maskSize.height);
		pSrc = (Fw8u *)((Fw8u*) pSrc + srcStep);
	}

	for(y = yBegin + maskSize.height; y < yBegin + maskSize.height + bufferResultHeight - 1; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
	}

	copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
	//dilate SSE2
	bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
	//dilation ref
	dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

	int backOffset = srcStep * (maskSize.height - 1 + bufferResultHeight - 1)
						- (anchor.y * srcStep + anchor.x * channel);

	for(y = yBegin + maskSize.height + bufferResultHeight; y < yEnd + maskSize.height + anchor.y-1; y++, pSrc += srcStep)
	{
		//copy the result to the pSrc where the data will not be used any more.
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
		//copy the data from the pSrc to the bufferImage.
		copyOneLine_Dilate(pSrc + srcStep, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_C1IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);
	}

	for(y = 0; y < anchor.y; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
	}

	fwFree(resultMask);

	for(x = 0; x < bufferResultHeight; x++)
	{
		fwFree(bufferResult[x]);
	}
	
	fwFree(bufferResult);

	for(x = 0; x < maskSize.height; x++)
	{
		fwFree(bufferImage[x]);
	}

	fwFree(bufferImage);

	return;
}

void dilate_C3IR_SSE2_16(Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 3;
	int x = 0, y = 0;
	dstStep = dstStep;
	pDst = pDst;
	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//use reference code to deal with the remain part
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C3R(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	//1. create a temporary bufferImage whose size is (roiSize.width + maskSize.width) * channel * maskSize.height to store data to be processed;
	Fw8u** bufferImage = (Fw8u**) fwMalloc(sizeof(int) * maskSize.height);
	int bufferImageWidth = roiSize.width + maskSize.width - 1;

	for(x = 0; x < maskSize.height; x++)
	{
		bufferImage[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	//2. create a temporary bufferResult whose size is roiSize.width * channel * anchor.y to store result data;
	int bufferResultHeight = anchor.y + 1;
	Fw8u** bufferResult = (Fw8u**) fwMalloc(sizeof(int) * bufferResultHeight);

	for(x = 0; x < bufferResultHeight; x++)
	{
		bufferResult[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	for(y = yBegin; y < yBegin + maskSize.height - 1; y++, pSrc += srcStep)
	{
		//copy the data to the bottom of the bufferImage.
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);

		//Move one line up
		shiftUpOneLine(bufferImage, maskSize.height);
		//pSrc = (Fw8u *)((Fw8u*) pSrc + srcStep);
	}

	for(y = yBegin + maskSize.height; y < yBegin + maskSize.height + bufferResultHeight - 1; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_C3IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
	}

	copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
	//dilate SSE2
	bufferDilate_C3IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
	//dilation ref
	dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

	int backOffset = srcStep * (maskSize.height - 1 + bufferResultHeight - 1)
						- (anchor.y * srcStep + anchor.x * channel);

	for(y = yBegin + maskSize.height + bufferResultHeight; y < yEnd + maskSize.height + anchor.y; y++, pSrc += srcStep)
	{
		//copy the result to the pSrc where the data will not be used any more.
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
		//copy the data from the pSrc to the bufferImage.
		copyOneLine_Dilate(pSrc + srcStep, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_C3IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);
	}

	for(y = 0; y < anchor.y; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
	}

	fwFree(resultMask);

	for(x = 0; x < bufferResultHeight; x++)
	{
		fwFree(bufferResult[x]);
	}
	fwFree(bufferResult);

	for(x = 0; x < maskSize.height; x++)
	{
		fwFree(bufferImage[x]);
	}
	fwFree(bufferImage);
	
	return;
}

void dilate_AC4IR_SSE2_16(Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
					 FwiPoint anchor, int channel, int yBegin, int yEnd)
{
	channel = 4;
	int x = 0, y = 0;
	dstStep = dstStep;
	pDst = pDst;
	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//use reference code to deal with the remain part
	int jumpStep = maskSize.width * (pMask_pad_width / maskSize.width);
	int mEnd = (int)((roiSize.width - jumpStep) / jumpStep) * jumpStep;

	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C4R(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	//1. create a temporary bufferImage whose size is (roiSize.width + maskSize.width) * channel * maskSize.height to store data to be processed;
	Fw8u** bufferImage = (Fw8u**) fwMalloc(sizeof(int) * maskSize.height);
	int bufferImageWidth = roiSize.width + maskSize.width - 1;

	for(x = 0; x < maskSize.height; x++)
	{
		bufferImage[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	//2. create a temporary bufferResult whose size is roiSize.width * channel * anchor.y to store result data;
	int bufferResultHeight = anchor.y + 1;
	Fw8u** bufferResult = (Fw8u**) fwMalloc(sizeof(int) * bufferResultHeight);

	for(x = 0; x < bufferResultHeight; x++)
	{
		bufferResult[x] = (Fw8u*)fwMalloc(sizeof(Fw8u) * bufferImageWidth * channel);
	}

	for(y = yBegin; y < yBegin + maskSize.height - 1; y++, pSrc += srcStep)
	{
		//copy the data to the bottom of the bufferImage.
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);

		//Move one line up
		shiftUpOneLine(bufferImage, maskSize.height);
		//pSrc = (Fw8u *)((Fw8u*) pSrc + srcStep);
	}

	for(y = yBegin + maskSize.height; y < yBegin + maskSize.height + bufferResultHeight - 1; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_AC4IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
	}

	copyOneLine_Dilate(pSrc, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
	//dilate SSE2
	bufferDilate_AC4IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
	//dilation ref
	dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);

	int backOffset = srcStep * (maskSize.height - 1 + bufferResultHeight - 1)
						- (anchor.y * srcStep + anchor.x * channel);

	for(y = yBegin + maskSize.height + bufferResultHeight; y < yEnd + maskSize.height + anchor.y; y++, pSrc += srcStep)
	{
		//copy the result to the pSrc where the data will not be used any more.
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
		shiftUpOneLine(bufferImage, maskSize.height);
		//copy the data from the pSrc to the bufferImage.
		copyOneLine_Dilate(pSrc + srcStep, bufferImage[maskSize.height - 1], bufferImageWidth, channel);
		//dilation SSE2
		bufferDilate_AC4IR_16(bufferImage, bufferResult[bufferResultHeight - 1], roiSize.width * channel, roiSize, pMask_pad, maskSize, pMask_pad_width, rmask, channel);
		//dilation ref
		dilate_ref(bufferImage, srcStep, bufferResult[bufferResultHeight-1], roiSize.width, roiSize, pMask_reflect, maskSize, anchor, channel, mEnd + jumpStep, roiSize.width, y, y);
	}

	for(y = 0; y < anchor.y; y++, pSrc += srcStep)
	{
		copyOneLine_Dilate(bufferResult[0], pSrc - backOffset, roiSize.width, channel);
		shiftUpOneLine(bufferResult, bufferResultHeight);
	}

	fwFree(resultMask);

	for(x = 0; x < bufferResultHeight; x++)
	{
		fwFree(bufferResult[x]);
	}
	fwFree(bufferResult);

	for(x = 0; x < maskSize.height; x++)
	{
		fwFree(bufferImage[x]);
	}
	fwFree(bufferImage);

	return;
}

//Dilate (C3R), maskSize.width <= 5.
void dilate_C3R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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
	__m128i xmm01;	//data
	__m128i xmm02;	//mask

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C3R(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
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

//Dilate (C4R), maskSize.width <= 4.
void dilate_C4R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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
	__m128i xmm01;	//data
	__m128i xmm02;	//mask

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C4R(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				_mm_prefetch((char*)(ps3), _MM_HINT_T1);

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

//TODO: add the mask to keep alpha channel data.
//Dilate (AC4R), maskSize.width <= 4.
void dilate_AC4R_SSE2_16(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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
	__m128i xmm01;	//data
	__m128i xmm02;	//mask

	int jumpStep = maskSize.width * (int)((pMask_pad_width / maskSize.width));
	int mEnd = (int)((roiSize.width * channel - jumpStep) / jumpStep) * jumpStep;
	pMask_reflect = pMask_reflect;

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

	//Create a mask just for save the dilation result to the pDst
	Fw8u* resultMask = (Fw8u*) fwMalloc(sizeof(Fw8u) * (pMask_pad_width * 1));
	initResultMask_C4R(resultMask, pMask_pad_width, maskSize, channel);
	__m128i rmask = *(__m128i*)(resultMask);

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		{
			ps3 = ps; pd3 = pd;

			for(x = m; x < m + maskSize.width; x++, ps3+= channel, pd3+= channel)
			{
				pMask = pMask_pad;
				ps2 = ps3;

				_mm_prefetch((char*)(ps3), _MM_HINT_T1);

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

//Dilate (C3R), 6 <= maskSize.width <= 10
void dilate_C3R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

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

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
		//for(m = 0; m <= mEnd; m+=channel, ps += channel, pd += channel)
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

//Dilate (C4R), 4 < maskSize.width <= 8
void dilate_C4R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

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

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
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
				//xmm020 = _mm_srli_si128(xmm010, 15);				//shifting
				//xmm021 = _mm_slli_si128(xmm011, 1);					//shifting
				//xmm020 = _mm_or_si128(xmm020, xmm021);				//Obtain the red byte back
				xmm00 = _mm_max_epu8(xmm010, xmm011);				//find the max of rows

				/*
				Because of the maskSize.width >= 6, so the whole mask (3 channels) will go over the 16 byte
				register. For example: if the maskSize.width = 7, then the original image (one line) will be:
					|							     |								   |
				...	|r g b r g b r g b r g b r g b r | g b r g b r g b r g b r g b ? ? |...
					|							   | |								   |
	 											   | 16								  16
												   ^
												   this byte has to be shift to the next register to gurantee
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
					//xmm020 = _mm_srli_si128(xmm010, 15);			//shifting
					//xmm021 = _mm_slli_si128(xmm011, 1);				//shifting
					//xmm020 = _mm_or_si128(xmm020, xmm021);			//Obtain the red byte back
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

//Dilate (AC4R), 4 < maskSize.width <= 8
void dilate_AC4R_SSE2_32(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize,
					 const Fw8u* pMask_reflect, Fw8u* pMask_pad, FwiSize maskSize, int pMask_pad_width,
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

	//use reference code to deal with the remain part
	dilate_ref(pSrc, srcStep, pDst, dstStep, roiSize, pMask_reflect,
			maskSize, anchor, channel, (mEnd + jumpStep) / channel,
			roiSize.width, yBegin, yEnd);

	pSrc -= (anchor.y * srcStep + anchor.x * channel);

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

	for(y = 0; y <= yEnd - yBegin; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps = pSrc;  pd = pDst;

		for(m = 0; m <= mEnd; m+=jumpStep, ps += jumpStep, pd += jumpStep)
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
				//xmm020 = _mm_srli_si128(xmm010, 15);				//shifting
				//xmm021 = _mm_slli_si128(xmm011, 1);					//shifting
				//xmm020 = _mm_or_si128(xmm020, xmm021);				//Obtain the red byte back
				xmm00 = _mm_max_epu8(xmm010, xmm011);				//find the max of rows

				/*
				Because of the maskSize.width >= 6, so the whole mask (3 channels) will go over the 16 byte
				register. For example: if the maskSize.width = 7, then the original image (one line) will be:
					|							     |								   |
				...	|r g b r g b r g b r g b r g b r | g b r g b r g b r g b r g b ? ? |...
					|							   | |								   |
	 											   | 16								  16
												   ^
												   this byte has to be shift to the next register to gurantee
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
					//xmm020 = _mm_srli_si128(xmm010, 15);			//shifting
					//xmm021 = _mm_slli_si128(xmm011, 1);				//shifting
					//xmm020 = _mm_or_si128(xmm020, xmm021);			//Obtain the red byte back
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

void dilate_ref(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize,
					   Fw8u* pMask_reflect, FwiSize maskSize, FwiPoint anchor, int channel)
{
//Dilate with specified square mask.
	for(int y = 0; y < roiSize.height; y++)
	{
		for(int x = 0; x < roiSize.width; x++)
		{
			for (int k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					*(pDst+y*dstStep+x*channel+k) = *(pSrc+y*srcStep+x*channel+k);
				}
				else
				{
					*(pDst+y*dstStep+x*channel+k) = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might does not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*(pSrc+(y+n)*srcStep+(x+m)*channel+k) > *(pDst+y*dstStep+x*channel+k)))
								{
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+(y+n)*srcStep+(x+m)*channel+k);
								}
						}
					}
				}
			}
		}
	}
}

//maskSize.width <= 16
//Circulaly pad the mask to pMask_pad, then in the dilation process, I don't need to load the mask anymore.
//In this function, the mask is padding through the whole line.
void padMask_16_C1R(const Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
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
void padMask_32_C1R(const Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
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

void padMask_16_C3R(const Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
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

void padMask_16_C4R(const Fw8u* pMask, Fw8u* pMask_pad, int pMask_padWidth, FwiSize maskSize)
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

void My_fwiDilate_8u_C1R(void* param)
{
	fwiDilate_8u_Not_In_Place *dilate_par = (fwiDilate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C1R(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilate_C1R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 16 && maskSize.width <= 32)
	{
		padMask_32_C1R(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilate_C1R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 32 && maskSize.width <= 64)
		pMask_pad_width = 64;
	else if (maskSize.width > 64 && maskSize.width <= 128)
		pMask_pad_width = 128;
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_C1IR(void* param)
{
	fwiDilate_8u_In_Place *dilate_par = (fwiDilate_8u_In_Place *) param;

	Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
	FwiSize maskSize = dilate_par->maskSize;
	FwiPoint anchor = dilate_par->anchor;
	Fw8u* remainBottomLines = dilate_par->remainBottomLines;
	Fw8u* remainTopLines = dilate_par->remainTopLines;

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
		padMask_16_C1R(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilate_C1IR_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, 
			pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd,
			remainBottomLines, remainTopLines);
	}
	else if (maskSize.width > 16 && maskSize.width <= 32)
	{
		padMask_32_C1R(pMask, pMask_pad, pMask_pad_width, maskSize);
		dilate_C1R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 32 && maskSize.width <= 64)
		pMask_pad_width = 64;
	else if (maskSize.width > 64 && maskSize.width <= 128)
		pMask_pad_width = 128;
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_C3R(void* param)
{
	fwiDilate_8u_Not_In_Place *dilate_par = (fwiDilate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C3R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C3R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 5 && maskSize.width <= 10)
	{
		padMask_16_C3R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C3R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 10 && maskSize.width <= 16)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_C4R(void* param)
{
	fwiDilate_8u_Not_In_Place *dilate_par = (fwiDilate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C4R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 4 && maskSize.width <= 8)
	{
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C4R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 8 && maskSize.width <= 12)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_AC4R(void* param)
{
	fwiDilate_8u_Not_In_Place *dilate_par = (fwiDilate_8u_Not_In_Place *) param;

	const Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_AC4R_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 4 && maskSize.width <= 8)
	{
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_AC4R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 8 && maskSize.width <= 12)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_C3IR(void* param)
{
	fwiDilate_8u_In_Place *dilate_par = (fwiDilate_8u_In_Place *) param;

	Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C3R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C3IR_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 5 && maskSize.width <= 10)
	{
		padMask_16_C3R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C3R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 10 && maskSize.width <= 16)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

void My_fwiDilate_8u_AC4IR(void* param)
{
	fwiDilate_8u_In_Place *dilate_par = (fwiDilate_8u_In_Place *) param;

	Fw8u* pSrc = dilate_par->pSrc + dilate_par->yBegin * dilate_par->srcStep;
	int srcStep = dilate_par->srcStep;
	Fw8u* pDst = dilate_par->pDst + dilate_par->yBegin * dilate_par->dstStep;
	int dstStep = dilate_par->dstStep;
	FwiSize roiSize = dilate_par->roiSize;
	int channel = dilate_par->channel;
	int yBegin = dilate_par->yBegin;
	int yEnd = dilate_par->yEnd;
	const Fw8u* pMask = dilate_par->pMask;
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
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_AC4IR_SSE2_16(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 4 && maskSize.width <= 8)
	{
		padMask_16_C4R(pMask, pMask_pad, pMask_pad_width, maskSize);
		//printout(pMask_pad, pMask_pad_width, maskSize.height, 1);
		dilate_C4R_SSE2_32(pSrc, srcStep, pDst, dstStep, roiSize, pMask, pMask_pad, maskSize, pMask_pad_width, anchor, channel, yBegin, yEnd);
	}
	else if (maskSize.width > 8 && maskSize.width <= 12)
	{
		pMask_pad_width = 48;
	}
	else
		pMask_pad_width = 128;

	fwFree(pMask_pad);
}

// Case 1: Not-in-place operation.
template< class TS, CH chSrc, DispatchType disp >
FwStatus MyFW_Dilate(const TS* pSrc, int srcStep, TS* pDst, int dstStep, FwiSize roiSize,
					   const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	if (pSrc==NULL || pDst==NULL || pMask == NULL) return fwStsNullPtrErr;

	if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

	if (roiSize.height < 1 || roiSize.width < 1 || maskSize.width < 1 || maskSize.height < 1) return fwStsSizeErr;

	if (anchor.x < 0 || anchor.x >= maskSize.width || anchor.y < 0 || anchor.y >= maskSize.height) return fwStsAnchorErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);

	TS* srcAddr;
	TS* dstAddr;

	//Check if all mask values are equal to zero.
	for(x = 0; x < maskSize.width * maskSize.height; x++)
	{
		if(*(pMask+x) != 0)
		{
			//Whenever found a nonzero value, I will set the x out of bound. So they can go out of the loop.
			x = maskSize.width * maskSize.height;
		}
	}

	if(x != maskSize.width * maskSize.height + 1)
		return fwStsZeroMaskValuesErr;

	//With comparison to other vendors' result, it was found that we do not need to do mask reflection in Dilate
	//Obtain the mask reflection
	//Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);
	//if(pMask_reflect == NULL) return fwStsMemAllocErr;

	//for(y = 0; y < maskSize.height; y++)
	//{
	//	for(x = 0; x < maskSize.width; x++)
	//	{
	//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
	//	}
	//}

	bool isAC4 = false;

	if(channel == 5)	//AC4
	{
		channel = 4;
		isAC4 = true;
	}

	//if((maskSize.width-1+roiSize.width)*channel > srcStep || (maskSize.width-1+roiSize.width)*channel > dstStep) return fwStsStrideErr;
	if(((maskSize.width-1+roiSize.width)*channel*(int)sizeof(TS) > srcStep) ||
		(roiSize.width*channel*(int)sizeof(TS) > dstStep)) return fwStsStrideErr;

	//Dilate with specified square mask.
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3 && isAC4)
				{
					srcAddr = (TS*)((Fw8u*)pSrc+y*srcStep+x*channel+k);
					dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);
					*dstAddr = *srcAddr;
				}
				else
				{
					dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*dstAddr = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							srcAddr = (TS*)((Fw8u*)pSrc+(y+n)*srcStep+(x+m)*channel+k);
							dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

							//if((*(pMask_reflect + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
							if((*(pMask + (n+anchor.y)*maskSize.width + (m+anchor.x)) > 0) &&
								(*srcAddr > *dstAddr))
								{
									*dstAddr = *srcAddr;
								}
						}
					}
				}
			}
		}
	}

	//fwFree(pMask_reflect);
	return fwStsNoErr;
}

// Case 2: In-place operation.
/*
	Note: even though it is in-place operation, but we cannot just overwrite to the source image ROI.
		Because any dilation operation will conside its neighbors intensity.
		So, I create a temporary destination image ROI to save the dilation operation outcomes.
*/

template< class TS, CH chSrc, DispatchType disp >	
FwStatus MyFW_Dilate(TS* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	if (pSrcDst == NULL || pMask == NULL) return fwStsNullPtrErr;

	if (srcDstStep < 1) return fwStsStepErr;

	if (roiSize.height < 1 || roiSize.width < 1 || maskSize.width < 1 || maskSize.height < 1) return fwStsSizeErr;

	if (anchor.x < 0 || anchor.x >= maskSize.width || anchor.y < 0 || anchor.y >= maskSize.height) return fwStsAnchorErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);
	TS max;
	TS* srcAddr;
	TS* dstAddr;

	//Check if all mask values are equal to zero.
	for(x = 0; x < maskSize.width * maskSize.height; x++)
	{
		if(*(pMask+x) != 0)
		{
			//Whenever found a nonzero value, I will set the x out of bound. So they can go out of the loop.
			x = maskSize.width * maskSize.height;
		}
	}

	if(x != maskSize.width * maskSize.height + 1)
		return fwStsZeroMaskValuesErr;

	//Obtain the mask reflection
	//Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);
	//if(pMask_reflect == NULL) return fwStsMemAllocErr;

	//for(y = 0; y < maskSize.height; y++)
	//{
	//	for(x = 0; x < maskSize.width; x++)
	//	{
	//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
	//	}
	//}

	//Create a temporary destination image ROI to save operation outcomes.
	TS* t_pDst = (TS*) fwMalloc(sizeof(TS) * (roiSize.height * roiSize.width * channel));
	if(t_pDst == NULL) return fwStsMemAllocErr;

	//Temporary destination ROI image initialization.
	for(y = 0; y < roiSize.height; y++)
	{
		for(x = 0; x < roiSize.width; x++)
		{
			for(k = 0; k < channel; k++)
			{
				dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width+x*channel+k);
				*dstAddr = 0;
			}
		}
	}

	bool isAC4 = false;

	if(channel == 5)	//AC4
	{
		channel = 4;
		isAC4 = true;
	}

	if((maskSize.width-1+roiSize.width)*channel*(int)sizeof(TS) > srcDstStep) return fwStsStrideErr;

	//Dilate with specified square mask.
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k = 0; k < channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3 && isAC4)
				{
					srcAddr = (TS*)((Fw8u*)pSrcDst+y*srcDstStep+x*channel+k);
					dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);
					*dstAddr = *srcAddr;
				}
				else
				{
					max = 0;

					for(int n = -anchor.y; n < maskSize.height-anchor.y; n++)
					{
						for(int m = -anchor.x; m < maskSize.width-anchor.x; m++)
						{
							//To overlook the boundary effects, I can just ignore the boundary pixels when operates.
							//This will give me the same outcome as Matlab. But other vendors might not use this way.
							//It simply use ROI without the boundary region.
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							srcAddr = (TS*)((Fw8u*)pSrcDst+(y+n)*srcDstStep+(x+m)*channel+k);

							//if(*(pMask_reflect + (n+anchor.y) * maskSize.width + (m+anchor.x)) > 0)
							if(*(pMask + (n+anchor.y) * maskSize.width + (m+anchor.x)) > 0)
							{
								if(*srcAddr > max)
								{
									max = *srcAddr;
								}
							}
						}
					}

					dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);
					*dstAddr = max;
				}
			}
		}
	}

	//Copy the temporary destination image ROI to the source image ROI.
	for(y = 0; y < roiSize.height; y++)
	{
		for(x = 0; x < roiSize.width; x++)
		{
			for(k = 0; k < channel; k++)
			{
				srcAddr = (TS*)((Fw8u*)pSrcDst+y*srcDstStep+x*channel+k);
				dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);

				*srcAddr=*dstAddr;
			}
		}
	}

	//fwFree(pMask_reflect);
	fwFree(t_pDst);
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL) return fwStsNullPtrErr;

			if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 1;

			//if(((maskSize.width+roiSize.width-1)*channel > srcStep) || ((maskSize.width+roiSize.width-1)*channel > dstStep)) return fwStsStrideErr;
			if(((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) ||
				(roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			//Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilate_8u_Not_In_Place dilate_par[MAXTHREADNUM];

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrc;
					dilate_par[thread_num].srcStep = srcStep;
					dilate_par[thread_num].pDst = pDst;
					dilate_par[thread_num].dstStep = dstStep;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					//dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].pMask = pMask;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_C1R, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL) return fwStsNullPtrErr;

			if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 3;

			//if(((maskSize.width-1+roiSize.width)*channel > srcStep) || ((maskSize.width-1+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) ||
				(roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;
#ifdef Hai_DEBUG
			total_thread_num = 1;
#endif

			/*
			With comparison to other vendors' result, it was found that we do not need to do mask reflection in Dilate
			*/
			//Obtain the mask reflection
			//Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilate_8u_Not_In_Place dilate_par[MAXTHREADNUM];

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrc;
					dilate_par[thread_num].srcStep = srcStep;
					dilate_par[thread_num].pDst = pDst;
					dilate_par[thread_num].dstStep = dstStep;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					//dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].pMask = pMask;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_C3R, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL) return fwStsNullPtrErr;

			if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 4;

			//if(((maskSize.width-1+roiSize.width)*channel > srcStep) || ((maskSize.width-1+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) ||
				(roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;
#ifdef Hai_DEBUG
			total_thread_num = 1;
#endif

			//Obtain the mask reflection
			//Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			//for(int y = 0; y < maskSize.height; y++)
			//{
			//	for(int x = 0; x < maskSize.width; x++)
			//	{
			//		*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
			//	}
			//}

			fwiDilate_8u_Not_In_Place dilate_par[MAXTHREADNUM];

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrc;
					dilate_par[thread_num].srcStep = srcStep;
					dilate_par[thread_num].pDst = pDst;
					dilate_par[thread_num].dstStep = dstStep;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					//dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].pMask = pMask;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_C4R, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			//fwFree(pMask_reflect);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_AC4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrc == NULL || pDst == NULL) return fwStsNullPtrErr;

			if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 4;

			//if(((maskSize.width-1+roiSize.width)*channel > srcStep) || ((maskSize.width-1+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) ||
				(roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;
#ifdef Hai_DEBUG
			total_thread_num = 1;
#endif

			//Obtain the mask reflection
			Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			for(int y = 0; y < maskSize.height; y++)
			{
				for(int x = 0; x < maskSize.width; x++)
				{
					*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
				}
			}

			fwiDilate_8u_Not_In_Place dilate_par[MAXTHREADNUM];

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrc;
					dilate_par[thread_num].srcStep = srcStep;
					dilate_par[thread_num].pDst = pDst;
					dilate_par[thread_num].dstStep = dstStep;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_AC4R, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			fwFree(pMask_reflect);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, AC4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return MyFW_Dilate<Fw32f, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return MyFW_Dilate<Fw32f, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return MyFW_Dilate<Fw32f, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_AC4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return MyFW_Dilate<Fw32f, AC4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, pMask, maskSize, anchor);
}

//The idea will be use C1R to get the Dilation results, then copy to a temporary buffer. Finally, copy back to pSrc.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_C1IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL || pMask == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1 || maskSize.width < 1 || maskSize.height < 1) return fwStsSizeErr;

			if (anchor.x < 0 || anchor.x >= maskSize.width || anchor.y < 0 || anchor.y >= maskSize.height) return fwStsAnchorErr;

			int channel = 1;

			if ((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;
			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

#ifdef Hai_DEBUG
			total_thread_num = 1;
#endif

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			for(int y = 0; y < maskSize.height; y++)
			{
				for(int x = 0; x < maskSize.width; x++)
				{
					*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
				}
			}

			fwiDilate_8u_In_Place dilate_par[MAXTHREADNUM];

			Fw8u** remainBottomLines = (Fw8u**) fwMalloc(sizeof(int) * total_thread_num);
			Fw8u** remainTopLines = (Fw8u**) fwMalloc(sizeof(int) * total_thread_num);

			for(int i = 0; i < total_thread_num; i++)
			{
				remainBottomLines[i] = (Fw8u*)fwMalloc(sizeof(Fw8u) * (roiSize.width * channel));
				remainTopLines[i] = (Fw8u*)fwMalloc(sizeof(Fw8u) * (roiSize.width * channel));
			}

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					//if(thread_num > 0)
					//{
					//	//Because of the "analysis window", in each strip, the starting address 
					//	//should go back a little.
					//	y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					//}

					dilate_par[thread_num].pSrc = pSrcDst;
					dilate_par[thread_num].srcStep = srcDstStep;
					//dilate_par[thread_num].pDst = t_pDst;
					//dilate_par[thread_num].dstStep = roiSize.width * channel;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;
					dilate_par[thread_num].remainBottomLines = remainBottomLines[thread_num];
					dilate_par[thread_num].remainTopLines = remainTopLines[thread_num];

					ThreadPool::Run(My_fwiDilate_8u_C1IR, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();

				//copy the remain lines processed result back to pSrc
				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					copyOneLine_Dilate(remainTopLines[thread_num], pSrcDst + (dilate_par[thread_num].yBegin) * srcDstStep, 
						roiSize.width, channel);
					copyOneLine_Dilate(remainBottomLines[thread_num], pSrcDst + (dilate_par[thread_num].yEnd) * srcDstStep, 
						roiSize.width, channel);
				}
			}

			for(int i = 0; i < total_thread_num; i++)
			{
				fwFree(remainBottomLines[i]);
				fwFree(remainTopLines[i]);
			}
			fwFree(remainBottomLines);
			fwFree(remainTopLines);
			
			fwFree(pMask_reflect);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, C1, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_C3IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL || pMask == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1 || maskSize.width < 1 || maskSize.height < 1) return fwStsSizeErr;

			if (anchor.x < 0 || anchor.x >= maskSize.width || anchor.y < 0 || anchor.y >= maskSize.height) return fwStsAnchorErr;

			int channel = 3;

			if ((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;
			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			for(int y = 0; y < maskSize.height; y++)
			{
				for(int x = 0; x < maskSize.width; x++)
				{
					*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
				}
			}

			fwiDilate_8u_In_Place dilate_par[MAXTHREADNUM];

			//Temporary buffer
			Fw8u* t_pDst = (Fw8u*) fwMalloc(sizeof(Fw8u) * roiSize.width * roiSize.height * channel);
			if(t_pDst == NULL) return fwStsMemAllocErr;

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrcDst;
					dilate_par[thread_num].srcStep = srcDstStep;
					dilate_par[thread_num].pDst = t_pDst;
					dilate_par[thread_num].dstStep = roiSize.width * channel;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_C3IR, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			fwFree(pMask_reflect);
			fwFree(t_pDst);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, C3, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_8u_AC4IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL || pMask == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1 || maskSize.width < 1 || maskSize.height < 1) return fwStsSizeErr;

			if (anchor.x < 0 || anchor.x >= maskSize.width || anchor.y < 0 || anchor.y >= maskSize.height) return fwStsAnchorErr;

			int channel = 4;

			if (((maskSize.width-1+roiSize.width)*channel*(int)sizeof(Fw8u)) > srcDstStep) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();

			//If the image height in a thread is less than the mask height, I will use Ref code to process the image
			if((roiSize.height/total_thread_num)-1 <= maskSize.height)
				total_thread_num = 1;

			//Obtain the mask reflection
			Fw8u* pMask_reflect = (Fw8u*) fwMalloc(sizeof(char) * maskSize.height * maskSize.width);

			for(int y = 0; y < maskSize.height; y++)
			{
				for(int x = 0; x < maskSize.width; x++)
				{
					*(pMask_reflect + (-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)) = *(pMask+y*maskSize.width+x);
				}
			}

			fwiDilate_8u_In_Place dilate_par[MAXTHREADNUM];

			//Temporary buffer
			Fw8u* t_pDst = (Fw8u*) fwMalloc(sizeof(Fw8u) * roiSize.width * roiSize.height * channel);
			if(t_pDst == NULL) return fwStsMemAllocErr;

			if(total_thread_num >= 1)
			{
				int y_bgn, y_end;
				int thread_num;
				
				if(total_thread_num > MAXTHREADNUM) total_thread_num = MAXTHREADNUM;

				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					//The starting and ending Y position in each thread.
					y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					y_end   = y_bgn+(roiSize.height/total_thread_num)-1;

					if (thread_num==total_thread_num-1)
					{
						y_end = roiSize.height - 1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address 
						//should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num); 
					}

					dilate_par[thread_num].pSrc = pSrcDst;
					dilate_par[thread_num].srcStep = srcDstStep;
					dilate_par[thread_num].pDst = t_pDst;
					dilate_par[thread_num].dstStep = roiSize.width * channel;
					dilate_par[thread_num].roiSize = roiSize;
					dilate_par[thread_num].yBegin = y_bgn;
					dilate_par[thread_num].yEnd = y_end;
					dilate_par[thread_num].channel = channel;
					dilate_par[thread_num].pMask = pMask_reflect;
					dilate_par[thread_num].maskSize = maskSize;
					dilate_par[thread_num].anchor = anchor;

					ThreadPool::Run(My_fwiDilate_8u_AC4IR, (void *) &dilate_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			fwFree(pMask_reflect);
			fwFree(t_pDst);
			return fwStsNoErr;
		}
		default:
			return MyFW_Dilate<Fw8u, AC4, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_C1IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	return MyFW_Dilate<Fw32f, C1, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_C3IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	return MyFW_Dilate<Fw32f, C3, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiDilate_32f_AC4IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor)
{
	return MyFW_Dilate<Fw32f, AC4, DT_REFR> (pSrcDst, srcDstStep, roiSize, pMask, maskSize, anchor);
}

#endif //BUILD_NUM_AT_LEAST
#endif
