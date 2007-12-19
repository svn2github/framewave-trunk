/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "logical.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C1R  	)( const Fw8u   *pSrc, int srcStep,       Fw32u value,    Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if ( value == 0) return fwiCopy_8u_C1R				( pSrc, srcStep, pDst, dstStep, roiSize );
		if ( value >= 8) return fwiSet_8u_C1R	(       0,  pDst, dstStep, roiSize );
		RShift_8u::RShift_B2_C1 dat(value);
		return OPT_LEVEL::fe< RShift_8u::RShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C1IR  	)(       Fw32u value,    Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if (value == 0) return fwStsNoErr;
		if ( value >= 8) return fwiSet_8u_C1R	(       0,  pSrcDst, srcDstStep, roiSize );
		RShift_8u::RShift_B1_C1 dat(value);
		return OPT_LEVEL::fe< RShift_8u::RShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C3R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_8u::RShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_8u::RShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_AC4R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[3], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_8u::RShift_B2_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_AC4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_8u::RShift_B2_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C3IR   )( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_8u::RShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8u::RShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_AC4IR  )( const Fw32u value[3], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_8u::RShift_B1_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_AC4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8u::RShift_B1_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C4R  	)( const Fw8u   *pSrc, int srcStep, const Fw32u value[4], Fw8u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_8u::RShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_8u::RShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8u_C4IR   )( const Fw32u value[4], Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_8u::RShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8u::RShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< RShift_8u::RShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C1R  	)( const Fw8s   *pSrc, int srcStep,       Fw32u value,    Fw8s *pDst, int dstStep, FwiSize roiSize)
	{ 
	if ( value == 0) return fwiCopy_8u_C1R				( (Fw8u*)pSrc, srcStep, (Fw8u*)pDst, dstStep, roiSize );
	RShift_8s::RShift_B2_C1 dat(value);
		return OPT_LEVEL::fe< RShift_8s::RShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C1IR  	)(       Fw32u value,    Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if (value == 0) return fwStsNoErr;
		RShift_8s::RShift_B1_C1 dat(value);
		return OPT_LEVEL::fe< RShift_8s::RShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 
	}

	
	FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C3R  	)( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_8s::RShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_8s::RShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_AC4R  	)( const Fw8s   *pSrc, int srcStep, const Fw32u value[3], Fw8s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_8s::RShift_B2_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_AC4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_8s::RShift_B2_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C3IR   )( const Fw32u value[3], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_8s::RShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8s::RShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_AC4IR  )( const Fw32u value[3], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_8s::RShift_B1_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_AC4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8s::RShift_B1_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C4R  	)( const Fw8s   *pSrc, int srcStep, const Fw32u value[4], Fw8s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_8s::RShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_8s::RShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_8s_C4IR   )( const Fw32u value[4], Fw8s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_8s::RShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_8s::RShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< RShift_8s::RShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}





FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C1R 	)( const Fw16u  *pSrc, int srcStep,       Fw32u value,    Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if ( value == 0 ) return fwiCopy_16s_C1R			( (Fw16s*)pSrc, srcStep, (Fw16s*)pDst, dstStep, roiSize );
		if ( value >= 16) return fwiSet_16s_C1R				(   0,  (Fw16s*)pDst, dstStep, roiSize );
		RShift_16u::RShift_B2_C1 dat(value);
		return OPT_LEVEL::fe< RShift_16u::RShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C1IR 	)(       Fw32u value,    Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if ( value >= 16) return fwiSet_16s_C1R				(   0,  (Fw16s*)pSrcDst, srcDstStep, roiSize );
		RShift_16u::RShift_B1_C1 dat(value);
		return OPT_LEVEL::fe< RShift_16u::RShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 

	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C3R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_16u::RShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_16u::RShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_AC4R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[3], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		RShift_16u::RShift_B2_AC4 dat(value);
		return OPT_LEVEL::fe< RShift_16u::RShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C3IR   )( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_16u::RShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_16u::RShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_AC4IR  )( const Fw32u value[3], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		RShift_16u::RShift_B1_AC4 dat(value);
		return OPT_LEVEL::fe< RShift_16u::RShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C4R  	)( const Fw16u   *pSrc, int srcStep, const Fw32u value[4], Fw16u *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_16u::RShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_16u::RShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16u_C4IR   )( const Fw32u value[4], Fw16u *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_16u::RShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_16u::RShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< RShift_16u::RShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C1R 	)( const Fw16s  *pSrc, int srcStep,       Fw32u value,    Fw16s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if ( value == 0 ) return fwiCopy_16s_C1R			( pSrc, srcStep, pDst, dstStep, roiSize );
		RShift_16s::RShift_B2_C1 dat(value);
		return OPT_LEVEL::fe< RShift_16s::RShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C1IR 	)(       Fw32u value,    Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		RShift_16s::RShift_B1_C1 dat(value);
		return OPT_LEVEL::fe< RShift_16s::RShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 

	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C3R  	)( const Fw16s   *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_16s::RShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_16s::RShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_AC4R  	)( const Fw16s   *pSrc, int srcStep, const Fw32u value[3], Fw16s *pDst, int dstStep, FwiSize roiSize)
	{ 
		RShift_16s::RShift_B2_AC4 dat(value);
		return OPT_LEVEL::fe< RShift_16s::RShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C3IR   )( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_16s::RShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_16s::RShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_AC4IR  )( const Fw32u value[3], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		RShift_16s::RShift_B1_AC4 dat(value);
		return OPT_LEVEL::fe< RShift_16s::RShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C4R  	)( const Fw16s   *pSrc, int srcStep, const Fw32u value[4], Fw16s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_16s::RShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_16s::RShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_16s_C4IR   )( const Fw32u value[4], Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_16s::RShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_16s::RShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< RShift_16s::RShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C1R 	)( const Fw32s  *pSrc, int srcStep,       Fw32u value,    Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		Fw32u val;
		if(value > 31) val = 31;
		else	val = value;
		if (val==0) return fwiCopy_32s_C1R			( pSrc, srcStep, pDst, dstStep, roiSize );
		RShift_32s::RShift_B2_C1 dat(val);
		return OPT_LEVEL::fe< RShift_32s::RShift_B2_C1 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 

	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C1IR 	)(       Fw32u value,    Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		Fw32u val;
		if(value > 31) val = 31;
		else	val = value;
		RShift_32s::RShift_B1_C1 dat(val);
		return OPT_LEVEL::fe< RShift_32s::RShift_B1_C1 >( dat, pSrcDst, srcDstStep, roiSize ); 

	}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C3R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_32s::RShift_B2_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_C3_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else 
		{
			RShift_32s::RShift_B2_C3 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_C3 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_AC4R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[3], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_32s::RShift_B2_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_AC4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_32s::RShift_B2_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_AC4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C3IR   )( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2])
		{
			RShift_32s::RShift_B1_C3_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_C3_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_32s::RShift_B1_C3 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_C3 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_AC4IR  )( const Fw32u value[3], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]) 
		{
			RShift_32s::RShift_B1_AC4_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_AC4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_32s::RShift_B1_AC4 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_AC4 >( dat, pSrcDst, srcDstStep, roiSize );
		}
	}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C4R  	)( const Fw32s   *pSrc, int srcStep, const Fw32u value[4], Fw32s *pDst, int dstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_32s::RShift_B2_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_C4_eq >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
		else
		{
			RShift_32s::RShift_B2_C4 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B2_C4 >( dat, pSrc, srcStep, pDst, dstStep, roiSize ); 
		}
	}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiRShiftC_32s_C4IR   )( const Fw32u value[4], Fw32s *pSrcDst, int srcDstStep, FwiSize roiSize)
	{ 
		if(value[0] == value[1] && value[1] == value[2]&& value[2] == value[3])
		{
			RShift_32s::RShift_B1_C4_eq dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_C4_eq >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
		else 
		{
			RShift_32s::RShift_B1_C4 dat(value);
			return OPT_LEVEL::fe< RShift_32s::RShift_B1_C4 >( dat, pSrcDst, srcDstStep, roiSize ); 
		}
	}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
