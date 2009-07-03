/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

class TestAddProduct_8u32f_C1IR : public SrcSrcSDstRoi< U8, 1, F32, 1 >
{
public:
    TestAddProduct_8u32f_C1IR( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoi< U8, 1, F32, 1 >( parent, "fwiAddProduct_8u32f_C1IR", fwiAddProduct_8u32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 0 ", "8 5 4 6 3.3 0","1 2 3 4 5 6" ,
            "25 12 35 16 11 6 ", 0.0001f ); //Pi's digit
         RunTest( "5 6 16 100 1 5", "7 100 3 2 MAX 2 ","1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 602.222 51.333 204.444 260.555 16.666  ", 0.0001f );
        
    }
};

class TestAddProduct_8s32f_C1IR  : public SrcSrcSDstRoi< S8, 1, F32, 1 >
{
public:
    TestAddProduct_8s32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoi< S8, 1, F32, 1 >( parent, "fwiAddProduct_8s32f_C1IR", fwiAddProduct_8s32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2 MAX ", "8 5 4 6 3 1","1.11 2.22 3.33 4.44 5.55 6.66",
            "25.11 12.22 35.33 16.44 11.55 133.66", 0.0001f ); //Pi's digit
         RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001","1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 602.222 51.333 204.444 7.555 6.666 ", 0.0001f );
        
    }
};
class TestAddProduct_16u32f_C1IR  : public SrcSrcSDstRoi< U16, 1, F32, 1 >
{
public:
    TestAddProduct_16u32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoi< U16, 1, F32, 1 >( parent, "fwiAddProduct_16u32f_C1IR", fwiAddProduct_16u32f_C1IR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2 MAX ", "8 5 4 6 3 1","1.11 2.22 3.33 4.44 5.55 6.66",
            "25.11 12.22 35.33 16.44 11.55 65541.66", 0.0001f ); //Pi's digit
       RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001","1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 602.222 51.333 204.444 7.555 6.666 ", 0.0001f );
        
    }
};

class TestAddProduct_32f_C1IR  : public SrcSrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestAddProduct_32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiAddProduct_32f_C1IR", fwiAddProduct_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.2323 2.001 8.23 2.1 2.2 0 ", "8 5 4 6 3.3 1","0.629595 6.28318 7.9605 45.79443 0.12345 MAX",
                 "26.487995 16.28818 40.8805 58.39443 7.38345 MAX ", 0.0001f ); //Pi's digit
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 -2.6535 8.9793"," ", 
                 "14 -300 0.629595 6.28318 7.9605 45.79443 ", 0.0001f );
        
    }
};

class TestAddProduct_8u32f_C1IMR : public SrcSrcSDstRoiMask< U8, 1, F32, 1 >
{
public:
    TestAddProduct_8u32f_C1IMR( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoiMask< U8, 1, F32, 1 >( parent, "fwiAddProduct_8u32f_C1IMR", fwiAddProduct_8u32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 0 ", "8 5 4 6 3.3 0",
            "1 0 1 0 1 0", "1 2 3 4 5 6" , "25 2 35 4 11 6", 0.0001f ); //Pi's digit
         RunTest( "5 6 16 100 1 5", "7 100 3 2 MAX 2 "," 1 1 1 0 0 0", "1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 602.222 51.333 4.444 5.555 6.666 ", 0.0001f );
        
    }
};

class TestAddProduct_8s32f_C1IMR  : public SrcSrcSDstRoiMask< S8, 1, F32, 1 >
{
public:
    TestAddProduct_8s32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoiMask< S8, 1, F32, 1 >( parent, "fwiAddProduct_8s32f_C1IMR", fwiAddProduct_8s32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2 MAX ", "8 5 4 6 3 1"," 0 0 0 0 0 0", "1.11 2.22 3.33 4.44 5.55 6.66",
            "1.11 2.22 3.33 4.44 5.55 6.66", 0.0001f ); //Pi's digit
         RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001","1 0 0 1 0 0", "1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 2.222 3.333 204.444 5.555 6.666 ", 0.0001f );
        
    }
};
class TestAddProduct_16u32f_C1IMR  : public SrcSrcSDstRoiMask< U16, 1, F32, 1 >
{
public:
    TestAddProduct_16u32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoiMask< U16, 1, F32, 1 >( parent, "fwiAddProduct_16u32f_C1IMR", fwiAddProduct_16u32f_C1IMR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2 MAX ", "8 5 4 6 3 1","0 0 1 0 1 0", "1.11 2.22 3.33 4.44 5.55 6.66",
            "1.11 2.22 35.33 4.44 11.55 6.66", 0.0001f ); //Pi's digit
       RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001","1 0 1 0 1 0", "1.111 2.222 3.333 4.444 5.555 6.666", 
            "36.111 2.222 51.333 4.444 7.555 6.666 ", 0.0001f );
        
    }
};

class TestAddProduct_32f_C1IMR  : public SrcSrcSDstRoiMask< F32, 1, F32, 1 >
{
public:
    TestAddProduct_32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSrcSDstRoiMask< F32, 1, F32, 1 >( parent, "fwiAddProduct_32f_C1IMR", fwiAddProduct_32f_C1IMR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.2323 2.001 8.23 2.1 2.2 0 ", "8 5 4 6 3.3 1", " 0 1 1 3 1 0", "0.629595 6.28318 7.9605 45.79443 0.12345 MAX",
                 "0.629595 16.28818 40.8805 58.39443 7.38345 MAX ", 0.0001f ); //Pi's digit
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 -2.6535 8.9793","1 0 0 0 1 1", " ", 
                 "14 0 0 0 7.9605 45.79443 ", 0.0001f );
        
    }
};




DEFINE_TEST_TABLE( AddProductTestCatalog )

TEST_ENTRY( TestAddProduct_8u32f_C1IR )  
TEST_ENTRY( TestAddProduct_8s32f_C1IR )
TEST_ENTRY( TestAddProduct_16u32f_C1IR)
TEST_ENTRY( TestAddProduct_32f_C1IR)

TEST_ENTRY( TestAddProduct_8u32f_C1IMR ) 
TEST_ENTRY( TestAddProduct_8s32f_C1IMR ) 
TEST_ENTRY( TestAddProduct_16u32f_C1IMR) 
TEST_ENTRY( TestAddProduct_32f_C1IMR   ) 



END_TEST_TABLE()
