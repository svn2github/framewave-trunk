/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"


class TestStdDev_32f: public SrcDstLenHint< F32, F32 >
{
public:
    TestStdDev_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLenHint< F32, F32 >( parent, "fwsStdDev_32f", fwsStdDev_32f )
    {}

    virtual void RunAll()
    {
         int len = 6;
         RunTest( "5 5 5 5 5 5","0",len,fwAlgHintNone);
    }
};

class TestStdDev_64f: public SrcDstLen< F64, F64 >
{
public:
    TestStdDev_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsStdDev_64f", fwsStdDev_64f )
    {}

    virtual void RunAll()
    {
         int len = 6;
        RunTest( "5 5 5 5 5 5","0",len,fwAlgHintNone);
    }
};

class TestStdDev_16s32s_Sfs: public SrcDstLenScale< S16, S32 >
{
public:
    TestStdDev_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S32 >( parent, "fwsStdDev_16s32s_Sfs", fwsStdDev_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","45",len,scale);
    }
};

class TestStdDev_16s_Sfs: public SrcDstLenScale< S16, S16 >
{
public:
    TestStdDev_16s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S16 >( parent, "fwsStdDev_16s_Sfs", fwsStdDev_16s_Sfs )
    {}

    virtual void RunAll()
    {
         int len = 7;
         int scale = 1;
         RunTest( "0 1 22 63 64 16 254","45",len,scale);
    }
};


DEFINE_TEST_TABLE( StdDevTestCatalog )

TEST_ENTRY( TestStdDev_32f )
TEST_ENTRY( TestStdDev_64f )
TEST_ENTRY( TestStdDev_16s32s_Sfs )
TEST_ENTRY( TestStdDev_16s_Sfs )

END_TEST_TABLE()