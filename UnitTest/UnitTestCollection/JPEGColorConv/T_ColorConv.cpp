/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "JPEGTest.h"

#include "FunctionSignatures.h"
#include "fwJPEG.h"

//class TestRGBToY_JPEG_8u_P3C1R : public PSrcDstRoi< U8, 3, U8, 1 >
//{
//
//public:
//    TestRGBToY_JPEG_8u_P3C1R( UnitTestCatalogBase & parent ) : 
//	  PSrcDstRoi< U8, 3, U8, 1 >( parent, "fwiRGBToY_JPEG_8u_P3C1R", fwiRGBToY_JPEG_8u_P3C1R )
//    {}
//
//    virtual void RunAll()
//    {
//        const char *srcStr[3] = {"9 45 6","1 4 7","3 5 8"};
//        RunTest( srcStr, "0 0 0");
//    }
//};


class TestRGBToY_JPEG_8u_P3C1R : public PSrcDstRoi< U8, 3, U8, 1>
{
public:
    TestRGBToY_JPEG_8u_P3C1R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 3, U8, 1>( parent, "fwiRGBToY_JPEG_8u_P3C1R", fwiRGBToY_JPEG_8u_P3C1R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"0 9 6","1 4 7","3 5 8"};
        RunTest( srcStr, "1 6 7" );
    }
};


class TestRGBToY_JPEG_8u_C3C1R : public SrcDstRoi< U8, 3, U8, 1 >
{

public:
    TestRGBToY_JPEG_8u_C3C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 3, U8, 1 >( parent, "fwiRGBToY_JPEG_8u_C3C1R", fwiRGBToY_JPEG_8u_C3C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "9 45 6 1 4 7 3 5 8", "30 3 5");
    }
};


class TestBGRToY_JPEG_8u_C3C1R : public SrcDstRoi< U8, 3, U8, 1 >
{

public:
    TestBGRToY_JPEG_8u_C3C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 3, U8, 1 >( parent, "fwiBGRToY_JPEG_8u_C3C1R", fwiBGRToY_JPEG_8u_C3C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "9 45 6 1 4 7 3 5 8", "29 5 6");
    }
};

class TestRGBToYCbCr_JPEG_8u_P3R : public PSrcPDstRoi< U8, 3, U8, 3>
{
public:
    TestRGBToYCbCr_JPEG_8u_P3R( UnitTestCatalogBase & parent ) : PSrcPDstRoi< U8, 3, U8, 3>( parent, "fwiRGBToYCbCr_JPEG_8u_P3R", fwiRGBToYCbCr_JPEG_8u_P3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"0 9 6","1 4 7","3 5 8"};
        const char *dstStr[3] = {"1 6 7","129 128 129","127 130 127"};
        RunTest( srcStr, dstStr );
    }
};

class TestRGBToYCbCr_JPEG_8u_C3P3R : public SrcPDstRoi< U8, 3, U8, 3>
{
public:
    TestRGBToYCbCr_JPEG_8u_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 3, U8, 3>( parent, "fwiRGBToYCbCr_JPEG_8u_C3P3R", fwiRGBToYCbCr_JPEG_8u_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr = {"0 9 6 1 4 7 3 5 8"};
        const char *dstStr[3] = {"6 3 5","128 130 130","124 126 127"};
        RunTest( srcStr, dstStr );
    }
};


class TestYCbCrToRGB_JPEG_8u_P3R : public PSrcPDstRoi< U8, 3, U8, 3>
{
public:
    TestYCbCrToRGB_JPEG_8u_P3R( UnitTestCatalogBase & parent ) : PSrcPDstRoi< U8, 3, U8, 3>( parent, "fwiYCbCrToRGB_JPEG_8u_P3R", fwiYCbCrToRGB_JPEG_8u_P3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"1 6 7","129 128 129","127 130 127"};
        const char *dstStr[3] = {"0 9 6","1 5 7","3 6 9"};
        RunTest( srcStr, dstStr );
    }
};

class TestYCbCrToRGB_JPEG_8u_P3C3R : public PSrcDstRoi< U8, 3, U8, 3>
{
public:
    TestYCbCrToRGB_JPEG_8u_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 3, U8, 3>( parent, "fwiYCbCrToRGB_JPEG_8u_P3C3R", fwiYCbCrToRGB_JPEG_8u_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"6 3 5","128 130 130","124 126 127"};
        const char *dstStr = {"0 8 5 0 3 5 3 5 7"};
        RunTest( srcStr, dstStr );
    }
};

class TestBGRToYCbCr_JPEG_8u_C3P3R : public SrcPDstRoi< U8, 3, U8, 3>
{
public:
    TestBGRToYCbCr_JPEG_8u_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 3, U8, 3>( parent, "fwiBGRToYCbCr_JPEG_8u_C3P3R", fwiBGRToYCbCr_JPEG_8u_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr = {"6 9 0 7 4 1 8 5 3"};
        const char *dstStr[3] = {"6 3 5","128 130 130","124 126 127"};
        RunTest( srcStr, dstStr );
    }
};


class TestYCbCrToBGR_JPEG_8u_P3C3R : public PSrcDstRoi< U8, 3, U8, 3>
{
public:
    TestYCbCrToBGR_JPEG_8u_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 3, U8, 3>( parent, "fwiYCbCrToBGR_JPEG_8u_P3C3R", fwiYCbCrToBGR_JPEG_8u_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"6 3 5","128 130 130","124 126 127"};
        const char *dstStr = {"6 9 0 7 4 0 9 5 4"};
        RunTest( srcStr, dstStr );
    }
};

class TestCMYKToYCCK_JPEG_8u_P4R : public PSrcPDstRoi< U8, 4, U8, 4>
{
public:
    TestCMYKToYCCK_JPEG_8u_P4R( UnitTestCatalogBase & parent ) : PSrcPDstRoi< U8, 4, U8, 4>( parent, "fwiCMYKToYCCK_JPEG_8u_P4R", fwiCMYKToYCCK_JPEG_8u_P4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"0 9 6","1 5 7","3 6 9", "1 7 4"};
        const char *dstStr[4] = {"254 249 248","127 128 127","129 126 129", "1 7 4"};
        RunTest( srcStr, dstStr );
    }
};

class TestCMYKToYCCK_JPEG_8u_C4P4R : public SrcPDstRoi< U8, 4, U8, 4>
{
public:
    TestCMYKToYCCK_JPEG_8u_C4P4R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 4, U8, 4>( parent, "fwiCMYKToYCCK_JPEG_8u_C4P4R", fwiCMYKToYCCK_JPEG_8u_C4P4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr = {"0 9 6 1 5 7 3 6 9 1 7 4"};
        const char *dstStr[4] = {"249 249 251","128 130 126","132 129 124", "1 6 4"};
        RunTest( srcStr, dstStr );
    }
};

class TestYCCKToCMYK_JPEG_8u_P4R : public PSrcPDstRoi< U8, 4, U8, 4>
{
public:
    TestYCCKToCMYK_JPEG_8u_P4R( UnitTestCatalogBase & parent ) : PSrcPDstRoi< U8, 4, U8, 4>( parent, "fwiYCCKToCMYK_JPEG_8u_P4R", fwiYCCKToCMYK_JPEG_8u_P4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"0 9 6","1 5 7","3 6 9", "1 7 4"};
        const char *dstStr[4] = {"255 255 255","122 117 122","255 255 255", "1 7 4"};
        RunTest( srcStr, dstStr );
    }
};

class TestYCCKToCMYK_JPEG_8u_P4C4R : public PSrcDstRoi< U8, 4, U8, 4>
{
public:
    TestYCCKToCMYK_JPEG_8u_P4C4R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 4, U8, 4>( parent, "fwiYCCKToCMYK_JPEG_8u_P4C4R", fwiYCCKToCMYK_JPEG_8u_P4C4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"249 249 251","128 130 126","132 129 124", "1 6 4"};
        const char *dstStr = {"1 9 7 1 5 8 4 6 10 1 9 4"};
        RunTest( srcStr, dstStr );
    }
};


DEFINE_TEST_TABLE( JPEGColorConvTestCatalog )

TEST_ENTRY( TestRGBToY_JPEG_8u_P3C1R )
TEST_ENTRY( TestRGBToY_JPEG_8u_C3C1R )
TEST_ENTRY( TestBGRToY_JPEG_8u_C3C1R )
TEST_ENTRY( TestRGBToYCbCr_JPEG_8u_P3R )
TEST_ENTRY( TestRGBToYCbCr_JPEG_8u_C3P3R )
TEST_ENTRY( TestYCbCrToRGB_JPEG_8u_P3R )
TEST_ENTRY( TestYCbCrToRGB_JPEG_8u_P3C3R )
TEST_ENTRY( TestBGRToYCbCr_JPEG_8u_C3P3R )
TEST_ENTRY( TestYCbCrToBGR_JPEG_8u_P3C3R )
TEST_ENTRY( TestCMYKToYCCK_JPEG_8u_P4R )
TEST_ENTRY( TestCMYKToYCCK_JPEG_8u_C4P4R )
TEST_ENTRY( TestYCCKToCMYK_JPEG_8u_P4R )
TEST_ENTRY( TestYCCKToCMYK_JPEG_8u_P4C4R )

END_TEST_TABLE()
