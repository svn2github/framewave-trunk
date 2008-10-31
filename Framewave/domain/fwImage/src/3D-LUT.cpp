/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "3D-LUT.h"

using namespace OPT_LEVEL;

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLookUp3DSpecInitAlloc)(const Fw16u cubeDimensions, const Fw16u cubeMax, FwiLUTSpec** ppLUTSpec)
    {
    return LookUp3DSpecInitAlloc(cubeDimensions, cubeMax, ppLUTSpec);
    }

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLookUp3D_16u_C3R)(const Fw16u *pSrc, int srcStep, Fw16u *pDst, int dstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec)
    {
    return LookUp3D_16u_C3R(pSrc, srcStep, pDst, dstStep, roiSize, pCube, pLUTSpec);
    }

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLookUp3D_16u_C3IR)(Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16u *pCube, FwiLUTSpec* pLUTSpec)
    {
    return LookUp3D_16u_C3IR(pSrcDst, srcDstStep, roiSize, pCube, pLUTSpec);
    }

ASTAT PREFIX_OPT(OPT_PREFIX, fwiLookUp3DSpecFree)(FwiLUTSpec* pLUTSpec)
    {
    return LookUp3DSpecFree(pLUTSpec);
    }

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 

