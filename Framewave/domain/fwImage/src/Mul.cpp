/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Arithmetic/Def/MulDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C1IRSfs )( cA8U *s,  int sStep,   A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_8u<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
        DEF_MUL::MUL::C1::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u_PosScale<A8U> > (data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C3IRSfs )( cA8U *s,  int sStep,   A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C3::Mul_8u<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C3::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u_PosScale<A8U> > (data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_AC4IRSfs)( cA8U *s,  int sStep,   A8U *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::AC4::Mul_8u<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::AC4::Mul_8u<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C4IRSfs )( cA8U *s,  int sStep,   A8U *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C4::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C4::Mul_8u<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C4::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u_PosScale<A8U> > (data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C1RSfs  )( cA8U *s1, int s1Step, cA8U *s2, int s2Step, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{	
        DEF_MUL::MUL::C1::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_8u_PosScale<A8U> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C3RSfs  )( cA8U *s1, int s1Step, cA8U *s2, int s2Step, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C3::Mul_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C3::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_8u_PosScale<A8U> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_AC4RSfs )( cA8U *s1, int s1Step, cA8U *s2, int s2Step, A8U *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::AC4::Mul_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::AC4::Mul_8u<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_8u<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C4RSfs  )( cA8U *s1, int s1Step, cA8U *s2, int s2Step, A8U *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C4::Mul_8u<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C4::Mul_8u<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C4::Mul_8u_PosScale<A8U> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_8u_PosScale<A8U> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C1IRSfs )( cA16S *s,  int sStep,   A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_16s<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C1::Mul_16s<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C3IRSfs )( cA16S *s,  int sStep,   A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C3::Mul_16s<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C3::Mul_16s<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_AC4IRSfs)( cA16S *s,  int sStep,   A16S *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::AC4::Mul_16s<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::AC4::Mul_16s<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C4IRSfs )( cA16S *s,  int sStep,   A16S *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C4::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C4::Mul_16s<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C4::Mul_16s<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C1RSfs  )( cA16S *s1, int s1Step, cA16S *s2, int s2Step, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C1::Mul_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16s<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C3RSfs  )( cA16S *s1, int s1Step, cA16S *s2, int s2Step, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C3::Mul_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C3::Mul_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16s<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_AC4RSfs )( cA16S *s1, int s1Step, cA16S *s2, int s2Step, A16S *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::AC4::Mul_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::AC4::Mul_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16s<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C4RSfs  )( cA16S *s1, int s1Step, cA16S *s2, int s2Step, A16S *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C4::Mul_16s<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C4::Mul_16s<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C4::Mul_16s<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_16s<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C1IRSfs )( cA16SC *s,  int sStep,   A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C1::Mul_16sc<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C1::Mul_16sc<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C3IRSfs )( cA16SC *s,  int sStep,   A16SC *sd, int sdStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::C3::Mul_16sc<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::C3::Mul_16sc<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_AC4IRSfs)( cA16SC *s,  int sStep,   A16SC *sd, int sdStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<0> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else if(scale < 0)																				
	{
		DEF_MUL::MUL::AC4::Mul_16sc<-1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<-1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}																								
	else																							
	{
		DEF_MUL::MUL::AC4::Mul_16sc<1> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<1> >(data, s, sStep, sd, sdStep, sd, sdStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C1RSfs  )( cA16SC *s1, int s1Step, cA16SC *s2, int s2Step, A16SC *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C1::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C1::Mul_16sc<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C1::Mul_16sc<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_16sc<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C3RSfs  )( cA16SC *s1, int s1Step, cA16SC *s2, int s2Step, A16SC *d, int dStep, ASZ roi, int scale)
{ 
	if(scale == 0)
	{
		DEF_MUL::MUL::C3::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::C3::Mul_16sc<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::C3::Mul_16sc<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_16sc<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_AC4RSfs )( cA16SC *s1, int s1Step, cA16SC *s2, int s2Step, A16SC *d, int dStep, ASZ roi, int scale)
{
	if(scale == 0)
	{
		DEF_MUL::MUL::AC4::Mul_16sc<0> data(scale);
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<0> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else if(scale < 0)																			
	{																	 
		DEF_MUL::MUL::AC4::Mul_16sc<-1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<-1> >(data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}																							
	else																						
	{																	 
		DEF_MUL::MUL::AC4::Mul_16sc<1> data(scale);						 
		return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_16sc<1> > (data, s1, s1Step, s2, s2Step, d, dStep, roi);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C1IRSfs )( cA32SC *s,  int sStep,   A32SC *sd, int sdStep, ASZ roi, int scale)
{
	DEF_MUL::MUL::C1::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32sc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C3IRSfs )( cA32SC *s,  int sStep,   A32SC *sd, int sdStep, ASZ roi, int scale)
{
	DEF_MUL::MUL::C3::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32sc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_AC4IRSfs)( cA32SC *s,  int sStep,   A32SC *sd, int sdStep, ASZ roi, int scale)
{ 
	DEF_MUL::MUL::AC4::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32sc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C1RSfs  )( cA32SC *s1, int s1Step, cA32SC *s2, int s2Step, A32SC *d, int dStep, ASZ roi, int scale)
{ 
	DEF_MUL::MUL::C1::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32sc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C3RSfs  )( cA32SC *s1, int s1Step, cA32SC *s2, int s2Step, A32SC *d, int dStep, ASZ roi, int scale)
{ 
	DEF_MUL::MUL::C3::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32sc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_AC4RSfs )( cA32SC *s1, int s1Step, cA32SC *s2, int s2Step, A32SC *d, int dStep, ASZ roi, int scale)
{
	DEF_MUL::MUL::AC4::Mul_32sc data(scale);
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32sc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C1IR )( cA32FC *s,  int sStep,   A32FC *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MUL::C1::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32fc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C3IR )( cA32FC *s,  int sStep,   A32FC *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MUL::C3::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32fc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_AC4IR)( cA32FC *s,  int sStep,   A32FC *sd, int sdStep, ASZ roi)
{ 
	DEF_MUL::MUL::AC4::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32fc>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C1R  )( cA32FC *s1, int s1Step, cA32FC *s2, int s2Step, A32FC *d, int dStep, ASZ roi)
{ 
	DEF_MUL::MUL::C1::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32fc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C3R  )( cA32FC *s1, int s1Step, cA32FC *s2, int s2Step, A32FC *d, int dStep, ASZ roi)
{ 
	DEF_MUL::MUL::C3::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32fc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_AC4R )( cA32FC *s1, int s1Step, cA32FC *s2, int s2Step, A32FC *d, int dStep, ASZ roi)
{
	DEF_MUL::MUL::AC4::Mul_32fc data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32fc> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C1IR )( cA32F *s,  int sStep,   A32F *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MUL::C1::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32f>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C3IR )( cA32F *s,  int sStep,   A32F *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MUL::C3::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32f>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_AC4IR)( cA32F *s,  int sStep,   A32F *sd, int sdStep, ASZ roi)
{ 
	DEF_MUL::MUL::AC4::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32f>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C4IR )( cA32F *s,  int sStep,   A32F *sd, int sdStep, ASZ roi)
{
	DEF_MUL::MUL::C4::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_32f>(data, s, sStep, sd, sdStep, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C1R  )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *d, int dStep, ASZ roi)
{ 
	DEF_MUL::MUL::C1::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C1::Mul_32f> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C3R  )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *d, int dStep, ASZ roi)
{ 
	DEF_MUL::MUL::C3::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C3::Mul_32f> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_AC4R )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *d, int dStep, ASZ roi)
{
	DEF_MUL::MUL::AC4::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::AC4::Mul_32f> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C4R  )( cA32F *s1, int s1Step, cA32F *s2, int s2Step, A32F *d, int dStep, ASZ roi)
{ 
	DEF_MUL::MUL::C4::Mul_32f data;
	return OPT_LEVEL::fe< DEF_MUL::MUL::C4::Mul_32f> (data, s1, s1Step, s2, s2Step, d, dStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
