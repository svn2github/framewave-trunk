/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AbsDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C1R)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C1::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_16s>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C3R)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C3::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C3::Abs_16s>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C4R)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C4::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C4::Abs_16s>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_AC4R)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi)
{
	DEF_ABS::ABS::AC4::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::AC4::Abs_16s>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C1IR	)( A16S *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C1::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_16s>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C3IR	)( A16S *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C3::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C3::Abs_16s>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C4IR	)( A16S *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C4::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C4::Abs_16s>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_AC4IR	)( A16S *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::AC4::Abs_16s data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::AC4::Abs_16s>(data, sd, sdStep, sd, sdStep, roi);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C1R)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C1::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32f>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C3R)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C3::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C3::Abs_32f>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C4R)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
{
	DEF_ABS::ABS::C4::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C4::Abs_32f>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_AC4R)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi)
{
	DEF_ABS::ABS::AC4::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::AC4::Abs_32f>(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C1IR	)( A32F *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C1::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C1::Abs_32f>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C3IR	)( A32F *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C3::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C3::Abs_32f>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C4IR	)( A32F *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::C4::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::C4::Abs_32f>(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_AC4IR	)( A32F *sd, int sdStep, ASZ roi )
{
	DEF_ABS::ABS::AC4::Abs_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABS::AC4::Abs_32f>(data, sd, sdStep, sd, sdStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
