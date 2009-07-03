/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include "fwdev.h"

namespace OPT_LEVEL
{

namespace{
	template<class TD, CH cd>
	SYS_WEAKINLINE STATIC TD* ImgMalloc(int widthPixels, int heightPixels, int* pStepBytes) {
		// Bytes per line of data
		*pStepBytes = widthPixels * ChannelCount(cd) * sizeof(TD);
		
		// Determine how many bytes to pad at the end of each line
		int pad = *pStepBytes%4;
		if(pad > 0) pad = 4-pad;

		// Calculate size, and allocate!
		*pStepBytes += pad;
		U32 size = (*pStepBytes) * heightPixels;

		TD* img = (TD*)fwMalloc(size);
		memset(img, 0, size);

		return img;
	}
}

} //OPT_LEVEL

#endif

