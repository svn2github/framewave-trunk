/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel integer or complex data.
class TestMulCScale_8u_C1R : public SrcValDstRoi< U8, 1, U8, 1 >
{
public:
    TestMulCScale_8u_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< U8, 1, U8, 1 >( parent, "fwiMulCScale_8u_C1R", fwiMulCScale_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest(  "MAX 100 3 22 128", "3", "3 1 0 0 2");
    }
};

class TestMulCScale_16u_C1R : public SrcValDstRoi< U16, 1, U16, 1 >
{
public:
    TestMulCScale_16u_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< U16, 1, U16, 1 >( parent, "fwiMulCScale_16u_C1R", fwiMulCScale_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "54900 100 3 MAX 6241 33", "3", "3 0 0 3 0 0" );
    }
};

//Case 2: Not-in-place operation on multi-channel integer or complex data
class TestMulCScale_8u_C3R : public SrcPValDstRoi< U8, 3, U8, 3 >
{
public:
    TestMulCScale_8u_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U8, 3, U8, 3 >( parent, "fwiMulCScale_8u_C3R", fwiMulCScale_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest("10 128 MAX MAX 22 50", "2 3 MAX", "0 2 255 2 0 50" );
    }
};

class TestMulCScale_8u_AC4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestMulCScale_8u_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiMulCScale_8u_AC4R", fwiMulCScale_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest("128 9 MAX 222 MAX 22 50 222", "3 3 MAX 111", "2 0 255 0 3 0 50 0" );
    }
};


class TestMulCScale_8u_C4R : public SrcPValDstRoi< U8, 4, U8, 4 >
{
public:
    TestMulCScale_8u_C4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U8, 4, U8, 4 >( parent, "fwiMulCScale_8u_C4R", fwiMulCScale_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest("128 9 MAX 222 MAX 22 50 222", "3 3 MAX 111", "2 0 255 97 3 0 50 97" );
    }
};

class TestMulCScale_16u_C3R : public SrcPValDstRoi< U16, 3, U16, 3 >
{
public:
    TestMulCScale_16u_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U16, 3, U16, 3 >( parent, "fwiMulCScale_16u_C3R", fwiMulCScale_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest("10 54900 MAX MAX 22 50", "2 3 MAX", "0 3 65535 2 0 50 " );
    }
};

class TestMulCScale_16u_AC4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulCScale_16u_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiMulCScale_16u_AC4R", fwiMulCScale_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest("10 54900 MAX 222 MAX 22 50 222", "2 3 MAX 111", "0 3 65535 0 2 0 50 0" );
    }
};

class TestMulCScale_16u_C4R : public SrcPValDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulCScale_16u_C4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< U16, 4, U16, 4 >( parent, "fwiMulCScale_16u_C4R", fwiMulCScale_16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest("0 54900 MAX 222 MAX 222 50 22", "3 3 MAX 111", "0 3 65535 0 3 0 50 0 " );
    }
};


//Case 3: In-place operation on 1-channel integer or complex data
class TestMulCScale_8u_C1IR : public ValSDstRoi< U8, 1 >
{
public:
    TestMulCScale_8u_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< U8, 1 >( parent, "fwiMulCScale_8u_C1IR", fwiMulCScale_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3", "7 128 3 MAX", "0 2 0 3" );
    }
};

class TestMulCScale_16u_C1IR : public ValSDstRoi< U16, 1>
{
public:
    TestMulCScale_16u_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< U16, 1>( parent, "fwiMulCScale_16u_C1IR", fwiMulCScale_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3", "7 54900 3 MAX MIN 33", "0 3 0 3 0 0" );
    }
};


//Case 4: In-place operation on multi-channel integer or complex data.
class TestMulCScale_8u_C3IR : public PValSDstRoi< U8, 3 >
{
public:
    TestMulCScale_8u_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U8, 3 >( parent, "fwiMulCScale_8u_C3IR", fwiMulCScale_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX", "10 128 MAX MAX 22 50", "0 2 255 2 0 50" );
    }
};

class TestMulCScale_8u_AC4IR : public PValSDstRoi< U8, 4>
{
public:
    TestMulCScale_8u_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U8, 4>( parent, "fwiMulCScale_8u_AC4IR", fwiMulCScale_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 128 MAX 222 MAX 22 50 222", "0 2 255 222 2 0 50 222" );
    }
};

class TestMulCScale_8u_C4IR : public PValSDstRoi< U8, 4>
{
public:
    TestMulCScale_8u_C4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U8, 4>( parent, "fwiMulCScale_8u_C4IR", fwiMulCScale_8u_C4IR)
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 128 MAX 222 MAX 22 50 222", "0 2 255 96 2 0 50 96 " );
    }
};

class TestMulCScale_16u_C3IR : public PValSDstRoi< U16, 3>
{
public:
    TestMulCScale_16u_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U16, 3>( parent, "fwiMulCScale_16u_C3IR", fwiMulCScale_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX", "10 54900 MAX MAX 22 50", "0 3 65535 2 0 50" );
    }
};

class TestMulCScale_16u_AC4IR : public PValSDstRoi< U16, 4>
{
public:
    TestMulCScale_16u_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U16, 4>( parent, "fwiMulCScale_16u_AC4IR", fwiMulCScale_16u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 54900 MAX 222 MAX 22 50 222", "0 3 65535 222 2 0 50 222" );
    }
};

class TestMulCScale_16u_C4IR : public PValSDstRoi< U16, 4>
{
public:
    TestMulCScale_16u_C4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< U16, 4>( parent, "fwiMulCScale_16u_C4IR", fwiMulCScale_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 54900 MAX 222 MAX 22 50 222", "0 3 65535 0 2 0 50 0 " );
    }
};






DEFINE_TEST_TABLE( MulCScaleTestCatalog )

//All functions display rounding error
//Case 1
TEST_ENTRY( TestMulCScale_8u_C1R )         
TEST_ENTRY( TestMulCScale_16u_C1R )   //Failing 

//Case 2
TEST_ENTRY( TestMulCScale_8u_C3R )  
TEST_ENTRY( TestMulCScale_8u_AC4R )   //Failing 
TEST_ENTRY( TestMulCScale_8u_C4R )    //Failing 
TEST_ENTRY( TestMulCScale_16u_C3R ) 
TEST_ENTRY( TestMulCScale_16u_AC4R )  //Failing
TEST_ENTRY( TestMulCScale_16u_C4R )   //Failing 
//Case 3
TEST_ENTRY( TestMulCScale_8u_C1IR )         
TEST_ENTRY( TestMulCScale_16u_C1IR )  //Failing

//Case 4
TEST_ENTRY( TestMulCScale_8u_C3IR )   
TEST_ENTRY( TestMulCScale_8u_AC4IR )  //Failing 
TEST_ENTRY( TestMulCScale_8u_C4IR )   //Failing 
TEST_ENTRY( TestMulCScale_16u_C3IR )  
TEST_ENTRY( TestMulCScale_16u_AC4IR ) //Failing
TEST_ENTRY( TestMulCScale_16u_C4IR )  //Failing


END_TEST_TABLE()
