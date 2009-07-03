/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel data.
class TestRShiftC_8u_C1R : public SrcValuDstRoi< U8, 1, U8, 1 >
{
public:
    TestRShiftC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< U8, 1, U8, 1 >( parent, "fwiRShiftC_8u_C1R", fwiRShiftC_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 127 128 201 254 MAX", "1", "0 0 11 63 64 100 127 127");
		RunTest( "0 22 63 64 127 128 254 MAX", "2", "0 5 15 16 31 32 63 63");
		RunTest( "0 1 22 127 128 200 254 MAX", "0", "0 1 22 127 128 200 254 MAX");
    }
};

class TestRShiftC_16u_C1R : public SrcValuDstRoi< U16, 1, U16, 1 >
{
public:
    TestRShiftC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< U16, 1, U16, 1 >( parent, "fwiRShiftC_16u_C1R", fwiRShiftC_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 32768 40001 65534 MAX", "1", 
			"0 61 1111 16383 16384 20000 32767 32767");
    }
};

class TestRShiftC_32s_C1R : public SrcValuDstRoi< S32, 1, S32, 1 >
{
public:
    TestRShiftC_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValuDstRoi< S32, 1, S32, 1 >( parent, "fwiRShiftC_32s_C1R", fwiRShiftC_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -1234567890 1073741823 MAX -254 MIN", "2", 
			"0 -1 555 -308641973 268435455 536870911 -64 -536870912");
    }
};

//Case 2: Not-in-place operation on multi-channel data.
class TestRShiftC_8u_C3R : public SrcPValuDstRoi< U8, 3, U8, 3 >
{
public:
    TestRShiftC_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 3, U8, 3 >( parent, "fwiRShiftC_8u_C3R", fwiRShiftC_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 64 254 MAX MAX", "0 1 2", "0 0 5 63 32 16 254 127 63");
    }
};

class TestRShiftC_8u_C4R : public SrcPValuDstRoi< U8, 4, U8, 4 >
{
public:
    TestRShiftC_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 4, U8, 4 >( parent, "fwiRShiftC_8u_C4R", fwiRShiftC_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 254 MAX MAX", "0 1 2 3", "0 0 5 7 64 127 63 31");
    }
};

class TestRShiftC_8u_AC4R : public SrcPValuDstRoi< U8, 4, U8, 4 >
{
public:
    TestRShiftC_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U8, 4, U8, 4 >( parent, "fwiRShiftC_8u_AC4R", fwiRShiftC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 254 MAX MAX", "0 1 2", "0 0 5 0 64 127 63 0");
    }
};

class TestRShiftC_16u_C3R : public SrcPValuDstRoi< U16, 3, U16, 3 >
{
public:
    TestRShiftC_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 3, U16, 3 >( parent, "fwiRShiftC_16u_C3R", fwiRShiftC_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 32768 40001 12345 65534 MAX", "0 1 2", 
			"0 61 555 32767 16384 10000 12345 32767 16383");
    }
};

class TestRShiftC_16u_C4R : public SrcPValuDstRoi< U16, 4, U16, 4 >
{
public:
    TestRShiftC_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 4, U16, 4 >( parent, "fwiRShiftC_16u_C4R", fwiRShiftC_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 40001 32768 65534 MAX", "0 1 2 3", 
			"0 61 555 4095 40001 16384 16383 8191");
    }
};

class TestRShiftC_16u_AC4R : public SrcPValuDstRoi< U16, 4, U16, 4 >
{
public:
    TestRShiftC_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< U16, 4, U16, 4 >( parent, "fwiRShiftC_16u_AC4R", fwiRShiftC_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 123 2222 32767 40001 32768 65534 MAX", "0 1 2", 
			"0 61 555 0 40001 16384 16383 0");
    }
};

class TestRShiftC_32s_C3R : public SrcPValuDstRoi< S32, 3, S32, 3 >
{
public:
    TestRShiftC_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 3, S32, 3 >( parent, "fwiRShiftC_32s_C3R", fwiRShiftC_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 1073741823 -1234567890 MAX -254 -123 MIN", "0 1 2", 
			"0 -1 555 1073741823 -617283945 536870911 -254 -62 -536870912");
    }
};

class TestRShiftC_32s_C4R : public SrcPValuDstRoi< S32, 4, S32, 4 >
{
public:
    TestRShiftC_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 4, S32, 4 >( parent, "fwiRShiftC_32s_C4R", fwiRShiftC_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -1234567890 1073741823  MAX -254 MIN", "0 1 2 3", 
			"0 -1 555 -154320987 1073741823 1073741823 -64 -268435456");
    }
};

class TestRShiftC_32s_AC4R : public SrcPValuDstRoi< S32, 4, S32, 4 >
{
public:
    TestRShiftC_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValuDstRoi< S32, 4, S32, 4 >( parent, "fwiRShiftC_32s_AC4R", fwiRShiftC_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -1234567890 1073741823  MAX -254 MIN", "0 1 2", 
			"0 -1 555 0 1073741823 1073741823 -64 0");
    }
};

//Case 3: In-place operation on 1-channel data.
class TestRShiftC_8u_C1IR : public ValuSDstRoi< U8, 1 >
{
public:
    TestRShiftC_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< U8, 1 >( parent, "fwiRShiftC_8u_C1IR", fwiRShiftC_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "0 1 22 127 128 201 254 MAX", "0 0 11 63 64 100 127 127");
		RunTest( "2", "0 22 63 64 127 128 254 MAX", "0 5 15 16 31 32 63 63");
		RunTest( "0", "0 1 22 127 128 200 254 MAX", "0 1 22 127 128 200 254 MAX");
    }
};

class TestRShiftC_16u_C1IR : public ValuSDstRoi< U16, 1 >
{
public:
    TestRShiftC_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< U16, 1 >( parent, "fwiRShiftC_16u_C1IR", fwiRShiftC_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "0 123 2222 32767 32768 40001 65534 MAX", 
			"0 61 1111 16383 16384 20000 32767 32767");
    }
};

class TestRShiftC_32s_C1IR : public ValuSDstRoi< S32, 1 >
{
public:
    TestRShiftC_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  ValuSDstRoi< S32, 1 >( parent, "fwiRShiftC_32s_C1IR", fwiRShiftC_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2", "0 -1 2222 -1234567890 1073741823 MAX -254 MIN", 
			"0 -1 555 -308641973 268435455 536870911 -64 -536870912");
    }
};

//Case 4: In-place operation on multi-channel data.
class TestRShiftC_8u_C3IR : public PValuSDstRoi< U8, 3 >
{
public:
    TestRShiftC_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 3 >( parent, "fwiRShiftC_8u_C3IR", fwiRShiftC_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 1 22 63 64 64 254 MAX MAX", "0 0 5 63 32 16 254 127 63");
    }
};

class TestRShiftC_8u_C4IR : public PValuSDstRoi< U8, 4 >
{
public:
    TestRShiftC_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 4 >( parent, "fwiRShiftC_8u_C4IR", fwiRShiftC_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 1 22 63 64 254 MAX MAX", "0 0 5 7 64 127 63 31");
    }
};

class TestRShiftC_8u_AC4IR : public PValuSDstRoi< U8, 4 >
{
public:
    TestRShiftC_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U8, 4 >( parent, "fwiRShiftC_8u_AC4IR", fwiRShiftC_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 1 22 63 64 254 MAX MAX", "0 0 5 63 64 127 63 MAX");
    }
};

class TestRShiftC_16u_C3IR : public PValuSDstRoi< U16, 3 >
{
public:
    TestRShiftC_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 3 >( parent, "fwiRShiftC_16u_C3IR", fwiRShiftC_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 123 2222 32767 32768 40001 12345 65534 MAX", 
			"0 61 555 32767 16384 10000 12345 32767 16383");
    }
};

class TestRShiftC_16u_C4IR : public PValuSDstRoi< U16, 4 >
{
public:
    TestRShiftC_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 4 >( parent, "fwiRShiftC_16u_C4IR", fwiRShiftC_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 123 2222 32767 40001 32768 65534 MAX", 
			"0 61 555 4095 40001 16384 16383 8191");
    }
};

class TestRShiftC_16u_AC4IR : public PValuSDstRoi< U16, 4 >
{
public:
    TestRShiftC_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< U16, 4 >( parent, "fwiRShiftC_16u_AC4IR", fwiRShiftC_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 123 2222 32767 40001 32768 65534 MAX", 
			"0 61 555 32767 40001 16384 16383 MAX");
    }
};

class TestRShiftC_32s_C3IR : public PValuSDstRoi< S32, 3 >
{
public:
    TestRShiftC_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 3 >( parent, "fwiRShiftC_32s_C3IR", fwiRShiftC_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 -1 2222 1073741823 -1234567890 MAX -254 -123 MIN", 
			"0 -1 555 1073741823 -617283945 536870911 -254 -62 -536870912");
    }
};

class TestRShiftC_32s_C4IR : public PValuSDstRoi< S32, 4 >
{
public:
    TestRShiftC_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 4 >( parent, "fwiRShiftC_32s_C4IR", fwiRShiftC_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3", "0 -1 2222 -1234567890 1073741823  MAX -254 MIN", 
			"0 -1 555 -154320987 1073741823 1073741823 -64 -268435456");
    }
};

class TestRShiftC_32s_AC4IR : public PValuSDstRoi< S32, 4 >
{
public:
    TestRShiftC_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValuSDstRoi< S32, 4 >( parent, "fwiRShiftC_32s_AC4IR", fwiRShiftC_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2", "0 -1 2222 -1234567890 1073741823  MAX -254 MIN", 
			"0 -1 555 -1234567890 1073741823 1073741823 -64 MIN");
    }
};

DEFINE_TEST_TABLE( RShiftCTestCatalog )
//Case 1
TEST_ENTRY( TestRShiftC_8u_C1R )
TEST_ENTRY( TestRShiftC_16u_C1R )
TEST_ENTRY( TestRShiftC_32s_C1R )

//Case 2
TEST_ENTRY( TestRShiftC_8u_C3R )
TEST_ENTRY( TestRShiftC_8u_C4R )
TEST_ENTRY( TestRShiftC_8u_AC4R )
TEST_ENTRY( TestRShiftC_16u_C3R )
TEST_ENTRY( TestRShiftC_16u_C4R )
TEST_ENTRY( TestRShiftC_16u_AC4R )
TEST_ENTRY( TestRShiftC_32s_C3R )
TEST_ENTRY( TestRShiftC_32s_C4R )
TEST_ENTRY( TestRShiftC_32s_AC4R )

//Case 3
TEST_ENTRY( TestRShiftC_8u_C1IR )
TEST_ENTRY( TestRShiftC_16u_C1IR )
TEST_ENTRY( TestRShiftC_32s_C1IR )

//Case 4
TEST_ENTRY( TestRShiftC_8u_C3IR )
TEST_ENTRY( TestRShiftC_8u_C4IR )
TEST_ENTRY( TestRShiftC_8u_AC4IR )
TEST_ENTRY( TestRShiftC_16u_C3IR )
TEST_ENTRY( TestRShiftC_16u_C4IR )
TEST_ENTRY( TestRShiftC_16u_AC4IR )
TEST_ENTRY( TestRShiftC_32s_C3IR )
TEST_ENTRY( TestRShiftC_32s_C4IR )
TEST_ENTRY( TestRShiftC_32s_AC4IR )

END_TEST_TABLE()
