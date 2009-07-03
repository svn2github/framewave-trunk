/* 
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FixedAccuracy.h"

#include "FunctionSignatures.h"
#include "fwSignal.h"


class TestPowx_32f_A11 : public SrcConstDstLen< F32,F32 >
{
public:
    TestPowx_32f_A11( UnitTestCatalogBase & parent ) : 
	  SrcConstDstLen< F32,F32 >( parent, "fwsPowx_32f_A11", fwsPowx_32f_A11 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        F32 cval = 2;
        RunTest( "1 2.456783 3 4 5 6 7 8", cval, "1 6.0357828 9 16 25 36 49 64",len);
    }
};


class TestPowx_32f_A21 : public SrcConstDstLen< F32,F32 >
{
public:
    TestPowx_32f_A21( UnitTestCatalogBase & parent ) : 
	  SrcConstDstLen< F32,F32 >( parent, "fwsPowx_32f_A21", fwsPowx_32f_A21 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        F32 cval = 2;
        RunTest( "1 2.456783 3 4 5 6 7 8", cval, "1 6.0357828 9 16 25 36 49 64",len);
    }
};


class TestPowx_32f_A24 : public SrcConstDstLen< F32,F32 >
{
public:
    TestPowx_32f_A24( UnitTestCatalogBase & parent ) : 
	  SrcConstDstLen< F32,F32 >( parent, "fwsPowx_32f_A24", fwsPowx_32f_A24 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        F32 cval = 2;
        RunTest( "1 2.456783 3 4 5 6 7 8", cval, "1 6.0357827090890011 9 16 25 36 49 64",len);
    }
};

class TestPowx_64f_A50 : public SrcConstDstLen< F64,F64 >
{
public:
    TestPowx_64f_A50( UnitTestCatalogBase & parent ) : 
	  SrcConstDstLen< F64,F64 >( parent, "fwsPowx_64f_A50", fwsPowx_64f_A50 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        F32 cval = 2;
        RunTest( "1 2.456783 3 4 5 6 7 8", cval, "1 6.0357827090890011 9 16 25 36 49 64",len);
    }
};

class TestPowx_64f_A53 : public SrcConstDstLen< F64,F64 >
{
public:
    TestPowx_64f_A53( UnitTestCatalogBase & parent ) : 
	  SrcConstDstLen< F64,F64 >( parent, "fwsPowx_64f_A53", fwsPowx_64f_A53 )
    {}

    virtual void RunAll()
    {
        int len = 8;
        F32 cval = 2;
        RunTest( "1 2.456783 3 4 5 6 7 8", cval, "1 6.0357827090890011 9 16 25 36 49 64",len);
    }
};



DEFINE_TEST_TABLE( PowXTestCatalog )

TEST_ENTRY( TestPowx_32f_A11 )
TEST_ENTRY( TestPowx_32f_A21 )
TEST_ENTRY( TestPowx_32f_A24 )
TEST_ENTRY( TestPowx_64f_A50 )
TEST_ENTRY( TestPowx_64f_A53 )


END_TEST_TABLE()
