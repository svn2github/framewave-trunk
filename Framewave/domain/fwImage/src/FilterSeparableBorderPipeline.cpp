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

namespace FiltersSeparableWithBorder
{
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterColumn
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32s * pKernel,
                                        int kernelSize, 
                                        int yAnchor, int divisor )
        {
            CalcType sum = 0;
            
            for( int k = 0; k < kernelSize; k++ )
            {
                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( k - yAnchor ) * srcStep );
                const Fw32s  * pKrn = pKernel + k;

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            sum = (CalcType)((int)sum / divisor);

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterColumn32f
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32f * pKernel,
                                        int kernelSize, 
                                        int yAnchor )
        {
            CalcType sum = 0;
            
            for( int k = 0; k < kernelSize; k++ )
            {
                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( k - yAnchor ) * srcStep );
                const Fw32f  * pKrn = pKernel + k;

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterRow
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32s * pKernel,
                                        int kernelSize, 
                                        int xAnchor, int divisor )
        {
            CalcType sum = 0;
            
            for( int k = 0; k < kernelSize; k++ )
            {
                const SrcType * pSrc = pSrcPtr + ( k - xAnchor ) * chCount;
                const Fw32s  * pKrn = pKernel + k;

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            sum = (CalcType)((int)sum / divisor);

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterRow32f
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32f * pKernel,
                                        int kernelSize, 
                                        int xAnchor )
        {
            CalcType sum = 0;
            
            for( int k = 0; k < kernelSize; k++ )
            {
                const SrcType * pSrc = pSrcPtr + ( k - xAnchor ) * chCount;
                const Fw32f  * pKrn = pKernel + k;

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< typename SrcDataType, typename DstDataType >
    struct RowData
    {
        const SrcDataType *pSrc;
        int                srcStep;
        DstDataType      **ppDst;
        FwiSize           dstRoiSize;
        void*              pKernel;
        int                kernelSize;
        int                xAnchor;
        int                divisor;
    };

    template< CH chCount, typename SrcType >
    class ImageDuplicator
    {
    public:
        inline static void DuplicateImage( SrcType *pDst, int dstStep, const SrcType *pSrc, int srcStep, const FwiSize & roiSize, int xAnchor )
        {
            const int lineLength = roiSize.width * chCount * sizeof(SrcType);
            pDst += xAnchor * chCount;
            const SrcType *pSrcEnd = (const SrcType*)(((Fw8u*)pSrc) + srcStep * roiSize.height);
            while( pSrc < pSrcEnd )
            {
                memcpy( pDst, pSrc, lineLength );
                pDst = (SrcType*)(((Fw8u*)pDst) + dstStep );
                pSrc = (const SrcType*)(((Fw8u*)pSrc) + srcStep ); 
            }
        }

        inline static void DuplicateImage( SrcType *pDst, int dstStep, const SrcType **ppSrc, const FwiSize & roiSize, int yAnchor )
        {
            const int lineLength = roiSize.width * chCount * sizeof(SrcType);
            pDst = (SrcType*)(((Fw8u*)pDst) + yAnchor * dstStep );
            const SrcType ** ppSrcEnd = ppSrc + roiSize.height;
            while( ppSrc < ppSrcEnd )
            {
                const SrcType * pSrc = *ppSrc;
                memcpy( pDst, pSrc, lineLength );
                pDst = (SrcType*)(((Fw8u*)pDst) + dstStep );
                ppSrc++;
            }
        }
    };

    template< CH chCount, typename SrcType >
    class BorderReplicatorVertical
    {
        inline static void ConstBorder( SrcType *pDst, int width, int height, int kernelSize, int yAnchor, SrcType borderValue[chCount] )
        {
            const int topBorder = yAnchor;
            const int btmBorder = kernelSize - yAnchor - 1;
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * topBorder );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * btmBorder );

            // top border
            while( pDst < pTop )
            {
                SrcType *pElement = pDst;
                SrcType *pLineEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                while( pElement < pLineEnd )
                {
                    *(pElement+0) = borderValue[0];
                    *(pElement+1) = borderValue[1];
                    *(pElement+2) = borderValue[2];
                    pElement += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            pDst = pBtm;
            // bottom border
            while( pDst < pEnd )
            {
                SrcType *pElement = pDst;
                SrcType *pLineEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                while( pElement < pLineEnd )
                {
                    *(pElement+0) = borderValue[0];
                    *(pElement+1) = borderValue[1];
                    *(pElement+2) = borderValue[2];
                    pElement += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void ReplBorder( SrcType *pBuffer, int width, int height, int kernelSize, int yAnchor )
        {
            const int topBorder = yAnchor;
            const int btmBorder = kernelSize - yAnchor - 1;
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * topBorder );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * btmBorder );

            // top border
            SrcType * pSrc = pTop;
            SrcType * pDst = pBuffer;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            // bottom border
            pSrc = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );
            pDst = pBtm;
            while( pDst < pEnd )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void WrapBorder( SrcType *pBuffer, int width, int height, int kernelSize, int yAnchor )
        {
            const int topBorder = yAnchor;
            const int btmBorder = kernelSize - yAnchor - 1;
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * topBorder );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * btmBorder );

            // top border
            SrcType * pSrc = (SrcType *)(((Fw8u*)pBtm) - topBorder * nAlignedStepSize );
            SrcType * pDst = pBuffer;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
            }

            // bottom border
            pSrc = pTop;
            pDst = pBtm;
            while( pDst < pEnd )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
            }
        }

        inline static void MirrorBorder( SrcType *pBuffer, int width, int height, int kernelSize, int yAnchor )
        {
            const int topBorder = yAnchor;
            const int btmBorder = kernelSize - yAnchor - 1;
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * topBorder );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * btmBorder );

            // top border
            SrcType * pDst = (SrcType *)(((Fw8u*)pTop) - nAlignedStepSize );
            SrcType * pSrc = pTop;
            while( pDst >= pBuffer )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) - nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
            }

            // bottom border
            pSrc = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );
            pDst = pBtm;
            while( pDst < pEnd )
            {
                memcpy( pDst, pSrc, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) - nAlignedStepSize );
            }
        }

        inline static void Mirror2Border( SrcType *pBuffer, int width, int height, int kernelSize, int yAnchor )
        {
            pBuffer;
            width;
            height;
            kernelSize;
            yAnchor;
        }
        
        inline static int StepSize( const int width )
        {
            int len = width * chCount * sizeof( SrcType );
            return len;
        }

    public:
        inline static int AlignedStepSize( const int width )
        {
            int len = StepSize( width ) + 16;
            len &= ~(0xF);
            return len;
        }

        inline static FwStatus GetBufferSize( const FwiSize & roiSize, int kernelSize, int *pBufferSize )
        {
            if( pBufferSize == NULL )
                return fwStsNullPtrErr;
            if( roiSize.width <= 0 || roiSize.height <= 0 )
                return fwStsSizeErr;

            int bytes = AlignedStepSize( roiSize.width );
            bytes *= roiSize.height + kernelSize - 1;
            *pBufferSize = bytes;
            return fwStsNoErr;
        }

        inline static FwStatus CreateBorder( SrcType *pBuffer, const FwiSize & roiSize, FwiBorderType borderType, 
                                              SrcType borderValue[3], SrcType **pNewSrc, int kernelSize, int yAnchor )
        {
            switch( borderType )
            {
            case fwBorderConst:
                ConstBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, yAnchor, borderValue );
                break;
            case fwBorderRepl:
                ReplBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, yAnchor );
                break;
            case fwBorderWrap:
                WrapBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, yAnchor );
                break;
            case fwBorderMirror:
                MirrorBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, yAnchor );
                break;
            case fwBorderMirror2:
                Mirror2Border( pBuffer, roiSize.width, roiSize.height, kernelSize, yAnchor );
                break;
            }

            int nAlignedStepSize = AlignedStepSize( roiSize.width );
            SrcType * pCorrectedSrc = (SrcType *)(((Fw8u*)pBuffer) + yAnchor * nAlignedStepSize );
            *pNewSrc = pCorrectedSrc;

            return fwStsNoErr;
        }
    };

    template< CH chCount, typename SrcType >
    class BorderReplicatorHorizontal
    {
        inline static void ConstBorder( SrcType *pDst, int width, int height, const SrcType borderValue[chCount], int kernelSize, int xAnchor )
        {
            const int leftBorder = xAnchor * chCount;
            const int middlePart = width * chCount;
            const int nAlignedStepSize = AlignedStepSize( width, kernelSize );

            SrcType * pEnd = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * height );

            while( pDst < pEnd )
            {
                SrcType *pLeftBorderEnd = pDst + leftBorder;

                // left and right borders
                SrcType *pElement = pDst;
                SrcType *pElement2 = pDst + leftBorder + middlePart;
                while( pElement < pLeftBorderEnd )
                {
                    for( int n = 0; n < chCount; n++ )
                    {
                        *(pElement+n) = borderValue[n];
                        *(pElement2+n) = borderValue[n];
                    }
                    pElement += chCount;
                    pElement2 += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void ReplBorder( SrcType *pBuffer, int width, int height, int kernelSize, int xAnchor )
        {
            const int leftBorder = xAnchor * chCount;
            const int middlePart = width * chCount;
            const int nAlignedStepSize = AlignedStepSize( width, kernelSize );

            SrcType * pDst = pBuffer;
            SrcType * pEnd = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * height );

            while( pDst < pEnd )
            {
                SrcType *pLeftBorderEnd = pDst + leftBorder;

                // left and right borders
                SrcType *pElement = pDst;
                SrcType *pElement2 = pDst + leftBorder + middlePart;
                SrcType *pSrc = pDst + leftBorder;
                SrcType *pSrc2 = pElement2 - chCount;
                while( pElement < pLeftBorderEnd )
                {
                    for( int n = 0; n < chCount; n++ )
                    {
                        *(pElement+n) = pSrc[n];
                        *(pElement2+n) = pSrc2[n];
                    }
                    pElement += chCount;
                    pElement2 += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void WrapBorder( SrcType *pBuffer, int width, int height, int kernelSize, int xAnchor )
        {
            const int leftBorder = xAnchor * chCount;
            const int middlePart = width * chCount;
            const int nAlignedStepSize = AlignedStepSize( width, kernelSize );

            SrcType * pDst = pBuffer;
            SrcType * pEnd = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * height );

            while( pDst < pEnd )
            {
                SrcType *pLeftBorderEnd = pDst + leftBorder;

                // left and right borders
                SrcType *pElement = pDst;
                SrcType *pElement2 = pDst + leftBorder + middlePart;
                SrcType *pSrc = pElement2 - leftBorder;
                SrcType *pSrc2 = pLeftBorderEnd;
                while( pElement < pLeftBorderEnd )
                {
                    *pElement = *pSrc;
                    *pElement2 = *pSrc2;
                    pElement++;
                    pElement2++;
                    pSrc++;
                    pSrc2++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void MirrorBorder( SrcType *pBuffer, int width, int height, int kernelSize, int xAnchor )
        {
            const int leftBorder = xAnchor * chCount;
            const int middlePart = width * chCount;
            const int nAlignedStepSize = AlignedStepSize( width, kernelSize );

            SrcType * pDst = pBuffer;
            SrcType * pEnd = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * height );

            while( pDst < pEnd )
            {
                // left and right borders
                SrcType *pElement2 = pDst + leftBorder + middlePart;
                SrcType *pSrc2 = pElement2 - chCount;
                SrcType *pSrc = pDst + leftBorder;
                SrcType *pElement = pSrc - chCount;
                while( pElement >= pDst )
                {
                    for( int n = 0; n < chCount; n++ )
                    {
                        pElement[n] = pSrc[n];
                        pElement[n] = pSrc2[n];
                    }
                    pElement -= chCount;
                    pElement2 += chCount;
                    pSrc += chCount;
                    pSrc2 -= chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void Mirror2Border( SrcType *pBuffer, int width, int height, int kernelSize, int xAnchor )
        {
            pBuffer;
            width;
            height;
            kernelSize;
            xAnchor;
        }
        
        inline static int StepSize( const int width, const int kernelSize )
        {
            int len = ( width + kernelSize - 1 ) * chCount * sizeof( SrcType );
            return len;
        }

    public:
        inline static int AlignedStepSize( const int width, const int kernelSize )
        {
            int len = StepSize( width, kernelSize ) + 16;
            len &= ~(0xF);
            return len;
        }

        inline static FwStatus GetBufferSize( const FwiSize & roiSize, const int kernelSize, int *pBufferSize )
        {
            if( pBufferSize == NULL )
                return fwStsNullPtrErr;
            if( roiSize.width <= 0 || roiSize.height <= 0 )
                return fwStsSizeErr;

            int bytes = AlignedStepSize( roiSize.width, kernelSize );
            bytes *= roiSize.height;
            *pBufferSize = bytes;
            return fwStsNoErr;
        }

        inline static FwStatus CreateBorder( SrcType *pBuffer, const FwiSize & roiSize, 
                                              FwiBorderType borderType, const SrcType borderValue[3], 
                                              SrcType **pNewSrc, int kernelSize, int xAnchor )
        {
            switch( borderType )
            {
            case fwBorderConst:
                ConstBorder( pBuffer, roiSize.width, roiSize.height, borderValue, kernelSize, xAnchor );
                break;
            case fwBorderRepl:
                ReplBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, xAnchor );
                break;
            case fwBorderWrap:
                WrapBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, xAnchor );
                break;
            case fwBorderMirror:
                MirrorBorder( pBuffer, roiSize.width, roiSize.height, kernelSize, xAnchor );
                break;
            case fwBorderMirror2:
                Mirror2Border( pBuffer, roiSize.width, roiSize.height, kernelSize, xAnchor );
                break;
            }

            *pNewSrc = pBuffer + xAnchor * chCount;

            return fwStsNoErr;
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            const SrcType * pSrc = pData->pSrc;
            DstType ** ppDst = pData->ppDst;
            DstType ** ppDstEnd = pData->ppDst + pData->dstRoiSize.height;
            while( ppDst < ppDstEnd )
            {
                DstType * pDst = *ppDst;
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, 0,
                               (const Fw32s *)pData->pKernel, pData->kernelSize, 
                               pData->xAnchor, pData->divisor );
                    pSrcElement++;
                    pDstElement++;
                }
                ppDst++;
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
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
    struct TProcessor_pSrc_sStep_ppDst_roi_kernel32f_size_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            const SrcType * pSrc = pData->pSrc;
            DstType ** ppDst = pData->ppDst;
            DstType ** ppDstEnd = pData->ppDst + pData->dstRoiSize.height;
            while( ppDst < ppDstEnd )
            {
                DstType * pDst = *ppDst;
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, 0,
                               (const Fw32f *)pData->pKernel, pData->kernelSize, 
                               pData->xAnchor );
                    pSrcElement++;
                    pDstElement++;
                }
                ppDst++;
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
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
    struct DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
    {
        static inline FwStatus Run( const SrcType * pSrc, int srcStep, 
                                     DstType ** ppDst, FwiSize dstRoiSize, 
                                     const Fw32s * pKernel, int kernelSize, int xAnchor,
                                     FwiBorderType borderType, const SrcType borderValue[],
                                     int divisor, Fw8u * pBuffer )
        {
            typedef RowData< SrcType, DstType > DataClass;

            if( pSrc == NULL || ppDst == NULL || pBuffer == NULL )
                return fwStsNullPtrErr;
                                        
            if( dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if( srcStep <= 0 )
                return fwStsStepErr;

            if( kernelSize <= 0 )
                return fwStsMaskSizeErr;

            if( xAnchor <= 0 || xAnchor >= kernelSize )
                return fwStsAnchorErr;

            // determine the steps size of the image (with borders) in the temporary buffer
            int newSrcStep = BorderReplicatorHorizontal< chCount, SrcType >::AlignedStepSize( dstRoiSize.width, kernelSize );
            // copy the source image into the temporary buffer
            ImageDuplicator< chCount, SrcType >::DuplicateImage( (SrcType *)pBuffer, 
                                                                               newSrcStep, 
                                                                               pSrc, 
                                                                               srcStep, 
                                                                               dstRoiSize, 
                                                                               xAnchor );

            // create borders around the image in the temporary buffer
            SrcType * pNewSrc = NULL;
            FwStatus sts = BorderReplicatorHorizontal< chCount, SrcType >
                ::CreateBorder( (SrcType*)pBuffer, dstRoiSize, borderType, borderValue, &pNewSrc, kernelSize, xAnchor );
            if( sts != fwStsNoErr )
                return sts;

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

                    const SrcType * ps = pNewSrc;
                    DstType ** ppd = ppDst;

                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].ppDst				= ppd;
                        data[i].srcStep				= newSrcStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].xAnchor	            = xAnchor;
                        data[i].divisor	            = divisor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * newSrcStep );
                        ppd += data[i].dstRoiSize.height;
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pNewSrc;
                    data.srcStep = newSrcStep;
                    data.ppDst = ppDst;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.xAnchor = xAnchor;
                    data.divisor = divisor;

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
    struct DivideAndConquer_pSrc_sStep_ppDst_roi_kernel32f_size_anchor_borderType_borderValue_buffer
    {
        static inline FwStatus Run( const SrcType * pSrc, int srcStep, 
                                     DstType ** ppDst, FwiSize dstRoiSize, 
                                     const Fw32f * pKernel, int kernelSize, int xAnchor,
                                     FwiBorderType borderType, const SrcType borderValue[chCount],
                                     Fw8u * pBuffer )
        {
            typedef RowData< SrcType, DstType > DataClass;

            if( pSrc == NULL || ppDst == NULL || pBuffer == NULL )
                return fwStsNullPtrErr;
                                        
            if( dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if( srcStep <= 0 )
                return fwStsStepErr;

            if( kernelSize <= 0 )
                return fwStsMaskSizeErr;

            if( xAnchor <= 0 || xAnchor >= kernelSize )
                return fwStsAnchorErr;

            // determine the steps size of the image (with borders) in the temporary buffer
            int newSrcStep = BorderReplicatorHorizontal< chCount, SrcType >::AlignedStepSize( dstRoiSize.width, kernelSize );
            // copy the source image into the temporary buffer
            ImageDuplicator< chCount, SrcType >::DuplicateImage( (SrcType *)pBuffer, 
                                                                               newSrcStep, 
                                                                               pSrc, 
                                                                               srcStep, 
                                                                               dstRoiSize, 
                                                                               xAnchor );

            // create borders around the image in the temporary buffer
            SrcType * pNewSrc = NULL;
            FwStatus sts = BorderReplicatorHorizontal< chCount, SrcType >
                ::CreateBorder( (SrcType*)pBuffer, dstRoiSize, borderType, borderValue, &pNewSrc, kernelSize, xAnchor );
            if( sts != fwStsNoErr )
                return sts;

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

                    const SrcType * ps = pNewSrc;
                    DstType ** ppd = ppDst;

                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].ppDst				= ppd;
                        data[i].srcStep				= newSrcStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].xAnchor	            = xAnchor;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0x0F ) ) // is src data aligned or the src row step size 16 bytes aligned?
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * newSrcStep );
                        ppd += data[i].dstRoiSize.height;
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pNewSrc;
                    data.srcStep = newSrcStep;
                    data.ppDst = ppDst;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.xAnchor = xAnchor;

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
    struct DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
    {
        typedef DataType< SrcType, DstType > DataClass;

        static inline FwStatus Run( const SrcType ** ppSrc, DstType * pDst, int dstStep, FwiSize dstRoiSize,
                                     const Fw32s * pKernel, int kernelSize, int yAnchor,
                                     FwiBorderType borderType, SrcType borderValue[chCount], 
                                     int divisor, Fw8u * pBuffer )
        {
            if( ppSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if (dstStep <= 0)
                return fwStsStepErr;

            if( kernelSize <= 0 )
                return fwStsMaskSizeErr;

            if( yAnchor <= 0 || yAnchor >= kernelSize )
                return fwStsAnchorErr;

            // determine the steps size of the image (with borders) in the temporary buffer
            int newSrcStep = BorderReplicatorVertical< chCount, SrcType >::AlignedStepSize( dstRoiSize.width );
            // copy the source image into the temporary buffer
            ImageDuplicator< chCount, SrcType >::DuplicateImage( (SrcType *)pBuffer, newSrcStep, ppSrc, dstRoiSize, yAnchor );

            // create borders around the image in the temporary buffer
            SrcType * pNewSrc = NULL;
            FwStatus sts = BorderReplicatorVertical< chCount, SrcType >
                ::CreateBorder( (SrcType*)pBuffer, dstRoiSize, borderType, borderValue, &pNewSrc, kernelSize, yAnchor );
            if( sts != fwStsNoErr )
                return sts;

            // process the source data in the temporary buffer
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                chCount,
                SrcType,
                DstType,
                CalcType,
                Processor,
                Operator, DataType >
            ::Run( pNewSrc, newSrcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
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
    struct DivideAndConquer_ppSrc_pDst_dStep_roi_kernel32f_size_anchor_border_borderValue_buffer
    {
        typedef DataType< SrcType, DstType > DataClass;

        static inline FwStatus Run( const SrcType ** ppSrc, DstType * pDst, int dstStep, FwiSize dstRoiSize,
                                     const Fw32f * pKernel, int kernelSize, int yAnchor,
                                     FwiBorderType borderType, SrcType borderValue[chCount], 
                                     Fw8u * pBuffer )
        {
            if( ppSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;

            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
                return fwStsSizeErr;

            if (dstStep <= 0)
                return fwStsStepErr;

            if( kernelSize <= 0 )
                return fwStsMaskSizeErr;

            if( yAnchor <= 0 || yAnchor >= kernelSize )
                return fwStsAnchorErr;

            // determine the steps size of the image (with borders) in the temporary buffer
            int newSrcStep = BorderReplicatorVertical< chCount, SrcType >::AlignedStepSize( dstRoiSize.width );
            // copy the source image into the temporary buffer
            ImageDuplicator< chCount, SrcType >::DuplicateImage( (SrcType *)pBuffer, newSrcStep, ppSrc, dstRoiSize, yAnchor );

            // create borders around the image in the temporary buffer
            SrcType * pNewSrc = NULL;
            FwStatus sts = BorderReplicatorVertical< chCount, SrcType >
                ::CreateBorder( (SrcType*)pBuffer, dstRoiSize, borderType, borderValue, &pNewSrc, kernelSize, yAnchor );
            if( sts != fwStsNoErr )
                return sts;

            // process the source data in the temporary buffer
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
                chCount,
                SrcType,
                DstType,
                CalcType,
                Processor,
                Operator, DataType >
            ::Run( pNewSrc, newSrcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor );
        }
    };

} // namespace FiltersSeparable

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_8u16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C1, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_8u16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C3, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C1, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C3, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_32f_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C1, Fw32f >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_32f_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C3, Fw32f >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_Low_8u16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C1, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_Low_8u16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C3, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_Low_16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C1, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipelineGetBufferSize_Low_16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorHorizontal< C3, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_8u16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C1, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_8u16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C3, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C1, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C3, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_32f_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C1, Fw32f >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_32f_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C3, Fw32f >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_Low_8u16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C1, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_Low_8u16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C3, Fw8u >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_Low_16s_C1R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C1, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipelineGetBufferSize_Low_16s_C3R)( FwiSize dstRoiSize, int kernelSize, int * pBufferSize )
{
    return FiltersSeparableWithBorder::BorderReplicatorVertical< C3, Fw16s >::GetBufferSize( dstRoiSize, kernelSize, pBufferSize );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw8u borderValue,
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C1,
        Fw8u,
        Fw16s,
        Fw64s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_8u16s_C3R)( const Fw8u * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw8u borderValue[3],
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C3,
        Fw8u,
        Fw16s,
        Fw64s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw16s borderValue,
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw64s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw16s borderValue[3],
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw64s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_32f_C1R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f ** ppDst, FwiSize dstRoiSize,
                                              const Fw32f * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw32f borderValue,
                                              Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel32f_size_anchor_borderType_borderValue_buffer
        < 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel32f_size_anchor,
        FiltersSeparableWithBorder::FilterRow32f, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                              Fw32f ** ppDst, FwiSize dstRoiSize,
                                              const Fw32f * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw32f borderValue[3],
                                              Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel32f_size_anchor_borderType_borderValue_buffer
        < 
        C3,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel32f_size_anchor,
        FiltersSeparableWithBorder::FilterRow32f, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_Low_8u16s_C1R)( const Fw8u * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw8u borderValue,
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C1,
        Fw8u,
        Fw16s,
        Fw32s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_Low_8u16s_C3R)( const Fw8u * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw8u borderValue[3],
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C3,
        Fw8u,
        Fw16s,
        Fw32s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_Low_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw16s borderValue,
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw32s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRowBorderPipeline_Low_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                              Fw16s ** ppDst, FwiSize dstRoiSize,
                                              const Fw32s * pKernel, int kernelSize,
                                              int xAnchor, 
                                              FwiBorderType borderType, Fw16s borderValue[3],
                                              int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_pSrc_sStep_ppDst_roi_kernel_size_anchor_borderType_borderValue_divisor_buffer
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw32s,
        FiltersSeparableWithBorder::TProcessor_pSrc_sStep_ppDst_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterRow, 
        FiltersSeparableWithBorder::RowData 
        >
    ::Run( pSrc, srcStep, ppDst, dstRoiSize, pKernel, kernelSize, xAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_8u16s_C1R)( const Fw8u ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw8u borderValue,
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C1,
        Fw8u,
        Fw16s,
        Fw64s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_8u16s_C3R)( const Fw8u ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw8u borderValue[3],
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C3,
        Fw8u,
        Fw16s,
        Fw64s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_16s_C1R)( const Fw16s ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw16s borderValue,
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw64s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_16s_C3R)( const Fw16s ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw16s borderValue[3],
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw64s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_32f_C1R)( const Fw32f ** ppSrc, Fw32f * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32f * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw32f borderValue,
                                                   Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel32f_size_anchor_border_borderValue_buffer
        < 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_32f_C3R)( const Fw32f ** ppSrc, Fw32f * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32f * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw32f borderValue[3],
                                                   Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel32f_size_anchor_border_borderValue_buffer
        < 
        C3,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, borderValue, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_Low_8u16s_C1R)( const Fw8u ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw8u borderValue,
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C1,
        Fw8u,
        Fw16s,
        Fw32s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_Low_8u16s_C3R)( const Fw8u ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw8u borderValue,
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C3,
        Fw8u,
        Fw16s,
        Fw32s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_Low_16s_C1R)( const Fw16s ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw16s borderValue,
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw32s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, &borderValue, divisor, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumnBorderPipeline_Low_16s_C3R)( const Fw16s ** ppSrc, Fw16s * pDst, int dstStep,
                                                   FwiSize dstRoiSize, const Fw32s * pKernel, 
                                                   int kernelSize, int yAnchor, 
                                                   FwiBorderType borderType, Fw16s borderValue[3],
                                                   int divisor, Fw8u * pBuffer )
{
    return FiltersSeparableWithBorder::DivideAndConquer_ppSrc_pDst_dStep_roi_kernel_size_anchor_border_borderValue_divisor_buffer
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw32s,
        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
        FiltersSeparableWithBorder::FilterColumn, 
        FilterCommon::DataWithLinearKernel
        >
    ::Run( ppSrc, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, borderType, borderValue, divisor, pBuffer );
}

#endif // BUILD_NUM_AT_LEAST

//#endif

#endif

