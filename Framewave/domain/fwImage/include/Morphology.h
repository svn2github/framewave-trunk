/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __MORPHOLOGY_H__
#define __MORPHOLOGY_H__

namespace OPT_LEVEL
{

#if BUILD_NUM_AT_LEAST( 9999 )
	struct fwcvMorphState
	{
		Fw8u *pMask;
		FwiSize maskSize;
		FwiPoint anchor;
		bool isRectangule;
		bool isSymmetric;
		int roiWidth;
	};

	struct fwcvMorphAdvState
	{
		FwiSize roiSize;
		Fw8u *pMask;
		FwiSize maskSize;
		FwiPoint anchor;
		bool isRectangule;
		bool isSymmetric;
	};

	struct fwcvMorphGrayState
	{
		FwiSize roiSize;
		Fw8u *pMask;
		FwiSize maskSize;
		FwiPoint anchor;
		bool isRectangule;
		bool isSymmetric;
	};
#endif //BUILD_NUM_AT_LEAST( 9999 )

} // OPT_LEVEL
#endif //__MORPHOLOGY_H__
