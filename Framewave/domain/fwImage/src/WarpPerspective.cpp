/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

#if BUILD_NUM_AT_LEAST( 100 )

namespace OPT_LEVEL
{
	//General paramter checking with destination ROI fixing
	template< class TS>
	extern FwStatus My_FW_ParaCheck2(const TS* pSrc, FwiSize srcSize, int srcStep, 
		FwiRect srcRoi, TS* pDst, int dstStep, 
		FwiRect dstRoi, int channel);

	//handle each point individually
	template< class TS, DispatchType disp >
	extern void My_FW_PointHandle(double xmap, double ymap, int x, int y,
		const TS* pSrc, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, int interpolation, int *flag, 
		int channel, int channel1, Fw32f round);

	//Description
	//The function fwiWarpPerspective is declared in the fwi.h file. This perspective warp
	//function transforms the source image pixel coordinates (x,y) according to the following formulas:
	//	x? = (c00*x + c01*y + c02)/(c20*x + c21*y + c22)
	//	y? = (c10*x + c11*y + c12)/(c20*x + c21*y + c22)
	//where x? and y? denote the pixel coordinates in the transformed image, and cij are the perspective
	//transform coefficients passed in the array coeffs.

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpPerspective(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[3][3], int interpolation)
	{
		//if c22=0, c20*x+ c21*y + c22=0 when x=0 & y=0
		if (coeffs[2][2] == 0) return fwStsCoeffErr;

		//Get whether we need edge smooth feature
		int interpolationE = interpolation ^ FWI_SMOOTH_EDGE;
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) {
				if ( interpolationE != FWI_INTER_NN && interpolationE != FWI_INTER_LINEAR 
					&& interpolationE != FWI_INTER_CUBIC)
					return fwStsInterpolationErr;	
				interpolation = interpolationE;
				interpolationE = FWI_SMOOTH_EDGE;
		}	

		//Parameter checking
		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		double det[3][3];
		det[0][0] = coeffs[0][0]*coeffs[1][1] - coeffs[1][0]*coeffs[0][1];
		det[0][1] = coeffs[0][1]*coeffs[1][2] - coeffs[1][1]*coeffs[0][2];	
		det[0][2] = coeffs[0][2]*coeffs[1][0] - coeffs[1][2]*coeffs[0][0];
		if (coeffs[2][2] * det[0][0] + coeffs[2][0] * det[0][1] + 
			coeffs[2][1] * det[0][2] == 0) return fwStsCoeffErr;

		det[1][0] = coeffs[1][0]*coeffs[2][1] - coeffs[2][0]*coeffs[1][1];
		det[1][1] = coeffs[1][1]*coeffs[2][2] - coeffs[2][1]*coeffs[1][2];	
		det[1][2] = coeffs[1][2]*coeffs[2][0] - coeffs[2][2]*coeffs[1][0];

		det[2][0] = coeffs[2][0]*coeffs[0][1] - coeffs[0][0]*coeffs[2][1];
		det[2][1] = coeffs[2][1]*coeffs[0][2] - coeffs[0][1]*coeffs[2][2];	
		det[2][2] = coeffs[2][2]*coeffs[0][0] - coeffs[0][2]*coeffs[2][0];

		double xmap, ymap, detx, dety, tx, ty;
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

		for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
			dety = det[0][0] + y*det[2][0];
			tx   = det[0][1] + y*det[2][1];
			ty   = det[0][2] + y*det[2][2];

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
				detx = dety + det[1][0]*x;			
				//if (detx == 0.0) return fwStsCoeffErr;
				// case detx == 0, we set the value to be 0 and doesn't return
				// fwStsCoeffErr, so the image will be nicer
				if (detx == 0.0) {
					xmap=-1;
					ymap=-1;
				} else {
					xmap = (tx + det[1][1]*x)/detx;
					ymap = (ty + det[1][2]*x)/detx;
				}

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		if (interpolationE == FWI_SMOOTH_EDGE) {
			//use filter function to smooth edge.
		}

		return fwStsNoErr;
	}


	//Description
	//The function fwiWarpPerspectiveBack is declared in the fwi.h file. This function
	//performs the inverse transform to that defined by fwiWarpPerspective function. Pixel
	//coordinates x? and y? in the transformed image are obtained from the following equations
	//	(c00*x? + c01*y? + c02)/(c20*x? + c21*y? + c22) = x
	//	(c10*x? + c11*y? + c12)/(c20*x? + c21*y? + c22) = y
	//where x and y denote the pixel coordinates in the source image, and coefficients cij are given in the
	//array coeffs.Thus, you don’t need to invert transform coefficients in your application program
	//before calling fwiWarpPerspectiveBack.

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpPerspectiveBack(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[3][3], int interpolation)
	{
		//WarpPerspectiveBack doesn't support smooth_edge functionality
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) 
			return fwStsInterpolationErr;	

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		double xmap, ymap, tx, ty, detx, dety, det;
		int x, y, flag=0;

		det  = coeffs[0][0]*coeffs[1][1] - coeffs[1][0]*coeffs[0][1];
		detx = coeffs[0][1]*coeffs[1][2] - coeffs[1][1]*coeffs[0][2];	
		dety = coeffs[0][2]*coeffs[1][0] - coeffs[1][2]*coeffs[0][0];
		if (coeffs[2][2] * det + coeffs[2][0] * detx + 
			coeffs[2][1] * dety == 0) return fwStsCoeffErr;
		if (coeffs[2][2] == 0) return fwStsCoeffErr;
		if (coeffs[2][0]==0 && coeffs[2][1] ==0) return fwStsCoeffErr;

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

		for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
			dety = coeffs[2][1] * y + coeffs[2][2];
			tx =   coeffs[0][1] * y + coeffs[0][2];
			ty =   coeffs[1][1] * y + coeffs[1][2];

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
				detx = coeffs[2][0] * x +dety;
				//if (detx == 0.0) return fwStsCoeffErr;
				// case detx == 0, we set the value to be 0 and doesn't return
				// fwStsCoeffErr, so the image will be nicer
				if (detx == 0.0) {
					xmap=-1;
					ymap=-1;
				} else {
					xmap = (coeffs[0][0] * x + tx)/detx;
					ymap = (coeffs[1][0] * x + ty)/detx;
				}

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		return fwStsNoErr;
	}


	//Description
	//The function fwiWarpPerspectiveQuad is declared in the fwi.h file. This function applies
	//a perspective transform to an arbitrary quadrangle srcQuad in the source image pSrc. The
	//operations take place only in the intersection of the source image ROI srcRoi and the source
	//quadrangle. The function fwiWarpPerspectiveQuad uses the same formulas for pixel
	//mapping as in the case of the fwiWarpPerspective function. Transform coefficients are
	//computed internally, based on the mapping of the source quadrangle to the quadrangle dstQuad
	//specified in the destination image pDst. The dstQuad should have a non-empty intersection with
	//the destination image ROI dstRoi.
	//The first dimension [4] of the array specifying the quadrangle srcQuad[4][2] or
	//dstQuad[4][2] is equal to the number of vertices, and the second dimension [2] holds x and y
	//coordinates of the vertex.
	//Edge smoothing interpolation is applicable only if the destination quadrangle lies in the 
	//destination image ROI.

	extern void My_FW_Triangle_Sort( double triangle[3][2]);

	extern void My_FW_Triangle_Intersection (double triangle[3][2], FwiRect dstRoi, int y,
		int *xleft, int *xright);

	//coeffs is based on quadrangle coordinate mapping from dstQuad to SrcQuad.
	int My_FW_PerspectiveQuadCoeff(const double dstQuad[4][2], const double srcQuad[4][2], double coeff[3][3])
	{
		//shorter naming convention
		double x0=dstQuad[0][0], y0=dstQuad[0][1], sx0=srcQuad[0][0], sy0=srcQuad[0][1];
		double x1=dstQuad[1][0], y1=dstQuad[1][1], sx1=srcQuad[1][0], sy1=srcQuad[1][1];
		double x2=dstQuad[2][0], y2=dstQuad[2][1], sx2=srcQuad[2][0], sy2=srcQuad[2][1];
		double x3=dstQuad[3][0], y3=dstQuad[3][1], sx3=srcQuad[3][0], sy3=srcQuad[3][1];

		//level two coeffs
		//S[0][0]*C20 + S[0][1]*C21 + S[0][2]*C22 = S[0][3]*C00+s[0][4]*c01
		//S1[0][0]*C20 + S1[0][1]*C21 + S1[0][2]*C22 = S1[0][3]*C00+s1[0][4]*c01
		double s[3][5], s1[3][5];
		s[0][0] = x1*sx1-x0*sx0;
		s[0][1] = y1*sx1-y0*sx0;
		s[0][2] = sx1   -   sx0;
		s[0][3] = x1 - x0;
		s[0][4] = y1 - y0;
		s[1][0] = x2*sx2-x0*sx0;
		s[1][1] = y2*sx2-y0*sx0;
		s[1][2] = sx2   -   sx0;
		s[1][3] = x2 - x0;
		s[1][4] = y2 - y0;
		s[2][0] = x3*sx3-x0*sx0;
		s[2][1] = y3*sx3-y0*sx0;
		s[2][2] = sx3   -   sx0;
		s[2][3] = x3 - x0;
		s[2][4] = y3 - y0;

		s1[0][0] = x1*sy1-x0*sy0;
		s1[0][1] = y1*sy1-y0*sy0;
		s1[0][2] = sy1   -   sy0;
		s1[0][3] = x1 - x0;
		s1[0][4] = y1 - y0;
		s1[1][0] = x2*sy2-x0*sy0;
		s1[1][1] = y2*sy2-y0*sy0;
		s1[1][2] = sy2   -   sy0;
		s1[1][3] = x2 - x0;
		s1[1][4] = y2 - y0;
		s1[2][0] = x3*sy3-x0*sy0;
		s1[2][1] = y3*sy3-y0*sy0;
		s1[2][2] = sy3   -   sy0;
		s1[2][3] = x3 - x0;
		s1[2][4] = y3 - y0;

		//level three coeffs
		//t[0][0]*C20 + t[0][1]*C21 + t[0][2]*C22 = t[0][3]*c00
		//t1[0][0]*C20 + t1[0][1]*C21 + t1[0][2]*C22 = t1[0][3]*c00
		double t[2][4], t1[2][4];
		if (s[0][4]!=0) {
			t[0][0] = s[0][0] * s[1][4] - s[1][0] * s[0][4];
			t[0][1] = s[0][1] * s[1][4] - s[1][1] * s[0][4];
			t[0][2] = s[0][2] * s[1][4] - s[1][2] * s[0][4];
			t[0][3] = s[0][3] * s[1][4] - s[1][3] * s[0][4];
			t[1][0] = s[0][0] * s[2][4] - s[2][0] * s[0][4];
			t[1][1] = s[0][1] * s[2][4] - s[2][1] * s[0][4];
			t[1][2] = s[0][2] * s[2][4] - s[2][2] * s[0][4];
			t[1][3] = s[0][3] * s[2][4] - s[2][3] * s[0][4];
		} else {
			t[0][0] = s[0][0];
			t[0][1] = s[0][1];
			t[0][2] = s[0][2];
			t[0][3] = s[0][3];
			if (s[1][4]!=0) {
				t[1][0] = s[1][0] * s[2][4] - s[2][0] * s[1][4];
				t[1][1] = s[1][1] * s[2][4] - s[2][1] * s[1][4];
				t[1][2] = s[1][2] * s[2][4] - s[2][2] * s[1][4];
				t[1][3] = s[1][3] * s[2][4] - s[2][3] * s[1][4];
			} else {
				t[1][0] = s[1][0];
				t[1][1] = s[1][1];
				t[1][2] = s[1][2];
				t[1][3] = s[1][3];
			}
		}

		if (s1[0][4]!=0) {
			t1[0][0] = s1[0][0] * s1[1][4] - s1[1][0] * s1[0][4];
			t1[0][1] = s1[0][1] * s1[1][4] - s1[1][1] * s1[0][4];
			t1[0][2] = s1[0][2] * s1[1][4] - s1[1][2] * s1[0][4];
			t1[0][3] = s1[0][3] * s1[1][4] - s1[1][3] * s1[0][4];
			t1[1][0] = s1[0][0] * s1[2][4] - s1[2][0] * s1[0][4];
			t1[1][1] = s1[0][1] * s1[2][4] - s1[2][1] * s1[0][4];
			t1[1][2] = s1[0][2] * s1[2][4] - s1[2][2] * s1[0][4];
			t1[1][3] = s1[0][3] * s1[2][4] - s1[2][3] * s1[0][4];
		} else {
			t1[0][0] = s1[0][0];
			t1[0][1] = s1[0][1];
			t1[0][2] = s1[0][2];
			t1[0][3] = s1[0][3];
			if (s1[1][4]!=0) {
				t1[1][0] = s1[1][0] * s1[2][4] - s1[2][0] * s1[1][4];
				t1[1][1] = s1[1][1] * s1[2][4] - s1[2][1] * s1[1][4];
				t1[1][2] = s1[1][2] * s1[2][4] - s1[2][2] * s1[1][4];
				t1[1][3] = s1[1][3] * s1[2][4] - s1[2][3] * s1[1][4];
			} else {
				t1[1][0] = s1[1][0];
				t1[1][1] = s1[1][1];
				t1[1][2] = s1[1][2];
				t1[1][3] = s1[1][3];
			}
		}

		//level four coeffs
		//f[0]*c20  + f[1]*c21  + f[2]*c22  =0
		//f1[0]*c20 + f1[1]*c21 + f1[2]*c22 =0
		double f[3], f1[3];
		if (t[0][3] !=0) {
			f[0] = t[0][0] * t[1][3] - t[1][0] * t[0][3];
			f[1] = t[0][1] * t[1][3] - t[1][1] * t[0][3];
			f[2] = t[0][2] * t[1][3] - t[1][2] * t[0][3];
		} else {
			f[0] = t[0][0];
			f[1] = t[0][1];
			f[2] = t[0][2];
		}

		if (t1[0][3] !=0) {
			f1[0] = t1[0][0] * t1[1][3] - t1[1][0] * t1[0][3];
			f1[1] = t1[0][1] * t1[1][3] - t1[1][1] * t1[0][3];
			f1[2] = t1[0][2] * t1[1][3] - t1[1][2] * t1[0][3];
		} else {
			f1[0] = t1[0][0];
			f1[1] = t1[0][1];
			f1[2] = t1[0][2];
		}

		//Level five coeffs
		//u*c21+v*c22=0
		double u, v;
		if (f[0]!=0) {
			u = f[1] * f1[0] - f1[1]*f[0];
			v = f[2] * f1[0] - f1[2]*f[0];
		} else {
			u=f[1];
			v=f[2];
		}

		if (u==0 && v==0) return 1;//Error, infinite solution

		double c00, c01, c02, c10, c11, c12, c20, c21, c22;
		if (u==0) {
			c22=0;
			c21=1;
		} else {
			c22=1;
			c21=-v/u;
		}

		if (f[0]!=0) {
			c20 = - (c21*f[1]+c22*f[2])/f[0];
		} else {//f1[0]!=0 otherwise u=0, v=0
			c20 = - (c21*f1[1]+c22*f1[2])/f1[0];
		}

		// t[0][3] = (x1-x0)(y2-y0)-(x2-x0)(y1-y0)
		// if t[0][3] = 0 
		// case 1 x1=x0, we have x2=x0 or y1=y0, either cases are error
		// case 2 x2=x0 similar to case 1
		// case 3, x1!=x0, x2!=x0, we have (y2-y0)/(x2-x0)=(y1-y0)/(x1-x0)
		//        this means (x0, y0), (x1, y1), (x2, y2) are in ONE straight line
		if (t[0][3] == 0 || t1[0][3] == 0) return 1;
		c00 = (c20*t[0][0] + c21*t[0][1] + c22*t[0][2])/t[0][3];
		c10 = (c20*t1[0][0] + c21*t1[0][1] + c22*t1[0][2])/t1[0][3];

		if (s[0][4] !=0) {//s04= y1-y0
			c01 = (c20*s[0][0] + c21*s[0][1] + c22*s[0][2] - c00*s[0][3])/s[0][4];
		} else { 
			//s14 = y2-y0, if s04=0 & s14=0, then t[0][3]=0, this case has been handled
			//by above case. this means s14!=0 in this step
			c01 = (c20*s[1][0] + c21*s[1][1] + c22*s[1][2] - c00*s[1][3])/s[1][4];
		}
		if (s1[0][4] !=0) {//s1[0][4]= y1-y0
			c11 = (c20*s1[0][0] + c21*s1[0][1] + c22*s1[0][2] - c10*s1[0][3])/s1[0][4];
		} else { 
			//s1[1][4] = y2-y0, if s1[0][4]=0 & s1[1][4]=0, then t[0][3]=0, this case has been handled
			//by above case. this means s1[1][4]!=0 in this step
			c11 = (c20*s1[1][0] + c21*s1[1][1] + c22*s1[1][2] - c10*s1[1][3])/s1[1][4];
		}

		c02 = (c20*x0 + c21*y0 + c22) * sx0 - c00*x0 -c01*y0;
		c12 = (c20*x0 + c21*y0 + c22) * sy0 - c10*x0 -c11*y0;

		coeff[0][0]=c00; 
		coeff[0][1]=c01; 
		coeff[0][2]=c02;
		coeff[1][0]=c10; 
		coeff[1][1]=c11; 
		coeff[1][2]=c12;
		coeff[2][0]=c20; 
		coeff[2][1]=c21; 
		coeff[2][2]=c22;

		return 0; //normal case
	}

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpPerspectiveQuad(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		const double srcQuad[4][2], TS* pDst, int dstStep, FwiRect dstRoi, 
		const double dstQuad[4][2], int interpolation)
	{
		//step 0: argument checking
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
		FwStatus fwstatus = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (fwstatus !=fwStsNoErr) return fwstatus;

		// step 1: find coeff for WarpPerspective
		// this coeff is mapping from dstQuad to srcQuad, inverse mapping
		double coeff[3][3];
		int status;
		status=My_FW_PerspectiveQuadCoeff(dstQuad, srcQuad, coeff);
		if (status!=0) return fwStsQuadErr;

		//Step 2: split the quadrangle into two triangles
		// Step 2.1 decide whether quadrangle is convex type
		double b0 = dstQuad[2][0]-dstQuad[0][0];
		double b1 = dstQuad[2][1]-dstQuad[0][1];
		// dstQuad[1]
		double x1 = dstQuad[1][0]-dstQuad[0][0];
		double y1 = dstQuad[1][1]-dstQuad[0][1];
		double result1 = x1/b0-y1/b1;
		//dstQuad[3]
		x1 = dstQuad[3][0]-dstQuad[0][0];
		y1 = dstQuad[3][1]-dstQuad[0][1];
		double result2 = x1/b0-y1/b1;

		// Step 2.2 split into two triangles
		double triangle1[3][2], triangle2[3][2];
		//Triangle one has [0] & [1]
		triangle1[0][0] = dstQuad[0][0];
		triangle1[0][1] = dstQuad[0][1];
		triangle1[1][0] = dstQuad[1][0];
		triangle1[1][1] = dstQuad[1][1];
		//Triangle two has [2] & [3]
		triangle2[0][0] = dstQuad[3][0];
		triangle2[0][1] = dstQuad[3][1];
		triangle2[1][0] = dstQuad[2][0];
		triangle2[1][1] = dstQuad[2][1];

		if (result1*result2 > 0) {
			// dstQuad[1] and dstQuad[3] are in the same side of line [0] & [2]
			// triangle one has [0][1][3] triangle two has [2][3][1]
			triangle1[2][0] = dstQuad[3][0];
			triangle1[2][1] = dstQuad[3][1];

			triangle2[2][0] = dstQuad[1][0];
			triangle2[2][1] = dstQuad[1][1];
		} 
		else if (result1*result2 < 0) {
			// dstQuad[1] and dstQuad[3] are in the different side of line [0] & [2]
			// triangle one has [0][1][2] triangle two has [2][3][0]
			triangle1[2][0] = dstQuad[2][0];
			triangle1[2][1] = dstQuad[2][1];

			triangle2[2][0] = dstQuad[0][0];
			triangle2[2][1] = dstQuad[0][1];
		} else {
			//result1=0 or result2=0 
			// the quadrangle becomes line or triangle
			return fwStsQuadErr; 
		}

		//Step3: sort triangle
		My_FW_Triangle_Sort(triangle1);
		My_FW_Triangle_Sort(triangle2);

		//step4: handle points
		double xmap, ymap, tx, ty, detx, dety;
		int x, y, xleft, xright, flag=0;

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

		for (y=dstRoi.y;y<dstRoi.y+dstRoi.height;y++) {
			dety = coeff[2][1] * y + coeff[2][2];
			tx   = coeff[0][1] * y + coeff[0][2];
			ty   = coeff[1][1] * y + coeff[1][2];

			//points in first triangle
			My_FW_Triangle_Intersection (triangle1, dstRoi, y, &xleft, &xright);
			for (x=xleft;x<=xright;x++) {
				detx = coeff[2][0] * x + dety;
				//if (detx == 0.0) return fwStsQuadErr;
				// case detx == 0, we set the value to be 0 and doesn't return
				// fwStsCoeffErr, so the image will be nicer
				if (detx == 0.0) {
					xmap=-1;
					ymap=-1;
				} else {
					xmap = (coeff[0][0] * x + tx) / detx;
					ymap = (coeff[1][0] * x + ty) / detx;
				}

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}

			//points in second triangle
			My_FW_Triangle_Intersection (triangle2, dstRoi, y, &xleft, &xright);
			for (x=xleft;x<=xright;x++) {
				detx = coeff[2][0] * x + dety;
				//if (detx == 0.0) return fwStsQuadErr;
				// case detx == 0, we set the value to be 0 and doesn't return
				// fwStsCoeffErr, so the image will be nicer
				if (detx == 0.0) {
					xmap=-1;
					ymap=-1;
				} else {
					xmap = (coeff[0][0] * x + tx) / detx;
					ymap = (coeff[1][0] * x + ty) / detx;
				}

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		//Smooth Egde
		if (interpolationE == FWI_SMOOTH_EDGE) {
			//check whether destination quadrangle is in dstRoi
			//If not, no edge smooth will be done
			int yheight = dstRoi.y + dstRoi.height;
			int xwidth  = dstRoi.x + dstRoi.width;
			if (triangle1[0][1] < dstRoi.y || triangle1[2][1] > yheight ||
				triangle2[0][1] < dstRoi.y || triangle2[2][1] > yheight ||
				triangle1[0][0] < dstRoi.x || triangle1[0][0] < xwidth  ||
				triangle1[1][0] < dstRoi.x || triangle1[1][0] < xwidth  ||
				triangle1[2][0] < dstRoi.x || triangle1[2][0] < xwidth  ||
				triangle2[0][0] < dstRoi.x || triangle2[0][0] < xwidth  ||
				triangle2[1][0] < dstRoi.x || triangle2[1][0] < xwidth  ||
				triangle2[2][0] < dstRoi.x || triangle2[2][0] < xwidth )
				return fwStsNoErr;

			//otherwise, we will do the edge smoothing
			//to-do
		}

		return fwStsNoErr;
	}


	//Description
	//The function fwiGetPerspectiveQuad is declared in the fwi.h file. It operates with ROI
	//(see ROI Processing in Geometric Transforms).
	//This function is used as a support function for fwiWarpPerspective. It computes vertex
	//coordinates of the quadrangle, to which the source rectangular ROI would be mapped by the
	//perspective transform function fwiWarpPerspective using the given coefficients coeffs.
	//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
	//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
	//following meaning:
	//quad[0] corresponds to the transformed top-left corner of the source ROI,
	//quad[1] corresponds to the transformed top-right corner of the source ROI,
	//quad[2] corresponds to the transformed bottom-right corner of the source ROI,
	//quad[3] corresponds to the transformed bottom-left corner of the source ROI.

	FwStatus My_FW_LineCheck(double quad[4][2])
	{
		float temp1, temp2;

		//using floating precise
		//check 0,1,2
		temp1=(float)((quad[2][0]-quad[0][0])*(quad[1][1]-quad[0][1]));
		temp2=(float)((quad[2][1]-quad[0][1])*(quad[1][0]-quad[0][0]));
		if (temp1==temp2) return fwStsCoeffErr;

		//check 0,1,3
		temp1=(float)((quad[3][0]-quad[0][0])*(quad[1][1]-quad[0][1]));
		temp2=(float)((quad[3][1]-quad[0][1])*(quad[1][0]-quad[0][0]));
		if (temp1==temp2) return fwStsCoeffErr;

		//check 0,2,3
		temp1=(float)((quad[3][0]-quad[0][0])*(quad[2][1]-quad[0][1]));
		temp2=(float)((quad[3][1]-quad[0][1])*(quad[2][0]-quad[0][0]));
		if (temp1==temp2) return fwStsCoeffErr;

		//check 1,2,3
		temp1=(float)((quad[3][0]-quad[1][0])*(quad[2][1]-quad[1][1]));
		temp2=(float)((quad[3][1]-quad[1][1])*(quad[2][0]-quad[1][0]));
		if (temp1==temp2) return fwStsCoeffErr;

		return fwStsNoErr;
	}


	//The function fwiGetPerspectiveBound is declared in the fwi.h file. It operates with ROI
	//(see ROI Processing in Geometric Transforms).
	//This function is used as a support function for fwiWarpPerspective. It computes vertex
	//coordinates of the smallest bounding rectangle for the quadrangle quad, to which the source ROI
	//would be mapped by the perspective transform function fwiWarpPerspective using the given
	//coefficients coeffs.
	//bound[0] specifies x, y coordinates of the top-left corner, bound[1] specifies x, y coordinates
	//of the bottom-right corner.

	extern void My_FW_QuadBound (const double quad[4][2], double bound[2][2]);
} // namespace OPT_LEVEL 

using namespace OPT_LEVEL;
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspective <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspective <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspective <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspective <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspective <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	return My_FW_WarpPerspectiveBack <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspectiveBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspectiveBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspectiveBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[3][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspectiveBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpPerspectiveQuad <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspectiveQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspectiveQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpPerspectiveQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpPerspectiveQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveQuad)(
	FwiRect srcRoi, double quad[4][2], const double coeffs[3][3])
{
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	double width = srcRoi.width-1, height=srcRoi.height-1;

	double xwidth = width* coeffs[0][0];
	double ywidth = width * coeffs[1][0];
	double dwidth = width * coeffs[2][0];
	double xheight = height * coeffs[0][1];
	double yheight = height * coeffs[1][1];
	double dheight = height * coeffs[2][1];

	double xup = srcRoi.x * coeffs[0][0] + srcRoi.y * coeffs[0][1] + coeffs[0][2];
	double yup = srcRoi.x * coeffs[1][0] + srcRoi.y * coeffs[1][1] + coeffs[1][2];
	double dwn = srcRoi.x * coeffs[2][0] + srcRoi.y * coeffs[2][1] + coeffs[2][2];
	if (dwn == 0.0) return fwStsCoeffErr;
	// this function just check the four vertex are in good mapping and did not
	// check the whole area.

	quad[0][0] = xup/dwn;
	quad[0][1] = yup/dwn;
	double denum = dwn+dwidth;
	if (denum == 0.0) return fwStsCoeffErr;
	quad[1][0] = (xup+xwidth)/denum;
	quad[1][1] = (yup+ywidth)/denum;

	denum = dwn+dheight;
	if (denum == 0.0) return fwStsCoeffErr;
	quad[3][0] = (xup+xheight)/denum;
	quad[3][1] = (yup+yheight)/denum;

	denum += dwidth;
	if (denum == 0.0) return fwStsCoeffErr;
	quad[2][0] = (xup+xwidth+xheight)/denum;
	quad[2][1] = (yup+ywidth+yheight)/denum;

	return My_FW_LineCheck(quad);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveBound)(
	FwiRect srcRoi, double bound[2][2], const double coeffs[3][3])
{
	if (srcRoi.width <= 0 || srcRoi.height <=0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	double quad[4][2];
	FwStatus status;

	status=fwiGetPerspectiveQuad (srcRoi, quad, coeffs);
	if (status != fwStsNoErr) return status;

	My_FW_QuadBound(quad, bound);

	return fwStsNoErr;
}


//Description
//The function fwiGetPerspectiveTransform is declared in the fwi.h file. It operates with
//ROI (see ROI Processing in Geometric Transforms).
//This function is used as a support function for fwiWarpPerspective. It computes the
//coefficients coeffs that should be used by the function fwiWarpPerspective to map the
//source rectangular ROI to the quadrangle with the given vertex coordinates quad.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//quad[0] corresponds to the transformed top-left corner of the source ROI,
//quad[1] corresponds to the transformed top-right corner of the source ROI,
//quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//quad[3] corresponds to the transformed bottom-left corner of the source ROI.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveTransform)(FwiRect srcRoi, const double quad[4][2],
															  double coeffs[3][3])
{
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	int status;
	double width = srcRoi.width-1, height=srcRoi.height-1;
	double start[4][2];
	start[0][0]=srcRoi.x;
	start[0][1]=srcRoi.y;
	start[1][0]=srcRoi.x+width;
	start[1][1]=srcRoi.y;
	start[3][0]=srcRoi.x;
	start[3][1]=srcRoi.y+height;
	start[2][0]=srcRoi.x+width;
	start[2][1]=srcRoi.y+height;

	status=My_FW_PerspectiveQuadCoeff(start, quad, coeffs);
	if (status!=0) return fwStsCoeffErr;

	return fwStsNoErr;
}

#endif //FW_BUILD_NUM >= 001

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
