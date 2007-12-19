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
// NON-CHROMA-SAMPLING Functions
/////////////////////////////////

//----------------------------
// RGBToXYZ
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::ChannelToChannel< CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToXYZ_8u8u, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_PLU_PLU, CMC::PLU_PLU_PLU, 
                                                CMC::RGBToXYZ, CMC::PROC_NORMAL                      > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToXYZ_8u8u, 
                                                CMC::PLU_PLU_PLU, CMC::PLU_PLU_PLU, CMC::PLU_PLU_PLU, 
                                                CMC::RGBToXYZ, CMC::PROC_NORMAL                      > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, RGBToXYZ_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, RGBToXYZ_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, RGBToXYZ_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, RGBToXYZ_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, RGBToXYZ_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToXYZ_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, RGBToXYZ_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


//----------------------------
// XYZToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel< CMC::CH_C3, Fw8u, C3, Fw8u, C3, XYZToRGB_8u8u, 
                                                CMC::PLU_MIN_MIN, CMC::MIN_PLU_PLU, CMC::PLU_MIN_PLU, 
                                                CMC::XYZToRGB, CMC::PROC_SHIFT13                 > FeType; 
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel< CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, XYZToRGB_8u8u, 
                                                CMC::PLU_MIN_MIN, CMC::MIN_PLU_PLU, CMC::PLU_MIN_PLU, 
                                                CMC::XYZToRGB, CMC::PROC_SHIFT13                 > FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, XYZToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, XYZToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, XYZToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, XYZToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, XYZToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiXYZToRGB_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, XYZToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}



//----------------------------
// RGBToLUV
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToLUV_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToLUV_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, RGBToLUV_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, RGBToLUV_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, RGBToLUV_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, RGBToLUV_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_32f_C3R  )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, RGBToLUV_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToLUV_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, RGBToLUV_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


//----------------------------
// LUVToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, LUVToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, LUVToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, LUVToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, LUVToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, LUVToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, LUVToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, LUVToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLUVToRGB_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, LUVToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


//----------------------------
// BGRToLAB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToLab_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, BGRToLab_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToLab_8u16u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                            Fw16u *pDst,      int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw16u, C3, BGRToLab_8u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// LABToBGR
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiLabToBGR_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, LabToBGR_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiLabToBGR_16u8u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                            Fw8u *pDst,        int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw8u, C3, LabToBGR_16u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


//----------------------------
// RGBToYCC
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToYCC_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToYCC_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, RGBToYCC_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, RGBToYCC_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, RGBToYCC_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, RGBToYCC_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, RGBToYCC_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToYCC_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, RGBToYCC_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// YCCToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, YCCToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, YCCToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, YCCToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, YCCToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, YCCToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, YCCToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, YCCToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiYCCToRGB_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, YCCToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// RGBToHLS
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToHLS_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToHLS_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, RGBToHLS_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, RGBToHLS_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, RGBToHLS_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );    
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, RGBToHLS_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, RGBToHLS_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHLS_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, RGBToHLS_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// HLSToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, HLSToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, HLSToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_16s_C3R )( const Fw16s *pSrc, int srcStep, 
                                                          Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16s, C3, Fw16s, C3, HLSToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_16s_AC4R )( const Fw16s *pSrc, int srcStep, 
                                                           Fw16s *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16s, AC4, Fw16s, AC4, HLSToRGB_16s16s> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, HLSToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );    
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, HLSToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_32f_C3R )( const Fw32f *pSrc, int srcStep, 
                                                          Fw32f *pDst,       int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw32f, C3, Fw32f, C3, HLSToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToRGB_32f_AC4R )( const Fw32f *pSrc, int srcStep, 
                                                           Fw32f *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw32f, AC4, Fw32f, AC4, HLSToRGB_32f32f> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// BGRToHLS
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize ) 
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, BGRToHLS_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_C3P3R )( const Fw8u *pSrc,    int srcStep, 
                                                           Fw8u *const pDst[3], int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR incr by 3*1 bytes, HLS planar by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 3, 1, 
                                      CMC::CH_C3, CMC::CH_P3, Fw8u, Fw8u, BGRToHLS
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_AC4P4R )( const Fw8u *pSrc,    int srcStep, 
                                                            Fw8u *const pDst[4], int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 4*1 bytes, HLS Planar incr by 4*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 4, 1, 
                                      CMC::CH_AC4, CMC::CH_AP4, Fw8u, Fw8u, BGRToHLS
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                           Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    typedef NoChromaSampling::PlanarToChannel_REF<CMC::CH_P3C3, Fw8u, C1, Fw8u, C3, BGRToHLS_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_AP4C4R )( const Fw8u *const pSrc[4], int srcStep, 
                                                            Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 4*1 bytes
    return CMC_SS::Model::ConvertREF<  
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 1, 4, 
                                      CMC::CH_AP4, CMC::CH_AC4, Fw8u, Fw8u, BGRToHLS
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                          Fw8u *const pDst[3],      int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 1, 1, 1, 
                                      CMC::CH_P3, CMC::CH_P3, Fw8u, Fw8u, BGRToHLS
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiBGRToHLS_8u_AP4R )( const Fw8u *const pSrc[4], int srcStep, 
                                                          Fw8u *const pDst[4],       int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 1, 1, 1, 
                                      CMC::CH_AP4, CMC::CH_AP4, Fw8u, Fw8u, BGRToHLS
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

//----------------------------
// HLSToBGR
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_C3P3R )( const Fw8u *pSrc,    int srcStep, 
                                                           Fw8u *const pDst[3], int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR incr by 3*1 bytes, HLS planar by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 3, 1, 
                                      CMC::CH_C3, CMC::CH_P3, Fw8u, Fw8u, HLSToBGR
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_AC4P4R )( const Fw8u *pSrc, int srcStep, 
                                                            Fw8u *const pDst[4], int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 4*1 bytes, HLS Planar incr by 4*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_CHANNEL, CMC_SS::STEP_PLANAR, 1, 4, 1, 
                                      CMC::CH_AC4, CMC::CH_AP4, Fw8u, Fw8u, HLSToBGR
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_AP4R )( const Fw8u *const pSrc[4], int srcStep, 
                                                          Fw8u *const pDst[4],       int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 1, 1, 1, 
                                      CMC::CH_AP4, CMC::CH_AP4, Fw8u, Fw8u, HLSToBGR
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_P3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                         Fw8u *const pDst[3],       int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 1*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_PLANAR, 1, 1, 1, 
                                      CMC::CH_P3, CMC::CH_P3, Fw8u, Fw8u, HLSToBGR
                                    > (pSrc, srcStep, const_cast<Fw8u**>(pDst), dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_AP4C4R )( const Fw8u *const pSrc[4], int srcStep, 
                                                            Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS Planar incr by 4*1 bytes
    return CMC_SS::Model::ConvertREF<
                                      CMC_SS::STEP_PLANAR, CMC_SS::STEP_CHANNEL, 1, 1, 4, 
                                      CMC::CH_AP4, CMC::CH_AC4, Fw8u, Fw8u, HLSToBGR
                                    > (pSrc, srcStep, pDst, dstStep, roiSize, 0);
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHLSToBGR_8u_P3C3R )( const Fw8u *const pSrc[3], int srcStep, 
                                                           Fw8u *pDst,                int dstStep, FwiSize roiSize )
{
    // Vertical Step 1, BGR planar by 1*1 bytes, HLS incr by 3*1 bytes
    typedef NoChromaSampling::PlanarToChannel_REF<CMC::CH_P3C3, Fw8u, C1, Fw8u, C3, HLSToBGR_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc[0], srcStep, pSrc[1], srcStep, pSrc[2], srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// RGBToHSV
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHSV_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize )        
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, RGBToHSV_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHSV_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, RGBToHSV_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHSV_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, RGBToHSV_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );    
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiRGBToHSV_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, RGBToHSV_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

//----------------------------
// HSVToRGB
//----------------------------
FwStatus PREFIX_OPT( OPT_PREFIX, fwiHSVToRGB_8u_C3R )( const Fw8u *pSrc, int srcStep, 
                                                         Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw8u, C3, Fw8u, C3, HSVToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHSVToRGB_8u_AC4R )( const Fw8u *pSrc, int srcStep, 
                                                          Fw8u *pDst,       int dstStep, FwiSize roiSize )    
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw8u, AC4, Fw8u, AC4, HSVToRGB_8u8u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHSVToRGB_16u_C3R )( const Fw16u *pSrc, int srcStep, 
                                                          Fw16u *pDst,       int dstStep, FwiSize roiSize )
{ 
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_C3, Fw16u, C3, Fw16u, C3, HSVToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );    
}

FwStatus PREFIX_OPT( OPT_PREFIX, fwiHSVToRGB_16u_AC4R )( const Fw16u *pSrc, int srcStep, 
                                                           Fw16u *pDst,       int dstStep, FwiSize roiSize )    
{
    typedef NoChromaSampling::ChannelToChannel_REF<CMC::CH_AC4, Fw16u, AC4, Fw16u, AC4, HSVToRGB_16u16u> FeType;
    FeType data;
    return fe< FeType >( data, pSrc, srcStep, pDst, dstStep, roiSize );
}


// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 OF10H 
