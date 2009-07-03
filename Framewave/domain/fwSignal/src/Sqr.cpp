/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/SqrXDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;


FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32f> (data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_64f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_64f> (data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_32f_I)(Fw32f* pSrcDst, int len)				
{
	DEF_SQRX::SQR::C1::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32f>(data, pSrcDst, pSrcDst,  len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_64f_I)(Fw64f* pSrcDst, int len)				
{
	DEF_SQRX::SQR::C1::Sqr_64f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_64f>(data, pSrcDst, pSrcDst,  len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_8u_Sfs)(const Fw8u* pSrc, Fw8u* pDst, int len,int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_8u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<0>  > (data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_8u<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_8u<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<1>  > (data, pSrc, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16s_Sfs)(const Fw16s* pSrc, Fw16s* pDst, int len,int scaleFactor)	
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16s<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<0>   > (data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16s<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16s<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<1>  > (data, pSrc, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16u_Sfs)(const Fw16u* pSrc, Fw16u* pDst, int len,int scaleFactor)	
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<0>   > (data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16u<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16u<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<1>  > (data, pSrc, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_8u_ISfs)(Fw8u* pSrcDst, int len, int scaleFactor)			
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_8u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<0>  >(data, pSrcDst, pSrcDst,  len);
	}																										
	else if(scaleFactor < 0)																						
	{														   
		DEF_SQRX::SQR::C1::Sqr_8u<-1> data(scaleFactor);		   
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<-1> >(data, pSrcDst, pSrcDst,  len);
	}																										
	else																									
	{														   
		DEF_SQRX::SQR::C1::Sqr_8u<1> data(scaleFactor);			   
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<1>  >(data, pSrcDst, pSrcDst,  len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16s_ISfs)(Fw16s* pSrcDst, int len, int scaleFactor)		
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16s<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<0>  >(data, pSrcDst, pSrcDst,  len);
	}																										
	else if(scaleFactor < 0)																						
	{															
		DEF_SQRX::SQR::C1::Sqr_16s<-1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<-1> >(data, pSrcDst, pSrcDst,  len);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C1::Sqr_16s<1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<1>  >(data, pSrcDst, pSrcDst,  len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16u_ISfs)(Fw16u* pSrcDst, int len, int scaleFactor)		
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<0>  >(data, pSrcDst, pSrcDst,  len);
	}																										
	else if(scaleFactor < 0)																						
	{															
		DEF_SQRX::SQR::C1::Sqr_16u<-1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<-1> >(data, pSrcDst, pSrcDst,  len);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C1::Sqr_16u<1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<1>  >(data, pSrcDst, pSrcDst,  len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_32fc data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32fc> (data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_64fc data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_64fc> (data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_32fc_I)(Fw32fc* pSrcDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_32fc data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32fc>(data, pSrcDst, pSrcDst,  len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_64fc_I)(Fw64fc* pSrcDst, int len)
{
	DEF_SQRX::SQR::C1::Sqr_64fc data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_64fc>(data, pSrcDst, pSrcDst,  len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16sc_Sfs)(const Fw16sc* pSrc, Fw16sc* pDst, int len,int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16sc<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<0>   > (data, pSrc, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16sc<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<-1> >(data, pSrc, pDst, len);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16sc<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<1>  > (data, pSrc, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsSqr_16sc_ISfs)(Fw16sc* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16sc<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<0>  >(data, pSrcDst, pSrcDst,  len);
	}																										
	else if(scaleFactor < 0)																						
	{															
		DEF_SQRX::SQR::C1::Sqr_16sc<-1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<-1> >(data, pSrcDst, pSrcDst,  len);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C1::Sqr_16sc<1> data(scaleFactor);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16sc<1>  >(data, pSrcDst, pSrcDst,  len);
	}
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
