/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __COLORMODELCONVCONST_H__
#define __COLORMODELCONVCONST_H__

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"

// --- Color Model Conversion Constants ---
extern const unsigned short int CONST_RGB2YUVsubR, CONST_RGB2YUVsubG, CONST_RGB2YUVsubB;
extern const double CONST_RGB2YUV_YR, CONST_RGB2YUV_YG, CONST_RGB2YUV_YB;
extern const double CONST_RGB2YUV_UR, CONST_RGB2YUV_UG, CONST_RGB2YUV_UB;
extern const double CONST_RGB2YUV_VR, CONST_RGB2YUV_VG, CONST_RGB2YUV_VB;
extern const unsigned short int CONST_RGB2YUVaddY, CONST_RGB2YUVaddU, CONST_RGB2YUVaddV;

extern const unsigned short int CONST_YUV2RGBsubY, CONST_YUV2RGBsubU, CONST_YUV2RGBsubV;
extern const double CONST_YUV2RGB_RY, CONST_YUV2RGB_RU, CONST_YUV2RGB_RV;
extern const double CONST_YUV2RGB_GY, CONST_YUV2RGB_GU, CONST_YUV2RGB_GV;
extern const double CONST_YUV2RGB_BY, CONST_YUV2RGB_BU, CONST_YUV2RGB_BV;
extern const unsigned short int CONST_YUV2RGBaddR, CONST_YUV2RGBaddG, CONST_YUV2RGBaddB;

extern const unsigned short int CONST_RGB2YCbCrsubR, CONST_RGB2YCbCrsubG, CONST_RGB2YCbCrsubB;
extern const double CONST_RGB2YCbCr_YR , CONST_RGB2YCbCr_YG , CONST_RGB2YCbCr_YB ;
extern const double CONST_RGB2YCbCr_CbR, CONST_RGB2YCbCr_CbG, CONST_RGB2YCbCr_CbB;
extern const double CONST_RGB2YCbCr_CrR, CONST_RGB2YCbCr_CrG, CONST_RGB2YCbCr_CrB;
extern const double CONST_RGB2YCbCraddY,     CONST_RGB2YCbCraddCb,   CONST_RGB2YCbCraddCr;

extern const unsigned short int CONST_YCbCr2RGBsubY, CONST_YCbCr2RGBsubCb, CONST_YCbCr2RGBsubCr;
extern const double CONST_YCbCr2RGB_RY, CONST_YCbCr2RGB_RCb, CONST_YCbCr2RGB_RCr;
extern const double CONST_YCbCr2RGB_GY, CONST_YCbCr2RGB_GCb, CONST_YCbCr2RGB_GCr;
extern const double CONST_YCbCr2RGB_BY, CONST_YCbCr2RGB_BCb, CONST_YCbCr2RGB_BCr;
extern const unsigned short int CONST_YCbCr2RGBaddR, CONST_YCbCr2RGBaddG, CONST_YCbCr2RGBaddB;

extern const unsigned short int CONST_RGB2XYZsubR, CONST_RGB2XYZsubG, CONST_RGB2XYZsubB;
extern const double CONST_RGB2XYZ_XR, CONST_RGB2XYZ_XG, CONST_RGB2XYZ_XB;
extern const double CONST_RGB2XYZ_YR, CONST_RGB2XYZ_YG, CONST_RGB2XYZ_YB;
extern const double CONST_RGB2XYZ_ZR, CONST_RGB2XYZ_ZG, CONST_RGB2XYZ_ZB;
extern const unsigned short int CONST_RGB2XYZaddX, CONST_RGB2XYZaddY, CONST_RGB2XYZaddZ;
extern const int CONST_RGB2XYZOffsetX, CONST_RGB2XYZOffsetY, CONST_RGB2XYZOffsetZ; // For 16s formats-Offset

extern const unsigned short int CONST_XYZ2RGBsubX, CONST_XYZ2RGBsubY, CONST_XYZ2RGBsubZ;
extern const double CONST_XYZ2RGB_RX, CONST_XYZ2RGB_RY, CONST_XYZ2RGB_RZ;
extern const double CONST_XYZ2RGB_GX, CONST_XYZ2RGB_GY, CONST_XYZ2RGB_GZ;
extern const double CONST_XYZ2RGB_BX, CONST_XYZ2RGB_BY, CONST_XYZ2RGB_BZ;
extern const unsigned short int CONST_XYZ2RGBaddR, CONST_XYZ2RGBaddG, CONST_XYZ2RGBaddB;
extern const int CONST_XYZ2RGBOffsetR, CONST_XYZ2RGBOffsetG, CONST_XYZ2RGBOffsetB; // For 16s formats

extern const double CONST_LUV_Lmin, CONST_LUV_Lmax;
extern const double CONST_LUV_Umin, CONST_LUV_Umax;
extern const double CONST_LUV_Vmin, CONST_LUV_Vmax;
extern const double CONST_LUV_Un,   CONST_LUV_Vn;

extern const double CONST_BGR2Lab_ftConst, CONST_BGR2Lab_ftCoeff;
extern const double CONST_BGR2Lab_k;
extern const double CONST_Lab_Xn, CONST_Lab_Yn, CONST_Lab_Zn;

extern const double CONST_RGB2YCC_YR , CONST_RGB2YCC_YG , CONST_RGB2YCC_YB ;
extern const double CONST_RGB2YCC_C1R, CONST_RGB2YCC_C1G, CONST_RGB2YCC_C1B;
extern const double CONST_RGB2YCC_C2R, CONST_RGB2YCC_C2G, CONST_RGB2YCC_C2B;
extern const double CONST_RGB2YCC_YConst, CONST_RGB2YCC_C1Const, 
                    CONST_RGB2YCC_C2Const;
extern const double CONST_YCC_C1Off, CONST_YCC_C2Off;

extern const double CONST_YCC2RGB_Y ;
extern const double CONST_YCC2RGB_C1, CONST_YCC2RGB_GC1;
extern const double CONST_YCC2RGB_C2, CONST_YCC2RGB_GC2;

#endif // __COLORMODELCONVCONST_H__
