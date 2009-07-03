/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/DivDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

// DivC
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_32f)(const Fw32f* pSrc, Fw32f val, Fw32f* pDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32f>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_64f)(const Fw64f* pSrc, Fw64f val, Fw64f* pDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_64f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_64f>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_32fc)(const Fw32fc* pSrc, Fw32fc val, Fw32fc* pDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32fc data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32fc>(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_64fc)(const Fw64fc* pSrc, Fw64fc val, Fw64fc* pDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_64fc data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_64fc>(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_32f_I)(Fw32f val, Fw32f* pSrcDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32f>(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_64f_I)(Fw64f val, Fw64f* pSrcDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;
	if( val == 1) return fwStsNoErr;

	DEF_DIV::DIVC::C1::DivC_64f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_64f>(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_32fc_I)(Fw32fc val, Fw32fc* pSrcDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32fc data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32fc>(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_64fc_I)(Fw64fc val, Fw64fc* pSrcDst, int len)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_64fc data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_64fc>(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_8u_Sfs)(const Fw8u* pSrc, Fw8u val, Fw8u* pDst, int len,int scaleFactor)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_8u data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_8u>(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_16s_Sfs)(const Fw16s* pSrc, Fw16s val, Fw16s* pDst, int len, int scaleFactor)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16s data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16s>(data, pSrc, pDst, len);
}	
		
FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_16sc_Sfs)(const Fw16sc* pSrc, Fw16sc val, Fw16sc* pDst,int len, int scaleFactor)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16sc data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16sc>(data, pSrc, pDst, len);
}	

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_8u_ISfs)(Fw8u val, Fw8u* pSrcDst, int len, int scaleFactor)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_8u data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_8u>(data, pSrcDst, pSrcDst, len);
}	

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_16s_ISfs)(Fw16s val, Fw16s* pSrcDst, int len, int scaleFactor)
{
	if( val == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16s data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16s>(data, pSrcDst, pSrcDst, len);
}	

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivC_16sc_ISfs)(Fw16sc val, Fw16sc* pSrcDst, int len, int scaleFactor)
{
	if( val == 0)
		return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16sc data(scaleFactor, val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16sc>(data, pSrcDst, pSrcDst, len);
}	



// DivCRev

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivCRev_16u)(const Fw16u* pSrc, Fw16u val, Fw16u* pDst, int len)
{
	if (0 == val)
		return fwsSet_16s(0, (Fw16s*) pDst, len);

	DEF_DIV::DIVC::C1::DivCRev_16u data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivCRev_16u>(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivCRev_32f)( const Fw32f* pSrc, Fw32f val, Fw32f* pDst, int len)
{
	if (0 == val)
		return fwsSet_32f(0, pDst, len);

	DEF_DIV::DIVC::C1::DivCRev_32f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivCRev_32f>(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivCRev_16u_I)(Fw16u val, Fw16u* pSrcDst, int len)
{
	if (0 == val)
		return fwsSet_16s(0, (Fw16s*) pSrcDst, len);

	DEF_DIV::DIVC::C1::DivCRev_16u data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivCRev_16u>(data, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDivCRev_32f_I)(Fw32f val, Fw32f* pSrcDst, int len)
{
	if (0 == val)
		return fwsSet_32f(0, pSrcDst, len);

	DEF_DIV::DIVC::C1::DivCRev_32f data(val);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivCRev_32f>(data, pSrcDst, pSrcDst, len);
}



// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
