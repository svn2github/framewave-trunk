/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
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

namespace FilterConvolution
{
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterConvFull
    {
        static inline void CalculateDstRoiSize( FwiSize srcSize1, FwiSize srcSize2, FwiSize & dstSize )
        {
            dstSize.width = srcSize1.width + srcSize2.width - 1;
            dstSize.height = srcSize1.height + srcSize2.height - 1;
        }

        static inline CalcType ConvFull( const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                         const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                         const FwiPoint & dstPoint )
        {
            CalcType hval = 0;
            FwiSize dstSize;
            CalculateDstRoiSize( srcSize1, srcSize2, dstSize );

            int minRow = dstPoint.y - srcSize2.height + 1;
            if( minRow < 0 ) minRow = 0;
            int maxRow = ( dstPoint.y < srcSize1.height ) ? dstPoint.y : srcSize1.height - 1;

            for( int l = minRow; l <= maxRow; l++ )
            {
                int minCol = dstPoint.x - srcSize2.width + 1;
                if( minCol < 0 ) minCol = 0;
                int maxCol = ( dstPoint.x < srcSize1.width ) ? dstPoint.x : srcSize1.width - 1;

                int y1 = l;
                int y2 = dstPoint.y - l;
                ASSERT( y1 >= 0 );
                ASSERT( y2 >= 0 );
                ASSERT( y1 < srcSize1.height );
                ASSERT( y2 < srcSize2.height );

                for( int k = minCol; k <= maxCol; k++ )
                {
                    int x1 = k;
                    int x2 = dstPoint.x - k;
                    ASSERT( x1 >= 0 );
                    ASSERT( x2 >= 0 );
                    ASSERT( x1 < srcSize1.width );
                    ASSERT( x2 < srcSize2.width );

                    const SrcType * pFval = ( (const SrcType *)(((Fw8u*)pSrc1) + y1 * srcStep1 ) ) + x1 * chCount;
                    const SrcType * pGval = ( (const SrcType *)(((Fw8u*)pSrc2) + y2 * srcStep2 ) ) + x2 * chCount;

                    hval += (CalcType)(*pFval) * (CalcType)(*pGval);
                } // for k
            } // for l

            return hval;
        }

        static inline void Calculate(   const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                        const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                        DstType * pDst, int /*dstStep*/, const FwiPoint & dstPoint )
        {
            CalcType result = ConvFull( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, dstPoint );
            *pDst = FW_REF::Limits<DstType>::Sat( result );
        }

        static inline void Calculate(   const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                        const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                        DstType * pDst, int /*dstStep*/, const FwiPoint & dstPoint, int divisor )
        {
            CalcType result = ConvFull( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, dstPoint );
            result /= divisor;
            *pDst = FW_REF::Limits<DstType>::Sat( result );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterConvValid
    {
        static inline void CalculateDstRoiSize( FwiSize srcSize1, FwiSize srcSize2, FwiSize & dstSize )
        {
            ASSERT( srcSize1.width >= srcSize2.width );
            ASSERT( srcSize1.height >= srcSize2.height );
            dstSize.width = srcSize1.width - srcSize2.width + 1;
            dstSize.height = srcSize1.height - srcSize2.height + 1;
        }

        static inline CalcType ConvValid( const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                          const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                          const FwiPoint & dstPoint )
        {
            CalcType hval = 0;

            srcSize1;
            ASSERT( srcSize1.width >= srcSize2.width );
            ASSERT( srcSize1.height >= srcSize2.height );

            for( int l = 0; l < srcSize2.height; l++ )
            {
                for( int k = 0; k < srcSize2.width; k++ )
                {
                    const SrcType * pFval = (const SrcType *)(((Fw8u*)pSrc1) + ( dstPoint.y + l ) * srcStep1 );
                    pFval += ( dstPoint.x + k ) * chCount;
                    SrcType fval = *pFval;

                    const SrcType * pGval = (const SrcType *)(((Fw8u*)pSrc2) + ( srcSize2.height - l - 1 ) * srcStep2 );
                    pGval += ( srcSize2.width - k - 1 ) * chCount;
                    SrcType gval = *pGval;

                    hval += fval * gval;
                } // for k
            } // for l

            return hval;
        }

        static inline void Calculate(   const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                        const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                        DstType * pDst, int /*dstStep*/, const FwiPoint & dstPoint )
        {
            CalcType result = ConvValid( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, dstPoint );
            *pDst = FW_REF::Limits<DstType>::Sat( result );
        }

        static inline void Calculate(   const SrcType * pSrc1, int srcStep1, FwiSize srcSize1,
                                        const SrcType * pSrc2, int srcStep2, FwiSize srcSize2,
                                        DstType * pDst, int /*dstStep*/, const FwiPoint & dstPoint, int divisor )
        {
            CalcType result = ConvValid( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, dstPoint );
            result /= divisor;
            *pDst = FW_REF::Limits<DstType>::Sat( result );
        }
    };

    template< typename SrcDataType, typename DstDataType >
    struct ConvData
    {
        const SrcDataType *pSrc1;
        const SrcDataType *pSrc2;
        DstDataType       *pDst;
        int                srcStep1;
        int                srcStep2;
        int                dstStep;
        FwiSize           srcSize1;
        FwiSize           srcSize2;
        FwiSize           dstRoiSize;
        int                divisor;
        int                startRow;
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            FwiPoint dstPoint;
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            dstPoint.x = 0;
            dstPoint.y = pData->startRow;
            DstType * pDst = pData->pDst;
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            while( pDst < pEndDst )
            {
                dstPoint.x = 0;
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        Calculate( pData->pSrc1+ch, pData->srcStep1, pData->srcSize1,
                                   pData->pSrc2+ch, pData->srcStep2, pData->srcSize2,
                                   pDstElement, pData->dstStep, dstPoint, pData->divisor );
                        pDstElement++;
                    }
                    dstPoint.x++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                dstPoint.y++;
            }
        }
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor< AC4, SrcType, DstType, CalcType,
                                                                                  Operator, DataType >
        : public Operator< C4, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            FwiPoint dstPoint;
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            dstPoint.x = 0;
            dstPoint.y = pData->startRow;
            DstType * pDst = pData->pDst;
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            while( pDst < pEndDst )
            {
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pData->pSrc1+0, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+0, pData->srcStep2, pData->srcSize2,
                               pDstElement+0, pData->dstStep, dstPoint, pData->divisor );
                    Calculate( pData->pSrc1+1, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+1, pData->srcStep2, pData->srcSize2,
                               pDstElement+1, pData->dstStep, dstPoint, pData->divisor );
                    Calculate( pData->pSrc1+2, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+2, pData->srcStep2, pData->srcSize2,
                               pDstElement+2, pData->dstStep, dstPoint, pData->divisor );
                    pDstElement += C4;
                    dstPoint.x += C4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                dstPoint.y++;
            }
        }
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            FwiPoint dstPoint;
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            dstPoint.x = 0;
            dstPoint.y = pData->startRow;
            DstType * pDst = pData->pDst;
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            while( pDst < pEndDst )
            {
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        Calculate( pData->pSrc1+ch, pData->srcStep1, pData->srcSize1,
                                   pData->pSrc2+ch, pData->srcStep2, pData->srcSize2,
                                   pDstElement, pData->dstStep, dstPoint );
                        pDstElement++;
                    }
                    dstPoint.x += chCount;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                dstPoint.y++;
            }
        }
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep< AC4, SrcType, DstType, CalcType,
                                                                                  Operator, DataType >
        : public Operator< C4, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            FwiPoint dstPoint;
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            dstPoint.x = 0;
            dstPoint.y = pData->startRow;
            DstType * pDst = pData->pDst;
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            while( pDst < pEndDst )
            {
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pData->pSrc1+0, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+0, pData->srcStep2, pData->srcSize2,
                               pDstElement+0, pData->dstStep, dstPoint );
                    Calculate( pData->pSrc1+1, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+1, pData->srcStep2, pData->srcSize2,
                               pDstElement+1, pData->dstStep, dstPoint );
                    Calculate( pData->pSrc1+2, pData->srcStep1, pData->srcSize1,
                               pData->pSrc2+2, pData->srcStep2, pData->srcSize2,
                               pDstElement+2, pData->dstStep, dstPoint );
                    pDstElement += C4;
                    dstPoint.x += C4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                dstPoint.y++;
            }
        }
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;

        static inline FwStatus Run( const SrcType * pSrc1, int srcStep1, FwiSize srcSize1, 
                                     const SrcType * pSrc2, int srcStep2, FwiSize srcSize2, 
                                     DstType * pDst, int dstStep, int divisor )
        {
            if( pSrc1 == NULL || pSrc2 == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if( srcSize1.height <= 0 || srcSize1.width <= 0 || 
                srcSize2.height <= 0 || srcSize2.width <= 0 )
                return fwStsSizeErr;

            if( srcStep1 <= 0 || srcStep2 <= 0 || dstStep <= 0 )
                return fwStsStepErr;

            if( divisor == 0 )
                return fwStsDivisorErr;

            FwiSize dstRoiSize;
            Operator< chCount, SrcType, DstType, CalcType >::CalculateDstRoiSize( srcSize1, srcSize2, dstRoiSize );

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    DstType * pd = pDst;

                    int nStartRow = 0;
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc1				= pSrc1;
                        data[i].srcStep1			= srcStep1;				
                        data[i].srcSize1            = srcSize1;
                        data[i].pSrc2				= pSrc2;
                        data[i].srcStep2			= srcStep2;				
                        data[i].srcSize2            = srcSize2;
                        data[i].pDst				= pd;
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].divisor             = divisor;
                        data[i].startRow            = nStartRow;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)pSrc1) & (sizeof(SrcType)-1) ) || ( srcStep1 & 0x0F )
                         || ( ((size_t)pSrc2) & (sizeof(SrcType)-1) ) || ( srcStep2 & 0x0F )) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                        nStartRow += data[i].dstRoiSize.height;
                    }

                    ThreadPool::Wait();
                    return fwStsNoErr;
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc1				= pSrc1;
                    data.srcStep1			= srcStep1;
                    data.srcSize1           = srcSize1;
                    data.pSrc2				= pSrc2;
                    data.srcStep2			= srcStep2;
                    data.srcSize2           = srcSize2;
                    data.pDst				= pDst;
                    data.dstStep			= dstStep;
                    data.dstRoiSize         = dstRoiSize;
                    data.divisor            = divisor;
                    data.startRow           = 0;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );

                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    template< CH chCount, 
              typename SrcType, 
              typename DstType, 
              typename CalcType, 
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class Processor,
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType
    >
    struct DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
    {
        typedef DataType< SrcType, DstType > DataClass;

        static inline FwStatus Run( const SrcType * pSrc1, int srcStep1, FwiSize srcSize1, 
                                     const SrcType * pSrc2, int srcStep2, FwiSize srcSize2, 
                                     DstType * pDst, int dstStep )
        {
            if( pSrc1 == NULL || pSrc2 == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if( srcSize1.height <= 0 || srcSize1.width <= 0 || 
                srcSize2.height <= 0 || srcSize2.width <= 0 )
                return fwStsSizeErr;

            if( srcStep1 <= 0 || srcStep2 <= 0 || dstStep <= 0 )
                return fwStsStepErr;

            FwiSize dstRoiSize;
            Operator< chCount, SrcType, DstType, CalcType >::CalculateDstRoiSize( srcSize1, srcSize2, dstRoiSize );

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    DstType * pd = pDst;

                    int nStartRow = 0;
                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc1				= pSrc1;
                        data[i].srcStep1			= srcStep1;				
                        data[i].srcSize1            = srcSize1;
                        data[i].pSrc2				= pSrc2;
                        data[i].srcStep2			= srcStep2;				
                        data[i].srcSize2            = srcSize2;
                        data[i].pDst				= pd;
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].startRow            = nStartRow;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)pSrc1) & (sizeof(SrcType)-1) ) || ( srcStep1 & 0x0F )
                         || ( ((size_t)pSrc2) & (sizeof(SrcType)-1) ) || ( srcStep2 & 0x0F )) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                        nStartRow += data[i].dstRoiSize.height;
                    }

                    ThreadPool::Wait();
                    return fwStsNoErr;
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc1				= pSrc1;
                    data.srcStep1			= srcStep1;
                    data.srcSize1           = srcSize1;
                    data.pSrc2				= pSrc2;
                    data.srcStep2			= srcStep2;
                    data.srcSize2           = srcSize2;
                    data.pDst				= pDst;
                    data.dstStep			= dstStep;
                    data.dstRoiSize         = dstRoiSize;
                    data.startRow           = 0;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );

                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

} // namespace FilterConvolution


FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_8u_C1R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw8u * pDst, int dstStep, int divisor )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
        < 
        C1,
        Fw8u,
        Fw8u,
        Fw32s,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_8u_C3R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw8u * pDst, int dstStep, int divisor )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
        < 
        C3,
        Fw8u,
        Fw8u,
        Fw32s,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_16s_C1R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw16s * pDst, int dstStep, int divisor )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw64s,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_16s_C3R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw16s * pDst, int dstStep, int divisor )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw64s,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_32f_C1R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw32f * pDst, int dstStep )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
        < 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvFull_32f_C3R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw32f * pDst, int dstStep )
{
    return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
        < 
        C3,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
        FilterConvolution::FilterConvFull,
        FilterConvolution::ConvData
        >
    ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_8u_C1R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw8u * pDst, int dstStep, int divisor )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C1,
            Fw8u,
            Fw8u,
            Fw32s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C1,
            Fw8u,
            Fw8u,
            Fw32s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep, divisor );
    }
    else
        return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_8u_C3R)( const Fw8u * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw8u * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw8u * pDst, int dstStep, int divisor )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C3,
            Fw8u,
            Fw8u,
            Fw32s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C3,
            Fw8u,
            Fw8u,
            Fw32s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep, divisor );
    }
    else
        return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_16s_C1R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw16s * pDst, int dstStep, int divisor )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C1,
            Fw16s,
            Fw16s,
            Fw64s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C1,
            Fw16s,
            Fw16s,
            Fw64s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep, divisor );
    }
    else
        return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_16s_C3R)( const Fw16s * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw16s * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw16s * pDst, int dstStep, int divisor )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C3,
            Fw16s,
            Fw16s,
            Fw64s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep, divisor );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor
            < 
            C3,
            Fw16s,
            Fw16s,
            Fw64s,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep_divisor,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep, divisor );
    }
    else
        return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_32f_C1R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw32f * pDst, int dstStep )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
            < 
            C1,
            Fw32f,
            Fw32f,
            Fw32f,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
            < 
            C1,
            Fw32f,
            Fw32f,
            Fw32f,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep );
    }
    else
        return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiConvValid_32f_C3R)( const Fw32f * pSrc1, int srcStep1, FwiSize srcSize1,
                                     const Fw32f * pSrc2, int srcStep2, FwiSize srcSize2,
                                     Fw32f * pDst, int dstStep )
{
    if( srcSize1.height >= srcSize2.height && srcSize1.width >= srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
            < 
            C3,
            Fw32f,
            Fw32f,
            Fw32f,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc1, srcStep1, srcSize1, pSrc2, srcStep2, srcSize2, pDst, dstStep );
    } 
    else if( srcSize1.height < srcSize2.height && srcSize1.width < srcSize2.width )
    {
        return FilterConvolution::DivideAndConquerWithBuffer_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep
            < 
            C3,
            Fw32f,
            Fw32f,
            Fw32f,
            FilterConvolution::TProcessor_pSrc1_sStep1_sSize1_pSrc2_sStep2_sSize2_pDst_dStep,
            FilterConvolution::FilterConvValid,
            FilterConvolution::ConvData
            >
        ::Run( pSrc2, srcStep2, srcSize2, pSrc1, srcStep1, srcSize1, pDst, dstStep );
    }
    else
        return fwStsNoErr;
}

#endif // BUILD_NUM_AT_LEAST

//#endif

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
