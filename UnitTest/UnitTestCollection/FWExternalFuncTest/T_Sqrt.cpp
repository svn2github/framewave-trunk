/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer data.
class TestSqrt_8u_C1RSfs : public SrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestSqrt_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiSqrt_8u_C1RSfs", fwiSqrt_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 MAX", "2 3 4 11 16", 0);
		RunTest( "5 7 17 121 MAX", "4 5 8 22 32", -1);
    }
};

class TestSqrt_8u_C3RSfs : public SrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestSqrt_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiSqrt_8u_C3RSfs", fwiSqrt_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 16", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 32", -1);
    }
};

class TestSqrt_8u_AC4RSfs : public SrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSqrt_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSqrt_8u_AC4RSfs", fwiSqrt_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 9 121 168 MAX 25", "2 3 4 0 11 13 16 0", 0);
		RunTest( "5 7 17 9 121 168 MAX 25", "4 5 8 0 22 26 32 0", -1);
    }
};

class TestSqrt_16u_C1RSfs : public SrcDstRoiScale< U16, 1, U16, 1 >
{
public:
    TestSqrt_16u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 1, U16, 1 >( parent, "fwiSqrt_16u_C1RSfs", fwiSqrt_16u_C1RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 MAX", "2 3 4 11 256", 0);
		RunTest( "5 7 17 121 MAX", "4 5 8 22 512", -1);
    }
};

class TestSqrt_16u_C3RSfs : public SrcDstRoiScale< U16, 3, U16, 3 >
{
public:
    TestSqrt_16u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 3, U16, 3 >( parent, "fwiSqrt_16u_C3RSfs", fwiSqrt_16u_C3RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 256", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 512", -1);
    }
};

class TestSqrt_16u_AC4RSfs : public SrcDstRoiScale< U16, 4, U16, 4 >
{
public:
    TestSqrt_16u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< U16, 4, U16, 4 >( parent, "fwiSqrt_16u_AC4RSfs", fwiSqrt_16u_AC4RSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 100 121 168 MAX 100", "2 3 4 0 11 13 256 0", 0);
		RunTest( "5 7 17 100 121 168 MAX 100", "4 5 8 0 22 26 512 0", -1);
    }
};

class TestSqrt_16s_C1RSfs : public SrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestSqrt_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiSqrt_16s_C1RSfs", fwiSqrt_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 100 121 168 MAX 100", "2 3 4 10 11 13 181 10", 0);
		RunTest( "5 7 17 100 121 168 MAX 100", "4 5 8 20 22 26 362 20", -1);
		//Wrong error return
		//RunTest( "0", "0", 1, 0);
    }
};

class TestSqrt_16s_C3RSfs : public SrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestSqrt_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiSqrt_16s_C3RSfs", fwiSqrt_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 181", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 362", -1);
		//Wrong error return
		//RunTest( "0 -6 -7", "0", 1, 0);
    }
};

class TestSqrt_16s_AC4RSfs : public SrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSqrt_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSqrt_16s_AC4RSfs", fwiSqrt_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 100 121 168 MAX 100", "2 3 4 0 11 13 181 0", 0);
		RunTest( "5 7 17 100 121 168 MAX 100", "4 5 8 0 22 26 362 0", -1);
		//Wrong error return
		//RunTest( "0 -6 -7 -8", "0", 1, 0);
    }
};

//Case 2: Not-in-place operation on floating-point data
class TestSqrt_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestSqrt_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiSqrt_32f_C1R", fwiSqrt_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "2.25832 2.48998 2.710166 0.1", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX 0", "4.294182e+18 1.378405e+18 1.84467e+19 0", 1.0e+15f);
		//Wrong error return
		//RunTest( "-5 -6.2 -7.2 -8.4", "0", 0);
    }
};

class TestSqrt_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestSqrt_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiSqrt_32f_C3R", fwiSqrt_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345", "2.25832 2.48998 2.710166", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX", "4.294182e+18 1.378405e+18 1.84467e+19", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2", "0", 0);
    }
};

class TestSqrt_32f_AC4R : public SrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestSqrt_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiSqrt_32f_AC4R", fwiSqrt_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "2.25832 2.48998 2.710166 0", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX 1.23e+32", "4.294182e+18 1.378405e+18 1.84467e+19 0", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2 -8.4", "0", 0);
    }
};

//Case 3: In-place operation on integer data
class TestSqrt_8u_C1IRSfs : public SDstRoiScale< U8, 1 >
{
public:
    TestSqrt_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 1>( parent, "fwiSqrt_8u_C1IRSfs", fwiSqrt_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 MAX", "2 3 4 11 16", 0);
		RunTest( "5 7 17 121 MAX", "4 5 8 22 32", -1);
    }
};

class TestSqrt_8u_C3IRSfs : public SDstRoiScale< U8, 3 >
{
public:
    TestSqrt_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 3 >( parent, "fwiSqrt_8u_C3IRSfs", fwiSqrt_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 16", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 32", -1);
    }
};

class TestSqrt_8u_AC4IRSfs : public SDstRoiScale< U8, 4 >
{
public:
    TestSqrt_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U8, 4 >( parent, "fwiSqrt_8u_AC4IRSfs", fwiSqrt_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 9 121 168 MAX 25", "2 3 4 9 11 13 16 25", 0);
		RunTest( "5 7 17 9 121 168 MAX 25", "4 5 8 9 22 26 32 25", -1);
    }
};

class TestSqrt_16u_C1IRSfs : public SDstRoiScale< U16, 1 >
{
public:
    TestSqrt_16u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 1 >( parent, "fwiSqrt_16u_C1IRSfs", fwiSqrt_16u_C1IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 MAX", "2 3 4 11 256", 0);
		RunTest( "5 7 17 121 MAX", "4 5 8 22 512", -1);
    }
};

class TestSqrt_16u_C3IRSfs : public SDstRoiScale< U16, 3 >
{
public:
    TestSqrt_16u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 3 >( parent, "fwiSqrt_16u_C3IRSfs", fwiSqrt_16u_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 256", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 512", -1);
    }
};

class TestSqrt_16u_AC4IRSfs : public SDstRoiScale< U16, 4 >
{
public:
    TestSqrt_16u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< U16, 4 >( parent, "fwiSqrt_16u_AC4IRSfs", fwiSqrt_16u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
		//inconsistent rounding between reference and SSE2, reference code rounding seems wrong
        RunTest( "5 7 17 100 121 168 MAX 100", "2 3 4 100 11 13 256 100", 0);
		RunTest( "5 7 17 100 121 168 MAX 100", "4 5 8 100 22 26 512 100", -1);
    }
};

class TestSqrt_16s_C1IRSfs : public SDstRoiScale< S16, 1 >
{
public:
    TestSqrt_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 1 >( parent, "fwiSqrt_16s_C1IRSfs", fwiSqrt_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 100 121 168 MAX 100", "2 3 4 10 11 13 181 10", 0);
		RunTest( "5 7 17 100 121 168 MAX 100", "4 5 8 20 22 26 362 20", -1);
		//Wrong error return
		//RunTest( "0", "0", 1, 0);
    }
};

class TestSqrt_16s_C3IRSfs : public SDstRoiScale< S16, 3 >
{
public:
    TestSqrt_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 3 >( parent, "fwiSqrt_16s_C3IRSfs", fwiSqrt_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 121 168 MAX", "2 3 4 11 13 181", 0);
		RunTest( "5 7 17 121 168 MAX", "4 5 8 22 26 362", -1);
		//Wrong error return
		//RunTest( "-5 -6 -7", "0", 1, 0);
    }
};

class TestSqrt_16s_AC4IRSfs : public SDstRoiScale< S16, 4 >
{
public:
    TestSqrt_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SDstRoiScale< S16, 4 >( parent, "fwiSqrt_16s_AC4IRSfs", fwiSqrt_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 17 100 121 168 MAX -100", "2 3 4 100 11 13 181 -100", 0);
		RunTest( "5 7 17 100 121 168 MAX -100", "4 5 8 100 22 26 362 -100", -1);
		//Wrong error return
		//RunTest( "0 -6 -7 -8", "0", 1, 0);
    }
};

//Case 4: In-place operation on floating-point data.
class TestSqrt_32f_C1IR : public SDstRoi< F32, 1 >
{
public:
    TestSqrt_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 1 >( parent, "fwiSqrt_32f_C1IR", fwiSqrt_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "2.25832 2.48998 2.710166 0.1", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX 0", "4.294182e+18 1.378405e+18 1.84467e+19 0", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2 -8.4", "0", 0);
    }
};

class TestSqrt_32f_C3IR : public SDstRoi< F32, 3 >
{
public:
    TestSqrt_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 3 >( parent, "fwiSqrt_32f_C3IR", fwiSqrt_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345", "2.25832 2.48998 2.710166", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX", "4.294182e+18 1.378405e+18 1.84467e+19", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2", "0", 0);
    }
};

class TestSqrt_32f_C4IR : public SDstRoi< F32, 4 >
{
public:
    TestSqrt_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiSqrt_32f_C4IR", fwiSqrt_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 0.01", "2.25832 2.48998 2.710166 0.1", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX 1.23e+32", "4.294182e+18 1.378405e+18 1.84467e+19 1.109e+16", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2 -8.4", "0", 0);
    }
};

class TestSqrt_32f_AC4IR : public SDstRoi< F32, 4 >
{
public:
    TestSqrt_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiSqrt_32f_AC4IR", fwiSqrt_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.345 -0.01", "2.25832 2.48998 2.710166 -0.01", 0.0001f);
		RunTest( "1.844e+37 1.9e+36 MAX -1.23e+18", "4.294182e+18 1.378405e+18 1.84467e+19 -1.23e+18", 1.0e+15f);
		//Wrong error return
		//RunTest( "0 -6.2 -7.2 -8.4", "0", 0);
    }
};

DEFINE_TEST_TABLE( SqrtTestCatalog )
//Case 1
TEST_ENTRY( TestSqrt_8u_C1RSfs )
TEST_ENTRY( TestSqrt_8u_C3RSfs )
TEST_ENTRY( TestSqrt_8u_AC4RSfs )
TEST_ENTRY( TestSqrt_16u_C1RSfs )
TEST_ENTRY( TestSqrt_16u_C3RSfs )
TEST_ENTRY( TestSqrt_16u_AC4RSfs )
TEST_ENTRY( TestSqrt_16s_C1RSfs )
TEST_ENTRY( TestSqrt_16s_C3RSfs )
TEST_ENTRY( TestSqrt_16s_AC4RSfs )
//Case 2
TEST_ENTRY( TestSqrt_32f_C1R )
TEST_ENTRY( TestSqrt_32f_C3R )
TEST_ENTRY( TestSqrt_32f_AC4R )
//Case 3
TEST_ENTRY( TestSqrt_8u_C1IRSfs )
TEST_ENTRY( TestSqrt_8u_C3IRSfs )
TEST_ENTRY( TestSqrt_8u_AC4IRSfs )
TEST_ENTRY( TestSqrt_16u_C1IRSfs )
TEST_ENTRY( TestSqrt_16u_C3IRSfs )
TEST_ENTRY( TestSqrt_16u_AC4IRSfs )
TEST_ENTRY( TestSqrt_16s_C1IRSfs )
TEST_ENTRY( TestSqrt_16s_C3IRSfs )
TEST_ENTRY( TestSqrt_16s_AC4IRSfs )
//Case 4
TEST_ENTRY( TestSqrt_32f_C1IR )
TEST_ENTRY( TestSqrt_32f_C3IR )
TEST_ENTRY( TestSqrt_32f_C4IR )
TEST_ENTRY( TestSqrt_32f_AC4IR )
END_TEST_TABLE()
