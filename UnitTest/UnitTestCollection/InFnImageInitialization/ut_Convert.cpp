/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "InFnImageInitialization.h"

#include "FwSharedCode.h"
#include "ConvertDef.h"
#include "fwdev.h"

#include "PixelFunctionObjects.h"

using namespace OPT_LEVEL;

class Test_Convert_8u16_C14R : public UnitTestBase
{
public:
    Test_Convert_8u16_C14R( UnitTestCatalogBase & unitTestCatalog )
        : UnitTestBase( unitTestCatalog, "Test_Convert_8u16_C14R" )
    {
    }
    void Test()
    {
        #define INIT_STR_1 "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15"
        #define INIT_STR_2 "0 1 2 3 4 5 6 7"

        const char *init[2][4] =    {
                                        {INIT_STR_1, "", "", ""},
                                        {INIT_STR_2, "", "", ""}
                                    };

        FwUnitTest::PixelFunc1Src1Dst< Convert_8u16_C14R<U8,C1,U16,C1> > obj(init);
        bool isPass = obj.Run();
        ReportStatus( isPass );
    }

    virtual void RunAll()
    {
        Test();
    }
};

DEFINE_TEST_TABLE( Catalog_Convert )
TEST_ENTRY( Test_Convert_8u16_C14R )
END_TEST_TABLE()
