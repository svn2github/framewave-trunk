/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __IMAGEJAEHNE_H__
#define __IMAGEJAEHNE_H__

#include "fe.h"

namespace OPT_LEVEL
{
/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus Jaehne(TD* pDst, int dstStep, FwiSize roiSize,  Fw32f a, Fw32f c) {
	if( FW_REF::PtrNotOK ( pDst	) ) return fwStsNullPtrErr;	
	if( FW_REF::StepNotOK( dstStep	) ) return fwStsStepErr;
	if( FW_REF::RoiNotOK ( roiSize	) ) return fwStsSizeErr;

	Fw32f x2, y2, h;
	h = FW_REF::To<Fw32f>::From(roiSize.height);
	const float pi = 3.1415926535897f/2.0f;//should we improve?
	float precalc[2] = { roiSize.width / 2.0f, roiSize.height / 2.0f };

	for( int y = 0 ;y < roiSize.height; FW_REF::Inc( pDst,dstStep ), ++y ) {
		TD* d = pDst;
		for( int x = 0 ;x<roiSize.width; d += ChannelCount(cd), ++x ) {
			x2 = pow( (x - precalc[0]) + 1, 2 );
			y2 = pow( (y - precalc[1]) + 1, 2 );
			TD t2 = FW_REF::To<TD>::From( (a*sin((pi * (x2 + y2)) / h))+c );
			switch( cd ) {
				case C4:  d[3] = t2; 
				case AC4:
				case C3:  d[2] = t2;  
						  d[1] = t2;  
				case C1:  d[0] = t2;  
			}	
		}
	}
	return fwStsNoErr;
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus JaehneU(TD* pDst, int dstStep, FwiSize roiSize) {
	Fw32f a = (FW_REF::Limits<TD>::MaxValue() / 2.0f);
	return Jaehne<TD,cd>(pDst, dstStep, roiSize, a, a);
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus JaehneS(TD* pDst, int dstStep, FwiSize roiSize) {
	Fw32f a = FW_REF::Limits<TD>::MaxValue() * 2.0f;
	return Jaehne<TD,cd>(pDst, dstStep, roiSize, a, 0.0);
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus JaehneF(TD* pDst, int dstStep, FwiSize roiSize) {
	return Jaehne<TD,cd>(pDst, dstStep, roiSize, 0.5, 0.5);
}

} // OPT_LEVEL
#endif

