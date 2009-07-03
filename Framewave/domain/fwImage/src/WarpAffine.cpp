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
	//handle each point individually
	template< class TS, DispatchType disp >
	void My_FW_PointHandle(double xmap, double ymap, int x, int y,
		const TS* pSrc, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, int interpolation, int *flag, 
		int channel, int channel1, Fw32f round);

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

	//Description
	//The function fwiWarpAffine is declared in the fwImage.h file. This affine warp function
	//transforms the source image pixel coordinates (x,y) according to the following formulas:
	//		x? = c00*x + c01*y + c02
	//		y? = c10*x + c11*y + c12
	//where x? and y? denote the pixel coordinates in the transformed image, and cij are the affine
	//transform coefficients passed in the array coeffs.
	//The affine warping is a general linear transform which incorporates such elementary
	//transformations as scaling, rotation, translation, stretching, and shearing. It always transforms
	//parallel lines into parallel lines and preserves equal distances between points on a line.
	//The transformed part of the image is resampled using the interpolation method specified by the
	//interpolation parameter, and written to the destination image ROI.

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

	//To decide whether we need edge smooth
	SYS_INLINE FwStatus My_FW_GetInterpolation(int &interpolation, int &interpolationE)
	{
		interpolationE = interpolation ^ FWI_SMOOTH_EDGE;
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) {
				if ( interpolationE != FWI_INTER_NN && interpolationE != FWI_INTER_LINEAR 
					&& interpolationE != FWI_INTER_CUBIC)
					return fwStsInterpolationErr;	
				interpolation = interpolationE;
				interpolationE = FWI_SMOOTH_EDGE;
		}
		return fwStsNoErr;
	}

	//special version of copying 8u data
	template < CH channel>
	SYS_INLINE static void My_FW_WarpCopy(const Fw8u *src, Fw8u *dst)
	{
		switch(channel) {
		case (C4) :
			*(int*)dst = *(int*)src ;   
			break;
		case (AC4):
		case (C3) :
			//the first element will be copied under C1 case
			*((Fw16s*)(dst+1)) = *((Fw16s*)(src+1)) ;
			//no break, intentional!
		case (C1) :
			*dst = *src ;   
			//no break is needed since no default is allowed.
		}
	}
	
	//special version of copying 16u data
	template < CH channel>
	SYS_INLINE static void My_FW_WarpCopy(const Fw16u *src, Fw16u *dst)
	{
		switch(channel) {
		case (C4) :
			*(int*)dst  = *(int*)src ;   
			*((int*)dst+1) = *((int*)(src)+1) ;   
			break;
		case (AC4):
		case (C3) :
			//the first element will be copied under C1 case
			*((int*)(dst+1)) = *((int*)(src+1)) ; 
			//no break, intentional!
		case (C1) :
			*dst = *src ;  
			//no break is needed since no default is allowed.
		}
	}
	
	//special version of copying float data
	template < CH channel>
	SYS_INLINE static void My_FW_WarpCopy(const Fw32f *src, Fw32f *dst)
	{
		__m128 t;
		switch(channel) {
		case (C4) :
			//use SSE2 instruction for 128 bit (4*32) data
			t = _mm_loadu_ps(src);
			_mm_storeu_ps(dst, t); 
			break;
		case (AC4):
		case (C3) :
			//the first element will be copied under C1 case
			*(dst+1) = *(src+1) ;   
			*(dst+2) = *(src+2) ;  
			//no break, intentional!
		case (C1) :
			*dst = *src ;  
			//no break is needed since no default is allowed.
		}
	}

	//Optimized internal WarpAffine function
	template <class TS, CH channel>
	FwStatus warpAffine_sse2_NN(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeff[2][3], int interpolation)
	{
		int interpolationE;
		int ch = ChannelCount(channel);
		My_FW_GetInterpolation(interpolation, interpolationE);

		//use pixel size instead of byte size
		int n = sizeof(*pSrc);
		srcStep /= n ;
		dstStep /= n ;

		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, ch);
		if (status !=fwStsNoErr) return status;

		double det = (coeff[0][0]*coeff[1][1] - coeff[1][0]*coeff[0][1]);
		if(det == 0.0) return fwStsCoeffErr;
		double inv_Det = 1/det ;

		const float r00 = (float)( coeff[1][1] * inv_Det);	//  matrix inverse
		const float r01 = (float)(-coeff[0][1] * inv_Det);
		const float r10 = (float)(-coeff[1][0] * inv_Det);
		const float r11 = (float)( coeff[0][0] * inv_Det);
		const float const1 =  - (float)(r00 * coeff[0][2] + r01*coeff[1][2] - srcRoi.x);
		const float const2 =  - (float)(r10 * coeff[0][2] + r11*coeff[1][2] - srcRoi.y);

		__m128 tx, ty, xmap, ymap, mask ;
		int x, y, xi, yi;
		XMM128 xint = {0}, yint = {0};   

		float *r00_x, *r10_x, xm, ym;
		//use fwMalloc instead of malloc for aligned address
		r00_x = (float*) fwMalloc(dstRoi.width * sizeof(float));
		r10_x = (float*) fwMalloc(dstRoi.width * sizeof(float));

		for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++  ){
			r00_x[x-dstRoi.x] = r00 * x;
			r10_x[x-dstRoi.x] = r10 * x;
		}

		const __m128 roiX = _mm_set1_ps((float)srcRoi.x);
		const __m128 roiY = _mm_set1_ps((float)srcRoi.y);
		const __m128 x_Width = _mm_set1_ps((float)srcRoi.x+srcRoi.width );
		const __m128 y_Height = _mm_set1_ps((float)srcRoi.y+srcRoi.height);

		for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
			tx = _mm_set1_ps(r01 * y + const1);
			ty = _mm_set1_ps(r11 * y + const2);

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width)-4; x += 4 ) 
			{   
				xmap = _mm_load_ps(&r00_x[x-dstRoi.x]); 
				xmap = _mm_add_ps(xmap, tx);  // xmap <- r00*x + tx
				ymap = _mm_load_ps(&r10_x[x-dstRoi.x]); 
				ymap = _mm_add_ps(ymap, ty);   // xmap <- r10*x + ty

				mask = _mm_cmpge_ps(xmap, roiX);
				mask = _mm_and_ps(mask, _mm_cmplt_ps(xmap, x_Width));
				mask = _mm_and_ps(mask, _mm_cmpge_ps(ymap, roiY));
				mask = _mm_and_ps(mask, _mm_cmplt_ps(ymap, y_Height));

				int m = _mm_movemask_ps(mask);
				if(m){
					xint.i = _mm_cvttps_epi32(xmap);
					yint.i = _mm_cvttps_epi32(ymap);    

					if(m & 1){
						yi = yint.s32[0] ; 
						xi = xint.s32[0] ;
						My_FW_WarpCopy<channel>(pSrc+ yi*srcStep+xi*ch, pDst+y*dstStep+x*ch);
					}
					if(m & 2){
						yi = yint.s32[1] ; 
						xi = xint.s32[1] ;
						My_FW_WarpCopy<channel>(pSrc+ yi*srcStep+xi*ch, pDst+y*dstStep+(x+1)*ch);
					}
					if(m & 4){
						yi = yint.s32[2] ; 
						xi = xint.s32[2] ;
						My_FW_WarpCopy<channel>(pSrc+ yi*srcStep+xi*ch, pDst+y*dstStep+(x+2)*ch);
					}
					if(m & 8){
						yi = yint.s32[3] ; 
						xi = xint.s32[3] ;
						My_FW_WarpCopy<channel>(pSrc+ yi*srcStep+xi*ch, pDst+y*dstStep+(x+3)*ch);
					}
				}
			}

			// fall back to reference code for remainder cases
			for( ; x<(dstRoi.x+dstRoi.width); x++ )
			{
				xm = r00_x[x-dstRoi.x]+ r01 * y + const1 ;  // xmap <- r00*x + tx
				ym = r10_x[x-dstRoi.x]+ r11 * y + const2;   // xmap <- r10*x + ty

				if(xm >= srcRoi.x && xm < srcRoi.x + srcRoi.width && 
					ym >= srcRoi.y && ym < srcRoi.y + srcRoi.height )
				{
					xi = (int)xm;
					yi = (int)ym;                      
					My_FW_WarpCopy<channel>(pSrc+ yi*srcStep+xi*ch, pDst+y*dstStep+x*ch);
				}            
			}
		}

		//free allocated memory by fwMalloc
		fwFree(r00_x);
		fwFree(r10_x);
		
		return fwStsNoErr;
	}



    template <class TS, CH channel>
	FwStatus warpAffine_sse2_Linear(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeff[2][3], int interpolation)
	{
		int interpolationE;
		int chCount = ChannelCount(channel);
		My_FW_GetInterpolation(interpolation, interpolationE);

		//use pixel size instead of byte size
		int n = sizeof(*pSrc);
		srcStep /= n ;
		dstStep /= n ;

		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, chCount);
		if (status !=fwStsNoErr) return status;

		double det = (coeff[0][0]*coeff[1][1] - coeff[1][0]*coeff[0][1]);
		if(det == 0.0) return fwStsCoeffErr;
		double inv_Det = 1/det ;

		const float r00 = (float)( coeff[1][1] * inv_Det);	//  matrix inverse
		const float r01 = (float)(-coeff[0][1] * inv_Det);
		const float r10 = (float)(-coeff[1][0] * inv_Det);
		const float r11 = (float)( coeff[0][0] * inv_Det);
		const float const1 =  - (float)(r00 * coeff[0][2] + r01*coeff[1][2] - srcRoi.x);
		const float const2 =  - (float)(r10 * coeff[0][2] + r11*coeff[1][2] - srcRoi.y);

		__m128 tx, ty, xmap, ymap ;
		int x, y;// xi, yi;
		XMM128 xint = {0}, yint = {0}, xint1 = {0}, yint1 = {0}, mask;   

		float *r00_x, *r10_x, xm, ym;
		//use fwMalloc instead of malloc for aligned address
		r00_x = (float*) fwMalloc(dstRoi.width * sizeof(float));
		r10_x = (float*) fwMalloc(dstRoi.width * sizeof(float));

		for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++  ){
			r00_x[x-dstRoi.x] = r00 * x;
			r10_x[x-dstRoi.x] = r10 * x;
		}

		const __m128 roiX = _mm_set1_ps((float)srcRoi.x);
		const __m128 roiY = _mm_set1_ps((float)srcRoi.y);
		const __m128 x_Width = _mm_set1_ps((float)srcRoi.x+srcRoi.width );
		const __m128 y_Height = _mm_set1_ps((float)srcRoi.y+srcRoi.height);
        const __m128i x_Widthi = _mm_set1_epi32(srcRoi.x+srcRoi.width );
		const __m128i y_Heighti = _mm_set1_epi32(srcRoi.y+srcRoi.height);

        int channel1 = (channel == 5)? 3 : channel;

        //      else if (interpolation == FWI_INTER_LINEAR) {
		//	int xint = (int) xmap;
		//	int yint = (int) ymap;

		//	int	xint1 = xint+1;
		//	if (xint1 >= srcRoi.x + srcRoi.width)  xint1=xint;
		//	int	yint1 = yint+1;
		//	if (yint1 >= srcRoi.y + srcRoi.height) yint1=yint;

		//	double  xfraction = xmap-xint;
		//	double	yfraction = ymap-yint;					

		//	for (k=0;k<channel1;k++) {
		//		result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
		//			+ (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
		//			+ xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
		//			+ xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));

		//		*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
		//	}
		//}

		for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++) {
			tx = _mm_set1_ps(r01 * y + const1);
			ty = _mm_set1_ps(r11 * y + const2);

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width)-4; x += 4 ) 
			{   
				xmap = _mm_load_ps(&r00_x[x-dstRoi.x]); 
				xmap = _mm_add_ps(xmap, tx);  // xmap <- r00*x + tx
				ymap = _mm_load_ps(&r10_x[x-dstRoi.x]); 
				ymap = _mm_add_ps(ymap, ty);   // xmap <- r10*x + ty

                mask.f = _mm_cmpge_ps(xmap, roiX);
				mask.f = _mm_and_ps(mask.f, _mm_cmplt_ps(xmap, x_Width));
				mask.f = _mm_and_ps(mask.f, _mm_cmpge_ps(ymap, roiY));
				mask.f = _mm_and_ps(mask.f, _mm_cmplt_ps(ymap, y_Height));

                int m = _mm_movemask_ps(mask.f);

                if(m){
					xint.i = _mm_cvttps_epi32(xmap);
					yint.i = _mm_cvttps_epi32(ymap);    

                    const __m128i one = _mm_set1_epi32(1);

                    //	int	xint1 = xint+1;
	                //	if (xint1 >= srcRoi.x + srcRoi.width)  xint1=xint;
                    xint1.i = _mm_add_epi32(xint.i, one);
                    mask.i = _mm_cmplt_epi32(xint1.i, x_Widthi);
				    xint1.i = _mm_and_si128( xint1.i, mask.i );
                    mask.i = _mm_andnot_si128( mask.i, xint.i );
                    xint1.i = _mm_or_si128(xint1.i, mask.i);

                    //	int	yint1 = yint+1;
		            //	if (yint1 >= srcRoi.y + srcRoi.height) yint1=yint;
                    yint1.i = _mm_add_epi32(yint.i, one);
				    mask.i = _mm_cmplt_epi32(yint1.i, y_Heighti);
                    yint1.i = _mm_and_si128( yint1.i, mask.i );
                    mask.i = _mm_andnot_si128( mask.i, yint.i );
                    yint1.i = _mm_or_si128(yint1.i, mask.i);

                    __m128 xFraction, yFraction, one_minus_xFraction, one_minus_yFraction;

                    //xFraction = xmap-xint;
                    //yFraction = ymap-yint;
                    xFraction = _mm_cvtepi32_ps(xint.i);
                    yFraction = _mm_cvtepi32_ps(yint.i);

                    xFraction = _mm_sub_ps( xmap, xFraction);
                    yFraction = _mm_sub_ps( ymap, yFraction);

                    const __m128 oneFloat = _mm_set1_ps(1.0);
                    //(1-xFraction)
                    //(1-yFraction)
                    one_minus_xFraction = _mm_sub_ps( oneFloat, xFraction);
                    one_minus_yFraction = _mm_sub_ps( oneFloat, yFraction);

                    //	for (k=0;k<channel1;k++) {
		            //		result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
		            //			+ (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
		            //			+ xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
		            //			+ xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));

		            //		*(pDst+y*dstStep+x*channel+k) = FW_REF::Limits<TS>::Sat(result+round);
                    XMM128 srcYintXint = {0}, srcYint1Xint = {0}, srcYintXint1 = {0}, srcYint1Xint1 = {0};

                    if(channel == 1)
                    {

                        for(int i =0; i < 4; i++)
                        {   
                            if(m & (1 << i))
                            {
                                srcYintXint.u32[i]   = *(pSrc + yint.s32[i] * srcStep + xint.s32[i]);
                                srcYint1Xint.u32[i]  = *(pSrc + yint1.s32[i] * srcStep + xint.s32[i]);
                                srcYintXint1.u32[i]  = *(pSrc + yint.s32[i] * srcStep + xint1.s32[i]);
                                srcYint1Xint1.u32[i] = *(pSrc + yint1.s32[i] * srcStep + xint1.s32[i]);
                            }
                        }


                	    /*result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
				        + (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
				        + xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
				        + xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));*/

                        srcYintXint.f = _mm_cvtepi32_ps( srcYintXint.i);
                        srcYint1Xint.f = _mm_cvtepi32_ps( srcYint1Xint.i);
                        srcYintXint1.f = _mm_cvtepi32_ps( srcYintXint1.i);
                        srcYint1Xint1.f = _mm_cvtepi32_ps( srcYint1Xint1.i);

                        srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_xFraction);
                        srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_yFraction);

                        srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, one_minus_xFraction);
                        srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, yFraction);

                        srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, xFraction);
                        srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, one_minus_yFraction);

                        srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, xFraction);
                        srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, yFraction);

                        srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint.f);
                        srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYintXint1.f);
                        srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint1.f);

				       // *(pDst+y*dstStep+x) = FW_REF::Limits<TS>::Sat(result+round);

                        //__m128 roundFloat = _mm_set1_ps(0.5);

                        //srcYintXint.f = _mm_add_ps(srcYintXint.f, roundFloat);  
                        srcYintXint.i = _mm_cvtps_epi32(srcYintXint.f);
                        srcYintXint.i = _mm_packs_epi32(srcYintXint.i, srcYintXint.i);
                        srcYintXint.i = _mm_packus_epi16(srcYintXint.i, srcYintXint.i);
                        
                        if((m & 15) == 15)
                        {
                             *((Fw32u*)(pDst+y*dstStep+x)) = srcYintXint.u32[0];
                        }
                        else
                        {
                            if(m & 1){
						        *(pDst+y*dstStep+x) = srcYintXint.u8[0];
					        }
					        if(m & 2){
						        *(pDst+y*dstStep+x+1) = srcYintXint.u8[1];
					        }
					        if(m & 4){
						        *(pDst+y*dstStep+x+2) = srcYintXint.u8[2];
					        }
					        if(m & 8){
						        *(pDst+y*dstStep+x+3) = srcYintXint.u8[3];
					        }
                        }


                    }//end of C1
                    else if(channel == 4)
                    {
                        for(int i =0; i < 4; i++)
                        {   
                            if(m & (1 << i))
                            for(int k =0; k < 4; k++)
                            { 
                                srcYintXint.u32[k]   = *(pSrc + yint.s32[i] * srcStep + xint.s32[i]* channel + k) ;
                                srcYint1Xint.u32[k]  = *(pSrc + yint1.s32[i] * srcStep + xint.s32[i]* channel + k);
                                srcYintXint1.u32[k]  = *(pSrc + yint.s32[i] * srcStep + xint1.s32[i]* channel + k);
                                srcYint1Xint1.u32[k] = *(pSrc + yint1.s32[i] * srcStep + xint1.s32[i]* channel + k);
                            }
                        

                	        /*result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
				            + (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
				            + xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
				            + xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));*/

                            srcYintXint.f = _mm_cvtepi32_ps( srcYintXint.i);
                            srcYint1Xint.f = _mm_cvtepi32_ps( srcYint1Xint.i);
                            srcYintXint1.f = _mm_cvtepi32_ps( srcYintXint1.i);
                            srcYint1Xint1.f = _mm_cvtepi32_ps( srcYint1Xint1.i);

                            srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_xFraction);
                            srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_yFraction);

                            srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, one_minus_xFraction);
                            srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, yFraction);

                            srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, xFraction);
                            srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, one_minus_yFraction);

                            srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, xFraction);
                            srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, yFraction);

                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint.f);
                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYintXint1.f);
                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint1.f);

				           // *(pDst+y*dstStep+x) = FW_REF::Limits<TS>::Sat(result+round);

                            //__m128 roundFloat = _mm_set1_ps(0.5);

                            //srcYintXint.f = _mm_add_ps(srcYintXint.f, roundFloat);  
                            srcYintXint.i = _mm_cvtps_epi32(srcYintXint.f);
                            srcYintXint.i = _mm_packs_epi32(srcYintXint.i, srcYintXint.i);
                            srcYintXint.i = _mm_packus_epi16(srcYintXint.i, srcYintXint.i);
                            //*((Fw32u)(pDst+y*dstStep+x)) = srcYintXint.u32[0];

                            if(m & (1 << i)){
						        *((Fw32u*)(pDst+y*dstStep+(x+i)*channel)) = srcYintXint.u32[0];
					        }
					       
                        }
                    }//end of C4
             
                    else
                    {   //C3 or AC4
                        for(int i =0; i < 4; i++)
                        {   
                            if(m & (1 << i))
                            for(int k =0; k < 3; k++)
                            { 
                                srcYintXint.u32[k] = *(pSrc + yint.s32[i] * srcStep + xint.s32[i]* chCount + k) ;
                                srcYint1Xint.u32[k] = *(pSrc + yint1.s32[i] * srcStep + xint.s32[i]* chCount + k);
                                srcYintXint1.u32[k] = *(pSrc + yint.s32[i] * srcStep + xint1.s32[i]* chCount + k);
                                srcYint1Xint1.u32[k] = *(pSrc + yint1.s32[i] * srcStep + xint1.s32[i]* chCount + k);
                            }
                        

                	        /*result = (1-xfraction) * (1-yfraction) * (*(pSrc+yint*srcStep+xint*channel+k))
				            + (1-xfraction) * yfraction * (*(pSrc+yint1*srcStep+xint*channel+k))
				            + xfraction * (1-yfraction) * (*(pSrc+yint*srcStep+xint1*channel+k))
				            + xfraction * yfraction * (*(pSrc+yint1*srcStep+xint1*channel+k));*/

                            srcYintXint.f = _mm_cvtepi32_ps( srcYintXint.i);
                            srcYint1Xint.f = _mm_cvtepi32_ps( srcYint1Xint.i);
                            srcYintXint1.f = _mm_cvtepi32_ps( srcYintXint1.i);
                            srcYint1Xint1.f = _mm_cvtepi32_ps( srcYint1Xint1.i);

                            srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_xFraction);
                            srcYintXint.f = _mm_mul_ps(srcYintXint.f, one_minus_yFraction);

                            srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, one_minus_xFraction);
                            srcYint1Xint.f = _mm_mul_ps(srcYint1Xint.f, yFraction);

                            srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, xFraction);
                            srcYintXint1.f = _mm_mul_ps(srcYintXint1.f, one_minus_yFraction);

                            srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, xFraction);
                            srcYint1Xint1.f = _mm_mul_ps(srcYint1Xint1.f, yFraction);

                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint.f);
                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYintXint1.f);
                            srcYintXint.f = _mm_add_ps(srcYintXint.f, srcYint1Xint1.f);

				           // *(pDst+y*dstStep+x) = FW_REF::Limits<TS>::Sat(result+round);

                           // __m128 roundFloat = _mm_set1_ps(0.5);

                            //srcYintXint.f = _mm_add_ps(srcYintXint.f, roundFloat);  
                            srcYintXint.i = _mm_cvtps_epi32(srcYintXint.f);
                            srcYintXint.i = _mm_packs_epi32(srcYintXint.i, srcYintXint.i);
                            srcYintXint.i = _mm_packus_epi16(srcYintXint.i, srcYintXint.i);
                            //*((Fw32u)(pDst+y*dstStep+x)) = srcYintXint.u32[0];

                            if(m & (1 << i)){
						        *((Fw16u*)(pDst+y*dstStep+(x+i)*chCount)) = srcYintXint.u16[0];
                                *(pDst+y*dstStep+(x+i)*chCount+2) = srcYintXint.u8[2];
                               /* *(pDst+y*dstStep+(x+i)*chCount) = srcYintXint.u8[0];
                                *(pDst+y*dstStep+(x+i)*chCount+1) = srcYintXint.u8[1];
                                *(pDst+y*dstStep+(x+i)*chCount+2) = srcYintXint.u8[2];*/

					        }
					        
                        }
                    }//end of C3 or AC4

				}
            }

			// fall back to reference code for remainder cases
			for( ; x<(dstRoi.x+dstRoi.width); x++ )
			{
				xm = r00_x[x-dstRoi.x]+ r01 * y + const1 ;  // xmap <- r00*x + tx
				ym = r10_x[x-dstRoi.x]+ r11 * y + const2;   // xmap <- r10*x + ty

				if(xm >= srcRoi.x && xm < srcRoi.x + srcRoi.width && 
					ym >= srcRoi.y && ym < srcRoi.y + srcRoi.height )
				{
					int xi = (int) xm;
			        int yi = (int) ym;

			        int	xi1 = xi+1;
			        if (xi1 >= srcRoi.x + srcRoi.width)  xi1=xi;
			        int	yi1 = yi+1;
			        if (yi1 >= srcRoi.y + srcRoi.height) yi1=yi;

			        double  xFrac = xm-xi;
			        double	yFrac = ym-yi;					
                    double round = 0.5;
			        for (int k=0;k<channel1;k++) {
				        double result = (1-xFrac) * (1-yFrac) * (*(pSrc+yi*srcStep+xi*chCount+k))
					        + (1-xFrac) * yFrac * (*(pSrc+yi1*srcStep+xi*chCount+k))
					        + xFrac * (1-yFrac) * (*(pSrc+yi*srcStep+xi1*chCount+k))
					        + xFrac * yFrac * (*(pSrc+yi1*srcStep+xi1*chCount+k));

				        *(pDst+y*dstStep+x*chCount+k) = FW_REF::Limits<TS>::Sat(result+round);
                    }
				}          

	          }
		}
        
		//free allocated memory by fwMalloc
		fwFree(r00_x);
		fwFree(r10_x);
		
		return fwStsNoErr;
	}

	//Description
	//The function fwiWarpAffineBack is declared in the fwImage.h file. This function performs the
	//inverse transform to that defined by fwiWarpAffine function. Pixel coordinates x? and y? in the
	//transformed image are obtained from the following equations:
	//		c00*x? + c01*y? + c02 = x
	//		c10*x? + c11*y? + c12 = y
	//where x and y denote the pixel coordinates in the source image, and coefficients cij are given in the
	//array coeffs. Thus, you do not need to invert transform coefficients in your application program
	//before calling fwiWarpAffineBack.
	//Note that inverse transform functions handle source and destination ROI in a different way than
	//other geometric transform functions. Their implementation include the following logic:
	//	• backward transform is applied to coordinates of each pixel in the destination ROI, which
	//gives as a result coordinates of some pixel in the source image
	//	• if the obtained source pixel is inside source ROI, the corresponding pixel in destination ROI is
	//modified accordingly; otherwise no change is made.
	//The above algorithm can be represented in pseudocode as follows:
	//		for (j = dstRoi.y; j < dstRoi.y + dstRoi.height; j ++) {
	//			for (i = dstRoi.x; i < dstRoi.x + dstRoi.width; i ++) {
	//				sx = TransformX(i, j);
	//				sy = TransformY(i, j);
	//				if ( sx >= srcRoi.x && sx < srcRoi.x + srcRoi.width && sy >=
	//					srcRoi.y && sy < srcRoi.y + srcRoi.height) {
	//					dst[i, j] = Interpolate(sx, sy);}
	//			}
	//		}

	//internal function for WarpAffine transformation
	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpAffineBack(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][3], int interpolation)
	{
		//WarpAffineBack doesn't support smooth_edge functionality
		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
			&& interpolation != FWI_INTER_CUBIC) 
			return fwStsInterpolationErr;	

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		//use shorter naming convention
		double c00, c01, c02, c10, c11, c12;

		c00 = coeffs[0][0];
		c01 = coeffs[0][1];
		c02 = coeffs[0][2];
		c10 = coeffs[1][0];
		c11 = coeffs[1][1];
		c12 = coeffs[1][2];

		double det = c00*c11 - c10*c01;

		if(det == 0.0) return fwStsCoeffErr;

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
			tx = c01 * y + c02;
			ty = c11 * y + c12;

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
				xmap = c00 * x + tx;
				ymap = c10 * x + ty;
				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		return fwStsNoErr;
	}

	//Description
	//The function fwiWarpAffineQuad is declared in the fwImage.h file. This function applies the
	//affine transform to an arbitrary quadrangle srcQuad in the source image pSrc. The operations
	//take place only inside the intersection of the source image ROI srcRoi and the source quadrangle.
	//The function fwiWarpAffineQuad uses the same formulas for pixel mapping as in the case of
	//the fwiWarpAffine function. Transform coefficients are computed internally, based on the
	//mapping of the source quadrangle to the quadrangle dstQuad specified in the destination image
	//pDst. The dstQuad should have a non-empty intersection with the destination image ROI
	//dstRoi. The function computes the coordinates of the 4th vertex of the destination quadrangle
	//that uniquely depends on the three other vertices. If the computed coordinates are not equal to the
	//ones specified in dstQuad, the function returns the warning message and continues operation with
	//the computed values.
	//The first dimension [4] of the array specifying the quadrangle srcQuad[4][2] or
	//dstQuad[4][2] is equal to the number of vertices, and the second dimension [2] holds
	//x and y coordinates of the vertex.
	//Edge smoothing interpolation is applicable only if the destination quadrangle lies in the destination image
	//ROI.

	// sort triangle coordinate using y coordinate
	void My_FW_Triangle_Sort( double triangle[3][2]) 
	{
		int y[3];

		//sort y order
		if (triangle[0][1] > triangle[1][1]) {
			if (triangle[2][1] > triangle[0][1]) {
				y[0]=1;
				y[1]=0;
				y[2]=2;
			} else {
				if (triangle[2][1] > triangle[1][1]) {
					y[0]=1;
					y[1]=2;
					y[2]=0;
				} else {
					y[0]=2;
					y[1]=1;
					y[2]=0;
				}
			}
		} else {//triangle[0][1] <= triangle[1][1]
			if (triangle[2][1] > triangle[1][1]) {
				y[0]=0;
				y[1]=1;
				y[2]=2;
			} else {
				if (triangle[2][1] > triangle[0][1]) {
					y[0]=0;
					y[1]=2;
					y[2]=1;
				} else {
					y[0]=2;
					y[1]=0;
					y[2]=1;
				}
			}
		}

		double x[3][2];
		int i, j;
		//exchange order based on sorted y order
		for (i=0;i<3;i++) {
			for(j=0;j<2;j++) x[i][j]=triangle[i][j];
		}

		for (i=0;i<3;i++) {
			for(j=0;j<2;j++) triangle[i][j]=x[y[i]][j];
		}

		return;
	}

	//This triangle is sorted by above function. 
	//triangle[0][1] <= triangle[1][1] <=triangle[2][1]
	//in most of case, *xleft <= *xright
	//in the case of *xleft > *xright, this means no integer points
	//inside the triangle
	void My_FW_Triangle_Intersection (double triangle[3][2], FwiRect dstRoi, int y,
		int *xleft, int *xright)
	{
		if (y<triangle[0][1] || y>triangle[2][1] ||
			triangle[0][1] == triangle[2][1]) {
				*xleft=0;
				*xright=-1;
				return;
		} 

		//intersection for line [0] and [2]
		double c0 = triangle[2][0] - triangle[0][0];
		double pC1 = triangle[2][1] - triangle[0][1];

		double y0 = y - triangle[0][1];

		double inter1 = triangle[0][0] + (y0*c0)/pC1;

		double inter2;

		if (y < triangle[1][1]) {
			//intersction for line between [0] and [1]
			//in this case y[0]<= y <y[1]
			c0 = triangle[1][0] - triangle[0][0];
			pC1 = triangle[1][1] - triangle[0][1];
			y0 = y - triangle[0][1];

			inter2 = triangle[0][0] + (y0*c0)/pC1;
		} else if (y > triangle[1][1]) {
			//intersction for line between [1] and [2]
			//in this case y[1]< y <=y[2]
			c0 = triangle[2][0] - triangle[1][0];
			pC1 = triangle[2][1] - triangle[1][1];
			y0 = y - triangle[1][1];

			inter2 = triangle[1][0] + (y0*c0)/pC1;
		} else { //y=triangle[1][1] case
			inter2 = triangle[1][0];
		}

		//swap inter1 & inter 2 if needed
		if (inter1 > inter2) {
			c0=inter1;
			inter1=inter2;
			inter2=c0;
		}

		//integer points inside the interval
		*xleft=(int)ceil(inter1);
		*xright=(int)floor(inter2);

		if (*xleft  < dstRoi.x) *xleft = dstRoi.x;
		int xmax = dstRoi.x+dstRoi.width -1;
		if (*xright > xmax) *xright = xmax;

		return;
	}

	//internal function for WarpAffineQuad transformation
	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_WarpAffineQuad(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
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

		// step 1: find coeff for WarpAffine
		// this coeff is mapping from dstQuad to srcQuad, inverse mapping
		double coeff[2][3];
		double a00, a01, a10, a11, b0, b1, det;

		//The following calculation is based on solving the linear equations
		a00 = dstQuad[1][0] - dstQuad[0][0];
		a01 = dstQuad[1][1] - dstQuad[0][1];
		a10 = dstQuad[3][0] - dstQuad[0][0];
		a11 = dstQuad[3][1] - dstQuad[0][1];

		b0 = srcQuad[1][0] - srcQuad[0][0];
		b1 = srcQuad[3][0] - srcQuad[0][0];

		det = a00*a11-a01*a10;

		if (det == 0) return fwStsQuadErr;

		coeff[0][0] = (a11*b0-a01*b1)/det;
		coeff[0][1] = (a00*b1-a10*b0)/det;
		coeff[0][2] = srcQuad[0][0]-coeff[0][0]*dstQuad[0][0]-coeff[0][1]*dstQuad[0][1];
		//check consistent
		if ((coeff[0][0]*dstQuad[2][0]+coeff[0][1]*dstQuad[2][1]+coeff[0][2])
			!=srcQuad[2][0])
			return fwStsQuadErr;

		// same determination, different coeffient
		b0 = srcQuad[1][1] - srcQuad[0][1];
		b1 = srcQuad[3][1] - srcQuad[0][1];

		coeff[1][0] = (a11*b0-a01*b1)/det;
		coeff[1][1] = (a00*b1-a10*b0)/det;
		coeff[1][2] = srcQuad[0][1]-coeff[1][0]*dstQuad[0][0]-coeff[1][1]*dstQuad[0][1];
		//check consistent
		if ((coeff[1][0]*dstQuad[2][0]+coeff[1][1]*dstQuad[2][1]+coeff[1][2])
			!=srcQuad[2][1])
			return fwStsQuadErr;

		//Step 2: split the quadrangle into two triangles
		// Step 2.1 decide whether quadrangle is convex type
		b0 = dstQuad[2][0]-dstQuad[0][0];
		b1 = dstQuad[2][1]-dstQuad[0][1];
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

		//Step3: sort triangle, so we know which area we are handling
		My_FW_Triangle_Sort(triangle1);
		My_FW_Triangle_Sort(triangle2);

		//step4: handle points
		double xmap, ymap, tx, ty;
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
			tx = coeff[0][1] * y + coeff[0][2];
			ty = coeff[1][1] * y + coeff[1][2];

			//points in first triangle
			My_FW_Triangle_Intersection (triangle1, dstRoi, y, &xleft, &xright);
			for (x=xleft;x<=xright;x++) {
				xmap = coeff[0][0] * x + tx;
				ymap = coeff[1][0] * x + ty;
				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}

			//points in second triangle
			My_FW_Triangle_Intersection (triangle2, dstRoi, y, &xleft, &xright);
			for (x=xleft;x<=xright;x++) {
				xmap = coeff[0][0] * x + tx;
				ymap = coeff[1][0] * x + ty;
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
	//The function fwiGetAffineBound is declared in the fwImage.h file. This function is used as a
	//support function for fwiWarpAffine. It computes vertex coordinates of the smallest bounding
	//rectangle for the quadrangle quad, to which the source ROI would be mapped by the affine
	//transform function fwiWarpAffine using coefficients coeffs.
	//bound[0] specifies x, y coordinates of the top-left corner, bound[1] specifies x, y coordinates
	//of the bottom-right corner.

	//internal function for finding the boundary retangle for Quadrangle
	void My_FW_QuadBound (const double quad[4][2], double bound[2][2]) 
	{
		double xmin[2], xmax[2], ymin[2], ymax[2];
		int i;

		//Find the min, max coordinations of the quadrangle
		for (i=0;i<2;i++) {
			if (quad[i*2][0] > quad[i*2+1][0]) {
				xmax[i]=quad[i*2][0];
				xmin[i]=quad[i*2+1][0];
			} else {
				xmax[i]=quad[i*2+1][0];
				xmin[i]=quad[i*2][0];
			}

			if (quad[i*2][1] > quad[i*2+1][1]) {
				ymax[i]=quad[i*2][1];
				ymin[i]=quad[i*2+1][1];
			} else {
				ymax[i]=quad[i*2+1][1];
				ymin[i]=quad[i*2][1];
			}
		}

		bound[0][0] = (xmin[0] > xmin[1])? xmin[1] : xmin[0];
		bound[0][1] = (ymin[0] > ymin[1])? ymin[1] : ymin[0];
		bound[1][0] = (xmax[0] > xmax[1])? xmax[0] : xmax[1];
		bound[1][1] = (ymax[0] > ymax[1])? ymax[0] : ymax[1];

		//Below variation might useful in some cases
		//bound rectangle should have integer coordinates
		//add a small number to prevent double precision problem
		//bound[0][0] = floor (bound[0][0]+0.00000000001);
		//bound[0][1] = floor (bound[0][1]+0.00000000001);
		//bound[1][0] = ceil  (bound[1][0]-0.00000000001);
		//bound[1][1] = ceil  (bound[1][1]-0.00000000001);

		return;
	}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C1R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
														Fw8u *pDst, int dstStep, FwiRect dstRoi, 
														const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw8u, C1>(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
        else if(interpolation == FWI_INTER_LINEAR)
			return warpAffine_sse2_Linear<Fw8u, C1 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw8u, C1, DT_REFR> (
			pSrc, srcSize, srcStep, srcRoi, 	pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw8u, C3>(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
        else if(interpolation == FWI_INTER_LINEAR)
			return warpAffine_sse2_Linear<Fw8u, C3 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw8u, C3, DT_REFR> (
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{  
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw8u, C4>(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
        else if(interpolation == FWI_INTER_LINEAR)
			return warpAffine_sse2_Linear<Fw8u, C4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw8u, C4, DT_REFR> (
			pSrc, srcSize, srcStep, srcRoi,pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw8u, AC4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
                else if(interpolation == FWI_INTER_LINEAR)
			return warpAffine_sse2_Linear<Fw8u, AC4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw8u, AC4, DT_REFR> (
			pSrc, srcSize, srcStep, srcRoi,pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C1R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw16u, C1 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
        
	default:
		return My_FW_WarpAffine <Fw16u, C1, DT_REFR> (
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C3R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw16u, C3 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw16u, C4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_AC4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw16u, AC4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw32f, C1 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw32f, C3 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw32f, C4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case(DT_SSE3):
	case(DT_SSE2):
		//NN mode is optimized
		if(interpolation == FWI_INTER_NN)
			return warpAffine_sse2_NN<Fw32f, AC4 >(
			pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, coeffs, interpolation);
	default:
		return My_FW_WarpAffine <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoi, coeffs, interpolation);
	}
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffine <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffine <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffine <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffine <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffine <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffine <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C1R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw16u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C3R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_AC4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *pDst, int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	return My_FW_WarpAffineBack <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
		pDst, dstStep, dstRoi, coeffs, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffineBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffineBack <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffineBack <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw16u *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffineBack <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffineBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, const double coeffs[2][3], int interpolation) 
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffineBack <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi,
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}


// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *pDst, int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	return My_FW_WarpAffineQuad <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		srcQuad, pDst, dstStep, dstRoi, dstQuad, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffineQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffineQuad <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_WarpAffineQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	const double srcQuad[4][2], Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
	const double dstQuad[4][2], int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_WarpAffineQuad <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			srcQuad, pDst[i], dstStep, dstRoi, dstQuad, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr; 
}

//Description
//The function fwiGetAffineQuad is declared in the fwImage.h file. This function is used as a
//support function for fwiWarpAffine. It computes vertex coordinates of the quadrangle, to
//which the source rectangular ROI would be mapped by the affine transform function
//fwiWarpAffine using the given coefficients coeffs.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//	quad[0] corresponds to the transformed top-left corner of the source ROI,
//	quad[1] corresponds to the transformed top-right corner of the source ROI,
//	quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//	quad[3] corresponds to the transformed bottom-left corner of the source ROI.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetAffineQuad)(FwiRect srcRoi, double quad[4][2], const double coeffs[2][3])
{
	//Parameter checking
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	if ((coeffs[0][0]*coeffs[1][1] -coeffs[1][0]*coeffs[0][1])==0.0)
		return fwStsCoeffErr;

	double width = srcRoi.width-1, height=srcRoi.height-1;
	double xwidth = width * coeffs[0][0];
	double ywidth = width * coeffs[1][0];
	double xheight = height * coeffs[0][1];
	double yheight = height * coeffs[1][1];

	quad[0][0] = srcRoi.x * coeffs[0][0] + srcRoi.y * coeffs[0][1] + coeffs[0][2];
	quad[0][1] = srcRoi.x * coeffs[1][0] + srcRoi.y * coeffs[1][1] + coeffs[1][2];
	quad[1][0] = quad[0][0]+xwidth;
	quad[1][1] = quad[0][1]+ywidth;
	quad[3][0] = quad[0][0]+xheight;
	quad[3][1] = quad[0][1]+yheight;
	quad[2][0] = quad[1][0]+xheight;
	quad[2][1] = quad[1][1]+yheight;

	return fwStsNoErr;
}

//Get the smallest bounding rectangle who contains the quadrangle
FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetAffineBound)(FwiRect srcRoi, double bound[2][2], const double coeffs[2][3])
{
	//Parameter checking
	if (srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	double quad[4][2];
	FwStatus status;

	// calculate the quad coordinates
	status=fwiGetAffineQuad (srcRoi, quad, coeffs);
	if (status != fwStsNoErr) return status;

	//Find the bounding rectangle
	My_FW_QuadBound(quad, bound);

	return fwStsNoErr;
}

//Description
//The function fwiGetAffineTransform is declared in the fwImage.h file. This function is used as
//a support function for fwiWarpAffine. It computes the coefficients coeffs of the affine
//transform that should be used by the function fwiWarpAffine to map the source rectangular
//ROI to the quadrangle with the specified vertex coordinates quad.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//	quad[0] corresponds to the transformed top-left corner of the source ROI,
//	quad[1] corresponds to the transformed top-right corner of the source ROI,
//	quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//	quad[3] corresponds to the transformed bottom-left corner of the source ROI.
//The function computes the coordinates of the 4th vertex of the destination quadrangle that
//uniquely depends on the three other vertices. If the computed coordinates are not equal to the ones
//specified in quad, the function returns the warning message and continues operation with the
//computed values.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetAffineTransform)(FwiRect srcRoi, const double quad[4][2],
														 double coeffs[2][3])
{
	//Parameter checking
	if (srcRoi.x < 0 || srcRoi.y < 0 || 
		srcRoi.width <= 0 || srcRoi.height <= 0 )
		return fwStsSizeErr;

	if (srcRoi.width == 1 || srcRoi.height == 1 )
		return fwStsRectErr;

	double width = srcRoi.width-1, height=srcRoi.height-1;
	coeffs[0][0] = (quad[1][0] - quad[0][0]) / width;
	coeffs[1][0] = (quad[1][1] - quad[0][1]) / width;
	coeffs[0][1] = (quad[3][0] - quad[0][0]) / height;
	coeffs[1][1] = (quad[3][1] - quad[0][1]) / height;
	coeffs[0][2] = quad[0][0] - srcRoi.x * coeffs[0][0] - srcRoi.y * coeffs[0][1];
	coeffs[1][2] = quad[0][1] - srcRoi.x * coeffs[1][0] - srcRoi.y * coeffs[1][1];

	if ((coeffs[0][0]*coeffs[1][1] -coeffs[1][0]*coeffs[0][1])==0.0)
		return fwStsCoeffErr;

	//check consistent
	double xq3 = (srcRoi.x + width) * coeffs[0][0] + (srcRoi.y + height)* coeffs[0][1] + coeffs[0][2];
	double yq3 = (srcRoi.x + width) * coeffs[1][0] + (srcRoi.y + height)* coeffs[1][1] + coeffs[1][2];

	if (xq3 != quad[2][0] || yq3 != quad[2][1] ) 
		return fwStsQuadErr;

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
