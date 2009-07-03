/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel integer or complex data.
class TestMulC_8u_C1RSfs : public SrcValDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestMulC_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< U8, 1, U8, 1 >( parent, "fwiMulC_8u_C1RSfs", fwiMulC_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 22", "3", "21 MAX 9 66 ", 0 );
        RunTest( "7 100 3 22", "3", "10 150 4 33", 1 );
    }
};

class TestMulC_16s_C1RSfs : public SrcValDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestMulC_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< S16, 1, S16, 1 >( parent, "fwiMulC_16s_C1RSfs", fwiMulC_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX MIN -33", "-3", "-21 -300 -9 -32768 32767 99", 0 );
        RunTest( "7 100 3 MAX MIN -33", "-3", "-10 -150 -4 -32768 32767 50", 1 );
    }
};

class TestMulC_16sc_C1RSfs : public SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestMulC_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiMulC_16sc_C1RSfs", fwiMulC_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("7 100 3 MAX 0 0",  "2 -3", "314 179 32767 32767 0 0 ", 0 );
        RunTest("7 100 3 MAX 0 0",  "2 -3", "157 90 32767 32762 0 0", 1 );
    }
};

class TestMulC_32sc_C1RSfs : public SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestMulC_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiMulC_32sc_C1RSfs", fwiMulC_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("7 100 3 MAX 0 0", "2 -3",  
            "314 179 2147483647 2147483647 0 0", 0 );
        RunTest("7 100 3 MAX 0 0", "2 -3", 
            "157 90 2147483647 2147483642 0 0", 1 );
    }
};

//Case 2: Not-in-place operation on multi-channel integer or complex data
class TestMulC_8u_C3RSfs : public SrcPValDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestMulC_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 3, U8, 3 >( parent, "fwiMulC_8u_C3RSfs", fwiMulC_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "20 27 MAX MAX 66 MAX", 0 );
        RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "10 14 MAX MAX 33 MAX", 1 );
    }
};

class TestMulC_8u_AC4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMulC_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMulC_8u_AC4RSfs", fwiMulC_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "20 27 MAX 0 MAX 66 MAX 0", 0 );
        RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "10 14 MAX 0 MAX 33 MAX 0", 1 );
    }
};


class TestMulC_8u_C4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestMulC_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiMulC_8u_C4RSfs", fwiMulC_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX 22 MAX 22 50 22", "2 3 MAX 11", "20 27 MAX 242 MAX 66 MAX 242", 0 );
        RunTest("10 9 MAX 22 MAX 22 50 22", "2 3 MAX 11", "10 14 MAX 121 MAX 33 MAX 121", 1 );
    }
};

class TestMulC_16s_C3RSfs : public SrcPValDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestMulC_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 3, S16, 3 >( parent, "fwiMulC_16s_C3RSfs", fwiMulC_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "20 27 32767 32767 66 32767", 0 );
        RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "10 14 32767 32767 33 32767", 1 );
    }
};

class TestMulC_16s_AC4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMulC_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMulC_16s_AC4RSfs", fwiMulC_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "20 27 32767 0 32767 66 32767 0", 0 );
        RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "10 14 32767 0 32767 33 32767 0", 1 );
    }
};

class TestMulC_16s_C4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestMulC_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiMulC_16s_C4RSfs", fwiMulC_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("10 9 MAX 22 MAX 22 50 22", "2 3 MAX 11", "20 27 32767 242 32767 66 32767 242", 0 );
        RunTest("10 9 MAX 22 MAX 22 50 22", "2 3 MAX 11", "10 14 32767 121 32767 33 32767 121", 1 );
    }
};

class TestMulC_16sc_C3RSfs : public SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestMulC_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiMulC_16sc_C3RSfs", fwiMulC_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX 0 0", "2 -3 1 -2 3 4", "314 179 32767 32761 0 0", 0 );
        RunTest( "7 100 3 MAX 0 0", "2 -3 1 -2 3 4", "157 90 32767 16380 0 0", 1 );
    }
};

class TestMulC_16sc_AC4RSfs : public SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestMulC_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiMulC_16sc_AC4RSfs", fwiMulC_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX 0 0 222 222", "2 -3 1 -2 3 4 2 2", 
            "314 179 32767 32761 0 0 0 0", 0 );
        RunTest( "7 100 3 MAX 0 0 222 222", "2 -3 1 -2 3 4 2 2", 
            "157 90 32767 16380 0 0 0 0", 1 );
    }
};

class TestMulC_32sc_C3RSfs : public SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestMulC_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiMulC_32sc_C3RSfs", fwiMulC_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX 0 0", "2 -3 1 -2 3 4",  
            "314 179 2147483647 2147483641 0 0", 0 );
        RunTest( "7 100 3 MAX 0 0", "2 -3 1 -2 3 4", 
            "157 90 2147483647 1073741820 0 0", 1 );
    }
};

class TestMulC_32sc_AC4RSfs : public SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestMulC_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiMulC_32sc_AC4RSfs", fwiMulC_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX 0 0 222 222", "2 -3 1 -2 3 4 111 111", 
            "314 179 2147483647 2147483641 0 0 0 0", 0 );
        RunTest( "7 100 3 MAX 0 0 222 222", "2 -3 1 -2 3 4 111 111", 
            "157 90 2147483647 1073741820 0 0 0 0", 1 );
    }
};

//Case 3: Not-in-place operation on 1-channel floating-point or complex data.
class TestMulC_32f_C1R : public SrcValDstRoi< F32, 1, F32, 1 >
{
public:
    TestMulC_32f_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< F32, 1, F32, 1 >( parent, "fwiMulC_32f_C1R", fwiMulC_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 0.12345","2 ", "14 200 6 0.2469", 0.0001f );
        RunTest( "2 2.301 5.556 4","-3", "-6 -6.903 -16.668 -12 ", 1.0e+33f );
    }
};

class TestMulC_32fc_C1R : public SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestMulC_32fc_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiMulC_32fc_C1R", fwiMulC_32fc_C1R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "7 100 3 4", "2 -3", "314 179 18 -1", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        RunTest( "MAX MIN 0 0", "3 1", "6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

//Case 4: Not-in-place operation on multi-channel floating-point or complex data
class TestMulC_32f_C3R : public SrcPValDstRoi< F32, 3, F32, 3 >
{
public:
    TestMulC_32f_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 3, F32, 3 >( parent, "fwiMulC_32f_C3R", fwiMulC_32f_C3R )
    {}

    virtual void RunAll()
    {
        
        RunTest( "7 100 0.12345 3.14159 2.6535 8.9793", "2 -3 5.1", 
            "14 -300 0.629595 6.28318 -7.9605 45.79443 ", 0.0001f );
        RunTest( "MAX MIN MAX MIN 0 0", "3 1 -0.5",  
            "10.208457e+38 -3.40282e+38 -1.70141e+38 -10.208457e+38 0 0", 1.0e+33f );
    }
};

class TestMulC_32f_AC4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestMulC_32f_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiMulC_32f_AC4R", fwiMulC_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 0.12345 111 3.14159 2.6535 8.9793 111", "2 -3 5.1 222", 
            "14 -300 0.629595 0 6.28318 -7.9605 45.79443 0", 0.0001f );
        RunTest( "MAX MIN MAX INF MAX MIN MAX INF", "3 1 -0.5 INF ",  
            "10.208457e+38 -3.40282e+38 -1.70141e+38 0 10.208457e+38 -3.40282e+38 -1.70141e+38 0 ", 1.0e+33f );
    }
};

class TestMulC_32f_C4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestMulC_32f_C4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiMulC_32f_C4R", fwiMulC_32f_C4R )
    {}

    virtual void RunAll()
    {
       RunTest( "7 100 0.12345 22 3.14159 2.6535 8.9793 22", "2 -3 5.1 11", 
            "14 -300 0.629595 242 6.28318 -7.9605 45.79443 242", 0.0001f );
        RunTest( "MAX MIN MAX 22 MAX MIN MAX 22", "3 1 -0.5 11",  
            "10.208457e+38 -3.40282e+38 -1.70141e+38 242 10.208457e+38 -3.40282e+38 -1.70141e+38 242", 1.0e+33f );
    }
};

class TestMulC_32fc_C3R : public SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestMulC_32fc_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiMulC_32fc_C3R", fwiMulC_32fc_C3R )
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

class TestMulC_32fc_AC4R : public SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestMulC_32fc_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiMulC_32fc_AC4R", fwiMulC_32fc_AC4R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
                    "314 179 18 -1 7 26 0 0", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        RunTest( "3 1 -3 -1 3 4 111 111", "MAX MIN MAX MIN 0 0 222 222", 
                 "6.805638e+38 -6.805638e+38 -6.805638e+38 6.805638e+38 0 0 222 222", errorMargin );
    }
};

//Case 5: In-place operation on 1-channel integer or complex data
class TestMulC_8u_C1IRSfs : public ValSDstRoiScale< U8, 1 >
{
public:
    TestMulC_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< U8, 1 >( parent, "fwiMulC_8u_C1IRSfs", fwiMulC_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "3", "7 100 3 22", "21 MAX 9 66", 0 );
        RunTest( "3", "7 100 3 22", "10 150 4 33", 1 );
    }
};

class TestMulC_16s_C1IRSfs : public ValSDstRoiScale< S16, 1>
{
public:
    TestMulC_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< S16, 1>( parent, "fwiMulC_16s_C1IRSfs", fwiMulC_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "-3", "7 100 3 MAX MIN -33", "-21 -300 -9 -32768 32767 99", 0 );
        RunTest( "-3", "7 100 3 MAX MIN -33", "-10 -150 -4 -32768 32767 50", 1 );
    }
};

class TestMulC_16sc_C1IRSfs : public ValSDstRoiScale< Fw16sc, 1>
{
public:
    TestMulC_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< Fw16sc, 1>( parent, "fwiMulC_16sc_C1IRSfs", fwiMulC_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 -3", "7 100 3 MAX 0 0",  "314 179 32767 32767 0 0 ", 0 );
        RunTest("2 -3", "7 100 3 MAX 0 0",  "157 90 32767 32762 0 0", 1 );
    }
};

class TestMulC_32sc_C1IRSfs : public ValSDstRoiScale< Fw32sc, 1>
{
public:
    TestMulC_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< Fw32sc, 1>( parent, "fwiMulC_32sc_C1IRSfs", fwiMulC_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 -3",  "7 100 3 MAX 0 0", 
            "314 179 2147483647 2147483647 0 0", 0 );
        RunTest("2 -3", "7 100 3 MAX 0 0", 
            "157 90 2147483647 2147483642 0 0", 1 );
    }
};

//Case 6: In-place operation on multi-channel integer or complex data.
class TestMulC_8u_C3IRSfs : public PValSDstRoiScale< U8, 3 >
{
public:
    TestMulC_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 3 >( parent, "fwiMulC_8u_C3IRSfs", fwiMulC_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "20 27 MAX MAX 66 MAX", 0 );
        RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "10 14 MAX MAX 33 MAX", 1 );
    }
};

class TestMulC_8u_AC4IRSfs : public PValSDstRoiScale< U8, 4>
{
public:
    TestMulC_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 4>( parent, "fwiMulC_8u_AC4IRSfs", fwiMulC_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "20 27 MAX 222 MAX 66 MAX 222", 0 );
        RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "10 14 MAX 222 MAX 33 MAX 222", 1 );
    }
};

class TestMulC_8u_C4IRSfs : public PValSDstRoiScale< U8, 4>
{
public:
    TestMulC_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 4>( parent, "fwiMulC_8u_C4IRSfs", fwiMulC_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 11", "10 9 MAX 22 MAX 22 50 22", "20 27 MAX 242 MAX 66 MAX 242", 0 );
        RunTest("2 3 MAX 11", "10 9 MAX 22 MAX 22 50 22", "10 14 MAX 121 MAX 33 MAX 121", 1 );
    }
};

class TestMulC_16s_C3IRSfs : public PValSDstRoiScale< S16, 3>
{
public:
    TestMulC_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 3>( parent, "fwiMulC_16s_C3IRSfs", fwiMulC_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "20 27 32767 32767 66 32767", 0 );
        RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "10 14 32767 32767 33 32767", 1 );
    }
};

class TestMulC_16s_AC4IRSfs : public PValSDstRoiScale< S16, 4>
{
public:
    TestMulC_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 4>( parent, "fwiMulC_16s_AC4IRSfs", fwiMulC_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "20 27 32767 222 32767 66 32767 222", 0 );
        RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "10 14 32767 222 32767 33 32767 222", 1 );
    }
};

class TestMulC_16s_C4IRSfs : public PValSDstRoiScale< S16, 4>
{
public:
    TestMulC_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 4>( parent, "fwiMulC_16s_C4IRSfs", fwiMulC_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 3 MAX 11", "10 9 MAX 22 MAX 22 50 22", "20 27 32767 242 32767 66 32767 242", 0 );
        RunTest("2 3 MAX 11", "10 9 MAX 22 MAX 22 50 22", "10 14 32767 121 32767 33 32767 121", 1 );
    }
};

class TestMulC_16sc_C3IRSfs : public PValSDstRoiScale< Fw16sc, 3>
{
public:
    TestMulC_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw16sc, 3>( parent, "fwiMulC_16sc_C3IRSfs", fwiMulC_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "314 179 32767 32761 0 0", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", "157 90 32767 16380 0 0", 1 );
    }
};

class TestMulC_16sc_AC4IRSfs : public PValSDstRoiScale< Fw16sc, 4>
{
public:
    TestMulC_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw16sc, 4>( parent, "fwiMulC_16sc_AC4IRSfs", fwiMulC_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 2 2", "7 100 3 MAX 0 0 222 222", 
            "314 179 32767 32761 0 0 222 222", 0 );
        RunTest( "2 -3 1 -2 3 4 2 2", "7 100 3 MAX 0 0 222 222", 
            "157 90 32767 16380 0 0 222 222", 1 );
    }
};

class TestMulC_32sc_C3IRSfs : public PValSDstRoiScale< Fw32sc, 3>
{
public:
    TestMulC_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw32sc, 3>( parent, "fwiMulC_32sc_C3IRSfs", fwiMulC_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0",  
            "314 179 2147483647 2147483641 0 0", 0 );
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX 0 0", 
            "157 90 2147483647 1073741820 0 0", 1 );
    }
};

class TestMulC_32sc_AC4IRSfs : public PValSDstRoiScale< Fw32sc, 4>
{
public:
    TestMulC_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw32sc, 4>( parent, "fwiMulC_32sc_AC4IRSfs", fwiMulC_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
            "314 179 2147483647 2147483641 0 0 222 222", 0 );
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX 0 0 222 222", 
            "157 90 2147483647 1073741820 0 0 222 222", 1 );
    }
};

//Case 7: In-place operation on 1-channel floating-point or complex data.
class TestMulC_32f_C1IR : public ValSDstRoi< F32, 1 >
{
public:
    TestMulC_32f_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< F32, 1 >( parent, "fwiMulC_32f_C1IR", fwiMulC_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( " 2", "7 -100 3 -0.12345", "14 -200 6 -0.2469", 0.0001f );
        RunTest( "-3", "2 2.301 5.556 4",  "-6 -6.903 -16.668 -12 ", 1.0e+33f );
    }
};

class TestMulC_32fc_C1IR : public ValSDstRoi< Fw32fc, 1 >
{
public:
    TestMulC_32fc_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< Fw32fc, 1 >( parent, "fwiMulC_32fc_C1IR", fwiMulC_32fc_C1IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "314 179 18 -1", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        RunTest( "3 1", "MAX MIN 0 0", "6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

//Case 8: In-place operation on multi-channel floating-point or complex data.
class TestMulC_32f_C3IR : public PValSDstRoi< F32, 3>
{
public:
    TestMulC_32f_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 3>( parent, "fwiMulC_32f_C3IR", fwiMulC_32f_C3IR )
    {}

    virtual void RunAll()
    {
        //Functions make mistake on duplicating the first channel data of C for all channels
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
            "14 -300 0.629595 6.28318 -7.9605 45.79443 ", 0.0001f );
        RunTest( "3 1 -0.5", "MAX MIN MAX MIN 0 0", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 -10.208457e+38 0 0", 1.0e+33f );
    }
};

class TestMulC_32f_AC4IR : public PValSDstRoi< F32, 4>
{
public:
    TestMulC_32f_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 4>( parent, "fwiMulC_32f_AC4IR", fwiMulC_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 INF", "7 100 0.12345 INF 3.14159 2.6535 8.9793 INF", 
            "14 -300 0.629595 INF 6.28318 -7.9605 45.79443 INF", 0.0001f );
        RunTest( "3 1 -0.5 INF", "MAX MIN MAX INF MAX MIN MAX INF", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 INF 10.208457e+38 -3.40282e+38 -1.70141e+38 INF ", 1.0e+33f );
    }
};

class TestMulC_32f_C4IR : public PValSDstRoi< F32, 4>
{
public:
    TestMulC_32f_C4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 4>( parent, "fwiMulC_32f_C4IR", fwiMulC_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 11", "7 100 0.12345 22 3.14159 2.6535 8.9793 22", 
            "14 -300 0.629595 242 6.28318 -7.9605 45.79443 242", 0.0001f );
          RunTest( "3 1 -0.5 11", "MAX MIN MAX 22 MAX MIN MAX 22", 
            "10.208457e+38 -3.40282e+38 -1.70141e+38 242 10.208457e+38 -3.40282e+38 -1.70141e+38 242", 1.0e+33f );
    }
};

class TestMulC_32fc_C3IR : public PValSDstRoi< Fw32fc, 3>
{
public:
    TestMulC_32fc_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< Fw32fc, 3>( parent, "fwiMulC_32fc_C3IR", fwiMulC_32fc_C3IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "314 179 18 -1 7 26", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        RunTest( "3 1 3 1 3 4", "MAX MIN MAX MIN 0 0", 
            "6.805638e+38 -6.805638e+38 6.805638e+38 -6.805638e+38 0 0", errorMargin );
    }
};

class TestMulC_32fc_AC4IR : public PValSDstRoi< Fw32fc, 4>
{
public:
    TestMulC_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< Fw32fc, 4>( parent, "fwiMulC_32fc_AC4IR", fwiMulC_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {0.0001f, 0.0001f};
        //Bugs in the function, results are NOT expected
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
            "314 179 18 -1 7 26 INF INF", errorMargin );
        errorMargin.re = 1.0e+33f;
        errorMargin.im = 1.0e+33f;
        RunTest( "3 1 -3 1 3 4 INF INF", "MAX MIN MAX MIN 0 0 INF INF", 
            "6.805638e+38 -6.805638e+38 -6.805638e+38 6.805638e+38 0 0 INF INF", errorMargin );
    }
};

DEFINE_TEST_TABLE( MulCTestCatalog )
//Case 1
TEST_ENTRY( TestMulC_8u_C1RSfs )        
TEST_ENTRY( TestMulC_16s_C1RSfs )
TEST_ENTRY( TestMulC_16sc_C1RSfs )
TEST_ENTRY( TestMulC_32sc_C1RSfs )

//Case 2
TEST_ENTRY( TestMulC_8u_C3RSfs )
TEST_ENTRY( TestMulC_8u_AC4RSfs )
TEST_ENTRY( TestMulC_8u_C4RSfs ) 
TEST_ENTRY( TestMulC_16s_C3RSfs )
TEST_ENTRY( TestMulC_16s_AC4RSfs )
TEST_ENTRY( TestMulC_16s_C4RSfs ) 
TEST_ENTRY( TestMulC_16sc_C3RSfs )
TEST_ENTRY( TestMulC_16sc_AC4RSfs )
TEST_ENTRY( TestMulC_32sc_C3RSfs )
TEST_ENTRY( TestMulC_32sc_AC4RSfs )

//Case3
TEST_ENTRY( TestMulC_32f_C1R )
TEST_ENTRY( TestMulC_32fc_C1R )

//Case4
TEST_ENTRY( TestMulC_32f_C3R )      
TEST_ENTRY( TestMulC_32f_AC4R )     
TEST_ENTRY( TestMulC_32f_C4R ) 
TEST_ENTRY( TestMulC_32fc_C3R )
TEST_ENTRY( TestMulC_32fc_AC4R )

//Case5
TEST_ENTRY( TestMulC_8u_C1IRSfs )       
TEST_ENTRY( TestMulC_16s_C1IRSfs )
TEST_ENTRY( TestMulC_16sc_C1IRSfs )
TEST_ENTRY( TestMulC_32sc_C1IRSfs )

//Case6
TEST_ENTRY( TestMulC_8u_C3IRSfs )
TEST_ENTRY( TestMulC_8u_AC4IRSfs )
TEST_ENTRY( TestMulC_8u_C4IRSfs )      
TEST_ENTRY( TestMulC_16s_C3IRSfs )
TEST_ENTRY( TestMulC_16s_AC4IRSfs )
TEST_ENTRY( TestMulC_16s_C4IRSfs )     
TEST_ENTRY( TestMulC_16sc_C3IRSfs )
TEST_ENTRY( TestMulC_16sc_AC4IRSfs )
TEST_ENTRY( TestMulC_32sc_C3IRSfs )
TEST_ENTRY( TestMulC_32sc_AC4IRSfs )

//Case7
TEST_ENTRY( TestMulC_32f_C1IR )
TEST_ENTRY( TestMulC_32fc_C1IR )

//Case8
TEST_ENTRY( TestMulC_32f_C3IR )
TEST_ENTRY( TestMulC_32f_AC4IR )
TEST_ENTRY( TestMulC_32f_C4IR )       
TEST_ENTRY( TestMulC_32fc_C3IR )
TEST_ENTRY( TestMulC_32fc_AC4IR )

END_TEST_TABLE()
