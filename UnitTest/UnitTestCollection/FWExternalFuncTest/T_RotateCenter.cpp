/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "UnitTestFramework.h"
#include "FW_Tests.h"

#include "Object.h"
#include "fwImage.h"

class TestRotateCenter_8u_C1R : public FunctionObjectBase
{
    ChannelBuffer<U8, 1> src;
    ChannelBuffer<U8, 1> dst;
    ChannelBuffer<U8, 1> dstExpected;

    const Fw8u *pSrc;
    FwiSize srcSize;
    int srcStep;
    FwiRect srcRoi;
    Fw8u *pDst;
    int dstStep;
    FwiRect dstRoi;
    double angle;
    double xCenter;
    double yCenter;
    int interpolation;

public:
	TestRotateCenter_8u_C1R( UnitTestCatalogBase & parent )
        : FunctionObjectBase( parent, "fwiRotateCenter_8u_C1R" )
    {}

    virtual void ResetFnParams( const AdviceBase &adv )
    {
        const int w = 5;
        const int h = 5;

        src.Reset(  "  0  1  2  3  4 "
                    "  5  6  7  8  9 "
                    " 10 11 12 13 14 "
                    " 15 16 17 18 19 "
                    " 20 21 22 23 24 ", w, h );
        dst.Reset("", w, h);
        dstExpected.Reset(  " 4  9 14 19 24 "
                            " 3  8 13 18 23 "
                            " 2  7 12 17 22 "
                            " 1  6 11 16 21 "
                            " 0  5 10 15 20 ", w, h );

        pSrc = src.Ptr();
        srcSize.width = w; srcSize.height = h;
        srcStep = w * sizeof(U8);
        srcRoi.x = 0; srcRoi.y = 0; srcRoi.width = w; srcRoi.height = h;
        pDst = dst.Ptr();
        dstStep = w * sizeof(U8);
        dstRoi.x = 0; dstRoi.y = 0; dstRoi.width = w; dstRoi.height = h;
        angle = 90.0;
        xCenter = (w-1)/2;
        yCenter = (h-1)/2;
        interpolation = FWI_INTER_NN;
    }

    virtual bool Eval()
    {
        return dst.Eval(Log(), dstExpected);
    }

    virtual FwStatus CallFn()
    {
        return fwiRotateCenter_8u_C1R(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
    }

    virtual void RunAll()
    {
        ExecuteTest(fwStsNoErr);
    }
};

class TestRotateCenter_16u_AC4R : public FunctionObjectBase
{
    const static int numChannels = 4;

    ChannelBuffer<U16, numChannels> src;
    ChannelBuffer<U16, numChannels> dst;
    ChannelBuffer<U16, numChannels> dstExpected;

    const Fw16u *pSrc;
    FwiSize srcSize;
    int srcStep;
    FwiRect srcRoi;
    Fw16u *pDst;
    int dstStep;
    FwiRect dstRoi;
    double angle;
    double xCenter;
    double yCenter;
    int interpolation;

public:
	TestRotateCenter_16u_AC4R( UnitTestCatalogBase & parent )
        : FunctionObjectBase( parent, "fwiRotateCenter_16u_AC4R" )
    {}

    virtual void ResetFnParams( const AdviceBase &adv )
    {
        const int w = 5;
        const int h = 5;

        src.Reset(  "  0 101 102 103  1 105 106 107  2 109 110 111  3 113 114 115  4 117 118 119 "
                    "  5 121 122 123  6 125 126 127  7 129 130 131  8 133 134 135  9 137 138 139 "
                    " 10 141 142 143 11 145 146 147 12 149 150 151 13 153 154 155 14 157 158 159 "
                    " 15 161 162 163 16 165 166 167 17 169 170 171 18 173 174 175 19 177 178 179 "
                    " 20 181 182 183 21 185 186 187 22 189 190 191 23 193 194 195 24 197 198 199 ", w, h );
        dst.Reset("", w, h);
        dstExpected.Reset(  "  0   0   0   0  0   0   0   0  6 126 127   0  0   0   0   0  0   0   0   0 "
                            "  0   0   0   0  5 121 122   0  9 138 139   0 13 155 156   0  0   0   0   0 "
                            "  4 115 116   0  8 132 133   0 12 149 150   0 16 166 167   0 20 183 184   0 "
                            "  0   0   0   0 11 143 144   0 15 160 161   0 19 177 178   0  0   0   0   0 "
                            "  0   0   0   0  0   0   0   0 18 172 173   0  0   0   0   0  0   0   0   0 ", w, h );

        pSrc = src.Ptr();
        srcSize.width = w; srcSize.height = h;
        srcStep = w * sizeof(U16) * numChannels;
        srcRoi.x = 0; srcRoi.y = 0; srcRoi.width = w; srcRoi.height = h;
        pDst = dst.Ptr();
        dstStep = w * sizeof(U16) * numChannels;
        dstRoi.x = 0; dstRoi.y = 0; dstRoi.width = w; dstRoi.height = h;
        angle = 45.0;
        xCenter = (w-1)/2;
        yCenter = (h-1)/2;
        interpolation = FWI_INTER_LINEAR;
    }

    virtual bool Eval()
    {
        return dst.Eval(Log(), dstExpected);
    }

    virtual FwStatus CallFn()
    {
        return fwiRotateCenter_16u_AC4R(pSrc, srcSize, srcStep, srcRoi, pDst, dstStep, dstRoi, angle, xCenter, yCenter, interpolation);
    }

    virtual void RunAll()
    {
        ExecuteTest(fwStsNoErr);
    }
};

DEFINE_TEST_TABLE( RotateCenterTestCatalog )

TEST_ENTRY( TestRotateCenter_8u_C1R )
TEST_ENTRY( TestRotateCenter_16u_AC4R )

END_TEST_TABLE()
