/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//-----------------------------------------------------------------------
//This file includes the Zigzag functions from Data Exchange and 
//Initialization Chapter
//	fwiZigzagFwd8x8_16s_C1
//  fwiZigzagInv8x8_16s_C1
//-----------------------------------------------------------------------

#include "fwdev.h"
#include "fwImage.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

//forward order for ZigZag (from conventional order)
const Fw8u ZigZagFwdOrder[64] =
{
	 0,  1,  8, 16,  9,  2,  3, 10, 
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34, 
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36, 
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46, 
	53, 60, 61, 54, 47, 55, 62, 63
};

//Inverse order for ZigZag (from ZigZaged order)
const Fw8u ZigZagInvOrder[64] =
{	
	 0,  1,  5,  6, 14, 15, 27, 28,
	 2,  4,  7, 13, 16, 26, 29, 42,
	 3,  8, 12, 17, 25, 30, 41, 43,
	 9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63
};

//-----------------------------------------------------------------------
//This functions converts the Conventional ordered Source to ZigZag order
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiZigzagFwd8x8_16s_C1)(const Fw16s* pSrc, Fw16s* pDst)
{
    if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	//Only one 8x8 block will be handled
	for (int i=0;i<64;i++) {
		pDst[i] = pSrc[ZigZagFwdOrder[i]];
	}
	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This functions inverses the ZigZag ordered Source to Conventional order
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiZigzagInv8x8_16s_C1)(const Fw16s* pSrc, Fw16s* pDst)
{
    if (pSrc == 0 || pDst == 0) return fwStsNullPtrErr;

	//Only one 8x8 block will be handled
	for (int i=0;i<64;i++) {
		pDst[i] = pSrc[ZigZagInvOrder[i]];
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR

