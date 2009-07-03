/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AddDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;


// Add* functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_32f		)(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pSrcDst, int len)
{
	DEF_ADD::ADDX::C1::AddProduct_32f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_32f> (data, pSrc1, pSrc2, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_64f		)(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pSrcDst, int len)
{
	DEF_ADD::ADDX::C1::AddProduct_64f data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_64f> (data, pSrc1, pSrc2, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_32fc		)(const Fw32fc* pSrc1, const Fw32fc* pSrc2, Fw32fc* pSrcDst, int len)
{
	DEF_ADD::ADDX::C1::AddProduct_32fc data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_32fc> (data, pSrc1, pSrc2, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_64fc		)(const Fw64fc* pSrc1, const Fw64fc* pSrc2, Fw64fc* pSrcDst, int len)
{
	DEF_ADD::ADDX::C1::AddProduct_64fc data;
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_64fc> (data, pSrc1, pSrc2, pSrcDst, len);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_16s_Sfs	)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw16s* pSrcDst, int len, int scaleFactor)
{
	DEF_ADD::ADDX::C1::AddProduct_16s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_16s> (data, pSrc1, pSrc2, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_32s_Sfs	)(const Fw32s* pSrc1, const Fw32s* pSrc2, Fw32s* pSrcDst, int len, int scaleFactor)
{ 
	DEF_ADD::ADDX::C1::AddProduct_32s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_32s> (data, pSrc1, pSrc2, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAddProduct_16s32s_Sfs	)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw32s* pSrcDst, int len, int scaleFactor)
{
	DEF_ADD::ADDX::C1::AddProduct_16s32s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_ADD::ADDX::C1::AddProduct_16s32s> (data, pSrc1, pSrc2, pSrcDst, len);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
