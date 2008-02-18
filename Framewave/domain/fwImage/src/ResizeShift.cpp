/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

#if BUILD_NUM_AT_LEAST( 100 )

#pragma warning( disable: 4244 )

#ifndef __RESIZE_SHIFT
#define __RESIZE_SHIFT
#define	FW_WEIGHT	7	
#define	FW_WEIGHT_6	6	
#endif //__RESIZE_SHIFT guard

namespace OPT_LEVEL
{
	struct Linear_Array{
		int floor;
		int ceil;
		short ifraction;
		short ione_Minus_Val;
	};

	struct Cubic_Array{
		int pos[4];
		short  icoeff[4];
	};

	//General paramter checking with destination ROI fixing
	template< class TS>
	extern FwStatus My_FW_ParaCheck2(const TS*pSrc, FwiSize srcSize, int srcStep, 
		FwiRect srcRoi, TS*pDst, int dstStep, 
		FwiRect dstRoi, int channel);

	template< class TS>
	FwStatus My_FW_ParaCheck(const TS*pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS*pDst, int dstStep, FwiSize dstRoiSize, int channel);

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

	template< class TS, CH chSrc, DispatchType disp >
	extern FwStatus My_FW_Resize(const TS *pSrc, int srcStep, FwiRect srcRoi,	
		TS*pDst, int dstStep, FwiSize dstRoiSize,
		double xFactor, double yFactor, int interpolation);
	//  fwiResizeShift:
	//         Resize an image tile by xFactor and yFactor

	template <class TS>				  		  
	static SYS_INLINE FwStatus iResizeShift_C1R(const TS *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
		TS *pDst, int dstStep, FwiSize dstRoiSize,	
		double xFr, double yFr, double xShift, double yShift, int interpolation)	
	{	
		double fEnd_x,fEnd_y; 
		long   iStart_x, iEnd_x, iStart_y, iEnd_y; 
		int x=0,y=0;
		//short half_FW_WEIGHT = FW_WEIGHT/2 ;

		if (xFr <= 0.0 || yFr <= 0.0)	
			return fwStsResizeFactorErr;

		if (interpolation != FWI_INTER_LINEAR)	return fwStsInterpolationErr;	

		FwStatus status = My_FW_ParaCheck<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, 1);
		if (status !=fwStsNoErr) return status;

		if(xShift > 0.0) {
			iStart_x = 0;
			fEnd_x = (double)(srcRoi.width-xShift) / xFr;	
			if (fEnd_x <1) return fwStsWrongIntersectROI;

			if((double)dstRoiSize.width < fEnd_x)	
				iEnd_x = dstRoiSize.width;	
			else	
				iEnd_x = (int)fEnd_x;	
		} else {	
			iStart_x =(long)( xShift / xFr);	
			fEnd_x = (double)(srcRoi.width) / xFr;	
			if((double)dstRoiSize.width < fEnd_x)	
				iEnd_x = dstRoiSize.width;	
			else	
				iEnd_x = (int)fEnd_x;	
		}	

		if(yShift > 0.0) {	
			iStart_y = 0;	
			fEnd_y = (double)(srcRoi.height-yShift) / yFr;	
			if (fEnd_y <1) return fwStsWrongIntersectROI;

			if((double)dstRoiSize.height < fEnd_y)	
				iEnd_y = dstRoiSize.height;	
			else	
				iEnd_y = (int)fEnd_y;	
		} else {	
			iStart_y = (long)(yShift / yFr);	
			fEnd_y = (double)(srcRoi.width) / yFr;	
			if((double)dstRoiSize.height < fEnd_y)	
				iEnd_y = dstRoiSize.height;	
			else	
				iEnd_y = (int)fEnd_y;	
		}	

		//use fwMalloc instead of malloc for aligned address
		Linear_Array *pX_Array_Value = (Linear_Array*) fwMalloc(dstRoiSize.width*sizeof(Linear_Array));	
		Linear_Array *pY_Array_Value = (Linear_Array*) fwMalloc(dstRoiSize.height*sizeof(Linear_Array));	

		//resizeshift_pixel_mapping3(srcStep,srcRoi,pDst,dstStep,xFr,yFr,xShift,yShift,pX_Array_Value,
		// pY_Array_Value,iStart_x,iEnd_x,iStart_y,iEnd_y);	
		double fraction_X, fractY, one_Minus_X, one_Minus_Y, weight_shift, srcRoix, srcRoiy;	
		int ceil_X, ceil_Y, floor_X, floor_Y;	
		int ifraction_x, ifraction_y, ione_minus_x, ione_minus_y;	

		srcRoix = (double)srcRoi.x + xShift;	
		srcRoiy = (double)srcRoi.y + yShift;	

		weight_shift = (double) (1<<FW_WEIGHT) ;	

		//Pre-calculate the y coefficient.
		for (y = iStart_y; y < iEnd_y; y++)	
		{	
			floor_Y = (int)floor((double)y * yFr);	
			ceil_Y = floor_Y + 1;
			//Protection for over-boundary reading
			if (ceil_Y >= srcRoi.height) ceil_Y = floor_Y;	

			fractY = y*yFr - floor_Y;	
			one_Minus_Y = 1.0 - fractY;	

			//Shifted for integer calculation
			ifraction_y  = (int)(fractY * weight_shift);	
			ione_minus_y = (int)(one_Minus_Y * weight_shift);	

			floor_Y = (int)((srcRoiy + (double)floor_Y)*(double)srcStep + 0.5);	
			ceil_Y  = (int)((srcRoiy + (double)ceil_Y)*(double)srcStep +0.5);	

			pY_Array_Value[y].floor=floor_Y;	
			pY_Array_Value[y].ceil=ceil_Y;	
			//		pY_Array_Value[y].fraction=fractY;	
			//		pY_Array_Value[y].one_minus_val=one_Minus_Y;	
			pY_Array_Value[y].ifraction=(short)ifraction_y;	
			pY_Array_Value[y].ione_Minus_Val=(short)ione_minus_y;			
		}    

		//Pre-calculate the x coefficient.
		for (x = iStart_x; x < iEnd_x; x++)	
		{	
			floor_X = (int)floor((double)x * xFr);	

			//Protection for over-boundary reading
			ceil_X = floor_X + 1;	
			if (ceil_X >= srcRoi.width) ceil_X = floor_X;	

			fraction_X = x*xFr - floor_X;	
			one_Minus_X = 1.0 - fraction_X;	

			//Shifted for integer calculation
			ifraction_x  = (int)(fraction_X * weight_shift);	
			ione_minus_x = (int)(one_Minus_X * weight_shift);	

			floor_X = (int)((srcRoix + (double)floor_X) + 0.5);	
			ceil_X  = (int)((srcRoix + (double)ceil_X) + 0.5);	

			pX_Array_Value[x].floor=floor_X;	
			pX_Array_Value[x].ceil=ceil_X;	
			//		pX_Array_Value[x].fraction=fraction_X;	
			//		pX_Array_Value[x].one_minus_val=one_Minus_X;	
			pX_Array_Value[x].ifraction=(short)ifraction_x;	
			pX_Array_Value[x].ione_Minus_Val=(short)ione_minus_x;	
		}	


		//	stat_from_interpolation = resizeshift_interpolation_func(pSrc,srcRoi,pDst,dstStep,dstRoiSize,	
		//xFr,yFr,xShift,yShift,pX_Array_Value,pY_Array_Value,iStart_x,iEnd_x,iStart_y,iEnd_y);	

		//This function will be used for Fw8u type only.
		if (sizeof(TS) != 1) return fwStsErr;
		else //if(sizeof(TYPE) == 1)	// if TYPE == Fw8u
		{
			unsigned char p1, p2, p3, p4, t1, t2;	

			//__m128i rxmm7;	
			//rxmm7  = _mm_set1_epi8(0);				

			//int tempval_width=dstRoiSize.width%16;	
			//for (y = 0; y < dstRoiSize.height; y++)	
			//{	
			//	
			//	if(dstRoiSize.width>=16)	
			//	{	
			//		for (x = 0; x < dstRoiSize.width-tempval_width; x+=16)		
			//		   _mm_storeu_si128  ((__m128i *)(pDst + x + y*dstStep),rxmm7);	

			//		for (;x < dstRoiSize.width; x++) *(pDst + x + y*dstStep) = 0;	
			//	}	
			//	else	
			//	{	
			//		for (x=0;x < dstRoiSize.width; x++) *(pDst + x + y*dstStep) = 0;	
			//	}	
			//}	

			__m128i rxmm0 , rxmm1, rxmm2, rxmm3, rxmm4, rxmm5, rxmm6, rxmm7, rxmm8;	
			XMM128 pp1={0}, pp2={0}, pp3={0}, pp4={0};	
			XMM128 pIfx={0}, pIofx={0};	
			Fw8u *pSrc_FloorY;	
			Fw8u *pSrc_CeilY;	
			short half_FW_WEIGHT = FW_WEIGHT/2 ;	

			rxmm8  = _mm_set1_epi16(half_FW_WEIGHT);								

			for (y = iStart_y; y < iEnd_y; y++)	
			{		
				pSrc_CeilY   = (Fw8u*)pSrc;	
				pSrc_FloorY  = (Fw8u*)pSrc;	
				pSrc_CeilY  += pY_Array_Value[y].ceil;	
				pSrc_FloorY += pY_Array_Value[y].floor;	

				ifraction_y  = pY_Array_Value[y].ifraction;	
				ione_minus_y = pY_Array_Value[y].ione_Minus_Val;	

				rxmm0  = _mm_set1_epi16((short)ione_minus_y);                         
				rxmm7  = _mm_set1_epi16((short)ifraction_y);                         

				if((iEnd_x-iStart_x)>=8)	
				{	
					for (x = iStart_x; x <= iEnd_x-8; x+=8)	// process 8 pixels in parallel
					{	
						for (int xx = 0; xx < 8; xx++)	// process 8 pixels in parallel	
						{	
							ceil_X		 = pX_Array_Value[xx+x].ceil;	
							floor_X		 = pX_Array_Value[xx+x].floor;	
							ifraction_x  = pX_Array_Value[xx+x].ifraction;	
							ione_minus_x = pX_Array_Value[xx+x].ione_Minus_Val;	

							pp1.u16[xx] = (unsigned short)(*(pSrc_FloorY + floor_X));	
							pp2.u16[xx] = (unsigned short)(*(pSrc_FloorY + ceil_X));		
							pp3.u16[xx] = (unsigned short)(*(pSrc_CeilY  + floor_X));	
							pp4.u16[xx] = (unsigned short)(*(pSrc_CeilY  + ceil_X));		

							pIfx.u16[xx] = (unsigned short)ifraction_x;	
							pIofx.u16[xx]= (unsigned short)ione_minus_x;	
						}	

						rxmm1 = _mm_load_si128(&pp1.i);	
						rxmm2 = _mm_load_si128(&pp2.i);	
						rxmm3 = _mm_load_si128(&pp3.i);	
						rxmm4 = _mm_load_si128(&pp4.i);	

						rxmm5 = _mm_load_si128(&pIfx.i);	// ifraction_x 	
						rxmm6 = _mm_load_si128(&pIofx.i);	// ione_minus_x		

						// resize	
						// t1 = (unsigned char)((ione_minus_x *p1 + ifraction_x *p2) >> FW_WEIGHT); 
						rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm6);	//  ione_minus_x *p1 	
						rxmm2 = _mm_mullo_epi16 (rxmm2, rxmm5);	//  ifraction_x  *p2		
						rxmm1 = _mm_add_epi16(rxmm1, rxmm2);	
						rxmm1 = _mm_add_epi16(rxmm1, rxmm8);	
						rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);	

						//  t2 = (unsigned char)((ione_minus_x *p3 + ifraction_x *p4) >> FW_WEIGHT); 	
						rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm6);	//  ione_minus_x *p1 	
						rxmm4 = _mm_mullo_epi16 (rxmm4, rxmm5);	//  ifraction_x  *p2 	
						rxmm3 = _mm_add_epi16(rxmm3, rxmm4);            
						rxmm3 = _mm_add_epi16(rxmm3, rxmm8);            
						rxmm3 = _mm_srli_epi16(rxmm3, FW_WEIGHT);	


						// *(pDst + x + y*dstStep) = (unsigned char)((ione_minus_y *t1 + ifraction_y * t2) >> FW_WEIGHT)	
						rxmm1 = _mm_mullo_epi16 (rxmm1, rxmm0);	//  ione_minus_y * t1 	
						rxmm3 = _mm_mullo_epi16 (rxmm3, rxmm7);	//  ifraction_y  * t2		
						rxmm1 = _mm_add_epi16(rxmm1, rxmm3);	
						rxmm1 = _mm_add_epi16(rxmm1, rxmm8);	
						rxmm1 = _mm_srli_epi16(rxmm1, FW_WEIGHT);	

						rxmm1 = _mm_packus_epi16(rxmm1, rxmm1);		//  convert to 8 bit 	

						_mm_storel_epi64((__m128i *)(pDst + x + y*dstStep), rxmm1);	
					}	

					for (; x < iEnd_x; x++)	//  for remaining pixels 
					{	

						ceil_X=pX_Array_Value[x].ceil;	
						floor_X=pX_Array_Value[x].floor;	
						ifraction_x = pX_Array_Value[x].ifraction;	
						ione_minus_x = pX_Array_Value[x].ione_Minus_Val;	

						p1 = *(pSrc_FloorY + floor_X);
						p2 = *(pSrc_FloorY + ceil_X);
						p3 = *(pSrc_CeilY  + floor_X);
						p4 = *(pSrc_CeilY  + ceil_X);

						// ione_minus_x and ifraction_x value has been shifted by FW_WEIGHT, but no sturation is needed
						// + half_FW_WEIGHT for rounding
						t1 = (Fw8u)((ione_minus_x *p1 + ifraction_x *p2 + half_FW_WEIGHT) >> FW_WEIGHT); 
						t2 = (Fw8u)((ione_minus_x *p3 + ifraction_x *p4 + half_FW_WEIGHT) >> FW_WEIGHT); 

						*(pDst + x + y*dstStep) = (Fw8u)((ione_minus_y*t1 + ifraction_y*t2 + half_FW_WEIGHT) >> FW_WEIGHT); 
					}
				} 
				else 
				{ 
					for (x = iStart_x; x < iEnd_x; x++)	//  for remaining pixels	
					{	
						ceil_X=pX_Array_Value[x].ceil;	
						floor_X=pX_Array_Value[x].floor;	
						ifraction_x = pX_Array_Value[x].ifraction;	
						ione_minus_x = pX_Array_Value[x].ione_Minus_Val;	

						p1 = *(pSrc_FloorY + floor_X);
						p2 = *(pSrc_FloorY + ceil_X);
						p3 = *(pSrc_CeilY  + floor_X);
						p4 = *(pSrc_CeilY  + ceil_X);

						// ione_minus_x and ifraction_x value has been shifted by FW_WEIGHT, but no sturation is needed
						// + half_FW_WEIGHT for rounding
						t1 = (Fw8u)((ione_minus_x *p1 + ifraction_x *p2 + half_FW_WEIGHT) >> FW_WEIGHT);
						t2 = (Fw8u)((ione_minus_x *p3 + ifraction_x *p4 + half_FW_WEIGHT) >> FW_WEIGHT);

						*(pDst + x + y*dstStep) = (Fw8u)((ione_minus_y*t1 + ifraction_y*t2 + half_FW_WEIGHT) >> FW_WEIGHT);
					}
				}
			}
		}	

		//free allocated memory by fwMalloc
		fwFree(pX_Array_Value);	
		fwFree(pY_Array_Value);	

		return fwStsNoErr;	
	}	

#if BUILD_NUM_AT_LEAST( 9999 )
	FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_C1IRSfs)(
		const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
		Fw16u *pDst, int dstStep, FwiSize dstRoiSize,	
		double xFr, double yFr, double xShift, double yShift, int interpolation)	
	{
		return iResizeShift_C1R <Fw16u> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation) ;

	}
#endif

	//generic function call for fwiResizeShift
	template< class TS, CH chSrc, DispatchType disp >
	static FwStatus My_FW_ResizeShift(const TS *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
		TS*pDst, int dstStep, FwiSize dstRoiSize,
		double xFr, double yFr, double xShift, double yShift, int interpolation)
	{
		if (xFr <= 0.0 || yFr <= 0.0)	
			return fwStsResizeFactorErr;	

		if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
			interpolation != FWI_INTER_CUBIC  && interpolation != FWI_INTER_LANCZOS)	
			return fwStsInterpolationErr;

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoiSize, channel);
		if (status !=fwStsNoErr) return status;

		FwiRect newSrcRoi;

		//Shift is in the opposite direction, + means shift right
		// - means shift left from source point of view
		if(xShift > 0.0) {
			newSrcRoi.x=srcRoi.x + (int) xShift;
			newSrcRoi.width = (int) (srcRoi.width-(int)xShift);	
		} else {	
			newSrcRoi.x=srcRoi.x;
			newSrcRoi.width = (int) (srcRoi.width+(int)xShift);
		}
		if (newSrcRoi.width < 1) return fwStsWrongIntersectROI;

		if(yShift > 0.0) {
			newSrcRoi.y=srcRoi.y + (int) yShift;
			newSrcRoi.height = (int) (srcRoi.height-(int)yShift);	
		} else {	
			newSrcRoi.y=srcRoi.y;
			newSrcRoi.height = (int) (srcRoi.height+(int)yShift);
		}
		if (newSrcRoi.height < 1) return fwStsWrongIntersectROI;

		return My_FW_Resize<TS, chSrc, disp> (pSrc, srcStep, newSrcRoi, 
			pDst, dstStep, dstRoiSize, ((double)1.0/xFr), ((double)1.0/yFr), interpolation);
	}


	//Description
	//The function fwiResizeSqrPixel is declared in the fwi.h file. It operates with Roi (see
	//Roi Processing in Geometric Transforms).
	//This function resizes the source image Roi by xFactor in the x direction and yFactor in the y
	//direction. The image size can be either reduced or increased in each direction, depending on the
	//values of xFactor, yFactor. The result is resampled using the interpolation method specified
	//by the interpolation parameter, and written to the destination image Roi.
	//Unlike fwiResize, this function uses different algorithm for interpolation.
	//Pixel coordinates x? and y? in the resized image are obtained from the following equations:
	//x? = xFactor*x + xShift
	//y? = yFactor*y + yShift
	//where x and y denote the pixel coordinates in the source image.
	//The function requires the external buffer pBuffer, its size can be previously computed by calling
	//the function fwiResizeSqrPixelGetBufSize.

	//handle each point individually
	template< class TS, DispatchType disp >
	extern void My_FW_PointHandle(double xmap, double ymap, int x, int y,
		const TS*pSrc, int srcStep, FwiRect srcRoi,
		TS*pDst, int dstStep, int interpolation, int *flag, 
		int channel, int channel1, Fw32f round);

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_ResizeSqrPixel(const TS *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS*pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
		double xShift, double yShift, int interpolation, Fw8u *pBuffer)
	{
		// doesn't see the need for pBuffer, should we check?
		// if (pBuffer==0) return fwStsNullPtrErr;
		pBuffer;

		if (xFactor <= 0.0 || yFactor <= 0.0)	
			return fwStsResizeFactorErr;	

		if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
			interpolation != FWI_INTER_CUBIC && interpolation != FWI_INTER_LANCZOS)	
			return fwStsInterpolationErr;

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		// use WarpAffine to handle points
		// This is not an optimized version

		double xmap, ymap;
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
			ymap = (y-yShift)/yFactor;

			for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++) {
				xmap = (x-xShift)/xFactor;

				My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
					pDst, dstStep, interpolation, &flag, channel, channel1, round);
			}
		}

		//if no point is handled, return warning
		if (flag==0) return fwStsWrongIntersectQuad;

		return fwStsNoErr;
	}

template< class TS, CH chSrc,  DispatchType disp >
	FwStatus My_FW_ResizeSqrPixel_SSE2(const TS *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS*pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
		double xShift, double yShift, int interpolation, Fw8u *pBuffer)
	{
		// doesn't see the need for pBuffer, should we check?
		// if (pBuffer==0) return fwStsNullPtrErr;
		pBuffer;

		if (xFactor <= 0.0 || yFactor <= 0.0)	
			return fwStsResizeFactorErr;	

		if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
			interpolation != FWI_INTER_CUBIC && interpolation != FWI_INTER_LANCZOS)	
			return fwStsInterpolationErr;

		int channel=ChannelCount(chSrc);
		FwStatus status = My_FW_ParaCheck2<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
			dstRoi, channel);
		if (status !=fwStsNoErr) return status;

		
		//double xmap, ymap;
		int x, y, flagX=0, flagY=0;

		//dstStep and srcStep are byte size
		//we need to change it with data array size
		dstStep = dstStep / (sizeof(TS));
		srcStep = srcStep / (sizeof(TS));

		//int channel1;
		// Will not change 4th channel element in AC4
		/*if (chSrc == AC4) channel1=3;
		else channel1=channel;*/
		//Fw32f round;
		// 32f is supported, but not 32u and 32s
		// No rounding is needed for 32f type
		//if (sizeof(TS) == 4) round=0;
		//else round=0.5;

		int* XMM_xmap = (int*) fwMalloc((dstRoi.width + dstRoi.x) * sizeof(int));
		int* XMM_ymap = (int*) fwMalloc((dstRoi.height + dstRoi.y) * sizeof(int));

        int newDstX = dstRoi.x, newDstWidth = dstRoi.width;
        int newDstY = dstRoi.y, newDstHeight = dstRoi.height;

		for (y=dstRoi.y; y<(dstRoi.y+dstRoi.height); y++)
        {
            int ymap = (y-yShift)/yFactor;
            if (ymap < 0)
            {   
                newDstY = y + 1;
                continue;
            }
            if (ymap > srcRoi.height - 1)
            {
                newDstHeight = y - 1;
                break;
            }
            XMM_ymap[y] = (int)ymap + srcRoi.y;
            flagY = 1;
        }

        
		for (x=dstRoi.x; x<(dstRoi.x+dstRoi.width);x++)
        {
            int xmap = (x-xShift)/xFactor;
            if (xmap < 0)
            {   
                newDstX = x + 1;
                continue;
            }
            if (xmap > srcRoi.width - 1)
            {
                newDstWidth = x - 1;
                break;
            }
            XMM_xmap[x] = xmap+ srcRoi.x;
            flagX = 1;
        }

        XMM128 pp={0};
		for (y=newDstY; y<(dstRoi.y+newDstHeight); y++) {
			//ymap = (y-yShift)/yFactor;
            int &yint = XMM_ymap[y];
            int y_dstStep = y * dstStep;
            int yint_srcStep = yint*srcStep;
			for (x=newDstX; x<(dstRoi.x+newDstWidth)-16; x = x + 16) {
            //for (x=newDstX; x<(dstRoi.x+newDstWidth); x++ ) {
                //_mm_prefetch((char*)(XMM_xmap + x), _MM_HINT_T1);
                //_mm_prefetch((char*)(XMM_xmap + x + 4), _MM_HINT_T1);
                //_mm_prefetch((char*)(XMM_xmap + x + 8), _MM_HINT_T1);
                //_mm_prefetch((char*)(XMM_xmap + x + 12), _MM_HINT_T1);
				for(int xx = 0; xx < 16; xx++)
                { 
                    int xint = (int)XMM_xmap[x + xx];
                    //int xint = (int)XMM_xmap[x];
			        //*(pDst+y_dstStep+x ) = *(pSrc+ yint_srcStep+xint);
					pp.u8[xx] = *(pSrc+ yint_srcStep+xint);
                }
               _mm_storeu_si128((__m128i *)(pDst+y_dstStep+x), pp.i);
			}
            for (; x<(dstRoi.x+newDstWidth); x++ )
            {
                int &xint = XMM_xmap[x];
                *(pDst+y_dstStep+x ) = *(pSrc+ yint_srcStep+xint);
            }
		}
        //if no point is handled, return warning
		if (flagX == 0 || flagY == 0) return fwStsWrongIntersectQuad; //todo

		return fwStsNoErr;
	}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;
//  fwiGetResizeFract: 
//         Recalculate resize factors for tiled image processing

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetResizeFract)(
	FwiSize srcSize, FwiRect srcRoi, double xFactor, double yFactor,
	double *xFr, double *yFr, int interpolation)
{
	if (xFr == 0 || yFr == 0) return fwStsNullPtrErr;

	if (srcSize.height <= 0 || srcSize.width <= 0 || 
		srcRoi.width <=0 || srcRoi.height <=0)	
		return fwStsSizeErr;	

	if (xFactor <= 0.0 || yFactor <= 0.0)	
		return fwStsResizeFactorErr;	

	if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
		interpolation != FWI_INTER_CUBIC  && interpolation != FWI_INTER_LANCZOS)	
		return fwStsInterpolationErr;	

	if ((srcRoi.x + srcRoi.width -1) < 0 || (srcRoi.x >= srcSize.width) || 
		(srcRoi.y + srcRoi.height -1)< 0 || (srcRoi.y >= srcSize.height))	
		return fwStsWrongIntersectROI;

	if (interpolation == FWI_INTER_NN) {
		*xFr = 1.0/xFactor;
		*yFr = 1.0/yFactor;
	} else {
		int width, height;
		if (srcRoi.x + srcRoi.width > srcSize.width)
			width = srcSize.width - srcRoi.x;
		else width = srcRoi.width;

		if (srcRoi.y + srcRoi.height > srcSize.height)
			height = srcSize.height - srcRoi.y;
		else height = srcRoi.height;

		double dwidth = xFactor * width -1;
		if (dwidth <= 0) *xFr = 0;
		else *xFr = ((double)width-1.0)/dwidth;
		double dheight = yFactor * height -1;
		if (dheight <= 0) *yFr = 0;
		else *yFr = ((double)height -1.0)/dheight;
	}
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C1R )(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
														  Fw8u *pDst, int dstStep, FwiSize dstRoiSize,	
														  double xFr, double yFr, double xShift, double yShift, int interpolation)	
{
	switch( Dispatch::Type<DT_SSE2>() )
	{
	case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_LINEAR && xShift>0 && yShift>0) 
			return iResizeShift_C1R <Fw8u>  (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	default:
		return My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi,
			pDst, dstStep,dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C3R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
														 Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
														 double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
														 Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
														 double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C1R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C3R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw16u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw16u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_AC4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw16u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *pDst, int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	return My_FW_ResizeShift <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[3], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw8u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw8u, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
		pDst[3], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw16u *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw16u, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
		pDst[3], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,	
	Fw32f *const pDst[4], int dstStep, FwiSize dstRoiSize,
	double xFr, double yFr, double xShift, double yShift, int interpolation)
{
	FwStatus status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[0], srcSize, srcStep, srcRoi, 
		pDst[0], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[1], srcSize, srcStep, srcRoi, 
		pDst[1], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[2], srcSize, srcStep, srcRoi, 
		pDst[2], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);
	if (status != fwStsNoErr) return status;

	status = My_FW_ResizeShift <Fw32f, C1, DT_REFR> (pSrc[3], srcSize, srcStep, srcRoi, 
		pDst[3], dstStep, dstRoiSize, xFr, yFr, xShift, yShift, interpolation);

	return status;
}

// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C1R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	switch( Dispatch::Type<DT_SSE2>() )
    //switch( Dispatch::Type() )
	{
    case DT_SSE3:
	case DT_SSE2:
		if (interpolation==FWI_INTER_NN) 
			return My_FW_ResizeSqrPixel_SSE2<Fw8u, C1, DT_SSE2>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
			
	default: 
	        return My_FW_ResizeSqrPixel<Fw8u, C1, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
	}
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C3R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw8u, C3, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw8u, C4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_AC4R)(
	const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw8u, AC4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

// unsigned short data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C1R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw16u, C1, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

// unsigned short data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C3R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw16u, C3, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

// unsigned short data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw16u, C4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

// unsigned short data type with 4(3+alpha) channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_AC4R)(
	const Fw16u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw16u, AC4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C1R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw32f, C1, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C3R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw32f, C3, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw32f, C4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_AC4R)(
	const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	return My_FW_ResizeSqrPixel<Fw32f, AC4, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, xFactor,
		yFactor, xShift, yShift, interpolation, pBuffer);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_ResizeSqrPixel<Fw8u, C1, DT_REFR>(pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_ResizeSqrPixel<Fw8u, C1, DT_REFR>(
			pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_P3R)(
	const Fw16u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_ResizeSqrPixel<Fw16u, C1, DT_REFR>(pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//unsigned short data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_P4R)(
	const Fw16u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw16u *const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_ResizeSqrPixel<Fw16u, C1, DT_REFR>(pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_ResizeSqrPixel<Fw32f, C1, DT_REFR>(pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor, 
	double xShift, double yShift, int interpolation, Fw8u *pBuffer)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_ResizeSqrPixel<Fw32f, C1, DT_REFR>(pSrc[i], srcSize, srcStep, srcRoi, pDst[i], dstStep, dstRoi, xFactor,
			yFactor, xShift, yShift, interpolation, pBuffer);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}


//Description
//The function fwiResizeSqrPixelGetBufSize is declared in the fwi.h file. This function
//computes the size of the external buffer that is required for the function fwiResizeSqrPixel.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixelGetBufSize)(FwiSize dstSize, int nChannel,
															   int interpolation, int *pBufferSize)
{
	//This function is a place holder.
	if (pBufferSize == 0) return fwStsNullPtrErr;
	if (dstSize.height < 1 || dstSize.width < 1)
		return fwStsSizeErr;

	if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
		interpolation != FWI_INTER_CUBIC && interpolation != FWI_INTER_LANCZOS)	
		return fwStsInterpolationErr;

	if (nChannel != 1 && nChannel != 3 && nChannel != 4)
		return fwStsNumChannelsErr;

	int h16, w16;

	h16 = (dstSize.height-1)>>4;
	w16 = (dstSize.width-1)>>4;

	if (interpolation == FWI_INTER_NN) {
		*pBufferSize = (h16<<6) + (w16<<6) +143;
	} else if (interpolation == FWI_INTER_LINEAR) {
		int w8 = (dstSize.width -1) >>3;
		*pBufferSize = (h16<<7) + (w16<<7) + ((w8<<6)+ 128)*nChannel + 271;
	} else if (interpolation == FWI_INTER_CUBIC) {
		int w4 = (dstSize.width -1) >>2;
		*pBufferSize = (h16<<7) + (w16<<7) + ((w4<<6)+ 256)*nChannel + 271;
	} else {//interpolation == FWI_INTER_LANCZOS
		int wm8 = (dstSize.width - 1)&0x7;//(mod 8)
		int hm8 = (dstSize.height- 1)&0x7;//(mod 8)
		int w8  = (dstSize.width - 1)>>3;
		int h8  = (dstSize.height- 1)>>3;
		int wm83= (wm8+1)/3;
		int hm83= (hm8+1)/3;

		*pBufferSize = (h16<<7) + (w16<<7) + ((wm83<<6)+(w8<<6)*3)*(nChannel+1) + 
			(hm83<<6) + (h8<<6)*3 + 384*nChannel + 1039;
	}

	return fwStsNoErr;
}

#endif //FW_BUILD_NUM >= 001


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
