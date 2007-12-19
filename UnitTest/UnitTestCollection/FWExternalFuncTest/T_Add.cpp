/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

// In place functions

class TestAdd_8u_C1IRSfs : public SrcSDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestAdd_8u_C1IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< U8, 1, U8, 1 >( parent, "fwiAdd_8u_C1IRSfs", fwiAdd_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 MAX", "6, 8, 10, 255", 0 );
        RunTest( "1 2 3 MAX", "5 6 7 1", "3, 4, 5, 128", 1 );
    }
};

class TestAdd_8u_C3IRSfs : public SrcSDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestAdd_8u_C3IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< U8, 3, U8, 3 >( parent, "fwiAdd_8u_C3IRSfs", fwiAdd_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 5 MAX", "2 4 8 6 10 MAX", "2 3 6 5 8 MAX", 1 );
    }
};

class TestAdd_8u_C4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAdd_8u_C4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAdd_8u_C4IRSfs", fwiAdd_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 4 7", 0 );
        RunTest( "2 4 8 6 10 5 MAX MAX", "3 6 11 10 15 11 2 MAX", "2 5 10 8 12 8 128 MAX", 1 );
    }
};

class TestAdd_8u_AC4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAdd_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAdd_8u_AC4IRSfs", fwiAdd_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 10 25 16 4 4", 0 );
      RunTest( "2 4 8 6 10 5 2 MAX", "3 6 11 10 15 11 2 MAX", "2 5 10 10 12 8 2 MAX", 1 );
    }
};

class TestAdd_16s_C1IRSfs : public SrcSDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestAdd_16s_C1IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< S16, 1, S16, 1 >( parent, "fwiAdd_16s_C1IRSfs", fwiAdd_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6 8 10 12", 0 );
        RunTest( "1 2 MAX MAX", "5 6 7 MAX", "3 4 16387 32767", 1 );
    }
};

class TestAdd_16s_C3IRSfs : public SrcSDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestAdd_16s_C3IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< S16, 3, S16, 3 >( parent, "fwiAdd_16s_C3IRSfs", fwiAdd_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 MAX MAX", "2 4 8 6 10 MAX", "2 3 6 5 16388 32767", 1 );
    }
};

class TestAdd_16s_C4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAdd_16s_C4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAdd_16s_C4IRSfs", fwiAdd_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 4 7", 0 );
        RunTest( "2 4 8 6 10 5 MAX MAX", "3 6 11 10 15 11 2 MAX", "2 5 10 8 12 8 16384 32767", 1 );
    }
};

class TestAdd_16s_AC4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAdd_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAdd_16s_AC4IRSfs", fwiAdd_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 10 25 16 4 4", 0 );
      RunTest( "2 4 8 6 10 5 MAX MAX", "3 6 11 10 15 11 2 MAX", "2 5 10 10 12 8 16384 32767", 1 );
    }
};

class TestAdd_16sc_C1IRSfs : public SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestAdd_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiAdd_16sc_C1IRSfs", fwiAdd_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
    }
};

class TestAdd_16sc_C3IRSfs : public SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestAdd_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiAdd_16sc_C3IRSfs", fwiAdd_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
    }
};


class TestAdd_16sc_AC4IRSfs : public SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestAdd_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiAdd_16sc_AC4IRSfs", fwiAdd_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 2 4", 0 );
    }
};

class TestAdd_32sc_C1IRSfs : public SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestAdd_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiAdd_32sc_C1IRSfs", fwiAdd_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
    }
};

class TestAdd_32sc_C3IRSfs : public SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestAdd_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiAdd_32sc_C3IRSfs", fwiAdd_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
    }
};


class TestAdd_32sc_AC4IRSfs : public SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestAdd_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiAdd_32sc_AC4IRSfs", fwiAdd_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 2 4", 0 );
    }
};

// Not in place functions

class TestAdd_8u_C1RSfs : public SrcSrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestAdd_8u_C1RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiAdd_8u_C1RSfs", fwiAdd_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
        RunTest( "1 2 3 4", "5 6 7 8", "3, 4, 5, 6", 1 ); //Nearest to the even rounding
    }
};

class TestAdd_8u_C3RSfs : public SrcSrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestAdd_8u_C3RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiAdd_8u_C3RSfs", fwiAdd_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "2 3 6 5 8 6", 1 ); //Nearest to the even rounding
    }
};

class TestAdd_8u_C4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAdd_8u_C4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAdd_8u_C4RSfs", fwiAdd_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 4 7", 0 );
    }
};

class TestAdd_8u_AC4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAdd_8u_AC4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAdd_8u_AC4RSfs", fwiAdd_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 1 2 3 4", "5 6 7 8 5 6 7 8", "6 8 10 0 6 8 10 0", 0 );
    
    }
};

class TestAdd_16s_C1RSfs : public SrcSrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestAdd_16s_C1RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiAdd_16s_C1RSfs", fwiAdd_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
        RunTest( "1 2 3 4", "5 6 7 8", "3, 4, 5, 6", 1 );
    }
};

class TestAdd_16s_C3RSfs : public SrcSrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestAdd_16s_C3RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiAdd_16s_C3RSfs", fwiAdd_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "2 3 6 5 8 6", 1 );
    }
};

class TestAdd_16s_C4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAdd_16s_C4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAdd_16s_C4RSfs", fwiAdd_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
       RunTest( "2 4 8 6 10 5 2 3", "3 6 11 10 15 11 2 4", "5 10 19 16 25 16 4 7", 0 );
    }
};

class TestAdd_16s_AC4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAdd_16s_AC4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAdd_16s_AC4RSfs", fwiAdd_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "1 2 3 4 1 2 3 4", "5 6 7 8 5 6 7 8", "6 8 10 0 6 8 10 0", 0 );
    }
};

class TestAdd_16sc_C1RSfs : public SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestAdd_16sc_C1RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiAdd_16sc_C1RSfs", fwiAdd_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
        RunTest( "1 2 3 4", "5 6 7 8", "3, 4, 5, 6", 1 );
    }
};

class TestAdd_16sc_C3RSfs : public SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestAdd_16sc_C3RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiAdd_16sc_C3RSfs", fwiAdd_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "2 3 6 5 8 6", 1 );
    }
};

class TestAdd_16sc_AC4RSfs : public SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestAdd_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiAdd_16sc_AC4RSfs", fwiAdd_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "1 2 3 4 1 2 3 4", "5 6 7 8 5 6 7 8", "6 8 10 12 6 8 0 0 ", 0 );
    }
};

class TestAdd_32sc_C1RSfs : public SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestAdd_32sc_C1RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiAdd_32sc_C1RSfs", fwiAdd_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 6 7 8", "6, 8, 10, 12", 0 );
        RunTest( "1 2 3 4", "5 6 7 8", "3, 4, 5, 6", 1 );
    }
};

class TestAdd_32sc_C3RSfs : public SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestAdd_32sc_C3RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiAdd_32sc_C3RSfs", fwiAdd_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "3 6 11 10 15 11", 0 );
        RunTest( "1 2 3 4 5 6", "2 4 8 6 10 5", "2 3 6 5 8 6", 1 );
    }
};

class TestAdd_32sc_AC4RSfs : public SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestAdd_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiAdd_32sc_AC4RSfs", fwiAdd_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
      RunTest( "1 2 3 4 1 2 3 4", "5 6 7 8 5 6 7 8", "6 8 10 12 6 8 0 0", 0 );
    }
};


//Floating point not in place funtions

class TestAdd_32f_C1R : public SrcSrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestAdd_32f_C1R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< F32, 1, F32, 1 >( parent, "fwiAdd_32f_C1R", fwiAdd_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "8.3279 5.0288 4.1971 6.9399 3.7510",
            "11.46949 7.6823 13.1764 9.3245 6.3943", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "12.3 106.5 19.85678 MAX", (F32) 0.0001 );
    }
};

class TestAdd_32f_C3R : public SrcSrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestAdd_32f_C3R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< F32, 3, F32, 3 >( parent, "fwiAdd_32f_C3R", fwiAdd_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.46949 7.6823 13.1764 9.3245 6.3943 0", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001", (F32) 0.0001 );
    }
};

class TestAdd_32f_C4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestAdd_32f_C4R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiAdd_32f_C4R", fwiAdd_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 0 3.2135 8.2489",
            "11.46949 7.6823 13.1764 9.3245 6.3943 0 5.5602 14.038", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001 5.55 13.62", (F32) 0.0001 );
    }
};

class TestAdd_32f_AC4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestAdd_32f_AC4R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiAdd_32f_AC4R", fwiAdd_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 0 3.2135 8.2489",
            "11.46949 7.6823 13.1764 0.0 6.3943 0 5.5602 0.0", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.3 106.5 19.85678 0.0 3.001 2.3001 5.55 0.0", (F32) 0.0001 );
    }
};

//Floating point in place funtions

class TestAdd_32f_C1IR : public SrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestAdd_32f_C1IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiAdd_32f_C1IR", fwiAdd_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "8.3279 5.0288 4.1971 6.9399 3.7510",
            "11.46949 7.6823 13.1764 9.3245 6.3943", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "12.3 106.5 19.85678 MAX", (F32) 0.0001 );
    }
};

class TestAdd_32f_C3IR : public SrcSDstRoi< F32, 3, F32, 3 >
{
public:
    TestAdd_32f_C3IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 3, F32, 3 >( parent, "fwiAdd_32f_C3IR", fwiAdd_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.46949 7.6823 13.1764 9.3245 6.3943 0", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001", (F32) 0.0001 );
    }
};

class TestAdd_32f_C4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestAdd_32f_C4IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiAdd_32f_C4IR", fwiAdd_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 0 3.2135 8.2489",
            "11.46949 7.6823 13.1764 9.3245 6.3943 0 5.5602 14.038", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001 5.55 13.62", (F32) 0.0001 );
    }
};

class TestAdd_32f_AC4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestAdd_32f_AC4IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiAdd_32f_AC4IR", fwiAdd_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 0 3.2135 8.2489",
            "11.46949 7.6823 13.1764 6.9399 6.3943 0 5.5602 8.2489", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.3 106.5 19.85678 2.0 3.001 2.3001 5.55 7.89", (F32) 0.0001 );
    }
};




// Floating point Complex in place functions

class TestAdd_32fc_C1IR : public SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestAdd_32fc_C1IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiAdd_32fc_C1IR", fwiAdd_32fc_C1IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
            "11.46949 7.6823 13.1764 9.3245 6.3943 11.4902", errorMargin );
        RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "12.3 106.5 19.85678 MAX", errorMargin );
    }
};

class TestAdd_32fc_C3IR : public SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestAdd_32fc_C3IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiAdd_32fc_C3IR", fwiAdd_32fc_C3IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
            "11.46949 7.6823 13.1764 9.3245 6.3943 11.4902", errorMargin ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001 ", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001", errorMargin );
    }
};

class TestAdd_32fc_AC4IR : public SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestAdd_32fc_AC4IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiAdd_32fc_AC4IR", fwiAdd_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 1.2 3.2135 8.2489",
            "11.46949 7.6823 13.1764 9.3245 6.3943 1.2 3.2135 8.2489", errorMargin ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 0.9876 100.78 1.5 0 0.12345", "7.2 100.3 3.4 0.2567 3.7891 3.456 5.32 9.9", 
            "12.3 106.5 19.85678 1.2443 104.5691 4.956 5.32 9.9", errorMargin );
    }
};

// Floating Point Complex Not in Place

class TestAdd_32fc_C1R : public SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestAdd_32fc_C1R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiAdd_32fc_C1R", fwiAdd_32fc_C1R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
            "11.46949 7.6823 13.1764 9.3245 6.3943 11.4902", errorMargin );
        RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "12.3 106.5 19.85678 MAX", errorMargin );
    }
};

class TestAdd_32fc_C3R : public SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestAdd_32fc_C3R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiAdd_32fc_C3R", fwiAdd_32fc_C3R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
            "11.46949 7.6823 13.1764 9.3245 6.3943 11.4902", errorMargin ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "7.2 100.3 3.4 2.0 2.001 2.3001 ", 
            "12.3 106.5 19.85678 102.78 3.001 2.3001", errorMargin );
    }
};

class TestAdd_32fc_AC4R : public SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestAdd_32fc_AC4R( UnitTestCatalogBase & parent ) : SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiAdd_32fc_AC4R", fwiAdd_32fc_AC4R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 1.2 3.2135 8.2489",
            "11.46949 7.6823 13.1764 9.3245 6.3943 1.2 0 0", errorMargin ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 0.9876 100.78 1.5 0 0.12345", "7.2 100.3 3.4 0.2567 3.7891 3.456 5.32 9.9", 
            "12.3 106.5 19.85678 1.2443 104.5691 4.956 0 0", errorMargin );
    }
};

class TestAdd_8u32f_C1IR : public SrcSDstRoi< U8, 1, F32, 1 >
{
public:
    TestAdd_8u32f_C1IR( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 1, F32, 1 >( parent, "fwiAdd_8u32f_C1IR", fwiAdd_8u32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 7.0288 12.1971 8.9399 5.7510 0 ", 0.0001f ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 106.3 19.4 102.0 3.001 2.3001 5.10 12.89", 0.0001f );
        
    }
};

class TestAdd_8s32f_C1IR  : public SrcSDstRoi< S8, 1, F32, 1 >
{
public:
    TestAdd_8s32f_C1IR ( UnitTestCatalogBase & parent ) : SrcSDstRoi< S8, 1, F32, 1 >( parent, "fwiAdd_8s32f_C1IR", fwiAdd_8s32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 7.0288 12.1971 8.9399 5.7510 0 ", 0.0001f ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 106.3 19.4 102.0 3.001 2.3001 5.10 12.89", 0.0001f );
        
    }
};

class TestAdd_16u32f_C1IR  : public SrcSDstRoi< U16, 1, F32, 1 >
{
public:
    TestAdd_16u32f_C1IR ( UnitTestCatalogBase & parent ) : SrcSDstRoi< U16, 1, F32, 1 >( parent, "fwiAdd_16u32f_C1IR", fwiAdd_16u32f_C1IR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 7.0288 12.1971 8.9399 5.7510 0 ", 0.0001f ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 106.3 19.4 102.0 3.001 2.3001 5.10 12.89", 0.0001f );
        
    }
};

class TestAdd_8u32f_C1IMR : public SrcSDstRoiMask< U8, 1, F32, 1 >
{
public:
    TestAdd_8u32f_C1IMR( UnitTestCatalogBase & parent ) : SrcSDstRoiMask< U8, 1, F32, 1 >( parent, "fwiAdd_8u32f_C1IMR", fwiAdd_8u32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ","1 2 0 3 2 0", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 7.0288 4.1971 8.9399 5.7510 0 ", 0.0001f ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73","1 0 1 0 1 0 2 5", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 100.3 19.4 2.0 3.001 2.3001 5.10 12.89", 0.0001f );
        
    }
};

class TestAdd_8s32f_C1IMR  : public SrcSDstRoiMask< S8, 1, F32, 1 >
{
public:
    TestAdd_8s32f_C1IMR ( UnitTestCatalogBase & parent ) : SrcSDstRoiMask< S8, 1, F32, 1 >( parent, "fwiAdd_8s32f_C1IMR", fwiAdd_8s32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ","1 0 2 0 3 0", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 5.0288 12.1971 6.9399 5.7510 0 ", 0.0001f ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "1 5 6 0 0 1 2 4", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 106.3 19.4 2.0 2.001 2.3001 5.10 12.89", 0.0001f );
        
    }
};

class TestAdd_16u32f_C1IMR  : public SrcSDstRoiMask< U16, 1, F32, 1 >
{
public:
    TestAdd_16u32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< U16, 1, F32, 1 >( parent, "fwiAdd_16u32f_C1IMR", fwiAdd_16u32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ","1 0 0 2 4 5", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.3279 5.0288 4.1971 8.9399 5.7510 0 ", (F32) 0.0001 ); //Pi's digit
          RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73","2 3 0 1 4 5 0 0", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.2 106.3 3.4 102.0 3.001 2.3001 2.10 7.89", (F32) 0.0001 );
        
    }
};

class TestAdd_32f_C1IMR  : public SrcSDstRoiMask< F32, 1, F32, 1 >
{
public:
    TestAdd_32f_C1IMR ( UnitTestCatalogBase & parent ) 
        : SrcSDstRoiMask< F32, 1, F32, 1 >( parent, "fwiAdd_32f_C1IMR", fwiAdd_32f_C1IMR )
    {}

    virtual void RunAll()
    {
         RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 ","1 0 0 2 4 5", "8.3279 5.0288 4.1971 6.9399 3.7510 0",
            "11.46949 5.0288 4.1971 9.3245 6.3943 0 ", 0.0001f ); //Pi's digit
         RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73","2 3 0 1 4 5 0 0", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
            "12.3 106.5 3.4 102.78 3.001 2.3001 2.10 7.89", 0.0001f );
        
    }
};







DEFINE_TEST_TABLE( AddTestCatalog )
TEST_ENTRY( TestAdd_8u_C1IRSfs  )
TEST_ENTRY( TestAdd_8u_C3IRSfs  )
TEST_ENTRY( TestAdd_8u_C4IRSfs  )
TEST_ENTRY( TestAdd_8u_AC4IRSfs )
TEST_ENTRY( TestAdd_16s_C1IRSfs )
TEST_ENTRY( TestAdd_16s_C3IRSfs )
TEST_ENTRY( TestAdd_16s_C4IRSfs )
TEST_ENTRY( TestAdd_16s_AC4IRSfs)

TEST_ENTRY( TestAdd_16sc_C1IRSfs )
TEST_ENTRY( TestAdd_16sc_C3IRSfs )
TEST_ENTRY( TestAdd_16sc_AC4IRSfs) 
TEST_ENTRY( TestAdd_32sc_C1IRSfs )
TEST_ENTRY( TestAdd_32sc_C3IRSfs )
TEST_ENTRY( TestAdd_32sc_AC4IRSfs)  

TEST_ENTRY( TestAdd_8u_C1RSfs   )
TEST_ENTRY( TestAdd_8u_C3RSfs   )
TEST_ENTRY( TestAdd_8u_C4RSfs   )
TEST_ENTRY( TestAdd_8u_AC4RSfs  )
TEST_ENTRY( TestAdd_16s_C1RSfs  )
TEST_ENTRY( TestAdd_16s_C3RSfs  )
TEST_ENTRY( TestAdd_16s_C4RSfs  )
TEST_ENTRY( TestAdd_16s_AC4RSfs )
TEST_ENTRY( TestAdd_16sc_C1RSfs )
TEST_ENTRY( TestAdd_16sc_C3RSfs )
TEST_ENTRY( TestAdd_16sc_AC4RSfs) 
TEST_ENTRY( TestAdd_32sc_C1RSfs )
TEST_ENTRY( TestAdd_32sc_C3RSfs )
TEST_ENTRY( TestAdd_32sc_AC4RSfs) 

TEST_ENTRY( TestAdd_32f_C1R   )
TEST_ENTRY( TestAdd_32f_C3R   )
TEST_ENTRY( TestAdd_32f_C4R   )
TEST_ENTRY( TestAdd_32f_AC4R  )
TEST_ENTRY( TestAdd_32f_C1IR  )
TEST_ENTRY( TestAdd_32f_C3IR  )
TEST_ENTRY( TestAdd_32f_C4IR  )
TEST_ENTRY( TestAdd_32f_AC4IR )

TEST_ENTRY( TestAdd_32fc_AC4IR)
TEST_ENTRY( TestAdd_32fc_C3IR )
TEST_ENTRY( TestAdd_32fc_C1IR )
TEST_ENTRY( TestAdd_32fc_C1R  )
TEST_ENTRY( TestAdd_32fc_C3R  )
TEST_ENTRY( TestAdd_32fc_AC4R )

TEST_ENTRY( TestAdd_8u32f_C1IR )  
TEST_ENTRY( TestAdd_8s32f_C1IR )
TEST_ENTRY( TestAdd_16u32f_C1IR)

TEST_ENTRY( TestAdd_8u32f_C1IMR ) 
TEST_ENTRY( TestAdd_8s32f_C1IMR ) 
TEST_ENTRY( TestAdd_16u32f_C1IMR) 
TEST_ENTRY( TestAdd_32f_C1IMR   ) 


END_TEST_TABLE()
