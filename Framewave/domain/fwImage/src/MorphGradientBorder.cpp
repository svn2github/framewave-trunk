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

//Morphology Gradient Operation:
//A grad B = (A dilate B) - (A erode B)

template< class TS, CH chSrc, DispatchType disp >
FwStatus My_FW_MorphGradientBorder(const TS *pSrc, int srcStep, TS *pDst,
									 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState)
{
	if (pSrc == 0 || pDst == 0 || pState == 0) return fwStsNullPtrErr;
	if (roiSize.height < 1 || roiSize.width < 1 || roiSize.width > pState->roiSize.width) return fwStsSizeErr;

	int x = 0, y = 0, k = 0, channel = ChannelCount(chSrc);

	//dstStep and srcStep are byte size
	//we need to change it with data array size
	//dstStep = dstStep / (sizeof(TS));
	//srcStep = srcStep / (sizeof(TS));

	if(srcStep < pState->roiSize.width || dstStep < pState->roiSize.width) return fwStsStepErr;
	if(srcStep %4 != 0 || dstStep %4 != 0) return fwStsNotEvenStepErr;

	border = fwBorderRepl;
	if(border != fwBorderRepl) return fwStsBadArgErr;

	//check if all pMask elements are zero or not.
	//Check if all mask values are equal to zero.
	
	//for(x = 0; x < pState->maskSize.width * pState->maskSize.height; x++)
	//if(*(pState->pMask+x) != 0)
	////Whenever found a nonzero value, set the x out of bound. So they can go out of the loop.
	//x = pState->maskSize.width * pState->maskSize.height + 1;

	//if(x != pState->maskSize.width * pState->maskSize.height + 1)
	//return fwStsZeroMaskValuesErr;
	
	//(A dilate B)
	//The temporary buffer for dilation
	TS *pDst_T_Dilate = (TS*) fwMalloc(sizeof(TS) * (roiSize.width * roiSize.height * channel));

	TS *pSrcAddr, *pDstAddr, *pDstAddr2;

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
					pDstAddr = (TS*)((Fw8u*)pDst_T_Dilate+y*roiSize.width*channel+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pDstAddr = (TS*)((Fw8u*)pDst_T_Dilate+y*roiSize.width*channel+x*channel+k);

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
								pDstAddr = (TS*)((Fw8u*)pDst_T_Dilate+y*roiSize.width*channel+x*channel+k);

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

	//(A erode B)
	//The temporary buffer for erosion
	TS *pDst_T_Erode = (TS*) fwMalloc(sizeof(TS) * (roiSize.width * roiSize.height * channel));

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
					pDstAddr = (TS*)((Fw8u*)pDst_T_Erode+y*roiSize.width*channel+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pDstAddr = (TS*)((Fw8u*)pDst_T_Erode+y*roiSize.width*channel+x*channel+k);

					*pDstAddr = 255;

					for(int n = 0; n < pState->maskSize.height; n++)
					{
						for(int m = 0; m < pState->maskSize.width; m++)
						{
							if((y+n) < 0 || (x+m) < 0 || (y+n) >= roiSize.height || (x+m) >= roiSize.width)
								continue;

							if((pState->pMask[(n+pState->anchor.y)*(pState->maskSize.width) + (m+pState->anchor.x)]) > 0)
							{
								pSrcAddr = (TS*)((Fw8u*)pSrc+(y+n)*srcStep+(x+m)*channel+k);
								pDstAddr = (TS*)((Fw8u*)pDst_T_Erode+y*roiSize.width*channel+x*channel+k);

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

	//A grad B = (A dilate B) - (A erode B)
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
					pDstAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);

					*pDstAddr = *pSrcAddr;
				}
				else
				{
					pSrcAddr = (TS*)((Fw8u*)pDst+y*dstStep+x*channel+k);
					pDstAddr = (TS*)((Fw8u*)pDst_T_Dilate+y*roiSize.width*channel+x*channel+k);
					pDstAddr2 = (TS*)((Fw8u*)pDst_T_Erode+y*roiSize.width*channel+x*channel+k);

					//*pSrcAddr = *pDstAddr - *pDstAddr2;
					*pSrcAddr = FW_REF::Limits<TS>::Sat(*pDstAddr - *pDstAddr2);
				}
			}
		}
	}

	fwFree(pDst_T_Dilate);
	fwFree(pDst_T_Erode);
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C1R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
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
			//fwiDilate_8u_C1R(pSrc, srcStep, pDst_T, dstStep, roiSize, pState->pMask, pState_T->pMask, pState_T->roiSize);
			//fwiErode_8u_C1R(pSrc, srcStep, pDst, dstStep, roiSize, pState_T->pMask, pState_T->pMask, pState_T->roiSize);
			//fwiSub_8u_C1IRSfs(pDst_T, dstStep, pDst, dstStep, pState_T->roiSize, 1);
			fwFree(pDst_T);

			return fwStsNoErr;
		}
	default:
		return My_FW_MorphGradientBorder<Fw8u, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C3R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
																 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	return My_FW_MorphGradientBorder<Fw8u, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_8u_C4R)(const Fw8u *pSrc, int srcStep, Fw8u *pDst, 
																 int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	return My_FW_MorphGradientBorder<Fw8u, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C1R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	return My_FW_MorphGradientBorder<Fw32f, C1, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C3R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	return My_FW_MorphGradientBorder<Fw32f, C3, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiMorphGradientBorder_32f_C4R)(const Fw32f *pSrc, int srcStep, Fw32f *pDst, 
																  int dstStep, FwiSize roiSize, FwiBorderType border, FwiMorphAdvState *pState) 
{
	return My_FW_MorphGradientBorder<Fw32f, C4, DT_REFR> (pSrc, srcStep, pDst, dstStep, roiSize, border, pState);
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
