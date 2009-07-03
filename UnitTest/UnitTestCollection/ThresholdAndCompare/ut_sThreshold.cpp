/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ut_ThresholdAndCompare.h"

#include "ThresholdAndCompareObjects.h"
#include "fwSignal.h"

class Test_sThreshold_16s_I : public SDstLenThreshold<S16, S16>
{
public:
    Test_sThreshold_16s_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<S16, S16>( parent, "fwsThreshold_16s_I", fwsThreshold_16s_I )    {}

    virtual void RunAll()
    {
        S16 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10; fwCmpOp = fwCmpGreater; 
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 126 127 200 100", "127 127 127 200 127", threshold, fwCmpOp );
    }
};

class Test_sThreshold_32f_I : public SDstLenThreshold<F32, F32>
{
public:
    Test_sThreshold_32f_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<F32, F32>( parent, "fwsThreshold_32f_I", fwsThreshold_32f_I )    {}

    virtual void RunAll()
    {
        F32 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10.3f; fwCmpOp = fwCmpGreater; 
        RunTest( "0.5 5.1 10.6 15.1 20.7", "0.5 5.1 10.3 10.3 10.3", threshold, fwCmpOp );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 127.1 127.2 127.3 150.6", "127.2 127.2 127.2 127.3 150.6", threshold, fwCmpOp );
    }
};

class Test_sThreshold_64f_I : public SDstLenThreshold<F64, F64>
{
public:
    Test_sThreshold_64f_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<F64, F64>( parent, "fwsThreshold_64f_I", fwsThreshold_64f_I )    {}

    virtual void RunAll()
    {
        F64 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10.3; fwCmpOp = fwCmpGreater; 
        RunTest( "0.5 5.1 10.6 15.1 20.7", "0.5 5.1 10.3 10.3 10.3", threshold, fwCmpOp );
        threshold = 127.2; fwCmpOp = fwCmpLess;
        RunTest( "0.3 127.1 127.2 127.3 150.6", "127.2 127.2 127.2 127.3 150.6", threshold, fwCmpOp );
    }
};

class Test_sThreshold_16sc_I : public SDstLenThreshold<Fw16sc, S16>
{
public:
    Test_sThreshold_16sc_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<Fw16sc, S16>( parent, "fwsThreshold_16sc_I", fwsThreshold_16sc_I )    {}

    virtual void RunAll()
    {
        S16 threshold;
        FwCmpOp fwCmpOp;

        threshold = 50; fwCmpOp = fwCmpGreater;
        RunTest( "1 6 11 16 21 234 230 235", "1 6 11 16 4 49 34 35", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 26 89 50 100 10 127 100", "0 127 111 63 127 13 127 100", threshold, fwCmpOp  );
    }
};

class Test_sThreshold_32fc_I : public SDstLenThreshold<Fw32fc, F32>
{
public:
    Test_sThreshold_32fc_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<Fw32fc, F32>( parent, "fwsThreshold_32fc_I", fwsThreshold_32fc_I )    {}

    virtual void RunAll()
    {
        F32 threshold;
        FwCmpOp fwCmpOp;
        Fw32fc errorMargin = {0.01f,0.01f};

        threshold = 10.3f; fwCmpOp = fwCmpGreater;
        RunTest( "0.5 5.1 10.6 15.1 20.7 234.7 230.5 235.1", "0.5 5.1 5.92 8.43 0.90 10.26 7.21 7.35", threshold, fwCmpOp, errorMargin );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 25.6 127.1 50.4 127.2 50.5 127.3 100.0", "1.49 127.2 127.1 50.4 127.2 50.5 127.3 100.0", threshold, fwCmpOp, errorMargin  );
    }
};

class Test_sThreshold_64fc_I : public SDstLenThreshold<Fw64fc, F64>
{
public:
    Test_sThreshold_64fc_I( UnitTestCatalogBase & parent ) : SDstLenThreshold<Fw64fc, F64>( parent, "fwsThreshold_64fc_I", fwsThreshold_64fc_I )    {}

    virtual void RunAll()
    {
        F64 threshold;
        FwCmpOp fwCmpOp;
        Fw64fc errorMargin = {0.01,0.01};

        threshold = 10.3f; fwCmpOp = fwCmpGreater;
        RunTest( "0.5 5.1 10.6 15.1 20.7 234.7 230.5 235.1", "0.5 5.1 5.92 8.43 0.90 10.26 7.21 7.35", threshold, fwCmpOp, errorMargin );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 25.6 127.1 50.4 127.2 50.5 127.3 100.0", "1.49 127.2 127.1 50.4 127.2 50.5 127.3 100.0", threshold, fwCmpOp, errorMargin  );
    }
};

class Test_sThreshold_16s : public SrcDstLenThreshold<S16, S16, S16>
{
public:
    Test_sThreshold_16s( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<S16, S16, S16>( parent, "fwsThreshold_16s", fwsThreshold_16s )    {}

    virtual void RunAll()
    {
        S16 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10; fwCmpOp = fwCmpGreater; 
        RunTest( "0 5 10 15 20", "0 5 10 10 10", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 126 127 200 100", "127 127 127 200 127", threshold, fwCmpOp );
    }
};

class Test_sThreshold_32f : public SrcDstLenThreshold<F32, F32, F32>
{
public:
    Test_sThreshold_32f( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<F32, F32, F32>( parent, "fwsThreshold_32f", fwsThreshold_32f )    {}

    virtual void RunAll()
    {
        F32 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10.3f; fwCmpOp = fwCmpGreater; 
        RunTest( "0.5 5.1 10.6 15.1 20.7", "0.5 5.1 10.3 10.3 10.3", threshold, fwCmpOp );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 127.1 127.2 127.3 150.6", "127.2 127.2 127.2 127.3 150.6", threshold, fwCmpOp );
    }
};

class Test_sThreshold_64f : public SrcDstLenThreshold<F64, F64, F64>
{
public:
    Test_sThreshold_64f( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<F64, F64, F64>( parent, "fwsThreshold_64f", fwsThreshold_64f )    {}

    virtual void RunAll()
    {
        F64 threshold;
        FwCmpOp fwCmpOp;

        threshold = 10.3; fwCmpOp = fwCmpGreater; 
        RunTest( "0.5 5.1 10.6 15.1 20.7", "0.5 5.1 10.3 10.3 10.3", threshold, fwCmpOp );
        threshold = 127.2; fwCmpOp = fwCmpLess;
        RunTest( "0.3 127.1 127.2 127.3 150.6", "127.2 127.2 127.2 127.3 150.6", threshold, fwCmpOp );
    }
};

class Test_sThreshold_16sc : public SrcDstLenThreshold<Fw16sc, Fw16sc, S16>
{
public:
    Test_sThreshold_16sc( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<Fw16sc, Fw16sc, S16>( parent, "fwsThreshold_16sc", fwsThreshold_16sc )    {}

    virtual void RunAll()
    {
        S16 threshold;
        FwCmpOp fwCmpOp;

        threshold = 50; fwCmpOp = fwCmpGreater;
        RunTest( "1 6 11 16 21 234 230 235", "1 6 11 16 4 49 34 35", threshold, fwCmpOp );
        threshold = 127; fwCmpOp = fwCmpLess;
        RunTest( "0 26 89 50 100 10 127 100", "0 127 111 63 127 13 127 100", threshold, fwCmpOp  );
    }
};

class Test_sThreshold_32fc : public SrcDstLenThreshold<Fw32fc, Fw32fc, F32>
{
public:
    Test_sThreshold_32fc( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<Fw32fc, Fw32fc, F32>( parent, "fwsThreshold_32fc", fwsThreshold_32fc )    {}

    virtual void RunAll()
    {
        F32 threshold;
        FwCmpOp fwCmpOp;
        Fw32fc errorMargin = {0.01f,0.01f};

        threshold = 10.3f; fwCmpOp = fwCmpGreater;
        RunTest( "0.5 5.1 10.6 15.1 20.7 234.7 230.5 235.1", "0.5 5.1 5.92 8.43 0.90 10.26 7.21 7.35", threshold, fwCmpOp, errorMargin );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 25.6 127.1 50.4 127.2 50.5 127.3 100.0", "1.49 127.2 127.1 50.4 127.2 50.5 127.3 100.0", threshold, fwCmpOp, errorMargin  );
    }
};

class Test_sThreshold_64fc : public SrcDstLenThreshold<Fw64fc, Fw64fc, F64>
{
public:
    Test_sThreshold_64fc( UnitTestCatalogBase & parent ) : SrcDstLenThreshold<Fw64fc, Fw64fc, F64>( parent, "fwsThreshold_64fc", fwsThreshold_64fc )    {}

    virtual void RunAll()
    {
        F64 threshold;
        FwCmpOp fwCmpOp;
        Fw64fc errorMargin = {0.01,0.01};

        threshold = 10.3f; fwCmpOp = fwCmpGreater;
        RunTest( "0.5 5.1 10.6 15.1 20.7 234.7 230.5 235.1", "0.5 5.1 5.92 8.43 0.90 10.26 7.21 7.35", threshold, fwCmpOp, errorMargin );
        threshold = 127.2f; fwCmpOp = fwCmpLess;
        RunTest( "0.3 25.6 127.1 50.4 127.2 50.5 127.3 100.0", "1.49 127.2 127.1 50.4 127.2 50.5 127.3 100.0", threshold, fwCmpOp, errorMargin  );
    }
};

DEFINE_TEST_TABLE( Catalog_sThreshold )

TEST_ENTRY( Test_sThreshold_16s_I )
TEST_ENTRY( Test_sThreshold_32f_I )
TEST_ENTRY( Test_sThreshold_64f_I )
TEST_ENTRY( Test_sThreshold_16sc_I )
TEST_ENTRY( Test_sThreshold_32fc_I )
TEST_ENTRY( Test_sThreshold_64fc_I )

TEST_ENTRY( Test_sThreshold_16s )
TEST_ENTRY( Test_sThreshold_32f )
TEST_ENTRY( Test_sThreshold_64f )
TEST_ENTRY( Test_sThreshold_16sc )
TEST_ENTRY( Test_sThreshold_32fc )
TEST_ENTRY( Test_sThreshold_64fc )

END_TEST_TABLE()
