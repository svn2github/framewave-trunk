/* 
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FixedAccuracy.h"

#include "FunctionSignatures.h"
#include "fwSignal.h"


class TestPow_32f_A11 : public SrcSrcDstLen< F32,F32,F32 >
{
public:
    TestPow_32f_A11( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32,F32 >( parent, "fwsPow_32f_A11", fwsPow_32f_A11 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "1 2.456783 3 4 5 6 7 8", "1 2 3 4 5 6 7 8", "1 6.0357828 27 256 3125 46656 823543 16777216",len);
    }
};


class TestPow_32f_A21 : public SrcSrcDstLen< F32,F32,F32 >
{
public:
    TestPow_32f_A21( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32,F32 >( parent, "fwsPow_32f_A21", fwsPow_32f_A21 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "1 2.456783 3 4 5 6 7 8", "1 2 3 4 5 6 7 8", "1 6.0357828 27 256 3125 46656 823543 16777216",len);
    }
};


class TestPow_32f_A24 : public SrcSrcDstLen< F32,F32,F32 >
{
public:
    TestPow_32f_A24( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32,F32 >( parent, "fwsPow_32f_A24", fwsPow_32f_A24 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "1 2.456783 3 4 5 6 7 8", "1 2 3 4 5 6 7 8", "1 6.0357827090890011 27 256 3125 46656 823543 16777216",len);
    }
};

class TestPow_64f_A50 : public SrcSrcDstLen< F64,F64,F64 >
{
public:
    TestPow_64f_A50( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64,F64,F64 >( parent, "fwsPow_64f_A50", fwsPow_64f_A50 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "1 2.456783 3 4 5 6 7 8", "1 2 3 4 5 6 7 8", "1 6.0357827090890011 27 256 3125 46656 823543 16777216",len);
    }
};

class TestPow_64f_A53 : public SrcSrcDstLen< F64,F64,F64 >
{
public:
    TestPow_64f_A53( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64,F64,F64 >( parent, "fwsPow_64f_A53", fwsPow_64f_A53 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "1 2.456783 3 4 5 6 7 8", "1 2 3 4 5 6 7 8", "1 6.0357827090890011 27 256 3125 46656 823543 16777216",len);
    }
};




DEFINE_TEST_TABLE( PowTestCatalog )

TEST_ENTRY( TestPow_32f_A11 )
TEST_ENTRY( TestPow_32f_A21 )
TEST_ENTRY( TestPow_32f_A24 )
TEST_ENTRY( TestPow_64f_A50 )
TEST_ENTRY( TestPow_64f_A53 )


END_TEST_TABLE()


