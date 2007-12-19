/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel integer or complex data.
class TestDivC_8u_C1RSfs : public SrcValDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestDivC_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< U8, 1, U8, 1 >( parent, "fwiDivC_8u_C1RSfs", fwiDivC_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX", "3", "2 33 1 85", 0 );
		RunTest( "7 100 3 MAX", "3", "1 17 0 42", 1 );
    }
};

class TestDivC_16s_C1RSfs : public SrcValDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestDivC_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< S16, 1, S16, 1 >( parent, "fwiDivC_16s_C1RSfs", fwiDivC_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX MIN -33", "-3", "-2 -33 -1 -10922 10923 11", 0 );
		RunTest( "7 100 3 MAX MIN -33", "-3", "-1 -17  0  -5461  5461  6", 1 );
    }
};

class TestDivC_16sc_C1RSfs : public SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestDivC_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiDivC_16sc_C1RSfs", fwiDivC_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("7 100 3 MAX MIN MIN",  "2 -3", "-22 17 -7561 5042 2521 -12603", 0 );
		RunTest("7 100 3 MAX MIN MIN",  "2 -3", "-11  8 -3781 2521 1260  -6302", 1 );
    }
};

class TestDivC_32sc_C1RSfs : public SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestDivC_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiDivC_32sc_C1RSfs", fwiDivC_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest("7 100 3 MAX MIN MIN", "2 -3",  
			"-22 17 -495573149 330382100 165191050 -825955249", 0 );
		RunTest("7 100 3 MAX MIN MIN", "2 -3", 
			"-11  8 -247786574 165191050  82595525 -412977625", 1 );
    }
};

//Case 2: Not-in-place operation on multi-channel integer or complex data
class TestDivC_8u_C3RSfs : public SrcPValDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestDivC_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< U8, 3, U8, 3 >( parent, "fwiDivC_8u_C3RSfs", fwiDivC_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
		//Bugs inside the function call
		RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "5 3 1 128 7 0", 0 );
		RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "2 2 0  64 4 0", 1 );
    }
};

class TestDivC_8u_AC4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestDivC_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiDivC_8u_AC4RSfs", fwiDivC_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "5 3 1 0 128 7 0 0", 0 );
		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "2 2 0 0  64 4 0 0", 1 );
    }
};

//Missing function
//class TestDivC_8u_C4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
//{
//public:
//    TestDivC_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoiScale( parent, "fwiDivC_8u_C4RSfs", fwiDivC_8u_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "5 3 1 2 128 7 0 2", 0 );
//		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "2 2 0 1  64 4 0 1", 1 );
//    }
//};

class TestDivC_16s_C3RSfs : public SrcPValDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestDivC_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< S16, 3, S16, 3 >( parent, "fwiDivC_16s_C3RSfs", fwiDivC_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
		RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "5 3 1 16384 7 0", 0 );
		RunTest("10 9 MAX MAX 22 50", "2 3 MAX", "2 2 0  8192 4 0", 1 );
    }
};

class TestDivC_16s_AC4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestDivC_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiDivC_16s_AC4RSfs", fwiDivC_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "5 3 1 0 16384 7 0 0", 0 );
		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "2 2 0 0  8192 4 0 0", 1 );
    }
};

//Missing function
//class TestDivC_16s_C4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
//{
//public:
//    TestDivC_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoiScale( parent, "fwiDivC_16s_C4RSfs", fwiDivC_16s_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "5 3 1 2 16384 7 0 0 2", 0 );
//		RunTest("10 9 MAX 222 MAX 22 50 222", "2 3 MAX 111", "2 2 0 1  8192 4 0 0 1", 1 );
//    }
//};

class TestDivC_16sc_C3RSfs : public SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestDivC_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiDivC_16sc_C3RSfs", fwiDivC_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
		//Bugs inside the function
        RunTest( "7 100 3 MAX MIN MIN", "2 -3 1 -2 3 4", "-22 17 -13106 6555 -9175 1311", 0 );
		RunTest( "7 100 3 MAX MIN MIN", "2 -3 1 -2 3 4", "-11  8  -6553 3277 -4588  655", 1 );
    }
};

class TestDivC_16sc_AC4RSfs : public SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestDivC_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiDivC_16sc_AC4RSfs", fwiDivC_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX MIN MIN 222 222", "2 -3 1 -2 3 4 2 2", 
			"-22 17 -13106 6555 -9175 1311 0 0", 0 );
		RunTest( "7 100 3 MAX MIN MIN 222 222", "2 -3 1 -2 3 4 2 2", 
			"-11  8  -6553 3277 -4588  655 0 0", 1 );
    }
};

class TestDivC_32sc_C3RSfs : public SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestDivC_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiDivC_32sc_C3RSfs", fwiDivC_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX MIN MIN", "2 -3 1 -2 3 4",  
			"-22 17 -858993458 429496731 -601295421 85899346", 0 );
		RunTest( "7 100 3 MAX MIN MIN", "2 -3 1 -2 3 4", 
			"-11  8 -429496729 214748365 -300647711 42949673", 1 );
    }
};

class TestDivC_32sc_AC4RSfs : public SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestDivC_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiDivC_32sc_AC4RSfs", fwiDivC_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 3 MAX MIN MIN 222 222", "2 -3 1 -2 3 4 111 111", 
			"-22 17  -858993458 429496731 -601295421 85899346 0 0", 0 );
		RunTest( "7 100 3 MAX MIN MIN 222 222", "2 -3 1 -2 3 4 111 111", 
			"-11  8  -429496729 214748365 -300647711 42949673 0 0", 1 );
    }
};

//Case 3: Not-in-place operation on 1-channel floating-point or complex data.
class TestDivC_32f_C1R : public SrcValDstRoi< F32, 1, F32, 1 >
{
public:
    TestDivC_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< F32, 1, F32, 1 >( parent, "fwiDivC_32f_C1R", fwiDivC_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "7 -100 3 -0.12345", " 2", "3.5 -50 1.5 -0.061725", 0.0001f );
		RunTest( "MAX MIN 2.124e+38", "-3", "-1.134273e+38 1.134273e+38 -7.08e+37", 1.0e+33f );
    }
};

class TestDivC_32fc_C1R : public SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestDivC_32fc_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiDivC_32fc_C1R", fwiDivC_32fc_C1R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "7 100 3 4", "2 -3", "-22 17 -0.461538 1.30769", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "MAX MIN MIN MIN", "3 1", "6.80565e+37 -1.36113e+38 -1.36113e+38 -6.80565e+37", errorMargin );
    }
};

//Case 4: Not-in-place operation on multi-channel floating-point or complex data
class TestDivC_32f_C3R : public SrcPValDstRoi< F32, 3, F32, 3 >
{
public:
    TestDivC_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< F32, 3, F32, 3 >( parent, "fwiDivC_32f_C3R", fwiDivC_32f_C3R )
    {}

    virtual void RunAll()
    {
		//Functions make mistake on duplicating the first channel data of C for all channels
        RunTest( "7 100 0.12345 3.14159 2.6535 8.9793", "2 -3 5.1", 
			"3.5 -33.3333 0.0242059 1.570795 -0.8845 1.760647", 0.0001f );
		RunTest( "MAX MIN MAX INF 1.5123e+37 1.5123e+37", "3 1 -0.5", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF 1.5123e+37 -3.0246e+37", 1.0e+33f );
    }
};

class TestDivC_32f_AC4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestDivC_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiDivC_32f_AC4R", fwiDivC_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "7 100 0.12345 INF 3.14159 2.6535 8.9793 INF", "2 -3 5.1 INF", 
			"3.5 -33.3333 0.0242059 0 1.570795 -0.8845 1.760647 0", 0.0001f );
		RunTest( "MAX MIN MAX INF INF 1.5123e+37 1.5123e+37 INF", "3 1 -0.5 INF", 
			"1.134273e+38 -3.40282e+38 INF_NEG 0 INF 1.5123e+37 -3.0246e+37 0", 1.0e+33f );
    }
};

//Missing function
//class TestDivC_32f_C4R : public SrcPValDstRoi< F32, 4, F32, 4 >
//{
//public:
//    TestDivC_32f_C4R( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoi( parent, "fwiDivC_32f_C4R", fwiDivC_32f_C4R )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "7 100 0.12345 222 3.14159 2.6535 8.9793 222", "2 -3 5.1 111", 
//			"3.5 -33.3333 0.0242059 2 1.570795 -0.8845 1.760647 2", 0.0001f );
//		RunTest( "MAX MIN MAX 222 INF 1.5123e+37 1.5123e+37 222", "3 1 -0.5 111", 
//			"1.134273e+38 -3.40282e+38 INF_NEG 2 INF 1.5123e+37 -3.0246e+37 2", 1.0e+33f );
//    }
//};

class TestDivC_32fc_C3R : public SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestDivC_32fc_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiDivC_32fc_C3R", fwiDivC_32fc_C3R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "7 100 3 4 5 2", "2 -3 2 -3 3 4", "-22 17 -0.461538 1.30769 0.92 -0.56", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "MAX MIN MAX MIN MAX MIN", "3 1 -5 -2 3 4", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37", errorMargin );
    }
};

class TestDivC_32fc_AC4R : public SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestDivC_32fc_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiDivC_32fc_AC4R", fwiDivC_32fc_AC4R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
		//Bugs in the function, results are NOT expected
        RunTest( "7 100 3 4 5 2 INF INF", "2 -3 2 -3 3 4 INF INF", 
			"-22 17 -0.461538 1.30769 0.92 -0.56 0 0", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "MAX MIN MAX MIN MAX MIN INF INF", "3 1 -5 -2 3 4 INF INF", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37 0 0", errorMargin );
    }
};

//Case 5: In-place operation on 1-channel integer or complex data
class TestDivC_8u_C1IRSfs : public ValSDstRoiScale< U8, 1 >
{
public:
    TestDivC_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< U8, 1 >( parent, "fwiDivC_8u_C1IRSfs", fwiDivC_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "3", "7 100 3 MAX", "2 33 1 85", 0 );
		RunTest( "3", "7 100 3 MAX", "1 17 0 42", 1 );
    }
};

class TestDivC_16s_C1IRSfs : public ValSDstRoiScale< S16, 1>
{
public:
    TestDivC_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< S16, 1>( parent, "fwiDivC_16s_C1IRSfs", fwiDivC_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "-3", "7 100 3 MAX MIN -33", "-2 -33 -1 -10922 10923 11", 0 );
		RunTest( "-3", "7 100 3 MAX MIN -33", "-1 -17  0  -5461  5461  6", 1 );
    }
};

class TestDivC_16sc_C1IRSfs : public ValSDstRoiScale< Fw16sc, 1>
{
public:
    TestDivC_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< Fw16sc, 1>( parent, "fwiDivC_16sc_C1IRSfs", fwiDivC_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 -3", "7 100 3 MAX MIN MIN",  "-22 17 -7561 5042 2521 -12603", 0 );
		RunTest("2 -3", "7 100 3 MAX MIN MIN",  "-11  8 -3781 2521 1260  -6302", 1 );
    }
};

class TestDivC_32sc_C1IRSfs : public ValSDstRoiScale< Fw32sc, 1>
{
public:
    TestDivC_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< Fw32sc, 1>( parent, "fwiDivC_32sc_C1IRSfs", fwiDivC_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest("2 -3",  "7 100 3 MAX MIN MIN", 
			"-22 17 -495573149 330382100 165191050 -825955249", 0 );
		RunTest("2 -3", "7 100 3 MAX MIN MIN", 
			"-11  8 -247786574 165191050  82595525 -412977625", 1 );
    }
};

//Case 6: In-place operation on multi-channel integer or complex data.
class TestDivC_8u_C3IRSfs : public PValSDstRoiScale< U8, 3 >
{
public:
    TestDivC_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< U8, 3 >( parent, "fwiDivC_8u_C3IRSfs", fwiDivC_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//Bugs inside the function call
		RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "5 3 1 128 7 0", 0 );
		RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "2 2 0  64 4 0", 1 );
    }
};

class TestDivC_8u_AC4IRSfs : public PValSDstRoiScale< U8, 4>
{
public:
    TestDivC_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< U8, 4>( parent, "fwiDivC_8u_AC4IRSfs", fwiDivC_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "5 3 1 222 128 7 0 222", 0 );
		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "2 2 0 222  64 4 0 222", 1 );
    }
};

//Missing function
//class TestDivC_8u_C4IRSfs : public PValSDstRoiScale< U8, 4>
//{
//public:
//    TestDivC_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoiScale( parent, "fwiDivC_8u_C4IRSfs", fwiDivC_8u_C4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "5 3 1 2 128 7 0 2", 0 );
//		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "2 2 0 1  64 4 0 1", 1 );
//    }
//};

class TestDivC_16s_C3IRSfs : public PValSDstRoiScale< S16, 3>
{
public:
    TestDivC_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< S16, 3>( parent, "fwiDivC_16s_C3IRSfs", fwiDivC_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
		RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "5 3 1 16384 7 0", 0 );
		RunTest("2 3 MAX", "10 9 MAX MAX 22 50", "2 2 0  8192 4 0", 1 );
    }
};

class TestDivC_16s_AC4IRSfs : public PValSDstRoiScale< S16, 4>
{
public:
    TestDivC_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< S16, 4>( parent, "fwiDivC_16s_AC4IRSfs", fwiDivC_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "5 3 1 222 16384 7 0 222", 0 );
		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "2 2 0 222  8192 4 0 222", 1 );
    }
};

//Missing function
//class TestDivC_16s_C4IRSfs : public PValSDstRoiScale< S16, 4>
//{
//public:
//    TestDivC_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoiScale( parent, "fwiDivC_16s_C4IRSfs", fwiDivC_16s_C4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "5 3 1 2 16384 7 0 0 2", 0 );
//		RunTest("2 3 MAX 111", "10 9 MAX 222 MAX 22 50 222", "2 2 0 1  8192 4 0 0 1", 1 );
//    }
//};

class TestDivC_16sc_C3IRSfs : public PValSDstRoiScale< Fw16sc, 3>
{
public:
    TestDivC_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw16sc, 3>( parent, "fwiDivC_16sc_C3IRSfs", fwiDivC_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
		//Bugs inside the function
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-22 17 -13106 6555 -9175 1311", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", "-11  8  -6553 3277 -4588  655", 1 );
    }
};

class TestDivC_16sc_AC4IRSfs : public PValSDstRoiScale< Fw16sc, 4>
{
public:
    TestDivC_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw16sc, 4>( parent, "fwiDivC_16sc_AC4IRSfs", fwiDivC_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4 2 2", "7 100 3 MAX MIN MIN 222 222", 
			"-22 17 -13106 6555 -9175 1311 222 222", 0 );
		RunTest( "2 -3 1 -2 3 4 2 2", "7 100 3 MAX MIN MIN 222 222", 
			"-11  8  -6553 3277 -4588  655 222 222", 1 );
    }
};

class TestDivC_32sc_C3IRSfs : public PValSDstRoiScale< Fw32sc, 3>
{
public:
    TestDivC_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw32sc, 3>( parent, "fwiDivC_32sc_C3IRSfs", fwiDivC_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN",  
			"-22 17 -858993458 429496731 -601295421 85899346", 0 );
		RunTest( "2 -3 1 -2 3 4", "7 100 3 MAX MIN MIN", 
			"-11  8 -429496729 214748365 -300647711 42949673", 1 );
    }
};

class TestDivC_32sc_AC4IRSfs : public PValSDstRoiScale< Fw32sc, 4>
{
public:
    TestDivC_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw32sc, 4>( parent, "fwiDivC_32sc_AC4IRSfs", fwiDivC_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
		//Bugs in the fourth channel data
        RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-22 17  -858993458 429496731 -601295421 85899346 222 222", 0 );
		RunTest( "2 -3 1 -2 3 4 111 111", "7 100 3 MAX MIN MIN 222 222", 
			"-11  8  -429496729 214748365 -300647711 42949673 222 222", 1 );
    }
};

//Case 7: In-place operation on 1-channel floating-point or complex data.
class TestDivC_32f_C1IR : public ValSDstRoi< F32, 1 >
{
public:
    TestDivC_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< F32, 1 >( parent, "fwiDivC_32f_C1IR", fwiDivC_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( " 2", "7 -100 3 -0.12345", "3.5 -50 1.5 -0.061725", 0.0001f );
		RunTest( "-3", "MAX MIN 2.124e+38", "-1.134273e+38 1.134273e+38 -7.08e+37", 1.0e+33f );
    }
};

class TestDivC_32fc_C1IR : public ValSDstRoi< Fw32fc, 1 >
{
public:
    TestDivC_32fc_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< Fw32fc, 1 >( parent, "fwiDivC_32fc_C1IR", fwiDivC_32fc_C1IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3", "7 100 3 4", "-22 17 -0.461538 1.30769", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "3 1", "MAX MIN MIN MIN", "6.80565e+37 -1.36113e+38 -1.36113e+38 -6.80565e+37", errorMargin );
    }
};

//Case 8: In-place operation on multi-channel floating-point or complex data.
class TestDivC_32f_C3IR : public PValSDstRoi< F32, 3>
{
public:
    TestDivC_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< F32, 3>( parent, "fwiDivC_32f_C3IR", fwiDivC_32f_C3IR )
    {}

    virtual void RunAll()
    {
		//Functions make mistake on duplicating the first channel data of C for all channels
        RunTest( "2 -3 5.1", "7 100 0.12345 3.14159 2.6535 8.9793", 
			"3.5 -33.3333 0.0242059 1.570795 -0.8845 1.760647", 0.0001f );
		RunTest( "3 1 -0.5", "MAX MIN MAX INF 1.5123e+37 1.5123e+37", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF 1.5123e+37 -3.0246e+37", 1.0e+33f );
    }
};

class TestDivC_32f_AC4IR : public PValSDstRoi< F32, 4>
{
public:
    TestDivC_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< F32, 4>( parent, "fwiDivC_32f_AC4IR", fwiDivC_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 -3 5.1 INF", "7 100 0.12345 INF 3.14159 2.6535 8.9793 INF", 
			"3.5 -33.3333 0.0242059 INF 1.570795 -0.8845 1.760647 INF", 0.0001f );
		RunTest( "3 1 -0.5 INF", "MAX MIN MAX INF INF 1.5123e+37 1.5123e+37 INF", 
			"1.134273e+38 -3.40282e+38 INF_NEG INF INF 1.5123e+37 -3.0246e+37 INF", 1.0e+33f );
    }
};

//Missing function
//class TestDivC_32f_C4IR : public PValSDstRoi< F32, 4>
//{
//public:
//    TestDivC_32f_C4IR( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoi( parent, "fwiDivC_32f_C4IR", fwiDivC_32f_C4IR )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "2 -3 5.1 111", "7 100 0.12345 222 3.14159 2.6535 8.9793 222", 
//			"3.5 -33.3333 0.0242059 2 1.570795 -0.8845 1.760647 2", 0.0001f );
//		  RunTest( "3 1 -0.5 111", "MAX MIN MAX 222 INF 1.5123e+37 1.5123e+37 222", 
//			"1.134273e+38 -3.40282e+38 INF_NEG 2 INF 1.5123e+37 -3.0246e+37 2", 1.0e+33f );
//    }
//};

class TestDivC_32fc_C3IR : public PValSDstRoi< Fw32fc, 3>
{
public:
    TestDivC_32fc_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< Fw32fc, 3>( parent, "fwiDivC_32fc_C3IR", fwiDivC_32fc_C3IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "2 -3 2 -3 3 4", "7 100 3 4 5 2", "-22 17 -0.461538 1.30769 0.92 -0.56", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "3 1 -5 -2 3 4", "MAX MIN MAX MIN MAX MIN", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37", errorMargin );
    }
};

class TestDivC_32fc_AC4IR : public PValSDstRoi< Fw32fc, 4>
{
public:
    TestDivC_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< Fw32fc, 4>( parent, "fwiDivC_32fc_AC4IR", fwiDivC_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
		//Bugs in the function, results are NOT expected
        RunTest( "2 -3 2 -3 3 4 INF INF", "7 100 3 4 5 2 INF INF", 
			"-22 17 -0.461538 1.30769 0.92 -0.56 INF INF", errorMargin );
		errorMargin.re = 1.0e+33f;
		errorMargin.im = 1.0e+33f;
		RunTest( "3 1 -5 -2 3 4 INF INF", "MAX MIN MAX MIN MAX MIN INF INF", 
			"6.80565e+37 -1.36113e+38 -3.52016e+37 8.21371e+37 -1.36113e+37 -9.527896e+37 INF INF", errorMargin );
    }
};

DEFINE_TEST_TABLE( DivCTestCatalog )
//Case 1
TEST_ENTRY( TestDivC_8u_C1RSfs )
TEST_ENTRY( TestDivC_16s_C1RSfs )
TEST_ENTRY( TestDivC_16sc_C1RSfs )
TEST_ENTRY( TestDivC_32sc_C1RSfs )
//Case 2
TEST_ENTRY( TestDivC_8u_C3RSfs )
TEST_ENTRY( TestDivC_8u_AC4RSfs )
//TEST_ENTRY( TestDivC_8u_C4RSfs )      //Unsupported
TEST_ENTRY( TestDivC_16s_C3RSfs )
TEST_ENTRY( TestDivC_16s_AC4RSfs )
//TEST_ENTRY( TestDivC_16s_C4RSfs )     //Unsupported
TEST_ENTRY( TestDivC_16sc_C3RSfs )
TEST_ENTRY( TestDivC_16sc_AC4RSfs )
TEST_ENTRY( TestDivC_32sc_C3RSfs )
TEST_ENTRY( TestDivC_32sc_AC4RSfs )
//Case3
TEST_ENTRY( TestDivC_32f_C1R )
TEST_ENTRY( TestDivC_32fc_C1R )
//Case4
TEST_ENTRY( TestDivC_32f_C3R )
TEST_ENTRY( TestDivC_32f_AC4R )
//TEST_ENTRY( TestDivC_32f_C4R )        //Unsupported
TEST_ENTRY( TestDivC_32fc_C3R )
TEST_ENTRY( TestDivC_32fc_AC4R )
//Case5
TEST_ENTRY( TestDivC_8u_C1IRSfs )
TEST_ENTRY( TestDivC_16s_C1IRSfs )
TEST_ENTRY( TestDivC_16sc_C1IRSfs )
TEST_ENTRY( TestDivC_32sc_C1IRSfs )
//Case6
TEST_ENTRY( TestDivC_8u_C3IRSfs )
TEST_ENTRY( TestDivC_8u_AC4IRSfs )
//TEST_ENTRY( TestDivC_8u_C4IRSfs )      //Unsupported
TEST_ENTRY( TestDivC_16s_C3IRSfs )
TEST_ENTRY( TestDivC_16s_AC4IRSfs )
//TEST_ENTRY( TestDivC_16s_C4IRSfs )     //Unsupported
TEST_ENTRY( TestDivC_16sc_C3IRSfs )
TEST_ENTRY( TestDivC_16sc_AC4IRSfs )
TEST_ENTRY( TestDivC_32sc_C3IRSfs )
TEST_ENTRY( TestDivC_32sc_AC4IRSfs )
//Case7
TEST_ENTRY( TestDivC_32f_C1IR )
TEST_ENTRY( TestDivC_32fc_C1IR )
//Case8
TEST_ENTRY( TestDivC_32f_C3IR )
TEST_ENTRY( TestDivC_32f_AC4IR )
//TEST_ENTRY( TestDivC_32f_C4IR )       //Unsupported
TEST_ENTRY( TestDivC_32fc_C3IR )
TEST_ENTRY( TestDivC_32fc_AC4IR )

END_TEST_TABLE()
