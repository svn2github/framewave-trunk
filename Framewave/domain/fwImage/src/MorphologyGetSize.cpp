/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

//In this function, only binary (0,1) masks are processed.
#if BUILD_NUM_AT_LEAST( 9999 )

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_MorphologyGetSize(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	if (pMask==0 || pSize==0) return fwStsNullPtrErr;

	if (maskSize.height <= 0 || maskSize.width <= 0 || roiWidth < 1) return fwStsSizeErr;

	*(pSize) = sizeof(Fw8u)*maskSize.width*maskSize.height
		+ sizeof(FwiSize) + sizeof(FwiPoint) + 2*sizeof(bool) + sizeof(int);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C1R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw8u, C1, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C3R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw8u, C3, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C4R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw8u, C4, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C1R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw32f, C1, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C3R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw32f, C3, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C4R)(int roiWidth, const Fw8u *pMask, FwiSize maskSize, int *pSize) 
{
	return My_FW_MorphologyGetSize<Fw32f, C4, DT_REFR> (roiWidth, pMask, maskSize, pSize);
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
