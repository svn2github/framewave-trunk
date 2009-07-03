/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_GTVal_8u_C1IR : public SDstRoiThresholdSingleChannel_NoCmpOp_Val<U8>
{
public:
    Test_iThreshold_GTVal_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel_NoCmpOp_Val<U8>( parent, "fwiThreshold_GTVal_8u_C1IR", fwiThreshold_GTVal_8u_C1IR )
    {}

    virtual void RunAll()
    {
        U8 threshold;
        U8 value;

        threshold = 10; value = 16;
        RunTest( "0 5 10 15 20", "0 5 10 16 16", threshold, value );
    }
};

class Test_iThreshold_GTVal_8u_C1R : public SrcDstRoiThresholdSingleChannel_NoCmpOp_Val<U8, U8>
{
public:
    Test_iThreshold_GTVal_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel_NoCmpOp_Val<U8,U8>( parent, "fwiThreshold_GTVal_8u_C1R", fwiThreshold_GTVal_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 threshold;
        U8 value;

        threshold = 10; value = 16;
        RunTest( "0 5 10 15 20", "0 5 10 16 16", threshold, value );
    }
};

class Test_iThreshold_GTVal_8u_C3IR : public SDstRoiThresholdMultiChannel_NoCmpOp_Val<U8, 3, 3>
{
public:
    Test_iThreshold_GTVal_8u_C3IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdMultiChannel_NoCmpOp_Val<U8,3,3>( parent, "fwiThreshold_GTVal_8u_C3IR", fwiThreshold_GTVal_8u_C3IR )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        U8 value[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        value[0] = 25; value[1] = 9; value[2] = 51;
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 25 6 9 25 9 51", threshold, value );
    }
};

class Test_iThreshold_GTVal_8u_C3R : public SrcDstRoiThresholdMultiChannel_NoCmpOp_Val<U8, 3, U8, 3, 3>
{
public:
    Test_iThreshold_GTVal_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdMultiChannel_NoCmpOp_Val<U8,3,U8,3,3>( parent, "fwiThreshold_GTVal_8u_C3R", fwiThreshold_GTVal_8u_C3R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        U8 value[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        value[0] = 25; value[1] = 9; value[2] = 51;
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 25 6 9 25 9 51", threshold, value );
    }
};

class Test_iThreshold_GTVal_8u_AC4R : public SrcSDstRoiThresholdMultiChannel_NoCmpOp_Val<U8, 4, U8, 4, 3>
{
public:
    Test_iThreshold_GTVal_8u_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoiThresholdMultiChannel_NoCmpOp_Val<U8,4,U8,4,3>( parent, "fwiThreshold_GTVal_8u_AC4R", fwiThreshold_GTVal_8u_AC4R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        U8 value[3];

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15;
        value[0] = 25; value[1] = 9; value[2] = 51;
        RunTest( "0 1 2 0 3 6 9 0 17 50 89 0", "0 0 0 56 0 0 0 78 0 0 0 255", "0 1 2 56 25 6 9 78 25 9 51 255", threshold, value );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold_GTVal )

TEST_ENTRY( Test_iThreshold_GTVal_8u_C1IR )
TEST_ENTRY( Test_iThreshold_GTVal_8u_C1R )
TEST_ENTRY( Test_iThreshold_GTVal_8u_C3IR )
TEST_ENTRY( Test_iThreshold_GTVal_8u_C3R )
TEST_ENTRY( Test_iThreshold_GTVal_8u_AC4R )

END_TEST_TABLE()
