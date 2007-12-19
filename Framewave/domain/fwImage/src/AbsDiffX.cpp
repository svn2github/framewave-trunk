/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AbsDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// =============================
//			AbsDiff*
// =============================
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_8u_C1R  )( cA8U  *s1, int s1Step, cA8U  *s2, int s2Step, A8U  *d, int dStep, ASZ roi)
{
	DEF_ABS::ABSDIFF::C1::AbsDiff_8u data;
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFF::C1::AbsDiff_8u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_16u_C1R )( cA16U *s1, int s1Step, cA16U *s2, int s2Step, A16U *d, int dStep, ASZ roi)
{
	DEF_ABS::ABSDIFF::C1::AbsDiff_16u data;
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFF::C1::AbsDiff_16u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_32f_C1R )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *d, int dStep, ASZ roi)
{
	DEF_ABS::ABSDIFF::C1::AbsDiff_32f data;
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFF::C1::AbsDiff_32f>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

// =============================
//			AbsDiffC*
// =============================
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_8u_C1R )( cA8U  *s, int sStep,  A8U  *d, int dStep, ASZ roi, int  v )
{
	DEF_ABS::ABSDIFFC::C1::AbsDiffC_8u data( FW_REF::Limits<Fw8u>::Sat(v) );
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFFC::C1::AbsDiffC_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_16u_C1R)( cA16U *s, int sStep,  A16U *d, int dStep, ASZ roi, int  v )
{
	DEF_ABS::ABSDIFFC::C1::AbsDiffC_16u data(FW_REF::Limits<Fw16u>::Sat(v));
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFFC::C1::AbsDiffC_16u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_32f_C1R)( cA32F *s, int sStep,  A32F *d, int dStep, ASZ roi, A32F v )
{
	DEF_ABS::ABSDIFFC::C1::AbsDiffC_32f data(v);
	return OPT_LEVEL::fe<DEF_ABS::ABSDIFFC::C1::AbsDiffC_32f>(data, s, sStep, d, dStep, roi);
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
