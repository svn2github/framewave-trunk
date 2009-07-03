/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"
#include "Resize.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 100 )

#pragma warning( disable: 4244 )

//Internal data structure to record point position 
//and corresponding coefficients

//for Cubic interpolation with double for coefficients
struct Cubic_Array_Double{
	int pos[4];
	double icoeff[4];
};

//for Cubic interpolation with short integer for coefficients
//(shifted value)
struct Cubic_Array_Short{
	int pos[4];
	Fw16s icoeff[4];
};

//for Cubic interpolation with float for coefficients
//struct Cubic_Array_Float{
//	int pos[4];
//	float icoeff[4];
//};

//for LancZos interpolation with double for coefficients
struct Lanczos_Array_Double{
	int pos[6];
	double icoeff[6];
};

namespace OPT_LEVEL
{
	//Internal function for resizeCenter 8u data with Linear interpolation
	FwStatus My_FW_ResizeCenter_8u_LN(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, double xCenter, double yCenter, int channel)
	{
		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		int x, y;//, k;
		int xbegin, xend, ybegin, yend;
		int resizeWidth = dstRoiSize.width;
		int resizeHeight = dstRoiSize.height;

		double xnewCenter=(double)dstRoiSize.width/2;
		double ynewCenter=(double)dstRoiSize.height/2;

		//calculate the begin and end of x&y coordinate for source image 
		//corresponding to destination ROI.
		xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
		if (xend > dstRoiSize.width) xend=dstRoiSize.width;
		xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
		if (xbegin<0) xbegin=0;
		yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
		if (yend > dstRoiSize.height) yend=dstRoiSize.height;
		ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
		if (ybegin<0) ybegin=0;

		//use fwMalloc instead of malloc for aligned address
		Linear_Array *pX_Array_Value = (Linear_Array*) fwMalloc(resizeWidth *sizeof(Linear_Array));
		Linear_Array *pY_Array_Value = (Linear_Array*) fwMalloc(resizeHeight*sizeof(Linear_Array));

		int ceil_X, floor_X, ceil_Y, floor_Y;
		double fraction_X, fractY, weight_shift;
		short ifraction_x, ione_minus_x, ifraction_y, ione_minus_y;

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		Fw16u *pIfx, *pIofx;
		pIfx = (Fw16u *) fwMalloc(resizeWidth*channel1*sizeof(Fw16u));
		pIofx = (Fw16u *) fwMalloc(resizeWidth*channel1*sizeof(Fw16u));

		weight_shift = (double) (1<<FW_WEIGHT);

		//Pre-calculate the x coefficient.
		for (x = xbegin; x < xend; x++ ) {   
			double fx = (double)(x-xnewCenter)/xFactor + xCenter;
			floor_X = (int)fx;

			fraction_X = fx - floor_X;
			//Shifted for integer calculation
			ifraction_x = (Fw16u)(fraction_X * weight_shift);
			ione_minus_x = (Fw16u)(weight_shift - ifraction_x);

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;
			for (int k=0;k<channel1;k++) {
				pIfx[x*channel1+k] = ifraction_x;
				pIofx[x*channel1+k]= ione_minus_x;			
			}
		}

		//Pre-calculate the y coefficient.
		for (y = ybegin; y < yend; y++)
		{
			double fy = (double)(y-ynewCenter)/yFactor+yCenter;
			floor_Y = (int)fy;

			fractY = fy - floor_Y;
			//Shifted for integer calculation
			ifraction_y  = (Fw16u)(fractY * weight_shift);
			ione_minus_y = (Fw16u)(weight_shift - ifraction_y);

			if (ifraction_y) {
				ceil_Y = floor_Y + 1;
			} else {
				ceil_Y = floor_Y;
			}
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=ifraction_y;	
			pY_Array_Value[y].ione_Minus_Val=ione_minus_y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
        getOptThreadNum(total_thread_num, srcRoi);

		FW_Resize_8u_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = (yend- ybegin)/total_thread_num;

			//seperate the region to be multi-threaded
			//along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			//For example, the last line of the first region will not have information
			//of the first line of the second region; and in the single thread case, the
			//image will be smoother along the region boundary.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = ybegin + threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=yend;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = xbegin;
				resizePar[threadNum].xEnd = xend;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;

				ThreadPool::Run(My_FW_Resize_8u_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			if (channel == 1) {
				//Special handling for 1 channel data, since we do not need to loop for channels
				My_FW_Resize_8u_LN_Op1(pSrc, pDst, dstStep, resizeWidth,  
					pX_Array_Value, pY_Array_Value, xbegin, xend, ybegin, yend, pIfx, pIofx);
			} else if (channel == 5) {					
				//channel 5 is for AC4, which is 4 channel.
				//AC4 Alpha channel data will not be touched
				My_FW_Resize_8u_LN_Op_AC4(pSrc, pDst, dstStep, resizeWidth, 
					pX_Array_Value, pY_Array_Value, xbegin, xend, ybegin, yend, pIfx, pIofx, 4);
			}else {
				My_FW_Resize_8u_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
					pX_Array_Value, pY_Array_Value, xbegin, xend, ybegin, yend, pIfx, pIofx, channel);
			}
		}

		//free the allocated buffer
		fwFree(pY_Array_Value);
		fwFree(pX_Array_Value);
		fwFree(pIfx);
		fwFree(pIofx);

		return fwStsNoErr;
	}

	//Internal function for resizeCenter unsigned short data with Linear interpolation
	static FwStatus My_FW_ResizeCenter_16u_Linear(const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
		Fw16u *pDst, int dstStep, FwiSize dstRoiSize, 
		double xFactor, double yFactor, double xCenter, double yCenter,
		int channel)
	{
		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		int x, y, k;//, channel;
		int xbegin, xend, ybegin, yend;
		int resizeWidth = dstRoiSize.width;
		int resizeHeight = dstRoiSize.height;
		double xnewCenter=(double)dstRoiSize.width/2;
		double ynewCenter=(double)dstRoiSize.height/2;

		//calculate the begin and end of x&y coordinate for source image 
		//corresponding to destination ROI.
		xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
		if (xend > dstRoiSize.width) xend=dstRoiSize.width;
		xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
		if (xbegin<0) xbegin=0;
		yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
		if (yend > dstRoiSize.height) yend=dstRoiSize.height;
		ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
		if (ybegin<0) ybegin=0;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw16u));
		srcStep = srcStep / (sizeof(Fw16u));

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		//use fwMalloc instead of malloc for aligned address
		Fw32f *pIfx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		Fw32f *pIofx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		Linear_Array_Float *pX_Array_Value = (Linear_Array_Float*) 
			fwMalloc(resizeWidth*sizeof(Linear_Array_Float));
		Linear_Array_Float *pY_Array_Value = (Linear_Array_Float*) 
			fwMalloc(resizeHeight*sizeof(Linear_Array_Float));

		Fw32f round;
		round=0.5;

		int ceil_X, ceil_Y, floor_X, floor_Y;
		double fraction_X, fractY, one_Minus_X, one_Minus_Y;

		//Pre-calculate the x coefficient.
		for (x = xbegin; x < xend; x++ ) {   
			double fx = (double)(x-xnewCenter)/xFactor + xCenter;
			floor_X = (int)fx;

			fraction_X = fx - floor_X;
			one_Minus_X = 1.0 - fraction_X;

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;
			pX_Array_Value[x].ifraction=fraction_X;
			pX_Array_Value[x].ione_Minus_Val=one_Minus_X;
			for(k=0 ; k<channel1; k++)
			{
				pIfx[x*channel1+k] = fraction_X;
				pIofx[x*channel1+k] = one_Minus_X;
			}
		}

		//Pre-calculate the y coefficient.
		for (y = ybegin; y < yend; y++)
		{
			double fy = (double)(y-ynewCenter)/yFactor+yCenter;
			floor_Y = (int)fy;

			fractY = fy - floor_Y;
			one_Minus_Y = 1.0 - fractY;

			ceil_Y = floor_Y + 1;
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=fractY;	
			pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
        getOptThreadNum(total_thread_num, srcRoi);

		////debug
		//total_thread_num=1;

		FW_Resize_16u_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = (yend- ybegin)/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = ybegin + threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=yend;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = xbegin;
				resizePar[threadNum].xEnd = xend;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;

				ThreadPool::Run(My_FW_Resize_16u_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			My_FW_Resize_16u_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
				pX_Array_Value, pY_Array_Value, xbegin, xend, ybegin, yend, pIfx, pIofx, channel);

		}

		return fwStsNoErr;
	}

	//Internal function for resizeCenter float data with Linear interpolation
	static FwStatus My_FW_ResizeCenter_32f_Linear(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
		Fw32f *pDst, int dstStep, FwiSize dstRoiSize, 
		double xFactor, double yFactor, double xCenter, double yCenter,
		int channel)
	{
		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		int x, y, k;//, channel;
		int xbegin, xend, ybegin, yend;
		int resizeWidth = dstRoiSize.width;
		int resizeHeight = dstRoiSize.height;
		double xnewCenter=(double)dstRoiSize.width/2;
		double ynewCenter=(double)dstRoiSize.height/2;

		//calculate the begin and end of x&y coordinate for source image 
		//corresponding to destination ROI.
		xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
		if (xend > dstRoiSize.width) xend=dstRoiSize.width;
		xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
		if (xbegin<0) xbegin=0;
		yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
		if (yend > dstRoiSize.height) yend=dstRoiSize.height;
		ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
		if (ybegin<0) ybegin=0;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw32f));
		srcStep = srcStep / (sizeof(Fw32f));

		//channel 5 is for AC4, which is 4 channel.
		//AC4 Alpha channel data will not be touched
		int channel1 = (channel == 5)? 4 :  channel;

		//use fwMalloc instead of malloc for aligned address
		Fw32f *pIfx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		Fw32f *pIofx = (Fw32f *) fwMalloc(resizeWidth*channel1*sizeof(Fw32f));
		Linear_Array_Float *pX_Array_Value = (Linear_Array_Float*) 
			fwMalloc(resizeWidth*sizeof(Linear_Array_Float));
		Linear_Array_Float *pY_Array_Value = (Linear_Array_Float*) 
			fwMalloc(resizeHeight*sizeof(Linear_Array_Float));

		Fw32f round;
		round=0.5;

		int ceil_X, ceil_Y, floor_X, floor_Y;
		double fraction_X, fractY, one_Minus_X, one_Minus_Y;

		//Pre-calculate the x coefficient.
		for (x = xbegin; x < xend; x++ ) {   
			double fx = (double)(x-xnewCenter)/xFactor + xCenter;
			floor_X = (int)fx;

			fraction_X = fx - floor_X;
			one_Minus_X = 1.0 - fraction_X;

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;

			pX_Array_Value[x].floor=floor_X;
			pX_Array_Value[x].ceil=ceil_X;
			pX_Array_Value[x].ifraction=fraction_X;
			pX_Array_Value[x].ione_Minus_Val=one_Minus_X;
			for(k=0 ; k<channel1; k++)
			{
				pIfx[(x-xbegin)*channel1+k] = fraction_X;
				pIofx[(x-xbegin)*channel1+k] = one_Minus_X;
			}
		}

		//Pre-calculate the y coefficient.
		for (y = ybegin; y < yend; y++)
		{
			double fy = (double)(y-ynewCenter)/yFactor+yCenter;
			floor_Y = (int)fy;

			fractY = fy - floor_Y;
			one_Minus_Y = 1.0 - fractY;

			ceil_Y = floor_Y + 1;
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;

			pY_Array_Value[y].floor=floor_Y*srcStep;
			pY_Array_Value[y].ceil=ceil_Y*srcStep;
			pY_Array_Value[y].ifraction=fractY;	
			pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
		}

		//Multi-thread setup
		ThreadPool::Init();
		int total_thread_num = ThreadPool::ThreadCount();
        getOptThreadNum(total_thread_num, srcRoi);
		FW_Resize_32f_Linear resizePar[MAX_THREADS];

		if(total_thread_num > 1) {
			int y_bgn, y_end; 
			int threadNum;

			if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
			int threadheight = (yend- ybegin)/total_thread_num;

			//seperate the region to be multi-threaded
			//Along the seperation line, there will be some in-accurate results 
			//comparing to the single thread answers.
			for(threadNum=0; threadNum<total_thread_num; threadNum++) {
				y_bgn   = ybegin + threadNum * threadheight; 
				y_end   = y_bgn + threadheight;
				//last thread handle all remaining area
				if (threadNum==total_thread_num-1) {
					y_end=yend;
				}

				//Pack the seperated data for multi-thread 
				resizePar[threadNum].pSrc = pSrc;
				resizePar[threadNum].pDst = pDst;
				resizePar[threadNum].dstStep = dstStep;
				resizePar[threadNum].resizeWidth = resizeWidth;
				resizePar[threadNum].pX_Array_Value = pX_Array_Value;
				resizePar[threadNum].pY_Array_Value = pY_Array_Value;
				resizePar[threadNum].xBegin = xbegin;
				resizePar[threadNum].xEnd = xend;
				resizePar[threadNum].yBegin = y_bgn;
				resizePar[threadNum].yEnd = y_end;
				resizePar[threadNum].pIfx = pIfx;
				resizePar[threadNum].pIofx = pIofx;
				resizePar[threadNum].channel = channel;

				ThreadPool::Run(My_FW_Resize_32f_LN_Thread, (void *) &resizePar[threadNum]);
			}
			ThreadPool::Wait();

		} else {
			//for single thread, no data packing
			My_FW_Resize_32f_LN_Op(pSrc, pDst, dstStep, resizeWidth, 
				pX_Array_Value, pY_Array_Value, xbegin, xend, ybegin, yend, pIfx, pIofx, channel);
		}

		//free allocated memory by fwMalloc
		fwFree(pY_Array_Value);
		fwFree(pX_Array_Value);
		fwFree(pIfx);
		fwFree(pIofx);

		return fwStsNoErr;
	}

	//Internal function for resizeCenter 8u data with Cubic interpolation
	static FwStatus My_FW_ResizeCenter_8u_C1R_CU(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
		Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, double xCenter, double yCenter, int /*channel*/)
	{
		//Adjusting source ROI
		if (srcRoi.x <0 ) {
			srcRoi.width +=srcRoi.x;
			srcRoi.x=0;
		}
		if (srcRoi.y <0 ) {
			srcRoi.height +=srcRoi.y;
			srcRoi.y=0;
		}
		if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
		if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

		int x, y;
		int xbegin, xend, ybegin, yend;
		int resizeWidth = dstRoiSize.width;
		int resizeHeight = dstRoiSize.height;

		double xnewCenter=(double)dstRoiSize.width/2;
		double ynewCenter=(double)dstRoiSize.height/2;

		//calculate the begin and end of x&y coordinate for source image
		//corresponding to destination ROI.
		xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
		if (xend > dstRoiSize.width) xend=dstRoiSize.width;
		xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
		if (xbegin<0) xbegin=0;
		yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
		if (yend > dstRoiSize.height) yend=dstRoiSize.height;
		ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
		if (ybegin<0) ybegin=0;
		short half_FW_WEIGHT_6 = 1<<(FW_WEIGHT_6-1) ;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(Fw8u));
		srcStep = srcStep / (sizeof(Fw8u));

		//use fwMalloc instead of malloc for aligned address
		Cubic_Array_Short *pX_Array_Value = (Cubic_Array_Short*) fwMalloc(resizeWidth 
			* sizeof(Cubic_Array_Short));
		Cubic_Array_Short *pY_Array_Value = (Cubic_Array_Short*) fwMalloc(resizeHeight 
			* sizeof(Cubic_Array_Short));
		Fw16s *pxCoeff = (Fw16s*) fwMalloc(resizeWidth*sizeof(Fw16s)*4*4); //4 xcoeff *4copies

		int l1, l2, r1, r2;
		//aA=-0.5 is picked for LancZos interpolation
		double aA=-0.5, weight_shift_6;
		int  iweight_shift_6 = 1<<FW_WEIGHT_6;
		weight_shift_6 = (double) iweight_shift_6 ;

		for (x = xbegin; x < xend; x++) {
			double fx = (double)(x-xnewCenter)/xFactor + xCenter;
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

				//Shifted for integer calculation
				pX_Array_Value[x].icoeff[i] = (Fw16s)(hh * weight_shift_6 );

				px += 1.0;
			}
			//protect overflow
			pX_Array_Value[x].icoeff[1] =  iweight_shift_6 - pX_Array_Value[x].icoeff[0]
			- pX_Array_Value[x].icoeff[2] - pX_Array_Value[x].icoeff[3];

		}

		for(x = xbegin; x <= xend - 2; x = x+2)
		{
			pxCoeff[x*4*4] = pxCoeff[x*4*4+1] = pxCoeff[x*4*4+2] = 
				pxCoeff[x*4*4+3] = pX_Array_Value[x].icoeff[0];
			pxCoeff[x*4*4+4] = pxCoeff[x*4*4+5] = pxCoeff[x*4*4+6] = 
				pxCoeff[x*4*4+7] = pX_Array_Value[x+1].icoeff[0];

			pxCoeff[x*4*4+8] = pxCoeff[x*4*4+9] = pxCoeff[x*4*4+10] = 
				pxCoeff[x*4*4+11] = pX_Array_Value[x].icoeff[1];
			pxCoeff[x*4*4+12] = pxCoeff[x*4*4+13] = pxCoeff[x*4*4+14] = 
				pxCoeff[x*4*4+15] = pX_Array_Value[x+1].icoeff[1];

			pxCoeff[(x+1)*4*4] = pxCoeff[(x+1)*4*4+1] = pxCoeff[(x+1)*4*4+2] = 
				pxCoeff[(x+1)*4*4+3] = pX_Array_Value[x].icoeff[2];
			pxCoeff[(x+1)*4*4+4] = pxCoeff[(x+1)*4*4+5] = pxCoeff[(x+1)*4*4+6] = 
				pxCoeff[(x+1)*4*4+7] = pX_Array_Value[x+1].icoeff[2];

			pxCoeff[(x+1)*4*4+8] = pxCoeff[(x+1)*4*4+9] = pxCoeff[(x+1)*4*4+10] = 
				pxCoeff[(x+1)*4*4+11] = pX_Array_Value[x].icoeff[3];
			pxCoeff[(x+1)*4*4+12] = pxCoeff[(x+1)*4*4+13] = pxCoeff[(x+1)*4*4+14] = 
				pxCoeff[(x+1)*4*4+15] = pX_Array_Value[x+1].icoeff[3];
		}

		// loop over all points of Y
		for (y = ybegin; y < yend; y++){
			double fy = (double)(y-ynewCenter)/yFactor+yCenter;
			l1 = (int)fy;
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

				//Shifted for integer calculation
				pY_Array_Value[y].icoeff[i] = (Fw16s)(hh * weight_shift_6 );

				py += 1.0;
			}
			//protect overflow
			pY_Array_Value[y].icoeff[1] =  iweight_shift_6 - pY_Array_Value[y].icoeff[0]
			- pY_Array_Value[y].icoeff[2] - pY_Array_Value[y].icoeff[3];

			pY_Array_Value[y].pos[0] = l2 * srcStep;
			pY_Array_Value[y].pos[1] = l1 * srcStep;
			pY_Array_Value[y].pos[2] = r1 * srcStep;
			pY_Array_Value[y].pos[3] = r2 * srcStep;
		}

		//Calculating the pixel value
		XMM128 pReg[4], xcoefReg[4], temp, ycoefReg, half_FW_WEIGHT_6_Reg;//, rReg, temp
		half_FW_WEIGHT_6_Reg.i  = _mm_set1_epi16(half_FW_WEIGHT_6);
		for (y = ybegin; y < yend; y++) {
			int ypos0 = pY_Array_Value[y].pos[0];
			int ypos1 = pY_Array_Value[y].pos[1];
			int ypos2 = pY_Array_Value[y].pos[2];
			int ypos3 = pY_Array_Value[y].pos[3];

			double iycoef0 = pY_Array_Value[y].icoeff[0];
			double iycoef1 = pY_Array_Value[y].icoeff[1];
			double iycoef2 = pY_Array_Value[y].icoeff[2];
			double iycoef3 = pY_Array_Value[y].icoeff[3];

			ycoefReg.i = _mm_set_epi16(	pY_Array_Value[y].icoeff[3], pY_Array_Value[y].icoeff[2], 
				pY_Array_Value[y].icoeff[1], pY_Array_Value[y].icoeff[0], pY_Array_Value[y].icoeff[3], 
				pY_Array_Value[y].icoeff[2], pY_Array_Value[y].icoeff[1], pY_Array_Value[y].icoeff[0]);

			for (x = xbegin; x <= xend-2; x= x + 2) {

				int &xpos00 = pX_Array_Value[x].pos[0];
				int &xpos01 = pX_Array_Value[x].pos[1];
				int &xpos02 = pX_Array_Value[x].pos[2];
				int &xpos03 = pX_Array_Value[x].pos[3];

				int &xpos10 = pX_Array_Value[x+1].pos[0];
				int &xpos11 = pX_Array_Value[x+1].pos[1];
				int &xpos12 = pX_Array_Value[x+1].pos[2];
				int &xpos13 = pX_Array_Value[x+1].pos[3];

				xcoefReg[0].i = _mm_load_si128((__m128i*)(&pxCoeff[x*4*4]));
				xcoefReg[1].i = _mm_load_si128((__m128i*)(&pxCoeff[x*4*4+8])); 

				xcoefReg[2].i = _mm_load_si128((__m128i*)(&pxCoeff[(x+1)*4*4]));
				xcoefReg[3].i = _mm_load_si128((__m128i*)(&pxCoeff[(x+1)*4*4+8]));

				pReg[0].i = _mm_set_epi16(pSrc[ypos3+ xpos10], pSrc[ypos2+ xpos10], pSrc[ypos1+ xpos10],pSrc[ypos0+ xpos10],
					pSrc[ypos3+ xpos00], pSrc[ypos2+ xpos00], pSrc[ypos1+ xpos00],pSrc[ypos0+ xpos00]);

				pReg[1].i = _mm_set_epi16(pSrc[ypos3+ xpos11], pSrc[ypos2+ xpos11], pSrc[ypos1+ xpos11],pSrc[ypos0+ xpos11],
					pSrc[ypos3+ xpos01], pSrc[ypos2+ xpos01], pSrc[ypos1+ xpos01],pSrc[ypos0+ xpos01]);					

				pReg[0].i = _mm_mullo_epi16(pReg[0].i, xcoefReg[0].i);
				pReg[1].i = _mm_mullo_epi16(pReg[1].i, xcoefReg[1].i);
				pReg[0].i = _mm_add_epi16(pReg[0].i, pReg[1].i);

				pReg[2].i = _mm_set_epi16(pSrc[ypos3+ xpos12], pSrc[ypos2+ xpos12], pSrc[ypos1+ xpos12],pSrc[ypos0+ xpos12],
					pSrc[ypos3+ xpos02], pSrc[ypos2+ xpos02], pSrc[ypos1+ xpos02],pSrc[ypos0+ xpos02]);

				pReg[3].i = _mm_set_epi16(pSrc[ypos3+ xpos13], pSrc[ypos2+ xpos13], pSrc[ypos1+ xpos13],pSrc[ypos0+ xpos13],
					pSrc[ypos3+ xpos03], pSrc[ypos2+ xpos03], pSrc[ypos1+ xpos03],pSrc[ypos0+ xpos03]);

				pReg[2].i = _mm_mullo_epi16(pReg[2].i, xcoefReg[2].i);
				pReg[3].i = _mm_mullo_epi16(pReg[3].i, xcoefReg[3].i);
				pReg[2].i = _mm_add_epi16(pReg[2].i, pReg[3].i);

				pReg[0].i = _mm_add_epi16(pReg[0].i, pReg[2].i);

				pReg[0].i = _mm_add_epi16(pReg[0].i, half_FW_WEIGHT_6_Reg.i);
				pReg[0].i = _mm_srli_epi16(pReg[0].i, FW_WEIGHT_6);

				pReg[0].i = _mm_mullo_epi16(pReg[0].i, ycoefReg.i);
				temp.i = _mm_srli_si128(pReg[0].i, 4);
				pReg[0].i = _mm_add_epi16(pReg[0].i, temp.i);
				temp.i = _mm_srli_si128(pReg[0].i, 2);
				pReg[0].i = _mm_add_epi16(pReg[0].i, temp.i);
				pReg[0].i = _mm_add_epi16(pReg[0].i, half_FW_WEIGHT_6_Reg.i);
				pReg[0].i = _mm_srli_epi16(pReg[0].i, FW_WEIGHT_6);

				temp.i = _mm_srli_si128(pReg[0].i, 8);
				temp.i = _mm_slli_si128(temp.i, 2);
				pReg[0].i = _mm_or_si128(pReg[0].i, temp.i);

				pReg[0].i = _mm_packus_epi16(pReg[0].i, pReg[0].i);

				*((Fw16u*)(pDst + y*dstStep + x)) = _mm_extract_epi16 (pReg[0].i, 0);
			}

			if(x < xend) {
				double p[4];
				int xpos0 = pX_Array_Value[x].pos[0];
				int xpos1 = pX_Array_Value[x].pos[1];
				int xpos2 = pX_Array_Value[x].pos[2];
				int xpos3 = pX_Array_Value[x].pos[3];

				double ixcoef0 = pX_Array_Value[x].icoeff[0];
				double ixcoef1 = pX_Array_Value[x].icoeff[1];
				double ixcoef2 = pX_Array_Value[x].icoeff[2];
				double ixcoef3 = pX_Array_Value[x].icoeff[3];

				//y-1 line
				p[0] = pSrc[ypos0+ xpos0];
				p[1] = pSrc[ypos0+ xpos1];
				p[2] = pSrc[ypos0+ xpos2];
				p[3] = pSrc[ypos0+ xpos3];
				double r0 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

				//y line
				p[0] = pSrc[ypos1+ xpos0];
				p[1] = pSrc[ypos1+ xpos1];
				p[2] = pSrc[ypos1+ xpos2];
				p[3] = pSrc[ypos1+ xpos3];
				double r1 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

				//y+1 line
				p[0] = pSrc[ypos2+ xpos0];
				p[1] = pSrc[ypos2+ xpos1];
				p[2] = pSrc[ypos2+ xpos2];
				p[3] = pSrc[ypos2+ xpos3];
				double r2 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

				//y+2 line
				p[0] = pSrc[ypos3+ xpos0];
				p[1] = pSrc[ypos3+ xpos1];
				p[2] = pSrc[ypos3+ xpos2];
				p[3] = pSrc[ypos3+ xpos3];
				double r3 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

				//saturate the result for corresponding data type
				*(pDst + y*dstStep + x) = FW_REF::Limits<Fw8u>::Sat(r0*iycoef0 
					+ r1*iycoef1 + r2*iycoef2 + r3*iycoef3 + 0.5);			

			}
		}
		
		//free allocated memory by fwMalloc
		fwFree(pxCoeff);

		return fwStsNoErr;
	}

	//Internal function for resize center parameter checking
	static FwStatus My_FW_ResizeCenterCheck(FwiSize srcSize, int srcStep, FwiRect srcRoi,
		int dstStep, FwiSize dstRoiSize, double xFactor, 
		double yFactor, double xCenter, double yCenter, int interpolation)//, int channel)
	{

		if (srcSize.height <= 0 || srcSize.width <= 0 || 
			dstRoiSize.height <= 0 || dstRoiSize.width<= 0 )	
			return fwStsSizeErr;	

		if (srcStep <= 0 || dstStep <= 0) 
			return fwStsStepErr;	

		if (xFactor <= 0.0 || yFactor <= 0.0)	
			return fwStsResizeFactorErr;	

		if (interpolation != FWI_INTER_NN && interpolation != FWI_INTER_LINEAR &&
			interpolation != FWI_INTER_CUBIC  && interpolation != FWI_INTER_LANCZOS && 
			interpolation != FWI_INTER_SUPER)	
			return fwStsInterpolationErr;

		if ((srcRoi.x + srcRoi.width -1) < 0 || (srcRoi.x >= srcSize.width) || 
			(srcRoi.y + srcRoi.height -1)< 0 || (srcRoi.y >= srcSize.height)||
			srcRoi.width < xCenter || srcRoi.height < yCenter)	
			return fwStsWrongIntersectROI;

		return fwStsNoErr;
	}

	namespace{
		//Internal general reference function for resizeCenter with all data types and all interpolation modes
		template< class TS, CH chSrc, DispatchType disp >
		static FwStatus My_FW_ResizeCenter(const TS * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
			TS* pDst, int dstStep, FwiSize dstRoiSize, 
			double xFactor, double yFactor, double xCenter, double yCenter,
			int interpolation)
		{
			//Adjusting source ROI
			if (srcRoi.x <0 ) {
				srcRoi.width +=srcRoi.x;
				srcRoi.x=0;
			}
			if (srcRoi.y <0 ) {
				srcRoi.height +=srcRoi.y;
				srcRoi.y=0;
			}
			if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
			if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

			int x, y, k, channel;
			int xbegin, xend, ybegin, yend;
			int resizeWidth = dstRoiSize.width;
			int resizeHeight = dstRoiSize.height;
			double xnewCenter=(double)dstRoiSize.width/2;
			double ynewCenter=(double)dstRoiSize.height/2;
			channel = ChannelCount(chSrc);

			//calculate the begin and end of x&y coordinate for source image 
			//corresponding to destination ROI.
			xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
			if (xend > dstRoiSize.width) xend=dstRoiSize.width;
			xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
			if (xbegin<0) xbegin=0;
			yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
			if (yend > dstRoiSize.height) yend=dstRoiSize.height;
			ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
			if (ybegin<0) ybegin=0;

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

			//to-compute....
			if (interpolation == FWI_INTER_NN) {
				int *px_NN, py_NN;

				px_NN=(int *)malloc(sizeof(int)*resizeWidth);

				// precalculate the x value 
				for (x=xbegin;x<xend;x++) {
					px_NN[x]=(int)((double)(x-xnewCenter)/xFactor+xCenter);
				}

				for (y=ybegin; y<yend;y++) {
					py_NN=(int)((double)(y-ynewCenter)/yFactor+yCenter);
					for (x=xbegin;x<xend;x++) {
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

				for (x=xbegin;x<xend;x++) {
					temp1 = ((double)(x-xnewCenter) / xFactor + xCenter);
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

					//point x-2
					xpos-=2;
					if (xpos <0) pX_Array_Value[x].pos[0] = srcRoi.x;
					else pX_Array_Value[x].pos[0] = srcRoi.x + xpos;

					//point x-1
					xpos++;
					if (xpos <0) pX_Array_Value[x].pos[1] = srcRoi.x;
					else pX_Array_Value[x].pos[1] = srcRoi.x + xpos;

					//point x+1
					xpos+=2;
					if (xpos >= srcRoi.width) {
						pX_Array_Value[x].pos[3] = pX_Array_Value[x].pos[2];
						pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[2];
						pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[2];
					} 
					else pX_Array_Value[x].pos[3] = srcRoi.x + xpos;

					//point x+2
					xpos++;
					if (xpos >= srcRoi.width) {
						pX_Array_Value[x].pos[4] = pX_Array_Value[x].pos[3];
						pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[3];
					} 
					else pX_Array_Value[x].pos[4] = srcRoi.x + xpos;

					//point x+3
					xpos++;
					if (xpos >= srcRoi.width) {
						pX_Array_Value[x].pos[5] = pX_Array_Value[x].pos[4];
					} 
					else pX_Array_Value[x].pos[5] = srcRoi.x + xpos;
				}

				for (y=ybegin; y<yend; y++) {
					temp1 = ((double)(y-ynewCenter) / yFactor+yCenter);
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

					//line y-2
					ypos-=2;
					if (ypos <0) pY_Array_Value[y].pos[0] = srcRoi.y * srcStep;
					else pY_Array_Value[y].pos[0] = (srcRoi.y + ypos) * srcStep;

					//line y-1
					ypos++;
					if (ypos <0) pY_Array_Value[y].pos[1] = srcRoi.y *srcStep;
					else pY_Array_Value[y].pos[1] = pY_Array_Value[y].pos[0] + srcStep;

					//line y+1
					ypos+=2;
					if (ypos >= srcRoi.height) {
						pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2];
						pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[2];
						pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[2];
					} 
					else pY_Array_Value[y].pos[3] = pY_Array_Value[y].pos[2] + srcStep;

					//line y+2
					ypos++;
					if (ypos >= srcRoi.height) {
						pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3];
						pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[3];
					} 
					else pY_Array_Value[y].pos[4] = pY_Array_Value[y].pos[3] + srcStep;

					//line y+3
					ypos++;
					if (ypos >= srcRoi.height) {
						pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4];
					} 
					else pY_Array_Value[y].pos[5] = pY_Array_Value[y].pos[4] + srcStep;
				}

				//Calculating the pixel value
				int typos[6], txpos[6];
				double iycoef[6], ixcoef[6];

				for (y=ybegin; y<yend; y++) {
					for (i=0;i<6;i++) {
						typos[i] = pY_Array_Value[y].pos[i];
						iycoef[i]= pY_Array_Value[y].icoeff[i];
					}
					for (x=xbegin;x<xend;x++) {
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
				int xsrcbegin, xsrcend, ysrcbegin, ysrcend;

				result = (double *)malloc(sizeof(double)*resizeWidth*resizeHeight*channel);

				//initialize pDst, so we could mul_add
				for (y=ybegin; y<yend; y++) {
					for (x=xbegin*channel;x<xend*channel;x++) {
						*(result+x+y*resizeWidth*channel)=0;
					}
				}

				// calculate the begin and end of x&y coordinate for source image 
				// corresponding to destination ROI.
				xsrcbegin = (int)((xbegin-xnewCenter)/xFactor+xCenter);
				if (xsrcbegin < srcRoi.x) xsrcbegin = srcRoi.x;
				ysrcbegin = (int)((ybegin-ynewCenter)/yFactor+yCenter);
				if (ysrcbegin < srcRoi.y) ysrcbegin = srcRoi.y;
				xsrcend   = (int)((xend-xnewCenter)/xFactor+xCenter);
				//data loading protection
				if (xsrcend > srcRoi.x + srcRoi.width - 1)
					xsrcend = srcRoi.x + srcRoi.width - 1;
				ysrcend   = (int)((yend-ynewCenter)/yFactor+yCenter);
				if (ysrcend > srcRoi.y + srcRoi.height - 1)
					ysrcend = srcRoi.y + srcRoi.height - 1;

				jpos=ybegin-1;
				for (y=ysrcbegin; y<=ysrcend; y++) {
					yFr=((double)(jpos+1-ynewCenter))/yFactor+yCenter-y;
					if (yFr>1) yFr=1;

					ipos=xbegin-1;
					for (x=xsrcbegin;x<=xsrcend;x++) {
						xFr=((double)(ipos+1-xnewCenter))/xFactor+xCenter-x;
						if (xFr>1) xFr=1; //Could pre-calculated

						for (k=0;k<channel1;k++) {
							if (ipos >= xbegin && jpos >=ybegin)
								*(result+(ipos+jpos*resizeWidth)*channel+k) 
								+= *(pSrc+y*srcStep+x*channel+k)*xFr*yFr;
							if (yFr<1 && (jpos+1) < resizeHeight && ipos >= xbegin) 
								*(result+(ipos+(jpos+1)*resizeWidth)*channel+k) += 
								*(pSrc+y*srcStep+x*channel+k)*xFr*(1-yFr);
							if (xFr<1 && (ipos+1) < resizeWidth && 
								yFr<1&& (jpos+1) < resizeHeight )
								*(result+((ipos+1)+(jpos+1)*resizeWidth)*channel+k) += 
								*(pSrc+y*srcStep+x*channel+k)*(1-xFr)*(1-yFr);
							if (xFr<1 && (ipos+1) < resizeWidth && jpos >=ybegin) 
								*(result+(ipos+1+jpos*resizeWidth)*channel+k) += 
								*(pSrc+y*srcStep+x*channel+k)*(1-xFr)*yFr;
						}
						if (xFr<1 && (ipos+1) < resizeWidth) ipos++;
					}

					if (yFr<1) jpos++;
				}

				//divide the weighted sum by Src/Dst=1/(xFactor*yFactor).
				yFr=xFactor*yFactor;
				for (y=ybegin; y<yend; y++) {
					for (x=xbegin;x<xend;x++) {
						for (k=0;k<channel1;k++) {
							//saturate the result for corresponding data type
							*(pDst+y*dstStep+x*channel+k) = 
								FW_REF::Limits<TS>::Sat(*(result+(x+y*resizeWidth)*channel+k) * yFr + round);
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

				for (x = xbegin; x < xend; x++) {
					double fx = (double)(x-xnewCenter)/xFactor + xCenter;
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
					- pX_Array_Value[x].icoeff[2] - pX_Array_Value[x].icoeff[3]; //for safety

				}

				// loop over all points of Y
				for (y = ybegin; y < yend; y++){
					double fy = (double)(y-ynewCenter)/yFactor+yCenter;
					l1 = (int)fy;
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

					pY_Array_Value[y].pos[0] = l2 * srcStep;
					pY_Array_Value[y].pos[1] = l1 * srcStep;
					pY_Array_Value[y].pos[2] = r1 * srcStep;
					pY_Array_Value[y].pos[3] = r2 * srcStep;
				}

				//Calculating the pixel value
				for (y = ybegin; y < yend; y++) {
					int ypos0 = pY_Array_Value[y].pos[0];
					int ypos1 = pY_Array_Value[y].pos[1];
					int ypos2 = pY_Array_Value[y].pos[2];
					int ypos3 = pY_Array_Value[y].pos[3];

					double iycoef0 = pY_Array_Value[y].icoeff[0];
					double iycoef1 = pY_Array_Value[y].icoeff[1];
					double iycoef2 = pY_Array_Value[y].icoeff[2];
					double iycoef3 = pY_Array_Value[y].icoeff[3];

					for (x = xbegin; x < xend; x++) {
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
							p[0] = pSrc[ypos0+ xpos0*channel+k];
							p[1] = pSrc[ypos0+ xpos1*channel+k];
							p[2] = pSrc[ypos0+ xpos2*channel+k];
							p[3] = pSrc[ypos0+ xpos3*channel+k];
							double r0 = p[0]*ixcoef0 + p[1]*ixcoef1 + p[2]*ixcoef2 + p[3]*ixcoef3;

							//y line
							p[0] = pSrc[ypos1+ xpos0*channel+k];
							p[1] = pSrc[ypos1+ xpos1*channel+k];
							p[2] = pSrc[ypos1+ xpos2*channel+k];
							p[3] = pSrc[ypos1+ xpos3*channel+k];
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
							*(pDst + y*dstStep + x*channel+k) = FW_REF::Limits<TS>::Sat(r0*iycoef0 
								+ r1*iycoef1 + r2*iycoef2 + r3*iycoef3 + round);
						}
					}
				}
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
				TS *pSrc_FloorY, *pSrc_CeilY;

				//Pre-calculate the x coefficient.
				for (x = xbegin; x < xend; x++ ) {   
					double fx = (double)(x-xnewCenter)/xFactor + xCenter;
					floor_X = (int)fx;

					fraction_X = fx - floor_X;
					one_Minus_X = 1.0 - fraction_X;

					//Protection for over-boundary reading
					ceil_X = floor_X + 1;
					if (ceil_X >= srcRoi.width) ceil_X = floor_X;

					pX_Array_Value[x].floor=floor_X;
					pX_Array_Value[x].ceil=ceil_X;
					pX_Array_Value[x].ifraction=fraction_X;
					pX_Array_Value[x].ione_Minus_Val=one_Minus_X;			
				}

				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++)
				{
					double fy = (double)(y-ynewCenter)/yFactor+yCenter;
					floor_Y = (int)fy;

					fractY = fy - floor_Y;
					one_Minus_Y = 1.0 - fractY;

					ceil_Y = floor_Y + 1;
					//Protection for over-boundary reading
					if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;

					pY_Array_Value[y].floor=floor_Y*srcStep;
					pY_Array_Value[y].ceil=ceil_Y*srcStep;
					pY_Array_Value[y].ifraction=fractY;	
					pY_Array_Value[y].ione_Minus_Val=one_Minus_Y;			
				}

				//Pre-calculate the y coefficient.
				for (y = ybegin; y < yend; y++) {
					pSrc_CeilY  = (TS*)pSrc;
					pSrc_FloorY = (TS*)pSrc;
					pSrc_CeilY  += pY_Array_Value[y].ceil;
					pSrc_FloorY += pY_Array_Value[y].floor;

					fractY = pY_Array_Value[y].ifraction;
					one_Minus_Y = pY_Array_Value[y].ione_Minus_Val;

					for (x=xbegin; x < xend; x++){			
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
								FW_REF::Limits<TS>::Sat(one_Minus_Y*t1 + fractY*t2 + round);
						}
					}
				}
			}

			return fwStsNoErr;
		}

	}

	//Nearest Neighbor interpolation functions
	namespace{
		//Internal function for resizeCenter 8u data with Nearest Neighbor interpolation
		FwStatus My_FW_ResizeCenter_8u_NN(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
			Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
			double xFactor, double yFactor, double xCenter, double yCenter, int channel)
		{
			//Adjusting source ROI
			if (srcRoi.x <0 ) {
				srcRoi.width +=srcRoi.x;
				srcRoi.x=0;
			}
			if (srcRoi.y <0 ) {
				srcRoi.height +=srcRoi.y;
				srcRoi.y=0;
			}
			if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
			if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

			int x, y;//, k;
			int xbegin, xend, ybegin, yend;
			int resizeWidth = dstRoiSize.width;
			int resizeHeight = dstRoiSize.height;
			// we think xCenter, yCenter means for both srcRoi coordinate
			// and dstROI cordinate
			double xnewCenter=(double)dstRoiSize.width/2;
			double ynewCenter=(double)dstRoiSize.height/2;

			//calculate the begin and end of x&y coordinate for source image
			//corresponding to destination ROI.
			xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
			if (xend > dstRoiSize.width) xend=dstRoiSize.width;
			xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
			if (xbegin<0) xbegin=0;
			yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
			if (yend > dstRoiSize.height) yend=dstRoiSize.height;
			ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
			if (ybegin<0) ybegin=0;

			//dstStep and srcStep are byte size
			//we need to change it with data array size
			dstStep = dstStep / (sizeof(Fw8u));
			srcStep = srcStep / (sizeof(Fw8u));

			int *px_NN, *py_NN;

			//use fwMalloc instead of malloc for aligned address
			px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
			py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

			// precalculate the x value 
			for (x=xbegin;x<xend;x++) {
				px_NN[x]=(int)((double)(x-xnewCenter)/xFactor+xCenter);
			}

			// precalculate the y value
			for (y=ybegin; y<yend;y++) {
				py_NN[y]=(int)((double)(y-ynewCenter)/yFactor+yCenter) * srcStep;
			}

			//Multi-thread setup
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
			FW_Resize_8u_NN resizePar[MAX_THREADS];

			if(total_thread_num > 1) {
				int y_bgn, y_end; 
				int threadNum;

				if(total_thread_num > MAX_THREADS) total_thread_num=MAX_THREADS;
                getOptThreadNum(total_thread_num, srcRoi);
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
					resizePar[threadNum].xBegin = xbegin;
					resizePar[threadNum].xEnd = xend;
					resizePar[threadNum].yBegin = ybegin;
					resizePar[threadNum].yEnd = yend;


					ThreadPool::Run(My_FW_Resize_8u_NN_Thread, (void *) &resizePar[threadNum]);
				}
				ThreadPool::Wait();

			} else {
				//for single thread, no data packing
				if (channel ==1) {
					//Special handling for 1 channel data, since we do not need to loop for channels
					My_FW_Resize_8u_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend);
				} else {
					My_FW_Resize_8u_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend, channel);
				}
			}

			//free allocated memory by fwMalloc
			fwFree(px_NN);
			fwFree(py_NN);

			return fwStsNoErr;
		}

		//Internal function for resizeCenter unsigned short data with Nearest Neighbor interpolation
		FwStatus My_FW_ResizeCenter_16u_NN(const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
			Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
			double xFactor, double yFactor, double xCenter, double yCenter, int channel)
		{
			//Adjusting source ROI
			if (srcRoi.x <0 ) {
				srcRoi.width +=srcRoi.x;
				srcRoi.x=0;
			}
			if (srcRoi.y <0 ) {
				srcRoi.height +=srcRoi.y;
				srcRoi.y=0;
			}
			if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
			if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

			int x, y;//, k;
			int xbegin, xend, ybegin, yend;
			int resizeWidth = dstRoiSize.width;
			int resizeHeight = dstRoiSize.height;
			// we think xCenter, yCenter means for both srcRoi coordinate
			// and dstROI cordinate
			double xnewCenter=(double)dstRoiSize.width/2;
			double ynewCenter=(double)dstRoiSize.height/2;

			//calculate the begin and end of x&y coordinate for source image 
			//corresponding to destination ROI.
			xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
			if (xend > dstRoiSize.width) xend=dstRoiSize.width;
			xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
			if (xbegin<0) xbegin=0;
			yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
			if (yend > dstRoiSize.height) yend=dstRoiSize.height;
			ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
			if (ybegin<0) ybegin=0;

			//dstStep and srcStep are byte size
			//we need to change it with data array size
			dstStep = dstStep / (sizeof(Fw16u));
			srcStep = srcStep / (sizeof(Fw16u));

			int *px_NN, *py_NN;

			//use fwMalloc instead of malloc for aligned address
			px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
			py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

			// precalculate the x value 
			for (x=xbegin;x<xend;x++) {
				px_NN[x]=(int)((double)(x-xnewCenter)/xFactor+xCenter);
			}

			// precalculate the y value
			for (y=ybegin; y<yend;y++) {
				py_NN[y]=(int)((double)(y-ynewCenter)/yFactor+yCenter) * srcStep;
			}

			//Multi-thread setup
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
            getOptThreadNum(total_thread_num, srcRoi);
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
					resizePar[threadNum].xBegin = xbegin;
					resizePar[threadNum].xEnd = xend;
					resizePar[threadNum].yBegin = ybegin;
					resizePar[threadNum].yEnd = yend;

					ThreadPool::Run(My_FW_Resize_16u_NN_Thread, (void *) &resizePar[threadNum]);
				}
				ThreadPool::Wait();

			} else {
				//for single thread, no data packing
				if (channel ==1) {
					//Special handling for 1 channel data, since we do not need to loop for channels
					My_FW_Resize_16u_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend);
				} else {
					My_FW_Resize_16u_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend, channel);
				}
			}

			//free allocated memory by fwMalloc
			fwFree(px_NN);
			fwFree(py_NN);

			return fwStsNoErr;
		}

		//Internal function for resizeCenter float data with Nearest Neighbor interpolation
		FwStatus My_FW_ResizeCenter_32f_NN(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
			Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
			double xFactor, double yFactor, double xCenter, double yCenter, int channel)
		{
			//Adjusting source ROI
			if (srcRoi.x <0 ) {
				srcRoi.width +=srcRoi.x;
				srcRoi.x=0;
			}
			if (srcRoi.y <0 ) {
				srcRoi.height +=srcRoi.y;
				srcRoi.y=0;
			}
			if (srcRoi.x+srcRoi.width>srcSize.width) srcRoi.width=srcSize.width-srcRoi.x;
			if (srcRoi.y+srcRoi.height>srcSize.height) srcRoi.height=srcSize.height-srcRoi.y;

			int x, y;
			int xbegin, xend, ybegin, yend;
			int resizeWidth = dstRoiSize.width;
			int resizeHeight = dstRoiSize.height;
			// we think xCenter, yCenter means for both srcRoi coordinate
			// and dstROI cordinate
			double xnewCenter=(double)dstRoiSize.width/2;
			double ynewCenter=(double)dstRoiSize.height/2;

			//calculate the begin and end of x&y coordinate for source image 
			//corresponding to destination ROI.
			xend = (int)(((double)srcRoi.width + srcRoi.x - xCenter)*xFactor+xnewCenter);
			if (xend > dstRoiSize.width) xend=dstRoiSize.width;
			xbegin = (int) ceil(xnewCenter-(xCenter-srcRoi.x)*xFactor);
			if (xbegin<0) xbegin=0;
			yend = (int)(((double)srcRoi.height + srcRoi.y - yCenter)*yFactor+ynewCenter);
			if (yend > dstRoiSize.height) yend=dstRoiSize.height;
			ybegin = (int) ceil(ynewCenter-(yCenter-srcRoi.y)*yFactor);
			if (ybegin<0) ybegin=0;

			//dstStep and srcStep are byte size
			//we need to change it with data array size
			dstStep = dstStep / (sizeof(Fw32f));
			srcStep = srcStep / (sizeof(Fw32f));

			int *px_NN, *py_NN;

			//use fwMalloc instead of malloc for aligned address
			px_NN=(int *)fwMalloc(sizeof(int)*resizeWidth);
			py_NN=(int *)fwMalloc(sizeof(int)*resizeHeight);

			// precalculate the x value 
			for (x=xbegin;x<xend;x++) {
				px_NN[x]=(int)((double)(x-xnewCenter)/xFactor+xCenter);
			}

			// precalculate the y value 
			for (y=ybegin; y<yend;y++) {
				py_NN[y]=(int)((double)(y-ynewCenter)/yFactor+yCenter) * srcStep;
			}

			//Multi-thread setup
			ThreadPool::Init();
			int total_thread_num = ThreadPool::ThreadCount();
            getOptThreadNum(total_thread_num, srcRoi);
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
					resizePar[threadNum].xBegin = xbegin;
					resizePar[threadNum].xEnd = xend;
					resizePar[threadNum].yBegin = ybegin;
					resizePar[threadNum].yEnd = yend;

					ThreadPool::Run(My_FW_Resize_32f_NN_Thread, (void *) &resizePar[threadNum]);
				}
				ThreadPool::Wait();

			} else {
				//for single thread, no data packing
				if (channel ==1) {
					//Special handling for 1 channel data, since we do not need to loop for channels
					My_FW_Resize_32f_NN_Op1(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend);
				} else {
					My_FW_Resize_32f_NN_Op(pSrc, srcStep, pDst, dstStep, resizeWidth, 
						px_NN, py_NN, xbegin, xend, ybegin, yend, channel);
				}
			}

			//free allocated memory by fwMalloc
			fwFree(px_NN);
			fwFree(py_NN);

			return fwStsNoErr;
		}

	}
}//end of namespace OPT_LEVEL

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C1R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter, int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_8u_LN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_8u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
		else if (interpolation == FWI_INTER_CUBIC) 
			return My_FW_ResizeCenter_8u_C1R_CU(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
	default:
		return My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C3R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter, int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_8u_LN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_8u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);
	default:
		return My_FW_ResizeCenter <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C4R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_8u_LN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 4);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_8u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 4);
	default:
		return My_FW_ResizeCenter <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_AC4R)(
	const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_8u_LN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 5);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_8u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 5);
	default:
		return My_FW_ResizeCenter <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C1R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_16u_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
		else if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_16u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
	default:
		return My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C3R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_16u_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);
		else if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_16u_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);

	default:
		return My_FW_ResizeCenter <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C4R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_16u_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 4);

	default:
		return My_FW_ResizeCenter <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_AC4R)(
	const Fw16u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_16u_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 5);

	default:
		return My_FW_ResizeCenter <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_32f_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);
		else if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_32f_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

	default:
		return My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
			return My_FW_ResizeCenter_32f_NN(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);
		else if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_32f_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 3);

	default:
		return My_FW_ResizeCenter <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_32f_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 4);

	default:
		return My_FW_ResizeCenter <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_LINEAR) 
			return My_FW_ResizeCenter_32f_Linear(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter, 5);

	default:
		return My_FW_ResizeCenter <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
	}
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_8u_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		//To be confirmed for Linear interpolation on Solaris system
		//else if (interpolation == FWI_INTER_LINEAR) 
		//{	status = My_FW_ResizeCenter_8u_LN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
		//dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

		//status = My_FW_ResizeCenter_8u_LN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
		//dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

		//status = My_FW_ResizeCenter_8u_LN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
		//dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

		//return status;
		//}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_16u_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		else if (interpolation == FWI_INTER_LINEAR) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_16u_Linear(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_Linear(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_Linear(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_32f_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		else if (interpolation == FWI_INTER_LINEAR) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_32f_Linear(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_Linear(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_Linear(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_8u_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_NN(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		else if (interpolation == FWI_INTER_LINEAR) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_8u_LN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_LN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_LN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_8u_LN(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw8u, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
			pDst[3], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_16u_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_NN(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		else if (interpolation == FWI_INTER_LINEAR) 
		{
			//expand the for loop
			status = My_FW_ResizeCenter_16u_Linear(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_Linear(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_Linear(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_16u_Linear(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw16u, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
			pDst[3], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFactor, double yFactor, double xCenter, double yCenter,  int interpolation)
{
	//parameter checking
	if (pSrc == 0 || pDst == 0) 
		return fwStsNullPtrErr;
	FwStatus status;
	status = My_FW_ResizeCenterCheck(srcSize, srcStep, srcRoi, dstStep, dstRoiSize, 
		xFactor, yFactor, xCenter, yCenter, interpolation);
	if (status!=fwStsNoErr) return status;

	switch( Dispatch::Type<DT_SSE2>() ) 
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation == FWI_INTER_NN) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_32f_NN(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_NN(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_NN(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_NN(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
		else if (interpolation == FWI_INTER_LINEAR) 
		{	
			//expand the for loop
			status = My_FW_ResizeCenter_32f_Linear(pSrc[0], srcSize, srcStep, srcRoi, pDst[0], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_Linear(pSrc[1], srcSize, srcStep, srcRoi, pDst[1], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_Linear(pSrc[2], srcSize, srcStep, srcRoi, pDst[2], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			status = My_FW_ResizeCenter_32f_Linear(pSrc[3], srcSize, srcStep, srcRoi, pDst[3], dstStep,
				dstRoiSize, xFactor, yFactor, xCenter, yCenter, 1);

			return status;
		}
	default:
		//expand the for loop
		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
			pDst[0], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
			pDst[1], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
			pDst[2], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);
		if (status != fwStsNoErr) return status;

		status = My_FW_ResizeCenter <Fw32f, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
			pDst[3], dstStep, dstRoiSize, xFactor, yFactor, xCenter, yCenter,  interpolation);

		return status;
	}
}

#endif //FW_BUILD_NUM >= 001


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
