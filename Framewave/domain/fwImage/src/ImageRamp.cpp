/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ImageRamp.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )

ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8u_C1R)(A8U *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A8U,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8u_C3R)(A8U *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A8U,C3> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8s_C1R)(A8S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A8S,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_8s_C3R)(A8S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A8S,C3> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16u_C1R)(A16U *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A16U,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16u_C3R)(A16U *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A16U,C3> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16s_C1R)(A16S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A16S,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_16s_C3R)(A16S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A16S,C3> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32s_C1R)(A32S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A32S,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32s_C3R)(A32S *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A32S,C3> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32f_C1R)(A32F *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A32F,C1> (pDst, dStep, roiSize, offset, slope, axis); }
ASTAT PREFIX_OPT(OPT_PREFIX, fwiImageRamp_32f_C3R)(A32F *pDst, int dStep, ASZ roiSize, float offset, float slope, FwiAxis axis){ return Ramp<A32F,C3> (pDst, dStep, roiSize, offset, slope, axis); }

#endif


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
