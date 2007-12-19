/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer or complex data
class TestDiv_8u_C1RSfs : public SrcSrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestDiv_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiDiv_8u_C1RSfs", fwiDiv_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
		//currently inconsistent rounding
        RunTest( "2 3", "7 100 3 MAX", "4 33 2 85", 0 );
		RunTest( "2 3", "7 100 3 MAX", "2 17 1 42", 1 );
    }
};

class TestDiv_8u_C3RSfs : public SrcSrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestDiv_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiDiv_8u_C3RSfs", fwiDiv_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
		//Show rounding 255/2=128, currently inconsistent rounding
		RunTest( "1 0 MAX 2 3 MAX", "5 0 MAX MAX 0 1", "5   0 1 128 0  0", 0 );
		RunTest( "1 0 MAX 2 3 5", "5 1 MAX MAX 0 100", "2 255 0  64 0 10", 1 );
    }
};

//Unsupported functions
//class TestDiv_8u_AC4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
//{
//public:
//    TestDiv_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiDiv_8u_AC4RSfs", fwiDiv_8u_AC4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 0 128 0  0 0", 0 );
//		RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 255 0 0  64 0 10 0", 1 );
//    }
//};
//
//class TestDiv_8u_C4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
//{
//public:
//    TestDiv_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiDiv_8u_C4RSfs", fwiDiv_8u_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 2 128 0  0 2", 0 );
//		RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 255 0 2  64 0 10 2", 1 );
//    }
//};

class TestDiv_16s_C1RSfs : public SrcSrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestDiv_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiDiv_16s_C1RSfs", fwiDiv_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
		//currently inconsistent rounding,
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", "4 -33 2 -10922 -16384 10923", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", "2 -17 1  -5461  -8192  5461", 1 );
    }
};

class TestDiv_16s_C3RSfs : public SrcSrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestDiv_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiDiv_16s_C3RSfs", fwiDiv_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
		//Show rounding 255/2=128, currently inconsistent rounding
		RunTest( "1 0 MAX -2 -3 MIN", "5 0 MAX MAX 0   1", "5   0 1 -16384 0  0", 0 );
		RunTest( "1 0 MAX -2 -3   5", "5 1 MAX MAX 0 100", "2 MAX 0  -8192 0 10", 1 );
    }
};

//Missing function
//class TestDiv_16s_AC4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
//{
//public:
//    TestDiv_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiDiv_16s_AC4RSfs", fwiDiv_16s_AC4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 -2 -3 MIN 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 0 -16384 0  0 0", 0 );
//		RunTest( "1 0 MAX 111 -2 -3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 MAX 0 0  -8192 0 10 0", 1 );
//    }
//};

//Missing function
//class TestDiv_16s_C4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
//{
//public:
//    TestDiv_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiDiv_16s_C4RSfs", fwiDiv_16s_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 -2 -3 MIN 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 2 -16384 0  0 2", 0 );
//		RunTest( "1 0 MAX 111 -2 -3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 MAX 0 2  -8192 0 10 2", 1 );
//    }
//};

class TestDiv_16sc_C1RSfs : public SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestDiv_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiDiv_16sc_C1RSfs", fwiDiv_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", "-22 17 -7561 5042 2521 -12603", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", "-11  8 -3781 2521 1260  -6302", 1 );
    }
};

class TestDiv_16sc_C3RSfs : public SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestDiv_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiDiv_16sc_C3RSfs", fwiDiv_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-22 17 -13106 6555 -9175 1311", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-11  8  -6553 3277 -4588  655", 1 );
    }
};

class TestDiv_16sc_AC4RSfs : public SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestDiv_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiDiv_16sc_AC4RSfs", fwiDiv_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", "-22 17 -13106 6555 -9175 1311 0 0", 0 );
		RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", "-11  8  -6553 3277 -4588  655 0 0", 1 );
    }
};

class TestDiv_32sc_C1RSfs : public SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestDiv_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiDiv_32sc_C1RSfs", fwiDiv_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", 
			"-22 17 -495573149 330382100 165191050 -825955249", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", 
			"-11  8 -247786574 165191050  82595525 -412977625", 1 );
    }
};

class TestDiv_32sc_C3RSfs : public SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestDiv_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiDiv_32sc_C3RSfs", fwiDiv_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", 
			"-22 17 -858993458 429496731 -601295421 85899346", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", 
			"-11  8 -429496729 214748365 -300647711 42949673", 1 );
    }
};

class TestDiv_32sc_AC4RSfs : public SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestDiv_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiDiv_32sc_AC4RSfs", fwiDiv_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-22 17  -858993458 429496731 -601295421 85899346 0 0", 0 );
		RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-11  8  -429496729 214748365 -300647711 42949673 0 0", 1 );
    }
};

//Case 2: Not-in-place operation on floating-point or complex data.
class TestDiv_32f_C1R : public SrcSrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestDiv_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 1, F32, 1 >( parent, "fwiDiv_32f_C1R", fwiDiv_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 0.12345", "3.5 -33.3333 1.5 -0.04115", 0.0001f );
		RunTest( "3 1 -5 -2", "MAX MIN MAX MIN", "1.134273e+38 -3.40282e+38 -6.80564e+37 1.70141e+38", 1.0e+33f );
    }
};

class TestDiv_32f_C3R : public SrcSrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestDiv_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 3, F32, 3 >( parent, "fwiDiv_32f_C3R", fwiDiv_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
			"3.5 -33.3333 0.0242059 1.570795 -0.8845 1.760647", 0.0001f );
		RunTest( "3 1 -0.5 -0.2 0 0", "MAX MIN MAX MIN MAX MIN", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF INF INF_NEG", 1.0e+33f );
    }
};

class TestDiv_32f_AC4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestDiv_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiDiv_32f_AC4R", fwiDiv_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0", "7 100 0.12345 0 3.14159 2.6535 8.9793 0", 
			"3.5 -33.3333 0.0242059 0 1.570795 -0.8845 1.760647 0", 0.0001f );
		RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX INF MIN MAX MIN INF", 
			"1.134273e+38 -3.40282e+38 INF_NEG 0 INF INF INF_NEG 0", 1.0e+33f );
    }
};

class TestDiv_32f_C4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestDiv_32f_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiDiv_32f_C4R", fwiDiv_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0.1", "7 100 0.12345 0 3.14159 2.6535 8.9793 10", 
			"3.5 -33.3333 0.0242059 0 1.570795 -0.8845 1.760647 100", 0.0001f );
		RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX 1 MIN MAX MIN 2", 
			"1.134273e+38 -3.40282e+38 INF_NEG 0 INF INF INF_NEG 0", 1.0e+33f );
    }
};

class TestDiv_32fc_C1R : public SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestDiv_32fc_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiDiv_32fc_C1R", fwiDiv_32fc_C1R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "-22 17 -0.461538 1.30769", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2", "MAX MIN MAX MIN", "6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37", errorMargin );
    }
};

class TestDiv_32fc_C3R : public SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestDiv_32fc_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiDiv_32fc_C3R", fwiDiv_32fc_C3R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "-22 17 -0.461538 1.30769 0.92 -0.56", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2 3 4", "MAX MIN MAX MIN MAX MIN", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37", errorMargin );
    }
};

class TestDiv_32fc_AC4R : public SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestDiv_32fc_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiDiv_32fc_AC4R", fwiDiv_32fc_AC4R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
			"-22 17 -0.461538 1.30769 0.92 -0.56 0 0", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2 3 4 INF INF", "MAX MIN MAX MIN MAX MIN INF INF", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37 0 0", errorMargin );
    }
};

//Case 3: In-place operation on integer or complex data.
class TestDiv_8u_C1IRSfs : public SrcSDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestDiv_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 1, U8, 1 >( parent, "fwiDiv_8u_C1IRSfs", fwiDiv_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
		//currently inconsistent rounding
        RunTest( "2 3", "7 100 3 MAX", "4 33 2 85", 0 );
		RunTest( "2 3", "7 100 3 MAX", "2 17 1 42", 1 );
    }
};

class TestDiv_8u_C3IRSfs : public SrcSDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestDiv_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 3, U8, 3 >( parent, "fwiDiv_8u_C3IRSfs", fwiDiv_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//Show rounding 255/2=128, currently inconsistent rounding
		RunTest( "1 0 MAX 2 3 MAX", "5 0 MAX MAX 0 1", "5   0 1 128 0  0", 0 );
		RunTest( "1 0 MAX 2 3 5", "5 1 MAX MAX 0 100", "2 255 0  64 0 10", 1 );
    }
};

//Missing functions
//class TestDiv_8u_AC4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
//{
//public:
//    TestDiv_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiDiv_8u_AC4IRSfs", fwiDiv_8u_AC4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 0 128 0  0 0", 0 );
//		RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 255 0 0  64 0 10 0", 1 );
//    }
//};
//
//class TestDiv_8u_C4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
//{
//public:
//    TestDiv_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiDiv_8u_C4IRSfs", fwiDiv_8u_C4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 2 3 MAX 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 2 128 0  0 2", 0 );
//		RunTest( "1 0 MAX 111 2 3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 255 0 2  64 0 10 2", 1 );
//    }
//};

class TestDiv_16s_C1IRSfs : public SrcSDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestDiv_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 1, S16, 1 >( parent, "fwiDiv_16s_C1IRSfs", fwiDiv_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
		//currently inconsistent rounding,
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", "4 -33 2 -10922 -16384 10923", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", "2 -17 1  -5461  -8192  5461", 1 );
    }
};

class TestDiv_16s_C3IRSfs : public SrcSDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestDiv_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 3, S16, 3 >( parent, "fwiDiv_16s_C3IRSfs", fwiDiv_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//Show rounding 255/2=128, currently inconsistent rounding
		RunTest( "1 0 MAX -2 -3 MIN", "5 0 MAX MAX 0   1", "5   0 1 -16384 0  0", 0 );
		RunTest( "1 0 MAX -2 -3   5", "5 1 MAX MAX 0 100", "2 MAX 0  -8192 0 10", 1 );
    }
};

//Missing functions
//class TestDiv_16s_AC4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
//{
//public:
//    TestDiv_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiDiv_16s_AC4IRSfs", fwiDiv_16s_AC4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 -2 -3 MIN 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 0 -16384 0  0 0", 0 );
//		RunTest( "1 0 MAX 111 -2 -3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 MAX 0 0  -8192 0 10 0", 1 );
//    }
//};
//
//class TestDiv_16s_C4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
//{
//public:
//    TestDiv_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiDiv_16s_C4IRSfs", fwiDiv_16s_C4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		//Show rounding 255/2=128, currently inconsistent rounding
//		RunTest( "1 0 MAX 111 -2 -3 MIN 111", "5 0 MAX 222 MAX 0   1 222", "5   0 1 2 -16384 0  0 2", 0 );
//		RunTest( "1 0 MAX 111 -2 -3   5 111", "5 1 MAX 222 MAX 0 100 222", "2 MAX 0 2  -8192 0 10 2", 1 );
//    }
//};

class TestDiv_16sc_C1IRSfs : public SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestDiv_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiDiv_16sc_C1IRSfs", fwiDiv_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", "-22 17 -7561 5042 2521 -12603", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", "-11  8 -3781 2521 1260  -6302", 1 );
    }
};

class TestDiv_16sc_C3IRSfs : public SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestDiv_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiDiv_16sc_C3IRSfs", fwiDiv_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-22 17 -13106 6555 -9175 1311", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-11  8  -6553 3277 -4588  655", 1 );
    }
};

class TestDiv_16sc_AC4IRSfs : public SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestDiv_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiDiv_16sc_AC4IRSfs", fwiDiv_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-22 17 -13106 6555 -9175 1311 222 222", 0 );
		RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-11  8  -6553 3277 -4588  655 222 222", 1 );
    }
};

class TestDiv_32sc_C1IRSfs : public SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestDiv_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiDiv_32sc_C1IRSfs", fwiDiv_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 MAX MIN MIN", 
			"-22 17 -495573149 330382100 165191050 -825955249", 0 );
		RunTest( "2 -3", "7 100 3 MAX MIN MIN", 
			"-11  8 -247786574 165191050  82595525 -412977625", 1 );
    }
};

class TestDiv_32sc_C3IRSfs : public SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestDiv_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiDiv_32sc_C3IRSfs", fwiDiv_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", 
			"-22 17 -858993458 429496731 -601295421 85899346", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", 
			"-11  8 -429496729 214748365 -300647711 42949673", 1 );
    }
};

class TestDiv_32sc_AC4IRSfs : public SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestDiv_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiDiv_32sc_AC4IRSfs", fwiDiv_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-22 17  -858993458 429496731 -601295421 85899346 222 222", 0 );
		RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-11  8  -429496729 214748365 -300647711 42949673 222 222", 1 );
    }
};

//Case 4: In-place operation on floating-point or complex data.
class TestDiv_32f_C1IR : public SrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestDiv_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiDiv_32f_C1IR", fwiDiv_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3", "7 100 3 0.12345", "3.5 -33.3333 1.5 -0.04115", 0.0001f );
		RunTest( "3 1 -5 -2", "MAX MIN MAX MIN", "1.134273e+38 -3.40282e+38 -6.80564e+37 1.70141e+38", 1.0e+33f );
    }
};

class TestDiv_32f_C3IR : public SrcSDstRoi< F32, 3, F32, 3 >
{
public:
    TestDiv_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 3, F32, 3 >( parent, "fwiDiv_32f_C3IR", fwiDiv_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
			"3.5 -33.3333 0.0242059 1.570795 -0.8845 1.760647", 0.0001f );
		RunTest( "3 1 -0.5 -0.2 0 0", "MAX MIN MAX MIN MAX MIN", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF INF INF_NEG", 1.0e+33f );
    }
};

class TestDiv_32f_AC4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestDiv_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiDiv_32f_AC4IR", fwiDiv_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 INF", "7 100 0.12345 INF 3.14159 2.6535 8.9793 INF", 
			"3.5 -33.3333 0.0242059 INF 1.570795 -0.8845 1.760647 INF", 0.0001f );
		RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX INF MIN MAX MIN INF", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF INF INF INF_NEG INF", 1.0e+33f );
    }
};

class TestDiv_32f_C4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestDiv_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiDiv_32f_C4IR", fwiDiv_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 0.1", "7 100 0.12345 0 3.14159 2.6535 8.9793 10", 
			"3.5 -33.3333 0.0242059 0 1.570795 -0.8845 1.760647 100", 0.0001f );
		RunTest( "3 1 -0.5 INF -0.2 0 0 INF", "MAX MIN MAX 1 MIN MAX MIN 2", 
			"1.134273e+38 -3.40282e+38 INF_NEG 0 INF INF INF_NEG 0", 1.0e+33f );
    }
};

class TestDiv_32fc_C1IR : public SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestDiv_32fc_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiDiv_32fc_C1IR", fwiDiv_32fc_C1IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "-22 17 -0.461538 1.30769", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2", "MAX MIN MAX MIN", "6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37", errorMargin );
    }
};

class TestDiv_32fc_C3IR : public SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestDiv_32fc_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiDiv_32fc_C3IR", fwiDiv_32fc_C3IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "-22 17 -0.461538 1.30769 0.92 -0.56", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2 3 4", "MAX MIN MAX MIN MAX MIN", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37", errorMargin );
    }
};

class TestDiv_32fc_AC4IR : public SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestDiv_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiDiv_32fc_AC4IR", fwiDiv_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
			"-22 17 -0.461538 1.30769 0.92 -0.56 INF INF", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		//inconsistent results
		RunTest( "3 1 -5 -2 3 4 INF INF", "MAX MIN MAX MIN MAX MIN INF INF", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37 INF INF", errorMargin );
    }
};

DEFINE_TEST_TABLE( DivTestCatalog )
TEST_ENTRY( TestDiv_8u_C1RSfs )
TEST_ENTRY( TestDiv_8u_C3RSfs )
//TEST_ENTRY( TestDiv_8u_AC4RSfs )     //Unsupported
//TEST_ENTRY( TestDiv_8u_C4RSfs )      //Unsupported
TEST_ENTRY( TestDiv_16s_C1RSfs )
TEST_ENTRY( TestDiv_16s_C3RSfs )
//TEST_ENTRY( TestDiv_16s_AC4RSfs )     //Unsupported
//TEST_ENTRY( TestDiv_16s_C4RSfs )      //Unsupported
TEST_ENTRY( TestDiv_16sc_C1RSfs )
TEST_ENTRY( TestDiv_16sc_C3RSfs )
TEST_ENTRY( TestDiv_16sc_AC4RSfs )
TEST_ENTRY( TestDiv_32sc_C1RSfs )
TEST_ENTRY( TestDiv_32sc_C3RSfs )
TEST_ENTRY( TestDiv_32sc_AC4RSfs )
TEST_ENTRY( TestDiv_32f_C1R )
TEST_ENTRY( TestDiv_32f_C3R )
TEST_ENTRY( TestDiv_32f_AC4R )
TEST_ENTRY( TestDiv_32f_C4R )
TEST_ENTRY( TestDiv_32fc_C1R )
TEST_ENTRY( TestDiv_32fc_C3R )
TEST_ENTRY( TestDiv_32fc_AC4R )
TEST_ENTRY( TestDiv_8u_C1IRSfs )
TEST_ENTRY( TestDiv_8u_C3IRSfs )
//TEST_ENTRY( TestDiv_8u_AC4IRSfs )     //Unsupported
//TEST_ENTRY( TestDiv_8u_C4IRSfs )      //Unsupported
TEST_ENTRY( TestDiv_16s_C1IRSfs )
TEST_ENTRY( TestDiv_16s_C3IRSfs )
//TEST_ENTRY( TestDiv_16s_AC4IRSfs )     //Unsupported
//TEST_ENTRY( TestDiv_16s_C4IRSfs )      //Unsupported
TEST_ENTRY( TestDiv_16sc_C1IRSfs )
TEST_ENTRY( TestDiv_16sc_C3IRSfs )
TEST_ENTRY( TestDiv_16sc_AC4IRSfs )
TEST_ENTRY( TestDiv_32sc_C1IRSfs )
TEST_ENTRY( TestDiv_32sc_C3IRSfs )
TEST_ENTRY( TestDiv_32sc_AC4IRSfs )
TEST_ENTRY( TestDiv_32f_C1IR )
TEST_ENTRY( TestDiv_32f_C3IR )
TEST_ENTRY( TestDiv_32f_AC4IR )
TEST_ENTRY( TestDiv_32f_C4IR )
TEST_ENTRY( TestDiv_32fc_C1IR )
TEST_ENTRY( TestDiv_32fc_C3IR )
TEST_ENTRY( TestDiv_32fc_AC4IR )
END_TEST_TABLE()
