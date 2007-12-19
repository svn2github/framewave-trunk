/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_8u_C1IR : public SDstRoiThresholdSingleChannel<U8>
{
public:
    Test_iThreshold_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel<U8>( parent, "fwiThreshold_8u_C1IR", fwiThreshold_8u_C1IR )    {}

    virtual void RunAll()
    {
        U8 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10; fwCmpOp = fwCmpGreater; 
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 126 127 200 100", "127 127 127 200 127", threshold, fwCmpOp );
    }
};

class Test_iThreshold_8u_C1R : public SrcDstRoiThresholdSingleChannel<U8, U8>
{
public:
    Test_iThreshold_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel<U8,U8>( parent, "fwiThreshold_8u_C1R", fwiThreshold_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10; fwCmpOp = fwCmpGreater; 
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 126 127 200 100", "127 127 127 200 127", threshold, fwCmpOp );
    }
};

class Test_iThreshold_8u_C3IR : public SDstRoiThresholdMultiChannel<U8, 3, 3>
{
public:
    Test_iThreshold_8u_C3IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdMultiChannel<U8,3,3>( parent, "fwiThreshold_8u_C3IR", fwiThreshold_8u_C3IR )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        FwCmpOp fwCmpOp;

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15; fwCmpOp = fwCmpGreater; 
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 2 6 9 2 10 15", threshold, fwCmpOp );
    }
};

class Test_iThreshold_8u_C3R : public SrcDstRoiThresholdMultiChannel<U8, 3, U8, 3, 3>
{
public:
    Test_iThreshold_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdMultiChannel<U8,3,U8,3,3>( parent, "fwiThreshold_8u_C3R", fwiThreshold_8u_C3R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        FwCmpOp fwCmpOp;

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15; fwCmpOp = fwCmpGreater; 
        RunTest( "0 1 2 3 6 9 17 50 89", "0 1 2 2 6 9 2 10 15", threshold, fwCmpOp );
    }
};

class Test_iThreshold_8u_AC4R : public SrcSDstRoiThresholdMultiChannel<U8, 4, U8, 4, 3>
{
public:
    Test_iThreshold_8u_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoiThresholdMultiChannel<U8,4,U8,4,3>( parent, "fwiThreshold_8u_AC4R", fwiThreshold_8u_AC4R )
    {}

    virtual void RunAll()
    {
        U8 threshold[3];
        FwCmpOp fwCmpOp;

        threshold[0] = 2; threshold[1] = 10; threshold[2] = 15; fwCmpOp = fwCmpGreater; 
        RunTest( "0 1 2 0 3 6 9 0 17 50 89 0", "0 0 0 56 0 0 0 78 0 0 0 255", "0 1 2 56 2 6 9 78 2 10 15 255", threshold, fwCmpOp );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold )

TEST_ENTRY( Test_iThreshold_8u_C1IR )
TEST_ENTRY( Test_iThreshold_8u_C1R )
TEST_ENTRY( Test_iThreshold_8u_C3IR )
TEST_ENTRY( Test_iThreshold_8u_C3R )
TEST_ENTRY( Test_iThreshold_8u_AC4R )

END_TEST_TABLE()
