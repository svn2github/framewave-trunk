/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation
class TestMulScale_8u_C1R : public SrcSrcDstRoi< U8, 1, U8, 1 >
{
public:
    TestMulScale_8u_C1R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U8, 1, U8, 1 >( parent, "fwiMulScale_8u_C1R", fwiMulScale_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 3 2 3", "7 128 3 MAX", "0 1 0 3" );
     }
};

class TestMulScale_8u_C3R : public SrcSrcDstRoi< U8, 3, U8, 3 >
{
public:
    TestMulScale_8u_C3R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U8, 3, U8, 3 >( parent, "fwiMulScale_8u_C3R", fwiMulScale_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 2 3 MAX", "5 128 23 MAX 0 MAX", "0 1 23 2 0 MAX " );
    }
};

class TestMulScale_8u_AC4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestMulScale_8u_AC4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiMulScale_8u_AC4R", fwiMulScale_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 111 2 3 MAX 111", "5 128 MAX 222 MAX 128 1 222", "0 1 MAX 0 2 1 1 0 ");
    }
};

class TestMulScale_8u_C4R : public SrcSrcDstRoi< U8, 4, U8, 4 >
{
public:
    TestMulScale_8u_C4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U8, 4, U8, 4 >( parent, "fwiMulScale_8u_C4R", fwiMulScale_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 11 2 3 MAX 11", "5 128 MAX 22 MAX 128 1 22", "0 1 MAX 0 2 1 1 0" );
        
    }
};

class TestMulScale_16u_C1R : public SrcSrcDstRoi< U16, 1, U16, 1 >
{
public:
    TestMulScale_16u_C1R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U16, 1, U16, 1 >( parent, "fwiMulScale_16u_C1R", fwiMulScale_16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 3", "MAX 54900 MAX MAX MAX MIN", "2 2 2 3 2 0" );
       
    }
};

class TestMulScale_16u_C3R : public SrcSrcDstRoi< U16, 3, U16, 3 >
{
public:
    TestMulScale_16u_C3R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U16, 3, U16, 3 >( parent, "fwiMulScale_16u_C3R", fwiMulScale_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX MAX 3 5", "5 54900 2 MAX MIN 1", "0 2 2 65535 0 0" );

    }
};

class TestMulScale_16u_AC4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulScale_16u_AC4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiMulScale_16u_AC4R", fwiMulScale_16u_AC4R )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX 11 2 MAX 5 22", "5 54900 2 11 4 MIN 1 22", "0 2 2 0 0 MIN 0 0" );
    }
};

class TestMulScale_16u_C4R : public SrcSrcDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulScale_16u_C4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< U16, 4, U16, 4 >( parent, "fwiMulScale_16u_C4R", fwiMulScale_16u_C4R )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 2 MIN 5 222", "5 54900 MAX 111 4 MAX 1 MAX", "0 2 65535 0 0 0 0 222 " );
    }
};


//In-place Operation
class TestMulScale_8u_C1IR : public SrcSDstRoi< U8, 1, U8, 1 >
{
public:
    TestMulScale_8u_C1IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U8, 1, U8, 1 >( parent, "fwiMulScale_8u_C1IR", fwiMulScale_8u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 3", "7 128 3 MAX", "0 1 0 3" );
    }
};

class TestMulScale_8u_C3IR : public SrcSDstRoi< U8, 3, U8, 3 >
{
public:
    TestMulScale_8u_C3IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U8, 3, U8, 3 >( parent, "fwiMulScale_8u_C3IR", fwiMulScale_8u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 2 3 MAX", "5 128 MAX MAX 0 1", "0 1 MAX 2 0 1" );
             
    }
};


class TestMulScale_8u_AC4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public: 
    TestMulScale_8u_AC4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiMulScale_8u_AC4IR", fwiMulScale_8u_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 111 2 3 MAX 111", "5 128 MAX 222 MAX 0 1 222", "0 1 MAX 222 2 0 1 222 ");
    }
};

class TestMulScale_8u_C4IR : public SrcSDstRoi< U8, 4, U8, 4 >
{
public:
    TestMulScale_8u_C4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U8, 4, U8, 4 >( parent, "fwiMulScale_8u_C4IR", fwiMulScale_8u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 111 2 3 MAX 111", "5 128 MAX 222 MAX 0 1 222", "0 1 MAX 96 2 0 1 96");
        
    }
};

class TestMulScale_16u_C1IR : public SrcSDstRoi< U16, 1, U16, 1 >
{
public:
    TestMulScale_16u_C1IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U16, 1, U16, 1 >( parent, "fwiMulScale_16u_C1IR", fwiMulScale_16u_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 3 MAX", "7 54900 3 MAX MIN MAX", "0 2 3 2 0 65535 " );
    }
};

class TestMulScale_16u_C3IR : public SrcSDstRoi< U16, 3, U16, 3 >
{
public:
    TestMulScale_16u_C3IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U16, 3, U16, 3 >( parent, "fwiMulScale_16u_C3IR", fwiMulScale_16u_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 54900 MAX 1 3 5", "5 3 MAX MAX 0 1", "0 2 65535 1 0 0" );
    }
};


class TestMulScale_16u_AC4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulScale_16u_AC4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiMulScale_16u_AC4IR", fwiMulScale_16u_AC4IR)
    {}

    virtual void RunAll()
    {
        RunTest( "1 54900 MAX 111 2 3 MIN 111", "5 3 MAX 222 MAX 0 1 222", "0 2 65535 222 2 0 0 222 ");
    }
};

class TestMulScale_16u_C4IR : public SrcSDstRoi< U16, 4, U16, 4 >
{
public:
    TestMulScale_16u_C4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< U16, 4, U16, 4 >( parent, "fwiMulScale_16u_C4IR", fwiMulScale_16u_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 54900 MAX 111 2 3 MIN 111", "5 3 MAX 222 MAX 0 1 222", "0 2 65535 0 2 0 0 0");
    }
};


DEFINE_TEST_TABLE( MulScaleTestCatalog )

// All functions display Rounding Error
TEST_ENTRY( TestMulScale_8u_C1R )    
TEST_ENTRY( TestMulScale_8u_C3R )  
TEST_ENTRY( TestMulScale_8u_AC4R )     
TEST_ENTRY( TestMulScale_8u_C4R ) 
TEST_ENTRY( TestMulScale_16u_C1R )  
TEST_ENTRY( TestMulScale_16u_C3R )  
TEST_ENTRY( TestMulScale_16u_AC4R )     
TEST_ENTRY( TestMulScale_16u_C4R )
TEST_ENTRY( TestMulScale_8u_C1IR )        
TEST_ENTRY( TestMulScale_8u_C3IR )
TEST_ENTRY( TestMulScale_8u_AC4IR )     
TEST_ENTRY( TestMulScale_8u_C4IR ) 
TEST_ENTRY( TestMulScale_16u_C1IR ) 
TEST_ENTRY( TestMulScale_16u_C3IR ) 
TEST_ENTRY( TestMulScale_16u_AC4IR )   
TEST_ENTRY( TestMulScale_16u_C4IR ) 

END_TEST_TABLE()
