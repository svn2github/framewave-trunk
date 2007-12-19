/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer or complex data
class TestMul_8u_C1RSfs : public SrcSrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestMul_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiMul_8u_C1RSfs", fwiMul_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        //currently inconsistent rounding
        RunTest( "2 3", "7 100 3 MAX", "14 MAX 6 MAX", 0 );
        RunTest( "2 3", "7 100 3 MAX", "7 150 3 MAX", 1 );
    }
};

class TestMul_8u_C3RSfs : public SrcSrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestMul_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiMul_8u_C3RSfs", fwiMul_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 2 3 MAX", "5 0 23 MAX 0 1", "5 0 MAX MAX 0 MAX", 0 );
        RunTest( "1 0 MAX 2 3 5", "5 1 23 22 0 100", "2 0 MAX 22 0 250", 1 );
    }
};

class TestMul_8u_AC4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMul_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMul_8u_AC4RSfs", fwiMul_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5 0 MAX 0 MAX 0 MAX 0", 0 );
        RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 0 MAX 0 MAX 0 250 0", 1 );
    }
};

class TestMul_8u_C4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMul_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMul_8u_C4RSfs", fwiMul_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 11 2 3 MAX 11", "5 0 MAX 22 MAX 0   1 22", "5 0 MAX 242 MAX 0 MAX 242", 0 );
        RunTest( "1 0 MAX 11 2 3   5 11", "5 1 MAX 22 MAX 0 100 22", "2 0 MAX 121 MAX 0 250 121", 1 );
    }
};

class TestMul_16s_C1RSfs : public SrcSrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestMul_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiMul_16s_C1RSfs", fwiMul_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        //currently inconsistent rounding,
        RunTest( "2 -3", "7 100 3 2 MIN MIN", "14 -300 6 -6 MIN 32767 ", 0 );
        RunTest( "2 -3", "7 100 3 2 MIN MIN", " 7 -150 3 -3 MIN 32767 ", 1 );
    }
};

class TestMul_16s_C3RSfs : public SrcSrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestMul_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiMul_16s_C3RSfs", fwiMul_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX -2 -3 5", "5 0 2 4 MIN   1", "5 0 MAX -8 MAX 5", 0 );
        RunTest( "1 0 MAX -2 -3 5", "5 1 2 4 MIN 100", "2 0 MAX -4 MAX 250", 1 );
    }
};

class TestMul_16s_AC4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMul_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMul_16s_AC4RSfs", fwiMul_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 11 -2 -3 5 22", "5 0 2 11 4 MIN   1 22", "5 0 MAX 0 -8 MAX 5 0", 0 );
        RunTest( "1 0 MAX 11 -2 -3 5 22", "5 1 2 11 4 MIN 100 22", "2 0 MAX 0 -4 MAX 250 0", 1 );
    }
};

class TestMul_16s_C4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMul_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMul_16s_C4RSfs", fwiMul_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 11 -2 -3 5 22", "5 0 2 11 4 MIN   1 22", "5 0 MAX 121 -8 MAX 5 484 ", 0 );
        RunTest( "1 0 MAX 11 -2 -3 5 22", "5 1 2 11 4 MIN 100 22", "2 0 MAX 60 -4 MAX 250 242", 1 );
    }
};

class TestMul_16sc_C1RSfs : public SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestMul_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiMul_16sc_C1RSfs", fwiMul_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 5 6 9 ","314 179 21 1 39 0 ", 0 );
        RunTest( "2 -3", "7 100 3 5 6 9", "157 90 10 0 20 0  ", 1 );
    }
};

class TestMul_16sc_C3RSfs : public SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestMul_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiMul_16sc_C3RSfs", fwiMul_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "314 179 32767 32761 0 0", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "157 90 32767 16380 0 0", 1 );
    }
};

class TestMul_16sc_AC4RSfs : public SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestMul_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiMul_16sc_AC4RSfs", fwiMul_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 0 0 111 111", "7 100 3 MAX 0 0 222 222", 
                 "314 179 32767 32761 0 0 0 0", 0 );
        RunTest( "2 -3 1 -2 0 0 111 111", "7 100 3 MAX 0 0 222 222", 
                 "157 90 32767 16380 0 0 0 0", 1 );
    }
};

class TestMul_32sc_C1RSfs : public SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestMul_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiMul_32sc_C1RSfs", fwiMul_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX 0 0", 
                 "314 179 2147483647 2147483647 0 0 ", 0 );
        RunTest( "2 -3", "7 100 3 MAX 0 0", 
                 "157 90 2147483647 2147483642 0 0", 1 );
    }
};

class TestMul_32sc_C3RSfs : public SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestMul_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiMul_32sc_C3RSfs", fwiMul_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", 
                 "314 179 2147483647 2147483641 0 0 ", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", 
                 "157 90 2147483647 1073741820 0 0", 1 );
    }
};

class TestMul_32sc_AC4RSfs : public SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestMul_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiMul_32sc_AC4RSfs", fwiMul_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
            "314 179 2147483647 2147483641 0 0 0 0", 0 );
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
            "157 90 2147483647 1073741820 0 0 0 0", 1 );
    }
};

//Case 2: Not-in-place operation on floating-point or complex data.
class TestMul_32f_C1R : public SrcSrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestMul_32f_C1R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< F32, 1, F32, 1 >( parent, "fwiMul_32f_C1R", fwiMul_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 0.12345", "14 -300 6 -0.37035", 0.0001f );
        RunTest( "3 1 -5 -2", "2 2.301 5.556 4", "6 2.301 -27.78 -8 ", 1.0e+33f );
    }
};

class TestMul_32f_C3R : public SrcSrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestMul_32f_C3R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< F32, 3, F32, 3 >( parent, "fwiMul_32f_C3R", fwiMul_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
            "14 -300 0.629595 6.28318 -7.9605 45.79443 ", 0.0001f );
        RunTest( "3 1 -0.5 -0.2 0 0", "MAX MIN MAX MIN MAX MIN", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 0.680564e+38 0 0", 1.0e+33f );
    }
};

class TestMul_32f_AC4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestMul_32f_AC4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiMul_32f_AC4R", fwiMul_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0", "7 100 0.12345 0 3.14159 2.6535 8.9793 0", 
            "14 -300 0.629595 0 6.28318 -7.9605 45.79443 0", 0.0001f );
        RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX INF MIN MAX MIN INF", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 0 0.680564e+38 0 0 0", 1.0e+33f );
    }
};

class TestMul_32f_C4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestMul_32f_C4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiMul_32f_C4R", fwiMul_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0.1", "7 100 0.12345 0 3.14159 2.6535 8.9793 10", 
            "14 -300 0.629595 0 6.28318 -7.9605 45.79443 1", 0.0001f );
        RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX 1 MIN MAX MIN 2", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 INF 0.680564e+38 0 0 INF", 1.0e+33f );
    }
};

class TestMul_32fc_C1R : public SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestMul_32fc_C1R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiMul_32fc_C1R", fwiMul_32fc_C1R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "314 179 18 -1", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 -5 -2", "MAX MIN 0 0", "6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

class TestMul_32fc_C3R : public SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestMul_32fc_C3R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiMul_32fc_C3R", fwiMul_32fc_C3R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "314 179 18 -1 7 26", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 3 1 3 4", "MAX MIN MAX MIN 0 0", 
            "6.805638e+38 -6.805638e+38 6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

class TestMul_32fc_AC4R : public SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestMul_32fc_AC4R( UnitTestCatalogBase & parent ) : 
      SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiMul_32fc_AC4R", fwiMul_32fc_AC4R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
            "314 179 18 -1 7 26 0 0", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 -3 -1 3 4 111 111", "MAX MIN MAX MIN 0 0 222 222", 
            "6.805638e+38 -6.805638e+38 -6.805638e+38 6.805638e+38 0 0 222 222", errorMargin );
    }
};

//Case 3: In-place operation on integer or complex data.
class TestMul_8u_C1IRSfs : public SrcSDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestMul_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< U8, 1, U8, 1 >( parent, "fwiMul_8u_C1IRSfs", fwiMul_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        //currently inconsistent rounding
        RunTest( "2 3", "7 100 3 MAX", "14 MAX 6 MAX", 0 );
        RunTest( "2 3", "7 100 3 MAX", "7 150 3 MAX", 1 );
    }
};

class TestMul_8u_C3IRSfs : public SrcSDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestMul_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< U8, 3, U8, 3 >( parent, "fwiMul_8u_C3IRSfs", fwiMul_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 2 3 MAX", "5 0 MAX MAX 0 1", "5 0 MAX MAX 0 MAX", 0 );
        RunTest( "1 0 MAX 2 3 5", "5 1 MAX MAX 0 100", "2 0 MAX MAX 0 250", 1 );
    }
};


class TestMul_8u_AC4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMul_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMul_8u_AC4IRSfs", fwiMul_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5 0 MAX 222 MAX 0 MAX 222", 0 );
        RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 0 MAX 222 MAX 0 250 222", 1 );
    }
};

class TestMul_8u_C4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMul_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMul_8u_C4IRSfs", fwiMul_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 11 2 3 MAX 11", "5 0 MAX 22 MAX 0   1 22", "5 0 MAX 242 MAX 0 MAX 242", 0 );
        RunTest( "1 0 MAX 11 2 3   5 11", "5 1 MAX 22 MAX 0 100 22", "2 0 MAX 121 MAX 0 250 121", 1 );
    }
};

class TestMul_16s_C1IRSfs : public SrcSDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestMul_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< S16, 1, S16, 1 >( parent, "fwiMul_16s_C1IRSfs", fwiMul_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        //currently inconsistent rounding,
        RunTest( "2 -3", "7 100 3 MAX MIN 5", "14 -300 6 -32768 MIN -15", 0 );
        RunTest( "2 -3", "7 100 3 MAX MIN 5", "7 -150 3 -32768 MIN -8", 1 );
    }
};

class TestMul_16s_C3IRSfs : public SrcSDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestMul_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< S16, 3, S16, 3 >( parent, "fwiMul_16s_C3IRSfs", fwiMul_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 1 -3 5", "5 0 MAX MAX 0   1", "5 0 MAX MAX 0   5", 0 );
        RunTest( "1 0 MAX 1 -3 5", "5 1 MAX MAX 0 100", "2 0 MAX 16384 0 250", 1 );
    }
};


class TestMul_16s_AC4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMul_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMul_16s_AC4IRSfs", fwiMul_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 111 2 -3 MIN 111", "5 0 MAX 222 23 0   1 222", "5 0 MAX 222 46 0 MIN 222 ", 0 );
        RunTest( "1 0 MAX 111 2 -3   5 111", "5 1 MAX 222 23 0 100 222", "2 0 MAX 222 23 0 250 222 ", 1 );
    }
};

class TestMul_16s_C4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMul_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMul_16s_C4IRSfs", fwiMul_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        //Show rounding 255/2=128, currently inconsistent rounding
        RunTest( "1 0 MAX 11 2 -3 MIN 11", "5 0 MAX 22 23 0   1 22", "5 0 MAX 242 46 0 MIN 242", 0 );
        RunTest( "1 0 MAX 11 2 -3   5 11", "5 1 MAX 22 23 0 100 22", "2 0 MAX 121 23 0 250 121", 1 );
    }
};

class TestMul_16sc_C1IRSfs : public SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestMul_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiMul_16sc_C1IRSfs", fwiMul_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 5 6 9", "314 179 21 1 39 0 ", 0 );
        RunTest( "2 -3", "7 100 3 30 0 0", "157 90 48 26  0 0 ", 1 );
    }
};

class TestMul_16sc_C3IRSfs : public SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestMul_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiMul_16sc_C3IRSfs", fwiMul_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "314 179 32767 32761 0 0", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "157 90 32767 16380 0 0", 1 );
    }
};

class TestMul_16sc_AC4IRSfs : public SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestMul_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiMul_16sc_AC4IRSfs", fwiMul_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 0 0 111 111", "7 100 3 MAX 0 0 222 222", 
            "314 179 32767 32761 0 0 222 222", 0 );
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
            "157 90 32767 16380 0 0 222 222", 1 );
    }
};

class TestMul_32sc_C1IRSfs : public SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestMul_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiMul_32sc_C1IRSfs", fwiMul_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX 0 0", 
            "314 179 2147483647 2147483647 0 0 ", 0 );
        RunTest( "2 -3", "7 100 3 MAX 0 0", 
            "157 90 2147483647 2147483642 0 0", 1 );
    }
};

class TestMul_32sc_C3IRSfs : public SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestMul_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiMul_32sc_C3IRSfs", fwiMul_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", 
                 "314 179 2147483647 2147483641 0 0 ", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", 
                 "157 90 2147483647 1073741820 0 0", 1 );
    }
};

class TestMul_32sc_AC4IRSfs : public SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestMul_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiMul_32sc_AC4IRSfs", fwiMul_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
                 "314 179 2147483647 2147483641 0 0 222 222", 0 );
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
                 "157 90 2147483647 1073741820 0 0 222 222", 1 );
    }
};

//Case 4: In-place operation on floating-point or complex data.
class TestMul_32f_C1IR : public SrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestMul_32f_C1IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiMul_32f_C1IR", fwiMul_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 0.12345", "14 -300 6 -0.37035", 0.0001f );
        RunTest( "3 1 -5 -2", "2 2.301 5.556 4", "6 2.301 -27.78 -8 ", 1.0e+33f );
    }
};

class TestMul_32f_C3IR : public SrcSDstRoi< F32, 3, F32, 3 >
{
public:
    TestMul_32f_C3IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< F32, 3, F32, 3 >( parent, "fwiMul_32f_C3IR", fwiMul_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
            "14 -300 0.629595 6.28318 -7.9605 45.79443", 0.0001f );
        RunTest( "3 1 -0.5 -0.2 0 0", "MAX MIN MAX MIN MAX MIN", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 0.680564e+38 0 0", 1.0e+33f );
    }
};

class TestMul_32f_AC4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestMul_32f_AC4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiMul_32f_AC4IR", fwiMul_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 INF", "7 100 0.12345 INF 3.14159 2.6535 8.9793 INF", 
            "14 -300 0.629595 INF 6.28318 -7.9605 45.79443 INF", 0.0001f );
        RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX INF MIN MAX MIN INF", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 INF 0.680564e+38 0 0 INF", 1.0e+33f );
    }
};

class TestMul_32f_C4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestMul_32f_C4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiMul_32f_C4IR", fwiMul_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0.1", "7 100 0.12345 0 3.14159 2.6535 8.9793 10", 
            "14 -300 0.629595 0 6.28318 -7.9605 45.79443 1", 0.0001f );
        RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX 1 MIN MAX MIN 2", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 INF 0.680564e+38 0 0 INF", 1.0e+33f );
    }
};

class TestMul_32fc_C1IR : public SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestMul_32fc_C1IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiMul_32fc_C1IR", fwiMul_32fc_C1IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "314 179 18 -1", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 -5 -2", "MAX MIN 0 0", "6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

class TestMul_32fc_C3IR : public SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestMul_32fc_C3IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiMul_32fc_C3IR", fwiMul_32fc_C3IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "314 179 18 -1 7 26", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 3 1 3 4", "MAX MIN MAX MIN 0 0", 
            "6.805638e+38 -6.805638e+38 6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

class TestMul_32fc_AC4IR : public SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestMul_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
      SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiMul_32fc_AC4IR", fwiMul_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
            "314 179 18 -1 7 26 INF INF", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        //inconsistent results
        RunTest( "3 1 3 1 3 4 INF INF", "MAX MIN MAX MIN 0 0 INF INF", 
            "6.805638e+38 -6.805638e+38 6.805638e+38 -6.805638e+38 0 0 INF INF", errorMargin );
    }
};

DEFINE_TEST_TABLE( MulTestCatalog )
TEST_ENTRY( TestMul_8u_C1RSfs )   
TEST_ENTRY( TestMul_8u_C3RSfs )
TEST_ENTRY( TestMul_8u_AC4RSfs )     
TEST_ENTRY( TestMul_8u_C4RSfs )      
TEST_ENTRY( TestMul_16s_C1RSfs )
TEST_ENTRY( TestMul_16s_C3RSfs )
TEST_ENTRY( TestMul_16s_AC4RSfs )     
TEST_ENTRY( TestMul_16s_C4RSfs )      
TEST_ENTRY( TestMul_16sc_C1RSfs )
TEST_ENTRY( TestMul_16sc_C3RSfs )     
TEST_ENTRY( TestMul_16sc_AC4RSfs )
TEST_ENTRY( TestMul_32sc_C1RSfs )
TEST_ENTRY( TestMul_32sc_C3RSfs )
TEST_ENTRY( TestMul_32sc_AC4RSfs )
TEST_ENTRY( TestMul_32f_C1R )
TEST_ENTRY( TestMul_32f_C3R )
TEST_ENTRY( TestMul_32f_AC4R )
TEST_ENTRY( TestMul_32f_C4R )

TEST_ENTRY( TestMul_32fc_C1R )
TEST_ENTRY( TestMul_32fc_C3R )        
TEST_ENTRY( TestMul_32fc_AC4R )

TEST_ENTRY( TestMul_8u_C1IRSfs )        
TEST_ENTRY( TestMul_8u_C3IRSfs )
TEST_ENTRY( TestMul_8u_AC4IRSfs )     
TEST_ENTRY( TestMul_8u_C4IRSfs )  
TEST_ENTRY( TestMul_16s_C1IRSfs )
TEST_ENTRY( TestMul_16s_C3IRSfs )
TEST_ENTRY( TestMul_16s_AC4IRSfs )    
TEST_ENTRY( TestMul_16s_C4IRSfs ) 
TEST_ENTRY( TestMul_16sc_C1IRSfs )
TEST_ENTRY( TestMul_16sc_C3IRSfs )
TEST_ENTRY( TestMul_16sc_AC4IRSfs )
TEST_ENTRY( TestMul_32sc_C1IRSfs )
TEST_ENTRY( TestMul_32sc_C3IRSfs )
TEST_ENTRY( TestMul_32sc_AC4IRSfs )
TEST_ENTRY( TestMul_32f_C1IR )
TEST_ENTRY( TestMul_32f_C3IR )
TEST_ENTRY( TestMul_32f_AC4IR )
TEST_ENTRY( TestMul_32f_C4IR )
TEST_ENTRY( TestMul_32fc_C1IR )
TEST_ENTRY( TestMul_32fc_C3IR )
TEST_ENTRY( TestMul_32fc_AC4IR )

END_TEST_TABLE()
