/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

class TestAddWeighted_8u32f_C1IR : public SrcSDstRoiWeighted< U8, 1, F32, 1 >
{
public:
    TestAddWeighted_8u32f_C1IR( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiWeighted< U8, 1, F32, 1 >( parent, "fwiAddWeighted_8u32f_C1IR", fwiAddWeighted_8u32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 MAX ", "1 2 3 4 5 6" ,
            "9 2 23 -4 -7 1002", 4, 0.0001f ); 
        RunTest( "5 6 16 100 1 5 ", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 -47.335 -377.78 23.775 13.33 ", -4, 0.0001f ); 
    }
};

class TestAddWeighted_8s32f_C1IR  : public SrcSDstRoiWeighted< S8, 1, F32, 1 >
{
public:
    TestAddWeighted_8s32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiWeighted< S8, 1, F32, 1 >( parent, "fwiAddWeighted_8s32f_C1IR", fwiAddWeighted_8s32f_C1IR )
    {}

    virtual void RunAll()
    {
          RunTest( "3 2 8 2 2.2 MAX ", "1 2 3 4 5 6" ,
            "9 2 23 -4 -7 490", 4, 0.0001f ); 
        RunTest( "5 6 16 100 1 5 ", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 -47.335 -377.78 23.775 13.33 ", -4, 0.0001f );
        
    }
};

class TestAddWeighted_16u32f_C1IR  : public SrcSDstRoiWeighted< U16, 1, F32, 1 >
{
public:
    TestAddWeighted_16u32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiWeighted< U16, 1, F32, 1 >( parent, "fwiAddWeighted_16u32f_C1IR", fwiAddWeighted_16u32f_C1IR )
    {}

    virtual void RunAll()
    {
      RunTest( "3 2 8 2 2.2 MAX ", "1 2 3 4 5 6" ,
            "9 2 23 -4 -7 262122", 4, 0.0001f ); 
      RunTest( "5 6 16 100 1 5 ", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 -47.335 -377.78 23.775 13.33 ", -4, 0.0001f );
   
    }
};

class TestAddWeighted_32f_C1IR  : public SrcSDstRoiWeighted< F32, 1, F32, 1 >
{
public:
    TestAddWeighted_32f_C1IR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiWeighted< F32, 1, F32, 1 >( parent, "fwiAddWeighted_32f_C1IR", fwiAddWeighted_32f_C1IR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2.2 MAX ", "1 2 3 4 5 0" ,
            "9 2 23 -4 -6.2 INF", 4, 0.0001f ); 
      RunTest( "5 6 16 100 1 5 ", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 -47.335 -377.78 23.775 13.33 ", -4, 0.0001f );
        
    }
};

class TestAddWeighted_8u32f_C1IMR : public SrcSDstRoiMaskWeighted< U8, 1, F32, 1 >
{
public:
    TestAddWeighted_8u32f_C1IMR( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMaskWeighted< U8, 1, F32, 1 >( parent, "fwiAddWeighted_8u32f_C1IMR", fwiAddWeighted_8u32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3 2 8 2 2.2 MAX ", "1 0 1 0 1 0", "1 2 3 4 5 6" ,
            "9 2 23 4 -7 6", 4, 0.0001f ); 
        RunTest( "5 6 16 100 1 5 ", "1 2 0 1 0 0", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 3.333 -377.78 5.555 6.666 ", -4, 0.0001f ); 
        
    }
};

class TestAddWeighted_8s32f_C1IMR  : public SrcSDstRoiMaskWeighted< S8, 1, F32, 1 >
{
public:
    TestAddWeighted_8s32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMaskWeighted< S8, 1, F32, 1 >( parent, "fwiAddWeighted_8s32f_C1IMR", fwiAddWeighted_8s32f_C1IMR )
    {}

    virtual void RunAll()
    {
        RunTest( "3 2 8 2 2.2 MAX ", " 0 1 2 0 3 5", "1 2 3 4 5 6" ,
            "1 2 23 4 -7 490", 4, 0.0001f ); 
        RunTest( "5 6 16 100 1 5 ", "1 3 5 0 0 2", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 -12.89 -47.335 4.444 5.555 13.33 ", -4, 0.0001f );
        
    }
};
class TestAddWeighted_16u32f_C1IMR  : public SrcSDstRoiMaskWeighted< U16, 1, F32, 1 >
{
public:
    TestAddWeighted_16u32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMaskWeighted< U16, 1, F32, 1 >( parent, "fwiAddWeighted_16u32f_C1IMR", fwiAddWeighted_16u32f_C1IMR )
    {}

    virtual void RunAll()
    {
       RunTest( "3 2 8 2 2.2 MAX ", " 0 0 0 2 4 6", "1 2 3 4 5 6" ,
            "1 2 3 -4 -7 262122", 4, 0.0001f ); 
      RunTest( "5 6 16 100 1 5 "," 0 2 4 6 0 5", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "1.111 -12.89 -47.335 -377.78 5.555 13.33 ", -4, 0.0001f );
    }
};

class TestAddWeighted_32f_C1IMR  : public SrcSDstRoiMaskWeighted< F32, 1, F32, 1 >
{
public:
    TestAddWeighted_32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMaskWeighted< F32, 1, F32, 1 >( parent, "fwiAddWeighted_32f_C1IMR", fwiAddWeighted_32f_C1IMR )
    {}

    virtual void RunAll()
    {
      RunTest( "3 2 8 2 2.2 MAX ", "1 0 5 6 8 1", "1 2 3 4 5 0" ,
            "9 2 23 -4 -6.2 INF", 4, 0.0001f ); 
      RunTest( "5 6 16 100 1 5 ","1 0 1", "1.111 2.222 3.333 4.444 5.555 6.666" ,
            "-14.445 2.222 -47.335 -377.78 5.555 13.33 ", -4, 0.0001f );
    }
};



DEFINE_TEST_TABLE( AddWeightedTestCatalog )

TEST_ENTRY( TestAddWeighted_8u32f_C1IR )
TEST_ENTRY( TestAddWeighted_8s32f_C1IR )
TEST_ENTRY( TestAddWeighted_16u32f_C1IR)
TEST_ENTRY( TestAddWeighted_32f_C1IR)

TEST_ENTRY( TestAddWeighted_8u32f_C1IMR )
TEST_ENTRY( TestAddWeighted_8s32f_C1IMR ) 
TEST_ENTRY( TestAddWeighted_16u32f_C1IMR) 
TEST_ENTRY( TestAddWeighted_32f_C1IMR   ) 

END_TEST_TABLE()
