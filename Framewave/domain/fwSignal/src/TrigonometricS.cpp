/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "TrigonometricDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

//Cos
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCos_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cos_32f_All dat;
	return OPT_LEVEL::fe<Cos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCos_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cos_32f_All dat;
	return OPT_LEVEL::fe<Cos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCos_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cos_32f_All dat;
	return OPT_LEVEL::fe<Cos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCos_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cos_64f_All dat;
	return OPT_LEVEL::fe<Cos_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCos_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cos_64f_All dat;
	return OPT_LEVEL::fe<Cos_64f_All> (dat, pSrc, pDst, len);
}

//Sin
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsSin_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sin_32f_All dat;
	return OPT_LEVEL::fe<Sin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsSin_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sin_32f_All dat;
	return OPT_LEVEL::fe<Sin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsSin_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sin_32f_All dat;
	return OPT_LEVEL::fe<Sin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsSin_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Sin_64f_All dat;
	return OPT_LEVEL::fe<Sin_64f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsSin_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Sin_64f_All dat;
	return OPT_LEVEL::fe<Sin_64f_All> (dat, pSrc, pDst, len);
}

//SinCos
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinCos_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst1, Fw32f* pDst2, int len)
{
	SinCos_32f_All dat;
	return OPT_LEVEL::fe<SinCos_32f_All> (dat, pSrc, pDst1, pDst2, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinCos_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst1, Fw32f* pDst2, int len)
{
	SinCos_32f_All dat;
	return OPT_LEVEL::fe<SinCos_32f_All> (dat, pSrc, pDst1, pDst2, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinCos_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst1, Fw32f* pDst2, int len)
{
	SinCos_32f_All dat;
	return OPT_LEVEL::fe<SinCos_32f_All> (dat, pSrc, pDst1, pDst2, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinCos_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst1, Fw64f* pDst2, int len)
{
	SinCos_64f_All dat;
	return OPT_LEVEL::fe<SinCos_64f_All> (dat, pSrc, pDst1, pDst2, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinCos_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst1, Fw64f* pDst2, int len)
{
	SinCos_64f_All dat;
	return OPT_LEVEL::fe<SinCos_64f_All> (dat, pSrc, pDst1, pDst2, len);
}

//Tan
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsTan_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tan_32f_All dat;
	return OPT_LEVEL::fe<Tan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsTan_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tan_32f_All dat;
	return OPT_LEVEL::fe<Tan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsTan_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tan_32f_All dat;
	return OPT_LEVEL::fe<Tan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTan_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Tan_64f_All dat;
	return OPT_LEVEL::fe<Tan_64f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsTan_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Tan_64f_All dat;
	return OPT_LEVEL::fe<Tan_64f_All> (dat, pSrc, pDst, len);
}

//Acos
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAcos_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acos_32f_All dat;
	return OPT_LEVEL::fe<Acos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAcos_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acos_32f_All dat;
	return OPT_LEVEL::fe<Acos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAcos_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acos_32f_All dat;
	return OPT_LEVEL::fe<Acos_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcos_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Acos_64f_All dat;
	return OPT_LEVEL::fe<Acos_64f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAcos_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Acos_64f_All dat;
	return OPT_LEVEL::fe<Acos_64f_All> (dat, pSrc, pDst, len);
}

//Asin
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAsin_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asin_32f_All dat;
	return OPT_LEVEL::fe<Asin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAsin_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asin_32f_All dat;
	return OPT_LEVEL::fe<Asin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAsin_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asin_32f_All dat;
	return OPT_LEVEL::fe<Asin_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsin_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Asin_64f_All dat;
	return OPT_LEVEL::fe<Asin_64f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAsin_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Asin_64f_All dat;
	return OPT_LEVEL::fe<Asin_64f_All> (dat, pSrc, pDst, len);
}

//Atan
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAtan_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atan_32f_All dat;
	return OPT_LEVEL::fe<Atan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAtan_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atan_32f_All dat;
	return OPT_LEVEL::fe<Atan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAtan_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atan_32f_All dat;
	return OPT_LEVEL::fe<Atan_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Atan_64f_All dat;
	return OPT_LEVEL::fe<Atan_64f_All> (dat, pSrc, pDst, len);
}
FwStatus  PREFIX_OPT(OPT_PREFIX, fwsAtan_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Atan_64f_All dat;
	return OPT_LEVEL::fe<Atan_64f_All> (dat, pSrc, pDst, len);
}

//Atan2
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan2_32f_A11 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Atan2_32f_All dat;
	return OPT_LEVEL::fe<Atan2_32f_All> (dat, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan2_32f_A21 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Atan2_32f_All dat;
	return OPT_LEVEL::fe<Atan2_32f_All> (dat, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan2_32f_A24 )(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f* pDst, int len)
{
	Atan2_32f_All dat;
	return OPT_LEVEL::fe<Atan2_32f_All> (dat, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan2_64f_A50 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len)
{
	Atan2_64f_All dat;
	return OPT_LEVEL::fe<Atan2_64f_All> (dat, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtan2_64f_A53 )(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f* pDst, int len)
{
	Atan2_64f_All dat;
	return OPT_LEVEL::fe<Atan2_64f_All> (dat, pSrc1, pSrc2, pDst, len);
}

//Hyperbolic Functions

//Cosh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCosh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cosh_32f_All dat;
	return OPT_LEVEL::fe<Cosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCosh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cosh_32f_All dat;
	return OPT_LEVEL::fe<Cosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCosh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cosh_32f_All dat;
	return OPT_LEVEL::fe<Cosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCosh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cosh_64f_All dat;
	return OPT_LEVEL::fe<Cosh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCosh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Cosh_64f_All dat;
	return OPT_LEVEL::fe<Cosh_64f_All> (dat, pSrc, pDst, len);
}

//Sinh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sinh_32f_All dat;
	return OPT_LEVEL::fe<Sinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sinh_32f_All dat;
	return OPT_LEVEL::fe<Sinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Sinh_32f_All dat;
	return OPT_LEVEL::fe<Sinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Sinh_64f_All dat;
	return OPT_LEVEL::fe<Sinh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsSinh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Sinh_64f_All dat;
	return OPT_LEVEL::fe<Sinh_64f_All> (dat, pSrc, pDst, len);
}


//Tanh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTanh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tanh_32f_All dat;
	return OPT_LEVEL::fe<Tanh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTanh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tanh_32f_All dat;
	return OPT_LEVEL::fe<Tanh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTanh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Tanh_32f_All dat;
	return OPT_LEVEL::fe<Tanh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTanh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Tanh_64f_All dat;
	return OPT_LEVEL::fe<Tanh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsTanh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Tanh_64f_All dat;
	return OPT_LEVEL::fe<Tanh_64f_All> (dat, pSrc, pDst, len);
}

//Acosh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcosh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acosh_32f_All dat;
	return OPT_LEVEL::fe<Acosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcosh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acosh_32f_All dat;
	return OPT_LEVEL::fe<Acosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcosh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Acosh_32f_All dat;
	return OPT_LEVEL::fe<Acosh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcosh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Acosh_64f_All dat;
	return OPT_LEVEL::fe<Acosh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAcosh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Acosh_64f_All dat;
	return OPT_LEVEL::fe<Acosh_64f_All> (dat, pSrc, pDst, len);
}

//Asinh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsinh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asinh_32f_All dat;
	return OPT_LEVEL::fe<Asinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsinh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asinh_32f_All dat;
	return OPT_LEVEL::fe<Asinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsinh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Asinh_32f_All dat;
	return OPT_LEVEL::fe<Asinh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsinh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Asinh_64f_All dat;
	return OPT_LEVEL::fe<Asinh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAsinh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Asinh_64f_All dat;
	return OPT_LEVEL::fe<Asinh_64f_All> (dat, pSrc, pDst, len);
}


//Atanh
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtanh_32f_A11 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atanh_32f_All dat;
	return OPT_LEVEL::fe<Atanh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtanh_32f_A21 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atanh_32f_All dat;
	return OPT_LEVEL::fe<Atanh_32f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtanh_32f_A24 )(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Atanh_32f_All dat;
	return OPT_LEVEL::fe<Atanh_32f_All> (dat, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtanh_64f_A50 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Atanh_64f_All dat;
	return OPT_LEVEL::fe<Atanh_64f_All> (dat, pSrc, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAtanh_64f_A53 )(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Atanh_64f_All dat;
	return OPT_LEVEL::fe<Atanh_64f_All> (dat, pSrc, pDst, len);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
