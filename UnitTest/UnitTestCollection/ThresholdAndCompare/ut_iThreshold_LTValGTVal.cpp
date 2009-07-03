/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwImage.h"

class Test_iThreshold_LTValGTVal_8u_C1IR : public SDstRoiThresholdSingleChannel_Double_NoCmpOp_Val<U8>
{
public:
    Test_iThreshold_LTValGTVal_8u_C1IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdSingleChannel_Double_NoCmpOp_Val<U8>( parent, "fwiThreshold_LTValGTVal_8u_C1IR", fwiThreshold_LTValGTVal_8u_C1IR )
    {}

    virtual void RunAll()
    {
        U8 thresholdLT;
        U8 valueLT;
        U8 thresholdGT;
        U8 valueGT;

        thresholdLT = 50; valueLT = 25; thresholdGT = 120; valueGT = 175;
        RunTest( "0 45 50 70 125", "25 25 50 70 175", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

class Test_iThreshold_LTValGTVal_8u_C1R : public SrcDstRoiThresholdSingleChannel_Double_NoCmpOp_Val<U8, U8>
{
public:
    Test_iThreshold_LTValGTVal_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdSingleChannel_Double_NoCmpOp_Val<U8,U8>( parent, "fwiThreshold_LTValGTVal_8u_C1R", fwiThreshold_LTValGTVal_8u_C1R )
    {}

    virtual void RunAll()
    {
        U8 thresholdLT;
        U8 valueLT;
        U8 thresholdGT;
        U8 valueGT;

        thresholdLT = 50; valueLT = 25; thresholdGT = 120; valueGT = 175;
        RunTest( "0 45 50 70 125", "25 25 50 70 175", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

class Test_iThreshold_LTValGTVal_8u_C3IR : public SDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8, 3, 3>
{
public:
    Test_iThreshold_LTValGTVal_8u_C3IR( UnitTestCatalogBase & parent ) : SDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8,3,3>( parent, "fwiThreshold_LTValGTVal_8u_C3IR", fwiThreshold_LTValGTVal_8u_C3IR )
    {}

    virtual void RunAll()
    {
        U8 thresholdLT[3];
        U8 valueLT[3];
        U8 thresholdGT[3];
        U8 valueGT[3];

        thresholdLT[0] = 2; thresholdLT[1] = 10; thresholdLT[2] = 15;
        valueLT[0] = 25; valueLT[1] = 9; valueLT[2] = 51;
        thresholdGT[0] = 10; thresholdGT[1] = 25; thresholdGT[2] = 43;
        valueGT[0] = 11; valueGT[1] = 34; valueGT[2] = 75;
        RunTest( "0 1 2 3 6 27 17 50 89", "25 9 51 3 9 27 11 34 75", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

class Test_iThreshold_LTValGTVal_8u_C3R : public SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8, 3, U8, 3, 3>
{
public:
    Test_iThreshold_LTValGTVal_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8,3,U8,3,3>( parent, "fwiThreshold_LTValGTVal_8u_C3R", fwiThreshold_LTValGTVal_8u_C3R )
    {}

    virtual void RunAll()
    {
        U8 thresholdLT[3];
        U8 valueLT[3];
        U8 thresholdGT[3];
        U8 valueGT[3];

        thresholdLT[0] = 2; thresholdLT[1] = 10; thresholdLT[2] = 15;
        valueLT[0] = 25; valueLT[1] = 9; valueLT[2] = 51;
        thresholdGT[0] = 10; thresholdGT[1] = 25; thresholdGT[2] = 43;
        valueGT[0] = 11; valueGT[1] = 34; valueGT[2] = 75;
        RunTest( "0 1 2 3 6 27 17 50 89", "25 9 51 3 9 27 11 34 75", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

class Test_iThreshold_LTValGTVal_8u_AC4R : public SrcSDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8, 4, U8, 4, 3>
{
public:
    Test_iThreshold_LTValGTVal_8u_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoiThresholdMultiChannel_Double_NoCmpOp_Val<U8,4,U8,4,3>( parent, "fwiThreshold_LTValGTVal_8u_AC4R", fwiThreshold_LTValGTVal_8u_AC4R )
    {}

    virtual void RunAll()
    {
        U8 thresholdLT[3];
        U8 valueLT[3];
        U8 thresholdGT[3];
        U8 valueGT[3];

        thresholdLT[0] = 2; thresholdLT[1] = 10; thresholdLT[2] = 15;
        valueLT[0] = 25; valueLT[1] = 9; valueLT[2] = 51;
        thresholdGT[0] = 10; thresholdGT[1] = 25; thresholdGT[2] = 43;
        valueGT[0] = 11; valueGT[1] = 34; valueGT[2] = 75;
        RunTest( "0 1 2 0 3 6 27 0 17 50 89 0", "0 0 0 134 0 0 0 13 0 0 0 201", "25 9 51 134 3 9 27 13 11 34 75 201", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

DEFINE_TEST_TABLE( Catalog_iThreshold_LTValGTVal )

TEST_ENTRY( Test_iThreshold_LTValGTVal_8u_C1IR )
TEST_ENTRY( Test_iThreshold_LTValGTVal_8u_C1R )
TEST_ENTRY( Test_iThreshold_LTValGTVal_8u_C3IR )
TEST_ENTRY( Test_iThreshold_LTValGTVal_8u_C3R )
TEST_ENTRY( Test_iThreshold_LTValGTVal_8u_AC4R )

END_TEST_TABLE()
