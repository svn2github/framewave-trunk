/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer data.
class TestExp_8u_C1RSfs : public SrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestExp_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiExp_8u_C1RSfs", fwiExp_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 5 6 MAX 0", "3 7 148 MAX MAX 1", 0 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 5 6 MAX 0", "1 4  74 202 MAX 1", 1 );
    }
};

class TestExp_8u_C3RSfs : public SrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestExp_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiExp_8u_C3RSfs", fwiExp_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 5 6 MAX 0", "3 7 148 MAX MAX 1", 0 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 5 6 MAX 0", "1 4  74 202 MAX 1", 1 );
    }
};

class TestExp_16s_C1RSfs : public SrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestExp_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiExp_16s_C1RSfs", fwiExp_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 10 11 MAX 0", "3 7 22026   MAX MAX 1", 0 );
		RunTest( "-1 -2 -3 -4 -5 MIN", "24 9 3 1 0 0", -6 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 10 11 MAX 0", "1 4 11013 29937 MAX 1", 1 );
    }
};

class TestExp_16s_C3RSfs : public SrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestExp_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiExp_16s_C3RSfs", fwiExp_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 10 11 MAX 0", "3 7 22026   MAX MAX 1", 0 );
		RunTest( "-1 -2 -3 -4 -5 MIN", "24 9 3 1 0 0", -6 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 10 11 MAX 0", "1 4 11013 29937 MAX 1", 1 );
    }
};

//Case 2: Not-in-place operation on floating-point data
class TestExp_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestExp_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiExp_32f_C1R", fwiExp_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2.123 1 0.0015 0 -0.0015 -1 -2.123 -7.01 MIN", 
			"8.35617 2.71828 1.0015 1 0.9985 0.36788 0.11967 9.028e-4 0", 0.0001f );
		RunTest( "88 89 MAX", "1.65164e+38 INF INF", 1.0e+34f);
    }
};

class TestExp_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestExp_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiExp_32f_C3R", fwiExp_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "2.123 1 0.0015 0 -0.0015 -1 -2.123 -7.01 MIN", 
			"8.35617 2.71828 1.0015 1 0.9985 0.36788 0.11967 9.028e-4 0", 0.0001f );
		RunTest( "88 89 MAX", "1.65164e+38 INF INF", 1.0e+34f);
    }
};

//Case 3: In-place operation on integer data.
class TestExp_8u_C1IRSfs : public SDstRoiScale< U8, 1>
{
public:
    TestExp_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 1>( parent, "fwiExp_8u_C1IRSfs", fwiExp_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 5 6 MAX 0", "3 7 148 MAX MAX 1", 0 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 5 6 MAX 0", "1 4  74 202 MAX 1", 1 );
    }
};

class TestExp_8u_C3IRSfs : public SDstRoiScale< U8, 3 >
{
public:
    TestExp_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 3 >( parent, "fwiExp_8u_C3IRSfs", fwiExp_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 5 6 MAX 0", "3 7 148 MAX MAX 1", 0 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 5 6 MAX 0", "1 4  74 202 MAX 1", 1 );
    }
};

class TestExp_16s_C1IRSfs : public SDstRoiScale< S16, 1 >
{
public:
    TestExp_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 1 >( parent, "fwiExp_16s_C1IRSfs", fwiExp_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 10 11 MAX 0", "3 7 22026   MAX MAX 1", 0 );
		RunTest( "-1 -2 -3 -4 -5 MIN", "24 9 3 1 0 0", -6 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 10 11 MAX 0", "1 4 11013 29937 MAX 1", 1 );
    }
};

class TestExp_16s_C3IRSfs : public SDstRoiScale< S16, 3 >
{
public:
    TestExp_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 3 >( parent, "fwiExp_16s_C3IRSfs", fwiExp_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 10 11 MAX 0", "3 7 22026   MAX MAX 1", 0 );
		RunTest( "-1 -2 -3 -4 -5 MIN", "24 9 3 1 0 0", -6 );
		//incorrect rounding, should use round-to-even
		RunTest( "1 2 10 11 MAX 0", "1 4 11013 29937 MAX 1", 1 );
    }
};

//Case 4: In-place operation on floating-point data
class TestExp_32f_C1IR : public SDstRoi< F32, 1 >
{
public:
    TestExp_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 1 >( parent, "fwiExp_32f_C1IR", fwiExp_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2.123 1 0.0015 0 -0.0015 -1 -2.123 -7.01 MIN", 
			"8.35617 2.71828 1.0015 1 0.9985 0.36788 0.11967 9.028e-4 0", 0.0001f );
		RunTest( "88 89 MAX", "1.65164e+38 INF INF", 1.0e+34f);
    }
};

class TestExp_32f_C3IR : public SDstRoi< F32, 3 >
{
public:
    TestExp_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 3 >( parent, "fwiExp_32f_C3IR", fwiExp_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2.123 1 0.0015 0 -0.0015 -1 -2.123 -7.01 MIN", 
			"8.35617 2.71828 1.0015 1 0.9985 0.36788 0.11967 9.028e-4 0", 0.0001f );
		RunTest( "88 89 MAX", "1.65164e+38 INF INF", 1.0e+34f);
    }
};


DEFINE_TEST_TABLE( ExpTestCatalog )
//Case 1
TEST_ENTRY( TestExp_8u_C1RSfs )
TEST_ENTRY( TestExp_8u_C3RSfs )
TEST_ENTRY( TestExp_16s_C1RSfs )
TEST_ENTRY( TestExp_16s_C3RSfs )
//Case 2
TEST_ENTRY( TestExp_32f_C1R )
TEST_ENTRY( TestExp_32f_C3R )
//Case 3
TEST_ENTRY( TestExp_8u_C1IRSfs )
TEST_ENTRY( TestExp_8u_C3IRSfs )
TEST_ENTRY( TestExp_16s_C1IRSfs )
TEST_ENTRY( TestExp_16s_C3IRSfs )
//Case 4
TEST_ENTRY( TestExp_32f_C1IR )
TEST_ENTRY( TestExp_32f_C3IR )
END_TEST_TABLE()
