/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef	__FWIMAGE_OPT_H__
#define	__FWIMAGE_OPT_H__

#include "buildnum.h"
#include "fwdev.h"
#include "fwImage.h"


OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C1R 		)( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C3R 		)( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C4R 		)( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_AC4R		)( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C1R 		)( const Fw8u *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C3R 		)( const Fw8u *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_C4R 		)( const Fw8u *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16s_AC4R		)( const Fw8u *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C1R 		)( const Fw8u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C3R 		)( const Fw8u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_C4R 		)( const Fw8u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32s_AC4R		)( const Fw8u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C1R 		)( const Fw8s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C3R 		)( const Fw8s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_C4R 		)( const Fw8s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32s_AC4R		)( const Fw8s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C1R 		)( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C3R 		)( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_C4R 		)( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32s_AC4R		)( const Fw16u *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C1R 		)( const Fw8u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C3R 		)( const Fw8u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_C4R 		)( const Fw8u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u32f_AC4R		)( const Fw8u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C1R 		)( const Fw8s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C3R 		)( const Fw8s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_C4R 		)( const Fw8s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8s32f_AC4R		)( const Fw8s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C1R 		)( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C3R 		)( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_C4R 		)( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u32f_AC4R		)( const Fw16u *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C1R 		)( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C3R 		)( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_C4R 		)( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s32f_AC4R		)( const Fw16s *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C1R 		)( const Fw16u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C3R 		)( const Fw16u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_C4R 		)( const Fw16u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16u8u_AC4R		)( const Fw16u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C1R 		)( const Fw16s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C3R 		)( const Fw16s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_C4R 		)( const Fw16s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_16s8u_AC4R		)( const Fw16s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C1R 		)( const Fw32s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C3R 		)( const Fw32s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_C4R 		)( const Fw32s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8u_AC4R		)( const Fw32s *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C1R 		)( const Fw32s *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C3R 		)( const Fw32s *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_C4R 		)( const Fw32s *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32s8s_AC4R		)( const Fw32s *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8u_C1R 		)( const Fw32f *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8u_C3R 		)( const Fw32f *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8u_C4R 		)( const Fw32f *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8u_AC4R		)( const Fw32f *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8s_C1R 		)( const Fw32f *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8s_C3R 		)( const Fw32f *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8s_C4R 		)( const Fw32f *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f8s_AC4R		)( const Fw32f *pSrc, int srcStep, Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16u_C1R 		)( const Fw32f *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16u_C3R 		)( const Fw32f *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16u_C4R 		)( const Fw32f *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16u_AC4R		)( const Fw32f *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16s_C1R 		)( const Fw32f *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16s_C3R 		)( const Fw32f *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16s_C4R 		)( const Fw32f *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_32f16s_AC4R		)( const Fw32f *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1R				)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3R				)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4R				)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3AC4R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4C3R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1MR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3MR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4MR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_AC4MR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3CR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4CR			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3C1R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4C1R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1C3R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C1C4R			)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C3P3R			)( const Fw8u *pSrc, int srcStep, Fw8u * const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_C4P4R			)( const Fw8u *pSrc, int srcStep, Fw8u * const pDst[4], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_P3C3R			)( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_8u_P4C4R			)( const Fw8u * const pSrc[4], int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3AC4R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4C3R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1MR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3MR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4MR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_AC4MR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3CR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4CR			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3C1R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4C1R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1C3R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C1C4R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C3P3R			)( const Fw16s *pSrc, int srcStep, Fw16s * const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_C4P4R			)( const Fw16s *pSrc, int srcStep, Fw16s * const pDst[4], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_P3C3R			)( const Fw16s * const pSrc[3], int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_16s_P4C4R			)( const Fw16s * const pSrc[4], int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3AC4R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4C3R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1MR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3MR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4MR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_AC4MR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3CR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4CR			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3C1R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4C1R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1C3R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C1C4R			)( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C3P3R			)( const Fw32s *pSrc, int srcStep, Fw32s * const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_C4P4R			)( const Fw32s *pSrc, int srcStep, Fw32s * const pDst[4], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_P3C3R			)( const Fw32s * const pSrc[3], int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32s_P4C4R			)( const Fw32s * const pSrc[4], int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3AC4R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4C3R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1MR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3MR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4MR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_AC4MR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw8u *pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3CR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4CR			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3C1R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4C1R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1C3R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C1C4R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C3P3R			)( const Fw32f * pSrc, int srcStep, Fw32f * const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_C4P4R			)( const Fw32f * pSrc, int srcStep, Fw32f * const pDst[4], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_P3C3R			)( const Fw32f * const pSrc[3], int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCopy_32f_P4C4R			)( const Fw32f * const pSrc[4], int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_C3R  )(const Fw8u *pSrc,int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_AC4R )(const Fw8u *pSrc,int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_16u_C3R )(const Fw16u *pSrc,int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_16u_AC4R)(const Fw16u *pSrc,int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32s_C3R )(const Fw32s *pSrc,int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32s_AC4R)(const Fw32s *pSrc,int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32f_C3R)(const Fw32f *pSrc,int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_32f_AC4R)(const Fw32f *pSrc,int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize,const int dstOrder[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSwapChannels_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep,FwiSize roiSize, const int dstOrder[3]);

#if BUILD_NUM_AT_LEAST( 102 )
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8u_C1R			)( Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8u_C3R			)( Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8s_C1R			)( Fw8s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8s_C3R			)( Fw8s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16u_C1R			)( Fw16u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16u_C3R			)( Fw16u *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16s_C1R			)( Fw16s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16s_C3R			)( Fw16s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32s_C1R			)( Fw32s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32s_C3R			)( Fw32s *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32f_C1R			)( Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32f_C3R			)( Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f offset, Fw32f slope, FwiAxis axis );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C1R		)(Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C3R		)(Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_C4R		)(Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8u_AC4R		)(Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C1R		)(Fw8s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C3R		)(Fw8s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_C4R		)(Fw8s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_8s_AC4R		)(Fw8s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C1R		)(Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C3R		)(Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_C4R		)(Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16u_AC4R		)(Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C1R		)(Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C3R		)(Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_C4R		)(Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_16s_AC4R		)(Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C1R		)(Fw32s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C3R		)(Fw32s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_C4R		)(Fw32s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32s_AC4R		)(Fw32s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C1R		)(Fw32f* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C3R		)(Fw32f* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_C4R		)(Fw32f* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiImageJaehne_32f_AC4R		)(Fw32f* pDst, int dstStep, FwiSize roiSize);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiZigzagFwd8x8_16s_C1)(const Fw16s* pSrc, Fw16s* pDst);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiZigzagInv8x8_16s_C1)(const Fw16s* pSrc, Fw16s* pDst);
#endif

OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C1R				)(       Fw8u  value,    Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3R				)( const Fw8u  value[3], Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_AC4R				)( const Fw8u  value[3], Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4R				)( const Fw8u  value[4], Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C1MR				)(       Fw8u  value,    Fw8u  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3MR				)( const Fw8u  value[3], Fw8u  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_AC4MR			)( const Fw8u  value[3], Fw8u  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4MR				)( const Fw8u  value[4], Fw8u  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C3CR				)(       Fw8u  value,    Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_8u_C4CR				)(       Fw8u  value,    Fw8u  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C1R				)(       Fw16s  value,    Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3R				)( const Fw16s  value[3], Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_AC4R			)( const Fw16s  value[3], Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4R				)( const Fw16s  value[4], Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C1MR			)(       Fw16s  value,    Fw16s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3MR			)( const Fw16s  value[3], Fw16s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_AC4MR			)( const Fw16s  value[3], Fw16s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4MR			)( const Fw16s  value[4], Fw16s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C3CR			)(		Fw16s  value,    Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_16s_C4CR			)(		Fw16s  value,    Fw16s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C1R				)(       Fw32s  value,    Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3R				)( const Fw32s  value[3], Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_AC4R			)( const Fw32s  value[3], Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4R				)( const Fw32s  value[4], Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C1MR			)(       Fw32s  value,    Fw32s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3MR			)( const Fw32s  value[3], Fw32s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_AC4MR			)( const Fw32s  value[3], Fw32s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4MR			)( const Fw32s  value[4], Fw32s  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C3CR			)(		Fw32s  value,    Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32s_C4CR			)(		Fw32s  value,    Fw32s  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C1R				)(       Fw32f  value,    Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3R				)( const Fw32f  value[3], Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_AC4R			)( const Fw32f  value[3], Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4R				)( const Fw32f  value[4], Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C1MR			)(       Fw32f  value,    Fw32f  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3MR			)( const Fw32f  value[3], Fw32f  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_AC4MR			)( const Fw32f  value[3], Fw32f  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4MR			)( const Fw32f  value[4], Fw32f  * pDst, int dstStep, FwiSize roiSize, const Fw8u * pMask, int maskStep );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C3CR			)(		Fw32f  value,    Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSet_32f_C4CR			)(		Fw32f  value,    Fw32f  * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C1R 		)( const Fw8u * pSrc, int srcStep, Fw16u * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C3R 		)( const Fw8u * pSrc, int srcStep, Fw16u * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_C4R 		)( const Fw8u * pSrc, int srcStep, Fw16u * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16u_AC4R		)( const Fw8u * pSrc, int srcStep, Fw16u * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C1R 		)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C3R 		)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_C4R 		)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u16s_AC4R		)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C1R 		)( const Fw8u * pSrc, int srcStep, Fw32s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C3R 		)( const Fw8u * pSrc, int srcStep, Fw32s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_C4R 		)( const Fw8u * pSrc, int srcStep, Fw32s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32s_AC4R		)( const Fw8u * pSrc, int srcStep, Fw32s * pDst, int dstStep, FwiSize roiSize );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C1R 		)( const Fw8u * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C3R 		)( const Fw8u * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_C4R 		)( const Fw8u * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_8u32f_AC4R		)( const Fw8u * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C1R 		)( const Fw16u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C3R 		)( const Fw16u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_C4R 		)( const Fw16u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16u8u_AC4R		)( const Fw16u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C1R 		)( const Fw16s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C3R 		)( const Fw16s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_C4R 		)( const Fw16s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_16s8u_AC4R		)( const Fw16s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C1R 		)( const Fw32s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C3R 		)( const Fw32s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_C4R 		)( const Fw32s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32s8u_AC4R		)( const Fw32s * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, FwHintAlgorithm hint );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C1R 		)( const Fw32f * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C3R 		)( const Fw32f * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_C4R 		)( const Fw32f * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR             FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiScale_32f8u_AC4R		)( const Fw32f * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize roiSize, Fw32f vMin, Fw32f vMax );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C1R 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C3R 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C4R 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_AC4R			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C1R 			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C3R 			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C4R 			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_AC4R			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C1IR 			)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C3IR 			)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_C4IR 			)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_16s_AC4IR			)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C1IR 			)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C3IR 			)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_C4IR 			)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbs_32f_AC4IR			)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_8u_C1R 			)( const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2,  int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_16u_C1R			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,  int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiff_32f_C1R			)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2,  int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_8u_C1R 		)( const Fw8u  *pSrc, int srcStep,  Fw8u  *pDst, int dstStep, FwiSize roiSize, int    value );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_16u_C1R		)( const Fw16u *pSrc, int srcStep,  Fw16u *pDst, int dstStep, FwiSize roiSize, int    value );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAbsDiffC_32f_C1R		)( const Fw32f *pSrc, int srcStep,  Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f value );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C1IRSfs  		)( const Fw8u   *pSrc,  int srcStep,		Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C3IRSfs  		)( const Fw8u   *pSrc,  int srcStep,		Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_AC4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C1RSfs  			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C3RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_C4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u_AC4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C1IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C3IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_AC4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C1RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C3RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_C4RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16s_AC4RSfs 		)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_C1IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_C3IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_AC4IRSfs 		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_C1RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_C3RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16sc_AC4RSfs		)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_C1IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_C3IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_AC4IRSfs 		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_C1RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_C3RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32sc_AC4RSfs		)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C1IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C3IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_AC4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C1R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C3R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C4R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_AC4R    		)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_C1IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_C3IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_AC4IR   		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_C1R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_C3R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32fc_AC4R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u32f_C1IR 			)( const Fw8u  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8s32f_C1IR 			)( const Fw8s  *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16u32f_C1IR 		)( const Fw16u *pSrc, int srcStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8u32f_C1IMR 		)( const Fw8u  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_8s32f_C1IMR 		)( const Fw8s  *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_16u32f_C1IMR 		)( const Fw16u *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAdd_32f_C1IMR 			)( const Fw32f *pSrc, int srcStep, const Fw8u *pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C1IRSfs  		)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C3IRSfs   		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_AC4IRSfs  		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C4IRSfs   		)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C1RSfs  		)( const Fw8u   *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C3RSfs  		)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_AC4RSfs  		)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_8u_C4RSfs  		)( const Fw8u   *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C1IRSfs 		)(       Fw16s value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C3IRSfs  		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_AC4IRSfs 		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C4IRSfs  		)( const Fw16s value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C1RSfs 		)( const Fw16s  *pSrc, int srcStep,       Fw16s value,    Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C3RSfs 		)( const Fw16s  *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_AC4RSfs 		)( const Fw16s  *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16s_C4RSfs 		)( const Fw16s  *pSrc, int srcStep, const Fw16s value[4], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C1IRSfs		)(       Fw16sc value,    Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C3IRSfs 		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_AC4IRSfs		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C1RSfs		)( const Fw16sc *pSrc, int srcStep,       Fw16sc value,    Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_C3RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_16sc_AC4RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C1IRSfs		)(       Fw32sc value,    Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C3IRSfs 		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_AC4IRSfs		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C1RSfs		)( const Fw32sc *pSrc, int srcStep,       Fw32sc value,    Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_C3RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32sc_AC4RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C1IR			)(       Fw32f value,    Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C3IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_AC4IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C4IR			)( const Fw32f value[4], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C1R			)( const Fw32f *pSrc, int srcStep,      Fw32f value,    Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C3R			)( const Fw32f *pSrc, int srcStep,const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_AC4R			)( const Fw32f *pSrc, int srcStep,const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32f_C4R			)( const Fw32f *pSrc, int srcStep,const Fw32f value[4], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C1IR			)(       Fw32fc  value,    Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C3IR			)( const Fw32fc  value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_AC4IR			)( const Fw32fc  value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C1R			)( const Fw32fc  *pSrc, int srcStep, Fw32fc value, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_C3R			)( const Fw32fc  *pSrc, int srcStep,const Fw32fc  value[3], Fw32fc  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddC_32fc_AC4R			)( const Fw32fc  *pSrc, int srcStep,const Fw32fc  value[3], Fw32fc  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8u32f_C1IR    	)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8s32f_C1IR    	)( const Fw8s *pSrc1, int src1Step, const Fw8s *pSrc2,   int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_16u32f_C1IR   	)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_32f_C1IR      	)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8u32f_C1IMR   	)( const Fw8u *pSrc1,  int src1Step, const Fw8u *pSrc2,  int src2Step, const Fw8u* pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_8s32f_C1IMR   	)( const Fw8s *pSrc1,  int src1Step, const Fw8s *pSrc2,  int src2Step, const Fw8u* pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_16u32f_C1IMR  	)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2, int src2Step, const Fw8u* pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddProduct_32f_C1IMR     	)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, const Fw8u* pMask, int maskStep, Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8u32f_C1IR 	   	)( const Fw8u *pSrc,  int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8s32f_C1IR 	   	)( const Fw8s *pSrc,  int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_16u32f_C1IR	   	)( const Fw16u *pSrc, int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_32f_C1IR   	   	)( const Fw32f *pSrc, int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8u32f_C1IMR    	)( const Fw8u *pSrc,  int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_8s32f_C1IMR    	)( const Fw8s *pSrc,  int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_16u32f_C1IMR   	)( const Fw16u *pSrc, int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddSquare_32f_C1IMR      	)( const Fw32f *pSrc, int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8u32f_C1IR   	)( const Fw8u *pSrc,  int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8s32f_C1IR   	)( const Fw8s *pSrc,  int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_16u32f_C1IR  	)( const Fw16u *pSrc, int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_32f_C1IR     	)( const Fw32f *pSrc, int srcStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8u32f_C1IMR  	)( const Fw8u *pSrc, int srcStep,  const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_8s32f_C1IMR  	)( const Fw8s *pSrc,  int srcStep,  const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_16u32f_C1IMR 	)( const Fw16u *pSrc, int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddWeighted_32f_C1IMR    	)( const Fw32f *pSrc, int srcStep, const Fw8u* pMask, int maskStep, Fw32f* pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f alpha);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_8u_C1IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_8u_C3IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_8u_C1RSfs  			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_8u_C3RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u   *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16s_C1IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16s_C3IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16s_C1RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16s_C3RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_C1IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_C3IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_AC4IRSfs 		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_C1RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_C3RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_16sc_AC4RSfs		)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_C1IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_C3IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_AC4IRSfs 		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_C1RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_C3RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32sc_AC4RSfs		)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C1IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C3IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_AC4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C1R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C3R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_C4R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32f_AC4R    		)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_C1IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_C3IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_AC4IR   		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_C1R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_C3R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDiv_32fc_AC4R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C1IRSfs			)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C3IRSfs   		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_AC4IRSfs  		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C1RSfs  		)( const Fw8u *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_C3RSfs			)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_8u_AC4RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C1IRSfs		)(       Fw16s value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C3IRSfs  		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_AC4IRSfs 		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C1RSfs 		)( const Fw16s *pSrc, int srcStep,       Fw16s value,    Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_C3RSfs			)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16s_AC4RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C1IRSfs		)(       Fw16sc value,    Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C3IRSfs 		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_AC4IRSfs		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C1RSfs		)( const Fw16sc *pSrc, int srcStep,       Fw16sc value,    Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_C3RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_16sc_AC4RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C1IRSfs		)(       Fw32sc value,    Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C3IRSfs 		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_AC4IRSfs		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C1RSfs		)( const Fw32sc *pSrc, int srcStep, Fw32sc value, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_C3RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32sc_AC4RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C1IR  			)(       Fw32f value,    Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C3IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_AC4IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C1R			)( const Fw32f *pSrc, int srcStep,       Fw32f value,    Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_C3R  			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32f_AC4R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C1IR 			)(       Fw32fc value,    Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C3IR			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_AC4IR			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C1R			)( const Fw32fc *pSrc, int srcStep,       Fw32fc value,    Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_C3R 			)( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDivC_32fc_AC4R			)( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C1IRSfs 			)(       Fw8u  *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C3IRSfs 			)(       Fw8u  *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C1RSfs  			)( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_8u_C3RSfs  			)( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C1IRSfs			)(       Fw16s *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C3IRSfs			)(       Fw16s *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C1RSfs 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_16s_C3RSfs 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C1IR   			)(       Fw32f *pSrcDst, int srcDstStep,  FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C3IR   			)(       Fw32f *pSrcDst, int srcDstStep,  FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C1R    			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiExp_32f_C3R    			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C1IRSfs 			)(       Fw8u  *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C3IRSfs 			)(       Fw8u  *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C1RSfs  			)( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_8u_C3RSfs  			)( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C1IRSfs			)(       Fw16s *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C3IRSfs			)(       Fw16s *pSrcDst,  int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C1RSfs 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_16s_C3RSfs 			)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C1IR   			)(       Fw32f *pSrcDst, int srcDstStep,  FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C3IR   			)(       Fw32f *pSrcDst, int srcDstStep,  FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C1R    			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLn_32f_C3R    			)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C1IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C3IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_AC4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C1RSfs  			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C3RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_C4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_8u_AC4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C1IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C3IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_AC4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C1RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C3RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_C4RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16s_AC4RSfs 		)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C1IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C3IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_AC4IRSfs 		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C1RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_C3RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_16sc_AC4RSfs		)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C1IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C3IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_AC4IRSfs 		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C1RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_C3RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32sc_AC4RSfs		)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C1IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C3IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_AC4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C1R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C3R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_C4R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32f_AC4R    		)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C1IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C3IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_AC4IR   		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C1R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_C3R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMul_32fc_AC4R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C1IRSfs  		)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C3IRSfs   		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_AC4IRSfs  		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C4IRSfs   		)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C1RSfs  		)( const Fw8u *pSrc, int srcStep,      Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C3RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_AC4RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_8u_C4RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C1IRSfs 		)(       Fw16s value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C3IRSfs  		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_AC4IRSfs 		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C4IRSfs  		)( const Fw16s value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C1RSfs 		)( const Fw16s *pSrc, int srcStep,       Fw16s value,    Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C3RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_AC4RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16s_C4RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[4], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_C1IRSfs		)(       Fw16sc value,    Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_C3IRSfs 		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_AC4IRSfs		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_C1RSfs		)( const Fw16sc *pSrc, int srcStep,       Fw16sc value,    Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_C3RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_16sc_AC4RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_C1IRSfs		)(       Fw32sc value,    Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_C3IRSfs 		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_AC4IRSfs		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_C1RSfs		)( const Fw32sc *pSrc, int srcStep,       Fw32sc value,    Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_C3RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32sc_AC4RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C1IR  			)(       Fw32f value,    Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C3IR  			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_AC4IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C4IR			)( const Fw32f value[4], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C1R			)( const Fw32f *pSrc, int srcStep,       Fw32f value,    Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C3R  			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_AC4R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32f_C4R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_C1IR 			)(       Fw32fc value,    Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_C3IR 			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_AC4IR			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_C1R			)( const Fw32fc *pSrc, int srcStep,       Fw32fc value,    Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_C3R 			)( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulC_32fc_AC4R			)( const Fw32fc *pSrc, int srcStep, const Fw32fc value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C1IR  		)(  const Fw8u  *pSrc,  int srcStep,        Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C3IR  		)(  const Fw8u  *pSrc,  int srcStep,        Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C4IR  		)(  const Fw8u  *pSrc,  int srcStep,        Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_AC4IR 		)(  const Fw8u  *pSrc,  int srcStep,        Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C1R   		)(  const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C3R   		)(  const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_C4R   		)(  const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_8u_AC4R  		)(  const Fw8u  *pSrc1, int src1Step, const Fw8u  *pSrc2, int src2Step, Fw8u  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C1IR 		)(  const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C3IR 		)(  const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C4IR 		)(  const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_AC4IR		)(  const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C1R  		)(  const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C3R  		)(  const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_C4R  		)(  const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulScale_16u_AC4R 		)(  const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C1IR  		)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C3IR  		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_AC4IR 		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C4IR  		)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C1R   		)( const Fw8u *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C3R   		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_AC4R  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_8u_C4R   		)( const Fw8u *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C1IR 		)(       Fw16u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C3IR 		)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_AC4IR		)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C4IR 		)( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C1R  		)( const Fw16u *pSrc, int srcStep,       Fw16u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C3R  		)( const Fw16u *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_AC4R 		)( const Fw16u *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMulCScale_16u_C4R  		)( const Fw16u *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C1IRSfs  		)(       Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C3IRSfs  		)(       Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C4IRSfs  		)(       Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_AC4IRSfs 		)(       Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C1RSfs   		)( const Fw8u  *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C3RSfs   		)( const Fw8u  *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_C4RSfs   		)( const Fw8u  *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_8u_AC4RSfs  		)( const Fw8u  *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C1IRSfs 		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C3IRSfs 		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C4IRSfs 		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_AC4IRSfs		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C1RSfs  		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C3RSfs  		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_C4RSfs  		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16u_AC4RSfs 		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C1IRSfs 		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C3IRSfs 		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C4IRSfs 		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_AC4IRSfs		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C1RSfs  		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C3RSfs  		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_C4RSfs  		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_16s_AC4RSfs 		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C1IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C3IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C4IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_AC4IR   		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C1R     		)( const Fw32f *pSrc,    int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_C3R     		)( const Fw32f *pSrc,    int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqr_32f_AC4R    		)( const Fw32f *pSrc,    int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_C1IRSfs 		)(       Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_C3IRSfs 		)(       Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_AC4IRSfs		)(       Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_C1RSfs  		)( const Fw8u *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_C3RSfs  		)( const Fw8u *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_8u_AC4RSfs 		)( const Fw8u *pSrc,    int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_C1IRSfs 		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_C3IRSfs 		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_AC4IRSfs		)(       Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_C1RSfs  		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_C3RSfs  		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16s_AC4RSfs 		)( const Fw16s *pSrc,    int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_C1IRSfs 		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_C3IRSfs 		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_AC4IRSfs		)(       Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_C1RSfs  		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_C3RSfs  		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_16u_AC4RSfs 		)( const Fw16u *pSrc,    int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_C1IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_C3IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_C4IR    		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_AC4IR   		)(       Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_C1R     		)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_C3R     		)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSqrt_32f_AC4R    		)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C1IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C3IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_AC4IRSfs  		)( const Fw8u   *pSrc,  int srcStep,        Fw8u   *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C1RSfs  			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C3RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_C4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_8u_AC4RSfs			)( const Fw8u   *pSrc1, int src1Step, const Fw8u   *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C1IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C3IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_AC4IRSfs  		)( const Fw16s  *pSrc,  int srcStep,        Fw16s  *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C1RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C3RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_C4RSfs 			)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16s_AC4RSfs 		)( const Fw16s  *pSrc1, int src1Step, const Fw16s  *pSrc2,   int src2Step, Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_C1IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_C3IRSfs  		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_AC4IRSfs 		)( const Fw16sc *pSrc,  int srcStep,        Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_C1RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_C3RSfs			)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_16sc_AC4RSfs		)( const Fw16sc *pSrc1, int src1Step, const Fw16sc *pSrc2,   int src2Step, Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_C1IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_C3IRSfs  		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_AC4IRSfs 		)( const Fw32sc *pSrc,  int srcStep,        Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_C1RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_C3RSfs			)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32sc_AC4RSfs		)( const Fw32sc *pSrc1, int src1Step, const Fw32sc *pSrc2,   int src2Step, Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C1IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C3IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_AC4IR    		)( const Fw32f  *pSrc,  int srcStep,        Fw32f  *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C1R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C3R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_C4R    			)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32f_AC4R    		)( const Fw32f  *pSrc1, int src1Step, const Fw32f  *pSrc2,   int src2Step, Fw32f  *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_C1IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_C3IR    		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_AC4IR   		)( const Fw32fc *pSrc,  int srcStep,        Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_C1R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_C3R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSub_32fc_AC4R    		)( const Fw32fc *pSrc1, int src1Step, const Fw32fc *pSrc2,   int src2Step, Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C1IRSfs  		)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C3IRSfs   		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_AC4IRSfs  		)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C4IRSfs   		)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C1RSfs  		)( const Fw8u *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C3RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_AC4RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_8u_C4RSfs  		)( const Fw8u *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C1IRSfs 		)(       Fw16s value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C3IRSfs  		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_AC4IRSfs 		)( const Fw16s value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C4IRSfs  		)( const Fw16s value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C1RSfs 		)( const Fw16s *pSrc, int srcStep,       Fw16s value,    Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C3RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_AC4RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16s_C4RSfs 		)( const Fw16s *pSrc, int srcStep, const Fw16s value[4], Fw16s *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_C1IRSfs		)(       Fw16sc value,    Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_C3IRSfs 		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_AC4IRSfs		)( const Fw16sc value[3], Fw16sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_C1RSfs		)( const Fw16sc *pSrc, int srcStep,       Fw16sc value,    Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_C3RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_16sc_AC4RSfs		)( const Fw16sc *pSrc, int srcStep, const Fw16sc value[3], Fw16sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_C1IRSfs		)(       Fw32sc value,    Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_C3IRSfs 		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_AC4IRSfs		)( const Fw32sc value[3], Fw32sc *pSrcDst, int srcDstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_C1RSfs		)( const Fw32sc *pSrc, int srcStep,       Fw32sc value,    Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_C3RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32sc_AC4RSfs		)( const Fw32sc *pSrc, int srcStep, const Fw32sc value[3], Fw32sc *pDst, int dstStep, FwiSize roiSize, int scaleFactor );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C1IR  			)(       Fw32f value,    Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C3IR  			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_AC4IR			)( const Fw32f value[3], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C4IR			)( const Fw32f value[4], Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C1R			)( const Fw32f *pSrc, int srcStep,       Fw32f value,    Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C3R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_AC4R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32f_C4R			)( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw32f *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_C1IR 			)(       Fw32fc value,    Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_C3IR 			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_AC4IR			)( const Fw32fc value[3], Fw32fc *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_C1R			)( const Fw32fc *pSrc, int srcStep,       Fw32fc value,     Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_C3R			)( const Fw32fc *pSrc, int srcStep, const Fw32fc  value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSubC_32fc_AC4R			)( const Fw32fc *pSrc, int srcStep, const Fw32fc  value[3], Fw32fc *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C1IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C3IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C4IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_AC4IR			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C1R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C3R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_C4R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_8u_AC4R 			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C1IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C3IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C4IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_AC4IR			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C1R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C3R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_C4R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_16u_AC4R 			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C1IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C3IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C4IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_AC4IR			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C1R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C3R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_C4R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAnd_32s_AC4R 			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C1IR 			)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C3IR  			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_AC4IR 			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C4IR  			)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C1R  			)( const Fw8u   *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C3R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_AC4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_8u_C4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C1IR 			)(       Fw16u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C3IR 			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_AC4IR			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C4IR 			)( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C1R 			)( const Fw16u  *pSrc, int srcStep,       Fw16u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C3R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_AC4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_16u_C4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C1IR 			)(       Fw32s value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C3IR 			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_AC4IR			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C4IR 			)( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C1R 			)( const Fw32s  *pSrc, int srcStep,       Fw32s value,    Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C3R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_AC4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAndC_32s_C4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiComplement_32s_C1IR		)(Fw32s* pSrcDst, int srcDstStep,FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C1IR  			)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C3IR				)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_AC4IR			)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C4IR				)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C1R  			)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C3R  			)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_AC4R  			)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiNot_8u_C4R  			)( const Fw8u   *pSrc, int srcStep,    Fw8u *pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C1IR 				)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C3IR 				)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C4IR 				)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_AC4IR				)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C1R  				)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C3R  				)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_C4R  				)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_8u_AC4R 				)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C1IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C3IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C4IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_AC4IR			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C1R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C3R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_C4R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_16u_AC4R 			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C1IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C3IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C4IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_AC4IR			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C1R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C3R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_C4R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOr_32s_AC4R 			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C1IR  			)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C3IR   			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_AC4IR  			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C4IR   			)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C1R  			)( const Fw8u   *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C3R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_AC4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_8u_C4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C1IR 			)(       Fw16u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C3IR  			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_AC4IR 			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C4IR  			)( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C1R 			)( const Fw16u  *pSrc, int srcStep,       Fw16u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C3R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_AC4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_16u_C4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C1IR 			)(       Fw32s value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C3IR  			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_AC4IR 			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C4IR  			)( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C1R 			)( const Fw32s  *pSrc, int srcStep,       Fw32s value,    Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C3R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_AC4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiOrC_32s_C4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C1IR  		)(       Fw32u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C3IR   		)( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_AC4IR  		)( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C4IR   		)( const Fw32u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C1R  		)( const Fw8u   *pSrc, int srcStep,       Fw32u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C3R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_AC4R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C4R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C1IR 		)(       Fw32u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C3IR  		)( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_AC4IR 		)( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C4IR  		)( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C1R 		)( const Fw16u  *pSrc, int srcStep,       Fw32u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C3R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_AC4R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C4R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C1IR 		)(       Fw32u value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C3IR  		)( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_AC4IR 		)( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C4IR  		)( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C1R 		)( const Fw32s  *pSrc, int srcStep,       Fw32u value,    Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C3R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_AC4R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C4R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C1IR  		)(       Fw32u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C3IR   		)( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_AC4IR  		)( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C4IR   		)( const Fw32u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C1R  		)( const Fw8u   *pSrc, int srcStep,       Fw32u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C3R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_AC4R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C4R  		)( const Fw8u   *pSrc, int srcStep, const Fw32u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C1IR  		)(       Fw32u value,    Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C3IR   		)( const Fw32u value[3], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_AC4IR  		)( const Fw32u value[3], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C4IR   		)( const Fw32u value[4], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C1R  		)( const Fw8s   *pSrc, int srcStep,       Fw32u value,    Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C3R  		)( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_AC4R  		)( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C4R  		)( const Fw8s   *pSrc, int srcStep, const Fw32u value[4], Fw8s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C1IR 		)(       Fw32u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C3IR  		)( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_AC4IR 		)( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C4IR  		)( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C1R 		)( const Fw16u  *pSrc, int srcStep,       Fw32u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C3R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_AC4R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C4R 		)( const Fw16u  *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C1IR 		)(       Fw32u value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C3IR  		)( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_AC4IR 		)( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C4IR  		)( const Fw32u value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C1R 		)( const Fw16s  *pSrc, int srcStep,       Fw32u value,    Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C3R 		)( const Fw16s  *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_AC4R 		)( const Fw16s  *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C4R 		)( const Fw16s  *pSrc, int srcStep, const Fw32u value[4], Fw16s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C1IR 		)(       Fw32u value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C3IR  		)( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_AC4IR 		)( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C4IR  		)( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C1R 		)( const Fw32s  *pSrc, int srcStep,       Fw32u value,    Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C3R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_AC4R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C4R 		)( const Fw32s  *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C1IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C3IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C4IR 			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_AC4IR			)( const Fw8u	*pSrc,  int srcStep,        Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C1R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C3R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_C4R  			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_8u_AC4R 			)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2,   int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C1IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C3IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C4IR 			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_AC4IR			)( const Fw16u *pSrc,  int srcStep,        Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C1R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C3R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_C4R  			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_16u_AC4R 			)( const Fw16u *pSrc1, int src1Step, const Fw16u *pSrc2,   int src2Step, Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C1IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C3IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C4IR 			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_AC4IR			)( const Fw32s *pSrc,  int srcStep,        Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C1R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C3R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_C4R  			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXor_32s_AC4R 			)( const Fw32s *pSrc1, int src1Step, const Fw32s *pSrc2,   int src2Step, Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C1IR  			)(       Fw8u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C3IR			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_AC4IR			)( const Fw8u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C4IR			)( const Fw8u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C1R  			)( const Fw8u   *pSrc, int srcStep,       Fw8u value,    Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C3R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_AC4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_8u_C4R  			)( const Fw8u   *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C1IR 			)(       Fw16u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C3IR			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_AC4IR			)( const Fw16u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C4IR			)( const Fw16u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C1R 			)( const Fw16u  *pSrc, int srcStep,       Fw16u value,    Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C3R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_AC4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_16u_C4R 			)( const Fw16u  *pSrc, int srcStep, const Fw16u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C1IR 			)(       Fw32s value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C3IR			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_AC4IR			)( const Fw32s value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C4IR			)( const Fw32s value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C1R 			)( const Fw32s  *pSrc, int srcStep,       Fw32s value,    Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C3R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_AC4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[3], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiXorC_32s_C4R 			)( const Fw32s  *pSrc, int srcStep, const Fw32s value[4], Fw32s *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GT_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LT_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, Fw8u value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, Fw16s value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, Fw32f value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, Fw8u value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, Fw16s value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, Fw32f value, FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, Fw8u value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, Fw16s value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, Fw32f value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, Fw8u value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, Fw16s value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, Fw32f value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_8u_C4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_16s_C4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_GTVal_32f_C4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, Fw8u value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, Fw16s value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, Fw32f value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, Fw8u value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, Fw16s value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, Fw32f value);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_8u_C4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[4], const Fw8u value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_16s_C4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[4], const Fw16s value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTVal_32f_C4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[4], const Fw32f value[4]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u thresholdLT, Fw8u valueLT, Fw8u thresholdGT, Fw8u valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s thresholdLT, Fw16s valueLT, Fw16s thresholdGT, Fw16s valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f thresholdLT, Fw32f valueLT, Fw32f thresholdGT, Fw32f valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u thresholdLT, Fw8u valueLT, Fw8u thresholdGT, Fw8u valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s thresholdLT, Fw16s valueLT, Fw16s thresholdGT, Fw16s valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f thresholdLT, Fw32f valueLT, Fw32f thresholdGT, Fw32f valueGT);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3]);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C1R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C1R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C1R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C3R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C3R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C3R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_C4R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_C4R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_C4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_8u_AC4R)( const Fw8u *pSrc1, int src1Step, const Fw8u *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_16s_AC4R)( const Fw16s *pSrc1, int src1Step, const Fw16s *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompare_32f_AC4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u value, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s value, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f value, Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C3R)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C3R)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C3R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_AC4R)( const Fw8u *pSrc, int srcStep, const Fw8u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_AC4R)( const Fw16s *pSrc, int srcStep, const Fw16s value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_AC4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_8u_C4R)( const Fw8u *pSrc, int srcStep, const Fw8u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_16s_C4R)( const Fw16s *pSrc, int srcStep, const Fw16s value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareC_32f_C4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, FwCmpOp fwCmpOp );
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C1R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C3R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_C4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEps_32f_AC4R)( const Fw32f *pSrc1, int src1Step, const Fw32f *pSrc2, int src2Step, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f value, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C3R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_AC4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[3], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);
OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiCompareEqualEpsC_32f_C4R)( const Fw32f *pSrc, int srcStep, const Fw32f value[4], Fw8u *pDst, int dstStep, FwiSize roiSize, Fw32f eps);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilate3x3_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize);

#if BUILD_NUM_AT_LEAST( 9999 )
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_8u_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyGetSize_32f_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_8u_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_8u_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_8u_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_32f_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_32f_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvGetSize_32f_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, int* pSize);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C1R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C3R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C4R)(int roiWidth, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C1R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C3R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C4R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C1R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C3R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C4R)(FwiMorphAdvState** ppState, FwiSize roiSize, const Fw8u* pMask, FwiSize maskSize, FwiPoint anchor);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDilateBorderReplicate_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiErodeBorderReplicate_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphOpenBorder_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphCloseBorder_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphTophatBorder_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C4R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C3R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C4R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphologyFree)(FwiMorphState* pState);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMorphAdvFree)(FwiMorphState* pState);
#endif
#if BUILD_NUM_AT_LEAST( 9999 )
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_8u_C1R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                           const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                           Fw8u * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_8u_C3R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                           const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                           Fw8u * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_16s_C1R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw16s * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_16s_C3R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw16s * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_32f_C1R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw32f * pDst, int dstStep );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvFull_32f_C3R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw32f * pDst, int dstStep );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_8u_C1R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw8u * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_8u_C3R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw8u * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_16s_C1R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw16s * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_16s_C3R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw16s * pDst, int dstStep, int divisor );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_32f_C1R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                      Fw32f * pDst, int dstStep );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvValid_32f_C3R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
OREFR OSSE2                                      const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
OREFR OSSE2                                     Fw32f * pDst, int dstStep );

#endif

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Fwd_16s_C1 )(const Fw16s* pSrc, Fw16s* pDst);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Fwd_16s_C1I )(Fw16s* pSrcDst);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_16s_C1 )(const Fw16s* pSrc, Fw16s* pDst);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiDCT8x8Inv_16s_C1I )(Fw16s* pSrcDst);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C1R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C3R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_C4R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							  Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
							  double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							  Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
							  double xFactor, double yFactor, int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_P3R )( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* const pDst[3], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw16u* const pDst[3], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw32f* const pDst[3], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_8u_P4R )( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw8u* const pDst[4], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw16u* const pDst[4], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResize_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							 Fw32f* const pDst[4], int dstStep, FwiSize dstRoiSize,
							 double xFactor, double yFactor, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
							      double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								    Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								    double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* const pDst[3], int dstStep, FwiSize dstRoiSize,
								  double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* const pDst[3], int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* const pDst[3], int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* const pDst[4], int dstStep, FwiSize dstRoiSize,
								  double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* const pDst[4], int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeCenter_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* const pDst[4], int dstStep, FwiSize dstRoiSize,
								   double xFactor, double yFactor, double xCenter, double yCenter, int interpolation );

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetResizeFract)(FwiSize srcSize, FwiRect srcRoi, double xFactor, double yFactor,
							 double* xFr, double* yFr, int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C1R )( const Fw8u * pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								 double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								 Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								 double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw16u* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
								   Fw32f* pDst, int dstStep, FwiSize dstRoiSize,
								   double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								 Fw8u* const pDst[3], int dstStep, FwiSize dstRoiSize,
								 double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw16u* const pDst[3], int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw32f* const pDst[3], int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								 Fw8u* const pDst[4], int dstStep, FwiSize dstRoiSize,
								 double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw16u* const pDst[4], int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeShift_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
								  Fw32f* const pDst[4], int dstStep, FwiSize dstRoiSize,
								  double xFr, double yFr, double xShift, double yShift, int interpolation );

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixelGetBufSize)(FwiSize dstSize, int nChannel,int interpolation, int* pBufferSize);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C1R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C3R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_C4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_AC4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C1R)(const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C3R)(const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_C4R)(const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_AC4R)(const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C1R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C3R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_C4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_AC4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* pDst, int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_P3R)(const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_8u_P4R)(const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw8u* const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_P3R)(const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_16u_P4R)(const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw16u* const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_P3R)(const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* const pDst[3], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeSqrPixel_32f_P4R)(const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
			Fw32f* const pDst[4], int dstStep, FwiRect dstRoi, double xFactor, double yFactor,
			double xShift, double yShift, int interpolation, Fw8u* pBuffer);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiResizeYUV422_8u_C2R)( const Fw8u* pSrc, FwiSize srcSize,
								  int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiSize dstRoiSize,
								  double xFactor, double yFactor, int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffine_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw16u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineBack_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][3], int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpAffineQuad_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetAffineQuad)(FwiRect srcRoi, double quad[4][2], const double coeffs[2][3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetAffineBound)(FwiRect srcRoi, double bound[2][2], const double coeffs[2][3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetAffineTransform)(FwiRect srcRoi, const double quad[4][2],double coeffs[2][3]);

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspective_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveBack_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[3][3], int interpolation );

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpPerspectiveQuad_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation );

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveQuad)(FwiRect srcRoi, double quad[4][2], const double coeffs[3][3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveBound)(FwiRect srcRoi, double bound[2][2], const double coeffs[3][3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetPerspectiveTransform)(FwiRect srcRoi, const double quad[4][2],double coeffs[3][3]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C1R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C3R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_C4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_AC4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C1R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C3R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_C4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_AC4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_P3R)(const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_8u_P4R)(const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_P3R)(const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinear_32f_P4R)(const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C1R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C3R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_C4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_AC4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C1R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C3R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_C4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_AC4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_P3R)(const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_8u_P4R)(const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_P3R)(const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearBack_32f_P4R)(const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep,
								FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
								const double coeffs[2][4], int interpolation);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C1R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C3R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_C4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_AC4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C1R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C3R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_C4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_AC4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* pDst, int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_P3R)(const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_8u_P4R)(const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw8u* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_P3R)(const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[3], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiWarpBilinearQuad_32f_P4R)(const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi,
							        const double srcQuad[4][2], Fw32f* const pDst[4], int dstStep, FwiRect dstRoi,
									const double dstQuad[4][2], int interpolation);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetBilinearQuad)(FwiRect srcRoi, double quad[4][2], const double coeffs[2][4]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetBilinearBound)(FwiRect srcRoi, double bound[2][2], const double coeffs[2][4]);

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetBilinearTransform)(FwiRect srcRoi, const double quad[4][2],double coeffs[2][4]);

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C1R   )( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C3R   )( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C4R   )( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_AC4R  )( const Fw8u  *pSrc, int srcStep, Fw8u  *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C1R  )( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C3R  )( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C4R  )( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_AC4R )( const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C1R  )( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C3R  )( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C4R  )( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_AC4R )( const Fw32s *pSrc, int srcStep, Fw32s *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C1R  )( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C3R  )( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C4R  )( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_AC4R )( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C1IR )(Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C3IR )(Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_C4IR )(Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32f_AC4IR)(Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C1IR  )(Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C3IR  )(Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_C4IR  )(Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_8u_AC4IR )(Fw8u  *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C1IR )(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C3IR )(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_C4IR )(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_16u_AC4IR)(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C1IR )(Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C3IR )(Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_C4IR )(Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiMirror_32s_AC4IR)(Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize, FwiAxis flip);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C1R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C3R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C4R )( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C1R )( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C3R )( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C4R )( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* pDst, int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_P3R )( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_P4R )( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw8u* const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_P3R )( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_P4R )( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, const Fw32f* pxMap, int xMapStep, const Fw32f* pyMap, int yMapStep, Fw32f* const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C1R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C3R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_C4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_AC4R)( const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C1R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C3R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_C4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_AC4R)( const Fw16u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C1R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C3R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_C4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_AC4R)( const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_P3R)( const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_8u_P4R)( const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_P3R)( const Fw16u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_16u_P4R)( const Fw16u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_P3R)( const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotate_32f_P4R)( const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xShift, double yShift, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetRotateShift)(double xCenter, double yCenter, double angle, double* xShift, double* yShift);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiAddRotateShift)(double xCenter, double yCenter, double angle, double* xShift, double* yShift);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetRotateQuad )(FwiRect srcRoi, double quad[4][2], double angle, double xShift, double yShift);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetRotateBound)(FwiRect srcRoi, double bound[2][2], double angle, double xShift, double yShift);
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C1R)( const Fw8u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C3R)( const Fw8u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_C4R)( const Fw8u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_AC4R)( const Fw8u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C1R)( const Fw16u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C3R)( const Fw16u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_C4R)( const Fw16u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_AC4R)( const Fw16u* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C1R)( const Fw32f* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C3R)( const Fw32f* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_C4R)( const Fw32f* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_AC4R)( const Fw32f* pSrc,FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_P3R)( const Fw8u* const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_8u_P4R)( const Fw8u* const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_P3R)( const Fw16u* const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_16u_P4R)( const Fw16u* const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw16u* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_P3R)( const Fw32f* const pSrc[3],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiRotateCenter_32f_P4R)( const Fw32f* const pSrc[4],FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi, double angle, double xCenter, double yCenter, int interpolation );

OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C1R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C3R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_AC4R)(const Fw8u* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C1R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C3R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_AC4R)(const Fw32f* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* pDst, int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation );
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_P3R)(const Fw8u* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[3], int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_8u_P4R)(const Fw8u* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw8u* const pDst[4], int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_P3R)(const Fw32f* const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[3], int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiShear_32f_P4R)(const Fw32f* const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, Fw32f* const pDst[4], int dstStep, FwiRect dstRoi, double xShear, double yShear, double xShift, double yShift, int interpolation);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetShearQuad )(FwiRect srcRoi, double quad[4][2], double xShear, double yShear, double xShift, double yShift);
OREFR       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiGetShearBound)(FwiRect srcRoi, double bound[2][2], double xShear, double yShear, double xShift, double yShift);

#ifndef __SUNPRO_CC

OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2 FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSharpen_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

//@NEW FUNCTIONS BLOCK START
//OREFR OSSE2 OF10H FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiConvert_8u16u_C1R 		)( const Fw8u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C1IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C3IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C4IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_AC4IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C1IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C3IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C4IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_AC4IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C1IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C3IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C4IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_AC4IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor );


OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                           Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                           Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                           Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                           Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                           Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                           Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                           Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                           Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                           FwiSize maskSize, FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                               FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                                FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                                 Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                                 FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                                 FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                                  Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                                  FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                                 Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                                 FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                                  Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                                  FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C1R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C3R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C1R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C3R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor );


OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C1R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C3R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int yAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C1R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C3R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int yAnchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C1R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C3R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, int kernelSize,
                              int xAnchor, int divisor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C1R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C3R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, int kernelSize,
                              int xAnchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizMask_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertMask_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize,  FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u16s_C1R)( const Fw8u * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8s16s_C1R)( const Fw8s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );
OREFR OSSE2       FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize );

#endif

OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_8u_C3R        )( const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_8u_AC4R       )( const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToXYZ_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_8u_C3R        )( const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_8u_AC4R       )( const Fw8u*  pSrc, int srcStep, Fw8u*  pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiXYZToRGB_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_8u_C3R        )( const Fw8u*  pSrc, int srcStep, Fw8u*  pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_8u_AC4R       )( const Fw8u*  pSrc, int srcStep, Fw8u*  pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToLUV_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_8u_C3R        )( const Fw8u* pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_8u_AC4R       )( const Fw8u* pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLUVToRGB_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToLab_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToLab_8u16u_C3R     )( const Fw8u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLabToBGR_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLabToBGR_16u8u_C3R     )( const Fw16u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCC_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCCToRGB_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHLS_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_16s_C3R       )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_16s_AC4R      )( const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_32f_C3R       )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToRGB_32f_AC4R      )( const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_C3P3R      )( const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_AC4P4R     )( const A8U* pSrc, int srcStep, A8U* const pDst[4], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_P3C3R      )( const A8U* const pSrc[3], int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_AP4C4R     )( const A8U* const pSrc[4], int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_P3R        )( const A8U* const pSrc[3], int srcStep, A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToHLS_8u_AP4R       )( const A8U* const pSrc[4], int srcStep, A8U* const pDst[4], int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_C3P3R      )(const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_AC4P4R     )(const A8U* pSrc, int srcStep, A8U* const pDst[4], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_AP4R       )(const A8U* const pSrc[4], int srcStep, A8U* const pDst[4], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_P3R        )(const A8U* const pSrc[3], int srcStep, A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_AP4C4R     )(const A8U* const pSrc[4], int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHLSToBGR_8u_P3C3R      )(const A8U* const pSrc[3], int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHSV_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHSV_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHSV_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToHSV_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHSVToRGB_8u_C3R        )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHSVToRGB_8u_AC4R       )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHSVToRGB_16u_C3R       )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiHSVToRGB_16u_AC4R      )( const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );



//----------------------------
// RGBToYCbCr
//----------------------------
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_8u_C3R          )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_8u_AC4R         )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr_8u_P3R          )( const Fw8u * const pSrc[3], int srcStep, Fw8u* const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_8u_C3R          )( const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_8u_AC4R         )( const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_8u_P3R          )( const Fw8u* const pSrc[3], int srcStep, Fw8u* const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB_8u_P3C3R        )( const Fw8u* const pSrc[3], int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB565_8u16u_C3R    )( const Fw8u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB555_8u16u_C3R    )( const Fw8u* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB444_8u16u_C3R    )( const Fw8u* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB565_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB555_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToRGB444_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR565_8u16u_C3R    )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR555_8u16u_C3R    )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR444_8u16u_C3R    )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR565_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR555_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCrToBGR444_8u16u_P3C3R  )(const A8U* const pSrc[3], int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr422_8u_C3C2R     )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr422_8u_P3C2R     )(const A8U* const pSrc[3], int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB_8u_C2C3R     )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB_8u_C2P3R     )(const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB_8u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToCbYCr422_8u_C3C2R     )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToCbYCr422Gamma_8u_C3C2R    )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiCbYCr422ToRGB_8u_C2C3R         )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToCbYCr422_8u_AC4C2R        )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiCbYCr422ToBGR_8u_C2C4R         )(const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize, A8U aval );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB565_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB555_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB444_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB565_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB555_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToRGB444_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR565_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR555_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR444_8u16u_C2C3R   )(const A8U* pSrc, int srcStep, A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR565_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR555_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToBGR444_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToRGB565_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToRGB555_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToRGB444_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToBGR_8u_P3C3R         )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToBGR565_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToBGR555_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToBGR444_8u16u_P3C3R   )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR_8u_P3C3R         )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr411ToBGR_8u_P3C4R         )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize, A8U aval );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYCbCr420_8u_C3P3R         )(const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep[3], FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr420ToRGB_8u_P3C3R         )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr420_8u_C3P3R         )(const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiBGRToYCbCr420_8u_AC4P3R        )(const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep[3], FwiSize roiSize );


//----------------------------
// RGBToYUV
//----------------------------
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV_8u_C3R                )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV_8u_AC4R               )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV_8u_P3R                )( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV_8u_C3P3R              )( const Fw8u *pSrc, int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );

OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUVToRGB_8u_C3R                )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUVToRGB_8u_AC4R               )( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUVToRGB_8u_P3R                )( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2 OF10H   FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUVToRGB_8u_P3C3R              )( const Fw8u * const pSrc[3], int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV422_8u_C3C2R           )( const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV422_8u_P3R             )( const A8U* const pSrc[3], int srcStep, A8U* pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV422_8u_P3              )( const A8U* const pSrc[3], A8U* pDst[3], FwiSize imgSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV422_8u_C3P3R           )( const A8U* pSrc, int srcStep, A8U* pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV422_8u_C3P3            )( const A8U* pSrc, A8U* pDst[3], FwiSize imgSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_C2C3R           )( const A8U* pSrc, int srcStep, A8U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_P3R             )( const A8U* const pSrc[3], int srcStep[3], A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_P3              )( const A8U* const pSrc[3], A8U* const pDst[3], FwiSize imgSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_P3C3R           )( const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_P3C3            )( const A8U* const pSrc[3], A8U* pDst, FwiSize imgSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV422ToRGB_8u_P3AC4R          )( const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV420_8u_P3R             )( const A8U* const pSrc[3], int srcStep, A8U* const pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV420_8u_P3              )( const A8U* const pSrc[3], A8U* const pDst[3], FwiSize imgSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV420_8u_C3P3R           )( const A8U* pSrc, int srcStep, A8U* const pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToYUV420_8u_C3P3            )( const A8U* pSrc, A8U* const pDst[3], FwiSize imgSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB_8u_P3R             )(const A8U* const pSrc[3], int srcStep[3], A8U* const pDst[3], int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB_8u_P3              )(const A8U* const pSrc[3], A8U* const pDst[3], FwiSize imgSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB_8u_P3C3R           )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB_8u_P3C3            )(const A8U* const pSrc[3], A8U* pDst, FwiSize imgSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB_8u_P3AC4R          )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToBGR_8u_P3C3R           )(const A8U* const pSrc[3], int srcStep[3], A8U* pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB565_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB555_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToRGB444_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToBGR565_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToBGR555_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiYUV420ToBGR444_8u16u_P3C3R     )(const A8U* const pSrc[3], int srcStep[3], A16U* pDst, int dstStep, FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422_8u_C2P3R              )(const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize );
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422_8u_P3C2R              )(const Fw8u* const pSrc[3], int srcStep[3], Fw8u* pDst, int dstStep, FwiSize roiSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiCbYCr422ToYCbCr420_8u_C2P3R    )( const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize );

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_8u_C1R                     )( const Fw8u* pSrc, int srcStep, FwiSize roiSize, Fw64f* pSum );
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_16s_C1R                    )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f* pSum);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_8u_C3R                     )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_16s_C3R                    )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_8u_AC4R                    )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_16s_AC4R                   )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_8u_C4R                     )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_16s_C4R                    )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[4]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_32f_C3R                    )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3], FwHintAlgorithm hint);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_32f_AC4R                   )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f sum[3], FwHintAlgorithm hint);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiSum_32f_C4R                    )(const Fw32f* pSrc, int srcStep, FwiSize roiSize, Fw64f sum[4], FwHintAlgorithm hint);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_8u_C1R                    )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMin);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_16s_C1R                   )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMin);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_32f_C1R                   )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMin);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_8u_AC4R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_16s_AC4R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_32f_AC4R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_8u_C4R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_16s_C4R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_32f_C4R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[4]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_8u_C3R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_16s_C3R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMin_32f_C3R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3]);


OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_8u_C1R                    )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMax);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_16s_C1R                   )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMax);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_32f_C1R                   )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMax);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_8u_AC4R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_16s_AC4R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_32f_AC4R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[3]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_8u_C4R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_16s_C4R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_32f_C4R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[4]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_8u_C3R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_16s_C3R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMax_32f_C3R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f max[3]);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_8u_C1R                )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u* pMin,Fw8u* pMax);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_16s_C1R               )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s* pMin, Fw16s* pMax);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_32f_C1R               )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f* pMin, Fw32f* pMax);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_8u_AC4R               )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3], Fw8u max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_16s_AC4R              )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3], Fw16s max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_32f_AC4R              )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3], Fw32f max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_8u_C4R                )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[4], Fw8u max[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_16s_C4R               )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[4], Fw16s max[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_32f_C4R               )(const Fw32f* pSrc, int srcStep,FwiSize roiSize,Fw32f min[4],Fw32f max[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_8u_C3R                )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw8u min[3], Fw8u max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_16s_C3R               )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw16s min[3], Fw16s max[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMinMax_32f_C3R               )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw32f min[3], Fw32f max[3]);


OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_8u_C1R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_16s_C1R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_32f_C1R                  )(const Fw32f* pSrc, int srcStep,FwiSize roiSize, Fw64f* pMean, FwHintAlgorithm hint);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_8u_C3R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_16s_C3R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_8u_AC4R                  )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_16s_AC4R                 )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_8u_C4R                   )(const Fw8u* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[4]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX,  fwiMean_16s_C4R                  )(const Fw16s* pSrc, int srcStep,FwiSize roiSize, Fw64f mean[4]);

OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToYCbCr420_8u_P3R      )( const Fw8u* pSrc[3], int srcStep[3], Fw8u* pDst[3], int dstStep[3], FwiSize roiSize);
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiYCbCr422ToYCbCr420_8u_C2P3R    )(const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_8u_C3C1R           )(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_16u_C3C1R          )(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_16s_C3C1R          )(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_32f_C3C1R          )(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_8u_AC4C1R          )(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_16u_AC4C1R         )(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_16s_AC4C1R         )(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiColorToGray_32f_AC4C1R         )(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3]);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_8u_C3C1R             )(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_16u_C3C1R            )(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_16s_C3C1R            )(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_32f_C3C1R            )(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_8u_AC4C1R            )(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_16u_AC4C1R           )(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_16s_AC4C1R           )(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiRGBToGray_32f_AC4C1R           )(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize);

OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiLookUp3DSpecInitAlloc          )(const Fw16u cubeDimensions, const Fw16u cubeMax, FwiLUTSpec** ppLUTSpec);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiLookUp3D_16u_C3R               )(const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec);
OREFR OSSE2         FwStatus PREFIX_OPT(OPT_PREFIX, fwiLookUp3D_16u_C3IR              )(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec);
OREFR               FwStatus PREFIX_OPT(OPT_PREFIX, fwiLookUp3DSpecFree               )(FwiLUTSpec* pLUTSpec);


#endif	// __FWIMAGE_OPT_H__

