/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwSignal.h"

class Test_sThreshold_GTVal_16s_I : public SDstLenThreshold_NoCmpOp_Val<S16, S16>
{
public:
    Test_sThreshold_GTVal_16s_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_NoCmpOp_Val<S16, S16>( parent, "fwsThreshold_GTVal_16s_I", fwsThreshold_GTVal_16s_I )    {}

    virtual void RunAll()
    {
        S16 threshold;
        S16 value;

        threshold = 127; value = 200;
        RunTest( "0 128 127 201 100", "0 200 127 200 100", threshold, value );
    }
};

class Test_sThreshold_GTVal_16s : public SrcDstLenThreshold_NoCmpOp_Val<S16, S16, S16>
{
public:
    Test_sThreshold_GTVal_16s( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_NoCmpOp_Val<S16, S16, S16>( parent, "fwsThreshold_GTVal_16s", fwsThreshold_GTVal_16s )    {}

    virtual void RunAll()
    {
        S16 threshold;
        S16 value;

        threshold = 127; value = 200;
        RunTest( "0 128 127 201 100", "0 200 127 200 100", threshold, value );
    }
};

DEFINE_TEST_TABLE( Catalog_sThreshold_GTVal )

TEST_ENTRY( Test_sThreshold_GTVal_16s_I )
TEST_ENTRY( Test_sThreshold_GTVal_16s )

END_TEST_TABLE()
