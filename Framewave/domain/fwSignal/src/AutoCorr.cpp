/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "AutoCorr.h"

using namespace OPT_LEVEL;

// Normal Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_32f)(const  Fw32f*  pSrc,  int  srcLen,  Fw32f*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_fn_32f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
         result = AutoCorr::AutoCorr_fn_32f(pSrc, srcLen, pDst, dstLen);
   }
   
   return result;   
}

// Biased Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormA_32f)(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormA_fn_32f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormA_fn_32f(pSrc, srcLen, pDst, dstLen);
   }
   
   return result;
}

// Un-Biased Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormB_32f)(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormB_fn_32f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormB_fn_32f(pSrc, srcLen, pDst, dstLen);
   }
   
   return result;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_32fc)(const  Fw32fc*  pSrc,  int  srcLen,  Fw32fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_fn_32fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_fn_32fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormA_32fc)(const  Fw32fc*  pSrc,  int  srcLen,  Fw32fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormA_fn_32fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormA_fn_32fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormB_32fc)(const  Fw32fc*  pSrc,  int  srcLen,  Fw32fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormB_fn_32fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormB_fn_32fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_64f)(const Fw64f* pSrc, int srcLen, Fw64f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_fn_64f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
	      result = AutoCorr::AutoCorr_fn_64f(pSrc, srcLen, pDst, dstLen);
	}
   
   return result;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormA_64f)(const Fw64f* pSrc, int srcLen, Fw64f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormA_fn_64f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
	      result = AutoCorr::AutoCorr_NormA_fn_64f(pSrc, srcLen, pDst, dstLen);
	}
	
	return result;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormB_64f)(const Fw64f* pSrc, int srcLen, Fw64f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormB_fn_64f_SSE2(pSrc, srcLen, pDst, dstLen);
         break;	            
	  default:
	      result = AutoCorr::AutoCorr_NormB_fn_64f(pSrc, srcLen, pDst, dstLen);
	}
	
	return result;

}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_64fc)(const  Fw64fc*  pSrc,  int  srcLen,  Fw64fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_fn_64fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_fn_64fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormA_64fc)(const  Fw64fc*  pSrc,  int  srcLen,  Fw64fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormA_fn_64fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormA_fn_64fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormB_64fc)(const  Fw64fc*  pSrc,  int  srcLen,  Fw64fc*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;     
   if( dstLen > srcLen)                 return fwStsLengthErr;
   
   FwStatus result;
   
   switch(Dispatch::Type<DT_SSE2>())
	{
     case DT_SSE3:
     case DT_SSE2:
         result = AutoCorr::AutoCorr_NormB_fn_64fc_SSE2(pSrc, srcLen, pDst, dstLen);      // SSE2 Code
         break;	            
	  default:
         result = AutoCorr::AutoCorr_NormB_fn_64fc(pSrc, srcLen, pDst, dstLen);           // REFR Code
   }
   
   return result;   
}
// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2
