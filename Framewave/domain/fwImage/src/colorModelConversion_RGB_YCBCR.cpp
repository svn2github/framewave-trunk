/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "colorModelConversion.h"
#include "colorModelConvSpecial.h"
#include "colorModelConvSpecialDef.h"
#include "colorModelConversionDefs.h"
#include "color.h"
#include "fwImage.h"
#include "fe.h"

using namespace OPT_LEVEL;

// This file contains all function definitions that involve color model conversion between RGB and YCbCr


/////////////////////////////////////////////////
// NON-CHROMA-SAMPLING RGB <--> YCbCr Functions
/////////////////////////////////////////////////

//----------------------------
// RGBToYCbCr
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                 Fw8u *pDst, int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToYCbCr, 
                                                CMC::PLU_PLU_PLU,  CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYCbCr, CMC::PROC_NORMAL                
                                              > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                                  Fw8u *pDst, int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToYCbCr, 
                                                CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYCbCr, CMC::PROC_NORMAL
                                              > FeType;

    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                                 Fw8u *const pDst[3], int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::Planar3ToPlanar3< 
                                                CMC::CH_P3, Fw8u, C1, Fw8u, C1, RGBToYCbCr, 
                                                CMC::PLU_PLU_PLU, CMC::MIN_MIN_PLU, CMC::PLU_MIN_MIN, 
                                                CMC::RGBToYCbCr, CMC::PROC_NORMAL
                                              > FeType;
    FeType data;
    return fec3S3D< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, 
                                    pDst[0], dstStep, pDst[1], dstStep, pDst[2], dstStep, roiSize );
}

//----------------------------
// YCbCrToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                 Fw8u *pDst, int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_C3, Fw8u, C3, Fw8u, C3, YCbCrToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YCbCrToRGB, CMC::PROC_SHIFT13
                                              > FeType;

    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                                  Fw8u *pDst, int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::ChannelToChannel< 
                                                CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, YCbCrToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YCbCrToRGB, CMC::PROC_SHIFT13
                                              > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize ); 
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB_8u_P3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                Fw8u *const pDst[3], int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::Planar3ToPlanar3< 
                                                CMC::CH_P3, Fw8u, C1, Fw8u, C1, YCbCrToRGB, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                                CMC::YCbCrToRGB, CMC::PROC_SHIFT13
                                              > FeType;
    FeType data;
    return fec3S3D< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, 
                                    pDst[0], dstStep, pDst[1], dstStep, pDst[2], dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB_8u_P3C3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                           Fw8u *pDst, int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::PlanarToChannel< 
                                               CMC::CH_P3C3, Fw8u, C1, Fw8u, C3, YCbCrToRGB, 
                                               CMC::PLU_PLU_PLU, CMC::PLU_MIN_MIN, CMC::PLU_PLU_PLU, 
                                               CMC::YCbCrToRGB, CMC::PROC_SHIFT13
                                             > FeType; 
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB565_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                      Fw16u *pDst, int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF< 
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToRGB565_8u16u 
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB555_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                      Fw16u *pDst, int dstStep, FwiSize roiSize )        
{ 
    typedef NoChromaSampling::ChannelToChannel_REF< 
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToRGB555_8u16u 
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB444_8u16u_C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst, int dstStep, FwiSize roiSize )        
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToRGB444_8u16u
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


///////////////////////////////
// CHROMA-SAMPLING Functions
///////////////////////////////

// ------------------
// YCbCr To RGBxxx
// ------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB565_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                  Fw16u *pDst,               int dstStep, 
                                                                  FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToRGB565_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                  Fw16u *pDst,               int dstStep, 
                                                                  FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToRGB555_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToRGB444_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                  Fw16u *pDst,               int dstStep, 
                                                                  FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToRGB444_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

// ------------------
// YCbCr To BGRxxx
// ------------------

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR565_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                 Fw16u *pDst,      int dstStep, 
                                                                 FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToBGR565_8u16u
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR555_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                 Fw16u *pDst,      int dstStep, 
                                                                 FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToBGR555_8u16u
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR444_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                                 Fw16u *pDst,      int dstStep, 
                                                                 FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<
                                                    CMC::CH_C3, Fw8u, C3, Fw16u, C1, YCbCrToBGR444_8u16u
                                                  > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR565_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                                   Fw16u *pDst,               int dstStep, 
                                                                   FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToBGR565_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep, 
                                                                  Fw16u *pDst,               int dstStep, 
                                                                  FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToBGR555_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCrToBGR444_8u16u_P3C3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                                   Fw16u *pDst,               int dstStep, 
                                                                   FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<
                                                   CMC::CH_P3C3, Fw8u, C1, Fw16u, C1, YCbCrToBGR444_8u16u
                                                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// RGB To YCbCr422
//  These functions compress RGB data into a 4:2:2 YCbCr
//  Every 2 pixels w/ 3 Channels of RGB are put into 2 pixel sets of 2 Channels of YCbCr
//  (R0 G0 B0 | R1 G1 B1 -> Y0 Cb0 | Y1 Cr0)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr422_8u_C3C2R )( const Fw8u *pSrc, int srcStep, 
                                                                Fw8u *pDst,       int dstStep, 
                                                                FwiSize roiSize )
{
    // Vertical Step = 1 byte, Increment RGB by 3*2 bytes, Increment YCbCr422 by 2*2 bytes
    return CMC_SS::Model::ConvertREF< 
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 6, 4, 
                                      CMC::CH_C3, CMC::CH_C2_422_ABAC, Fw8u, Fw8u, RGBToYCbCr          
                                    >( pSrc, srcStep, pDst, dstStep, roiSize, 0 );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr422_8u_P3C2R)( const Fw8u *const pSrc[3], int srcStep, 
                                                               Fw8u *pDst,                int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 1 byte, Increment RGB planes by 2*1 bytes, Increment YCbCr422 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF< 
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3, CMC::CH_C2_422_ABAC, Fw8u, Fw8u, RGBToYCbCr          
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}


//----------------------------
// YCbCr422 To RGB
//  These functions decompress 4:2:2 YCbCr to RGB data
//  Every 2 pixel set of YCbCr correspond to an RGB pair
//  (Y0 Cb0 | Y1 Cr0 -> R0 G0 B0 | R1 G1 B1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB_8u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                               Fw8u *pDst,       int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 planes by 2*2 bytes, Increment RGB by 3byte/px*2px=6 bytes
    return CMC_SS::Model::ConvertREF< 
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 6, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw8u, YCbCrToRGB
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB_8u_C2P3R)( const Fw8u *pSrc,    int srcStep, 
                                                               Fw8u *const pDst[3], int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 planes by 2*2 bytes, Increment RGB by 1byte/px*2px=2 bytes
    return CMC_SS::Model::ConvertREF< 
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 4, 2, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_P3, Fw8u, Fw8u, YCbCrToRGB
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB_8u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 planes by 1*2 bytes, Increment RGB by 3byte/px*2px=6 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 6, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw8u, YCbCrToRGB
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

//----------------------------
// RGB To CbYCr422
//  These functions compress RGB data into a 4:2:2 CbYCr
//  Every 2 pixels w/ 3 Channels of RGB are put into 2 pixel sets of 2 Channels of CbYCr
//  (R0 G0 B0 | R1 G1 B1 -> Cb0 Y0 | Cr0 Y1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToCbYCr422_8u_C3C2R)( const Fw8u *pSrc, int srcStep, 
                                                               Fw8u *pDst,       int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 1, Increment RGB by 3*2 bytes, Increment CbYCr422 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 6, 4, 
                                      CMC::CH_C3, CMC::CH_C2_422_BACA, Fw8u, Fw8u, RGBToYCbCr
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToCbYCr422Gamma_8u_C3C2R)( const Fw8u *pSrc, int srcStep, 
                                                                    Fw8u *pDst,       int dstStep, 
                                                                    FwiSize roiSize )
{
    // Vertical Step = 1, Increment RGB by 3*2 bytes, Increment CbYCr422 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 6, 4, 
                                      CMC::CH_C3, CMC::CH_C2_422_BACA, Fw8u, Fw8u, RGBToYCbCrGamma
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

//----------------------------
// CbYCr422 To RGB
//  These functions decompress 4:2:2 CbYCr to RGB data
//  Every 2 pixel set of CbYCr correspond to an RGB pair
//  (Cb0 Y0 | Cr0 Y1 -> R0 G0 B0 | R1 G1 B1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiCbYCr422ToRGB_8u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                               Fw8u *pDst,       int dstStep, FwiSize roiSize )
{
    // Vertical Step = 1, Increment CbYCr422 by 2*2 bytes, Increment RGB by 3byte/px*2px=6 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 6, 
                                      CMC::CH_C2_422_BACA, CMC::CH_C3, Fw8u, Fw8u, YCbCrToRGB
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

//----------------------------
// BGR (4-channel) To CbYCr422
//  These functions compress BGR data into a 4:2:2 CbYCr
//  Every 2 pixels w/ 3 Channels of BGR are put into 2 pixel sets of 2 Channels of CbYCr
//  (B0 G0 R0 A0 | B1 G1 R1 A1 -> Cb0 Y0 | Cr0 Y1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToCbYCr422_8u_AC4C2R)( const Fw8u *pSrc, int srcStep, 
                                                                Fw8u *pDst,       int dstStep, 
                                                                FwiSize roiSize )
{
    // Vertical Step = 1, Increment RGB by 4*2 bytes, Increment CbYCr422 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 8, 4, 
                                      CMC::CH_AC4, CMC::CH_C2_422_BACA, Fw8u, Fw8u, BGRToYCbCr
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

//----------------------------
// CbYCr422 To BGR (4-channel)
//  These functions decompress from BGR data into a 4:2:2 CbYCr
//  2 pixel sets of 2 Channels of CbYCr are put into 2 pixels w/ 3 Channels of BGR
//  (Cb0 Y0 | Cr0 Y1 -> B0 G0 R0 A0 | B1 G1 R1 A1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiCbYCr422ToBGR_8u_C2C4R)( const Fw8u *pSrc, int srcStep, 
                                                               Fw8u *pDst,       int dstStep, 
                                                               FwiSize roiSize,  Fw8u alpha )
{
    // Vertical Step = 1, Increment CbYCr422 by 2*2 bytes, Increment RGB by 4byte/px*2px=8 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 8, 
                                      CMC::CH_C2_422_BACA, CMC::CH_AC4, Fw8u, Fw8u, YCbCrToBGR
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, alpha);
}

//----------------------------
// YCbCr422 To RGBXXX
//    These functions decompress 4:2:2 YCbCr and pack to RGBXXX data
//    Every 2 pixel set of YCbCr correspond to an RGB pair
//    (Y0 Cb0 | Y1 Cr0 -> R0 G0 B0 | R1 G1 B1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB565_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment RGB565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB555_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment RGB555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB444_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment RGB444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr422 To RGBXXX Planar ---
//   These functions decompress Planar 4:2:2 YCbCr  and pack to RGBXXX data
//   Every 2 pixel set of YCbCr correspond to an RGB pair
//   (Y0 Cb0 | Y1 Cr0 -> R0 G0 B0 | R1 G1 B1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB565_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment RGB565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment RGB555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToRGB444_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment RGB444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr422 To BGRXXX ---
//    These functions decompress 4:2:2 YCbCr and pack to BGRXXX data
//    Every 2 pixel set of YCbCr correspond to an BGR pair
//    (Y0 Cb0 | Y1 Cr0 -> B0 G0 R0 | B1 G1 R1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR565_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment BGR565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR555_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment BGR555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR444_8u16u_C2C3R)( const Fw8u *pSrc, int srcStep, 
                                                                     Fw16u *pDst,      int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*2 bytes, Increment BGR444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_CHANNEL, 1, 4, 4, 
                                      CMC::CH_C2_422_ABAC, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr422 To BGRXXX Planar
//    These functions decompress Planar 4:2:2 YCbCr  and pack to BGRXXX data
//    Every 2 pixel set of YCbCr correspond to an BGR pair
//    (Y0 Cb0 | Y1 Cr0 -> B0 G0 R0 | B1 G1 R1)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR565_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment BGR565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment BGR555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr422ToBGR444_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr422 by 2*1 bytes, Increment BGR444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 2, 4, 
                                      CMC::CH_P3_422, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr420 To RGBXXX 
//    These functions decompress from 4:2:0 YCbCr to RGBXXX
//    Every 2x2 pixel set of Y and 1 pixel of CbCr correspond to 2x2 RGBXXX
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToRGB565_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment RGB565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToRGB555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment RGB555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToRGB444_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment RGB444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToRGB444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr420 To BGR
//    These functions decompress from 4:2:0 YCbCr to BGR
//    Every 2x2 pixel set of Y and 1 pixel of YCbCr correspond to 2x2 BGR
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToBGR_8u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep, 
                                                               FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment RGB by 3byte/px*2px=6 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 6, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw8u, YCbCrToBGR
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr420 To BGRXXX
//    These functions decompress from 4:2:0 YCbCr to BGRXXX
//    Every 2x2 pixel set of Y and 1 pixel of CbCr correspond to 2x2 BGRXXX
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToBGR565_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment BGR565 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR565
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToBGR555_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment BGR555 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR555
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToBGR444_8u16u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                                     Fw16u *pDst,               int dstStep, 
                                                                     FwiSize roiSize )
{
    // Vertical Step = 2, Increment YCbCr420 by 2*1 bytes, Increment BGR444 by 2byte/px*2px=4 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 2, 2, 4, 
                                      CMC::CH_P3_420, CMC::CH_C3, Fw8u, Fw16u, YCbCrToBGR444
                                    > (pSrc, srcStep, (Fw8u *)pDst, dstStep, roiSize, 0);
}

//----------------------------
// YCbCr411 To BGR 
//    These functions decompress from 4:2:2 YCbCr to BGR
//    Every 4 pixels of Y and 1 pixel of CbCr correspond to a 4 pixel BGR set
//    ([Y0 Y1 Y2 Y3] [U0] [V0] -> B0 G0 R0 | B1 G1 R1 | B2 G2 R2 | B3 G3 R3)
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr411ToBGR_8u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep,   
                                                               FwiSize roiSize )
{
    // Vertical Step = 1, Increment YCbCr411 by 1*4 bytes, Increment BGR by 3byte/px*4px=12 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 4, 12, 
                                      CMC::CH_P3_411, CMC::CH_C3, Fw8u, Fw8u, YCbCrToBGR
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr411ToBGR_8u_P3C4R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep, 
                                                               FwiSize roiSize,           Fw8u alpha )
{
    // Vertical Step = 1, Increment YCbCr411 by 1*4 bytes, Increment BGR by 4byte/px*4px=12 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 4, 16, 
                                      CMC::CH_P3_411, CMC::CH_AC4, Fw8u, Fw8u, YCbCrToBGR
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, alpha);
}



//----------------------------
// RGB To YCbCr420
//    These functions compress RGB data into a 4:2:0 YCbCr
//    Every 2x2 RGB pixel correspond to a 2x2 set of Y and 1 pixel of CbCr
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCbCr420_8u_C3P3R)( const Fw8u *pSrc,    int srcStep, 
                                                               Fw8u *const pDst[3], int dstStep[3], FwiSize roiSize )
{
    CMC_CHECK_PARAMS;
    if ( !dstStep[0] || !dstStep[1] || !dstStep[2] ) return fwStsStepErr;
    if ( !pDst[0]    || !pDst[1]    || !pDst[2]    ) return fwStsNullPtrErr;

    return ConvertRGBToYCbCr420_8u_C3P3R_REF_2D (pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// YCbCr420 To RGB
//    These functions decompress from 4:2:0 YCbCr to RGB
//    Every 2x2 pixel set of Y and 1 pixel of CbCr correspond to 2x2 RGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCbCr420ToRGB_8u_P3C3R)( const Fw8u *const pSrc[3], int srcStep[3], 
                                                               Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef YCbCrToRGB_DEF::Src420_To_Dst444::PlanarToChannel<  CMC::CH_P3_420, Fw8u, C1, 
                                                                CMC::CH_C3,     Fw8u, C3, RGB_FORMAT> FeType;
    
    FeType data( srcStep, dstStep, roiSize );
    
    // This function processes two rows of the image at a time. The ROI passed into FE needs to be changed so that FE 
    // thinks that the ROI is (roiSize.width*2, floor(roiSize.height/2)) rather than the ROI passed in. By changing the ROI,
    // the appropriate step sizes need to be doubled. By manipulating the ROI and step size, FE skips a row of pixels. 
    FwiSize newRoiSize = { roiSize.width<<1, roiSize.height>>1 };
    return fe< FeType >( data, pSrc[0], srcStep[0]<<1, 
                               pSrc[1], srcStep[1], 
                               pSrc[2], srcStep[2], 
                               pDst,    dstStep<<1, newRoiSize );

}


//----------------------------
// BGR To YCbCr420
//    These functions compress BGR data into a 4:2:0 YCbCr
//    Every 2x2 BGR pixel correspond to a 2x2 set of Y and 1 pixel of CbCr
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToYCbCr420_8u_C3P3R )( const Fw8u *pSrc,    int srcStep, 
                                                                Fw8u *const pDst[3], int dstStep[3], FwiSize roiSize )
{
    CMC_CHECK_PARAMS;
    if ( !dstStep[0] || !dstStep[1] || !dstStep[2] ) return fwStsStepErr;
    if ( !pDst[0]    || !pDst[1]    || !pDst[2]    ) return fwStsNullPtrErr;

    return ConvertBGRToYCbCr420_8u_C3P3R_REF_2D (pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToYCbCr420_8u_AC4P3R)( const Fw8u *pSrc,    int srcStep, 
                                                                Fw8u *const pDst[3], int dstStep[3], FwiSize roiSize )
{
    CMC_CHECK_PARAMS;
    if ( !dstStep[0] || !dstStep[1] || !dstStep[2] ) return fwStsStepErr;
    if ( !pDst[0]    || !pDst[1]    || !pDst[2]    ) return fwStsNullPtrErr;

    return ConvertBGRToYCbCr420_8u_AC4P3R_REF_2D (pSrc, srcStep, pDst, dstStep, roiSize );
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
