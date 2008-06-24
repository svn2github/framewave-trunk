/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "FwSharedCode.h"
#include "fwSignal.h"

namespace OPT_LEVEL
{
   namespace AutoCorr
   {
      template<typename TS>
      FwStatus AutoCorr_fn(const  TS*  pSrc,  int  srcLen, TS*  pDst,  int  dstLen)
      {
         TS sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum = 0;
            for ( int j = 0; (j < srcLen - 1) && (j + i) < srcLen  ; j++ ) {
               sum += pSrc[j] * pSrc[j + i];
            }
            pDst[i] = sum;
         }
         return fwStsNoErr;
      }
      
      template<typename TS>
      FwStatus AutoCorr_NormA_fn(const  TS*  pSrc,  int  srcLen, TS*  pDst,  int  dstLen)
      {
         TS sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum = 0;
            for ( int j = 0; (j < srcLen - 1) && (j + i) < srcLen ; j++ ) {
               sum += pSrc[j] * pSrc[j + i];
            }
            pDst[i] = sum/srcLen;                         // Biased
         }
         return fwStsNoErr;
      }
      
      template<typename TS>
      FwStatus AutoCorr_NormB_fn(const  TS*  pSrc,  int  srcLen, TS*  pDst,  int  dstLen)
      {
         TS sum;
         
         for ( int i = 0; i < dstLen; i++ ) {
            sum = 0;
            for ( int j = 0; (j < srcLen - 1) && (j + i) < srcLen ; j++ ) {
               sum += pSrc[j] * pSrc[j + i];
            }
            pDst[i] = sum/(srcLen - i);                         // Un-Biased
         }
         return fwStsNoErr;
      }
   }
}

using namespace OPT_LEVEL;

// Normal Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_32f)(const  Fw32f*  pSrc,  int  srcLen,  Fw32f*  pDst,  int  dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result = AutoCorr::AutoCorr_fn<Fw32f>(pSrc, srcLen, pDst, dstLen);
   return result;
      
}

// Biased Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormA_32f)(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result = AutoCorr::AutoCorr_NormA_fn<Fw32f>(pSrc, srcLen, pDst, dstLen);
   return result;

}

// Un-Biased Auto-Correlation Functions
FwStatus PREFIX_OPT(OPT_PREFIX, fwsAutoCorr_NormB_32f)(const Fw32f* pSrc, int srcLen, Fw32f* pDst, int dstLen)
{
   if( FW_REF::PtrNotOK ( pSrc, pDst )) return fwStsNullPtrErr;
   if( FW_REF::SizeNotOK( srcLen ))     return fwStsLengthErr;
   if( FW_REF::SizeNotOK( dstLen ))     return fwStsLengthErr;
     
   if( dstLen > srcLen)                return fwStsLengthErr;
   
   FwStatus result = AutoCorr::AutoCorr_NormB_fn<Fw32f>(pSrc, srcLen, pDst, dstLen);
   return result;

}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR
