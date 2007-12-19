/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ConvertDef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

// 8U16U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C1R )( cA8U *s, int sStep, A16U *d, int dStep, ASZ roi )
    {
    Convert_8u16_C14R<A8U,C1,A16U, C1> dat;
    return OPT_LEVEL::fe< Convert_8u16_C14R<A8U,C1,A16U, C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C3R )( cA8U *s, int sStep, A16U *d, int dStep, ASZ roi )
    {
    Convert_8u16_C3R<A8U, A16U> dat;
    return OPT_LEVEL::fe< Convert_8u16_C3R<A8U, A16U> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_AC4R)( cA8U *s, int sStep, A16U *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A8U,AC4,A16U,AC4> dat;
    return OPT_LEVEL::fe<  Convert_AC4R<A8U,AC4,A16U,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C4R )( cA8U *s, int sStep, A16U *d, int dStep, ASZ roi )
    {
    Convert_8u16_C14R<A8U,C4,A16U,C4> dat;
    return OPT_LEVEL::fe< Convert_8u16_C14R<A8U,C4,A16U,C4> > ( dat, s, sStep, d, dStep, roi);
    } 

// 8U16S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C1R )( cA8U *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Convert_8u16_C14R<A8U,C1,A16S,C1> dat;
    return OPT_LEVEL::fe< Convert_8u16_C14R<A8U,C1,A16S,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C3R )( cA8U *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Convert_8u16_C3R<A8U, A16S> dat;
    return OPT_LEVEL::fe< Convert_8u16_C3R<A8U, A16S> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_AC4R)( cA8U *s, int sStep, A16S *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A8U,AC4,A16S,AC4> dat;
    return OPT_LEVEL::fe<  Convert_AC4R<A8U,AC4,A16S,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C4R )( cA8U *s, int sStep, A16S *d, int dStep, ASZ roi )
    { 
    Convert_8u16_C14R<A8U,C4,A16S,C4> dat;
    return OPT_LEVEL::fe< Convert_8u16_C14R<A8U,C4,A16S,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 8U32S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C1R )( cA8U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8u32s_C14R_Custom<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_8u32s_C14R_Custom<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C3R )( cA8U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8u32s_C14R_Custom<C3,C3> dat;
    return OPT_LEVEL::fe< Convert_8u32s_C14R_Custom<C3,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_AC4R)( cA8U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A8U,AC4,A32S,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A8U,AC4,A32S,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C4R )( cA8U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8u32s_C14R_Custom<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_8u32s_C14R_Custom<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 8U32F
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C1R )( cA8U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_8u32f_C14R_Custom<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_8u32f_C14R_Custom<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C3R )( cA8U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_8u32f_C14R_Custom<C3,C3> dat;
    return OPT_LEVEL::fe< Convert_8u32f_C14R_Custom<C3,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_AC4R)( cA8U *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Convert_AC4R<A8U,AC4,A32F,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A8U,AC4,A32F,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C4R )( cA8U *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Convert_8u32f_C14R_Custom<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_8u32f_C14R_Custom<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 8S32S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C1R )( cA8S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8s32s_C14R_Custom<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_8s32s_C14R_Custom<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C3R )( cA8S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8s32s_C14R_Custom<C3,C3> dat;
    return OPT_LEVEL::fe< Convert_8s32s_C14R_Custom<C3,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_AC4R)( cA8S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A8S,AC4,A32S,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A8S,AC4,A32S,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C4R )( cA8S *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_8s32s_C14R_Custom<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_8s32s_C14R_Custom<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 8S32F
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C1R )( cA8S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_8s32f_C14R_Custom<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_8s32f_C14R_Custom<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C3R )( cA8S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_8s32f_C14R_Custom<C3,C3> dat;
    return OPT_LEVEL::fe< Convert_8s32f_C14R_Custom<C3,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_AC4R)( cA8S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A8S,AC4,A32F,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A8S,AC4,A32F,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C4R )( cA8S *s, int sStep, A32F *d, int dStep, ASZ roi )
    { 
    Convert_8s32f_C14R_Custom<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_8s32f_C14R_Custom<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 16U32S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C1R )( cA16U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_16u32s_C14R<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_16u32s_C14R<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C3R )( cA16U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_16u32s_C3R dat;
    return OPT_LEVEL::fe< Convert_16u32s_C3R > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_AC4R)( cA16U *s, int sStep, A32S *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A16U,AC4,A32S,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A16U,AC4,A32S,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C4R )( cA16U *s, int sStep, A32S *d, int dStep, ASZ roi )
    { 
    Convert_16u32s_C14R<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_16u32s_C14R<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 16U32F
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C1R )( cA16U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16u32f_C14R<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_16u32f_C14R<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C3R )( cA16U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16u32f_C3R dat;
    return OPT_LEVEL::fe< Convert_16u32f_C3R > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_AC4R)( cA16U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A16U,AC4,A32F,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A16U,AC4,A32F,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C4R )( cA16U *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16u32f_C14R<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_16u32f_C14R<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 16S32F
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C1R )( cA16S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16s32f_C14R<C1,C1> dat;
    return OPT_LEVEL::fe< Convert_16s32f_C14R<C1,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C3R )( cA16S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16s32f_C3R dat;
    return OPT_LEVEL::fe< Convert_16s32f_C3R > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_AC4R)( cA16S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_AC4R<A16S,AC4,A32F,AC4> dat;
    return OPT_LEVEL::fe< Convert_AC4R<A16S,AC4,A32F,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C4R )( cA16S *s, int sStep, A32F *d, int dStep, ASZ roi )
    {
    Convert_16s32f_C14R<C4,C4> dat;
    return OPT_LEVEL::fe< Convert_16s32f_C14R<C4,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 16S8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C1R )( cA16S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16s8u_Custom<A16S,A8U,C1> dat;
    return OPT_LEVEL::fe< Convert_16s8u_Custom<A16S,A8U,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C3R )( cA16S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16s8u_C3R_Custom<A16S,A8U,C3> dat;
    return OPT_LEVEL::fe< Convert_16s8u_C3R_Custom<A16S,A8U,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_AC4R)( cA16S *s, int sStep, A8U *d, int dStep, ASZ roi )
    { 
    Convert_16s8uAC4_Custom<A16S,A8U,AC4> dat;
    return OPT_LEVEL::fe< Convert_16s8uAC4_Custom<A16S,A8U,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C4R )( cA16S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16s8u_Custom<A16S,A8U,C4> dat;
    return OPT_LEVEL::fe< Convert_16s8u_Custom<A16S,A8U,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 16U8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C1R )( cA16U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16u8u_Custom<A16U,A8U,C1> dat;
    return OPT_LEVEL::fe< Convert_16u8u_Custom<A16U,A8U,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C3R )( cA16U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16u8u_C3R_Custom<A16U,A8U,C3> dat;
    return OPT_LEVEL::fe< Convert_16u8u_C3R_Custom<A16U,A8U,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_AC4R)( cA16U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16u8uAC4_Custom<A16U,A8U,AC4> dat;
    return OPT_LEVEL::fe< Convert_16u8uAC4_Custom<A16U,A8U,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C4R )( cA16U *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_16u8u_Custom<A16U,A8U,C4> dat;
    return OPT_LEVEL::fe< Convert_16u8u_Custom<A16U,A8U,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 32S8U
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C1R )( cA32S *s, int sStep, A8U *d, int dStep, ASZ roi )
    { 
    Convert_32s8u_Custom<A32S,A8U,C1> dat;
    return OPT_LEVEL::fe< Convert_32s8u_Custom<A32S,A8U,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C3R )( cA32S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_32s8u_Custom<A32S,A8U,C3> dat;
    return OPT_LEVEL::fe< Convert_32s8u_Custom<A32S,A8U,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_AC4R)( cA32S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_32s8uAC4_Custom<A32S,A8U,AC4> dat;
    return OPT_LEVEL::fe< Convert_32s8uAC4_Custom<A32S,A8U,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C4R )( cA32S *s, int sStep, A8U *d, int dStep, ASZ roi )
    {
    Convert_32s8u_Custom<A32S,A8U,C4> dat;
    return OPT_LEVEL::fe< Convert_32s8u_Custom<A32S,A8U,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// 32S8S
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C1R )( cA32S *s, int sStep, A8S *d, int dStep, ASZ roi )
    { 
    Convert_32s8s_Custom<A32S,A8S,C1> dat;
    return OPT_LEVEL::fe< Convert_32s8s_Custom<A32S,A8S,C1> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C3R )( cA32S *s, int sStep, A8S *d, int dStep, ASZ roi )
    {
    Convert_32s8s_Custom<A32S,A8S,C3> dat;
    return OPT_LEVEL::fe< Convert_32s8s_Custom<A32S,A8S,C3> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_AC4R)( cA32S *s, int sStep, A8S *d, int dStep, ASZ roi )
    {
    Convert_32s8sAC4_Custom<A32S,A8S,AC4> dat;
    return OPT_LEVEL::fe< Convert_32s8sAC4_Custom<A32S,A8S,AC4> > ( dat, s, sStep, d, dStep, roi);
    }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C4R )( cA32S *s, int sStep, A8S *d, int dStep, ASZ roi )
    {
    Convert_32s8s_Custom<A32S,A8S,C4> dat;
    return OPT_LEVEL::fe< Convert_32s8s_Custom<A32S,A8S,C4> > ( dat, s, sStep, d, dStep, roi);
    }

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
