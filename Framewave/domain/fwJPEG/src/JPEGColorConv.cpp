/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the color conversion functions from JPEG Chapter
//	fwiRGBToY_JPEG_8u_P3C1R
//  fwiRGBToY_JPEG_8u_C3C1R
//	fwiBGRToY_JPEG_8u_C3C1R
//	fwiRGBToYCbCr_JPEG_8u_P3R
//	fwiRGBToYCbCr_JPEG_8u_C3P3R
//	fwiYCbCrToRGB_JPEG_8u_P3R
//	fwiYCbCrToRGB_JPEG_8u_P3C3R
//	fwiRGB565ToYCbCr_JPEG_16u8u_C3P3R
//	fwiRGB555ToYCbCr_JPEG_16u8u_C3P3R
//	fwiYCbCrToRGB565_JPEG_8u16u_P3C3R
//	fwiYCbCrToRGB555_JPEG_8u16u_P3C3R
//	fwiBGRToYCbCr_JPEG_8u_C3P3R
//	fwiYCbCrToBGR_JPEG_8u_P3C3R
//	fwiBGR565ToYCbCr_JPEG_16u8u_C3P3R
//	fwiBGR555ToYCbCr_JPEG_16u8u_C3P3R
//	fwiYCbCrToBGR565_JPEG_8u16u_P3C3R
//	fwiYCbCrToBGR555_JPEG_8u16u_P3C3R
//	fwiCMYKToYCCK_JPEG_8u_P4R
//	fwiCMYKToYCCK_JPEG_8u_C4P4R
//	fwiYCCKToCMYK_JPEG_8u_P4R
//	fwiYCCKToCMYK_JPEG_8u_P4C4R
//************************************************************************* 

#include "fwdev.h"
#include "fwJPEG.h"
#include "FwSharedCode_SSE2.h"
#include "JPEGColorConv.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

#ifndef __JPEGCOLORCONVERSION
#define __JPEGCOLORCONVERSION
#define STEPCHECK(X, Y) if (X<=0 || Y<=0) return fwStsStepErr
#define ROISIZECHECK(X) if (X.height <=0 || X.width <=0) return fwStsSizeErr

#define FWJ_LIMIT5(val) (((val)<=0)?0:(((val)>=0x1f)?0x1f:((unsigned short)(val))))
#define FWJ_LIMIT6(val) (((val)<=0)?0:(((val)>=0x3f)?0x3f:((unsigned short)(val))))
#endif

//-----------------------------------------------------------------------
//The function FwiRGBToY_JPEG is declared in the fwImage.h file. It operates 
//with ROI (see Regions of Interest in manual). This function converts the 
//RGB image to gray scale using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToY_JPEG_8u_P3C1R)(const Fw8u *pSrcRGB[3], int srcStep, 
								   Fw8u *pDstY, int dstStep, FwiSize roiSize)
{
	if (pSrcRGB == 0 || pDstY == 0) return fwStsNullPtrErr;
	if (pSrcRGB[0] == 0 || pSrcRGB[1] == 0 || pSrcRGB[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			result = 77 * pSrcRGB[0][srcPos] + 150 * pSrcRGB[1][srcPos]
				+ 29 * pSrcRGB[2][srcPos++] + 128;
			pDstY [dstPos++] = (Fw8u)(result>>8);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToY_JPEG_8u_C3C1R)(const Fw8u *pSrcRGB, int srcStep,
								   Fw8u *pDstY, int dstStep, FwiSize roiSize)
{
    RGBToY_JPEG_8u_C3C1  data;
    if(roiSize.width>32)
    {
        return OPT_LEVEL::fe< RGBToY_JPEG_8u_C3C1 >( data, pSrcRGB, srcStep, pDstY, dstStep, roiSize ); 
    }
    else
    {
		if (pSrcRGB == 0 || pDstY == 0) return fwStsNullPtrErr;
		STEPCHECK(srcStep, dstStep);
		ROISIZECHECK(roiSize);

        switch( Dispatch::Type<DT_SSE2>() )
        {
        case DT_SSE3:
        case DT_SSE2:
                if(roiSize.width ==32 && roiSize.height == 32)
                {
                    data.SSE_32(pSrcRGB,srcStep,pDstY, dstStep);
                }
                else
                    data.REF_CODE(pSrcRGB,srcStep,pDstY,dstStep,roiSize);
            break;
        default:
            {
                    data.REF_CODE(pSrcRGB,srcStep,pDstY,dstStep,roiSize);
            }
        }
	    return fwStsNoErr;
    }
}

//-----------------------------------------------------------------------
//The function FwiBGRToY_JPEG is declared in the fwImage.h file. It operates 
//with ROI (see Regions of Interest in manual). This function converts the 
//RGB image to gray scale using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//-----------------------------------------------------------------------

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToY_JPEG_8u_C3C1R)(const Fw8u *pSrcBGR, int srcStep,
								   Fw8u *pDstY, int dstStep, FwiSize roiSize)
{
	if (pSrcBGR == 0 || pDstY == 0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			result = 29 * pSrcBGR[srcPos] + 150 * pSrcBGR[srcPos+1]
				+ 77 * pSrcBGR[srcPos+2] + 128;
			srcPos += 3;
			pDstY [dstPos++] = (Fw8u)(result>>8);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The function FwiRGBToYCbCr_JPEG is declared in the fwImage.h file. It 
//operates with ROI (see Regions of Interest in manual). This function 
//converts the RGB image to YCbCr Color image using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_JPEG_8u_P3R)(const Fw8u *pSrcRGB[3], int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcRGB == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pSrcRGB[0] == 0 || pSrcRGB[1] == 0 || pSrcRGB[2] == 0 ||
		pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	//int result;
	unsigned short result;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			result = 77 * pSrcRGB[0][srcPos] + 150 * pSrcRGB[1][srcPos]
				+ 29 * pSrcRGB[2][srcPos] + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * pSrcRGB[0][srcPos] - 85 * pSrcRGB[1][srcPos] + 
				128 * pSrcRGB[2][srcPos] + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * pSrcRGB[0][srcPos] - 107 * pSrcRGB[1][srcPos] - 
				21 * pSrcRGB[2][srcPos++] + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_JPEG_8u_C3P3R)(const Fw8u *pSrcRGB, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{

    RGBToYCBCR_C3P3  data;
    if(roiSize.width>32)
    {
        return OPT_LEVEL::fec1S3D< RGBToYCBCR_C3P3 >( data, pSrcRGB, srcStep, pDstYCbCr[0], dstStep, pDstYCbCr[1], dstStep, pDstYCbCr[2], dstStep, roiSize ); 
    }
    else
    {
	    if (pSrcRGB == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	    if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		    return fwStsNullPtrErr;

	    STEPCHECK(srcStep, dstStep);
	    ROISIZECHECK(roiSize);


        switch( Dispatch::Type<DT_SSE2>() )
        {
        case DT_SSE3:
        case DT_SSE2:
                if(roiSize.width ==32 && roiSize.height == 32)
                {
                    data.SSE_32(pSrcRGB,srcStep,pDstYCbCr[0],pDstYCbCr[1],pDstYCbCr[2], dstStep);
                }
                else
                    data.REF_CODE(pSrcRGB,srcStep,pDstYCbCr,dstStep,roiSize);
            break;
        default:
            {
                    data.REF_CODE(pSrcRGB,srcStep,pDstYCbCr,dstStep,roiSize);
            }
        }
	    return fwStsNoErr;
    }
}

//-----------------------------------------------------------------------
//The function fwiYCbCrToRGB_JPEG is declared in the fwImage.h file. It 
//operates with ROI (see Regions of Interest in Manual).
//This function converts an YCbCr image to the RGB image according to 
//the following formulas:
//	R = Y + 1.402*Cr - 179.456
//	G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
//	B = Y + 1.772*Cb - 226.816
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_JPEG_8u_P3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw8u *pDstRGB[3], int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstRGB == 0) return fwStsNullPtrErr;
	if (pDstRGB[0] == 0 || pDstRGB[1] == 0 || pDstRGB[2] == 0 ||
		pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			pDstRGB[0][dstPos] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.402*pSrcYCbCr[2][srcPos] - 178.956);
			pDstRGB[1][dstPos] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] - 
				0.34414*pSrcYCbCr[1][srcPos] - 0.71414*pSrcYCbCr[2][srcPos]+ 135.95984);
			pDstRGB[2][dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.772*pSrcYCbCr[1][srcPos++] - 226.316);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_JPEG_8u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw8u *pDstRGB, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstRGB == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.402*pSrcYCbCr[2][srcPos] - 178.956);
			pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] - 
				0.34414*pSrcYCbCr[1][srcPos] - 0.71414*pSrcYCbCr[2][srcPos]+ 135.95984);
			pDstRGB[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.772*pSrcYCbCr[1][srcPos++] - 226.316);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The functions fwiRGB565ToYCbCr_JPEG and fwiRGB555ToYCbCr_JPEG are 
//declared in the fwImage.h file. These functions convert an RGB image to 
//the YCbCr image using the same formulas as in fwiRGBToYCbCr_JPEG function 
//for computing Y, Cb, and Cr component values. The source image pSrcRGB has 
//a reduced bit depth of 16 bits per pixel , and it can be in one of two 
//possible formats : RGB565 (5 bits for red, 6 bits for green, 5 bits for 
//blue), or RGB555 (5 bits for red, green, blue). This function converts 
//the RGB image to YCbCr Color image using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGB565ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u *pSrcRGB, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcRGB == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result, RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	//adjust srcStep to use pixel array
	srcStep=srcStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			RVal=(pSrcRGB[srcPos]&0x1f)<<3; 
			GVal=((pSrcRGB[srcPos]>>5)&0x3f)<<2;
			BVal=(pSrcRGB[srcPos++]>>11)<<3;

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGB555ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u *pSrcRGB, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcRGB == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result, RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	//adjust srcStep to use pixel array
	srcStep=srcStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			RVal=(pSrcRGB[srcPos]&0x1f)<<3; 
			GVal=((pSrcRGB[srcPos]>>5)&0x1f)<<3;
			BVal=((pSrcRGB[srcPos++]>>10)&0x1f)<<3;

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The functions fwiYCbCrToRGB565_JPEG and fwiYCbCrToRGB565_JPEG are 
//declared in the fwImage.h file. They operate with ROI.
//
//These functions convert an YCbCr image to the RGB format using the following 
//formulas.
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//
//The destination image pDstRGB has a reduced bit depth of 16 bits per pixel,
//and it can be in one of two possible formats : RGB565 (5 bits for red, 
//6 bits for green, 5 bits for blue), or RGB555 (5 bits for red, green, blue).
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB565_JPEG_8u16u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw16u *pDstRGB, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstRGB == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	unsigned short RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;
	double val;

	//adjust dstStep to use pixel array
	dstStep=dstStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			val = (pSrcYCbCr[0][srcPos] + 1.402*pSrcYCbCr[2][srcPos] - 178.956)/8;
			RVal = FWJ_LIMIT5(val);
			val = (pSrcYCbCr[0][srcPos] - 0.34414*pSrcYCbCr[1][srcPos] - 
				0.71414*pSrcYCbCr[2][srcPos] + 135.95984)/4;
			GVal = FWJ_LIMIT6(val) << 5;
			val = (pSrcYCbCr[0][srcPos] + 1.772*pSrcYCbCr[1][srcPos++] - 226.316)/8;
			BVal = FWJ_LIMIT5(val)<<11;
			pDstRGB[dstPos++]=RVal|GVal|BVal; 
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB555_JPEG_8u16u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw16u *pDstRGB, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstRGB == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	unsigned short RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;
	double val;

	//adjust dstStep to use pixel array
	dstStep=dstStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			val = (pSrcYCbCr[0][srcPos] + 1.402*pSrcYCbCr[2][srcPos] - 178.956)/8;
			RVal = FWJ_LIMIT5(val);
			val = (pSrcYCbCr[0][srcPos] - 0.34414*pSrcYCbCr[1][srcPos] - 
				0.71414*pSrcYCbCr[2][srcPos] + 135.95984)/8;
			GVal = FWJ_LIMIT5(val) << 5;
			val = (pSrcYCbCr[0][srcPos] + 1.772*pSrcYCbCr[1][srcPos++] - 226.316)/8;
			BVal = FWJ_LIMIT5(val) << 10;
			pDstRGB[dstPos++]=RVal|GVal|BVal; 
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The function FwiBGRToYCbCr_JPEG is declared in the fwImage.h file. It 
//operates with ROI (see Regions of Interest in manual). This function 
//converts the BGR image to YCbCr Color image using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr_JPEG_8u_C3P3R)(const Fw8u *pSrcBGR, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcBGR == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result, RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			BVal=pSrcBGR[srcPos++]; 
			GVal=pSrcBGR[srcPos++];
			RVal=pSrcBGR[srcPos++];

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The function fwiYCbCrToBGR_JPEG is declared in the fwImage.h file. It 
//operates with ROI (see Regions of Interest in Manual).
//This function converts an YCbCr image to the BGR image according to 
//the following formulas:
//	R = Y + 1.402*Cr - 179.456
//	G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
//	B = Y + 1.772*Cb - 226.816
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR_JPEG_8u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw8u *pDstBGR, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstBGR == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			pDstBGR[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.772*pSrcYCbCr[1][srcPos] - 226.316);
			pDstBGR[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] - 
				0.34414*pSrcYCbCr[1][srcPos] - 0.71414*pSrcYCbCr[2][srcPos]+ 135.95984);
			pDstBGR[dstPos++] = FW_REF::Limits<U8>::Sat(pSrcYCbCr[0][srcPos] + 
				1.402*pSrcYCbCr[2][srcPos] - 178.956);
			srcPos++;
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The functions fwiBGR565ToYCbCr_JPEG and fwiBGR555ToYCbCr_JPEG are 
//declared in the fwImage.h file. These functions convert an BGR image to 
//the YCbCr image using the same formulas as in fwiBGRToYCbCr_JPEG function 
//for computing Y, Cb, and Cr component values. The source image pSrcBGR has 
//a reduced bit depth of 16 bits per pixel , and it can be in one of two 
//possible formats : BGR565 (5 bits for red, 6 bits for green, 5 bits for 
//blue), or BGR555 (5 bits for red, green, blue). This function converts 
//the BGR image to YCbCr Color image using the following formula:
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR565ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u *pSrcBGR, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcBGR == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result, RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	//adjust srcStep to use pixel array
	srcStep=srcStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
			GVal=((pSrcBGR[srcPos]>>5)&0x3f)<<2;
			RVal=(pSrcBGR[srcPos++]>>11)<<3;

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGR555ToYCbCr_JPEG_16u8u_C3P3R)(const Fw16u *pSrcBGR, int srcStep, 
								 Fw8u *pDstYCbCr[3], int dstStep, FwiSize roiSize)
{
	if (pSrcBGR == 0 || pDstYCbCr == 0) return fwStsNullPtrErr;
	if (pDstYCbCr[0] == 0 || pDstYCbCr[1] == 0 || pDstYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	unsigned short result, RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	//adjust srcStep to use pixel array
	srcStep=srcStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			BVal=(pSrcBGR[srcPos]&0x1f)<<3; 
			GVal=((pSrcBGR[srcPos]>>5)&0x1f)<<3;
			RVal=((pSrcBGR[srcPos++]>>10)&0x1f)<<3;

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCbCr [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal	+ 128 * BVal + 128;
			pDstYCbCr [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCbCr [2][dstPos++] = (Fw8u)((result>>8)+128);
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The functions fwiYCbCrToBGR565_JPEG and fwiYCbCrToBGR565_JPEG are 
//declared in the fwImage.h file. They operate with ROI.
//
//These functions convert an YCbCr image to the BGR format using the following 
//formulas.
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128	
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//
//The destination image pDstBGR has a reduced bit depth of 16 bits per pixel,
//and it can be in one of two possible formats : BGR565 (5 bits for red, 
//6 bits for green, 5 bits for blue), or BGR555 (5 bits for red, green, blue).
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR565_JPEG_8u16u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw16u *pDstBGR, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstBGR == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	unsigned short RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;
	double val;

	//adjust dstStep to use pixel array
	dstStep=dstStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			val = (pSrcYCbCr[0][srcPos] + 1.402*pSrcYCbCr[2][srcPos] - 178.956)/8;
			RVal = FWJ_LIMIT5(val) << 11;
			val = (pSrcYCbCr[0][srcPos] - 0.34414*pSrcYCbCr[1][srcPos] - 
				0.71414*pSrcYCbCr[2][srcPos] + 135.95984)/4;
			GVal = FWJ_LIMIT6(val) << 5;
			val = (pSrcYCbCr[0][srcPos] + 1.772*pSrcYCbCr[1][srcPos++] - 226.316)/8;
			BVal = FWJ_LIMIT5(val);
			pDstBGR[dstPos++]=RVal|GVal|BVal; 
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR555_JPEG_8u16u_P3C3R)(const Fw8u *pSrcYCbCr[3], int srcStep, 
									 Fw16u *pDstBGR, int dstStep, FwiSize roiSize)
{
	if (pSrcYCbCr == 0 || pDstBGR == 0) return fwStsNullPtrErr;
	if (pSrcYCbCr[0] == 0 || pSrcYCbCr[1] == 0 || pSrcYCbCr[2] == 0 )
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	unsigned short RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;
	double val;

	//adjust dstStep to use pixel array
	dstStep=dstStep/sizeof(Fw16u);

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			val = (pSrcYCbCr[0][srcPos] + 1.402*pSrcYCbCr[2][srcPos] - 178.956)/8;
			RVal = FWJ_LIMIT5(val) << 10;
			val = (pSrcYCbCr[0][srcPos] - 0.34414*pSrcYCbCr[1][srcPos] - 
				0.71414*pSrcYCbCr[2][srcPos] + 135.95984)/8;
			GVal = FWJ_LIMIT5(val) << 5;
			val = (pSrcYCbCr[0][srcPos] + 1.772*pSrcYCbCr[1][srcPos++] - 226.316)/8;
			BVal = FWJ_LIMIT5(val);
			pDstBGR[dstPos++]=RVal|GVal|BVal; 
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//The function fwiCMYKToYCCK_JPEG_8u_P4R converts a CMYK image to the YCCK 
//image in two steps. First, conversion is done into RGB format:
//	R = 255 – C
//	G = 255 – M
//	B = 255 – Y
//After that, conversion to YCCK image is performed as:
//	Y = 0.299*R + 0.587*G + 0.114*B
//	Cb = -0.16874*R - 0.33126*G + 0.5*B + 128
//	Cr = 0.5*R - 0.41869*G - 0.08131*B + 128
//The values of K channel are written without modification.
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK_JPEG_8u_P4R)(const Fw8u *pSrcCMYK[4], int srcStep, 
									 Fw8u *pDstYCCK[4], int dstStep, FwiSize roiSize)
{
	if (pSrcCMYK == 0 || pDstYCCK == 0) return fwStsNullPtrErr;
	if (pSrcCMYK[0] == 0 || pSrcCMYK[1] == 0 || pSrcCMYK[2] == 0 ||
		pDstYCCK[0] == 0 || pDstYCCK[1] == 0 || pDstYCCK[2] == 0 ||
		pSrcCMYK[3] == 0 || pDstYCCK[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	//int result;
	unsigned short result;
	unsigned char RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			RVal = ~(pSrcCMYK[0][srcPos]);//R=255-C
			GVal = ~(pSrcCMYK[1][srcPos]);//G=255-M
			BVal = ~(pSrcCMYK[2][srcPos]);//B=255-Y

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCCK [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
			pDstYCCK [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCCK [2][dstPos] = (Fw8u)((result>>8)+128);
			pDstYCCK [3][dstPos++] = pSrcCMYK[3][srcPos++];
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiCMYKToYCCK_JPEG_8u_C4P4R)(const Fw8u *pSrcCMYK, int srcStep, 
									 Fw8u *pDstYCCK[4], int dstStep, FwiSize roiSize)
{
	if (pSrcCMYK == 0 || pDstYCCK == 0) return fwStsNullPtrErr;
	if (pDstYCCK[0] == 0 || pDstYCCK[1] == 0 || pDstYCCK[2] == 0 ||
		pDstYCCK[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//DEV code use shift 8 bit data for coeffcients
	//0.299*256=76.544, 0.587*256=150.272, 0.114*256=29.184
	//We use 77, 150, 29 as the modified coeff, and then shift the result
	//-0.16874*256 = -43.19744, -0.33126*256=-84.80256, 0.5*256=128
	//0.5*256=128, -0.41869*256 = -107.18464, -0.08131*256=-20.81536
	//The final answer is equal to nearest neighbor rounding
	//SEE2 should use 16 bit data shift
	//int result;
	unsigned short result;
	unsigned char RVal, GVal, BVal;
	int x, y;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			RVal = ~(pSrcCMYK[srcPos++]);//R=255-C
			GVal = ~(pSrcCMYK[srcPos++]);//G=255-M
			BVal = ~(pSrcCMYK[srcPos++]);//B=255-Y

			//add 0.5 for nearest neighbor rounding
			result = 77 * RVal + 150 * GVal + 29 * BVal + 128;
			pDstYCCK [0][dstPos] = (Fw8u)(result>>8);
			result = -43 * RVal - 85 * GVal + 128 * BVal + 128;
			pDstYCCK [1][dstPos] = (Fw8u)((result>>8)+128);
			result = 128 * RVal - 107 * GVal - 21 * BVal + 128;
			pDstYCCK [2][dstPos] = (Fw8u)((result>>8)+128);
			pDstYCCK [3][dstPos++] = pSrcCMYK[srcPos++];
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This function converts an YCCK image to the CMYK image in two steps. First, conversion is
//done into RGB format as:
//	R = Y + 1.402*Cr - 179.456
//	G = Y - 0.34414*Cb - 0.71414*Cr + 135.45984
//	B = Y + 1.772*Cb - 226.816
//After that, conversion to CMYK image is performed as follows:
//	C = 255 – R
//	M = 255 – G
//	Y = 255 – B
//The values of K channel are written without modification.
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCKToCMYK_JPEG_8u_P4R)(const Fw8u *pSrcYCCK[4], int srcStep, 
									 Fw8u *pDstCMYK[4], int dstStep, FwiSize roiSize)
{
	if (pSrcYCCK == 0 || pDstCMYK == 0) return fwStsNullPtrErr;
	if (pDstCMYK[0] == 0 || pDstCMYK[1] == 0 || pDstCMYK[2] == 0 ||
		pSrcYCCK[0] == 0 || pSrcYCCK[1] == 0 || pSrcYCCK[2] == 0 || 
		pDstCMYK[3] == 0 || pSrcYCCK[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	unsigned char RVal, GVal, BVal;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			RVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
				1.402*pSrcYCCK[2][srcPos] - 178.956);
			GVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] - 
				0.34414*pSrcYCCK[1][srcPos] - 0.71414*pSrcYCCK[2][srcPos]+ 135.95984);
			BVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
				1.772*pSrcYCCK[1][srcPos] - 226.316);
			pDstCMYK[0][dstPos] = ~RVal; //C=255-R
			pDstCMYK[1][dstPos] = ~GVal; //M=255-G
			pDstCMYK[2][dstPos] = ~BVal; //Y=255-B
			pDstCMYK[3][dstPos++] = pSrcYCCK[3][srcPos++];
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCKToCMYK_JPEG_8u_P4C4R)(const Fw8u *pSrcYCCK[4], int srcStep, 
									 Fw8u *pDstCMYK, int dstStep, FwiSize roiSize)
{
	if (pSrcYCCK == 0 || pDstCMYK == 0) return fwStsNullPtrErr;
	if (pSrcYCCK[0] == 0 || pSrcYCCK[1] == 0 || pSrcYCCK[2] == 0 || 
		pSrcYCCK[3] == 0)
		return fwStsNullPtrErr;

	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	//Reference code only.
	//SSE2 code need to shift 16 bit 
	int x, y;
	unsigned char RVal, GVal, BVal;
	int srcPos, dstPos;

	for (y=0;y<roiSize.height; y++) {
		srcPos = y*srcStep;
		dstPos = y*dstStep;
		for (x=0;x<roiSize.width;x++) {
			//add 0.5 for nearest neighbor rounding
			RVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
				1.402*pSrcYCCK[2][srcPos] - 178.956);
			GVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] - 
				0.34414*pSrcYCCK[1][srcPos] - 0.71414*pSrcYCCK[2][srcPos]+ 135.95984);
			BVal = FW_REF::Limits<U8>::Sat(pSrcYCCK[0][srcPos] + 
				1.772*pSrcYCCK[1][srcPos] - 226.316);
			pDstCMYK[dstPos++] = ~RVal; //C=255-R
			pDstCMYK[dstPos++] = ~GVal; //M=255-G
			pDstCMYK[dstPos++] = ~BVal; //Y=255-B
			pDstCMYK[dstPos++] = pSrcYCCK[3][srcPos++];
		}
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2
