/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatisticsNorm.h"
#include "fwSignal.h"


using namespace OPT_LEVEL;



FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_32f)(const Fw32f* pSrc, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_Inf_32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_32f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_64f)(const Fw64f* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_Inf_64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_16s32f)(const Fw16s* pSrc, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_Inf_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_16s32f>(data,pSrc,len,THREAD_SW_OFF);
    Fw32s mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
    inttofloat src;
    src.fVal = (Fw32f)data.max;
    src.iVal = src.iVal & mask;
    *pNorm = src.fVal;

    return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_32fc32f)(const Fw32fc* pSrc, int len, Fw32f* pNorm)
{
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	if(len == 1)
	{
		*pNorm = sqrt(pSrc->re * pSrc->re + pSrc->im * pSrc->im);
		 return fwStsNoErr;
	}

	FwStatus retStatus;	
	StatNorm_Inf_32fc32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_32fc32f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = (Fw32f)sqrt(data.max);
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_64fc64f)(const Fw64fc* pSrc, int len, Fw64f* pNorm)
{
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	FwStatus retStatus;	
	StatNorm_Inf_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_64fc64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.max);
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_Inf_16s32s_Sfs)(const Fw16s* pSrc, int len, Fw32s* pNorm,int scaleFactor)
{
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	FwStatus retStatus;	
	StatNorm_Inf_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_Inf_16s32f>(data,pSrc,len,THREAD_SW_OFF);
    inttofloat src;
    Fw32s mask = CBL_LIBRARY::Limits<Fw32s>::MaxValue();
    src.fVal = (Fw32f)data.max;
    src.iVal = src.iVal & mask;


	*pNorm = FW_REF::Limits<S32>::Sat(FW_REF::Scale(src.fVal ,scaleFactor));
	return retStatus;
}




FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_32f)(const Fw32f* pSrc, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_L1_32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_32f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_64f)(const Fw64f* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_L1_64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_16s32f)(const Fw16s* pSrc, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNorm_L1_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_16s32f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_32fc64f)(const Fw32fc* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	if(len == 1)
	{
		*pNorm = sqrt(pSrc->re * pSrc->re + pSrc->im * pSrc->im);
		 return fwStsNoErr;
	}

	StatNorm_L1_32fc64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_32fc64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_64fc64f)(const Fw64fc* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNorm_L1_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_64fc64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L1_16s32s_Sfs)(const Fw16s* pSrc, int len, Fw32s* pNorm,int scaleFactor)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L1_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatNorm_L1_16s32s_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<S32>::Sat((FW_REF::Scale((Fw64f)data.sum,scaleFactor)));
	return retStatus;
}




FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_32f)(const Fw32f* pSrc, int len, Fw32f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L2_32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_32f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_64f)(const Fw64f* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L2_64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_16s32f)(const Fw16s* pSrc, int len, Fw32f* pNorm)
{
	Fw64f result;
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L2_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_16s32f>(data,pSrc,len,THREAD_SW_OFF);

	if(SYS_FINITE(data.sum))
	{
		result = sqrt(data.sum);
		*pNorm = FW_REF::Limits<F32>::Sat(result);
	}
	else
	{
		*pNorm  = Const::IND_32F;
	}

	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_32fc64f)(const Fw32fc* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L2_32fc64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_32fc64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_64fc64f)(const Fw64fc* pSrc, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNorm_L2_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_64fc64f>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNorm_L2_16s32s_Sfs)(const Fw16s* pSrc, int len, Fw32s* pNorm, int scaleFactor)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNorm_L2_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatNorm_L2_16s32s_Sfs>(data,pSrc,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<S32>::Sat(FW_REF::Scale(sqrt((Fw64f)data.sum),scaleFactor));

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_32f)(const Fw32f* pSrc1,const Fw32f* pSrc2,int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_Inf_32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_64f)(const Fw64f* pSrc1,const Fw64f* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_Inf_64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_16s32f)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_Inf_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_16s32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.max;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_32fc32f)(const Fw32fc* pSrc1,const Fw32fc* pSrc2, int len, Fw32f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNormDiff_Inf_32fc32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_32fc32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm =  FW_REF::Limits<F32>::Sat(sqrt(data.max));	
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_64fc64f)(const Fw64fc* pSrc1,const Fw64fc* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNormDiff_Inf_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_64fc64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.max);
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_Inf_16s32s_Sfs)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32s* pNorm,int scaleFactor)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNormDiff_Inf_16s32s data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_Inf_16s32s>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.max,scaleFactor));

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_32f)(const Fw32f* pSrc1,const Fw32f* pSrc2, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_L1_32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_64f)(const Fw64f* pSrc1,const Fw64f* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_L1_64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_16s32f)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32f* pNorm)
{
	FwStatus retStatus;	
	StatNormDiff_L1_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_16s32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_32fc64f)(const Fw32fc* pSrc1,const Fw32fc* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L1_32fc64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_32fc64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_64fc64f)(const Fw64fc* pSrc1,const Fw64fc* pSrc2, int len, Fw64f* pNorm)
{	
	FwStatus retStatus;	
	StatNormDiff_L1_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_64fc64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = data.sum;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L1_16s32s_Sfs)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32s* pNorm,int scaleFactor)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNormDiff_L1_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L1_16s32s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_32f)(const Fw32f* pSrc1,const Fw32f* pSrc2, int len, Fw32f* pNorm)
{

	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;

	StatNormDiff_L2_32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_64f)(const Fw64f* pSrc1,const Fw64f* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L2_64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);

	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_16s32f)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L2_16s32f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_16s32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<F32>::Sat(sqrt(data.sum));
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_32fc64f)(const Fw32fc* pSrc1,const Fw32fc* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L2_32fc64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_32fc64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_64fc64f)(const Fw64fc* pSrc1,const Fw64fc* pSrc2, int len, Fw64f* pNorm)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L2_64fc64f data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_64fc64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = sqrt(data.sum);
	return retStatus;
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormDiff_L2_16s32s_Sfs)(const Fw16s* pSrc1,const Fw16s* pSrc2, int len, Fw32s* pNorm, int scaleFactor)
{
	FwStatus retStatus;
	if( FW_REF::SizeNotOK( len   ) ) return fwStsSizeErr;
	StatNormDiff_L2_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatNormDiff_L2_16s32s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pNorm = FW_REF::Limits<S32>::Sat(FW_REF::Scale(sqrt(data.sum),scaleFactor));
	return retStatus;
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
