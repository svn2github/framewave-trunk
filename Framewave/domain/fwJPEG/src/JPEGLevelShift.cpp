/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the level shift functions from JPEG Chapter
//	fwiSub128_JPEG_8u16s_C1R
//  fwiAdd128_JPEG_16s8u_C1R
//************************************************************************* 

#include "fwdev.h"
#include "fwJPEG.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

#ifndef __JPEGLEVELSHIFT
#define __JPEGLEVELSHIFT
#define STEPCHECK(X, Y) if (X<=0 || Y<=0) return fwStsStepErr
#define ROISIZECHECK(X) if (X.height <=0 || X.width <=0) return fwStsSizeErr
#endif

//-----------------------------------------------------------------------
//Convert data from unsigned char range to the signed 16 bit integer range
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSub128_JPEG_8u16s_C1R)(const Fw8u *pSrc, int srcStep, 
						Fw16s *pDst, int dstStep, FwiSize roiSize)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	int x, y;
	const Fw8u *ptSrc;
	Fw16s *ptDst;

	for (y=0;y<roiSize.height;y++) {
		ptSrc=pSrc+y*srcStep;
		ptDst=pDst+y*dstStep/2; //16 bits
		for (x=0;x<roiSize.width;x++) {
			*(ptDst++)=*(ptSrc++)-128;
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//Convert data from 16 bit signed integer to unsigned char range
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAdd128_JPEG_16s8u_C1R)(const Fw16s *pSrc, int srcStep,
						Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;
	STEPCHECK(srcStep, dstStep);
	ROISIZECHECK(roiSize);

	int x, y;
	const Fw16s *ptSrc;
	Fw8u *ptDst;

	for (y=0;y<roiSize.height;y++) {
		ptSrc=pSrc+y*srcStep/2; //16 bits
		ptDst=pDst+y*dstStep; 
		for (x=0;x<roiSize.width;x++) {
			*(ptDst++)=FW_REF::Limits<U8>::Sat(*(ptSrc++)+128);
		}
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR 
