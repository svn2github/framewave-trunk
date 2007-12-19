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

//MorphologyInitAlloc
//Initializes morphology state structure for erosion or dilation operation.

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_MorphologyInitAlloc(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	if (ppState==0 || (*ppState)==0 || pMask==0) return fwStsNullPtrErr;

	if (maskSize.height <= 0 || maskSize.width <= 0 || roiSize.width < 1) return fwStsSizeErr;

	if (anchor.x < 0 || anchor.y < 0 || anchor.x >= maskSize.width || anchor.y >= maskSize.height) return fwStsAnchorErr;

	//Memory Allocation
	int stateSize = sizeof(Fw8u*)* maskSize.width*maskSize.height + sizeof(FwiSize) + sizeof(FwiPoint) + sizeof(bool) + sizeof(bool);
	ppState = (FwiMorphAdvState **)fwMalloc(stateSize);
	if(ppState == 0) return fwStsMemAllocErr;

	//ppState Initialization
	int x = 0, y = 0;

	//Rectangularity checking
	(*ppState)->isRectangule = true;
	for(x = 0; x < maskSize.width * maskSize.height; x++)
		if(*(pMask+x) == 0)
		{
			(*ppState)->isRectangule = false;
			break;
		}

		//Symmetry checking
		(*ppState)->isSymmetric = true;
		for(y = 0; y < maskSize.height; y++)
			for(x = 0; x < maskSize.width; x++)
				if(!pMask[x+y*maskSize.width] == pMask[(maskSize.width - x ) + (maskSize.height - y) * maskSize.width])
				{
					//if(any unsymmetric happens, just make ppState->isSymmetric false and go out of the loop.
					(*ppState)->isSymmetric = false;
					x = maskSize.width;
					y = maskSize.height;
				}

				(*ppState)->maskSize = maskSize;
				(*ppState)->anchor = anchor;
				(*ppState)->roiSize = roiSize;

				//When initialized the pState->pMask, I do the "refelction operation" here
				for(y = 0; y < maskSize.height; y++)
					for(x = 0; x < maskSize.width; x++)
						(*ppState)->pMask[(-x + maskSize.width-anchor.x)+(-y+maskSize.height-anchor.y)*maskSize.width] = pMask[x+y*maskSize.width];

				return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C1R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw8u, C1, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C3R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw8u, C3, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_8u_C4R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw8u, C4, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C1R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw32f, C1, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C3R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw32f, C3, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphAdvInitAlloc_32f_C4R)(FwiMorphAdvState **ppState, FwiSize roiSize, const Fw8u *pMask, FwiSize maskSize, FwiPoint anchor) 
{
	return My_FW_MorphologyInitAlloc<Fw32f, C4, DT_REFR> (ppState, roiSize, pMask, maskSize, anchor);
}

#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
