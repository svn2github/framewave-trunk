/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel data.
class TestXorC_8u_C1R : public SrcValDstRoi< U8, 1, U8, 1 >
{
public:
    TestXorC_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U8, 1, U8, 1 >( parent, "fwiXorC_8u_C1R", fwiXorC_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "123", "123 122 109 68 59 107 133 132");
    }
};

class TestXorC_16u_C1R : public SrcValDstRoi< U16, 1, U16, 1 >
{
public:
    TestXorC_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< U16, 1, U16, 1 >( parent, "fwiXorC_16u_C1R", fwiXorC_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "56123", 
			"56123 56122 54165 50054 10028 56107 56261 9412");
    }
};

class TestXorC_32s_C1R : public SrcValDstRoi< S32, 1, S32, 1 >
{
public:
    TestXorC_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< S32, 1, S32, 1 >( parent, "fwiXorC_32s_C1R", fwiXorC_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 MAX 254 MIN", "-34112", 
			"-34112 34111 -36242 40323 -31017 -2147449537 -34242 2147449536");
    }
};

//Case 2: Not-in-place operation on multi-channel data.
class TestXorC_8u_C3R : public SrcPValDstRoi< U8, 3, U8, 3 >
{
public:
    TestXorC_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 3, U8, 3 >( parent, "fwiXorC_8u_C3R", fwiXorC_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX MAX", "123 111 134", "123 110 144 68 47 150 133 144 121");
    }
};

class TestXorC_8u_C4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestXorC_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiXorC_8u_C4R", fwiXorC_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "33 34 56 123", "33 35 46 68 97 50 198 132");
    }
};

class TestXorC_8u_AC4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestXorC_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiXorC_8u_AC4R", fwiXorC_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "33 34 56", "33 35 46 0 97 50 198 0");
    }
};

class TestXorC_16u_C3R : public SrcPValDstRoi< U16, 3, U16, 3 >
{
public:
    TestXorC_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 3, U16, 3 >( parent, "fwiXorC_16u_C3R", fwiXorC_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX 78", "3311 34112 56123", 
			"3311 34113 54165 5202 31063 56107 3089 31423 56181");
    }
};

class TestXorC_16u_C4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestXorC_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiXorC_16u_C4R", fwiXorC_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "3311 34112 56123 123", 
			"3311 34113 54165 6342 61688 34128 56261 65412");
    }
};

class TestXorC_16u_AC4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestXorC_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiXorC_16u_AC4R", fwiXorC_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2222 6333 64535 16 254 MAX", "3311 34112 56123", 
			"3311 34113 54165 0 61688 34128 56261 0");
    }
};

class TestXorC_32s_C3R : public SrcPValDstRoi< S32, 3, S32, 3 >
{
public:
    TestXorC_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 3, S32, 3 >( parent, "fwiXorC_32s_C3R", fwiXorC_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"-34112 -56123 1234567890", 
			"-34112 56122 1234569852 40323 -10030 -732207150 -34242 2147427525 0");
    }
};

class TestXorC_32s_C4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestXorC_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiXorC_32s_C4R", fwiXorC_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 254 MIN 1234567890 -1647483648", "-34112 -56123 1234567890 3311", 
			"-34112 56122 1234569852 -5204 -34242 2147427525 0 -1647482385");
    }
};

class TestXorC_32s_AC4R : public SrcPValDstRoi< S32, 4, S32, 4 >
{
public:
    TestXorC_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< S32, 4, S32, 4 >( parent, "fwiXorC_32s_AC4R", fwiXorC_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 -1 2222 -6333 254 MIN 1234567890 -1647483648", "-34112 -56123 1234567890", 
			"-34112 56122 1234569852 0 -34242 2147427525 0 0");
    }
};

//Case 3: In-place operation on 1-channel data.
class TestXorC_8u_C1IR : public ValSDstRoi< U8, 1 >
{
public:
    TestXorC_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U8, 1 >( parent, "fwiXorC_8u_C1IR", fwiXorC_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "123", "0 1 22 63 64 16 254 MAX", "123 122 109 68 59 107 133 132");
    }
};

class TestXorC_16u_C1IR : public ValSDstRoi< U16, 1 >
{
public:
    TestXorC_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< U16, 1 >( parent, "fwiXorC_16u_C1IR", fwiXorC_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "56123", "0 1 2222 6333 64535 16 254 MAX", 
			"56123 56122 54165 50054 10028 56107 56261 9412");
    }
};

class TestXorC_32s_C1IR : public ValSDstRoi< S32, 1 >
{
public:
    TestXorC_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< S32, 1 >( parent, "fwiXorC_32s_C1IR", fwiXorC_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112", "0 -1 2222 -6333 64535 MAX 254 MIN", 
			"-34112 34111 -36242 40323 -31017 -2147449537 -34242 2147449536");
    }
};

//Case 4: In-place operation on multi-channel data.
class TestXorC_8u_C3IR : public PValSDstRoi< U8, 3 >
{
public:
    TestXorC_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 3 >( parent, "fwiXorC_8u_C3IR", fwiXorC_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "123 111 134", "0 1 22 63 64 16 254 MAX MAX", 
			"123 110 144 68 47 150 133 144 121");
    }
};

class TestXorC_8u_C4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestXorC_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiXorC_8u_C4IR", fwiXorC_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "33 34 56 123", "0 1 22 63 64 16 254 MAX", 
			"33 35 46 68 97 50 198 132");
    }
};

class TestXorC_8u_AC4IR : public PValSDstRoi< U8, 4 >
{
public:
    TestXorC_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U8, 4 >( parent, "fwiXorC_8u_AC4IR", fwiXorC_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "33 34 56", "0 1 22 63 64 16 254 MAX", 
			"33 35 46 63 97 50 198 MAX");
    }
};

class TestXorC_16u_C3IR : public PValSDstRoi< U16, 3 >
{
public:
    TestXorC_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 3 >( parent, "fwiXorC_16u_C3IR", fwiXorC_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123", "0 1 2222 6333 64535 16 254 MAX 78", 
			"3311 34113 54165 5202 31063 56107 3089 31423 56181");
    }
};

class TestXorC_16u_C4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestXorC_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiXorC_16u_C4IR", fwiXorC_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123 123", "0 1 2222 6333 64535 16 254 MAX", 
			"3311 34113 54165 6342 61688 34128 56261 65412");
    }
};

class TestXorC_16u_AC4IR : public PValSDstRoi< U16, 4 >
{
public:
    TestXorC_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< U16, 4 >( parent, "fwiXorC_16u_AC4IR", fwiXorC_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3311 34112 56123", "0 1 2222 6333 64535 16 254 MAX", 
			"3311 34113 54165 6333 61688 34128 56261 MAX");
    }
};

class TestXorC_32s_C3IR : public PValSDstRoi< S32, 3 >
{
public:
    TestXorC_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 3 >( parent, "fwiXorC_32s_C3IR", fwiXorC_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890", 
			"0 -1 2222 -6333 64535 -1647483648 254 MIN 1234567890", 
			"-34112 56122 1234569852 40323 -10030 -732207150 -34242 2147427525 0");
    }
};

class TestXorC_32s_C4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestXorC_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiXorC_32s_C4IR", fwiXorC_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890 3311", 
			"0 -1 2222 -6333 254 MIN 1234567890 -1647483648", 
			"-34112 56122 1234569852 -5204 -34242 2147427525 0 -1647482385");
    }
};

class TestXorC_32s_AC4IR : public PValSDstRoi< S32, 4 >
{
public:
    TestXorC_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< S32, 4 >( parent, "fwiXorC_32s_AC4IR", fwiXorC_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "-34112 -56123 1234567890", 
			"0 -1 2222 -6333 254 MIN 1234567890 -1647483648", 
			"-34112 56122 1234569852 -6333 -34242 2147427525 0 -1647483648");
    }
};

DEFINE_TEST_TABLE( XorCTestCatalog )
//Case 1
TEST_ENTRY( TestXorC_8u_C1R )
TEST_ENTRY( TestXorC_16u_C1R )
TEST_ENTRY( TestXorC_32s_C1R )

//Case 2
TEST_ENTRY( TestXorC_8u_C3R )
TEST_ENTRY( TestXorC_8u_C4R )
TEST_ENTRY( TestXorC_8u_AC4R )
TEST_ENTRY( TestXorC_16u_C3R )
TEST_ENTRY( TestXorC_16u_C4R )
TEST_ENTRY( TestXorC_16u_AC4R )
TEST_ENTRY( TestXorC_32s_C3R )
TEST_ENTRY( TestXorC_32s_C4R )
TEST_ENTRY( TestXorC_32s_AC4R )

//Case 3
TEST_ENTRY( TestXorC_8u_C1IR )
TEST_ENTRY( TestXorC_16u_C1IR )
TEST_ENTRY( TestXorC_32s_C1IR )

//Case 4
TEST_ENTRY( TestXorC_8u_C3IR )
TEST_ENTRY( TestXorC_8u_C4IR )
TEST_ENTRY( TestXorC_8u_AC4IR )
TEST_ENTRY( TestXorC_16u_C3IR )
TEST_ENTRY( TestXorC_16u_C4IR )
TEST_ENTRY( TestXorC_16u_AC4IR )
TEST_ENTRY( TestXorC_32s_C3IR )
TEST_ENTRY( TestXorC_32s_C4IR )
TEST_ENTRY( TestXorC_32s_AC4IR )

END_TEST_TABLE()
