/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iCompareEqualEps_32f_C1R : public SrcDstRoiCompareEqualEps<F32, 1>
{
public:
    Test_iCompareEqualEps_32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareEqualEps<F32,1>( parent, "fwiCompareEqualEps_32f_C1R", fwiCompareEqualEps_32f_C1R )
    {}

    virtual void RunAll()
    {
        F32 eps;

        eps = 0; 
        RunTest( "1 4 15 9 255", "1.001 4 14.99 9 255", "0 255 0 255 255", eps );
    }
};

class Test_iCompareEqualEps_32f_AC4R : public SrcDstRoiCompareEqualEps<F32, 4>
{
public:
    Test_iCompareEqualEps_32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareEqualEps<F32,4>( parent, "fwiCompareEqualEps_32f_AC4R", fwiCompareEqualEps_32f_AC4R )
    {}

    virtual void RunAll()
    {
        F32 eps;

        eps = 0; 
        RunTest( "1 4 15 32 56 89 30 16 78 9 255 64", "1.001 4 14.99 32 56 89 30 101 78 9 254.999 4", "0 255 0", eps );
    }
};

DEFINE_TEST_TABLE( Catalog_iCompareEqualEps )

TEST_ENTRY( Test_iCompareEqualEps_32f_C1R )
TEST_ENTRY( Test_iCompareEqualEps_32f_AC4R )

END_TEST_TABLE()
