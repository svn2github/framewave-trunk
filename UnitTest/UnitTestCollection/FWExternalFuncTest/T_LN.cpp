/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer data.
class TestLN_8u_C1RSfs : public SrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestLN_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiLn_8u_C1RSfs", fwiLn_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 MAX", "0 1 4  6", 0 );
		RunTest( "1 2 39 MAX", "0 1 7 11", -1 );
		RunTest( "0", "0", 0, fwStsLnZeroArg );
    }
};

class TestLN_8u_C3RSfs : public SrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestLN_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiLn_8u_C3RSfs", fwiLn_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 100 200 MAX", "0 1 4 5 5 6", 0 );
		RunTest( "1 2 39 100 200 MAX", "0 1 7 9 11 11", -1 );
		RunTest( "0 0 0", "0 0 0", 0, fwStsLnZeroArg );
    }
};

class TestLN_16s_C1RSfs : public SrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestLN_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiLn_16s_C1RSfs", fwiLn_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 MAX", "0 1 4 10", 0 );
		RunTest( "1 2 39 MAX", "0 1 7 21", -1 );
		RunTest( "0", "MIN", 0, fwStsLnZeroArg );
		RunTest( "-5 MIN", "MIN MIN", 0);
    }
};

class TestLN_16s_C3RSfs : public SrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestLN_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiLn_16s_C3RSfs", fwiLn_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 10000 30000 MAX", "0 1 4  9 10 10", 0 );
		RunTest( "1 2 39 10000 30000 MAX", "0 1 7 18 21 21", -1 );
		RunTest( "0 0 0", "MIN MIN MIN", 0, fwStsLnZeroArg );
		RunTest( "-5 -6000 MIN", "MIN MIN MIN", 0);
    }
};

//Case 2: Not-in-place operation on floating-point data
class TestLN_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestLN_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiLn_32f_C1R", fwiLn_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2.123 39.456 MAX 1.5e+38 1.5e-38", "0 0.75283 3.675186 88.7228 87.9037 -87.0927", 0.0001f );
		//Wrong return values for the following two cases
		//RunTest( "0", "INF_NEG", 1.0e+37f, fwStsLnZeroArg );
		//The following expected answer should be IND_NEG
		//RunTest( "-5.1 -100.234 MIN", "0 0 0", 1.0e+37f);
    }
};

class TestLN_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestLN_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiLn_32f_C3R", fwiLn_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2.123 39.456 MAX 1.5e+38 1.5e-38", "0 0.75283 3.675186 88.7228 87.9037 -87.0927", 0.0001f );
		//Wrong return values for the following two cases
		//RunTest( "0", "INF_NEG", 1.0e+37f, fwStsLnZeroArg );
		//The following expected answer should be IND_NEG
		//RunTest( "-5.1 -100.234 MIN", "0 0 0", 1.0e+37f);
    }
};

//Case 3: In-place operation on integer data.
class TestLN_8u_C1IRSfs : public SDstRoiScale< U8, 1 >
{
public:
    TestLN_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 1>( parent, "fwiLn_8u_C1IRSfs", fwiLn_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 MAX", "0 1 4  6", 0 );
		RunTest( "1 2 39 MAX", "0 1 7 11", -1 );
		RunTest( "0", "0", 0, fwStsLnZeroArg );
    }
};

class TestLN_8u_C3IRSfs : public SDstRoiScale< U8, 3 >
{
public:
    TestLN_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 3 >( parent, "fwiLn_8u_C3IRSfs", fwiLn_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 100 200 MAX", "0 1 4 5 5 6", 0 );
		RunTest( "1 2 39 100 200 MAX", "0 1 7 9 11 11", -1 );
		RunTest( "0 0 0", "0 0 0", 0, fwStsLnZeroArg );
    }
};

class TestLN_16s_C1IRSfs : public SDstRoiScale< S16, 1 >
{
public:
    TestLN_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 1>( parent, "fwiLn_16s_C1IRSfs", fwiLn_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 MAX", "0 1 4 10", 0 );
		RunTest( "1 2 39 MAX", "0 1 7 21", -1 );
		RunTest( "0", "MIN", 0, fwStsLnZeroArg );
		RunTest( "-5 MIN", "MIN MIN", 0);
    }
};

class TestLN_16s_C3IRSfs : public SDstRoiScale< S16, 3>
{
public:
    TestLN_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 3>( parent, "fwiLn_16s_C3IRSfs", fwiLn_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 39 10000 30000 MAX", "0 1 4  9 10 10", 0 );
		RunTest( "1 2 39 10000 30000 MAX", "0 1 7 18 21 21", -1 );
		RunTest( "0 0 0", "MIN MIN MIN", 0, fwStsLnZeroArg );
		RunTest( "-5 -6000 MIN", "MIN MIN MIN", 0);
    }
};

//Case 4: In-place operation on floating-point data
class TestLN_32f_C1IR : public SDstRoi< F32, 1>
{
public:
    TestLN_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 1>( parent, "fwiLn_32f_C1IR", fwiLn_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2.123 39.456 MAX 1.5e+38 1.5e-38", "0 0.75283 3.675186 88.7228 87.9037 -87.0927", 0.0001f );
		//Wrong return values for the following two cases
		//RunTest( "0", "INF_NEG", 1.0e+37f, fwStsLnZeroArg );
		//The following expected answer should be IND_NEG
		//RunTest( "-5.1 -100.234 MIN", "0 0 0", 1.0e+37f);
    }
};

class TestLN_32f_C3IR : public SDstRoi< F32, 3 >
{
public:
    TestLN_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 3 >( parent, "fwiLn_32f_C3IR", fwiLn_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2.123 39.456 MAX 1.5e+38 1.5e-38", "0 0.75283 3.675186 88.7228 87.9037 -87.0927", 0.0001f );
		//Wrong return values for the following two cases
		//RunTest( "0", "INF_NEG", 1.0e+37f, fwStsLnZeroArg );
		//The following expected answer should be IND_NEG
		//RunTest( "-5.1 -100.234 MIN", "0 0 0", 1.0e+37f);
    }
};

DEFINE_TEST_TABLE( LnTestCatalog )
//Case 1
TEST_ENTRY( TestLN_8u_C1RSfs )
TEST_ENTRY( TestLN_8u_C3RSfs )
TEST_ENTRY( TestLN_16s_C1RSfs )
TEST_ENTRY( TestLN_16s_C3RSfs )
//Case 2
TEST_ENTRY( TestLN_32f_C1R )
TEST_ENTRY( TestLN_32f_C3R )
//Case 3
TEST_ENTRY( TestLN_8u_C1IRSfs )
TEST_ENTRY( TestLN_8u_C3IRSfs )
TEST_ENTRY( TestLN_16s_C1IRSfs )
TEST_ENTRY( TestLN_16s_C3IRSfs )
//Case 4
TEST_ENTRY( TestLN_32f_C1IR )
TEST_ENTRY( TestLN_32f_C3IR )
END_TEST_TABLE()
