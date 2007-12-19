/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "ThresholdIdef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

//---------------------------------------------------------------------------------------------------//
//--------------------------------------iThreshold_GT_-----------------------------------------------//
//---------------------------------------------------------------------------------------------------//

// not in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold )
{ 
	Threshold_8u_C1R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold )
{ 
	Threshold_16s_C1R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold )
{ 
	Threshold_32f_C1R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}
// not in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3] )
{ 
	Threshold_8u_C3R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3] )
{ 
	Threshold_16s_C3R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3] )
{ 
	Threshold_32f_C3R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3] )
{ 
	Threshold_8u_AC4R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3] )
{ 
	Threshold_16s_AC4R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3] )
{ 
	Threshold_32f_AC4R_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);		
}

// in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold )
{ 
	Threshold_8u_C1IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold )
{ 
	Threshold_16s_C1IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold )
{ 
	Threshold_32f_C1IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

// in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3] )
{ 
	Threshold_8u_C3IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3] )
{ 
	Threshold_16s_C3IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3] )
{ 
	Threshold_32f_C3IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3] )
{ 
	Threshold_8u_AC4IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_8u_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3] )
{ 
	Threshold_16s_AC4IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_16s_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3] )
{ 
	Threshold_32f_AC4IR_GT data(threshold);
	return OPT_LEVEL::fe<Threshold_32f_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}


//---------------------------------------------------------------------------------------------------//
//------------------------------------iThreshold_GTVal_----------------------------------------------//
//---------------------------------------------------------------------------------------------------//

// not in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, Fw8u value )
{ 
	Threshold_Val_8u_C1R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, Fw16s value )
{ 
	Threshold_Val_16s_C1R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, Fw32f value )
{ 
	Threshold_Val_32f_C1R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
// not in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3] )
{ 
	Threshold_Val_8u_C3R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3] )
{ 
	Threshold_Val_16s_C3R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3] )
{ 
	Threshold_Val_32f_C3R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3] )
{ 
	Threshold_Val_8u_AC4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3] )
{ 
	Threshold_Val_16s_AC4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3] )
{ 
	Threshold_Val_32f_AC4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4] )
{ 
	Threshold_Val_8u_C4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4] )
{ 
	Threshold_Val_16s_C4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4] )
{ 
	Threshold_Val_32f_C4R_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}
// in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, Fw8u value )
{
	Threshold_Val_8u_C1IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, Fw16s value )
{
	Threshold_Val_16s_C1IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, Fw32f value )
{
	Threshold_Val_32f_C1IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
// in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3] )
{
	Threshold_Val_8u_C3IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3] )
{
	Threshold_Val_16s_C3IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3] )
{
	Threshold_Val_32f_C3IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3] )
{
	Threshold_Val_8u_AC4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3] )
{
	Threshold_Val_16s_AC4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3] )
{
	Threshold_Val_32f_AC4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4] )
{
	Threshold_Val_8u_C4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_8u_C4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4] )
{
	Threshold_Val_16s_C4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_16s_C4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4] )
{
	Threshold_Val_32f_C4IR_GT data(threshold,value);
	return OPT_LEVEL::fe<Threshold_Val_32f_C4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
