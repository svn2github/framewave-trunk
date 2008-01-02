/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/AddDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C1IRSfs )(  A8U v,    A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0 && v == 0)
	{
		return fwStsNoErr;
	}
	else
	{
		if(scale == 0)
		{
			DEF_ADD::ADDC::C1::AddC_8u<0> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<0> >(data, sd, sdStep, sd, sdStep, roi);
		}																							
		else if(scale < 0)																			
		{																							
			DEF_ADD::ADDC::C1::AddC_8u<-1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<-1> >(data, sd, sdStep, sd, sdStep, roi);
		}																							
		else																						
		{																							
			DEF_ADD::ADDC::C1::AddC_8u<1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<1> >(data, sd, sdStep, sd, sdStep, roi);
		}
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C3IRSfs )( cA8U v[3], A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_8u_scale_zero<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u_scale_zero<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::C3::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::C3::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_AC4IRSfs)( cA8U v[3], A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_8u<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C4IRSfs )( cA8U v[4], A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C4::AddC_8u<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::C4::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::C4::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C1RSfs  )( cA8U *s, int sStep,  A8U v,    A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0 && v == 0)
	{
		return fwiCopy_8u_C1R(s, sStep, d, dStep, roi);
	}
	else
	{
		if(scale == 0)
		{
			DEF_ADD::ADDC::C1::AddC_8u<0> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<0> >(data, s, sStep, d, dStep, roi);
		}																							
		else if(scale < 0)																			
		{																							
			DEF_ADD::ADDC::C1::AddC_8u<-1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<-1> >(data, s, sStep, d, dStep, roi);
		}																							
		else																						
		{																							
			DEF_ADD::ADDC::C1::AddC_8u<1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_8u<1> >(data, s, sStep, d, dStep, roi);
		}
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C3RSfs  )( cA8U *s, int sStep, cA8U v[3], A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_8u_scale_zero<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u_scale_zero<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C3::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C3::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_8u<1> >(data, s, sStep, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_AC4RSfs )( cA8U *s, int sStep, cA8U v[3], A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_8u<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::AC4::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::AC4::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_8u<1> >(data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C4RSfs  )( cA8U *s, int sStep, cA8U v[4], A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C4::AddC_8u<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C4::AddC_8u<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C4::AddC_8u<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_8u<1> >(data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C1IRSfs )(  A16S v,    A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0 && v == 0)
	{
		return fwStsNoErr;
	}
	else
	{
		if(scale == 0)
		{
			DEF_ADD::ADDC::C1::AddC_16s<0> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<0> >(data, sd, sdStep, sd, sdStep, roi);
		}																							
		else if(scale < 0)																			
		{																							
			DEF_ADD::ADDC::C1::AddC_16s<-1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<-1> >(data, sd, sdStep, sd, sdStep, roi);
		}		
        else if(scale == 1)
        {
			DEF_ADD::ADDC::C1::AddC_16s<2> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<2> >(data, sd, sdStep, sd, sdStep, roi);
        }
		else																						
		{																							
			DEF_ADD::ADDC::C1::AddC_16s<1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<1> >(data, sd, sdStep, sd, sdStep, roi);
		}
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C3IRSfs )( cA16S v[3], A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::C3::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::C3::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_AC4IRSfs)( cA16S v[3], A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C4IRSfs )( cA16S v[4], A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C4::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::C4::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::C4::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C1RSfs  )( cA16S *s, int sStep,  A16S v,    A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0 && v == 0)
	{
		return fwiCopy_16s_C1R(s, sStep, d, dStep, roi);
	}
	else
	{
		if(scale == 0)
		{
			DEF_ADD::ADDC::C1::AddC_16s<0> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<0> >(data, s, sStep, d, dStep, roi);
		}																							
		else if(scale < 0)																			
		{																							
			DEF_ADD::ADDC::C1::AddC_16s<-1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<-1> >(data, s, sStep, d, dStep, roi);
		}																							
		else																						
		{																							
			DEF_ADD::ADDC::C1::AddC_16s<1> data(scale, v);
			return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16s<1> >(data, s, sStep, d, dStep, roi);
		}
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C3RSfs  )( cA16S *s, int sStep, cA16S v[3], A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C3::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C3::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16s<1> >(data, s, sStep, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_AC4RSfs )( cA16S *s, int sStep, cA16S v[3], A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::AC4::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::AC4::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16s<1> >(data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C4RSfs  )( cA16S *s, int sStep, cA16S v[4], A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C4::AddC_16s<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C4::AddC_16s<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C4::AddC_16s<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_16s<1> >(data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C1IRSfs )(  A16SC v,    A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C1::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C1::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C1::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C3IRSfs )( cA16SC v[3], A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::C3::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::C3::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_AC4IRSfs)( cA16SC v[3], A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<0> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else if(scale < 0)																							  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<-1> >(data, sd, sdStep, sd, sdStep, roi);
	}																											  
	else																										  
	{																											  
		DEF_ADD::ADDC::AC4::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<1> >(data, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C1RSfs  )( cA16SC *s, int sStep,  A16SC v,    A16SC *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C1::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C1::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C1::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_16sc<1> >(data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C3RSfs  )( cA16SC *s, int sStep, cA16SC v[3], A16SC *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::C3::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::C3::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::C3::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_16sc<1> >(data, s, sStep, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_AC4RSfs )( cA16SC *s, int sStep, cA16SC v[3], A16SC *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_ADD::ADDC::AC4::AddC_16sc<0> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<0> >(data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																							
		DEF_ADD::ADDC::AC4::AddC_16sc<-1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																							
		DEF_ADD::ADDC::AC4::AddC_16sc<1> data(scale, v);
		return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_16sc<1> >(data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C1IRSfs )(  A32SC v,    A32SC *sd, int sdStep, ASZ roi, int scale)
{
	DEF_ADD::ADDC::C1::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32sc >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C3IRSfs )( cA32SC v[3], A32SC *sd, int sdStep, ASZ roi, int scale)
{
	DEF_ADD::ADDC::C3::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32sc >(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_AC4IRSfs)( cA32SC v[3], A32SC *sd, int sdStep, ASZ roi, int scale)
{
	DEF_ADD::ADDC::AC4::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32sc >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C1RSfs  )( cA32SC *s, int sStep,  A32SC v,    A32SC *d, int dStep, ASZ roi, int scale)
{ 
	DEF_ADD::ADDC::C1::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32sc >(data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C3RSfs  )( cA32SC *s, int sStep, cA32SC v[3], A32SC *d, int dStep, ASZ roi, int scale)
{ 
	DEF_ADD::ADDC::C3::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32sc >(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_AC4RSfs )( cA32SC *s, int sStep, cA32SC v[3], A32SC *d, int dStep, ASZ roi, int scale)
{ 
	DEF_ADD::ADDC::AC4::AddC_32sc data(scale, v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32sc >(data, s, sStep, d, dStep, roi);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C1IR )(  A32FC v,    A32FC *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::C1::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32fc >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C3IR )( cA32FC v[3], A32FC *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::C3::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32fc >(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_AC4IR)( cA32FC v[3], A32FC *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::AC4::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32fc >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C1R  )( cA32FC *s, int sStep,  A32FC v,    A32FC *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::C1::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32fc >(data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C3R  )( cA32FC *s, int sStep, cA32FC v[3], A32FC *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::C3::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32fc >(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_AC4R )( cA32FC *s, int sStep, cA32FC v[3], A32FC *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::AC4::AddC_32fc data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32fc >(data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C1IR )(  A32F v,    A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::C1::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32f >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C3IR )( cA32F v[3], A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::C3::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32f >(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_AC4IR)( cA32F v[3], A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::AC4::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32f >(data, sd, sdStep, sd, sdStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C4IR)( cA32F v[3], A32F *sd, int sdStep, ASZ roi)
{
	DEF_ADD::ADDC::C4::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_32f >(data, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C1R  )( cA32F *s, int sStep,  A32F v,    A32F *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::C1::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C1::AddC_32f >(data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C3R  )( cA32F *s, int sStep, cA32F v[3], A32F *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::C3::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C3::AddC_32f >(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_AC4R )( cA32F *s, int sStep, cA32F v[3], A32F *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::AC4::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::AC4::AddC_32f >(data, s, sStep, d, dStep, roi);
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C4R )( cA32F *s, int sStep, cA32F v[3], A32F *d, int dStep, ASZ roi)
{ 
	DEF_ADD::ADDC::C4::AddC_32f data(v);
	return OPT_LEVEL::fe<DEF_ADD::ADDC::C4::AddC_32f >(data, s, sStep, d, dStep, roi);
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
