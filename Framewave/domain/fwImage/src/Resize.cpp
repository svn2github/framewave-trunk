/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include all variation of Resize function.
//C1, 8u, Linear & Cubic are originally developed by Wei-Lien
//C1, C3, C4, P3 & P4, 8u, Linear optimization version are created/modified
//according to HP request.
//************************************************************************* 

#include "fwdev.h"
#include "fwImage.h"
#include "system.h"
#include "FwSharedCode_SSE2.h"
#include "Resize.h"

#if BUILD_NUM_AT_LEAST( 100 )

#pragma warning( disable: 4244 )

namespace OPT_LEVEL
{
	struct Cubic_Array {
		int pos[4];
		short  icoeff[4];
	};

	struct Cubic_Array_Double {
		int pos[4];
		double  icoeff[4];
	};

	struct Lanczos_Array_Double {
		int pos[6];
		double  icoeff[6];
	};

	struct FW_Resize_8u_C1R_Cubic {
		const Fw8u *pSrc;
		Fw8u *pDst;
		int dstStep; 
		int resizeWidth;
		Cubic_Array *pX_Array_Value;
		Cubic_Array *pY_Array_Value; 
		int yBegin;
		int yEnd;
	};

	//General/support function declaration
	//void FW_memset(void *dest, unsigned char c, size_t count);

	static FwStatus My_FW_ResizeCheck(FwiSize srcSize, int srcStep, FwiRect srcRoi,
		int dstStep, FwiSize dstRoiSize, double xFactor, 
		double yFactor, int interpolation, int channel);

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_Resize(const TS * pSrc, int srcStep, FwiRect srcRoi,	
		TS* pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int interpolation);

	static FwStatus My_FW_Resize_8u_NN_planar(const Fw8u **pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u **pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int planar);

	static FwStatus My_FW_Resize_8u_LN(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel);

	static FwStatus My_FW_Resize_8u_LN_planar(const Fw8u *const *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u **pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int planar);

	static FwStatus My_FW_Resize_8u_C1R_CU(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor);

	static void My_FW_Resize_8u_CU_op(const Fw8u *pSrc, Fw8u *pDst, int dstStep, int resizeX, 
		Cubic_Array *pX_Array_Value, Cubic_Array *pY_Array_Value, 
		int yBegin, int yEnd);

	static void My_FW_Resize_8u_CU_Op_Thread(void *param);

	static FwStatus My_FW_Resize_8u_C1R_LZ(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor);

	static FwStatus My_FW_Resize_8u_C1R_SU(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor);

	//Internal paramter checking for general resize functions
	static FwStatus My_FW_ResizeCheck(FwiSize srcSize, int srcStep, FwiRect srcRoi,
		int dstStep, FwiSize dstRoiSize, double xFactor, 
		double yFactor, int interpolation, int channel)
	{
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR &&
			interpolation != FWI_INTER_CUBIC  && interpolation != FWI_INTER_LANCZOS &&
			interpolation != FWI_INTER_SUPER)	
			return fwStsInterpolationErr;

		if (srcSize.height <= 0 || srcSize.width <= 0 || 
			srcRoi.height <= 0 || srcRoi.width<= 0 ||
			dstRoiSize.height <= 0 || dstRoiSize.width<= 0 )	
			return fwStsSizeErr;	

		if (srcStep < channel || dstStep < channel) //at least one pixel 
			return fwStsStepErr;	

		if (xFactor <= 0.0 || yFactor <= 0.0)	
			return fwStsResizeFactorErr;	
		
		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if ((srcRoi.x + srcRoi.width -1 ) < 0 || (srcRoi.x >= srcSize.width) || 
			(srcRoi.y + srcRoi.height -1) < 0 || (srcRoi.y >= srcSize.height))	
			return fwStsWrongIntersectROI;

		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		return fwStsNoErr;
	}

	//generic function call for fwiResize
	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_Resize(const TS * pSrc, int srcStep, FwiRect srcRoi,	
		TS* pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int interpolation)
	{	
		int x, y, k;
		int channel = ChannelCount(chSrc);

		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(TS));
		srcStep = srcStep / (sizeof(TS));

		int channel1;
		// Will not change 4th channel element in AC4
		if (chSrc == AC4) channel1=3;
		else channel1=channel;
		Fw32f round;
		// 32f is supported, but not 32u and 32s
		// No rounding is needed for 32f type
		if (sizeof(TS) == 4) round=0;
		else round=0.5;

		if (interpolation == FWI_INTER_NN) {
			int *px_NN, py_NN;

			px_NN=(int *)malloc(sizeof(int)*resizeWidth);

			// precalculate the x value 
			for (x=0;x<resizeWidth;x++) {
				px_NN[x]=(int)((double)x/xFactor)+srcRoi.x;
			}

			for (y=0; y< resizeHeight;y++) {
				py_NN=(int)((double)y/yFactor)+srcRoi.y;
				for (x=0;x<resizeWidth;x++) {
					for (k=0; k<channel1; k++) {
						*(pDst+y*dstStep+x*channel+k) = 
							*(pSrc+py_NN*srcStep+px_NN[x]*channel+k);
					}
				}
			}

			free(px_NN);
		}
		else if (interpolation == FWI_INTER_LANCZOS) {
			int xpos, ypos, i;
			double sin_piX, cos_piX3, sin_piX3, pi, sqrt3, temp1, temp2, temp3;
			double value, ry;

			pi=3.1415926535897;
			sqrt3=1.73205081;

			//use fwMalloc instead of malloc for aligned address
			Lanczos_Array_Double *pX_Array_Value = (Lanczos_Array_Double*) fwMalloc(resizeWidth 
				* sizeof(Lanczos_Array_Double));
			Lanczos_Array_Double *pY_Array_Value = (Lanczos_Array_Double*) fwMalloc(resizeHeight 
				* sizeof(Lanczos_Array_Double));

			for (x=0;x<resizeWidth;x++) {
				temp1 = ((double)x / xFactor);
				xpos = (int) temp1;
				value = temp1 - xpos;
				sin_piX3 = sin(pi*value/3.0);
				cos_piX3 = cos(pi*value/3.0);
				sin_piX = sin_piX3*(3.0-4.0*sin_piX3*sin_piX3);
				temp1 = (sqrt3*cos_piX3+sin_piX3)/2.0;
				temp2 = (sqrt3*cos_piX3-sin_piX3)/2.0;
				temp3 = 3.0*sin_piX/(pi*pi);

				pX_Array_Value[x].icoeff[0] = temp3*temp2/((value+2)*(value+2));
				pX_Array_Value[x].icoeff[1] =-temp3*temp1/((value+1)*(value+1));
				pX_Array_Value[x].icoeff[3] = temp3*temp2/((1-value)*(1-value));
				pX_Array_Value[x].icoeff[4] =-temp3*temp1/((2-value)*(2-value));
				pX_Array_Value[x].icoeff[5] = temp3*sin_piX3/((3-value)*(3-value));
				//protect overflow
				pX_Array_Value[x].icoeff[2] = 1 - pX_Array_Value[x].icoeff[0] - pX_Array_Value[x].icoeff[1] -
					pX_Array_Value[x].icoeff[3] - pX_Array_Value[x].icoeff[4] - pX_Array_Value[x].icoeff[5];

				pX_Array_Value[x].pos[2] = srcRoi.x + xpos;

				xpos-=2;
				if (xpos <0) pX_Array_Value[x].pos[0] = srcRoi.x;
				else pX_Array_Value[x].pos[0] = srcRoi.x + xpos;

				xpos++;
				if (xpos <0) pX_Array_Value[x].pos[1] = srcRoi.x;
				else pX_Array_Value[x].pos[1] = srcRoi.x + xpos;

				xpos+=2;
				if (xpos >= srcRoi.width) {
					pX_Array_Value[x].pos[3] = pX_Array_Value[x].pos[2];
					pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[2];
					pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[2];
				} 
				else pX_Array_Value[x].pos[3] = srcRoi.x + xpos;

				xpos++;
				if (xpos >= srcRoi.width) {
					pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[3];
					pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[3];
				} 
				else pX_Array_Value[x].pos[4] = srcRoi.x + xpos;

				xpos++;
				if (xpos >= srcRoi.width) {
					pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[4];
				} 
				else pX_Array_Value[x].pos[5] = srcRoi.x + xpos;
			}

			for (y=0; y<resizeHeight; y++) {
				temp1 = ((double)y / yFactor);
				ypos = (int) temp1;
				value = temp1-ypos;
				sin_piX3 = sin(pi*value/3.0);
				cos_piX3 = cos(pi*value/3.0);
				sin_piX = sin_piX3*(3.0-4.0*sin_piX3*sin_piX3);
				temp1 = (sqrt3*cos_piX3+sin_piX3)/2.0;
				temp2 = (sqrt3*cos_piX3-sin_piX3)/2.0;
				temp3 = 3.0*sin_piX/(pi*pi);

				pY_Array_Value[y].icoeff[0] = temp3*temp2/((value+2)*(value+2));
				pY_Array_Value[y].icoeff[1] =-temp3*temp1/((value+1)*(value+1));
				pY_Array_Value[y].icoeff[3] = temp3*temp2/((1-value)*(1-value));
				pY_Array_Value[y].icoeff[4] =-temp3*temp1/((2-value)*(2-value));
				pY_Array_Value[y].icoeff[5] = temp3*sin_piX3/((3-value)*(3-value));
				//protect overflow
				pY_Array_Value[y].icoeff[2] = 1 - pY_Array_Value[y].icoeff[0] - pY_Array_Value[y].icoeff[1] -
					pY_Array_Value[y].icoeff[3] - pY_Array_Value[y].icoeff[4] - pY_Array_Value[y].icoeff[5];

				pY_Array_Value[y].pos[2] = (srcRoi.y + ypos) * srcStep;

				ypos-=2;
				if (ypos <0) pY_Array_Value[y].pos[0] = srcRoi.y * srcStep;
				else pY_Array_Value[y].pos[0] = (srcRoi.y + ypos) * srcStep;

				ypos++;
				if (ypos <0) pY_Array_Value[y].pos[1] = srcRoi.y *srcStep;
				else pY_Array_Value[y].pos[1] = pY_Array_Value[y].pos[0] + srcStep;

				ypos+=2;
				if (ypos >= srcRoi.height) {
					pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2];
					pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[2];
					pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[2];
				} 
				else pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2] + srcStep;

				ypos++;
				if (ypos >= srcRoi.height) {
					pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3];
					pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[3];
				} 
				else pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3] + srcStep;

				ypos++;
				if (ypos >= srcRoi.height) {
					pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4];
				} 
				else pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4] + srcStep;
			}

			//Calculating the pixel value
			int typos[6], txpos[6];
			double iycoef[6], ixcoef[6];

			for (y = 0; y < resizeHeight; y++) {
				for (i=0;i<6;i++) {
					typos[i] = pY_Array_Value[y].pos[i];
					iycoef[i]= pY_Array_Value[y].icoeff[i];
				}
				for (x = 0; x < resizeWidth; x++) {
					for (i=0;i<6;i++) {
						txpos[i] = pX_Array_Value[x].pos[i];
						ixcoef[i]= pX_Array_Value[x].icoeff[i];
					}

					for (k=0;k < channel1; k++) {
						ry=0;
						for (i=0;i<6;i++) {
							ry += (*(pSrc + typos[i] + txpos[0]*channel+k) * ixcoef[0] +
								*(pSrc + typos[i] + txpos[1]*channel+k) * ixcoef[1] +
								*(pSrc + typos[i] + txpos[2]*channel+k) * ixcoef[2] +
								*(pSrc + typos[i] + txpos[3]*channel+k) * ixcoef[3] +
								*(pSrc + typos[i] + txpos[4]*channel+k) * ixcoef[4] +
								*(pSrc + typos[i] + txpos[5]*channel+k) * ixcoef[5])*
								iycoef[i];
						}

						//saturate the result for corresponding data type
						*(pDst + y*dstStep + x*channel+k) = FW_REF::Limits<TS>::Sat(ry+round);
					}
				}
			}

			//free allocated memory by fwMalloc
			fwFree(pX_Array_Value);
			fwFree(pY_Array_Value);
		} 
		else if (interpolation == FWI_INTER_SUPER) {
			//super sampling only support xFactor and yFactor <=1 cases.
			if (xFactor > 1.0 || yFactor > 1.0) return fwStsResizeFactorErr;

			int ipos, jpos;
			double xFr, yFr, *result;

			result = (double *)malloc(sizeof(double)*resizeWidth*resizeHeight*channel);

			//initialize pDst, so we could mul_add
			for (y=0; y<resizeHeight; y++) {
				for (x=0;x<resizeWidth*channel;x++) {
					*(result+x+y*resizeWidth*channel)=0;
				}
			}

			//Real Add, should use FW_weight! To-Do ....
			jpos=0;
			for (y=0; y<((double)resizeHeight/yFactor); y++) {
				yFr=((double)(jpos+1))/yFactor-y;
				if (yFr>1) yFr=1;

				ipos=0;
				for (x=0;x<((double)resizeWidth/xFactor);x++) {
					xFr=((double)(ipos+1))/xFactor-x;
					if (xFr>1) xFr=1; //Could pre-calculated

					for (k=0;k<channel1;k++) 
						*(result+(ipos+jpos*resizeWidth)*channel+k) 
						+= *(pSrc+(y+srcRoi.y)*srcStep+(x+srcRoi.x)*channel+k)*xFr*yFr;

					if (yFr<1 && (jpos+1) < resizeHeight) {
						for (k=0;k<channel1;k++)
							*(result+(ipos+(jpos+1)*resizeWidth)*channel+k) += 
							*(pSrc+(y+srcRoi.y)*srcStep+(x+srcRoi.x)*channel+k)*xFr*(1-yFr);
					}
					if (xFr<1 && (ipos+1) < resizeWidth && 
						yFr<1 && (jpos+1) < resizeHeight ) {
							for (k=0;k<channel1;k++) 
								*(result+((ipos+1)+(jpos+1)*resizeWidth)*channel+k) += 
								*(pSrc+(y+srcRoi.y)*srcStep+(x+srcRoi.x)*channel+k)*(1-xFr)*(1-yFr);
					}
					if (xFr<1 && (ipos+1) < resizeWidth) {
						ipos++;
						for (k=0;k<channel1;k++)
							*(result+(ipos+jpos*resizeWidth)*channel+k) += 
							*(pSrc+(y+srcRoi.y)*srcStep+(x+srcRoi.x)*channel+k)*(1-xFr)*yFr;
					}
				}

				if (yFr<1) jpos++;
			}

			//divide the weighted sum by Src/Dst=1/(xFactor*yFactor).
			//Should use fw_weight to fix the accuracy issue
			yFr=xFactor*yFactor;
			for (y=0; y<resizeHeight; y++) {
				for (x=0;x<resizeWidth;x++) {
					for (k=0;k<channel1;k++) {
						//saturate the result for corresponding data type
						*(pDst+y*dstStep+x*channel+k) = 
							FW_REF::Limits<TS>::Sat(*(result+(x+y*resizeWidth)*channel+k) * yFr+round);
					}
				}
			}

			free(result);
		}
		else if (interpolation == FWI_INTER_CUBIC) {
			//use fwMalloc instead of malloc for aligned address
			Cubic_Array_Double *pX_Array_Value = (Cubic_Array_Double*) fwMalloc(resizeWidth 
				* sizeof(Cubic_Array_Double));
			Cubic_Array_Double *pY_Array_Value = (Cubic_Array_Double*) fwMalloc(resizeHeight 
				* sizeof(Cubic_Array_Double));

			// pixel mapping and coefficient generation

			int l1, l2, r1, r2, x, y;
			double p[4];
			// loop over all points of X
			//change aA=0.2 to aA=-0.5
			double aA=-0.5; 

			for (x = 0; x < resizeWidth; x++) {
				double fx = (double)x/xFactor;
				l1 = (int) fx;
				l2 = l1 -1;
				if(l2 < 0) l2 = l1;
				r1 = l1 + 1;
				//Protection for over-boundary reading
				if (r1 >= srcRoi.width) r1 = r2 = l1;
				else {
					r2 = r1 +1;
					if (r2 >= srcRoi.width) r2 = r1;
				}

				pX_Array_Value[x].pos[0] = l2;
				pX_Array_Value[x].pos[1] = l1;
				pX_Array_Value[x].pos[2] = r1;
				pX_Array_Value[x].pos[3] = r2;

				// BC-cubic spline
				double px = (double) pX_Array_Value[x].pos[1] - fx - 1.0; 

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

				pX_Array_Value[x].pos[0] += srcRoi.x;
				pX_Array_Value[x].pos[1] += srcRoi.x;
				pX_Array_Value[x].pos[2] += srcRoi.x;
				pX_Array_Value[x].pos[3] += srcRoi.x;
			}

			// loop over all points of Y
			for (y = 0; y < resizeHeight; y++){
				l1 = (int)((double)y / yFactor);
				l2 = l1 -1;
				if(l2 < 0) l2 = l1;
				r1 = l1 + 1;
				//Protection for over-boundary reading
				if (r1 >= srcRoi.height) r1 = r2 = l1;
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

				for (int i=0; i<4; i++) {

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
				- pY_Array_Value[y].icoeff[2] - pY_Array_Value[y].icoeff[3];

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

				for (x = 0; x < resizeWidth; x++) {
					int xpos0 = pX_Array_Value[x].pos[0];
					int xpos1 = pX_Array_Value[x].pos[1];
					int xpos2 = pX_Array_Value[x].pos[2];
					int xpos3 = pX_Array_Value[x].pos[3];

					double ixcoef0 = pX_Array_Value[x].icoeff[0];
					double ixcoef1 = pX_Array_Value[x].icoeff[1];
					double ixcoef2 = pX_Array_Value[x].icoeff[2];
					double ixcoef3 = pX_Array_Value[x].icoeff[3];

					for (k=0; k<channel1; k++) {
						//y-1 line
						p[0] = pSrc[ypos0 + xpos0*channel+k];
						p[1] = pSrc[ypos0 + xpos1*channel+k];
						p[2] = pSrc[ypos0 + xpos2*channel+k];
						p[3] = pSrc[ypos0 + xpos3*channel+k];
						double r0 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

						//y line
						p[0] = pSrc[ypos1 + xpos0*channel+k];
						p[1] = pSrc[ypos1 + xpos1*channel+k];
						p[2] = pSrc[ypos1 + xpos2*channel+k];
						p[3] = pSrc[ypos1 + xpos3*channel+k];
						double r1 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

						//y+1 line
						p[0] = pSrc[ypos2+ xpos0*channel+k];
						p[1] = pSrc[ypos2+ xpos1*channel+k];
						p[2] = pSrc[ypos2+ xpos2*channel+k];
						p[3] = pSrc[ypos2+ xpos3*channel+k];
						double r2 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

						//y+2 line
						p[0] = pSrc[ypos3+ xpos0*channel+k];
						p[1] = pSrc[ypos3+ xpos1*channel+k];
						p[2] = pSrc[ypos3+ xpos2*channel+k];
						p[3] = pSrc[ypos3+ xpos3*channel+k];
						double r3 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

						//saturate the result for corresponding data type
						*(pDst + y*dstStep + x*channel+k) = 
							FW_REF::Limits<TS>::Sat(r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+round);
					}
				}
			}
			
			//free allocated memory by fwMalloc
			fwFree(pX_Array_Value);
			fwFree(pY_Array_Value);
		}
		else {//if (interpolation == FWI_INTER_LINEAR) {
			//use fwMalloc instead of malloc for aligned address
			Linear_Array_Double *pX_Array_Value = (Linear_Array_Double*) 
				fwMalloc(resizeWidth*sizeof(Linear_Array_Double));
			Linear_Array_Double *pY_Array_Value = (Linear_Array_Double*) 
				fwMalloc(resizeHeight*sizeof(Linear_Array_Double));

			int ceil_X, ceil_Y, floor_X, floor_Y;
			double fraction_X, fractY, one_Minus_X, one_Minus_Y;
			double t1, t2, p1, p2, p3, p4;
			TS * pSrc_FloorY, *pSrc_CeilY;

			//Pre-calculate the x coefficient.
			for (x = 0; x < resizeWidth; x++ ){    
				floor_X = (int)((double)x / xFactor);

				fraction_X = x/xFactor - floor_X;
				one_Minus_X = 1.0 - fraction_X;

				//Protection for over-boundary reading
				ceil_X = floor_X + 1;
				if (ceil_X >= srcRoi.width) ceil_X = floor_X;
				floor_X += srcRoi.x;
				ceil_X+= srcRoi.x;

				pX_Array_Value[x].floor=floor_X;
				pX_Array_Value[x].ceil=ceil_X;
				pX_Array_Value[x].ifraction=fraction_X;
				pX_Array_Value[x].ione_Minus_Val=one_Minus_X;			
			}

			//Pre-calculate the y coefficient.
			for (y = 0; y < resizeHeight; y++){
				floor_Y = (int)((double)y / yFactor);

				fractY = y/yFactor - floor_Y;
				one_Minus_Y = 1.0 - fractY;

				ceil_Y = floor_Y + 1;
				//Protection for over-boundary reading
				if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;
				floor_Y += srcRoi.y;
				ceil_Y+= srcRoi.y;

				pY_Array_Value[y].floor=floor_Y*srcStep;
				pY_Array_Value[y].ceil=ceil_Y*srcStep;
				pY_Array_Value[y].ifraction=fractY;	
				pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
			}

			//Pre-calculate the y coefficient.
			for (y = 0; y < resizeHeight; y++) {
				pSrc_CeilY  = (TS*)pSrc;
				pSrc_FloorY = (TS*)pSrc;
				pSrc_CeilY  += pY_Array_Value[y].ceil;
				pSrc_FloorY += pY_Array_Value[y].floor;

				fractY = pY_Array_Value[y].ifraction;
				one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

				for (x=0; x < resizeWidth; x++){			
					ceil_X =pX_Array_Value[x].ceil;
					floor_X=pX_Array_Value[x].floor;
					fraction_X  = pX_Array_Value[x].ifraction;
					one_Minus_X = pX_Array_Value[x].ione_Minus_Val;

					// resize
					for (k=0;k<channel1;k++) {
						p1 = *(pSrc_FloorY + floor_X*channel+k);
						p2 = *(pSrc_FloorY + ceil_X*channel+k);
						p3 = *(pSrc_CeilY  + floor_X*channel+k);
						p4 = *(pSrc_CeilY  + ceil_X*channel+k);

						//line y
						t1 = one_Minus_X * p1 + fraction_X * p2;
						//line y+1
						t2 = one_Minus_X * p3 + fraction_X * p4;

						//saturate the result for corresponding data type
						*(pDst + y*dstStep + x*channel+k) = 
							FW_REF::Limits<TS>::Sat(one_Minus_Y*t1 + fractY*t2+round);
					}
				}
			}
		}

		return fwStsNoErr;
	}

	//Resize_8u
	//Nearest Neighbor interpolation functions
	FwStatus My_FW_Reisze_8u_NN(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel)
	{
		//FwStatus stat_from_interpolation;	
		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;	

		int x, y;
		int *px_NN, *py_NN;
		//use fwMalloc instead of malloc for aligned address
		px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
		py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

		// precalculate the x, y value 
		for (x=0;x<resizeWidth;x++) {
			px_NN[x]=(int)((double)x/xFactor)+srcRoi.x;
		}
		for (y=0;y<resizeHeight;y++) {
			py_NN[y]=((int)((double)y/yFactor)+srcRoi.y)*srcStep;
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_8u_NN resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].srcStep = srcStep;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].px_NN = px_NN;
				resizePar[threadNum].py_NN = py_NN;
				resizePar[threadNum].channel = channel;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;

				ThreadPool::Run(My_FW_Resize_8u_NN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			if (channel ==1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight);
			} else {
				My_FW_Resize_8u_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight, channel);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(px_NN);
		fwFree(py_NN);

		return fwStsNoErr;
	}


	//Internal function for resize unsigned short data with Nearest Neighbor interpolation
	FwStatus My_FW_Resize_16u_NN(const Fw16u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel)
	{

		int x, y;

		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw16u));
		srcStep = srcStep / (sizeof(Fw16u));

		int channel1;

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		if (channel == 5) {channel1=3, channel=4;}
		else channel1=channel;

		int *px_NN, *py_NN;
		//use fwMalloc instead of malloc for aligned address
		px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
		py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

		// precalculate the x value 
		for (x=0;x<resizeWidth;x++) {
			px_NN[x]=(int)((double)(x)/xFactor+srcRoi.x);
		}

		// precalculate the y value 
		for (y=0; y<resizeHeight;y++) {
			py_NN[y]=(int)((double)(y)/yFactor+srcRoi.y) * srcStep;
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_16u_NN resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].srcStep = srcStep;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].px_NN = px_NN;
				resizePar[threadNum].py_NN = py_NN;
				resizePar[threadNum].channel = channel;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;


				ThreadPool::Run(My_FW_Resize_16u_NN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			if (channel ==1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_16u_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight);
			} else {
				My_FW_Resize_16u_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight, channel);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(px_NN);
		fwFree(py_NN);

		return fwStsNoErr;
	}

	//Internal function for resize float data with Nearest Neighbor interpolation
	FwStatus My_FW_Resize_32f_NN(const Fw32f *pSrc, int srcStep, FwiRect srcRoi, 
		Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel)
	{
		int x, y;

		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int channel1;
		
		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		if (channel == 5) {channel1=3, channel =4;}
		else channel1=channel;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw32f));
		srcStep = srcStep / (sizeof(Fw32f));

		int *px_NN, *py_NN;
		//use fwMalloc instead of malloc for aligned address
		px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
		py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

		// precalculate the x value 
		for (x=0;x<resizeWidth;x++) {
			px_NN[x]=(int)((double)(x)/xFactor+srcRoi.x);
		}

		// precalculate the y value 
		for (y=0; y<resizeHeight;y++) {
			py_NN[y]=(int)((double)(y)/yFactor+srcRoi.y) * srcStep;
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_32f_NN resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].srcStep = srcStep;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].px_NN = px_NN;
				resizePar[threadNum].py_NN = py_NN;
				resizePar[threadNum].channel = channel;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;


				ThreadPool::Run(My_FW_Resize_32f_NN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			if (channel ==1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_32f_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight);
			} else {
				My_FW_Resize_32f_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight, channel);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(px_NN);
		fwFree(py_NN);

		return fwStsNoErr;
	}

	//Internal function for resize 8u data with Nearest Neighbor interpolation
	//Planar data, we could save the parameter checking part and coefficient calculation
	static FwStatus My_FW_Resize_8u_NN_planar(const Fw8u *const *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u **pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int planar)
	{
		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int i, x, y;
		int *px_NN, *py_NN;
		//use fwMalloc instead of malloc for aligned address
		px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
		py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

		// precalculate the x, y value 
		for (x=0;x<resizeWidth;x++) {
			px_NN[x]=(int)((double)x/xFactor+0.5)+srcRoi.x;
		}
		for (y=0;y<resizeHeight;y++) {
			py_NN[y]=((int)((double)y/yFactor+0.5)+srcRoi.y)*srcStep;
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_8u_NN resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			for (i=0;i<planar;i++) {	

				//seperate the region to be multi-threaded
				//Along the seperation line, there will be some in-accurate results 
				//comparing to the single thread answers.
				for(threadNum=0; threadNum<total_thread_num; threadNum++) {
					y_bgn   = threadNum * threadheight; 
					y_end   = y_bgn + threadheight;
					//last thread handle all remaining area
					if (threadNum==total_thread_num-1) {
						y_end=resizeHeight;
					}

					//Pack the seperated data for multi-thread 
					resizePar[threadNum].pSrc = pSrc[i];
					resizePar[threadNum].srcStep = srcStep;
					resizePar[threadNum].pDst = pDst[i];
					resizePar[threadNum].dstStep = dstStep;
					resizePar[threadNum].resizeWidth = resizeWidth;
					resizePar[threadNum].px_NN = px_NN;
					resizePar[threadNum].py_NN = py_NN;
					resizePar[threadNum].channel = 1;
					resizePar[threadNum].xBegin = 0;
					resizePar[threadNum].xEnd = resizeWidth;
					resizePar[threadNum].yBegin = y_bgn;
					resizePar[threadNum].yEnd = y_end;

					ThreadPool::Run(My_FW_Resize_8u_NN_Thread, (void *) &resizePar[threadNum]);
				}
				ThreadPool::Wait();
			}

		} else {
			//for single thread, no data packing
			for (i=0;i<planar;i++) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_NN_Op1(pSrc[i], srcStep, pDst[i], dstStep, resizeWidth, 
					px_NN, py_NN, 0, resizeWidth, 0, resizeHeight);
			} 
		}

		//free allocated memory by fwMalloc
		fwFree(px_NN);
		fwFree(py_NN);

		return fwStsNoErr;
	}

	//Internal function for resize 8u data with Linear interpolation
	FwStatus My_FW_Resize_8u_LN(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel)
	{
		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int x, y;

		//Outside buffer will not be initialized
		//dstRoiSize.width > resizeWidth case will be handled by each thread
		//if(dstRoiSize.height > resizeHeight) {
		//	for (y=resizeHeight;y<dstRoiSize.height;y++){
		//		FW_memset((pDst+y*dstStep), 0, (dstRoiSize.width*channel));
		//	}
		//}

		//use fwMalloc instead of malloc for aligned address
		Linear_Array *pX_Array_Value = (Linear_Array*) fwMalloc(resizeWidth *sizeof(Linear_Array));
		Linear_Array *pY_Array_Value = (Linear_Array*) fwMalloc(resizeHeight*sizeof(Linear_Array));

		int ceil_X, floor_X, ceil_Y, floor_Y;
		double fraction_X, fractY, weight_shift;
		short ifraction_x, ione_minus_x, ifraction_y, ione_minus_y;
		
		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		Fw16u * pIfx, *pIofx;
		pIfx = (Fw16u *) fwMalloc(resizeWidth*channel1*sizeof(Fw16u));
		pIofx = (Fw16u *) fwMalloc(resizeWidth*channel1*sizeof(Fw16u));

		weight_shift = (double) (1<<FW_WEIGHT);

		//Pre-calculate the x coefficient.
		for (x = 0; x < resizeWidth; x++ ){    
			floor_X = (int)((double)x / xFactor);

			fraction_X = x/xFactor - floor_X;
			//Shifted for integer calculation
			ifraction_x = (Fw16u)(fraction_X * weight_shift);
			ione_minus_x = (Fw16u)(weight_shift - ifraction_x);

			//Even fraction_X=0, we allow ceil_X = floor_X+1
			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;
			floor_X += srcRoi.x;
			ceil_X  += srcRoi.x;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;		
			for (int k=0;k<channel1;k++) {
				pIfx[x*channel1+k] = ifraction_x;
				pIofx[x*channel1+k]= ione_minus_x;			
			}
		}

		//Pre-calculate the y coefficient.
		for (y = 0; y < resizeHeight; y++){
			floor_Y = (int)((double)y / yFactor);

			fractY = y/yFactor - floor_Y;

			//Shifted for integer calculation
			ifraction_y  = (Fw16u)(fractY * weight_shift);
			ione_minus_y = (Fw16u)(weight_shift - ifraction_y);

			//For ifraction_y=0 case, we do NOT want to load next line data
			//So set ceil_Y=floor_Y;
			if (ifraction_y) {
				ceil_Y = floor_Y + 1;
			} else {
				ceil_Y = floor_Y;
			}
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;
			floor_Y += srcRoi.y;
			ceil_Y  += srcRoi.y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=ifraction_y;	
			pY_Array_Value[y].ione_Minus_Val=ione_minus_y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_8u_Linear resizePar[MAX_THREADS];

		//debug build
		//total_thread_num =1;

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;

				ThreadPool::Run(My_FW_Resize_8u_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			if (channel ==1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_LN_Op1(pSrc, pDst, dstStep, resizeWidth,  
					pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx);
			} 
			else if (channel == 5) {
				//channel 5 is for AC4, which is 4 channel.
				//AC4 Alpha channel data will not be touched
				My_FW_Resize_8u_LN_Op_AC4(pSrc, pDst, dstStep, resizeWidth, 
					pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx, 4);
			}
			else {
				My_FW_Resize_8u_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
					pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx, channel);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(pY_Array_Value);
		fwFree(pX_Array_Value);
		fwFree(pIfx);
		fwFree(pIofx);

		return fwStsNoErr;
	}

	//Internal function for resize unsigned short data with Linear interpolation
	static FwStatus My_FW_Resize_16u_Linear(const Fw16u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int channel)
	{

		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int x, y, k;

		//use fwMalloc instead of malloc for aligned address
		Linear_Array_Float *pX_Array_Value = (Linear_Array_Float*) fwMalloc(resizeWidth *sizeof(Linear_Array_Float));
		Linear_Array_Float *pY_Array_Value = (Linear_Array_Float*) fwMalloc(resizeHeight*sizeof(Linear_Array_Float));

		int ceil_X, floor_X, ceil_Y, floor_Y;
		
		double fraction_X, fractY, one_Minus_X, one_Minus_Y;

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		Fw32f *pIfx, *pIofx;
		pIfx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		pIofx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw16u));
		srcStep = srcStep / (sizeof(Fw16u));

		//Pre-calculate the x coefficient.
		for (x = 0; x < resizeWidth; x++ ) {   
			double fx = (double)x/xFactor;
			floor_X = (int)fx;

			fraction_X = fx - floor_X;
			one_Minus_X = 1.0 - fraction_X;

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;

			floor_X += srcRoi.x;
			ceil_X  += srcRoi.x;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;
			pX_Array_Value[x].ifraction=fraction_X;
			pX_Array_Value[x].ione_Minus_Val=one_Minus_X;
			for(k=0 ; k< channel1; k++)
			{
				pIfx[(x)*channel1+k] = fraction_X;
				pIofx[(x)*channel1+k] = one_Minus_X;
			}
		}

		//Pre-calculate the y coefficient.
		for (y = 0; y < resizeHeight; y++)
		{
			double fy = (double)y/yFactor;
			floor_Y = (int)fy;

			fractY = fy - floor_Y;
			one_Minus_Y = 1.0 - fractY;

			//ceil_Y = floor_Y + 1;
			if (fractY) {
				ceil_Y = floor_Y + 1;
			} else {
				ceil_Y = floor_Y;
			}

			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;
			floor_Y += srcRoi.y;
			ceil_Y  += srcRoi.y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=fractY;	
			pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();

		////debug
		//total_thread_num=1;

		FW_Resize_16u_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = (resizeHeight)/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;

				ThreadPool::Run(My_FW_Resize_16u_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		}
		else {
			//for single thread, no data packing
			My_FW_Resize_16u_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
				pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx, channel);

		}

		return fwStsNoErr;
	}

	//Internal function for resize float data with Linear interpolation
	static FwStatus My_FW_Resize_32f_Linear(const Fw32f *pSrc, int srcStep, FwiRect srcRoi, 
		Fw32f *pDst, int dstStep, FwiSize dstRoiSize, 
		double xFactor, double yFactor, int channel)
	{

		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int x, y, k;

		//Outside buffer will not be initialized
		//dstRoiSize.width > resizeWidth case will be handled by each thread
		//if(dstRoiSize.height > resizeHeight) {
		//	for (y=resizeHeight;y<dstRoiSize.height;y++){
		//		FW_memset((pDst+y*dstStep), 0, (dstRoiSize.width*channel));
		//	}
		//}

		//use fwMalloc instead of malloc for aligned address
		Linear_Array_Float *pX_Array_Value = (Linear_Array_Float*) fwMalloc(resizeWidth *sizeof(Linear_Array_Float));
		Linear_Array_Float *pY_Array_Value = (Linear_Array_Float*) fwMalloc(resizeHeight*sizeof(Linear_Array_Float));

		int ceil_X, floor_X, ceil_Y, floor_Y;
		double fraction_X, fractY, one_Minus_X, one_Minus_Y;

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		Fw32f *pIfx, *pIofx;
		pIfx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		pIofx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw32f));
		srcStep = srcStep / (sizeof(Fw32f));

		//Pre-calculate the x coefficient.
		for (x = 0; x < resizeWidth; x++ )
		{    
			double fx = (double)((x)/xFactor);
			floor_X = (int)fx;
			fraction_X = fx - floor_X;
			one_Minus_X = 1.0 - fraction_X;

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;

			floor_X += srcRoi.x;
			ceil_X  += srcRoi.x;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;
			pX_Array_Value[x].ifraction=fraction_X;
			pX_Array_Value[x].ione_Minus_Val=one_Minus_X;
			for(k=0 ; k< channel1; k++)
			{
				pIfx[x*channel1+k] = fraction_X;
				pIofx[x*channel1+k] = one_Minus_X;
			}

		}

		//Pre-calculate the y coefficient.
		for (y = 0; y < resizeHeight; y++)
		{
			double fy = (double)((y)/yFactor);
			floor_Y = (int)fy;

			fractY = fy - floor_Y;
			one_Minus_Y = 1.0 - fractY;

			ceil_Y = floor_Y + 1;
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;

			floor_Y += srcRoi.y;
			ceil_Y  += srcRoi.y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=fractY;	
			pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_32f_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = (resizeHeight)/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeHeight;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = 0;
				resizePar[threadNum].xEnd = resizeWidth;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;

				ThreadPool::Run(My_FW_Resize_32f_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			My_FW_Resize_32f_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
				pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx, channel);
		}

		//free allocated memory by fwMalloc
		fwFree(pY_Array_Value);
		fwFree(pX_Array_Value);
		fwFree(pIfx);
		fwFree(pIofx);

		return fwStsNoErr;
	}

	//Internal function for resize 8u data with Linear interpolation
	//Planar data, we could save the parameter checking part and coefficient calculation
	FwStatus My_FW_Resize_8u_LN_planar(const Fw8u *const *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u **pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int planar)
	{
		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;

		int i, x, y;

		//Outside buffer will not be initialized
		//dstRoiSize.width > resizeWidth case will be handled by each thread
		//if(dstRoiSize.height > resizeHeight) {
		//	for (i=0;i<planar;i++) {
		//		for (y=resizeHeight;y<dstRoiSize.height;y++){
		//			FW_memset((pDst[i]+y*dstStep), 0, dstRoiSize.width);
		//		}
		//	}
		//}

		//use fwMalloc instead of malloc for aligned address
		Linear_Array *pX_Array_Value = (Linear_Array*) fwMalloc(resizeWidth *sizeof(Linear_Array));
		Linear_Array *pY_Array_Value = (Linear_Array*) fwMalloc(resizeHeight*sizeof(Linear_Array));
		Fw16u * pIfx, *pIofx;	
		pIfx = (Fw16u *) fwMalloc(resizeWidth*sizeof(Fw16u));
		pIofx = (Fw16u *) fwMalloc(resizeWidth*sizeof(Fw16u));

		int ceil_X, floor_X, ceil_Y, floor_Y;
		double fraction_X, fractY, weight_shift;
		short ifraction_x, ione_minus_x, ifraction_y, ione_minus_y;

		weight_shift = (double) (1<<FW_WEIGHT);

		//Pre-calculate the x coefficient.
		for (x = 0; x < resizeWidth; x++ ){    
			floor_X = (int)((double)x / xFactor);

			fraction_X = x/xFactor - floor_X;
			//Shifted for integer calculation
			ifraction_x = (Fw16u)(fraction_X * weight_shift);
			ione_minus_x = (Fw16u)(weight_shift - ifraction_x);

			//Even fraction_X=0, we allow ceil_X = floor_X+1
			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;
			floor_X += srcRoi.x;
			ceil_X  += srcRoi.x;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;		

			pIfx[x] = ifraction_x;
			pIofx[x]= ione_minus_x;			
		}

		//Pre-calculate the y coefficient.
		for (y = 0; y < resizeHeight; y++){
			floor_Y = (int)((double)y / yFactor);

			fractY = y/yFactor - floor_Y;

			//Shifted for integer calculation
			ifraction_y  = (Fw16u)(fractY * weight_shift);
			ione_minus_y = (Fw16u)(weight_shift - ifraction_y);

			//For ifraction_y=0 case, we do NOT want to load next line data
			//So set ceil_Y=floor_Y;
			if (ifraction_y) {
				ceil_Y = floor_Y + 1;
			} else {
				ceil_Y = floor_Y;
			}
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;
			floor_Y += srcRoi.y;
			ceil_Y  += srcRoi.y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=ifraction_y;	
			pY_Array_Value[y].ione_Minus_Val=ione_minus_y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_8u_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = resizeHeight/total_thread_num;

			for (i=0;i<planar;i++) {

				//seperate the region to be multi-threaded
				//Along the seperation line, there will be some in-accurate results 
				//comparing to the single thread answers.
				for(threadNum=0; threadNum<total_thread_num; threadNum++) {
					y_bgn   = threadNum * threadheight; 
					y_end   = y_bgn + threadheight;
					//last thread handle all remaining area
					if (threadNum==total_thread_num-1) {
						y_end=resizeHeight;
					}

					//Pack the seperated data for multi-thread 
					resizePar[threadNum].pSrc = pSrc[i];
					resizePar[threadNum].pDst = pDst[i];
					resizePar[threadNum].dstStep = dstStep;
					resizePar[threadNum].resizeWidth = resizeWidth;
					resizePar[threadNum].pX_Array_Value = pX_Array_Value;
					resizePar[threadNum].pY_Array_Value = pY_Array_Value;
					resizePar[threadNum].pIfx = pIfx;
					resizePar[threadNum].pIofx = pIofx;
					resizePar[threadNum].channel = 1;			
					resizePar[threadNum].yBegin = y_bgn;
					resizePar[threadNum].yEnd = y_end;
					resizePar[threadNum].xBegin = 0;
					resizePar[threadNum].xEnd = resizeWidth;

					ThreadPool::Run(My_FW_Resize_8u_LN_Thread, (void *) &resizePar[threadNum]);
				}
				ThreadPool::Wait();
			}

		} else {
			//for single thread, no data packing
			for (i=0;i<planar;i++) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_LN_Op1(pSrc[i], pDst[i], dstStep, resizeWidth,  
					pX_Array_Value, pY_Array_Value, 0, resizeWidth, 0, resizeHeight, pIfx, pIofx);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(pY_Array_Value);
		fwFree(pX_Array_Value);
		fwFree(pIfx);
		fwFree(pIofx);

		return fwStsNoErr;
	}

	//Internal function for resize 8u data with Cubic interpolation
	FwStatus My_FW_Resize_8u_C1R_CU(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor)
	{
		int DsrcRoiwidth, DsrcRoiheight;
		int resizeX, resizeY;

		DsrcRoiheight = (int)floor((double)srcRoi.height * yFactor);
		DsrcRoiwidth  = (int)floor((double)srcRoi.width  * xFactor);

		resizeX = DsrcRoiwidth;
		resizeY = DsrcRoiheight;

		if(dstRoiSize.width < resizeX) resizeX = dstRoiSize.width;
		if(dstRoiSize.height< resizeY) resizeY = dstRoiSize.height;

		//use fwMalloc instead of malloc for aligned address
		Cubic_Array *pX_Array_Value = (Cubic_Array*) fwMalloc(resizeX * sizeof(Cubic_Array));
		Cubic_Array *pY_Array_Value = (Cubic_Array*) fwMalloc(resizeY * sizeof(Cubic_Array));

		// pixel mapping and coefficient generation
		int l1, l2, r1, r2, x, y;

		// loop over all points of X
		//Change aA=0.2 to aA=-0.5
		double aA=-0.5, weight_shift_6;
		int  iweight_shift_6 = 1<<FW_WEIGHT_6;

		weight_shift_6 = (double) iweight_shift_6 ;

		for (x = 0; x < resizeX; x++) {
			l1 = (int)floor((double)x / xFactor);
			l2 = l1 -1;
			if(l2 < 0) l2 = l1;
			r1 = l1 + 1;
			//Protection for over-boundary reading
			if (r1 >= srcRoi.width) r1 = r2 = l1;
			else {
				r2 = r1 +1;
				if (r2 >= srcRoi.width) r2 = r1;
			}

			pX_Array_Value[x].pos[0] = l2;
			pX_Array_Value[x].pos[1] = l1;
			pX_Array_Value[x].pos[2] = r1;
			pX_Array_Value[x].pos[3] = r2;

			double fx = (double)x/xFactor;
			// BC-cubic spline
			double px = (double) pX_Array_Value[x].pos[1] - fx - 1.0; //?

			for (int i=0; i<4; i++) {
				double hh;
				double xx = px;

				//following the Lanczos formula to calculate the coefficients
				if (xx < 0.0) xx = -xx; // x = |x|
				if (xx < 1.0) {
					hh = (aA + 2.0)*xx*xx*xx-(aA+3.0)*xx*xx + 1;	// 0 <= |x| < 1
				} else if (xx < 2.0) {
					hh =  aA*xx*xx*xx-(5.0*aA)*xx*xx + (8*aA)*xx - 4.0*aA; // 1<= |x| < 2
				} else {			
					hh = 0.0;								// else						
				}

				//	pX_Array_Value[x].fcoeff[i] = hh;
				//Shifted for integer calculation
				pX_Array_Value[x].icoeff[i] = (short)(hh * weight_shift_6 );

				px += 1.0;
			}
			//protect overflow
			pX_Array_Value[x].icoeff[1] =  iweight_shift_6 - pX_Array_Value[x].icoeff[0]
			- pX_Array_Value[x].icoeff[2] - pX_Array_Value[x].icoeff[3];

			pX_Array_Value[x].pos[0] += srcRoi.x;
			pX_Array_Value[x].pos[1] += srcRoi.x;
			pX_Array_Value[x].pos[2] += srcRoi.x;
			pX_Array_Value[x].pos[3] += srcRoi.x;
		}

		// loop over all points of Y
		for (y = 0; y < resizeY; y++) {
			l1 = (int)floor((double)y / yFactor);
			l2 = l1 -1;
			if(l2 < 0) l2 = l1;
			r1 = l1 + 1;
			//Protection for over-boundary reading
			if (r1 >= srcRoi.height) r1 = r2 = l1;
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

			for (int i=0; i<4; i++) {
				double hh;
				double yy = py;

				//following the Lanczos formula to calculate y direction coefficients
				if (yy < 0.0) yy = -yy; // y = |y|
				if (yy < 1.0) {
					hh = (aA + 2.0)*yy*yy*yy-(aA+3.0)*yy*yy + 1;	// 0 <= |y| < 1
				} else if (yy < 2.0) {
					hh =  aA*yy*yy*yy-(5.0*aA)*yy*yy + (8*aA)*yy - 4.0*aA; // 1<= |y| < 2
				} else {			
					hh = 0.0;								// else						
				}

				//Shifted for integer calculation
				pY_Array_Value[y].icoeff[i] = (short)(hh * weight_shift_6 );

				py += 1.0;
			}
			//protect overflow
			pY_Array_Value[y].icoeff[1] =  iweight_shift_6 - pY_Array_Value[y].icoeff[0]
			- pY_Array_Value[y].icoeff[2] - pY_Array_Value[y].icoeff[3];

			pY_Array_Value[y].pos[0] = (l2 + srcRoi.y) * srcStep;
			pY_Array_Value[y].pos[1] = (l1 + srcRoi.y) * srcStep;
			pY_Array_Value[y].pos[2] = (r1 + srcRoi.y) * srcStep;
			pY_Array_Value[y].pos[3] = (r2 + srcRoi.y) * srcStep;
		}
	
		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
		FW_Resize_8u_C1R_Cubic resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end;
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++){

				y_bgn   = threadNum * (resizeY/total_thread_num); 
				y_end   = y_bgn+(resizeY/total_thread_num);
				//dy_bgn  = threadNum * (dstRoiSize.height/total_thread_num); 
				//dy_end  = dy_bgn+(dstRoiSize.height/total_thread_num); 

				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=resizeY;
				}

				if(threadNum > 0) {
					y_bgn   = threadNum * (resizeY/total_thread_num) -1; 
					if(y_bgn % 2 == 0) y_bgn--;
					//dy_bgn  = threadNum * (dstRoiSize.height/total_thread_num) -1; 
					//if(dy_bgn % 2 == 0) dy_bgn--;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeX;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				//resizePar[threadNum].dstRoiSize = dstRoiSize;
				//resizePar[threadNum].dsrcRoiwidth  = DsrcRoiwidth;
				//resizePar[threadNum].dsrcRoiheight = DsrcRoiheight;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				//resizePar[threadNum].dyBegin = dy_bgn;
				//resizePar[threadNum].dyEnd = dy_end;

				//			My_FW_Resize_8u_C1R_Cubic((void *) &resizePar);
				ThreadPool::Run(My_FW_Resize_8u_CU_Op_Thread, (void *) &resizePar[threadNum]);
			}

			ThreadPool::Wait();
		} else {
			//for single thread, no data packing
			My_FW_Resize_8u_CU_op(pSrc, pDst, dstStep, resizeX,  pX_Array_Value, pY_Array_Value, 
				0, resizeY);
		}

		//free allocated memory by fwMalloc
		fwFree(pX_Array_Value);
		fwFree(pY_Array_Value);

		return fwStsNoErr;
	}

	//internal function for threading Resize, 8u data, cubic interpolation
	void My_FW_Resize_8u_CU_Op_Thread(void *param)
	{
		FW_Resize_8u_C1R_Cubic *presizePar = (FW_Resize_8u_C1R_Cubic *) param;

		const Fw8u *pSrc = presizePar->pSrc;
		Fw8u *pDst = presizePar->pDst;
		int dstStep = presizePar->dstStep;
		int resizeWidth = presizePar->resizeWidth;
		Cubic_Array *pX_Array_Value = presizePar->pX_Array_Value;
		Cubic_Array *pY_Array_Value = presizePar->pY_Array_Value;
		//FwiSize dstRoiSize = presizePar->dstRoiSize;
		//int dsrcRoiwidth = presizePar->dsrcRoiwidth;
		//int dsrcRoiheight = presizePar->dsrcRoiheight;
		int yBegin = presizePar->yBegin;
		int yEnd = presizePar->yEnd;
		//int dyBegin = presizePar->dyBegin;
		//int dyEnd  = presizePar->dyEnd;

		My_FW_Resize_8u_CU_op(pSrc, pDst, dstStep, resizeWidth, pX_Array_Value, pY_Array_Value, 
			yBegin, yEnd);

	}

	//internal function for Resize, 8u data, cubic interpolation, single threaded unit
	void My_FW_Resize_8u_CU_op(const Fw8u *pSrc, Fw8u *pDst, int dstStep, int resizeX, 
		Cubic_Array *pX_Array_Value, Cubic_Array *pY_Array_Value, 
		int yBegin, int yEnd)
	{
		Fw8u p[4];
		short half_FW_WEIGHT_6 = 1<<(FW_WEIGHT_6-1) ;
		Fw8u *pSrc_ypos0, *pSrc_ypos1, *pSrc_ypos2, *pSrc_ypos3;

		//use fwMalloc instead of malloc for aligned address
		Fw8u *pRow0 = (Fw8u*) fwMalloc(2*resizeX*sizeof(Fw8u)); //pf2
		Fw8u *pRow1 = (Fw8u*) fwMalloc(2*resizeX*sizeof(Fw8u)); //pf1
		Fw8u *pRow2 = (Fw8u*) fwMalloc(2*resizeX*sizeof(Fw8u)); //pC1
		Fw8u *pRow3 = (Fw8u*) fwMalloc(2*resizeX*sizeof(Fw8u)); //pC2

		Fw8u *pf2, *prevF2, *pf1, *prevF1, *pC1, *prevC1;
		Fw8u *pC2, *prevC2, *pcurrent;

		__m128i rxmm0, rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7;
		//_MM_ALIGN16 unsigned short pp00[8], pp01[8], pp02[8], pp03[8];
		//_MM_ALIGN16 unsigned short pp10[8], pp11[8], pp12[8], pp13[8];
		//_MM_ALIGN16 unsigned short pp20[8], pp21[8], pp22[8], pp23[8];
		//_MM_ALIGN16 unsigned short pp30[8], pp31[8], pp32[8], pp33[8];
		//_MM_ALIGN16 unsigned short icoeffx0[8], icoeffx1[8], icoeffx2[8], icoeffx3[8];
		XMM128 pp00={0}, pp01={0}, pp02={0}, pp03={0};
		XMM128 pp10={0}, pp11={0}, pp12={0}, pp13={0};
		XMM128 pp20={0}, pp21={0}, pp22={0}, pp23={0};
		XMM128 pp30={0}, pp31={0}, pp32={0}, pp33={0};
		XMM128 icoeffx0={0}, icoeffx1={0}, icoeffx2={0}, icoeffx3={0}; 

		int   *pX_Array_Ptri;
		short *pX_Array_Ptrs;

		int  iweight_shift_6 = 1<<FW_WEIGHT_6;

		int x=0,y=0, xx;

		//Outside buffer will not be initialized
		//if (dstRoiSize.width > DsrcRoiwidth || 
		//	dstRoiSize.height > DsrcRoiheight) {
		//	int tempval_width=dstRoiSize.width%16;	
		//	rxmm7  = _mm_set1_epi8(0);		
		//		
		//	for (y = dyBegin; y < dyEnd; y++) {	
		//				
		//		if(dstRoiSize.width>=16) {	
		//			for (x = 0; x < dstRoiSize.width-tempval_width; x+=16)		
		//			   _mm_storeu_si128  ((__m128i *)(pDst + x + y*dstStep),rxmm7);	
		//		
		//			for (;x < dstRoiSize.width; x++) *(pDst + x + y*dstStep) = 0;	
		//		
		//		} else {	
		//			for (x=0;x < dstRoiSize.width; x++) *(pDst + x + y*dstStep) = 0;	
		//		}	
		//	}	
		//}	

		pC1 = pC2 = pf1 = pf2 = pcurrent = 0;
		prevC1 = prevC2 = prevF1 = prevF2 = 0;

		rxmm7  = _mm_set1_epi16(half_FW_WEIGHT_6);                         

		for (y = yBegin; y < yEnd; y++) {
			int ypos0 = pY_Array_Value[y].pos[0];
			int ypos1 = pY_Array_Value[y].pos[1];
			int ypos2 = pY_Array_Value[y].pos[2];
			int ypos3 = pY_Array_Value[y].pos[3];

			pSrc_ypos0 =  (Fw8u*)pSrc + ypos0;
			pSrc_ypos1 =  (Fw8u*)pSrc + ypos1;
			pSrc_ypos2 =  (Fw8u*)pSrc + ypos2;
			pSrc_ypos3 =  (Fw8u*)pSrc + ypos3;

			short iycoef0 = pY_Array_Value[y].icoeff[0];
			short iycoef1 = pY_Array_Value[y].icoeff[1];
			short iycoef2 = pY_Array_Value[y].icoeff[2];
			short iycoef3 = pY_Array_Value[y].icoeff[3];

			int xpos0, xpos1, xpos2, xpos3;

			pf2=pSrc_ypos0;
			pf1=pSrc_ypos1;
			pC1=pSrc_ypos2;
			pC2=pSrc_ypos3;
			pcurrent = pSrc_ypos1;		// record line number

			if(iycoef1 == iweight_shift_6)	{// integer line
				if(resizeX >= 8) {						
					if (pcurrent == prevC1) {

						Fw8u *pTemp;
						pTemp = pRow0;	// swap line arrays
						pRow0 = pRow1;
						pRow1 = pRow2;
						pRow2 = pRow3;
						pRow3=pTemp;

						for (x = 0; x <= resizeX-8; x+= 8)	{// restore pC1
							rxmm1 = _mm_load_si128((__m128i*)(pRow1 + x*2));
							rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm1);
						}

						if (pC2 != prevC2)	pC2=0;	// pC2 not available yet

					} else {
						if (pf1 == pf2){
							for (x = 0; x <= resizeX-8; x+= 8) {
								for (xx = 0; xx < 8; xx++)	{
									// process 8 pixels in parallel	

									pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
									xpos0 = *(pX_Array_Ptri++);
									xpos1 = *(pX_Array_Ptri++);
									xpos2 = *(pX_Array_Ptri++);
									xpos3 = *(pX_Array_Ptri++);

									pp10.u8[xx] = *(pSrc_ypos1 + xpos0);
									pp11.u8[xx] = *(pSrc_ypos1 + xpos1);	
									pp12.u8[xx] = *(pSrc_ypos1 + xpos2);
									pp13.u8[xx] = *(pSrc_ypos1 + xpos3);	

									rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
									int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
									icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
									icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
									rxmm0 = _mm_srli_si128 (rxmm0, 4);
									icoeff32     = _mm_cvtsi128_si32 (rxmm0);
									icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
									icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								}

								rxmm1 = _mm_load_si128(&pp10.i);
								rxmm2 = _mm_load_si128(&pp11.i);
								rxmm3 = _mm_load_si128(&pp12.i);
								rxmm4 = _mm_load_si128(&pp13.i);
								rxmm5 = _mm_load_si128(&icoeffx0.i);
								rxmm6 = _mm_load_si128(&icoeffx1.i);

								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[0]*ixcoef0 
								rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[1]*ixcoef1 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);

								rxmm5 = _mm_load_si128(&icoeffx2.i);
								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[2]*ixcoef2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);

								rxmm6 = _mm_load_si128(&icoeffx3.i);
								rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[3]*ixcoef3 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm4);

								rxmm1 = _mm_add_epi16(rxmm1, rxmm7);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT_6);

								_mm_store_si128((__m128i *)(pRow0 + x*2), rxmm1);	// store pf1 and pf2
								_mm_store_si128((__m128i *)(pRow1 + x*2), rxmm1);

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm1);
							}
						} else {
							for (x = 0; x <= resizeX-8; x+= 8) {
								for (xx = 0; xx < 8; xx++)	{
									// process 8 pixels in parallel
									pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
									xpos0 = *(pX_Array_Ptri++);
									xpos1 = *(pX_Array_Ptri++);
									xpos2 = *(pX_Array_Ptri++);
									xpos3 = *(pX_Array_Ptri++);

									pp10.u8[xx] = *(pSrc_ypos1 + xpos0);
									pp11.u8[xx] = *(pSrc_ypos1 + xpos1);	
									pp12.u8[xx] = *(pSrc_ypos1 + xpos2);
									pp13.u8[xx] = *(pSrc_ypos1 + xpos3);	

									rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
									int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
									icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
									icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
									rxmm0 = _mm_srli_si128 (rxmm0, 4);
									icoeff32     = _mm_cvtsi128_si32 (rxmm0);
									icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
									icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								}

								//LINE-1
								rxmm1 = _mm_load_si128(&pp10.i);
								rxmm2 = _mm_load_si128(&pp11.i);
								rxmm3 = _mm_load_si128(&pp12.i);
								rxmm4 = _mm_load_si128(&pp13.i);
								rxmm5 = _mm_load_si128(&icoeffx0.i);
								rxmm6 = _mm_load_si128(&icoeffx1.i);

								rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[0]*ixcoef0 
								rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[1]*ixcoef1 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm2);

								rxmm5 = _mm_load_si128(&icoeffx2.i);
								rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[2]*ixcoef2 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm3);

								rxmm6 = _mm_load_si128(&icoeffx3.i);
								rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[3]*ixcoef3 
								rxmm1 = _mm_add_epi16(rxmm1, rxmm4);

								rxmm1 = _mm_add_epi16(rxmm1, rxmm7);
								rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT_6);

								_mm_store_si128((__m128i *)(pRow1 + x*2), rxmm1);	// store pf1 and pf2

								rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		// Convert to 8 bit

								_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm1);
							}
						}
						pC1=pC2=0;	// pC1, pC2 not available yet
					}

					for (; x < resizeX; x++) {
						int xpos0 = pX_Array_Value[x].pos[0];
						int xpos1 = pX_Array_Value[x].pos[1];
						int xpos2 = pX_Array_Value[x].pos[2];
						int xpos3 = pX_Array_Value[x].pos[3];

						int ixcoef0 = pX_Array_Value[x].icoeff[0];
						int ixcoef1 = pX_Array_Value[x].icoeff[1];
						int ixcoef2 = pX_Array_Value[x].icoeff[2];
						int ixcoef3 = pX_Array_Value[x].icoeff[3];

						p[0] = pSrc[xpos0 + ypos1];
						p[1] = pSrc[xpos1 + ypos1];
						p[2] = pSrc[xpos2 + ypos1];
						p[3] = pSrc[xpos3 + ypos1];
						int r1 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						*(pDst + x + y*dstStep) = (unsigned char) r1;

					}
				} else {
					for (x = 0; x < resizeX; x++) {
						int xpos0 = pX_Array_Value[x].pos[0];
						int xpos1 = pX_Array_Value[x].pos[1];
						int xpos2 = pX_Array_Value[x].pos[2];
						int xpos3 = pX_Array_Value[x].pos[3];

						int ixcoef0 = pX_Array_Value[x].icoeff[0];
						int ixcoef1 = pX_Array_Value[x].icoeff[1];
						int ixcoef2 = pX_Array_Value[x].icoeff[2];
						int ixcoef3 = pX_Array_Value[x].icoeff[3];


						p[0] = pSrc[xpos0 + ypos1];
						p[1] = pSrc[xpos1 + ypos1];
						p[2] = pSrc[xpos2 + ypos1];
						p[3] = pSrc[xpos3 + ypos1];
						int r1 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						*(pDst + x + y*dstStep) = (unsigned char) r1;
					}
				}
				//pf1=pcurrent;
			} else { // interpolated line
				if (resizeX >= 8) {
					int imode = -1;

					// ====================== 
					if (pf1 == prevF1 && pC1 == prevC1) {
						if (pf2 == prevF2) {			
							if(pC2 == prevC2) // pf1=prevF1, pf2=prevF2, pC1=prevC1, pC2=prevC2
								imode = 2;
							else // pf1=prevF1, pf2==prevF2, pC1=prevC1, pC2!=prevC2
								imode = 3;
						}
						else // pf1= prevF1, pC1=prevC1, pf2!=prevF2,
						{
							imode = 4;
						}
					}	
					else if(pf1 == prevF1 && pC1 != prevC1)
					{
						//if(pf1 == pf2)	// pf2=pf1=prevF1, pC1 != prevC1
						//{
						imode = 1;
						//} else {
						//}
					} else {
						imode = 0;	// load all 4 lines from image
					}

					switch(imode) 
					{
					case 0:		
						for (x = 0; x <= resizeX-8; x+= 8) {
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	
								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp00.u8[xx] = *(pSrc_ypos0 + xpos0);
								pp01.u8[xx] = *(pSrc_ypos0 + xpos1);	
								pp02.u8[xx] = *(pSrc_ypos0 + xpos2);
								pp03.u8[xx] = *(pSrc_ypos0 + xpos3);	

								pp10.u8[xx] = *(pSrc_ypos1 + xpos0);
								pp11.u8[xx] = *(pSrc_ypos1 + xpos1);	
								pp12.u8[xx] = *(pSrc_ypos1 + xpos2);
								pp13.u8[xx] = *(pSrc_ypos1 + xpos3);	

								pp20.u8[xx] = *(pSrc_ypos2 + xpos0);
								pp21.u8[xx] = *(pSrc_ypos2 + xpos1);	
								pp22.u8[xx] = *(pSrc_ypos2 + xpos2);
								pp23.u8[xx] = *(pSrc_ypos2 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								pX_Array_Ptrs = (short *)pX_Array_Ptri;
								icoeffx0.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx1.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx2.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx3.u16[xx] = *(pX_Array_Ptrs);

							}

							// LINE-0
							rxmm0 = _mm_load_si128(&pp00.i);
							rxmm1 = _mm_load_si128(&pp01.i);
							rxmm2 = _mm_load_si128(&pp02.i);
							rxmm3 = _mm_load_si128(&pp03.i);

							rxmm4  = _mm_load_si128(&icoeffx0.i);
							rxmm5  = _mm_load_si128(&icoeffx1.i);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// p[0]*ixcoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[1]*ixcoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm6  = _mm_load_si128(&icoeffx2.i);
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[2]*ixcoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// p[3]*ixcoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow0 + x*2), rxmm0);


							//	// LINE-1
							rxmm1 = _mm_load_si128(&pp10.i);
							rxmm2 = _mm_load_si128(&pp11.i);
							rxmm3 = _mm_load_si128(&pp12.i);
							rxmm4 = _mm_load_si128(&pp13.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[0]*ixcoef0 
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[1]*ixcoef1 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[2]*ixcoef2 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[3]*ixcoef3 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm4);

							rxmm1 = _mm_add_epi16(rxmm1, rxmm7);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow1 + x*2), rxmm1);					

							// LINE-2
							rxmm2 = _mm_load_si128(&pp20.i);
							rxmm3 = _mm_load_si128(&pp21.i);
							rxmm4 = _mm_load_si128(&pp22.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// p[0]*ixcoef0 
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[1]*ixcoef1 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// p[2]*ixcoef2 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm4);

							rxmm3 = _mm_load_si128(&pp23.i);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[3]*ixcoef3 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm2 = _mm_add_epi16(rxmm2, rxmm7);
							rxmm2 = _mm_srli_epi16(rxmm2, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow2 + x*2), rxmm2);	


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);	

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							// *(pDst + x + y*dstStep) = (unsigned char)
							//	((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);
						}
						break;

					case 1: // pf2=pf1=prevF1, pC1 != prevC1

						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	

								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp20.u8[xx] = *(pSrc_ypos2 + xpos0);
								pp21.u8[xx] = *(pSrc_ypos2 + xpos1);	
								pp22.u8[xx] = *(pSrc_ypos2 + xpos2);
								pp23.u8[xx] = *(pSrc_ypos2 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
								int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
								icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								rxmm0 = _mm_srli_si128 (rxmm0, 4);
								icoeff32     = _mm_cvtsi128_si32 (rxmm0);
								icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);

							}

							// LINE-0
							// LINE-1
							rxmm0 = _mm_load_si128((__m128i*)(pRow1 + x*2));
							rxmm1 = rxmm0;

							// LINE-2
							rxmm2 = _mm_load_si128(&pp20.i);
							rxmm3 = _mm_load_si128(&pp21.i);
							rxmm4 = _mm_load_si128(&pp22.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// p[0]*ixcoef0 
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[1]*ixcoef1 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// p[2]*ixcoef2 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm4);

							rxmm3 = _mm_load_si128(&pp23.i);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[3]*ixcoef3 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm2 = _mm_add_epi16(rxmm2, rxmm7);
							rxmm2 = _mm_srli_epi16(rxmm2, FW_WEIGHT_6);
							//pRow2=rxmm2;
							_mm_store_si128((__m128i *)(pRow2 + x*2), rxmm2);


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);

							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							//pRow3=rxmm3;
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);


							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							// *(pDst + x + y*dstStep) = (unsigned char)
							// 	((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}
						break;
					case 2: // pf1=prevF1, pf2=prevF2, pC1=prevC1, pC2=prevC2
						for (x = 0; x <= resizeX-8; x+= 8)
						{
							//for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							//{	
							//	pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
							//	xpos0 = *(pX_Array_Ptri++);
							//	xpos1 = *(pX_Array_Ptri++);
							//	xpos2 = *(pX_Array_Ptri++);
							//	xpos3 = *(pX_Array_Ptri++);

							//	pX_Array_Ptrs = (short *)pX_Array_Ptri;
							//	icoeffx0.u16[xx] = *(pX_Array_Ptrs++);
							//	icoeffx1.u16[xx] = *(pX_Array_Ptrs++);
							//	icoeffx2.u16[xx] = *(pX_Array_Ptrs++);
							//	icoeffx3.u16[xx] = *(pX_Array_Ptrs);

							//}

							// LINE-0
							rxmm0 = _mm_load_si128((__m128i*)(pRow0 + x*2));
							// LINE-1
							rxmm1 = _mm_load_si128((__m128i*)(pRow1 + x*2));
							// LINE-2
							rxmm2 = _mm_load_si128((__m128i*)(pRow2 + x*2));
							// LINE-3
							rxmm3 = _mm_load_si128((__m128i*)(pRow3 + x*2));

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//	*(pDst + x + y*dstStep) = (unsigned char)
							//	((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}
						break;

					case 3: //pf1!=pf2 && pC1==pC2 
						//pf2=pf1; whidbey

						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	

								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								pX_Array_Ptrs = (short *)pX_Array_Ptri;
								icoeffx0.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx1.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx2.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx3.u16[xx] = *(pX_Array_Ptrs);

							}

							// LINE-0
							rxmm0 = _mm_load_si128((__m128i*)(pRow0 + x*2));
							// LINE-1
							rxmm1 = _mm_load_si128((__m128i*)(pRow1 + x*2));
							// LINE-2
							rxmm2 = _mm_load_si128((__m128i*)(pRow2 + x*2));

							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);


							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);

							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);


							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							// *(pDst + x + y*dstStep) = (unsigned char)
							//	((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);
						}
						break;

					case 4: //pf1==pf2 && pC1==pC2

						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	

								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp00.u8[xx] = *(pSrc_ypos0 + xpos0);
								pp01.u8[xx] = *(pSrc_ypos0 + xpos1);	
								pp02.u8[xx] = *(pSrc_ypos0 + xpos2);
								pp03.u8[xx] = *(pSrc_ypos0 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								pX_Array_Ptrs = (short *)pX_Array_Ptri;
								icoeffx0.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx1.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx2.u16[xx] = *(pX_Array_Ptrs++);
								icoeffx3.u16[xx] = *(pX_Array_Ptrs);
							}

							// LINE-0
							rxmm0 = _mm_load_si128(&pp00.i);
							rxmm1 = _mm_load_si128(&pp01.i);
							rxmm2 = _mm_load_si128(&pp02.i);
							rxmm3 = _mm_load_si128(&pp03.i);

							rxmm4  = _mm_load_si128(&icoeffx0.i);
							rxmm5  = _mm_load_si128(&icoeffx1.i);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// p[0]*ixcoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[1]*ixcoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm6  = _mm_load_si128(&icoeffx2.i);
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[2]*ixcoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// p[3]*ixcoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow0 + x*2), rxmm0);

							//	// LINE-1
							rxmm1 = _mm_load_si128((__m128i*)(pRow1 + x*2));										
							// LINE-2
							rxmm2 = _mm_load_si128((__m128i*)(pRow2 + x*2));				

							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);	

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//	*(pDst + x + y*dstStep) = (unsigned char)
							//	((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);
						}
						break;

					case 5:  //pf2==prevF1 && pf1!=prevC1 && pC1 !=prevC2
						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	
								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp10.u8[xx] = *(pSrc_ypos1 + xpos0);
								pp11.u8[xx] = *(pSrc_ypos1 + xpos1);	
								pp12.u8[xx] = *(pSrc_ypos1 + xpos2);
								pp13.u8[xx] = *(pSrc_ypos1 + xpos3);	

								pp20.u8[xx] = *(pSrc_ypos2 + xpos0);
								pp21.u8[xx] = *(pSrc_ypos2 + xpos1);	
								pp22.u8[xx] = *(pSrc_ypos2 + xpos2);
								pp23.u8[xx] = *(pSrc_ypos2 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	


								rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
								int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
								icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								rxmm0 = _mm_srli_si128 (rxmm0, 4);
								icoeff32     = _mm_cvtsi128_si32 (rxmm0);
								icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
							}

							// LINE-0
							pRow0=pRow1;
							rxmm0 = _mm_load_si128((__m128i*)(pRow0 + x*2));

							// LINE-1
							rxmm1 = _mm_load_si128(&pp10.i);
							rxmm2 = _mm_load_si128(&pp11.i);
							rxmm3 = _mm_load_si128(&pp12.i);
							rxmm4 = _mm_load_si128(&pp13.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[0]*ixcoef0 
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[1]*ixcoef1 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm2);

							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[2]*ixcoef2 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[3]*ixcoef3 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm4);

							rxmm1 = _mm_add_epi16(rxmm1, rxmm7);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow1 + x*2), rxmm1);					

							// LINE-2
							rxmm2 = _mm_load_si128(&pp20.i);
							rxmm3 = _mm_load_si128(&pp21.i);
							rxmm4 = _mm_load_si128(&pp22.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// p[0]*ixcoef0 
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[1]*ixcoef1 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// p[2]*ixcoef2 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm4);

							rxmm3 = _mm_load_si128(&pp23.i);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[3]*ixcoef3 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm2 = _mm_add_epi16(rxmm2, rxmm7);
							rxmm2 = _mm_srli_epi16(rxmm2, FW_WEIGHT_6);
							//pRow2=rxmm2;
							_mm_store_si128((__m128i *)(pRow2 + x*2), rxmm2);


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);
							rxmm4  = _mm_load_si128(&icoeffx3.i);

							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);	

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//			*(pDst + x + y*dstStep) = (unsigned char)
							//									((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}

						break;

					case 6: //pf2!=prevF1 && pf1==prevC1 && pC1 !=prevC2
						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	
								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp00.u8[xx] = *(pSrc_ypos0 + xpos0);
								pp01.u8[xx] = *(pSrc_ypos0 + xpos1);	
								pp02.u8[xx] = *(pSrc_ypos0 + xpos2);
								pp03.u8[xx] = *(pSrc_ypos0 + xpos3);	

								pp20.u8[xx] = *(pSrc_ypos2 + xpos0);
								pp21.u8[xx] = *(pSrc_ypos2 + xpos1);	
								pp22.u8[xx] = *(pSrc_ypos2 + xpos2);
								pp23.u8[xx] = *(pSrc_ypos2 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
								int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
								icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								rxmm0 = _mm_srli_si128 (rxmm0, 4);
								icoeff32     = _mm_cvtsi128_si32 (rxmm0);
								icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);

							}

							// LINE-0
							rxmm0 = _mm_load_si128(&pp00.i);
							rxmm1 = _mm_load_si128(&pp01.i);
							rxmm2 = _mm_load_si128(&pp02.i);
							rxmm3 = _mm_load_si128(&pp03.i);

							rxmm4  = _mm_load_si128(&icoeffx0.i);
							rxmm5  = _mm_load_si128(&icoeffx1.i);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// p[0]*ixcoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[1]*ixcoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm6  = _mm_load_si128(&icoeffx2.i);
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[2]*ixcoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// p[3]*ixcoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);
							//pRow0=rxmm0;
							_mm_store_si128((__m128i *)(pRow0 + x*2), rxmm0);


							//	// LINE-1
							pRow1=pRow2;

							// LINE-2
							rxmm2 = _mm_load_si128(&pp20.i);
							rxmm3 = _mm_load_si128(&pp21.i);
							rxmm4 = _mm_load_si128(&pp22.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	// p[0]*ixcoef0 
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[1]*ixcoef1 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	// p[2]*ixcoef2 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm4);

							rxmm3 = _mm_load_si128(&pp23.i);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	// p[3]*ixcoef3 
							rxmm2 = _mm_add_epi16(rxmm2, rxmm3);

							rxmm2 = _mm_add_epi16(rxmm2, rxmm7);
							rxmm2 = _mm_srli_epi16(rxmm2, FW_WEIGHT_6);
							//pRow2=rxmm2;
							_mm_store_si128((__m128i *)(pRow2 + x*2), rxmm2);


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);
							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);

							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);


							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//			*(pDst + x + y*dstStep) = (unsigned char)
							//									((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}
						break;
					case 7: //pf2!=prevF1 && pf1!=prevC1 && pC1 ==prevC2
						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	
								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp00.u8[xx] = *(pSrc_ypos0 + xpos0);
								pp01.u8[xx] = *(pSrc_ypos0 + xpos1);	
								pp02.u8[xx] = *(pSrc_ypos0 + xpos2);
								pp03.u8[xx] = *(pSrc_ypos0 + xpos3);	

								pp10.u8[xx] = *(pSrc_ypos1 + xpos0);
								pp11.u8[xx] = *(pSrc_ypos1 + xpos1);	
								pp12.u8[xx] = *(pSrc_ypos1 + xpos2);
								pp13.u8[xx] = *(pSrc_ypos1 + xpos3);	

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
								int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
								icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								rxmm0 = _mm_srli_si128 (rxmm0, 4);
								icoeff32     = _mm_cvtsi128_si32 (rxmm0);
								icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
							}

							// LINE-0
							rxmm0 = _mm_load_si128(&pp00.i);
							rxmm1 = _mm_load_si128(&pp01.i);
							rxmm2 = _mm_load_si128(&pp02.i);
							rxmm3 = _mm_load_si128(&pp03.i);

							rxmm4  = _mm_load_si128(&icoeffx0.i);
							rxmm5  = _mm_load_si128(&icoeffx1.i);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// p[0]*ixcoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[1]*ixcoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm6  = _mm_load_si128(&icoeffx2.i);
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[2]*ixcoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// p[3]*ixcoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);
							//pRow0=rxmm0;
							_mm_store_si128((__m128i *)(pRow0 + x*2), rxmm0);


							//	// LINE-1
							rxmm1 = _mm_load_si128(&pp10.i);
							rxmm2 = _mm_load_si128(&pp11.i);
							rxmm3 = _mm_load_si128(&pp12.i);
							rxmm4 = _mm_load_si128(&pp13.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// p[0]*ixcoef0 
							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// p[1]*ixcoef1 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm2);
							rxmm5  = _mm_load_si128(&icoeffx2.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[2]*ixcoef2 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm3);

							rxmm6  = _mm_load_si128(&icoeffx3.i);
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[3]*ixcoef3 
							rxmm1 = _mm_add_epi16(rxmm1, rxmm4);

							rxmm1 = _mm_add_epi16(rxmm1, rxmm7);
							rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow1 + x*2), rxmm1);					

							// LINE-2
							pRow2=pRow3;


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);

							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);
							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);

							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);	

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//			*(pDst + x + y*dstStep) = (unsigned char)
							//									((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}

						break;
					case 8: //pf2==prevF1 && pf1==prevC1 && pC1 ==prevC2
						for (x = 0; x <= resizeX-8; x+= 8)
						{
							for (xx = 0; xx < 8; xx++)	// process 8 pixels in parallel
							{	
								pX_Array_Ptri = &pX_Array_Value[x+xx].pos[0];
								xpos0 = *(pX_Array_Ptri++);
								xpos1 = *(pX_Array_Ptri++);
								xpos2 = *(pX_Array_Ptri++);
								xpos3 = *(pX_Array_Ptri++);

								pp30.u8[xx] = *(pSrc_ypos3 + xpos0);
								pp31.u8[xx] = *(pSrc_ypos3 + xpos1);	
								pp32.u8[xx] = *(pSrc_ypos3 + xpos2);
								pp33.u8[xx] = *(pSrc_ypos3 + xpos3);	

								rxmm0 = _mm_loadl_epi64((__m128i*)(&pX_Array_Value[x+xx].icoeff[0]));
								int icoeff32 = _mm_cvtsi128_si32 (rxmm0);
								icoeffx0.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx1.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
								rxmm0 = _mm_srli_si128 (rxmm0, 4);
								icoeff32     = _mm_cvtsi128_si32 (rxmm0);
								icoeffx2.u16[xx] = (unsigned short)(icoeff32 & 0xffff);
								icoeffx3.u16[xx] = (unsigned short)((icoeff32 >> 16) & 0xffff);
							}

							//// LINE-0
							pRow0=pRow1;
							rxmm0 = _mm_load_si128((__m128i*)(pRow0 + x*2));

							//	// LINE-1
							pRow1=pRow2;
							rxmm1 = _mm_load_si128((__m128i*)(pRow1 + x*2));

							// LINE-2
							pRow2=pRow3;
							rxmm2 = _mm_load_si128((__m128i*)(pRow2 + x*2));


							// LINE-3
							rxmm3 = _mm_load_si128(&pp30.i);
							rxmm4 = _mm_load_si128(&pp31.i);

							rxmm5  = _mm_load_si128(&icoeffx0.i);
							rxmm6  = _mm_load_si128(&icoeffx1.i);
							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm5);	// p[0]*ixcoef0 
							rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm6);	// p[1]*ixcoef1 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm4);

							rxmm5 = _mm_load_si128(&pp32.i);
							rxmm6 = _mm_load_si128(&pp33.i);

							rxmm4  = _mm_load_si128(&icoeffx2.i);
							rxmm5 = _mm_mullo_epi16 (rxmm5, rxmm4);	// p[2]*ixcoef2 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm5);

							rxmm4  = _mm_load_si128(&icoeffx3.i);

							rxmm6 = _mm_mullo_epi16 (rxmm6, rxmm4);	// p[3]*ixcoef3 
							rxmm3 = _mm_add_epi16(rxmm3, rxmm6);
							rxmm3 = _mm_add_epi16(rxmm3, rxmm7);
							rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT_6);
							_mm_store_si128((__m128i *)(pRow3 + x*2), rxmm3);	

							// mm0 = r0, mm1 = r1, mm2 = r2, mm3 = r3
							//			*(pDst + x + y*dstStep) = (unsigned char)
							//									((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );		
							rxmm4  = _mm_set1_epi16(iycoef0);
							rxmm5  = _mm_set1_epi16(iycoef1);
							rxmm6  = _mm_set1_epi16(iycoef2);

							rxmm0 = _mm_mullo_epi16 (rxmm0, rxmm4);	// r0*iycoef0 
							rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm5);	// r1*iycoef1 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm1);

							rxmm4  = _mm_set1_epi16(iycoef3);

							rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm6);	// r2*iycoef2 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm2);

							rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm4);	// r3*iycoef3 
							rxmm0 = _mm_add_epi16(rxmm0, rxmm3);

							rxmm0 = _mm_add_epi16(rxmm0, rxmm7);
							rxmm0 = _mm_srli_epi16(rxmm0, FW_WEIGHT_6);

							rxmm0 = _mm_packus_epi16(rxmm0, rxmm0);		// Convert to 8 bit
							_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm0);

						}

						break;
					default:
						break;
					}

					for (; x < resizeX; x++)
					{
						int xpos0 = pX_Array_Value[x].pos[0];
						int xpos1 = pX_Array_Value[x].pos[1];
						int xpos2 = pX_Array_Value[x].pos[2];
						int xpos3 = pX_Array_Value[x].pos[3];

						int ixcoef0 = pX_Array_Value[x].icoeff[0];
						int ixcoef1 = pX_Array_Value[x].icoeff[1];
						int ixcoef2 = pX_Array_Value[x].icoeff[2];
						int ixcoef3 = pX_Array_Value[x].icoeff[3];

						p[0] = pSrc[xpos0 + ypos0];
						p[1] = pSrc[xpos1 + ypos0];
						p[2] = pSrc[xpos2 + ypos0];
						p[3] = pSrc[xpos3 + ypos0];
						int r0 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos1];
						p[1] = pSrc[xpos1 + ypos1];
						p[2] = pSrc[xpos2 + ypos1];
						p[3] = pSrc[xpos3 + ypos1];
						int r1 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos2];
						p[1] = pSrc[xpos1 + ypos2];
						p[2] = pSrc[xpos2 + ypos2];
						p[3] = pSrc[xpos3 + ypos2];
						int r2 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos3];
						p[1] = pSrc[xpos1 + ypos3];
						p[2] = pSrc[xpos2 + ypos3];
						p[3] = pSrc[xpos3 + ypos3];
						int r3 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						*(pDst + x + y*dstStep) = (unsigned char)
							((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

					}
				}
				else
				{
					for (x = 0; x < resizeX; x++)
					{
						int xpos0 = pX_Array_Value[x].pos[0];
						int xpos1 = pX_Array_Value[x].pos[1];
						int xpos2 = pX_Array_Value[x].pos[2];
						int xpos3 = pX_Array_Value[x].pos[3];

						int ixcoef0 = pX_Array_Value[x].icoeff[0];
						int ixcoef1 = pX_Array_Value[x].icoeff[1];
						int ixcoef2 = pX_Array_Value[x].icoeff[2];
						int ixcoef3 = pX_Array_Value[x].icoeff[3];

						p[0] = pSrc[xpos0 + ypos0];
						p[1] = pSrc[xpos1 + ypos0];
						p[2] = pSrc[xpos2 + ypos0];
						p[3] = pSrc[xpos3 + ypos0];
						int r0 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos1];
						p[1] = pSrc[xpos1 + ypos1];
						p[2] = pSrc[xpos2 + ypos1];
						p[3] = pSrc[xpos3 + ypos1];
						int r1 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos2];
						p[1] = pSrc[xpos1 + ypos2];
						p[2] = pSrc[xpos2 + ypos2];
						p[3] = pSrc[xpos3 + ypos2];
						int r2 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						p[0] = pSrc[xpos0 + ypos3];
						p[1] = pSrc[xpos1 + ypos3];
						p[2] = pSrc[xpos2 + ypos3];
						p[3] = pSrc[xpos3 + ypos3];
						int r3 = (int) ((p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

						*(pDst + x + y*dstStep) = (unsigned char)
							((r0*iycoef0 + r1*iycoef1 + r2*iycoef2 + r3*iycoef3+half_FW_WEIGHT_6)>>FW_WEIGHT_6 );

					}
				}
			}
			prevF1=pf1;
			prevF2=pf2;
			prevC1=pC1;
			prevC2=pC2;
		}

		//free allocated memory by fwMalloc
		fwFree(pRow0);
		fwFree(pRow1);
		fwFree(pRow2);
		fwFree(pRow3);

	}

	//Internal function for resize 8u data with Lanczos interpolation
	FwStatus My_FW_Resize_8u_C1R_LZ(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor)
	{
		// FwStatus stat_from_interpolation		
		int resizeHeight = (int)((double)srcRoi.height * yFactor);	
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;	

		int x, y, xpos, ypos, i;
		double sin_piX, cos_piX3, sin_piX3, pi, sqrt3, temp1, temp2, temp3;
		double value, ry;

		pi=3.1415926535897;
		sqrt3=1.73205081;

		//use fwMalloc instead of malloc for aligned address
		Lanczos_Array_Double *pX_Array_Value = (Lanczos_Array_Double*) fwMalloc(resizeWidth 
			* sizeof(Lanczos_Array_Double));
		Lanczos_Array_Double *pY_Array_Value = (Lanczos_Array_Double*) fwMalloc(resizeHeight 
			* sizeof(Lanczos_Array_Double));

		for (x=0;x<resizeWidth;x++) {
			temp1 = ((double)x / xFactor);
			xpos = (int) temp1;
			value = temp1 - xpos;
			sin_piX3 = sin(pi*value/3.0);
			cos_piX3 = cos(pi*value/3.0);
			sin_piX = sin_piX3*(3.0-4.0*sin_piX3*sin_piX3);
			temp1 = (sqrt3*cos_piX3+sin_piX3)/2.0;
			temp2 = (sqrt3*cos_piX3-sin_piX3)/2.0;
			temp3 = 3.0*sin_piX/(pi*pi);

			pX_Array_Value[x].icoeff[0] = temp3*temp2/((value+2)*(value+2));
			pX_Array_Value[x].icoeff[1] =-temp3*temp1/((value+1)*(value+1));
			pX_Array_Value[x].icoeff[3] = temp3*temp2/((1-value)*(1-value));
			pX_Array_Value[x].icoeff[4] =-temp3*temp1/((2-value)*(2-value));
			pX_Array_Value[x].icoeff[5] = temp3*sin_piX3/((3-value)*(3-value));
			//protect overflow
			pX_Array_Value[x].icoeff[2] = 1 - pX_Array_Value[x].icoeff[0] - pX_Array_Value[x].icoeff[1] -
				pX_Array_Value[x].icoeff[3] - pX_Array_Value[x].icoeff[4] - pX_Array_Value[x].icoeff[5];

			pX_Array_Value[x].pos[2] = srcRoi.x + xpos;

			xpos-=2;
			if (xpos <0) pX_Array_Value[x].pos[0] = srcRoi.x;
			else pX_Array_Value[x].pos[0] = srcRoi.x + xpos;

			xpos++;
			if (xpos <0) pX_Array_Value[x].pos[1] = srcRoi.x;
			else pX_Array_Value[x].pos[1] = srcRoi.x + xpos;

			xpos+=2;
			if (xpos >= srcRoi.width) {
				pX_Array_Value[x].pos[3] = pX_Array_Value[x].pos[2];
				pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[2];
				pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[2];
			} 
			else pX_Array_Value[x].pos[3] = srcRoi.x + xpos;

			xpos++;
			if (xpos >= srcRoi.width) {
				pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[3];
				pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[3];
			} 
			else pX_Array_Value[x].pos[4] = srcRoi.x + xpos;

			xpos++;
			if (xpos >= srcRoi.width) {
				pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[4];
			} 
			else pX_Array_Value[x].pos[5] = srcRoi.x + xpos;
		}

		for (y=0; y<resizeHeight; y++) {
			temp1 = ((double)y / yFactor);
			ypos = (int) temp1;
			value = temp1-ypos;
			sin_piX3 = sin(pi*value/3.0);
			cos_piX3 = cos(pi*value/3.0);
			sin_piX = sin_piX3*(3.0-4.0*sin_piX3*sin_piX3);
			temp1 = (sqrt3*cos_piX3+sin_piX3)/2.0;
			temp2 = (sqrt3*cos_piX3-sin_piX3)/2.0;
			temp3 = 3.0*sin_piX/(pi*pi);

			pY_Array_Value[y].icoeff[0] = temp3*temp2/((value+2)*(value+2));
			pY_Array_Value[y].icoeff[1] =-temp3*temp1/((value+1)*(value+1));
			pY_Array_Value[y].icoeff[3] = temp3*temp2/((1-value)*(1-value));
			pY_Array_Value[y].icoeff[4] =-temp3*temp1/((2-value)*(2-value));
			pY_Array_Value[y].icoeff[5] = temp3*sin_piX3/((3-value)*(3-value));
			//protect overflow
			pY_Array_Value[y].icoeff[2] = 1 - pY_Array_Value[y].icoeff[0] - pY_Array_Value[y].icoeff[1] -
				pY_Array_Value[y].icoeff[3] - pY_Array_Value[y].icoeff[4] - pY_Array_Value[y].icoeff[5];

			pY_Array_Value[y].pos[2] = (srcRoi.y + ypos) * srcStep;

			ypos-=2;
			if (ypos <0) pY_Array_Value[y].pos[0] = srcRoi.y * srcStep;
			else pY_Array_Value[y].pos[0] = (srcRoi.y + ypos) * srcStep;

			ypos++;
			if (ypos <0) pY_Array_Value[y].pos[1] = srcRoi.y *srcStep;
			else pY_Array_Value[y].pos[1] = pY_Array_Value[y].pos[0] + srcStep;

			ypos+=2;
			if (ypos >= srcRoi.height) {
				pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2];
				pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[2];
				pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[2];
			} 
			else pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2] + srcStep;

			ypos++;
			if (ypos >= srcRoi.height) {
				pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3];
				pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[3];
			} 
			else pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3] + srcStep;

			ypos++;
			if (ypos >= srcRoi.height) {
				pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4];
			} 
			else pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4] + srcStep;
		}

		//Calculating the pixel value
		int typos[6], txpos[6];
		double iycoef[6], ixcoef[6];

		for (y = 0; y < resizeHeight; y++) {
			for (i=0;i<6;i++) {
				typos[i] = pY_Array_Value[y].pos[i];
				iycoef[i]= pY_Array_Value[y].icoeff[i];
			}
			for (x = 0; x < resizeWidth; x++) {
				for (i=0;i<6;i++) {
					txpos[i] = pX_Array_Value[x].pos[i];
					ixcoef[i]= pX_Array_Value[x].icoeff[i];
				}

				ry=0;
				for (i=0;i<6;i++) {
					ry += (*(pSrc + typos[i] + txpos[0]) * ixcoef[0] +
						*(pSrc + typos[i] + txpos[1]) * ixcoef[1] +
						*(pSrc + typos[i] + txpos[2]) * ixcoef[2] +
						*(pSrc + typos[i] + txpos[3]) * ixcoef[3] +
						*(pSrc + typos[i] + txpos[4]) * ixcoef[4] +
						*(pSrc + typos[i] + txpos[5]) * ixcoef[5])*iycoef[i];
				}
				//saturate the result for corresponding data type
				*(pDst + y*dstStep + x) = FW_REF::Limits<Fw8u>::Sat(ry+0.5);
			}
		}

		//free allocated memory by fwMalloc
		fwFree(pX_Array_Value);
		fwFree(pY_Array_Value);

		return fwStsNoErr;
	}

	//Internal function for resize 8u data with SuperSampling interpolation
	FwStatus My_FW_Resize_8u_C1R_SU(const Fw8u *pSrc, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor)
	{
		//interpolation != FWI_INTER_SUPER
		//super sampling only support xFactor and yFactor <=1 cases.
		if (xFactor > 1.0 || yFactor > 1.0) return fwStsResizeFactorErr;
		// FwStatus stat_from_interpolation	
		int resizeHeight = (int)((double)srcRoi.height * yFactor);		
		int resizeWidth = (int)((double)srcRoi.width  * xFactor);	
		if(dstRoiSize.width < resizeWidth) resizeWidth = dstRoiSize.width;	
		if(dstRoiSize.height< resizeHeight) resizeHeight = dstRoiSize.height;	

		int x, y, ipos, jpos;
		double xFr, yFr, *result;

		// Outside buffer will not be initialized
		//if(dstRoiSize.width > resizeWidth) {
		//	for (x=resizeWidth;x<dstRoiSize.width;x++){
		//		for (y=0;y<dstRoiSize.height;y++) {
		//			*(pDst+x+y*dstStep) =0;
		//		}
		//	}
		//}
		//if(dstRoiSize.height > resizeHeight) {
		//	for (y=resizeHeight;y<dstRoiSize.height;y++){
		//		for (x=0;x<dstRoiSize.width;x++) {
		//			*(pDst+x+y*dstStep) =0;
		//		}
		//	}
		//}

		result = (double *)malloc(sizeof(double)*resizeWidth*resizeHeight);

		//initialize pDst, so we could mul_add
		for (y=0; y<resizeHeight; y++) {
			for (x=0;x<resizeWidth;x++) {
				*(result+x+y*resizeWidth)=0;
			}
		}

		//Real Add, should use FW_weight! To-Do ....
		jpos=0;
		for (y=0; y<((double)resizeHeight/yFactor); y++) {
			yFr=((double)(jpos+1))/yFactor-y;
			if (yFr>1) yFr=1;

			ipos=0;
			for (x=0;x<((double)resizeWidth/xFactor);x++) {
				xFr=((double)(ipos+1))/xFactor-x;
				if (xFr>1) xFr=1; //Could pre-calculated

				*(result+ipos+jpos*resizeWidth) += 
					*(pSrc+(x+srcRoi.x)+(y+srcRoi.y)*srcStep)*xFr*yFr;
				if (yFr<1 && (jpos+1) < resizeHeight) 
					*(result+ipos+(jpos+1)*resizeWidth) += 
					*(pSrc+(x+srcRoi.x)+(y+srcRoi.y)*srcStep)*xFr*(1-yFr);
				if (xFr<1 && (ipos+1) < resizeWidth && yFr<1&& (jpos+1) < resizeHeight )
					*(result+(ipos+1)+(jpos+1)*resizeWidth) += 
					*(pSrc+(x+srcRoi.x)+(y+srcRoi.y)*srcStep)*(1-xFr)*(1-yFr);
				if (xFr<1 && (ipos+1) < resizeWidth) {
					ipos++;
					*(result+ipos+jpos*resizeWidth) += 
						*(pSrc+(x+srcRoi.x)+(y+srcRoi.y)*srcStep)*(1-xFr)*yFr;
				}
			}

			if (yFr<1) jpos++;
		}

		//divide the weighted sum by Src/Dst=1/(xFactor*yFactor).
		//Should use fw_weight to fix the accuracy issue
		yFr=xFactor*yFactor;
		for (y=0; y<resizeHeight; y++) {
			for (x=0;x<resizeWidth;x++) {
				//saturate the result for corresponding data type
				*(pDst+x+y*dstStep) = FW_REF::Limits<Fw8u>::Sat
					(*(result+x+y*resizeWidth)*yFr+0.5);
			}
		}

		free(result);

		return fwStsNoErr;
	}
} //namespace OPT_LEVEL

using namespace OPT_LEVEL;


//Special function for C1 data type
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_Reisze_8u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);
		else if (interpolation == FWI_INTER_CUBIC) 
			return My_FW_Resize_8u_C1R_CU(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor);
		else if (interpolation == FWI_INTER_LANCZOS) 
			return My_FW_Resize_8u_C1R_LZ(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor);
		else //if (interpolation == FWI_INTER_SUPER) 
			return My_FW_Resize_8u_C1R_SU(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor);
	default:
		return My_FW_Resize <Fw8u, C1, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}


//All other functions defined in external header
// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C3R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 3);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
		else if (interpolation == FWI_INTER_NN)
			return My_FW_Reisze_8u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
	default:
		return My_FW_Resize <Fw8u, C3, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C4R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;						

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 4);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
		else if (interpolation == FWI_INTER_NN)
			return My_FW_Reisze_8u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
	default:
		return My_FW_Resize <Fw8u, C4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_AC4R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 4);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
		else if (interpolation == FWI_INTER_NN)
			//Add special handle for AC4 type. pDst will not get the 4th Value
			return My_FW_Reisze_8u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 5);
	default:
		return My_FW_Resize <Fw8u, AC4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C1R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_16u_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_Resize_16u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);

	default:
		return My_FW_Resize <Fw16u, C1, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C3R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 3);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_16u_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_Resize_16u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);

	default:
		return My_FW_Resize <Fw16u, C3, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C4R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 4);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_16u_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
		//else if (interpolation == FWI_INTER_NN) 
		//	return My_FW_Resize_16u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
		//	dstRoiSize, xFactor, yFactor, 4);
	default:
		return My_FW_Resize <Fw16u, C4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_AC4R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 5);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_16u_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 5);
		//else if (interpolation == FWI_INTER_NN) 
		//return My_FW_Resize_16u_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
		//dstRoiSize, xFactor, yFactor, 5);

	default:
		return My_FW_Resize <Fw16u, AC4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C1R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
													 Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
													 double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_32f_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_Resize_32f_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 1);

	default:
		return My_FW_Resize <Fw32f, C1, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C3R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
													 Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
													 double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 3);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_32f_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_Resize_32f_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);

	default:
		return My_FW_Resize <Fw32f, C3, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
													 Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
													 double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 4);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_32f_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
		//else if (interpolation == FWI_INTER_NN) 
		//return My_FW_Resize_32f_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
		//dstRoiSize, xFactor, yFactor, 4);
	default:
		return My_FW_Resize <Fw32f, C4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_AC4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
													  Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
													  double xFactor, double yFactor, int interpolation)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;							

	FwStatus status;
	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 5);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_32f_Linear(pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 5);
		//else if (interpolation == FWI_INTER_NN) 
		//return My_FW_Resize_32f_NN(pSrc, srcStep, srcRoi, pDst, dstStep,
		//dstRoiSize, xFactor, yFactor, 5);
	default:
		return My_FW_Resize <Fw32f, AC4, DT_REFR> (pSrc, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, interpolation);
	}
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<3;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2: 
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN_planar((const Fw8u**)pSrc, srcStep, srcRoi, (Fw8u**)pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
		else if (interpolation == FWI_INTER_NN)
			return My_FW_Resize_8u_NN_planar(pSrc, srcStep, srcRoi, (Fw8u**)pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 3);
	default:
		for (i=0;i<3; i++) {
			status = My_FW_Resize <Fw8u, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
				pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
			if (status != fwStsNoErr) return status;
		}
	}

	return status;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<3;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	for (i=0;i<3; i++) {
		status = My_FW_Resize <Fw16u, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
			pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
		if (status != fwStsNoErr) return status;
	}

	return status;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<3;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	for (i=0;i<3; i++) {
		status = My_FW_Resize <Fw32f, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
			pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
		if (status != fwStsNoErr) return status;
	}

	return status;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<4;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_Resize_8u_LN_planar(pSrc, srcStep, srcRoi, (Fw8u**)pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
		else if (interpolation == FWI_INTER_NN)
			return My_FW_Resize_8u_NN_planar(pSrc, srcStep, srcRoi, (Fw8u**)pDst, dstStep,
			dstRoiSize, xFactor, yFactor, 4);
	default:
		for (i=0;i<4; i++) {
			status = My_FW_Resize <Fw8u, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
				pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
			if (status != fwStsNoErr) return status;
		}
	}

	return status;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<4;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	for (i=0;i<4; i++) {
		status = My_FW_Resize <Fw16u, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
			pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
		if (status != fwStsNoErr) return status;
	}

	return status;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResize_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, int interpolation)
{
	FwStatus status=fwStsErr;
	int i;
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	for (i=0;i<4;i++) {
		if (pSrc[i] == 0 || pDst[i] == 0)  return fwStsNullPtrErr;
	}		

	status = My_FW_ResizeCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, interpolation, 1);
	if (status!=fwStsNoErr) return status;

	for (i=0;i<4; i++) {
		status = My_FW_Resize <Fw32f, C1, DT_REFR> (pSrc[i], srcStep, srcRoi, 
			pDst[i], dstStep, dstRoiSize, xFactor, yFactor, interpolation);
		if (status != fwStsNoErr) return status;
	}

	return status;
}

#endif //FW_BUILD_NUM >= 001


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
