/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#if 0

#include "fwdev.h"
#include "fwImage.h"
#include "FwSharedCode_SSE2.h"

//#if !defined( SOL32 ) && !defined( SOL64 )

#if BUILD_NUM_AT_LEAST( 9999 )
#include <vector>
#include "Filter.h"

#include "FiltersFixed.h"

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHorizGetBufferSize_8u16s_C1R)( FwiSize roiSize, int* pBufferSize )
{
    return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHorizGetBufferSize_32f_C1R)( FwiSize roiSize, int* pBufferSize )
{
    return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVertGetBufferSize_8u16s_C1R)( FwiSize roiSize, int* pBufferSize )
{
    return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVertGetBufferSize_32f_C1R)( FwiSize roiSize, int* pBufferSize )
{
    return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelNegVertGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelNegVertGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecondGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecondGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecondGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecondGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCrossGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCrossGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus fwiFilterLPREFIX_OPT(OPT_PREFIX, fwacianGetBufferSize_8u16s_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus fwiFilterLPREFIX_OPT(OPT_PREFIX, fwacianGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpassGetBufferSize_8u_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpassGetBufferSize_32f_C1R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C1, Fw32f, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}



FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHorizBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                FwiBorderType borderType, Fw8u borderValue, Fw8u * pBuffer )
{
    return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
        C1,
        Fw8u,
        Fw16s,
        Fw16s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrHoriz, FilterCommon::Data,
        1
    >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrHorizBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                FwiBorderType borderType, Fw32f borderValue, Fw8u * pBuffer )
{
    return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrHoriz, FilterCommon::Data,
        1
    >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVertBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                FwiBorderType borderType, Fw8u borderValue, Fw8u * pBuffer )
{
    return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
        C1,
        Fw8u,
        Fw16s,
        Fw16s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrVert, FilterCommon::Data,
        1
    >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterScharrVertBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                FwiBorderType borderType, Fw32f borderValue, Fw8u * pBuffer )
{
    return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
        FiltersFixed::FilterScharrVert, FilterCommon::Data,
        1
    >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVert5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelNegVertBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelNegVert, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelNegVert5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelNegVertBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelNegVert, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelNegVert5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecondBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizSecondBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHorizSecond5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecondBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelVertSecondBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelVertSecond5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCrossBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelCrossBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelCross5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus fwiFilterLPREFIX_OPT(OPT_PREFIX, fwacianBorder_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus fwiFilterLPREFIX_OPT(OPT_PREFIX, fwacianBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLaplace5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpassBorder_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                            Fw8u * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u,
            Fw8u,
            Fw16s,
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLowpass, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw8u,
            Fw8u,
            Fw16s,
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLowpass5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterLowpassBorder_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                            Fw32f * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw32f borderValue, Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLowpass, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C1,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterLowpass5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, &borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

// @todo: remove the 2 functions below
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizGetBufferSize_8u16s_C3R)( FwiSize roiSize, FwiMaskSize mask, int* pBufferSize )
{
    if( mask == fwMskSize3x3 )
        return BordersReplication::BorderReplicator< C3, Fw8u, 1 >::GetBufferSize( roiSize, pBufferSize );
    else if( mask == fwMskSize5x5 )
        return BordersReplication::BorderReplicator< C3, Fw8u, 2 >::GetBufferSize( roiSize, pBufferSize );
    else
        return fwStsMaskSizeErr;
}


FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterSobelHorizBorder_8u16s_C3R)( const Fw8u * pSrc, int srcStep,
                                            Fw16s * pDst, int dstStep, FwiSize dstRoiSize, FwiMaskSize mask,
                                            FwiBorderType borderType, Fw8u borderValue[3], Fw8u* pBuffer )
{
    if( mask == fwMskSize3x3 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C3,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz, FilterCommon::Data,
            1 
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, borderValue, pBuffer );
    }
    else if( mask == fwMskSize5x5 )
    {
        return FilterCommon::DivideAndConquer_ReplicateBorder_pSrc_sStep_pDst_dStep_roi_buffer< 
            C3,
            Fw8u, 
            Fw16s, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi,
            FiltersFixed::FilterSobelHoriz5x5, FilterCommon::Data,
            2
        >::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, borderType, borderValue, pBuffer );
    }
    else
        return fwStsMaskSizeErr;
}

#endif // BUILD_NUM_AT_LEAST

//#endif

#endif

