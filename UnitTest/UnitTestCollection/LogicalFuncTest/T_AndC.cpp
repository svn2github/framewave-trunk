/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel data
class TestAndC_8u_C1R : public SrcValDstRoi< U8, 1, U8, 1 >
{
public:
    TestAndC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U8, 1, U8, 1 >( parent, "fwiAndC_8u_C1R", fwiAndC_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 32 254 MAX", "170", "0 0  2 42  0 32 170 170");
		RunTest( "0 1 22 63 64 32 254 MAX", " 85", "0 1 20 21 64  0  84 85");
    }
};

class TestAndC_16u_C1R : public SrcValDstRoi< U16, 1, U16, 1 >
{
public:
    TestAndC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U16, 1, U16, 1 >( parent, "fwiAndC_16u_C1R", fwiAndC_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 16383 16384 8192 65534 MAX", "43690", "0 0  2 10922     0 8192 43690 43690");
		RunTest( "0 1 22 16383 16384 8192 65534 MAX", "21845", "0 1 20  5461 16384    0 21844 21845");
    }
};

class TestAndC_32s_C1R : public SrcValDstRoi< S32, 1, S32, 1 >
{
public:
    TestAndC_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< S32, 1, S32, 1 >( parent, "fwiAndC_32s_C1R", fwiAndC_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 -1 -2 -16384 MIN MAX", "-1431655766", "0 0  2 -1431655766 -1431655766 -1431666688 MIN  715827882");
		RunTest( "0 1 22 -1 -2 -16384 MIN MAX", " 1431655765", "0 1 20  1431655765  1431655764  1431650304   0 1431655765");
    }
};

//Case 2: Not-in-place operation on multi-channel data
class TestAndC_8u_C3R : public SrcPValDstRoi< U8, 3, U8, 3 >
{
public:
    TestAndC_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 3, U8, 3 >( parent, "fwiAndC_8u_C3R", fwiAndC_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 32 127 254 MAX", "170 42 10", "0 0 2 42 0 0 42 42 10");
		RunTest( "0 1 22 63 64 32 127 254 MAX", " 85 21  5", "0 1 4 21 0 0 85 20  5");
    }
};

class TestAndC_8u_C4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestAndC_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiAndC_8u_C4R", fwiAndC_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 127 254 MAX 213", "170 42 10 165", "0 0 2 37 42 42 10 133");
		RunTest( "0 1 22 63 127 254 MAX 213", " 85 21  5 100", "0 1 4 36 85 20  5  68");
    }
};

class TestAndC_8u_AC4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestAndC_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiAndC_8u_AC4R", fwiAndC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 127 254 MAX 213", "170 42 10 165", "0 0 2 0 42 42 10 0");
		RunTest( "0 1 22 63 127 254 MAX 213", " 85 21  5 100", "0 1 4 0 85 20  5 0");
    }
};

class TestAndC_16u_C3R : public SrcPValDstRoi< U16, 3, U16, 3 >
{
public:
    TestAndC_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 3, U16, 3 >( parent, "fwiAndC_16u_C3R", fwiAndC_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 16383 16384 8192 65534 MAX 345", "43690 12345 10000", 
			"0 1 16 10922     0 8192 43690 12345 272");
		RunTest( "0 1 22 16383 16384 8192 65534 MAX 543", "21845 54320 20000", 
			"0 0  0  5461 16384    0 21844 54320 512");
    }
};

class TestAndC_16u_C4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestAndC_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiAndC_16u_C4R", fwiAndC_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 16383 65534 MAX 345 8192", "43690 12345 10000 2345", 
			"0 1 16 2345 43690 12345 272 0");
		RunTest( "0 1 22 16383 65534 MAX 543 1024", "21845 54320 20000 7878", 
			"0 0  0  7878 21844 54320 512 1024");
    }
};

class TestAndC_16u_AC4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestAndC_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiAndC_16u_AC4R", fwiAndC_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 16383 65534 MAX 345 8192", "43690 12345 10000 2345", 
			"0 1 16 0 43690 12345 272 0");
		RunTest( "0 1 22 16383 65534 MAX 543 1024", "21845 54320 20000 7878", 
			"0 0  0 0 21844 54320 512 0");   
	}
};

class TestAndC_32s_C3R : public SrcPValDstRoi< S32, 3, S32, 3 >
{
public:
    TestAndC_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 3, S32, 3 >( parent, "fwiAndC_32s_C3R", fwiAndC_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 -1 -2 -16384 MIN MAX 2147473546", "-1431655766 -124567890 -987654321", 
			"0 0 6 -1431655766 -124567890 -987660288 0 2022915758  1159827466");
		RunTest( "0 1 22 -1 -2 -16384 MIN MAX 2147473546", " 1431655765  124567890  987654321", 
			"0 0 16 1431655765 124567890 987643904 0 124567890 987646080");

    }
};

class TestAndC_32s_C4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestAndC_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiAndC_32s_C4R", fwiAndC_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", "-1431655766 -124567890 -987654321 -123", 
			"0 0 6 -512 -1431655766 -124567890 -987660288 384 0 2022915758  1159827466 5");
		RunTest( "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", " 1431655765  124567890  987654321  123", 
			"0 0 16 56 1431655765 124567890 987643904 72 0 124567890 987646080 65");
    }
};

class TestAndC_32s_AC4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestAndC_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiAndC_32s_AC4R", fwiAndC_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", "-1431655766 -124567890 -987654321 -123", 
			"0 0 6 0 -1431655766 -124567890 -987660288 0 0 2022915758  1159827466 0");
		RunTest( "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", " 1431655765  124567890  987654321  123", 
			"0 0 16 0 1431655765 124567890 987643904 0 0 124567890 987646080 0");
    }
};

//Case 3: In-place operation on 1-channel data
class TestAndC_8u_C1IR : public ValSDstRoi< U8, 1>
{
public:
    TestAndC_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U8, 1>( parent, "fwiAndC_8u_C1IR", fwiAndC_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "170", "0 1 22 63 64 32 254 MAX", "0 0  2 42  0 32 170 170");
		RunTest( " 85", "0 1 22 63 64 32 254 MAX", "0 1 20 21 64  0  84 85");
    }
};

class TestAndC_16u_C1IR : public ValSDstRoi< U16, 1>
{
public:
    TestAndC_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U16, 1>( parent, "fwiAndC_16u_C1IR", fwiAndC_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "43690", "0 1 22 16383 16384 8192 65534 MAX", "0 0  2 10922     0 8192 43690 43690");
		RunTest( "21845", "0 1 22 16383 16384 8192 65534 MAX", "0 1 20  5461 16384    0 21844 21845");
    }
};

class TestAndC_32s_C1IR : public ValSDstRoi< S32, 1>
{
public:
    TestAndC_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< S32, 1 >( parent, "fwiAndC_32s_C1IR", fwiAndC_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-1431655766", "0 1 22 -1 -2 -16384 MIN MAX", "0 0  2 -1431655766 -1431655766 -1431666688 MIN  715827882");
		RunTest( " 1431655765", "0 1 22 -1 -2 -16384 MIN MAX", "0 1 20  1431655765  1431655764  1431650304   0 1431655765");

    }
};

//Case 4: In-place operation on multi-channel data
class TestAndC_8u_C3IR : public PValSDstRoi< U8, 3 >
{
public:
    TestAndC_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 3 >( parent, "fwiAndC_8u_C3IR", fwiAndC_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "170 42 10", "0 1 22 63 64 32 127 254 MAX", "0 0 2 42 0 0 42 42 10");
		RunTest( " 85 21  5", "0 1 22 63 64 32 127 254 MAX", "0 1 4 21 0 0 85 20  5");
    }
};

class TestAndC_8u_C4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestAndC_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiAndC_8u_C4IR", fwiAndC_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "170 42 10 165", "0 1 22 63 127 254 MAX 213", "0 0 2 37 42 42 10 133");
		RunTest( " 85 21  5 100", "0 1 22 63 127 254 MAX 213", "0 1 4 36 85 20  5  68");
    }
};

class TestAndC_8u_AC4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestAndC_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiAndC_8u_AC4IR", fwiAndC_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "170 42 10 165", "0 1 22 63 127 254 MAX 213", "0 0 2 63 42 42 10 213");
		RunTest( " 85 21  5 100", "0 1 22 63 127 254 MAX 213", "0 1 4 63 85 20  5 213");
    }
};

class TestAndC_16u_C3IR : public PValSDstRoi< U16, 3 >
{
public:
    TestAndC_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 3 >( parent, "fwiAndC_16u_C3IR", fwiAndC_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "43690 12345 10000", "0 1 22 16383 16384 8192 65534 MAX 345", 
			"0 1 16 10922     0 8192 43690 12345 272");
		RunTest( "21845 54320 20000", "0 1 22 16383 16384 8192 65534 MAX 543", 
			"0 0  0  5461 16384    0 21844 54320 512");
    }
};

class TestAndC_16u_C4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestAndC_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiAndC_16u_C4IR", fwiAndC_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "43690 12345 10000 2345", "0 1 22 16383 65534 MAX 345 8192", 
			"0 1 16 2345 43690 12345 272 0");
		RunTest( "21845 54320 20000 7878", "0 1 22 16383 65534 MAX 543 1024", 
			"0 0  0  7878 21844 54320 512 1024");
	}
};

class TestAndC_16u_AC4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestAndC_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiAndC_16u_AC4IR", fwiAndC_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "43690 12345 10000 2345", "0 1 22 16383 65534 MAX 345 8192", 
			"0 1 16 16383 43690 12345 272 8192");
		RunTest( "21845 54320 20000 7878", "0 1 22 16383 65534 MAX 543 1024", 
			"0 0  0 16383 21844 54320 512 1024");   
    }
};

class TestAndC_32s_C3IR : public PValSDstRoi< S32, 3 >
{
public:
    TestAndC_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 3 >( parent, "fwiAndC_32s_C3IR", fwiAndC_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-1431655766 -124567890 -987654321", "0 1 22 -1 -2 -16384 MIN MAX 2147473546", 
			"0 0 6 -1431655766 -124567890 -987660288 0 2022915758  1159827466");
		RunTest( " 1431655765  124567890  987654321", "0 1 22 -1 -2 -16384 MIN MAX 2147473546", 
			"0 0 16 1431655765 124567890 987643904 0 124567890 987646080");
    }
};

class TestAndC_32s_C4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestAndC_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiAndC_32s_C4IR", fwiAndC_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-1431655766 -124567890 -987654321 -123", "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", 
			"0 0 6 -512 -1431655766 -124567890 -987660288 384 0 2022915758  1159827466 5");
		RunTest( " 1431655765  124567890  987654321  123", "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", 
			"0 0 16 56 1431655765 124567890 987643904 72 0 124567890 987646080 65");
    }
};

class TestAndC_32s_AC4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestAndC_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiAndC_32s_AC4IR", fwiAndC_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-1431655766 -124567890 -987654321 -123", "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", 
			"0 0 6 -456 -1431655766 -124567890 -987660288 456 0 2022915758  1159827466 69");
		RunTest( " 1431655765  124567890  987654321  123", "0 1 22 -456 -1 -2 -16384 456 MIN MAX 2147473546 69", 
			"0 0 16 -456 1431655765 124567890 987643904 456 0 124567890 987646080 69");
    }
};

DEFINE_TEST_TABLE( AndCTestCatalog )
//Case 1
TEST_ENTRY( TestAndC_8u_C1R )
TEST_ENTRY( TestAndC_16u_C1R )
TEST_ENTRY( TestAndC_32s_C1R )
//Case 2
TEST_ENTRY( TestAndC_8u_C3R )
TEST_ENTRY( TestAndC_8u_C4R )
TEST_ENTRY( TestAndC_8u_AC4R )
TEST_ENTRY( TestAndC_16u_C3R )
TEST_ENTRY( TestAndC_16u_C4R )
TEST_ENTRY( TestAndC_16u_AC4R )
TEST_ENTRY( TestAndC_32s_C3R )
TEST_ENTRY( TestAndC_32s_C4R )
TEST_ENTRY( TestAndC_32s_AC4R )
//Case 3
TEST_ENTRY( TestAndC_8u_C1IR )
TEST_ENTRY( TestAndC_16u_C1IR )
TEST_ENTRY( TestAndC_32s_C1IR )
//Case4
TEST_ENTRY( TestAndC_8u_C3IR )
TEST_ENTRY( TestAndC_8u_C4IR )
TEST_ENTRY( TestAndC_8u_AC4IR )
TEST_ENTRY( TestAndC_16u_C3IR )
TEST_ENTRY( TestAndC_16u_C4IR )
TEST_ENTRY( TestAndC_16u_AC4IR )
TEST_ENTRY( TestAndC_32s_C3IR )
TEST_ENTRY( TestAndC_32s_C4IR )
TEST_ENTRY( TestAndC_32s_AC4IR )
END_TEST_TABLE()
