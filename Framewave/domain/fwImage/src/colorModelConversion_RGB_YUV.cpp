/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "colorModelConversion.h"
#include "colorModelConvSpecial.h"
#include "colorModelConvSpecialDef.h"
#include "colorModelConversionDefs.h"
#include "fwImage.h"
#include "fe.h"

using namespace OPT_LEVEL;

/////////////////////////////////
// NON-CHROMA SAMPLING Functions
/////////////////////////////////

//----------------------------
// RGBToYUV
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToYUV, 
                                                CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYUV, CMC::PROC_NORMAL                  
                                              > FeType;
    FeType data;
    return fe< FeType > (data, pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToYUV, 
                                                CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYUV, CMC::PROC_NORMAL                  
                                              > FeType;
    FeType data;
    return fe< FeType > (data, pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV_8u_P3R )( const Fw8u * const pSrc[3], int srcStep, 
                                                         Fw8u *pDst[3],              int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::Planar3ToPlanar3< 
                                                CMC::CH_P3, Fw8u, C1, Fw8u, C1, RGBToYUV, 
                                                CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYUV, CMC::PROC_NORMAL
                                              > FeType;
    FeType data;
    return fec3S3D< FeType >(data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, 
                                   pDst[0], dstStep, pDst[1], dstStep, pDst[2], dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV_8u_C3P3R )( const Fw8u *pSrc, int srcStep, 
                                                           Fw8u *pDst[3],    int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::Channel3ToPlanar3< 
                                                 CMC::CH_C3P3, Fw8u, C3, Fw8u, C1, 
                                                 RGBToYUV, CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                 CMC::RGBToYUV, CMC::PROC_NORMAL                 
                                               > FeType;
    FeType data;
    return fec1S3D< FeType >(data, pSrc, srcStep, pDst[0], dstStep, pDst[1], dstStep, pDst[2], dstStep, roiSize );
}


//----------------------------
// YUVToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUVToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_C3, Fw8u, C3, Fw8u, C3, YUVToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YUVToRGB, CMC::PROC_SHIFT13 
                                              > FeType;
    FeType data;
    return fe< FeType > (data, pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUVToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, YUVToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YUVToRGB, CMC::PROC_SHIFT13 
                                              > FeType;
    FeType data;
    return fe< FeType > (data, pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUVToRGB_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                         Fw8u *pDst[3],             int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::Planar3ToPlanar3< 
                                                CMC::CH_P3, Fw8u, C1, Fw8u, C1, YUVToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YUVToRGB, CMC::PROC_SHIFT13 
                                              > FeType;
    FeType data;
    return fec3S3D< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, 
                                    pDst[0], dstStep, pDst[1], dstStep, pDst[2], dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUVToRGB_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                           Fw8u *pDst,                int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::PlanarToChannel< 
                                               CMC::CH_P3C3, Fw8u, C1, Fw8u, C3, YUVToRGB, 
                                               CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                               CMC::YUVToRGB, CMC::PROC_SHIFT13 
                                             > FeType;
    FeType data;
    return fe< FeType > (data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize);
}

///////////////////////////////
// SUB-SAMPLING Functions
///////////////////////////////

//----------------------------
// RGB To YUV422
//  These functions compress RGB data into a 4:2:2 YUV
//  Every 2 pixels w/ 3 Channels of RGB are put into 2 pixel sets of 2 Channels of YUV
//  (R0 G0 B0 | R1 G1 B1 -> Y0 U0 | Y1 V0)
//----------------------------

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV422_8u_C3C2R )( const Fw8u *pSrc, int srcStep, 
                                                              Fw8u *pDst,       int dstStep, FwiSize roiSize )
{
    // Vertical Step = 1 byte, Increment RGB by 3*2 bytes, Increment YUV422 by 2*2 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 6, 4, 
                                      CMC::CH_C3, CMC::CH_C2_422_ABAC, Fw8u, Fw8u, RGBToYUV
                                    >( pSrc, srcStep, pDst, dstStep, roiSize, 0 );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV422_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                            Fw8u *pDst[3],             int dstStep[3], FwiSize roiSize )
{
    // Vertical Step = 1 byte, Increment RGB by 2 bytes (2bytes/pixel*1pixel= 2 bytes)
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 1, 2, 2, 
                                      CMC::CH_P3, CMC::CH_P3_422, Fw8u, Fw8u, RGBToYUV
                                    >( pSrc, srcStep, pDst, dstStep, roiSize, 0 );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV422_8u_P3 )( const Fw8u *const pSrc[3], Fw8u *pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) ) is the step size of the source buffer.
    // For the 422 sampled destination buffers, the step size of the Y channel is the same as the step size of the 
    // source buffer, but the U and V channels' step size are half.

    int step[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiRGBToYUV422_8u_P3R( pSrc, step[0], pDst, step, imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV422_8u_C3P3R )( const Fw8u *pSrc, int srcStep, 
                                                              Fw8u *pDst[3],    int dstStep[3], FwiSize roiSize )
{
    // Vertical Step = 1 byte, Increment RGB by 3*2 bytes, Increment YUV422 planes by 2*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 6, 2, 
                                      CMC::CH_C3, CMC::CH_P3_422, Fw8u, Fw8u, RGBToYUV
                                    >( pSrc, srcStep, pDst, dstStep, roiSize, 0 );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV422_8u_C3P3 )( const Fw8u *pSrc, Fw8u *pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) * 3 ) is the step size of the 3 channel source buffer.
    // For the 422 sampled destination buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.

    int srcStep[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiRGBToYUV422_8u_C3P3R ( pSrc, srcStep[0]*3, pDst, srcStep, imgSize);
}

//----------------------------
// YUV422 To RGB
//  These functions decompress from 4:2:2 YUV to RGB
//  Every 2 pixel set of YUV correspond to an RGB pair
//  (Y0 U0 | Y1 V0 -> R0 G0 B0 | R1 G1 B1)
//----------------------------

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_C2C3R )( const Fw8u *pSrc, int srcStep, 
                                                              Fw8u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src422_To_Dst444::ChannelToChannel< 
                                                              CMC::CH_C2_422_ABAC, Fw8u, C2, 
                                                              CMC::CH_C3, Fw8u, C3, RGB_FORMAT
                                                            > FeType;
    FeType data(srcStep, dstStep, roiSize);
    return fe< FeType > (data, pSrc, srcStep, pDst, dstStep, roiSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_P3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                            Fw8u *const pDst[3],       int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src422_To_Dst444::PlanarToPlanar<
                                                            CMC::CH_P3_422, Fw8u, C1, 
                                                            CMC::CH_P3,     Fw8u, C1, RGB_FORMAT
                                                          > FeType;
    FeType data(srcStep, dstStep, roiSize);
    return fec3S3D< FeType >( data, 
                              pSrc[0], srcStep[0], pSrc[1], srcStep[1], pSrc[2], srcStep[2],
                              pDst[0], dstStep,    pDst[1], dstStep,    pDst[2], dstStep,    roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_P3 )( const Fw8u *const pSrc[3], 
                                                           Fw8u *const pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) ) is the step size of the destination buffer.
    // For the 422 sampled source buffers, the step size of the Y channel is the same as the step size of the 
    // source buffer, but the U and V channels' step size are half.

    int srcStep[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiYUV422ToRGB_8u_P3R (pSrc, srcStep, pDst, srcStep[0], imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                              Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src422_To_Dst444::PlanarToChannel<  
                                                             CMC::CH_P3_422, Fw8u, C1, 
                                                             CMC::CH_C3,     Fw8u, C3, RGB_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    return fe< FeType >( data, pSrc[0], srcStep[0], pSrc[1], srcStep[1], pSrc[2], srcStep[2], pDst, dstStep, roiSize );

}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_P3C3 )( const Fw8u *const pSrc[3], Fw8u *pDst, FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) * 3 ) is the step size of the 3 channel destination buffer.
    // For the 422 sampled source buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.

    int srcStep[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiYUV422ToRGB_8u_P3C3R (pSrc, srcStep, pDst, srcStep[0]*3, imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV422ToRGB_8u_P3AC4R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src422_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_422, Fw8u, C1, 
                                                             CMC::CH_AC4,    Fw8u, AC4, RGB_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    return fe< FeType >( data, pSrc[0], srcStep[0], pSrc[1], srcStep[1], pSrc[2], srcStep[2], pDst, dstStep, roiSize );
}

//----------------------------
// RGB To YUV420 ---
//  These functions compress RGB data into a 4:2:0 YUV
//  Every 2x2 pixels w/ 3 Channels of RGB are put into 2x2 pixel sets of Y and 1 pixel of UV
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV420_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                            Fw8u *const pDst[3],       int dstStep[3], FwiSize roiSize )
{
    // Vertical Step = 2, Increment RGB & Y channel by 2 bytes (2bytes/pixel*1pixel= 2 bytes)
    return CMC_SS::Model::ConvertREF< 
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 2, 2, 2, 
                                      CMC::CH_P3, CMC::CH_P3_420, Fw8u, Fw8u, RGBToYUV
                                    >( pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0 );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV420_8u_P3 )( const Fw8u *const pSrc[3], 
                                                           Fw8u *const pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) ) is the step size of the 3 plane source buffer.
    // For the 420 sampled destination buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.
    int step[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiRGBToYUV420_8u_P3R( pSrc, step[0], pDst, step, imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV420_8u_C3P3R )( const Fw8u *pSrc,    int srcStep, 
                                                              Fw8u *const pDst[3], int dstStep[3], FwiSize roiSize )
{
    // Vertical Step = 2, Increment RGB by 3*2 bytes, YUV420 planar by 2*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 2, 6, 2,
                                      CMC::CH_C3, CMC::CH_P3_420, Fw8u, Fw8u, RGBToYUV
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYUV420_8u_C3P3 )( const Fw8u *pSrc, Fw8u *const pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) * 3 ) is the step size of the 3 channel source buffer.
    // For the 420 sampled destination buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.
    int step[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiRGBToYUV420_8u_C3P3R (pSrc, step[0]*3, pDst, step, imgSize);
}

//----------------------------
// YUV420 To RGB ---
//  These functions decompress from 4:2:0 YUV to RGB
//  Every 2x2 pixel set of Y and 1 pixel of UV correspond to 2x2 RGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB_8u_P3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                            Fw8u *const pDst[3],       int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToPlanar< 
                                                            CMC::CH_P3, Fw8u, C1, 
                                                            CMC::CH_P3, Fw8u, C1 
                                                          > FeType;
    FeType data(srcStep, dstStep, roiSize);

    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels. 
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };

    return fec3S3D< FeType >( data, pSrc[0], srcStep[0]<<1, pSrc[1], srcStep[1], pSrc[2], srcStep[2], 
                                    pDst[0], dstStep<<1, pDst[1], dstStep<<1, pDst[2], dstStep<<1, newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB_8u_P3 )( const Fw8u *const pSrc[3], 
                                                           Fw8u *const pDst[3], FwiSize imgSize )
{
    // When processing the whole image, ( image width * sizeof(Fw8u) ) is the step size of the 3 plane destination buffers.
    // For the 420 sampled source buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.
    int step[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiYUV420ToRGB_8u_P3R (pSrc, step, pDst, step[0], imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                              Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel< 
                                                             CMC::CH_P3_420, Fw8u, C1, 
                                                             CMC::CH_C3,     Fw8u, C3, RGB_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration. 
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, pSrc[0], srcStep[0]<<1, 
                               pSrc[1], srcStep[1], 
                               pSrc[2], srcStep[2], 
                               pDst, dstStep<<1, newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB_8u_P3C3 )( const Fw8u *const pSrc[3], Fw8u *pDst, FwiSize imgSize )
{
    // Assumptions:
    //     1) For every U and V, there are exactly 4 Y channels.
    //     2) Step size determined for source buffer is step size times 3, for the destination buffer.
    
    // When processing the whole image, ( image width * sizeof(Fw8u) ) is the step size of the 3 plane destination buffers.
    // For the 420 sampled source buffers, the step size of the Y channel is ( image width * sizeof(Fw8u) ), and 
    // the U and V channels' step size are half of the Y channel step size.
    
    int step[3] = { imgSize.width, imgSize.width>>1, imgSize.width>>1};
    return fwiYUV420ToRGB_8u_P3C3R (pSrc, step, pDst, step[0]*3, imgSize);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB_8u_P3AC4R  )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel< 
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_AC4,    Fw8u, AC4, RGB_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

//----------------------------
// YUV420 To BGR
//  These functions decompress from 4:2:0 YUV to BGR
//  Every 2x2 pixel set of Y and 1 pixel of UV correspond to 2x2 BGR
//----------------------------

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToBGR_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                              Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    // Vertical Step 2, YUV420 planar by 2*1 bytes, Increment RGB by 3*2 bytes
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<  
                                                             CMC::CH_P3_420, Fw8u, C1, 
                                                             CMC::CH_C3,     Fw8u, C3, BGR_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

//----------------------------
// YUV420 To RGBXXX ---
//  These functions decompress from 4:2:0 YUV to RGBXXX
//  Every 2x2 pixel set of Y and 1 pixel of UV correspond to 2x2 RGBXXX
//----------------------------

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB565_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, RGB565_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);

    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB555_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<  
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, RGB555_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);

    /// This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToRGB444_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, RGB444_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

//----------------------------
// YUV420 To BGRXXX
//  These functions decompress from 4:2:0 YUV to BGRXXX
//  Every 2x2 pixel set of Y and 1 pixel of UV correspond to 2x2 BGRXXX
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToBGR565_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{

    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, BGR565_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToBGR555_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, BGR555_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYUV420ToBGR444_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                    Fw16u *pDst,               int dstStep, 
                                                                    FwiSize roiSize )
{
    typedef YUVToRGB_DEF::Src420_To_Dst444::PlanarToChannel<
                                                             CMC::CH_P3_420, Fw8u,  C1, 
                                                             CMC::CH_C3,     Fw16u, C1, BGR444_FORMAT
                                                           > FeType;
    FeType data(srcStep, dstStep, roiSize);
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels 
    // during each iteration.
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, 
                         pSrc[0], srcStep[0]<<1,
                         pSrc[1], srcStep[1],
                         pSrc[2], srcStep[2],
                         pDst,    dstStep<<1,
                         newRoiSize );
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
