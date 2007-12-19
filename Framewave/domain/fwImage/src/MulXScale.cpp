/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/MulDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;


// =============================
//			MulScale
// =============================
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C1IR  )( cA8U  *s,  int sStep,   A8U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulScale_8u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C3IR  )( cA8U  *s,  int sStep,   A8U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulScale_8u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C4IR  )( cA8U  *s,  int sStep,   A8U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulScale_8u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_AC4IR )( cA8U  *s,  int sStep,   A8U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulScale_8u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C1R   )( cA8U  *s1, int s1Step, cA8U  *s2, int s2Step, A8U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulScale_8u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C3R   )( cA8U  *s1, int s1Step, cA8U  *s2, int s2Step, A8U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulScale_8u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C4R   )( cA8U  *s1, int s1Step, cA8U  *s2, int s2Step, A8U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulScale_8u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_AC4R  )( cA8U  *s1, int s1Step, cA8U  *s2, int s2Step, A8U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulScale_8u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulScale_8u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C1IR  )( cA16U  *s,  int sStep,   A16U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulScale_16u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C3IR  )( cA16U  *s,  int sStep,   A16U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulScale_16u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C4IR  )( cA16U  *s,  int sStep,   A16U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulScale_16u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_AC4IR )( cA16U  *s,  int sStep,   A16U  *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulScale_16u>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C1R   )( cA16U  *s1, int s1Step, cA16U  *s2, int s2Step, A16U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulScale_16u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C3R   )( cA16U  *s1, int s1Step, cA16U  *s2, int s2Step, A16U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulScale_16u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C4R   )( cA16U  *s1, int s1Step, cA16U  *s2, int s2Step, A16U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulScale_16u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_AC4R  )( cA16U  *s1, int s1Step, cA16U  *s2, int s2Step, A16U  *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulScale_16u data;
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulScale_16u>(data, s1, s1Step, s2, s2Step, d, dStep, roi);
}



// =============================
//			MulCScale
// =============================
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C1IR  )(  A8U v,    A8U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C1::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulCScale_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C3IR  )( cA8U v[3], A8U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C3::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulCScale_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_AC4IR )( cA8U v[3], A8U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::AC4::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulCScale_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C4IR  )( cA8U v[4], A8U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C4::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulCScale_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C1R   )( cA8U *s, int sStep,  A8U v,    A8U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulCScale_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C3R   )( cA8U *s, int sStep, cA8U v[3], A8U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulCScale_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_AC4R  )( cA8U *s, int sStep, cA8U v[4], A8U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulCScale_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C4R   )( cA8U *s, int sStep, cA8U v[4], A8U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulCScale_8u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulCScale_8u>(data, s, sStep, d, dStep, roi);
}



ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C1IR  )(  A16U v,    A16U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C1::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulCScale_16u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C3IR  )( cA16U v[3], A16U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C3::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulCScale_16u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_AC4IR )( cA16U v[3], A16U *sd, int sdStep, ASZ roi )
{	
	DEF_MUL::MULXSCALE::AC4::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulCScale_16u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C4IR  )( cA16U v[4], A16U *sd, int sdStep, ASZ roi )
{
	DEF_MUL::MULXSCALE::C4::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulCScale_16u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C1R   )( cA16U *s, int sStep,  A16U v,    A16U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C1::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C1::MulCScale_16u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C3R   )( cA16U *s, int sStep, cA16U v[3], A16U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C3::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C3::MulCScale_16u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_AC4R  )( cA16U *s, int sStep, cA16U v[4], A16U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::AC4::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::AC4::MulCScale_16u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C4R   )( cA16U *s, int sStep, cA16U v[4], A16U *d, int dStep, ASZ roi)
{
	DEF_MUL::MULXSCALE::C4::MulCScale_16u data(v);
	return OPT_LEVEL::fe< DEF_MUL::MULXSCALE::C4::MulCScale_16u>(data, s, sStep, d, dStep, roi);
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
