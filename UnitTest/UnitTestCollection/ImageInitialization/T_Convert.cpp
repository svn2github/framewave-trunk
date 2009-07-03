/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ImageInitialization.h"

#include "FunctionSignatures.h"
#include "fwImage.h"


class TestConvert_8u16u_C1R : public SrcDstRoi< U8, 1, U16, 1>
{
public:
    TestConvert_8u16u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 1, U16, 1>( parent, "fwiConvert_8u16u_C1R", fwiConvert_8u16u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_8u16u_C3R : public SrcDstRoi< U8, 3, U16, 3>
{
public:
    TestConvert_8u16u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U16, 3>( parent, "fwiConvert_8u16u_C3R", fwiConvert_8u16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_8u16u_C4R : public SrcDstRoi< U8, 4, U16, 4>
{
public:
    TestConvert_8u16u_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U16, 4>( parent, "fwiConvert_8u16u_C4R", fwiConvert_8u16u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_8u16u_AC4R : public SrcDstRoi< U8, 4, U16, 4>
{
public:
    TestConvert_8u16u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U16, 4>( parent, "fwiConvert_8u16u_AC4R", fwiConvert_8u16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_8u16s_C1R : public SrcDstRoi< U8, 1, S16, 1>
{
public:
    TestConvert_8u16s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 1, S16, 1>( parent, "fwiConvert_8u16s_C1R", fwiConvert_8u16s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_8u16s_C3R : public SrcDstRoi< U8, 3, S16, 3>
{
public:
    TestConvert_8u16s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, S16, 3>( parent, "fwiConvert_8u16s_C3R", fwiConvert_8u16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_8u16s_C4R : public SrcDstRoi< U8, 4, S16, 4>
{
public:
    TestConvert_8u16s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, S16, 4>( parent, "fwiConvert_8u16s_C4R", fwiConvert_8u16s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_8u16s_AC4R : public SrcDstRoi< U8, 4, S16, 4>
{
public:
    TestConvert_8u16s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, S16, 4>( parent, "fwiConvert_8u16s_AC4R", fwiConvert_8u16s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_8u32s_C1R : public SrcDstRoi< U8, 1, S32, 1>
{
public:
    TestConvert_8u32s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 1, S32, 1>( parent, "fwiConvert_8u32s_C1R", fwiConvert_8u32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_8u32s_C3R : public SrcDstRoi< U8, 3, S32, 3>
{
public:
    TestConvert_8u32s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, S32, 3>( parent, "fwiConvert_8u32s_C3R", fwiConvert_8u32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_8u32s_C4R : public SrcDstRoi< U8, 4, S32, 4>
{
public:
    TestConvert_8u32s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, S32, 4>( parent, "fwiConvert_8u32s_C4R", fwiConvert_8u32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_8u32s_AC4R : public SrcDstRoi< U8, 4, S32, 4>
{
public:
    TestConvert_8u32s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, S32, 4>( parent, "fwiConvert_8u32s_AC4R", fwiConvert_8u32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_8s32s_C1R : public SrcDstRoi< S8, 1, S32, 1>
{
public:
    TestConvert_8s32s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 1, S32, 1>( parent, "fwiConvert_8s32s_C1R", fwiConvert_8s32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 -128 127" );
    }
};

class TestConvert_8s32s_C3R : public SrcDstRoi< S8, 3, S32, 3>
{
public:
    TestConvert_8s32s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 3, S32, 3>( parent, "fwiConvert_8s32s_C3R", fwiConvert_8s32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 -128 127 -128 127" );
    }
};

class TestConvert_8s32s_C4R : public SrcDstRoi< S8, 4, S32, 4>
{
public:
    TestConvert_8s32s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 4, S32, 4>( parent, "fwiConvert_8s32s_C4R", fwiConvert_8s32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 127 -128 127 5 4" );
    }
};

class TestConvert_8s32s_AC4R : public SrcDstRoi< S8, 4, S32, 4>
{
public:
    TestConvert_8s32s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 4, S32, 4>( parent, "fwiConvert_8s32s_AC4R", fwiConvert_8s32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 0 -128 127 5 0" );
    }
};

class TestConvert_16u32s_C1R : public SrcDstRoi< U16, 1, S32, 1>
{
public:
    TestConvert_16u32s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 1, S32, 1>( parent, "fwiConvert_16u32s_C1R", fwiConvert_16u32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 65535" );
    }
};

class TestConvert_16u32s_C3R : public SrcDstRoi< U16, 3, S32, 3>
{
public:
    TestConvert_16u32s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 3, S32, 3>( parent, "fwiConvert_16u32s_C3R", fwiConvert_16u32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 65535 0 65535" );
    }
};

class TestConvert_16u32s_C4R : public SrcDstRoi< U16, 4, S32, 4>
{
public:
    TestConvert_16u32s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, S32, 4>( parent, "fwiConvert_16u32s_C4R", fwiConvert_16u32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 65535 0 65535 5 4" );
    }
};

class TestConvert_16u32s_AC4R : public SrcDstRoi< U16, 4, S32, 4>
{
public:
    TestConvert_16u32s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, S32, 4>( parent, "fwiConvert_16u32s_AC4R", fwiConvert_16u32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 65535 5 0" );
    }
};

//Reduced to bit depth

class TestConvert_16u8u_C1R : public SrcDstRoi< U16, 1, U8, 1>
{
public:
    TestConvert_16u8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 1, U8, 1>( parent, "fwiConvert_16u8u_C1R", fwiConvert_16u8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_16u8u_C3R : public SrcDstRoi< U16, 3, U8, 3>
{
public:
    TestConvert_16u8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 3, U8, 3>( parent, "fwiConvert_16u8u_C3R", fwiConvert_16u8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_16u8u_C4R : public SrcDstRoi< U16, 4, U8, 4>
{
public:
    TestConvert_16u8u_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, U8, 4>( parent, "fwiConvert_16u8u_C4R", fwiConvert_16u8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_16u8u_AC4R : public SrcDstRoi< U16, 4, U8, 4>
{
public:
    TestConvert_16u8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, U8, 4>( parent, "fwiConvert_16u8u_AC4R", fwiConvert_16u8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_16s8u_C1R : public SrcDstRoi< S16, 1, U8, 1>
{
public:
    TestConvert_16s8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 1, U8, 1>( parent, "fwiConvert_16s8u_C1R", fwiConvert_16s8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_16s8u_C3R : public SrcDstRoi< S16, 3, U8, 3>
{
public:
    TestConvert_16s8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 3, U8, 3>( parent, "fwiConvert_16s8u_C3R", fwiConvert_16s8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_16s8u_C4R : public SrcDstRoi< S16, 4, U8, 4>
{
public:
    TestConvert_16s8u_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, U8, 4>( parent, "fwiConvert_16s8u_C4R", fwiConvert_16s8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_16s8u_AC4R : public SrcDstRoi< S16, 4, U8, 4>
{
public:
    TestConvert_16s8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, U8, 4>( parent, "fwiConvert_16s8u_AC4R", fwiConvert_16s8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_32s8u_C1R : public SrcDstRoi< S32, 1, U8, 1>
{
public:
    TestConvert_32s8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 1, U8, 1>( parent, "fwiConvert_32s8u_C1R", fwiConvert_32s8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_32s8u_C3R : public SrcDstRoi< S32, 3, U8, 3>
{
public:
    TestConvert_32s8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 3, U8, 3>( parent, "fwiConvert_32s8u_C3R", fwiConvert_32s8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_32s8u_C4R : public SrcDstRoi< S32, 4, U8, 4>
{
public:
    TestConvert_32s8u_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 4, U8, 4>( parent, "fwiConvert_32s8u_C4R", fwiConvert_32s8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_32s8u_AC4R : public SrcDstRoi< S32, 4, U8, 4>
{
public:
    TestConvert_32s8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 4, U8, 4>( parent, "fwiConvert_32s8u_AC4R", fwiConvert_32s8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_32s8s_C1R : public SrcDstRoi< S32, 1, S8, 1>
{
public:
    TestConvert_32s8s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 1, S8, 1>( parent, "fwiConvert_32s8s_C1R", fwiConvert_32s8s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 -128 127" );
    }
};

class TestConvert_32s8s_C3R : public SrcDstRoi< S32, 3, S8, 3>
{
public:
    TestConvert_32s8s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 3, S8, 3>( parent, "fwiConvert_32s8s_C3R", fwiConvert_32s8s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 -128 127 -128 127" );
    }
};

class TestConvert_32s8s_C4R : public SrcDstRoi< S32, 4, S8, 4>
{
public:
    TestConvert_32s8s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 4, S8, 4>( parent, "fwiConvert_32s8s_C4R", fwiConvert_32s8s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 127 -128 127 5 4" );
    }
};

class TestConvert_32s8s_AC4R : public SrcDstRoi< S32, 4, S8, 4>
{
public:
    TestConvert_32s8s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 4, S8, 4>( parent, "fwiConvert_32s8s_AC4R", fwiConvert_32s8s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 0 -128 127 5 0" );
    }
};

//Floating Point
class TestConvert_8u32f_C1R : public SrcDstRoi< U8, 1, F32, 1>
{
public:
    TestConvert_8u32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 1, F32, 1>( parent, "fwiConvert_8u32f_C1R", fwiConvert_8u32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 255" );
    }
};

class TestConvert_8u32f_C3R : public SrcDstRoi< U8, 3, F32, 3>
{
public:
    TestConvert_8u32f_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, F32, 3>( parent, "fwiConvert_8u32f_C3R", fwiConvert_8u32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 255 0 255" );
    }
};

class TestConvert_8u32f_C4R : public SrcDstRoi< U8, 4, F32, 4>
{
public:
    TestConvert_8u32f_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, F32, 4>( parent, "fwiConvert_8u32f_C4R", fwiConvert_8u32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 255 0 255 5 4" );
    }
};

class TestConvert_8u32f_AC4R : public SrcDstRoi< U8, 4, F32, 4>
{
public:
    TestConvert_8u32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, F32, 4>( parent, "fwiConvert_8u32f_AC4R", fwiConvert_8u32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 255 5 0" );
    }
};

class TestConvert_8s32f_C1R : public SrcDstRoi< S8, 1, F32, 1>
{
public:
    TestConvert_8s32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 1, F32, 1>( parent, "fwiConvert_8s32f_C1R", fwiConvert_8s32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 -128 127" );
    }
};

class TestConvert_8s32f_C3R : public SrcDstRoi< S8, 3, F32, 3>
{
public:
    TestConvert_8s32f_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 3, F32, 3>( parent, "fwiConvert_8s32f_C3R", fwiConvert_8s32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 -128 127 -128 127" );
    }
};

class TestConvert_8s32f_C4R : public SrcDstRoi< S8, 4, F32, 4>
{
public:
    TestConvert_8s32f_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 4, F32, 4>( parent, "fwiConvert_8s32f_C4R", fwiConvert_8s32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 127 -128 127 5 4" );
    }
};

class TestConvert_8s32f_AC4R : public SrcDstRoi< S8, 4, F32, 4>
{
public:
    TestConvert_8s32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S8, 4, F32, 4>( parent, "fwiConvert_8s32f_AC4R", fwiConvert_8s32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -128 0 -128 127 5 0" );
    }
};

class TestConvert_16u32f_C1R : public SrcDstRoi< U16, 1, F32, 1>
{
public:
    TestConvert_16u32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 1, F32, 1>( parent, "fwiConvert_16u32f_C1R", fwiConvert_16u32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 0 65535" );
    }
};

class TestConvert_16u32f_C3R : public SrcDstRoi< U16, 3, F32, 3>
{
public:
    TestConvert_16u32f_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 3, F32, 3>( parent, "fwiConvert_16u32f_C3R", fwiConvert_16u32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 0 65535 0 65535" );
    }
};

class TestConvert_16u32f_C4R : public SrcDstRoi< U16, 4, F32, 4>
{
public:
    TestConvert_16u32f_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, F32, 4>( parent, "fwiConvert_16u32f_C4R", fwiConvert_16u32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 65535 0 65535 5 4" );
    }
};

class TestConvert_16u32f_AC4R : public SrcDstRoi< U16, 4, F32, 4>
{
public:
    TestConvert_16u32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, F32, 4>( parent, "fwiConvert_16u32f_AC4R", fwiConvert_16u32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 0 0 0 65535 5 0" );
    }
};

class TestConvert_16s32f_C1R : public SrcDstRoi< S16, 1, F32, 1>
{
public:
    TestConvert_16s32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 1, F32, 1>( parent, "fwiConvert_16s32f_C1R", fwiConvert_16s32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX", "0 1 -32768 32767" );
    }
};

class TestConvert_16s32f_C3R : public SrcDstRoi< S16, 3, F32, 3>
{
public:
    TestConvert_16s32f_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 3, F32, 3>( parent, "fwiConvert_16s32f_C3R", fwiConvert_16s32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX", "0 1 -32768 32767 -32768 32767" );
    }
};

class TestConvert_16s32f_C4R : public SrcDstRoi< S16, 4, F32, 4>
{
public:
    TestConvert_16s32f_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, F32, 4>( parent, "fwiConvert_16s32f_C4R", fwiConvert_16s32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -32768 32767 -32768 32767 5 4" );
    }
};

class TestConvert_16s32f_AC4R : public SrcDstRoi< S16, 4, F32, 4>
{
public:
    TestConvert_16s32f_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, F32, 4>( parent, "fwiConvert_16s32f_AC4R", fwiConvert_16s32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 MIN MAX MIN MAX 5 4 ", "0 1 -32768 0 -32768 32767 5 0" );
    }
};







DEFINE_TEST_TABLE( ConvertTestCatalog )
TEST_ENTRY( TestConvert_8u16u_C1R     )
TEST_ENTRY( TestConvert_8u16u_C3R     )
TEST_ENTRY( TestConvert_8u16u_C4R     )
TEST_ENTRY( TestConvert_8u16u_AC4R    )

TEST_ENTRY( TestConvert_8u16s_C1R     )
TEST_ENTRY( TestConvert_8u16s_C3R     )
TEST_ENTRY( TestConvert_8u16s_C4R     )
TEST_ENTRY( TestConvert_8u16s_AC4R    )

TEST_ENTRY( TestConvert_8u32s_C1R     )
TEST_ENTRY( TestConvert_8u32s_C3R     )
TEST_ENTRY( TestConvert_8u32s_C4R     )
TEST_ENTRY( TestConvert_8u32s_AC4R    )

TEST_ENTRY( TestConvert_8s32s_C1R     )
TEST_ENTRY( TestConvert_8s32s_C3R     )
TEST_ENTRY( TestConvert_8s32s_C4R     )
TEST_ENTRY( TestConvert_8s32s_AC4R    )

TEST_ENTRY( TestConvert_16u32s_C1R    )
TEST_ENTRY( TestConvert_16u32s_C3R    )
TEST_ENTRY( TestConvert_16u32s_C4R    )
TEST_ENTRY( TestConvert_16u32s_AC4R   )

TEST_ENTRY( TestConvert_16u8u_C1R     )
TEST_ENTRY( TestConvert_16u8u_C3R     )
TEST_ENTRY( TestConvert_16u8u_C4R     )
TEST_ENTRY( TestConvert_16u8u_AC4R    )

TEST_ENTRY( TestConvert_16s8u_C1R     )
TEST_ENTRY( TestConvert_16s8u_C3R     )
TEST_ENTRY( TestConvert_16s8u_C4R     )
TEST_ENTRY( TestConvert_16s8u_AC4R    )

TEST_ENTRY( TestConvert_32s8u_C1R     )
TEST_ENTRY( TestConvert_32s8u_C3R     )
TEST_ENTRY( TestConvert_32s8u_C4R     )
TEST_ENTRY( TestConvert_32s8u_AC4R    )

TEST_ENTRY( TestConvert_32s8s_C1R     )
TEST_ENTRY( TestConvert_32s8s_C3R     )
TEST_ENTRY( TestConvert_32s8s_C4R     )
TEST_ENTRY( TestConvert_32s8s_AC4R    )

TEST_ENTRY( TestConvert_8u32f_C1R     )
TEST_ENTRY( TestConvert_8u32f_C3R     )
TEST_ENTRY( TestConvert_8u32f_C4R     )
TEST_ENTRY( TestConvert_8u32f_AC4R    )

TEST_ENTRY( TestConvert_8s32f_C1R     )
TEST_ENTRY( TestConvert_8s32f_C3R     )
TEST_ENTRY( TestConvert_8s32f_C4R     )
TEST_ENTRY( TestConvert_8s32f_AC4R    )

TEST_ENTRY( TestConvert_16u32f_C1R     )
TEST_ENTRY( TestConvert_16u32f_C3R     )
TEST_ENTRY( TestConvert_16u32f_C4R     )
TEST_ENTRY( TestConvert_16u32f_AC4R    )

TEST_ENTRY( TestConvert_16s32f_C1R     )
TEST_ENTRY( TestConvert_16s32f_C3R     )
TEST_ENTRY( TestConvert_16s32f_C4R     )
TEST_ENTRY( TestConvert_16s32f_AC4R    )


END_TEST_TABLE()
