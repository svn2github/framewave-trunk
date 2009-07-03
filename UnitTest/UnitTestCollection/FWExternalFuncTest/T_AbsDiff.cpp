/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//AbsDiff function
class TestAbsDiff_8u_C1R : public SrcSrcDstRoi< U8, 1, U8, 1 >
{
public:
    TestAbsDiff_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 1, U8, 1 >( parent, "fwiAbsDiff_8u_C1R", fwiAbsDiff_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8", "9 3 MAX 0", "4 3 248 8");
		RunTest( "MAX MAX 0 7", "MAX 10 123 123", "0 245 123 116");
    }
};

class TestAbsDiff_16u_C1R : public SrcSrcDstRoi< U16, 1, U16, 1 >
{
public:
    TestAbsDiff_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 1, U16, 1 >( parent, "fwiAbsDiff_16u_C1R", fwiAbsDiff_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8", "9 3 MAX 0", "4 3 65528 8");
		RunTest( "MAX MAX 0 7", "MAX 10 123 123", "0 65525 123 116");
    }
};

class TestAbsDiff_32f_C1R : public SrcSrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestAbsDiff_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 1, F32, 1 >( parent, "fwiAbsDiff_32f_C1R", fwiAbsDiff_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 6.2 7.3", "9.2 3.3 0.45678", "4.1 2.9 6.84322", 0.00001f);
		RunTest( "MAX MAX 0 3.14159E+37", "MAX 10 1.012e+37 1.012e+36", "0 MAX 1.012e+37 3.04039e+37", 1.0e+34f);
    }
};

//AbdDiffC functions
class TestAbsDiffC_8u_C1R : public SrcDstRoiValI< U8, 1, U8, 1>
{
public:
    TestAbsDiffC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiValI< U8, 1, U8, 1>( parent, "fwiAbsDiffC_8u_C1R", fwiAbsDiffC_8u_C1R )
    {}

    virtual void RunAll()
    {
		//Src, Value, Expected Dst
        RunTest( "5 6 7 8 MAX", "7", "2 1 0 1 248");
		RunTest( "5 6 7 8 MAX", "-7", "5 6 7 8 MAX");
		RunTest( "5 6 7 8 MAX", "300", "250 249 248 247 0");
    }
};

class TestAbsDiffC_16u_C1R : public SrcDstRoiValI< U16, 1, U16, 1>
{
public:
    TestAbsDiffC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiValI< U16, 1, U16, 1>( parent, "fwiAbsDiffC_16u_C1R", fwiAbsDiffC_16u_C1R )
    {}

    virtual void RunAll()
    {
		//Src, Value, Expected Dst
        RunTest( "5 6 7 8 MAX", "7", "2 1 0 1 65528");
		RunTest( "5 6 7 8 MAX", "-7", "5 6 7 8 MAX");
		RunTest( "5 6 7 8 MAX", "65540", "65530 65529 65528 65527 0");
    }
};

class TestAbsDiffC_32f_C1R : public SrcDstRoiVal< F32, 1, F32, 1>
{
public:
    TestAbsDiffC_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoiVal< F32, 1, F32, 1>( parent, "fwiAbsDiffC_32f_C1R", fwiAbsDiffC_32f_C1R )
    {}

    virtual void RunAll()
    {
		//Src, Value, Expected Dst
        RunTest( "5.1 -6.2 7.3 -8.12345", "7", "1.9 13.2 0.3 15.12345", 0.0001f);
		RunTest( "MAX MIN 3.14159e+38 -2.14148e+38", "-2.14159e+38", "INF 1.26123e+38f INF 1.1e+34f", 2.0e+33f);
    }
};

DEFINE_TEST_TABLE( AbsDiffTestCatalog )
//AbsDiff
TEST_ENTRY( TestAbsDiff_8u_C1R )
TEST_ENTRY( TestAbsDiff_16u_C1R )
TEST_ENTRY( TestAbsDiff_32f_C1R )

//AbsDiffC
TEST_ENTRY( TestAbsDiffC_8u_C1R )
TEST_ENTRY( TestAbsDiffC_16u_C1R )
TEST_ENTRY( TestAbsDiffC_32f_C1R )

END_TEST_TABLE()
