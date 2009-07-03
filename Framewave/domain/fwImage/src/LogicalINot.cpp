/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "logical.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

//Not

FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C1IR   )( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Not_All::Not_B1<Fw8u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B1<Fw8u,C1> >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C3IR   )( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Not_All::Not_B1_C3 dat;
	return OPT_LEVEL::fe< Not_All::Not_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_AC4IR   )( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Not_All::Not_B1_AC4 dat;
	return OPT_LEVEL::fe< Not_All::Not_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C4IR   )( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
{
	Not_All::Not_B1<Fw8u, C4> dat;
	return OPT_LEVEL::fe< Not_All::Not_B1<Fw8u, C4> >( dat, pSrcDst, srcDstStep, roiSize ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C1R  	)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	Not_All::Not_B2<Fw8u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B2<Fw8u, C1> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C3R  	)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	Not_All::Not_B2_C3 dat;
	return OPT_LEVEL::fe< Not_All::Not_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_AC4R  	)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	Not_All::Not_B2_AC4 dat;
	return OPT_LEVEL::fe< Not_All::Not_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C4R  	)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize)
{
	Not_All::Not_B2<Fw8u, C4> dat;
	return OPT_LEVEL::fe< Not_All::Not_B2<Fw8u, C4> >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
