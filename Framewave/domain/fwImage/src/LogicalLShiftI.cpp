/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "logical.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

//LShiftC

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C1R  	)( const Fw8u   *pSrc, int srcStep,       Fw32u value,    Fw8u *pDst, int dstStep, FwiSize roiSize)
	{
	if ( value == 0)	return fwiCopy_8u_C1R				( pSrc, srcStep, pDst, dstStep, roiSize );
	if ( value >= 8)	return fwiSet_8u_C1R	(       0,  pDst, dstStep, roiSize );
	LShift_8u::LShift_B2_C1 dat(value);
	return OPT_LEVEL::fe< LShift_8u::LShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 

	}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C1IR  	)(       Fw32u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
	if (value == 0)		return fwStsNoErr;
	if ( value >= 8)	return fwiSet_8u_C1R	(       0,  pSrcDst, srcDstStep, roiSize );
	LShift_8u::LShift_B1_C1 dat(value);
	return OPT_LEVEL::fe< LShift_8u::LShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C3R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_8u::LShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			LShift_8u::LShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_AC4R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			LShift_8u::LShift_B2_AC4_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_AC4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			LShift_8u::LShift_B2_AC4 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C3IR   )( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_8u::LShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_8u::LShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_AC4IR  )( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			LShift_8u::LShift_B1_AC4_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_AC4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_8u::LShift_B1_AC4 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C4R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_8u::LShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			LShift_8u::LShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_8u_C4IR   )( const Fw32u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_8u::LShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_8u::LShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< LShift_8u::LShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C1R 	)( const Fw16u  *pSrc, int srcStep,       Fw32u value,    Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
	if ( value == 0 ) return fwiCopy_16s_C1R			( (Fw16s*)pSrc, srcStep, (Fw16s*)pDst, dstStep, roiSize );
	if ( value >= 16) return fwiSet_16s_C1R				(   0,  (Fw16s*)pDst, dstStep, roiSize );
	LShift_16u::LShift_B2_C1 dat(value);
	return OPT_LEVEL::fe< LShift_16u::LShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C1IR 	)(       Fw32u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
	if ( value >= 16) return fwiSet_16s_C1R				(   0,  (Fw16s*)pSrcDst, srcDstStep, roiSize );
	LShift_16u::LShift_B1_C1 dat(value);
	return OPT_LEVEL::fe< LShift_16u::LShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 

	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C3R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_16u::LShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			LShift_16u::LShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_AC4R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		LShift_16u::LShift_B2_AC4 dat(value);
		return OPT_LEVEL::fe< LShift_16u::LShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C3IR   )( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_16u::LShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_16u::LShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_AC4IR  )( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		LShift_16u::LShift_B1_AC4 dat(value);
		return OPT_LEVEL::fe< LShift_16u::LShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C4R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_16u::LShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			LShift_16u::LShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_16u_C4IR   )( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_16u::LShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_16u::LShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< LShift_16u::LShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C1R 	)( const Fw32s  *pSrc, int srcStep,       Fw32u value,    Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		Fw32u val;
		if(value > 32) val = 32;
		else	val = value;
		if (val==0) return fwiCopy_32s_C1R			( pSrc, srcStep, pDst, dstStep, roiSize );
		if ( val >= 32) return fwiSet_32s_C1R(  0,    pDst, dstStep, roiSize );
		LShift_32s::LShift_B2_C1 dat(value);
		return OPT_LEVEL::fe< LShift_32s::LShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C1IR 	)(       Fw32u value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		
		Fw32u val;
		if(value > 32) val = 32;
		else	val = value;
		if ( val >= 32) return fwiSet_32s_C1R(  0,    pSrcDst, srcDstStep, roiSize );
		LShift_32s::LShift_B1_C1 dat(value);
		return OPT_LEVEL::fe< LShift_32s::LShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 

	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C3R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_32s::LShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			LShift_32s::LShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_AC4R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			LShift_32s::LShift_B2_AC4_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_AC4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			LShift_32s::LShift_B2_AC4 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C3IR   )( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			LShift_32s::LShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_32s::LShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_AC4IR  )( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			LShift_32s::LShift_B1_AC4_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_AC4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_32s::LShift_B1_AC4 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C4R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_32s::LShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			LShift_32s::LShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiLShiftC_32s_C4IR   )( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			LShift_32s::LShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			LShift_32s::LShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< LShift_32s::LShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
