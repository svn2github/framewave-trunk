/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#include "UnitTestFramework.h"
#include "CMC_Tests.h"

#include "CMC_FunctionObjects.h"
#include "FunctionSignatures.h"
#include "fwImage.h"

////////////////////////////////////////////////////////////////
//                        YUVToRGB
////////////////////////////////////////////////////////////////
class iYUVToRGB_8u_C3R : public SrcDstRoi< U8, 3, U8, 3 >
{
public:
    iYUVToRGB_8u_C3R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 3, U8, 3 >( parent, "iYUVToRGB_8u_C3R", fwiYUVToRGB_8u_C3R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX MIN MIN MIN MAX MIN MAX MIN MIN MIN MAX MIN MIN MIN MAX MAX MAX MAX MIN", 
                 "255 131 255   0 124   0 255 231   0   0 124   0 109 255   0 144   0 255 109 255 255", 1 );
    }
};


class iYUVToRGB_8u_AC4R : public SrcDstRoi< U8, 4, U8, 4 >
{
public:
    iYUVToRGB_8u_AC4R( UnitTestCatalogBase & parent ) : SrcDstRoi< U8, 4, U8, 4 >( parent, "iYUVToRGB_8u_AC4R", fwiYUVToRGB_8u_AC4R )
    {}

    virtual void RunAll()
    {
        RunTest( "MAX MAX MAX 0 MIN MIN MIN 0 MAX MIN MAX 0 MIN MIN MIN 0 MAX MIN MIN 0 MIN MAX MAX 0 MAX MAX MIN 0", 
                 "255 131 255 0   0 124   0 0 255 231   0 0   0 124   0 0 109 255   0 0 144   0 255 0 109 255 255 0", 1 );
    }
};

//////////////////////////
// TEST TABLE DEFINITION
//////////////////////////
DEFINE_TEST_TABLE( iYUVToRGB_Chapter )

TEST_ENTRY( iYUVToRGB_8u_C3R )
TEST_ENTRY( iYUVToRGB_8u_AC4R )

END_TEST_TABLE()

