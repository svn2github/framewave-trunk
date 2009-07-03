/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORFORMATCONVERSION_H__
#define __COLORFORMATCONVERSION_H__

#include "fwdev.h"
#include "FwSharedCode.h"


namespace OPT_LEVEL
{

    FwStatus ConvertYcbcr422toYcbcr420_8uP3(const Fw8u* pSrc[3], int srcStep[3], Fw8u* pDst[3], int dstStep[3], FwiSize roiSize)
    {

        if( FW_REF::PtrNotOK ( pSrc[0], pSrc[1], pSrc[2], pDst[0], pDst[1], pDst[2]) ) return fwStsNullPtrErr;   
        if( FW_REF::StepZeroNotOK( srcStep[0],srcStep[1],srcStep[2],dstStep[0],dstStep[1],dstStep[2]) ) return fwStsStepErr;
        if( FW_REF::RoiNotOK ( roiSize                        ) ) return fwStsSizeErr;


        Fw32s cbcrWidth = roiSize.width/2;
        Fw32s cbcrHeight = roiSize.height/2;

        const Fw8u* scompY, *scompCb,*scompCr;

        Fw8u* dcompY;
        Fw8u* dcompCb;
        Fw8u* dcompCr;

        Fw8u* dbeginY= pDst[0];
        const Fw8u* sbeginY= pSrc[0];
        
        Fw8u* dbeginCb= pDst[1];
        const Fw8u* sbeginCb= pSrc[1];

        Fw8u* dbeginCr= pDst[2];
        const Fw8u* sbeginCr= pSrc[2];



        for(Fw32s bufHeight = 0; bufHeight< cbcrHeight; bufHeight++)
        {
            dcompY = dbeginY;
            scompY = sbeginY;
            
            dcompCb = dbeginCb;
            scompCb = sbeginCb;

            dcompCr = dbeginCr;
            scompCr = sbeginCr;

            for(Fw32s bufwidth = 0; bufwidth<cbcrWidth; bufwidth++)
            {
                *dcompY++ = *scompY++; 
                *dcompCb++ = *scompCb++;      
                *dcompCr++ = *scompCr++;     
            }

            dbeginCb+=(dstStep[1]);
            sbeginCb+=(srcStep[1] * 2);
            dbeginCr+=(dstStep[2]);
            sbeginCr+=(srcStep[2] * 2);

            for(Fw32s bufwidth = 0; bufwidth<cbcrWidth; bufwidth++)
            {
                *dcompY++ = *scompY++;                   
            }

            dbeginY+=dstStep[0];
            sbeginY+=srcStep[0];
            for(Fw32s bufwidth = 0; bufwidth<roiSize.width; bufwidth++)
            {
                *dcompY++ = *scompY++;                   
            }
            dbeginY+=dstStep[0];
            sbeginY+=srcStep[0];
        }

        return fwStsNoErr;
    }

FwStatus ConvertYcbcr422toYcbcr420_8uC2P3(const Fw8u* pSrc, int srcStep, Fw8u* pDst[3], int dstStep[3], FwiSize roiSize)
    {

    if( FW_REF::PtrNotOK ( pSrc, pDst[0], pDst[1], pDst[2]) ) return fwStsNullPtrErr;   
    if( FW_REF::StepZeroNotOK( srcStep, dstStep[0],dstStep[1],dstStep[2]) ) return fwStsStepErr;
    if( FW_REF::RoiNotOK ( roiSize                        ) ) return fwStsSizeErr;

    int i,j;
    const Fw8u *pS, *pSRowBegin;   
    Fw8u *pY,*pCb,*pCr,*pYRowBegin,*pCbRowBegin,*pCrRowBegin;
    int height = roiSize.height/2;
    int width  = roiSize.width/2;

    pYRowBegin  = pDst[0];
    pCbRowBegin = pDst[1];
    pCrRowBegin = pDst[2];
    pSRowBegin = pSrc;

    for(i=0;i<height;i++)
        {        
        pS  = pSRowBegin;
        pY  = pYRowBegin;
        pCb = pCbRowBegin;
        pCr = pCrRowBegin;

        for(j=0;j<width;j++)
            {
            *pY++  = *pS++;
            *pCb++ = *pS++;
            *pY++  = *pS++;
            *pCr++ = *pS++;
            }

        pSRowBegin += srcStep;
        pYRowBegin += dstStep[0];
        pS  = pSRowBegin;
        pY  = pYRowBegin;

        for(j=0;j<width*4;j+=2)
            *pY++  = pS[j];

        pSRowBegin += srcStep;
        pYRowBegin += dstStep[0];
        pCbRowBegin += dstStep[1];
        pCrRowBegin += dstStep[2];
        }
    return fwStsNoErr;
    }

}


#endif // __COLORFORMATCONVERSION_H__
