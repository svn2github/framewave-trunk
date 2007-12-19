/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ImageInitialization.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

class TestSet_8u_C1R : public ValDstRoi< U8, 1 >
{
public:
    TestSet_8u_C1R( UnitTestCatalogBase & parent ) : 
      ValDstRoi< U8, 1 >( parent, "fwiSet_8u_C1R", fwiSet_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 2 2" );
     }
};

class TestSet_8u_C3R : public PValDstRoi< U8, 3 >
{
public:
    TestSet_8u_C3R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< U8, 3 >( parent, "fwiSet_8u_C3R", fwiSet_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 255 1 3 255 " );
    }
};

class TestSet_8u_AC4R : public PValDstRoi< U8, 4 >
{
public:
    TestSet_8u_AC4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< U8, 4 >( parent, "fwiSet_8u_AC4R", fwiSet_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 111", "1 3 255 0 1 3 255 0");
    }
};

class TestSet_8u_C4R : public PValDstRoi< U8, 4 >
{
public:
    TestSet_8u_C4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< U8, 4 >( parent, "fwiSet_8u_C4R", fwiSet_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 11 2 3 MAX 11", "1 3 MAX 11 1 3 MAX 11" );
        
    }
};

class TestSet_8u_C3CR : public ValDstRoi< U8, 3 >
{
public:
    TestSet_8u_C3CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< U8, 3 >( parent, "fwiSet_8u_C3CR", fwiSet_8u_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 ", "1 0 0 1 0 0 " );
    }
};

class TestSet_8u_C4CR : public ValDstRoi< U8, 4 >
{
public:
    TestSet_8u_C4CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< U8, 4 >( parent, "fwiSet_8u_C4CR", fwiSet_8u_C4CR )
    {}

    virtual void RunAll()
    {
        RunTest( "255", "255 0 0 0 255 0 0 0" );
        
    }
};


class TestSet_16s_C1R : public ValDstRoi<  S16, 1 >
{
public:
    TestSet_16s_C1R( UnitTestCatalogBase & parent ) : 
      ValDstRoi<  S16, 1 >( parent, "fwiSet_16s_C1R", fwiSet_16s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 2 2 2 2" );
       
    }
};

class TestSet_16s_C3R : public PValDstRoi< S16, 3 >
{
public:
    TestSet_16s_C3R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S16, 3 >( parent, "fwiSet_16s_C3R", fwiSet_16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 32767 1 3 32767" );

    }
};

class TestSet_16s_AC4R : public PValDstRoi< S16, 4 >
{
public:
    TestSet_16s_AC4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S16, 4 >( parent, "fwiSet_16s_AC4R", fwiSet_16s_AC4R )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ", "1 3 32767 0 1 3 32767 0" );
    }
};

class TestSet_16s_C4R : public PValDstRoi< S16, 4 >
{
public:
    TestSet_16s_C4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S16, 4 >( parent, "fwiSet_16s_C4R", fwiSet_16s_C4R )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "1 3 32767 111 1 3 32767 111" );
    }
};

class TestSet_16s_C3CR : public ValDstRoi< S16, 3 >
{
public:
    TestSet_16s_C3CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< S16, 3 >( parent, "fwiSet_16s_C3CR", fwiSet_16s_C3CR )
    {}

    virtual void RunAll()
    {   
        RunTest( "1",  "1 0 0 1 0 0 " );
    }
};

class TestSet_16s_C4CR : public ValDstRoi< S16, 4 >
{
public:
    TestSet_16s_C4CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< S16, 4 >( parent, "fwiSet_16s_C4CR", fwiSet_16s_C4CR )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 ",  "1 0 0 0 1 0 0 0" );
    }
};

class TestSet_32s_C1R : public ValDstRoi<  S32, 1 >
{
public:
    TestSet_32s_C1R( UnitTestCatalogBase & parent ) : 
      ValDstRoi<  S32, 1 >( parent, "fwiSet_32s_C1R", fwiSet_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 2 2 2 2" );
       
    }
};

class TestSet_32s_C3R : public PValDstRoi< S32, 3 >
{
public:
    TestSet_32s_C3R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S32, 3 >( parent, "fwiSet_32s_C3R", fwiSet_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 MAX 1 3 MAX" );

    }
};

class TestSet_32s_AC4R : public PValDstRoi< S32, 4 >
{
public:
    TestSet_32s_AC4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S32, 4 >( parent, "fwiSet_32s_AC4R", fwiSet_32s_AC4R )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ",  "1 3 MAX 0 1 3 MAX 0" );
    }
};

class TestSet_32s_C4R : public PValDstRoi< S32, 4 >
{
public:
    TestSet_32s_C4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< S32, 4 >( parent, "fwiSet_32s_C4R", fwiSet_32s_C4R )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "1 3 MAX 111 1 3 MAX 111" );
    }
};

class TestSet_32s_C3CR : public ValDstRoi< S32, 3 >
{
public:
    TestSet_32s_C3CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< S32, 3 >( parent, "fwiSet_32s_C3CR", fwiSet_32s_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 ",  "1 0 0 1 0 0" );

    }
};

class TestSet_32s_C4CR : public ValDstRoi< S32, 4 >
{
public:
    TestSet_32s_C4CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< S32, 4 >( parent, "fwiSet_32s_C4CR", fwiSet_32s_C4CR )
    {}

    virtual void RunAll()
    {   
        RunTest( "111 ", "111 0 0 0 111 0 0 0" );
    }
};

class TestSet_32f_C1R : public ValDstRoi< F32, 1 >
{
public:
    TestSet_32f_C1R( UnitTestCatalogBase & parent ) : 
      ValDstRoi< F32, 1 >( parent, "fwiSet_32f_C1R", fwiSet_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 2 2 2 2" );
       
    }
};

class TestSet_32f_C3R : public PValDstRoi< F32, 3 >
{
public:
    TestSet_32f_C3R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< F32, 3 >( parent, "fwiSet_32f_C3R", fwiSet_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 MAX 1 3 MAX" );

    }
};

class TestSet_32f_AC4R : public PValDstRoi< F32, 4 >
{
public:
    TestSet_32f_AC4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< F32, 4 >( parent, "fwiSet_32f_AC4R", fwiSet_32f_AC4R )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ", "1 3 MAX 0 1 3 MAX 0" );
    }
};

class TestSet_32f_C4R : public PValDstRoi< F32, 4 >
{
public:
    TestSet_32f_C4R( UnitTestCatalogBase & parent ) : 
      PValDstRoi< F32, 4 >( parent, "fwiSet_32f_C4R", fwiSet_32f_C4R )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "1 3 MAX 111 1 3 MAX 111" );
    }
};

class TestSet_32f_C3CR : public ValDstRoi< F32, 3 >
{
public:
    TestSet_32f_C3CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< F32, 3 >( parent, "fwiSet_32f_C3CR", fwiSet_32f_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "3 ", "3 0 0 3 0 0" );

    }
};

class TestSet_32f_C4CR : public ValDstRoi< F32, 4 >
{
public:
    TestSet_32f_C4CR( UnitTestCatalogBase & parent ) : 
      ValDstRoi< F32, 4 >( parent, "fwiSet_32f_C4CR", fwiSet_32f_C4CR )
    {}

    virtual void RunAll()
    {   
        RunTest( "111 ", "111 0 0 0 111 0 0 0" );
    }
};

//Masked

class TestSet_8u_C1MR : public ValDstRoiMask< U8, 1 >
{
public:
    TestSet_8u_C1MR( UnitTestCatalogBase & parent ) : 
      ValDstRoiMask< U8, 1 >( parent, "fwiSet_8u_C1MR", fwiSet_8u_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 0 0", "1 2 0 0" );
     }
};
 
class TestSet_8u_C3MR : public PValDstRoiMask< U8, 3 >
{
public:
    TestSet_8u_C3MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< U8, 3 >( parent, "fwiSet_8u_C3MR", fwiSet_8u_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 255 0 0 0 1 3 255 0 0 0 ", "1 0 2 0" );
    }
};

class TestSet_8u_AC4MR : public PValDstRoiMask< U8, 4 >
{
public:
    TestSet_8u_AC4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< U8, 4 >( parent, "fwiSet_8u_AC4MR", fwiSet_8u_AC4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 111", "1 3 255 0 0 0 0 0", " 1 0 ");
    }
};

class TestSet_8u_C4MR : public PValDstRoiMask< U8, 4 >
{
public:
    TestSet_8u_C4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< U8, 4 >( parent, "fwiSet_8u_C4MR", fwiSet_8u_C4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX 11 ", "1 3 MAX 11 0 0 0 0", " 3 0 " );
        
    }
};


class TestSet_16s_C1MR : public ValDstRoiMask<  S16, 1 >
{
public:
    TestSet_16s_C1MR( UnitTestCatalogBase & parent ) : 
      ValDstRoiMask<  S16, 1 >( parent, "fwiSet_16s_C1MR", fwiSet_16s_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 0 2 0 2 0", " 1 0 1 0 1 0" );
       
    }
};

class TestSet_16s_C3MR : public PValDstRoiMask< S16, 3 >
{
public:
    TestSet_16s_C3MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S16, 3 >( parent, "fwiSet_16s_C3MR", fwiSet_16s_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "0 0 0 1 3 32767", "0 5" );

    }
};

class TestSet_16s_AC4MR : public PValDstRoiMask< S16, 4 >
{
public:
    TestSet_16s_AC4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S16, 4 >( parent, "fwiSet_16s_AC4MR", fwiSet_16s_AC4MR )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ", "1 3 32767 0 0 0 0 0", " 1 0" );
    }
};

class TestSet_16s_C4MR : public PValDstRoiMask< S16, 4 >
{
public:
    TestSet_16s_C4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S16, 4 >( parent, "fwiSet_16s_C4MR", fwiSet_16s_C4MR )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "0 0 0 0 1 3 32767 111", " 0 6" );
    }
};


class TestSet_32s_C1MR : public ValDstRoiMask<  S32, 1 >
{
public:
    TestSet_32s_C1MR( UnitTestCatalogBase & parent ) : 
      ValDstRoiMask<  S32, 1 >( parent, "fwiSet_32s_C1MR", fwiSet_32s_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "2 2 2 0 0 0", " 1 2 3 0 0 0" );
       
    }
};

class TestSet_32s_C3MR : public PValDstRoiMask< S32, 3 >
{
public:
    TestSet_32s_C3MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S32, 3 >( parent, "fwiSet_32s_C3MR", fwiSet_32s_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 MAX 1 3 MAX", " 1 2" );

    }
};

class TestSet_32s_AC4MR : public PValDstRoiMask< S32, 4 >
{
public:
    TestSet_32s_AC4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S32, 4 >( parent, "fwiSet_32s_AC4MR", fwiSet_32s_AC4MR )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ",  "1 3 MAX 0 1 3 MAX 0 0 0 0 0" , " 2 4 0");
    }
};

class TestSet_32s_C4MR : public PValDstRoiMask< S32, 4 >
{
public:
    TestSet_32s_C4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< S32, 4 >( parent, "fwiSet_32s_C4MR", fwiSet_32s_C4MR )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "1 3 MAX 111 1 3 MAX 111 0 0 0 0", " 2 4 0" );
    }
};


class TestSet_32f_C1MR : public ValDstRoiMask< F32, 1 >
{
public:
    TestSet_32f_C1MR( UnitTestCatalogBase & parent ) : 
      ValDstRoiMask< F32, 1 >( parent, "fwiSet_32f_C1MR", fwiSet_32f_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 ", "0 2 0 2 0 2", " 0 1 0 2 0 6" );
       
    }
};

class TestSet_32f_C3MR : public PValDstRoiMask< F32, 3 >
{
public:
    TestSet_32f_C3MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< F32, 3 >( parent, "fwiSet_32f_C3MR", fwiSet_32f_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 3 MAX ", "1 3 MAX 1 3 MAX", " 2 1" );

    }
};

class TestSet_32f_AC4MR : public PValDstRoiMask< F32, 4 >
{
public:
    TestSet_32f_AC4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< F32, 4 >( parent, "fwiSet_32f_AC4MR", fwiSet_32f_AC4MR )
    {}

    virtual void RunAll()
    {  
        RunTest( "1 3 MAX ", "1 3 MAX 0 0 0 0 0 1 3 MAX 0", " 1 0 1" );
    }
};

class TestSet_32f_C4MR : public PValDstRoiMask< F32, 4 >
{
public:
    TestSet_32f_C4MR( UnitTestCatalogBase & parent ) : 
      PValDstRoiMask< F32, 4 >( parent, "fwiSet_32f_C4MR", fwiSet_32f_C4MR )
    {}

    virtual void RunAll()
    {   
        RunTest( "1 3 MAX 111 ", "1 3 MAX 111 0 0 0 0 1 3 MAX 111", " 1 0 1" );
    }
};

DEFINE_TEST_TABLE( SetTestCatalog )

TEST_ENTRY( TestSet_8u_C1R )    
TEST_ENTRY( TestSet_8u_C3R )  
TEST_ENTRY( TestSet_8u_AC4R )     
TEST_ENTRY( TestSet_8u_C4R ) 
TEST_ENTRY( TestSet_8u_C3CR )
TEST_ENTRY( TestSet_8u_C4CR )

TEST_ENTRY( TestSet_16s_C1R )  
TEST_ENTRY( TestSet_16s_C3R )  
TEST_ENTRY( TestSet_16s_AC4R )     
TEST_ENTRY( TestSet_16s_C4R )
TEST_ENTRY( TestSet_16s_C3CR )  
TEST_ENTRY( TestSet_16s_C4CR )  

TEST_ENTRY( TestSet_32s_C1R )  
TEST_ENTRY( TestSet_32s_C3R )  
TEST_ENTRY( TestSet_32s_AC4R )     
TEST_ENTRY( TestSet_32s_C4R )
TEST_ENTRY( TestSet_32s_C3CR )  
TEST_ENTRY( TestSet_32s_C4CR )

TEST_ENTRY( TestSet_32f_C1R )  
TEST_ENTRY( TestSet_32f_C3R )  
TEST_ENTRY( TestSet_32f_AC4R )     
TEST_ENTRY( TestSet_32f_C4R )
TEST_ENTRY( TestSet_32f_C3CR )  
TEST_ENTRY( TestSet_32f_C4CR )

//Masked

TEST_ENTRY( TestSet_8u_C1MR )
TEST_ENTRY( TestSet_8u_C3MR )  
TEST_ENTRY( TestSet_8u_AC4MR )     
TEST_ENTRY( TestSet_8u_C4MR ) 

TEST_ENTRY( TestSet_16s_C1MR )  
TEST_ENTRY( TestSet_16s_C3MR )  
TEST_ENTRY( TestSet_16s_AC4MR )     
TEST_ENTRY( TestSet_16s_C4MR )

TEST_ENTRY( TestSet_32s_C1MR )  
TEST_ENTRY( TestSet_32s_C3MR )  
TEST_ENTRY( TestSet_32s_AC4MR )     
TEST_ENTRY( TestSet_32s_C4MR )

TEST_ENTRY( TestSet_32f_C1MR )  
TEST_ENTRY( TestSet_32f_C3MR )  
TEST_ENTRY( TestSet_32f_AC4MR )     
TEST_ENTRY( TestSet_32f_C4MR )


END_TEST_TABLE()
