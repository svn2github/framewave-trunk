/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

#if BUILD_NUM_AT_LEAST( 100 )

namespace OPT_LEVEL
{
	//internal function for WarpAffine transformation
	template< class TS, CH chSrc, DispatchType disp >
	extern FwStatus My_FW_WarpAffine(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi,
		TS* pDst, int dstStep, FwiRect dstRoi, 
		const double coeffs[2][3], int interpolation);

	//Description
	//The function fwiShear is declared in the fwi.h file. The shearing transform, performed by
	//this function, maps the source image pixel coordinates (x,y) according to the following formulas:
	//		x? = x + xShear * y + xShift
	//		y? = yShear * x + y + yShift
	//where x? and y? denote the pixel coordinates in the sheared image. The shearing transform is a
	//special case of an affine transform, performed by the fwiWarpAffine function.
	//The transformed part of the image is resampled using the interpolation method specified by the
	//interpolation parameter, and written to the destination image ROI.

	template< class TS, CH chSrc, DispatchType disp >
	FwStatus My_FW_Shear(const TS* pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
		TS* pDst, int dstStep, FwiRect dstRoi, double xShear, 
		double yShear, double xShift, double yShift, int interpolation)
	{
		double coeffs[2][3];

		// [ 1        xShear ]
		// [ yShear   1      ]
		coeffs[0][0] = 1;
		coeffs[0][1] = xShear;
		coeffs[0][2] = xShift;
		coeffs[1][0] = yShear;
		coeffs[1][1] = 1;
		coeffs[1][2] = yShift;

		return My_FW_WarpAffine <TS, chSrc, disp> (pSrc, srcSize, srcStep, srcRoi, 
			pDst, dstStep, dstRoi, coeffs, interpolation);

	}

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;
// 8u data type with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C1R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
												   Fw8u *pDst, int dstStep, FwiRect dstRoi, double xShear, 
												   double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw8u, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

// 8u data type with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C3R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
												   Fw8u *pDst, int dstStep, FwiRect dstRoi, double xShear, 
												   double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw8u, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

// 8u data type with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_C4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
												   Fw8u *pDst, int dstStep, FwiRect dstRoi, double xShear, 
												   double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw8u, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//8u data type with 4 channels (alpha channel will not be changed in the destination buffer during transformation)
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_AC4R)(const Fw8u *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
													Fw8u *pDst, int dstStep, FwiRect dstRoi, double xShear, 
													double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw8u, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//float data with 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C1R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
													Fw32f *pDst, int dstStep, FwiRect dstRoi, double xShear, 
													double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw32f, C1, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//float data with 3 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C3R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
													Fw32f *pDst, int dstStep, FwiRect dstRoi, double xShear, 
													double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw32f, C3, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//float data with 4 channels
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_C4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
													Fw32f *pDst, int dstStep, FwiRect dstRoi, double xShear, 
													double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw32f, C4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//float data with 3+ alpha channels, destination alpha channel data will not be changed.
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_AC4R)(const Fw32f *pSrc, FwiSize srcSize, int srcStep, FwiRect srcRoi, 
													 Fw32f *pDst, int dstStep, FwiRect dstRoi, double xShear, 
													 double yShear, double xShift, double yShift, int interpolation)
{
	return My_FW_Shear <Fw32f, AC4, DT_REFR> (pSrc, srcSize, srcStep, srcRoi, 
		pDst, dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
}

//8u data with 3 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_P3R)(
	const Fw8u *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[3], int dstStep, FwiRect dstRoi, double xShear,
	double yShear, double xShift, double yShift, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_Shear <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//8u data with 4 planars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_8u_P4R)(
	const Fw8u *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw8u *const pDst[4], int dstStep, FwiRect dstRoi, double xShear,
	double yShear, double xShift, double yShift, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_Shear <Fw8u, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 3 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_P3R)(
	const Fw32f *const pSrc[3], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[3], int dstStep, FwiRect dstRoi, double xShear,
	double yShear, double xShift, double yShift, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<3;i++) {
		status = My_FW_Shear <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//float data with 4 plannars
FwStatus PREFIX_OPT(OPT_PREFIX, fwiShear_32f_P4R)(
	const Fw32f *const pSrc[4], FwiSize srcSize, int srcStep, FwiRect srcRoi, 
	Fw32f *const pDst[4], int dstStep, FwiRect dstRoi, double xShear,
	double yShear, double xShift, double yShift, int interpolation)
{
	FwStatus status;
	int i;

	for (i=0;i<4;i++) {
		status = My_FW_Shear <Fw32f, C1, DT_REFR> (pSrc[i], srcSize, srcStep, srcRoi, 
			pDst[i], dstStep, dstRoi, xShear, yShear, xShift, yShift, interpolation);
		if (status!=fwStsNoErr) return status;
	}

	return fwStsNoErr;
}

//Description
//The function fwiGetShearQuad is declared in the fwi.h file. This function is used as a
//support function for fwiShear. It computes vertex coordinates of the quadrangle, to which the
//source rectangular ROI would be mapped by the shearing transform function fwiShear using
//coefficients xShear, yShear and shift values xShift, yShift.
//The first dimension [4] of the array quad[4][2] is equal to the number of vertices, and the
//second dimension [2] means x and y coordinates of the vertex. Quadrangle vertices have the
//following meaning:
//quad[0] corresponds to the transformed top-left corner of the source ROI,
//quad[1] corresponds to the transformed top-right corner of the source ROI,
//quad[2] corresponds to the transformed bottom-right corner of the source ROI,
//quad[3] corresponds to the transformed bottom-left corner of the source ROI.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetShearQuad)(FwiRect srcRoi, double quad[4][2], double xShear,
												   double yShear, double xShift, double yShift)
{
	double c[2][3];

	c[0][0] = 1;
	c[0][1] = xShear;
	c[1][0] = yShear;
	c[1][1] = 1;
	c[0][2] = xShift;
	c[1][2] = yShift;

	return fwiGetAffineQuad(srcRoi, quad, c);
}

//Description
//The function fwiGetShearBound is declared in the fwi.h file. This function is used as a
//support function for fwiShear. It computes vertex coordinates of the smallest bounding
//rectangle for the quadrangle quad, to which the source ROI would be mapped by the shearing
//transform function fwiShear using coefficients xShear, yShear and shift values xShift,
//yShift.
//bound[0] specifies x, y coordinates of the top-left corner, bound[1] specifies x, y coordinates
//of the bottom-right corner.

FwStatus PREFIX_OPT(OPT_PREFIX, fwiGetShearBound)(FwiRect srcRoi, double bound[2][2], double xShear,
													double yShear, double xShift, double yShift)
{
	double c[2][3];

	c[0][0] = 1;
	c[0][1] = xShear;
	c[1][0] = yShear;
	c[1][1] = 1;
	c[0][2] = xShift;
	c[1][2] = yShift;

	return fwiGetAffineBound(srcRoi, bound, c);
}

#endif //BUILD_NUM_AT_LEAST

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
