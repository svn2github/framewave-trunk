/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"

class TestMean_32f: public SrcDstLenHint< F32, F32 >
{
public:
    TestMean_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLenHint< F32, F32 >( parent, "fwsMean_32f", fwsMean_32f )
    {}

    virtual void RunAll()
    {
         int len = 7;
         RunTest( "0 1 22 63 64 16 254","60",len,fwAlgHintNone);
    }
};


class TestMean_32fc: public SrcDstLenHint< Fw32fc , Fw32fc >
{
public:
    TestMean_32fc( UnitTestCatalogBase & parent ) : 
	  SrcDstLenHint< Fw32fc, Fw32fc >( parent, "fwsMean_32fc", fwsMean_32fc )
    {}

    virtual void RunAll()
    {
        int len  = 4;
         RunTest( "0 1 22 63 64 16 254 10","85 22.5",len,fwAlgHintNone);
    }
};


class TestMean_64f: public SrcDstLen< F64, F64 >
{
public:
    TestMean_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsMean_64f", fwsMean_64f )
    {}

    virtual void RunAll()
    {
         int len = 7;
         RunTest( "0 1 22 63 64 16 254","60",len);
    }
};


class TestMean_64fc: public SrcDstLen< Fw64fc , Fw64fc >
{
public:
    TestMean_64fc( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw64fc, Fw64fc >( parent, "fwsMean_64fc", fwsMean_64fc )
    {}

    virtual void RunAll()
    {
        int len  = 4;
         RunTest( "0 1 22 63 64 16 254 10","85 22.5",len);
    }
};


class TestMean_16s_Sfs: public SrcDstLenScale< S16, S16 >
{
public:
    TestMean_16s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S16 >( parent, "fwsMean_16s_Sfs", fwsMean_16s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","30",len,scale);
    }
};


class TestMean_16sc_Sfs: public SrcDstLenScale< Fw16sc , Fw16sc >
{
public:
    TestMean_16sc_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< Fw16sc, Fw16sc >( parent, "fwsMean_16sc_Sfs", fwsMean_16sc_Sfs )
    {}

    virtual void RunAll()
    {
        int len  = 4;
        int scale = 1;
        RunTest( "0 1 22 63 64 16 254 10","42 11",len,scale);
    }
};


DEFINE_TEST_TABLE( MeanTestCatalog )

TEST_ENTRY( TestMean_32f )
TEST_ENTRY( TestMean_32fc )
TEST_ENTRY( TestMean_64f )
TEST_ENTRY( TestMean_64fc )
TEST_ENTRY( TestMean_16s_Sfs )
TEST_ENTRY( TestMean_16sc_Sfs )

END_TEST_TABLE()
