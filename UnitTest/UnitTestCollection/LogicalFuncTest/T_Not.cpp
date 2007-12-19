/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "LogicalTests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation
class TestNot_8u_C1R : public SrcDstRoi< U8, 1, U8, 1 >
{
public:
    TestNot_8u_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 1, U8, 1 >( parent, "fwiNot_8u_C1R", fwiNot_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 192 191 239 1 0");
    }
};

class TestNot_8u_C3R : public SrcDstRoi< U8, 3, U8, 3 >
{
public:
    TestNot_8u_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 3, U8, 3 >( parent, "fwiNot_8u_C3R", fwiNot_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 100", "255 254 233 192 191 239 1 0 155");
    }
};

class TestNot_8u_C4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestNot_8u_C4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 4, U8, 4 >( parent, "fwiNot_8u_C4R", fwiNot_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 192 191 239 1 0");
    }
};

class TestNot_8u_AC4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestNot_8u_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< U8, 4, U8, 4 >( parent, "fwiNot_8u_AC4R", fwiNot_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 0 191 239 1 0");
    }
};

//Case 2: In-place operation
class TestNot_8u_C1IR : public SDstRoi< U8, 1 >
{
public:
    TestNot_8u_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< U8, 1 >( parent, "fwiNot_8u_C1IR", fwiNot_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 192 191 239 1 0");
    }
};

class TestNot_8u_C3IR : public SDstRoi< U8, 3 >
{
public:
    TestNot_8u_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< U8, 3 >( parent, "fwiNot_8u_C3IR", fwiNot_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX 100", "255 254 233 192 191 239 1 0 155");
    }
};

class TestNot_8u_C4IR : public SDstRoi< U8, 4>
{
public:
    TestNot_8u_C4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< U8, 4 >( parent, "fwiNot_8u_C4IR", fwiNot_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 192 191 239 1 0");
    }
};

class TestNot_8u_AC4IR : public SDstRoi< U8, 4 >
{
public:
    TestNot_8u_AC4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< U8, 4 >( parent, "fwiNot_8u_AC4IR", fwiNot_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 22 63 64 16 254 MAX", "255 254 233 63 191 239 1 MAX");
    }
};

DEFINE_TEST_TABLE( NotTestCatalog )
//Case 1
TEST_ENTRY( TestNot_8u_C1R )
TEST_ENTRY( TestNot_8u_C3R )
TEST_ENTRY( TestNot_8u_C4R )
TEST_ENTRY( TestNot_8u_AC4R )

//Case 2
TEST_ENTRY( TestNot_8u_C1IR )
TEST_ENTRY( TestNot_8u_C3IR )
TEST_ENTRY( TestNot_8u_C4IR )
TEST_ENTRY( TestNot_8u_AC4IR )

END_TEST_TABLE()
