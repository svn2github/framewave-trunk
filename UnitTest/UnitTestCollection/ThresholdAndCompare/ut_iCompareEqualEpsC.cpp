/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iCompareEqualEpsC_32f_C1R : public SrcDstRoiCompareEqualEpsCSingleChannel<F32>
{
public:
    Test_iCompareEqualEpsC_32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareEqualEpsCSingleChannel<F32>( parent, "fwiCompareEqualEpsC_32f_C1R", fwiCompareEqualEpsC_32f_C1R )
    {}

    virtual void RunAll()
    {
        F32 value;
        F32 eps;

        value = 15; eps = 0; 
        RunTest( "1 4 15 9 255", "0 0 255 0 0", value, eps );
    }
};

class Test_iCompareEqualEpsC_32f_AC4R : public SrcDstRoiCompareEqualEpsCMultiChannel<F32, 4, 3>
{
public:
    Test_iCompareEqualEpsC_32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoiCompareEqualEpsCMultiChannel<F32,4,3>( parent, "fwiCompareEqualEpsC_32f_C4R", fwiCompareEqualEpsC_32f_C4R )
    {}

    virtual void RunAll()
    {
        F32 value[3];
        F32 eps;

        value[0] = 56; value[1] = 89; value[2] = 30; eps = 0; 
        RunTest( "1 4 15 32 56 89 30 16 78 9 255 64", "0 255 0", value, eps );
    }
};

DEFINE_TEST_TABLE( Catalog_iCompareEqualEpsC )

TEST_ENTRY( Test_iCompareEqualEpsC_32f_C1R )
//TEST_ENTRY( Test_iCompareEqualEpsC_32f_AC4R )

END_TEST_TABLE()
