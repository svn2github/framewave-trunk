/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ColorFormatConversion.h"
#include "fwImage.h"

using namespace OPT_LEVEL;


FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToYCbCr420_8u_P3R)( const Fw8u* pSrc[3], int srcStep[3], Fw8u* pDst[3], int dstStep[3], FwiSize roiSize)
{
    return ConvertYcbcr422toYcbcr420_8uP3(pSrc,srcStep,pDst,dstStep,roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX,fwiYCbCr422ToYCbCr420_8u_C2P3R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize)
{
    return ConvertYcbcr422toYcbcr420_8uC2P3(pSrc,srcStep,pDst,dstStep,roiSize);
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
