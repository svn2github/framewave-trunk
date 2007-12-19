/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwSignal.h"

class Test_sThreshold_LTInv_32f_I : public SDstLenThreshold_NoCmpOp<F32, F32>
{
public:
    Test_sThreshold_LTInv_32f_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_NoCmpOp<F32, F32>( parent, "fwsThreshold_LTInv_32f_I", fwsThreshold_LTInv_32f_I )    {}

    virtual void RunAll()
    {
        F32 threshold;
        Fw32f errorMargin = 0.00001f;

        threshold = 10.3f;
        RunTest( "0 5.1 -5.1 15.1 20.7", "0.09708 0.09708 -0.09708 0.06623 0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_64f_I : public SDstLenThreshold_NoCmpOp<F64, F64>
{
public:
    Test_sThreshold_LTInv_64f_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_NoCmpOp<F64, F64>( parent, "fwsThreshold_LTInv_64f_I", fwsThreshold_LTInv_64f_I )    {}

    virtual void RunAll()
    {
        F64 threshold;
        Fw64f errorMargin = 0.00001;

        threshold = 10.3f;
        RunTest( "0 5.1 -5.1 15.1 20.7", "0.09708 0.09708 -0.09708 0.06623 0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_32fc_I : public SDstLenThreshold_NoCmpOp<Fw32fc, F32>
{
public:
    Test_sThreshold_LTInv_32fc_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_NoCmpOp<Fw32fc, F32>( parent, "fwsThreshold_LTInv_32fc_I", fwsThreshold_LTInv_32fc_I )    {}

    virtual void RunAll()
    {
        F32 threshold;
        Fw32fc errorMargin = {0.00001f, 0.00001f};

        threshold = 10.3f;
        RunTest( "0 0 -1.2 8.9 5.1 -15.1 0 20.7", "0.09708 0 -0.01297 -0.09622 0.02008 0.05944 0 -0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_64fc_I : public SDstLenThreshold_NoCmpOp<Fw64fc, F64>
{
public:
    Test_sThreshold_LTInv_64fc_I( UnitTestCatalogBase & parent ) : SDstLenThreshold_NoCmpOp<Fw64fc, F64>( parent, "fwsThreshold_LTInv_64fc_I", fwsThreshold_LTInv_64fc_I )    {}

    virtual void RunAll()
    {
        F64 threshold;
        Fw64fc errorMargin = {0.00001, 0.00001};

        threshold = 10.3f;
        RunTest( "0 0 -1.2 8.9 5.1 -15.1 0 20.7", "0.09708 0 -0.01297 -0.09622 0.02008 0.05944 0 -0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_32f : public SrcDstLenThreshold_NoCmpOp<F32, F32, F32>
{
public:
    Test_sThreshold_LTInv_32f( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_NoCmpOp<F32, F32, F32>( parent, "fwsThreshold_LTInv_32f", fwsThreshold_LTInv_32f )    {}

    virtual void RunAll()
    {
        F32 threshold;
        Fw32f errorMargin = 0.00001f;

        threshold = 10.3f;
        RunTest( "0 5.1 -5.1 15.1 20.7", "0.09708 0.09708 -0.09708 0.06623 0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_64f : public SrcDstLenThreshold_NoCmpOp<F64, F64, F64>
{
public:
    Test_sThreshold_LTInv_64f( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_NoCmpOp<F64, F64, F64>( parent, "fwsThreshold_LTInv_64f", fwsThreshold_LTInv_64f )    {}

    virtual void RunAll()
    {
        F64 threshold;
        Fw64f errorMargin = 0.00001;

        threshold = 10.3f;
        RunTest( "0 5.1 -5.1 15.1 20.7", "0.09708 0.09708 -0.09708 0.06623 0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_32fc : public SrcDstLenThreshold_NoCmpOp<Fw32fc, Fw32fc, F32>
{
public:
    Test_sThreshold_LTInv_32fc( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_NoCmpOp<Fw32fc, Fw32fc, F32>( parent, "fwsThreshold_LTInv_32fc", fwsThreshold_LTInv_32fc )    {}

    virtual void RunAll()
    {
        F32 threshold;
        Fw32fc errorMargin = {0.00001f, 0.00001f};

        threshold = 10.3f;
        RunTest( "0 0 -1.2 8.9 5.1 -15.1 0 20.7", "0.09708 0 -0.01297 -0.09622 0.02008 0.05944 0 -0.04831", threshold, errorMargin );
    }
};

class Test_sThreshold_LTInv_64fc : public SrcDstLenThreshold_NoCmpOp<Fw64fc, Fw64fc, F64>
{
public:
    Test_sThreshold_LTInv_64fc( UnitTestCatalogBase & parent ) : SrcDstLenThreshold_NoCmpOp<Fw64fc, Fw64fc, F64>( parent, "fwsThreshold_LTInv_64fc", fwsThreshold_LTInv_64fc )    {}

    virtual void RunAll()
    {
        F64 threshold;
        Fw64fc errorMargin = {0.00001, 0.00001};

        threshold = 10.3f;
        RunTest( "0 0 -1.2 8.9 5.1 -15.1 0 20.7", "0.09708 0 -0.01297 -0.09622 0.02008 0.05944 0 -0.04831", threshold, errorMargin );
    }
};

DEFINE_TEST_TABLE( Catalog_sThreshold_LTInv )

TEST_ENTRY( Test_sThreshold_LTInv_32f_I )
TEST_ENTRY( Test_sThreshold_LTInv_64f_I )
TEST_ENTRY( Test_sThreshold_LTInv_32fc_I )
TEST_ENTRY( Test_sThreshold_LTInv_64fc_I )

TEST_ENTRY( Test_sThreshold_LTInv_32f )
TEST_ENTRY( Test_sThreshold_LTInv_64f )
TEST_ENTRY( Test_sThreshold_LTInv_32fc )
TEST_ENTRY( Test_sThreshold_LTInv_64fc )

END_TEST_TABLE()
