/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "SetDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// C1R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C1R	)(  A8U  v,    A8U  * d, int dStep, ASZ roi )   
{	
	Set_C1R_Custom<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C1R_Custom<A8U> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C1R	)(  A16S  v,    A16S  * d, int dStep, ASZ roi )  
{
	Set_C1R_Custom<A16S> dat(v);
	return OPT_LEVEL::fe<Set_C1R_Custom<A16S> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C1R	)(  A32S  v,    A32S  * d, int dStep, ASZ roi )	
{	
	Set_C1R_Custom<A32S> dat(v);
	return OPT_LEVEL::fe<Set_C1R_Custom<A32S> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C1R	)(  A32F  v,    A32F  * d, int dStep, ASZ roi ) 	
{	
	Set_32f_C1R_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_C1R_Custom> (dat, d, dStep, roi);
}

// C3R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3R	)( cA8U  v[3], A8U  * d, int dStep, ASZ roi )	
{	
	Set_C3R_Custom<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C3R_Custom<A8U> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3R	)( cA16S  v[3], A16S  * d, int dStep, ASZ roi )	
{	
	Set_C3R_Custom<A16S> dat(v);
	return OPT_LEVEL::fe<Set_C3R_Custom<A16S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3R	)( cA32S  v[3], A32S  * d, int dStep, ASZ roi )	
{	
	Set_C3R_Custom<A32S> dat(v);
	return OPT_LEVEL::fe<Set_C3R_Custom<A32S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3R	)( cA32F  v[3], A32F  * d, int dStep, ASZ roi )   
{	
	Set_C3R_Custom<A32F> dat(v);
	return OPT_LEVEL::fe<Set_C3R_Custom<A32F> > (dat, d, dStep, roi);
}

// C4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4R	)( cA8U  v[4], A8U  * d, int dStep, ASZ roi ) 
{	

	Set_C4R_Custom<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C4R_Custom<A8U> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4R	)( cA16S  v[4], A16S  * d, int dStep, ASZ roi )
{ 
	Set_C4R_Custom<A16S> dat(v);
	return OPT_LEVEL::fe<Set_C4R_Custom<A16S> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4R	)( cA32S  v[4], A32S  * d, int dStep, ASZ roi )	
{	
	Set_C4R_Custom<A32S> dat(v);
	return OPT_LEVEL::fe<Set_C4R_Custom<A32S> >(dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4R	)( cA32F  v[4], A32F  * d, int dStep, ASZ roi )	
{	
	Set_32f_C4R_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_C4R_Custom> (dat, d, dStep, roi);
}

// AC4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_AC4R	)( cA8U  v[3], A8U  * d, int dStep, ASZ roi )
{
	Set_AC4R<A8U> dat(v);
	return OPT_LEVEL::fe<Set_AC4R<A8U> >(dat, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_AC4R	)( cA16S  v[3], A16S  * d, int dStep, ASZ roi )
{
	Set_AC4R<A16S> dat(v);
	return OPT_LEVEL::fe<Set_AC4R<A16S> >(dat, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_AC4R	)( cA32S  v[3], A32S  * d, int dStep, ASZ roi )
{
	Set_AC4R<A32S> dat(v);
	return OPT_LEVEL::fe<Set_AC4R<A32S> >(dat, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_AC4R	)( cA32F  v[3], A32F  * d, int dStep, ASZ roi )
{
	Set_AC4R<A32F> dat(v);
	return OPT_LEVEL::fe<Set_AC4R<A32F> >(dat, d, dStep, roi );
}

// C1MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C1MR	)(A8U  v,    A8U  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{	
	Set_C1MR<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C1MR<A8U> >(dat, m, mStep, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C1MR	)(  A16S  v,    A16S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_16s_C1MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_16s_C1MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C1MR)(  A32S  v,    A32S  * d, int dStep, ASZ roi, cA8U * m, int mStep )	    
{  	
	Set_32s_C1MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32s_C1MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C1MR	)(  A32F  v,    A32F  * d, int dStep, ASZ roi, cA8U * m, int mStep )	
{	
	Set_32f_C1MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_C1MR_Custom> (dat, m, mStep, d, dStep, roi);
}

// C3MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3MR	)( cA8U  v[3], A8U  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_C3MR<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C3MR<A8U> >(dat, m, mStep, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3MR	)( cA16S  v[3], A16S  * d, int dStep, ASZ roi, cA8U * m, int mStep )	
{
    if( v[0] == v[1] == v[2])
    {
        Set_16s_C3MR_eq_Custom dat(v);
	    return OPT_LEVEL::fe<Set_16s_C3MR_eq_Custom >(dat, m, mStep, d, dStep, roi );
    }
	Set_16s_C3MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_16s_C3MR_Custom >(dat, m, mStep, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3MR	)( cA32S  v[3], A32S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{	
	Set_32s_C3MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32s_C3MR_Custom> (dat, m, mStep, d, dStep, roi); 
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3MR	)( cA32F  v[3], A32F  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_32f_C3MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_C3MR_Custom> (dat, m, mStep, d, dStep, roi); 
}

// C4MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4MR	)( cA8U  v[4], A8U  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_C4MR<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C4MR<A8U> >(dat, m, mStep, d, dStep, roi );
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4MR	)( cA16S  v[4], A16S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{ 
	Set_16s_C4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_16s_C4MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4MR	)( cA32S  v[4], A32S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{ 
	Set_32s_C4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32s_C4MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4MR	)( cA32F  v[4], A32F  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{ 
	Set_32f_C4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_C4MR_Custom> (dat, m, mStep, d, dStep, roi);
}

// AC4MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_AC4MR	)( cA8U  v[3], A8U  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_AC4MR<A8U> dat(v);
	return OPT_LEVEL::fe<Set_AC4MR<A8U> >(dat, m, mStep, d, dStep, roi );
}

ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_AC4MR	)( cA16S  v[3], A16S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{	
	Set_16s_AC4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_16s_AC4MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_AC4MR	)( cA32S  v[3], A32S  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_32s_AC4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32s_AC4MR_Custom> (dat, m, mStep, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_AC4MR	)( cA32F  v[3], A32F  * d, int dStep, ASZ roi, cA8U * m, int mStep )
{
	Set_32f_AC4MR_Custom dat(v);
	return OPT_LEVEL::fe<Set_32f_AC4MR_Custom> (dat, m, mStep, d, dStep, roi);
}

// C3CR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3CR   )(  A8U  v,    A8U  * d, int dStep, ASZ roi ) 
{
	Set_C3CR_Custom<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C3CR_Custom<A8U> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3CR  )(  A16S  v,    A16S  * d, int dStep, ASZ roi ) 
{
	Set_C3CR_Custom<A16S> dat(v);
	return OPT_LEVEL::fe<Set_C3CR_Custom<A16S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3CR  )(  A32S  v,    A32S  * d, int dStep, ASZ roi )
{
	Set_C3CR_Custom<A32S> dat(v);
	return OPT_LEVEL::fe<Set_C3CR_Custom<A32S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3CR  )(  A32F  v,    A32F  * d, int dStep, ASZ roi ) 
{	
	Set_C3CR_Custom<A32F> dat(v);
	return OPT_LEVEL::fe<Set_C3CR_Custom<A32F> > (dat, d, dStep, roi);
}

// C4CR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4CR   )(  A8U  v,    A8U  * d, int dStep, ASZ roi ) 
{
	Set_C4CR_Custom<A8U> dat(v);
	return OPT_LEVEL::fe<Set_C4CR_Custom<A8U> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4CR  )(  A16S  v,    A16S  * d, int dStep, ASZ roi ) 
{
	Set_C4CR_Custom<A16S> dat(v);
	return OPT_LEVEL::fe<Set_C4CR_Custom<A16S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4CR  )(  A32S  v,    A32S  * d, int dStep, ASZ roi )
{	
	Set_C4CR_Custom<A32S> dat(v);
	return OPT_LEVEL::fe<Set_C4CR_Custom<A32S> > (dat, d, dStep, roi);
}
ASTAT PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4CR  )(  A32F  v,    A32F  * d, int dStep, ASZ roi )
{	
	Set_C4CR_Custom<A32F> dat(v);
	return OPT_LEVEL::fe<Set_C4CR_Custom<A32F> > (dat, d, dStep, roi);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
