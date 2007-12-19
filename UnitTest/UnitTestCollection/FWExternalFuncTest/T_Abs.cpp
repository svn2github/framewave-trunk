/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation .
class TestAbs_16s_C1R : public SrcDstRoi< S16, 1, S16, 1 >
{
public:
    TestAbs_16s_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< S16, 1, S16, 1 >( parent, "fwiAbs_16s_C1R", fwiAbs_16s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8", "5 6 7 8");
		RunTest( "MAX MIN", "MAX MAX");
    }
};

class TestAbs_16s_C3R : public SrcDstRoi< S16, 3, S16, 3 >
{
public:
    TestAbs_16s_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< S16, 3, S16, 3 >( parent, "fwiAbs_16s_C3R", fwiAbs_16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10", "5 6 7 8 9 10");
		RunTest( "MAX MIN 0", "MAX MAX 0");
    }
};

class TestAbs_16s_C4R : public SrcDstRoi< S16, 4, S16, 4 >
{
public:
    TestAbs_16s_C4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< S16, 4, S16, 4 >( parent, "fwiAbs_16s_C4R", fwiAbs_16s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 8 9 10 11 12");
		RunTest( "MAX MIN 0 MIN", "MAX MAX 0 MAX");
    }
};

class TestAbs_16s_AC4R : public SrcDstRoi< S16, 4, S16, 4 >
{
public:
    TestAbs_16s_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< S16, 4, S16, 4 >( parent, "fwiAbs_16s_AC4R", fwiAbs_16s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 0 9 10 11 0");
		RunTest( "MAX MIN 0 MIN", "MAX MAX 0 0");
    }
};

class TestAbs_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestAbs_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiAbs_32f_C1R", fwiAbs_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.456789", "5.1 6.2 7.3 8.4567", 0.0001f);
		RunTest( "MAX MIN", "MAX MAX", 0.0001f);
    }
};

class TestAbs_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestAbs_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiAbs_32f_C3R", fwiAbs_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.4 -9.5 10.67891", "5.1 6.2 7.3 8.4 9.5 10.6789", 0.0001f);
		RunTest( "MAX MIN 0", "MAX MAX 0", 0.0001f);
    }
};

class TestAbs_32f_C4R : public SrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestAbs_32f_C4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiAbs_32f_C4R", fwiAbs_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.4 -9.5 10.6 -11.7 -12.89012", "5.1 6.2 7.3 8.4 9.5 10.6 11.7 12.8901", 0.0001f);
		RunTest( "MAX MIN 0 -3.14159e+38", "MAX MAX 0 3.14159e+38", 0.0001f);
    }
};

class TestAbs_32f_AC4R : public SrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestAbs_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiAbs_32f_AC4R", fwiAbs_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 0 9 10 11 0", 0.0001f);
		RunTest( "MAX MIN 0 -3.14159e+38", "MAX MAX 0 0", 0.0001f);
    }
};

//Case 2: In-place operation
class TestAbs_16s_C1IR : public SDstRoi< S16, 1 >
{
public:
    TestAbs_16s_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< S16, 1 >( parent, "fwiAbs_16s_C1IR", fwiAbs_16s_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8", "5 6 7 8");
		RunTest( "MAX MIN", "MAX MAX");
    }
};

class TestAbs_16s_C3IR : public SDstRoi< S16, 3 >
{
public:
    TestAbs_16s_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< S16, 3 >( parent, "fwiAbs_16s_C3IR", fwiAbs_16s_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10", "5 6 7 8 9 10");
		RunTest( "MAX MIN 0", "MAX MAX 0");
    }
};

class TestAbs_16s_C4IR : public SDstRoi< S16, 4 >
{
public:
    TestAbs_16s_C4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< S16, 4 >( parent, "fwiAbs_16s_C4IR", fwiAbs_16s_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 8 9 10 11 12");
		RunTest( "MAX MIN 0 MIN", "MAX MAX 0 MAX");
    }
};

class TestAbs_16s_AC4IR : public SDstRoi< S16, 4 >
{
public:
    TestAbs_16s_AC4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< S16, 4 >( parent, "fwiAbs_16s_AC4IR", fwiAbs_16s_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 -8 9 10 11 -12");
		RunTest( "MAX MIN 0 MIN", "MAX MAX 0 MIN");
    }
};

class TestAbs_32f_C1IR : public SDstRoi< F32, 1 >
{
public:
    TestAbs_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 1 >( parent, "fwiAbs_32f_C1IR", fwiAbs_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.456789", "5.1 6.2 7.3 8.4567", 0.0001f);
		RunTest( "MAX MIN", "MAX MAX", 0.0001f);
    }
};

class TestAbs_32f_C3IR : public SDstRoi< F32, 3 >
{
public:
    TestAbs_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 3 >( parent, "fwiAbs_32f_C3IR", fwiAbs_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.4 -9.5 10.67891", "5.1 6.2 7.3 8.4 9.5 10.6789", 0.0001f);
		RunTest( "MAX MIN 0", "MAX MAX 0", 0.0001f);
    }
};

class TestAbs_32f_C4IR : public SDstRoi< F32, 4 >
{
public:
    TestAbs_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiAbs_32f_C4IR", fwiAbs_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5.1 -6.2 7.3 -8.4 -9.5 10.6 -11.7 -12.89012", "5.1 6.2 7.3 8.4 9.5 10.6 11.7 12.8901", 0.0001f);
		RunTest( "MAX MIN 0 -3.14159e+38", "MAX MAX 0 3.14159e+38", 0.0001f);
    }
};

class TestAbs_32f_AC4IR : public SDstRoi< F32, 4 >
{
public:
    TestAbs_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  SDstRoi< F32, 4 >( parent, "fwiAbs_32f_AC4IR", fwiAbs_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "5 -6 7 -8 -9 10 -11 -12", "5 6 7 -8 9 10 11 -12", 0.0001f);
		RunTest( "MAX MIN 0 -3.14159e+38", "MAX MAX 0 -3.14159e+38", 0.0001f);
    }
};

DEFINE_TEST_TABLE( AbsTestCatalog )
//Case 1
TEST_ENTRY( TestAbs_16s_C1R )
TEST_ENTRY( TestAbs_16s_C3R )
TEST_ENTRY( TestAbs_16s_C4R )
TEST_ENTRY( TestAbs_16s_AC4R )
TEST_ENTRY( TestAbs_32f_C1R )
TEST_ENTRY( TestAbs_32f_C3R )
TEST_ENTRY( TestAbs_32f_C4R )
TEST_ENTRY( TestAbs_32f_AC4R )
//Case 2
TEST_ENTRY( TestAbs_16s_C1IR )
TEST_ENTRY( TestAbs_16s_C3IR )
TEST_ENTRY( TestAbs_16s_C4IR )
TEST_ENTRY( TestAbs_16s_AC4IR )
TEST_ENTRY( TestAbs_32f_C1IR )
TEST_ENTRY( TestAbs_32f_C3IR )
TEST_ENTRY( TestAbs_32f_C4IR )
TEST_ENTRY( TestAbs_32f_AC4IR )

END_TEST_TABLE()
