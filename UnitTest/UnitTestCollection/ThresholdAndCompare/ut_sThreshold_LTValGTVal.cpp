/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwSignal.h"

class Test_sThreshold_LTValGTVal_16s_I : public SDstLenThreshold_Double_NoCmpOp_Val<S16, S16>
{
public:
    Test_sThreshold_LTValGTVal_16s_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_Double_NoCmpOp_Val<S16, S16>( parent, "fwsThreshold_LTValGTVal_16s_I", fwsThreshold_LTValGTVal_16s_I )    {}

    virtual void RunAll()
    {
        S16 thresholdLT;
        S16 valueLT;
        S16 thresholdGT;
        S16 valueGT;

        thresholdLT = 50; valueLT = 25; thresholdGT = 120; valueGT = 175;
        RunTest( "0 45 50 70 125", "25 25 50 70 175", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

class Test_sThreshold_LTValGTVal_16s : public SrcDstLenThreshold_Double_NoCmpOp_Val<S16, S16, S16>
{
public:
    Test_sThreshold_LTValGTVal_16s( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_Double_NoCmpOp_Val<S16, S16, S16>( parent, "fwsThreshold_LTValGTVal_16s", fwsThreshold_LTValGTVal_16s )    {}

    virtual void RunAll()
    {
        S16 thresholdLT;
        S16 valueLT;
        S16 thresholdGT;
        S16 valueGT;

        thresholdLT = 50; valueLT = 25; thresholdGT = 120; valueGT = 175;
        RunTest( "0 45 50 70 125", "25 25 50 70 175", thresholdLT, valueLT, thresholdGT, valueGT );
    }
};

DEFINE_TEST_TABLE( Catalog_sThreshold_LTValGTVal )

TEST_ENTRY( Test_sThreshold_LTValGTVal_16s_I )
TEST_ENTRY( Test_sThreshold_LTValGTVal_16s )

END_TEST_TABLE()
