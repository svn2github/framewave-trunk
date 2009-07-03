/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "logical.h"
#include "fwSignal.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_8u)(const Fw8u* pSrc1, const Fw8u* pSrc2, Fw8u* pDst, int len)
{  
	And_all::And_B3<Fw8u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B3<Fw8u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_16u)(const Fw16u* pSrc1, const Fw16u* pSrc2, Fw16u* pDst, int len)	
{ 
	And_all::And_B3<Fw16u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B3<Fw16u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_32u)(const Fw32u* pSrc1, const Fw32u* pSrc2, Fw32u* pDst, int len)	
{ 
	And_all::And_B3<Fw32u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B3<Fw32u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_8u_I)(const Fw8u* pSrc, Fw8u* pSrcDst, int len)			
{ 
	And_all::And_B2<Fw8u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B2<Fw8u,C1> >( dat, pSrc, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_16u_I)(const Fw16u* pSrc, Fw16u* pSrcDst, int len) 
{ 
	And_all::And_B2<Fw16u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B2<Fw16u,C1> >( dat, pSrc, pSrcDst, len ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAnd_32u_I)(const Fw32u* pSrc, Fw32u* pSrcDst, int len) 
{ 
	And_all::And_B2<Fw32u,C1> dat;
	return OPT_LEVEL::fe< And_all::And_B2<Fw32u,C1> >( dat, pSrc, pSrcDst, len ); 
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_8u)(const Fw8u* pSrc1, const Fw8u* pSrc2, Fw8u* pDst, int len)
{  
	Or_all::Or_B3<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B3<Fw8u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_16u)(const Fw16u* pSrc1, const Fw16u* pSrc2, Fw16u* pDst, int len)	
{ 
	Or_all::Or_B3<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B3<Fw16u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_32u)(const Fw32u* pSrc1, const Fw32u* pSrc2, Fw32u* pDst, int len)	
{ 
	Or_all::Or_B3<Fw32u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B3<Fw32u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_8u_I)(const Fw8u* pSrc, Fw8u* pSrcDst, int len)			
{ 
	Or_all::Or_B2<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B2<Fw8u,C1> >( dat, pSrc, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_16u_I)(const Fw16u* pSrc, Fw16u* pSrcDst, int len) 
{ 
	Or_all::Or_B2<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B2<Fw16u,C1> >( dat, pSrc, pSrcDst, len ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOr_32u_I)(const Fw32u* pSrc, Fw32u* pSrcDst, int len) 
{ 
	Or_all::Or_B2<Fw32u,C1> dat;
	return OPT_LEVEL::fe< Or_all::Or_B2<Fw32u,C1> >( dat, pSrc, pSrcDst, len ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_8u)(const Fw8u* pSrc1, const Fw8u* pSrc2, Fw8u* pDst, int len)
{  
	Xor_all::Xor_B3<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw8u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_16u)(const Fw16u* pSrc1, const Fw16u* pSrc2, Fw16u* pDst, int len)	
{ 
	Xor_all::Xor_B3<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw16u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_32u)(const Fw32u* pSrc1, const Fw32u* pSrc2, Fw32u* pDst, int len)	
{ 
	Xor_all::Xor_B3<Fw32u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B3<Fw32u,C1> >( dat, pSrc1, pSrc2, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_8u_I)(const Fw8u* pSrc, Fw8u* pSrcDst, int len)			
{ 
	Xor_all::Xor_B2<Fw8u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw8u,C1> >( dat, pSrc, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_16u_I)(const Fw16u* pSrc, Fw16u* pSrcDst, int len) 
{ 
	Xor_all::Xor_B2<Fw16u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw16u,C1> >( dat, pSrc, pSrcDst, len ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXor_32u_I)(const Fw32u* pSrc, Fw32u* pSrcDst, int len) 
{ 
	Xor_all::Xor_B2<Fw32u,C1> dat;
	return OPT_LEVEL::fe< Xor_all::Xor_B2<Fw32u,C1> >( dat, pSrc, pSrcDst, len ); 
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_8u)(const Fw8u* pSrc, Fw8u val, Fw8u* pDst, int len)					
{ 
	if(val == 0) return fwsZero_8u(pDst, len);
	AndC_all::AndC_B2_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B2_C1<Fw8u> >( dat, pSrc, pDst, len ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_16u)(const Fw16u* pSrc, Fw16u val, Fw16u* pDst, int len)				
{ 
	if(val == 0) return fwsZero_16s((Fw16s*)pDst, len);
	AndC_all::AndC_B2_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B2_C1<Fw16u> >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_32u)(const Fw32u* pSrc, Fw32u val, Fw32u* pDst, int len)				
{
	if(val == 0) return fwsZero_32f((Fw32f*)pDst, len);
	AndC_all::AndC_B2_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B2_C1<Fw32u> >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_8u_I)(Fw8u val, Fw8u* pSrcDst, int len)									
{
	if(val == 0) return fwsZero_8u(pSrcDst, len);
	AndC_all::AndC_B1_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B1_C1<Fw8u> >( dat, pSrcDst, len );
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_16u_I)(Fw16u val, Fw16u* pSrcDst, int len)								
{
	if(val == 0) return fwsZero_16s((Fw16s*)pSrcDst, len);
	AndC_all::AndC_B1_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B1_C1<Fw16u> >( dat, pSrcDst, len );
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAndC_32u_I)(Fw32u val, Fw32u* pSrcDst, int len)								
{
	if(val == 0) return fwsZero_32f((Fw32f*)pSrcDst, len);
	AndC_all::AndC_B1_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< AndC_all::AndC_B1_C1<Fw32u> >( dat, pSrcDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_8u)(const Fw8u* pSrc, Fw8u val, Fw8u* pDst, int len)
{ 
	OrC_all::OrC_B2_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B2_C1<Fw8u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_16u)(const Fw16u* pSrc, Fw16u val, Fw16u* pDst, int len)
{ 
	OrC_all::OrC_B2_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B2_C1<Fw16u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_32u)(const Fw32u* pSrc, Fw32u val, Fw32u* pDst, int len)	
{ 
	OrC_all::OrC_B2_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B2_C1<Fw32u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_8u_I)(Fw8u val, Fw8u* pSrcDst, int len)									
{	
	if (val == 0) return fwStsNoErr;
	OrC_all::OrC_B1_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B1_C1<Fw8u> > ( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_16u_I)(Fw16u val, Fw16u* pSrcDst, int len)								
{
	if (val == 0) return fwStsNoErr;
	OrC_all::OrC_B1_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B1_C1<Fw16u> > ( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsOrC_32u_I)(Fw32u val, Fw32u* pSrcDst, int len)								
{
	if (val == 0) return fwStsNoErr;
	OrC_all::OrC_B1_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< OrC_all::OrC_B1_C1<Fw32u> > ( dat, pSrcDst, len ); 
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_8u)(const Fw8u* pSrc, Fw8u val, Fw8u* pDst, int len)
{ 
	XorC_all::XorC_B2_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw8u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_16u)(const Fw16u* pSrc, Fw16u val, Fw16u* pDst, int len)
{ 
	XorC_all::XorC_B2_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw16u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_32u)(const Fw32u* pSrc, Fw32u val, Fw32u* pDst, int len)	
{ 
	XorC_all::XorC_B2_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B2_C1<Fw32u> > ( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_8u_I)(Fw8u val, Fw8u* pSrcDst, int len)									
{	
	if (val == 0) return fwStsNoErr;
	XorC_all::XorC_B1_C1<Fw8u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw8u> > ( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_16u_I)(Fw16u val, Fw16u* pSrcDst, int len)								
{
	if (val == 0) return fwStsNoErr;
	XorC_all::XorC_B1_C1<Fw16u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw16u> > ( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsXorC_32u_I)(Fw32u val, Fw32u* pSrcDst, int len)								
{
	if (val == 0) return fwStsNoErr;
	XorC_all::XorC_B1_C1<Fw32u> dat(val);
	return OPT_LEVEL::fe< XorC_all::XorC_B1_C1<Fw32u> > ( dat, pSrcDst, len ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_8u)(const Fw8u* pSrc, Fw8u* pDst, int len)
{ 
	Not_All::Not_B2<Fw8u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B2<Fw8u, C1> >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_16u)(const Fw16u* pSrc, Fw16u* pDst, int len)
{ 
	Not_All::Not_B2<Fw16u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B2<Fw16u, C1> >( dat, pSrc, pDst, len );
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_32u)(const Fw32u* pSrc, Fw32u* pDst, int len)
{ 
	Not_All::Not_B2<Fw32u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B2<Fw32u, C1> >( dat, pSrc, pDst, len );
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_8u_I)(Fw8u* pSrcDst, int len)
{ 
	Not_All::Not_B1<Fw8u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B1<Fw8u,C1> >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_16u_I)(Fw16u* pSrcDst, int len)
{ 
	Not_All::Not_B1<Fw16u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B1<Fw16u,C1> >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNot_32u_I)(Fw32u* pSrcDst, int len)
{ 
	Not_All::Not_B1<Fw32u, C1> dat;
	return OPT_LEVEL::fe< Not_All::Not_B1<Fw32u,C1> >( dat, pSrcDst, len ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_8u)(const Fw8u* pSrc, int val, Fw8u* pDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	if ( val >= 8) return fwsZero_8u(pDst, len);
	LShift_8u::LShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< LShift_8u::LShift_B2_C1 >( dat, pSrc, pDst, len ); 

}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_16s)(const Fw16s* pSrc, int val, Fw16s* pDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	if ( val >= 16) return fwsZero_16s(pDst, len);
	LShift_16u::LShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< LShift_16u::LShift_B2_C1 >( dat, (Fw16u*)pSrc, (Fw16u*)pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_16u)(const Fw16u* pSrc, int val, Fw16u* pDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	if ( val >= 16) return fwsZero_16s((Fw16s*)pDst, len);
	LShift_16u::LShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< LShift_16u::LShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_32s)(const Fw32s* pSrc, int val, Fw32s* pDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if ( val >= 32) return fwsZero_32f((Fw32f*)pDst, len);
	LShift_32s::LShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< LShift_32s::LShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_8u_I)(int val, Fw8u* pSrcDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	if ( val >= 8) return fwsZero_8u(pSrcDst, len);
	LShift_8u::LShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< LShift_8u::LShift_B1_C1 >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_16u_I)(int val, Fw16u* pSrcDst, int len)
{ 
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	if ( val >= 16) return fwsZero_16s((Fw16s*)pSrcDst, len);
	LShift_16u::LShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< LShift_16u::LShift_B1_C1 >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_16s_I)(int val, Fw16s* pSrcDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	if ( val >= 16) return fwsZero_16s(pSrcDst, len);
	LShift_16u::LShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< LShift_16u::LShift_B1_C1 >( dat, (Fw16u*)pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLShiftC_32s_I)(int val, Fw32s* pSrcDst, int len)
{ 
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	if ( val >= 32) return fwsZero_32f((Fw32f*)pSrcDst, len);
	LShift_32s::LShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< LShift_32s::LShift_B1_C1 >( dat, pSrcDst, len ); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_8u)(const Fw8u* pSrc, int val, Fw8u* pDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if ( val >= 8) return fwsZero_8u(pDst, len);
	RShift_8u::RShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< RShift_8u::RShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_16s)(const Fw16s* pSrc, int val, Fw16s* pDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	RShift_16s::RShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< RShift_16s::RShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_16u)(const Fw16u* pSrc, int val, Fw16u* pDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	if ( val >= 16) return fwsZero_16s((Fw16s*)pDst, len);
	RShift_16u::RShift_B2_C1 dat(val);
	return OPT_LEVEL::fe< RShift_16u::RShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_32s)(const Fw32s* pSrc, int val, Fw32s* pDst, int len)
{
	int value;
	if(val > 31) value = 31;
	else	value = val;
	if (value < 0) return fwStsSizeErr;
	RShift_32s::RShift_B2_C1 dat(value);
	return OPT_LEVEL::fe< RShift_32s::RShift_B2_C1 >( dat, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_8u_I)(int val, Fw8u* pSrcDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	if ( val >= 8) return fwsZero_8u(pSrcDst, len);
	RShift_8u::RShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< RShift_8u::RShift_B1_C1 >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_16u_I)(int val, Fw16u* pSrcDst, int len)
{
	if (val < 0) return fwStsSizeErr; 
	if (val == 0) return fwStsNoErr; 
	if ( val >= 16) return fwsZero_16s((Fw16s*)pSrcDst, len);
	RShift_16u::RShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< RShift_16u::RShift_B1_C1 >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_16s_I)(int val, Fw16s* pSrcDst, int len)
{
	if (val < 0) return fwStsSizeErr;
	if (val == 0) return fwStsNoErr; 
	RShift_16s::RShift_B1_C1 dat(val);
	return OPT_LEVEL::fe< RShift_16s::RShift_B1_C1 >( dat, pSrcDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsRShiftC_32s_I)(int val, Fw32s* pSrcDst, int len)
{ 
	int value;
	if(val > 31) value = 31;
	else	value = val;
	if (value < 0) return fwStsSizeErr;
	if (value == 0) return fwStsNoErr; 
	RShift_32s::RShift_B1_C1 dat(value);
	return OPT_LEVEL::fe< RShift_32s::RShift_B1_C1 >( dat, pSrcDst, len ); 
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
