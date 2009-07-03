/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

//#if !defined( SOL32 ) && !defined( SOL64 )

#if BUILD_NUM_AT_LEAST( 100 )
#include <vector>
#include "Filter.h"

#include "FiltersFixed.h"

namespace OPT_LEVEL
{
static FwiSize kernel3x3 = { 3, 3 };
static FwiPoint anchor3x3 = { 1, 1 };
static FwiSize kernel5x5 = { 5, 5 };
static FwiPoint anchor5x5 = { 2, 2 };

//////////////////////////////////////////////////////
// kernel masks for various fixed filters
//////////////////////////////////////////////////////

const Fw32s rgPrewittHorizKernel[] = {  1,  1,  1,
                                         0,  0,  0,
                                        -1, -1, -1, };

const Fw32s rgPrewittVertKernel[] = {  -1,  0,  1,
                                        -1,  0,  1,
                                        -1,  0,  1, };

const Fw32f rgPrewittHorizKernel32f[] = {  1,  1,  1,
                                         0,  0,  0,
                                        -1, -1, -1, };

const Fw32f rgPrewittVertKernel32f[] = {  -1,  0,  1,
                                        -1,  0,  1,
                                        -1,  0,  1, };

//////////////////////////////////////////////////////

const Fw32s rgScharrHorizKernel[] = {   3,  10,  3,
                                         0,   0,  0,
                                        -3, -10, -3 };

const Fw32f rgScharrHorizKernel32f[] = {   3,  10,  3,
                                            0,   0,  0,
                                           -3, -10, -3 };

const Fw32s rgScharrVertKernel[] = {   3,  0,  -3,
                                       10,  0,  -10,
                                        3,  0,  -3 };

const Fw32f rgScharrVertKernel32f[] = {   3,  0,  -3,
                                          10,  0,  -10,
                                           3,  0,  -3 };

//////////////////////////////////////////////////////

const Fw32s rgSobelHorizKernel[] = {    1,   2,  1,
                                         0,   0,  0,
                                        -1,  -2, -1 };

const Fw32s rgSobelVertKernel[] =  {  -1,  0,   1,
                                       -2,  0,   2,
                                       -1,  0,   1 };

const Fw32f rgSobelHorizKernel32f[] = {    1,   2,  1,
                                            0,   0,  0,
                                           -1,  -2, -1 };

const Fw32f rgSobelVertKernel32f[] =  {  -1,  0,   1,
                                          -2,  0,   2,
                                          -1,  0,   1 };

//////////////////////////////////////////////////////

const Fw32s rgSobelHorizKernel5x5[] = {    1,  4,   6,  4,  1,
                                            2,  8,  12,  8,  2,
                                            0,  0,   0,  0,  0,
                                           -2, -8, -12, -8, -2,
                                           -1, -4,  -6, -4, -1 };

const Fw32s rgSobelVertKernel5x5[] =  {   -1,  -2, 0,  2, 1,
                                           -4,  -8, 0,  8, 4,
                                           -6, -12, 0, 12, 6,
                                           -4,  -8, 0,  8, 4,
                                           -1,  -2, 0,  2, 1 };

const Fw32f rgSobelHorizKernel5x5_32f[] = {    1,  4,   6,  4,  1,
                                                2,  8,  12,  8,  2,
                                                0,  0,   0,  0,  0,
                                               -2, -8, -12, -8, -2,
                                               -1, -4,  -6, -4, -1 };

const Fw32f rgSobelVertKernel5x5_32f[] =  {   -1,  -2, 0,  2, 1,
                                               -4,  -8, 0,  8, 4,
                                               -6, -12, 0, 12, 6,
                                               -4,  -8, 0,  8, 4,
                                               -1,  -2, 0,  2, 1 };

//////////////////////////////////////////////////////

const Fw32s rgSobelHorizSecondKernel[] = {    1,   2,  1,
                                              -2,  -4, -2,
                                               1,   2,  1 };

const Fw32f rgSobelHorizSecondKernel32f[] = {    1,   2,  1,
                                                 -2,  -4, -2,
                                                  1,   2,  1 };

const Fw32s rgSobelVertSecondKernel[] =  {  1, -2,  1,
                                             2, -4,  2,
                                             1, -2,  1 };

const Fw32f rgSobelVertSecondKernel32f[] =  {  1, -2,  1,
                                                2, -4,  2,
                                                1, -2,  1 };

const Fw32s rgSobelHorizSecondKernel5x5[] = {    1,  4,   6,  4,  1,
                                                  0,  0,   0,  0,  0,
                                                 -2, -8, -12, -8, -2,
                                                  0,  0,   0,  0,  0,
                                                  1,  4,   6,  4,  1 };

const Fw32f rgSobelHorizSecondKernel5x5_32f[] = {    1,  4,   6,  4,  1,
                                                      0,  0,   0,  0,  0,
                                                     -2, -8, -12, -8, -2,
                                                      0,  0,   0,  0,  0,
                                                      1,  4,   6,  4,  1 };

const Fw32s rgSobelVertSecondKernel5x5[] =  {   1,  0,  -2, 0, 1,
                                                 4,  0,  -8, 0, 4,
                                                 6,  0, -12, 0, 6,
                                                 4,  0,  -8, 0, 4,
                                                 1,  0,  -2, 0, 1 };

const Fw32f rgSobelVertSecondKernel5x5_32f[] =  {   1,  0,  -2, 0, 1,
                                                     4,  0,  -8, 0, 4,
                                                     6,  0, -12, 0, 6,
                                                     4,  0,  -8, 0, 4,
                                                     1,  0,  -2, 0, 1 };
//////////////////////////////////////////////////////

const Fw32s rgSobelCrossKernel[] =  {  -1, 0,  1,
                                         0, 0,  0,
                                         1, 0, -1 };

const Fw32f rgSobelCrossKernel32f[] =  {  -1, 0,  1,
                                            0, 0,  0,
                                            1, 0, -1 };

const Fw32s rgSobelCrossKernel5x5[] = {   -1, -2,  0,  2,  1,
                                           -2, -4,  0,  4,  2,
                                            0,  0,  0,  0,  0,
                                            2,  4,  0, -4, -2,
                                            1,  2,  0, -2, -1 };

const Fw32f rgSobelCrossKernel5x5_32f[] = {   -1, -2,  0,  2,  1,
                                               -2, -4,  0,  4,  2,
                                                0,  0,  0,  0,  0,
                                                2,  4,  0, -4, -2,
                                                1,  2,  0, -2, -1 };

//////////////////////////////////////////////////////

const Fw32s rgLaplaceKernel[] =  {  -1, -1, -1,
                                     -1,  8, -1,
                                     -1, -1, -1 };

const Fw32f rgLaplaceKernel32f[] =  {  -1, -1, -1,
                                        -1,  8, -1,
                                        -1, -1, -1 };

const Fw32s rgLaplaceKernel5x5[] = {   -1, -3, -4, -3, -1,
                                        -3,  0,  6,  0, -3,
                                        -4,  6, 20,  6, -4,
                                        -3,  0,  6,  0, -3,
                                        -1, -3, -4, -3, -1 };

const Fw32f rgLaplaceKernel5x5_32f[] = {   -1, -3, -4, -3, -1,
                                            -3,  0,  6,  0, -3,
                                            -4,  6, 20,  6, -4,
                                            -3,  0,  6,  0, -3,
                                            -1, -3, -4, -3, -1 };

//////////////////////////////////////////////////////

const Fw32s rgGaussKernel[] =  {  1, 2, 1,
                                   2, 4, 2,
                                   1, 2, 1 };

const Fw32s rgGaussKernel5x5[] = {   2,  7, 12,  7,  2,
                                      7, 31, 52, 31,  7,
                                     12, 52,127, 52, 12,
                                      7, 31, 52, 31,  7,
                                      2,  7, 12,  7,  2 };

const Fw32f rgGaussKernel32f[] =  {  0.0625f, 0.125f, 0.0625f,
                                      0.1250f, 0.250f, 0.1250f,
                                      0.0625f, 0.125f, 0.0625f };

const Fw32f rgGaussKernel5x5_32f[] = 
{    0.003502626970227671f,  0.01225919439579685f,  0.02101576182136602f,  0.01225919439579685f,  0.003502626970227671f,
      0.01225919439579685f,   0.0542907180385289f,  0.09106830122591944f,   0.0542907180385289f,   0.01225919439579685f,
      0.02101576182136602f,  0.09106830122591944f,   0.2224168126094571f,  0.09106830122591944f,   0.02101576182136602f,
      0.01225919439579685f,   0.0542907180385289f,  0.09106830122591944f,   0.0542907180385289f,   0.01225919439579685f,
     0.003502626970227671f,  0.01225919439579685f,  0.02101576182136602f,  0.01225919439579685f,  0.003502626970227671f };

//////////////////////////////////////////////////////

const Fw32s rgHipassKernel[] =  {  -1, -1, -1,
                                    -1,  8, -1,
                                    -1, -1, -1 };

const Fw32f rgHipassKernel32f[] =  {  -1, -1, -1,
                                       -1,  8, -1,
                                       -1, -1, -1 };

const Fw32s rgHipassKernel5x5[] = {  -1, -1, -1, -1, -1,
                                      -1, -1, -1, -1, -1,
                                      -1, -1, 24, -1, -1,
                                      -1, -1, -1, -1, -1,
                                      -1, -1, -1, -1, -1 };

const Fw32f rgHipassKernel5x5_32f[] = {  -1, -1, -1, -1, -1,
                                          -1, -1, -1, -1, -1,
                                          -1, -1, 24, -1, -1,
                                          -1, -1, -1, -1, -1,
                                          -1, -1, -1, -1, -1 };

//////////////////////////////////////////////////////

const Fw32s rgLowpassKernel[] =  {  1, 1, 1,
                                     1, 1, 1,
                                     1, 1, 1 };

const Fw32f rgLowpassKernel32f[] =  {  0.111111111111f, 0.111111111111f, 0.111111111111f,
                                        0.111111111111f, 0.111111111111f, 0.111111111111f,
                                        0.111111111111f, 0.111111111111f, 0.111111111111f };

const Fw32s rgLowpassKernel5x5[] = {   1,  1,  1,  1,  1,
                                        1,  1,  1,  1,  1,
                                        1,  1,  1,  1,  1,
                                        1,  1,  1,  1,  1,
                                        1,  1,  1,  1,  1 };

const Fw32f rgLowpassKernel5x5_32f[] = {   0.04f,  0.04f,  0.04f,  0.04f,  0.04f,
                                            0.04f,  0.04f,  0.04f,  0.04f,  0.04f,
                                            0.04f,  0.04f,  0.04f,  0.04f,  0.04f,
                                            0.04f,  0.04f,  0.04f,  0.04f,  0.04f,
                                            0.04f,  0.04f,  0.04f,  0.04f,  0.04f };

} // namespace OPT_LEVEL

//////////////////////////////////////////////////////

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittHoriz_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
    //    C1,
    //    Fw8u, 
    //    Fw8u, 
    //    Fw16s, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
    //    FiltersFixed::FilterPrewittVert, FilterCommon::Data >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterPrewittVert_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgPrewittVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8u, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrHoriz, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrHoriz, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgScharrHorizKernel32f, kernel3x3, anchor3x3 );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
    //    C1,
    //    Fw32f, 
    //    Fw32f, 
    //    Fw32f, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
    //    FiltersFixed::FilterScharrHoriz, FilterCommon::Data >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8u, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrVert, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrVert, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVert_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgScharrVertKernel32f, kernel3x3, anchor3x3 );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
    //    C1,
    //    Fw32f, 
    //    Fw32f, 
    //    Fw32f, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
    //    FiltersFixed::FilterScharrVert, FilterCommon::Data >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
    //    C1,
    //    Fw8u, 
    //    Fw8u, 
    //    Fw16s, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
    //    FiltersFixed::FilterSobelHoriz, FilterCommon::Data >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHoriz_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizMask_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
    //    C1,
    //    Fw8u, 
    //    Fw8u, 
    //    Fw16s, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
    //    FiltersFixed::FilterSobelVert, FilterCommon::Data >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel, kernel3x3, anchor3x3, 1 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel32f, kernel3x3, anchor3x3 );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVert_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertMask_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecond_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizSecondKernel32f, kernel3x3, anchor3x3 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelHorizSecond, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelHorizSecondKernel5x5_32f, kernel5x5, anchor5x5 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelHorizSecond5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecond_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertSecondKernel32f, kernel3x3, anchor3x3 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelVertSecond, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelVertSecondKernel5x5_32f, kernel5x5, anchor5x5 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelVertSecond5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCross_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelCrossKernel32f, kernel3x3, anchor3x3 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelCross, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgSobelCrossKernel5x5_32f, kernel5x5, anchor5x5 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw32f, 
        //    Fw32f, 
        //    Fw32f, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterSobelCross5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsDown_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsDown, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                         Fw8u * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                         Fw16s * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRobertsUp_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                         Fw32f * pDst, int dstStep, FwiSize dstRoiSize )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        AC4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterRobertsUp, FilterCommon::Data >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterLaplace, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterLaplace5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLaplaceKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLaplace_8s16s_C1R)( const Fw8s * pSrc, int srcStep,
                                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                                FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
            C1,
            Fw8s, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace5x5, FilterCommon::Data >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterGauss, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterGauss5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel, kernel3x3, anchor3x3, 16 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5, kernel5x5, anchor5x5, 571 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterGauss_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgGaussKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel, kernel3x3, anchor3x3, 1 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5, kernel5x5, anchor5x5, 1 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_C4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterHipass_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgHipassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterLowpass, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
        //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi< 
        //    C1,
        //    Fw8u, 
        //    Fw8u, 
        //    Fw16s, 
        //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        //    FiltersFixed::FilterLowpass5x5, FilterCommon::Data >
        //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                    Fw8u * pDst, int dstStep, FwiSize dstRoiSize, 
                                    FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                     Fw16s * pDst, int dstStep, FwiSize dstRoiSize, 
                                     FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel, kernel3x3, anchor3x3, 9 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5, kernel5x5, anchor5x5, 25 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpass_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize, 
                                              FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel32f, kernel3x3, anchor3x3 );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, rgLowpassKernel5x5_32f, kernel5x5, anchor5x5 );
    }
    else
        return fwStsMaskSizeErr;
}

#endif // BUILD_NUM_AT_LEAST

//#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
