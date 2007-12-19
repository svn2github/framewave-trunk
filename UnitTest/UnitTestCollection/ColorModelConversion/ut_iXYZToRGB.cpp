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
//                        XYZToRGB
////////////////////////////////////////////////////////////////
class iXYZToRGB_8u_C3R : public SrcDstRoi< U8, 3, U8, 3 >
{
public:
    iXYZToRGB_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U8, 3 >( parent, "iXYZToRGB_8u_C3R", fwiXYZToRGB_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "255 241 231 255 231   0 255   0 255 255   0  14   0 255 217   0 255   0   0  10 255   0   0   0", 1 );
    }
};

class iXYZToRGB_8u_AC4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    iXYZToRGB_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U8, 4 >( parent, "iXYZToRGB_8u_AC4R", fwiXYZToRGB_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "255 241 231 0 255 231   0 0 255   0 255 0 255   0  14 0   0 255 217 0   0 255   0 0   0  10 255 0   0   0   0 0", 1 );
    }
};

class iXYZToRGB_16u_C3R : public SrcDstRoi< U16, 3, U16, 3 >
{
public:
    iXYZToRGB_16u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 3, U16, 3 >( parent, "iXYZToRGB_16u_C3R", fwiXYZToRGB_16u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "65535 62147 59566 65535 59423 0 65535 0 65535 65535 0 3646 0 65535 55919 0 65535 0 0 2723 65535 0 0 0", 1 );
    }
};

class iXYZToRGB_16u_AC4R : public SrcDstRoi< U16, 4, U16, 4 >
{
public:
    iXYZToRGB_16u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U16, 4, U16, 4 >( parent, "iXYZToRGB_16u_AC4R", fwiXYZToRGB_16u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "65535 62147 59566 0 65535 59423 0 0 65535 0 65535 0 65535 0 3646 0 0 65535 55919 0 0 65535 0 0 0 2723 65535 0 0 0 0 0", 1 );
    }
};

class iXYZToRGB_16s_C3R : public SrcDstRoi< S16, 3, S16, 3 >
{
public:
    iXYZToRGB_16s_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 3, S16, 3 >( parent, "iXYZToRGB_16s_C3R", fwiXYZToRGB_16s_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MAX MAX MIN MAX MIN MAX MAX MIN MIN MIN MAX MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN", 
                 "32767 29378 26797 32767 26654 -32768 32767 -32768 32767 32767 -32768 -29121 -32768 32767 23150 -32768 32767 -32768 -32768 -30044 32767 -32768 -32768 -32768", 5 );
    }
};

class iXYZToRGB_16s_AC4R : public SrcDstRoi< S16, 4, S16, 4>
{
public:
    iXYZToRGB_16s_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< S16, 4, S16, 4>( parent, "iXYZToRGB_16s_AC4R", fwiXYZToRGB_16s_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MAX MAX MIN 0 MAX MIN MAX 0 MAX MIN MIN 0 MIN MAX MAX 0 MIN MAX MIN 0 MIN MIN MAX 0 MIN MIN MIN 0", 
                 "32767 29378 26797 0 32767 26654 -32768 0 32767 -32768 32767 0 32767 -32768 -29121 0 -32768 32767 23150 0 -32768 32767 -32768 0 -32768 -30044 32767 0 -32768 -32768 -32768 0", 5 );
    }
};


//////////////////////////
// TEST TABLE DEFINITION
//////////////////////////
DEFINE_TEST_TABLE( iXYZToRGB_Chapter )
TEST_ENTRY( iXYZToRGB_8u_C3R   )
TEST_ENTRY( iXYZToRGB_8u_AC4R  )
TEST_ENTRY( iXYZToRGB_16u_C3R  )
TEST_ENTRY( iXYZToRGB_16u_AC4R )
TEST_ENTRY( iXYZToRGB_16s_C3R  )
TEST_ENTRY( iXYZToRGB_16s_AC4R )
END_TEST_TABLE()

