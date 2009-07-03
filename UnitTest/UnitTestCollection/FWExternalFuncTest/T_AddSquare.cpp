/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

class TestAddSquare_8u32f_C1IR : public SrcSDstRoi< U8, 1, F32, 1 >
{
public:
    TestAddSquare_8u32f_C1IR( UnitTestCatalogBase & parent ) 
        : SrcSDstRoi< U8, 1, F32, 1 >( parent, "fwiAddSquare_8u32f_C1IR", fwiAddSquare_8u32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 0 ", "1 2 3 4 5 6" ,
            "10 6 67 8 9 6", 0.0001f ); //Pi's digit
        RunTest( "5 6 16 100 1 5", "1.111 2.222 3.333 4.444 5.555 6.666", 
            "26.111 38.222 259.333 10004.444 6.555 31.666 ", 0.0001f );
        
    }
};

class TestAddSquare_8s32f_C1IR  : public SrcSDstRoi< S8, 1, F32, 1 >
{
public:
    TestAddSquare_8s32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoi< S8, 1, F32, 1 >( parent, "fwiAddSquare_8s32f_C1IR", fwiAddSquare_8s32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2 MAX ", "1.11 2.22 3.33 4.44 5.55 6.66",
            "10.11 6.22 67.33 8.44 9.55 16135.66", 0.0001f ); //Pi's digit
         RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001", 
                  "32.2 136.3 259.4 10002 3.001 2.3001 ", 0.0001f );
        
    }
};
class TestAddSquare_16u32f_C1IR  : public SrcSDstRoi< U16, 1, F32, 1 >
{
public:
    TestAddSquare_16u32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoi< U16, 1, F32, 1 >( parent, "fwiAddSquare_16u32f_C1IR", fwiAddSquare_16u32f_C1IR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2 MAX ", "1.11 2.22 3.33 4.44 5.55 6.66",
            "10.11 6.22 67.33 8.44 9.55 4294836231.66", 0.0001f ); //Pi's digit
       RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001", 
            "32.2 136.3 259.4 10002 3.001 2.3001 ", 0.0001f );
        
    }
};

class TestAddSquare_32f_C1IR  : public SrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestAddSquare_32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiAddSquare_32f_C1IR", fwiAddSquare_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.2323 2.001 8.23 2.1 2.2 0 ", "0.629595 6.28318 7.9605 45.79443 0.12345 MAX",
                 "11.07735829 10.287181 75.6934 50.20443 4.9635 MAX", 0.0001f ); //Pi's digit
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 -2.6535 8.9793", 
                 "11 109 26.13345 7.14159 6.3465 34.9893", 0.0001f );
        
    }
};

class TestAddSquare_8u32f_C1IMR : public SrcSDstRoiMask< U8, 1, F32, 1 >
{
public:
    TestAddSquare_8u32f_C1IMR( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< U8, 1, F32, 1 >( parent, "fwiAddSquare_8u32f_C1IMR", fwiAddSquare_8u32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 0 ", "1 0 1 0 1 0",
                  "1 2 3 4 5 6" , "10 2 67 4 9 6", 0.0001f ); //Pi's digit
        RunTest( "5 6 16 100 1 5", " 1 2 0 1 0 0", "1.111 2.222 3.333 4.444 5.555 6.666", 
            "26.111 38.222  3.333 10004.444 5.555 6.666 ", 0.0001f );
        
    }
};

class TestAddSquare_8s32f_C1IMR  : public SrcSDstRoiMask< S8, 1, F32, 1 >
{
public:
    TestAddSquare_8s32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< S8, 1, F32, 1 >( parent, "fwiAddSquare_8s32f_C1IMR", fwiAddSquare_8s32f_C1IMR )
    {}

    virtual void RunAll()
    {
        RunTest( "3 2 8 2 2 MAX ", "0 1 2 0 3 5", "1.11 2.22 3.33 4.44 5.55 6.66",
            "1.11 6.22 67.33 4.44 9.55 16135.66", 0.0001f ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", " 1 3 5 0 0 2", "7.2 100.3 3.4 2.0 2.001 2.3001", 
                  "32.2 136.3 259.4 2.0 2.001 2.3001 ", 0.0001f );
        
    }
};
class TestAddSquare_16u32f_C1IMR  : public SrcSDstRoiMask< U16, 1, F32, 1 >
{
public:
    TestAddSquare_16u32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< U16, 1, F32, 1 >( parent, "fwiAddSquare_16u32f_C1IMR", fwiAddSquare_16u32f_C1IMR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2 MAX ", " 0 0 0 2 4 6", "1.11 2.22 3.33 4.44 5.55 6.66",
            "1.11 2.22 3.33 8.44 9.55 4294836231.66", 0.0001f ); //Pi's digit
       RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "0 2 4 6 0 5", "7.2 100.3 3.4 2.0 2.001 2.3001", 
            "7.2 136.3 259.4 10002 2.001 2.3001 ", 0.0001f );
        
    }
};

class TestAddSquare_32f_C1IMR  : public SrcSDstRoiMask< F32, 1, F32, 1 >
{
public:
    TestAddSquare_32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< F32, 1, F32, 1 >( parent, "fwiAddSquare_32f_C1IMR", fwiAddSquare_32f_C1IMR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.2323 2.001 8.23 2.1 2.2 0 ", "1 0 5 6 8 0", "0.629595 6.28318 7.9605 45.79443 0.12345 MAX",
                 "11.07735829 6.28318 75.6934 50.20443 4.9635 MAX", 0.0001f ); //Pi's digit
        RunTest( "2 -3 5.1", " 1 0 1",  "7 100 0.12345 3.14159 -2.6535 8.9793", 
                 "11 100 26.13345 7.14159 -2.6535 34.9893", 0.0001f );
    }
};




DEFINE_TEST_TABLE( AddSquareTestCatalog )

TEST_ENTRY( TestAddSquare_8u32f_C1IR )  
TEST_ENTRY( TestAddSquare_8s32f_C1IR )
TEST_ENTRY( TestAddSquare_16u32f_C1IR)
TEST_ENTRY( TestAddSquare_32f_C1IR)

TEST_ENTRY( TestAddSquare_8u32f_C1IMR ) 
TEST_ENTRY( TestAddSquare_8s32f_C1IMR ) 
TEST_ENTRY( TestAddSquare_16u32f_C1IMR) 
TEST_ENTRY( TestAddSquare_32f_C1IMR   ) 



END_TEST_TABLE()
