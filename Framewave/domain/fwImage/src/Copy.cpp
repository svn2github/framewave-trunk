/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "CopyDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// C1R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_CR_Custom<A8U,C1> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A8U,C1> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1R		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A16S,C1> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A16S,C1> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1R		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A32S,C1> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32S,C1> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1R		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A32F,C1> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32F,C1> >(dat, s, sStep, d, dStep, roi ); 
    }

// C3R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_CR_Custom<A8U,C3> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A8U,C3> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3R		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {	
    Copy_C3R_Custom<A16S> dat;
    return OPT_LEVEL::fe<Copy_C3R_Custom<A16S> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3R		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A32S,C3> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32S,C3> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3R		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A32F,C3> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32F,C3> >(dat, s, sStep, d, dStep, roi ); 
    }

// C4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A8U,C4> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A8U,C4> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4R		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A16S,C4> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A16S,C4> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4R		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {	
    Copy_CR_Custom<A32S,C4> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32S,C4> >(dat, s, sStep, d, dStep, roi ); 

    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4R		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 	
    Copy_CR_Custom<A32F,C4> dat;
    return OPT_LEVEL::fe<Copy_CR_Custom<A32F,C4> >(dat, s, sStep, d, dStep, roi ); 
    }

// AC4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_AC4R<A8U> dat;
    return OPT_LEVEL::fe<Copy_AC4R<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4R		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_AC4R<A16S> dat;
    return OPT_LEVEL::fe<Copy_AC4R<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4R		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_AC4R<A32S> dat;
    return OPT_LEVEL::fe<Copy_AC4R<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4R		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Copy_AC4R<A32F> dat;
    return OPT_LEVEL::fe<Copy_AC4R<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C1MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1MR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C1MR<A8U> dat;
    return OPT_LEVEL::fe<Copy_C1MR<A8U> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1MR		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {	
    Copy_16s_C1MR_Custom dat;
    return OPT_LEVEL::fe<Copy_16s_C1MR_Custom>(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1MR		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_32_C1MR_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_32_C1MR_Custom<A32S> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1MR		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_32_C1MR_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_32_C1MR_Custom<A32F> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }

// C3MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3MR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C3MR<A8U> dat;
    return OPT_LEVEL::fe<Copy_C3MR<A8U> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3MR		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C3MR_16s_custom dat;
    return OPT_LEVEL::fe<Copy_C3MR_16s_custom>(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3MR		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C3MR_custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_C3MR_custom<A32S> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3MR		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C3MR_custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_C3MR_custom<A32F> >(dat, m, mStep, s, sStep, d, dStep, roi );

    }

// C4MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4MR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_C4MR<A8U> dat;
    return OPT_LEVEL::fe<Copy_C4MR<A8U> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4MR		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_16s_C4MR_Custom dat;
    return OPT_LEVEL::fe<Copy_16s_C4MR_Custom>(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4MR		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_32_C4MR_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_32_C4MR_Custom<A32S> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4MR		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_32_C4MR_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_32_C4MR_Custom<A32F> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }

// AC4MR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4MR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_AC4MR<A8U> dat;
    return OPT_LEVEL::fe<Copy_AC4MR<A8U> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4MR	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {	
    Copy_16s_AC4MR_Custom dat;
    return OPT_LEVEL::fe<Copy_16s_AC4MR_Custom>(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4MR	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {
    Copy_32_AC4MR_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_32_AC4MR_Custom<A32S> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4MR	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi, cA8U *m, int mStep )
    {	
    Copy_32_AC4MR_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_32_AC4MR_Custom<A32F> >(dat, m, mStep, s, sStep, d, dStep, roi );
    }

// C3CR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3CR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {	
    Copy_C3CR_Custom<A8U> dat;
    return OPT_LEVEL::fe<Copy_C3CR_Custom<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3CR		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi ) 
    {
    Copy_C3CR_Custom<A16S> dat;
    return OPT_LEVEL::fe<Copy_C3CR_Custom<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3CR		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi ) 
    {
    Copy_C3CR_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_C3CR_Custom<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3CR		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {	
    Copy_C3CR_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_C3CR_Custom<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C4CR
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4CR		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_C4CR_Custom<A8U> dat;
    return OPT_LEVEL::fe<Copy_C4CR_Custom<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4CR		)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi ) 
    {
    Copy_C4CR_Custom<A16S> dat;
    return OPT_LEVEL::fe<Copy_C4CR_Custom<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4CR		)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi ) 
    {
    Copy_C4CR_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_C4CR_Custom<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4CR		)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Copy_C4CR_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_C4CR_Custom<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C3C1R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3C1R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_C3C1R<A8U> dat;
    return OPT_LEVEL::fe<Copy_C3C1R<A8U> >(dat, s, sStep, d, dStep, roi ); 
    }

ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3C1R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_C3C1R<A16S> dat;
    return OPT_LEVEL::fe<Copy_C3C1R<A16S> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3C1R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_C3C1R<A32S> dat;
    return OPT_LEVEL::fe<Copy_C3C1R<A32S> >(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3C1R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Copy_C3C1R<A32F> dat;
    return OPT_LEVEL::fe<Copy_C3C1R<A32F> >(dat, s, sStep, d, dStep, roi ); 
    }

// C4C1R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4C1R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {	
    Copy_8u_C4C1R_Custom dat;
    return OPT_LEVEL::fe<Copy_8u_C4C1R_Custom>(dat, s, sStep, d, dStep, roi ); 
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4C1R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_16s_C4C1R_Custom dat;
    return OPT_LEVEL::fe<Copy_16s_C4C1R_Custom>(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4C1R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_C4C1R<A32S> dat;
    return OPT_LEVEL::fe<Copy_C4C1R<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4C1R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Copy_C4C1R<A32F> dat;
    return OPT_LEVEL::fe<Copy_C4C1R<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C1C3R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1C3R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_C1C3R<A8U> dat;
    return OPT_LEVEL::fe<Copy_C1C3R<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1C3R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_C1C3R<A16S> dat;
    return OPT_LEVEL::fe<Copy_C1C3R<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1C3R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_32_C1C3R_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_32_C1C3R_Custom<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1C3R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Copy_32_C1C3R_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_32_C1C3R_Custom<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C1C4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1C4R		)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_C1C4R_Custom<A8U> dat;
    return OPT_LEVEL::fe<Copy_C1C4R_Custom<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1C4R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_C1C4R_Custom<A16S> dat;
    return OPT_LEVEL::fe<Copy_C1C4R_Custom<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1C4R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_C1C4R_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_C1C4R_Custom<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1C4R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Copy_C1C4R_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_C1C4R_Custom<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// C3AC4R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3AC4R	)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_C3AC4R<A8U> dat;
    return OPT_LEVEL::fe<Copy_C3AC4R<A8U> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3AC4R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_C3AC4R<A16S> dat;
    return OPT_LEVEL::fe<Copy_C3AC4R<A16S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3AC4R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_C3AC4R<A32S> dat;
    return OPT_LEVEL::fe<Copy_C3AC4R<A32S> >(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3AC4R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Copy_C3AC4R<A32F> dat;
    return OPT_LEVEL::fe<Copy_C3AC4R<A32F> >(dat, s, sStep, d, dStep, roi );
    }

// AC4C3R
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4C3R	)( cA8U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Copy_8U_AC4C3R dat;
    return OPT_LEVEL::fe<Copy_8U_AC4C3R>(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4C3R	)( cA16S *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Copy_16S_AC4C3R dat;
    return OPT_LEVEL::fe<Copy_16S_AC4C3R>(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4C3R	)( cA32S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Copy_32S_AC4C3R dat;
    return OPT_LEVEL::fe<Copy_32S_AC4C3R>(dat, s, sStep, d, dStep, roi );
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4C3R	)( cA32F *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Copy_32F_AC4C3R dat;
    return OPT_LEVEL::fe<Copy_32F_AC4C3R>(dat, s, sStep, d, dStep, roi );
    }

// P3C3R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_P3C3R)( cA8U * const pSrc[3], int sStep, Fw8u *pDst, int dStep, ASZ roi )
    {	
    Copy_P3C3R_Custom<A8U> dat;
    return OPT_LEVEL::fe<Copy_P3C3R_Custom<A8U> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_P3C3R)( cA16S * const pSrc[3], int sStep, Fw16s *pDst, int dStep, ASZ roi )
    {
    Copy_P3C3R_Custom<A16S> dat;
    return OPT_LEVEL::fe<Copy_P3C3R_Custom<A16S> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_P3C3R)( cA32S * const pSrc[3], int sStep, Fw32s *pDst, int dStep, ASZ roi )
    {
    Copy_32_P3C3R_Custom<A32S> dat;
    return OPT_LEVEL::fe<Copy_32_P3C3R_Custom<A32S> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_P3C3R)( cA32F * const pSrc[3], int sStep, Fw32f *pDst, int dStep, ASZ roi )
    {
    Copy_32_P3C3R_Custom<A32F> dat;
    return OPT_LEVEL::fe<Copy_32_P3C3R_Custom<A32F> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pDst, dStep, roi );
    }

// P4C4R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_P4C4R)( cA8U * const pSrc[4], int sStep, Fw8u *pDst, int dStep, ASZ roi )
    {
    Copy_P4C4R_Custom<A8U> dat;
    return OPT_LEVEL::fec4S1D<Copy_P4C4R_Custom<A8U> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pSrc[3], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_P4C4R)( cA16S * const pSrc[4], int sStep, Fw16s *pDst, int dStep, ASZ roi )
    {
    Copy_P4C4R_Custom<A16S> dat;
    return OPT_LEVEL::fec4S1D<Copy_P4C4R_Custom<A16S> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pSrc[3], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_P4C4R)( cA32S * const pSrc[4], int sStep, Fw32s *pDst, int dStep, ASZ roi )
    {
    Copy_32_P4C4R_Custom<A32S> dat;
    return OPT_LEVEL::fec4S1D<Copy_32_P4C4R_Custom<A32S> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pSrc[3], sStep, pDst, dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_P4C4R)( cA32F * const pSrc[4], int sStep, Fw32f *pDst, int dStep, ASZ roi )
    {
    Copy_32_P4C4R_Custom<A32F> dat;
    return OPT_LEVEL::fec4S1D<Copy_32_P4C4R_Custom<A32F> >(dat, pSrc[0], sStep, pSrc[1], sStep, pSrc[2], sStep, pSrc[3], sStep, pDst, dStep, roi );
    }

// C3P3R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3P3R)( cA8U *pSrc, int sStep, A8U * const pDst[3], int dStep, ASZ roi )
    {
    Copy_C3P3R_Custom<A8U> dat;
    return OPT_LEVEL::fec1S3D<Copy_C3P3R_Custom<A8U> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3P3R)( cA16S *pSrc, int sStep, A16S * const pDst[3], int dStep, ASZ roi )
    {
    Copy_C3P3R_Custom<A16S> dat;
    return OPT_LEVEL::fec1S3D<Copy_C3P3R_Custom<A16S> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3P3R)( cA32S *pSrc, int sStep, A32S * const pDst[3], int dStep, ASZ roi )
    {
    Copy_32_C3P3R_Custom<A32S> dat;
    return OPT_LEVEL::fec1S3D<Copy_32_C3P3R_Custom<A32S> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3P3R)( cA32F *pSrc, int sStep, A32F * const pDst[3], int dStep, ASZ roi )
    {
    Copy_32_C3P3R_Custom<A32F> dat;
    return OPT_LEVEL::fec1S3D<Copy_32_C3P3R_Custom<A32F> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, roi );
    }

// C4P4R
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4P4R)( cA8U *pSrc, int sStep, Fw8u * const pDst[4], int dStep, ASZ roi )
    {
    Copy_C4P4R_Custom<A8U> dat;
    return OPT_LEVEL::fec1S4D<Copy_C4P4R_Custom<A8U> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, pDst[3], dStep, roi );
    }	
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4P4R)( cA16S *pSrc, int sStep, Fw16s * const pDst[4], int dStep, ASZ roi )
    {
    Copy_C4P4R_Custom<A16S> dat;
    return OPT_LEVEL::fec1S4D<Copy_C4P4R_Custom<A16S> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, pDst[3], dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4P4R)( cA32S *pSrc, int sStep, Fw32s * const pDst[4], int dStep, ASZ roi )
    {
    Copy_32_C4P4R_Custom<A32S> dat;
    return OPT_LEVEL::fec1S4D<Copy_32_C4P4R_Custom<A32S> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, pDst[3], dStep, roi );
    }
FwStatus PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4P4R)( cA32F *pSrc, int sStep, Fw32f * const pDst[4], int dStep, ASZ roi )
    {
    Copy_32_C4P4R_Custom<A32F> dat;
    return OPT_LEVEL::fec1S4D<Copy_32_C4P4R_Custom<A32F> >(dat, pSrc, sStep, pDst[0], dStep, pDst[1], dStep, pDst[2], dStep, pDst[3], dStep, roi );
    }


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
