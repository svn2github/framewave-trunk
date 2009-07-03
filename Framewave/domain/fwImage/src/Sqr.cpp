/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/SqrXDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C1IRSfs )( A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{														   
		DEF_SQRX::SQR::C1::Sqr_8u<-1> data(scale);		   
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{														   
		DEF_SQRX::SQR::C1::Sqr_8u<1> data(scale);			   
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C3IRSfs )( A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C3::Sqr_8u<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C3::Sqr_8u<1> data(scale);				
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_AC4IRSfs)( A8U *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															 
		DEF_SQRX::SQR::AC4::Sqr_8u<-1> data(scale);			 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															 
		DEF_SQRX::SQR::AC4::Sqr_8u<1> data(scale);			 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C4IRSfs )( A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C4::Sqr_8u<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C4::Sqr_8u<1> data(scale);				
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C1RSfs  )( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_8u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_8u<1>  > (data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C3RSfs  )( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C3::Sqr_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C3::Sqr_8u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_8u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_AC4RSfs )( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_8u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_8u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C4RSfs  )( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C4::Sqr_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C4::Sqr_8u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_8u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C1IRSfs )( A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C1::Sqr_16s<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C1::Sqr_16s<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C3IRSfs )( A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C3::Sqr_16s<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C3::Sqr_16s<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C4IRSfs )( A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C4::Sqr_16s<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C4::Sqr_16s<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_AC4IRSfs)( A16S *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															  
		DEF_SQRX::SQR::AC4::Sqr_16s<-1> data(scale);			  
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															  
		DEF_SQRX::SQR::AC4::Sqr_16s<1> data(scale);			  
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C1RSfs  )( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<0>   > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16s<1>  > (data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C3RSfs  )( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C3::Sqr_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C3::Sqr_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16s<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C4RSfs  )( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C4::Sqr_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C4::Sqr_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16s<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_AC4RSfs )( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16s<1>  > (data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C1IRSfs )( A16U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C1::Sqr_16u<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C1::Sqr_16u<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C3IRSfs )( A16U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C3::Sqr_16u<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C3::Sqr_16u<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C4IRSfs )( A16U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															
		DEF_SQRX::SQR::C4::Sqr_16u<-1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															
		DEF_SQRX::SQR::C4::Sqr_16u<1> data(scale);			
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_AC4IRSfs)( A16U *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<0>  >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else if(scale < 0)																						
	{															  
		DEF_SQRX::SQR::AC4::Sqr_16u<-1> data(scale);			  
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<-1> >(data, sd, sdStep, sd, sdStep,  roi);
	}																										
	else																									
	{															  
		DEF_SQRX::SQR::AC4::Sqr_16u<1> data(scale);			  
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<1>  >(data, sd, sdStep, sd, sdStep,  roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C1RSfs  )( cA16U *s, int sStep, A16U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C1::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<0>   > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C1::Sqr_16u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_16u<1>  > (data, s, sStep, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C3RSfs  )( cA16U *s, int sStep, A16U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C3::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C3::Sqr_16u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C3::Sqr_16u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_16u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C4RSfs  )( cA16U *s, int sStep, A16U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_SQRX::SQR::C4::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::C4::Sqr_16u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::C4::Sqr_16u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_16u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_AC4RSfs )( cA16U *s, int sStep, A16U *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_SQRX::SQR::AC4::Sqr_16u<0> data(scale);
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<0>  > (data, s, sStep, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_16u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<-1> >(data, s, sStep, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_SQRX::SQR::AC4::Sqr_16u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_16u<1>  > (data, s, sStep, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C1IR )( A32F *sd, int sdStep, ASZ roi)
{
	DEF_SQRX::SQR::C1::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32f>(data, sd, sdStep, sd, sdStep,  roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C3IR )( A32F *sd, int sdStep, ASZ roi)
{
	DEF_SQRX::SQR::C3::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_32f>(data, sd, sdStep, sd, sdStep,  roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_AC4IR)( A32F *sd, int sdStep, ASZ roi)
{ 
	DEF_SQRX::SQR::AC4::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_32f>(data, sd, sdStep, sd, sdStep,  roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C4IR )( A32F *sd, int sdStep, ASZ roi)
{
	DEF_SQRX::SQR::C4::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C4::Sqr_32f>(data, sd, sdStep, sd, sdStep,  roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C1R  )( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi)
{ 
	DEF_SQRX::SQR::C1::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C1::Sqr_32f> (data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C3R  )( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi)
{ 
	DEF_SQRX::SQR::C3::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::C3::Sqr_32f> (data, s, sStep, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_AC4R )( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi)
{
	DEF_SQRX::SQR::AC4::Sqr_32f data;
	return OPT_LEVEL::fe< DEF_SQRX::SQR::AC4::Sqr_32f> (data, s, sStep, d, dStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
