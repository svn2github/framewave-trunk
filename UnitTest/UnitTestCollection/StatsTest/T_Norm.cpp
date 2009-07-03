/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2 License.
*/


#include "UnitTestFramework.h"
#include "StatsTest.h"

#include "StatsObject.h"
#include "fwSignal.h"


class TestNorm_Inf_32f: public SrcDstLen< F32, F32 >
{
public:
    TestNorm_Inf_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F32, F32 >( parent, "fwsNorm_Inf_32f", fwsNorm_Inf_32f )
    {}

    virtual void RunAll()
    {
         int len = 7;
         RunTest( "0 1 22 63 64 16 254","254",len);
    }
};


class TestNorm_Inf_64f: public SrcDstLen< F64, F64>
{
public:
    TestNorm_Inf_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsNorm_Inf_64f", fwsNorm_Inf_64f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};

class TestNorm_Inf_16s32f: public SrcDstLen< S16, F32 >
{
public:
    TestNorm_Inf_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, F32  >( parent, "fwsNorm_Inf_16s32f", fwsNorm_Inf_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","254",len);
    }
};


class TestNorm_Inf_32fc32f: public SrcDstLen< Fw32fc , F32 >
{
public:
    TestNorm_Inf_32fc32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw32fc, F32 >( parent, "fwsNorm_Inf_32fc32f", fwsNorm_Inf_32fc32f )
    {}

    virtual void RunAll()
    {
        int len  = 4;
         RunTest( "0 1 22 63 64 16 254 10","254.19677417308033179453960880301",len);
    }
};


class TestNorm_Inf_64fc64f: public SrcDstLen< Fw64fc, F64>
{
public:
    TestNorm_Inf_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw64fc, F64 >( parent, "fwsNorm_Inf_64fc64f", fwsNorm_Inf_64fc64f )
    {}

    virtual void RunAll()
    {
        int len  = 4;
        RunTest( "0 1 22 63 64 16 254 10","254.19677417308033179453960880301",len);
    }
};


class TestNorm_L1_32f: public SrcDstLen< F32, F32 >
{
public:
    TestNorm_L1_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F32, F32 >( parent, "fwsNorm_L1_32f", fwsNorm_L1_32f )
    {}

    virtual void RunAll()
    {
        int len =  7;
         RunTest( "0 1 22 63 64 16 254","420",len);
    }
};

class TestNorm_L1_64f: public SrcDstLen< F64, F64>
{
public:
    TestNorm_L1_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsNorm_L1_64f", fwsNorm_L1_64f )
    {}

    virtual void RunAll()
    {
        int len =7;
        RunTest( "0 1 22 63 64 16 254","420",len);
    }
};

class TestNorm_L1_16s32f: public SrcDstLen< S16, F32 >
{
public:
    TestNorm_L1_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, F32  >( parent, "fwsNorm_L1_16s32f", fwsNorm_L1_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 1 22 63 64 16 254 10","430",len);
    }
};

class TestNorm_L1_32fc64f: public SrcDstLen< Fw32fc, F64>
{
public:
    TestNorm_L1_32fc64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw32fc, F64 >( parent, "fwsNorm_L1_32fc64f", fwsNorm_L1_32fc64f )
    {}

    virtual void RunAll()
    {
        int len = 1;
        RunTest( "2 2 ","2.8284270763397217",len);
    }                        
};

class TestNorm_L1_64fc64f: public SrcDstLen< Fw64fc, F64>
{
public:
    TestNorm_L1_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw64fc, F64 >( parent, "fwsNorm_L1_64fc64f", fwsNorm_L1_64fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 1 22 63 64 16 254 10","387.89726666577440",len);
    }
};


class TestNorm_L2_32f: public SrcDstLen< F32, F32 >
{
public:
    TestNorm_L2_32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F32, F32 >( parent, "fwsNorm_L2_32f", fwsNorm_L2_32f )
    {}

    virtual void RunAll()
    {
        int len = 7;
         RunTest( "0 1 22 63 64 16 254","270.7803538",len);
    }
};

class TestNorm_L2_64f: public SrcDstLen< F64, F64>
{
public:
    TestNorm_L2_64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< F64, F64 >( parent, "fwsNorm_L2_64f", fwsNorm_L2_64f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 1 22 63 64 16 254","270.78035379251577",len);
    }
};

class TestNorm_L2_16s32f: public SrcDstLen< S16, F32 >
{
public:
    TestNorm_L2_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< S16, F32  >( parent, "fwsNorm_L2_16s32f", fwsNorm_L2_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 1 22 63 64 16 254 10","270.9649424",len);
    }
};


class TestNorm_L2_32fc64f: public SrcDstLen< Fw32fc, F64>
{
public:
    TestNorm_L2_32fc64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw32fc, F64 >( parent, "fwsNorm_L2_32fc64f", fwsNorm_L2_32fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 1 22 63 64 16 254 10","270.96494238185130",len);
    }
};

class TestNorm_L2_64fc64f: public SrcDstLen< Fw64fc, F64>
{
public:
    TestNorm_L2_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcDstLen< Fw64fc, F64 >( parent, "fwsNorm_L2_64fc64f", fwsNorm_L2_64fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 1 22 63 64 16 254 10","270.96494238185130",len);
    }
};


class TestNorm_Inf_16s32s_Sfs: public SrcDstLenScale< S16, S32>
{
public:
    TestNorm_Inf_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S32>( parent, "fwsNorm_Inf_16s32s_Sfs", fwsNorm_Inf_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 1 22 63 64 16 254","127",len,scale);
    }
};


class TestNorm_L1_16s32s_Sfs: public SrcDstLenScale< S16, S32>
{
public:
    TestNorm_L1_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S32>( parent, "fwsNorm_L1_16s32s_Sfs", fwsNorm_L1_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 1 22 63 64 16 254","210",len,scale);
    }
};


class TestNorm_L2_16s32s_Sfs: public SrcDstLenScale< S16, S32>
{
public:
    TestNorm_L2_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcDstLenScale< S16, S32>( parent, "fwsNorm_L2_16s32s_Sfs", fwsNorm_L2_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 1 22 63 64 16 254","135",len,scale);
    }
};


class TestNormDiff_Inf_32f: public SrcSrcDstLen< F32, F32, F32 >
{
public:
    TestNormDiff_Inf_32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32, F32, F32 >( parent, "fwsNormDiff_Inf_32f", fwsNormDiff_Inf_32f )
    {}

    virtual void RunAll()
    {
        int len =  7;
         RunTest( "0 2 44 126 128 32 508", "0 1 22 63 64 16 254","254",len);
    }
};


class TestNormDiff_Inf_64f: public SrcSrcDstLen< F64, F64, F64 >
{
public:
    TestNormDiff_Inf_64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64, F64, F64 >( parent, "fwsNormDiff_Inf_64f", fwsNormDiff_Inf_64f )
    {}

    virtual void RunAll()
    {
        int len =  7;
         RunTest( "0  2 44 126 128 32 508", "0 1 22 63 64 16 254","254",len);
    }
};


class TestNormDiff_Inf_16s32f: public SrcSrcDstLen< S16, S16, F32 >
{
public:
    TestNormDiff_Inf_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16, S16, F32 >( parent, "fwsNormDiff_Inf_16s32f", fwsNormDiff_Inf_16s32f )
    {}

    virtual void RunAll()
    {
        int len =  7;
         RunTest( "1 2 44 126 128 32 508", "0 1 22 63 64 16 254","254",len);
    }
};

class TestNormDiff_Inf_32fc32f: public SrcSrcDstLen< Fw32fc ,Fw32fc , F32 >
{
public:
    TestNormDiff_Inf_32fc32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw32fc ,Fw32fc, F32 >( parent, "fwsNormDiff_Inf_32fc32f", fwsNormDiff_Inf_32fc32f )
    {}

    virtual void RunAll()
    {
        int len  = 4;
         RunTest( "1 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","254.19677417308033179453960880301",len);
    }
};


class TestNormDiff_Inf_64fc64f: public SrcSrcDstLen< Fw64fc ,Fw64fc , F64 >
{
public:
    TestNormDiff_Inf_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw64fc ,Fw64fc, F64 >( parent, "fwsNormDiff_Inf_64fc64f", fwsNormDiff_Inf_64fc64f )
    {}

    virtual void RunAll()
    {
        int len  = 4;
         RunTest( "1 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","254.19677417308033179453960880301",len);
    }
};


class TestNormDiff_L1_32f: public SrcSrcDstLen< F32, F32, F32 >
{
public:
    TestNormDiff_L1_32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32, F32, F32 >( parent, "fwsNormDiff_L1_32f", fwsNormDiff_L1_32f )
    {}

    virtual void RunAll()
    {
        int len =  7;
         RunTest( "0 2 44 126 128 32 508","0 1 22 63 64 16 254","420",len);
    }
};

class TestNormDiff_L1_64f: public SrcSrcDstLen< F64, F64, F64>
{
public:
    TestNormDiff_L1_64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64, F64, F64 >( parent, "fwsNormDiff_L1_64f", fwsNormDiff_L1_64f )
    {}

    virtual void RunAll()
    {
        int len =7;
        RunTest( "0 2 44 126 128 32 508", "0 1 22 63 64 16 254","420",len);
    }
};

class TestNormDiff_L1_16s32f: public SrcSrcDstLen< S16,S16, F32 >
{
public:
    TestNormDiff_L1_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16,S16, F32  >( parent, "fwsNormDiff_L1_16s32f", fwsNormDiff_L1_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","430",len);
    }
};

class TestNormDiff_L1_32fc64f: public SrcSrcDstLen< Fw32fc,Fw32fc, F64>
{
public:
    TestNormDiff_L1_32fc64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw32fc,Fw32fc, F64 >( parent, "fwsNormDiff_L1_32fc64f", fwsNormDiff_L1_32fc64f )
    {}

    virtual void RunAll()
    {
        int len = 1;
        RunTest( "4 4 ","2 2","2.8284271247461903",len);
    }                        
};

class TestNormDiff_L1_64fc64f: public SrcSrcDstLen< Fw64fc,Fw64fc, F64>
{
public:
    TestNormDiff_L1_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw64fc,Fw64fc, F64 >( parent, "fwsNormDiff_L1_64fc64f", fwsNormDiff_L1_64fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20","0 1 22 63 64 16 254 10","387.89726666577440",len);
    }
};


class TestNormDiff_L2_32f: public SrcSrcDstLen< F32,F32, F32 >
{
public:
    TestNormDiff_L2_32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F32,F32, F32 >( parent, "fwsNormDiff_L2_32f", fwsNormDiff_L2_32f )
    {}

    virtual void RunAll()
    {
        int len = 7;
         RunTest( "0 2 44 126 128 32 508", "0 1 22 63 64 16 254","270.7803538",len);
    }
};

class TestNormDiff_L2_64f: public SrcSrcDstLen< F64,F64, F64>
{
public:
    TestNormDiff_L2_64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< F64,F64, F64 >( parent, "fwsNormDiff_L2_64f", fwsNormDiff_L2_64f )
    {}

    virtual void RunAll()
    {
        int len = 7;
        RunTest( "0 2 44 126 128 32 508","0 1 22 63 64 16 254","270.78035379251577",len);
    }
};

class TestNormDiff_L2_16s32f: public SrcSrcDstLen< S16,S16, F32 >
{
public:
    TestNormDiff_L2_16s32f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< S16,S16, F32  >( parent, "fwsNormDiff_L2_16s32f", fwsNormDiff_L2_16s32f )
    {}

    virtual void RunAll()
    {
        int len = 8;
        RunTest( "0 2 44 126 128 32 508 20" ,"0 1 22 63 64 16 254 10","270.9649424",len);
    }
};


class TestNormDiff_L2_32fc64f: public SrcSrcDstLen< Fw32fc,Fw32fc, F64>
{
public:
    TestNormDiff_L2_32fc64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw32fc,Fw32fc, F64 >( parent, "fwsNormDiff_L2_32fc64f", fwsNormDiff_L2_32fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","270.96494238185130",len);
    }
};

class TestNormDiff_L2_64fc64f: public SrcSrcDstLen< Fw64fc,Fw64fc, F64>
{
public:
    TestNormDiff_L2_64fc64f( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLen< Fw64fc,Fw64fc, F64 >( parent, "fwsNormDiff_L2_64fc64f", fwsNormDiff_L2_64fc64f )
    {}

    virtual void RunAll()
    {
        int len = 4;
        RunTest( "0 2 44 126 128 32 508 20", "0 1 22 63 64 16 254 10","270.96494238185130",len);
    }
};


class TestNormDiff_Inf_16s32s_Sfs: public SrcSrcDstLenScale< S16,S16, S32>
{
public:
    TestNormDiff_Inf_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S16,S16, S32>( parent, "fwsNormDiff_Inf_16s32s_Sfs", fwsNormDiff_Inf_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 ","0 1 22 63 64 16 254","127",len,scale);
    }
};


class TestNormDiff_L1_16s32s_Sfs: public SrcSrcDstLenScale< S16,S16, S32>
{
public:
    TestNormDiff_L1_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S16,S16, S32>( parent, "fwsNormDiff_L1_16s32s_Sfs", fwsNormDiff_L1_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 ","0 1 22 63 64 16 254","210",len,scale);
    }
};


class TestNormDiff_L2_16s32s_Sfs: public SrcSrcDstLenScale< S16,S16, S32>
{
public:
    TestNormDiff_L2_16s32s_Sfs( UnitTestCatalogBase & parent ) : 
	  SrcSrcDstLenScale< S16,S16, S32>( parent, "fwsNormDiff_L2_16s32s_Sfs", fwsNormDiff_L2_16s32s_Sfs )
    {}

    virtual void RunAll()
    {
        int len = 7;
        int scale = 1;
        RunTest( "0 2 44 126 128 32 508 ","0 1 22 63 64 16 254","135",len,scale);
    }
};


DEFINE_TEST_TABLE( NormTestCatalog )

TEST_ENTRY( TestNorm_Inf_32f )
TEST_ENTRY( TestNorm_Inf_64f )
TEST_ENTRY( TestNorm_Inf_16s32f )
TEST_ENTRY( TestNorm_Inf_32fc32f )
TEST_ENTRY( TestNorm_Inf_64fc64f )
TEST_ENTRY( TestNorm_L1_32f )
TEST_ENTRY( TestNorm_L1_64f )
TEST_ENTRY( TestNorm_L1_16s32f )
TEST_ENTRY( TestNorm_L1_32fc64f )
TEST_ENTRY( TestNorm_L1_64fc64f )
TEST_ENTRY( TestNorm_L2_32f )
TEST_ENTRY( TestNorm_L2_64f )
TEST_ENTRY( TestNorm_L2_16s32f )
TEST_ENTRY( TestNorm_L2_32fc64f )
TEST_ENTRY( TestNorm_L2_64fc64f )
TEST_ENTRY( TestNorm_Inf_16s32s_Sfs )
TEST_ENTRY( TestNorm_L1_16s32s_Sfs )
TEST_ENTRY( TestNorm_L2_16s32s_Sfs )

TEST_ENTRY( TestNormDiff_Inf_32f )
TEST_ENTRY( TestNormDiff_Inf_64f )
TEST_ENTRY( TestNormDiff_Inf_16s32f )
TEST_ENTRY( TestNormDiff_Inf_32fc32f )
TEST_ENTRY( TestNormDiff_Inf_64fc64f )
TEST_ENTRY( TestNormDiff_L1_32f )
TEST_ENTRY( TestNormDiff_L1_64f )
TEST_ENTRY( TestNormDiff_L1_16s32f )
TEST_ENTRY( TestNormDiff_L1_32fc64f )
TEST_ENTRY( TestNormDiff_L1_64fc64f )
TEST_ENTRY( TestNormDiff_L2_32f )
TEST_ENTRY( TestNormDiff_L2_64f )
TEST_ENTRY( TestNormDiff_L2_16s32f )
TEST_ENTRY( TestNormDiff_L2_32fc64f )
TEST_ENTRY( TestNormDiff_L2_64fc64f )
TEST_ENTRY( TestNormDiff_Inf_16s32s_Sfs )
TEST_ENTRY( TestNormDiff_L1_16s32s_Sfs )
TEST_ENTRY( TestNormDiff_L2_16s32s_Sfs )

END_TEST_TABLE()
