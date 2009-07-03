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


//Morphology Blackhat Operation:
//A blackhat B = (A close B) - A

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_MorphBlackhatBorder(const TS *pSrc, int srcStep, TS *pDst,
									 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState)
{
	if (pSrc == 0 || pDst == 0 || pState == 0) return fwStsNullPtrErr;
	if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiSize.width) return fwStsSizeErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);

	if(srcStep < pState->roiSize.width || dstStep < pState->roiSize.width) return fwStsStepErr;
	if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;

	border = fwBorderRepl;
	if(border != fwBorderRepl) return fwStsBadArgErr;

	//Here, check if all pMask elements are zero or not.
	//Check if all mask values are equal to zero.

	//for(x = 0; x < pState->maskSize.width * pState->maskSize.height; x++)
	//	if(*(pState->pMask+x) != 0)
	//		//Whenever found a nonzero value, set the x out of bound. So they can go out of the loop.
	//		x = pState->maskSize.width * pState->maskSize.height + 1;

	//if(x != pState->maskSize.width * pState->maskSize.height + 1)
	//	return fwStsZeroMaskValuesErr;


	//The temporary buffer
	TS *pDst_T = (TS*) fwMalloc(sizeof(TS) * (roiSize.width * roiSize.height * channel));
	TS *pSrcAddr;
	TS *pDstAddr;

	//(A dilate B)
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					pSrcAddr = (TS*)((Fw8u*)pSrc+y*srcStep+x*channel+k);
					pDstAddr = (TS*)((Fw8u*)pDst_T+y*roiSize.width*channel+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pDstAddr = (TS*)((Fw8u*)pDst_T+y*roiSize.width*channel+x*channel+k);
					*pDstAddr = 0;

					for(int n = 0; n < pState->maskSize.height; n++)
					{
						for(int m = 0; m < pState->maskSize.width; m++)
						{
							if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
								continue;

							if((pState->pMask[(n+pState->anchor.y)*(pState->maskSize.width) + (m+pState->anchor.x)]) > 0)
							{
								pSrcAddr = (TS*)((Fw8u*)pSrc+(y+n)*srcStep+(x+m)*channel+k);
								pDstAddr = (TS*)((Fw8u*)pDst_T+y*roiSize.width*channel+x*channel+k);

								if(*pSrcAddr > *pDstAddr)
								{
									*pDstAddr = *pSrcAddr;
								}
							}
						}
					}
				}
			}
		}
	}

	//(A dilate B) erode B
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					pSrcAddr = (TS*)((Fw8u*)pDst_T+y*roiSize.width*channel+x*channel+k);
					pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*pDstAddr = 255;

					for(int n = 0; n < pState->maskSize.height; n++)
					{
						for(int m = 0; m < pState->maskSize.width; m++)
						{
							//if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
							//	continue;

							if((pState->pMask[(n+pState->anchor.y)*(pState->maskSize.width) + (m+pState->anchor.x)]) > 0)
							{
								pSrcAddr = (TS*)((Fw8u*)pDst_T+(y+n)*roiSize.width*channel+(x+m)*channel+k);
								pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

								if(*pSrcAddr < *pDstAddr)
								{
									*pDstAddr = *pSrcAddr;
								}
							}
						}
					}
				}
			}
		}
	}

	//In above operations, we obtain the (A close B)
	//To obtain blackhat:A blackhat B = (A close B) - A
	for(y = 0; y < roiSize.height; y++)
	{
		for( x = 0; x < roiSize.width; x++)
		{
			for (k=0; k<channel; k++)
			{
				//In the four-channel imamge, the alpha channel is not processed.
				if(k == 3)
				{
					pSrcAddr = (TS*)((Fw8u*)pDst_T+y*roiSize.width*channel+x*channel+k);
					pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pSrcAddr = (TS*)((Fw8u*)pSrc+y*srcStep+x*channel+k);
					pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					//*pDstAddr = *pSrcAddr - *pDstAddr;
					*pDstAddr = FW_REF::Limits<TS>::Sat(*pDstAddr - *pSrcAddr);
				}
			}
		}
	}

	fwFree(pDst_T);
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C1R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
																 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw8u *pDst_T = (Fw8u*) fwMalloc(sizeof(Fw8u) * (dstStep * roiSize.height * 1));
			fwiMorphCloseBorder_8u_C1R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_8u_C1RSfs(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize, 1);
			fwFree(pDst_T);
			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw8u, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C3R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
																 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw8u *pDst_T = (Fw8u*) fwMalloc(sizeof(Fw8u) * (dstStep * roiSize.height * 3));
			fwiMorphCloseBorder_8u_C3R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_8u_C3RSfs(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize, 1);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw8u, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_8u_C4R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
																 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw8u *pDst_T = (Fw8u*) fwMalloc(sizeof(Fw8u) * (dstStep * roiSize.height * 4));
			fwiMorphCloseBorder_8u_C4R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_8u_C4RSfs(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize, 1);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw8u, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C1R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw32f *pDst_T = (Fw32f*) fwMalloc(sizeof(Fw32f) * (dstStep * roiSize.height * 1));
			fwiMorphCloseBorder_32f_C1R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_32f_C1R(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw32f, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C3R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw32f *pDst_T = (Fw32f*) fwMalloc(sizeof(Fw32f) * (dstStep * roiSize.height * 3));
			fwiMorphCloseBorder_32f_C3R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_32f_C3R(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw32f, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphBlackhatBorder_32f_C4R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	switch( Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
		{
			FwiMorphAdvState *pState_T = (FwiMorphAdvState*)fwMalloc(sizeof(FwiMorphAdvState));
			pState_T->anchor = pState->anchor;
			pState_T->isRectangule = pState->isRectangule;
			pState_T->isSymmetric = pState->isSymmetric;
			pState_T->maskSize = pState->maskSize;
			pState_T->pMask = pState->pMask;
			pState_T->roiSize = pState->roiSize;

			Fw32f *pDst_T = (Fw32f*) fwMalloc(sizeof(Fw32f) * (dstStep * roiSize.height * 4));
			fwiMorphCloseBorder_32f_C4R(pSrc, srcStep, pDst_T, dstStep, roiSize,border, pState_T);
			fwiSub_32f_C4R(pDst_T, dstStep, pSrc, srcStep, pDst, dstStep, roiSize);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphBlackhatBorder<Fw32f, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
