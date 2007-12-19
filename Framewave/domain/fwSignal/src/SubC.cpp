/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/SubDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

// SUBC

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32f				)(const Fw32f* pSrc, Fw32f val, Fw32f* pDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_32f data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32f >(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32fc				)(const Fw32fc* pSrc, Fw32fc val,Fw32fc* pDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_32fc data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32fc >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_64fc				)(const Fw64fc* pSrc, Fw64fc val, Fw64fc* pDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_64fc data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_64fc >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_16s_I			)(Fw16s val, Fw16s* pSrcDst, int len)
{
	if(val == 0)	return fwStsNoErr;

	DEF_SUB::SUBC::C1::SubC_16s<0> data(0, val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<0> >(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32f_I			)(Fw32f val, Fw32f* pSrcDst, int len)
{	
	if(val == 0)	return fwStsNoErr;

	DEF_SUB::SUBC::C1::SubC_32f data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32f >(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_64f_I			)(Fw64f val, Fw64f* pSrcDst, int len)
{
	if(val == 0)	return fwStsNoErr;

	DEF_SUB::SUBC::C1::SubC_64f data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_64f >(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32fc_I			)(Fw32fc val, Fw32fc* pSrcDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_32fc data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32fc >(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_64f				)(const Fw64f* pSrc, Fw64f val, Fw64f* pDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_64f data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_64f >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_64fc_I			)(Fw64fc val, Fw64fc* pSrcDst, int len)
{
	DEF_SUB::SUBC::C1::SubC_64fc data(val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_64fc >(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_8u_Sfs			)(const Fw8u* pSrc, Fw8u val, Fw8u* pDst, int len, int scaleFactor) 
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_8u<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<0> >(data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_8u<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_8u<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<1> >(data, pSrc, pDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_16s_Sfs			)(const Fw16s* pSrc, Fw16s val, Fw16s* pDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_16s<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<0> >(data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_16s<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_16s<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<1> >(data, pSrc, pDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32s_Sfs			)(const Fw32s* pSrc, Fw32s val, Fw32s* pDst, int len, int scaleFactor)
{
	DEF_SUB::SUBC::C1::SubC_32s data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32s >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_16sc_Sfs			)(const Fw16sc* pSrc, Fw16sc val, Fw16sc* pDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_16sc<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<0> >(data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_16sc<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_16sc<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<1> >(data, pSrc, pDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32sc_Sfs			)(const Fw32sc* pSrc, Fw32sc val, Fw32sc* pDst, int len, int scaleFactor)
{
	DEF_SUB::SUBC::C1::SubC_32sc data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32sc >(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_8u_ISfs			)(Fw8u val, Fw8u* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_8u<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<0> >(data, pSrcDst, pSrcDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_8u<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<-1> >(data, pSrcDst, pSrcDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_8u<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_8u<1> >(data, pSrcDst, pSrcDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_16s_ISfs			)(Fw16s val, Fw16s* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_16s<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<0> >(data, pSrcDst, pSrcDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_16s<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<-1> >(data, pSrcDst, pSrcDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_16s<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16s<1> >(data, pSrcDst, pSrcDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32s_ISfs			)(Fw32s val, Fw32s* pSrcDst, int len, int scaleFactor)
{
	DEF_SUB::SUBC::C1::SubC_32s data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32s >(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_16sc_ISfs		)(Fw16sc val, Fw16sc* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SUB::SUBC::C1::SubC_16sc<0> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<0> >(data, pSrcDst, pSrcDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																							
		DEF_SUB::SUBC::C1::SubC_16sc<-1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<-1> >(data, pSrcDst, pSrcDst, len);
	}																							
	else																						
	{																							
		DEF_SUB::SUBC::C1::SubC_16sc<1> data(scaleFactor, val);
		return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_16sc<1> >(data, pSrcDst, pSrcDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSubC_32sc_ISfs		)(Fw32sc val, Fw32sc* pSrcDst, int len, int scaleFactor)
{
	DEF_SUB::SUBC::C1::SubC_32sc data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_SUB::SUBC::C1::SubC_32sc >(data, pSrcDst, pSrcDst, len);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
