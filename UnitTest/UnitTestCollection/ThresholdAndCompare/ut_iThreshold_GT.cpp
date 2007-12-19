/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_GT_8u_C1IR : public SDstRoiThresholdSingleChannel_NoCmpOp<U8>
{
public:
    Test_iThreshold_GT_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel_NoCmpOp<U8>( parent, "fwiThreshold_GT_8u_C1IR", fwiThreshold_GT_8u_C1IR )
    {}

    virtual void RunAll()
    {
        U8 threshold;

        threshold = 10;
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold );
    }
};

class Test_iThreshold_GT_8u_C1R : public SrcDstRoiThresholdSingleChannel_NoCmpOp<U8, U8>
{
public:
    Test_iThreshold_GT_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel_NoCmpOp<U8,U8>( parent, "fwiThreshold_GT_8u_C1R", fwiThreshold_GT_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 threshold;

        threshold = 10;
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold );
    }
};

class Test_iThreshold_GT_8u_C3IR : public SDstRoiThresholdMultiChannel_NoCmpOp<U8, 3, 3>
{
public:
    Test_iThreshold_GT_8u_C3IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdMultiChannel_NoCmpOp<U8,3,3>( parent, "fwiThreshold_GT_8u_C3IR", fwiThreshold_GT_8u_C3IR )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 2 6 9 2 10 15", threshold );
    }
};

class Test_iThreshold_GT_8u_C3R : public SrcDstRoiThresholdMultiChannel_NoCmpOp<U8, 3, U8, 3, 3>
{
public:
    Test_iThreshold_GT_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdMultiChannel_NoCmpOp<U8,3,U8,3,3>( parent, "fwiThreshold_GT_8u_C3R", fwiThreshold_GT_8u_C3R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 2 6 9 2 10 15", threshold );
    }
};

class Test_iThreshold_GT_8u_AC4R : public SrcSDstRoiThresholdMultiChannel_NoCmpOp<U8, 4, U8, 4, 3>
{
public:
    Test_iThreshold_GT_8u_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoiThresholdMultiChannel_NoCmpOp<U8,4,U8,4,3>( parent, "fwiThreshold_GT_8u_AC4R", fwiThreshold_GT_8u_AC4R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        RunTest( "0 1 2 56 3 6 9 78 17 50 89 255", "0 0 0 56 0 0 0 78 0 0 0 255", "0 1 2 56 2 6 9 78 2 10 15 255", threshold );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold_GT )

TEST_ENTRY( Test_iThreshold_GT_8u_C1IR )
TEST_ENTRY( Test_iThreshold_GT_8u_C1R )
TEST_ENTRY( Test_iThreshold_GT_8u_C3IR )
TEST_ENTRY( Test_iThreshold_GT_8u_C3R )
TEST_ENTRY( Test_iThreshold_GT_8u_AC4R )

END_TEST_TABLE()
