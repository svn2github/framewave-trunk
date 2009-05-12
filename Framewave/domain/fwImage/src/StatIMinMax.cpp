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

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_8u_C4R) (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_8u_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_8u_C4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];
	min[3] = data.min[3];
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_16s_C4R )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_16s_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_16s_C4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];
	min[3] = data.min[3];
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_32f_C4R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_32f_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMin_32f_C4R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];
	min[3] = data.min[3];
	return retStatus;
}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_8u_C3R )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatIMin_8u_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMin_8u_C3R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_16s_C3R) (const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_16s_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMin_16s_C3R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMin_32f_C3R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMin_32f_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMin_32f_C3R>(data,pSrc,srcStep,roiSize);
	min[0] = data.min[0];
	min[1] = data.min[1];
	min[2] = data.min[2];

	return retStatus;

}

////// Max

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_8u_C1R)(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMax)
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_8u_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMax_8u_C1R>(data,pSrc,srcStep,roiSize);
	*pMax = data.max;

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_16s_C1R)(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMax)
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_16s_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMax_16s_C1R>(data,pSrc,srcStep,roiSize);
	*pMax = data.max;

	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_32f_C1R)(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMax)
{

	FwStatus retStatus = fwStsNoErr;
	StatIMax_32f_C1R data;
	retStatus = OPT_LEVEL::fex<StatIMax_32f_C1R>(data,pSrc,srcStep,roiSize);
	*pMax = data.max;

	return retStatus;

}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_8u_AC4R) (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_8u_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_8u_AC4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_16s_AC4R )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_16s_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_16s_AC4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_32f_AC4R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_32f_AC4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_32f_AC4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_8u_C4R) (const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_8u_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_8u_C4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];
	max[3] = data.max[3];
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_16s_C4R )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_16s_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_16s_C4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];
	max[3] = data.max[3];
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_32f_C4R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[4])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_32f_C4R data;
	retStatus = OPT_LEVEL::fex<StatIMax_32f_C4R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];
	max[3] = data.max[3];
	return retStatus;
}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_8u_C3R )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[3])
{

	FwStatus retStatus = fwStsNoErr;
	StatIMax_8u_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMax_8u_C3R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_16s_C3R) (const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_16s_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMax_16s_C3R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMax_32f_C3R) (const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[3])
{
	FwStatus retStatus = fwStsNoErr;
	StatIMax_32f_C3R data;
	retStatus = OPT_LEVEL::fex<StatIMax_32f_C3R>(data,pSrc,srcStep,roiSize);
	max[0] = data.max[0];
	max[1] = data.max[1];
	max[2] = data.max[2];

	return retStatus;

}

////  Max end



// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2

