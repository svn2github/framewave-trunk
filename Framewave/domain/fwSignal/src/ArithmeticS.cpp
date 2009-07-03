/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "normalize.h" 
#include "Exp.h"
#include "Ln.h"
#include "DivSqrtCbrt.h"
#include "TrigonometricDef.h" //for Arctan functions
#include "fwSignal.h"
#include <limits.h>

using namespace OPT_LEVEL;

// Normalize functions

struct Normalize_32f : public fe2< Fw32f, C1, Fw32f, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw32f vsub,vdiv;

	Normalize_32f(const Fw32f vs, const Fw32f vd)
	{
			vsub = vs;
			vdiv = vd;		
	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_32f_setup_sse2(mVsub.f, mVdiv.f, vsub,vdiv);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].f = Normalize_sse2_32f_c1(r.src1[0].f,mVsub.f, mVdiv.f);		 
	}      
	IV REFR( const Fw32f *s, Fw32f *d) const                     
	{         
		NormalizeFS(s, d, vsub,vdiv);
	}      
} ;	

struct Normalize_64f : public fe2< Fw64f, C1, Fw64f, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw64f vsub,vdiv;

	Normalize_64f(const Fw64f vs, const Fw64f vd)
	{
			vsub = vs;
			vdiv = vd;		
	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_64f_setup_sse2(mVsub.d, mVdiv.d, vsub,vdiv);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].d = Normalize_sse2_64f_c1(r.src1[0].d,mVsub.d, mVdiv.d);		 
	}      
	IV REFR( const Fw64f *s, Fw64f *d) const                     
	{         
		NormalizeFS(s, d, vsub,vdiv);
	}      
} ;



struct Normalize_32fc : public fe2< Fw32fc, C1, Fw32fc, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw32fc vsub;
	Fw32f vdiv;

	Normalize_32fc(const Fw32fc vs, const Fw32f vd)
	{
			vsub = vs;
			vdiv = vd;		
	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_32fc_setup_sse2(mVsub.f, mVdiv.f, vsub,vdiv);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].f = Normalize_sse2_32f_c1(r.src1[0].f,mVsub.f, mVdiv.f);		 
	}      
	IV REFR( const Fw32fc *s, Fw32fc *d) const                     
	{         
		NormalizeFS(s, d, vsub,vdiv);
	}      
} ;


struct Normalize_64fc : public fe2< Fw64fc, C1, Fw64fc, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw64fc vsub;
	Fw64f vdiv;

	Normalize_64fc(const Fw64fc vs, const Fw64f vd)
	{
			vsub = vs;
			vdiv = vd;		
	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_64fc_setup_sse2(mVsub.d, mVdiv.d, vsub,vdiv);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].d = Normalize_sse2_64f_c1(r.src1[0].d,mVsub.d, mVdiv.d);		 
	}      
	IV REFR( const Fw64fc *s, Fw64fc *d) const                     
	{         
		NormalizeFS(s, d, vsub,vdiv);
	}      
} ;

struct Normalize_16s_Sfs : public fe2< Fw16s, C1, Fw16s, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw16s vsub;
	int vdiv;
	int scale;

	Normalize_16s_Sfs(const Fw16s vs, const int vd, const int s)
	{
			vsub = vs;
			vdiv = vd;	
			scale = s;

	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_16s_setup_sse2(mVsub.i, mVdiv.f, vsub,vdiv,scale);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].i = Normalize_sse2_16s_c1(r.src1[0].i,mVsub.i, mVdiv.f);		 
	}      
	IV REFR( const Fw16s *s, Fw16s *d) const                     
	{         
		NormalizeIS(s, d, vsub,vdiv,scale);
	}      
} ;

struct Normalize_16sc_Sfs : public fe2< Fw16sc, C1, Fw16sc, C1 >         
{   
	XMM128 mVsub,mVdiv;
	Fw16sc vsub;
	int vdiv;
	int scale;

	Normalize_16sc_Sfs(const Fw16sc vs, const int vd, const int s)
	{
			vsub = vs;
			vdiv = vd;	
			scale = s;
	}
	FE_SSE2_REF    
	IV SSE2_Init()
	{
		Normalize_16sc_setup_sse2(mVsub.i, mVdiv.f, vsub,vdiv,scale);		
	}
	IV SSE2( RegFile & r ) const                     
	{      
		r.dst[0].i = Normalize_sse2_16s_c1(r.src1[0].i,mVsub.i, mVdiv.f);		 
	}      
	IV REFR( const Fw16sc *s, Fw16sc *d) const                     
	{         
		NormalizeIS(s, d, vsub,vdiv,scale);
	}      
} ;

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_32f)(const Fw32f* pSrc, Fw32f* pDst, int len, Fw32f vsub,Fw32f vdiv)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}
	Normalize_32f data(vsub,vdiv);
	return OPT_LEVEL::fe<Normalize_32f>(data,pSrc,pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_64f)(const Fw64f* pSrc, Fw64f* pDst, int len, Fw64f vsub,Fw64f vdiv)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}
	Normalize_64f data(vsub,vdiv);
	return OPT_LEVEL::fe<Normalize_64f>(data,pSrc,pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_32fc)(const Fw32fc* pSrc, Fw32fc* pDst, int len, Fw32fc vsub, Fw32f vdiv)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}
	Normalize_32fc data(vsub,vdiv);
	return OPT_LEVEL::fe<Normalize_32fc>(data,pSrc,pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_64fc)(const Fw64fc* pSrc, Fw64fc* pDst, int len, Fw64fc vsub, Fw64f vdiv)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}

	Normalize_64fc data(vsub,vdiv);
	return OPT_LEVEL::fe<Normalize_64fc>(data,pSrc,pDst, len); 
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_16s_Sfs)(const Fw16s* pSrc, Fw16s* pDst, int len, Fw16s vsub, int vdiv, int scaleFactor)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}
	Normalize_16s_Sfs data(vsub,vdiv,scaleFactor);
	return OPT_LEVEL::fe<Normalize_16s_Sfs>(data,pSrc,pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsNormalize_16sc_Sfs)(const Fw16sc* pSrc, Fw16sc* pDst, int len, Fw16sc vsub, int vdiv, int scaleFactor)
{
	if(0 == vdiv)
	{
		return fwStsDivByZeroErr;
	}
	Normalize_16sc_Sfs data(vsub,vdiv,scaleFactor);
	return OPT_LEVEL::fe<Normalize_16sc_Sfs>(data,pSrc,pDst, len); 
}

//Exp

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_16s_ISfs)(Fw16s* pSrcDst, int len, int scaleFactor)
{
	Exp_16s_B1<C1> data(scaleFactor);
	if (-10 <= scaleFactor && scaleFactor <= 10)
		return OPT_LEVEL::fe<Exp_16s_B1<C1> >( data, pSrcDst, len ); 
	else if(scaleFactor < -10) {
        return fwsSet_16s( CBL_S16_MAX, pSrcDst, len );
    }
    else{ //scaleFactor > 10
        return fwsSet_16s( 0, pSrcDst, len );	
    } 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_16s_Sfs)(const Fw16s* pSrc, Fw16s* pDst, int len, int scaleFactor)
{	
	Exp_16s_B2<C1> data(scaleFactor);
	if (-10 <= scaleFactor && scaleFactor <= 10)
		return OPT_LEVEL::fe<Exp_16s_B2<C1> >(data,  pSrc, pDst, len ); 
	else if(scaleFactor < -10) {
        return fwsSet_16s( CBL_S16_MAX, pDst, len );
    }
    else{ //scaleFactor > 10
        return fwsSet_16s( 0, pDst, len );	
    }  
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32s_Sfs)(const Fw32s* pSrc, Fw32s* pDst, int len, int scaleFactor)
{
	Exp_32s_B2<C1> data(scaleFactor);
	return OPT_LEVEL::fe<Exp_32s_B2<C1> >( data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32s_ISfs)(Fw32s* pSrcDst, int len, int scaleFactor)
{
	Exp_32s_B1<C1> data(scaleFactor);
	return OPT_LEVEL::fe<Exp_32s_B1<C1> >( data, pSrcDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64s_Sfs)(const Fw64s* pSrc, Fw64s* pDst, int len, int scaleFactor)
{
	if(scaleFactor !=0){
		Exp_64s_scale_NZ_B2  data(scaleFactor);
		return OPT_LEVEL::fe< Exp_64s_scale_NZ_B2>( data, pSrc, pDst, len  );
	}
	else{
		Exp_64s_scale0_B2 data;
		return OPT_LEVEL::fe< Exp_64s_scale0_B2 >( data, pSrc, pDst, len  );
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64s_ISfs)(Fw64s* pSrcDst, int len, int scaleFactor)
{
	if(scaleFactor !=0){
		Exp_64s_scale_NZ_B1 data(scaleFactor);
		return OPT_LEVEL::fe< Exp_64s_scale_NZ_B1 >( data, pSrcDst, len );
	}
	else{
		Exp_64s_scale0_B1 data;
		return OPT_LEVEL::fe< Exp_64s_scale0_B1 >( data, pSrcDst, len );
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Exp_32f_B2<C1> data;
	return OPT_LEVEL::fe< Exp_32f_B2<C1> >(data, pSrc, pDst, len); 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f_I)(Fw32f* pSrcDst, int len)
{	
	Exp_32f_B1<C1> data;
	return OPT_LEVEL::fe< Exp_32f_B1<C1> >(data, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Exp_64f_B2<C1> data;
	return OPT_LEVEL::fe< Exp_64f_B2<C1> >(data, pSrc, pDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_64f_I)(Fw64f* pSrcDst, int len)
{
	Exp_64f_B1<C1> data;
	return OPT_LEVEL::fe< Exp_64f_B1<C1> >(data, pSrcDst, len);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsExp_32f64f)(const Fw32f* pSrc, Fw64f* pDst, int len)
{
	Exp_32f64f_B2<C1> data;
	return OPT_LEVEL::fe< Exp_32f64f_B2<C1> >(data, pSrc, pDst, len);
}

//Log

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Ln_32f_tang_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B2<C1> >(data, pSrc, pDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32f_I)(Fw32f* pSrcDst, int len)
{
	Ln_32f_tang_B1<C1> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B1<C1> >(data, pSrcDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_16s_Sfs)(const Fw16s* pSrc, Fw16s* pDst, int len, int scaleFactor)
{
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B2<C1> >(data, pSrc, pDst, len); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B2<C1> >(data, pSrc, pDst, len); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B2<C1> >(data, pSrc, pDst, len); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B2<C1> >(data, pSrc, pDst, len); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B2<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B2<C1> >(data, pSrc, pDst, len);

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32s_Sfs)(const Fw32s* pSrc, Fw32s* pDst, int len, int scaleFactor)
{
	Ln_32s_B2<C1> data(scaleFactor) ;
	return OPT_LEVEL::fe< Ln_32s_B2<C1> >(data, pSrc, pDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32s16s_Sfs)(const Fw32s* pSrc, Fw16s* pDst, int len, int scaleFactor)
{
	Ln_32s_16s_B2<C1> data(scaleFactor) ;
	return OPT_LEVEL::fe< Ln_32s_16s_B2<C1> >(data, pSrc, pDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_16s_ISfs)(Fw16s* pSrcDst, int len, int scaleFactor)
{
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B1<C1> >(data, pSrcDst, len); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B1<C1> >(data, pSrcDst, len ); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B1<C1> >(data, pSrcDst, len ); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B1<C1> >(data, pSrcDst, len ); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B1<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B1<C1> >(data, pSrcDst, len );  
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_32s_ISfs)(Fw32s* pSrcDst, int len, int scaleFactor)
{
	Ln_32s_B1<C1> data(scaleFactor) ;
	return OPT_LEVEL::fe< Ln_32s_B1<C1> >(data, pSrcDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_64f32f)(const Fw64f* pSrc, Fw32f* pDst, int len)
{
	Ln_64f_32f_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_64f_32f_B2<C1> >(data, pSrc, pDst, len ); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
{
	Ln_64f_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_64f_B2<C1> >(data, pSrc, pDst, len );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsLn_64f_I)(Fw64f* pSrcDst, int len)
{
	Ln_64f_B1<C1> data ;
	return OPT_LEVEL::fe< Ln_64f_B1<C1> >(data, pSrcDst, len );
}

//Cbrt
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCubrt_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
{
	Cbrt_32f_A24 data;
	return OPT_LEVEL::fe< Cbrt_32f_A24 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwsCubrt_32s16s_Sfs)(const Fw32s* pSrc, Fw16s* pDst, int len, int scaleFactor)
{
	Cubrt_32s16s data(scaleFactor);
	return OPT_LEVEL::fe< Cubrt_32s16s >(data, pSrc, pDst, len);
}

//Log 10

FwStatus PREFIX_OPT(OPT_PREFIX, fws10Log10_32s_Sfs)(const Fw32s* pSrc, Fw32s* pDst, int len, int scaleFactor)
{
	TenLog10_32s_B2 data(scaleFactor);
	return OPT_LEVEL::fe< TenLog10_32s_B2 >(data, pSrc, pDst, len); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fws10Log10_32s_ISfs)(Fw32s* pSrcDst, int len, int scaleFactor)
{
	TenLog10_32s_B1 data(scaleFactor);
	return OPT_LEVEL::fe< TenLog10_32s_B1 >(data, pSrcDst, len); 
}

//Arctan	//All the fns are in trigonometric.h
 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwsArctan_32f)(const Fw32f* pSrc, Fw32f* pDst, int len)
 {
	Atan_32f_All dat;
	return OPT_LEVEL::fe<Atan_32f_All> (dat, pSrc, pDst, len);
 }
 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwsArctan_64f)(const Fw64f* pSrc, Fw64f* pDst, int len)
 {
	Atan_64f_All dat;
	return OPT_LEVEL::fe<Atan_64f_All> (dat, pSrc, pDst, len);
 }
 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwsArctan_32f_I)(Fw32f* pSrcDst, int len)
 {
 	Atan_32f_Inplace dat;
	return OPT_LEVEL::fe<Atan_32f_Inplace> (dat, pSrcDst, len);
 }
 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwsArctan_64f_I)(Fw64f* pSrcDst, int len)
 {
  	Atan_64f_Inplace dat;
	return OPT_LEVEL::fe<Atan_64f_Inplace> (dat, pSrcDst, len);
 }

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
