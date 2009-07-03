/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "Exp.h"
#include "Ln.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// =============================
//			Exp
// =============================

// Exp 8u 
FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C1IRSfs )(  A8U  *sd, int sdStep, ASZ roi, int scale )
{
	Exp_8u_B1<C1> data(scale);
	if (scale > -8 && scale < 8){ return OPT_LEVEL::fe<Exp_8u_B1<C1> >( data, sd, sdStep, roi ); }
	else if(scale <= -8) 
        return fwiSet_8u_C1R( CBL_U8_MAX, sd, sdStep, roi );
    else //scale >= 8
    	return fwiSet_8u_C1R( 0, sd, sdStep, roi );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C3IRSfs )(  A8U  *sd, int sdStep, ASZ roi, int scale )
{
	Exp_8u_B1<C3> data(scale);
	if (scale > -8 && scale < 8){ return OPT_LEVEL::fe<Exp_8u_B1<C3> >( data, sd, sdStep, roi ); }
    else if(scale <= -8) {
        const Fw8u values[3] = {CBL_U8_MAX, CBL_U8_MAX, CBL_U8_MAX};
        return fwiSet_8u_C3R( values, sd, sdStep, roi );
    }
    else{ //scale >= 8
        const Fw8u values[3] = {0, 0, 0};
        return fwiSet_8u_C3R( values, sd, sdStep, roi );	
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C1RSfs  )( cA8U  *s,  int sStep, A8U  *d, int dStep, ASZ roi, int scale )
{	
	Exp_8u_B2<C1> data(scale);   
	if (scale > -8 && scale < 8) { return OPT_LEVEL::fe<Exp_8u_B2<C1> >( data, s, sStep, d, dStep, roi ); }
	else if(scale <= -8) 
        return fwiSet_8u_C1R( CBL_U8_MAX, d, dStep, roi );
    else //scale >= 8
    	return fwiSet_8u_C1R( 0, d, dStep, roi );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C3RSfs  )( cA8U  *s,  int sStep, A8U  *d, int dStep, ASZ roi, int scale )
{	
	Exp_8u_B2<C3> data(scale); 
	if (scale > -8 && scale < 8) { return OPT_LEVEL::fe<Exp_8u_B2<C3> >( data, s, sStep, d, dStep, roi ); }
	else if(scale <= -8) {
        const Fw8u values[3] = {CBL_U8_MAX, CBL_U8_MAX, CBL_U8_MAX};
        return fwiSet_8u_C3R( values, d, dStep, roi );
    }
    else{ //scale >= 8
        const Fw8u values[3] = {0, 0, 0};
        return fwiSet_8u_C3R( values, d, dStep, roi );	
    }
}


// Exp 16s 

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C1IRSfs )(  A16S  *sd, int sdStep, ASZ roi, int scale )
{
	Exp_16s_B1<C1> data(scale);
	if (-10 <= scale && scale <= 10)
		return OPT_LEVEL::fe<Exp_16s_B1<C1> >( data, sd, sdStep, roi ); 
	else if(scale <= -10) {
        return fwiSet_16s_C1R( CBL_S16_MAX, sd, sdStep, roi );
    }
    else{ //scale >= 10
        return fwiSet_16s_C1R( 0, sd, sdStep, roi );	
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C3IRSfs )(  A16S  *sd, int sdStep, ASZ roi, int scale )
{ 
	Exp_16s_B1<C3> data(scale);
	if (-10 <= scale && scale <= 10)
		return OPT_LEVEL::fe<Exp_16s_B1<C3> >( data, sd, sdStep, roi );
	else if(scale <= -10) {
        const Fw16s values[3] = {CBL_S16_MAX, CBL_S16_MAX, CBL_S16_MAX};
        return fwiSet_16s_C3R( values, sd, sdStep, roi );
    }
    else{ //scale >= 10
        const Fw16s values[3] = {0, 0, 0};
        return fwiSet_16s_C3R( values, sd, sdStep, roi );	
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C1RSfs  )( cA16S  *s,  int sStep, A16S  *d, int dStep, ASZ roi, int scale )
{	
	Exp_16s_B2<C1> data(scale);
	if (-10 <= scale && scale <= 10)
		return OPT_LEVEL::fe<Exp_16s_B2<C1> >(data,  s, sStep, d, dStep, roi ); 
	else if(scale <= -10) {
        return fwiSet_16s_C1R( CBL_S16_MAX, d, dStep, roi );
    }
    else{ //scale >= 10
        return fwiSet_16s_C1R( 0, d, dStep, roi );	
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C3RSfs  )( cA16S  *s,  int sStep, A16S  *d, int dStep, ASZ roi, int scale )
{	
	Exp_16s_B2<C3> data(scale);
	if (-10 <= scale && scale <= 10)
		return OPT_LEVEL::fe<Exp_16s_B2<C3> >(data, s, sStep, d, dStep, roi); 
	else if(scale <= -10) {
        const Fw16s values[3] = {CBL_S16_MAX, CBL_S16_MAX, CBL_S16_MAX};
        return fwiSet_16s_C3R( values, d, dStep, roi );
    }
    else{ //scale >= 10
        const Fw16s values[3] = {0, 0, 0};
        return fwiSet_16s_C3R( values, d, dStep, roi );	
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C1IR   )(  A32F *sd, int sdStep, ASZ roi  )
{
	Exp_32f_B1<C1> data;
	return OPT_LEVEL::fe< Exp_32f_B1<C1> >(data, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C3IR   )(  A32F *sd, int sdStep, ASZ roi  )
{
	Exp_32f_B1<C3> data;
	return OPT_LEVEL::fe<Exp_32f_B1<C3> >(data, sd, sdStep, roi);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C1R    )( cA32F *s,  int sStep, A32F *d, int dStep, ASZ roi )
{	
	Exp_32f_B2<C1> data;
	return OPT_LEVEL::fe<Exp_32f_B2<C1> >(data, s, sStep, d, dStep, roi); 
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C3R    )( cA32F *s,  int sStep, A32F *d, int dStep, ASZ roi )
{
	Exp_32f_B2<C3> data;
	return OPT_LEVEL::fe<Exp_32f_B2<C3> >(data, s, sStep, d, dStep, roi); 
}

// =============================
//			Ln
// =============================

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C1IRSfs )(  A8U  *sd, int sdStep, ASZ roi, int scaleFactor )
{
	Ln_8u_B1<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_8u_B1<C1> >(data, sd, sdStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C1RSfs  )( cA8U  *s,  int sStep, A8U  *d, int dStep, ASZ roi, int scaleFactor )
{
	Ln_8u_B2<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_8u_B2<C1> >(data, s, sStep, d, dStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C3IRSfs )(  A8U  *sd, int sdStep, ASZ roi, int scaleFactor )
{
	Ln_8u_B1<C3> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_8u_B1<C3> >(data, sd, sdStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C3RSfs  )( cA8U  *s,  int sStep, A8U  *d, int dStep, ASZ roi, int scaleFactor )
{
	Ln_8u_B2<C3> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_8u_B2<C3> >(data, s, sStep, d, dStep, roi);
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C1IRSfs)(  A16S *sd, int sdStep, ASZ roi, int scaleFactor )
{ 
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B1<C1> >(data, sd, sdStep, roi); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B1<C1> >(data, sd, sdStep, roi ); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B1<C1> >(data, sd, sdStep, roi ); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B1<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B1<C1> >(data, sd, sdStep, roi ); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B1<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B1<C1> >(data, sd, sdStep, roi );  
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C1RSfs )( cA16S *s,  int sStep, A16S *d, int dStep, ASZ roi, int scaleFactor )
{
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B2<C1> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B2<C1> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B2<C1> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B2<C1> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B2<C1> >(data, s, sStep, d, dStep, roi); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B2<C1> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B2<C1> >(data, s, sStep, d, dStep, roi); 
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C3IRSfs)(  A16S *sd, int sdStep, ASZ roi, int scaleFactor )
{ 
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B1<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B1<C3> >(data, sd, sdStep, roi); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B1<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B1<C3> >(data, sd, sdStep, roi ); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B1<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B1<C3> >(data, sd, sdStep, roi ); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B1<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B1<C3> >(data, sd, sdStep, roi ); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B1<C3> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B1<C3> >(data, sd, sdStep, roi );  	
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C3RSfs )( cA16S *s,  int sStep, A16S *d, int dStep, ASZ roi, int scaleFactor )
{
	if( scaleFactor < 0) {
		Ln_16s_ScaleLT0_B2<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_ScaleLT0_B2<C3> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 0){
		Ln_16s_Scale0_B2<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale0_B2<C3> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 1){
		Ln_16s_Scale1_B2<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale1_B2<C3> >(data, s, sStep, d, dStep, roi); 
	}
	else if(scaleFactor == 2){
		Ln_16s_Scale2_B2<C3> data(scaleFactor);
		return OPT_LEVEL::fe< Ln_16s_Scale2_B2<C3> >(data, s, sStep, d, dStep, roi); 
	}
	/*	else if (scale >2)*/
	Ln_16s_ScaleGT2_B2<C3> data(scaleFactor);
	return OPT_LEVEL::fe< Ln_16s_ScaleGT2_B2<C3> >(data, s, sStep, d, dStep, roi); 
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C1IR   )(  A32F *sd, int sdStep, ASZ roi )
{
	Ln_32f_tang_B1<C1> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B1<C1> >(data, sd, sdStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C3IR   )(  A32F *sd, int sdStep, ASZ roi )
{
	Ln_32f_tang_B1<C3> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B1<C3> >(data, sd, sdStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C1R    )( cA32F *s,  int sStep, A32F *d, int dStep, ASZ roi )
{
	Ln_32f_tang_B2<C1> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B2<C1> >(data, s, sStep, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C3R    )( cA32F *s,  int sStep, A32F *d, int dStep, ASZ roi )
{
	Ln_32f_tang_B2<C3> data ;
	return OPT_LEVEL::fe< Ln_32f_tang_B2<C3> >(data, s, sStep, d, dStep, roi );
}


// =============================
//			Complement
// =============================


struct Complement_32s : public fe1< Fw32s, C1 >
{   
	FE_SSE2_REF    
	IV SSE2( RegFile & r ) const                     
	{      
		__m128i comMask = _mm_setzero_si128();
		__m128i temp1 = _mm_setzero_si128();
		__m128i addMask = _mm_setzero_si128();
		
		addMask = _mm_srli_epi32 (r.dst[0].i,31);
		
		comMask = _mm_srai_epi32(r.dst[0].i,31);
		comMask = _mm_srli_epi32 (comMask,1);

		temp1 = _mm_and_si128(r.dst[0].i,comMask);
		r.dst[0].i = _mm_andnot_si128(comMask, r.dst[0].i);
		temp1 = _mm_xor_si128(temp1,comMask);
		temp1 = _mm_add_epi32(temp1,addMask);
		r.dst[0].i = _mm_or_si128(r.dst[0].i,temp1);
	}

	IV REFR( Fw32s *d) const                     
	{         
		Fw32s temp = d[0];
		Fw32s sign = temp & 0x80000000;
		temp  = (Fw32s)(temp >> 31);
		temp = (unsigned int)temp >> 1;
		d[0] = d[0] & 0x7FFFFFFF;
		d[0] = d[0] ^ temp;
		d[0] = d[0] + (Fw32s)((unsigned int)sign >> 31);
		d[0] = d[0] | sign;
	}      
} ;	


ASTAT PREFIX_OPT(OPT_PREFIX, fwiComplement_32s_C1IR		)(Fw32s* pSrcDst, int srcDstStep, FwiSize roiSize )
{
	Complement_32s data;
	return OPT_LEVEL::fe<Complement_32s> (data, pSrcDst, srcDstStep, roiSize);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
