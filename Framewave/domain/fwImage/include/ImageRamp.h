/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __IMAGERAMP_H__
#define __IMAGERAMP_H__

#include "fe.h"

namespace OPT_LEVEL
{
/////////////////////////////////////// REF FUNCTIONS ////////////////////////////////////////////

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus RampB(TD* pDst, int dStep, FwiSize size, float offset, float slope) {
	Fw32f f;
	for( int x = 0 ;x<size.width; pDst += ChannelCount(cd), ++x ) {
		TD* d = pDst;
		for( int y = 0 ;y < size.height; FW_REF::Inc( d,dStep ), ++y ) {
			f = offset + slope * (x+y);
			TD t = FW_REF::To<TD>::From( f );
			switch( cd ) {
				case C3:  d[2] = t;  
						  d[1] = t;  
				case C1:  d[0] = t;  
			}	
		}
	}
	return fwStsNoErr;
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus RampH(TD* pDst, int dStep, FwiSize size, float offset, float slope) {
	Fw32f f;
	for( int x = 0 ;x<size.width; pDst += ChannelCount(cd), ++x ) {
		f = offset + slope * x;
		TD t = FW_REF::To<TD>::From( f );
		TD* d = pDst;
		for( int y = 0 ;y < size.height; FW_REF::Inc( d,dStep ), ++y ) {
			switch( cd ) {
				case C3:  d[2] = t;  
						  d[1] = t;  
				case C1:  d[0] = t;  
			}	
		}
	}
	return fwStsNoErr;
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus RampV(TD* pDst, int dStep, FwiSize size, float offset, float slope) {
	Fw32f f;
	for( int y = 0 ;y < size.height; FW_REF::Inc( pDst,dStep ), ++y ) {
		f = offset + slope * y;
		TD t = FW_REF::To<TD>::From( f );
		TD* d = pDst;
		for( int x = 0 ;x<size.width; d += ChannelCount(cd), ++x ) {
			switch( cd ) {
				case C3:  d[2] = t;  
						  d[1] = t;  
				case C1:  d[0] = t;  
			}	
		}
	}
	return fwStsNoErr;
}

template<class TD, CH cd>
SYS_INLINE STATIC FwStatus Ramp(TD* pDst, int dStep, FwiSize size, float offset, float slope, FwiAxis axis) {
	if( FW_REF::PtrNotOK ( pDst  ) ) return fwStsNullPtrErr;	
	if( FW_REF::StepNotOK( dStep ) ) return fwStsStepErr;
	if( FW_REF::RoiNotOK ( size  ) ) return fwStsSizeErr;

	switch(axis) {
		case fwAxsHorizontal:
			return RampH<TD,cd> (pDst, dStep, size, offset, slope);
		case fwAxsVertical:
			return RampV<TD,cd> (pDst, dStep, size, offset, slope);
		case fwAxsBoth:
			return RampB<TD,cd> (pDst, dStep, size, offset, slope);
	}
	return fwStsBadArgErr;
}

} // OPT_LEVEL

#endif


