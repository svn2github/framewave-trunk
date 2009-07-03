/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FILTER_PROCESSOR_TEMPLATES_H__
#define __FILTER_PROCESSOR_TEMPLATES_H__

// This file contains the implementations of the 'processor' class that iterates through image
// rows and columns, performs the calculations of writes the destination values
// The class provides three methods:  REFR (reference implementation), SSE2 (optimized implementation
// for the aligned data scenario, and SSE2_Unaligned (optimized implementation for the unaligned data
// scenario).
// By default the SSE2 and SSE2_Unaligned methods are not implemented and simply call back to the 
// reference implementation, but they are overwritten for specific SSE2 optimizaed functions.
//
// The input parameters are not passed directly, but are encapsulated in a Data class (or its derivative)
// and passed as a void pointer.
//
// The AC4 specialized versions of these classes do the alpha processing with alpha channel where the 
// calculations are only performed on the Red, Green and Blue channels, and the Alpha channel is not 
// touched in the destination buffer

namespace FilterCommon
{
    // this is the most basic scenario where only src, dst, step sizes and roi are needed to calculate
    // the results.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data rows, top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    // perform the pixel calculations
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep );
                    pSrcElement++;
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data rows, top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep );
                    Calculate( pSrcElement+1, pData->srcStep, pDstElement+1, pData->dstStep );
                    Calculate( pSrcElement+2, pData->srcStep, pDstElement+2, pData->dstStep );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
                // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor );
                    pSrcElement++;
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
                // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor );
                    Calculate( pSrcElement+1, pData->srcStep, pDstElement+1, pData->dstStep, pData->maskSize, pData->anchor );
                    Calculate( pSrcElement+2, pData->srcStep, pDstElement+2, pData->dstStep, pData->maskSize, pData->anchor );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, 
                               pDstElement, pData->dstStep, 
                               (const Fw32s *)pData->pKernel, pData->kernelSize, 
                               pData->anchor, pData->divisor );
                    pSrcElement++;
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, 
                               pDstElement, pData->dstStep, 
                               (const Fw32s *)pData->pKernel, pData->kernelSize, 
                               pData->anchor, pData->divisor );
                    Calculate( pSrcElement+1, pData->srcStep, 
                               pDstElement+1, pData->dstStep, 
                               (const Fw32s *)pData->pKernel, pData->kernelSize, 
                               pData->anchor, pData->divisor );
                    Calculate( pSrcElement+2, pData->srcStep, 
                               pDstElement+2, pData->dstStep, 
                               (const Fw32s *)pData->pKernel, pData->kernelSize, 
                               pData->anchor, pData->divisor );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, 
                               pDstElement, pData->dstStep, 
                               (const Fw32f *)pData->pKernel, pData->kernelSize, 
                               pData->anchor );
                    pSrcElement++;
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, 
                               pDstElement, pData->dstStep, 
                               (const Fw32f *)pData->pKernel, pData->kernelSize, 
                               pData->anchor );
                    Calculate( pSrcElement+1, pData->srcStep, 
                               pDstElement+1, pData->dstStep, 
                               (const Fw32f *)pData->pKernel, pData->kernelSize, 
                               pData->anchor );
                    Calculate( pSrcElement+2, pData->srcStep, 
                               pDstElement+2, pData->dstStep, 
                               (const Fw32f *)pData->pKernel, pData->kernelSize, 
                               pData->anchor );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    pSrcElement++;
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    Calculate( pSrcElement+1, pData->srcStep, pDstElement+1, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    Calculate( pSrcElement+2, pData->srcStep, pDstElement+2, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    *(pDstElement+3) = *(pSrcElement+3);
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    CalculateAllChannels( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor );
                    pSrcElement += chCount;
                    pDstElement += chCount;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< C4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    CalculateAllChannels( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    CalculateAllChannels( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    pSrcElement += chCount;
                    pDstElement += chCount;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor<
        AC4, SrcType, DstType, CalcType, Operator, DataType > : public Operator< AC4, SrcType, DstType, CalcType >
    {
        typedef DataType< SrcType, DstType > DataClass;

        static void REFR( void *p )
        {
            DataClass *pData = (DataClass*) p;
            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                // calculate the pointer to first source element in a given row of data
                const SrcType *pSrcElement = pSrc;
                // calculate the pointer to the first destination element in a given row of data
                DstType *pDstElement = pDst;
                // calculate the pointer to the last destination element in a given row of data
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * C4;
                // iterate through the destination data from left to right
                while( pDstElement < pLineEnd )
                {
                    CalculateAllChannels( pSrcElement, pData->srcStep, pDstElement, pData->dstStep, pData->maskSize, pData->anchor, pData->pBufferObj );
                    pSrcElement += 4;
                    pDstElement += 4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
            }
        }

        static void SSE2( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }

        static void SSE2_Unaligned( void *p )
        {
            DataClass *pData = (DataClass*) p;
            REFR( pData );
        }
    };

    // this class will be used by functions that perform automatic border replication 
    // such as fwiFilterMinBorderReplicate_8u_C1R
    // Instead of operating on the source data directly, they use a BorderBuffer that 
    // contains a copy of the source data and automatically creates the borders used a 
    // specified technique.
    // in order to minimize memory usage we the BorderBuffer object does not store the
    // entire source data at once.  Instead it only contains parts of it, and a call to the
    // RollBuffer function will "scroll" through the source data visible in the BorderBuffer
    // object.
    // This approach also prevents certain multi-threading issues when multiple worker threads are used. 

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename,
                        template< CH, typename > class > class Operator,
              template< typename, typename > class DataType,
              template< CH, typename > class BorderType 
            >
    struct TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer : public Operator< chCount, SrcType, DstType, CalcType, BorderType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            BorderType< chCount, SrcType > & borderBufferObj = *(BorderType< chCount, SrcType >*)(pData->pBufferObj);
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                DstType * pDstElement = pDst;
                for( int col = 0; col < pData->dstRoiSize.width * chCount; col++ )
                {
                    Calculate( borderBufferObj, col, pDstElement, pData->maskSize, pData->anchor );
                    pDstElement++;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                if( pDst < pEndDst )
                {
                    borderBufferObj.RollBuffer();
                }
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // this is a AC4 specialized version of the class above.
    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename,
                        template< CH, typename > class > class Operator,
              template< typename, typename > class DataType,
              template< CH, typename > class BorderType 
            >
    struct TProcessor_WithBorder_pSrc_sStep_pDst_dStep_roi_mask_anchor_buffer
            < AC4, SrcType, DstType, CalcType, Operator, DataType, BorderType >: public Operator< AC4, SrcType, DstType, CalcType, BorderType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            // calculate the pointer to the beginning of the source data
            BorderType< AC4, SrcType > & borderBufferObj = *(BorderType< AC4, SrcType >*)(pData->pBufferObj);
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            // iterate through the destination data from top to bottom
            while( pDst < pEndDst )
            {
                DstType * pDstElement = pDst;
                for( int col = 0; col < pData->dstRoiSize.width * AC4; )
                {
                    Calculate( borderBufferObj, col, pDstElement, pData->maskSize, pData->anchor );
                    Calculate( borderBufferObj, col+1, pDstElement+1, pData->maskSize, pData->anchor );
                    Calculate( borderBufferObj, col+2, pDstElement+2, pData->maskSize, pData->anchor );
                    pDstElement+=4;
                    col+=4;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                if( pDst < pEndDst )
                {
                    borderBufferObj.RollBuffer();
                }
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // the processor classes below do not iterate through destination pixels in the 
    // top-to-bottom, left-to-right order.  Instead they zigzag through the destination pixels.
    // Some filter functions can reuse some of the calculations from one pixel to the next, assuming
    // the we process the pixels in the right order.  That's why the zigzag pattern is useful since
    // help ensure the 'right order'.  The intermediate calculations data that is reused is stored 
    // in the pBufferObj member of the data class object.
    // Several different zigzag patterns are defined: RDRD, DRDR, RDLD and DRUR
    //
    // The zigzag processor class templates assume that the operator class has defined not only a generic
    // Calculate method, but also specialized CalculateDown, CalculateUp, CalculateRight and CalculateLeft
    // methods.  The purpose of the specialized methods is to reuse the intermediate values from 
    // previous calculations.
    // Calculate will be called on the first pixel in the image
    // CalculateDown will be called on each subsequent pixel while iterating from top to bottom
    // CalculateUp will be called on each subsequent pixel while iterating from bottom to top
    // CalculateRight will be called on each subsequent pixel while iterating from left to right
    // CalculateLeft will be called on each subsequent pixel while iterating from right to left


    // Right Down Right Down (RDRD) zigzag pattern
    // -------------------------
    // |  --> -->  1  --> -->  |
    // |                       |
    // |  --> -->  2  --> -->  |
    // |                       |
    // |  --> --> ... --> -->  |
    // |                       |
    // |  --> --> N-1 --> -->  |
    // |                       |
    // |  --> -->  N  --> -->  |
    // -------------------------
    // This is nearly identical to the normal top-to-bottom, left-to-right approach
    // but also passes the pBufferObj parameter to the Calculate function to faciliate
    // reuse of the intermediate calculations.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagRDRD : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            const SrcType * pSrc = pData->pSrc;
            // calculate the pointer to the beginning of the source data
            pSrc -= pData->anchor * chCount;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );

            // iterate through rows, from top to bottom
            while( pDst < pEndDst )
            {
                // at the beginning of each row start by calling the generic Calculate method that does
                // not reuse any previous calculations
                Calculate(  pSrc, pData->srcStep,
                            pDst, pData->dstStep,
                            pData->maskSize, 
                            pData->anchor,
                            pData->pBufferObj );

                // now iterate through remaining pixels in this row, from left to right and call the 
                // CalculateRight function that will reuse the intermediate values from the previous 
                // calculation (the intermediate values are passed in and out via the pBufferObj pointer.
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst + chCount;
                DstType *pLineEnd = pDst + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    CalculateRight(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement += chCount;
                    pDstElement += chCount;
                }

                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // AC4 spezialized version of the class above
    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagRDRD< AC4, SrcType, DstType, CalcType, Operator, DataType > 
            : public Operator< AC4, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * C4;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );

            while( pDst < pEndDst )
            {
                // at the beginning of each row start by calling the generic Calculate method that does
                // not reuse any previous calculations
                Calculate(  pSrc, pData->srcStep,
                            pDst, pData->dstStep,
                            pData->maskSize, 
                            pData->anchor,
                            pData->pBufferObj );

                // now iterate through remaining pixels in this row, from left to right and call the 
                // CalculateRight function that will reuse the intermediate values from the previous 
                // calculation (the intermediate values are passed in and out via the pBufferObj pointer.
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst + C4;
                DstType *pLineEnd = pDst + pData->dstRoiSize.width * C4;
                while( pDstElement < pLineEnd )
                {
                    CalculateRight(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement += C4;
                    pDstElement += C4;
                }

                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);
            }
        }

        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // Down Right Down Right (DRDR) zigzag pattern
    // -------------------------
    // |  |  |  |  |  |  |  |  |
    // |  v  v  v  v  v  v  v  |
    // |  |  |  |  |  |  |  |  |
    // |  v  v  v  v  v  v  v  |
    // |                       |
    // |  1  2  3  .... N-1 N  |
    // |                       |
    // |  |  |  |  |  |  |  |  |
    // |  v  v  v  v  v  v  v  |
    // |  |  |  |  |  |  |  |  |
    // |  v  v  v  v  v  v  v  |
    // -------------------------
    // In this pattern we process the pixels by columns (from left to right), each column top-to-bottom
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagDRDR : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = (const SrcType *)(((Fw8u*)pData->pSrc) - pData->anchor * pData->srcStep);
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = pData->pDst + pData->dstRoiSize.width * chCount;

            // iterate through columns, from left to right
            while( pDst < pEndDst )
            {
                // at the top of each column call the generic Calculate method
                Calculate(  pSrc, pData->srcStep,
                            pDst, pData->dstStep,
                            pData->maskSize, 
                            pData->anchor,
                            pData->pBufferObj );

                // now iterate through remaining pixels in this column, top to bottom, calling
                // CalculateDown to reuse the intermediate results of the previous calculations.
                // The intermediate results are passed in/out via the pBufferObj pointer.
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = FW_REF::Offset(pDst, pData->dstStep); // start one pixel down
                DstType *pColumnEnd = FW_REF::Offset( pDst, pData->dstRoiSize.height * pData->dstStep );
                while( pDstElement < pColumnEnd )
                {
                    CalculateDown(  pSrcElement, pData->srcStep,
                                    pDstElement, pData->dstStep,
                                    pData->maskSize, 
                                    pData->anchor,
                                    pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep);
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                pSrc += chCount;
                pDst += chCount;
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    //AC4 specialized version of the class above
    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagDRDR< AC4, SrcType, DstType, CalcType, Operator, DataType > 
            : public Operator< AC4, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * C4;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = pData->pDst + pData->dstRoiSize.width * C4;

            // iterate through columns, from left to right
            while( pDst < pEndDst )
            {
                // at the top of each column call the generic Calculate method
                Calculate(  pSrc, pData->srcStep,
                            pDst, pData->dstStep,
                            pData->maskSize, 
                            pData->anchor,
                            pData->pBufferObj );

                // now iterate through remaining pixels in this column, top to bottom, calling
                // CalculateDown to reuse the intermediate results of the previous calculations.
                // The intermediate results are passed in/out via the pBufferObj pointer.
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = FW_REF::Offset(pDst, pData->dstStep); // start one pixel down
                DstType *pColumnEnd = FW_REF::Offset( pDst, pData->dstRoiSize.height * pData->dstStep );
                while( pDstElement < pColumnEnd )
                {
                    CalculateDown(  pSrcElement, pData->srcStep,
                                    pDstElement, pData->dstStep,
                                    pData->maskSize, 
                                    pData->anchor,
                                    pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep);
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                pSrc += C4;
                pDst += C4;
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // Right Down Left Down (RDLD) zigzag pattern
    // -------------------------
    // |  --> -->  1  --> -->  |
    // |                       |
    // |  <-- <--  2  <-- <--  |
    // |                       |
    // |  --> --> ... --> -->  |
    // |                       |
    // |  <-- <-- ... <-- <--  |
    // |                       |
    // |  --> --> N-1 --> -->  |
    // |                       |
    // |  <-- <--  N  <-- <--  |
    // |                       |
    // -------------------------
    // in this patter we process the rows from top to bottom.
    // odd numbered rows will be processed from left to right and the even numbered rows
    // will be processed from right to left, giving a complete zigzag over the entier image
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagRDLD : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * chCount;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );

            // start in the top left corner of the image
            Calculate(  pSrc, pData->srcStep,
                        pDst, pData->dstStep,
                        pData->maskSize, 
                        pData->anchor,
                        pData->pBufferObj );

            // now iterate through all rows, top to bottom
            while( pDst < pEndDst )
            {
                // for odd numbered rows iterate through each pixel in the row from left to right
                // and call the CalculateRight function, reusing the intermediate calculations from
                // a previous call.
                // The intermediate values are passed in/out via the pBufferObj pointer.
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst + chCount;
                DstType *pLineEnd = pDst + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    CalculateRight(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement += chCount;
                    pDstElement += chCount;
                }

                // move to the next row, call CalculateDown, reusing the intermediate calculations from
                // a previous call.
                pDstElement = FW_REF::Offset(pDstElement, pData->dstStep - chCount * sizeof(DstType));
                if( pDstElement >= pEndDst )
                    break;

                CalculateDown(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );

                // for even numbered rows iterate through each pixel in the row from right to left
                // and call the CalculateLeft function, reusing the intermediate calculations from
                // a previous call.
                // The intermediate values are passed in/out via the pBufferObj pointer.

                pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep );
                pDstElement -= chCount;
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);

                while( pDstElement >= pDst )
                {
                    CalculateLeft(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement -= chCount;
                    pDstElement -= chCount;
                }

                // move to the next row, call CalculateDown, reusing the intermediate calculations from
                // a previous call.
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + chCount * sizeof(DstType) );

                if( pDst >= pEndDst )
                    break;

                CalculateDown(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // AC4 specialized version of the class above
    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagRDLD< AC4, SrcType, DstType, CalcType, Operator, DataType > 
            : public Operator< AC4, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * C4;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );

            Calculate(  pSrc, pData->srcStep,
                        pDst, pData->dstStep,
                        pData->maskSize, 
                        pData->anchor,
                        pData->pBufferObj );

            while( pDst < pEndDst )
            {
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst + C4;
                DstType *pLineEnd = pDst + pData->dstRoiSize.width * C4;
                while( pDstElement < pLineEnd )
                {
                    CalculateRight(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement += C4;
                    pDstElement += C4;
                }

                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep - C4 * sizeof(DstType) );
                if( pDstElement >= pEndDst )
                    break;

                CalculateDown(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );
                pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep );
                pDstElement -= C4;
                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);

                while( pDstElement >= pDst )
                {
                    CalculateLeft(  pSrcElement, pData->srcStep,
                                     pDstElement, pData->dstStep,
                                     pData->maskSize, 
                                     pData->anchor,
                                     pData->pBufferObj );
                    pSrcElement -= C4;
                    pDstElement -= C4;
                }

                pSrc = FW_REF::Offset( pSrc, pData->srcStep );
                pDst = FW_REF::Offset(pDst, pData->dstStep);
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + C4 * sizeof(DstType) );

                if( pDst >= pEndDst )
                    break;

                CalculateDown(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // Down Right Up Right (DRUR) zigzag pattern
    // -------------------------
    // |  |  ^  |  ^  |  ^  |  |
    // |  v  |  v  |  v  |  v  |
    // |  |  ^  |  ^  |  ^  |  |
    // |  v  |  v  |  v  |  v  |
    // |                       |
    // |  1  2  3  .... N-1 N  |
    // |                       |
    // |  |  ^  |  ^  |  ^  |  |
    // |  v  |  v  |  v  |  v  |
    // |  |  ^  |  ^  |  ^  |  |
    // |  v  |  v  |  v  |  v  |
    // -------------------------
    // In this pattern we process the pixels by columns (from left to right).
    // Odd numbered columns will be processed top to bottom and even numbered columns
    // will be processed bottom to top
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagDRUR : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * chCount;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = pData->pDst + pData->dstRoiSize.width * chCount;
            DstType * pEndDst2 = FW_REF::Offset( pEndDst, (pData->dstRoiSize.height-1) * pData->dstStep );

            // start in the top left corner
            Calculate(  pSrc, pData->srcStep,
                        pDst, pData->dstStep,
                        pData->maskSize, 
                        pData->anchor,
                        pData->pBufferObj );

            // iterate through all columns, left to right
            while( pDst < pEndDst )
            {
                // for odd numbered columns iterate through all pixels in the column from top to bottom
                // call CalculateDown, resuing the data from the previous pixel calculations
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = FW_REF::Offset(pDst, pData->dstStep); // start one pixel down
                DstType *pColumnEnd = FW_REF::Offset( pDst, pData->dstRoiSize.height * pData->dstStep );
                while( pDstElement < pColumnEnd )
                {
                    CalculateDown(  pSrcElement, pData->srcStep,
                                    pDstElement, pData->dstStep,
                                    pData->maskSize, 
                                    pData->anchor,
                                    pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep);
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                // move to the next column, call CalculateRight to reuse the data from the previous pixel calculation
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + chCount * sizeof(DstType) );
                if( pDstElement >= pEndDst2 )
                    break;

                CalculateRight(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );

                // for even numbered columns iterate through all pixels in the column from bottom to top
                // call CalculateUp, resuing the data from the previous pixel calculations
                pSrcElement += chCount;
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                pSrc += chCount;
                pDst += chCount;

                while( pDstElement >= pDst )
                {
                    CalculateUp(  pSrcElement, pData->srcStep,
                                  pDstElement, pData->dstStep,
                                  pData->maskSize, 
                                  pData->anchor,
                                  pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep );
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                // move to the next column calling CalculateRight
                pSrc += chCount;
                pDst += chCount;
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + chCount * sizeof(DstType) );

                if( pDst >= pEndDst )
                    break;

                CalculateRight(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };

    // AC4 specialized version of the template class above
    template< typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_ZigZagDRUR< AC4, SrcType, DstType, CalcType, Operator, DataType > 
            : public Operator< AC4, SrcType, DstType, CalcType >
    {
        static void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            // calculate the pointer to the beginning of the source data
            const SrcType * pSrc = FW_REF::Offset( pData->pSrc, pData->anchor.y * pData->srcStep );
            pSrc -= pData->anchor.x * C4;
            // calculate the pointer to the beginning of the destination data
            DstType * pDst = pData->pDst;
            // calculate the pointer to the end of the destination data
            DstType * pEndDst = pData->pDst + pData->dstRoiSize.width * C4;
            DstType * pEndDst2 = FW_REF::Offset( pEndDst, (pData->dstRoiSize.height-1) * pData->dstStep );

            // start in the top left corner
            Calculate(  pSrc, pData->srcStep,
                        pDst, pData->dstStep,
                        pData->maskSize, 
                        pData->anchor,
                        pData->pBufferObj );

            // iterate through all columns, left to right
            while( pDst < pEndDst )
            {
                // for odd numbered columns iterate through all pixels in the column from top to bottom
                // call CalculateDown, resuing the data from the previous pixel calculations
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = FW_REF::Offset(pDst, pData->dstStep); // start one pixel down
                DstType *pColumnEnd = FW_REF::Offset( pDst, pData->dstRoiSize.height * pData->dstStep );
                while( pDstElement < pColumnEnd )
                {
                    CalculateDown(  pSrcElement, pData->srcStep,
                                    pDstElement, pData->dstStep,
                                    pData->maskSize, 
                                    pData->anchor,
                                    pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep );
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                // move to the next column, call CalculateRight to reuse the data from the previous pixel calculation

                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + C4 * sizeof(DstType) );
                if( pDstElement >= pEndDst2 )
                    break;

                CalculateRight(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );

                // for even numbered columns iterate through all pixels in the column from bottom to top
                // call CalculateUp, resuing the data from the previous pixel calculations
  
                pSrcElement += C4;
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                pSrc += C4;
                pDst += C4;

                while( pDstElement >= pDst )
                {
                    CalculateUp(  pSrcElement, pData->srcStep,
                                  pDstElement, pData->dstStep,
                                  pData->maskSize, 
                                  pData->anchor,
                                  pData->pBufferObj );
                    pSrcElement = FW_REF::Offset( pSrcElement, pData->srcStep );
                    pDstElement = FW_REF::Offset( pDstElement, pData->dstStep );
                }

                // move to the next column, call CalculateRight to reuse the data from the previous pixel calculation

                pSrc += C4;
                pDst += C4;
                pDstElement = FW_REF::Offset( pDstElement, pData->dstStep + C4 * sizeof(DstType) );

                if( pDst >= pEndDst )
                    break;

                CalculateRight(  pSrcElement, pData->srcStep,
                                 pDstElement, pData->dstStep,
                                 pData->maskSize, 
                                 pData->anchor,
                                 pData->pBufferObj );
            }
        }
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            REFR( pData );
        }
    };
}

#endif // __FILTER_PROCESSOR_TEMPLATES_H__
