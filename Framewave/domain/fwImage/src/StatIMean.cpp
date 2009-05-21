/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//#include "StatIMean.h"
#include "StatISum.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_8u_C1R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean)
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_8u data;
	retStatus = OPT_LEVEL::fex<StatISum_8u>(data,pSrc,srcStep,roiSize);
	*pMean = (Fw64f)data.sum / (roiSize.width * roiSize.height);

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_16s_C1R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean)
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_16s data;
	retStatus = OPT_LEVEL::fex<StatISum_16s>(data,pSrc,srcStep,roiSize);
	*pMean = (Fw64f)data.sum/ (roiSize.width * roiSize.height);

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_32f_C1R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean, FwHintAlgorithm hint)
{
    hint;
	FwStatus retStatus = fwStsNoErr;
	StatIMean_32f_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMean_32f_C1R>(data,pSrc,srcStep,roiSize);
	*pMean = (Fw64f)data.sum/ (roiSize.width * roiSize.height);

	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_8u_C3R) (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_8uC3 data;
	retStatus = OPT_LEVEL::fex<StatISum_8uC3>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;
}  
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_16s_C3R) (const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_16sC3 data;
	retStatus = OPT_LEVEL::fex<StatISum_16sC3>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;
}    

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_8u_AC4R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_8u_AC4 data;
	retStatus = OPT_LEVEL::fex<StatISum_8u_AC4>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_16s_AC4R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatISum_16s_AC4 data;
	retStatus = OPT_LEVEL::fex<StatISum_16s_AC4>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_8u_C4R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[4])
{

    FwStatus retStatus = fwStsNoErr;
	StatISum_8u_C4R data;
	retStatus = OPT_LEVEL::fex<StatISum_8u_C4R>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);
    mean[3] = data.sum[3]/ (roiSize.width * roiSize.height);

	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMean_16s_C4R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[4])
{

    FwStatus retStatus = fwStsNoErr;
	StatISum_16s_C4R data;
	retStatus = OPT_LEVEL::fex<StatISum_16s_C4R>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);
    mean[3] = data.sum[3]/ (roiSize.width * roiSize.height);

	return retStatus;

}

FwStatus  PREFIX_OPT(OPT_PREFIX, fwiMean_32f_C3R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3], FwHintAlgorithm hint)
{
    hint;
	FwStatus retStatus = fwStsNoErr;
	StatISum_32f_C3R data;
    
	retStatus = OPT_LEVEL::fex<StatISum_32f_C3R>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;

}

FwStatus  PREFIX_OPT(OPT_PREFIX, fwiMean_32f_AC4R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3], FwHintAlgorithm hint)
{
    hint;
	FwStatus retStatus = fwStsNoErr;
	StatISum_32f_AC4R data;
    
	retStatus = OPT_LEVEL::fex<StatISum_32f_AC4R>(data,pSrc,srcStep,roiSize);
	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);

	return retStatus;

}

FwStatus  PREFIX_OPT(OPT_PREFIX, fwiMean_32f_C4R)(const Fw32f* pSrc, int srcStep, FwiSize roiSize, Fw64f mean[4], FwHintAlgorithm hint)
{
    hint;
    FwStatus retStatus = fwStsNoErr;
	StatISum_32f_C4R data;
	retStatus = OPT_LEVEL::fex<StatISum_32f_C4R>(data,pSrc,srcStep,roiSize);

	mean[0] = data.sum[0]/ (roiSize.width * roiSize.height);
    mean[1] = data.sum[1]/ (roiSize.width * roiSize.height);
    mean[2] = data.sum[2]/ (roiSize.width * roiSize.height);
    mean[3] = data.sum[3]/ (roiSize.width * roiSize.height);

	return retStatus;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2
