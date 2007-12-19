/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

#if BUILD_NUM_AT_LEAST( 100 )

using namespace OPT_LEVEL;

namespace OPT_LEVEL
{

	//handle each point individually
	template< class TS, DispatchType disp >
	extern void My_FW_PointHandle(double xmap, double ymap, int x, int y,
		const TS* pSrc, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, int interpolation, int *flag, 
		int channel, int channel1, Fw32f round);

	//General paramter checking with destination ROI fixing
	template< class TS>
	extern FwStatus My_FW_ParaCheck2(const TS* pSrc, FwiSize srcSize, int srcStep, 
		FwiRect srcRoi, TS* pDst, int dstStep, 
		FwiRect dstRoi, int channel);


	//Description
	//The function fwiWarpBilinear is declared in the fwi.h file. It operates with ROI (see ROI
	//Processing in Geometric Transforms).
	//This bilinear warp function transforms the source image pixel coordinates (x,y) according to the
	//following formulas:
	//	x1 = c00*xy + c01*x + c02*y + c03
	//	y1 = c10*xy + c11*x + c12*y + c13
	//where x1 and y1 denote the pixel coordinates in the transformed image, and cij are the bilinear
	//transform coefficients passed in the array coeffs.
	//The bilinear transform preserves equal distances between points on a line.
	//The transformed part of the source image is resampled using the interpolation method specified by
	//the interpolation parameter, and written to the destination image ROI.

	//internal function for WarpAffine transformation
	template< class TS, CH chSrc, DispatchType disp >
	extern FwStatus My_FW_WarpAffine(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][3], int interpolation);

	//internal function for WarpAffine Bilinear transformation
	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpBilinear(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][4], int interpolation)
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

		double c00=coeffs[0][0], c01=coeffs[0][1], c02=coeffs[0][2], c03=coeffs[0][3];
		double c10=coeffs[1][0], c11=coeffs[1][1], c12=coeffs[1][2], c13=coeffs[1][3];

		if (c00==0 && c10==0) {
			double coeffs2[2][3];
			coeffs2[0][0] = c01;
			coeffs2[0][1] = c02;
			coeffs2[0][2] = c03;
			coeffs2[1][0] = c11;
			coeffs2[1][1] = c12;
			coeffs2[1][2] = c13;

			return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi,
				pDst, dstStep, dstRoi, coeffs2, interpolation);
		}

		//reduce the transform to equation a1*x+a2*y+a3=c10*x?-c00*y?
		double a1, a2, a3;
		a1=c01*c10-c11*c00;
		a2=c02*c10-c12*c00;
		a3=c03*c10-c13*c00;

		//a1=a2=0, then we have c10*x?-c00*y?=a3, which means that the picture is one line.
		if (a1==0 && a2==0) return fwStsCoeffErr;

		//special cases
		double b1, b2, b3;
		double xmap, ymap, detx, dety;
		int x, y, k, flag=0;

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

		if (a1==0) {
			//a2!=0
			//y=b1*x?+b2*y?+b3
			b1=c10/a2;
			b2=-c00/a2;
			b3=-a3/a2;

			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				dety =  b2*y + b3;

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					ymap = dety + b1 *x;
					detx = c00*ymap+c01;
					if (detx==0) {
						detx=c10*ymap+c11;
						if (detx==0) {//this mean both equations go to 0
							if (x==0 && y==0) {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+k);
								}
							} else if (x==0) {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = 
										*(pSrc+(y-1)*dstStep+x*channel+k);
								}
							} else {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = 
										*(pSrc+y*dstStep+(x-1)*channel+k);
								}
							}
							continue;
						}
						xmap = (y- c12*ymap-c13)/detx;
					} else {
						xmap = (x- c02*ymap-c03)/detx;
					}
					My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, interpolation, &flag, channel, channel1, round);
				}
			}
		}
		else if (a2==0) {
			//a1!=0
			//x=b1*x?+b2*y?+b3
			b1=c10/a1;
			b2=-c00/a1;
			b3=-a3/a1;

			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				dety =  b2*y + b3;

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					xmap = dety + b1 *x;
					detx = c00*xmap+c02;
					if (detx==0) {
						detx=c10*xmap+c12;
						if (detx==0) {//this mean both equations go to 0						
							if (x==0 && y==0) {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = *(pSrc+k);
								}
							} else if (x==0) {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = 
										*(pSrc+(y-1)*dstStep+x*channel+k);
								}
							} else {
								for (k=0;k<channel1;k++) {
									*(pDst+y*dstStep+x*channel+k) = 
										*(pSrc+y*dstStep+(x-1)*channel+k);
								}
							}
							continue;
						}
						ymap = (y- c11*xmap-c13)/detx;
					} else {
						ymap = (x- c01*xmap-c03)/detx;
					}

					My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, interpolation, &flag, channel, channel1, round);
				}
			}
		} else {//a1!=0 & a2!=0, no shortcuts! :-(
			//a1*x+a2*y+a3=c10*x?-c00*y? (1)
			//c00 & c10 cannot be 0 at the same time, otherwise a1=a2=0
			//need to special handle c00=0 case or c10=0 case because
			//the above equation will be deduced to the linear equation
			//which means we have substitue (1) to the bilinear equation
			for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
				if (c00!=0) {
					dety =  -(c00*y+a3)/a2 - c01/c00 + (c02*a1)/(c00*a2);
					detx = c01*y/a2 +(c01*a3-c03*a1)/(c00*a2);
				} else {
					dety =  -(c00*y+a3)/a2 - c11/c00 + (c12*a1)/(c10*a2);
					detx = (a1+c11*c00)*y/(c10*a2) +(c11*a3-c13*a1)/(c10*a2);
				}

				for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
					if (c00!=0) {
						b1 = (c10/a2) *x + dety;
						b2 = (a1-c01*c10)/(c00*a2) * x + detx;
					} else {
						b1 = (c10/a2) *x + dety;
						b2 = -(c11/a2)*x + detx;
					}

					b3 = b1*b1/4 - b2;

					if (b3 < 0) {
						for (k=0;k<channel1;k++) {
							*(pDst+y*dstStep+x*channel+k) = 0;
						}
						continue;
					}

					b3=sqrt(b3);
					ymap = b1/2+b3;
					if (ymap > srcRoi.y+srcRoi.height) {
						ymap = b1/2-b3;
					}

					xmap = (c10*x-c00*y-a3-a2*ymap)/a1;

					My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
						pDst, dstStep, interpolation, &flag, channel, channel1, round);
				}
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
	//The function fwiWarpBilinearBack is declared in the fwi.h file. It operates with ROI (see
	//ROI Processing in Geometric Transforms).
	//This function performs the inverse transform to that defined by fwiWarpBilinear function.
	//Pixel coordinates x? and y? in the transformed image are obtained from the following equations
	//	c00*x?y? + c01*x? + c02*y? + c03 = x
	//	c10*x?y? + c11*x? + c12*y? + c13 = y
	//where x and y denote the pixel coordinates in the source image, and coefficients cij are given in the
	//array coeffs. Thus, you don’t need to invert transform coefficients in your application program
	//before calling fwiWarpBilinearBack.
	//Note that inverse transform functions handle source and destination ROI in a different way than
	//other geometric transform functions. See implementation details in the description of
	//fwiWarpAffineBack function.

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpBilinearBack(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][4], int interpolation)
	{
		//WarpBilinearBack doesn't support smooth_edge functionality
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) 
			return fwStsInterpolationErr;	

		double detx, dety;
		detx= coeffs[1][2]*coeffs[0][0]-coeffs[0][2]*coeffs[1][0];
		dety= coeffs[1][1]*coeffs[0][0]-coeffs[0][1]*coeffs[1][0];
		if (detx==0 && dety==0) return fwStsCoeffErr;

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		double xmap, ymap, tx, ty;
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
			detx = coeffs[0][0] * y + coeffs[0][1];
			dety = coeffs[1][0] * y + coeffs[1][1];
			tx =   coeffs[0][2] * y + coeffs[0][3];
			ty =   coeffs[1][2] * y + coeffs[1][3];

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
				xmap = detx * x + tx;
				ymap = dety * x + ty;

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		return fwStsNoErr;
	}

	//Description
	//The function fwiWarpBilinearQuad is declared in the fwi.h file. It operates with ROI (see
	//ROI Processing in Geometric Transforms).
	//This function applies a bilinear transform to an arbitrary quadrangle srcQuad in the source image
	//pSrc. The operations take place only in the intersection of the source image ROI srcRoi and the
	//source quadrangle srcQuad. The function fwiWarpBilinearQuad uses the same formulas for
	//pixel mapping as in the case of the fwiWarpBilinear function. Transform coefficients are
	//computed internally, based on the mapping of the source quadrangle to the quadrangle dstQuad
	//specified in the destination image pDst. The dstQuad should have a non-empty intersection with
	//the destination image ROI dstRoi.
	//The first dimension [4] of the array specifying the quadrangle srcQuad[4][2] or
	//dstQuad[4][2] is equal to the number of vertices, and the second dimension [2] holds x and y
	//coordinates of the vertex.
	//Edge smoothing interpolation is applicable only if the source quadrangle lies in the source image
	//ROI.

	extern void My_FW_Triangle_Sort( double triangle[3][2]);

	extern void My_FW_Triangle_Intersection (double triangle[3][2], FwiRect dstRoi, int y,
		int *xleft, int *xright);

	//coeffs is based on quadrangle coordinate mapping from srcQuad to DstQuad.
	int My_FW_BilinearQuadCoeff(const double srcQuad[4][2], const double dstQuad[4][2], double coeff[2][4])
	{
		//shorter naming convention
		double x0=dstQuad[0][0], y0=dstQuad[0][1], sx0=srcQuad[0][0], sy0=srcQuad[0][1];
		double x1=dstQuad[1][0], y1=dstQuad[1][1], sx1=srcQuad[1][0], sy1=srcQuad[1][1];
		double x2=dstQuad[2][0], y2=dstQuad[2][1], sx2=srcQuad[2][0], sy2=srcQuad[2][1];
		double x3=dstQuad[3][0], y3=dstQuad[3][1], sx3=srcQuad[3][0], sy3=srcQuad[3][1];

		//level two coeffs
		//S[0][0]*C00 + S[0][1]*C01 + S[0][2]*C02 = S[0][3] 
		//S[0][0]*C10 + S[0][1]*C11 + S[1][2]*C12 = S[0][4]
		//s[0][3] corresonding to x coordinate, s[0][4] corresponding to y coordinate
		double s[3][5];
		s[0][0] = sx1*sy1-sx0*sy0;
		s[0][1] = sx1-sx0;
		s[0][2] = sy1-sy0;
		s[0][3] = x1-x0;
		s[0][4] = y1-y0;
		s[1][0] = sx2*sy2-sx0*sy0;
		s[1][1] = sx2-sx0;
		s[1][2] = sy2-sy0;
		s[1][3] = x2-x0;
		s[1][4] = y2-y0;
		s[2][0] = sx3*sy3-sx0*sy0;
		s[2][1] = sx3-sx0;
		s[2][2] = sy3-sy0;
		s[2][3] = x3-x0;
		s[2][4] = y3-y0;

		//dij is the 2x2 sub-determinant coresponding to s[i][j]
		double d00= s[1][1]*s[2][2]-s[2][1]*s[1][2];
		double d10= s[2][1]*s[0][2]-s[0][1]*s[2][2];
		double d20= s[0][1]*s[1][2]-s[1][1]*s[0][2];

		double d01= s[1][2]*s[2][0]-s[2][2]*s[1][0];
		double d11= s[2][2]*s[0][0]-s[0][2]*s[2][0];
		double d21= s[0][2]*s[1][0]-s[1][2]*s[0][0];

		double d02= s[1][0]*s[2][1]-s[2][0]*s[1][1];
		double d12= s[2][0]*s[0][1]-s[0][0]*s[2][1];
		double d22= s[0][0]*s[1][1]-s[1][0]*s[0][1];

		double det= d00*s[0][0]+d10*s[1][0]+d20*s[2][0];
		if (det == 0) return 1;
		//solution = |bi|/det

		coeff[0][0] = (d00*s[0][3]+d10*s[1][3]+d20*s[2][3])/det;
		coeff[0][1] = (d01*s[0][3]+d11*s[1][3]+d21*s[2][3])/det;
		coeff[0][2] = (d02*s[0][3]+d12*s[1][3]+d22*s[2][3])/det;
		coeff[1][0] = (d00*s[0][4]+d10*s[1][4]+d20*s[2][4])/det;
		coeff[1][1] = (d01*s[0][4]+d11*s[1][4]+d21*s[2][4])/det;
		coeff[1][2] = (d02*s[0][4]+d12*s[1][4]+d22*s[2][4])/det;

		coeff[0][3]= x0-coeff[0][0]*sx0*sy0-coeff[0][1]*sx0-coeff[0][2]*sy0;
		coeff[1][3]= y0-coeff[1][0]*sx0*sy0-coeff[1][1]*sx0-coeff[1][2]*sy0;
		return 0; //normal case
	}

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpBilinearQuad(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
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
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		// step 1: find coeff for WarpBilinear
		// this coeff is mapping from srcQuad to dstQuad, 
		// cannot inverse mapping
		double coeff[2][4];
		if (My_FW_BilinearQuadCoeff(srcQuad, dstQuad, coeff)!=0) return fwStsQuadErr;

		//step 2: map srcRoi to detination dstRoi
		status = My_FW_WarpBilinear<TS, chSrc, disp>(pSrc, srcSize, srcStep, srcRoi, 
			pDst, dstStep, dstRoi, coeff, interpolation);
		if (status != fwStsNoErr) return status;

		//Step 3: split the quadrangle into two triangles
		// Step 3.1 decide whether quadrangle is convex type
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

		// Step 3.2 split into two triangles
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

		//Step4: sort triangle
		My_FW_Triangle_Sort(triangle1);
		My_FW_Triangle_Sort(triangle2);

		//step5: Zero out points outside quadrangle.
		//Actually, dstQuad is not a polynomial. The following might be zeroed out
		// more then it should. To be corrected in the future
		int x, y, xleft1, xright1, xleft2, xright2;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(TS));

		for (y=dstRoi.y;y<dstRoi.y+dstRoi.height;y++) {
			//points in first triangle
			My_FW_Triangle_Intersection (triangle1, dstRoi, y, &xleft1, &xright1);

			//points in second triangle
			My_FW_Triangle_Intersection (triangle2, dstRoi, y, &xleft2, &xright2);

			if (xright1==-1) {
				if (xright2==-1) {
					xright1=dstRoi.x-1;//xleft1=0
				} else {
					xleft1=xleft2;
					xright1=xright2;
				}
			} else {
				if (xright2!=-1) {
					xleft1 = ((xleft1>xleft2) ? xleft2 : xleft1);
					xright1= ((xright1>xright2) ? xright1 : xright2);
				} //else keep xleft1 and xright1 unchange
			}

			//zero out less than xleft1 part and great than xright1
			for (x=dstRoi.x*channel; x< xleft1*channel; x++) {
				*(pDst+y*dstStep+x)=0;
			}
			if (xright1 < dstRoi.x) xright1=dstRoi.x-1;
			for (x=(xright1+1)*channel;x<(dstRoi.x+dstRoi.width)*channel;x++) {
				*(pDst+y*dstStep+x)=0;
			}
		}

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
	//The function fwGetBilinearQuad is declared in the fwi.h file. It operates with ROI (see
	//ROI Processing in Geometric Transforms).
	//This function is used as a support function for fwiWarpBilinear. It computes vertex
	//coordinates of the quadrangle, to which the source rectangular ROI would be mapped by the
	//bilinear transform function fwiWarpBilinear using coefficients coeffs.
	//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
	//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
	//following meaning:
	//	quad[0] corresponds to the transformed top-left corner of the source ROI,
	//	quad[1] corresponds to the transformed top-right corner of the source ROI,
	//	quad[2] corresponds to the transformed bottom-right corner of the source ROI,
	//	quad[3] corresponds to the transformed bottom-left corner of the source ROI.

	extern FwStatus My_FW_LineCheck(double quad[4][2]);


	//Description
	//The function fwiGetBilinearBound is declared in the fwi.h file. It operates with ROI (see
	//ROI Processing in Geometric Transforms).
	//This function is used as a support function for fwiWarpBilinear. It computes vertex
	//coordinates of the smallest bounding rectangle for the quadrangle quad, to which the source ROI
	//would be mapped by the bilinear transform function fwiWarpBilinear using coefficients
	//coeffs.
	//bound[0] specifies x, y coordinates of the top-left corner, bound[1] specifies x, y coordinates
	//of the bottom-right corner.

	extern void My_FW_QuadBound (const double quad[4][2], double bound[2][2]);

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinear <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinear <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinear <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinear <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinear <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	return My_FW_WarpBilinearBack <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinearBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinearBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinearBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double coeffs[2][4], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinearBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpBilinearQuad <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinearQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinearQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpBilinearQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpBilinearQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetBilinearQuad)(FwiRect srcRoi, double quad[4][2], const double coeffs[2][4])
{
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	quad[0][0] = coeffs[0][0]*srcRoi.x*srcRoi.y + coeffs[0][1]*srcRoi.x 
		+ coeffs[0][2]*srcRoi.y + coeffs[0][3];
	quad[0][1] = coeffs[1][0]*srcRoi.x*srcRoi.y + coeffs[1][1]*srcRoi.x 
		+ coeffs[1][2]*srcRoi.y + coeffs[1][3];

	int width = srcRoi.width-1, height=srcRoi.height-1;
	quad[1][0] = quad[0][0]+ coeffs[0][0]*width*srcRoi.y + coeffs[0][1]*width;
	quad[1][1] = quad[0][1]+ coeffs[1][0]*width*srcRoi.y + coeffs[1][1]*width;

	quad[3][0] = quad[0][0]+ coeffs[0][0]*srcRoi.x*height + coeffs[0][2]*height;
	quad[3][1] = quad[0][1]+ coeffs[1][0]*srcRoi.x*height + coeffs[1][2]*height;

	quad[2][0] = quad[1][0]+ coeffs[0][0]*srcRoi.x*height + coeffs[0][2]*height 
		+ coeffs[0][0]*width*height;
	quad[2][1] = quad[1][1]+ coeffs[1][0]*srcRoi.x*height + coeffs[1][2]*height
		+ coeffs[1][0]*width*height;

	return My_FW_LineCheck(quad);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetBilinearBound)(FwiRect srcRoi, double bound[2][2], const double coeffs[2][4])
{
	if (srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	double quad[4][2];
	FwStatus status;

	status=fwiGetBilinearQuad (srcRoi, quad, coeffs);
	if (status != fwStsNoErr) return status;

	// this might be wrong, since straight line will not map as straight line
	// To be corrected in the future
	My_FW_QuadBound(quad, bound);

	return fwStsNoErr;
}


//Description
//The function fwiGetBilinearTransform is declared in the fwi.h file. It operates with ROI
//(see ROI Processing in Geometric Transforms).
//This function is used as a support function for fwiWarpBilinear. It computes the coefficients
//coeffs of the bilinear transform that maps the source rectangular ROI to the quadrangle with the
//specified vertex coordinates quad.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//	quad[0] corresponds to the transformed top-left corner of the source ROI,
//	quad[1] corresponds to the transformed top-right corner of the source ROI,
//	quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//	quad[3] corresponds to the transformed bottom-left corner of the source ROI.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetBilinearTransform)(FwiRect srcRoi, const double quad[4][2],
														   double coeffs[2][4])
{
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width <= 1 || srcRoi.height <= 1 )
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

	status=My_FW_BilinearQuadCoeff(start, quad, coeffs);
	if (status!=0) return fwStsCoeffErr;

	return fwStsNoErr;
}

#endif //FW_BUILD_NUM >= 001

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR 
