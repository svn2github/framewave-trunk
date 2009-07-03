/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "ImageInitialization.h"

#include "FunctionSignatures.h"
#include "fwImage.h"

//Copying all pixels of all channels

class TestCopy_8u_C1R : public SrcDstRoi< U8, 1, U8, 1>
{
public:
    TestCopy_8u_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 1, U8, 1>( parent, "fwiCopy_8u_C1R", fwiCopy_8u_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 3 MAX", "0 1 3 255" );
        RunTest( "0 2 4 MAX", "0 2 4 255" );
        RunTest( "7 5 2 6", "7 5 2 6" );
    }
};

class TestCopy_8u_C3R : public SrcDstRoi< U8, 3, U8, 3>
{
public:
    TestCopy_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U8, 3>( parent, "fwiCopy_8u_C3R", fwiCopy_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX 3 5 6 7 8", "0 2 4 255 3 5 6 7 8" );
        RunTest( "0 2 4 MAX 1 2 43 4 5 ", "0 2 4 255 1 2 43 4 5 " );
        RunTest( "2 4 6 8 10 12 14 16 18 ", "2 4 6 8 10 12 14 16 18");

    }
};

class TestCopy_8u_C4R : public SrcDstRoi< U8, 4, U8, 4>
{
public:
    TestCopy_8u_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U8, 4>( parent, "fwiCopy_8u_C4R", fwiCopy_8u_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX 1 2 3 4 5 6 8 10 22 4 5 43", "0 2 4 255 1 2 3 4 5 6 8 10 22 4 5 43" );
        RunTest( "1 2 5 MAX 3 4 12 23 2 9 7 6 4 8 0 3", "1 2 5 255 3 4 12 23 2 9 7 6 4 8 0 3");
        RunTest( "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8", "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8");
    }
};

class TestCopy_8u_AC4R : public SrcSDstRoi< U8, 4, U8, 4>
{
public:
    TestCopy_8u_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 4, U8, 4>( parent, "fwiCopy_8u_AC4R", fwiCopy_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX","1 2 4 5", "0 2 4 5" );
        RunTest( "1 2 5 MAX 2 3 4 5","2 4 6 8 9 2 3 1", "1 2 5 8 2 3 4 1");
        RunTest( "2 4 6 8 12 43 5 6 7 1 9 10 4 6 8 9"," ", "2 4 6 0 12 43 5 0 7 1 9 0 4 6 8 0 ");
    }
};

class TestCopy_8u_C3AC4R : public SrcSDstRoi< U8, 3, U8, 4>
{
public:
    TestCopy_8u_C3AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 3, U8, 4>( parent, "fwiCopy_8u_C3AC4R", fwiCopy_8u_C3AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6 7 8 9", "2 4 6 8 7 2 3 5 9 1 3 2", "1 2 3 8 4 5 6 5 7 8 9 2" );
        RunTest( "1 3 5 2 4 7 89 12 1"," ", "1 3 5 0 2 4 7 0 89 12 1 0");
    }
};

class TestCopy_8u_AC4C3R : public SrcSDstRoi< U8, 4, U8, 3>
{
public:
    TestCopy_8u_AC4C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 4, U8, 3>( parent, "fwiCopy_8u_AC4C3R", fwiCopy_8u_AC4C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 6 5 6 7 8 ","1 2 5 6 8 9", "0 2 4 5 6 7" );
        RunTest( "1 2 5 MAX 2 4 5 7 8 12 34 9 1 4 7 8"," ", "1 2 5 2 4 5 8 12 34 1 4 7 ");
    }
};


class TestCopy_16s_C1R : public SrcDstRoi< S16, 1, S16, 1>
{
public:
    TestCopy_16s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 1, S16, 1>( parent, "fwiCopy_16s_C1R", fwiCopy_16s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 3 255", "0 1 3 255" );
        RunTest( "0 2 4 255", "0 2 4 255" );
        RunTest( "7 5 2 6", "7 5 2 6" );
    }
};

class TestCopy_16s_C3R : public SrcDstRoi< S16, 3, S16, 3>
{
public:
    TestCopy_16s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 3, S16, 3>( parent, "fwiCopy_16s_C3R", fwiCopy_16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 255 3 5 6 7 8", "0 2 4 255 3 5 6 7 8" );
        RunTest( "0 2 4 255 1 2 43 4 5 ", "0 2 4 255 1 2 43 4 5 " );
        RunTest( "2 4 6 8 10 12 14 16 18 ", "2 4 6 8 10 12 14 16 18");

    }
};

class TestCopy_16s_C4R : public SrcDstRoi< S16, 4, S16, 4>
{
public:
    TestCopy_16s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, S16, 4>( parent, "fwiCopy_16s_C4R", fwiCopy_16s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 255 1 2 3 4 5 6 8 10 22 4 5 43", "0 2 4 255 1 2 3 4 5 6 8 10 22 4 5 43" );
        RunTest( "1 2 5 255 3 4 12 23 2 9 7 6 4 8 0 3", "1 2 5 255 3 4 12 23 2 9 7 6 4 8 0 3");
        RunTest( "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8", "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8");
    }
};

class TestCopy_16s_AC4R : public SrcSDstRoi< S16, 4, S16, 4>
{
public:
    TestCopy_16s_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 4, S16, 4>( parent, "fwiCopy_16s_AC4R", fwiCopy_16s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX","1 2 4 5", "0 2 4 5" );
        RunTest( "1 2 5 MAX 2 3 4 5","2 4 6 8 9 2 3 1", "1 2 5 8 2 3 4 1");
        RunTest( "2 4 6 8 12 43 5 6 7 1 9 10 4 6 8 9"," ", "2 4 6 0 12 43 5 0 7 1 9 0 4 6 8 0 ");
   }
};

class TestCopy_16s_C3AC4R : public SrcSDstRoi< S16, 3, S16, 4>
{
public:
    TestCopy_16s_C3AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 3, S16, 4>( parent, "fwiCopy_16s_C3AC4R", fwiCopy_16s_C3AC4R )
    {}

    virtual void RunAll()
    {
       RunTest( "1 2 3 4 5 6 7 8 9", "2 4 6 8 7 2 3 5 9 1 3 2", "1 2 3 8 4 5 6 5 7 8 9 2" );
       RunTest( "1 3 5 2 4 7 89 12 1"," ", "1 3 5 0 2 4 7 0 89 12 1 0");
    }
};

class TestCopy_16s_AC4C3R : public SrcSDstRoi< S16, 4, S16, 3>
{
public:
    TestCopy_16s_AC4C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 4, S16, 3>( parent, "fwiCopy_16s_AC4C3R", fwiCopy_16s_AC4C3R )
    {}

    virtual void RunAll()
    {
      RunTest( "0 2 4 6 5 6 7 8 ","1 2 5 6 8 9", "0 2 4 5 6 7" );
      RunTest( "1 2 5 MAX 2 4 5 7 8 12 34 9 1 4 7 8"," ", "1 2 5 2 4 5 8 12 34 1 4 7 ");
    }
};

class TestCopy_32s_C1R : public SrcDstRoi< S32, 1, S32, 1>
{
public:
    TestCopy_32s_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 1, S32, 1>( parent, "fwiCopy_32s_C1R", fwiCopy_32s_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 21 3 255", "0 21 3 255" );
        RunTest( "0 2 3 255", "0 2 3 255" );
        RunTest( "7 5 2 6", "7 5 2 6" );
    }
};

class TestCopy_32s_C3R : public SrcDstRoi< S32, 3, S32, 3>
{
public:
    TestCopy_32s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 3, S32, 3>( parent, "fwiCopy_32s_C3R", fwiCopy_32s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 255 3 5 6 7 8", "0 2 4 255 3 5 6 7 8" );
        RunTest( "0 2 4 255 1 2 43 4 5 ", "0 2 4 255 1 2 43 4 5 " );
        RunTest( "2 4 6 8 10 12 14 16 18 ", "2 4 6 8 10 12 14 16 18");

    }
};

class TestCopy_32s_C4R : public SrcDstRoi< S32, 4, S32, 4>
{
public:
    TestCopy_32s_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S32, 4, S32, 4>( parent, "fwiCopy_32s_C4R", fwiCopy_32s_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 255 1 2 3 4 5 6 8 10 22 4 5 43", "0 2 4 255 1 2 3 4 5 6 8 10 22 4 5 43" );
        RunTest( "1 2 5 255 3 4 12 23 2 9 7 6 4 8 0 3", "1 2 5 255 3 4 12 23 2 9 7 6 4 8 0 3");
        RunTest( "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8", "3 4 12 23 2 9 7 6 4 8 0 3 2 4 6 8");
    }
};

class TestCopy_32s_AC4R : public SrcSDstRoi< S32, 4, S32, 4>
{
public:
    TestCopy_32s_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 4, S32, 4>( parent, "fwiCopy_32s_AC4R", fwiCopy_32s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX","1 2 4 5", "0 2 4 5" );
        RunTest( "1 2 5 MAX 2 3 4 5","2 4 6 8 9 2 3 1", "1 2 5 8 2 3 4 1");
        RunTest( "2 4 6 8 12 43 5 6 7 1 9 10 4 6 8 9"," ", "2 4 6 0 12 43 5 0 7 1 9 0 4 6 8 0 ");
    }
};

class TestCopy_32s_C3AC4R : public SrcSDstRoi< S32, 3, S32, 4>
{
public:
    TestCopy_32s_C3AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 3, S32, 4>( parent, "fwiCopy_32s_C3AC4R", fwiCopy_32s_C3AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 4 5 6 7 8 9", "2 4 6 8 7 2 3 5 9 1 3 2", "1 2 3 8 4 5 6 5 7 8 9 2" );
        RunTest( "1 3 5 2 4 7 89 12 1"," ", "1 3 5 0 2 4 7 0 89 12 1 0");
    }
};

class TestCopy_32s_AC4C3R : public SrcSDstRoi< S32, 4, S32, 3>
{
public:
    TestCopy_32s_AC4C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 4, S32, 3>( parent, "fwiCopy_32s_AC4C3R", fwiCopy_32s_AC4C3R )
    {}

    virtual void RunAll()
    {
       RunTest( "0 2 4 6 5 6 7 8 ","1 2 5 6 8 9", "0 2 4 5 6 7" );
       RunTest( "1 2 5 MAX 2 4 5 7 8 12 34 9 1 4 7 8"," ", "1 2 5 2 4 5 8 12 34 1 4 7 ");
    }
};

// Masked only

class TestCopy_8u_C1MR : public SrcDstRoiMask< U8, 1, U8, 1>
{
public:
    TestCopy_8u_C1MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< U8, 1, U8, 1>( parent, "fwiCopy_8u_C1MR", fwiCopy_8u_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX 3 6", "0 2 0 0 3 0", "0 1 0 0 1 0" );
    }
};

class TestCopy_8u_C3MR : public SrcDstRoiMask< U8, 3, U8, 3>
{
public:
    TestCopy_8u_C3MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< U8, 3, U8, 3>( parent, "fwiCopy_8u_C3MR", fwiCopy_8u_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 4 3 5 7 8 9 10", "1 2 4 0 0 0 8 9 10", "1 0 1" );
        RunTest( "2 4 6 8 1 3 5 7 9", "2 4 6 8 1 3 0 0 0", "1 1 0" );
    }
};

class TestCopy_8u_C4MR : public SrcDstRoiMask< U8, 4, U8, 4>
{
public:
    TestCopy_8u_C4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< U8, 4, U8, 4>( parent, "fwiCopy_8u_C4MR", fwiCopy_8u_C4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 6 1 2 3 4 2 4 6 8 9 5 9 5", "0 2 4 6 0 0 0 0 2 4 6 8 9 5 9 5", "1 0 1 1" );
        RunTest( "1 2 3 4 5 6 7 8 9 10 11 12 13 3 4 6 ", " 1 2 3 4 0 0 0 0 9 10 11 12 0 0 0 0", "1 0 1 0");
    }
};

class TestCopy_8u_AC4MR : public SrcDstRoiMask< U8, 4, U8, 4>
{
public:
    TestCopy_8u_AC4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< U8, 4, U8, 4>( parent, "fwiCopy_8u_AC4MR", fwiCopy_8u_AC4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4", "2 2 4 0 0 0 0 0 9 8 9 0 3 2 3 0 ", "1 0 1 1" );
        RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "0 0 0 0 7 8 8 0 0 0 0 0 9 3 8 0", "0 1 0 1" );
    }
};


class TestCopy_16s_C1MR : public SrcDstRoiMask< S16, 1, S16, 1>
{
public:
    TestCopy_16s_C1MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S16, 1, S16, 1>( parent, "fwiCopy_16s_C1MR", fwiCopy_16s_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX 3 6", "0 2 0 0 3 0", "0 1 0 0 1 0" );
    }
};

class TestCopy_16s_C3MR : public SrcDstRoiMask< S16, 3, S16, 3>
{
public:
    TestCopy_16s_C3MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S16, 3, S16, 3>( parent, "fwiCopy_16s_C3MR", fwiCopy_16s_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 4 3 5 7 8 9 10", "1 2 4 0 0 0 8 9 10", "1 0 1" );
        RunTest( "2 4 6 8 1 3 5 7 9", "2 4 6 8 1 3 0 0 0", "1 1 0" );
    }
};

class TestCopy_16s_C4MR : public SrcDstRoiMask< S16, 4, S16, 4>
{
public:
    TestCopy_16s_C4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S16, 4, S16, 4>( parent, "fwiCopy_16s_C4MR", fwiCopy_16s_C4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 6 1 2 3 4 2 4 6 8 9 5 9 5", "0 2 4 6 0 0 0 0 2 4 6 8 9 5 9 5", "1 0 1 1" );
        RunTest( "1 2 3 4 5 6 7 8 9 10 11 12 13 3 4 6 ", " 1 2 3 4 0 0 0 0 9 10 11 12 0 0 0 0", "1 0 1 0");
    }
};

class TestCopy_16s_AC4MR : public SrcDstRoiMask< S16, 4, S16, 4>
{
public:
    TestCopy_16s_AC4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S16, 4, S16, 4>( parent, "fwiCopy_16s_AC4MR", fwiCopy_16s_AC4MR )
    {}

    virtual void RunAll()
    {
       RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4", "2 2 4 0 0 0 0 0 9 8 9 0 3 2 3 0 ", "1 0 1 1" );
       RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "0 0 0 0 7 8 8 0 0 0 0 0 9 3 8 0", "0 1 0 1" );
    }
};

class TestCopy_32s_C1MR : public SrcDstRoiMask< S32, 1, S32, 1>
{
public:
    TestCopy_32s_C1MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S32, 1, S32, 1>( parent, "fwiCopy_32s_C1MR", fwiCopy_32s_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 MAX 3 6", "0 2 0 0 3 0", "0 1 0 0 1 0" );
    }
};

class TestCopy_32s_C3MR : public SrcDstRoiMask< S32, 3, S32, 3>
{
public:
    TestCopy_32s_C3MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S32, 3, S32, 3>( parent, "fwiCopy_32s_C3MR", fwiCopy_32s_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 4 3 5 7 8 9 10", "1 2 4 0 0 0 8 9 10", "1 0 1" );
        RunTest( "2 4 6 8 1 3 5 7 9", "2 4 6 8 1 3 0 0 0", "1 1 0" );
    }
};

class TestCopy_32s_C4MR : public SrcDstRoiMask< S32, 4, S32, 4>
{
public:
    TestCopy_32s_C4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S32, 4, S32, 4>( parent, "fwiCopy_32s_C4MR", fwiCopy_32s_C4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "0 2 4 6 1 2 3 4 2 4 6 8 9 5 9 5", "0 2 4 6 0 0 0 0 2 4 6 8 9 5 9 5", "1 0 1 1" );
        RunTest( "1 2 3 4 5 6 7 8 9 10 11 12 13 3 4 6 ", " 1 2 3 4 0 0 0 0 9 10 11 12 0 0 0 0", "1 0 1 0");
    }
};

class TestCopy_32s_AC4MR : public SrcDstRoiMask< S32, 4, S32, 4>
{
public:
    TestCopy_32s_AC4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< S32, 4, S32, 4>( parent, "fwiCopy_32s_AC4MR", fwiCopy_32s_AC4MR )
    {}

    virtual void RunAll()
    {
       RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4", "2 2 4 0 0 0 0 0 9 8 9 0 3 2 3 0 ", "1 0 1 1" );
       RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "0 0 0 0 7 8 8 0 0 0 0 0 9 3 8 0", "0 1 0 1" );
    }
};

// Copying a selected channel in a multi-channel image

class TestCopy_8u_C3CR : public SrcSDstRoi< U8, 3, U8, 3>
{
public:
    TestCopy_8u_C3CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 3, U8, 3>( parent, "fwiCopy_8u_C3CR", fwiCopy_8u_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 1 3 4 8 9 10 3 5 ", "1 2 3 4 5 6 7 8 9 ", "2 2 3 4 5 6 10 8 9" );
        RunTest( "1 2 4 3 5 7 8 9 10", "2 4 6 8 9 10 12 14 16", "1 4 6 3 9 10 8 14 16" );
        RunTest( "2 4 6 8 1 3 5 7 9", "", "2 0 0 8 0 0 5 0 0" );
    }
};

class TestCopy_8u_C4CR : public SrcSDstRoi< U8, 4, U8, 4>
{
public:
    TestCopy_8u_C4CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 4, U8, 4>( parent, "fwiCopy_8u_C4CR", fwiCopy_8u_C4CR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4","1 4 5 7 9 23 4 6 12 9 8 7 5 4 3 1 ", "2 4 5 7 1 23 4 6 9 9 8 7 3 4 3 1" );
        RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2 ", "1 6 7 8 7 2 3 4 5 5 1 9 9 7 6 2" );
        RunTest( "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2","", "5 0 0 0 9 0 0 0 7 0 0 0 5 0 0 0");
    }
};

class TestCopy_16s_C3CR : public SrcSDstRoi< S16, 3, S16, 3>
{
public:
    TestCopy_16s_C3CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 3, S16, 3>( parent, "fwiCopy_16s_C3CR", fwiCopy_16s_C3CR )
    {}

    virtual void RunAll()
    {
       RunTest( "2 1 3 4 8 9 10 3 5 ", "1 2 3 4 5 6 7 8 9 ", "2 2 3 4 5 6 10 8 9" );
       RunTest( "1 2 4 3 5 7 8 9 10", "2 4 6 8 9 10 12 14 16", "1 4 6 3 9 10 8 14 16" );
       RunTest( "2 4 6 8 1 3 5 7 9", "", "2 0 0 8 0 0 5 0 0" );

    }
};

class TestCopy_16s_C4CR : public SrcSDstRoi< S16, 4, S16, 4>
{
public:
    TestCopy_16s_C4CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 4, S16, 4>( parent, "fwiCopy_16s_C4CR", fwiCopy_16s_C4CR )
    {}

    virtual void RunAll()
    {
       RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4","1 4 5 7 9 23 4 6 12 9 8 7 5 4 3 1 ", "2 4 5 7 1 23 4 6 9 9 8 7 3 4 3 1" );
       RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2 ", "1 6 7 8 7 2 3 4 5 5 1 9 9 7 6 2" );
       RunTest( "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2","", "5 0 0 0 9 0 0 0 7 0 0 0 5 0 0 0");
    }
};

class TestCopy_32s_C3CR : public SrcSDstRoi< S32, 3, S32, 3>
{
public:
    TestCopy_32s_C3CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 3, S32, 3>( parent, "fwiCopy_32s_C3CR", fwiCopy_32s_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 1 3 4 8 9 10 3 5 ", "1 2 3 4 5 6 7 8 9 ", "2 2 3 4 5 6 10 8 9" );
        RunTest( "1 2 4 3 5 7 8 9 10", "2 4 6 8 9 10 12 14 16", "1 4 6 3 9 10 8 14 16" );
        RunTest( "2 4 6 8 1 3 5 7 9", "", "2 0 0 8 0 0 5 0 0" );

    }
};

class TestCopy_32s_C4CR : public SrcSDstRoi< S32, 4, S32, 4>
{
public:
    TestCopy_32s_C4CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 4, S32, 4>( parent, "fwiCopy_32s_C4CR", fwiCopy_32s_C4CR )
    {}

    virtual void RunAll()
    {
        RunTest( "2 2 4 3 1 3 5 7 9 8 9 8 3 2 3 4","1 4 5 7 9 23 4 6 12 9 8 7 5 4 3 1 ", "2 4 5 7 1 23 4 6 9 9 8 7 3 4 3 1" );
        RunTest( "1 22 45 6 7 8 8 3 5 3 6 2 9 3 8 5", "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2 ", "1 6 7 8 7 2 3 4 5 5 1 9 9 7 6 2" );
        RunTest( "5 6 7 8 9 2 3 4 7 5 1 9 5 7 6 2","", "5 0 0 0 9 0 0 0 7 0 0 0 5 0 0 0");
    }
};

// Copying a selected channel to a one channel image

class TestCopy_8u_C3C1R : public SrcSDstRoi< U8, 3, U8, 1>
{
public:
    TestCopy_8u_C3C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 3, U8, 1>( parent, "fwiCopy_8u_C3C1R", fwiCopy_8u_C3C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3 4 5 11 33 22","2 4 8", "0 3 11" );
        RunTest( "0 2 4 6 8 10 12 14 16","1 3 5", "0 6 12" );
        RunTest( "7 5 2 6 4 3 1 0 90", " ", "7 6 1" );
    }
};

class TestCopy_8u_C4C1R : public SrcSDstRoi< U8, 4, U8, 1>
{
public:
    TestCopy_8u_C4C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 4, U8, 1>( parent, "fwiCopy_8u_C4C1R", fwiCopy_8u_C4C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3 4 5 11 33 22 12 40 32 1 3 5 7"," 1 2 3 4", "0 4 22 1" );
        RunTest( "0 2 4 6 8 10 12 14 16 1 7 5 9 32 23 8", "2 3 4 5", "0 8 16 9" );
        RunTest( "7 5 2 6 4 3 1 0 90 22 24 26 54 56 78 2", " ", "7 4 90 54" );
    }
};

class TestCopy_16s_C3C1R : public SrcSDstRoi< S16, 3, S16, 1>
{
public:
    TestCopy_16s_C3C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 3, S16, 1>( parent, "fwiCopy_16s_C3C1R", fwiCopy_16s_C3C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3 4 5 11 33 22","2 4 8", "0 3 11" );
        RunTest( "0 2 4 6 8 10 12 14 16","1 3 5", "0 6 12" );
        RunTest( "7 5 2 6 4 3 1 0 90", " ", "7 6 1" );

    }
};

class TestCopy_16s_C4C1R : public SrcSDstRoi< S16, 4, S16, 1>
{
public:
    TestCopy_16s_C4C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 4, S16, 1>( parent, "fwiCopy_16s_C4C1R", fwiCopy_16s_C4C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3 4 5 11 33 22 12 40 32 1 3 5 7"," 1 2 3 4", "0 4 22 1" );
        RunTest( "0 2 4 6 8 10 12 14 16 1 7 5 9 32 23 8", "2 3 4 5", "0 8 16 9" );
        RunTest( "7 5 2 6 4 3 1 0 90 22 24 26 54 56 78 2", " ", "7 4 90 54" );

    }
};

class TestCopy_32s_C3C1R : public SrcSDstRoi< S32, 3, S32, 1>
{
public:
    TestCopy_32s_C3C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 3, S32, 1>( parent, "fwiCopy_32s_C3C1R", fwiCopy_32s_C3C1R )
    {}

    virtual void RunAll()
    { 
        RunTest( "0 1 2 3 4 5 11 33 22","2 4 8", "0 3 11" );
        RunTest( "0 2 4 6 8 10 12 14 16","1 3 5", "0 6 12" );
        RunTest( "7 5 2 6 4 3 1 0 90", " ", "7 6 1" );
    }
};

class TestCopy_32s_C4C1R : public SrcSDstRoi< S32, 4, S32, 1>
{
public:
    TestCopy_32s_C4C1R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 4, S32, 1>( parent, "fwiCopy_32s_C4C1R", fwiCopy_32s_C4C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 2 3 4 5 11 33 22 12 40 32 1 3 5 7"," 1 2 3 4", "0 4 22 1" );
        RunTest( "0 2 4 6 8 10 12 14 16 1 7 5 9 32 23 8", "2 3 4 5", "0 8 16 9" );
        RunTest( "7 5 2 6 4 3 1 0 90 22 24 26 54 56 78 2", " ", "7 4 90 54" );
    }
};

// Copying a one-channel image to a multi-channel image

class TestCopy_8u_C1C3R : public SrcSDstRoi< U8, 1, U8, 3>
{
public:
    TestCopy_8u_C1C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 1, U8, 3>( parent, "fwiCopy_8u_C1C3R", fwiCopy_8u_C1C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "1 2 3 ","", "1 0 0 2 0 0 3 0 0" );
        RunTest( "0 2 4 ","1 3 5 7 9 2 4 6 8", "0 3 5 2 9 2 4 6 8" );
    }
};

class TestCopy_8u_C1C4R : public SrcSDstRoi< U8, 1, U8, 4>
{
public:
    TestCopy_8u_C1C4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< U8, 1, U8, 4>( parent, "fwiCopy_8u_C1C4R", fwiCopy_8u_C1C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 3 23","2 4 6 8 10 1 3 5 7 9 2 7 1 4 7 9", "0 4 6 8 1 1 3 5 3 9 2 7 23 4 7 9" );
        RunTest( "0 2 4 255"," ", "0 0 0 0 2 0 0 0 4 0 0 0 255 0 0 0" );
    }
};

class TestCopy_16s_C1C3R : public SrcSDstRoi< S16, 1, S16, 3>
{
public:
    TestCopy_16s_C1C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 1, S16, 3>( parent, "fwiCopy_16s_C1C3R", fwiCopy_16s_C1C3R )
    {}

    virtual void RunAll()
    {
      RunTest( "1 2 3 ","", "1 0 0 2 0 0 3 0 0" );
      RunTest( "0 2 4 ","1 3 5 7 9 2 4 6 8", "0 3 5 2 9 2 4 6 8" );
    }
};

class TestCopy_16s_C1C4R : public SrcSDstRoi< S16, 1, S16, 4>
{
public:
    TestCopy_16s_C1C4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S16, 1, S16, 4>( parent, "fwiCopy_16s_C1C4R", fwiCopy_16s_C1C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 3 23","2 4 6 8 10 1 3 5 7 9 2 7 1 4 7 9", "0 4 6 8 1 1 3 5 3 9 2 7 23 4 7 9" );
        RunTest( "0 2 4 255"," ", "0 0 0 0 2 0 0 0 4 0 0 0 255 0 0 0" );

    }
};

class TestCopy_32s_C1C3R : public SrcSDstRoi< S32, 1, S32, 3>
{
public:
    TestCopy_32s_C1C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 1, S32, 3>( parent, "fwiCopy_32s_C1C3R", fwiCopy_32s_C1C3R )
    {}

    virtual void RunAll()
    {
      RunTest( "1 2 3 ","", "1 0 0 2 0 0 3 0 0" );
      RunTest( "0 2 4 ","1 3 5 7 9 2 4 6 8", "0 3 5 2 9 2 4 6 8" );

    }
};

class TestCopy_32s_C1C4R : public SrcSDstRoi< S32, 1, S32, 4>
{
public:
    TestCopy_32s_C1C4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< S32, 1, S32, 4>( parent, "fwiCopy_32s_C1C4R", fwiCopy_32s_C1C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "0 1 3 23","2 4 6 8 10 1 3 5 7 9 2 7 1 4 7 9", "0 4 6 8 1 1 3 5 3 9 2 7 23 4 7 9" );
        RunTest( "0 2 4 255"," ", "0 0 0 0 2 0 0 0 4 0 0 0 255 0 0 0" );
    }
};

// Splitting into separate planes

class TestCopy_8u_C3P3R : public SrcPDstRoi< U8, 3, U8, 3>
{
public:
    TestCopy_8u_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 3, U8, 3>( parent, "fwiCopy_8u_C3P3R", fwiCopy_8u_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[3] = {"0 MAX 6","1 4 7","3 5 8"};
        RunTest( "0 1 3 MAX 4 5 6 7 8", dstStr );
    }
};

class TestCopy_16s_C3P3R : public SrcPDstRoi< S16, 3, S16, 3>
{
public:
    TestCopy_16s_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< S16, 3, S16, 3>( parent, "fwiCopy_16s_C3P3R", fwiCopy_16s_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[3] = {"9 45 6","1 4 7","3 5 8"};
        RunTest( "9 1 3 45 4 5 6 7 8", dstStr );
    }
};

class TestCopy_32s_C3P3R : public SrcPDstRoi< S32, 3, S32, 3>
{
public:
    TestCopy_32s_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< S32, 3, S32, 3>( parent, "fwiCopy_32s_C3P3R", fwiCopy_32s_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[3] = {"0 5 6","1 4 7","3 5 8"};
        RunTest( "0 1 3 5 4 5 6 7 8", dstStr );
    }
};

class TestCopy_8u_C4P4R : public SrcPDstRoi< U8, 4, U8, 4>
{
public:
    TestCopy_8u_C4P4R( UnitTestCatalogBase & parent ) : SrcPDstRoi< U8, 4, U8, 4>( parent, "fwiCopy_8u_C4P4R", fwiCopy_8u_C4P4R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[4] = {"0 MAX 6 7","1 4 7 6","3 5 8 7", "1 2 3 5"};
        RunTest( "0 1 3 1 MAX 4 5 2 6 7 8 3 7 6 7 5", dstStr );
    }
};

class TestCopy_16s_C4P4R : public SrcPDstRoi< S16, 4, S16, 4>
{
public:
    TestCopy_16s_C4P4R( UnitTestCatalogBase & parent ) : SrcPDstRoi< S16, 4, S16, 4>( parent, "fwiCopy_16s_C4P4R", fwiCopy_16s_C4P4R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[4] = {"9 45 6 3","1 4 7 8","3 5 8 2","4 2 1 0"};
        RunTest( "9 1 3 4 45 4 5 2 6 7 8 1 3 8 2 0", dstStr );
    }
};

class TestCopy_32s_C4P4R : public SrcPDstRoi< S32, 4, S32, 4>
{
public:
    TestCopy_32s_C4P4R( UnitTestCatalogBase & parent ) : SrcPDstRoi< S32, 4, S32, 4>( parent, "fwiCopy_32s_C4P4R", fwiCopy_32s_C4P4R )
    {}

    virtual void RunAll()
    {
       const char *dstStr[4] = {"9 45 6 3","1 4 7 8","3 5 8 2","4 2 1 0"};
       RunTest( "9 1 3 4 45 4 5 2 6 7 8 1 3 8 2 0", dstStr );
    }
};

//Composing from Separate Planes


class TestCopy_8u_P3C3R : public PSrcDstRoi< U8, 3, U8, 3>
{
public:
    TestCopy_8u_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 3, U8, 3>( parent, "fwiCopy_8u_P3C3R", fwiCopy_8u_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"0 9 6","1 4 7","3 5 8"};
        RunTest( srcStr, "0 1 3 9 4 5 6 7 8" );
    }
};

class TestCopy_16s_P3C3R : public PSrcDstRoi< S16, 3, S16, 3>
{
public:
    TestCopy_16s_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< S16, 3, S16, 3>( parent, "fwiCopy_16s_P3C3R", fwiCopy_16s_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"2 9 6","1 4 7","3 5 8"};
        RunTest( srcStr, "2 1 3 9 4 5 6 7 8" );
    }
};

class TestCopy_32s_P3C3R : public PSrcDstRoi< S32, 3, S32, 3>
{
public:
    TestCopy_32s_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< S32, 3, S32, 3>( parent, "fwiCopy_32s_P3C3R", fwiCopy_32s_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"21 99 6","1 4 7","33 5 8"};
        RunTest( srcStr, "21 1 33 99 4 5 6 7 8" );
    }
};

class TestCopy_8u_P4C4R : public PSrcDstRoi< U8, 4, U8, 4>
{
public:
    TestCopy_8u_P4C4R( UnitTestCatalogBase & parent ) : PSrcDstRoi< U8, 4, U8, 4>( parent, "fwiCopy_8u_P4C4R", fwiCopy_8u_P4C4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"0 9 6 5","1 4 7 8","3 5 8 6","1 2 3 4"};
        RunTest( srcStr, "0 1 3 1 9 4 5 2 6 7 8 3 5 8 6 4" );
    }
};

class TestCopy_16s_P4C4R : public PSrcDstRoi< S16, 4, S16, 4>
{
public:
    TestCopy_16s_P4C4R( UnitTestCatalogBase & parent ) : PSrcDstRoi< S16, 4, S16, 4>( parent, "fwiCopy_16s_P4C4R", fwiCopy_16s_P4C4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"0 9 6 5","1 4 7 8","3 5 8 6","1 2 3 4"};
        RunTest( srcStr, "0 1 3 1 9 4 5 2 6 7 8 3 5 8 6 4" );
    }
};

class TestCopy_32s_P4C4R : public PSrcDstRoi< S32, 4, S32, 4>
{
public:
    TestCopy_32s_P4C4R( UnitTestCatalogBase & parent ) : PSrcDstRoi< S32, 4, S32, 4>( parent, "fwiCopy_32s_P4C4R", fwiCopy_32s_P4C4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"0 9 6 5","1 4 7 8","3 5 8 6","1 2 3 4"};
        RunTest( srcStr, "0 1 3 1 9 4 5 2 6 7 8 3 5 8 6 4" );
    }
};

//Floating point 

class TestCopy_32f_C1R : public SrcDstRoi< F32, 1, F32, 1 >
{
public:
    TestCopy_32f_C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 1, F32, 1 >( parent, "fwiCopy_32f_C1R", fwiCopy_32f_C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433", "3.14159 2.6535 8.9793 2.3846 2.6433", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_C3R : public SrcDstRoi< F32, 3, F32, 3 >
{
public:
    TestCopy_32f_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 3, F32, 3 >( parent, "fwiCopy_32f_C3R", fwiCopy_32f_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212", "3.14159 2.6535 8.9793 2.3846 2.6433 1.212", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_C4R : public SrcDstRoi< F32, 4, F32, 4 >
{
public:
    TestCopy_32f_C4R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 4, F32, 4 >( parent, "fwiCopy_32f_C4R", fwiCopy_32f_C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212 2.001 4.546", "3.14159 2.6535 8.9793 2.3846 2.6433 1.212 2.001 4.546", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_AC4R : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestCopy_32f_AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiCopy_32f_AC4R", fwiCopy_32f_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212 2.001 4.546", "8.3279 5.0288 4.1971 6.9399 3.7510 0 2.1 4.567",
                 "3.14159 2.6535 8.9793 6.9399 2.6433 1.212 2.001 4.567", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_C3AC4R : public SrcSDstRoi< F32, 3, F32, 4 >
{
public:
    TestCopy_32f_C3AC4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 3, F32, 4 >( parent, "fwiCopy_32f_C3AC4R", fwiCopy_32f_C3AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212 ", "8.3279 5.0288 4.1971 6.9399 3.7510 0 2.1 4.567", 
                 "3.14159 2.6535 8.9793 6.9399 2.3846 2.6433 1.212 4.567", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_AC4C3R : public SrcDstRoi< F32, 4, F32, 3 >
{
public:
    TestCopy_32f_AC4C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 4, F32, 3 >( parent, "fwiCopy_32f_AC4C3R", fwiCopy_32f_AC4C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212 2.001 4.546", "3.14159 2.6535 8.9793 2.6433 1.212 2.001", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_C3CR : public SrcSDstRoi< F32, 3, F32, 3 >
{
public:
    TestCopy_32f_C3CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 3, F32, 3 >( parent, "fwiCopy_32f_C3CR", fwiCopy_32f_C3CR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 1.212", "8.3279 5.0288 4.1971 6.9399 3.7510 4.567",
                 "3.14159 5.0288 4.1971 2.3846 3.7510 4.567", (F32) 0.0001 ); //Pi's digit
    }
};

class TestCopy_32f_C4CR : public SrcSDstRoi< F32, 4, F32, 4 >
{
public:
    TestCopy_32f_C4CR( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 4, F32, 4 >( parent, "fwiCopy_32f_C4CR", fwiCopy_32f_C4CR )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "8.3279 5.0288 4.1971 6.9399 3.7510 0 3.2135 8.2489",
                 "3.14159 5.0288 4.1971 6.9399 2.6433 0 3.2135 8.2489", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", "7.2 100.3 3.4 2.0 2.001 2.3001 2.10 7.89", 
                 "5.1 100.3 3.4 2.0 1.0 2.3001 2.10 7.89", (F32) 0.0001 );
    }
};

class TestCopy_32f_C3C1R : public SrcDstRoi< F32, 3, F32, 1 >
{
public:
    TestCopy_32f_C3C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 3, F32, 1 >( parent, "fwiCopy_32f_C3C1R", fwiCopy_32f_C3C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0","3.14159 2.3846", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0", "5.1 100.78", (F32) 0.0001 );
    }
};

class TestCopy_32f_C4C1R : public SrcDstRoi< F32, 4, F32, 1 >
{
public:
    TestCopy_32f_C4C1R( UnitTestCatalogBase & parent ) : SrcDstRoi< F32, 4, F32, 1 >( parent, "fwiCopy_32f_C4C1R", fwiCopy_32f_C4C1R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6535 8.9793 2.3846 2.6433 0 2.3467 5.7891", "3.14159 2.6433", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73","5.1 1.0", (F32) 0.0001 );
    }
};

class TestCopy_32f_C1C3R : public SrcSDstRoi< F32, 1, F32, 3 >
{
public:
    TestCopy_32f_C1C3R( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 1, F32, 3 >( parent, "fwiCopy_32f_C1C3R", fwiCopy_32f_C1C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.3846", " ", "3.14159 0 0 2.3846 0 0", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 100.78","3.5 6.2 16.45678 4.4 1.0 0 ", "5.1 6.2 16.45678 100.78 1.0 0",  (F32) 0.0001 );
    }
};

class TestCopy_32f_C1C4R : public SrcSDstRoi< F32, 1, F32, 4 >
{
public:
    TestCopy_32f_C1C4R( UnitTestCatalogBase & parent ) : SrcSDstRoi< F32, 1, F32, 4 >( parent, "fwiCopy_32f_C1C4R", fwiCopy_32f_C1C4R )
    {}

    virtual void RunAll()
    {
        RunTest( "3.14159 2.6433", " ", "3.14159 0 0 0 2.6433 0 0 0", (F32) 0.0001 ); //Pi's digit
        RunTest( "5.1 1.0", "1.2 6.2 16.45678 100.78 6.6 0 3.45 5.73 ","5.1 6.2 16.45678 100.78 1.0 0 3.45 5.73", (F32) 0.0001 );
    }
};

class TestCopy_32f_C3P3R : public SrcPDstRoi< F32, 3, F32, 3>
{
public:
    TestCopy_32f_C3P3R( UnitTestCatalogBase & parent ) : SrcPDstRoi< F32, 3, F32, 3>( parent, "fwiCopy_32f_C3P3R", fwiCopy_32f_C3P3R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[3] = {"2.3 9.245 8.99", "4.05 1.232 9.67", "6.786 7.89 4.56"};
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 8.99 9.67 4.56", dstStr );
    }
};

class TestCopy_32f_C4P4R : public SrcPDstRoi< F32, 4, F32, 4>
{
public:
    TestCopy_32f_C4P4R( UnitTestCatalogBase & parent ) : SrcPDstRoi< F32, 4, F32, 4>( parent, "fwiCopy_32f_C4P4R", fwiCopy_32f_C4P4R )
    {}

    virtual void RunAll()
    {
        const char *dstStr[4] = {"2.3 1.232 3.5 3.14159","4.05 7.89 6.2 2.6535", "6.786 5.76 16.45678 8.9793 ", "9.245 3.33 21.25 30.789"};
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33 3.5 6.2 16.45678 21.25 3.14159 2.6535 8.9793 30.789", dstStr );
    }
};

class TestCopy_32f_P3C3R : public PSrcDstRoi< F32, 3, F32, 3>
{
public:
    TestCopy_32f_P3C3R( UnitTestCatalogBase & parent ) : PSrcDstRoi< F32, 3, F32, 3>( parent, "fwiCopy_32f_P3C3R", fwiCopy_32f_P3C3R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[3] = {"2.3 9.245 8.99", "4.05 1.232 9.67", "6.786 7.89 4.56"};
        RunTest( srcStr, "2.3 4.05 6.786 9.245 1.232 7.89 8.99 9.67 4.56" );
    }
};

class TestCopy_32f_P4C4R : public PSrcDstRoi< F32, 4, F32, 4>
{
public:
    TestCopy_32f_P4C4R( UnitTestCatalogBase & parent ) : PSrcDstRoi< F32, 4, F32, 4>( parent, "fwiCopy_32f_P4C4R", fwiCopy_32f_P4C4R )
    {}

    virtual void RunAll()
    {
        const char *srcStr[4] = {"2.3 1.232 3.5 3.14159","4.05 7.89 6.2 2.6535", "6.786 5.76 16.45678 8.9793 ", "9.245 3.33 21.25 30.789"};
        RunTest( srcStr, "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33 3.5 6.2 16.45678 21.25 3.14159 2.6535 8.9793 30.789" );
    }
};

// Masked Floating point
class TestCopy_32f_C1MR : public SrcDstRoiMask< F32, 1, F32, 1>
{
public:
    TestCopy_32f_C1MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< F32, 1, F32, 1>( parent, "fwiCopy_32f_C1MR", fwiCopy_32f_C1MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89", "0 4.05 0 0 1.232 0", "0 1 0 0 1 0" );
    }
};

class TestCopy_32f_C3MR : public SrcDstRoiMask< F32, 3, F32, 3>
{
public:
    TestCopy_32f_C3MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< F32, 3, F32, 3>( parent, "fwiCopy_32f_C3MR", fwiCopy_32f_C3MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 8.99 9.67 4.56", "2.3 4.05 6.786 0 0 0 0 0 0", "1 0 0" );
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 8.99 9.67 4.56", "2.3 4.05 6.786 9.245 1.232 7.89 0 0 0", "1 1 0" );
    }
};

class TestCopy_32f_C4MR : public SrcDstRoiMask< F32, 4, F32, 4>
{
public:
    TestCopy_32f_C4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< F32, 4, F32, 4>( parent, "fwiCopy_32f_C4MR", fwiCopy_32f_C4MR )
    {}

    virtual void RunAll()
    {
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33", "0 0 0 0 1.232 7.89 5.76 3.33", "0 1" );
        RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33 3.5 6.2 16.45678 21.25 3.14159 2.6535 8.9793 30.789 ", 
                 "2.3 4.05 6.786 9.245 0 0 0 0 3.5 6.2 16.45678 21.25 0 0 0 0", "1 0 1 0");
    }
};

class TestCopy_32f_AC4MR : public SrcDstRoiMask< F32, 4, F32, 4>
{
public:
    TestCopy_32f_AC4MR( UnitTestCatalogBase & parent ) : SrcDstRoiMask< F32, 4, F32, 4>( parent, "fwiCopy_32f_AC4MR", fwiCopy_32f_AC4MR )
    {}

    virtual void RunAll()
    {
       RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33", "0 0 0 0 1.232 7.89 5.76 0", "0 1" );
       RunTest( "2.3 4.05 6.786 9.245 1.232 7.89 5.76 3.33 3.5 6.2 16.45678 21.25 3.14159 2.6535 8.9793 30.789 ", 
                "2.3 4.05 6.786 0 0 0 0 0 3.5 6.2 16.45678 0 0 0 0 0", "1 0 1 0");
    }
};



DEFINE_TEST_TABLE( CopyTestCatalog )
TEST_ENTRY( TestCopy_8u_C1R     )
TEST_ENTRY( TestCopy_8u_C3R     )
TEST_ENTRY( TestCopy_8u_C4R     )
TEST_ENTRY( TestCopy_8u_AC4R    )
TEST_ENTRY( TestCopy_8u_C3AC4R  )
TEST_ENTRY( TestCopy_8u_AC4C3R  )
TEST_ENTRY( TestCopy_16s_C1R    )
TEST_ENTRY( TestCopy_16s_C3R    )
TEST_ENTRY( TestCopy_16s_C4R    )
TEST_ENTRY( TestCopy_16s_AC4R   )
TEST_ENTRY( TestCopy_16s_C3AC4R )
TEST_ENTRY( TestCopy_16s_AC4C3R )
TEST_ENTRY( TestCopy_32s_C1R    )
TEST_ENTRY( TestCopy_32s_C3R    )
TEST_ENTRY( TestCopy_32s_C4R    )
TEST_ENTRY( TestCopy_32s_AC4R   )
TEST_ENTRY( TestCopy_32s_C3AC4R )
TEST_ENTRY( TestCopy_32s_AC4C3R )

TEST_ENTRY( TestCopy_8u_C1MR    )
TEST_ENTRY( TestCopy_8u_C3MR    )
TEST_ENTRY( TestCopy_8u_C4MR    )
TEST_ENTRY( TestCopy_8u_AC4MR   )
TEST_ENTRY( TestCopy_16s_C1MR   )
TEST_ENTRY( TestCopy_16s_C3MR   )
TEST_ENTRY( TestCopy_16s_C4MR   )
TEST_ENTRY( TestCopy_16s_AC4MR  )
TEST_ENTRY( TestCopy_32s_C1MR   )
TEST_ENTRY( TestCopy_32s_C3MR   )
TEST_ENTRY( TestCopy_32s_C4MR   )
TEST_ENTRY( TestCopy_32s_AC4MR  )

TEST_ENTRY( TestCopy_8u_C3CR    )
TEST_ENTRY( TestCopy_8u_C4CR    )
TEST_ENTRY( TestCopy_16s_C3CR   )
TEST_ENTRY( TestCopy_16s_C4CR   )
TEST_ENTRY( TestCopy_32s_C3CR   )
TEST_ENTRY( TestCopy_32s_C4CR   )

TEST_ENTRY( TestCopy_8u_C3C1R   )
TEST_ENTRY( TestCopy_8u_C4C1R   )
TEST_ENTRY( TestCopy_16s_C3C1R  )
TEST_ENTRY( TestCopy_16s_C4C1R  )
TEST_ENTRY( TestCopy_32s_C3C1R  )
TEST_ENTRY( TestCopy_32s_C4C1R  )

TEST_ENTRY( TestCopy_8u_C1C3R   )
TEST_ENTRY( TestCopy_8u_C1C4R   )
TEST_ENTRY( TestCopy_16s_C1C3R  )
TEST_ENTRY( TestCopy_16s_C1C4R  )
TEST_ENTRY( TestCopy_32s_C1C3R  )
TEST_ENTRY( TestCopy_32s_C1C4R  )

TEST_ENTRY( TestCopy_8u_C3P3R   )
TEST_ENTRY( TestCopy_16s_C3P3R  )
TEST_ENTRY( TestCopy_32s_C3P3R  )
TEST_ENTRY( TestCopy_8u_C4P4R   )
TEST_ENTRY( TestCopy_16s_C4P4R  )
TEST_ENTRY( TestCopy_32s_C4P4R  )

TEST_ENTRY( TestCopy_8u_P3C3R   )
TEST_ENTRY( TestCopy_16s_P3C3R  )
TEST_ENTRY( TestCopy_32s_P3C3R  )
TEST_ENTRY( TestCopy_8u_P4C4R   )
TEST_ENTRY( TestCopy_16s_P4C4R  )
TEST_ENTRY( TestCopy_32s_P4C4R  )

TEST_ENTRY( TestCopy_32f_C1R    )
TEST_ENTRY( TestCopy_32f_C3R    )
TEST_ENTRY( TestCopy_32f_C4R    )
TEST_ENTRY( TestCopy_32f_AC4R   )
TEST_ENTRY( TestCopy_32f_C3AC4R )
TEST_ENTRY( TestCopy_32f_AC4C3R )

TEST_ENTRY( TestCopy_32f_C3CR  )
TEST_ENTRY( TestCopy_32f_C4CR  )
TEST_ENTRY( TestCopy_32f_C3C1R )
TEST_ENTRY( TestCopy_32f_C4C1R )
TEST_ENTRY( TestCopy_32f_C1C3R )
TEST_ENTRY( TestCopy_32f_C1C4R )

TEST_ENTRY( TestCopy_32f_C3P3R )  
TEST_ENTRY( TestCopy_32f_C4P4R ) 
TEST_ENTRY( TestCopy_32f_P3C3R ) 
TEST_ENTRY( TestCopy_32f_P4C4R ) 

TEST_ENTRY( TestCopy_32f_C1MR ) 
TEST_ENTRY( TestCopy_32f_C3MR ) 
TEST_ENTRY( TestCopy_32f_C4MR ) 
TEST_ENTRY( TestCopy_32f_AC4MR) 

END_TEST_TABLE()
