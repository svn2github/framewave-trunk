/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
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

        if( FW_REF::PtrNotOK ( pSrc,pDst) ) return fwStsNullPtrErr;   
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


        for(Fw32s bufHeight = 0; bufHeight< roiSize.height; bufHeight++)
        {
            dcompY = dbeginY;
            scompY = sbeginY;
            for(Fw32s bufwidth = 0; bufwidth<roiSize.width; bufwidth++)
            {
                   *dcompY++ = *scompY++;                   
            }
            dbeginY+=dstStep[0];
            sbeginY+=srcStep[0];
        }

        for(Fw32s bufHeight = 0; bufHeight< cbcrHeight; bufHeight++)
        {
            dcompCb = dbeginCb;
            scompCb = sbeginCb;

            dcompCr = dbeginCr;
            scompCr = sbeginCr;

            for(Fw32s bufwidth = 0; bufwidth<cbcrWidth; bufwidth++)
            {
                   *dcompCb++ = *scompCb++;      
                   *dcompCr++ = *scompCr++;     
            }
            dbeginCb+=(dstStep[1]);
            sbeginCb+=(srcStep[1] * 2);
            dbeginCr+=(dstStep[2]);
            sbeginCr+=(srcStep[2] * 2);

        }
        return fwStsNoErr;
    }
}

#endif // __COLORFORMATCONVERSION_H__
