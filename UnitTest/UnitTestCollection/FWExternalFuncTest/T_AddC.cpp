/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Case 1: Not-in-place operation on 1-channel integer or complex data
class TestAddC_8u_C1RSfs : public SrcValDstRoiScale< U8, 1, U8, 1 >
{
public:
    TestAddC_8u_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< U8, 1, U8, 1 >( parent, "fwiAddC_8u_C1RSfs", fwiAddC_8u_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 25", "1", "6 7 8 26", 0 );
        RunTest( "5 6 7 25", "1", "3 4 4 13", 1 );
    }
};

class TestAddC_16s_C1RSfs : public SrcValDstRoiScale< S16, 1, S16, 1 >
{
public:
    TestAddC_16s_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< S16, 1, S16, 1 >( parent, "fwiAddC_16s_C1RSfs", fwiAddC_16s_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 6 7 MAX MIN 0", "1", "6 7 8 32767 -32767 1", 0 );
        RunTest( "5 6 7 MAX MIN 0", "1", "3 4 4 16384 -16384 0", 1 );
    }
};

class TestAddC_16sc_C1RSfs : public SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >
{
public:
    TestAddC_16sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< Fw16sc, 1, Fw16sc, 1 >( parent, "fwiAddC_16sc_C1RSfs", fwiAddC_16sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 7 54 67 0", "1 2", "6 9 8 56 68 2", 0 );
        RunTest( "5 7 7 54 67 0", "1 2", "3 4 4 28 34 1", 1 );
    }
};

class TestAddC_32sc_C1RSfs : public SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >
{
public:
    TestAddC_32sc_C1RSfs( UnitTestCatalogBase & parent ) : 
      SrcValDstRoiScale< Fw32sc, 1, Fw32sc, 1 >( parent, "fwiAddC_32sc_C1RSfs", fwiAddC_32sc_C1RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 7 54 67 0", "1 2", "6 9 8 56 68 2", 0 );
        RunTest( "5 7 7 54 67 0", "1 2", "3 4 4 28 34 1", 1 );
    }
};

//Case 2: Not-in-place operation on multi-channel integer or complex data
class TestAddC_8u_C3RSfs : public SrcPValDstRoiScale< U8, 3, U8, 3 >
{
public:
    TestAddC_8u_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 3, U8, 3 >( parent, "fwiAddC_8u_C3RSfs", fwiAddC_8u_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 44 0 102", "1 2 3", "6 9 12 45 2 105", 0 );
        RunTest( "5 7 9 44 0 102", "1 2 3", "3 4 6 22 1 52", 1 );
    }
};

class TestAddC_8u_AC4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAddC_8u_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAddC_8u_AC4RSfs", fwiAddC_8u_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 45 0 102 189", "1 2 3", "6 9 12 0 46 2 105 0", 0 );
        RunTest( "5 7 9 156 45 0 102 189", "1 2 3", "3 4 6 0 23 1 52 0", 1 );
    }
};

class TestAddC_8u_C4RSfs : public SrcPValDstRoiScale< U8, 4, U8, 4 >
{
public:
    TestAddC_8u_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< U8, 4, U8, 4 >( parent, "fwiAddC_8u_C4RSfs", fwiAddC_8u_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 44 0 102 180", "1 2 3 4", "6 9 12 127 45 2 105 184", 0 );
        RunTest( "5 7 9 123 44 0 102 180", "1 2 3 4", "3 4 6 64 22 1 52 92", 1 );
    }
};

class TestAddC_16s_C3RSfs : public SrcPValDstRoiScale< S16, 3, S16, 3 >
{
public:
    TestAddC_16s_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 3, S16, 3 >( parent, "fwiAddC_16s_C3RSfs", fwiAddC_16s_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 11 55 24 103", "1 2 3", "6 9 14 56 26 106", 0 );
        RunTest( "5 7 11 55 24 103", "1 2 3", "3 4 7 28 13 53", 1 );
    }
};

class TestAddC_16s_AC4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAddC_16s_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAddC_16s_AC4RSfs", fwiAddC_16s_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 24 65 102 1169", "1 2 3", "6 9 12 0 25 67 105 0", 0 );
        RunTest( "5 7 9 156 25 65 102 1167", "1 2 3", "3 4 6 0 13 34 52 0", 1 );
    }
};

class TestAddC_16s_C4RSfs : public SrcPValDstRoiScale< S16, 4, S16, 4 >
{
public:
    TestAddC_16s_C4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< S16, 4, S16, 4 >( parent, "fwiAddC_16s_C4RSfs", fwiAddC_16s_C4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 123 24 65 102 1160", "1 2 3 4", "6 9 12 127 25 67 105 1164", 0 );
        RunTest( "5 7 9 123 24 65 102 1160", "1 2 3 4", "3 4 6 64 12 34 52 582", 1 );
    }
};

class TestAddC_16sc_C3RSfs : public SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >
{
public:
    TestAddC_16sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw16sc, 3, Fw16sc, 3 >( parent, "fwiAddC_16sc_C3RSfs", fwiAddC_16sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 24 65 102", "1 2 3 4 5 6", "6 9 12 28 70 108", 0 );
        RunTest( "5 7 9 24 65 102", "1 2 3 4 5 6", "3 4 6 14 35 54", 1 );
    }
};

class TestAddC_16sc_AC4RSfs : public SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >
{
public:
    TestAddC_16sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw16sc, 4, Fw16sc, 4 >( parent, "fwiAddC_16sc_AC4RSfs", fwiAddC_16sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 24 65 102 888 888", "1 2 3 4 5 6", "6 9 12 28 70 108 0 0", 0 );
        RunTest( "5 7 9 24 65 102 888 888", "1 2 3 4 5 6", "3 4 6 14 35 54 0 0", 1 );
    }
};

class TestAddC_32sc_C3RSfs : public SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >
{
public:
    TestAddC_32sc_C3RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw32sc, 3, Fw32sc, 3 >( parent, "fwiAddC_32sc_C3RSfs", fwiAddC_32sc_C3RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 24 65 102", "1 2 3 4 5 6", "6 9 12 28 70 108", 0 );
        RunTest( "5 7 9 24 65 102", "1 2 3 4 5 6", "3 4 6 14 35 54", 1 );
    }
};

class TestAddC_32sc_AC4RSfs : public SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >
{
public:
    TestAddC_32sc_AC4RSfs( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoiScale< Fw32sc, 4, Fw32sc, 4 >( parent, "fwiAddC_32sc_AC4RSfs", fwiAddC_32sc_AC4RSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "5 7 9 24 65 102 888 888", "1 2 3 4 5 6", "6 9 12 28 70 108 0 0", 0 );
        RunTest( "5 7 9 24 65 102 888 888", "1 2 3 4 5 6", "3 4 6 14 35 54 0 0", 1 );
    }
};

//Case 3: Not-in-place operation on 1-channel floating-point or complex data
class TestAddC_32f_C1R : public SrcValDstRoi< F32, 1, F32, 1 >
{
public:
    TestAddC_32f_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< F32, 1, F32, 1 >( parent, "fwiAddC_32f_C1R", fwiAddC_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "1.0002", 
            "4.14179 3.6538 9.9795 3.3848 3.6435", (F32) 0.0001 ); //Pi's digit
        RunTest( "7.2 100.3 3.4 24.001 65.235 0", "1.0001", 
            "8.2001 101.3001 4.4001 25.0011 66.2351 1.0001", (F32) 0.0001 );
    }
};

class TestAddC_32fc_C1R : public SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >
{
public:
    TestAddC_32fc_C1R( UnitTestCatalogBase & parent ) : 
      SrcValDstRoi< Fw32fc, 1, Fw32fc, 1 >( parent, "fwiAddC_32fc_C1R", fwiAddC_32fc_C1R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0.00021", "1.0002 1.00019", 
            "4.14179 3.65379 9.9795 3.38479 3.6435 1.0004", errorMargin ); //Pi's digit
        RunTest( "7.2 100.3 3.4 24.001 65.235 0", "1.0001 2.0001", 
            "8.2001 102.3001 4.4001 26.0011 66.2351 2.0001", errorMargin );
    }
};

//Case 4: Not-in-place operation on multi-channel floating-point or complex data
class TestAddC_32f_C3R : public SrcPValDstRoi< F32, 3, F32, 3 >
{
public:
    TestAddC_32f_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 3, F32, 3 >( parent, "fwiAddC_32f_C3R", fwiAddC_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279", "1.0002 2.0001 3.0003", 
            "4.14179 4.6536 11.9796 3.3848 4.6434 11.3282 ", (F32) 0.0001 ); //Pi's digit
        RunTest( "7.2 100.3 3.4 24.001 65.235 0", "1.0001 2.0002 3.0003", 
            "8.2001 102.3002 6.4003 25.0011 67.2352 3.0003", (F32) 0.0001 );
    }
};

class TestAddC_32f_AC4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestAddC_32f_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiAddC_32f_AC4R", fwiAddC_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 888 2.3846 2.6433 8.3279 888", "1.0002 2.0001 3.0003", 
            "4.14179 4.6536 11.9796 0 3.3848 4.6434 11.3282 0", (F32) 0.0001 ); //Pi's digit
        RunTest( "7.2 100.3 3.4 888 24.0001 65.235 0 999", "1.0001 2.0002 3.0003", 
            "8.2001 102.3002 6.4003 0 25.0002 67.2352 3.0003 0 ", (F32) 0.0001 );
    }
};

class TestAddC_32f_C4R : public SrcPValDstRoi< F32, 4, F32, 4 >
{
public:
    TestAddC_32f_C4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< F32, 4, F32, 4 >( parent, "fwiAddC_32f_C4R", fwiAddC_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 888 2.3846 2.6433 8.3279 999", "1.0002 2.0001 3.0003 4.0001", 
            "4.14179 4.6536 11.9796 892.0001 3.3848 4.6434 11.3282 1003.0001", (F32) 0.0001 ); //Pi's digit
        RunTest( "7.2 100.3 3.4 888 24.001 65.235 0 999", "1.0001 2.0002 3.0003 0.0001", 
            "8.2001 102.3002 6.4003 888.0001 25.0011 67.2352 3.0003 999.0001 ", (F32) 0.0001 );
    }
};

class TestAddC_32fc_C3R : public SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >
{
public:
    TestAddC_32fc_C3R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< Fw32fc, 3, Fw32fc, 3 >( parent, "fwiAddC_32fc_C3R", fwiAddC_32fc_C3R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487", 
            "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
            "4.14179 4.6536 11.9796 3.3847 4.6435 11.328 9.3281 7.0289 7.1974 7.94 5.7512 11.3488 ", 
            errorMargin ); //Pi's digit
        /*RunTest( "7.2 100.3 3.4 MAX MIN 0 1.1 2.2 3.3 4.4 5.5 -6.6", 
            "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
            "6.1999 98.2998 0.39971 MAX MIN -3.0001 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001", 
            errorMargin );*/
    }
};

class TestAddC_32fc_AC4R : public SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >
{
public:
    TestAddC_32fc_AC4R( UnitTestCatalogBase & parent ) : 
      SrcPValDstRoi< Fw32fc, 4, Fw32fc, 4 >( parent, "fwiAddC_32fc_AC4R", fwiAddC_32fc_AC4R )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 1111 2222 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487 8888 9999", 
            "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
            "4.14179 4.6536 11.9796 3.3847 4.6435 11.328 0 0 9.3281 7.0289 7.1974 7.94 5.7512 11.3488 0 0", 
            errorMargin ); //Pi's digit
       /* RunTest( "7.2 100.3 3.4 MAX MIN 0 1111 2222 1.1 2.2 3.3 4.4 5.5 -6.6 8888 9999", 
            "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
            "6.1999 98.2998 0.39971 MAX MIN -3.0001 0 0 0.0999 0.1998 0.2997 3.3999 3.4998 -9.6001 0 0", 
            errorMargin );*/
    }
};

//Case 5: In-place operation on 1-channel integer or complex data
class TestAddC_8u_C1IRSfs : public ValSDstRoiScale< U8, 1 >
{
public:
    TestAddC_8u_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< U8, 1 >( parent, "fwiAddC_8u_C1IRSfs", fwiAddC_8u_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "5 6 7 23", "6 7 8 24 ", 0 );
        RunTest( "1", "5 6 7 23", "3 4 4 12 ", 1 );
    }
};

class TestAddC_16s_C1IRSfs : public ValSDstRoiScale< S16, 1 >
{
public:
    TestAddC_16s_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< S16, 1 >( parent, "fwiAddC_16s_C1IRSfs", fwiAddC_16s_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1", "5 6 7 23 32 0", "6 7 8 24 33 1", 0 );
        RunTest( "1", "5 6 7 23 32 0", "3 4 4 12 16 0", 1 );
    }
};

class TestAddC_16sc_C1IRSfs : public ValSDstRoiScale< Fw16sc, 1 >
{
public:
    TestAddC_16sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< Fw16sc, 1 >( parent, "fwiAddC_16sc_C1IRSfs", fwiAddC_16sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2", "5 6 7 23 45 0", "6 8 8 25 46 2", 0 );
        RunTest( "3 5", "5 6 7 23 45 0", "4 6 5 14 24 2 ", 1 );
    }
};

class TestAddC_32sc_C1IRSfs : public ValSDstRoiScale< Fw32sc, 1 >
{
public:
    TestAddC_32sc_C1IRSfs( UnitTestCatalogBase & parent ) : 
      ValSDstRoiScale< Fw32sc, 1 >( parent, "fwiAddC_32sc_C1IRSfs", fwiAddC_32sc_C1IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2", "5 6 7 23 45 0", "6 8 8 25 46 2", 0 );
        RunTest( "3 5", "5 6 7 23 45 0", "4 6 5 14 24 2", 1 );
    }
};

//Case 6: In-place operation on multi-channel integer or complex data
class TestAddC_8u_C3IRSfs : public PValSDstRoiScale< U8, 3 >
{
public:
    TestAddC_8u_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 3 >( parent, "fwiAddC_8u_C3IRSfs", fwiAddC_8u_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 23 0 102", "6 9 12 24 2 105", 0 );
        RunTest( "1 2 3", "5 7 9 23 0 102", "3 4 6 12 1 52", 1 );
    }
};

class TestAddC_8u_AC4IRSfs : public PValSDstRoiScale< U8, 4 >
{
public:
    TestAddC_8u_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 4 >( parent, "fwiAddC_8u_AC4IRSfs", fwiAddC_8u_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 111 23 0 102 111", "6 9 12 111 24 2 105 111", 0 );
        RunTest( "1 2 3", "5 7 9 111 23 0 102 111", "3 4 6 111 12 1 52 111", 1 );
    }
};

class TestAddC_8u_C4IRSfs : public PValSDstRoiScale< U8, 4 >
{
public:
    TestAddC_8u_C4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< U8, 4 >( parent, "fwiAddC_8u_C4IRSfs", fwiAddC_8u_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 23 0 102 222", "6 9 12 115 24 2 105 226", 0 );
        RunTest( "1 2 3 4", "5 7 9 111 23 0 102 222", "3 4 6 58 12 1 52 113", 1 );
    }
};


class TestAddC_16s_C3IRSfs : public PValSDstRoiScale< S16, 3 >
{
public:
    TestAddC_16s_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 3 >( parent, "fwiAddC_16s_C3IRSfs", fwiAddC_16s_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 23 0 45", "6 9 12 24 2 48", 0 );
        RunTest( "1 2 3", "5 7 9 23 0 45", "3 4 6 12 1 24", 1 );
    }
};

class TestAddC_16s_AC4IRSfs : public PValSDstRoiScale< S16, 4 >
{
public:
    TestAddC_16s_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 4 >( parent, "fwiAddC_16s_AC4IRSfs", fwiAddC_16s_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3", "5 7 9 111 23 0 45 222", "6 9 12 111 24 2 48 222", 0 );
        RunTest( "1 2 3", "5 7 9 111 23 0 45 222", "3 4 6 111 12 1 24 222 ", 1 );
    }
};

class TestAddC_16s_C4IRSfs : public PValSDstRoiScale< S16, 4 >
{
public:
    TestAddC_16s_C4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< S16, 4 >( parent, "fwiAddC_16s_C4IRSfs", fwiAddC_16s_C4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4", "5 7 9 111 23 0 45 222", "6 9 12 115 24 2 48 226", 0 );
        RunTest( "1 2 3 4", "5 7 9 111 23 0 45 222", "3 4 6 58 12 1 24 113 ", 1 );
    }
};

class TestAddC_16sc_C3IRSfs : public PValSDstRoiScale< Fw16sc, 3 >
{
public:
    TestAddC_16sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw16sc, 3 >( parent, "fwiAddC_16sc_C3IRSfs", fwiAddC_16sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 15 17 19 123 0 456", 
            "6 9 12 27 5 51 16 19 22 127 5 462", 0 );
        RunTest( "1 2 3 4 5 6", "5 7 9 2 0 23 15 17 19 123 0 456", 
            "3 4 6 3 2 14 8 10 11 64 2 231", 1 );
    }
};

class TestAddC_16sc_AC4IRSfs : public PValSDstRoiScale< Fw16sc, 4 >
{
public:
    TestAddC_16sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw16sc, 4 >( parent, "fwiAddC_16sc_AC4IRSfs", fwiAddC_16sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 111 222 15 17 19 123 0 456 888 999", 
            "6 9 12 27 5 51 111 222 16 19 22 127 5 462 888 999", 0 );
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 111 222 15 17 19 123 0 456 888 999", 
            "3 4 6 14 2 26 111 222 8 10 11 64 2 231 888 999 ", 1 );
    }
};

class TestAddC_32sc_C3IRSfs : public PValSDstRoiScale< Fw32sc, 3 >
{
public:
    TestAddC_32sc_C3IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw32sc, 3 >( parent, "fwiAddC_32sc_C3IRSfs", fwiAddC_32sc_C3IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 15 17 19 123 0 456", 
            "6 9 12 27 5 51 16 19 22 127 5 462", 0 );
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 15 17 19 123 0 456", 
            "3 4 6 14 2 26 8 10 11 64 2 231", 1 );
    }
};

class TestAddC_32sc_AC4IRSfs : public PValSDstRoiScale< Fw32sc, 4 >
{
public:
    TestAddC_32sc_AC4IRSfs( UnitTestCatalogBase & parent ) : 
      PValSDstRoiScale< Fw32sc, 4 >( parent, "fwiAddC_32sc_AC4IRSfs", fwiAddC_32sc_AC4IRSfs )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 1111 2222 15 17 19 123 0 456 1111 2222", 
            "6 9 12 27 5 51 1111 2222 16 19 22 127 5 462 1111 2222", 0 );
        RunTest( "1 2 3 4 5 6", "5 7 9 23 0 45 1111 2222 15 17 19 123 0 456 1111 2222", 
            "3 4 6 14 2 26 1111 2222 8 10 11 64 2 231 1111 2222", 1 );
    }
};

//Case 7: In-place operation on 1-channel floating-point or complex data
class TestAddC_32f_C1IR : public ValSDstRoi< F32, 1>
{
public:
    TestAddC_32f_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< F32, 1>( parent, "fwiAddC_32f_C1IR", fwiAddC_32f_C1IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002", "3.14159 2.6535 8.9793 2.3846 2.6433", 
            "4.14179 3.6538 9.9795 3.3848 3.6435 ", (F32)0.0001 ); //Pi's digit
        RunTest( "1.0001", "7.2 100.3 3.4 24.001 65.235 0",  
            "8.2001 101.3001 4.4001 25.0011 66.2351 1.0001", (F32)0.0001 );
    }
};

class TestAddC_32fc_C1IR : public ValSDstRoi< Fw32fc, 1 >
{
public:
    TestAddC_32fc_C1IR( UnitTestCatalogBase & parent ) : 
      ValSDstRoi< Fw32fc, 1 >( parent, "fwiAddC_32fc_C1IR", fwiAddC_32fc_C1IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "1.0002 1.00019", "3.14159 2.6535 8.9793 2.3846 2.6433 0.00021", 
            "4.14179 3.65379 9.9795 3.38479 3.6435 1.0004", errorMargin ); //Pi's digit
        RunTest( "1.0001 2.0001", "7.2 100.3 3.4 24.001 65.235 0", 
            "8.2001 102.3001 4.4001 26.0011 66.2351 2.0001", errorMargin );
    }
};

//Case 8: In-place operation on multi-channel floating-point or complex data
class TestAddC_32f_C3IR : public PValSDstRoi< F32, 3>
{
public:
    TestAddC_32f_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 3>( parent, "fwiAddC_32f_C3IR", fwiAddC_32f_C3IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003", "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279", 
            "4.14179 4.6536 11.9796 3.3848 4.6434 11.3282", (F32) 0.0001 ); //Pi's digit
        RunTest(  "1.0001 2.0002 3.0003", "7.2 100.3 3.4 24.001 65.235 0", 
            "8.2001 102.3002 6.4003 25.0011 67.2352 3.0003", (F32) 0.0001 );
    }
};

class TestAddC_32f_AC4IR : public PValSDstRoi< F32, 4>
{
public:
    TestAddC_32f_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 4>( parent, "fwiAddC_32f_AC4IR", fwiAddC_32f_AC4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003", "3.14159 2.6535 8.9793 1111 2.3846 2.6433 8.3279 8888", 
            "4.14179 4.6536 11.9796 1111 3.3848 4.6434 11.3282 8888", (F32) 0.0001 ); //Pi's digit
        RunTest(  "1.0001 2.0002 3.0003", "7.2 100.3 3.4 1111 24.001 65.235 0 8888", 
            "8.2001 102.3002 6.4003 1111 25.0011 67.2352 3.0003 8888", (F32) 0.0001 );
    }
};

class TestAddC_32f_C4IR : public PValSDstRoi< F32, 4>
{
public:
    TestAddC_32f_C4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< F32, 4>( parent, "fwiAddC_32f_C4IR", fwiAddC_32f_C4IR )
    {}

    virtual void RunAll()
    {
        RunTest( "1.0002 2.0001 3.0003 4.0004", "3.14159 2.6535 8.9793 1111 2.3846 2.6433 8.3279 8888", 
            "4.14179 4.6536 11.9796 1115.0004 3.3848 4.6434 11.3282 8892.0004", (F32) 0.0001 ); //Pi's digit
        RunTest( "1.0001 2.0002 3.0003 4.0004", "7.2 100.3 3.4 1111 24.001 65.235 0 8888", 
            "8.2001 102.3002 6.4003 1115.0004 25.0011 67.2352 3.0003 8892.0004", (F32) 0.0001 );
    }
};

class TestAddC_32fc_C3IR : public PValSDstRoi< Fw32fc, 3>
{
public:
    TestAddC_32fc_C3IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< Fw32fc, 3>( parent, "fwiAddC_32fc_C3IR", fwiAddC_32fc_C3IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001",
            "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487",  
            "4.14179 4.6535 11.9796 3.3847 4.6435 11.328 9.3281 7.0289 7.1974 7.94 5.7512 11.3488 ", 
            errorMargin ); //Pi's digit
        RunTest( "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
            "7.2 100.3 3.4 24.001 65.235 0 1.1 2.2 3.3 4.4 5.5 6.6", 
            "8.2001 102.3002 6.4003 25.0011 67.2352 3.0001 2.1001 4.2002 6.3003 5.4001 7.5002 9.6001", 
            errorMargin );
    }
};

class TestAddC_32fc_AC4IR : public PValSDstRoi< Fw32fc, 4>
{
public:
    TestAddC_32fc_AC4IR( UnitTestCatalogBase & parent ) : 
      PValSDstRoi< Fw32fc, 4>( parent, "fwiAddC_32fc_AC4IR", fwiAddC_32fc_AC4IR )
    {}

    virtual void RunAll()
    {
        Fw32fc errorMargin = {(F32)0.0001, (F32)0.0001};
        RunTest( "1.0002 2.0001 3.0003 1.0001 2.0002 3.0001", 
            "3.14159 2.6535 8.9793 2.3846 2.6433 8.3279 1111 2222 8.3279 5.0288 4.1971 6.9399 3.7510 8.3487 8888 9999", 
            "4.14179 4.6535 11.9796 3.3847 4.6435 11.328 1111 2222 9.3281 7.0289 7.1974 7.94 5.7512 11.3488 8888 9999", 
            errorMargin ); //Pi's digit
        RunTest( "1.0001 2.0002 3.0003 1.0001 2.0002 3.0001", 
            "7.2 100.3 3.4 24.001 65.235 0 1111 2222 1.1 2.2 3.3 4.4 5.5 6.6 8888 9999", 
            "8.2001 102.3002 6.4003 25.0011 67.2352 3.0001 1111 2222 2.1001 4.2002 6.3003 5.4001 7.5002 9.6001 8888 9999", 
            errorMargin );
    }
};







DEFINE_TEST_TABLE( AddCTestCatalog )
TEST_ENTRY( TestAddC_8u_C1RSfs )
TEST_ENTRY( TestAddC_16s_C1RSfs )
TEST_ENTRY( TestAddC_16sc_C1RSfs )
TEST_ENTRY( TestAddC_32sc_C1RSfs )

TEST_ENTRY( TestAddC_8u_C3RSfs )
TEST_ENTRY( TestAddC_8u_AC4RSfs )
TEST_ENTRY( TestAddC_8u_C4RSfs )
TEST_ENTRY( TestAddC_16s_C3RSfs ) 
TEST_ENTRY( TestAddC_16s_AC4RSfs ) 
TEST_ENTRY( TestAddC_16s_C4RSfs )  
TEST_ENTRY( TestAddC_16sc_C3RSfs ) 
TEST_ENTRY( TestAddC_16sc_AC4RSfs )
TEST_ENTRY( TestAddC_32sc_C3RSfs ) 
TEST_ENTRY( TestAddC_32sc_AC4RSfs )

TEST_ENTRY( TestAddC_32f_C1R )  
TEST_ENTRY( TestAddC_32fc_C1R )   
TEST_ENTRY( TestAddC_32f_C3R )
TEST_ENTRY( TestAddC_32f_AC4R )
TEST_ENTRY( TestAddC_32f_C4R )
TEST_ENTRY( TestAddC_32fc_C3R )
TEST_ENTRY( TestAddC_32fc_AC4R )

TEST_ENTRY( TestAddC_8u_C1IRSfs )
TEST_ENTRY( TestAddC_16s_C1IRSfs )    
TEST_ENTRY( TestAddC_16sc_C1IRSfs )   
TEST_ENTRY( TestAddC_32sc_C1IRSfs ) 

TEST_ENTRY( TestAddC_8u_C3IRSfs )
TEST_ENTRY( TestAddC_8u_AC4IRSfs )
TEST_ENTRY( TestAddC_8u_C4IRSfs )
TEST_ENTRY( TestAddC_16s_C3IRSfs )  

TEST_ENTRY( TestAddC_16s_AC4IRSfs ) 
TEST_ENTRY( TestAddC_16s_C4IRSfs )  
TEST_ENTRY( TestAddC_16sc_C3IRSfs ) 
TEST_ENTRY( TestAddC_16sc_AC4IRSfs )
TEST_ENTRY( TestAddC_32sc_C3IRSfs ) 
TEST_ENTRY( TestAddC_32sc_AC4IRSfs )

TEST_ENTRY( TestAddC_32f_C1IR )   
TEST_ENTRY( TestAddC_32fc_C1IR )   
TEST_ENTRY( TestAddC_32f_C3IR )
TEST_ENTRY( TestAddC_32f_AC4IR )
TEST_ENTRY( TestAddC_32f_C4IR )
TEST_ENTRY( TestAddC_32fc_C3IR )
TEST_ENTRY( TestAddC_32fc_AC4IR )


END_TEST_TABLE()
