/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "StatisticsDotProduct.h"
#include "fwSignal.h"


using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32f)(const Fw32f* pSrc1, const Fw32f* pSrc2, int len,Fw32f* pDp)
{

      FwStatus retStatus;
      StatDotProd_32f data;
      retStatus = OPT_LEVEL::fex<StatDotProd_32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
      *pDp = FW_REF::Limits<F32>::Sat(data.sum);
      return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32fc)(const Fw32fc* pSrc1, const Fw32fc* pSrc2, int len,Fw32fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_32fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = (Fw32f)data.sum.re;
	pDp->im = (Fw32f)data.sum.im;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32f32fc)(const Fw32f* pSrc1, const Fw32fc* pSrc2, int len, Fw32fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_32f32fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32f32fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = data.sum.re;
	pDp->im = data.sum.im;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32f64f)(const Fw32f* pSrc1, const Fw32f* pSrc2, int len,Fw64f* pDp)
{	
	FwStatus retStatus;
	StatDotProd_32f64f data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32f64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32fc64fc)(const Fw32fc* pSrc1, const Fw32fc* pSrc2, int len, Fw64fc* pDp)
{	
	FwStatus retStatus;
	StatDotProd_32fc64fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32fc64fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32f32fc64fc)(const Fw32f* pSrc1, const Fw32fc* pSrc2,int len, Fw64fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_32f32fc64fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32f32fc64fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_64f)(const Fw64f* pSrc1, const Fw64f* pSrc2, int len,Fw64f* pDp)
{
	FwStatus retStatus;
	StatDotProd_64f data;
	retStatus = OPT_LEVEL::fex<StatDotProd_64f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_64fc)(const Fw64fc* pSrc1, const Fw64fc* pSrc2, int len,Fw64fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_64fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_64fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_64f64fc)(const Fw64f* pSrc1, const Fw64fc* pSrc2, int len, Fw64fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_64f64fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_64f64fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s64s)(const Fw16s* pSrc1, const Fw16s* pSrc2, int len,Fw64s* pDp)
{
	FwStatus retStatus;
	StatDotProd_16s64s data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s64s>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16sc64sc)(const Fw16sc* pSrc1, const Fw16sc* pSrc2, int len, Fw64sc* pDp)
{
	FwStatus retStatus;
	StatDotProd_16sc64sc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16sc64sc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S64>::Sat(data.sum.re);
	pDp->im = FW_REF::Limits<S64>::Sat(data.sum.im);
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s16sc64sc)(const Fw16s* pSrc1, const Fw16sc* pSrc2,int len, Fw64sc* pDp)
{
	FwStatus retStatus;
	StatDotProd_16s16sc64sc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s16sc64sc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s32f)(const Fw16s* pSrc1, const Fw16s* pSrc2, int len,Fw32f* pDp)
{
	FwStatus retStatus;
	StatDotProd_16s32f data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s32f>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = (Fw32f)data.sum;
	return retStatus;	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16sc32fc)(const Fw16sc* pSrc1, const Fw16sc* pSrc2, int len, Fw32fc* pDp)
{

	FwStatus retStatus;
	StatDotProd_16sc32fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16sc32fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = (Fw32f)data.sum.re;
	pDp->im = (Fw32f)data.sum.im;
	return retStatus;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s16sc32fc)(const Fw16s* pSrc1, const Fw16sc* pSrc2,int len, Fw32fc* pDp)
{
	FwStatus retStatus;
	StatDotProd_16s16sc32fc data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s16sc32fc>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = (Fw32f)data.sum.re;
	pDp->im = (Fw32f)data.sum.im;
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s_Sfs)(const Fw16s* pSrc1, const Fw16s* pSrc2, int len, Fw16s* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = FW_REF::Limits<S16>::Sat(FW_REF::Scale(data.sum,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16sc_Sfs)(const Fw16sc* pSrc1, const Fw16sc* pSrc2, int len, Fw16sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S16>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S16>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32s_Sfs)(const Fw32s* pSrc1, const Fw32s* pSrc2, int len, Fw32s* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32sc_Sfs)(const Fw32sc* pSrc1, const Fw32sc* pSrc2, int len, Fw32sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_32sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s32s_Sfs)(const Fw16s* pSrc1, const Fw16s* pSrc2, int len, Fw32s* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s32s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s16sc32sc_Sfs)(const Fw16s* pSrc1, const Fw16sc* pSrc2, int len, Fw32sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16s16sc32sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s16sc32sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s32s32s_Sfs)(const Fw16s* pSrc1, const Fw32s* pSrc2,int len, Fw32s* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16s32s32s_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s32s32s_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	*pDp = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16s16sc_Sfs)(const Fw16s* pSrc1, const Fw16sc* pSrc2,int len, Fw16sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16s16sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16s16sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S16>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S16>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_16sc32sc_Sfs)(const Fw16sc* pSrc1, const Fw16sc* pSrc2,int len, Fw32sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_16sc32sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_16sc32sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsDotProd_32s32sc_Sfs)(const Fw32s* pSrc1, const Fw32sc* pSrc2,int len, Fw32sc* pDp, int scaleFactor)
{
	FwStatus retStatus;
	StatDotProd_32s32sc_Sfs data;
	retStatus = OPT_LEVEL::fex<StatDotProd_32s32sc_Sfs>(data,pSrc1,pSrc2,len,THREAD_SW_OFF);
	pDp->re = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.re,scaleFactor));
	pDp->im = FW_REF::Limits<S32>::Sat(FW_REF::Scale(data.sum.im,scaleFactor));
	return retStatus;
}
// end of dot product

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
