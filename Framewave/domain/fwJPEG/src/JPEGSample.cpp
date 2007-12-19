/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the level shift functions from JPEG Chapter
//	fwiSampleDownH2V1_JPEG_8u_C1R
//  fwiSampleDownH2V2_JPEG_8u_C1R
//	fwiSampleDownRowH2V1_Box_JPEG_8u_C1R
//	fwiSampleDownRowH2V2_Box_JPEG_8u_C1R
//	fwiSampleUpH2V1_JPEG_8u_C1R
//	fwiSampleUpH2V2_JPEG_8u_C1R
//	fwiSampleUpRowH2V1_Triangle_JPEG_8u_C1R
//	fwiSampleUpRowH2V2_Triangle_JPEG_8u_C1R
//	fwiSampleDown444LS_MCU_8u16s_C3P3R
//	fwiSampleDown422LS_MCU_8u16s_C3P3R
//	fwiSampleDown411LS_MCU_8u16s_C3P3R
//	fwiSampleUp444LS_MCU_16s8u_C3P3R
//	fwiSampleUp422LS_MCU_16s8u_C3P3R
//	fwiSampleUp411LS_MCU_16s8u_C3P3R
//************************************************************************* 

#include "fwdev.h"
#include "fwJPEG.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )
#ifndef __JPEGSAMPLE
#define __JPEGSAMPLE
#define STEPCHECK(X, Y) if (X<=0 || Y<=0) return fwStsStepErr
#define ROISIZECHECK(X) if (X.height <=0 || X.width <=0) return fwStsSizeErr
#endif

//-----------------------------------------------------------------------
// This function handles the common case of downsampling pixel valiues of a 
// a single component for 2:1 horizontal and 1:1 vertical without smoothing.
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDownH2V1_JPEG_8u_C1R)(const Fw8u *pSrc, int srcStep,
				FwiSize srcSize, Fw8u *pDst, int dstStep, FwiSize dstSize)
{

	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(srcSize);
	ROISIZECHECK(dstSize);
	
	int numcol, numrow, x, y;
	unsigned short result, round;
	Fw8u *pSrcPtr, *pDstPtr;

	if ((srcSize.width&1)==1) srcSize.width--;
	numcol = dstSize.width -(srcSize.width>>1);
	numrow = dstSize.height - srcSize.height;
	if (numcol > 0) dstSize.width = (srcSize.width >>1);
	if (numrow > 0) dstSize.height= srcSize.height;

	for (y=0;y<dstSize.height;y++) {
		pSrcPtr = (Fw8u *)pSrc+y*srcStep;
		pDstPtr = pDst+y*dstStep;
		round=0;
		for (x=0;x<dstSize.width;x++) {
			 result = pSrcPtr[0]+pSrcPtr[1]+round;
			 *(pDstPtr++) = FW_REF::Limits<U8>::Sat(result>>1);
			 round ^=1; //alternate rounding
			 pSrcPtr+=2;
		}

		if (numcol > 0) {// if dstSize is larger than srcSize
			for (;x<dstSize.width+numcol;x++) {
				*(pDstPtr++) = *(pSrcPtr-1);
			}
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function handles the common case of downsampling pixel valiues of a 
// a single component for 2:1 horizontal and 2:1 vertical without smoothing.
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDownH2V2_JPEG_8u_C1R)(const Fw8u *pSrc, int srcStep,
				FwiSize srcSize, Fw8u *pDst, int dstStep, FwiSize dstSize)
{

	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(srcSize);
	ROISIZECHECK(dstSize);
	
	int numcol, numrow, x, y;
	unsigned short result, round;
	Fw8u *pSrcPtr, *srcPtr1;
	Fw8u *pDstPtr;

	if ((srcSize.width&1)==1) srcSize.width--;
	if ((srcSize.height&1)==1) srcSize.height--;
	numcol = dstSize.width -(srcSize.width>>1);
	numrow = dstSize.height-(srcSize.height>>1);
	if (numcol > 0) dstSize.width = (srcSize.width >>1);
	if (numrow > 0) dstSize.height= (srcSize.height>>1);

	for (y=0;y<dstSize.height;y++) {
		pSrcPtr = (Fw8u *)pSrc+2*y*srcStep;
		srcPtr1 = (Fw8u *)pSrc+(2*y+1)*srcStep;
		pDstPtr = pDst+y*dstStep;
		round=1;
		for (x=0;x<dstSize.width;x++) {
			 result = pSrcPtr[0]+pSrcPtr[1]+srcPtr1[0]+srcPtr1[1]+round;
			 *(pDstPtr++) = FW_REF::Limits<U8>::Sat(result>>2);
			 round ^=3; //alternate rounding
			 pSrcPtr+=2;
			 srcPtr1+=2;
		}

		if (numcol > 0) {// if dstSize is larger than srcSize
			result = *(pSrcPtr-1) + *(srcPtr1-1);
			result >>= 1;
			for (;x<dstSize.width+numcol;x++) {
				*(pDstPtr++) = (Fw8u)result;
			}
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function handles one image row of downsampling pixel valiues of a 
// a single component for 2:1 horizontal and 1:1 vertical without smoothing.
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDownRowH2V1_Box_JPEG_8u_C1)(const Fw8u *pSrc,int srcWidth, 
							Fw8u *pDst)
{

	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	if (srcWidth <=0) return fwStsSizeErr;
	
	int x;
	unsigned short result, round;
	Fw8u *pSrcPtr;
	Fw8u *pDstPtr;

	if ((srcWidth&1)==1) srcWidth--;

	pSrcPtr = (Fw8u *)pSrc;
	pDstPtr = pDst;
	round=0;
	for (x=0;x<(srcWidth>>1);x++) {
		result = pSrcPtr[0]+pSrcPtr[1]+round;
		*(pDstPtr++) = FW_REF::Limits<U8>::Sat(result>>1);
		round ^=1; //alternate rounding
		pSrcPtr+=2;
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function handles an image row of downsampling pixel valiues of a 
// a single component for 2:1 horizontal and 2:1 vertical without smoothing.
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDownRowH2V2_Box_JPEG_8u_C1)(const Fw8u *pSrc1, 
							const Fw8u *pSrc2, int srcWidth, Fw8u *pDst)
{

	if (pSrc1 == 0 || pSrc2 == 0 || pDst==0) 
		return fwStsNullPtrErr;
	if (srcWidth <=0) return fwStsSizeErr;
	
	int x;
	unsigned short result, round;
	Fw8u *pSrcPtr, *srcPtr1;
	Fw8u *pDstPtr;

	if ((srcWidth&1)==1) srcWidth--;

	pSrcPtr = (Fw8u *)pSrc1;
	srcPtr1 = (Fw8u *)pSrc2;
	pDstPtr = pDst;
	round=1;

	for (x=0;x<(srcWidth>>1);x++) {
		result = pSrcPtr[0]+pSrcPtr[1]+srcPtr1[0]+srcPtr1[1]+round;
		*(pDstPtr++) = FW_REF::Limits<U8>::Sat(result>>2);
		round ^=3; //alternate rounding
		pSrcPtr+=2;
		srcPtr1+=2;
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function processes the common case of upsampling pixel valiues of a 
// a single component for 1:2 horizontal and 1:1 vertical. The upsampling 
// algorithm is linear interpolation between pixel centers, which the centers 
// of the output pixels are 1/4 and 3/4 of the way between input pixel centers.
//
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUpH2V1_JPEG_8u_C1R)(const Fw8u *pSrc, int srcStep,
				FwiSize srcSize, Fw8u *pDst, int dstStep, FwiSize dstSize)
{

	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(srcSize);
	ROISIZECHECK(dstSize);
	
	int numcol, numrow, x, y;
	unsigned short result;
	Fw8u *pSrcPtr;
	Fw8u *pDstPtr;

	numcol = dstSize.width -(srcSize.width<<1);
	numrow = dstSize.height - srcSize.height;

	if (numcol < 0) {
		// we don't need too much sample due to 
		// destination size.
		srcSize.width = dstSize.width >> 1;
	}
	//dstSize.height > srcSize.height
	if (numrow > 0) dstSize.height= srcSize.height;

	for (y=0;y<dstSize.height;y++) {
		pSrcPtr = (Fw8u *)pSrc+y*srcStep;
		pDstPtr = pDst+y*dstStep;

		//special case for first element
		result = *pSrcPtr++;
		*pDstPtr++ = (Fw8u) result;
		//rounding add 2.
		//3/4 *nearer pixel, 1/4 *further pixel
		*pDstPtr++ = (Fw8u)((result*3+ *pSrcPtr +2)>>2);

		for (x=0;x<srcSize.width-2;x++) {
			result = (*pSrcPtr++)*3;
			//first rounding is 1
			*(pDstPtr++) = (Fw8u)((result+pSrcPtr[-2]+1)>>2);
			//second rounding is 2
			*(pDstPtr++) = (Fw8u)((result+*pSrcPtr+2)>>2);
		}

		//Special case for last element
		result = *pSrcPtr;
		*(pDstPtr++) = (Fw8u)((result*3+pSrcPtr[-1]+1)>>2);
		*(pDstPtr++) = (Fw8u)(result);

		// if dstSize.width is larger than srcSize.width*2
		if (numcol > 0) {
			for (x=srcSize.width*2;x<dstSize.width;x++) {
				*(pDstPtr++) = (Fw8u)(result);
			}
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function processes the common case of upsampling pixel valiues of a 
// a single component for 1:2 horizontal and 1:2 vertical. The upsampling 
// algorithm is linear interpolation between pixel centers, which the centers 
// of the output pixels are 1/4 and 3/4 of the way between input pixel centers.
//
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUpH2V2_JPEG_8u_C1R)(const Fw8u *pSrc, int srcStep,
				FwiSize srcSize, Fw8u *pDst, int dstStep, FwiSize dstSize)
{

	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(srcSize);
	ROISIZECHECK(dstSize);
	
	int numcol, numrow, x, y, i;
	unsigned short current, next, last;
	Fw8u result;
	Fw8u *pSrcPtr, *srcPtr1;
	Fw8u *pDstPtr;

	numcol = dstSize.width -(srcSize.width<<1);
	numrow = dstSize.height - (srcSize.height<<1);

	if (numcol < 0) {
		// we don't need too much sample due to 
		// destination size.
		srcSize.width = (dstSize.width >> 1);
	}
	//dstSize.height > srcSize.height
	if (numrow > 0) dstSize.height= (srcSize.height<<1);
	else if (numrow < 0) srcSize.height = (dstSize.height >>1);

	for (y=0;y<srcSize.height;y++) {
		for (i=0; i<2; i++) {
			pSrcPtr = (Fw8u *)pSrc+y*srcStep;
			//We keep the srcPtr1 to be inside the Roi Region
			//Opensource might require the outside ROI boundary values
			if (i==0) {
				if (y==0) {
					srcPtr1=(Fw8u *)pSrc+y*srcStep;
				} else {
					srcPtr1=(Fw8u *)pSrc+(y-1)*srcStep;
				}
			} else { //i=1
				if (y==srcSize.height-1) {
					srcPtr1=(Fw8u *)pSrc+y*srcStep;
				} else {
					srcPtr1=(Fw8u *)pSrc+(y+1)*srcStep;
				}
			}
			pDstPtr = pDst+(2*y+i)*dstStep;

			//special case for first element
			current = (*pSrcPtr++)*3 + (*srcPtr1);
			next    = (*pSrcPtr++)*3 + (*srcPtr1++);
			*pDstPtr++ = (Fw8u) ((current*4+8)>>4);
			*pDstPtr++ = (Fw8u) ((current*3+next+7)>>4);
			last = current; 
			current = next;

			for (x=0;x<srcSize.width-2;x++) {
				//General case, 3/4*nearest+1/4* further pixel in 
				//each dimension, so we have 9/16, 3/16, 3/16 and 1/16
				//for adjacent pixels
				next = (*pSrcPtr++)*3+(*srcPtr1++);
				*pDstPtr++ = (Fw8u) ((current*3+last+8)>>4);
				*pDstPtr++ = (Fw8u) ((current*3+next+7)>>4);
				last = current; 
				current = next;
			}

			//Special case for last element
			*pDstPtr++ = (Fw8u) ((current*3+last+8)>>4);
			*pDstPtr++ = (Fw8u) ((current*4+7)>>4);

			// if dstSize.width is larger than srcSize.width*2
			if (numcol > 0) {
				result = (Fw8u) ((current*4+7)>>4);
				for (x=srcSize.width*2;x<dstSize.width;x++) {
					*(pDstPtr++) = (result);
				}
			}
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function processes an image row of upsampling pixel valiues of a 
// a single component for 1:2 horizontal and 1:1 vertical. The upsampling 
// algorithm is linear interpolation between pixel centers, which the centers 
// of the output pixels are 1/4 and 3/4 of the way between input pixel centers.
//
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUpRowH2V1_Triangle_JPEG_8u_C1)(const Fw8u *pSrc, 
							int srcWidth, Fw8u *pDst)
{
	if (pSrc == 0 || pDst==0) return fwStsNullPtrErr;
	if (srcWidth <=0) return fwStsSizeErr;
	
	int x;
	unsigned short result;
	Fw8u *pSrcPtr= (Fw8u *)pSrc;
	Fw8u *pDstPtr=pDst;

	//special case for first element
	result = *pSrcPtr++;
	*pDstPtr++ = (Fw8u) result;
	//rounding add 2.
	//3/4 *nearer pixel, 1/4 *further pixel
	*pDstPtr++ = (Fw8u)((result*3+ *pSrcPtr +2)>>2);

	for (x=0;x<srcWidth-2;x++) {
		result = (*pSrcPtr++)*3;
		//first rounding is 1
		*(pDstPtr++) = (Fw8u)((result+pSrcPtr[-2]+1)>>2);
		//second rounding is 2
		*(pDstPtr++) = (Fw8u)((result+*pSrcPtr+2)>>2);
	}

	//Special case for last element
	result = *pSrcPtr;
	*(pDstPtr++) = (Fw8u)((result*3+pSrcPtr[-1]+1)>>2);
	*(pDstPtr++) = (Fw8u)(result);

	return fwStsNoErr;
}


//-----------------------------------------------------------------------
// This function processes an image row of upsampling pixel valiues of a 
// a single component for 1:2 horizontal and 1:2 vertical. The upsampling 
// algorithm is linear interpolation between pixel centers, which the centers 
// of the output pixels are 1/4 and 3/4 of the way between input pixel centers.
//
// The rounding method is a simple ordered dither pattern. (It means 0.5 will 
// be rounded up or down at alternate pixel locations.)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUpRowH2V2_Triangle_JPEG_8u_C1)(const Fw8u *pSrc1, 
				const Fw8u *pSrc2, int srcWidth, Fw8u *pDst)
{

	if (pSrc1 == 0 || pSrc1 == 0 || pDst==0) 
		return fwStsNullPtrErr;
	if (srcWidth <=0) return fwStsSizeErr;
	
	int x;
	unsigned short current, next, last;
	Fw8u *pSrcPtr, *srcPtr1;
	Fw8u *pDstPtr;

	pSrcPtr  = (Fw8u *)pSrc1;
	srcPtr1 = (Fw8u *)pSrc2;
	pDstPtr  = pDst;

	//special case for first element
	current = (*pSrcPtr++)*3 + (*srcPtr1);
	next    = (*pSrcPtr++)*3 + (*srcPtr1++);
	*pDstPtr++ = (Fw8u) ((current*4+8)>>4);
	*pDstPtr++ = (Fw8u) ((current*3+next+7)>>4);
	last = current; 
	current = next;

	for (x=0;x<srcWidth-2;x++) {
		//General case, 3/4*nearest+1/4* further pixel in 
		//each dimension, so we have 9/16, 3/16, 3/16 and 1/16
		//for adjacent pixels
		next = (*pSrcPtr++)*3+(*srcPtr1++);
		*pDstPtr++ = (Fw8u) ((current*3+last+8)>>4);
		*pDstPtr++ = (Fw8u) ((current*3+next+7)>>4);
		last = current; 
		current = next;
	}

	//Special case for last element
	*pDstPtr++ = (Fw8u) ((current*3+last+8)>>4);
	*pDstPtr++ = (Fw8u) ((current*4+7)>>4);

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
// This function creates 4XX MCU with Level shift from source pixel array 
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDown444LS_MCU_8u16s_C3P3R)(const Fw8u *pSrc, 
							int srcStep, Fw16s *pDstMCU[3])
{
	if (pSrc == 0 || pDstMCU==0) return fwStsNullPtrErr;	
	if (pDstMCU[0]==0 || pDstMCU[1]==0 || pDstMCU[2]==0)
			return fwStsNullPtrErr;
	if (srcStep <=0) return fwStsStepErr;

	int x, y, index;
	Fw8u *pSrcPtr;

	for (y=0;y<8;y++) {
		pSrcPtr = (Fw8u *)pSrc+y*srcStep;
		for (x=0;x<8;x++) {
			index = x+y*8;
			pDstMCU[0][index]=(*pSrcPtr++)-128;
			pDstMCU[1][index]=(*pSrcPtr++)-128;
			pDstMCU[2][index]=(*pSrcPtr++)-128;
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDown422LS_MCU_8u16s_C3P3R)(const Fw8u *pSrc, 
							int srcStep, Fw16s *pDstMCU[3])
{
	if (pSrc == 0 || pDstMCU==0) return fwStsNullPtrErr;	
	if (pDstMCU[0]==0 || pDstMCU[1]==0 || pDstMCU[2]==0)
			return fwStsNullPtrErr;
	if (srcStep <=0) return fwStsStepErr;

	int x, y, index;
	Fw8u *pSrcPtr;

	for (y=0;y<16;y++) {
		pSrcPtr = (Fw8u *)pSrc+y*srcStep;
		for (x=0;x<8;x++) {
			index = x+y*8;
			pDstMCU[0][index]=(*pSrcPtr++)-128;
			if ((x&1)==0) {
				index= x/2+y*4;
				pDstMCU[1][index]=(*pSrcPtr++)-128;
				pDstMCU[2][index]=(*pSrcPtr++)-128;
			}
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleDown411LS_MCU_8u16s_C3P3R)(const Fw8u *pSrc, 
							int srcStep, Fw16s *pDstMCU[3])
{
	if (pSrc == 0 || pDstMCU==0) return fwStsNullPtrErr;	
	if (pDstMCU[0]==0 || pDstMCU[1]==0 || pDstMCU[2]==0)
			return fwStsNullPtrErr;
	if (srcStep <=0) return fwStsStepErr;

	int x, y, index;
	Fw8u *pSrcPtr;

	for (y=0;y<32;y++) {
		pSrcPtr = (Fw8u *)pSrc+y*srcStep;
		for (x=0;x<8;x++) {
			index = x+y*8;
			pDstMCU[0][index]=(*pSrcPtr++)-128;
			if ((x&1)==0) {
				index= x/4+y*2;
				pDstMCU[1][index]=(*pSrcPtr++)-128;
				pDstMCU[2][index]=(*pSrcPtr++)-128;
			}
		}
	}

	return fwStsNoErr;
}
//-----------------------------------------------------------------------
// This function creates interleaved pixel-order image from 4XX MCU. 
// Data was converted from Fw16s to Fw8u by level shift.
//-----------------------------------------------------------------------
void fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (
	const Fw16s *pSrcMCU[3], Fw8u *pDst, int dstStep, int blocknum,
	int xshift, int yshift)
{
	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	int ysrcPos, csrcPos, coffset, cPos, doffset, dstPos;

	ysrcPos = blocknum<<6;//*64
	coffset = ((blocknum&0x1)<<2) + ((blocknum&0x2)<<4);
	doffset = ((blocknum&0x1)<<3) * 3 + ((blocknum&0x2)<<2)* dstStep;

	for (y=0; y<8; y++) {
		csrcPos = (y>>yshift)*8 + coffset;
		dstPos = y*dstStep + doffset;
		for (x=0;x<8;x++) {
			//add 128 for each element of YCCK
			cPos = csrcPos + (x>>xshift);
			pDst[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[0][ysrcPos++] + 128);
			pDst[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[1][cPos] + 128);
			pDst[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcMCU[2][cPos] + 128);
		}
	}

	return;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUp444LS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
							Fw8u *pDst, int dstStep)
{
	if (pDst == 0 || pSrcMCU==0) return fwStsNullPtrErr;	
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
			return fwStsNullPtrErr;
	if (dstStep <=0) return fwStsStepErr;

	// Y:			    	Cb:			Cr:				RGB:
	//		  ----8----	     ---8---	---8----	==>	  --8--
	//		8 |   Ay   |	8 |A_cb ||	8 |A_cr |	==>	8 | A |
	//		  ---------	    ---------	--------	==>	  -----
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 0, 0, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUp422LS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
							Fw8u *pDst, int dstStep)
{
	if (pDst == 0 || pSrcMCU==0) return fwStsNullPtrErr;	
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
			return fwStsNullPtrErr;
	if (dstStep <=0) return fwStsStepErr;

	// Y:				Cb:				Cr:					RGB:
	//		  ----8----	     ---4-----4---	---4-----4---	==>	  --8---8--
	//		8 |   Ay   |	8 |A_cb | B_cb|	8 |A_cr | B_cr|	==>	8 | A | B |
	//		8 |   By   |	-------------	-------------	==>	  ---------
	//		  ----------
	// Proces each block independently
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 0, 1, 0);
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 1, 1, 0);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSampleUp411LS_MCU_16s8u_P3C3R)(const Fw16s *pSrcMCU[3],
							Fw8u *pDst, int dstStep)
{
	if (pDst == 0 || pSrcMCU==0) return fwStsNullPtrErr;	
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
			return fwStsNullPtrErr;
	if (dstStep <=0) return fwStsStepErr;
		
	// Y:				CbCr:								RGB:
	//		  ----8----		  ---4-----4---		  	==>			  --8---8--
	//		8 |   Ay   |		4 |A_c | B_c|		==>			8 | A | B |
	//		8 |   By   |		  -------------		==>			  ---------
	//		8 |   Cy   |		4 |C_c | D_c|		==>			8 | C | D |
	//		8 |   Dy   |		  -------------		==>			  ---------
	//		  ----------
	// Proces each block independently
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 0, 1, 1);
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 1, 1, 1);
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 2, 1, 1);
	fwiSampleUp4xxLS_MCU_16s8u_P3C3R_8x8Block (pSrcMCU, pDst, dstStep, 3, 1, 1);

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR 
