/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/MulDef.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_8u_Sfs)(const Fw8u* pSrc1, const Fw8u* pSrc2, Fw8u* pDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_8u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<0> > (data, pSrc1, pSrc2, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_8u<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<-1> >(data, pSrc1, pSrc2, pDst, len);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C1::Mul_8u<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<1> > (data, pSrc1, pSrc2, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s_Sfs)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw16s* pDst,int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_16s<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> > (data, pSrc1, pSrc2, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_16s<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<-1> >(data, pSrc1, pSrc2, pDst, len);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C1::Mul_16s<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<1> > (data, pSrc1, pSrc2, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16sc_Sfs)(const Fw16sc* pSrc1, const Fw16sc* pSrc2, Fw16sc *pDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_16sc<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<0> > (data, pSrc1, pSrc2, pDst, len);
	}																							
	else if(scaleFactor < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_16sc<-1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<-1> >(data, pSrc1, pSrc2, pDst, len);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C1::Mul_16sc<1> data(scaleFactor);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<1> > (data, pSrc1, pSrc2, pDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32sc_Sfs)(const Fw32sc* pSrc1, const Fw32sc* pSrc2, Fw32sc *pDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32sc data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32sc> (data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_8u_ISfs)(const Fw8u* pSrc, Fw8u* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_8u<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<0> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else if(scaleFactor < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_8u<-1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<-1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else																							
	{
		DEF_MUL::MUL::C1::Mul_8u<1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s_ISfs)(const Fw16s* pSrc, Fw16s* pSrcDst, int len, int scaleFactor)							
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_16s<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else if(scaleFactor < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_16s<-1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<-1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else																							
	{
		DEF_MUL::MUL::C1::Mul_16s<1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16sc_ISfs)(const Fw16sc* pSrc, Fw16sc* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor == 0)
	{
		DEF_MUL::MUL::C1::Mul_16sc<0> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<0> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else if(scaleFactor < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_16sc<-1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<-1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}																								
	else																							
	{
		DEF_MUL::MUL::C1::Mul_16sc<1> data(scaleFactor);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<1> >(data, pSrc, pSrcDst, pSrcDst, len);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32sc_ISfs)(const Fw32sc* pSrc, Fw32sc* pSrcDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32sc data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32sc>(data, pSrc, pSrcDst, pSrcDst, len);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32s_Sfs)(const Fw32s* pSrc1, const Fw32s* pSrc2, Fw32s* pDst,int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32s>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32s_ISfs)(const Fw32s* pSrc, Fw32s* pSrcDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32s>(data, pSrc, pSrcDst, pSrcDst, len);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16u16s_Sfs)(const Fw16u* pSrc1, const Fw16s* pSrc2, Fw16s *pDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_16u16s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16u16s>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s32s_Sfs)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw32s *pDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_16s32s data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s32s>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32s32sc_Sfs)(const Fw32s* pSrc1, const Fw32sc* pSrc2, Fw32sc *pDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32s32sc data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32s32sc>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32s32sc_ISfs)(const Fw32s* pSrc, Fw32sc* pSrcDst, int len, int scaleFactor)
{
	DEF_MUL::MUL::C1::Mul_32s32sc data(scaleFactor);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32s32sc>(data, pSrc, pSrcDst, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_8u16u)(const Fw8u* pSrc1, const Fw8u* pSrc2,Fw16u *pDst,int len)
{
	DEF_MUL::MUL::C1::Mul_8u16u data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u16u>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s32f)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw32f *pDst,int len)
{
	DEF_MUL::MUL::C1::Mul_16s32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s32f>(data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s)(const Fw16s* pSrc1, const Fw16s* pSrc2, Fw16s *pDst,int len)		
{
	DEF_MUL::MUL::C1::Mul_16s<0> data(0);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> > (data, pSrc1, pSrc2, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32f)(const Fw32f* pSrc1, const Fw32f* pSrc2, Fw32f *pDst,int len)		
{
	DEF_MUL::MUL::C1::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32f> (data, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_64f)(const Fw64f* pSrc1, const Fw64f* pSrc2, Fw64f *pDst,int len)		
{
	DEF_MUL::MUL::C1::Mul_64f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_64f> (data, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32fc)(const Fw32fc* pSrc1, const Fw32fc* pSrc2, Fw32fc *pDst, int len)	
{
	DEF_MUL::MUL::C1::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32fc> (data, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_64fc)(const Fw64fc* pSrc1, const Fw64fc* pSrc2, Fw64fc *pDst, int len)	
{
	DEF_MUL::MUL::C1::Mul_64fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_64fc> (data, pSrc1, pSrc2, pDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_16s_I)(const Fw16s* pSrc, Fw16s* pSrcDst, int len)						
{
	DEF_MUL::MUL::C1::Mul_16s<0> data(0);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> >(data, pSrc, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32f_I)(const Fw32f* pSrc, Fw32f* pSrcDst, int len)						
{
	DEF_MUL::MUL::C1::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32f>(data, pSrc, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_64f_I)(const Fw64f* pSrc, Fw64f* pSrcDst, int len)						
{
	DEF_MUL::MUL::C1::Mul_64f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_64f>(data, pSrc, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_32fc_I)(const Fw32fc* pSrc, Fw32fc* pSrcDst, int len)					
{
	DEF_MUL::MUL::C1::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32fc>(data, pSrc, pSrcDst, pSrcDst, len);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsMul_64fc_I)(const Fw64fc* pSrc, Fw64fc* pSrcDst, int len)					
{
	DEF_MUL::MUL::C1::Mul_64fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_64fc>(data, pSrc, pSrcDst, pSrcDst, len);
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
