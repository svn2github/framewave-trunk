/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "CMC_Tests.h"

#include "CMC_FunctionObjects.h"
#include "FunctionSignatures.h"
#include "fwImage.h"

////////////////////////////////////////////////////////////////
//                        RGBToXYZ
////////////////////////////////////////////////////////////////
class iRGBToXYZ_8u_C3R : public SrcDstRoi< U8, 3, U8, 3 >
{
public:
    iRGBToXYZ_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U8, 3 >( parent, "iRGBToXYZ_8u_C3R", fwiRGBToXYZ_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "242 255 255 196 236  35 151  72 247 105  54   4 137 200 255  91 182  30  46  18 242   0   0   0", 1 );
    }
};

class iRGBToXYZ_8u_AC4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    iRGBToXYZ_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U8, 4 >( parent, "iRGBToXYZ_8u_AC4R", fwiRGBToXYZ_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "242 255 255 0 196 236  35 0 151  72 247 0 105  54   4 0 137 200 255 0  91 182  30 0  46  18 242 0   0   0   0 0 ", 1 );
    }
};

class iRGBToXYZ_16u_C3R : public SrcDstRoi< U16, 3, U16, 3 >
{
public:
    iRGBToXYZ_16u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 3, U16, 3 >( parent, "iRGBToXYZ_16u_C3R", fwiRGBToXYZ_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "62289 65535 65535 50464 60806 9078 38854 18667 63541 27030 13937 1267 35258 51598 65535 23434 46868 7811 11824 4729 62274 0 0 0", 5 );
    }
};

class iRGBToXYZ_16u_AC4R : public SrcDstRoi< U16, 4, U16, 4 >
{
public:
    iRGBToXYZ_16u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, U16, 4 >( parent, "iRGBToXYZ_16u_AC4R", fwiRGBToXYZ_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "62289 65535 65535 0 50464 60806 9078 0 38854 18667 63541 0 27030 13937 1267 0 35258 51598 65535 0 23434 46868 7811 0 11824 4729 62274 0 0 0 0 0", 5 );
    }
};

class iRGBToXYZ_16s_C3R : public SrcDstRoi< S16, 3, S16, 3 >
{
public:
    iRGBToXYZ_16s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 3, S16, 3 >( parent, "iRGBToXYZ_16s_C3R", fwiRGBToXYZ_16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "29520, 32767, 32767, 17696, 28037, -23689, 6086, -14101, 30772, -5737, -18830, -31500, 2490, 18829, 32767, -9333, 14100, -24956, -20943, -28038, 29505, -32768, -32768, -32768", 5 );
    }
};

class iRGBToXYZ_16s_AC4R : public SrcDstRoi< S16, 4, S16, 4 >
{
public:
    iRGBToXYZ_16s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, S16, 4 >( parent, "iRGBToXYZ_16s_AC4R", fwiRGBToXYZ_16s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "29520 32767 32767 0 17696 28037 -23689 0 6086 -14101 30772 0 -5737 -18830 -31500 0 2490 18829 32767 0 -9333 14100 -24956 0 -20943 -28038 29505 0 -32768 -32768 -32768 0", 5 );
    }
};


//////////////////////////
// TEST TABLE DEFINITION
//////////////////////////
DEFINE_TEST_TABLE( iRGBToXYZ_Chapter )
TEST_ENTRY( iRGBToXYZ_8u_C3R   )
TEST_ENTRY( iRGBToXYZ_8u_AC4R  )
TEST_ENTRY( iRGBToXYZ_16u_C3R  )
TEST_ENTRY( iRGBToXYZ_16u_AC4R )
TEST_ENTRY( iRGBToXYZ_16s_C3R  )
TEST_ENTRY( iRGBToXYZ_16s_AC4R )
END_TEST_TABLE()

