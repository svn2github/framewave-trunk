/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )

#pragma warning( disable: 4244 )

#ifndef __RESIZE_50
#define __RESIZE_50
#define	FW_WEIGHT	7
#endif //global define guard

struct Linear_Array {
	int floor;
	int ceil;
	short ifraction;
	short ione_Minus_Val;
};

struct  Linear_Array_Double{
	int floor;
	int ceil;
	double ifraction;
	double ione_Minus_Val;
};

struct Cubic_Array_Double{
	int pos[4];
	double  icoeff[4];
};

//static void BilinearInterpolation(const Fw8u *pSrc, Fw8u *pDst, int dstStep, int resizeWidth, int resizeHeight, 
//						   Linear_Array *pX_Array_Value, Linear_Array *pY_Array_Value, 
//						   FwiSize dstRoiSize, int dsrcRoiwidth, int dsrcRoiheight,
//						   double xFactor, double yFactor, int yBegin, int yEnd, int dyBegin, int dyEnd);

FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeYUV422_8u_C2R)(const Fw8u *pSrc, FwiSize srcSize, 
														  int srcStep, FwiRect srcRoi, Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
														  double xFactor, double yFactor, int interpolation)
{
	if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR &&
		interpolation != FWI_INTER_CUBIC && interpolation != FWI_INTER_SUPER) 
		return fwStsInterpolationErr;

	if (pSrc == 0 || pDst == 0)
		return fwStsNullPtrErr;

	if (srcSize.height <= 0 || srcSize.width <= 0 || dstRoiSize.height <= 0 || dstRoiSize.width< 2 )	
		return fwStsSizeErr;	

	//guarantee the srcRoi start with YU format
	if (((srcRoi.width & 1)==1) || (srcRoi.x & 1)==1 ||((srcSize.width & 1)==1)) return fwStsSizeErr;

	if (srcStep <= 0 || dstStep <= 0)	
		return fwStsStepErr;	

	if (xFactor <= 0.0 || yFactor <= 0.0)	
		return fwStsResizeFactorErr;	

	// Initial Calculations
	int x, y;
	int resizeHeight = (int)((double)srcRoi.height * yFactor);
	int resizeWidth = (int)((double)srcRoi.width  * xFactor);
	if (resizeHeight <1 || resizeWidth < 1) return fwStsResizeNoOperationErr;

	if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
	if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;	

	int dstWidth = dstRoiSize.width, dstHeight = dstRoiSize.height;
	Fw8u *pTempDst = pDst, *pEndDst;

	if(dstWidth > resizeWidth)
	{
		int offsetWidth = (dstWidth - resizeWidth) * 2; // 2 bytes per pixel
		for (y = 0; y < dstHeight; y++)
		{
			pTempDst = pDst + resizeWidth*2 + y*dstStep; 
			pEndDst = pTempDst + offsetWidth;
			for (; pTempDst != pEndDst; pTempDst++)
			{
				*pTempDst++ = 0;
				*pTempDst = 0x80;
			}
		}
	}
	if(dstHeight > resizeHeight)
	{ 
		for (y = resizeHeight; y < dstHeight; y++)
		{
			pTempDst = pDst + y*dstStep; 
			pEndDst = pTempDst + dstWidth*2;// 2 bytes per pixel
			for (; pTempDst != pEndDst; pTempDst++)
			{
				*pTempDst++ = 0;
				*pTempDst = 0x80;
			}
		}
	}

	// Interpolation Algorithms
	switch (interpolation)
	{
	case FWI_INTER_NN:
		{ // Brackets Needed to be able to declare variables
			int py_NN;
			int *px_NN;

			px_NN=(int *)fwMalloc (sizeof(int)*resizeWidth*2); // 2 bytes per pixel

			// precalculate the x value 
			int tempX_Pixel;
			for (x=0;x<resizeWidth*2;x++)
			{
				//tempX_Pixel = (int)(((double)(x/2)/xFactor+0.5)+srcRoi.x) * 2;
				tempX_Pixel = (int)(((double)(x/2)/xFactor)+srcRoi.x) * 2;
				switch (x % 4)
				{
				case 1: // U0
					px_NN[x] = tempX_Pixel - (tempX_Pixel % 4) + 1; // 0*23|0*23 (We want the 2nd pixel)
					break;
				case 3: // V0
					px_NN[x] = tempX_Pixel - (tempX_Pixel % 4) + 3;
					break;
				case 0: // Y0
				case 2: // Y1
				default:
					px_NN[x] = tempX_Pixel;
					break;
				}
			}

			for (y=0; y< resizeHeight;y++)
			{
				//py_NN=(int)((double)y/yFactor+0.5)+srcRoi.y;
				py_NN=(int)((double)y/yFactor)+srcRoi.y;
				pTempDst = pDst + y*dstStep; 
				pEndDst = pTempDst + resizeWidth * 2; // 2 bytes per pixel
				for (x=0; pTempDst != pEndDst; pTempDst++, x++)
				{
					*pTempDst = *(pSrc+px_NN[x]+py_NN*srcStep);
				}
			}

			fwFree (px_NN);
		}
		break;

	case FWI_INTER_LINEAR:
		{
			//use fwMalloc instead of malloc for aligned address
			Linear_Array_Double *pX_Array_Value = (Linear_Array_Double*)fwMalloc(resizeWidth*2*sizeof(Linear_Array_Double));
			Linear_Array_Double *pY_Array_Value = (Linear_Array_Double*)fwMalloc(resizeHeight*sizeof(Linear_Array_Double));

			int ceil_X, ceil_Y, floor_X, floor_Y;
			double fraction_X, fractY, one_Minus_X, one_Minus_Y;
			double t1, t2, p1, p2, p3, p4;
			Fw8u *pSrc_FloorY, *pSrc_CeilY;

			//Pre-calculate the x coefficient.
			for (x = 0; x < resizeWidth*2; x++ ){
				//floor_X = (int)(((double)x/2)/xFactor) * 2;
				//fraction_X = ((double)x/2)/xFactor - (int)(floor_X / 2);
				fraction_X=(double)(x/2)/xFactor;
				floor_X = (int) fraction_X;
				fraction_X=fraction_X-floor_X;

				switch (x % 4)
				{
				case 1: // U0
					//floor_X = floor_X - (floor_X % 4) + 1; // 0*23|0*23 (We want the 2nd pixel)
					if ((floor_X & 1) == 0) {
						//floor_X is even
						floor_X = floor_X*2+1;
						fraction_X=fraction_X/2;
					} else {
						//floor_X is odd
						floor_X = floor_X*2-1;
						fraction_X=(1+fraction_X)/2;
					}

					ceil_X = floor_X + 4; // Next U-component		
					break;

				case 3: // V0
					//floor_X = floor_X - (floor_X % 4) + 3;
					if ((floor_X & 1) == 1) {
						//floor_X is odd
						floor_X = floor_X*2+1;
						fraction_X=fraction_X/2;
					} else {
						//floor_X is even
						floor_X = floor_X*2-1;
						fraction_X=(1+fraction_X)/2;
					}

					ceil_X = floor_X + 4; // Next V-component

					break;
				case 0: // Y0
				case 2: // Y1
				default:
					floor_X = floor_X*2;
					ceil_X = floor_X + 2; // Next Y-component
					break;
				}

				one_Minus_X = 1.0 - fraction_X;
				//Protection for over-boundary reading
				if (ceil_X >= srcRoi.width*2)
					ceil_X = floor_X;
				floor_X += srcRoi.x*2;
				ceil_X += srcRoi.x*2;

				pX_Array_Value[x].floor=floor_X;
				pX_Array_Value[x].ceil=ceil_X;
				pX_Array_Value[x].ifraction=fraction_X;
				pX_Array_Value[x].ione_Minus_Val=one_Minus_X;
			}

			//Pre-calculate the y coefficient.
			for (y = 0; y < resizeHeight; y++)
			{
				floor_Y = (int)((double)y / yFactor);

				fractY = (double)y/yFactor - floor_Y;
				one_Minus_Y = 1.0 - fractY;

				ceil_Y = floor_Y + 1;
				//Protection for over-boundary reading
				if (ceil_Y >= srcRoi.height)
					ceil_Y = floor_Y;
				floor_Y += srcRoi.y;
				ceil_Y+= srcRoi.y;

				pY_Array_Value[y].floor=floor_Y*srcStep;
				pY_Array_Value[y].ceil=ceil_Y*srcStep;
				pY_Array_Value[y].ifraction=fractY;
				pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;
			}

			//To calculate the pixel value
			for (y = 0; y < resizeHeight; y++)
			{
				pSrc_CeilY  = (Fw8u*)pSrc;
				pSrc_FloorY = (Fw8u*)pSrc;
				pSrc_CeilY  += pY_Array_Value[y].ceil;
				pSrc_FloorY += pY_Array_Value[y].floor;

				fractY = pY_Array_Value[y].ifraction;
				one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

				pTempDst = pDst + y*dstStep; 
				pEndDst = pTempDst + resizeWidth * 2; // 2 bytes per pixel
				for (x=0; pTempDst != pEndDst; pTempDst++, x++)
				{			
					ceil_X =pX_Array_Value[x].ceil;
					floor_X=pX_Array_Value[x].floor;
					fraction_X  = pX_Array_Value[x].ifraction;
					one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

					// resize
					// Two Channel Bytes
					p1 = (double)*(pSrc_FloorY + floor_X);
					p2 = (double)*(pSrc_FloorY + ceil_X);
					p3 = (double)*(pSrc_CeilY  + floor_X);
					p4 = (double)*(pSrc_CeilY  + ceil_X);

					//line y
					t1 = one_Minus_X * p1 + fraction_X * p2;
					//line y+1
					t2 = one_Minus_X * p3 + fraction_X * p4;

					*pTempDst = (Fw8u)(one_Minus_Y*t1 + fractY*t2);
				}
			}

			//free allocated memory by fwMalloc
			fwFree(pX_Array_Value);
			fwFree(pY_Array_Value);

		}
		break;

	case FWI_INTER_CUBIC:
		{
			//use fwMalloc instead of malloc for aligned address
			Cubic_Array_Double *pX_Array_Value = (Cubic_Array_Double*) fwMalloc(resizeWidth 
				* sizeof(Cubic_Array_Double) *2); //2 bytes per pixel
			Cubic_Array_Double *pY_Array_Value = (Cubic_Array_Double*) fwMalloc(resizeHeight 
				* sizeof(Cubic_Array_Double));

			// pixel mapping and coefficient generation
			int l1, l2, r1, r2, x, y;
			double p[4];
			double aA=-0.5; 

			for (x = 0; x < resizeWidth*2; x++) {
				double fx = (double)(x/2)/xFactor;
				l1 = (int) fx;
				double px=fx-l1;

				switch (x % 4) 
				{
				case 1: // U0
					if ((l1&1)==0) {//l1 is even number
						l1=2*l1+1;
						px=-1-(px/2); //distant of pos 0 to mapping pos 
					} else {//l1 is odd number
						l1=2*l1-1;
						px=-1-(1+px)/2;
					}

					l2=l1-4;
					r1=l1+4;
					r2=l1+8;
					break;
				case 3: // V0
					if ((l1&1)==1) {//l1 is odd number
						l1=2*l1+1;
						px=-1-(px/2); //distant of pos 0 to mapping pos 
					} else {//l1 is even number
						l1=2*l1-1;
						px=-1-(1+px)/2;
					}

					l2=l1-4;
					r1=l1+4;
					r2=l1+8;
					break;
				case 0:
				case 2:
				default:
					l1=l1*2;
					px=-1-px;
					l2=l1-2;
					r1=l1+2;
					r2=l1+4;
				}

				if(l2 < 0)l2 = l1;
				//Protection for over-boundary reading
				if (r1 >= srcRoi.width*2)
					r1 = r2 = l1;
				else if (r2 >= srcRoi.width*2) {
					r2 = r1;
				}

				pX_Array_Value[x].pos[0] = l2;
				pX_Array_Value[x].pos[1] = l1;
				pX_Array_Value[x].pos[2] = r1;
				pX_Array_Value[x].pos[3] = r2;

				// BC-cubic spline
				for (int i=0; i<4; i++) {
					double hh;
					double xx = px;

					//following the Lanczos formula to calculate the coefficients
					if (xx < 0.0) xx = -xx; // x = |x|

					if (xx < 1.0) {
						hh = (aA + 2.0)*xx*xx*xx-(aA+3.0)*xx*xx + 1;	// 0 <= |x| < 1
					} else if (xx < 2.0) {
						hh =  aA*(xx*xx*xx-5.0*xx*xx + 8*xx - 4.0); // 1<= |x| < 2
					} else {			
						hh = 0.0;								// else						
					}

					pX_Array_Value[x].icoeff[i] = hh;

					px += 1.0;
				}
				//protect overflow
				pX_Array_Value[x].icoeff[1] =  1.0 - pX_Array_Value[x].icoeff[0]
				- pX_Array_Value[x].icoeff[2] - pX_Array_Value[x].icoeff[3];
				//for safety

				pX_Array_Value[x].pos[0] += srcRoi.x*2;
				pX_Array_Value[x].pos[1] += srcRoi.x*2;
				pX_Array_Value[x].pos[2] += srcRoi.x*2;
				pX_Array_Value[x].pos[3] += srcRoi.x*2;
			}

			// loop over all points of Y
			for (y = 0; y < resizeHeight; y++) {
				l1 = (int)((double)y / yFactor);
				l2 = l1 -1;
				if(l2 < 0) l2 = l1;
				r1 = l1 + 1;
				//Protection for over-boundary reading
				if (r1 >= srcRoi.height)
					r1 = r2 = l1;
				else {
					r2 = r1 +1;
					if (r2 >= srcRoi.height) r2 = r1;
				}

				pY_Array_Value[y].pos[0] = l2;
				pY_Array_Value[y].pos[1] = l1;
				pY_Array_Value[y].pos[2] = r1;
				pY_Array_Value[y].pos[3] = r2;

				double fy = (double)y/yFactor;

				// BC-cubic spline
				double py = (double) pY_Array_Value[y].pos[1] - fy - 1.0; 

				for (int i=0; i<4; i++){
					double hh;
					double yy = py;

					//following the Lanczos formula to calculate y direction coefficients
					if (yy < 0.0) yy = -yy; // y = |y|
					if (yy < 1.0) {
						hh = (aA + 2.0)*yy*yy*yy-(aA+3.0)*yy*yy + 1;	// 0 <= |y| < 1
					} else if (yy < 2.0) {
						hh =  aA*(yy*yy*yy-5.0*yy*yy + 8*yy - 4.0); // 1<= |y| < 2
					} else {
						hh = 0.0;								// else						
					}

					pY_Array_Value[y].icoeff[i] = hh;

					py += 1.0;
				}
				//protect overflow
				pY_Array_Value[y].icoeff[1] =  1.0 - pY_Array_Value[y].icoeff[0]
				- pY_Array_Value[y].icoeff[2] - pY_Array_Value[y].icoeff[3]; //for safety

				pY_Array_Value[y].pos[0] = (l2 + srcRoi.y) * srcStep;
				pY_Array_Value[y].pos[1] = (l1 + srcRoi.y) * srcStep;
				pY_Array_Value[y].pos[2] = (r1 + srcRoi.y) * srcStep;
				pY_Array_Value[y].pos[3] = (r2 + srcRoi.y) * srcStep;
			}

			//Calculating the pixel value
			for (y = 0; y < resizeHeight; y++) {
				int ypos0 = pY_Array_Value[y].pos[0];
				int ypos1 = pY_Array_Value[y].pos[1];
				int ypos2 = pY_Array_Value[y].pos[2];
				int ypos3 = pY_Array_Value[y].pos[3];

				double iycoef0 = pY_Array_Value[y].icoeff[0];
				double iycoef1 = pY_Array_Value[y].icoeff[1];
				double iycoef2 = pY_Array_Value[y].icoeff[2];
				double iycoef3 = pY_Array_Value[y].icoeff[3];

				for (x = 0; x < resizeWidth*2; x++) {
					int xpos0 = pX_Array_Value[x].pos[0];
					int xpos1 = pX_Array_Value[x].pos[1];
					int xpos2 = pX_Array_Value[x].pos[2];
					int xpos3 = pX_Array_Value[x].pos[3];

					double ixcoef0 = pX_Array_Value[x].icoeff[0];
					double ixcoef1 = pX_Array_Value[x].icoeff[1];
					double ixcoef2 = pX_Array_Value[x].icoeff[2];
					double ixcoef3 = pX_Array_Value[x].icoeff[3];

					p[0] = pSrc[xpos0 + ypos0];
					p[1] = pSrc[xpos1 + ypos0];
					p[2] = pSrc[xpos2 + ypos0];
					p[3] = pSrc[xpos3 + ypos0];
					double r0 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

					p[0] = pSrc[xpos0 + ypos1];
					p[1] = pSrc[xpos1 + ypos1];
					p[2] = pSrc[xpos2 + ypos1];
					p[3] = pSrc[xpos3 + ypos1];
					double r1 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

					p[0] = pSrc[xpos0 + ypos2];
					p[1] = pSrc[xpos1 + ypos2];
					p[2] = pSrc[xpos2 + ypos2];
					p[3] = pSrc[xpos3 + ypos2];
					double r2 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

					p[0] = pSrc[xpos0 + ypos3];
					p[1] = pSrc[xpos1 + ypos3];
					p[2] = pSrc[xpos2 + ypos3];
					p[3] = pSrc[xpos3 + ypos3];
					double r3 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

					*(pDst + x + y*dstStep) = 
						(Fw8u) (r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3);
				}
			}
			
			//free allocated memory by fwMalloc
			fwFree(pX_Array_Value);
			fwFree(pY_Array_Value);
		}
		break;
	case FWI_INTER_SUPER:
		//super sampling only support xFactor and yFactor <=1 cases.
		if (xFactor > 1.0 || yFactor > 1.0) return fwStsResizeFactorErr;

		//Seperate data to Y, U, V data
		Fw8u *pSrcY, *pDstY, *pSrcU, *pDstU, *pSrcV, *pDstV;
		Fw8u *tempY, *tempU, *tempV, *tempS;

		//use fwMalloc instead of malloc for aligned address
		pDstY=(Fw8u *)fwMalloc(resizeHeight*resizeWidth*sizeof(Fw8u));
		pDstU=(Fw8u *)fwMalloc(resizeHeight*((resizeWidth+1)/2)*sizeof(Fw8u));
		pDstV=(Fw8u *)fwMalloc(resizeHeight*((resizeWidth+1)/2)*sizeof(Fw8u));
		pSrcY=(Fw8u *)fwMalloc(srcRoi.height *srcRoi.width*sizeof(Fw8u));
		pSrcU=(Fw8u *)fwMalloc(srcRoi.height *srcRoi.width/2*sizeof(Fw8u));
		pSrcV=(Fw8u *)fwMalloc(srcRoi.height *srcRoi.width/2*sizeof(Fw8u));

		for (y=0;y<srcRoi.height;y++) {	
			tempS=(Fw8u *)(pSrc + srcRoi.x + (y+srcRoi.y)*srcStep) ;
			tempY=pSrcY + srcRoi.width*y;
			tempU=pSrcU + (srcRoi.width/2)*y;
			tempV=pSrcV + (srcRoi.width/2)*y;

			for (x=0; x<srcRoi.width * 2; x++){
				switch (x%4) 
				{
				case 1://U
					*tempU++=*tempS++;
					break;
				case 3://V
					*tempV++=*tempS++;
					break;
				case 0://Y
				case 2:
				default:
					*tempY++=*tempS++;
				}
			}
		}		

        for (y=0;y<resizeHeight;y++) {	
			tempS=pDst  + y*dstStep ;
			tempY=pDstY + resizeWidth*y;
			tempU=pDstU + ((resizeWidth+1)/2)*y;
			tempV=pDstV + (resizeWidth/2)*y;

			for (x=0; x<resizeWidth * 2; x++){
				switch (x%4) 
				{
				case 1://U
					*tempU++ = *tempS++;
					break;
				case 3://V
					*tempV++ = *tempS++;
					break;
				case 0://Y
				case 2:
				default:
					*tempY++ = *tempS++;
				}
			}
		}		

		//handle Y
		FwStatus status;
		FwiSize tempSize;
		FwiRect tempRoi;
		FwiSize tempDstSize;
		tempSize.height= srcRoi.height;
		tempSize.width = srcRoi.width;
		tempRoi.height = srcRoi.height;
		tempRoi.width  = srcRoi.width;
		tempRoi.x = 0;
		tempRoi.y = 0;
		tempDstSize.height = resizeHeight;
		tempDstSize.width = resizeWidth;

		status=fwiResize_8u_C1R(pSrcY, tempSize, tempSize.width, tempRoi, 
			pDstY, resizeWidth, tempDstSize, xFactor, yFactor, FWI_INTER_SUPER);
		if (status != fwStsNoErr)return status;

		//handle U
		tempSize.width = srcRoi.width/2;
		tempRoi.width  = srcRoi.width/2;
		tempDstSize.width = (resizeWidth+1)/2; //in odd case, end with YU

		status=fwiResize_8u_C1R(pSrcU, tempSize, tempSize.width, tempRoi, 
			pDstU, (resizeWidth+1)/2, tempDstSize, xFactor, yFactor, FWI_INTER_SUPER);
		if (status != fwStsNoErr)return status;

		//Handle V
		tempDstSize.width = resizeWidth/2; //in odd case, end with YU
		status=fwiResize_8u_C1R(pSrcV, tempSize, tempSize.width, tempRoi, 
			pDstV, resizeWidth/2, tempDstSize, xFactor, yFactor, FWI_INTER_SUPER);
		if (status != fwStsNoErr)return status;

		//Combine YUV to destination
		for (y=0;y<resizeHeight;y++) {	
			tempS=pDst  + y*dstStep ;
			tempY=pDstY + resizeWidth*y;
			tempU=pDstU + ((resizeWidth+1)/2)*y;
			tempV=pDstV + (resizeWidth/2)*y;

			for (x=0; x<resizeWidth * 2; x++){
				switch (x%4) 
				{
				case 1://U
					*tempS++=*tempU++;
					break;
				case 3://V
					*tempS++=*tempV++;
					break;
				case 0://Y
				case 2:
				default:
					*tempS++=*tempY++;
				}
			}
		}		

		//free allocated memory by fwMalloc
		fwFree(pDstY);
		fwFree(pDstU);
		fwFree(pDstV);
		fwFree(pSrcY);
		fwFree(pSrcU);
		fwFree(pSrcV);

		break;
	default:
		return fwStsInterpolationErr;
	}

	return fwStsNoErr;
}

#endif //FW_BUILD_NUM >= 001


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
