/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

namespace OPT_LEVEL
{
//internal function for WarpAffine transformation
template< class TS, CH chSrc, DispatchType disp >
extern FwStatus My_FW_WarpAffine(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
						   TS* pDst, int dstStep, FwiRect dstRoi, 
						   const double coeffs[2][3], int interpolation);

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

//Parameters
//pSrc - Pointer to the source image origin. 
//srcSize Size - pixels of the source image.
//srcStep Step - bytes through the source image buffer.
//srcRoi - Region of interest in the source image.
//pDst - Pointer to the destination image origin. 
//dstStep - Step in bytes through the destination image buffer.
//dstRoi - Region of interest in the destination image.
//angle - The angle of rotation in degrees. The source image is rotated counterclockwise 
//		around the origin (0,0).
//xShift and yShift - The shifts along horizontal and vertical axes to perform after 
//					the rotation.
//interpolation - The type of interpolation to perform for resampling the image. 
//				Use one of the following he following values:
//					FWI_INTER_NN nearest neighbor interpolation
//					FWI_INTER_LINEAR linear interpolation
//					FWI_INTER_CUBIC cubic interpolation
//Description:
//
//  The function fwiRotate is declared in the fwi.h file. This function rotates the Roi 
//of the source image by angle degrees (counterclockwise for positive angle values) 
//around the origin (0,0) that is implied to be in the top left corner, and shifts it 
//by xShift and yShift values along the x- and y- axes, respectively. The result is 
//resampled using the interpolation method specified by the interpolation parameter, 
//and written to the destination image Roi.
//
//  If you need to rotate an image about an arbitrary center (xCenter, yCenter), 
//use the function fwiGetRotateShift to compute the appropriate xShift, yShift values, 
//and then call fwiRotate with these values as input parameters. Alternatively, you can 
//use the fwiRotateCenter function instead.

#ifndef __ROTATE_DEF
#define __ROTATE_DEF
#define FW_SWAP(x,y,z) z=x;x=y;y=z
#define FW_MIN(x,y) (x>y)?y:x
#define FW_MAX(x,y) (x>y)?x:y
//Define epsilon for calculation error
#define FW_EPSILON 0.0001 
#define FW_ZERO(x) (((x)<FW_EPSILON) && ((x) > - FW_EPSILON))
#endif //__ROTATE_DEF

template< class TS, DispatchType disp >
void My_FW_Rotate_Region(float xltop, float yltop, float xlbot, float ylbot,
						 float xrtop, float yrtop, float xrbot, float yrbot,
						 int ystart, int yend, float coeffs[2][3],
						 const TS* pSrc, int srcStep, FwiRect srcRoi,
						 TS* pDst, int dstStep, FwiRect dstRoi,
						 int interpolation, int *flag, 
						 int channel, int channel1, Fw32f round)
{
	float ratel, rater, coeffl, coeffr, xleft, xright;
	float tx, ty, cx, cy, xmap, ymap;
	int x, y, xstart, xend;

	ratel  = (xlbot-xltop)/(ylbot-yltop);
	coeffl = xltop - yltop * ratel;
	rater  = (xrbot-xrtop)/(yrbot-yrtop);
	coeffr = xrtop - yrtop * rater;

	for (y=ystart;y<=yend;y++) {
		xleft  = ratel * y + coeffl;
		xright = rater * y + coeffr;
		xstart = (int) (FW_MAX(xleft, dstRoi.x));
		xend   = (int) (FW_MIN(xright, (dstRoi.x + dstRoi.width -1)));
					
		cy = y - coeffs[1][2];
		tx = coeffs[1][0] * cy; //-sin(theta)*cy
		ty = coeffs[1][1] * cy; //cos(theta)*cy
	
		for (x=xstart; x<=xend; x++) {
			cx = x - coeffs[0][2];
			xmap = coeffs[0][0] * cx + tx;
			ymap = coeffs[0][1] * cx + ty;
				
			My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
				pDst, dstStep, interpolation, flag, channel, channel1, round);
		}
	}

	return;
}

template< class TS, DispatchType disp >
void My_FW_Rotate_Region_8u_SSE2(float xltop, float yltop, float xlbot, float ylbot,
						 float xrtop, float yrtop, float xrbot, float yrbot,
						 int ystart, int yend, float coeffs[2][3],
						 const TS* pSrc, int srcStep, FwiRect srcRoi,
						 TS* pDst, int dstStep, FwiRect dstRoi,
						 int /*interpolation*/, int* flag, 
						 int channel, int chSrc, Fw32f /*round*/)
{
	float ratel, rater, coeffl, coeffr, xleft, xright;
	float tx, ty,cy;//, xmap, ymap;
	int x, y, xstart, xend;

	ratel  = (xlbot-xltop)/(ylbot-yltop);
	coeffl = xltop - yltop * ratel;
	rater  = (xrbot-xrtop)/(yrbot-yrtop);
	coeffr = xrtop - yrtop * rater;
    float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

    for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
    {
        cx[x - dstRoi.x] = x - coeffs[0][2];
        cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
        cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
    }
	for (y=ystart;y<=yend;y++) {
		xleft  = ratel * y + coeffl;
		xright = rater * y + coeffr;
		xstart = (int) (FW_MAX(xleft, dstRoi.x));
		xend   = (int) (FW_MIN(xright, (dstRoi.x + dstRoi.width -1)));
					
		cy = y - coeffs[1][2];
		tx = coeffs[1][0] * cy; //-sin(theta)*cy
		ty = coeffs[1][1] * cy; //cos(theta)*cy
        __m128 txXMM = _mm_set1_ps(tx);
        __m128 tyXMM = _mm_set1_ps(ty);

        XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
        XMM128 dst = {0};
        int y_dstStep = y * dstStep;
		
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-16; x+=16) {

                    for(int xx = 0 ; xx < 16; xx = xx + 4)
                    {                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x+xx-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x+xx-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.u8[xx + xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            *flag = 1;
                        }
                    }
                    _mm_storeu_si128((__m128i *)(pDst+y_dstStep+x), dst.i);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                    ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    *flag = 1;
	            }
		    }//end of C1
            else //if(chSrc == C4 || chSrc == AC4)
            {
                for (x=xstart; x<=xend-4; x+=4) {
                  
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;

                            if(chSrc == C4)
                            *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                            else 
                            {   
                                *((Fw16u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw16u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel+2);
                            }
                            *flag = 1;
                        }
                    //}
                    //_mm_storeu_si128((__m128i *)(pDst+y_dstStep+x * channel), dst.i);
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    if(chSrc == C4)
                    *((Fw32u*)(pDst+y_dstStep+x * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                    else 
                    {   
                        *((Fw16u*)(pDst+y_dstStep+x * channel)) = *((Fw16u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *(pDst+y_dstStep+x * channel +2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                    }
                    *flag = 1;
                }
	        }//end of C4, AC4
	}
    fwFree (cx);
    fwFree (cx_coeff00);
    fwFree (cx_coeff01);
    
	return;
}

template< class TS, CH chSrc, DispatchType disp >
static FwStatus My_FW_Rotate_8u_SSE2(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
					   TS* pDst, int dstStep, FwiRect dstRoi, 
					   double angle, double xShift, double yShift, int interpolation)
{
	int interpolation_E = interpolation ^ FWI_SMOOTH_EDGE;
	if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
		&& interpolation != FWI_INTER_CUBIC) {
		if ( interpolation_E != FWI_INTER_NN && interpolation_E != FWI_INTER_LINEAR 
			&& interpolation_E != FWI_INTER_CUBIC)
			return fwStsInterpolationErr;	
		interpolation = interpolation_E;
		interpolation_E = FWI_SMOOTH_EDGE;
	}
	
	int channel=ChannelCount(chSrc);
	FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoi, channel);
	if (status !=fwStsNoErr) return status;

	float theta;
	float coeffs[2][3];

	// [ cos (theta)   sin(theta) ]
	// [-sin (theta)   cos(theta) ]
	// theta is in the counter-clockwise, but y axis is down direction
	theta = (float)(0.0174532925199 * angle);//(3.14159265359/180.0)
	//cos and sin value need to be fixed
	coeffs[0][0] = (float)((int)(cos(theta)*32768))/32768;
	coeffs[0][1] = (float)((int)(sin(theta)*32768))/32768;
	coeffs[0][2] = (float)xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = (float)yShift;

	//return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi, 
	//	pDst, dstStep, dstRoi, coeffs, interpolation);
	int channel1;
	// Will not change 4th channel element in AC4
	if (chSrc == AC4) channel1=5;
	else channel1=channel;
	Fw32f round;
	// 32f is supported, but not 32u and 32s
	// No rounding is needed for 32f type
	if (sizeof(TS) == 4) round=0;
	else round=0.5;

	float sortX[4], sortY[4];
	float mapX[4], mapY[4], tempx;
	float temp1, temp2;

	//srcROI mapped area
	//Force to be floating data
	mapX[0]=(float)(coeffs[0][0] * srcRoi.x + coeffs[0][1] * srcRoi.y + coeffs[0][2]);
	mapY[0]=(float)(coeffs[1][0] * srcRoi.x + coeffs[1][1] * srcRoi.y + coeffs[1][2]);
	mapX[1]=(float)(mapX[0]+coeffs[0][0]*(srcRoi.width-1));
	mapY[1]=(float)(mapY[0]+coeffs[1][0]*(srcRoi.width-1));
	temp1 = (float)(coeffs[0][1] * (srcRoi.height-1));
	temp2 = (float)(coeffs[1][1] * (srcRoi.height-1));
	mapX[2]=mapX[1] + (float)temp1;
	mapY[2]=mapY[1] + (float)temp2;
	mapX[3]=mapX[0] + (float)temp1;
	mapY[3]=mapY[0] + (float)temp2;

	//Sort X,Y coordinator according to Y value
	//Before any pexchange, possible orders
	//Angle [0, 90) 1<=0<=2<=3 or 1<=2<=0<=3
	//Angle [90, 180)2<=3<=1<=0 or 2<=1<=3<=0
	//Angle [180, 270) 3<=2<=0<=1 or 3<=0<=2<=1
	//Angle [270, 360) 0<=1<=3<=2 or 0<=3<=1<=2
	if (mapY[0] > mapY[2]){
		sortX[0]=mapX[2];
		sortY[0]=mapY[2];
		sortX[2]=mapX[0];
		sortY[2]=mapY[0];
	} else {
		sortX[0]=mapX[0];
		sortY[0]=mapY[0];
		sortX[2]=mapX[2];
		sortY[2]=mapY[2];
	}

	if (mapY[1] > mapY[3]) {
		sortX[1]=mapX[3];
		sortY[1]=mapY[3];
		sortX[3]=mapX[1];
		sortY[3]=mapY[1];
	} else {
		sortX[1]=mapX[1];
		sortY[1]=mapY[1];
		sortX[3]=mapX[3];
		sortY[3]=mapY[3];
	}

	//After two exchanges, we have 1<=0<=2<=3 or 0<=1<=3<=2
	if (sortY[0]>sortY[1]) {
		FW_SWAP(sortY[0], sortY[1], tempx);
		FW_SWAP(sortX[0], sortX[1], tempx);
		FW_SWAP(sortY[2], sortY[3], tempx);
		FW_SWAP(sortX[2], sortX[3], tempx);
	}
	//We have 0<=1<=3<=2 after sorting

	int xstart, xend, ystart, yend;
	int x, y, flag=0;
	float cy, tx, ty;
	//float xmap, ymap;

	//dstStep and srcStep are byte size
	//we need to change it with data array size
	dstStep = dstStep / (sizeof(TS));
	srcStep = srcStep / (sizeof(TS));

	if (FW_ZERO(sortY[0]-sortY[1])) {//sortY[0]==sortY[1], sortY[2]=sortY[3]
		// In this case, the rotation angle must be 0, 90, 180, 270
		// We should seperate the case for best performance
		
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		if (sortX[0] < sortX[1]) {
			xstart = (int) (FW_MAX(sortX[0], dstRoi.x));
			xend = (int) (FW_MIN(sortX[1], (dstRoi.x + dstRoi.width -1)));
		} else {
			xstart = (int) (FW_MAX(sortX[1], dstRoi.x));
			xend = (int) (FW_MIN(sortX[0], (dstRoi.x + dstRoi.width -1)));
		}

        float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

        for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
        {
            cx[x - dstRoi.x] = x - coeffs[0][2];
            cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
            cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
        }


		for (y=ystart;y<=yend;y++) {
			cy = y - coeffs[1][2];
			tx = coeffs[1][0] * cy; //-sin(theta)*cy
			ty = coeffs[1][1] * cy; //cos(theta)*cy

            __m128 txXMM = _mm_set1_ps(tx);
            __m128 tyXMM = _mm_set1_ps(ty);
    
            XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
            XMM128 dst = {0};
            int y_dstStep = y * dstStep;
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-16; x+=16) {

                    for(int xx = 0 ; xx < 16; xx = xx + 4)
                    {                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x+xx-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x+xx-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.u8[xx + xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            flag = 1;
                        }
                    }
                    _mm_storeu_si128((__m128i *)(pDst+y_dstStep+x), dst.i);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                    ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    flag = 1;
	            }
		    }//end of C1
            else 
            {
                for (x=xstart; x<=xend-4; x+=4) {
                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;
	
                            if(chSrc == C4)
                            *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                            else 
                            {   
                                *((Fw16u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw16u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel);
                            }
                            flag = 1;
                        }
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    if(chSrc == C4)
                    *((Fw32u*)(pDst+y_dstStep+x * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                    else 
                    {   
                        *((Fw16u*)(pDst+y_dstStep+x * channel)) = *((Fw16u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *(pDst+y_dstStep+x * channel +2) = *(pSrc+ ymap*srcStep+xmap * channel);
                    }
                    flag = 1;
                }
	        }//end of C4, AC4

        }
        fwFree (cx);
        fwFree (cx_coeff00);
        fwFree (cx_coeff01);

	} else if (FW_ZERO(sortY[1]-sortY[3])) {//sortY[1]==sortY[3]
		if (sortX[1] < sortX[3]) {
			FW_SWAP(sortX[1], sortX[3], tempx);
		}

		//First part
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
			sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);

		//Second part
		ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[3], sortY[3], sortX[2], sortY[2], 
			sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);
	} else { //general case

		if (sortX[1] < sortX[3]) {
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[0], sortY[0], sortX[1], sortY[1], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[3], sortY[3], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		} else {//sortX[1] >= sortX[3]
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_8u_SSE2 <TS, disp> (sortX[3], sortY[3], sortX[2], sortY[2], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		}
	}

	//if no point is handled, return warning
	if (flag==0) return fwStsWrongIntersectROI;

	return fwStsNoErr;
}








void My_FW_Rotate_Region_16u_SSE2(float xltop, float yltop, float xlbot, float ylbot,
						 float xrtop, float yrtop, float xrbot, float yrbot,
						 int ystart, int yend, float coeffs[2][3],
						 const Fw16u* pSrc, int srcStep, FwiRect srcRoi,
						 Fw16u* pDst, int dstStep, FwiRect dstRoi,
						 int /*interpolation*/, int* flag, 
						 int channel, int chSrc, Fw32f /*round*/)
{
	float ratel, rater, coeffl, coeffr, xleft, xright;
	float tx, ty,cy;//, xmap, ymap;
	int x, y, xstart, xend;

	ratel  = (xlbot-xltop)/(ylbot-yltop);
	coeffl = xltop - yltop * ratel;
	rater  = (xrbot-xrtop)/(yrbot-yrtop);
	coeffr = xrtop - yrtop * rater;
    float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

    for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
    {
        cx[x - dstRoi.x] = x - coeffs[0][2];
        cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
        cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
    }
	for (y=ystart;y<=yend;y++) {
		xleft  = ratel * y + coeffl;
		xright = rater * y + coeffr;
		xstart = (int) (FW_MAX(xleft, dstRoi.x));
		xend   = (int) (FW_MIN(xright, (dstRoi.x + dstRoi.width -1)));
					
		cy = y - coeffs[1][2];
		tx = coeffs[1][0] * cy; //-sin(theta)*cy
		ty = coeffs[1][1] * cy; //cos(theta)*cy
        __m128 txXMM = _mm_set1_ps(tx);
        __m128 tyXMM = _mm_set1_ps(ty);

        XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
        XMM128 dst = {0};
        int y_dstStep = y * dstStep;
		
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-8; x+=8) {

                    for(int xx = 0 ; xx < 8; xx = xx + 4)
                    {                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x+xx-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x+xx-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.u16[xx + xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            *flag = 1;
                        }
                    }
                    _mm_storeu_si128((__m128i *)(pDst+y_dstStep+x), dst.i);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                    ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    *flag = 1;
	            }
		    }//end of C1
            else 
            {
                for (x=xstart; x<=xend-4; x+=4) {
                  
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;

                            if(chSrc == C4)
                            {
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel + 2)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel + 2));
                            }
                            else
                            {   
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                            }
                            *flag = 1;
                        }
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    if(chSrc == C4)
                    {
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel + 2)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel + 2));
                    }
                    else 
                    {   
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *(pDst+y_dstStep+(x) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                    }
                    *flag = 1;
                }
	        }//end of C4, AC4
	}
    fwFree (cx);
    fwFree (cx_coeff00);
    fwFree (cx_coeff01);
    
	return;
}

template< CH chSrc >
static FwStatus My_FW_Rotate_16u_SSE2(const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
					   Fw16u* pDst, int dstStep, FwiRect dstRoi, 
					   double angle, double xShift, double yShift, int interpolation)
{
	int interpolation_E = interpolation ^ FWI_SMOOTH_EDGE;
	if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
		&& interpolation != FWI_INTER_CUBIC) {
		if ( interpolation_E != FWI_INTER_NN && interpolation_E != FWI_INTER_LINEAR 
			&& interpolation_E != FWI_INTER_CUBIC)
			return fwStsInterpolationErr;	
		interpolation = interpolation_E;
		interpolation_E = FWI_SMOOTH_EDGE;
	}
	
	int channel=ChannelCount(chSrc);
	FwStatus status = My_FW_ParaCheck2<Fw16u>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoi, channel);
	if (status !=fwStsNoErr) return status;

	float theta;
	float coeffs[2][3];

	// [ cos (theta)   sin(theta) ]
	// [-sin (theta)   cos(theta) ]
	// theta is in the counter-clockwise, but y axis is down direction
	theta = (float)(0.0174532925199 * angle);//(3.14159265359/180.0)
	//cos and sin value need to be fixed
	coeffs[0][0] = (float)((int)(cos(theta)*32768))/32768;
	coeffs[0][1] = (float)((int)(sin(theta)*32768))/32768;
	coeffs[0][2] = (float)xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = (float)yShift;

	//return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi, 
	//	pDst, dstStep, dstRoi, coeffs, interpolation);
	int channel1;
	// Will not change 4th channel element in AC4
	if (chSrc == AC4) channel1=5;
	else channel1=channel;
	Fw32f round;
	// 32f is supported, but not 32u and 32s
	// No rounding is needed for 32f type
	//if (sizeof(TS) == 4) round=0;
	//else 
    round=0.5;

	float sortX[4], sortY[4];
	float mapX[4], mapY[4], tempx;
	float temp1, temp2;

	//srcROI mapped area
	//Force to be floating data
	mapX[0]=(float)(coeffs[0][0] * srcRoi.x + coeffs[0][1] * srcRoi.y + coeffs[0][2]);
	mapY[0]=(float)(coeffs[1][0] * srcRoi.x + coeffs[1][1] * srcRoi.y + coeffs[1][2]);
	mapX[1]=(float)(mapX[0]+coeffs[0][0]*(srcRoi.width-1));
	mapY[1]=(float)(mapY[0]+coeffs[1][0]*(srcRoi.width-1));
	temp1 = (float)(coeffs[0][1] * (srcRoi.height-1));
	temp2 = (float)(coeffs[1][1] * (srcRoi.height-1));
	mapX[2]=mapX[1] + (float)temp1;
	mapY[2]=mapY[1] + (float)temp2;
	mapX[3]=mapX[0] + (float)temp1;
	mapY[3]=mapY[0] + (float)temp2;

	//Sort X,Y coordinator according to Y value
	//Before any pexchange, possible orders
	//Angle [0, 90) 1<=0<=2<=3 or 1<=2<=0<=3
	//Angle [90, 180)2<=3<=1<=0 or 2<=1<=3<=0
	//Angle [180, 270) 3<=2<=0<=1 or 3<=0<=2<=1
	//Angle [270, 360) 0<=1<=3<=2 or 0<=3<=1<=2
	if (mapY[0] > mapY[2]){
		sortX[0]=mapX[2];
		sortY[0]=mapY[2];
		sortX[2]=mapX[0];
		sortY[2]=mapY[0];
	} else {
		sortX[0]=mapX[0];
		sortY[0]=mapY[0];
		sortX[2]=mapX[2];
		sortY[2]=mapY[2];
	}

	if (mapY[1] > mapY[3]) {
		sortX[1]=mapX[3];
		sortY[1]=mapY[3];
		sortX[3]=mapX[1];
		sortY[3]=mapY[1];
	} else {
		sortX[1]=mapX[1];
		sortY[1]=mapY[1];
		sortX[3]=mapX[3];
		sortY[3]=mapY[3];
	}

	//After two exchanges, we have 1<=0<=2<=3 or 0<=1<=3<=2
	if (sortY[0]>sortY[1]) {
		FW_SWAP(sortY[0], sortY[1], tempx);
		FW_SWAP(sortX[0], sortX[1], tempx);
		FW_SWAP(sortY[2], sortY[3], tempx);
		FW_SWAP(sortX[2], sortX[3], tempx);
	}
	//We have 0<=1<=3<=2 after sorting

	int xstart, xend, ystart, yend;
	int x, y, flag=0;
	float cy, tx, ty;
	//float xmap, ymap;

	//dstStep and srcStep are byte size
	//we need to change it with data array size
	dstStep = dstStep / (sizeof(Fw16u));
	srcStep = srcStep / (sizeof(Fw16u));

	if (FW_ZERO(sortY[0]-sortY[1])) {//sortY[0]==sortY[1], sortY[2]=sortY[3]
		// In this case, the rotation angle must be 0, 90, 180, 270
		// We should seperate the case for best performance
		
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		if (sortX[0] < sortX[1]) {
			xstart = (int) (FW_MAX(sortX[0], dstRoi.x));
			xend = (int) (FW_MIN(sortX[1], (dstRoi.x + dstRoi.width -1)));
		} else {
			xstart = (int) (FW_MAX(sortX[1], dstRoi.x));
			xend = (int) (FW_MIN(sortX[0], (dstRoi.x + dstRoi.width -1)));
		}

        float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

        for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
        {
            cx[x - dstRoi.x] = x - coeffs[0][2];
            cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
            cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
        }


		for (y=ystart;y<=yend;y++) {
			cy = y - coeffs[1][2];
			tx = coeffs[1][0] * cy; //-sin(theta)*cy
			ty = coeffs[1][1] * cy; //cos(theta)*cy

            __m128 txXMM = _mm_set1_ps(tx);
            __m128 tyXMM = _mm_set1_ps(ty);
    
            XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
            XMM128 dst = {0};
            int y_dstStep = y * dstStep;
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-8; x+=8) {

                    for(int xx = 0 ; xx < 8; xx = xx + 4)
                    {                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x+xx-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x+xx-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.u16[xx + xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            flag = 1;
                        }
                    }
                    _mm_storeu_si128((__m128i *)(pDst+y_dstStep+x), dst.i);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                    ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    flag = 1;
	            }
		    }//end of C1
            else 
            {
                for (x=xstart; x<=xend-4; x+=4) {
                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;

                            if(chSrc == C4)
                            {
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel + 2)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel + 2));
                            }
                            else 
                            {   
                                *((Fw32u*)(pDst+y_dstStep+(x+xxx) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                            }
                            flag = 1;
                        }
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;
                    if(chSrc == C4)
                    {
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel + 2)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel + 2));
                    }
                    else 
                    {   
                        *((Fw32u*)(pDst+y_dstStep+(x) * channel)) = *((Fw32u*)(pSrc+ ymap*srcStep+xmap * channel));
                        *(pDst+y_dstStep+(x) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                    }
                    flag = 1;
                }
	        }//end of C4, AC4
            
        }
            fwFree (cx);
            fwFree (cx_coeff00);
            fwFree (cx_coeff01);

	} else if (FW_ZERO(sortY[1]-sortY[3])) {//sortY[1]==sortY[3]
		if (sortX[1] < sortX[3]) {
			FW_SWAP(sortX[1], sortX[3], tempx);
		}

		//First part
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_16u_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
			sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);

		//Second part
		ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_16u_SSE2 (sortX[3], sortY[3], sortX[2], sortY[2], 
			sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);
	} else { //general case

		if (sortX[1] < sortX[3]) {
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_16u_SSE2 (sortX[0], sortY[0], sortX[1], sortY[1], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_16u_SSE2 (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_16u_SSE2 (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[3], sortY[3], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		} else {//sortX[1] >= sortX[3]
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_16u_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_16u_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_16u_SSE2 (sortX[3], sortY[3], sortX[2], sortY[2], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		}
	}

	//if no point is handled, return warning
	if (flag==0) return fwStsWrongIntersectROI;

	return fwStsNoErr;
}










void My_FW_Rotate_Region_32f_SSE2(float xltop, float yltop, float xlbot, float ylbot,
						 float xrtop, float yrtop, float xrbot, float yrbot,
						 int ystart, int yend, float coeffs[2][3],
						 const Fw32f* pSrc, int srcStep, FwiRect srcRoi,
						 Fw32f* pDst, int dstStep, FwiRect dstRoi,
						 int /*interpolation*/, int* flag, 
						 int channel, int chSrc, Fw32f /*round*/)
{
	float ratel, rater, coeffl, coeffr, xleft, xright;
	float tx, ty,cy;//, xmap, ymap;
	int x, y, xstart, xend;

	ratel  = (xlbot-xltop)/(ylbot-yltop);
	coeffl = xltop - yltop * ratel;
	rater  = (xrbot-xrtop)/(yrbot-yrtop);
	coeffr = xrtop - yrtop * rater;
    float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
    float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

    for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
    {
        cx[x - dstRoi.x] = x - coeffs[0][2];
        cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
        cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
    }
	for (y=ystart;y<=yend;y++) {
		xleft  = ratel * y + coeffl;
		xright = rater * y + coeffr;
		xstart = (int) (FW_MAX(xleft, dstRoi.x));
		xend   = (int) (FW_MIN(xright, (dstRoi.x + dstRoi.width -1)));
					
		cy = y - coeffs[1][2];
		tx = coeffs[1][0] * cy; //-sin(theta)*cy
		ty = coeffs[1][1] * cy; //cos(theta)*cy
        __m128 txXMM = _mm_set1_ps(tx);
        __m128 tyXMM = _mm_set1_ps(ty);

        XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
        XMM128 dst = {0};
        int y_dstStep = y * dstStep;
		
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-4; x+=4) {
                  
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.f32[xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            *flag = 1;
                        }
                    _mm_storeu_ps((pDst+y_dstStep+x), dst.f);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                    ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    *flag = 1;
	            }
		    }//end of C1
            else 
            {
                for (x=xstart; x<=xend-4; x+=4) {
                  
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;
	
                            if(chSrc == C4)
                            {
                                dst.f = _mm_loadu_ps(pSrc+ ymap*srcStep+xmap * channel);
                                _mm_storeu_ps((pDst+y_dstStep+(x+xxx) * channel), dst.f);
                            }
                            else 
                            {   
                                *(pDst+y_dstStep+(x+xxx) * channel) = *(pSrc+ ymap*srcStep+xmap * channel);
                                *(pDst+y_dstStep+(x+xxx) * channel + 1) = *(pSrc+ ymap*srcStep+xmap * channel + 1);
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                            }
                            *flag = 1;
                        }
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    
                    if(chSrc == C4)
                    {
                        dst.f = _mm_loadu_ps(pSrc+ ymap*srcStep+xmap * channel);
                        _mm_storeu_ps((pDst+y_dstStep+(x) * channel), dst.f);

                    }
                    else 
                    {   
                        *(pDst+y_dstStep+(x) * channel) = *(pSrc+ ymap*srcStep+xmap * channel);
                        *(pDst+y_dstStep+(x) * channel + 1) = *(pSrc+ ymap*srcStep+xmap * channel + 1);
                        *(pDst+y_dstStep+(x) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                    }
                    *flag = 1;
                }
	        }//end of C4, AC4
	}
    fwFree (cx);
    fwFree (cx_coeff00);
    fwFree (cx_coeff01);
    
	return;
}

template< CH chSrc >
static FwStatus My_FW_Rotate_32f_SSE2(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
					   Fw32f* pDst, int dstStep, FwiRect dstRoi, 
					   double angle, double xShift, double yShift, int interpolation)
{
	int interpolation_E = interpolation ^ FWI_SMOOTH_EDGE;
	if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
		&& interpolation != FWI_INTER_CUBIC) {
		if ( interpolation_E != FWI_INTER_NN && interpolation_E != FWI_INTER_LINEAR 
			&& interpolation_E != FWI_INTER_CUBIC)
			return fwStsInterpolationErr;	
		interpolation = interpolation_E;
		interpolation_E = FWI_SMOOTH_EDGE;
	}
	
	int channel=ChannelCount(chSrc);
	FwStatus status = My_FW_ParaCheck2<Fw32f>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoi, channel);
	if (status !=fwStsNoErr) return status;

	float theta;
	float coeffs[2][3];

	// [ cos (theta)   sin(theta) ]
	// [-sin (theta)   cos(theta) ]
	// theta is in the counter-clockwise, but y axis is down direction
	theta = (float)(0.0174532925199 * angle);//(3.14159265359/180.0)
	//cos and sin value need to be fixed
	coeffs[0][0] = (float)((int)(cos(theta)*32768))/32768;
	coeffs[0][1] = (float)((int)(sin(theta)*32768))/32768;
	coeffs[0][2] = (float)xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = (float)yShift;

	//return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi, 
	//	pDst, dstStep, dstRoi, coeffs, interpolation);
	int channel1;
	// Will not change 4th channel element in AC4
	if (chSrc == AC4) channel1=5;
	else channel1=channel;
	Fw32f round;
	// 32f is supported, but not 32u and 32s
	// No rounding is needed for 32f type
	//if (sizeof(TS) == 4) round=0;
	//else 
    round=0.5;

	float sortX[4], sortY[4];
	float mapX[4], mapY[4], tempx;
	float temp1, temp2;

	//srcROI mapped area
	//Force to be floating data
	mapX[0]=(float)(coeffs[0][0] * srcRoi.x + coeffs[0][1] * srcRoi.y + coeffs[0][2]);
	mapY[0]=(float)(coeffs[1][0] * srcRoi.x + coeffs[1][1] * srcRoi.y + coeffs[1][2]);
	mapX[1]=(float)(mapX[0]+coeffs[0][0]*(srcRoi.width-1));
	mapY[1]=(float)(mapY[0]+coeffs[1][0]*(srcRoi.width-1));
	temp1 = (float)(coeffs[0][1] * (srcRoi.height-1));
	temp2 = (float)(coeffs[1][1] * (srcRoi.height-1));
	mapX[2]=mapX[1] + (float)temp1;
	mapY[2]=mapY[1] + (float)temp2;
	mapX[3]=mapX[0] + (float)temp1;
	mapY[3]=mapY[0] + (float)temp2;

	//Sort X,Y coordinator according to Y value
	//Before any pexchange, possible orders
	//Angle [0, 90) 1<=0<=2<=3 or 1<=2<=0<=3
	//Angle [90, 180)2<=3<=1<=0 or 2<=1<=3<=0
	//Angle [180, 270) 3<=2<=0<=1 or 3<=0<=2<=1
	//Angle [270, 360) 0<=1<=3<=2 or 0<=3<=1<=2
	if (mapY[0] > mapY[2]){
		sortX[0]=mapX[2];
		sortY[0]=mapY[2];
		sortX[2]=mapX[0];
		sortY[2]=mapY[0];
	} else {
		sortX[0]=mapX[0];
		sortY[0]=mapY[0];
		sortX[2]=mapX[2];
		sortY[2]=mapY[2];
	}

	if (mapY[1] > mapY[3]) {
		sortX[1]=mapX[3];
		sortY[1]=mapY[3];
		sortX[3]=mapX[1];
		sortY[3]=mapY[1];
	} else {
		sortX[1]=mapX[1];
		sortY[1]=mapY[1];
		sortX[3]=mapX[3];
		sortY[3]=mapY[3];
	}

	//After two exchanges, we have 1<=0<=2<=3 or 0<=1<=3<=2
	if (sortY[0]>sortY[1]) {
		FW_SWAP(sortY[0], sortY[1], tempx);
		FW_SWAP(sortX[0], sortX[1], tempx);
		FW_SWAP(sortY[2], sortY[3], tempx);
		FW_SWAP(sortX[2], sortX[3], tempx);
	}
	//We have 0<=1<=3<=2 after sorting

	int xstart, xend, ystart, yend;
	int x, y, flag=0;
	float cy, tx, ty;
	//float xmap, ymap;

	//dstStep and srcStep are byte size
	//we need to change it with data array size
	dstStep = dstStep / (sizeof(Fw32f));
	srcStep = srcStep / (sizeof(Fw32f));

	if (FW_ZERO(sortY[0]-sortY[1])) {//sortY[0]==sortY[1], sortY[2]=sortY[3]
		// In this case, the rotation angle must be 0, 90, 180, 270
		// We should seperate the case for best performance
		
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		if (sortX[0] < sortX[1]) {
			xstart = (int) (FW_MAX(sortX[0], dstRoi.x));
			xend = (int) (FW_MIN(sortX[1], (dstRoi.x + dstRoi.width -1)));
		} else {
			xstart = (int) (FW_MAX(sortX[1], dstRoi.x));
			xend = (int) (FW_MIN(sortX[0], (dstRoi.x + dstRoi.width -1)));
		}

        float* cx = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff00 = (float*)fwMalloc((dstRoi.width) * sizeof(float));
        float* cx_coeff01 = (float*)fwMalloc((dstRoi.width) * sizeof(float));

        for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width); x++) 
        {
            cx[x - dstRoi.x] = x - coeffs[0][2];
            cx_coeff00[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][0];
            cx_coeff01[x - dstRoi.x] = cx[x - dstRoi.x] * coeffs[0][1];
        }


		for (y=ystart;y<=yend;y++) {
			cy = y - coeffs[1][2];
			tx = coeffs[1][0] * cy; //-sin(theta)*cy
			ty = coeffs[1][1] * cy; //cos(theta)*cy

            __m128 txXMM = _mm_set1_ps(tx);
            __m128 tyXMM = _mm_set1_ps(ty);
    
            XMM128 cxCoeff00 = {0}, cxCoeff01 = {0};
            XMM128 dst = {0};
            int y_dstStep = y * dstStep;
            if(chSrc == C1)
            {
	            for (x=xstart; x<=xend-4; x+=4) {           
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
			                    ymap < 0 || ymap > srcRoi.height- 1) {
				                continue;
		                    }

		                    xmap += srcRoi.x;
		                    ymap += srcRoi.y;

				            dst.f32[xxx] =	*(pSrc+ ymap*srcStep+xmap);
                            flag = 1;
                        }
                    _mm_storeu_ps((pDst+y_dstStep+x), dst.f);
	            }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||ymap < 0 || ymap > srcRoi.height- 1) {
			                continue;
	                }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    *(pDst+y_dstStep+x) =	*(pSrc+ ymap*srcStep+xmap);
                    flag = 1;
	            }
		    }//end of C1
            else 
            {
                for (x=xstart; x<=xend-4; x+=4) {
                   
                        cxCoeff00.f = _mm_loadu_ps(cx_coeff00+(x-dstRoi.x));
                        cxCoeff01.f = _mm_loadu_ps(cx_coeff01+(x-dstRoi.x));
                        
                        cxCoeff00.f = _mm_add_ps(cxCoeff00.f, txXMM);
                        cxCoeff01.f = _mm_add_ps(cxCoeff01.f, tyXMM);

                        cxCoeff00.i = _mm_cvttps_epi32 (cxCoeff00.f);
                        cxCoeff01.i = _mm_cvttps_epi32 (cxCoeff01.f);

                        for(int xxx = 0; xxx < 4; xxx++)
                        {
                            int &xmap = cxCoeff00.s32[xxx];
                            int &ymap = cxCoeff01.s32[xxx];

                            if (xmap < 0 || xmap > srcRoi.width - 1 ||
		                        ymap < 0 || ymap > srcRoi.height- 1) {
			                    continue;
	                        }

	                        xmap += srcRoi.x;
	                        ymap += srcRoi.y;

                            if(chSrc == C4)
                            {
                                dst.f = _mm_loadu_ps(pSrc+ ymap*srcStep+xmap * channel);
                                _mm_storeu_ps(pDst+y_dstStep+(x+xxx) * channel, dst.f);
                            }
                            else //if(chSrc == AC4)
                            {   
                                *(pDst+y_dstStep+(x+xxx) * channel) = *(pSrc+ ymap*srcStep+xmap * channel);
                                *(pDst+y_dstStep+(x+xxx) * channel + 1) = *(pSrc+ ymap*srcStep+xmap * channel + 1);
                                *(pDst+y_dstStep+(x+xxx) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                            }
                            flag = 1;
                        }
                }
                for (; x<=xend; x++) {

                    int xmap = (int)(cx_coeff00[ x - dstRoi.x] + tx);
                    int ymap = (int)(cx_coeff01[ x - dstRoi.x] + ty);

                    if (xmap < 0 || xmap > srcRoi.width - 1 ||
	                        ymap < 0 || ymap > srcRoi.height- 1) {
		                    continue;
                    }

                    xmap += srcRoi.x;
                    ymap += srcRoi.y;

                    
                    if(chSrc == C4)
                    {
                        dst.f = _mm_loadu_ps(pSrc+ ymap*srcStep+xmap * channel);
                        _mm_storeu_ps(pDst+y_dstStep+x * channel, dst.f);
                    }
                    else 
                    {   
                        *(pDst+y_dstStep+(x) * channel) = *(pSrc+ ymap*srcStep+xmap * channel);
                        *(pDst+y_dstStep+(x) * channel + 1) = *(pSrc+ ymap*srcStep+xmap * channel + 1);
                        *(pDst+y_dstStep+(x) * channel + 2) = *(pSrc+ ymap*srcStep+xmap * channel + 2);
                    }
                    flag = 1;
                }
	        }//end of C4, AC4
            
        }
            fwFree (cx);
            fwFree (cx_coeff00);
            fwFree (cx_coeff01);

	} else if (FW_ZERO(sortY[1]-sortY[3])) {//sortY[1]==sortY[3]
		if (sortX[1] < sortX[3]) {
			FW_SWAP(sortX[1], sortX[3], tempx);
		}

		//First part
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_32f_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
			sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);

		//Second part
		ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region_32f_SSE2 (sortX[3], sortY[3], sortX[2], sortY[2], 
			sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);
	} else { //general case

		if (sortX[1] < sortX[3]) {
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_32f_SSE2 (sortX[0], sortY[0], sortX[1], sortY[1], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_32f_SSE2 (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_32f_SSE2 (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[3], sortY[3], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		} else {//sortX[1] >= sortX[3]
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region_32f_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_32f_SSE2 (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region_32f_SSE2 (sortX[3], sortY[3], sortX[2], sortY[2], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		}
	}

	//if no point is handled, return warning
	if (flag==0) return fwStsWrongIntersectROI;

	return fwStsNoErr;
}







template< class TS, CH chSrc, DispatchType disp >
static FwStatus My_FW_Rotate(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
					   TS* pDst, int dstStep, FwiRect dstRoi, 
					   double angle, double xShift, double yShift, int interpolation)
{
	int interpolation_E = interpolation ^ FWI_SMOOTH_EDGE;
	if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR 
		&& interpolation != FWI_INTER_CUBIC) {
		if ( interpolation_E != FWI_INTER_NN && interpolation_E != FWI_INTER_LINEAR 
			&& interpolation_E != FWI_INTER_CUBIC)
			return fwStsInterpolationErr;	
		interpolation = interpolation_E;
		interpolation_E = FWI_SMOOTH_EDGE;
	}
	
	int channel=ChannelCount(chSrc);
	FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoi, channel);
	if (status !=fwStsNoErr) return status;

	float theta;
	float coeffs[2][3];

	// [ cos (theta)   sin(theta) ]
	// [-sin (theta)   cos(theta) ]
	// theta is in the counter-clockwise, but y axis is down direction
	theta = (float)(0.0174532925199 * angle);//(3.14159265359/180.0)
	//cos and sin value need to be fixed
	coeffs[0][0] = (float)((int)(cos(theta)*32768))/32768;
	coeffs[0][1] = (float)((int)(sin(theta)*32768))/32768;
	coeffs[0][2] = (float)xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = (float)yShift;

	//return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi, 
	//	pDst, dstStep, dstRoi, coeffs, interpolation);
	int channel1;
	// Will not change 4th channel element in AC4
	if (chSrc == AC4) channel1=3;
	else channel1=channel;
	Fw32f round;
	// 32f is supported, but not 32u and 32s
	// No rounding is needed for 32f type
	if (sizeof(TS) == 4) round=0;
	else round=0.5;

	float sortX[4], sortY[4];
	float mapX[4], mapY[4], tempx;
	float temp1, temp2;

	//srcROI mapped area
	//Force to be floating data
	mapX[0]=(float)(coeffs[0][0] * srcRoi.x + coeffs[0][1] * srcRoi.y + coeffs[0][2]);
	mapY[0]=(float)(coeffs[1][0] * srcRoi.x + coeffs[1][1] * srcRoi.y + coeffs[1][2]);
	mapX[1]=(float)(mapX[0]+coeffs[0][0]*(srcRoi.width-1));
	mapY[1]=(float)(mapY[0]+coeffs[1][0]*(srcRoi.width-1));
	temp1 = (float)(coeffs[0][1] * (srcRoi.height-1));
	temp2 = (float)(coeffs[1][1] * (srcRoi.height-1));
	mapX[2]=mapX[1] + (float)temp1;
	mapY[2]=mapY[1] + (float)temp2;
	mapX[3]=mapX[0] + (float)temp1;
	mapY[3]=mapY[0] + (float)temp2;

	//Sort X,Y coordinator according to Y value
	//Before any pexchange, possible orders
	//Angle [0, 90) 1<=0<=2<=3 or 1<=2<=0<=3
	//Angle [90, 180)2<=3<=1<=0 or 2<=1<=3<=0
	//Angle [180, 270) 3<=2<=0<=1 or 3<=0<=2<=1
	//Angle [270, 360) 0<=1<=3<=2 or 0<=3<=1<=2
	if (mapY[0] > mapY[2]){
		sortX[0]=mapX[2];
		sortY[0]=mapY[2];
		sortX[2]=mapX[0];
		sortY[2]=mapY[0];
	} else {
		sortX[0]=mapX[0];
		sortY[0]=mapY[0];
		sortX[2]=mapX[2];
		sortY[2]=mapY[2];
	}

	if (mapY[1] > mapY[3]) {
		sortX[1]=mapX[3];
		sortY[1]=mapY[3];
		sortX[3]=mapX[1];
		sortY[3]=mapY[1];
	} else {
		sortX[1]=mapX[1];
		sortY[1]=mapY[1];
		sortX[3]=mapX[3];
		sortY[3]=mapY[3];
	}

	//After two exchanges, we have 1<=0<=2<=3 or 0<=1<=3<=2
	if (sortY[0]>sortY[1]) {
		FW_SWAP(sortY[0], sortY[1], tempx);
		FW_SWAP(sortX[0], sortX[1], tempx);
		FW_SWAP(sortY[2], sortY[3], tempx);
		FW_SWAP(sortX[2], sortX[3], tempx);
	}
	//We have 0<=1<=3<=2 after sorting

	int xstart, xend, ystart, yend;
	int x, y, flag=0;
	float cx, cy, tx, ty;
	float xmap, ymap;

	//dstStep and srcStep are byte size
	//we need to change it with data array size
	dstStep = dstStep / (sizeof(TS));
	srcStep = srcStep / (sizeof(TS));

	if (FW_ZERO(sortY[0]-sortY[1])) {//sortY[0]==sortY[1], sortY[2]=sortY[3]
		// In this case, the rotation angle must be 0, 90, 180, 270
		// We should seperate the case for best performance
		
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		if (sortX[0] < sortX[1]) {
			xstart = (int) (FW_MAX(sortX[0], dstRoi.x));
			xend = (int) (FW_MIN(sortX[1], (dstRoi.x + dstRoi.width -1)));
		} else {
			xstart = (int) (FW_MAX(sortX[1], dstRoi.x));
			xend = (int) (FW_MIN(sortX[0], (dstRoi.x + dstRoi.width -1)));
		}

		for (y=ystart;y<=yend;y++) {
			cy = y - coeffs[1][2];
			tx = coeffs[1][0] * cy; //-sin(theta)*cy
			ty = coeffs[1][1] * cy; //cos(theta)*cy

			for (x=xstart; x<=xend; x++) {
				cx = x - coeffs[0][2];
				xmap = coeffs[0][0] * cx + tx;
				ymap = coeffs[0][1] * cx + ty;
				
				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}
	} else if (FW_ZERO(sortY[1]-sortY[3])) {//sortY[1]==sortY[3]
		if (sortX[1] < sortX[3]) {
			FW_SWAP(sortX[1], sortX[3], tempx);
		}

		//First part
		ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
			sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);

		//Second part
		ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
		yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
		
		My_FW_Rotate_Region <TS, disp> (sortX[3], sortY[3], sortX[2], sortY[2], 
			sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
			pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
			channel, channel1, round);
	} else { //general case

		if (sortX[1] < sortX[3]) {
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region <TS, disp> (sortX[0], sortY[0], sortX[1], sortY[1], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region <TS, disp> (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[0], sortY[0], sortX[3], sortY[3], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region <TS, disp> (sortX[1], sortY[1], sortX[2], sortY[2], 
				sortX[3], sortY[3], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		} else {//sortX[1] >= sortX[3]
			//First part
			ystart = (int) (FW_MAX(sortY[0], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[1], (dstRoi.y + dstRoi.height -1)));
		
			My_FW_Rotate_Region <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[0], sortY[0], sortX[1], sortY[1], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Second part
			ystart = (int) (FW_MAX(sortY[1], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[3], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region <TS, disp> (sortX[0], sortY[0], sortX[3], sortY[3], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);

			//Third part
			ystart = (int) (FW_MAX(sortY[3], dstRoi.y));
			yend   = (int) (FW_MIN(sortY[2], (dstRoi.y + dstRoi.height -1)));
			
			My_FW_Rotate_Region <TS, disp> (sortX[3], sortY[3], sortX[2], sortY[2], 
				sortX[1], sortY[1], sortX[2], sortY[2], ystart, yend, coeffs,
				pSrc, srcStep, srcRoi, pDst, dstStep, dstRoi, interpolation, &flag, 
				channel, channel1, round);
		}
	}

	//if no point is handled, return warning
	if (flag==0) return fwStsWrongIntersectROI;

	return fwStsNoErr;
}

//Description
//The function fwiRotateCenter is declared in the fwi.h file. This function rotates the ROI of
//the source image by angle degrees (counterclockwise for positive angle values) around the point
//with coordinates xCenter, yCenter. The origin of the source image is implied to be in the top
//left corner. The result is resampled using the interpolation method specified by the
//interpolation parameter, and written to the destination image ROI.

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_RotateCenter(const TS* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							 TS* pDst, int dstStep, FwiRect dstRoi, 
							 double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return My_FW_Rotate<TS, chSrc, disp>(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C1R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw8u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	
    switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_8u_SSE2 <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default: 
	        return My_FW_Rotate <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C3R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw8u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_8u_SSE2 <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw8u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_8u_SSE2 <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_AC4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw8u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_8u_SSE2 <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C1R)(const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw16u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_16u_SSE2 <C1> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw16u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C3R)(const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw16u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_16u_SSE2 <C3> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C4R)(const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw16u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_16u_SSE2 <C4> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_AC4R)(const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw16u *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_16u_SSE2 <AC4> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C1R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw32f *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
    switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_32f_SSE2 <C1> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
	    return My_FW_Rotate <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C3R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw32f *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_32f_SSE2 <C3> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw32f *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_32f_SSE2 <C4> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_AC4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
							Fw32f *pDst, int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_Rotate_32f_SSE2 <AC4> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
			
	default:
        return My_FW_Rotate <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
    }
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_P3R)(const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status =  My_FW_WarpAffine <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_P4R)(const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status =  My_FW_WarpAffine <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_P3R)(const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw16u *const pDst[3], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status =  My_FW_WarpAffine <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_P4R)(const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw16u *const pDst[4], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status =  My_FW_WarpAffine <Fw16u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_P3R)(const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status =  My_FW_WarpAffine <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_P4R)(const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, 
							FwiRect srcRoi, Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
							double angle, double xShift, double yShift, int interpolation)
{
	double theta;
	double coeffs[2][3];

// [ cos (theta)   sin(theta) ]
// [-sin (theta)   cos(theta) ]
// theta is in the counter-clockwise, but y axis is down direction
	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	coeffs[0][0] = cos(theta);
	coeffs[0][1] = sin(theta);
	coeffs[0][2] = xShift;
	coeffs[1][0] =-coeffs[0][1];//-sin(theta)
	coeffs[1][1] = coeffs[0][0];//cos(theta)
	coeffs[1][2] = yShift;
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status =  My_FW_WarpAffine <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, coeffs, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//Description
//The function fwiGetRotateShift is declared in the fwi.h file. Use this function if you need
//to rotate an image about an arbitrary center (xCenter, yCenter) rather than the origin (0,0).
//The function helps compute shift values xShift, yShift that should be passed to fwiRotate
//function for the rotation around (xCenter, yCenter) to take place.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetRotateShift)(double xCenter, double yCenter, 
							 double angle, double *xShift, double *yShift)
{
	if (xShift == 0 || yShift == 0 )
		return fwStsNullPtrErr;

	double theta, c00, c01, c10, c11;

	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	c00 = cos(theta);
	c01 = sin(theta);
	c10 =-c01;//-sin(theta)
	c11 = c00;//cos(theta)
	double newCenterx = xCenter * c00 + yCenter * c01;
	double newCentery = xCenter * c10 + yCenter * c11;

	*xShift= xCenter-newCenterx; 
	*yShift= yCenter-newCentery;

	return fwStsNoErr;
}

//Description
//The function fwiAddRotateShift is declared in the fwi.h file. Use this function if you
//need to rotate an image about an arbitrary center (xCenter, yCenter) rather than the origin
//(0,0) with required shifts. The function helps compute shift values xShift, yShift that should
//be passed to fwiRotate function to perform the rotation around (xCenter, yCenter) and
//desired shifting. The shift values should be initialized. For example, to rotate an image around a
//point (xCenter, yCenter) by the angle with shift values (30.3, 26.2) the following code must be
//written:
//xShift = 30.3
//yShift = 26.2
//fwiAddRotateShift(xCenter,yCenter,angle,&xShift,&yShift);
//fwiRotate(angle,xShift,yShift);

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddRotateShift)(double xCenter, double yCenter, 
							 double angle, double *xShift, double *yShift)
{
	if (xShift == 0 || yShift == 0 )
		return fwStsNullPtrErr;

	double theta, c00, c01, c10, c11;

	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	c00 = cos(theta);
	c01 = sin(theta);
	c10 = -c01;//-sin(theta)
	c11 = c00;//cos(theta)
	double newCenterx = xCenter * c00 + yCenter * c01;
	double newCentery = xCenter * c10 + yCenter * c11;

	*xShift += xCenter-newCenterx; 
	*yShift += yCenter-newCentery;

	return fwStsNoErr;
}


//Description
//The function fwiGetRotateQuad is declared in the fwi.h file. This function is used as a
//support function for fwiRotate. It computes vertex coordinates of the quadrangle, to which the
//source rectangular ROI would be mapped by the fwiRotate function that rotates an image by
//angle degrees and shifts it by xShift, yShift.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//quad[0] corresponds to the transformed top-left corner of the source ROI,
//quad[1] corresponds to the transformed top-right corner of the source ROI,
//quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//quad[3] corresponds to the transformed bottom-left corner of the source ROI.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetRotateQuad)(FwiRect srcRoi, double quad[4][2], 
							 double angle, double xShift, double yShift)
{
	double theta, c[2][3];

	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	c[0][0] = cos(theta);
	c[0][1] = sin(theta);
	c[1][0] = -c[0][1];//-sin(theta)
	c[1][1] = c[0][0];//cos(theta)
	c[0][2] = xShift;
	c[1][2] = yShift;

	return fwiGetAffineQuad(srcRoi, quad, c);
}


//Description
//The function fwiGetRotateBound is declared in the fwi.h file. This function is used as a
//support function for fwiRotate. It computes vertex coordinates of the smallest bounding
//rectangle for the quadrangle quad, to which the source ROI would be mapped by the
//fwiRotate function that rotates an image by angle degrees and shifts it by xShift,
//yShift.
//bound[0] specifies x, y coordinates of the top-left corner, bound[1] specifies x, y coordinates
//of the bottom-right corner.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetRotateBound)(FwiRect srcRoi, double bound[2][2], 
							 double angle, double xShift, double yShift)
{
	double theta, c[2][3];

	theta = 0.0174532925199 * angle;//(3.14159265359/180.0)
	c[0][0] = cos(theta);
	c[0][1] = sin(theta);
	c[1][0] =-c[0][1];//-sin(theta)
	c[1][1] = c[0][0];//cos(theta)
	c[0][2] = xShift;
	c[1][2] = yShift;

	return fwiGetAffineBound(srcRoi, bound, c);
}
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C1R)(const Fw8u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C3R)(const Fw8u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C4R)(const Fw8u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_AC4R)(const Fw8u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C1R)(const Fw16u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw16u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw16u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C3R)(const Fw16u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw16u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C4R)(const Fw16u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw16u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_AC4R)(const Fw16u *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw16u *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C1R)(const Fw32f *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C3R)(const Fw32f *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C4R)(const Fw32f *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_AC4R)(const Fw32f *pSrc,FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *pDst, int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	return My_FW_RotateCenter <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_P3R)(const Fw8u *const pSrc[3],FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_8u_P3R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_P4R)(const Fw8u *const pSrc[4],FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_8u_P4R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_P3R)(const Fw16u *const pSrc[3],FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw16u *const pDst[3], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_16u_P3R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_P4R)(const Fw16u *const pSrc[4],FwiSize srcSize, int srcStep,
								  FwiRect srcRoi, Fw16u *const pDst[4], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_16u_P4R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_P3R)(const Fw32f *const pSrc[3],FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_32f_P3R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_P4R)(const Fw32f *const pSrc[4],FwiSize srcSize, int srcStep, 
								  FwiRect srcRoi, Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, 
								  double angle, double xCenter, double yCenter, int interpolation)
{
	double xShift, yShift;

	fwiGetRotateShift(xCenter, yCenter, angle, &xShift, &yShift);

	return fwiRotate_32f_P4R(pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, angle, xShift, yShift, interpolation);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
