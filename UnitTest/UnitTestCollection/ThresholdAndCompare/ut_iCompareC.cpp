/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iCompareC_8u_C1R : public SrcDstRoiCompareCSingleChannel<U8>
{
public:
    Test_iCompareC_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareCSingleChannel<U8>( parent, "fwiCompareC_8u_C1R", fwiCompareC_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 value;
        FwCmpOp fwCmpOp;

        value = 10; fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 9 255", "0 0 255 0 255", value, fwCmpOp );
        value = 10; fwCmpOp = fwCmpLess;
        RunTest( "1 4 15 9 255", "255 255 0 255 0", value, fwCmpOp );
    }
};

class Test_iCompareC_8u_C3R : public SrcDstRoiCompareCMultiChannel<U8, 3, 3>
{
public:
    Test_iCompareC_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareCMultiChannel<U8,3,3>( parent, "fwiCompareC_8u_C3R", fwiCompareC_8u_C3R )
    {}

    virtual void RunAll()
    {
        U8 value[3];
        FwCmpOp fwCmpOp;

        value[0] = 25; value[1] = 8; value[2] = 67; fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 56 89 30 78 9 255", "0 0 255", value, fwCmpOp );
        value[0] = 25; value[1] = 8; value[2] = 67; fwCmpOp = fwCmpLess; 
        RunTest( "0 3 10 60 85 21 74 5 254", "255 0 0", value, fwCmpOp );
    }
};

class Test_iCompareC_8u_AC4R : public SrcDstRoiCompareCMultiChannel<U8, 4, 3>
{
public:
    Test_iCompareC_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareCMultiChannel<U8,4,3>( parent, "fwiCompareC_8u_AC4R", fwiCompareC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        U8 value[3];
        FwCmpOp fwCmpOp;

        value[0] = 25; value[1] = 8; value[2] = 67; fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 31 56 89 30 99 78 9 255 3", "0 0 255", value, fwCmpOp );
        value[0] = 25; value[1] = 8; value[2] = 67; fwCmpOp = fwCmpLess; 
        RunTest( "0 3 10 31 60 85 21 99 74 5 254 3", "255 0 0", value, fwCmpOp );
    }
};

DEFINE_TEST_TABLE( Catalog_iCompareC )

TEST_ENTRY( Test_iCompareC_8u_C1R )
TEST_ENTRY( Test_iCompareC_8u_C3R )
TEST_ENTRY( Test_iCompareC_8u_AC4R )

END_TEST_TABLE()
