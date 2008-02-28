/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "fwBase.h"
#include "system.h"

// ColorModelConv Constants struct
// CMCDat is a structure of conversion constants that map the constants to the conversion function
struct CMCDat
{
    // FnID: Value = MulConst[*][0] * (x - SubConst[0]) + 
    //               MulConst[*][1] * (y - SubConst[1]) + 
    //               MulConst[*][2] * (z - SubConst[2]) + 
    //               AddConst[*];
    Fw32u FnID;
    Fw16u SubConst[3];
    Fw16u MulConst[3][3];
    Fw16u AddConst[3];
};

// ConvFunc is a enumeration of possible color model conversions. 
// This enum is used to extract the proper constant during compile time.
enum ConvFunc
{
    CF_RGBToYUV = 0,
    CF_YUVToRGB,
    CF_RGBToYCbCr,
    CF_YCbCrToRGB,
    CF_RGBToXYZ,
    CF_XYZToRGB,
    CF_RGBToLUV,
    CF_LUVToRGB,
    CF_BGRToLab,
    CF_LabToBGR,
    CF_RGBToYCC,
    CF_YCCToRGB,
    CF_RGBToHLS,
    CF_HLSToRGB,
    CF_BGRToHLS,
    CF_RGBToHSV,
    CF_HSVToRGB,
    CF_MAXConv
};

#endif // __CONSTANTS_H__
