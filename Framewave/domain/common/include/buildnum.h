/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

// To manage fw's build number.

#ifndef __BUILDNUM_H__
#define __BUILDNUM_H__

#define FW_MAJOR   1
#define FW_MINOR   3
#define FW_VERSION 1.3
#define FW_VERSTR "FW 1.3" // Framewave version 1.2 release


//---------------------------------------
//    FW Version Numbers and build number
//---------------------------------------
// 100  - Public Release 1.0
// 101  - Public Release 1.1
//        Erode3x3
//        Dilate3x3
// 102  - JPEG

// 9999 - Disabled



#define FW_BUILD_NUM  102
#define BUILD_NUM_AT_LEAST( arg ) FW_BUILD_NUM >= arg

#endif // __BUILDNUM_H__
