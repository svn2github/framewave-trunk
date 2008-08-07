/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __JPEG_HUFFCODEC_H__
#define __JPEG_HUFFCODEC_H__

#include "fe.h"
#include "FwSharedCode.h"
#include <stdio.h>

struct EncodeHuffmanSpec 
{
	Fw16u symcode[256];	            // symbol code
	Fw16u symlen[256];	            // symbol code length
};

struct EncodeHuffmanState 
{
	Fw32u accbuf;	                // accumulated buffer
	int accbitnum;	                // bit number for accumulated buffer
	int EOBRUN;		                // EOB run length
	int BE;		                    // count of buffered correction bits at the start of coding of the block
	Fw8u cor_AC[1024];              // Correction bit for AC refine
	Fw8u  *pCurrDst;                // Current dst pointer position
	int dstLenBytes;                // Bytes left in the current dst buffer
};

//struct DecodeHuffmanSpec 
//{ 
//	Fw8u  pListVals[256];		    // Copy of pListVals from SpecInit
//	Fw16u symcode[256];             // symbol code
//	Fw16u symlen[256];              // symbol code length
//	Fw32s mincode[18];              // smallest code value of specified length I
//	Fw32s maxcode[18];              // largest  code value of specified length I 
//	Fw32s valptr[18];               // index to the start of HUFFVAL decoded by code words of Length I 
//};
//
//struct DecodeHuffmanState 
//{
//	const Fw8u  *pCurrSrc;          //Current source pointer position
//	int srcLenBytes;                //Bytes left in the current source buffer
//	Fw32u accbuf;                   //accumulated buffer for extraction
//	int accbitnum;                  //bit number for accumulated buffer
//	int EOBRUN;                     //EOB run length
//	int marker;                     //JPEG marker
//};

extern const Fw8u zigZagFwdOrder[80];
#define EXTEND(V,T)  (V < (1<<(T-1)) ? (V + ((-1)<<T) + 1) : V)

namespace OPT_LEVEL
{
//Following JPEG standard Figure F.18
//ISV FILLBITS(FwiDecodeHuffmanState *pDecHuffState)
//    {
//    Fw32s B;
//    // read the data to fill the buffer
//    if(pDecHuffState->marker == 0)
//        {
//        while(pDecHuffState->accbitnum < 25)
//            {
//            if((--pDecHuffState->srcLenBytes) < 0)
//                return;                
//            B = *(pDecHuffState->pCurrSrc++);
//            if(B == 0x000000FF) // skipping the stuffed bits
//                {
//                do
//                   {
//                    if((--pDecHuffState->srcLenBytes) < 0)
//                        return;
//                    B = *(pDecHuffState->pCurrSrc++);
//                    }while(B==0x000000FF);
//
//                    if(B==0)
//                        B = 0x000000FF;
//                    else
//                        {             
//                        pDecHuffState->marker = B;
//                        break;
//                        }
//                    }
//                pDecHuffState->accbuf <<= 8;
//                pDecHuffState->accbuf |= B;
//                pDecHuffState->accbitnum += 8;
//            }
//            pDecHuffState->accbuf <<= (32 - pDecHuffState->accbitnum);
//        }
//    }
//
////Following JPEG standard Figure F.16
//IS Fw32s DECODE(const FwiDecodeHuffmanSpec *pTable, FwiDecodeHuffmanState *pDecHuffState)
//    {
//    int i=1, code, numberOfBits;
//
//    if(pDecHuffState->accbitnum < 8)
//        FILLBITS(pDecHuffState);
//
//    if(pDecHuffState->accbitnum > 7)
//        {
//        Fw32u look = pDecHuffState->accbuf >> 24;
//        numberOfBits = pTable->symcode[look];
//        if(numberOfBits != 0)
//            {
//            pDecHuffState->accbitnum -= numberOfBits;
//            pDecHuffState->accbuf <<= numberOfBits;
//            return pTable->symlen[look];
//            }
//        i = 8;
//        }
//
//    code = pDecHuffState->accbuf >> (32 - i);
//    while(code > pTable->maxcode[i])
//        {
//        i++;
//        if(pDecHuffState->accbitnum < i)
//            FILLBITS(pDecHuffState);
//        code = pDecHuffState->accbuf >> (32 - i);
//        }
//    pDecHuffState->accbuf <<= i;
//    pDecHuffState->accbitnum -= i;
//  
//    if(i>16) return 0;
//    return pTable->pListVals[pTable->valptr[i] + code - pTable->mincode[i]];
//    }
//
//IS FwStatus DecodeHuffman8x8_JPEG_1u16s_C1(
//    const Fw8u *pSrc, const int &srcLenBytes, int *pSrcCurrPos, Fw16s *pDst, 
//	Fw16s *pLastDC, int *pMarker, const FwiDecodeHuffmanSpec *pDcTable, 
//	const FwiDecodeHuffmanSpec *pAcTable, FwiDecodeHuffmanState *pDecHuffState)
//    {
//
//    if(FW_REF::PtrNotOK(pSrc, pSrcCurrPos, pDst, pLastDC, pMarker, pDcTable, pAcTable, pDecHuffState))
//        return fwStsNullPtrErr;
//
//    if ((srcLenBytes < 1) || (*pSrcCurrPos > srcLenBytes)) 
//		return fwStsSizeErr;
//
//	pDecHuffState->pCurrSrc = pSrc+*pSrcCurrPos;
//	pDecHuffState->srcLenBytes = srcLenBytes;
//	pDecHuffState->marker   = *pMarker;
//
//    // Decode DC Coefficient
//    // JPEG standard F.2.2.1 
//    Fw32s T, DIFF;
//    T = DECODE(pDcTable, pDecHuffState);
//    if(pDecHuffState->accbitnum < T)
//        FILLBITS(pDecHuffState);
//    DIFF = pDecHuffState->accbuf >> (32 - T);
//    pDecHuffState->accbuf <<= T;
//    pDecHuffState->accbitnum -= T;
//    DIFF = EXTEND(DIFF,T);
//
//    // make the dst buffer to be all zero's since we will be skipping
//    memset(pDst, 0, 128);//pDst is 16s type
//    *pDst = (Fw16s)(DIFF + *pLastDC);
//    *pLastDC = *pDst;
//
//    // Decode AC Coefficients
//    //Following JPEG standard Figure F.13
//    for(int k=1;k<64;k++)
//        {
//        Fw32s RS,SSSS,RRRR;
//        RS = DECODE(pAcTable, pDecHuffState);
//        SSSS = RS & 0x0000000F;
//        RRRR = RS >> 4;
//        if(SSSS)
//            {
//            k+=RRRR;
//            if(pDecHuffState->accbitnum < SSSS)
//                FILLBITS(pDecHuffState);
//            RS = pDecHuffState->accbuf >> (32 - SSSS);
//            pDecHuffState->accbuf <<= SSSS;
//            pDecHuffState->accbitnum -= SSSS;
//            RS = EXTEND(RS, SSSS);
//            pDst[zigZagFwdOrder[k]] = (Fw16s) RS;
//            }
//        else
//            {
//            if(RRRR!=15) 
//                break;
//            k+=15;
//            }
//        }
//
//    // update user variables
//    *pSrcCurrPos = (int)(pDecHuffState->pCurrSrc - pSrc);
//    *pMarker = pDecHuffState->marker;
//   
//    if(pDecHuffState->accbitnum < 0)
//        return fwStsJPEGOutOfBufErr;
//
//    if(pDecHuffState->marker)
//        return fwStsJPEGMarkerWarn;
//
//    return fwStsNoErr;
//    }

union float_n_int
    {
    float f;
    Fw32u ui;
    };

ISV ENCODE(FwiEncodeHuffmanState *pEncHuffState, Fw32u CODE, Fw32u SIZE)
    {
    Fw32s fillBits;
    Fw32u curAccBuf;

    fillBits = 32 - pEncHuffState->accbitnum;
    SIZE -= fillBits;
    curAccBuf = (pEncHuffState->accbuf << fillBits) | (CODE >> SIZE);  

    Fw8u FF_flag,bytes=0;
    Fw64u curAccBufRev = 0;
    const Fw32u eight = 8;

    for(Fw32u i=0;i<4;i++)
        {
        FF_flag = (Fw8u)curAccBuf;

        if(FF_flag == 0xff)
            {
            curAccBufRev <<= 16;
            curAccBufRev |= FF_flag;
            curAccBuf >>= eight;
            bytes+=2;
            continue;
            }

        curAccBufRev <<= eight;
        curAccBufRev |= FF_flag;
        curAccBuf >>= eight;
        bytes++;            
        }

    if((pEncHuffState->dstLenBytes -= bytes) < 0)
        return;

    *((Fw32u *)pEncHuffState->pCurrDst) = (Fw32u)curAccBufRev;
    pEncHuffState->pCurrDst+=4;
    curAccBufRev >>= 32;
    bytes-=4;

    while(bytes)
        {            
        *(pEncHuffState->pCurrDst) = (Fw8u)curAccBufRev;
        curAccBufRev >>= 8;
        pEncHuffState->pCurrDst++;
        bytes--;
        }

    CODE &= ((1<<SIZE) - 1);
    pEncHuffState->accbuf = CODE;
    pEncHuffState->accbitnum = SIZE;
    }

IS FwStatus EncodeHuffman8x8_JPEG_16s1u_C1(
	const Fw16s *pSrc, Fw8u *pDst, int dstLenBytes, int *pDstCurrPos, 
	Fw16s *pLastDC, const FwiEncodeHuffmanSpec *pDcTable, const 
	FwiEncodeHuffmanSpec *pAcTable, FwiEncodeHuffmanState *pEncHuffState, int bFlushState)
    {
    if (bFlushState)
        {
        if(FW_REF::PtrNotOK(pDst, pEncHuffState)) 
            return fwStsNullPtrErr;
	    if (dstLenBytes < 1 || (*pDstCurrPos > dstLenBytes))
            return fwStsSizeErr;

        Fw8u *pCurrDst  = pDst + *pDstCurrPos;
        Fw32u accbuf    = pEncHuffState->accbuf;
        Fw32s accbitnum = pEncHuffState->accbitnum;

        Fw32u curAccBuf, temp;
        Fw32u bytes = (accbitnum + 7) >> 3;    
        curAccBuf = accbuf << (32 -  accbitnum);
        curAccBuf |= ((0xffffffff) >> accbitnum);        

        while(bytes)
            {
            temp = curAccBuf >> 24;
            if((--dstLenBytes) < 0)
                    return fwStsSizeErr;
            *pCurrDst = (Fw8u)temp;
            pCurrDst++;
            if(((Fw8u)temp) == 0xff)
                {
                if((--dstLenBytes) < 0)
                    return fwStsSizeErr;
                *pCurrDst=0;
                pCurrDst++;
                }
            curAccBuf <<= 8;
            bytes--;
            }
        pEncHuffState->accbuf = 0;
        pEncHuffState->accbitnum = 0;
        return fwStsNoErr;
        }

	if(FW_REF::PtrNotOK(pSrc, pDst, pDstCurrPos, pLastDC, pDcTable, pAcTable, pEncHuffState))
		return fwStsNullPtrErr;

	if (dstLenBytes < 1 || (*pDstCurrPos > dstLenBytes))
		return fwStsSizeErr;

    pEncHuffState->pCurrDst = pDst + *pDstCurrPos;
    pEncHuffState->dstLenBytes = dstLenBytes;

    // encode DC coefficient following JPEG standard F.1.2.1.3
    Fw32s DIFF, SSSS, curAccBitLen;
    Fw32u CODE,SIZE;
    float_n_int temp;
    
    DIFF = *pSrc - *pLastDC;
    *pLastDC = *pSrc;    

    temp.f = (float)DIFF;
    temp.ui <<= 1;                               // take care of - ive (otherwise shift can go wrong) 
    SSSS =  temp.ui >> 24;                       // get the exponent
    if(SSSS)
        SSSS -= 126;                             // remove the bias

    if(SSSS > 11) 
        return fwStsJPEGDCTRangeErr;
 
    SIZE = pDcTable->symlen[SSSS];  
    CODE = pDcTable->symcode[SSSS];

    if(SSSS)                                     // add the additional bits to represent the diff
        {
        DIFF += (DIFF >> 31);
        DIFF &= ((1 << SSSS) -1);
        CODE <<= SSSS;
        CODE |= DIFF;
        SIZE += SSSS;
        }

    curAccBitLen = pEncHuffState->accbitnum + SIZE;    
    if(curAccBitLen < 33)
        {
        pEncHuffState->accbuf = (pEncHuffState->accbuf << SIZE)| CODE;
        pEncHuffState->accbitnum = curAccBitLen;
        }
    else
        ENCODE(pEncHuffState, CODE, SIZE);
        
    // encode AC coefficients following JPEG standard Figure F.2
    Fw32s ZZ_k,RS,R=0;
    for(int k=1;k<64;k++)
        {
        ZZ_k = pSrc[zigZagFwdOrder[k]];
        if(ZZ_k)
            {
            while(R>15)
                {
                curAccBitLen = pEncHuffState->accbitnum + pAcTable->symlen[0xF0];    
                if(curAccBitLen < 33)
                    {
                    pEncHuffState->accbuf = (pEncHuffState->accbuf << pAcTable->symlen[0xF0])| pAcTable->symcode[0xF0];
                    pEncHuffState->accbitnum = curAccBitLen;
                    }
                else
                    ENCODE(pEncHuffState, pAcTable->symcode[0xF0], pAcTable->symlen[0xF0]);
                 R -= 16;
                }
            // encode R and ZZ_k
            temp.f = (float)ZZ_k;
            temp.ui <<= 1;                               // take care of - ive (otherwise shift can go wrong) 
            SSSS =  temp.ui >> 24;                       // get the exponent
            SSSS -= 126;                                 // remove the bias

            if(SSSS > 10)
                return fwStsJPEGDCTRangeErr;

            RS = (R << 4) | SSSS;
            CODE = pAcTable->symcode[RS];
            SIZE = pAcTable->symlen[RS];
            ZZ_k += (ZZ_k >> 31);
            ZZ_k &= ((1 << SSSS) - 1);
            CODE <<= SSSS;
            CODE |= ZZ_k;
            SIZE += SSSS;
            curAccBitLen = pEncHuffState->accbitnum + SIZE;    
            if(curAccBitLen < 33)
                {
                pEncHuffState->accbuf = (pEncHuffState->accbuf << SIZE)| CODE;
                pEncHuffState->accbitnum = curAccBitLen;
                }
            else
                ENCODE(pEncHuffState, CODE, SIZE);
            R = 0;
            }
        else
            R++;
        }

    if(R > 0)// encode EOB
        {
        curAccBitLen = pEncHuffState->accbitnum + pAcTable->symlen[0];    
        if(curAccBitLen < 33)
            {
            pEncHuffState->accbuf = (pEncHuffState->accbuf << pAcTable->symlen[0])| pAcTable->symcode[0];
            pEncHuffState->accbitnum = curAccBitLen;
            }
        else
            ENCODE(pEncHuffState, pAcTable->symcode[0], pAcTable->symlen[0]);
        }

    *pDstCurrPos = (int)(pEncHuffState->pCurrDst - pDst);

    if(pEncHuffState->dstLenBytes < 0)
        return fwStsSizeErr;

    return fwStsNoErr;
    }

} // end namespace 

#endif

