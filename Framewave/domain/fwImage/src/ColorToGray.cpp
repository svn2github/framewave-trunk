/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ColorToGray.h"
#include "fwImage.h"

using namespace OPT_LEVEL;


FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_8u_C3C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_C3C1<Fw8u,C3,Fw8u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw8u,C3,Fw8u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_16u_C3C1R)(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_C3C1<Fw16u,C3,Fw16u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw16u,C3,Fw16u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_16s_C3C1R)(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_C3C1<Fw16s,C3,Fw16s,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw16s,C3,Fw16s,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_32f_C3C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_C3C1<Fw32f,C3,Fw32f,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw32f,C3,Fw32f,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_8u_AC4C1R)(const Fw8u* pSrc, int srcStep, Fw8u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_AC4C1<Fw8u,AC4,Fw8u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw8u,AC4,Fw8u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_16u_AC4C1R)(const Fw16u* pSrc, int srcStep, Fw16u* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_AC4C1<Fw16u,AC4,Fw16u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw16u,AC4,Fw16u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_16s_AC4C1R)(const Fw16s* pSrc, int srcStep, Fw16s* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_AC4C1<Fw16s,AC4,Fw16s,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw16s,AC4,Fw16s,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiColorToGray_32f_AC4C1R)(const Fw32f* pSrc, int srcStep, Fw32f* pDst, int dstStep, FwiSize roiSize, const Fw32f coeffs[3])
{
    ColorToGray_AC4C1<Fw32f,AC4,Fw32f,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw32f,AC4,Fw32f,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_8u_C3C1R)(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_C3C1<Fw8u,C3,Fw8u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw8u,C3,Fw8u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_16u_C3C1R)(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_C3C1<Fw16u,C3,Fw16u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw16u,C3,Fw16u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX,fwiRGBToGray_16s_C3C1R)(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_C3C1<Fw16s,C3,Fw16s,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw16s,C3,Fw16s,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_32f_C3C1R)(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_C3C1<Fw32f,C3,Fw32f,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_C3C1<Fw32f,C3,Fw32f,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_8u_AC4C1R)(const Fw8u* pSrc, int srcStep,Fw8u* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_AC4C1<Fw8u,AC4,Fw8u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw8u,AC4,Fw8u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_16u_AC4C1R)(const Fw16u* pSrc, int srcStep,Fw16u* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_AC4C1<Fw16u,AC4,Fw16u,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw16u,AC4,Fw16u,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_16s_AC4C1R)(const Fw16s* pSrc, int srcStep,Fw16s* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_AC4C1<Fw16s,AC4,Fw16s,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw16s,AC4,Fw16s,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToGray_32f_AC4C1R)(const Fw32f* pSrc, int srcStep,Fw32f* pDst, int dstStep, FwiSize roiSize)
{
    Fw32f coeffs[3] = {0.299F,0.587F,0.114F};
    ColorToGray_AC4C1<Fw32f,AC4,Fw32f,C1> data(coeffs);
    return OPT_LEVEL::fe< ColorToGray_AC4C1<Fw32f,AC4,Fw32f,C1> >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
