/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "InFnImageArithmeticAndLogical.h"

#include "FwSharedCode.h"
#include "AbsDef.h"
#include "ThresholdIdef.h"
#include "fwdev.h"

#include "PixelFunctionObjects.h"

using namespace OPT_LEVEL;

class Test_C1_Abs_16s : public UnitTestBase
{
public:
    Test_C1_Abs_16s( UnitTestCatalogBase & unitTestCatalog )
        : UnitTestBase( unitTestCatalog, "Test_C1_Abs_16s" )
    {
    }
    void Test()
    {
        #define INIT_STR_1 "0 -1 2 -3 4 -5 -6 7"
        #define INIT_STR_2 "0 1 2 3 4 5 6 7"

        const char *init[2][4] =    {
                                        {INIT_STR_1, "", "", ""},
                                        {INIT_STR_2, "", "", ""}
                                    };

        FwUnitTest::PixelFunc1Src1Dst<DEF_ABS::ABS::C1::Abs_16s> obj(init);
        bool isPass = obj.Run();
        ReportStatus( isPass );
    }

    virtual void RunAll()
    {
        Test();
    }
};

class Test_Threshold_8u_C1IR_LT : public UnitTestBase
{
public:
    Test_Threshold_8u_C1IR_LT( UnitTestCatalogBase & unitTestCatalog )
        : UnitTestBase( unitTestCatalog, "Test_Threshold_8u_C1IR_LT" )
    {
    }
    void Test()
    {
        #define INIT_STR_3 "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15"


        const char *init[2][4] =    {
                                        {INIT_STR_3, "", "", ""},
                                        {INIT_STR_3, "", "", ""}
                                    };

        FwUnitTest::PixelFunc1SrcDst_val<Threshold_8u_C1IR_LT> obj(init, 0);
        bool isPass = obj.Run();
        ReportStatus( isPass );
    }

    virtual void RunAll()
    {
        Test();
    }
};

DEFINE_TEST_TABLE( Catalog_Abs )
TEST_ENTRY( Test_C1_Abs_16s )
TEST_ENTRY( Test_Threshold_8u_C1IR_LT )
END_TEST_TABLE()
