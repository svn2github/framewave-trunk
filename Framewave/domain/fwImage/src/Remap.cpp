/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "system.h"
#include "FwSharedCode_SSE2.h"
#include "PointHandle.h"


#if BUILD_NUM_AT_LEAST( 100 )

#pragma warning( disable: 4244 )

namespace OPT_LEVEL
{

//Remap functions provide an arbitrary mapping from source image to destination image
//User will have to provide the look-up coordinate mapping table for this function.
//The formula for the mapping is as follows.
//		dst_pixel[x, y] = src_pixel[pxMap[x,y], pyMap[x,y]]
//where pxMap table contains the x coordinates of the source image pixel src_pixel
//that should be mapping to destination pixels
//pyMap table contains the y coordinates of the source image pixel src_pixel
//that will be mapping to destination pixel

//Reference internal function for remap
template< class TS, CH chSrc, DispatchType disp >
static FwStatus My_FW_Remap(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
					  const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep,
					  TS* pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	//Parameter checking
    if (pxMap==0 || pyMap==0) 
		return fwStsNullPtrErr;

	if (interpolation != FWI_INTER_LINEAR && interpolation != FWI_INTER_NN &&
		interpolation != FWI_INTER_CUBIC)	
		return fwStsInterpolationErr;
	
	int channel=ChannelCount(chSrc);
	FwStatus status = My_FW_ParaCheck<TS>(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep,
		dstRoiSize, channel);
	if (status !=fwStsNoErr) return status;

	int x, y, flag=0;
	double xmap, ymap;
	
	//dstStep and srcStep are byte size
	//we need to change it with data array size
	dstStep = dstStep / (sizeof(TS));
	srcStep = srcStep / (sizeof(TS));
	xMapStep = xMapStep / (sizeof(Fw32f));
	yMapStep = yMapStep / (sizeof(Fw32f));

	int channel1;
	// Will not change 4th channel element in AC4
	if (chSrc == AC4) channel1=3;
	else channel1=channel;
	Fw32f round;
	// 32f is supported, but not 32u and 32s
	// No rounding is needed for 32f type
	if (sizeof(TS) == 4) round=0;
	else round=0.5;

	for (y=0; y<dstRoiSize.height; y++) {
		for (x=0; x<dstRoiSize.width; x++) {
			// potential variation
			// xmap=*(pxMap+x+y*xMapStep)+0.5;
			// ymap=*(pyMap+x+y*yMapStep)+0.5;
			xmap=*(pxMap+x+y*xMapStep);
			ymap=*(pyMap+x+y*yMapStep);
			
			// Remap function can map source point to any destination point. 
			// only general reference code is provided.
			My_FW_PointHandle<TS, disp> (xmap, ymap, x, y, pSrc, srcStep, srcRoi, 
				pDst, dstStep, interpolation, &flag, channel, channel1, round);			
		}
	}

	return fwStsNoErr;
}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

// 8u data type with 1 channel 
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C1R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw8u *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw8u, C1, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C3R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw8u *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_C4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw8u *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_AC4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw8u *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C1R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw32f, C1, DT_REFR>(pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C3R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_C4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_AC4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
						   const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
						   Fw32f *pDst, int dstStep, FwiSize dstRoiSize, int interpolation)
{
	return My_FW_Remap<Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
		pDst, dstStep, dstRoiSize, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
	Fw8u *const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status=My_FW_Remap<Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
			pDst[i], dstStep, dstRoiSize, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
	Fw8u *const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status=My_FW_Remap<Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
			pDst[i], dstStep, dstRoiSize, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
	Fw32f *const pDst[3], int dstStep, FwiSize dstRoiSize, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status=My_FW_Remap<Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
			pDst[i], dstStep, dstRoiSize, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiRemap_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	const Fw32f *pxMap, int xMapStep, const Fw32f *pyMap, int yMapStep, 
	Fw32f *const pDst[4], int dstStep, FwiSize dstRoiSize, int interpolation)
{
	FwStatus status;

	for (int i=0;i<4;i++) {
		status=My_FW_Remap<Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, pxMap, xMapStep, pyMap, yMapStep,
			pDst[i], dstStep, dstRoiSize, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

#endif //BUILD_NUM_AT_LEAST


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
