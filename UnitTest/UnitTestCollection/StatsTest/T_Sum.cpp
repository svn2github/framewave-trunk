/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"


class TestSum_32f: public SrcDstLenHint< F32, F32 >
{
public:
    TestSum_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLenHint< F32, F32 >( parent, "fwsSum_32f", fwsSum_32f )
    {}

    virtual void RunAll()
    {
         int len = 7;
         RunTest( "0 1 22 63 64 16 254","420",len,fwAlgHintNone);
    }
};

class TestSum_32fc: public SrcDstLenHint< Fw32fc, Fw32fc >
{
public:
    TestSum_32fc( UnitTestCatalogBase & parent ) : 
	  SrcDstLenHint< Fw32fc, Fw32fc >( parent, "fwsSum_32fc", fwsSum_32fc )
    {}

    virtual void RunAll()
    {
         int len = 4;
         RunTest( "0 1 22 63 64 16 254 10","340 90",len,fwAlgHintNone);
    }
};


class TestSum_64f: public SrcDstLen< F64, F64 >
{
public:
    TestSum_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsSum_64f", fwsSum_64f )
    {}

    virtual void RunAll()
    {
         int len = 7;
         RunTest( "0 1 22 63 64 16 254","420",len);
    }
};

class TestSum_64fc: public SrcDstLen< Fw64fc, Fw64fc >
{
public:
    TestSum_64fc( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw64fc, Fw64fc >( parent, "fwsSum_64fc", fwsSum_64fc )
    {}

    virtual void RunAll()
    {
         int len = 4;
         RunTest( "0 1 22 63 64 16 254 10","340 90",len);
    }
};

class TestSum_16s_Sfs: public SrcDstLenScale< S16, S16 >
{
public:
    TestSum_16s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S16 >( parent, "fwsSum_16s_Sfs", fwsSum_16s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","210",len,scale);
    }
};

class TestSum_32s_Sfs: public SrcDstLenScale< S32, S32 >
{
public:
    TestSum_32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S32, S32 >( parent, "fwsSum_32s_Sfs", fwsSum_32s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","210",len,scale);
    }
};


class TestSum_16s32s_Sfs: public SrcDstLenScale< S16, S32 >
{
public:
    TestSum_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S32 >( parent, "fwsSum_16s32s_Sfs", fwsSum_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","210",len,scale);
    }
};

class TestSum_16sc_Sfs: public SrcDstLenScale< Fw16sc, Fw16sc >
{
public:
    TestSum_16sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< Fw16sc, Fw16sc >( parent, "fwsSum_16sc_Sfs", fwsSum_16sc_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 4;
         int scale=1;
         RunTest( "0 1 22 63 64 16 254 10","170 45",len,scale);
    }
};

class TestSum_16sc32sc_Sfs: public SrcDstLenScale< Fw16sc, Fw32sc >
{
public:
    TestSum_16sc32sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< Fw16sc, Fw32sc >( parent, "fwsSum_16sc32sc_Sfs", fwsSum_16sc32sc_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 4;
         int scale=1;
         RunTest( "0 1 22 63 64 16 254 10","170 45",len,scale);
    }
};



DEFINE_TEST_TABLE( SumTestCatalog )

TEST_ENTRY ( TestSum_32f )
TEST_ENTRY ( TestSum_32fc )
TEST_ENTRY ( TestSum_64f )
TEST_ENTRY ( TestSum_64fc )
TEST_ENTRY ( TestSum_16s_Sfs )
TEST_ENTRY ( TestSum_32s_Sfs )
TEST_ENTRY ( TestSum_16s32s_Sfs )
TEST_ENTRY ( TestSum_16sc_Sfs )
TEST_ENTRY ( TestSum_16sc32sc_Sfs )

END_TEST_TABLE()
