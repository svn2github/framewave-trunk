#include "fwBase.h"
#include "system.h"

namespace Const
{
    extern const U32    FW_ALIGN  = 32;
    extern const double FW_PI     = 3.14159265358979323846;
    extern const double FW_2PI    = 2 * FW_PI;

    // Debug
    extern const U32 ImageDisplayOnPass   =  0;         // 0 -> disable.
    extern const U32 ImageDisplayOnFail   =  30 * 1000; // 0 -> disable.
           
    extern const double BayerDither44[4][4] = {{0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5}};
           
    extern const U32 NAN_32Fi      = 0x7FC00000;
    extern const U32 INF_32Fi      = 0x7F800000 ;
    extern const U32 INF_NEG_32Fi  = 0xFF800000 ;
    extern const U32 IND_32Fi      = 0xFFC00000 ;
           
    extern const U32 NAN_64Fi[2]     = {0x0,0x7FF80000};
    extern const U32 INF_64Fi[2]     = {0x0,0x7FF00000};
    extern const U32 INF_NEG_64Fi[2] = {0x0,0xFFF00000};
    extern const U32 IND_64Fi[2]     = {0x0,0xFFF80000};
           
           
    extern const double NAN_64F      = * ( double* ) &NAN_64Fi;
    extern const double INF_64F      = * ( double* ) &INF_64Fi;
    extern const double INF_NEG_64F  = * ( double* ) &INF_NEG_64Fi;
    extern const double IND_64F      = * ( double* ) &IND_64Fi;
           
    extern const float NAN_32F       = * ( float*  ) &NAN_32Fi;
    extern const float INF_32F       = * ( float*  ) &INF_32Fi;
    extern const float INF_NEG_32F   = * ( float*  ) &INF_NEG_32Fi;
    extern const float IND_32F       = * ( float*  ) &IND_32Fi;
};


// --- Color Model Conversion Constants ---
extern const unsigned short int CONST_RGB2YUVsubR = 0, CONST_RGB2YUVsubG = 0, CONST_RGB2YUVsubB = 0;
extern const double CONST_RGB2YUV_YR    =    0.299, CONST_RGB2YUV_YG = 0.587, CONST_RGB2YUV_YB = 0.114;
extern const double CONST_RGB2YUV_UR    =    0.147, CONST_RGB2YUV_UG = 0.289, CONST_RGB2YUV_UB = 0.436;
extern const double CONST_RGB2YUV_VR    =    0.615, CONST_RGB2YUV_VG = 0.515, CONST_RGB2YUV_VB = 0.100;
extern const unsigned short int CONST_RGB2YUVaddY = 0, CONST_RGB2YUVaddU = 128, CONST_RGB2YUVaddV = 128;

extern const unsigned short int CONST_YUV2RGBsubY = 0, CONST_YUV2RGBsubU = 128, CONST_YUV2RGBsubV = 128;
extern const double CONST_YUV2RGB_RY    =    1.000, CONST_YUV2RGB_RU = 0.000, CONST_YUV2RGB_RV = 1.140;
extern const double CONST_YUV2RGB_GY    =    1.000, CONST_YUV2RGB_GU = 0.394, CONST_YUV2RGB_GV = 0.581;
extern const double CONST_YUV2RGB_BY    =    1.000, CONST_YUV2RGB_BU = 2.032, CONST_YUV2RGB_BV = 0.000;
extern const unsigned short int CONST_YUV2RGBaddR = 0, CONST_YUV2RGBaddG = 0, CONST_YUV2RGBaddB = 0;

extern const unsigned short int CONST_RGB2YCbCrsubR = 0, CONST_RGB2YCbCrsubG = 0, CONST_RGB2YCbCrsubB  = 0;
extern const double CONST_RGB2YCbCr_YR  =    0.257, CONST_RGB2YCbCr_YG   = 0.504, CONST_RGB2YCbCr_YB  = 0.098;
extern const double CONST_RGB2YCbCr_CbR =    0.148, CONST_RGB2YCbCr_CbG  = 0.291, CONST_RGB2YCbCr_CbB = 0.439;
extern const double CONST_RGB2YCbCr_CrR =    0.439, CONST_RGB2YCbCr_CrG  = 0.368, CONST_RGB2YCbCr_CrB = 0.071;
extern const double CONST_RGB2YCbCraddY =   16,     CONST_RGB2YCbCraddCb = 128,   CONST_RGB2YCbCraddCr = 128;

extern const unsigned short int CONST_YCbCr2RGBsubY = 16, CONST_YCbCr2RGBsubCb = 128, CONST_YCbCr2RGBsubCr = 128;
extern const double CONST_YCbCr2RGB_RY  =    1.164, CONST_YCbCr2RGB_RCb  = 0.000, CONST_YCbCr2RGB_RCr = 1.596;
extern const double CONST_YCbCr2RGB_GY  =    1.164, CONST_YCbCr2RGB_GCb  = 0.392, CONST_YCbCr2RGB_GCr = 0.813;
extern const double CONST_YCbCr2RGB_BY  =    1.164, CONST_YCbCr2RGB_BCb  = 2.017, CONST_YCbCr2RGB_BCr = 0.000;
extern const unsigned short int CONST_YCbCr2RGBaddR = 0, CONST_YCbCr2RGBaddG = 0, CONST_YCbCr2RGBaddB = 0;

extern const unsigned short int CONST_RGB2XYZsubR = 0, CONST_RGB2XYZsubG = 0, CONST_RGB2XYZsubB = 0;
extern const double CONST_RGB2XYZ_XR    =    0.4124530, CONST_RGB2XYZ_XG = 0.357580,  CONST_RGB2XYZ_XB = 0.1804230;
extern const double CONST_RGB2XYZ_YR    =    0.2126710, CONST_RGB2XYZ_YG = 0.715160,  CONST_RGB2XYZ_YB = 0.0721690;
extern const double CONST_RGB2XYZ_ZR    =    0.0193340, CONST_RGB2XYZ_ZG = 0.1191930, CONST_RGB2XYZ_ZB = 0.9502270;
extern const unsigned short int CONST_RGB2XYZaddX = 0, CONST_RGB2XYZaddY = 0, CONST_RGB2XYZaddZ = 0;
extern const int CONST_RGB2XYZOffsetX = -1623, CONST_RGB2XYZOffsetY = 0, CONST_RGB2XYZOffsetZ = 2908; // For 16s formats-Offset

extern const unsigned short int CONST_XYZ2RGBsubX = 0, CONST_XYZ2RGBsubY = 0, CONST_XYZ2RGBsubZ = 0;
extern const double CONST_XYZ2RGB_RX    =    3.240479, CONST_XYZ2RGB_RY = 1.537150, CONST_XYZ2RGB_RZ = 0.498535;
extern const double CONST_XYZ2RGB_GX    =    0.969256, CONST_XYZ2RGB_GY = 1.875991, CONST_XYZ2RGB_GZ = 0.041556;
extern const double CONST_XYZ2RGB_BX    =    0.055648, CONST_XYZ2RGB_BY = 0.204043, CONST_XYZ2RGB_BZ = 1.057311;
extern const unsigned short int CONST_XYZ2RGBaddR = 0, CONST_XYZ2RGBaddG = 0, CONST_XYZ2RGBaddB = 0;
extern const int CONST_XYZ2RGBOffsetR = 6711, CONST_XYZ2RGBOffsetG = -1694, CONST_XYZ2RGBOffsetB = -2985; // For 16s formats

extern const double CONST_LUV_Lmin = 0.0,    CONST_LUV_Lmax = 100.0;
extern const double CONST_LUV_Umin = -134.0, CONST_LUV_Umax = 220.0;
extern const double CONST_LUV_Vmin = -140.0, CONST_LUV_Vmax = 116.0;//122.0;
extern const double CONST_LUV_Un = 0.197833036997, CONST_LUV_Vn = 0.468330474353;

extern const double CONST_BGR2Lab_ftConst = 0.008856, CONST_BGR2Lab_ftCoeff = 7.787;
extern const double CONST_BGR2Lab_k = 903.3;
extern const double CONST_Lab_Xn = 0.950455, CONST_Lab_Yn = 1.000000, CONST_Lab_Zn = 1.088754;

extern const double CONST_RGB2YCC_YR     =    0.299, CONST_RGB2YCC_YG = 0.587, CONST_RGB2YCC_YB = 0.114;
extern const double CONST_RGB2YCC_C1R    =    0.299, CONST_RGB2YCC_C1G = 0.587, CONST_RGB2YCC_C1B = 0.886;
extern const double CONST_RGB2YCC_C2R    =    0.701, CONST_RGB2YCC_C2G = 0.587, CONST_RGB2YCC_C2B = 0.114;
extern const double CONST_RGB2YCC_YConst = 1.000 / 1.402, CONST_RGB2YCC_C1Const = 111.4 / 255.0, 
                    CONST_RGB2YCC_C2Const = 135.64 / 255.0;
extern const double CONST_YCC_C1Off = 156.0 / 255.0, CONST_YCC_C2Off = 137.0 / 255.0;

extern const double CONST_YCC2RGB_Y     =    1.3584;
extern const double CONST_YCC2RGB_C1    =    2.2179, CONST_YCC2RGB_GC1 = 0.194;
extern const double CONST_YCC2RGB_C2    =    1.8215, CONST_YCC2RGB_GC2 = 0.509;
