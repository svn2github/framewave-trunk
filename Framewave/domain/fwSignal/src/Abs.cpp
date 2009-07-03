/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AbsDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

/// Absolute value Functions

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_16s)(const Fw16s* pSrc, Fw16s* pDst, int len)
{
	DEF_ABS::ABS::C1::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_16s>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_32s)(const Fw32s* pSrc, Fw32s* pDst, int len)
{
	DEF_ABS::ABS::C1::Abs_32s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32s>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	DEF_ABS::ABS::C1::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32f>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	DEF_ABS::ABS::C1::Abs_64f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_64f>(data, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_16s_I)(Fw16s* pSrcDst, int len)
{
	DEF_ABS::ABS::C1::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_16s>(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_32s_I)(Fw32s* pSrcDst, int len)
{
	DEF_ABS::ABS::C1::Abs_32s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32s>(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_32f_I)(Fw32f* pSrcDst, int len)
{
	DEF_ABS::ABS::C1::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32f>(data, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAbs_64f_I)(Fw64f* pSrcDst, int len)
{
	DEF_ABS::ABS::C1::Abs_64f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_64f>(data, pSrcDst, pSrcDst, len);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
