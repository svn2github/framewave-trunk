/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "H264-Deblock.h"
#include "fwVideo.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_VerEdge_H264_8u_C1IR)( Fw8u *pSrcDst, Fw32s  srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
{
  return filterDeblockingLuma_VerEdge_H264_8u_C1IR( pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_HorEdge_H264_8u_C1IR)( Fw8u *pSrcDst, Fw32s srcDstStep,Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
{
  return filterDeblockingLuma_HorEdge_H264_8u_C1IR(pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_HorEdge_H264_8u_C1IR)( Fw8u *pSrcDst, Fw32s srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
{
  return filterDeblockingChroma_HorEdge_H264_8u_C1IR( pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_VerEdge_H264_8u_C1IR)( Fw8u *pSrcDst, Fw32s srcDstStep, Fw8u *pAlpha, Fw8u *pBeta, Fw8u *pThresholds, Fw8u *pBS)
{
  return filterDeblockingChroma_VerEdge_H264_8u_C1IR( pSrcDst, srcDstStep, pAlpha, pBeta, pThresholds, pBS);
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
