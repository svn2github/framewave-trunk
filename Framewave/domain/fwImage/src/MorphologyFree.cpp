/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "Morphology.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 9999 )

//MorphologyInit
//Initializes morphology state structure for erosion or dilation operation.

//template< class TS, CH chSrc, DispatchType disp >
FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyFree)(FwiMorphState *pState) 
{
	if (pState==0) return fwStsNullPtrErr;

	fwFree(pState->pMask);

	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphologyFree)(FwiMorphState **ppState) 
{
	if (*ppState==0) return fwStsNullPtrErr;

	free(*ppState);

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
