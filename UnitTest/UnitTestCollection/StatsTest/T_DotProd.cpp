/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"



class TestDotProd_32f: public SrcSrcDstLen< F32,F32, F32 >
{
public:
    TestDotProd_32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32, F32  >( parent, "fwsDotProd_32f", fwsDotProd_32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","146844",len);
    }
};

class TestDotProd_32fc: public SrcSrcDstLen< Fw32fc,Fw32fc, Fw32fc>
{
public:
    TestDotProd_32fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw32fc,Fw32fc, Fw32fc >( parent, "fwsDotProd_32fc", fwsDotProd_32fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","129540 19800",len);
    }
};

class TestDotProd_32f32fc: public SrcSrcDstLen< F32,Fw32fc, Fw32fc>
{
public:
    TestDotProd_32f32fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,Fw32fc, Fw32fc >( parent, "fwsDotProd_32f32fc", fwsDotProd_32f32fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "128 32 508 20", "0 1 22 63 64 16 254 10","38296 10472",len);
    }
};

class TestDotProd_32f64f: public SrcSrcDstLen< F32,F32, F64>
{
public:
    TestDotProd_32f64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32, F64 >( parent, "fwsDotProd_32f64f", fwsDotProd_32f64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "128 32 508 20", "0 1 22 63","12468",len);
    }
};


class TestDotProd_32fc64fc: public SrcSrcDstLen< Fw32fc,Fw32fc, Fw64fc>
{
public:
    TestDotProd_32fc64fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw32fc,Fw32fc, Fw64fc >( parent, "fwsDotProd_32fc64fc", fwsDotProd_32fc64fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "128 32 508 20", "0 1 22 63","19768 65144",len);
    }
};


class TestDotProd_32f32fc64fc: public SrcSrcDstLen< F32,Fw32fc, Fw64fc>
{
public:
    TestDotProd_32f32fc64fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,Fw32fc, Fw64fc >( parent, "fwsDotProd_32f32fc64fc", fwsDotProd_32f32fc64fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "128 32 508 20", "0 1 22 63","1144 3912",len);
    }
};

class TestDotProd_64f: public SrcSrcDstLen< F64,F64, F64 >
{
public:
    TestDotProd_64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64,F64, F64  >( parent, "fwsDotProd_64f", fwsDotProd_64f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","146844",len);
    }
};


class TestDotProd_64fc: public SrcSrcDstLen< Fw64fc,Fw64fc, Fw64fc >
{
public:
    TestDotProd_64fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw64fc,Fw64fc, Fw64fc  >( parent, "fwsDotProd_64fc", fwsDotProd_64fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","129540 19800",len);
    }
};

class TestDotProd_64f64fc: public SrcSrcDstLen< F64,Fw64fc, Fw64fc >
{
public:
    TestDotProd_64f64fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen<  F64,Fw64fc, Fw64fc  >( parent, "fwsDotProd_64f64fc", fwsDotProd_64f64fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126" ,"0 1 22 63 64 16 254 10","34864 2090",len);
    }
};


class TestDotProd_16s64s: public SrcSrcDstLen< S16,S16, S64 >
{
public:
    TestDotProd_16s64s( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16,S16, S64 >( parent, "fwsDotProd_16s64s", fwsDotProd_16s64s )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","146844",len);
    }
};

class TestDotProd_16sc64sc: public SrcSrcDstLen< Fw16sc,Fw16sc, Fw64sc >
{
public:
    TestDotProd_16sc64sc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw16sc,Fw16sc, Fw64sc >( parent, "fwsDotProd_16sc64sc", fwsDotProd_16sc64sc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","129540 19800",len);
    }
};

class TestDotProd_16s16sc64sc: public SrcSrcDstLen< Fw16s,Fw16sc, Fw64sc >
{
public:
    TestDotProd_16s16sc64sc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw16s,Fw16sc, Fw64sc >( parent, "fwsDotProd_16s16sc64sc", fwsDotProd_16s16sc64sc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126" ,"0 1 22 63 64 16 254 10","34864 2090",len);
    }
};


class TestDotProd_16s32f: public SrcSrcDstLen< S16,S16, F32 >
{
public:
    TestDotProd_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16,S16, F32 >( parent, "fwsDotProd_16s32f", fwsDotProd_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","146844",len);
    }
};

class TestDotProd_16sc32fc: public SrcSrcDstLen< Fw16sc,Fw16sc, Fw32fc >
{
public:
    TestDotProd_16sc32fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw16sc,Fw16sc, Fw32fc >( parent, "fwsDotProd_16sc32fc", fwsDotProd_16sc32fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","129540 19800",len);
    }
};

class TestDotProd_16s16sc32fc: public SrcSrcDstLen< S16,Fw16sc, Fw32fc >
{
public:
    TestDotProd_16s16sc32fc( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16,Fw16sc, Fw32fc >( parent, "fwsDotProd_16s16sc32fc", fwsDotProd_16s16sc32fc )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126" ,"0 1 22 63 64 16 254 10","34864 2090",len);
    }
};


class TestDotProd_16s_Sfs: public SrcSrcDstLenScale< S16,S16, S16 >
{
public:
    TestDotProd_16s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S16,S16, S16>( parent, "fwsDotProd_16s_Sfs", fwsDotProd_16s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 8;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","32767",len,scale);
    }
};


class TestDotProd_16sc_Sfs: public SrcSrcDstLenScale< Fw16sc,Fw16sc, Fw16sc >
{
public:
    TestDotProd_16sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< Fw16sc,Fw16sc, Fw16sc>( parent, "fwsDotProd_16sc_Sfs", fwsDotProd_16sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","32767 9900",len,scale);
    }
};

class TestDotProd_32s_Sfs: public SrcSrcDstLenScale< S32,S32, S32>
{
public:
    TestDotProd_32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S32,S32, S32>( parent, "fwsDotProd_32s_Sfs", fwsDotProd_32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 8;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","73422",len,scale);
    }
};


class TestDotProd_32sc_Sfs: public SrcSrcDstLenScale< Fw32sc,Fw32sc, Fw32sc >
{
public:
    TestDotProd_32sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< Fw32sc,Fw32sc, Fw32sc>( parent, "fwsDotProd_32sc_Sfs", fwsDotProd_32sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","64770 9900",len,scale);
    }
};


class TestDotProd_16s32s_Sfs: public SrcSrcDstLenScale< S16 ,S16, S32 >
{
public:
    TestDotProd_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S16 ,S16, S32>( parent, "fwsDotProd_16s32s_Sfs", fwsDotProd_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 8;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","73422",len,scale);
    }
};

class TestDotProd_16s16sc32sc_Sfs: public SrcSrcDstLenScale< S16,Fw16sc, Fw32sc >
{
public:
    TestDotProd_16s16sc32sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale<  S16,Fw16sc, Fw32sc>( parent, "fwsDotProd_16s16sc32sc_Sfs", fwsDotProd_16s16sc32sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126" ,"0 1 22 63 64 16 254 10","17432 1045",len,scale);
    }
};

class TestDotProd_16s32s32s_Sfs: public SrcSrcDstLenScale< S16,S32, S32 >
{
public:
    TestDotProd_16s32s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale<  S16,S32, S32 >( parent, "fwsDotProd_16s32s32s_Sfs", fwsDotProd_16s32s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 8;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","73422",len,scale);
    }
};

class TestDotProd_16s16sc_Sfs: public SrcSrcDstLenScale< S16,Fw16sc, Fw16sc >
{
public:
    TestDotProd_16s16sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale<  S16,Fw16sc, Fw16sc >( parent, "fwsDotProd_16s16sc_Sfs", fwsDotProd_16s16sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126" ,"0 1 22 63 64 16 254 10","17432 1045",len,scale);
    }
};

class TestDotProd_16sc32sc_Sfs: public SrcSrcDstLenScale< Fw16sc,Fw16sc, Fw32sc >
{
public:
    TestDotProd_16sc32sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< Fw16sc,Fw16sc, Fw32sc >( parent, "fwsDotProd_16sc32sc_Sfs", fwsDotProd_16sc32sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","64770 9900",len,scale);
    }
};

class TestDotProd_32s32sc_Sfs: public SrcSrcDstLenScale< S32,Fw32sc, Fw32sc >
{
public:
    TestDotProd_32s32sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale<S32,Fw32sc, Fw32sc >( parent, "fwsDotProd_32s32sc_Sfs", fwsDotProd_32s32sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 4;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","17432 1045",len,scale);
    }
};

DEFINE_TEST_TABLE( DotProdTestCatalog )

TEST_ENTRY( TestDotProd_32f )
TEST_ENTRY( TestDotProd_32fc)
TEST_ENTRY( TestDotProd_32f32fc)
TEST_ENTRY( TestDotProd_32f64f)
TEST_ENTRY( TestDotProd_32fc64fc)
TEST_ENTRY( TestDotProd_32f32fc64fc)
TEST_ENTRY( TestDotProd_64f)
TEST_ENTRY( TestDotProd_64fc)
TEST_ENTRY( TestDotProd_64f64fc)
TEST_ENTRY( TestDotProd_16s64s)
TEST_ENTRY( TestDotProd_16sc64sc)
TEST_ENTRY( TestDotProd_16s16sc64sc)
TEST_ENTRY( TestDotProd_16s32f)
TEST_ENTRY( TestDotProd_16sc32fc)
TEST_ENTRY( TestDotProd_16s16sc32fc)
TEST_ENTRY( TestDotProd_16s_Sfs)
TEST_ENTRY( TestDotProd_16sc_Sfs)
TEST_ENTRY( TestDotProd_32s_Sfs)
TEST_ENTRY( TestDotProd_32sc_Sfs)
TEST_ENTRY( TestDotProd_16s32s_Sfs)
TEST_ENTRY( TestDotProd_16s16sc32sc_Sfs)
TEST_ENTRY( TestDotProd_16s32s32s_Sfs)
TEST_ENTRY( TestDotProd_16s16sc_Sfs)
TEST_ENTRY( TestDotProd_16sc32sc_Sfs)
TEST_ENTRY( TestDotProd_32s32sc_Sfs)

END_TEST_TABLE()
