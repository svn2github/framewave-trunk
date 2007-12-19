/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_LT_8u_C1IR : public SDstRoiThresholdSingleChannel_NoCmpOp<U8>
{
public:
    Test_iThreshold_LT_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel_NoCmpOp<U8>( parent, "fwiThreshold_LT_8u_C1IR", fwiThreshold_LT_8u_C1IR )
    {}

    virtual void RunAll()
    {
        U8 threshold;

        threshold = 10;
        RunTest( "0 5 10 15 20", "10 10 10 15 20", threshold );
    }
};

class Test_iThreshold_LT_8u_C1R : public SrcDstRoiThresholdSingleChannel_NoCmpOp<U8, U8>
{
public:
    Test_iThreshold_LT_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel_NoCmpOp<U8,U8>( parent, "fwiThreshold_LT_8u_C1R", fwiThreshold_LT_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 threshold;

        threshold = 10;
        RunTest( "0 5 10 15 20", "10 10 10 15 20", threshold );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold_LT )

TEST_ENTRY( Test_iThreshold_LT_8u_C1IR )
TEST_ENTRY( Test_iThreshold_LT_8u_C1R )

END_TEST_TABLE()
