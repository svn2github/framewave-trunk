/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "Morphology.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_MorphologyInit(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	if (pState==0 || pMask==0) return fwStsNullPtrErr;

	if (maskSize.height <= 0 || maskSize.width <= 0 || roiSize.width < 1 || roiSize.height < 1) return fwStsSizeErr;

	if (anchor.x < 0 || anchor.y < 0 || anchor.x >= maskSize.width || anchor.y >= maskSize.height) return fwStsAnchorErr;

	//obtain the size of pState
	//int pSize;

	//if(!fwiMorphologyGetSize_8u_C1R(roiWidth, pMask, maskSize, &pSize))
	//	pState = (FwiMorphAdvState *)fwMalloc(pSize);

	if(pState == 0) return fwStsMemAllocErr;

	//pState->pMask = (Fw8u *) malloc(maskSize.width * maskSize.height);
	pState->pMask = (Fw8u *) fwMalloc(maskSize.width * maskSize.height);

	//pState Initialization
	int x = 0, y = 0;

	//Rectangularity checking
	pState->isRectangule = true;
	for(x = 0; x < maskSize.width * maskSize.height; x++)
	{
		if(*(pMask+x) == 0)
		{
			pState->isRectangule = false;
			break;
		}
	}

	//Symmetry checking
	pState->isSymmetric = true;
	for(y = 0; y < maskSize.height; y++)
	{
		for(x = 0; x < maskSize.width; x++)
		{
			if(!pMask[x+y*maskSize.width] == pMask[(maskSize.width - x - 1) + (maskSize.height - y - 1) * maskSize.width])
			{
				//if(any unsymmetric happens, just make pState->isSymmetric false and go out of the loop.
				pState->isSymmetric = false;
				x = maskSize.width;
				y = maskSize.height;
			}
		}
	}

	pState->maskSize = maskSize;
	pState->anchor = anchor;
	pState->roiSize = roiSize;

	//When initialized the pState->pMask, I do the "refelction operation" here
	for(y = 0; y < maskSize.height; y++)
	{
		for(x = 0; x < maskSize.width; x++)
		{
			pState->pMask[(-y+maskSize.height-1)*maskSize.width + (-x + maskSize.width-1)] = *(pMask+y*maskSize.width+x);
		}
	}

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C1R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInit<Fw8u, C1, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C3R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{	
	return My_FW_MorphologyInit<Fw8u, C3, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_8u_C4R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInit<Fw8u, C4, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C1R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInit<Fw32f, C1, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C3R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInit<Fw32f, C3, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyInit_32f_C4R)(FwiMorphAdvState *pState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInit<Fw32f, C4, DT_REFR> (pState, roiSize, pMask, maskSize, anchor);
}
#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
