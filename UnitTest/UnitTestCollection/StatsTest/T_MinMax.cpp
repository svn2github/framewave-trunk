/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"


class TestMax_16s: public SrcDstLen< S16, S16 >
{
public:
    TestMax_16s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, S16 >( parent, "fwsMax_16s", fwsMax_16s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};

class TestMax_32s: public SrcDstLen< S32, S32 >
{
public:
    TestMax_32s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S32, S32 >( parent, "fwsMax_32s", fwsMax_32s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};


class TestMax_64u: public SrcDstLen< U64, U64 >
{
public:
    TestMax_64u( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< U64, U64 >( parent, "fwsMax_64u", fwsMax_64u )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};


class TestMax_32f: public SrcDstLen< F32, F32 >
{
public:
    TestMax_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F32, F32 >( parent, "fwsMax_32f", fwsMax_32f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0.0 1.0 22.0 63.0 64.0 16.0 254.0","254.0",len);
    }
};

class TestMax_64f: public SrcDstLen< F64, F64>
{
public:
    TestMax_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsMax_64f", fwsMax_64f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0.0 1.0 22.0 63.0 64.0 16.0 254.0","254.0",len);
    }
};


class TestMin_16s: public SrcDstLen< S16, S16 >
{
public:
    TestMin_16s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, S16 >( parent, "fwsMin_16s", fwsMin_16s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","0",len);
    }
};

class TestMin_32s: public SrcDstLen< S32, S32 >
{
public:
    TestMin_32s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S32, S32 >( parent, "fwsMin_32s", fwsMin_32s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","0",len);
    }
};

class TestMin_64u: public SrcDstLen< U64, U64 >
{
public:
    TestMin_64u( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< U64, U64 >( parent, "fwsMin_64u", fwsMin_64u )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","0",len);
    }
};


class TestMin_32f: public SrcDstLen< F32, F32 >
{
public:
    TestMin_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F32, F32 >( parent, "fwsMin_32f", fwsMin_32f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0.0 1.0 22.0 63.0 64.0 16.0 254.0","0.0",len);
    }
};

class TestMin_64f: public SrcDstLen< F64, F64>
{
public:
    TestMin_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsMin_64f", fwsMin_64f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0.0 1.0 22.0 63.0 64.0 16.0 254.0","0.0",len);
    }
};


class TestMaxAbs_16s: public SrcDstLen< S16, S16 >
{
public:
    TestMaxAbs_16s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, S16 >( parent, "fwsMaxAbs_16s", fwsMaxAbs_16s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};

class TestMaxAbs_32s: public SrcDstLen< S32, S32 >
{
public:
    TestMaxAbs_32s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S32, S32 >( parent, "fwsMaxAbs_32s", fwsMaxAbs_32s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};

class TestMinAbs_16s: public SrcDstLen< S16, S16 >
{
public:
    TestMinAbs_16s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, S16 >( parent, "fwsMinAbs_16s", fwsMinAbs_16s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","0",len);
    }
};

class TestMinAbs_32s: public SrcDstLen< S32, S32 >
{
public:
    TestMinAbs_32s( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S32, S32 >( parent, "fwsMinAbs_32s", fwsMinAbs_32s )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","0",len);
    }
};



DEFINE_TEST_TABLE( MinMaxTestCatalog )

TEST_ENTRY( TestMax_16s )
TEST_ENTRY( TestMax_32s )
TEST_ENTRY( TestMax_64u )
TEST_ENTRY( TestMax_32f )
TEST_ENTRY( TestMax_64f )
TEST_ENTRY( TestMin_16s )
TEST_ENTRY( TestMin_32s )
TEST_ENTRY( TestMin_32f )
TEST_ENTRY( TestMin_64u )
TEST_ENTRY( TestMin_64f )
TEST_ENTRY( TestMaxAbs_16s )
TEST_ENTRY( TestMaxAbs_32s )
TEST_ENTRY( TestMinAbs_16s )
TEST_ENTRY( TestMinAbs_32s )

END_TEST_TABLE()
