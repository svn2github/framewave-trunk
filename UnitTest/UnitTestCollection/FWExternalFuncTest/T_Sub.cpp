/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on integer or complex data
class TestSub_8u_C1RSfs : public SrcSrcDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestSub_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 1, U8, 1 >( parent, "fwiSub_8u_C1RSfs", fwiSub_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8", "7 100 3 MAX", "2 94 0 247", 0 );
		RunTest( "5 6 7 8", "7 100 3 MAX", "1 47 0 124", 1 );
    }
};

class TestSub_8u_C3RSfs : public SrcSrcDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestSub_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 3, U8, 3 >( parent, "fwiSub_8u_C3RSfs", fwiSub_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 8 MAX", "MAX 7 100 3 MAX 0", "255 2 94 0 247 0", 0 );
		RunTest( "0 5 6 7 8 MAX", "MAX 7 100 3 MAX 0", "128 1 47 0 124 0", 1 );
    }
};

class TestSub_8u_C4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSub_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSub_8u_C4RSfs", fwiSub_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "255 2 94 116 117 0 247 0", 0 );
		RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "128 1 47  58  58 0 124 0", 1 ); //Nearest-to-the-even rounding
    }
};

class TestSub_8u_AC4RSfs : public SrcSrcDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSub_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSub_8u_AC4RSfs", fwiSub_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "255 2 94 0 117 0 247 0", 0 );
		RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "128 1 47 0  58 0 124 0", 1 );//Nearest-to-the-even rounding
    }
};

//class TestSub_16u_C1RSfs : public SrcSrcDstRoiScale< U16, 1, U16, 1 >
//{
//public:
//    TestSub_16u_C1RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale( parent, "fwiSub_16u_C1RSfs", fwiSub_16u_C1RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 0 65519", 0 );
//		  RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 0 32760", 1 );
//    }
//};
//
//class TestSub_16u_C3RSfs : public SrcSrcDstRoiScale< U16, 3, U16, 3 >
//{
//public:
//    TestSub_16u_C3RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale( parent, "fwiSub_16u_C3RSfs", fwiSub_16u_C3RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "65535 2 94 0 65519, 0", 0 );
//		  RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "32768 1 47 0 32760 0", 1 );
//    }
//};

//class TestSub_16u_C4RSfs : public SrcSrcDstRoiScale< U16, 4, U16, 4 >
//{
//public:
//    TestSub_16u_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale( parent, "fwiSub_16u_C4RSfs", fwiSub_16u_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 0 65519", 0 );
//		  RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 0 32760", 1 );
//    }
//};
//

//class TestSub_16u_AC4RSfs : public SrcSrcDstRoiScale< U16, 4, U16, 4 >
//{
//public:
//    TestSub_16u_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcSrcDstRoiScale( parent, "fwiSub_16u_AC4RSfs", fwiSub_16u_AC4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 0 0", 0 );
//		  RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 0 0", 1 );
//    }
//};
//

class TestSub_16s_C1RSfs : public SrcSrcDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestSub_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 1, S16, 1 >( parent, "fwiSub_16s_C1RSfs", fwiSub_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 -4 32751", 0 );
		RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 -2 16376", 1 );
    }
};

class TestSub_16s_C3RSfs : public SrcSrcDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestSub_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 3, S16, 3 >( parent, "fwiSub_16s_C3RSfs", fwiSub_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "32767 2 94 -4 32751 -32767", 0 );
		RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "16384 1 47 -2 16376 -16384", 1 );
    }
};

class TestSub_16s_C4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSub_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSub_16s_C4RSfs", fwiSub_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "32767 2 94 116 117 -4 32751 -32767", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "16384 1 47  58  58 -2 16376 -16384", 1 );
    }
};

class TestSub_16s_AC4RSfs : public SrcSrcDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSub_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSub_16s_AC4RSfs", fwiSub_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "32767 2 94 0 117 -4 32751 0", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "16384 1 47 0  58 -2 16376 0", 1 );
    }
};

class TestSub_16sc_C1RSfs : public SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestSub_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiSub_16sc_C1RSfs", fwiSub_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 -4 32751", 0 );
		RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 -2 16376", 1 );
    }
};

class TestSub_16sc_C3RSfs : public SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestSub_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiSub_16sc_C3RSfs", fwiSub_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "32767 2 94 -4 32751 -32767", 0 );
		RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "16384 1 47 -2 16376 -16384", 1 );
    }
};


class TestSub_16sc_AC4RSfs : public SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestSub_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiSub_16sc_AC4RSfs", fwiSub_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "32767 2 94 116 117 -4 0 0", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "16384 1 47  58  58 -2 0 0", 1 );
    }
};

class TestSub_32sc_C1RSfs : public SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestSub_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiSub_32sc_C1RSfs", fwiSub_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 32", "7 100 3 MAX", "2 94 -4 2147483615", 0 );
		RunTest( "5 6 7 32", "7 100 3 MAX", "1 47 -2 1073741808", 1 );
    }
};

class TestSub_32sc_C3RSfs : public SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestSub_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiSub_32sc_C3RSfs", fwiSub_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 32 MAX", "MAX 7 100 3 MAX 0", "2147483647 2 94 -4 2147483615 -2147483647", 0 );
		RunTest( "0 5 6 7 32 MAX", "MAX 7 100 3 MAX 0", "1073741824 1 47 -2 1073741808 -1073741824", 1 );
    }
};

class TestSub_32sc_AC4RSfs : public SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestSub_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiSub_32sc_AC4RSfs", fwiSub_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 32 MAX", "MAX 7 100 128 130 3 MAX 0", "2147483647 2 94 116 117 -4 0 0", 0 );
		RunTest( "0 5 6 12 13 7 32 MAX", "MAX 7 100 128 130 3 MAX 0", "1073741824 1 47  58  58 -2 0 0", 1 );
    }
};

//Case 2: Not-in-place operation on floating-point or complex data
class TestSub_32f_C1R : public SrcSrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestSub_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 1, F32, 1 >( parent, "fwiSub_32f_C1R", fwiSub_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "8.3279 5.0288 4.1971 6.9399 3.7510",
			"5.18632 2.3753 -4.7822 4.5553 1.1077", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "2.1 94.1 -13.0568 MAX", 0.0001f );
    }
};

class TestSub_32f_C3R : public SrcSrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestSub_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 3, F32, 3 >( parent, "fwiSub_32f_C3R", fwiSub_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", 0.0001f );
    }
};

class TestSub_32f_C4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestSub_32f_C4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiSub_32f_C4R", fwiSub_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", 0.0001f );
    }
};

class TestSub_32f_AC4R : public SrcSrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestSub_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< F32, 4, F32, 4 >( parent, "fwiSub_32f_AC4R", fwiSub_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 0.0002 2.3846 2.6433 MAX 0.0003", "8.3279 5.0288 4.1971 1.00001 6.9399 3.7510 MAX 2.00002",
			"5.18632 2.3753 -4.7822 0 4.5553 1.1077 0 0", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 0.9876 100.78 MAX 0 0.12345", "7.2 100.3 3.4 0.2567 MAX 0 MAX 0.7891", 
			"2.1 94.1 -13.0568 0 MAX MIN MAX 0", 0.0001f );
    }
};

class TestSub_32fc_C1R : public SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestSub_32fc_C1R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiSub_32fc_C1R", fwiSub_32fc_C1R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 5.2072", errorMargin );
		RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "2.1 94.1 -13.0568 MAX", errorMargin );
    }
};

class TestSub_32fc_C3R : public SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestSub_32fc_C3R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiSub_32fc_C3R", fwiSub_32fc_C3R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", errorMargin ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", errorMargin );
    }
};

class TestSub_32fc_AC4R : public SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestSub_32fc_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiSub_32fc_AC4R", fwiSub_32fc_AC4R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 0.0002 2.3846 2.6433 MAX 0.0003", "8.3279 5.0288 4.1971 1.00001 6.9399 3.7510 MAX 2.00002",
			"5.18632 2.3753 -4.7822 0.9998 4.5553 1.1077 0 0", errorMargin ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 0.9876 100.78 MAX 0 0.12345", "7.2 100.3 3.4 0.2567 MAX 0 MAX 0.7891", 
			"2.1 94.1 -13.0568 -0.7309 MAX MIN 0 0", errorMargin );
    }
};

//Case 3: In-place operation on integer or complex data
class TestSub_8u_C1IRSfs : public SrcSDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestSub_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 1, U8, 1 >( parent, "fwiSub_8u_C1IRSfs", fwiSub_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 8", "7 100 3 MAX", "2 94 0 247", 0 );
		RunTest( "5 6 7 8", "7 100 3 MAX", "1 47 0 124", 1 );
    }
};

class TestSub_8u_C3IRSfs : public SrcSDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestSub_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 3, U8, 3 >( parent, "fwiSub_8u_C3IRSfs", fwiSub_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 8 MAX", "MAX 7 100 3 MAX 0", "255 2 94 0 247 0", 0 );
		RunTest( "0 5 6 7 8 MAX", "MAX 7 100 3 MAX 0", "128 1 47 0 124 0", 1 );
    }
};

class TestSub_8u_C4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSub_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSub_8u_C4IRSfs", fwiSub_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "255 2 94 116 117 0 247 0", 0 );
		RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 0", "128 1 47  58  58 0 124 0", 1 ); //Nearest-to-the-even rounding
    }
};

class TestSub_8u_AC4IRSfs : public SrcSDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSub_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSub_8u_AC4IRSfs", fwiSub_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 1", "255 2 94 128 117 0 247 1", 0 );
		RunTest( "0 5 6 12 13 7 8 MAX", "MAX 7 100 128 130 3 MAX 2", "128 1 47 128  58 0 124 2", 1 );//Nearest-to-the-even rounding
    }
};

class TestSub_16s_C1IRSfs : public SrcSDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestSub_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 1, S16, 1 >( parent, "fwiSub_16s_C1IRSfs", fwiSub_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 -4 32751", 0 );
		RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 -2 16376", 1 );
    }
};

class TestSub_16s_C3IRSfs : public SrcSDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestSub_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 3, S16, 3 >( parent, "fwiSub_16s_C3IRSfs", fwiSub_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "32767 2 94 -4 32751 -32767", 0 );
		RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "16384 1 47 -2 16376 -16384", 1 );
    }
};

class TestSub_16s_C4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSub_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSub_16s_C4IRSfs", fwiSub_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "32767 2 94 116 117 -4 32751 -32767", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 0", "16384 1 47  58  58 -2 16376 -16384", 1 );
    }
};

class TestSub_16s_AC4IRSfs : public SrcSDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSub_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSub_16s_AC4IRSfs", fwiSub_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 1", "32767 2 94 128 117 -4 32751 1", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 2", "16384 1 47 128  58 -2 16376 2", 1 );
    }
};

class TestSub_16sc_C1IRSfs : public SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestSub_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiSub_16sc_C1IRSfs", fwiSub_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 16", "7 100 3 MAX", "2 94 -4 32751", 0 );
		RunTest( "5 6 7 16", "7 100 3 MAX", "1 47 -2 16376", 1 );
    }
};

class TestSub_16sc_C3IRSfs : public SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestSub_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiSub_16sc_C3IRSfs", fwiSub_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "32767 2 94 -4 32751 -32767", 0 );
		RunTest( "0 5 6 7 16 MAX", "MAX 7 100 3 MAX 0", "16384 1 47 -2 16376 -16384", 1 );
    }
};

class TestSub_16sc_AC4IRSfs : public SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestSub_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiSub_16sc_AC4IRSfs", fwiSub_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 1", "32767 2 94 116 117 -4 MAX 1", 0 );
		RunTest( "0 5 6 12 13 7 16 MAX", "MAX 7 100 128 130 3 MAX 2", "16384 1 47  58  58 -2 MAX 2", 1 );
    }
};

class TestSub_32sc_C1IRSfs : public SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestSub_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiSub_32sc_C1IRSfs", fwiSub_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 32", "7 100 3 MAX", "2 94 -4 2147483615", 0 );
		RunTest( "5 6 7 32", "7 100 3 MAX", "1 47 -2 1073741808", 1 );
    }
};

class TestSub_32sc_C3IRSfs : public SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestSub_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiSub_32sc_C3IRSfs", fwiSub_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 7 32 MAX", "MAX 7 100 3 MAX 0", "2147483647 2 94 -4 2147483615 -2147483647", 0 );
		RunTest( "0 5 6 7 32 MAX", "MAX 7 100 3 MAX 0", "1073741824 1 47 -2 1073741808 -1073741824", 1 );
    }
};

class TestSub_32sc_AC4IRSfs : public SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestSub_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiSub_32sc_AC4IRSfs", fwiSub_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "0 5 6 12 13 7 32 MAX", "MAX 7 100 128 130 3 MAX 1", "2147483647 2 94 116 117 -4 MAX 1", 0 );
		RunTest( "0 5 6 12 13 7 32 MAX", "MAX 7 100 128 130 3 MAX 2", "1073741824 1 47  58  58 -2 MAX 2", 1 );
    }
};

//Case 4: In-place operation on floating-point or complex data
class TestSub_32f_C1IR : public SrcSDstRoi< F32, 1, F32, 1 >
{
public:
    TestSub_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 1, F32, 1 >( parent, "fwiSub_32f_C1IR", fwiSub_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "8.3279 5.0288 4.1971 6.9399 3.7510",
			"5.18632 2.3753 -4.7822 4.5553 1.1077", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "2.1 94.1 -13.0568 MAX", 0.0001f );
    }
};

class TestSub_32f_C3IR : public SrcSDstRoi< F32, 3, F32, 3 >
{
public:
    TestSub_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 3, F32, 3 >( parent, "fwiSub_32f_C3IR", fwiSub_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", 0.0001f );
	}
};

class TestSub_32f_C4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestSub_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiSub_32f_C4IR", fwiSub_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", 0.0001f );
    }
};

class TestSub_32f_AC4IR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestSub_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiSub_32f_AC4IR", fwiSub_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 0.0002 2.3846 2.6433 MAX 0.0003", "8.3279 5.0288 4.1971 1.00001 6.9399 3.7510 MAX 2.00002",
			"5.18632 2.3753 -4.7822 1.00001 4.5553 1.1077 0 2.00002", 0.0001f ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 0.9876 100.78 MAX 0 0.12345", "7.2 100.3 3.4 0.2567 MAX 0 MAX 0.7891", 
			"2.1 94.1 -13.0568 0.2567 MAX MIN MAX 0.7891", 0.0001f );
    }
};

class TestSub_32fc_C1IR : public SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestSub_32fc_C1IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiSub_32fc_C1IR", fwiSub_32fc_C1IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 3.1415", "8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 5.2072", errorMargin );
		RunTest( "5.1 6.2 16.45678 100.78", "7.2 100.3 3.4 MAX", "2.1 94.1 -13.0568 MAX", errorMargin );
    }
};

class TestSub_32fc_C3IR : public SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestSub_32fc_C3IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiSub_32fc_C3IR", fwiSub_32fc_C3IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 MAX", "8.3279 5.0288 4.1971 6.9399 3.7510 MAX",
			"5.18632 2.3753 -4.7822 4.5553 1.1077 0", errorMargin ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 100.78 MAX 0", "7.2 100.3 3.4 MAX 0 MAX", 
			"2.1 94.1 -13.0568 MAX MIN MAX", errorMargin );
    }
};

class TestSub_32fc_AC4IR : public SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestSub_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
	  SrcSDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiSub_32fc_AC4IR", fwiSub_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 0.0002 2.3846 2.6433 MAX 0.0003", "8.3279 5.0288 4.1971 1.00001 6.9399 3.7510 MAX 2.00002",
			"5.18632 2.3753 -4.7822 0.9998 4.5553 1.1077 MAX 2.00002", errorMargin ); //Pi's digit
		RunTest( "5.1 6.2 16.45678 0.9876 100.78 MAX 0 0.12345", "7.2 100.3 3.4 0.2567 MAX 0 MAX 0.7891", 
			"2.1 94.1 -13.0568 -0.7309 MAX MIN MAX 0.7891", errorMargin );
    }
};

DEFINE_TEST_TABLE( SubTestCatalog )
TEST_ENTRY( TestSub_8u_C1RSfs )
TEST_ENTRY( TestSub_8u_C3RSfs )
TEST_ENTRY( TestSub_8u_C4RSfs )
TEST_ENTRY( TestSub_8u_AC4RSfs )
//TEST_ENTRY( TestSub_16u_C1RSfs ) //unsupported
//TEST_ENTRY( TestSub_16u_C3RSfs ) //unsupported
//TEST_ENTRY( TestSub_16u_C4RSfs ) //unsupported
//TEST_ENTRY( TestSub_16u_AC4RSfs ) //unsupported
TEST_ENTRY( TestSub_16s_C1RSfs )
TEST_ENTRY( TestSub_16s_C3RSfs )
TEST_ENTRY( TestSub_16s_C4RSfs )
TEST_ENTRY( TestSub_16s_AC4RSfs )
TEST_ENTRY( TestSub_16sc_C1RSfs )
TEST_ENTRY( TestSub_16sc_C3RSfs )
TEST_ENTRY( TestSub_16sc_AC4RSfs )
TEST_ENTRY( TestSub_32sc_C1RSfs )
TEST_ENTRY( TestSub_32sc_C3RSfs )
TEST_ENTRY( TestSub_32sc_AC4RSfs )
TEST_ENTRY( TestSub_32f_C1R )
TEST_ENTRY( TestSub_32f_C3R )
TEST_ENTRY( TestSub_32f_C4R )
TEST_ENTRY( TestSub_32f_AC4R )
TEST_ENTRY( TestSub_32fc_C1R )
TEST_ENTRY( TestSub_32fc_C3R )
TEST_ENTRY( TestSub_32fc_AC4R )
TEST_ENTRY( TestSub_8u_C1IRSfs )
TEST_ENTRY( TestSub_8u_C3IRSfs )
TEST_ENTRY( TestSub_8u_C4IRSfs )
TEST_ENTRY( TestSub_8u_AC4IRSfs )
//TEST_ENTRY( TestSub_16u_C1IRSfs )//unsupported
//TEST_ENTRY( TestSub_16u_C3IRSfs )//unsupported
//TEST_ENTRY( TestSub_16u_C4IRSfs )//unsupported
//TEST_ENTRY( TestSub_16u_AC4IRSfs )//unsupported
TEST_ENTRY( TestSub_16s_C1IRSfs )
TEST_ENTRY( TestSub_16s_C3IRSfs )
TEST_ENTRY( TestSub_16s_C4IRSfs )
TEST_ENTRY( TestSub_16s_AC4IRSfs )
TEST_ENTRY( TestSub_16sc_C1IRSfs )
TEST_ENTRY( TestSub_16sc_C3IRSfs )
TEST_ENTRY( TestSub_16sc_AC4IRSfs )
TEST_ENTRY( TestSub_32sc_C1IRSfs )
TEST_ENTRY( TestSub_32sc_C3IRSfs )
TEST_ENTRY( TestSub_32sc_AC4IRSfs )
TEST_ENTRY( TestSub_32f_C1IR )
TEST_ENTRY( TestSub_32f_C3IR )
TEST_ENTRY( TestSub_32f_C4IR )
TEST_ENTRY( TestSub_32f_AC4IR )
TEST_ENTRY( TestSub_32fc_C1IR )
TEST_ENTRY( TestSub_32fc_C3IR )
TEST_ENTRY( TestSub_32fc_AC4IR )
END_TEST_TABLE()
