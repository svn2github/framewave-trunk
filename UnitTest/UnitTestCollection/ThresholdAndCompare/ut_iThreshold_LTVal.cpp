/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_LTVal_8u_C1IR : public SDstRoiThresholdSingleChannel_NoCmpOp_Val<U8>
{
public:
    Test_iThreshold_LTVal_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel_NoCmpOp_Val<U8>( parent, "fwiThreshold_LTVal_8u_C1IR", fwiThreshold_LTVal_8u_C1IR )
    {}

    virtual void RunAll()
    {
        U8 threshold;
        U8 value;

        threshold = 10; value = 16;
        RunTest( "0 5 10 15 20", "16 16 10 15 20", threshold, value );
    }
};

class Test_iThreshold_LTVal_8u_C1R : public SrcDstRoiThresholdSingleChannel_NoCmpOp_Val<U8, U8>
{
public:
    Test_iThreshold_LTVal_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel_NoCmpOp_Val<U8,U8>( parent, "fwiThreshold_LTVal_8u_C1R", fwiThreshold_LTVal_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 threshold;
        U8 value;

        threshold = 10; value = 16;
        RunTest( "0 5 10 15 20", "16 16 10 15 20", threshold, value );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold_LTVal )

TEST_ENTRY( Test_iThreshold_LTVal_8u_C1IR )
TEST_ENTRY( Test_iThreshold_LTVal_8u_C1R )

END_TEST_TABLE()
