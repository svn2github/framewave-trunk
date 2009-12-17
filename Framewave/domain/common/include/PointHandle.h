/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __POINTHANDLE_H__
#define __POINTHANDLE_H__

#include "fwImage.h"
#include "Resize.h"


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


	//General paramter checking with destination ROI fixing
	template< class TS>
	FwStatus My_FW_ParaCheck2(const TS* pSrc, FwiSize srcSize, int srcStep, 
		FwiRect srcRoi, TS* pDst, int dstStep, 
		FwiRect dstRoi, int channel)
	{
		if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

		if (srcSize.height <= 0 || srcSize.width <= 0 || 
			srcRoi.height <= 0 || srcRoi.width<= 0 ||
			dstRoi.height <= 0 || dstRoi.width<= 0 )	
			return fwStsSizeErr;	

		if (srcStep < channel || dstStep < channel) //at least one pixel 
			return fwStsStepErr;	

		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if ((srcRoi.x + srcRoi.width ) < 1 || (srcRoi.x >= srcSize.width) || 
			(srcRoi.y + srcRoi.height) < 1 || (srcRoi.y >= srcSize.height))	
			return fwStsWrongIntersectROI;

		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		if (srcRoi.width == 1 || srcRoi.height == 1 )
			return fwStsRectErr;

		//fix dst Roi issues
		if (dstRoi.x <0 ) {
			dstRoi.width +=dstRoi.x;
			dstRoi.x=0;
		}
		if (dstRoi.y <0 ) {
			dstRoi.height +=dstRoi.y;
			dstRoi.y=0;
		}
		if ((dstRoi.x + dstRoi.width ) < 1 || 
			(dstRoi.y + dstRoi.height) < 1 )	
			return fwStsWrongIntersectROI;

		return fwStsNoErr;
	}



		//handle each point individually
	template< class TS, DispatchType disp, int interpolation >
	void My_FW_PointHandle(float xmap, float ymap, int x, int y,
		const TS* pSrc, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, int *flag, 
		int channel, int channel1, Fw32f round)
	{
		int k;
		double result;

		if (xmap < 0 || xmap > srcRoi.width - 1 ||
			ymap < 0 || ymap > srcRoi.height- 1) {
				return;
		}

		xmap += srcRoi.x;
		ymap += srcRoi.y;

		if (interpolation == FWI_INTER_NN) {
			// Nearest Neighbor interpolation
			// Another approach is to add 0.5 for xint and yint calculation
			int xint = (int)xmap;
			int yint = (int)ymap;

			for (k=0;k<channel1;k++) {
				*(pDst+y*dstStep+x*channel+k) = *(pSrc+ yint*srcStep+xint*channel+k); 
			}
		} 
		else if (interpolation == FWI_INTER_LINEAR) {
			//Linear interpolation
			int xint = (int) xmap;
			int yint = (int) ymap;

			int	xint1 = xint+1;
			if (xint1 >= srcRoi.x + srcRoi.width)  xint1=xint;
			int	yint1 = yint+1;
			if (yint1 >= srcRoi.y + srcRoi.height) yint1=yint;

			double  xfraction = xmap-xint;
			double	yfraction = ymap-yint;					

			for (k=0;k<channel1;k++) {
				result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
					+ (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
					+ xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
					+ xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));
				
				//Saturation is needed for the data type
				*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
			}
		}
		else {// FWI_INTER_CUBIC:
			//Cubic interpolation.
			//We only support NN, Linear, Cubic for WarpAffine Transformation
			int xint[4], yint[4], i, j;
			xint[1] = (int) xmap;
			yint[1] = (int) ymap;
			xint[0] = xint[1]-1;
			if (xint[0] < srcRoi.x) xint[0]++;
			yint[0] = yint[1]-1;
			if (yint[0] < srcRoi.y) yint[0]++;

			for (i=2;i<4;i++) {
				xint[i]= xint[1]+i-1;
				if (xint[i] >= srcRoi.x + srcRoi.width)  xint[i]=xint[i-1];

				yint[i]= yint[1]+i-1;
				if (yint[i] >= srcRoi.y + srcRoi.height) yint[i]=yint[i-1];
			}

			//Cubic factor choose to be -0.5
			double aA = -0.5;
			double xfraction = xmap-xint[1];
			double yfraction = ymap-yint[1];
			double xfactor[4], yfactor[4];

			//aA*(xx*xx*xx-5.0*xx*xx + 8*xx - 4.0); // 1<= |x| < 2
			//(aA + 2.0)*xx*xx*xx-(aA+3.0)*xx*xx + 1;	// 0 <= |x| < 1
			//for xint-1, yint-1
			xfactor[0] = aA*xfraction*(1-xfraction)*(1-xfraction);
			yfactor[0] = aA*yfraction*(1-yfraction)*(1-yfraction);
			//for xint, yint
			xfactor[1] = (1-xfraction)*(1+xfraction-(aA+2)*xfraction*xfraction);
			yfactor[1] = (1-yfraction)*(1+yfraction-(aA+2)*yfraction*yfraction);
			//for xint+1, yint+1
			xfactor[2] = xfraction * (2-xfraction-(aA+2)*(1-xfraction)*(1-xfraction));
			yfactor[2] = yfraction * (2-yfraction-(aA+2)*(1-yfraction)*(1-yfraction));
			//for xint+2, yint+2
			xfactor[3] = aA*(1-xfraction)*xfraction*xfraction;
			yfactor[3] = aA*(1-yfraction)*yfraction*yfraction;

			double resultx;

			for (k=0;k<channel1;k++) {
				result=0;
				for (j=0;j<4;j++) {
					resultx=0;
					for (i=0;i<4;i++) {
						resultx += xfactor[i] * (*(pSrc+yint[j]*srcStep+xint[i]*channel+k));
					}

					result += yfactor[j] * resultx;
				}

				//Saturation is needed for the data type
				*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
			}
		}

		//flag for actual point handled
		*flag=1;

		return;	
	}


	//internal function for WarpAffine transformation
	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpAffine(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][3], int interpolation)
	{
		int interpolationE = interpolation ^ FWI_SMOOTH_EDGE;
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) {
				if ( interpolationE != FWI_INTER_NN && interpolationE != FWI_INTER_LINEAR 
					&& interpolationE != FWI_INTER_CUBIC)
					return fwStsInterpolationErr;	
				interpolation = interpolationE;
				interpolationE = FWI_SMOOTH_EDGE;
		}

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		//Shorter naming convention
		const double c00 = coeffs[0][0];
		const double c01 = coeffs[0][1];
		const double c02 = coeffs[0][2];
		const double c10 = coeffs[1][0];
		const double c11 = coeffs[1][1];
		const double c12 = coeffs[1][2];

		double det = (c00*c11 - c10*c01);
		if(det == 0.0) return fwStsCoeffErr;
		double inv_Det = 1/det ;

		const float r00 = (float)( c11 * inv_Det);	//  matrix inverse
		const float r01 = (float)(-c01 * inv_Det);
		const float r10 = (float)(-c10 * inv_Det);
		const float r11 = (float)(c00 * inv_Det);

		//	double xmap, ymap, tx, ty;
		float xmap, ymap, tx, ty;

		int x, y, flag=0;

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

		float const1 =  - (float)(r00 * c02 + r01*c12);
		float const2 =  - (float)(r10 * c02 + r11*c12);

		switch(interpolation){
		case(FWI_INTER_NN):
			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				tx = r01 * y + const1;
				ty = r11 * y + const2;

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					xmap = r00 * x + tx ;
					ymap = r10 * x + ty ;

					My_FW_PointHandle<TS, disp, FWI_INTER_NN> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, &flag, channel, channel1, round);
				}
			}
			break;
		case(FWI_INTER_LINEAR):
			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				tx = r01 * y + const1;
				ty = r11 * y + const2;

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					xmap = r00 * x + tx ;
					ymap = r10 * x + ty ;

					My_FW_PointHandle<TS, disp, FWI_INTER_LINEAR> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, &flag, channel, channel1, round);
				}
			}
			break;
		case(FWI_INTER_CUBIC):
			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				tx = r01 * y + const1;
				ty = r11 * y + const2;

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					xmap = r00 * x + tx ;
					ymap = r10 * x + ty ;

					My_FW_PointHandle<TS, disp, FWI_INTER_CUBIC> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, &flag, channel, channel1, round);
				}
			}
			break;

		default:
			assert(false);
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		if (interpolationE == FWI_SMOOTH_EDGE) {
			//use filter function to smooth edge.
		}

		return fwStsNoErr;
	}



	//General paramter checking
	template< class TS>
	FwStatus My_FW_ParaCheck(const TS*pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS*pDst, int dstStep, FwiSize dstRoiSize, int channel)
	{
		if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

		if (srcSize.height <= 0 || srcSize.width <= 0 || 
			srcRoi.height <= 0 || srcRoi.width<= 0 ||
			dstRoiSize.height <= 0 || dstRoiSize.width<= 0 )	
			return fwStsSizeErr;	

		if (srcStep < channel || dstStep < channel) //at least one pixel 
			return fwStsStepErr;	

		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if ((srcRoi.x + srcRoi.width ) < 1 || (srcRoi.x >= srcSize.width) || 
			(srcRoi.y + srcRoi.height) < 1 || (srcRoi.y >= srcSize.height))	
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


	//handle each point individually
	template< class TS, DispatchType disp >
	void My_FW_PointHandle(double xmap, double ymap, int x, int y,
		const TS* pSrc, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, int interpolation, int *flag, 
		int channel, int channel1, Fw32f round)
	{
		int k;
		double result;

		if (xmap < 0 || xmap > srcRoi.width - 1 ||
			ymap < 0 || ymap > srcRoi.height- 1) {
				return;
		}

		xmap += srcRoi.x;
		ymap += srcRoi.y;

		if (interpolation == FWI_INTER_NN) {
			//Potential Variation
			//int xint = (int)(xmap+ 0.5 );
			//int yint = (int)(ymap+ 0.5 );
			int xint = (int)xmap;
			int yint = (int)ymap;

			//Nearest Neighbor interpolation, no rounding is needed 
			//due to the same data type
			for (k=0;k<channel1;k++) {
				*(pDst+y*dstStep+x*channel+k) = 
					*(pSrc+ yint*srcStep+xint*channel+k);
			}
		} 
		else if (interpolation == FWI_INTER_LINEAR) {
			int xint = (int) xmap;
			int yint = (int) ymap;

			int	xint1 = xint+1;
			if (xint1 >= srcRoi.x + srcRoi.width)  xint1=xint;
			int	yint1 = yint+1;
			if (yint1 >= srcRoi.y + srcRoi.height) yint1=yint;

			double  xfraction = xmap-xint;
			double	yfraction = ymap-yint;					

			for (k=0;k<channel1;k++) {
				//Linear interpolation
				result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
					+ (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
					+ xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
					+ xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));
				
				//Saturation is needed for the data type
				*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
			}
		}
		else {// FWI_INTER_CUBIC:
			int xint[4], yint[4], i, j;
			xint[1] = (int) xmap;
			yint[1] = (int) ymap;
			xint[0] = xint[1]-1;
			if (xint[0] < srcRoi.x) xint[0]++;
			yint[0] = yint[1]-1;
			if (yint[0] < srcRoi.y) yint[0]++;

			for (i=2;i<4;i++) {
				xint[i]= xint[1]+i-1;
				if (xint[i] >= srcRoi.x + srcRoi.width)  xint[i]=xint[i-1];

				yint[i]= yint[1]+i-1;
				if (yint[i] >= srcRoi.y + srcRoi.height) yint[i]=yint[i-1];
			}

			//Cubic factor choose to be -0.5
			double aA = -0.5;
			double xfraction = xmap-xint[1];
			double yfraction = ymap-yint[1];
			double xfactor[4], yfactor[4];

			//aA*(xx*xx*xx-5.0*xx*xx + 8*xx - 4.0); // 1<= |x| < 2
			//(aA + 2.0)*xx*xx*xx-(aA+3.0)*xx*xx + 1;	// 0 <= |x| < 1
			//for xint-1, yint-1
			xfactor[0] = aA*xfraction*(1-xfraction)*(1-xfraction);
			yfactor[0] = aA*yfraction*(1-yfraction)*(1-yfraction);
			//for xint, yint
			xfactor[1] = (1-xfraction)*(1+xfraction-(aA+2)*xfraction*xfraction);
			yfactor[1] = (1-yfraction)*(1+yfraction-(aA+2)*yfraction*yfraction);
			//for xint+1, yint+1
			xfactor[2] = xfraction * (2-xfraction-(aA+2)*(1-xfraction)*(1-xfraction));
			yfactor[2] = yfraction * (2-yfraction-(aA+2)*(1-yfraction)*(1-yfraction));
			//for xint+2, yint+2
			xfactor[3] = aA*(1-xfraction)*xfraction*xfraction;
			yfactor[3] = aA*(1-yfraction)*yfraction*yfraction;

			double resultx;

			for (k=0;k<channel1;k++) {
				result=0;
				for (j=0;j<4;j++) {
					//Cubic interpolation
					resultx=0;
					for (i=0;i<4;i++) {
						resultx += xfactor[i] * (*(pSrc+yint[j]*srcStep+xint[i]*channel+k));
					}

					result += yfactor[j] * resultx;
				}

				//Saturation is needed for the data type
				*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
			}
		}

		//flag for actual point handled
		*flag=1;

		return;	
	}






}

#endif // __POINTHANDLE_H__

