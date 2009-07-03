/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*******************************************************/
#include "buildnum.h"
#include "fwBase.h"
#include "fwdev.h"
#include "FwSharedCode.h"
#include "fwVideo.h"
#include "system.h"

#include <iostream>

using namespace OPT_LEVEL;

extern const unsigned int NUM_ELEMS;
extern SYS_FORCEALIGN_16 const Fw16s non_intra_quantizer_matrix[128];

FwStatus SYS_INLINE fwiQuantInvIntra_MPEG1_16s_C1I(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix)
{
	if(FW_REF::PtrNotOK(pSrcDst, pQPMatrix))return fwStsNullPtrErr;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
		for(unsigned int I = 1; I < NUM_ELEMS; I++)
		{
            int AcCoefficient;

            if(pSrcDst[I]<0)
                {
                AcCoefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((-pSrcDst[I] * QP * pQPMatrix[I])>>4);
                AcCoefficient = (AcCoefficient-1)|1;
                AcCoefficient = -AcCoefficient;
                }
            else
                {
                AcCoefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((pSrcDst[I] * QP * pQPMatrix[I])>>4);
                AcCoefficient = (AcCoefficient-1)|1;
                }
            pSrcDst[I] = (Fw16s)AcCoefficient;
         }
     }
	return fwStsNoErr;
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlockIntra_MPEG1_32s)(Fw32u **ppBitStream, int *pOffset, const Fw32s *pDCSizeTable, const Fw32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDCPred, Fw16s *pDstBlock, Fw32s *pDstSize)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pDCSizeTable, pACTable, pScanMatrix, pQPMatrix, pDCPred, pDstBlock)) return fwStsNullPtrErr;
	if(FW_REF::PtrNotOK(pDstSize))return fwStsNullPtrErr;

	FwStatus retVal;
    Fw32s code, diff, sign, DC, sdiff, i, j;
    Fw8s run;
    Fw16s level;

	retVal = fwiDecodeHuffmanOne_1u32s(ppBitStream, pOffset, &code, (FwVCHuffmanSpec_32s*)pDCSizeTable);

	if(retVal != fwStsNoErr)
		return retVal;

    if(code==0)
        diff = 0;
    else
        {
        diff = (**ppBitStream) << (31 - *pOffset);
        sdiff = diff;
        sign = (~sdiff) >> 31;
        diff = ((((Fw32u)(sign^sdiff))>>(32-(code))) ^ sign) - sign;  
        *pOffset = *pOffset - code;
        if(*pOffset < 0)
            {
               *pOffset = 31;
               *ppBitStream +=1;
            }
        }

    if (diff >= 0xffff)
        return fwStsH263VLCCodeErr;

    DC = *pDCPred + diff;
    *pDCPred = (Fw16s)DC;
    *pDstBlock = (Fw16s)(DC << 3);

    i = 0;
    *pDstSize = 0;
	for(;;)
	{
		retVal = fwiDecodeHuffmanPair_1u16s(ppBitStream, pOffset, pACTable, &run, &level);
		if(retVal != fwStsNoErr)
			return retVal;

        if(level==127)
            break;
        else if(level!=0)
            {
                i += run;
                j = pScanMatrix[i];
                if(((**ppBitStream)<<(31 - *pOffset)) >> 31)
                      level = -level;
                *pOffset = *pOffset - 1;
                if(*pOffset < 0 )
                    {
                    *pOffset = 31;
                    *ppBitStream +=1;
                    }
            }
        else
            {
             if(*pOffset >= 13)
                 {
                 run = (Fw8s)((**ppBitStream << (31 - *pOffset)) >> 26);
                 run = run + 1;
                 *pOffset = *pOffset - 6;
                 level = (Fw16s)(((Fw32s)(**ppBitStream << (31 - *pOffset))) >> 24);
                 *pOffset = *pOffset - 8;
                 if(*pOffset < 0 )
                     {
                     *pOffset = 31;
                     *ppBitStream +=1;
                     }
                 }
             else
                 {
                 Fw32u cur = **ppBitStream;
                 Fw32u next = *(*ppBitStream+1);
                 cur = cur << (31 - *pOffset);
                 next = next >> (*pOffset + 1);
                 cur = cur | next;
                 run = (Fw8s) (cur >> 26);
                 run = run + 1;
                 cur = cur << 6;
                 level = (Fw16s)(((Fw32s)cur)>>24);
                 *pOffset = *pOffset + 18;
                 *ppBitStream += 1;
                 }

               if(level == -128)
                   {
                    if(*pOffset >= 7)
                        {
                         level = (Fw16s)(Fw32s(Fw32u(**ppBitStream << (31 - *pOffset)) >> 24) - 256);
                         *pOffset = *pOffset - 8;
                         if(*pOffset < 0 )
                           {
                            *pOffset = 31;
                            *ppBitStream +=1;
                           }
                        }
                    else
                        {
                         Fw32u cur = **ppBitStream;
                         Fw32u next = *(*ppBitStream+1);
                         cur = cur << (31 - *pOffset);
                         next = next >> (*pOffset + 1);
                         cur = cur | next;
                         level = (Fw16s)((cur>>24) - 256);
                         *pOffset = *pOffset + 18;
                         *ppBitStream += 1;
                        }
                   }
               if(level == 0)
                   {
                    if(*pOffset >= 7)
                        {
                         level = (Fw16s)(Fw32u(**ppBitStream << (31 - *pOffset)) >> 24);
                         *pOffset = *pOffset - 8;
                         if(*pOffset < 0 )
                           {
                            *pOffset = 31;
                            *ppBitStream +=1;
                           }
                        }
                    else
                        {
                         Fw32u cur = **ppBitStream;
                         Fw32u next = *(*ppBitStream+1);
                         cur = cur << (31 - *pOffset);
                         next = next >> (*pOffset + 1);
                         cur = cur | next;
                         level = (Fw16s)(cur>>24);
                         *pOffset = *pOffset + 18;
                         *ppBitStream += 1;
                        }
                   }
               i += run;
               j = pScanMatrix[i];
            }
    if (i > 63)  
         return fwStsH263VLCCodeErr;

     pDstBlock[j] = level;

    }
    
    *pDstSize = i;

	retVal = fwiQuantInvIntra_MPEG1_16s_C1I(pDstBlock, QP, (NULL == pQPMatrix)? const_cast<Fw16s*>(non_intra_quantizer_matrix) : pQPMatrix);
    
    if(retVal != fwStsNoErr)
	     return retVal;
	return fwStsNoErr;
}

FwStatus STDCALL PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlockIntra_MPEG2_32s)(Fw32u **ppBitStream, int *pOffset, const FwVCHuffmanSpec_32s *pDCSizeTable, const FwVCHuffmanSpec_32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDCPred, Fw32s shiftDCVal, Fw16s *pDstBlock, Fw32s *pDstSize)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pDCSizeTable, pACTable, pScanMatrix, pQPMatrix, pDCPred, pDstBlock))return fwStsNullPtrErr;
	if(FW_REF::PtrNotOK(pDstSize))return fwStsNullPtrErr;

	FwStatus retVal;
    Fw32s code, diff, sign, DC, sdiff, i, j;
    Fw8s run;
    Fw16s level;

	retVal = fwiDecodeHuffmanOne_1u32s(ppBitStream, pOffset, &code, pDCSizeTable);

	if(retVal != fwStsNoErr)
		return retVal;

    if(code==0)
        diff = 0;
    else
        {
        diff = (**ppBitStream) << (31 - *pOffset);
        sdiff = diff;
        sign = (~sdiff) >> 31;
        diff = ((((Fw32u)(sign^sdiff))>>(32-(code))) ^ sign) - sign;  
        *pOffset = *pOffset - code;
        if(*pOffset < 0)
            {
               *pOffset = 31;
               *ppBitStream +=1;
            }
        }

    if (diff >= 0xffff)
        return fwStsH263VLCCodeErr;
    
    DC = *pDCPred + diff;
    *pDCPred = (Fw16s)DC;
    *pDstBlock = (Fw16s)(DC << shiftDCVal);

    i = 0;
    *pDstSize = 0;
	for(;;)
	{
		retVal = fwiDecodeHuffmanPair_1u16s(ppBitStream, pOffset, pACTable, &run, &level);
		if(retVal != fwStsNoErr)
			return retVal;

        if(level==127)
            break;
        else if(level!=0)
            {
                i += run;
                j = pScanMatrix[i];
                if(((**ppBitStream)<<(31 - *pOffset)) >> 31)
                      level = -level;
                *pOffset = *pOffset - 1;
                if(*pOffset < 0 )
                    {
                    *pOffset = 31;
                    *ppBitStream +=1;
                    }
            }
        else
            {
             if(*pOffset >= 17)
                 {
                 run = (Fw8s)((**ppBitStream << (31 - *pOffset)) >> 26);
                 run = run + 1;
                 *pOffset = *pOffset - 6;
                 level = (Fw16s)(((Fw32s)(**ppBitStream << (31 - *pOffset))) >> 20);
                 *pOffset = *pOffset - 12;
                 if(*pOffset < 0 )
                     {
                     *pOffset = 31;
                     *ppBitStream +=1;
                     }
                 }
             else
                 {
                 Fw32u cur = **ppBitStream;
                 Fw32u next = *(*ppBitStream+1);
                 cur = cur << (31 - *pOffset);
                 next = next >> (*pOffset + 1);
                 cur = cur | next;
                 run = (Fw8s) (cur >> 26);
                 run = run + 1;
                 cur = cur << 6;
                 level = (Fw16s)(((Fw32s)cur)>>20);
                 *pOffset = *pOffset + 14;
                 *ppBitStream += 1;
                 }     
               i += run;
               j = pScanMatrix[i];
            }
    if (i > 63)  
         return fwStsH263VLCCodeErr;

     pDstBlock[j] = level;

    }
    
    *pDstSize = i;

	retVal = fwiQuantInvIntra_MPEG2_16s_C1I(pDstBlock, QP, (NULL == pQPMatrix)? const_cast<Fw16s*>(non_intra_quantizer_matrix) : pQPMatrix);
    
    if(retVal != fwStsNoErr)
	     return retVal;
	return fwStsNoErr;
}

FwStatus SYS_INLINE fwiQuantInv_MPEG1_16s_C1I(Fw16s *pSrcDst, int QP, Fw16s *pQPMatrix)
{
	if(FW_REF::PtrNotOK(pSrcDst, pQPMatrix))return fwStsNullPtrErr;

	switch(Dispatch::Type<DT_SSE2>())
	{
	case DT_SSE3:
	case DT_SSE2:
	case DT_REFR:
	default:
        for(unsigned int I = 0; I < NUM_ELEMS; I++)
		{
            int coefficient;
            
            if(pSrcDst[I]==0)
                coefficient = 0;
            else if(pSrcDst[I]<0)
                {
                coefficient = CBL_LIBRARY::Limits<Fw16s>::Sat((( 2*(-pSrcDst[I]) + 1) * QP * pQPMatrix[I])>>5);
                coefficient = (coefficient-1)|1;
                coefficient = -coefficient;
                }
            else
                {
                coefficient = CBL_LIBRARY::Limits<Fw16s>::Sat(((2 * pSrcDst[I] + 1) * QP * pQPMatrix[I])>>5);
                coefficient = (coefficient-1)|1;
                }
            pSrcDst[I] = (Fw16s)coefficient;
            }
    }
	return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlock_MPEG1_32s)(Fw32u **ppBitStream, int *pOffset, const FwVCHuffmanSpec_32s *pDCTable, const FwVCHuffmanSpec_32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDstBlock, Fw32s *pDstSize)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pACTable, pScanMatrix, pQPMatrix, pDstBlock, pDstSize)) return fwStsNullPtrErr;
	pDCTable;
    FwStatus retVal;
    Fw32s i, j;
    Fw32s cur;
    Fw8s run;
    Fw16s level;
    
    i = -1;
    cur = (**ppBitStream) << (31 - *pOffset);

    if (cur < 0)
        {
           level = 1;
           if(((Fw32u)cur)&0x40000000) // assumed that offset is atleast 1
                level= -level;

            pDstBlock[0] = level;
            i++;   
            cur = cur << 2;
            *pOffset = *pOffset - 2;
            if(*pOffset < 0)
                {
                *pOffset = 31;
                *ppBitStream += 1;
                }
            
            if(cur <= (Fw32s)0xBFFFFFFF)
                {
                 *pOffset = *pOffset - 2;
                 if(*pOffset < 0)
                  {
                   *pOffset = 31;
                   *ppBitStream += 1;
                  }
                goto end;
                }
        }
	*pDstSize = 0;
	for(;;)
	{
		retVal = fwiDecodeHuffmanPair_1u16s(ppBitStream, pOffset, pACTable, &run, &level);
		if(retVal != fwStsNoErr)
			return retVal;

        if(level==127)
            break;
        else if(level!=0)
            {
                i += run;
                j = pScanMatrix[i];
                if(((**ppBitStream)<<(31 - *pOffset)) >> 31)
                      level = -level;
                *pOffset = *pOffset - 1;
                if(*pOffset < 0 )
                    {
                    *pOffset = 31;
                    *ppBitStream +=1;
                    }
            }
        else
            {
             if(*pOffset >= 13)
                 {
                 run = (Fw8s)((**ppBitStream << (31 - *pOffset)) >> 26);
                 run = run + 1;
                 *pOffset = *pOffset - 6;
                 level = (Fw16s)(((Fw32s)(**ppBitStream << (31 - *pOffset))) >> 24);
                 *pOffset = *pOffset - 8;
                 if(*pOffset < 0 )
                     {
                     *pOffset = 31;
                     *ppBitStream +=1;
                     }
                 }
             else
                 {
                 Fw32u cur = **ppBitStream;
                 Fw32u next = *(*ppBitStream+1);
                 cur = cur << (31 - *pOffset);
                 next = next >> (*pOffset + 1);
                 cur = cur | next;
                 run = (Fw8s) (cur >> 26);
                 run = run + 1;
                 cur = cur << 6;
                 level = (Fw16s)(((Fw32s)cur)>>24);
                 *pOffset = *pOffset + 18;
                 *ppBitStream += 1;
                 }

               if(level == -128)
                   {
                    if(*pOffset >= 7)
                        {
                         level = (Fw16s)(Fw32s(Fw32u(**ppBitStream << (31 - *pOffset)) >> 24) - 256);
                         *pOffset = *pOffset - 8;
                         if(*pOffset < 0 )
                           {
                            *pOffset = 31;
                            *ppBitStream +=1;
                           }
                        }
                    else
                        {
                         Fw32u cur = **ppBitStream;
                         Fw32u next = *(*ppBitStream+1);
                         cur = cur << (31 - *pOffset);
                         next = next >> (*pOffset + 1);
                         cur = cur | next;
                         level = (Fw16s)((cur>>24) - 256);
                         *pOffset = *pOffset + 18;
                         *ppBitStream += 1;
                        }
                   }
               if(level == 0)
                   {
                    if(*pOffset >= 7)
                        {
                         level = (Fw16s)(Fw32u(**ppBitStream << (31 - *pOffset)) >> 24);
                         *pOffset = *pOffset - 8;
                         if(*pOffset < 0 )
                           {
                            *pOffset = 31;
                            *ppBitStream +=1;
                           }
                        }
                    else
                        {
                         Fw32u cur = **ppBitStream;
                         Fw32u next = *(*ppBitStream+1);
                         cur = cur << (31 - *pOffset);
                         next = next >> (*pOffset + 1);
                         cur = cur | next;
                         level = (Fw16s)(cur>>24);
                         *pOffset = *pOffset + 18;
                         *ppBitStream += 1;
                        }
                   }  
               i += run;
               j = pScanMatrix[i];
            }
    if (i > 63)  
         return fwStsH263VLCCodeErr;

     pDstBlock[j] = level;
    }

end:
    *pDstSize = i;
	fwiQuantInv_MPEG1_16s_C1I(pDstBlock, QP, (NULL == pQPMatrix)? const_cast<Fw16s*>(non_intra_quantizer_matrix) : pQPMatrix);
	return fwStsNoErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiReconstructDCTBlock_MPEG2_32s)(Fw32u **ppBitStream, int *pOffset, const FwVCHuffmanSpec_32s *pDCTable, const FwVCHuffmanSpec_32s *pACTable, Fw32s *pScanMatrix, int QP, Fw16s *pQPMatrix, Fw16s *pDstBlock, Fw32s *pDstSize)
{
	if(FW_REF::PtrNotOK(ppBitStream, pOffset, pACTable, pScanMatrix, pQPMatrix, pDstBlock, pDstSize)) return fwStsNullPtrErr;
	pDCTable;
    FwStatus retVal;
    Fw32s i, j;
    Fw32s cur;
    Fw8s run;
    Fw16s level;

    i = -1;
    
    cur = (**ppBitStream) << (31 - *pOffset);

    if (cur < 0)
        {
           level = 1;
           if(((Fw32u)cur)&0x40000000) // assumed that offset is atleast 1
                level= -level;

            pDstBlock[0] = level;
            i++;   
            cur = cur << 2;
            *pOffset = *pOffset - 2;
            if(*pOffset < 0)
                {
                *pOffset = 31;
                *ppBitStream += 1;
                }
            
            if(cur <= (Fw32s)0xBFFFFFFF)
                {
                 *pOffset = *pOffset - 2;
                 if(*pOffset < 0)
                  {
                   *pOffset = 31;
                   *ppBitStream += 1;
                  }
                goto end;
                }
        }

	*pDstSize = 0;
	for(;;)
	{
		retVal = fwiDecodeHuffmanPair_1u16s(ppBitStream, pOffset, pACTable, &run, &level);
		if(retVal != fwStsNoErr)
			return retVal;

        if(level==127)
            break;
        else if(level!=0)
            {
                i += run;
                j = pScanMatrix[i];
                if(((**ppBitStream)<<(31 - *pOffset)) >> 31)
                      level = -level;
                *pOffset = *pOffset - 1;
                if(*pOffset < 0 )
                    {
                    *pOffset = 31;
                    *ppBitStream +=1;
                    }
            }
        else
            {
             if(*pOffset >= 17)
                 {
                 run = (Fw8s)((**ppBitStream << (31 - *pOffset)) >> 26);
                 run = run + 1;
                 *pOffset = *pOffset - 6;
                 level = (Fw16s)(((Fw32s)(**ppBitStream << (31 - *pOffset))) >> 20);
                 *pOffset = *pOffset - 12;
                 if(*pOffset < 0 )
                     {
                     *pOffset = 31;
                     *ppBitStream +=1;
                     }
                 }
             else
                 {
                 Fw32u cur = **ppBitStream;
                 Fw32u next = *(*ppBitStream+1);
                 cur = cur << (31 - *pOffset);
                 next = next >> (*pOffset + 1);
                 cur = cur | next;
                 run = (Fw8s) (cur >> 26);
                 run = run + 1;
                 cur = cur << 6;
                 level = (Fw16s)(((Fw32s)cur)>>20);
                 *pOffset = *pOffset + 14;
                 *ppBitStream += 1;
                 }     
               i += run;
               j = pScanMatrix[i];
            }
    if (i > 63)  
         return fwStsH263VLCCodeErr;

     pDstBlock[j] = level;
    }

end:
    *pDstSize = i;
	fwiQuantInv_MPEG2_16s_C1I(pDstBlock, QP, (NULL == pQPMatrix)? const_cast<Fw16s*>(non_intra_quantizer_matrix) : pQPMatrix);
	return fwStsNoErr;
}



// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
