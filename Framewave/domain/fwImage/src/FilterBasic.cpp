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

namespace OPT_LEVEL
{
namespace FilterBasic
{
    // these functions will determine what data type is required for intermediate calculations/results to
    // prevent overflow of the result.
    static FilterCommon::ArithmeticsType FilterBoxSizeAnalysis8bit( int width, int height )
    {
        const S64 maxValue = width * height * 255;
        if( maxValue <= CBL_LIBRARY::Limits<S16>::MaxValue() )
            return FilterCommon::arithmetics_16bit;
        else
            return FilterCommon::arithmetics_32bit;
    }

    static FilterCommon::ArithmeticsType FilterBoxSizeAnalysis16bit( int width, int height )
    {
        const S64 maxValue = width * height * 32768;
        if( maxValue <= CBL_LIBRARY::Limits<S32>::MaxValue() )
            return FilterCommon::arithmetics_32bit;
        else
            return FilterCommon::arithmetics_64bit;
    }

    // reference implementation of the FilterBox function
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterBox
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const FwiSize & maskSize, 
                                        const FwiPoint & anchor )
        {
            const Fw8u * pSrc = (const Fw8u *)pSrcPtr;
            CalcType sum = 0;
            for( int y = 0; y < maskSize.height; y++ )
            {
                for( int x = 0; x < maskSize.width; x++ )
                {
                    int offset = ( y - anchor.y ) * srcStep + ( x - anchor.x ) * chCount * sizeof( SrcType );
                    sum = sum + (CalcType)*(const SrcType *)(pSrc + offset );
                }
            }

            sum = sum / (CalcType)( maskSize.width * maskSize.height );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // reference implementation of the SumWindowRow function.
    // This function assumes that the processor will call the Calculate member while iterating over the
    // destination pixels in a zigzag order.  It preserves the results of some intermediate calculations
    // and reuses them in subsequent calls.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSumWindowRow
    {
        static inline int GetMinimumBufferSize( int /*maskSize*/ )
        {
            return chCount * sizeof( CalcType );
        }

        static inline void Calculate(   const SrcType * pSrc, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType * pSrcEnd = pSrc + maskSize * chCount;

            for( int ch = 0; ch < chCount; ch++ )
            {
                pRunningSum[ch] = 0;
            }

            while( pSrc < pSrcEnd )
            {
                for( int ch = 0; ch < chCount; ch++ )
                {
                    pRunningSum[ch] += (CalcType)pSrc[ch];
                }
                pSrc += chCount;
            }

            for( int ch = 0; ch < chCount; ch++ )
            {
                pDst[ch] = (DstType)pRunningSum[ch];
            }
        }

        static inline void CalculateRight(   const SrcType * pSrc, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = pSrc + maskSize * chCount;

            for( int ch = 0; ch < chCount; ch++ )
            {
                pRunningSum[ch] = pRunningSum[ch] - pOldElement[ch];
                pRunningSum[ch] = pRunningSum[ch] + pNewElement[ch];
                pDst[ch] = (DstType)pRunningSum[ch];
            }
        }
    };

    // AC4 version of the class above
    template< typename SrcType, typename DstType, typename CalcType >
    struct FilterSumWindowRow< AC4, SrcType, DstType, CalcType > 
    {
        static inline int GetMinimumBufferSize( int /*maskSize*/ )
        {
            return AC4 * sizeof( CalcType );
        }

        static inline void Calculate(   const SrcType * pSrc, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType * pSrcEnd = pSrc + C4;

            pRunningSum[0] = 0;
            pRunningSum[1] = 0;
            pRunningSum[2] = 0;

            int channel = 0;
            while( pSrc < pSrcEnd )
            {
                pRunningSum[0] += (CalcType)*pSrc[0];
                pRunningSum[1] += (CalcType)*pSrc[1];
                pRunningSum[2] += (CalcType)*pSrc[2];
                pSrc += C4;
            }

            pDst[0] = (DstType)*(pRunningSum[0]);
            pDst[1] = (DstType)*(pRunningSum[1]);
            pDst[2] = (DstType)*(pRunningSum[2]);
        }

        static inline void CalculateRight(   const SrcType * pSrc, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = pSrc + maskSize * C4;

            pRunningSum[0] = pRunningSum[0] - pOldElement[0];
            pRunningSum[0] = pRunningSum[0] + pNewElement[0];
            pDst[0] = (DstType)*(pRunningSum[0]);
            pRunningSum[1] = pRunningSum[1] - pOldElement[1];
            pRunningSum[1] = pRunningSum[1] + pNewElement[1];
            pDst[1] = (DstType)*(pRunningSum[1]);
            pRunningSum[2] = pRunningSum[2] - pOldElement[2];
            pRunningSum[2] = pRunningSum[2] + pNewElement[2];
            pDst[2] = (DstType)*(pRunningSum[2]);
        }
    };

    // reference implementation of the SumWindowColumn function.
    // This function assumes that the processor will call the Calculate member while iterating over the
    // destination pixels in a zigzag order.  It preserves the results of some intermediate calculations
    // and reuses them in subsequent calls.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSumWindowColumn
    {
        static inline int GetMinimumBufferSize( int /*maskSize*/ )
        {
            return chCount * sizeof( CalcType );
        }

        static inline void Calculate(   const SrcType * pSrc, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType * pSrcEnd = (const SrcType*)(((Fw8u*)(pSrc)) + maskSize * srcStep);

            for( int ch = 0; ch < chCount; ch++ )
            {
                pRunningSum[ch] = 0;
            }

            while( pSrc < pSrcEnd )
            {
                for( int ch = 0; ch < chCount; ch++ )
                {
                    pRunningSum[ch] += (CalcType)pSrc[ch];
                }
                pSrc = (const SrcType*)(((Fw8u*)(pSrc)) + srcStep);
            }

            for( int ch = 0; ch < chCount; ch++ )
            {
                pDst[ch] = (DstType)pRunningSum[ch];
            }
        }

        static inline void CalculateDown(   const SrcType * pSrc, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) + maskSize * srcStep);

            for( int ch = 0; ch < chCount; ch++ )
            {
                pRunningSum[ch] = pRunningSum[ch] - pOldElement[ch];
                pRunningSum[ch] = pRunningSum[ch] + pNewElement[ch];
                pDst[ch] = (DstType)pRunningSum[ch];
            }
        }
    };

    // AC4 version of the class above
    template< typename SrcType, typename DstType, typename CalcType >
    struct FilterSumWindowColumn< AC4, SrcType, DstType, CalcType > 
    {
        static inline int GetMinimumBufferSize( int /*maskSize*/ )
        {
            return C4 * sizeof( CalcType );
        }

        static inline void Calculate(   const SrcType * pSrc, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType * pSrcEnd = (const SrcType*)(((Fw8u*)(pSrc)) + maskSize * srcStep);

            pRunningSum[0] = 0;
            pRunningSum[1] = 0;
            pRunningSum[2] = 0;

            int channel = 0;
            while( pSrc < pSrcEnd )
            {
                pRunningSum[0] += (CalcType)*pSrc[0];
                pRunningSum[1] += (CalcType)*pSrc[1];
                pRunningSum[2] += (CalcType)*pSrc[2];
                pSrc = (const SrcType*)(((Fw8u*)(pSrc)) + srcStep);
            }

            pDst[0] = (DstType)*(pRunningSum[0]);
            pDst[1] = (DstType)*(pRunningSum[1]);
            pDst[2] = (DstType)*(pRunningSum[2]);
        }

        static inline void CalculateDown(   const SrcType * pSrc, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        int maskSize, 
                                        int /*anchor*/,
                                        void* pTempBuffer )
        {
            CalcType *pRunningSum = (CalcType*)pTempBuffer;
            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) + maskSize * srcStep);

            pRunningSum[0] = pRunningSum[0] - pOldElement[0];
            pRunningSum[0] = pRunningSum[0] + pNewElement[0];
            pDst[0] = (DstType)*(pRunningSum[0]);
            pRunningSum[1] = pRunningSum[1] - pOldElement[1];
            pRunningSum[1] = pRunningSum[1] + pNewElement[1];
            pDst[1] = (DstType)*(pRunningSum[1]);
            pRunningSum[2] = pRunningSum[2] - pOldElement[2];
            pRunningSum[2] = pRunningSum[2] + pNewElement[2];
            pDst[2] = (DstType)*(pRunningSum[2]);
        }
    };

    // reference implementation of the FilterMin function.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMin
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const FwiSize & maskSize, 
                                        const FwiPoint & anchor )
        {
            const Fw8u * pSrc = (const Fw8u *)pSrcPtr;
            SrcType min = *(const SrcType *)pSrc;
            for( int y = 0; y < maskSize.height; y++ )
            {
                for( int x = 0; x < maskSize.width; x++ )
                {
                    int offset = ( y - anchor.y ) * srcStep + ( x - anchor.x ) * chCount * sizeof( SrcType );
                    if( min > *(const SrcType *)(pSrc + offset) )
                        min = *(const SrcType *)(pSrc + offset);
                }
            }

            *pDst = min;
        }
    };

    // reference implementation of the FilterMax function.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMax
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const FwiSize & maskSize, 
                                        const FwiPoint & anchor )
        {
            const Fw8u * pSrc = (const Fw8u *)pSrcPtr;
            SrcType max = *(const SrcType *)pSrc;
            for( int y = 0; y < maskSize.height; y++ )
            {
                for( int x = 0; x < maskSize.width; x++ )
                {
                    int offset = ( y - anchor.y ) * srcStep + ( x - anchor.x ) * chCount * sizeof( SrcType );
                    if( max < *(const SrcType *)(pSrc + offset) )
                        max = *(const SrcType *)(pSrc + offset);
                }
            }

            *pDst = max;
        }
    };

#if BUILD_NUM_AT_LEAST( 9999 )
    template< CH chCount, typename SrcType, typename DstType, typename CalcType,
        template< CH, typename > class BorderType >
    struct FilterMinBorder
    {
        static inline void Calculate(   BorderType< chCount, SrcType > & buffer, int col,
                                        DstType * pDst,
                                        const FwiSize & maskSize, 
                                        const FwiPoint & /*anchor*/ )
        {
            const SrcType * pSrc = buffer.GetSrcLine( 0 );
            SrcType min = *(const SrcType *)( pSrc + col );
            for( int y = 0; y < maskSize.height; y++ )
            {
                pSrc = (const SrcType *)buffer.GetSrcLine( y );
                for( int x = 0; x < maskSize.width; x++ )
                {
                    int offset = col + x * chCount;
                    if( min > *(pSrc + offset) )
                        min = *(pSrc + offset);
                }
            }

            *pDst = min;
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType,
        template< CH, typename > class BorderType >
    struct FilterMaxBorder
    {
        static inline void Calculate(   BorderType< chCount, SrcType > & buffer, int col,
                                        DstType * pDst,
                                        const FwiSize & maskSize, 
                                        const FwiPoint & /*anchor*/ )
        {
            const SrcType * pSrc = buffer.GetSrcLine( 0 );
            SrcType max = *(const SrcType *)( pSrc + col );
            for( int y = 0; y < maskSize.height; y++ )
            {
                pSrc = (const SrcType *)buffer.GetSrcLine( y );
                for( int x = 0; x < maskSize.width; x++ )
                {
                    int offset = col + x * chCount;
                    if( max < *(pSrc + offset) )
                        max = *(pSrc + offset);
                }
            }

            *pDst = max;
        }
    };
#endif // BUILD_NUM_AT_LEAST
} // namespace FilterBasic
} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis8bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );

    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis8bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis8bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw8u, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis8bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis16bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_32bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw16s, 
            Fw64s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis16bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_32bit:
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw64s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis16bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_32bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw16s, 
            Fw16s, 
            Fw64s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    FilterCommon::ArithmeticsType atype = FilterBasic::FilterBoxSizeAnalysis16bit( maskSize.width, maskSize.height );
    switch( atype )
    {
    case FilterCommon::arithmetics_32bit:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw16s, 
            Fw16s, 
            Fw64s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    default:
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FilterBasic::FilterBox, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C1IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C3IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_C4IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_8u_AC4IR)( Fw8u * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C1IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C3IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_C4IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_16s_AC4IR)( Fw16s * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C1IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C3IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_C4IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterBox_32f_AC4IR)( Fw32f * pSrcDst, int srcDstStep,
                               FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_InPlace_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterBox, FilterCommon::DataWithMask >
    ::Run( pSrcDst, srcDstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMin_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMin, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw16s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw32s, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMax_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw32f, 
        Fw32f, 
        Fw32f, 
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FilterBasic::FilterMax, FilterCommon::DataWithMask >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_8u32f_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowRow_16s32f_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagRDRD,
            FilterBasic::FilterSumWindowRow, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_8u32f_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 0x800000 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw8u, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C1,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiSumWindowColumn_16s32f_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               int maskSize, int anchor )
{
    if( maskSize < 65536 )
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw16s, 
            Fw32f, 
            Fw32s,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
    else
    {
        return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C4,
            Fw16s, 
            Fw32f, 
            Fw32f,
            FilterCommon::TProcessor_ZigZagDRDR,
            FilterBasic::FilterSumWindowColumn, FilterCommon::DataWithLinearMaskAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
    }
}

#if BUILD_NUM_AT_LEAST( 9999 )

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_8u_C1R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_8u_C3R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_8u_C4R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_32f_C1R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_32f_C3R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinGetBufferSize_32f_C4R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMinBorderReplicate_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMinBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_8u_C1R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_8u_C3R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_8u_C4R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_32f_C1R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_32f_C1R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C1,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_32f_C3R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_32f_C3R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C3,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxGetBufferSize_32f_C4R)( int roiWidth, FwiSize maskSize, int* pBufferSize )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::GetBufferSize( roiWidth, maskSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMaxBorderReplicate_32f_C4R)( const Fw32f * pSrc, int srcStep,
                               Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor, Fw8u* pBuffer )
{
    return FilterCommon::DivideAndConquer_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer< 
        C4,
        Fw32f, 
        Fw32f, 
        Fw32f,
        FilterCommon::TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer,
        FilterBasic::FilterMaxBorder, 
        FilterCommon::DataWithBuffer,
        BordersReplication::BorderBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer );
}
#endif // BUILD_NUM_AT_LEAST

#endif // BUILD_NUM_AT_LEAST

//#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
