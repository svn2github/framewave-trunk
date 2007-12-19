/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/DivDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;


// =============================
//			DivC
// =============================
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C1IRSfs	)(  A8U v,    A8U *sd, int sdStep, ASZ roi, int scale)
{
	if( v == 0) return  fwStsDivByZeroErr;

	DEF_DIV::DIVC::C1::DivC_8u data(scale, v);
		return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C3IRSfs   )( cA8U v[3], A8U *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_8u data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_AC4IRSfs  )( cA8U v[3], A8U *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_8u data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_8u>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C1RSfs  	)( cA8U *s, int sStep,  A8U v,    A8U *d, int dStep, ASZ roi, int scale)
{
	if( v == 0) return  fwStsDivByZeroErr;

	DEF_DIV::DIVC::C1::DivC_8u data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C3RSfs	)( cA8U *s, int sStep, cA8U v[3], A8U *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_8u data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_8u>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_AC4RSfs  	)( cA8U *s, int sStep, cA8U v[3], A8U *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_8u data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_8u>(data, s, sStep, d, dStep, roi);
}

//16s

ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C1IRSfs	)(  A16S v,    A16S *sd, int sdStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16s>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C3IRSfs  )( cA16S v[3], A16S *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_16s>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_AC4IRSfs )( cA16S v[3], A16S *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_16s>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C1RSfs 	)( cA16S *s, int sStep,  A16S v,    A16S *d, int dStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16s>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C3RSfs	)( cA16S *s, int sStep, cA16S v[3], A16S *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_16s>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_AC4RSfs 	)( cA16S *s, int sStep, cA16S v[3], A16S *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_16s data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_16s>(data, s, sStep, d, dStep, roi);
}

//16sc
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C1IRSfs	)(  A16SC v,    A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16sc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C3IRSfs )( cA16SC v[3], A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_16sc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_AC4IRSfs)( cA16SC v[3], A16SC *sd, int sdStep, ASZ roi, int scale)
{	
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_16sc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C1RSfs	)( cA16SC *s, int sStep,  A16SC v,    A16SC *d, int dStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_16sc>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C3RSfs	)( cA16SC *s, int sStep, cA16SC v[3], A16SC *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_16sc>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_AC4RSfs	)( cA16SC *s, int sStep, cA16SC v[3], A16SC *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_16sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_16sc>(data, s, sStep, d, dStep, roi);
}
//32sc
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C1IRSfs	)(  A32SC v,    A32SC *sd, int sdStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32sc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C3IRSfs )( cA32SC v[3], A32SC *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32sc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_AC4IRSfs)( cA32SC v[3], A32SC *sd, int sdStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32sc>(data, sd, sdStep, sd, sdStep, roi);
 }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C1RSfs	)( cA32SC *s, int sStep,  A32SC v,    A32SC *d, int dStep, ASZ roi, int scale)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32sc>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C3RSfs	)( cA32SC *s, int sStep, cA32SC v[3], A32SC *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32sc>(data, s, sStep, d, dStep, roi);
 }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_AC4RSfs	)( cA32SC *s, int sStep, cA32SC v[3], A32SC *d, int dStep, ASZ roi, int scale)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32sc>(data, s, sStep, d, dStep, roi);
 }

//32f
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C1IR  	)(  A32F v,    A32F *sd, int sdStep, ASZ roi)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32f>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C3IR		)( cA32F v[3], A32F *sd, int sdStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0))
		return fwStsDivByZeroErr;

	DEF_DIV::DIVC::C3::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32f>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_AC4IR	)( cA32F v[3], A32F *sd, int sdStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0))
		return fwStsDivByZeroErr;

	DEF_DIV::DIVC::AC4::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32f>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C1R		)( cA32F *s, int sStep,  A32F v,    A32F *d, int dStep, ASZ roi)
{
	if( v == 0) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C1::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32f>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C3R  	)( cA32F *s, int sStep, cA32F v[3], A32F *d, int dStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0))
		return fwStsDivByZeroErr;

	DEF_DIV::DIVC::C3::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32f>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_AC4R		)( cA32F *s, int sStep, cA32F v[3], A32F *d, int dStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0))
		return fwStsDivByZeroErr;

	DEF_DIV::DIVC::AC4::DivC_32f data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32f>(data, s, sStep, d, dStep, roi);
}
//32fc
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C1IR 	)(  A32FC v,    A32FC *sd, int sdStep, ASZ roi)
{
	if( v == 0) return fwStsDivByZeroErr ;

	Fw32f denom = (v.re * v.re) + (v.im * v.im);
	
	FwiSize nroi = roi;
	nroi.width *= 2;

	if ((denom != denom) || ((denom - denom) != 0))
		return fwiSet_32f_C1R(0, (Fw32f*) sd, sdStep, nroi);

	DEF_DIV::DIVC::C1::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32fc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C3IR	)( cA32FC v[3], A32FC *sd, int sdStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32fc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_AC4IR	)( cA32FC v[3], A32FC *sd, int sdStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32fc>(data, sd, sdStep, sd, sdStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C1R		)( cA32FC *s, int sStep,  A32FC v,    A32FC *d, int dStep, ASZ roi)
{
	if	( v == 0) return fwStsDivByZeroErr ;

	Fw32f denom = (v.re * v.re) + (v.im * v.im);
	
	FwiSize nroi = roi;
	nroi.width *= 2;

	if ((denom != denom) || ((denom - denom) != 0))
		return fwiSet_32f_C1R(0, (Fw32f*) d, dStep, nroi);

	DEF_DIV::DIVC::C1::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C1::DivC_32fc>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C3R 	)( cA32FC *s, int sStep, cA32FC v[3], A32FC *d, int dStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::C3::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::C3::DivC_32fc>(data, s, sStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_AC4R	)( cA32FC *s, int sStep, cA32FC v[3], A32FC *d, int dStep, ASZ roi)
{
	if ((v[0] == 0) || (v[1] == 0) || (v[2] == 0)) return fwStsDivByZeroErr ;

	DEF_DIV::DIVC::AC4::DivC_32fc data(v);
	return OPT_LEVEL::fe<DEF_DIV::DIVC::AC4::DivC_32fc>(data, s, sStep, d, dStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
