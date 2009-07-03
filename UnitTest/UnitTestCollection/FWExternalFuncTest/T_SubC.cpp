/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel integer or complex data
class TestSubC_8u_C1RSfs : public SrcValDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestSubC_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< U8, 1, U8, 1 >( parent, "fwiSubC_8u_C1RSfs", fwiSubC_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 MAX", "1", "4 5 6 254", 0 );
		RunTest( "5 6 7 MAX", "1", "2 2 3 127", 1 );
    }
};
//
//class TestSubC_16u_C1RSfs : public SrcValDstRoiScale< U16, 1, U16, 1 >
//{
//public:
//    TestSubC_16u_C1RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcValDstRoiScale( parent, "fwiSubC_16u_C1RSfs", fwiSubC_16u_C1RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 6 7 MAX", "1", "4 5 6 65534", 0 );
//		RunTest( "5 6 7 MAX", "1", "2 2 3 32767", 1 );
//    }
//};

class TestSubC_16s_C1RSfs : public SrcValDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestSubC_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< S16, 1, S16, 1 >( parent, "fwiSubC_16s_C1RSfs", fwiSubC_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 MAX MIN 0", "1", "4 5 6 32766 -32768 -1", 0 );
		RunTest( "5 6 7 MAX MIN 0", "1", "2 2 3 16383 -16384  0", 1 );
    }
};

class TestSubC_16sc_C1RSfs : public SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestSubC_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiSubC_16sc_C1RSfs", fwiSubC_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 7 MAX MIN 0", "1 2", "4 5 6 32765 -32768 -2", 0 );
		RunTest( "5 7 7 MAX MIN 0", "1 2", "2 2 3 16382 -16384 -1", 1 );
    }
};

class TestSubC_32sc_C1RSfs : public SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestSubC_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiSubC_32sc_C1RSfs", fwiSubC_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 7 MAX MIN 0", "1 2", "4 5 6 2147483645 -2147483648 -2", 0 );
		RunTest( "5 7 7 MAX MIN 0", "1 2", "2 2 3 1073741822 -1073741824 -1", 1 );
    }
};

//Case 2: Not-in-place operation on multi-channel integer or complex data
class TestSubC_8u_C3RSfs : public SrcPValDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestSubC_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< U8, 3, U8, 3 >( parent, "fwiSubC_8u_C3RSfs", fwiSubC_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX 0 102", "1 2 3", "4 5 6 254 0 99", 0 );
		RunTest( "5 7 9 MAX 0 102", "1 2 3", "2 2 3 127 0 50", 1 );
    }
};

class TestSubC_8u_AC4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSubC_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSubC_8u_AC4RSfs", fwiSubC_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 MAX 0 102 189", "1 2 3", "4 5 6 0 254 0 99 0", 0 );
		RunTest( "5 7 9 156 MAX 0 102 187", "1 2 3", "2 2 3 0 127 0 50 0", 1 );
    }
};

class TestSubC_8u_C4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestSubC_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiSubC_8u_C4RSfs", fwiSubC_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 MAX 0 102 180", "1 2 3 4", "4 5 6 119 254 0 99 176", 0 );
		RunTest( "5 7 9 123 MAX 0 102 180", "1 2 3 4", "2 2 3  60 127 0 50  88", 1 );
    }
};
//
//class TestSubC_16u_C3RSfs : public SrcPValDstRoiScale< U16, 3, U16, 3 >
//{
//public:
//    TestSubC_16u_C3RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoiScale( parent, "fwiSubC_16u_C3RSfs", fwiSubC_16u_C3RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 7 9 MAX 0 102", "1 2 3", "4 5 6 65534 0 99", 0 );
//		RunTest( "5 7 9 MAX 0 102", "1 2 3", "2 2 3 32767 0 50", 1 );
//    }
//};
//
//class TestSubC_16u_AC4RSfs : public SrcPValDstRoiScale< U16, 4, U16, 4 >
//{
//public:
//    TestSubC_16u_AC4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoiScale( parent, "fwiSubC_16u_AC4RSfs", fwiSubC_16u_AC4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 7 9 123 MAX 0 102 1169", "1 2 3", "4 5 6 0 65534 0 99 0", 0 );
//		RunTest( "5 7 9 156 MAX 0 102 1167", "1 2 3", "2 2 3 0 32767 0 50 0", 1 );
//    }
//};
//
//class TestSubC_16u_C4RSfs : public SrcPValDstRoiScale< U16, 4, U16, 4 >
//{
//public:
//    TestSubC_16u_C4RSfs( UnitTestCatalogBase & parent ) : 
//	  SrcPValDstRoiScale( parent, "fwiSubC_16u_C4RSfs", fwiSubC_16u_C4RSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "5 7 9 123 MAX 0 102 1160", "1 2 3 4", "4 5 6 119 65534 0 99 176", 0 );
//		RunTest( "5 7 9 123 MAX 0 102 1160", "1 2 3 4", "2 2 3  60 32767 0 50  88", 1 );
//    }
//};
//

class TestSubC_16s_C3RSfs : public SrcPValDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestSubC_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< S16, 3, S16, 3 >( parent, "fwiSubC_16s_C3RSfs", fwiSubC_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX MIN 102", "1 2 3", "4 5 6 32766 -32768 99", 0 );
		RunTest( "5 7 9 MAX MIN 102", "1 2 3", "2 2 3 16383 -16385 50", 1 );
    }
};

class TestSubC_16s_AC4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSubC_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSubC_16s_AC4RSfs", fwiSubC_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 MAX MIN 102 1169", "1 2 3 4", "4 5 6 0 32766 -32768 99 0", 0 );
		RunTest( "5 7 9 156 MAX MIN 102 1167", "1 2 3 4", "2 2 3 0 16383 -16385 50 0", 1 );
    }
};

class TestSubC_16s_C4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestSubC_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiSubC_16s_C4RSfs", fwiSubC_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 MAX MIN 102 1160", "1 2 3 4", "4 5 6 119 32766 -32768 99 1156", 0 );
		RunTest( "5 7 9 123 MAX MIN 102 1160", "1 2 3 4", "2 2 3  60 16383 -16385 50  578", 1 );
    }
};

class TestSubC_16sc_C3RSfs : public SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestSubC_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiSubC_16sc_C3RSfs", fwiSubC_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX MIN 102", "1 2 3 4 5 6", "4 5 6 32763 -32768 96", 0 );
		RunTest( "5 7 9 MAX MIN 102", "1 2 3 4 5 6", "2 2 3 16382 -16386 48", 1 );
    }
};

class TestSubC_16sc_AC4RSfs : public SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestSubC_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiSubC_16sc_AC4RSfs", fwiSubC_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX MIN 102 888 888", "1 2 3 4 5 6 7 8", "4 5 6 32763 -32768 96 0 0", 0 );
		RunTest( "5 7 9 MAX MIN 102 888 888", "1 2 3 4 5 6 7 8", "2 2 3 16382 -16386 48 0 0", 1 );
    }
};

class TestSubC_32sc_C3RSfs : public SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestSubC_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiSubC_32sc_C3RSfs", fwiSubC_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX MIN 102", "1 2 3 4 5 6", "4 5 6 2147483643 -2147483648 96", 0 );
		RunTest( "5 7 9 MAX MIN 102", "1 2 3 4 5 6", "2 2 3 1073741822 -1073741826 48", 1 );
    }
};

class TestSubC_32sc_AC4RSfs : public SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestSubC_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiSubC_32sc_AC4RSfs", fwiSubC_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 MAX MIN 102 888 888", "1 2 3 4 5 6 7 8", "4 5 6 2147483643 -2147483648 96 0 0", 0 );
		RunTest( "5 7 9 MAX MIN 102 888 888", "1 2 3 4 5 6 7 8", "2 2 3 1073741822 -1073741826 48 0 0", 1 );
    }
};

//Case 3: Not-in-place operation on 1-channel floating-point or complex data
class TestSubC_32f_C1R : public SrcValDstRoi< F32, 1, F32, 1 >
{
public:
    TestSubC_32f_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< F32, 1, F32, 1 >( parent, "fwiSubC_32f_C1R", fwiSubC_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "1.0002", 
			"2.14137 1.6533 7.9791 1.3844 1.6431", 0.0001f ); //Pi's digit
		RunTest( "7.2 100.3 3.4 MAX MIN 0", "1.0001", 
			"6.1999 99.29999 2.39998 MAX MIN -1.0001", 0.0001f );
    }
};

class TestSubC_32fc_C1R : public SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestSubC_32fc_C1R( UnitTestCatalogBase & parent ) : 
	  SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiSubC_32fc_C1R", fwiSubC_32fc_C1R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0.00021", "1.0002 1.00019", 
			"2.14137 1.6533 7.9791 1.3844 1.6431 -1.0", errorMargin ); //Pi's digit
		RunTest( "7.2 100.3 3.4 MAX MIN 0", "1.0001 2.0001", 
			"6.1999 98.29999 2.39998 MAX MIN -2.0001", errorMargin );
    }
};

//Case 3: Not-in-place operation on 1-channel floating-point or complex data
class TestSubC_32f_C3R : public SrcPValDstRoi< F32, 3, F32, 3 >
{
public:
    TestSubC_32f_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< F32, 3, F32, 3 >( parent, "fwiSubC_32f_C3R", fwiSubC_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279", "1.0002 2.0001 3.0003", 
			"2.14137 0.6534 5.9790 1.3844 0.6432 5.3276", 0.0001f ); //Pi's digit
		RunTest( "7.2 100.3 3.4 MAX MIN 0", "1.0001 2.0002 3.0003", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0003", 0.0001f );
    }
};

class TestSubC_32f_AC4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestSubC_32f_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiSubC_32f_AC4R", fwiSubC_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 888 2.3846 2.6433 8.3279 888", "1.0002 2.0001 3.0003", 
			"2.14137 0.6534 5.9790 0 1.3844 0.6432 5.3276 0", 0.0001f ); //Pi's digit
		RunTest( "7.2 100.3 3.4 888 MAX MIN 0 999", "1.0001 2.0002 3.0003", 
			"6.1999 98.2998 0.39971 0 MAX MIN -3.0003 0", 0.0001f );
    }
};

class TestSubC_32f_C4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestSubC_32f_C4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiSubC_32f_C4R", fwiSubC_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 888 2.3846 2.6433 8.3279 999", "1.0002 2.0001 3.0003 4.0001", 
			"2.14137 0.6534 5.9790 883.9999 1.3844 0.6432 5.3276 994.9999", 0.0001f ); //Pi's digit
		RunTest( "7.2 100.3 3.4 888 MAX MIN 0 999", "1.0001 2.0002 3.0003 0.0001", 
			"6.1999 98.2998 0.39971 887.9999 MAX MIN -3.0003 998.9999", 0.0001f );
    }
};

class TestSubC_32fc_C3R : public SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestSubC_32fc_C3R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiSubC_32fc_C3R", fwiSubC_32fc_C3R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487", 
			"1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
			"2.14137 0.6534 5.9790 1.3845 0.6431 5.3278 7.3277 3.0287 1.1968 5.9398 1.7508 5.3486", 
			errorMargin ); //Pi's digit
		RunTest( "7.2 100.3 3.4 MAX MIN 0 1.1 2.2 3.3 4.4 5.5 -6.6", 
			"1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0001 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001", 
			errorMargin );
    }
};

class TestSubC_32fc_AC4R : public SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestSubC_32fc_AC4R( UnitTestCatalogBase & parent ) : 
	  SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiSubC_32fc_AC4R", fwiSubC_32fc_AC4R )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 1111 2222 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487 8888 9999", 
			"1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
			"2.14137 0.6534 5.9790 1.3845 0.6431 5.3278 0 0 7.3277 3.0287 1.1968 5.9398 1.7508 5.3486 0 0", 
			errorMargin ); //Pi's digit
		RunTest( "7.2 100.3 3.4 MAX MIN 0 1111 2222 1.1 2.2 3.3 4.4 5.5 -6.6 8888 9999", 
			"1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0001 0 0 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001 0 0", 
			errorMargin );
    }
};

//Case 5: In-place operation on 1-channel integer or complex data
class TestSubC_8u_C1IRSfs : public ValSDstRoiScale< U8, 1 >
{
public:
    TestSubC_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< U8, 1 >( parent, "fwiSubC_8u_C1IRSfs", fwiSubC_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "5 6 7 MAX", "4 5 6 254", 0 );
		RunTest( "1", "5 6 7 MAX", "2 2 3 127", 1 );
    }
};

//Missing function
//class TestSubC_16u_C1IRSfs : public ValSDstRoiScale< U16, 1 >
//{
//public:
//    TestSubC_16u_C1IRSfs( UnitTestCatalogBase & parent ) : 
//	  ValSDstRoiScale( parent, "fwiSubC_16u_C1IRSfs", fwiSubC_16u_C1IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "1", "5 6 7 MAX", "4 5 6 65534", 0 );
//		RunTest( "1", "5 6 7 MAX", "2 2 3 32767", 1 );
//    }
//};
//

class TestSubC_16s_C1IRSfs : public ValSDstRoiScale< S16, 1 >
{
public:
    TestSubC_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< S16, 1 >( parent, "fwiSubC_16s_C1IRSfs", fwiSubC_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "5 6 7 MAX MIN 0", "4 5 6 32766 MIN -1", 0 );
		RunTest( "3", "5 6 7 MAX MIN 0", "1 2 2 16382 -16386 -2", 1 );
    }
};

class TestSubC_16sc_C1IRSfs : public ValSDstRoiScale< Fw16sc, 1 >
{
public:
    TestSubC_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< Fw16sc, 1 >( parent, "fwiSubC_16sc_C1IRSfs", fwiSubC_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2", "5 6 7 MAX MIN 0", "4 4 6 32765 MIN -2", 0 );
		RunTest( "3 5", "5 6 7 MAX MIN 0", "1 0 2 16381 -16386 -2", 1 );
    }
};

class TestSubC_32sc_C1IRSfs : public ValSDstRoiScale< Fw32sc, 1 >
{
public:
    TestSubC_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
	  ValSDstRoiScale< Fw32sc, 1 >( parent, "fwiSubC_32sc_C1IRSfs", fwiSubC_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2", "5 6 7 MAX MIN 0", "4 4 6 2147483645 MIN -2", 0 );
		RunTest( "3 5", "5 6 7 MAX MIN 0", "1 0 2 1073741821 -1073741826 -2", 1 );
    }
};

//Case 6: In-place operation on multi-channel integer or complex data
class TestSubC_8u_C3IRSfs : public PValSDstRoiScale< U8, 3 >
{
public:
    TestSubC_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< U8, 3 >( parent, "fwiSubC_8u_C3IRSfs", fwiSubC_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 MAX 0 102", "4 5 6 254 0 99", 0 );
		RunTest( "1 2 3", "5 7 9 MAX 0 102", "2 2 3 127 0 50", 1 );
    }
};

class TestSubC_8u_AC4IRSfs : public PValSDstRoiScale< U8, 4 >
{
public:
    TestSubC_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< U8, 4 >( parent, "fwiSubC_8u_AC4IRSfs", fwiSubC_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 111", "4 5 6 111 254 0 99 111", 0 );
		RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 111", "2 2 3 111 127 0 50 111", 1 );
    }
};

class TestSubC_8u_C4IRSfs : public PValSDstRoiScale< U8, 4 >
{
public:
    TestSubC_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< U8, 4 >( parent, "fwiSubC_8u_C4IRSfs", fwiSubC_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 222", "4 5 6 107 254 0 99 218", 0 );
		RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 222", "2 2 3  54 127 0 50 109", 1 );
    }
};

//Missing function
//class TestSubC_16u_C3IRSfs : public PValSDstRoiScale< U16, 3 >
//{
//public:
//    TestSubC_16u_C3IRSfs( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoiScale( parent, "fwiSubC_16u_C3IRSfs", fwiSubC_16u_C3IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "1 2 3", "5 7 9 MAX 0 102", "4 5 6 65534 0 99", 0 );
//		RunTest( "1 2 3", "5 7 9 MAX 0 102", "2 2 3 32767 0 50", 1 );
//    }
//};
//
//class TestSubC_16u_AC4IRSfs : public PValSDstRoiScale< U16, 4 >
//{
//public:
//    TestSubC_16u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoiScale( parent, "fwiSubC_16u_AC4IRSfs", fwiSubC_16u_AC4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "1 2 3", "5 7 9 111 MAX 0 102 222", "4 5 6 111 65534 0 99 222", 0 );
//		RunTest( "1 2 3", "5 7 9 111 MAX 0 102 222", "2 2 3 111 32767 0 50 222", 1 );
//    }
//};
//
//class TestSubC_16u_C4IRSfs : public PValSDstRoiScale< U16, 4 >
//{
//public:
//    TestSubC_16u_C4IRSfs( UnitTestCatalogBase & parent ) : 
//	  PValSDstRoiScale( parent, "fwiSubC_16u_C4IRSfs", fwiSubC_16u_C4IRSfs )
//    {}
//
//    virtual void RunAll()
//    {
//        RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 222", "4 5 6 107 65534 0 99 218", 0 );
//		RunTest( "1 2 3 4", "5 7 9 111 MAX 0 102 222", "2 2 3  54 32767 0 50 109", 1 );
//    }
//};

class TestSubC_16s_C3IRSfs : public PValSDstRoiScale< S16, 3 >
{
public:
    TestSubC_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< S16, 3 >( parent, "fwiSubC_16s_C3IRSfs", fwiSubC_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 MAX 0 MIN", "4 5 6 32766 -2 MIN", 0 );
		RunTest( "1 2 3", "5 7 9 MAX 0 MIN", "2 2 3 16383 -1 -16386", 1 );
    }
};

class TestSubC_16s_AC4IRSfs : public PValSDstRoiScale< S16, 4 >
{
public:
    TestSubC_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< S16, 4 >( parent, "fwiSubC_16s_AC4IRSfs", fwiSubC_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 MAX 0 MIN 222", "4 5 6 111 32766 -2 MIN 222", 0 );
		RunTest( "1 2 3 4", "5 7 9 111 MAX 0 MIN 222", "2 2 3 111 16383 -1 -16386 222", 1 );
    }
};

class TestSubC_16s_C4IRSfs : public PValSDstRoiScale< S16, 4 >
{
public:
    TestSubC_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< S16, 4 >( parent, "fwiSubC_16s_C4IRSfs", fwiSubC_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 MAX 0 MIN 222", "4 5 6 107 32766 -2 MIN 218", 0 );
		RunTest( "1 2 3 4", "5 7 9 111 MAX 0 MIN 222", "2 2 3  54 16383 -1 -16386 109", 1 );
    }
};

class TestSubC_16sc_C3IRSfs : public PValSDstRoiScale< Fw16sc, 3 >
{
public:
    TestSubC_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw16sc, 3 >( parent, "fwiSubC_16sc_C3IRSfs", fwiSubC_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 MAX 0 MIN 15 17 19 123 0 456", 
			"4 5 6 32763 -5 MIN 14 15 16 119 -5 450", 0 );
		RunTest( "1 2 3 4 5 6", "5 7 9 MAX 0 MIN 15 17 19 123 0 456", 
			"2 2 3 16382 -2 -16387 7 8 8 60 -2 225", 1 );
    }
};

class TestSubC_16sc_AC4IRSfs : public PValSDstRoiScale< Fw16sc, 4 >
{
public:
    TestSubC_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw16sc, 4 >( parent, "fwiSubC_16sc_AC4IRSfs", fwiSubC_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6 7 8", "5 7 9 MAX 0 MIN 111 222 15 17 19 123 0 456 888 999", 
			"4 5 6 32763 -5 MIN 111 222 14 15 16 119 -5 450 888 999", 0 );
		RunTest( "1 2 3 4 5 6 7 8", "5 7 9 MAX 0 MIN 111 222 15 17 19 123 0 456 888 999", 
			"2 2 3 16382 -2 -16387 111 222 7 8 8 60 -2 225 888 999 ", 1 );
    }
};

class TestSubC_32sc_C3IRSfs : public PValSDstRoiScale< Fw32sc, 3 >
{
public:
    TestSubC_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw32sc, 3 >( parent, "fwiSubC_32sc_C3IRSfs", fwiSubC_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 MAX 0 MIN 15 17 19 123 0 456", 
			"4 5 6 2147483643 -5 MIN 14 15 16 119 -5 450", 0 );
		RunTest( "1 2 3 4 5 6", "5 7 9 MAX 0 MIN 15 17 19 123 0 456", 
			"2 2 3 1073741822 -2 -1073741827 7 8 8 60 -2 225", 1 );
    }
};

class TestSubC_32sc_AC4IRSfs : public PValSDstRoiScale< Fw32sc, 4 >
{
public:
    TestSubC_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
	  PValSDstRoiScale< Fw32sc, 4 >( parent, "fwiSubC_32sc_AC4IRSfs", fwiSubC_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6 7 8", "5 7 9 MAX 0 MIN 1111 2222 15 17 19 123 0 456 1111 2222", 
			"4 5 6 2147483643 -5 MIN 1111 2222 14 15 16 119 -5 450 1111 2222", 0 );
		RunTest( "1 2 3 4 5 6 7 8", "5 7 9 MAX 0 MIN 1111 2222 15 17 19 123 0 456 1111 2222", 
			"2 2 3 1073741822 -2 -1073741827 1111 2222 7 8 8 60 -2 225 1111 2222", 1 );
    }
};

//Case 7: In-place operation on 1-channel floating-point or complex data
class TestSubC_32f_C1IR : public ValSDstRoi< F32, 1>
{
public:
    TestSubC_32f_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< F32, 1>( parent, "fwiSubC_32f_C1IR", fwiSubC_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002", "3.14159 2.6535 8.9793 2.3846 2.6433", 
			"2.14137 1.6533 7.9791 1.3844 1.6431", 0.0001f ); //Pi's digit
		RunTest( "1.0001", "7.2 100.3 3.4 MAX MIN 0",  
			"6.1999 99.29999 2.39998 MAX MIN -1.0001", 0.0001f );
    }
};

class TestSubC_32fc_C1IR : public ValSDstRoi< Fw32fc, 1 >
{
public:
    TestSubC_32fc_C1IR( UnitTestCatalogBase & parent ) : 
	  ValSDstRoi< Fw32fc, 1 >( parent, "fwiSubC_32fc_C1IR", fwiSubC_32fc_C1IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "1.0002 1.00019", "3.14159 2.6535 8.9793 2.3846 2.6433 0.00021", 
			"2.14137 1.6533 7.9791 1.3844 1.6431 -1.0", errorMargin ); //Pi's digit
		RunTest( "1.0001 2.0001", "7.2 100.3 3.4 MAX MIN 0", 
			"6.1999 98.29999 2.39998 MAX MIN -2.0001", errorMargin );
    }
};

//Case 8: In-place operation on multi-channel floating-point or complex data
class TestSubC_32f_C3IR : public PValSDstRoi< F32, 3>
{
public:
    TestSubC_32f_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< F32, 3>( parent, "fwiSubC_32f_C3IR", fwiSubC_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003", "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279", 
			"2.14137 0.6534 5.9790 1.3844 0.6432 5.3276", 0.0001f ); //Pi's digit
		RunTest(  "1.0001 2.0002 3.0003", "7.2 100.3 3.4 MAX MIN 0", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0003", 0.0001f );
    }
};

class TestSubC_32f_AC4IR : public PValSDstRoi< F32, 4>
{
public:
    TestSubC_32f_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< F32, 4>( parent, "fwiSubC_32f_AC4IR", fwiSubC_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003", "3.14159 2.6535 8.9793 1111 2.3846 2.6433 8.3279 8888", 
			"2.14137 0.6534 5.9790  1111 1.3844 0.6432 5.3276 8888", 0.0001f ); //Pi's digit
		RunTest(  "1.0001 2.0002 3.0003", "7.2 100.3 3.4 1111 MAX MIN 0 8888", 
			"6.1999 98.2998 0.39971 1111 MAX MIN -3.0003 8888", 0.0001f );
    }
};

class TestSubC_32f_C4IR : public PValSDstRoi< F32, 4>
{
public:
    TestSubC_32f_C4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< F32, 4>( parent, "fwiSubC_32f_C4IR", fwiSubC_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003 4.0004", "3.14159 2.6535 8.9793 1111 2.3846 2.6433 8.3279 8888", 
			"2.14137 0.6534 5.9790  1106.9996 1.3844 0.6432 5.3276 8883.9996", 0.0001f ); //Pi's digit
		RunTest( "1.0001 2.0002 3.0003 4.0004", "7.2 100.3 3.4 1111 MAX MIN 0 8888", 
			"6.1999 98.2998 0.39971 1106.9996 MAX MIN -3.0003 8883.9996", 0.0001f );
    }
};

class TestSubC_32fc_C3IR : public PValSDstRoi< Fw32fc, 3>
{
public:
    TestSubC_32fc_C3IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< Fw32fc, 3>( parent, "fwiSubC_32fc_C3IR", fwiSubC_32fc_C3IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001",
			"3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",  
			"2.14137 0.6534 5.9790 1.3845 0.6431 5.3278 7.3277 3.0287 1.1968 5.9398 1.7508 5.3486", 
			errorMargin ); //Pi's digit
		RunTest( "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
			"7.2 100.3 3.4 MAX MIN 0 1.1 2.2 3.3 4.4 5.5 -6.6", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0001 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001", 
			errorMargin );
    }
};

class TestSubC_32fc_AC4IR : public PValSDstRoi< Fw32fc, 4>
{
public:
    TestSubC_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
	  PValSDstRoi< Fw32fc, 4>( parent, "fwiSubC_32fc_AC4IR", fwiSubC_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
		Fw32fc errorMargin = {0.0001f, 0.0001f};
        RunTest( "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
			"3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 1111 2222 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487 8888 9999", 
			"2.14137 0.6534 5.9790 1.3845 0.6431 5.3278 1111 2222 7.3277 3.0287 1.1968 5.9398 1.7508 5.3486 8888 9999", 
			errorMargin ); //Pi's digit
		RunTest( "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
			"7.2 100.3 3.4 MAX MIN 0 1111 2222 1.1 2.2 3.3 4.4 5.5 -6.6 8888 9999", 
			"6.1999 98.2998 0.39971 MAX MIN -3.0001 1111 2222 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001 8888 9999", 
			errorMargin );
    }
};

DEFINE_TEST_TABLE( SubCTestCatalog )
TEST_ENTRY( TestSubC_8u_C1RSfs )
//TEST_ENTRY( TestSubC_16u_C1RSfs )   //unsupported
TEST_ENTRY( TestSubC_16s_C1RSfs )
TEST_ENTRY( TestSubC_16sc_C1RSfs )
TEST_ENTRY( TestSubC_32sc_C1RSfs )
TEST_ENTRY( TestSubC_8u_C3RSfs )
TEST_ENTRY( TestSubC_8u_AC4RSfs )
TEST_ENTRY( TestSubC_8u_C4RSfs )
//TEST_ENTRY( TestSubC_16u_C3RSfs )    //unsupported
//TEST_ENTRY( TestSubC_16u_AC4RSfs )   //unsupported
//TEST_ENTRY( TestSubC_16u_C4RSfs )    //unsupported
TEST_ENTRY( TestSubC_16s_C3RSfs )      
TEST_ENTRY( TestSubC_16s_AC4RSfs ) 
TEST_ENTRY( TestSubC_16s_C4RSfs ) 
TEST_ENTRY( TestSubC_16sc_C3RSfs )  
TEST_ENTRY( TestSubC_16sc_AC4RSfs ) 
TEST_ENTRY( TestSubC_32sc_C3RSfs )  
TEST_ENTRY( TestSubC_32sc_AC4RSfs )
TEST_ENTRY( TestSubC_32f_C1R )  
TEST_ENTRY( TestSubC_32fc_C1R )   
TEST_ENTRY( TestSubC_32f_C3R )
TEST_ENTRY( TestSubC_32f_AC4R )
TEST_ENTRY( TestSubC_32f_C4R )
TEST_ENTRY( TestSubC_32fc_C3R )
TEST_ENTRY( TestSubC_32fc_AC4R )
TEST_ENTRY( TestSubC_8u_C1IRSfs )
//TEST_ENTRY( TestSubC_16u_C1IRSfs )   //unsupported
TEST_ENTRY( TestSubC_16s_C1IRSfs ) 
TEST_ENTRY( TestSubC_16sc_C1IRSfs )  
TEST_ENTRY( TestSubC_32sc_C1IRSfs ) 
TEST_ENTRY( TestSubC_8u_C3IRSfs )
TEST_ENTRY( TestSubC_8u_AC4IRSfs )
TEST_ENTRY( TestSubC_8u_C4IRSfs )
//TEST_ENTRY( TestSubC_16u_C3IRSfs )   //unsupported
//TEST_ENTRY( TestSubC_16u_AC4IRSfs )  //unsupported
//TEST_ENTRY( TestSubC_16u_C4IRSfs )   //unsupported
TEST_ENTRY( TestSubC_16s_C3IRSfs )     
TEST_ENTRY( TestSubC_16s_AC4IRSfs )
TEST_ENTRY( TestSubC_16s_C4IRSfs ) 
TEST_ENTRY( TestSubC_16sc_C3IRSfs )
TEST_ENTRY( TestSubC_16sc_AC4IRSfs ) 
TEST_ENTRY( TestSubC_32sc_C3IRSfs ) 
TEST_ENTRY( TestSubC_32sc_AC4IRSfs ) 
TEST_ENTRY( TestSubC_32f_C1IR )  
TEST_ENTRY( TestSubC_32fc_C1IR )   
TEST_ENTRY( TestSubC_32f_C3IR )
TEST_ENTRY( TestSubC_32f_AC4IR )
TEST_ENTRY( TestSubC_32f_C4IR )
TEST_ENTRY( TestSubC_32fc_C3IR )
TEST_ENTRY( TestSubC_32fc_AC4IR )
END_TEST_TABLE()
