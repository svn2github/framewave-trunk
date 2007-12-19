/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

//************************************************************************* 
//This file include the level shift functions from JPEG Chapter
//	fwiSplit422LS_MCU_8u16s_C2P3R
//  fwiAdd128_JPEG_16s8u_C1R
//************************************************************************* 

#include "fwdev.h"
#include "fwJPEG.h"
//#include "FwSharedCode_SSE2.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST( 102 )

//-----------------------------------------------------------------------
//This function splits the Standard 422 interleaved data to 422 MCU data. It
//also converts pixel ordered data to planar data with level shift (by 
//subtracting 128). Input data is the format of (YCbYCrYCbYCr...). Output data
//is 2 8*8 block of Y, 1 8*8 block of Cb and 1 block for Cr channels.
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiSplit422LS_MCU_8u16s_C2P3R)(const Fw8u *pSrc, int srcStep,
							Fw16s *pDstMCU[3])
{
	if (pSrc == 0 || pDstMCU==0) return fwStsNullPtrErr;	
	if (pDstMCU[0]==0 || pDstMCU[1]==0 || pDstMCU[2]==0)
			return fwStsNullPtrErr;
	if (srcStep <=0) return fwStsStepErr;

	int x, y, index;
	const Fw8u *pSrcPtr;

	for (y=0;y<8;y++) {
		pSrcPtr = pSrc+y*srcStep;
		for (x=0;x<8;x++) {
			index = x+y*8;
			pDstMCU[0][index]=(*pSrcPtr++)-128;
			index = (x>>1) + y*8;
			if ((x&1)==0) {	
				pDstMCU[1][index]=(*pSrcPtr++)-128;
			} else {
				pDstMCU[2][index]=(*pSrcPtr++)-128;
			}
		}
		for (x=0;x<8;x++) {
			index = 64+x+y*8;
			pDstMCU[0][index]=(*pSrcPtr++)-128;
			index = 4 + (x>>1) + y*8;
			if ((x&1)==0) {	
				pDstMCU[1][index]=(*pSrcPtr++)-128;
			} else {
				pDstMCU[2][index]=(*pSrcPtr++)-128;
			}
		}
	}

	return fwStsNoErr;
}

//-----------------------------------------------------------------------
//This function creates the Standard 422 interleaved data from 422 MCU data. 
//It also converts planar data to pixel ordered data with level shift (by adding
//128). Input data is MCU data containing 2 8*8 block of Y, 1 8*8 block of Cb and 
//1 block for Cr channels.Output data is the format of (YCbYCrYCbYCr...)
//-----------------------------------------------------------------------
FwStatus PREFIX_OPT(OPT_PREFIX, fwiJoin422LS_MCU_16s8u_P3C2R)(const Fw16s *pSrcMCU[3], 
							Fw8u*pDst, int dstStep)
{
	if (pDst == 0 || pSrcMCU==0) return fwStsNullPtrErr;	
	if (pSrcMCU[0]==0 || pSrcMCU[1]==0 || pSrcMCU[2]==0)
			return fwStsNullPtrErr;
	if (dstStep <=0) return fwStsStepErr;

	int x, y, index;
	Fw8u *pDstPtr;

	for (y=0;y<16;y++) {
		pDstPtr = pDst+y*dstStep;
		for (x=0;x<8;x++) {
			index = x+y*8;
			(*pDstPtr++)=(Fw8u)(pSrcMCU[0][index]+128);
			index >>= 1;
			if ((x&1)==0) {	
				(*pDstPtr++)=(Fw8u)(pSrcMCU[1][index]+128);
			} else {
				(*pDstPtr++)=(Fw8u)(pSrcMCU[2][index]+128);
			}
		}
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR 
