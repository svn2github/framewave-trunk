/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel data.
class TestLShiftC_8u_C1R : public SrcValuDstRoi< U8, 1, U8, 1 >
{
public:
    TestLShiftC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< U8, 1, U8, 1 >( parent, "fwiLShiftC_8u_C1R", fwiLShiftC_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 127 128 200 254 MAX", "1", "0 2 44 254 0 144 252 254");
		RunTest( "0 22 63 64 127 128 254 MAX", "2", "0 88 252 0 252 0 248 252");
		RunTest( "0 1 22 127 128 200 254 MAX", "0", "0 1 22 127 128 200 254 MAX");
    }
};

class TestLShiftC_16u_C1R : public SrcValuDstRoi< U16, 1, U16, 1 >
{
public:
    TestLShiftC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< U16, 1, U16, 1 >( parent, "fwiLShiftC_16u_C1R", fwiLShiftC_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 32768 40001 65534 MAX", "1", 
			"0 246 4444 65534 0 14466 65532 65534");
    }
};

class TestLShiftC_32s_C1R : public SrcValuDstRoi< S32, 1, S32, 1 >
{
public:
    TestLShiftC_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< S32, 1, S32, 1 >( parent, "fwiLShiftC_32s_C1R", fwiLShiftC_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 1073741824 1073741823 MAX 254 MIN", "2", 
			"0 -4 8888 0 -4 -4 1016 0");
    }
};

//Case 2: Not-in-place operation on multi-channel data.
class TestLShiftC_8u_C3R : public SrcPValuDstRoi< U8, 3, U8, 3 >
{
public:
    TestLShiftC_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 3, U8, 3 >( parent, "fwiLShiftC_8u_C3R", fwiLShiftC_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 64 254 MAX MAX", "0 1 2", "0 2 88 63 128 0 254 254 252");
    }
};

class TestLShiftC_8u_C4R : public SrcPValuDstRoi< U8, 4, U8, 4 >
{
public:
    TestLShiftC_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 4, U8, 4 >( parent, "fwiLShiftC_8u_C4R", fwiLShiftC_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 254 MAX MAX", "0 1 2 3", "0 2 88 248 64 252 252 248");
    }
};

class TestLShiftC_8u_AC4R : public SrcPValuDstRoi< U8, 4, U8, 4 >
{
public:
    TestLShiftC_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 4, U8, 4 >( parent, "fwiLShiftC_8u_AC4R", fwiLShiftC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 254 MAX MAX", "0 1 2", "0 2 88 0 64 252 252 0");
    }
};

class TestLShiftC_16u_C3R : public SrcPValuDstRoi< U16, 3, U16, 3 >
{
public:
    TestLShiftC_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 3, U16, 3 >( parent, "fwiLShiftC_16u_C3R", fwiLShiftC_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 32768 40001 12345 65534 MAX", "0 1 2", 
			"0 246 8888 32767 0 28932 12345 65532 65532");
    }
};

class TestLShiftC_16u_C4R : public SrcPValuDstRoi< U16, 4, U16, 4 >
{
public:
    TestLShiftC_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 4, U16, 4 >( parent, "fwiLShiftC_16u_C4R", fwiLShiftC_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 40001 32768 65534 MAX", "0 1 2 3", 
			"0 246 8888 65528 40001 0 65528 65528");
    }
};

class TestLShiftC_16u_AC4R : public SrcPValuDstRoi< U16, 4, U16, 4 >
{
public:
    TestLShiftC_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 4, U16, 4 >( parent, "fwiLShiftC_16u_AC4R", fwiLShiftC_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 40001 32768 65534 MAX", "0 1 2", 
			"0 246 8888 0 40001 0 65528 0");
    }
};

class TestLShiftC_32s_C3R : public SrcPValuDstRoi< S32, 3, S32, 3 >
{
public:
    TestLShiftC_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 3, S32, 3 >( parent, "fwiLShiftC_32s_C3R", fwiLShiftC_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 456 1073741824 1073741823 254 MAX  MIN", "0 1 2", 
			"0 -2 8888 456 -2147483648 -4 254 -2 0");
    }
};

class TestLShiftC_32s_C4R : public SrcPValuDstRoi< S32, 4, S32, 4 >
{
public:
    TestLShiftC_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 4, S32, 4 >( parent, "fwiLShiftC_32s_C4R", fwiLShiftC_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 1073741824 254 1073741823  MAX  MIN", "0 1 2 3", 
			"0 -2 8888 0 254 2147483646 -4 0");
    }
};

class TestLShiftC_32s_AC4R : public SrcPValuDstRoi< S32, 4, S32, 4 >
{
public:
    TestLShiftC_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 4, S32, 4 >( parent, "fwiLShiftC_32s_AC4R", fwiLShiftC_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 1073741824 254 1073741823  MAX  MIN", "0 1 2", 
			"0 -2 8888 0 254 2147483646 -4 0");
    }
};

//Case 3: In-place operation on 1-channel data.
class TestLShiftC_8u_C1IR : public ValuSDstRoi< U8, 1 >
{
public:
    TestLShiftC_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< U8, 1 >( parent, "fwiLShiftC_8u_C1IR", fwiLShiftC_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "0 1 22 127 128 200 254 MAX", "0 2 44 254 0 144 252 254");
		RunTest( "2", "0 22 63 64 127 128 254 MAX", "0 88 252 0 252 0 248 252");
		RunTest( "0", "0 1 22 127 128 200 254 MAX", "0 1 22 127 128 200 254 MAX");
    }
};

class TestLShiftC_16u_C1IR : public ValuSDstRoi< U16, 1 >
{
public:
    TestLShiftC_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< U16, 1 >( parent, "fwiLShiftC_16u_C1IR", fwiLShiftC_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "0 123 2222 32767 32768 40001 65534 MAX", 
			"0 246 4444 65534 0 14466 65532 65534");
    }
};

class TestLShiftC_32s_C1IR : public ValuSDstRoi< S32, 1 >
{
public:
    TestLShiftC_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< S32, 1 >( parent, "fwiLShiftC_32s_C1IR", fwiLShiftC_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2", "0 -1 2222 1073741824 1073741823 MAX 254 MIN", 
			"0 -4 8888 0 -4 -4 1016 0");
    }
};

//Case 4: In-place operation on multi-channel data.
class TestLShiftC_8u_C3IR : public PValuSDstRoi< U8, 3 >
{
public:
    TestLShiftC_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 3 >( parent, "fwiLShiftC_8u_C3IR", fwiLShiftC_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 1 22 63 64 64 254 MAX MAX", "0 2 88 63 128 0 254 254 252");
    }
};

class TestLShiftC_8u_C4IR : public PValuSDstRoi< U8, 4 >
{
public:
    TestLShiftC_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 4 >( parent, "fwiLShiftC_8u_C4IR", fwiLShiftC_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 1 22 63 64 254 MAX MAX", "0 2 88 248 64 252 252 248");
    }
};

class TestLShiftC_8u_AC4IR : public PValuSDstRoi< U8, 4 >
{
public:
    TestLShiftC_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 4 >( parent, "fwiLShiftC_8u_AC4IR", fwiLShiftC_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 1 22 63 64 254 MAX MAX", "0 2 88 63 64 252 252 255");
    }
};

class TestLShiftC_16u_C3IR : public PValuSDstRoi< U16, 3 >
{
public:
    TestLShiftC_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 3 >( parent, "fwiLShiftC_16u_C3IR", fwiLShiftC_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 123 2222 32767 32768 40001 12345 65534 MAX", 
			"0 246 8888 32767 0 28932 12345 65532 65532");
    }
};

class TestLShiftC_16u_C4IR : public PValuSDstRoi< U16, 4 >
{
public:
    TestLShiftC_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 4 >( parent, "fwiLShiftC_16u_C4IR", fwiLShiftC_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 123 2222 32767 40001 32768 65534 MAX", 
			"0 246 8888 65528 40001 0 65528 65528");
    }
};

class TestLShiftC_16u_AC4IR : public PValuSDstRoi< U16, 4 >
{
public:
    TestLShiftC_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 4 >( parent, "fwiLShiftC_16u_AC4IR", fwiLShiftC_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 123 2222 32767 40001 32768 65534 MAX", 
			"0 246 8888 32767 40001 0 65528 MAX");
    }
};

class TestLShiftC_32s_C3IR : public PValuSDstRoi< S32, 3 >
{
public:
    TestLShiftC_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 3 >( parent, "fwiLShiftC_32s_C3IR", fwiLShiftC_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 -1 2222 456 1073741824 1073741823 254 MAX  MIN", 
			"0 -2 8888 456 -2147483648 -4 254 -2 0");
    }
};

class TestLShiftC_32s_C4IR : public PValuSDstRoi< S32, 4 >
{
public:
    TestLShiftC_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 4 >( parent, "fwiLShiftC_32s_C4IR", fwiLShiftC_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 -1 2222 1073741824 254 1073741823  MAX  MIN", 
			"0 -2 8888 0 254 2147483646 -4 0");
    }
};

class TestLShiftC_32s_AC4IR : public PValuSDstRoi< S32, 4 >
{
public:
    TestLShiftC_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 4 >( parent, "fwiLShiftC_32s_AC4IR", fwiLShiftC_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 -1 2222 1073741824 254 1073741823 MAX MIN", 
			"0 -2 8888 1073741824 254 2147483646 -4 MIN");
    }
};

DEFINE_TEST_TABLE( LShiftCTestCatalog )
//Case 1
TEST_ENTRY( TestLShiftC_8u_C1R )
TEST_ENTRY( TestLShiftC_16u_C1R )
TEST_ENTRY( TestLShiftC_32s_C1R )

//Case 2
TEST_ENTRY( TestLShiftC_8u_C3R )
TEST_ENTRY( TestLShiftC_8u_C4R )
TEST_ENTRY( TestLShiftC_8u_AC4R )
TEST_ENTRY( TestLShiftC_16u_C3R )
TEST_ENTRY( TestLShiftC_16u_C4R )
TEST_ENTRY( TestLShiftC_16u_AC4R )
TEST_ENTRY( TestLShiftC_32s_C3R )
TEST_ENTRY( TestLShiftC_32s_C4R )
TEST_ENTRY( TestLShiftC_32s_AC4R )

//Case 3
TEST_ENTRY( TestLShiftC_8u_C1IR )
TEST_ENTRY( TestLShiftC_16u_C1IR )
TEST_ENTRY( TestLShiftC_32s_C1IR )

//Case 4
TEST_ENTRY( TestLShiftC_8u_C3IR )
TEST_ENTRY( TestLShiftC_8u_C4IR )
TEST_ENTRY( TestLShiftC_8u_AC4IR )
TEST_ENTRY( TestLShiftC_16u_C3IR )
TEST_ENTRY( TestLShiftC_16u_C4IR )
TEST_ENTRY( TestLShiftC_16u_AC4IR )
TEST_ENTRY( TestLShiftC_32s_C3IR )
TEST_ENTRY( TestLShiftC_32s_C4IR )
TEST_ENTRY( TestLShiftC_32s_AC4IR )

END_TEST_TABLE()
