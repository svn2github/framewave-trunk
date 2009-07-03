/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"
#include "formatConversion.h"
#include "colorModelConversion.h"

using namespace OPT_LEVEL;

#define CMC_CHECK_PARAMS    if( pDst   ==0 || pSrc   ==0 )               return fwStsNullPtrErr;   \
                            if( dstStep<=0 || srcStep<=0 )               return fwStsStepErr;      \
                            if( roiSize.height<=0 || roiSize.width <=0 ) return fwStsSizeErr;

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422_8u_C2P3R )( const Fw8u* pSrc, int srcStep, 
                                                           Fw8u* pDst[3],    int dstStep[3], 
                                                           FwiSize roiSize )
{
    CMC_CHECK_PARAMS; 
    return CMC::ModelCP <CMC::CH_FC_C2P3 >::ConvertREF( pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422_8u_P3C2R )( const Fw8u* const pSrc[3], int srcStep[3], 
                                                           Fw8u* pDst, int dstStep,   FwiSize roiSize )
{
    CMC_CHECK_PARAMS;
    return CMC::ModelPC< CMC::CH_FC_P3C2 >::ConvertREF( pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiCbYCr422ToYCbCr420_8u_C2P3R )( const Fw8u* pSrc, int srcStep, 
                                                                     Fw8u* pDst[3],    int dstStep[3], 
                                                                     FwiSize roiSize )
{
    if( (srcStep<0) || (dstStep[0]<0) ||(dstStep[1]<0) || (dstStep[2]<0) )
        return fwStsStepErr;

    typedef F_422To420::ChannelToPlanar< CMC::CH_C2_422_BACA, Fw8u, C2, CMC::CH_P3, Fw8u, C1 > FeType;
    FeType data( srcStep, dstStep, roiSize );
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels. 
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fec1S3D< FeType >( data, pSrc,    srcStep<<1, 
                                    pDst[0], dstStep[0]<<1, 
                                    pDst[1], dstStep[1], 
                                    pDst[2], dstStep[2], newRoiSize );
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H
