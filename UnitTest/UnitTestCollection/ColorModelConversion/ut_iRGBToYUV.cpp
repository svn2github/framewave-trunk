/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "CMC_Tests.h"

#include "CMC_FunctionObjects.h"
#include "FunctionSignatures.h"
#include "fwImage.h"

////////////////////////////////////////////////////////////////
//                        RGBToYUV
////////////////////////////////////////////////////////////////
class iRGBToYUV_8u_C3R : public SrcDstRoi< U8, 3, U8, 3 >
{
public:
    iRGBToYUV_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U8, 3 >( parent, "iRGBToYUV_8u_C3R", fwiRGBToYUV_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0   0   0  0   0 MAX MAX MAX   0 MAX  0   0   0 MAX MAX", 
                 "255 128 128 0 128 128 29 239 102 225  16 153  76 90 255 178 165   0", 1 );
    }
};


class iRGBToYUV_8u_AC4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    iRGBToYUV_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U8, 4 >( parent, "iRGBToYUV_8u_AC4R", fwiRGBToYUV_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX   0   0   0   0   0   0   0 MAX   0 MAX MAX   0   0 MAX   0   0   0   0 MAX MAX   0", 
                 "255 128 128   0   0 128 128   0  29 239 102   0 225  16 153   0  76  90 255   0 178 165   0   0", 1 );
    }
};

class iRGBToYUV_8u_P3R : public PSrcPDstRoi< U8, 3, U8, 3 >
{
public:
    iRGBToYUV_8u_P3R( UnitTestCatalogBase & parent ) : PSrcPDstRoi<  U8, 3, U8, 3 >( parent, "iRGBToYUV_8u_P3R", fwiRGBToYUV_8u_P3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"MAX   0   0 MAX MAX   0",
                                 "MAX   0   0 MAX   0 MAX",
                                 "MAX   0 MAX   0   0 MAX"};
        const char *dstStr[3] = {"255   0  29 225  76 178",
                                 "128 128 239  16  90 165",
                                 "128 128 102 153 255   0"};

        RunTest( srcStr, 
                 dstStr, 1 );
    }
};

//class iRGBToYUV_8u_C3P3R : public SrcPDstRoi< U8, 3, U8, 3 >
//{
//public:
//    iRGBToYUV_8u_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 3, U8, 3 >( parent, "iRGBToYUV_8u_C3P3R", fwiRGBToYUV_8u_C3P3R )
//    {}
//
//    virtual void RunAll()
//    {
//        const char *dstStr[3] = {"255   0  29 225  76 178",
//                                 "128 128 239  16  90 165",
//                                 "128 128 102 153 255   0"};
//
//        RunTest( "MAX MAX MAX 0   0   0  0   0 MAX MAX MAX   0 MAX  0   0   0 MAX MAX", 
//                 dstStr, 1 );
//    }
//};

////////////////////////////////////////////////////////////////
//                        RGBToYUV422
////////////////////////////////////////////////////////////////
class iRGBToYUV422_8u_C3C2R : public SrcDstRoiEvenWidth< U8, 3, U8, 2 >
{
public:
    iRGBToYUV422_8u_C3C2R( UnitTestCatalogBase & parent ) : SrcDstRoiEvenWidth< U8, 3, U8, 2 >( parent, "iRGBToYUV422_8u_C3C2R", fwiRGBToYUV422_8u_C3C2R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX   0   0   0   0   0 MAX MAX MAX   0 MAX  0   0   0 MAX MAX", 
                 "254 128   0 128  29 128 225 128  76 128 178 128", 1 );
    }
};
//
//class iRGBToYUV422_8u_P3R : public PSrcPDstRoiMultipleDstStepEvenWidth< U8, 3, U8, 3 >
//{
//public:
//    iRGBToYUV422_8u_P3R( UnitTestCatalogBase & parent ) : PSrcPDstRoiMultipleDstStepEvenWidth< U8, 3, U8, 3 >( parent, "iRGBToYUV422_8u_P3R", fwiRGBToYUV422_8u_P3R )
//    {}
//
//    virtual void RunAll()
//    {
//        const char *srcStr[3] = {"MAX   0   0 MAX MAX   0",
//                                 "MAX   0   0 MAX   0 MAX",
//                                 "MAX   0 MAX   0   0 MAX"};
//        const char *dstStr[3] = {"254   0  29 225 76 178",
//                                 "128 128 128",
//                                 "128 128 128"};
//        RunTest( srcStr, 
//                 dstStr, 1 );
//    }
//};


//////////////////////////
// TEST TABLE DEFINITION
//////////////////////////
DEFINE_TEST_TABLE( iRGBToYUV_Chapter )
TEST_ENTRY( iRGBToYUV_8u_C3R )
TEST_ENTRY( iRGBToYUV_8u_AC4R )
TEST_ENTRY( iRGBToYUV_8u_P3R )
//TEST_ENTRY( iRGBToYUV_8u_C3P3R )
TEST_ENTRY( iRGBToYUV422_8u_C3C2R )
//TEST_ENTRY( iRGBToYUV422_8u_P3R )

END_TEST_TABLE()

