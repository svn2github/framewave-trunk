/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ScaleDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// 8U16U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C1R )( cA8U * s, int sStep, A16U * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C1,A16U,C1> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C1,A16U,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C3R )( cA8U * s, int sStep, A16U * d, int dStep, ASZ roi )
{
	ScaleTo_Integer<A8U,C3,A16U,C3> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C3,A16U,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C4R )( cA8U * s, int sStep, A16U * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C4,A16U,C4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C4,A16U,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_AC4R)( cA8U * s, int sStep, A16U * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer_AC4<A8U,AC4,A16U,AC4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer_AC4<A8U,AC4,A16U,AC4> > (dat, s, sStep, d, dStep, roi);
}

// 8U16S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C1R )( cA8U * s, int sStep, A16S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C1,A16S,C1> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C1,A16S,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C3R )( cA8U * s, int sStep, A16S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C3,A16S,C3> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C3,A16S,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C4R )( cA8U * s, int sStep, A16S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C4,A16S,C4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C4,A16S,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_AC4R)( cA8U * s, int sStep, A16S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer_AC4<A8U,AC4,A16S,AC4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer_AC4<A8U,AC4,A16S,AC4> > (dat, s, sStep, d, dStep, roi);
}

// 8U32S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C1R )( cA8U * s, int sStep, A32S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C1,A32S,C1> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C1,A32S,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C3R )( cA8U * s, int sStep, A32S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C3,A32S,C3> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C3,A32S,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C4R )( cA8U * s, int sStep, A32S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer<A8U,C4,A32S,C4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer<A8U,C4,A32S,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_AC4R)( cA8U * s, int sStep, A32S * d, int dStep, ASZ roi )
{ 
	ScaleTo_Integer_AC4<A8U,AC4,A32S,AC4> dat;
	return OPT_LEVEL::fe< ScaleTo_Integer_AC4<A8U,AC4,A32S,AC4> > (dat, s, sStep, d, dStep, roi);
}

// 8U32F
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C1R )( cA8U * s, int sStep, A32F * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{ 
	ScaleTo_Float<A8U,C1, A32F,C1> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleTo_Float<A8U,C1, A32F,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C3R )( cA8U * s, int sStep, A32F * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleTo_Float<A8U,C3, A32F,C3> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleTo_Float<A8U,C3, A32F,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C4R )( cA8U * s, int sStep, A32F * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleTo_Float<A8U,C4, A32F,C4> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleTo_Float<A8U,C4, A32F,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_AC4R)( cA8U * s, int sStep, A32F * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleTo_Float_AC4<A8U,AC4,A32F,AC4> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleTo_Float_AC4<A8U,AC4,A32F,AC4> > (dat, s, sStep, d, dStep, roi);
}

// 32F8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C1R )( cA32F * s, int sStep, A8U * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleFrom_Float<A32F,C1,A8U,C1> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleFrom_Float<A32F,C1,A8U,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C3R )( cA32F * s, int sStep, A8U * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleFrom_Float<A32F,C3,A8U,C3> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleFrom_Float<A32F,C3,A8U,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C4R )( cA32F * s, int sStep, A8U * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleFrom_Float<A32F,C4,A8U,C4> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleFrom_Float<A32F,C4,A8U,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_AC4R)( cA32F * s, int sStep, A8U * d, int dStep, ASZ roi, A32F vMin, A32F vMax )
{
	ScaleFrom_Float_AC4<A32F,AC4,A8U,AC4> dat(vMin, vMax);
	return OPT_LEVEL::fe< ScaleFrom_Float_AC4<A32F,AC4,A8U,AC4> > (dat, s, sStep, d, dStep, roi);
}

// 16U8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C1R )( cA16U * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16U, C1, A8U, C1> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16U, C1, A8U, C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C3R )( cA16U * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16U, C3, A8U, C3> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16U, C3, A8U, C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C4R )( cA16U * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16U, C4, A8U, C4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16U, C4, A8U, C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_AC4R)( cA16U * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint_AC4<A16U, AC4,A8U, AC4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint_AC4<A16U, AC4,A8U, AC4> > (dat, s, sStep, d, dStep, roi);
}

// 16S8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C1R )( cA16S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16S, C1, A8U, C1> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16S, C1, A8U, C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C3R )( cA16S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16S, C3, A8U, C3> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16S, C3, A8U, C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C4R )( cA16S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A16S, C4, A8U, C4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A16S, C4, A8U, C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_AC4R)( cA16S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint_AC4<A16S, AC4,A8U, AC4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint_AC4<A16S, AC4,A8U, AC4> > (dat, s, sStep, d, dStep, roi);
}

// 32S8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C1R )( cA32S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A32S,C1,A8U,C1> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A32S,C1,A8U,C1> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C3R )( cA32S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A32S,C3,A8U,C3> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A32S,C3,A8U,C3> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C4R )( cA32S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint<A32S,C4,A8U,C4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint<A32S,C4,A8U,C4> > (dat, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_AC4R)( cA32S * s, int sStep, A8U * d, int dStep, ASZ roi, FwHintAlgorithm hint )
{
	Scale_Hint_AC4<A32S,AC4,A8U,AC4> dat(hint);
	return OPT_LEVEL::fe< Scale_Hint_AC4<A32S,AC4,A8U,AC4> > (dat, s, sStep, d, dStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR 
