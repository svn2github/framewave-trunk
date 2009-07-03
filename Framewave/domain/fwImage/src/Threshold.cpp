/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "ThresholdIdef.h"
#include "fwImage.h"

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, FwCmpOp fwCmpOp )
{

	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_C1R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_C1R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_C1R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_16s_C1R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_C1R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_C1R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_C3R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_C3R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_C3R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_16s_C3R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_C3R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_C3R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}	
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_AC4R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_AC4R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_AC4R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_16s_AC4R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_AC4R_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_AC4R_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

// in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_C1IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_C1IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_C1IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	case fwCmpGreater:					
		{
				Threshold_16s_C1IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_C1IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_C1IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
// in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_C3IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_C3IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_C3IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_16s_C3IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_C3IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_C3IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_8u_AC4IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_8u_AC4IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_8u_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_16s_AC4IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_16s_AC4IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_16s_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], FwCmpOp fwCmpOp )
{ 
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_32f_AC4IR_LT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_32f_AC4IR_GT data(threshold);
				return OPT_LEVEL::fe<Threshold_32f_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}







//---------------------------------------------------------------------------------------------------//
//--------------------------------------iThreshold_Val_----------------------------------------------//
//---------------------------------------------------------------------------------------------------//

// not in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u threshold, Fw8u value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_C1R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_C1R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s threshold, Fw16s value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_C1R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_C1R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f threshold, Fw32f value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_C1R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C1R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_C1R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C1R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}

// not in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_C3R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_C3R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_C3R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_C3R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_C3R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C3R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_C3R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C3R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_AC4R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_AC4R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_AC4R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_AC4R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_AC4R_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_AC4R_LT>(data,pSrc,srcStep,pDst,dstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_AC4R_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_AC4R_GT>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
// in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u threshold, Fw8u value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_C1IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_C1IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s threshold, Fw16s value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_C1IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_C1IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f threshold, Fw32f value, FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_C1IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C1IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_C1IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C1IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
// in-place, multi-channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_C3IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_C3IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_C3IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_C3IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_C3IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C3IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_C3IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_C3IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u threshold[3], const Fw8u value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_8u_AC4IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_8u_AC4IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_8u_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s threshold[3], const Fw16s value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_16s_AC4IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_16s_AC4IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_16s_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_Val_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f threshold[3], const Fw32f value[3], FwCmpOp fwCmpOp )
{
	switch(fwCmpOp)
	{
	case fwCmpLess:	
		{
				Threshold_Val_32f_AC4IR_LT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_AC4IR_LT>(data,pSrcDst,srcDstStep,roiSize);			
		}
	case fwCmpGreater:					
		{
				Threshold_Val_32f_AC4IR_GT data(threshold,value);
				return OPT_LEVEL::fe<Threshold_Val_32f_AC4IR_GT>(data,pSrcDst,srcDstStep,roiSize);		
		}
	default : 
		return fwStsNotSupportedModeErr;
	}
}



//---------------------------------------------------------------------------------------------------//
//---------------------------------iThreshold_LTValGTVal_--------------------------------------------//
//---------------------------------------------------------------------------------------------------//

// not in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C1R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, Fw8u thresholdLT, Fw8u valueLT, Fw8u thresholdGT, Fw8u valueGT )
{ 
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_8u_C1R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_C1R>(data,pSrc,srcStep,pDst,dstStep,roiSize);	

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C1R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, Fw16s thresholdLT, Fw16s valueLT, Fw16s thresholdGT, Fw16s valueGT )
{
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_16s_C1R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_C1R>(data,pSrc,srcStep,pDst,dstStep,roiSize);	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C1R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, Fw32f thresholdLT, Fw32f valueLT, Fw32f thresholdGT, Fw32f valueGT )
{
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_32f_C1R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_C1R>(data,pSrc,srcStep,pDst,dstStep,roiSize);
}


// not in-place, multi-channel

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C3R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_8u_C3R data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_C3R>(data,pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C3R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_16s_C3R data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_C3R>(data,pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C3R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3] )
{

	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_32f_C3R data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_C3R>(data,pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_AC4R)( const Fw8u *pSrc, int srcStep, Fw8u *pDst, int dstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3] )
{ 
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_8u_AC4R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_AC4R>(data,pSrc,srcStep,pDst,dstStep,roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_AC4R)( const Fw16s *pSrc, int srcStep, Fw16s *pDst, int dstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_16s_AC4R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_AC4R>(data,pSrc,srcStep,pDst,dstStep,roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_AC4R)( const Fw32f *pSrc, int srcStep, Fw32f *pDst, int dstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_32f_AC4R data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_AC4R>(data,pSrc,srcStep,pDst,dstStep,roiSize);
}

// in-place, 1 channel
FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C1IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, Fw8u thresholdLT, Fw8u valueLT, Fw8u thresholdGT, Fw8u valueGT )
{
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_8u_C1IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_C1IR>(data,pSrcDst,srcDstStep,roiSize);		
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C1IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, Fw16s thresholdLT, Fw16s valueLT, Fw16s thresholdGT, Fw16s valueGT )
{
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_16s_C1IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_C1IR>(data,pSrcDst,srcDstStep,roiSize);	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C1IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, Fw32f thresholdLT, Fw32f valueLT, Fw32f thresholdGT, Fw32f valueGT )
{
	if( thresholdLT > thresholdGT ) return fwStsThresholdErr;
	Threshold_LTValGTVal_32f_C1IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_C1IR>(data,pSrcDst,srcDstStep,roiSize);	
}

// in-place, multi-channel

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_C3IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_8u_C3IR data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_C3IR>(data,pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_C3IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3] )
{

	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_16s_C3IR data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_C3IR>(data,pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_C3IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3] )
{

	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;

	Threshold_LTValGTVal_32f_C3IR data(thresholdLT,valueLT,thresholdGT,valueGT);

	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_C3IR>(data,pSrcDst, srcDstStep, roiSize);
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_8u_AC4IR)( Fw8u *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw8u thresholdLT[3], const Fw8u valueLT[3], const Fw8u thresholdGT[3], const Fw8u valueGT[3] )
{ 
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_8u_AC4IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_8u_AC4IR>(data,pSrcDst,srcDstStep,roiSize);	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_16s_AC4IR)( Fw16s *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw16s thresholdLT[3], const Fw16s valueLT[3], const Fw16s thresholdGT[3], const Fw16s valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_16s_AC4IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_16s_AC4IR>(data,pSrcDst,srcDstStep,roiSize);	
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiThreshold_LTValGTVal_32f_AC4IR)( Fw32f *pSrcDst, int srcDstStep, FwiSize roiSize, const Fw32f thresholdLT[3], const Fw32f valueLT[3], const Fw32f thresholdGT[3], const Fw32f valueGT[3] )
{
	for(int i=0; i<3; ++i) if( thresholdLT[i] > thresholdGT[i] ) return fwStsThresholdErr;
	Threshold_LTValGTVal_32f_AC4IR data(thresholdLT, valueLT, thresholdGT, valueGT);
	return OPT_LEVEL::fe<Threshold_LTValGTVal_32f_AC4IR>(data,pSrcDst,srcDstStep,roiSize);	
}

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
