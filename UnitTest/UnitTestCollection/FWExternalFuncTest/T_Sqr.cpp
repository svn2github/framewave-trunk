/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer data.
class TestSqr_8u_C1RSfs : public SrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestSqr_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiSqr_8u_C1RSfs", fwiSqr_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 17 MAX", "25 36 49 MAX MAX", 0);
		RunTest( "5 6 7 17 MAX", "12 18 24 144 MAX", 1);
    }
};

class TestSqr_8u_C3RSfs : public SrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestSqr_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiSqr_8u_C3RSfs", fwiSqr_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 17 18 MAX", "25 36 49 MAX MAX MAX", 0);
		RunTest( "5 6 7 17 18 MAX", "12 18 24 144 162 MAX", 1);
    }
};

class TestSqr_8u_C4RSfs : public SrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSqr_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSqr_8u_C4RSfs", fwiSqr_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8 17 18 MAX 19", "25 36 49 64 MAX MAX MAX MAX", 0);
		RunTest( "5 6 7 8 17 18 MAX 19", "12 18 24 32 144 162 MAX 180", 1);
    }
};

class TestSqr_8u_AC4RSfs : public SrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSqr_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSqr_8u_AC4RSfs", fwiSqr_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8 17 18 MAX 19", "25 36 49 0 MAX MAX MAX 0", 0);
		RunTest( "5 6 7 8 17 18 MAX 19", "12 18 24 0 144 162 MAX 0", 1);
    }
};

class TestSqr_16u_C1RSfs : public SrcDstRoiScale< U16, 1, U16, 1 >
{
public:
    TestSqr_16u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 1, U16, 1 >( parent, "fwiSqr_16u_C1RSfs", fwiSqr_16u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 301 MAX", "25 49 289   MAX MAX", 0);
		RunTest( "5 7 17 301 MAX", "12 24 144 45300 MAX", 1);
    }
};

class TestSqr_16u_C3RSfs : public SrcDstRoiScale< U16, 3, U16, 3 >
{
public:
    TestSqr_16u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 3, U16, 3 >( parent, "fwiSqr_16u_C3RSfs", fwiSqr_16u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 300 301 MAX", "25 49 289   MAX   MAX MAX", 0);
		RunTest( "5 7 17 300 301 MAX", "12 24 144 45000 45300 MAX", 1);
    }
};

class TestSqr_16u_C4RSfs : public SrcDstRoiScale< U16, 4, U16, 4 >
{
public:
    TestSqr_16u_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 4, U16, 4 >( parent, "fwiSqr_16u_C4RSfs", fwiSqr_16u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 255 300 301 MAX 256", "25 49 289 65025   MAX   MAX MAX   MAX", 0);
		RunTest( "5 7 17 255 300 301 MAX 256", "12 24 144 32512 45000 45300 MAX 32768", 1);
    }
};

class TestSqr_16u_AC4RSfs : public SrcDstRoiScale< U16, 4, U16, 4 >
{
public:
    TestSqr_16u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 4, U16, 4 >( parent, "fwiSqr_16u_AC4RSfs", fwiSqr_16u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 255 300 301 MAX 256", "25 49 289 0   MAX   MAX MAX 0", 0);
		RunTest( "5 7 17 255 300 301 MAX 256", "12 24 144 0 45000 45300 MAX 0", 1);
    }
};

class TestSqr_16s_C1RSfs : public SrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestSqr_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiSqr_16s_C1RSfs", fwiSqr_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 17 -181 MAX MIN", "25 49 289 32761 MAX MAX", 0);
		RunTest( "5 -7 17 -181 MAX MIN", "12 24 144 16380 MAX MAX", 1);
    }
};

class TestSqr_16s_C3RSfs : public SrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestSqr_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiSqr_16s_C3RSfs", fwiSqr_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 -181 MAX MIN", "25 49   MAX 32761 MAX MAX", 0);
		RunTest( "5 -7 182 -181 MAX MIN", "12 24 16562 16380 MAX MAX", 1);
    }
};

class TestSqr_16s_C4RSfs : public SrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSqr_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSqr_16s_C4RSfs", fwiSqr_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 65 -181 MAX MIN -75", "25 49   MAX 4225 32761 MAX MAX 5625", 0);
		RunTest( "5 -7 182 65 -181 MAX MIN -75", "12 24 16562 2112 16380 MAX MAX 2812", 1);
    }
};

class TestSqr_16s_AC4RSfs : public SrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSqr_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSqr_16s_AC4RSfs", fwiSqr_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 65 -181 MAX MIN -75", "25 49   MAX 0 32761 MAX MAX 0", 0);
		RunTest( "5 -7 182 65 -181 MAX MIN -75", "12 24 16562 0 16380 MAX MAX 0", 1);
    }
};

//Case 2: Not-in-place operation on floating-point data
class TestSqr_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestSqr_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiSqr_32f_C1R", fwiSqr_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "26.01 38.44 53.94903 0.0001", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX MIN ", "3.400336e+38 INF INF INF", 1.0e+34f);
    }
};

class TestSqr_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestSqr_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiSqr_32f_C3R", fwiSqr_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01 -0.12 -3.14159", "26.01 38.44 53.94903 0.0001 0.0144 9.86959", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX MIN 1.844e+19 1.9e+20", 
			"3.400336e+38 INF INF INF 3.400336e+38 INF", 1.0e+34f);
    }
};

//Missing function
//class TestSqr_32f_C4R : public SrcDstRoi< F32, 4, F32, 4 >
//{
//public:
//    TestSqr_32f_C4R( UnitTestCatalogBase & parent ) : 
//	  SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiSqr_32f_C4R", fwiSqr_32f_C4R )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5.1 6.2 7.345 -8.123 0.01 -0.12 -3.14159 0.001", 
//			"26.01 38.44 53.94903 65.98313 0.0001 0.0144 9.86959 0", 0.0001f);
//		RunTest( "-1.844e+19 -1.9e+20 MAX 1.7e+18 MIN 1.844e+19 1.9e+20 1.6e+19", 
//			"3.400336e+38 INF INF 2.89e+36 INF 3.400336e+38 INF 2.56e+38", 1.0e+34f);
//    }
//};

class TestSqr_32f_AC4R : public SrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestSqr_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiSqr_32f_AC4R", fwiSqr_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 -8.123 0.01 -0.12 -3.14159 0.001", 
			"26.01 38.44 53.94903 0 0.0001 0.0144 9.86959 0", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX 1.7e+18 MIN 1.844e+19 1.9e+20 1.6e+19", 
			"3.400336e+38 INF INF 0 INF 3.400336e+38 INF 0", 1.0e+34f);
    }
};

//Case 3: In-place operation on integer data
class TestSqr_8u_C1IRSfs : public SDstRoiScale< U8, 1 >
{
public:
    TestSqr_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 1>( parent, "fwiSqr_8u_C1IRSfs", fwiSqr_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 17 MAX", "25 36 49 MAX MAX", 0);
		RunTest( "5 6 7 17 MAX", "12 18 24 144 MAX", 1);
    }
};

class TestSqr_8u_C3IRSfs : public SDstRoiScale< U8, 3 >
{
public:
    TestSqr_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 3 >( parent, "fwiSqr_8u_C3IRSfs", fwiSqr_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 17 18 MAX", "25 36 49 MAX MAX MAX", 0);
		RunTest( "5 6 7 17 18 MAX", "12 18 24 144 162 MAX", 1);
    }
};

class TestSqr_8u_C4IRSfs : public SDstRoiScale< U8, 4 >
{
public:
    TestSqr_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 4 >( parent, "fwiSqr_8u_C4IRSfs", fwiSqr_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8 17 18 MAX 19", "25 36 49 64 MAX MAX MAX MAX", 0);
		RunTest( "5 6 7 8 17 18 MAX 19", "12 18 24 32 144 162 MAX 180", 1);
    }
};

class TestSqr_8u_AC4IRSfs : public SDstRoiScale< U8, 4 >
{
public:
    TestSqr_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 4 >( parent, "fwiSqr_8u_AC4IRSfs", fwiSqr_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8 17 18 MAX 19", "25 36 49 8 MAX MAX MAX 19", 0);
		RunTest( "5 6 7 8 17 18 MAX 19", "12 18 24 8 144 162 MAX 19", 1);
    }
};

class TestSqr_16u_C1IRSfs : public SDstRoiScale< U16, 1 >
{
public:
    TestSqr_16u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 1 >( parent, "fwiSqr_16u_C1IRSfs", fwiSqr_16u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 301 MAX", "25 49 289   MAX MAX", 0);
		RunTest( "5 7 17 301 MAX", "12 24 144 45300 MAX", 1);
    }
};

class TestSqr_16u_C3IRSfs : public SDstRoiScale< U16, 3 >
{
public:
    TestSqr_16u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 3 >( parent, "fwiSqr_16u_C3IRSfs", fwiSqr_16u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 300 301 MAX", "25 49 289   MAX   MAX MAX", 0);
		RunTest( "5 7 17 300 301 MAX", "12 24 144 45000 45300 MAX", 1);
    }
};

class TestSqr_16u_C4IRSfs : public SDstRoiScale< U16, 4 >
{
public:
    TestSqr_16u_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 4 >( parent, "fwiSqr_16u_C4IRSfs", fwiSqr_16u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 255 300 301 MAX 256", "25 49 289 65025   MAX   MAX MAX   MAX", 0);
		RunTest( "5 7 17 255 300 301 MAX 256", "12 24 144 32512 45000 45300 MAX 32768", 1);
    }
};

class TestSqr_16u_AC4IRSfs : public SDstRoiScale< U16, 4 >
{
public:
    TestSqr_16u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 4 >( parent, "fwiSqr_16u_AC4IRSfs", fwiSqr_16u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 255 300 301 MAX 256", "25 49 289 255   MAX   MAX MAX 256", 0);
		RunTest( "5 7 17 255 300 301 MAX 256", "12 24 144 255 45000 45300 MAX 256", 1);
    }
};

class TestSqr_16s_C1IRSfs : public SDstRoiScale< S16, 1 >
{
public:
    TestSqr_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 1 >( parent, "fwiSqr_16s_C1IRSfs", fwiSqr_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 17 -181 MAX MIN", "25 49 289 32761 MAX MAX", 0);
		RunTest( "5 -7 17 -181 MAX MIN", "12 24 144 16380 MAX MAX", 1);
    }
};

class TestSqr_16s_C3IRSfs : public SDstRoiScale< S16, 3 >
{
public:
    TestSqr_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 3 >( parent, "fwiSqr_16s_C3IRSfs", fwiSqr_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 -181 MAX MIN", "25 49   MAX 32761 MAX MAX", 0);
		RunTest( "5 -7 182 -181 MAX MIN", "12 24 16562 16380 MAX MAX", 1);
    }
};

class TestSqr_16s_C4IRSfs : public SDstRoiScale< S16, 4 >
{
public:
    TestSqr_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 4 >( parent, "fwiSqr_16s_C4IRSfs", fwiSqr_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 65 -181 MAX MIN -75", "25 49   MAX 4225 32761 MAX MAX 5625", 0);
		RunTest( "5 -7 182 65 -181 MAX MIN -75", "12 24 16562 2112 16380 MAX MAX 2812", 1);
    }
};

class TestSqr_16s_AC4IRSfs : public SDstRoiScale< S16, 4 >
{
public:
    TestSqr_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 4 >( parent, "fwiSqr_16s_AC4IRSfs", fwiSqr_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -7 182 65 -181 MAX MIN -75", "25 49   MAX 65 32761 MAX MAX -75", 0);
		RunTest( "5 -7 182 65 -181 MAX MIN -75", "12 24 16562 65 16380 MAX MAX -75", 1);
    }
};

//Case 4: In-place operation on floating-point data.
class TestSqr_32f_C1IR : public SDstRoi< F32, 1 >
{
public:
    TestSqr_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 1 >( parent, "fwiSqr_32f_C1IR", fwiSqr_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "26.01 38.44 53.94903 0.0001", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX MIN ", "3.400336e+38 INF INF INF", 1.0e+34f);
    }
};

class TestSqr_32f_C3IR : public SDstRoi< F32, 3 >
{
public:
    TestSqr_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 3 >( parent, "fwiSqr_32f_C3IR", fwiSqr_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01 -0.12 -3.14159", "26.01 38.44 53.94903 0.0001 0.0144 9.86959", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX MIN 1.844e+19 1.9e+20", 
			"3.400336e+38 INF INF INF 3.400336e+38 INF", 1.0e+34f);
    }
};

class TestSqr_32f_C4IR : public SDstRoi< F32, 4 >
{
public:
    TestSqr_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiSqr_32f_C4IR", fwiSqr_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 -8.123 0.01 -0.12 -3.14159 0.001", 
			"26.01 38.44 53.94903 65.98313 0.0001 0.0144 9.86959 0", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX 1.7e+18 MIN 1.844e+19 1.9e+20 1.6e+19", 
			"3.400336e+38 INF INF 2.89e+36 INF 3.400336e+38 INF 2.56e+38", 1.0e+34f);
    }
};

class TestSqr_32f_AC4IR : public SDstRoi< F32, 4 >
{
public:
    TestSqr_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiSqr_32f_AC4IR", fwiSqr_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 -8.123 0.01 -0.12 -3.14159 0.001", 
			"26.01 38.44 53.94903 -8.123 0.0001 0.0144 9.86959 0.001", 0.0001f);
		RunTest( "-1.844e+19 -1.9e+20 MAX 1.7e+37 MIN 1.844e+19 1.9e+20 1.6e+36", 
			"3.400336e+38 INF INF 1.7e+37 INF 3.400336e+38 INF 1.6e+36", 1.0e+34f);
    }
};

DEFINE_TEST_TABLE( SqrTestCatalog )
//Case 1
TEST_ENTRY( TestSqr_8u_C1RSfs )
TEST_ENTRY( TestSqr_8u_C3RSfs )
TEST_ENTRY( TestSqr_8u_C4RSfs )
TEST_ENTRY( TestSqr_8u_AC4RSfs )
TEST_ENTRY( TestSqr_16u_C1RSfs )
TEST_ENTRY( TestSqr_16u_C3RSfs )
TEST_ENTRY( TestSqr_16u_C4RSfs )
TEST_ENTRY( TestSqr_16u_AC4RSfs )
TEST_ENTRY( TestSqr_16s_C1RSfs )
TEST_ENTRY( TestSqr_16s_C3RSfs )
TEST_ENTRY( TestSqr_16s_C4RSfs )
TEST_ENTRY( TestSqr_16s_AC4RSfs )
//Case 2
TEST_ENTRY( TestSqr_32f_C1R )
TEST_ENTRY( TestSqr_32f_C3R )
//TEST_ENTRY( TestSqr_32f_C4R )     //Unsupported function
TEST_ENTRY( TestSqr_32f_AC4R )
//Case 3
TEST_ENTRY( TestSqr_8u_C1IRSfs )
TEST_ENTRY( TestSqr_8u_C3IRSfs )
TEST_ENTRY( TestSqr_8u_C4IRSfs )
TEST_ENTRY( TestSqr_8u_AC4IRSfs )
TEST_ENTRY( TestSqr_16u_C1IRSfs )
TEST_ENTRY( TestSqr_16u_C3IRSfs )
TEST_ENTRY( TestSqr_16u_C4IRSfs )
TEST_ENTRY( TestSqr_16u_AC4IRSfs )
TEST_ENTRY( TestSqr_16s_C1IRSfs )
TEST_ENTRY( TestSqr_16s_C3IRSfs )
TEST_ENTRY( TestSqr_16s_C4IRSfs )
TEST_ENTRY( TestSqr_16s_AC4IRSfs )
//Case 4
TEST_ENTRY( TestSqr_32f_C1IR )
TEST_ENTRY( TestSqr_32f_C3IR )
TEST_ENTRY( TestSqr_32f_C4IR )
TEST_ENTRY( TestSqr_32f_AC4IR )
END_TEST_TABLE()
