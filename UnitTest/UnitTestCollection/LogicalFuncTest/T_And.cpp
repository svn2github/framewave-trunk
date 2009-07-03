/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation
class TestAnd_8u_C1R : public SrcSrcDstRoi< U8, 1, U8, 1 >
{
public:
    TestAnd_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 1, U8, 1 >( parent, "fwiAnd_8u_C1R", fwiAnd_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
    }
};

class TestAnd_8u_C3R : public SrcSrcDstRoi< U8, 3, U8, 3 >
{
public:
    TestAnd_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 3, U8, 3 >( parent, "fwiAnd_8u_C3R", fwiAnd_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 8 7 16 65 127 55 63", "0 0 6 8 0 16 64 127 55 62");
    }
};

class TestAnd_8u_C4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestAnd_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiAnd_8u_C4R", fwiAnd_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 8 7 16 65 127 55", "0 0 6 8 0 16 64 127 55");
    }
};

class TestAnd_8u_AC4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestAnd_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiAnd_8u_AC4R", fwiAnd_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 0 0 16 64 0");
    }
};

class TestAnd_16u_C1R : public SrcSrcDstRoi< U16, 1, U16, 1 >
{
public:
    TestAnd_16u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 1, U16, 1 >( parent, "fwiAnd_16u_C1R", fwiAnd_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 16 7 16 65 127", "0 0 6 16 0 16 64 127");
    }
};

class TestAnd_16u_C3R : public SrcSrcDstRoi< U16, 3, U16, 3 >
{
public:
    TestAnd_16u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 3, U16, 3 >( parent, "fwiAnd_16u_C3R", fwiAnd_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 16 7 16 65 127 55 63", "0 0 6 16 0 16 64 127 55 62");
    }
};

class TestAnd_16u_C4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestAnd_16u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiAnd_16u_C4R", fwiAnd_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 16 7 16 65 127 55", "0 0 6 16 0 16 64 127 55");
    }
};

class TestAnd_16u_AC4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestAnd_16u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiAnd_16u_AC4R", fwiAnd_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 16 7 16 65 127", "0 0 6 0 0 16 64 0");
    }
};

class TestAnd_32s_C1R : public SrcSrcDstRoi< S32, 1, S32, 1 >
{
public:
    TestAnd_32s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 1, S32, 1 >( parent, "fwiAnd_32s_C1R", fwiAnd_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_C3R : public SrcSrcDstRoi< S32, 3, S32, 3 >
{
public:
    TestAnd_32s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 3, S32, 3 >( parent, "fwiAnd_32s_C3R", fwiAnd_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 8 7 16 65 127 55 63", "0 0 6 8 0 16 64 127 55 62");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_C4R : public SrcSrcDstRoi< S32, 4, S32, 4 >
{
public:
    TestAnd_32s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 4, S32, 4 >( parent, "fwiAnd_32s_C4R", fwiAnd_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 8 7 16 65 127 55", "0 0 6 8 0 16 64 127 55");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_AC4R : public SrcSrcDstRoi< S32, 4, S32, 4 >
{
public:
    TestAnd_32s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< S32, 4, S32, 4 >( parent, "fwiAnd_32s_AC4R", fwiAnd_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 0 0 16 64 0");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 0");
    }
};

//Case 2: In-place operation
class TestAnd_8u_C1IR : public SrcSDstRoi< U8, 1, U8, 1 >
{
public:
    TestAnd_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 1, U8, 1 >( parent, "fwiAnd_8u_C1IR", fwiAnd_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
    }
};

class TestAnd_8u_C3IR : public SrcSDstRoi< U8, 3, U8, 3 >
{
public:
    TestAnd_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 3, U8, 3 >( parent, "fwiAnd_8u_C3IR", fwiAnd_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 8 7 16 65 127 55 63", "0 0 6 8 0 16 64 127 55 62");
    }
};

class TestAnd_8u_C4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public:
    TestAnd_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiAnd_8u_C4IR", fwiAnd_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 8 7 16 65 127 55", "0 0 6 8 0 16 64 127 55");
    }
};

class TestAnd_8u_AC4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public:
    TestAnd_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiAnd_8u_AC4IR", fwiAnd_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
    }
};

class TestAnd_16u_C1IR : public SrcSDstRoi< U16, 1, U16, 1 >
{
public:
    TestAnd_16u_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 1, U16, 1 >( parent, "fwiAnd_16u_C1IR", fwiAnd_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 16 7 16 65 127", "0 0 6 16 0 16 64 127");
    }
};

class TestAnd_16u_C3IR : public SrcSDstRoi< U16, 3, U16, 3 >
{
public:
    TestAnd_16u_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 3, U16, 3 >( parent, "fwiAnd_16u_C3IR", fwiAnd_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 16 7 16 65 127 55 63", "0 0 6 16 0 16 64 127 55 62");
    }
};

class TestAnd_16u_C4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestAnd_16u_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiAnd_16u_C4IR", fwiAnd_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 16 7 16 65 127 55", "0 0 6 16 0 16 64 127 55");
    }
};

class TestAnd_16u_AC4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestAnd_16u_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiAnd_16u_AC4IR", fwiAnd_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 16 7 16 65 127", "0 0 6 16 0 16 64 127");
    }
};

class TestAnd_32s_C1IR : public SrcSDstRoi< S32, 1, S32, 1 >
{
public:
    TestAnd_32s_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 1, S32, 1 >( parent, "fwiAnd_32s_C1IR", fwiAnd_32s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_C3IR : public SrcSDstRoi< S32, 3, S32, 3 >
{
public:
    TestAnd_32s_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 3, S32, 3 >( parent, "fwiAnd_32s_C3IR", fwiAnd_32s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55 62", "1 2 15 8 7 16 65 127 55 63", "0 0 6 8 0 16 64 127 55 62");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_C4IR : public SrcSDstRoi< S32, 4, S32, 4 >
{
public:
    TestAnd_32s_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 4, S32, 4 >( parent, "fwiAnd_32s_C4IR", fwiAnd_32s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 55", "1 2 15 8 7 16 65 127 55", "0 0 6 8 0 16 64 127 55");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

class TestAnd_32s_AC4IR : public SrcSDstRoi< S32, 4, S32, 4 >
{
public:
    TestAnd_32s_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< S32, 4, S32, 4 >( parent, "fwiAnd_32s_AC4IR", fwiAnd_32s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "1 2 15 8 7 16 65 127", "0 0 6 8 0 16 64 127");
		RunTest( "-1 -2 MAX MIN", "MIN 100 -100 MIN", "MIN 100 2147483548 MIN");
    }
};

DEFINE_TEST_TABLE( AndTestCatalog )
//Case 1
TEST_ENTRY( TestAnd_8u_C1R )
TEST_ENTRY( TestAnd_8u_C3R )
TEST_ENTRY( TestAnd_8u_C4R )
TEST_ENTRY( TestAnd_8u_AC4R )
TEST_ENTRY( TestAnd_16u_C1R )
TEST_ENTRY( TestAnd_16u_C3R )
TEST_ENTRY( TestAnd_16u_C4R )
TEST_ENTRY( TestAnd_16u_AC4R )
TEST_ENTRY( TestAnd_32s_C1R )
TEST_ENTRY( TestAnd_32s_C3R )
TEST_ENTRY( TestAnd_32s_C4R )
TEST_ENTRY( TestAnd_32s_AC4R )
//Case 2
TEST_ENTRY( TestAnd_8u_C1IR )
TEST_ENTRY( TestAnd_8u_C3IR )
TEST_ENTRY( TestAnd_8u_C4IR )
TEST_ENTRY( TestAnd_8u_AC4IR )
TEST_ENTRY( TestAnd_16u_C1IR )
TEST_ENTRY( TestAnd_16u_C3IR )
TEST_ENTRY( TestAnd_16u_C4IR )
TEST_ENTRY( TestAnd_16u_AC4IR )
TEST_ENTRY( TestAnd_32s_C1IR )
TEST_ENTRY( TestAnd_32s_C3IR )
TEST_ENTRY( TestAnd_32s_C4IR )
TEST_ENTRY( TestAnd_32s_AC4IR )

END_TEST_TABLE()
