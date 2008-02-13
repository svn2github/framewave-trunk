/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatISum.h"
#include "fwImage.h"

using namespace OPT_LEVEL;




FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_8u_C1R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f* pSum)
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_8u data;
	retStatus = OPT_LEVEL::fex<StatISum_8u>(data,pSrc,srcStep,roiSize,THREAD_SW_OFF);
	*pSum = (Fw64f)data.sum;

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_16s_C1R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f* pSum)
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_16s data;
	retStatus = OPT_LEVEL::fex<StatISum_16s>(data,pSrc,srcStep,roiSize,THREAD_SW_OFF);
	*pSum = (Fw64f)data.sum;

	return retStatus;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 