/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iCompare_8u_C1R : public SrcDstRoiCompare<U8, 1>
{
public:
    Test_iCompare_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiCompare<U8,1>( parent, "fwiCompare_8u_C1R", fwiCompare_8u_C1R )
    {}

    virtual void RunAll()
    {
        FwCmpOp fwCmpOp;

        fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 9 255", "0 5 10 15 255", "255 0 255 0 0", fwCmpOp );
        fwCmpOp = fwCmpLess;
        RunTest( "1 4 15 9 255", "0 5 10 15 255", "0 255 0 255 0", fwCmpOp );
    }
};

class Test_iCompare_8u_C3R : public SrcDstRoiCompare<U8, 3>
{
public:
    Test_iCompare_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiCompare<U8,3>( parent, "fwiCompare_8u_C3R", fwiCompare_8u_C3R )
    {}

    virtual void RunAll()
    {
        FwCmpOp fwCmpOp;

        fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 56 89 30 78 9 255", "0 3 10 60 85 21 74 5 254", "255 0 255", fwCmpOp );
        fwCmpOp = fwCmpLess;
        RunTest( "0 3 10 60 85 21 74 5 254", "1 4 15 56 89 30 78 9 255", "255 0 255", fwCmpOp );
    }
};

class Test_iCompare_8u_AC4R : public SrcDstRoiCompare<U8, 4>
{
public:
    Test_iCompare_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoiCompare<U8,4>( parent, "fwiCompare_8u_AC4R", fwiCompare_8u_AC4R )
    {}

    virtual void RunAll()
    {
        FwCmpOp fwCmpOp;

        fwCmpOp = fwCmpGreater; 
        RunTest( "1 4 15 32 56 89 30 16 78 9 255 64", "0 3 10 8 60 85 21 32 74 5 254 4", "255 0 255", fwCmpOp );
        fwCmpOp = fwCmpLess;
        RunTest( "0 3 10 8 60 85 21 32 74 5 254 4", "1 4 15 32 56 89 30 16 78 9 255 64", "255 0 255", fwCmpOp );
    }
};

DEFINE_TEST_TABLE( Catalog_iCompare )

TEST_ENTRY( Test_iCompare_8u_C1R )
TEST_ENTRY( Test_iCompare_8u_C3R )
TEST_ENTRY( Test_iCompare_8u_AC4R )

END_TEST_TABLE()
