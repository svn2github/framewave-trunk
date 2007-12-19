/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#if !defined(SOL32) && !defined(SOL64)

#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "Object.h"
#include "fwImage.h"

class TestFilterBox_8u_C1R : public FunctionObjectBase
{
    ChannelBuffer<U8, 1> src, dst, dstExpected;

    const Fw8u *pSrc;
    int srcStep;
    Fw8u *pDst;
    int dstStep;
    FwiSize dstRoiSize;
    FwiSize maskSize;
    FwiPoint anchor;

public:
	TestFilterBox_8u_C1R( UnitTestCatalogBase & parent )
        : FunctionObjectBase( parent, "fwiFilterBox_8u_C1R" )
    {}

    virtual void ResetFnParams( const AdviceBase &adv )
    {
        const int wSrc = 6;
        const int hSrc = 5;

        const int wDst = 4;
        const int hDst = 3;

        maskSize.width = 3; maskSize.height = 3;

        src.Reset(  " 100   1 201  31  54 125 "
                    "  46  77 168  49  10  11 "
                    " 112 113  54 215 255 196 "
                    " 238  65  20 121  22 213 "
                    " 240 255  26 127   8  90 ", wSrc, hSrc, adv.Alignment() );
        dst.Reset( "", wDst, hDst, adv.Alignment() );
        dstExpected.Reset(  "  96 101 115 105 "
                            "  99  98 101 121 "
                            " 124 110  94 138 ", wDst, hDst, adv.Alignment() );

        pSrc = src.Ptr();
        pSrc += (wSrc+1);
        srcStep = wSrc * sizeof(U8);
        pDst = dst.Ptr();
        dstStep = wDst * sizeof(U8);
        dstRoiSize.width = wDst; dstRoiSize.height = hDst;
        anchor.x = 1; anchor.y = 1;
    }

    virtual bool Eval()
    {
        return dst.Eval(Log(), dstExpected);
    }

    virtual FwStatus CallFn()
    {
        return fwiFilterBox_8u_C1R(pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor);
    }

    virtual void RunAll()
    {
        ExecuteTest(fwStsNoErr);
    }
};

DEFINE_TEST_TABLE( FilterBoxTestCatalog )

TEST_ENTRY( TestFilterBox_8u_C1R )

END_TEST_TABLE()

#endif
