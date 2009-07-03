/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#if 0
#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )
#define MAXTHREADNUM 4

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
}	fwiErode3x3_8u_Not_In_Place;

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
	Fw8u* remainTopLines;
	Fw8u* remainBottomLines;
}	fwiErode3x3_8u_In_Place;

//void printOut(Fw8u* pSrc, int width, int channel)
//{
//	printf("\n------------------------------------\n");
//	for(int i = 0; i < width * channel; i++)
//	{
//		printf("%d	", *(pSrc+i));
//	}
//	printf("\n------------------------------------\n");
//}

//Erode Process Reference Code: The computation starting at center pixel
//		---------
//		0 | 0 | 0
//		---------
//		0 | x | 0
//		---------
//		0 | 0 | 0
//		---------
void ErodeStartAtCenter_Ref(const Fw8u* pSrc, Fw8u* pDst, int srcStep, int xBegin, int xEnd, int channel)
{
	for(int x = xBegin; x < xEnd; x++)
	{
		for (int k=0; k<channel; k++)
		{
			//In the four-channel imamge, the alpha channel is not processed.
			*(pDst+x*channel+k) = 255;

			for(int n = -1; n <= 1; n++)
			{
				for(int m = -1; m <= 1; m++)
				{
					if(*(pSrc+n*srcStep+(x+m)*channel+k) < *(pDst+x*channel+k))
					{
						*(pDst+x*channel+k) = *(pSrc+n*srcStep+(x+m)*channel+k);
					}
				}
			}
		}
	}
}

//Erode Process Reference Code: The computation starting at (0,0) pixel
//		---------
//		x | 0 | 0
//		---------
//		0 | 0 | 0
//		---------
//		0 | 0 | 0
//		---------
void ErodeStartAtLeftCorner_Ref(Fw8u* line1, Fw8u* line2, Fw8u* line3, Fw8u* pDst, int xEnd, int channel)
{
	for(int x = 0; x < xEnd; x++)
	{
		for(int k = 0; k < channel; k++)
		{
			*(pDst+x*channel+k) = 255;
			//find the min in 3 lines
			for(int m = 0; m <= 2; m++)
			{
				if(*(line1 + (x+m)*channel+k) < *(pDst+x*channel+k))
					*(pDst+x*channel+k) = *(line1 + (x+m)*channel+k);

				if(*(line2 + (x+m)*channel+k) < *(pDst+x*channel+k))
					*(pDst+x*channel+k) = *(line2 + (x+m)*channel+k);

				if(*(line3 + (x+m)*channel+k) < *(pDst+x*channel+k))
					*(pDst+x*channel+k) = *(line3 + (x+m)*channel+k);
			}
		}
	}
}

//Load data from Source (pSrc) to Destination (pDst)
//The starting address might not be 16-byte aligned
//The border address might not be 16-byte aligned
void ErodeCopyOneLine(Fw8u* pSrc, Fw8u* pDst, int width, int channel)
{
	Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
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
			_mm_prefetch((char*)(ps), _MM_HINT_T1);

			xmm00 = _mm_load_si128( (__m128i*)(ps));
			//_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
			_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
		}
	}
	else
	{
		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
		{
			_mm_prefetch((char*)(ps), _MM_HINT_T1);

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

void ErodeCopyOneLine_noCache(Fw8u* pSrc, Fw8u* pDst, int width, int channel)
{
	Fw8u* ps  = pSrc; Fw8u* pd  = pDst;
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
			_mm_prefetch((char*)(ps), _MM_HINT_T1);

			xmm00 = _mm_load_si128( (__m128i*)(ps));
			_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
			//_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
		}
	}
	else
	{
		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
		{
			_mm_prefetch((char*)(ps), _MM_HINT_T1);

			xmm00 = _mm_loadu_si128( (__m128i*)(ps));
			_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
			//_mm_store_si128((__m128i *)(pd), xmm00); // store 128 bytes	faster than stream
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

//Erode process data stored in line1, line2 and line3.
void Erode3x3Process_SSE2_C1IR(Fw8u* pDst, Fw8u* line1, Fw8u* line2, Fw8u* line3, int width, int channel)
{
	__m128i min0, min1, min2;
	__m128i xmm00, xmm01, xmm02, xmm03, xmm04, xmm05, xmm06, xmm07, xmm08, xmm09, xmm10, xmm11;

	int xEnd = width * channel - (int)((Fw64u)(pDst + width * channel) % 16);

	for(int x = 0; x < xEnd; x+=16, line1 += 16, line2 += 16, line3 += 16, pDst += 16)
	{
		//loading data:
		xmm00 = *(__m128i*)(line1);
		xmm01 = *(__m128i*)(line2);
		xmm02 = *(__m128i*)(line3);
		xmm03 = *(__m128i*)(line1 + 16);
		xmm04 = *(__m128i*)(line2 + 16);
		xmm05 = *(__m128i*)(line3 + 16);

		//_mm_prefetch((char*)(line1), _MM_HINT_T1);
		//_mm_prefetch((char*)(line2), _MM_HINT_T1);
		//_mm_prefetch((char*)(line3), _MM_HINT_T1);
		_mm_prefetch((char*)(line1 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line2 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line3 + 16), _MM_HINT_T1);

		//Process the -1 row:
		min0 = _mm_min_epu8(xmm00, xmm02);
		min0 = _mm_min_epu8(min0, xmm01);

		//Process the 0 row:
		xmm06 = _mm_srli_si128(xmm00, 1);
		xmm07 = _mm_srli_si128(xmm01, 1);
		xmm08 = _mm_srli_si128(xmm02, 1);
		xmm09 = _mm_slli_si128(xmm03, 15);
		xmm10 = _mm_slli_si128(xmm04, 15);
		xmm11 = _mm_slli_si128(xmm05, 15);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min1 = _mm_min_epu8(xmm06, xmm07);
		min1 = _mm_min_epu8(min1, xmm08);

		//Process the +1 row:
		xmm06 = _mm_srli_si128(xmm00, 2);
		xmm07 = _mm_srli_si128(xmm01, 2);
		xmm08 = _mm_srli_si128(xmm02, 2);
		xmm09 = _mm_slli_si128(xmm03, 14);
		xmm10 = _mm_slli_si128(xmm04, 14);
		xmm11 = _mm_slli_si128(xmm05, 14);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min2 = _mm_min_epu8(xmm06, xmm07);
		min2 = _mm_min_epu8(min2, xmm08);

		//Find the min in the first 3 rows and save it to xmm0
		min0 = _mm_min_epu8(min0, min1);
		min0 = _mm_min_epu8(min0, min2);

		_mm_store_si128((__m128i *)(pDst), min0); // store 128 bytes
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		int diff = (width - xEnd/channel) * channel - (width * channel - xEnd);
		ErodeStartAtLeftCorner_Ref(line1-diff, line2-diff, line3-diff, pDst - diff, width - xEnd/channel+1, channel);
	}
}

void Erode3x3Process_SSE2_C3IR(Fw8u* pDst, Fw8u* line1, Fw8u* line2, Fw8u* line3, int width, int channel)
{
	__m128i min0, min1, min2;
	__m128i xmm00, xmm01, xmm02, xmm03, xmm04, xmm05, xmm06, xmm07, xmm08, xmm09, xmm10, xmm11;

	int xEnd = width * channel - (int)((Fw64u)(pDst + width * channel) % 16);

	for(int x = 0; x < xEnd; x+=16, line1 += 16, line2 += 16, line3 += 16, pDst += 16)
	{
		//loading data:
		xmm00 = *(__m128i*)(line1);
		xmm01 = *(__m128i*)(line2);
		xmm02 = *(__m128i*)(line3);
		xmm03 = *(__m128i*)(line1 + 16);
		xmm04 = *(__m128i*)(line2 + 16);
		xmm05 = *(__m128i*)(line3 + 16);

		//_mm_prefetch((char*)(line1), _MM_HINT_T1);
		//_mm_prefetch((char*)(line2), _MM_HINT_T1);
		//_mm_prefetch((char*)(line3), _MM_HINT_T1);
		_mm_prefetch((char*)(line1 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line2 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line3 + 16), _MM_HINT_T1);

		//Process the -1 row:
		min0 = _mm_min_epu8(*(__m128i*)(line1), *(__m128i*)(line3));
		min0 = _mm_min_epu8(min0, *(__m128i*)(line2));

		//Process the 0 row:
		xmm06 = _mm_srli_si128(xmm00, 3);
		xmm07 = _mm_srli_si128(xmm01, 3);
		xmm08 = _mm_srli_si128(xmm02, 3);
		xmm09 = _mm_slli_si128(xmm03, 13);
		xmm10 = _mm_slli_si128(xmm04, 13);
		xmm11 = _mm_slli_si128(xmm05, 13);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min1 = _mm_min_epu8(xmm06, xmm07);
		min1 = _mm_min_epu8(min1, xmm08);

		//Process the +1 line:
		xmm06 = _mm_srli_si128(xmm00, 6);
		xmm07 = _mm_srli_si128(xmm01, 6);
		xmm08 = _mm_srli_si128(xmm02, 6);
		xmm09 = _mm_slli_si128(xmm03, 10);
		xmm10 = _mm_slli_si128(xmm04, 10);
		xmm11 = _mm_slli_si128(xmm05, 10);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min2 = _mm_min_epu8(xmm06, xmm07);
		min2 = _mm_min_epu8(min2, xmm08);

		//Find the min in the first 3 rows and save it to xmm0
		min0 = _mm_min_epu8(min0, min1);
		min0 = _mm_min_epu8(min0, min2);

		_mm_store_si128((__m128i *)(pDst), min0); // store 128 bytes
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		int diff = (width - xEnd/channel) * channel - (width * channel - xEnd);
		ErodeStartAtLeftCorner_Ref(line1-diff, line2-diff, line3-diff, pDst - diff, width - xEnd/channel+1, channel);
	}
}

void Erode3x3Process_SSE2_AC4IR(Fw8u* pDst, Fw8u* line1, Fw8u* line2, Fw8u* line3, int width, int channel)
{
	__m128i min0, min1, min2;
	__m128i xmm00, xmm01, xmm02, xmm03, xmm04, xmm05, xmm06, xmm07, xmm08, xmm09, xmm10, xmm11;

	__m128i alphaChannel;
	Fw8u* alphaMask = (Fw8u*)fwMalloc(sizeof(Fw8u) * 16);
	
	alphaMask[0] = 255;
	alphaMask[1] = 255;
	alphaMask[2] = 255;
	alphaMask[3] = 0;
	alphaMask[4] = 255;
	alphaMask[5] = 255;
	alphaMask[6] = 255;
	alphaMask[7] = 0;
	alphaMask[8] = 255;
	alphaMask[9] = 255;
	alphaMask[10] = 255;
	alphaMask[11] = 0;
	alphaMask[12] = 255;
	alphaMask[13] = 255;
	alphaMask[14] = 255;
	alphaMask[15] = 0;

	__m128i aMask = *(__m128i*)alphaMask;

	fwFree(alphaMask);

	int xEnd = width * channel - (int)((Fw64u)(pDst + width * channel) % 16);

	for(int x = 0; x < xEnd; x+=16, line1 += 16, line2 += 16, line3 += 16, pDst += 16)
	{
		//loading data:
		xmm00 = *(__m128i*)(line1);
		xmm01 = *(__m128i*)(line2);
		xmm02 = *(__m128i*)(line3);
		xmm03 = *(__m128i*)(line1 + 16);
		xmm04 = *(__m128i*)(line2 + 16);
		xmm05 = *(__m128i*)(line3 + 16);

		//_mm_prefetch((char*)(line1), _MM_HINT_T1);
		//_mm_prefetch((char*)(line2), _MM_HINT_T1);
		//_mm_prefetch((char*)(line3), _MM_HINT_T1);
		_mm_prefetch((char*)(line1 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line2 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line3 + 16), _MM_HINT_T1);

		//Process the -1 row:
		min0 = _mm_min_epu8(xmm00, xmm02);
		min0 = _mm_min_epu8(min0, xmm01);

		//Process the 0 row:
		xmm06 = _mm_srli_si128(xmm00, 4);
		xmm07 = _mm_srli_si128(xmm01, 4);
		xmm08 = _mm_srli_si128(xmm02, 4);
		xmm09 = _mm_slli_si128(xmm03, 12);
		xmm10 = _mm_slli_si128(xmm04, 12);
		xmm11 = _mm_slli_si128(xmm05, 12);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		alphaChannel = _mm_andnot_si128(xmm07, aMask);		//obtain the alpha channel data

		min1 = _mm_min_epu8(xmm06, xmm07);
		min1 = _mm_min_epu8(min1, xmm08);

		//Process the +1 line:
		xmm06 = _mm_srli_si128(xmm00, 8);
		xmm07 = _mm_srli_si128(xmm01, 8);
		xmm08 = _mm_srli_si128(xmm02, 8);
		xmm09 = _mm_slli_si128(xmm03, 8);
		xmm10 = _mm_slli_si128(xmm04, 8);
		xmm11 = _mm_slli_si128(xmm05, 8);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min2 = _mm_min_epu8(xmm06, xmm07);
		min2 = _mm_min_epu8(min2, xmm08);

		//Find the min in the first 3 rows and save it to xmm00
		min0 = _mm_min_epu8(min0, min1);
		min0 = _mm_min_epu8(min0, min2);

		min0 = _mm_and_si128(min0, aMask);				//clear the data in the alpha channel
		min0 = _mm_or_si128(min0, alphaChannel);		//put the alpha channel data back

		_mm_store_si128((__m128i *)(pDst), min0); // store 128 bytes
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		int diff = (width - xEnd/channel) * channel - (width * channel - xEnd);
		ErodeStartAtLeftCorner_Ref(line1-diff, line2-diff, line3-diff, pDst - diff, width - xEnd/channel+1, channel);
	}
}

void Erode3x3Process_SSE2_C4IR(Fw8u* pDst, Fw8u* line1, Fw8u* line2, Fw8u* line3, int width, int channel)
{
	__m128i min0, min1, min2;
	__m128i xmm00, xmm01, xmm02, xmm03, xmm04, xmm05, xmm06, xmm07, xmm08, xmm09, xmm10, xmm11;

	int xEnd = width * channel - (int)((Fw64u)(pDst + width * channel) % 16);

	for(int x = 0; x < xEnd; x+=16, line1 += 16, line2 += 16, line3 += 16, pDst += 16)
	{
		//loading data:
		xmm00 = *(__m128i*)(line1);
		xmm01 = *(__m128i*)(line2);
		xmm02 = *(__m128i*)(line3);
		xmm03 = *(__m128i*)(line1 + 16);
		xmm04 = *(__m128i*)(line2 + 16);
		xmm05 = *(__m128i*)(line3 + 16);

		//_mm_prefetch((char*)(line1), _MM_HINT_T1);
		//_mm_prefetch((char*)(line2), _MM_HINT_T1);
		//_mm_prefetch((char*)(line3), _MM_HINT_T1);
		_mm_prefetch((char*)(line1 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line2 + 16), _MM_HINT_T1);
		_mm_prefetch((char*)(line3 + 16), _MM_HINT_T1);

		//Process the -1 row:
		min0 = _mm_min_epu8(xmm00, xmm02);
		min0 = _mm_min_epu8(min0, xmm01);

		//Process the 0 row:
		xmm06 = _mm_srli_si128(xmm00, 4);
		xmm07 = _mm_srli_si128(xmm01, 4);
		xmm08 = _mm_srli_si128(xmm02, 4);
		xmm09 = _mm_slli_si128(xmm03, 12);
		xmm10 = _mm_slli_si128(xmm04, 12);
		xmm11 = _mm_slli_si128(xmm05, 12);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min1 = _mm_min_epu8(xmm06, xmm07);
		min1 = _mm_min_epu8(min1, xmm08);

		//Process the +1 line:
		xmm06 = _mm_srli_si128(xmm00, 8);
		xmm07 = _mm_srli_si128(xmm01, 8);
		xmm08 = _mm_srli_si128(xmm02, 8);
		xmm09 = _mm_slli_si128(xmm03, 8);
		xmm10 = _mm_slli_si128(xmm04, 8);
		xmm11 = _mm_slli_si128(xmm05, 8);
		xmm06 = _mm_or_si128(xmm06, xmm09);
		xmm07 = _mm_or_si128(xmm07, xmm10);
		xmm08 = _mm_or_si128(xmm08, xmm11);

		min2 = _mm_min_epu8(xmm06, xmm07);
		min2 = _mm_min_epu8(min2, xmm08);

		//Find the min in the first 3 rows and save it to xmm0
		min0 = _mm_min_epu8(min0, min1);
		min0 = _mm_min_epu8(min0, min2);

		_mm_store_si128((__m128i *)(pDst), min0); // store 128 bytes
	}

	//Deal with the remain part
	if(xEnd != width * channel)
	{
		int diff = (width - xEnd/channel) * channel - (width * channel - xEnd);
		ErodeStartAtLeftCorner_Ref(line1-diff, line2-diff, line3-diff, pDst - diff, width - xEnd/channel+1, channel);
	}
}

//Erode3x3 with SSE2 optimization
//In-Place operaion.
/*
To improve the performance, we should use load/save operation as less as possible.
In this funciton, to reduce the load/save operation, I fist load all 3 lines into buffer.
Then with pointer operation and shift, I do not need load any data.
One further work is for dilate3x3 operation, I only need to load 3 lines at the first time. From then on,
I only need to load one more line of data with shift those 2 data lines above one line up.
*/
void My_fwiErode3x3_8u_C1IR(void* param)
{
	fwiErode3x3_8u_In_Place *erode3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = erode3x3_par->pSrc + erode3x3_par->yBegin * erode3x3_par->srcStep;
	int srcStep = erode3x3_par->srcStep;
	FwiSize roiSize = erode3x3_par->roiSize;
	int channel = erode3x3_par->channel;
	int yBegin = erode3x3_par->yBegin;
	int yEnd = erode3x3_par->yEnd;
	Fw8u* remainBottomLines = erode3x3_par->remainBottomLines;
	Fw8u* remainTopLines = erode3x3_par->remainTopLines;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_C1IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		if(y == yBegin +1)
		{
			//copy the first line result to a temporary buffer.
			ErodeCopyOneLine(result, remainTopLines, roiSize.width, channel);
		}
		else
		{
			ErodeCopyOneLine_noCache(result, pSrc - srcStep + channel, roiSize.width, channel);
		}

		Erode3x3Process_SSE2_C1IR(result, line1, line2, line3, roiSize.width, channel);
	}

	//The last line can not be directly save back to the pSrc bacause following part might need this
	//specific line to do the process. So I save it to a temp location. And save them back after all
	//process complete.
	ErodeCopyOneLine(result, remainBottomLines, roiSize.width, channel);
	//copyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

void My_fwiErode3x3_8u_C1R(void* param)
{
	fwiErode3x3_8u_In_Place *dilate3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = dilate3x3_par->pSrc + dilate3x3_par->yBegin * dilate3x3_par->srcStep;
	int srcStep = dilate3x3_par->srcStep;
	Fw8u* pDst = dilate3x3_par->pDst + dilate3x3_par->yBegin * dilate3x3_par->dstStep;
	int dstStep = dilate3x3_par->dstStep;
	FwiSize roiSize = dilate3x3_par->roiSize;
	int channel = dilate3x3_par->channel;
	int yBegin = dilate3x3_par->yBegin;
	int yEnd = dilate3x3_par->yEnd;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_C1IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		//ErodeCopyOneLine(result, pDst - dstStep + channel, roiSize.width, channel);
		ErodeCopyOneLine(result, pDst, roiSize.width, channel);

		Erode3x3Process_SSE2_C1IR(result, line1, line2, line3, roiSize.width, channel);
	}

	//ErodeCopyOneLine(result, pDst - dstStep + channel, roiSize.width, channel);
	ErodeCopyOneLine(result, pDst, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

//Erode3x3 with SSE2 optimization
//In-Place operaion.
void My_fwiErode3x3_8u_C3IR(void* param)
{
	fwiErode3x3_8u_In_Place *erode3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = erode3x3_par->pSrc + erode3x3_par->yBegin * erode3x3_par->srcStep;
	int srcStep = erode3x3_par->srcStep;
	FwiSize roiSize = erode3x3_par->roiSize;
	int channel = erode3x3_par->channel;
	int yBegin = erode3x3_par->yBegin;
	int yEnd = erode3x3_par->yEnd;
	Fw8u* remainBottomLines = erode3x3_par->remainBottomLines;
	Fw8u* remainTopLines = erode3x3_par->remainTopLines;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_C3IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		if(y == yBegin +1)
		{
			//copy the first line result to a temporary buffer.
			ErodeCopyOneLine(result, remainTopLines, roiSize.width, channel);
		}
		else
		{
			////overwrite line1 back to src
			ErodeCopyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);
		}

		Erode3x3Process_SSE2_C3IR(result, line1, line2, line3, roiSize.width, channel);
	}

	//The last line can not be directly save back to the pSrc bacause following part might need this
	//specific line to do the process. So I save it to a temp location. And save them back after all
	//process complete.
	ErodeCopyOneLine(result, remainBottomLines, roiSize.width, channel);
	//copyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

void My_fwiErode3x3_8u_C4IR(void* param)
{
	fwiErode3x3_8u_In_Place *erode3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = erode3x3_par->pSrc + erode3x3_par->yBegin * erode3x3_par->srcStep;
	int srcStep = erode3x3_par->srcStep;
	FwiSize roiSize = erode3x3_par->roiSize;
	int channel = erode3x3_par->channel;
	int yBegin = erode3x3_par->yBegin;
	int yEnd = erode3x3_par->yEnd;
	Fw8u* remainBottomLines = erode3x3_par->remainBottomLines;
	Fw8u* remainTopLines = erode3x3_par->remainTopLines;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_C4IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		if(y == yBegin +1)
		{
			//copy the first line result to a temporary buffer.
			ErodeCopyOneLine(result, remainTopLines, roiSize.width, channel);
		}
		else
		{
			////overwrite line1 back to src
			ErodeCopyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);
		}

		Erode3x3Process_SSE2_C4IR(result, line1, line2, line3, roiSize.width, channel);
	}

	//The last line can not be directly save back to the pSrc bacause following part might need this
	//specific line to do the process. So I save it to a temp location. And save them back after all
	//process complete.
	ErodeCopyOneLine(result, remainBottomLines, roiSize.width, channel);
	//copyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

void My_fwiErode3x3_8u_AC4IR(void* param)
{
	fwiErode3x3_8u_In_Place *erode3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = erode3x3_par->pSrc + erode3x3_par->yBegin * erode3x3_par->srcStep;
	int srcStep = erode3x3_par->srcStep;
	FwiSize roiSize = erode3x3_par->roiSize;
	int channel = erode3x3_par->channel;
	int yBegin = erode3x3_par->yBegin;
	int yEnd = erode3x3_par->yEnd;
	Fw8u* remainBottomLines = erode3x3_par->remainBottomLines;
	Fw8u* remainTopLines = erode3x3_par->remainTopLines;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_AC4IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		if(y == yBegin +1)
		{
			//copy the first line result to a temporary buffer.
			ErodeCopyOneLine(result, remainTopLines, roiSize.width, channel);
		}
		else
		{
			////overwrite line1 back to src
			ErodeCopyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);
		}

		Erode3x3Process_SSE2_AC4IR(result, line1, line2, line3, roiSize.width, channel);
	}

	//The last line can not be directly save back to the pSrc bacause following part might need this
	//specific line to do the process. So I save it to a temp location. And save them back after all
	//process complete.
	ErodeCopyOneLine(result, remainBottomLines, roiSize.width, channel);
	//copyOneLine(result, pSrc - srcStep + channel, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

void My_fwiErode3x3_8u_C3R(void* param)
{
	fwiErode3x3_8u_In_Place *dilate3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = dilate3x3_par->pSrc + dilate3x3_par->yBegin * dilate3x3_par->srcStep;
	int srcStep = dilate3x3_par->srcStep;
	Fw8u* pDst = dilate3x3_par->pDst + dilate3x3_par->yBegin * dilate3x3_par->dstStep;
	int dstStep = dilate3x3_par->dstStep;
	FwiSize roiSize = dilate3x3_par->roiSize;
	int channel = dilate3x3_par->channel;
	int yBegin = dilate3x3_par->yBegin;
	int yEnd = dilate3x3_par->yEnd;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_C3IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		////overwrite line1 back to src
		ErodeCopyOneLine(result, pDst, roiSize.width, channel);

		Erode3x3Process_SSE2_C3IR(result, line1, line2, line3, roiSize.width, channel);
	}

	ErodeCopyOneLine(result, pDst, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

void My_fwiErode3x3_8u_C4R(void* param)
{
	fwiErode3x3_8u_Not_In_Place *erode3x3_par = (fwiErode3x3_8u_Not_In_Place *) param;

	const Fw8u* pSrc = erode3x3_par->pSrc + erode3x3_par->yBegin * erode3x3_par->srcStep;
	int srcStep = erode3x3_par->srcStep;
	Fw8u* pDst = erode3x3_par->pDst + erode3x3_par->yBegin * erode3x3_par->dstStep;
	int dstStep = erode3x3_par->dstStep;
	FwiSize roiSize = erode3x3_par->roiSize;
	int channel = erode3x3_par->channel;
	int xBegin = 0, xEnd = 0;
	int yBegin = erode3x3_par->yBegin;
	int yEnd = erode3x3_par->yEnd;

	const Fw8u *ps = pSrc;
		  Fw8u *pd = pDst;

	int x = 0, y = 0;
	__m128i xmm00, xmm01, xmm02, xmm03, xmm04;
	bool bothAligned = false;

	// I assume that the src is aligned at the border.
	//				 the dst is aligned at the data.

	//	B B B 				//pSrc points at first S (src data)
	//  B S S    D D 		//pDst points at first D (dst data)
	//	B S S    D D
	//pSrc -= (srcStep + 1);	// Now pSrc points at the first B in each line

	//SSE2
	for(y = yBegin; y < yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		ps  = pSrc; pd  = pDst;
		xBegin = (16 - (int)((Fw64u)(pd))%16)%16;
		xEnd = roiSize.width * channel - (int)((Fw64u)(pd + roiSize.width * channel) % 16);

		if((16 - (int)((Fw64u)(ps))%16)%16 == xBegin)
			bothAligned = true;
		else
			bothAligned = false;

		//Process the beginning part
		ErodeStartAtCenter_Ref(pSrc, pDst, srcStep, 0, xBegin/channel + 1, channel);

		ps += xBegin;
		pd += xBegin;

		for(x = xBegin; x < xEnd; x+=16, ps += 16, pd += 16)
		{
			//Process the -1 row:
			xmm00 = _mm_loadu_si128( (__m128i*)(ps	-srcStep	-channel));	// Left pixel in the -1 row
			xmm01 = _mm_loadu_si128( (__m128i*)(ps				-channel));	// Center pixel in the -1 row
			xmm02 = _mm_loadu_si128( (__m128i*)(ps	+srcStep	-channel));	// Right pixel in the -1 row
			xmm00 = _mm_min_epu8(xmm00, xmm02);
			xmm00 = _mm_min_epu8(xmm00, xmm01);

			//Process the 0 row:
			xmm03 = _mm_loadu_si128( (__m128i*)(ps	-srcStep));	// Left pixel in the 0 row

			if(bothAligned)
				xmm01 = _mm_load_si128( (__m128i*)(ps		));	// Center pixel in the 0 row
			else
				xmm01 = _mm_loadu_si128( (__m128i*)(ps		));	// Center pixel in the 0 row

			xmm02 = _mm_loadu_si128( (__m128i*)(ps	+srcStep));	// Right pixel in the 0 row
			xmm03 = _mm_min_epu8(xmm03, xmm02);
			xmm03 = _mm_min_epu8(xmm03, xmm01);

			//Process the +1 line:
			xmm04 = _mm_loadu_si128( (__m128i*)(ps	-srcStep	+channel));	// Left pixel in the 1 row
			xmm01 = _mm_loadu_si128( (__m128i*)(ps				+channel));	// Center pixel in the 1 row
			xmm02 = _mm_loadu_si128( (__m128i*)(ps	+srcStep	+channel));	// Right pixel in the 1 row
			xmm04 = _mm_min_epu8(xmm04, xmm02);
			xmm04 = _mm_min_epu8(xmm04, xmm01);

			//Find the max in the first 3 rows and save it to xmm0
			xmm00 = _mm_min_epu8(xmm00, xmm03);
			xmm00 = _mm_min_epu8(xmm00, xmm04);

			_mm_stream_si128((__m128i *)(pd), xmm00); // store 128 bytes
		}

		//Deal with the remain part
		if(!(xEnd == roiSize.width * channel))
		{
			ErodeStartAtCenter_Ref(pSrc, pDst, srcStep, xEnd/channel, roiSize.width, channel);
		}
	}
}

//void My_fwiErode3x3_8u_C4R_goodButSlow(void* param)
//{
//	fwiErode3x3_8u_In_Place *dilate3x3_par = (fwiErode3x3_8u_In_Place *) param;
//	Fw8u* pSrc = dilate3x3_par->pSrc + dilate3x3_par->yBegin * dilate3x3_par->srcStep;
//	int srcStep = dilate3x3_par->srcStep;
//	Fw8u* pDst = dilate3x3_par->pDst + dilate3x3_par->yBegin * dilate3x3_par->dstStep;
//	int dstStep = dilate3x3_par->dstStep;
//	FwiSize roiSize = dilate3x3_par->roiSize;
//	int channel = dilate3x3_par->channel;
//	int yBegin = dilate3x3_par->yBegin;
//	int yEnd = dilate3x3_par->yEnd;
//
//	//2*channel than the roiSize.width because of the border (3x3 window)
//	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
//	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
//	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
//	Fw8u* tline;
//	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
//
//	//Load first 3 lines and process them
//	//Load line -1
//	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
//	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
//	pSrc += srcStep;
//	//Load line 0
//	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
//	pSrc += srcStep;
//	//Load line 1
//	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
//	//Process the very top line in the ROI(line0).
//	Erode3x3Process_SSE2_C4IR(result, line1, line2, line3, roiSize.width, channel);
//
//	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
//	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep, pDst += dstStep)
//	{
//		//move up line2 and line 3 and load one more line
//		tline = line1;
//		line1 = line2;
//		line2 = line3;
//		line3 = tline;
//		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);
//
//		////overwrite line1 back to src
//		ErodeCopyOneLine(result, pDst, roiSize.width, channel);
//
//		Erode3x3Process_SSE2_C4IR(result, line1, line2, line3, roiSize.width, channel);
//	}
//
//	ErodeCopyOneLine(result, pDst, roiSize.width, channel);
//
//	fwFree(line1);
//	fwFree(line2);
//	fwFree(line3);
//	fwFree(result);
//}

void My_fwiErode3x3_8u_AC4R(void* param)
{
	fwiErode3x3_8u_In_Place *dilate3x3_par = (fwiErode3x3_8u_In_Place *) param;
	Fw8u* pSrc = dilate3x3_par->pSrc + dilate3x3_par->yBegin * dilate3x3_par->srcStep;
	int srcStep = dilate3x3_par->srcStep;
	Fw8u* pDst = dilate3x3_par->pDst + dilate3x3_par->yBegin * dilate3x3_par->dstStep;
	int dstStep = dilate3x3_par->dstStep;
	FwiSize roiSize = dilate3x3_par->roiSize;
	int channel = dilate3x3_par->channel;
	int yBegin = dilate3x3_par->yBegin;
	int yEnd = dilate3x3_par->yEnd;

	//2*channel than the roiSize.width because of the border (3x3 window)
	Fw8u* line1 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line2 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* line3 = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);
	Fw8u* tline;
	Fw8u* result = (Fw8u*) fwMalloc(sizeof(Fw8u) * (roiSize.width+2) * channel);

	//Load first 3 lines and process them
	//Load line -1
	pSrc = pSrc - srcStep - channel;		//Move to the source image (0,0)
	ErodeCopyOneLine(pSrc, line1, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 0
	ErodeCopyOneLine(pSrc, line2, (roiSize.width+2), channel);
	pSrc += srcStep;
	//Load line 1
	ErodeCopyOneLine(pSrc, line3, (roiSize.width+2), channel);
	//Process the very top line in the ROI(line0).
	Erode3x3Process_SSE2_AC4IR(result, line1, line2, line3, roiSize.width, channel);

	//Go to loop and load 1 one line each time, process it with another 2 lines in the cache
	for(int y = yBegin+1; y <= yEnd; y++, pSrc += srcStep, pDst += dstStep)
	{
		//move up line2 and line 3 and load one more line
		tline = line1;
		line1 = line2;
		line2 = line3;
		line3 = tline;
		ErodeCopyOneLine(pSrc+srcStep, tline, (roiSize.width+2), channel);

		////overwrite line1 back to src
		ErodeCopyOneLine(result, pDst, roiSize.width, channel);

		Erode3x3Process_SSE2_AC4IR(result, line1, line2, line3, roiSize.width, channel);
	}

	ErodeCopyOneLine(result, pDst, roiSize.width, channel);

	fwFree(line1);
	fwFree(line2);
	fwFree(line3);
	fwFree(result);
}

// Case 1: Not-in-place operation.
template< class TS, CH chSrc, DispatchType disp >
FwStatus MyFW_Erode3x3(const TS* pSrc, int srcStep, TS* pDst, int dstStep, 
					   FwiSize roiSize) 
{
	if (pSrc == NULL || pDst == NULL) return fwStsNullPtrErr;

	if (srcStep < 1 || dstStep < 1) return fwStsStepErr;

	if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);

	TS* srcAddr;
	TS* dstAddr;

	bool isAC4 = false;

	if(channel == 5)	//AC4
	{
		channel = 4;
		isAC4 = true;
	}

	//if(((2+roiSize.width)*channel > srcStep) || ((2+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
	if(((2+roiSize.width)*channel*(int)sizeof(TS) > srcStep) || (roiSize.width*channel*(int)sizeof(TS) > dstStep)) return fwStsStrideErr;

	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In AC4(I)R image, the alpha channel is not processed.
				if(k == 3 && isAC4)
				{
					srcAddr = (TS*)((Fw8u*)pSrc+y*srcStep+x*channel+k);
					dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*dstAddr = *srcAddr;
				}
				else
				{
					dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*dstAddr = 255;

					for(int n = -1; n <= 1; n++)
					{
						for(int m = -1; m <= 1; m++)
						{
							srcAddr = (TS*)((Fw8u*)pSrc+(y+n)*srcStep+(x+m)*channel+k);
							dstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

							if(*srcAddr < *dstAddr)
							{
								*dstAddr = *srcAddr;
							}
						}
					}
				}
			}
		}
	}

	return fwStsNoErr;
}

// Case 2: In-place operation.
/*
	Note: even though it is in-place operation, but we cannot just overwrite to the source image ROI.
		Because any dilation (3 by 3 in this case) operation will conside its neighbors intensity.
		So, I create a temporary destination image ROI to save the dilation operation outcomes.
*/

template< class TS, CH chSrc, DispatchType disp >	
FwStatus MyFW_Erode3x3(TS* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	if (pSrcDst == NULL) return fwStsNullPtrErr;

	if (srcDstStep < 1) return fwStsStepErr;

	if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);
	TS min;
	TS* srcAddr;
	TS* dstAddr;

	bool isAC4 = false;

	if(channel == 5)	//AC4
	{
		channel = 4;
		isAC4 = true;
	}

	//if((2+roiSize.width)*channel > srcDstStep) return fwStsStrideErr;
	if((2+roiSize.width)*channel*(int)sizeof(TS) > srcDstStep) return fwStsStrideErr;

	//Create a temporary destination image ROI to save operation outcomes.
	TS* t_pDst = (TS*) fwMalloc(sizeof(TS) * (roiSize.height * roiSize.width * channel));

	if(t_pDst == NULL) return fwStsMemAllocErr;

	//Erode with 3 by 3 square mask.
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k = 0; k < channel; k++)
			{
				//In AC4(I)R image, the alpha channel is not processed.
				if(k == 3 && isAC4)
				{
					srcAddr = (TS*)((Fw8u*)pSrcDst+y*srcDstStep+x*channel+k);
					dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);

					*dstAddr = *srcAddr;
				}
				else
				{
					min = 255;

					for(int n = -1; n <= 1; n++)
					{
						for(int m = -1; m <= 1; m++)
						{
							srcAddr = (TS*)((Fw8u*)pSrcDst+(y+n)*srcDstStep+(x+m)*channel+k);

							if(*srcAddr < min)
							{
								min = *srcAddr;
							}
						}
					}

					dstAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);
					*dstAddr = min;
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
				srcAddr = (TS*)((Fw8u*)t_pDst+y*roiSize.width*channel+x*channel+k);
				dstAddr = (TS*)((Fw8u*)pSrcDst+y*srcDstStep+x*channel+k);

				*dstAddr=*srcAddr;
			}
		}
	}

	fwFree(t_pDst);
	return fwStsNoErr;
}


//Erode3x3_8u_C1R with Threading.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize) 
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

			//if(((2+roiSize.width)*channel > srcStep) || ((2+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) || (roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_Not_In_Place dilate3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num) -1; 
					}

					dilate3x3_par[thread_num].pSrc = pSrc;
					dilate3x3_par[thread_num].srcStep = srcStep;
					dilate3x3_par[thread_num].pDst = pDst;
					dilate3x3_par[thread_num].dstStep = dstStep;
					dilate3x3_par[thread_num].roiSize = roiSize;
					dilate3x3_par[thread_num].yBegin = y_bgn;
					dilate3x3_par[thread_num].yEnd = y_end;
					dilate3x3_par[thread_num].channel = channel;

					ThreadPool::Run(My_fwiErode3x3_8u_C1R, (void *) &dilate3x3_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize) 
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

			//if(((2+roiSize.width)*channel > srcStep) || ((2+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) || (roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_Not_In_Place dilate3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num) -1; 
					}

					dilate3x3_par[thread_num].pSrc = pSrc;
					dilate3x3_par[thread_num].srcStep = srcStep;
					dilate3x3_par[thread_num].pDst = pDst;
					dilate3x3_par[thread_num].dstStep = dstStep;
					dilate3x3_par[thread_num].roiSize = roiSize;
					dilate3x3_par[thread_num].yBegin = y_bgn;
					dilate3x3_par[thread_num].yEnd = y_end;
					dilate3x3_par[thread_num].channel = channel;

					ThreadPool::Run(My_fwiErode3x3_8u_C3R, (void *) &dilate3x3_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize) 
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

			//if(((2+roiSize.width)*channel > srcStep) || ((2+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) || (roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_Not_In_Place dilate3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num) -1; 
					}

					dilate3x3_par[thread_num].pSrc = pSrc;
					dilate3x3_par[thread_num].srcStep = srcStep;
					dilate3x3_par[thread_num].pDst = pDst;
					dilate3x3_par[thread_num].dstStep = dstStep;
					dilate3x3_par[thread_num].roiSize = roiSize;
					dilate3x3_par[thread_num].yBegin = y_bgn;
					dilate3x3_par[thread_num].yEnd = y_end;
					dilate3x3_par[thread_num].channel = channel;

					ThreadPool::Run(My_fwiErode3x3_8u_C4R, (void *) &dilate3x3_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_AC4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize) 
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

			//if(((2+roiSize.width)*channel > srcStep) || ((2+roiSize.width)*channel > dstStep)) return fwStsStrideErr;
			if(((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcStep) || (roiSize.width*channel*(int)sizeof(Fw8u) > dstStep)) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_Not_In_Place dilate3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					if(thread_num > 0)
					{
						//Because of the "analysis window", in each strip, the starting address should go back a little.
						y_bgn   = thread_num * (roiSize.height/total_thread_num) -1; 
					}

					dilate3x3_par[thread_num].pSrc = pSrc;
					dilate3x3_par[thread_num].srcStep = srcStep;
					dilate3x3_par[thread_num].pDst = pDst;
					dilate3x3_par[thread_num].dstStep = dstStep;
					dilate3x3_par[thread_num].roiSize = roiSize;
					dilate3x3_par[thread_num].yBegin = y_bgn;
					dilate3x3_par[thread_num].yEnd = y_end;
					dilate3x3_par[thread_num].channel = channel;

					ThreadPool::Run(My_fwiErode3x3_8u_AC4R, (void *) &dilate3x3_par[thread_num]);
				}

				ThreadPool::Wait();
			}

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, AC4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_AC4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, AC4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C1IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 1;

			//if((2+roiSize.width)*channel > srcDstStep) return fwStsStrideErr;
			if((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_In_Place erode3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					erode3x3_par[thread_num].pSrc = pSrcDst;
					erode3x3_par[thread_num].srcStep = srcDstStep;
					//erode3x3_par[thread_num].pDst = t_pDst;
					erode3x3_par[thread_num].dstStep = roiSize.width * channel;
					erode3x3_par[thread_num].roiSize = roiSize;
					erode3x3_par[thread_num].yBegin = y_bgn;
					erode3x3_par[thread_num].yEnd = y_end;
					erode3x3_par[thread_num].channel = channel;
					erode3x3_par[thread_num].remainBottomLines = remainBottomLines[thread_num];
					erode3x3_par[thread_num].remainTopLines = remainTopLines[thread_num];

					ThreadPool::Run(My_fwiErode3x3_8u_C1IR, (void *) &erode3x3_par[thread_num]);
				}

				ThreadPool::Wait();

				//copy the remain lines processed result back to pSrc
				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					ErodeCopyOneLine(remainTopLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yBegin) * srcDstStep, 
						roiSize.width, channel);
					ErodeCopyOneLine(remainBottomLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yEnd) * srcDstStep, 
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

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C1, DT_REFR> (pSrcDst, srcDstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C3IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 3;

			//if((2+roiSize.width)*channel > srcDstStep) return fwStsStrideErr;
			if((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_In_Place erode3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					erode3x3_par[thread_num].pSrc = pSrcDst;
					erode3x3_par[thread_num].srcStep = srcDstStep;
					//erode3x3_par[thread_num].pDst = t_pDst;
					erode3x3_par[thread_num].dstStep = roiSize.width * channel;
					erode3x3_par[thread_num].roiSize = roiSize;
					erode3x3_par[thread_num].yBegin = y_bgn;
					erode3x3_par[thread_num].yEnd = y_end;
					erode3x3_par[thread_num].channel = channel;
					erode3x3_par[thread_num].remainBottomLines = remainBottomLines[thread_num];
					erode3x3_par[thread_num].remainTopLines = remainTopLines[thread_num];

					ThreadPool::Run(My_fwiErode3x3_8u_C3IR, (void *) &erode3x3_par[thread_num]);
				}

				ThreadPool::Wait();

				//copy the remain lines processed result back to pSrc
				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					ErodeCopyOneLine_noCache(remainTopLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yBegin) * srcDstStep, 
						roiSize.width, channel);
					ErodeCopyOneLine_noCache(remainBottomLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yEnd) * srcDstStep, 
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

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C3, DT_REFR> (pSrcDst, srcDstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_C4IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 4;

			//if((2+roiSize.width)*channel > srcDstStep) return fwStsStrideErr;
			if((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_In_Place erode3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					erode3x3_par[thread_num].pSrc = pSrcDst;
					erode3x3_par[thread_num].srcStep = srcDstStep;
					//erode3x3_par[thread_num].pDst = t_pDst;
					erode3x3_par[thread_num].dstStep = roiSize.width * channel;
					erode3x3_par[thread_num].roiSize = roiSize;
					erode3x3_par[thread_num].yBegin = y_bgn;
					erode3x3_par[thread_num].yEnd = y_end;
					erode3x3_par[thread_num].channel = channel;
					erode3x3_par[thread_num].remainBottomLines = remainBottomLines[thread_num];
					erode3x3_par[thread_num].remainTopLines = remainTopLines[thread_num];

					ThreadPool::Run(My_fwiErode3x3_8u_C4IR, (void *) &erode3x3_par[thread_num]);
				}

				ThreadPool::Wait();

				//copy the remain lines processed result back to pSrc
				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					ErodeCopyOneLine(remainTopLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yBegin) * srcDstStep, 
						roiSize.width, channel);
					ErodeCopyOneLine(remainBottomLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yEnd) * srcDstStep, 
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

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, C4, DT_REFR> (pSrcDst, srcDstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_8u_AC4IR)(Fw8u* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
		case DT_SSE3:
		case DT_SSE2:
		{
			if (pSrcDst == NULL) return fwStsNullPtrErr;

			if (srcDstStep < 1) return fwStsStepErr;

			if (roiSize.height < 1 || roiSize.width < 1) return fwStsSizeErr;

			int channel = 4;

			//if((2+roiSize.width)*channel > srcDstStep) return fwStsStrideErr;
			if((2+roiSize.width)*channel*(int)sizeof(Fw8u) > srcDstStep) return fwStsStrideErr;

			/*
			The idea of threading is simple, almost separate the image into strips vertically. Then in each thread,
			process one strip. The number of strips depends on the number of thread: 2 CPU, 1 core each -> 2 threads.
			1 CPU, 2 core each -> 2 threads. 2 CPU, 2 core each -> 4 threads.
			*/
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			fwiErode3x3_8u_In_Place erode3x3_par[MAXTHREADNUM];

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
						y_end=roiSize.height-1;
					}

					erode3x3_par[thread_num].pSrc = pSrcDst;
					erode3x3_par[thread_num].srcStep = srcDstStep;
					//erode3x3_par[thread_num].pDst = t_pDst;
					erode3x3_par[thread_num].dstStep = roiSize.width * channel;
					erode3x3_par[thread_num].roiSize = roiSize;
					erode3x3_par[thread_num].yBegin = y_bgn;
					erode3x3_par[thread_num].yEnd = y_end;
					erode3x3_par[thread_num].channel = channel;
					erode3x3_par[thread_num].remainBottomLines = remainBottomLines[thread_num];
					erode3x3_par[thread_num].remainTopLines = remainTopLines[thread_num];

					ThreadPool::Run(My_fwiErode3x3_8u_AC4IR, (void *) &erode3x3_par[thread_num]);
				}

				ThreadPool::Wait();

				//copy the remain lines processed result back to pSrc
				for(thread_num=0; thread_num<total_thread_num; thread_num++)
				{
					ErodeCopyOneLine(remainTopLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yBegin) * srcDstStep, 
						roiSize.width, channel);
					ErodeCopyOneLine(remainBottomLines[thread_num], pSrcDst + (erode3x3_par[thread_num].yEnd) * srcDstStep, 
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

			return fwStsNoErr;
		}
		default:
			return MyFW_Erode3x3<Fw8u, AC4, DT_REFR> (pSrcDst, srcDstStep, roiSize);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C1IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C1, DT_REFR> (pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C3IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C3, DT_REFR> (pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_C4IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, C4, DT_REFR> (pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiErode3x3_32f_AC4IR)(Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize) 
{
	return MyFW_Erode3x3<Fw32f, AC4, DT_REFR> (pSrcDst, srcDstStep, roiSize);
}

#endif //BUILD_NUM_AT_LEAST
#endif
