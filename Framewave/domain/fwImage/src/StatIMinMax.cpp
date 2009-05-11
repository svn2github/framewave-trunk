/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatIMinMax.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_8u_C1R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMin)
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_8u_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMin_8u_C1R>(data,pSrc,srcStep,roiSize);
	*pMin = data.min;

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_16s_C1R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMin)
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_16s_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMin_16s_C1R>(data,pSrc,srcStep,roiSize);
	*pMin = data.min;

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_32f_C1R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMin)
{

	FwStatus retStatus = fwStsNoErr;
	StatIMin_32f_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMin_32f_C1R>(data,pSrc,srcStep,roiSize);
	*pMin = data.min;

	return retStatus;

}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_8u_AC4R) (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_8u_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_8u_AC4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_16s_AC4R )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_16s_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_16s_AC4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_32f_AC4R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_32f_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_32f_AC4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;
}



// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2

