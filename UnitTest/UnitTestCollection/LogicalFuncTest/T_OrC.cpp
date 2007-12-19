/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel data.
class TestOrC_8u_C1R : public SrcValDstRoi< U8, 1, U8, 1 >
{
public:
    TestOrC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U8, 1, U8, 1 >( parent, "fwiOrC_8u_C1R", fwiOrC_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "123", "123 123 127 127 123 123 255 MAX");
    }
};

class TestOrC_16u_C1R : public SrcValDstRoi< U16, 1, U16, 1 >
{
public:
    TestOrC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U16, 1, U16, 1 >( parent, "fwiOrC_16u_C1R", fwiOrC_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "56123", 
			"56123 56123 56255 56255 65343 56123 56319 MAX");
    }
};

class TestOrC_32s_C1R : public SrcValDstRoi< S32, 1, S32, 1 >
{
public:
    TestOrC_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< S32, 1, S32, 1 >( parent, "fwiOrC_32s_C1R", fwiOrC_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 MAX 254 MIN", "-34112", 
			"-34112 -1 -34066 -61 -297 -1 -34050 -34112");
    }
};

//Case 2: Not-in-place operation on multi-channel data.
class TestOrC_8u_C3R : public SrcPValDstRoi< U8, 3, U8, 3 >
{
public:
    TestOrC_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 3, U8, 3 >( parent, "fwiOrC_8u_C3R", fwiOrC_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX MAX", "123 111 134", "123 111 150 127 111 150 255 MAX MAX");
    }
};

class TestOrC_8u_C4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestOrC_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiOrC_8u_C4R", fwiOrC_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "33 34 56 123", "33 35 62 127 97 50 254 MAX");
    }
};

class TestOrC_8u_AC4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestOrC_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiOrC_8u_AC4R", fwiOrC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "33 34 56", "33 35 62 0 97 50 254 0");
    }
};

class TestOrC_16u_C3R : public SrcPValDstRoi< U16, 3, U16, 3 >
{
public:
    TestOrC_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 3, U16, 3 >( parent, "fwiOrC_16u_C3R", fwiOrC_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX 78", "3311 34112 56123", 
			"3311 34113 56255 7423 64855 56123 3327 MAX 56191");
    }
};

class TestOrC_16u_C4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestOrC_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiOrC_16u_C4R", fwiOrC_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "3311 34112 56123 123", 
			"3311 34113 56255 6399 64767 34128 56319 MAX");
    }
};

class TestOrC_16u_AC4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestOrC_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiOrC_16u_AC4R", fwiOrC_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "3311 34112 56123", 
			"3311 34113 56255 0 64767 34128 56319 0");
    }
};

class TestOrC_32s_C3R : public SrcPValDstRoi< S32, 3, S32, 3 >
{
public:
    TestOrC_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 3, S32, 3 >( parent, "fwiOrC_32s_C3R", fwiOrC_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"-34112 -56123 1234567890", 
			"-34112 -1 1234569982 -61 -809 -572561454 -34050 -56123 1234567890");
    }
};

class TestOrC_32s_C4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestOrC_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiOrC_32s_C4R", fwiOrC_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 254 MIN 1234567890 -1647483648", "-34112 -56123 1234567890 3311", 
			"-34112 -1 1234569982 -4113 -34050 -56123 1234567890 -1647481361");
    }
};

class TestOrC_32s_AC4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestOrC_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiOrC_32s_AC4R", fwiOrC_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 254 MIN 1234567890 -1647483648", "-34112 -56123 1234567890", 
			"-34112 -1 1234569982 0 -34050 -56123 1234567890 0");
    }
};

//Case 3: In-place operation on 1-channel data.
class TestOrC_8u_C1IR : public ValSDstRoi< U8, 1 >
{
public:
    TestOrC_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U8, 1 >( parent, "fwiOrC_8u_C1IR", fwiOrC_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "123", "0 1 22 63 64 16 254 MAX", "123 123 127 127 123 123 255 MAX");
    }
};

class TestOrC_16u_C1IR : public ValSDstRoi< U16, 1 >
{
public:
    TestOrC_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U16, 1 >( parent, "fwiOrC_16u_C1IR", fwiOrC_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "56123", "0 1 2222 6333 64535 16 254 MAX", 
			"56123 56123 56255 56255 65343 56123 56319 MAX");
    }
};

class TestOrC_32s_C1IR : public ValSDstRoi< S32, 1 >
{
public:
    TestOrC_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< S32, 1 >( parent, "fwiOrC_32s_C1IR", fwiOrC_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112", "0 -1 2222 -6333 64535 MAX 254 MIN", 
			"-34112 -1 -34066 -61 -297 -1 -34050 -34112");
    }
};

//Case 4: In-place operation on multi-channel data.
class TestOrC_8u_C3IR : public PValSDstRoi< U8, 3 >
{
public:
    TestOrC_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 3 >( parent, "fwiOrC_8u_C3IR", fwiOrC_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "123 111 134", "0 1 22 63 64 16 254 MAX MAX", 
			"123 111 150 127 111 150 255 MAX MAX");
    }
};

class TestOrC_8u_C4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestOrC_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiOrC_8u_C4IR", fwiOrC_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "33 34 56 123", "0 1 22 63 64 16 254 MAX", 
			"33 35 62 127 97 50 254 MAX");
    }
};

class TestOrC_8u_AC4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestOrC_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiOrC_8u_AC4IR", fwiOrC_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "33 34 56", "0 1 22 63 64 16 254 MAX", 
			"33 35 62 63 97 50 254 MAX");
    }
};

class TestOrC_16u_C3IR : public PValSDstRoi< U16, 3 >
{
public:
    TestOrC_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 3 >( parent, "fwiOrC_16u_C3IR", fwiOrC_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123", "0 1 2222 6333 64535 16 254 MAX 78", 
			"3311 34113 56255 7423 64855 56123 3327 MAX 56191");
    }
};

class TestOrC_16u_C4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestOrC_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiOrC_16u_C4IR", fwiOrC_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123 123", "0 1 2222 6333 64535 16 254 MAX", 
			"3311 34113 56255 6399 64767 34128 56319 MAX");
    }
};

class TestOrC_16u_AC4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestOrC_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiOrC_16u_AC4IR", fwiOrC_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123", "0 1 2222 6333 64535 16 254 MAX", 
			"3311 34113 56255 6333 64767 34128 56319 MAX");
    }
};

class TestOrC_32s_C3IR : public PValSDstRoi< S32, 3 >
{
public:
    TestOrC_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 3 >( parent, "fwiOrC_32s_C3IR", fwiOrC_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890", 
			"0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"-34112 -1 1234569982 -61 -809 -572561454 -34050 -56123 1234567890");
    }
};

class TestOrC_32s_C4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestOrC_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiOrC_32s_C4IR", fwiOrC_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890 3311", 
			"0 -1 2222 -6333 254 MIN 1234567890 -1647483648", 
			"-34112 -1 1234569982 -4113 -34050 -56123 1234567890 -1647481361");
    }
};

class TestOrC_32s_AC4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestOrC_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiOrC_32s_AC4IR", fwiOrC_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890", 
			"0 -1 2222 -6333 254 MIN 1234567890 -1647483648", 
			"-34112 -1 1234569982 -6333 -34050 -56123 1234567890 -1647483648");
    }
};

DEFINE_TEST_TABLE( OrCTestCatalog )
//Case 1
TEST_ENTRY( TestOrC_8u_C1R )
TEST_ENTRY( TestOrC_16u_C1R )
TEST_ENTRY( TestOrC_32s_C1R )

//Case 2
TEST_ENTRY( TestOrC_8u_C3R )
TEST_ENTRY( TestOrC_8u_C4R )
TEST_ENTRY( TestOrC_8u_AC4R )
TEST_ENTRY( TestOrC_16u_C3R )
TEST_ENTRY( TestOrC_16u_C4R )
TEST_ENTRY( TestOrC_16u_AC4R )
TEST_ENTRY( TestOrC_32s_C3R )
TEST_ENTRY( TestOrC_32s_C4R )
TEST_ENTRY( TestOrC_32s_AC4R )

//Case 3
TEST_ENTRY( TestOrC_8u_C1IR )
TEST_ENTRY( TestOrC_16u_C1IR )
TEST_ENTRY( TestOrC_32s_C1IR )

//Case 4
TEST_ENTRY( TestOrC_8u_C3IR )
TEST_ENTRY( TestOrC_8u_C4IR )
TEST_ENTRY( TestOrC_8u_AC4IR )
TEST_ENTRY( TestOrC_16u_C3IR )
TEST_ENTRY( TestOrC_16u_C4IR )
TEST_ENTRY( TestOrC_16u_AC4IR )
TEST_ENTRY( TestOrC_32s_C3IR )
TEST_ENTRY( TestOrC_32s_C4IR )
TEST_ENTRY( TestOrC_32s_AC4IR )

END_TEST_TABLE()
