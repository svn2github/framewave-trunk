/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatICountinRange.h"
#include "fwImage.h"

using namespace OPT_LEVEL;


FwStatus  PREFIX_OPT(OPT_PREFIX,  fwiCountInRange_8u_C1R)(const Fw8u* pSrc, int srcStep, FwiSize roiSize, int* counts, Fw8u lowerBound,Fw8u upperBound)
{

	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_8u data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_8u>(data,pSrc,srcStep,roiSize);
	*counts = data.count;
	return retStatus;

}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwiCountInRange_32f_C1R)(const Fw32f* pSrc, int srcStep, FwiSize roiSize, int* counts, Fw32f lowerBound,Fw32f upperBound)
{
	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_32f data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_32f>(data,pSrc,srcStep,roiSize);
	*counts = data.count;
	return retStatus;
}


FwStatus  PREFIX_OPT(OPT_PREFIX, fwiCountInRange_8u_C3R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, int counts[3], Fw8u lowerBound[3],Fw8u upperBound[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_8u_C3 data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_8u_C3>(data,pSrc,srcStep,roiSize);
	counts[0] = data.count[0];
	counts[1] = data.count[1];
	counts[2] = data.count[2];
	return retStatus;
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwiCountInRange_32f_C3R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, int counts[3], Fw32f lowerBound[3],Fw32f upperBound[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_32f_C3 data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_32f_C3>(data,pSrc,srcStep,roiSize);
	counts[0] = data.count[0];
	counts[1] = data.count[1];
	counts[2] = data.count[2];
	return retStatus;
}

FwStatus  PREFIX_OPT(OPT_PREFIX, fwiCountInRange_8u_AC4R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, int counts[3], Fw8u lowerBound[3],Fw8u upperBound[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_8u_AC4 data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_8u_AC4>(data,pSrc,srcStep,roiSize);
	counts[0] = data.count[0];
	counts[1] = data.count[1];
	counts[2] = data.count[2];
	return retStatus;
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwiCountInRange_32f_AC4R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, int counts[3], Fw32f lowerBound[3],Fw32f upperBound[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatICountInRange_32f_AC4 data(lowerBound,upperBound);
	retStatus = OPT_LEVEL::fex<StatICountInRange_32f_AC4>(data,pSrc,srcStep,roiSize);
	counts[0] = data.count[0];
	counts[1] = data.count[1];
	counts[2] = data.count[2];
	return retStatus;
}



// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2
