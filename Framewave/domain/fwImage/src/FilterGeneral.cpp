/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
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
namespace FiltersGeneral
{
    // this is the generic, not-SSE-optimized implementation of the Filter_* functions
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct Filter
    {
        static SYS_INLINE void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32s * pKernel,
                                        const FwiSize & kernelSize, 
                                        const FwiPoint & anchor, int divisor = 1)
        {
            CalcType sum = 0;

            // the anchor coordinates are relative to the bottom-right corner of the kernel matrix
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;
            int H = kernelSize.height;
            int W = kernelSize.width;
            int P = H - anchor.y - 1;
            int Q = W - anchor.x - 1;

            for( int m = 0; m < H; m++ )
            {
                for( int n = 0; n < W; n++ )
                {
                    const SrcType * pSrc = FW_REF::Offset(pSrcPtr, ( m - P ) * srcStep + ( n - Q ) * actualChCount * sizeof(SrcType));
                    const Fw32s  * pKrn = pKernel + ( H - m - 1 ) * W + ( W - n - 1 );

                    sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
                }
            }

            sum = (CalcType)((int)sum / divisor);

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // this is the generic, not-SSE-optimized implementation of the Filter32f_* functions
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct Filter32f
    {
        static SYS_INLINE void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32f * pKernel,
                                        const FwiSize & kernelSize, 
                                        const FwiPoint & anchor )
        {
            CalcType sum = 0;
            
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;
            int H = kernelSize.height;
            int W = kernelSize.width;
            int P = H - anchor.y - 1;
            int Q = W - anchor.x - 1;

            for( int m = 0; m < H; m++ )
            {
                for( int n = 0; n < W; n++ )
                {
                    const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( m - P ) * srcStep + ( n - Q ) * actualChCount * sizeof(SrcType));
                    const Fw32f  * pKrn = pKernel + ( H - m - 1 ) * W + ( W - n - 1 );

                    sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
                }
            }

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // all SSE2 optimized implementations of the Filter_* and Filter32f_* functions come in two variants:
    // for aligned and for unaligned data.  The body of these functions is essentially the same and the only 
    // difference is the instructions used to access memory.
    // 
    // the processor class below implements both paths up the the point where we iterate over all destination
    // image pixels (row by row, column by column order), processes the misaligned prefix and postfix data 
    // using reference implementation and then calls into the innermost loop body SSE2_Aligned_Impl or SSE2_Unaligned_Impl.
  
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType,
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class SubProcessor
    >
    struct FilterProcessor_superclass : 
        public SubProcessor< chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        // aligned data code path
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int yoffset = pData->kernelSize.height - pData->anchor.y - 1;
            int xoffset = pData->kernelSize.width - pData->anchor.x - 1;
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;

            const SrcType *pSrc = FW_REF::Offset( pData->pSrc, - yoffset * pData->srcStep - xoffset * actualChCount * sizeof(SrcType) );
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = FW_REF::Offset( pData->pDst, pData->dstStep * pData->dstRoiSize.height );
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 divisor_inv;
            GetDivisorInv( divisor_inv, pData->divisor );

            // calculate destination mask for the 4 channel with alpha scenario
            XMM128 alphaMask;
            if( chCount == AC4 )
            {
                switch( sizeof(DstType) )
                {
                case 1:
                    alphaMask.i = _mm_set1_epi32( 0x00FFFFFF );
                    break;
                case 2:
                    alphaMask.i = _mm_set_epi32( 0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF );
                    break;
                case 4:
                    alphaMask.i = _mm_set_epi32( 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF );
                    break;
                }
            }

            XMM128 * kernel = (XMM128*)pData->pBufferObj;

            // iterate over each row of destination image
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width * actualChCount * sizeof(SrcType);
                U32 startWidth = BytesToAlign(ps, 16*actualChCount);	    // Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % (16*actualChCount);   // Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		        // Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                DstType *pEndStart  = FW_REF::Offset( pd, startWidth );			// End of unaligned data / Start of aligned data 			
                DstType *pEndMiddle = FW_REF::Offset( pd, startWidth + midwidth );	// End of aligned x 16 data.
                DstType *pEnd       = FW_REF::Offset( pd, width );				    // Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEndStart ) // Process 1 at a time until we hit an alignment boundary
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // process the "main" data
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndMiddle; ps+=(16/sizeof(SrcType)), pd+=(16/sizeof(SrcType)) )	// Process 1 horizontal line (16 wide)
                {
                    SSE2_Aligned_Impl( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEnd )					// Process the remainder.
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // move to the next line in the destination buffer
                /////////////////////////////////////////////////////////////////
                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned data code path
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int xoffset = pData->kernelSize.width - pData->anchor.x - 1;
            int yoffset = pData->kernelSize.height - pData->anchor.y - 1;
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;

            const SrcType *pSrc = FW_REF::Offset( pData->pSrc, - yoffset * pData->srcStep - xoffset * actualChCount * sizeof(SrcType) );
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = FW_REF::Offset( pData->pDst, pData->dstStep * pData->dstRoiSize.height );
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 divisor_inv;
            GetDivisorInv( divisor_inv, pData->divisor );

            XMM128 alphaMask;
            if( chCount == AC4 )
            {
                switch( sizeof(DstType) )
                {
                case 1:
                    alphaMask.i = _mm_set1_epi32( 0x00FFFFFF );
                    break;
                case 2:
                    alphaMask.i = _mm_set_epi32( 0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF );
                    break;
                case 4:
                    alphaMask.i = _mm_set_epi32( 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF );
                    break;
                }
            }

            XMM128 * kernel = (XMM128*)pData->pBufferObj;

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width * actualChCount * sizeof(SrcType);
                U32 endWidth   = width % (16*actualChCount);		// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - endWidth;		    // Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                DstType *pEndMiddle = FW_REF::Offset( pd, midwidth );	// End of aligned x 16 data.
                DstType *pEnd       = FW_REF::Offset( pd, width );		// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the "main" data
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndMiddle; ps+=(16/sizeof(SrcType)), pd+=(16/sizeof(DstType)) )				// Process 1 horizontal line (16 wide)
                {
                        SSE2_Unaligned_Impl( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEnd )
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // move to the next line in the destination buffer
                /////////////////////////////////////////////////////////////////
                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // this class template is equivalent to the one above, but it does not pass the divisor argument
    // this is useful in cases where the divisor is 1 or -1 and we do not have to perform division
    // in the innermost loop
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType,
              template< CH, 
                        typename, 
                        typename, 
                        typename, 
                        template< CH, typename, typename, typename > class,
                        template< typename, typename > class > class SubProcessor,
              typename KernelType = Fw32f
    >
    struct FilterProcessor_NoDivision_superclass : 
        public SubProcessor< chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int yoffset = pData->kernelSize.height - pData->anchor.y - 1;
            int xoffset = pData->kernelSize.width - pData->anchor.x - 1;
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;

            const SrcType *pSrc = FW_REF::Offset( pData->pSrc, - yoffset * pData->srcStep - xoffset * actualChCount * sizeof(SrcType) );
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = FW_REF::Offset( pData->pDst, pData->dstStep * pData->dstRoiSize.height );
            KernelType * pKernel = (KernelType *)pData->pKernel;

            XMM128 zero;
            zero.i = _mm_setzero_si128();

            // calculate destination mask for the 4 chanel alpha scenario
            XMM128 alphaMask;
            if( chCount == AC4 )
            {
                switch( sizeof(DstType) )
                {
                case 1:
                    alphaMask.i = _mm_set1_epi32( 0x00FFFFFF );
                    break;
                case 2:
                    alphaMask.i = _mm_set_epi32( 0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF );
                    break;
                case 4:
                    alphaMask.i = _mm_set_epi32( 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF );
                    break;
                }
            }

            XMM128 * kernel = (XMM128*)pData->pBufferObj;

            // iterate over each row of the destination image
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width * actualChCount * sizeof(SrcType);
                U32 startWidth = BytesToAlign(ps, 16*actualChCount);	// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % (16*actualChCount); // Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                DstType *pEndStart  = FW_REF::Offset( pd, startWidth );			    // End of unaligned data / Start of aligned data 			
                DstType *pEndMiddle = FW_REF::Offset( pd, startWidth + midwidth );	// End of aligned x 16 data.
                DstType *pEnd       = FW_REF::Offset( pd, width );				    // Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEndStart ) // Process 1 at a time until we hit an alignment boundary
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // process the "main" data
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndMiddle; ps+=(16/sizeof(SrcType)), pd+=(16/sizeof(SrcType)) )	// Process 1 horizontal line (16 wide)
                {
                    SSE2_Aligned_Impl( pData, zero, kernel, ps, pd, alphaMask );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEnd )					// Process the remainder.
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // move to the next line in the destination buffer
                /////////////////////////////////////////////////////////////////
                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned code path version of the function above
        static void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int yoffset = pData->kernelSize.height - pData->anchor.y - 1;
            int xoffset = pData->kernelSize.width - pData->anchor.x - 1;
            int actualChCount = ( chCount == AC4 ) ? C4 : chCount;

            const SrcType *pSrc = FW_REF::Offset( pData->pSrc, - yoffset * pData->srcStep - xoffset * actualChCount * sizeof(SrcType) );
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = FW_REF::Offset( pData->pDst, pData->dstStep * pData->dstRoiSize.height );
            KernelType * pKernel = (KernelType *)pData->pKernel;
            
            XMM128 zero;
            zero.i = _mm_setzero_si128();

            // calculate the destination mask for the 4 channel alpha scenario
            XMM128 alphaMask;
            if( chCount == AC4 )
            {
                switch( sizeof(DstType) )
                {
                case 1:
                    alphaMask.i = _mm_set1_epi32( 0x00FFFFFF );
                    break;
                case 2:
                    alphaMask.i = _mm_set_epi32( 0x0000FFFF, 0xFFFFFFFF, 0x0000FFFF, 0xFFFFFFFF );
                    break;
                case 4:
                    alphaMask.i = _mm_set_epi32( 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF );
                    break;
                }
            }

            XMM128 * kernel = (XMM128*)pData->pBufferObj;

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width * actualChCount * sizeof(SrcType);
                U32 endWidth   = width % (16*actualChCount);		// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - endWidth;		    // Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                DstType *pEndMiddle = FW_REF::Offset( pd, midwidth );	// End of aligned x 16 data.
                DstType *pEnd       = FW_REF::Offset( pd, width );		// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the "main" data
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndMiddle; ps+=(16/sizeof(SrcType)), pd+=(16/sizeof(DstType)) )				// Process 1 horizontal line (16 wide)
                {
                    SSE2_Unaligned_Impl( pData, zero, kernel, ps, pd, alphaMask );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                while( pd<pEnd )
                {
                    switch( chCount )
                    {
                    case C1:
                    case C3:
                    case C4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd++;
                        ps++;
                        break;
                    case AC4:
                        Calculate( FW_REF::Offset( ps, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+1, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+1, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        Calculate( FW_REF::Offset( ps+2, yoffset * pData->srcStep + xoffset * actualChCount * sizeof(SrcType) ),
                                   pData->srcStep, pd+2, pData->dstStep, pKernel, pData->kernelSize, pData->anchor );
                        pd += 4;
                        ps += 4;
                        break;
                    }
                }

                /////////////////////////////////////////////////////////////////
                // move to the next line in the destination buffer
                /////////////////////////////////////////////////////////////////
                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // The classes below provide implementation of the innermost loop of the filter function.
    // depending on the data type, the size of the kernel mask, and the kernel values we can 
    // perform the intermediate calculations on smaller or type (ex. 16s) or we may need a larger type (64f)
    // to preserve accuracy.
    // for 8U data we have three implementations that use 16S, 32F or 64F type for the intermediate calculations.
    // for 16S data we have two implementations that uses 32F type for the intermediate calculations.
    // for 32F data we have one implementation that uses 32F or 64F type for the intermediate calculations.
    // we also have separate implementation where we need to scale the result (absolute value of divisor is not 1)
    // and where we do perform the division.

    // this is the code for 8u data using 64f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_64f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].d = _mm_set1_pd( (double)pKernel[ kernelSize - n - 1] );
            }
        }

        static SYS_INLINE void GetDivisorInv( XMM128 & divisor_inv, int divisor )
        {
            divisor_inv.d = _mm_set1_pd( (double)1 / (double)divisor );
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.d = _mm_setzero_pd();
            sum1.d = _mm_setzero_pd();
            sum2.d = _mm_setzero_pd();
            sum3.d = _mm_setzero_pd();
            sum4.d = _mm_setzero_pd();
            sum5.d = _mm_setzero_pd();
            sum6.d = _mm_setzero_pd();
            sum7.d = _mm_setzero_pd();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1, src2, src3, src4, src5, src6, src7;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                src4.i = _mm_unpackhi_epi8( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                src6.i = _mm_unpackhi_epi16( src4.i, zero.i );
                src4.i = _mm_unpacklo_epi16( src4.i, zero.i );
                src2.i = _mm_unpackhi_epi16( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi16( src0.i, zero.i );

                // convert to float
                src1.i = _mm_srli_si128( src0.i, 8 );
                src0.d = _mm_cvtepi32_pd( src0.i );
                src1.d = _mm_cvtepi32_pd( src1.i );
                src3.i = _mm_srli_si128( src2.i, 8 );
                src2.d = _mm_cvtepi32_pd( src2.i );
                src3.d = _mm_cvtepi32_pd( src3.i );
                src5.i = _mm_srli_si128( src4.i, 8 );
                src4.d = _mm_cvtepi32_pd( src4.i );
                src5.d = _mm_cvtepi32_pd( src5.i );
                src7.i = _mm_srli_si128( src6.i, 8 );
                src6.d = _mm_cvtepi32_pd( src6.i );
                src7.d = _mm_cvtepi32_pd( src7.i );

                // multiply by kernel[0] and add to sum
                tmp.d = _mm_mul_pd( src0.d, kernelRow[0].d );
                sum0.d = _mm_add_pd( sum0.d, tmp.d );
                tmp.d = _mm_mul_pd( src1.d, kernelRow[0].d );
                sum1.d = _mm_add_pd( sum1.d, tmp.d );
                tmp.d = _mm_mul_pd( src2.d, kernelRow[0].d );
                sum2.d = _mm_add_pd( sum2.d, tmp.d );
                tmp.d = _mm_mul_pd( src3.d, kernelRow[0].d );
                sum3.d = _mm_add_pd( sum3.d, tmp.d );
                tmp.d = _mm_mul_pd( src4.d, kernelRow[0].d );
                sum4.d = _mm_add_pd( sum4.d, tmp.d );
                tmp.d = _mm_mul_pd( src5.d, kernelRow[0].d );
                sum5.d = _mm_add_pd( sum5.d, tmp.d );
                tmp.d = _mm_mul_pd( src6.d, kernelRow[0].d );
                sum6.d = _mm_add_pd( sum6.d, tmp.d );
                tmp.d = _mm_mul_pd( src7.d, kernelRow[0].d );
                sum7.d = _mm_add_pd( sum7.d, tmp.d );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src7

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_64f( src0.d, src1.d, src2.d, src3.d, src4.d, src5.d, src6.d, src7.d, 
                                                       *FW_REF::Offset( ps, row * pData->srcStep + 15 + N ) );
                        break;
                    case C3:
                        {
                            int offset = N * 3;
                            CBL_SSE2::ShiftAndCarry_3R_64f( src0.d, src1.d, src2.d, src3.d, src4.d, src5.d, src6.d, src7.d, 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 13 + offset ), 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 13 + offset + 1 ), 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 13 + offset + 2 ) );
                        }
                        break;
                    case C4:
                    case AC4:
                        {
                            int offset = N * 4;
                            CBL_SSE2::ShiftAndCarry_4R_64f( src0.d, src1.d, src2.d, src3.d, src4.d, src5.d, src6.d, src7.d, 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 12 + offset ), 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 12 + offset + 1 ), 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 12 + offset + 2 ), 
                                                            *FW_REF::Offset( ps, row * pData->srcStep + 12 + offset + 3 ) );
                        }
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.d = _mm_mul_pd( src0.d, kernelRow[N].d );
                    sum0.d = _mm_add_pd( sum0.d, tmp.d );
                    tmp.d = _mm_mul_pd( src1.d, kernelRow[N].d );
                    sum1.d = _mm_add_pd( sum1.d, tmp.d );
                    tmp.d = _mm_mul_pd( src2.d, kernelRow[N].d );
                    sum2.d = _mm_add_pd( sum2.d, tmp.d );
                    tmp.d = _mm_mul_pd( src3.d, kernelRow[N].d );
                    sum3.d = _mm_add_pd( sum3.d, tmp.d );
                    tmp.d = _mm_mul_pd( src4.d, kernelRow[N].d );
                    sum4.d = _mm_add_pd( sum4.d, tmp.d );
                    tmp.d = _mm_mul_pd( src5.d, kernelRow[N].d );
                    sum5.d = _mm_add_pd( sum5.d, tmp.d );
                    tmp.d = _mm_mul_pd( src6.d, kernelRow[N].d );
                    sum6.d = _mm_add_pd( sum6.d, tmp.d );
                    tmp.d = _mm_mul_pd( src7.d, kernelRow[N].d );
                    sum7.d = _mm_add_pd( sum7.d, tmp.d );
                }

                kernelRow += pData->kernelSize.width;
            }

            // divide by divisor
            sum0.d = _mm_mul_pd( sum0.d, divisor_inv.d );
            sum1.d = _mm_mul_pd( sum1.d, divisor_inv.d );
            sum2.d = _mm_mul_pd( sum2.d, divisor_inv.d );
            sum3.d = _mm_mul_pd( sum3.d, divisor_inv.d );
            sum4.d = _mm_mul_pd( sum4.d, divisor_inv.d );
            sum5.d = _mm_mul_pd( sum5.d, divisor_inv.d );
            sum6.d = _mm_mul_pd( sum6.d, divisor_inv.d );
            sum7.d = _mm_mul_pd( sum7.d, divisor_inv.d );

            // convert to 32 bit integer
            tmp.i = _mm_cvtpd_epi32( sum1.d );
            tmp.i = _mm_slli_si128( tmp.i, 8 );
            sum0.i = _mm_cvtpd_epi32( sum0.d );
            sum0.i = _mm_or_si128( sum0.i, tmp.i );
            tmp.i = _mm_cvtpd_epi32( sum3.d );
            tmp.i = _mm_slli_si128( tmp.i, 8 );
            sum2.i = _mm_cvtpd_epi32( sum2.d );
            sum2.i = _mm_or_si128( sum2.i, tmp.i );
            tmp.i = _mm_cvtpd_epi32( sum5.d );
            tmp.i = _mm_slli_si128( tmp.i, 8 );
            sum4.i = _mm_cvtpd_epi32( sum4.d );
            sum4.i = _mm_or_si128( sum4.i, tmp.i );
            tmp.i = _mm_cvtpd_epi32( sum7.d );
            tmp.i = _mm_slli_si128( tmp.i, 8 );
            sum6.i = _mm_cvtpd_epi32( sum6.d );
            sum6.i = _mm_or_si128( sum6.i, tmp.i );

            // saturate the result
            sum0.i = _mm_packs_epi32( sum0.i, sum2.i );
            sum4.i = _mm_packs_epi32( sum4.i, sum6.i );
            sum0.i = _mm_packus_epi16( sum0.i, sum4.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }
    };

    // this is the code for 8u data using 32f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_32f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        static SYS_INLINE void GetDivisorInv( XMM128 & divisor_inv, int divisor )
        {
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)divisor );
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, sum2, sum3, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.f = _mm_setzero_ps();
            sum1.f = _mm_setzero_ps();
            sum2.f = _mm_setzero_ps();
            sum3.f = _mm_setzero_ps();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1, src2, src3;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                src2.i = _mm_unpacklo_epi16( src2.i, zero.i );
                src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi16( src0.i, zero.i );

                // convert to float
                src0.f = _mm_cvtepi32_ps( src0.i );
                src1.f = _mm_cvtepi32_ps( src1.i );
                src2.f = _mm_cvtepi32_ps( src2.i );
                src3.f = _mm_cvtepi32_ps( src3.i );

                // multiply by kernel[0] and add to sum
                tmp.f = _mm_mul_ps( src0.f, kernelRow[0].f );
                sum0.f = _mm_add_ps( sum0.f, tmp.f );
                tmp.f = _mm_mul_ps( src1.f, kernelRow[0].f );
                sum1.f = _mm_add_ps( sum1.f, tmp.f );
                tmp.f = _mm_mul_ps( src2.f, kernelRow[0].f );
                sum2.f = _mm_add_ps( sum2.f, tmp.f );
                tmp.f = _mm_mul_ps( src3.f, kernelRow[0].f );
                sum3.f = _mm_add_ps( sum3.f, tmp.f );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src3

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_32f( src0.f, src1.f, src2.f, src3.f,
                                                       (float)*FW_REF::Offset( ps, row * pData->srcStep + 15 + N ) );
                        break;
                    case C3:
                        {
                            CBL_SSE2::ShiftAndCarry_3R_32f( src0.f, src1.f, src2.f, src3.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + 13 + 3 * N ) );
                        }
                        break;
                    case C4:
                    case AC4:
                        {
                            CBL_SSE2::ShiftAndCarry_4R_32f( src0.f, src1.f, src2.f, src3.f, 
                                                            FW_REF::Offset( ps, row * pData->srcStep + 12 + 4 * N ) );
                        }
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.f = _mm_mul_ps( src0.f, kernelRow[N].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = _mm_mul_ps( src1.f, kernelRow[N].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = _mm_mul_ps( src2.f, kernelRow[N].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = _mm_mul_ps( src3.f, kernelRow[N].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );
                }

                kernelRow += pData->kernelSize.width;
            }

            // divide by divisor
            sum0.f = _mm_mul_ps( sum0.f, divisor_inv.f );
            sum1.f = _mm_mul_ps( sum1.f, divisor_inv.f );
            sum2.f = _mm_mul_ps( sum2.f, divisor_inv.f );
            sum3.f = _mm_mul_ps( sum3.f, divisor_inv.f );

            // convert to 32 bit integer
            sum0.i = _mm_cvtps_epi32( sum0.f );
            sum1.i = _mm_cvtps_epi32( sum1.f );
            sum2.i = _mm_cvtps_epi32( sum2.f );
            sum3.i = _mm_cvtps_epi32( sum3.f );

            // saturate the result
            sum0.i = _mm_packs_epi32( sum0.i, sum1.i );
            sum2.i = _mm_packs_epi32( sum2.i, sum3.i );
            sum0.i = _mm_packus_epi16( sum0.i, sum2.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }
    };

    // this is the code for 8u data using 16s for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_16s_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].i = _mm_set1_epi16( (short)pKernel[ kernelSize - n - 1] );
            }
        }

        static SYS_INLINE void GetDivisorInv( XMM128 & divisor_inv, int divisor )
        {
            CBL_SSE2::PrepFastDivide_16s_const( divisor_inv.i, (short)divisor );
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.i = _mm_setzero_si128();
            sum1.i = _mm_setzero_si128();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                src1.i = _mm_unpackhi_epi8( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi8( src0.i, zero.i );

                // multiply by kernel[0] and add to sum
                tmp.i = _mm_mullo_epi16( src0.i, kernelRow[0].i );
                sum0.i = _mm_add_epi16( sum0.i, tmp.i );
                tmp.i = _mm_mullo_epi16( src1.i, kernelRow[0].i );
                sum1.i = _mm_add_epi16( sum1.i, tmp.i );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src7

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_16s( src0.i, src1.i, 
                                                       *FW_REF::Offset( ps, row * pData->srcStep + 15 + N ) );
                        break;
                    case C3:
                        CBL_SSE2::ShiftAndCarry_3R_16s( src0.i, src1.i,
                                                        FW_REF::Offset( ps, row * pData->srcStep + 13 + N * 3 ) );
                        break;
                    case C4:
                    case AC4:
                        CBL_SSE2::ShiftAndCarry_4R_16s( src0.i, src1.i,
                                                        FW_REF::Offset( ps, row * pData->srcStep + 12 + N * 4 ) );
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.i = _mm_mullo_epi16( src0.i, kernelRow[N].i );
                    sum0.i = _mm_add_epi16( sum0.i, tmp.i );
                    tmp.i = _mm_mullo_epi16( src1.i, kernelRow[N].i );
                    sum1.i = _mm_add_epi16( sum1.i, tmp.i );
                }

                kernelRow += pData->kernelSize.width;
            }

            // divide by divisor
            sum0.i = _mm_mulhi_epi16( sum0.i, divisor_inv.i );
            sum1.i = _mm_mulhi_epi16( sum1.i, divisor_inv.i );

            // saturate the result
            sum0.i = _mm_packus_epi16( sum0.i, sum1.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }
    };

    // this is the code for 8u data using 16s for intermediate calculations with no scaling (division)
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_16s_NoDivision_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].i = _mm_set1_epi16( (short)pKernel[ kernelSize - n - 1] );
            }
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.i = _mm_setzero_si128();
            sum1.i = _mm_setzero_si128();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                src1.i = _mm_unpackhi_epi8( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi8( src0.i, zero.i );

                // multiply by kernel[0] and add to sum
                tmp.i = _mm_mullo_epi16( src0.i, kernelRow[0].i );
                sum0.i = _mm_add_epi16( sum0.i, tmp.i );
                tmp.i = _mm_mullo_epi16( src1.i, kernelRow[0].i );
                sum1.i = _mm_add_epi16( sum1.i, tmp.i );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src7

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_16s( src0.i, src1.i, 
                                                       *FW_REF::Offset( ps, row * pData->srcStep + 15 + N ) );
                        break;
                    case C3:
                        CBL_SSE2::ShiftAndCarry_3R_16s( src0.i, src1.i,
                                                        FW_REF::Offset( ps, row * pData->srcStep + 13 + N * 3 ) );
                        break;
                    case C4:
                    case AC4:
                        CBL_SSE2::ShiftAndCarry_4R_16s( src0.i, src1.i,
                                                        FW_REF::Offset( ps, row * pData->srcStep + 12 + N * 4 ) );
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.i = _mm_mullo_epi16( src0.i, kernelRow[N].i );
                    sum0.i = _mm_add_epi16( sum0.i, tmp.i );
                    tmp.i = _mm_mullo_epi16( src1.i, kernelRow[N].i );
                    sum1.i = _mm_add_epi16( sum1.i, tmp.i );
                }

                kernelRow += pData->kernelSize.width;
            }

            // saturate the result
            sum0.i = _mm_packus_epi16( sum0.i, sum1.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }
    };

    // this is the code for 16s data using 32f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_16s_32f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        static SYS_INLINE void GetDivisorInv( XMM128 & divisor_inv, int divisor )
        {
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)divisor );
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & /*zero*/, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, sum2, sum3, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.f = _mm_setzero_ps();
            sum1.f = _mm_setzero_ps();
            sum2.f = _mm_setzero_ps();
            sum3.f = _mm_setzero_ps();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                CBL_SSE2::Unpack16STo32S( src0.i, src1.i );

                // convert to float
                src0.f = _mm_cvtepi32_ps( src0.i );
                src1.f = _mm_cvtepi32_ps( src1.i );

                // multiply by kernel[0] and add to sum
                tmp.f = _mm_mul_ps( src0.f, kernelRow[0].f );
                sum0.f = _mm_add_ps( sum0.f, tmp.f );
                tmp.f = _mm_mul_ps( src1.f, kernelRow[0].f );
                sum1.f = _mm_add_ps( sum1.f, tmp.f );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src3

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_32f( src0.f, src1.f,
                                                       (float)*FW_REF::Offset( ps, row * pData->srcStep + ( 7 + N ) * sizeof(SrcType) ) );
                        break;
                    case C3:
                        {
                            CBL_SSE2::ShiftAndCarry_3R_32f( src0.f, src1.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + ( 5 + 3 * N ) * sizeof(SrcType) ) );
                        }
                        break;
                    case C4:
                    case AC4:
                        {
                            CBL_SSE2::ShiftAndCarry_4R_32f( src0.f, src1.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + ( 4 + 4 * N ) * sizeof(SrcType) ) );
                        }
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.f = _mm_mul_ps( src0.f, kernelRow[N].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = _mm_mul_ps( src1.f, kernelRow[N].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                }

                kernelRow += pData->kernelSize.width;
            }

            // divide by divisor
            sum0.f = _mm_mul_ps( sum0.f, divisor_inv.f );
            sum1.f = _mm_mul_ps( sum1.f, divisor_inv.f );

            // convert to 32 bit integer
            sum0.i = _mm_cvtps_epi32( sum0.f );
            sum1.i = _mm_cvtps_epi32( sum1.f );

            // saturate the result
            sum0.i = _mm_packs_epi32( sum0.i, sum1.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 & divisor_inv, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, divisor_inv, kernel, ps, pd, alphaMask );
        }
    };

    // this is the Filter32f code for 8u data using 32f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor32f_NxN_8u_32f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32f * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( pKernel[ kernelSize - n - 1] );
            }
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, sum2, sum3, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.f = _mm_setzero_ps();
            sum1.f = _mm_setzero_ps();
            sum2.f = _mm_setzero_ps();
            sum3.f = _mm_setzero_ps();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1, src2, src3;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                src2.i = _mm_unpacklo_epi16( src2.i, zero.i );
                src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                src0.i = _mm_unpacklo_epi16( src0.i, zero.i );

                // convert to float
                src0.f = _mm_cvtepi32_ps( src0.i );
                src1.f = _mm_cvtepi32_ps( src1.i );
                src2.f = _mm_cvtepi32_ps( src2.i );
                src3.f = _mm_cvtepi32_ps( src3.i );

                // multiply by kernel[0] and add to sum
                tmp.f = _mm_mul_ps( src0.f, kernelRow[0].f );
                sum0.f = _mm_add_ps( sum0.f, tmp.f );
                tmp.f = _mm_mul_ps( src1.f, kernelRow[0].f );
                sum1.f = _mm_add_ps( sum1.f, tmp.f );
                tmp.f = _mm_mul_ps( src2.f, kernelRow[0].f );
                sum2.f = _mm_add_ps( sum2.f, tmp.f );
                tmp.f = _mm_mul_ps( src3.f, kernelRow[0].f );
                sum3.f = _mm_add_ps( sum3.f, tmp.f );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src3

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_32f( src0.f, src1.f, src2.f, src3.f,
                                                       (float)*FW_REF::Offset( ps, row * pData->srcStep + 15 + N ) );
                        break;
                    case C3:
                        {
                            CBL_SSE2::ShiftAndCarry_3R_32f( src0.f, src1.f, src2.f, src3.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + 13 + 3 * N ) );
                        }
                        break;
                    case C4:
                    case AC4:
                        {
                            CBL_SSE2::ShiftAndCarry_4R_32f( src0.f, src1.f, src2.f, src3.f, 
                                                            FW_REF::Offset( ps, row * pData->srcStep + 12 + 4 * N ) );
                        }
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.f = _mm_mul_ps( src0.f, kernelRow[N].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = _mm_mul_ps( src1.f, kernelRow[N].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = _mm_mul_ps( src2.f, kernelRow[N].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = _mm_mul_ps( src3.f, kernelRow[N].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );
                }

                kernelRow += pData->kernelSize.width;
            }

            // convert to 32 bit integer
            sum0.i = _mm_cvtps_epi32( sum0.f );
            sum1.i = _mm_cvtps_epi32( sum1.f );
            sum2.i = _mm_cvtps_epi32( sum2.f );
            sum3.i = _mm_cvtps_epi32( sum3.f );

            // saturate the result
            sum0.i = _mm_packs_epi32( sum0.i, sum1.i );
            sum2.i = _mm_packs_epi32( sum2.i, sum3.i );
            sum0.i = _mm_packus_epi16( sum0.i, sum2.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }
    };

    // this is the Filter32f code for 16s data using 32f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor32f_NxN_16s_32f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32f * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( pKernel[ kernelSize - n - 1] );
            }
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & /*zero*/, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum0, sum1, sum2, sum3, tmp;
            const XMM128 * kernelRow = kernel;
            sum0.f = _mm_setzero_ps();
            sum1.f = _mm_setzero_ps();
            sum2.f = _mm_setzero_ps();
            sum3.f = _mm_setzero_ps();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src0, src1;

                // load 16 bytes at a time
                LoadClass::Load( src0.i, (__m128i*)FW_REF::Offset( ps, row * pData->srcStep ) );

                // unpack values
                CBL_SSE2::Unpack16STo32S( src0.i, src1.i );

                // convert to float
                src0.f = _mm_cvtepi32_ps( src0.i );
                src1.f = _mm_cvtepi32_ps( src1.i );

                // multiply by kernel[0] and add to sum
                tmp.f = _mm_mul_ps( src0.f, kernelRow[0].f );
                sum0.f = _mm_add_ps( sum0.f, tmp.f );
                tmp.f = _mm_mul_ps( src1.f, kernelRow[0].f );
                sum1.f = _mm_add_ps( sum1.f, tmp.f );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src3

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_SSE2::ShiftAndCarry_R_32f( src0.f, src1.f,
                                                       (float)*FW_REF::Offset( ps, row * pData->srcStep + ( 7 + N ) * sizeof(SrcType) ) );
                        break;
                    case C3:
                        {
                            CBL_SSE2::ShiftAndCarry_3R_32f( src0.f, src1.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + ( 5 + 3 * N ) * sizeof(SrcType) ) );
                        }
                        break;
                    case C4:
                    case AC4:
                        {
                            CBL_SSE2::ShiftAndCarry_4R_32f( src0.f, src1.f,
                                                            FW_REF::Offset( ps, row * pData->srcStep + ( 4 + 4 * N ) * sizeof(SrcType) ) );
                        }
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.f = _mm_mul_ps( src0.f, kernelRow[N].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = _mm_mul_ps( src1.f, kernelRow[N].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                }

                kernelRow += pData->kernelSize.width;
            }

            // convert to 32 bit integer
            sum0.i = _mm_cvtps_epi32( sum0.f );
            sum1.i = _mm_cvtps_epi32( sum1.f );

            // saturate the result
            sum0.i = _mm_packs_epi32( sum0.i, sum1.i );

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum0.i = _mm_and_si128( sum0.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum0.i = _mm_or_si128( sum0.i, tmp.i );
            }

            StoreClass::Store( (__m128i*)pd, sum0.i );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }
    };

    // this is the Filter32f code for 32f data using 32f for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_32f_32f_Impl : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static SYS_INLINE void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32f * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( pKernel[ kernelSize - n - 1] );
            }
        }

        template< typename LoadClass, typename StoreClass >
        static SYS_INLINE void SSE2_Impl( DataType< SrcType, DstType > *pData, const XMM128 & /*zero*/, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            XMM128 sum, tmp;
            const XMM128 * kernelRow = kernel;
            sum.f = _mm_setzero_ps();

            for( int row = 0; row < pData->kernelSize.height; row++ )
            {
                XMM128 src;

                // load 16 bytes at a time
                LoadClass::Load( src.f, FW_REF::Offset( ps, row * pData->srcStep ) );

                // multiply by kernel[0] and add to sum
                tmp.f = _mm_mul_ps( src.f, kernelRow[0].f );
                sum.f = _mm_add_ps( sum.f, tmp.f );

                for( int N = 1; N < pData->kernelSize.width; N++ )
                {
                    // shift the data in src0...src3

                    // note that the chCount parameter is a compile time constant so the switch statement 
                    // below should be optimized away
                    switch( chCount )
                    {
                    case C1:
                        CBL_LIBRARY::CBL_UnalignedLoad::Load( src.f, FW_REF::Offset( ps, row * pData->srcStep + N * sizeof(SrcType) ) );
                        break;
                    case C3:
                        CBL_LIBRARY::CBL_UnalignedLoad::Load( src.f, FW_REF::Offset( ps, row * pData->srcStep + 3 * N * sizeof(SrcType) ) );
                        break;
                    case C4:
                    case AC4:
                        LoadClass::Load( src.f, FW_REF::Offset( ps, row * pData->srcStep + 4 * N * sizeof(SrcType) ) );
                        break;
                    }

                    // multiply by kernel[N] and add to sum
                    tmp.f = _mm_mul_ps( src.f, kernelRow[N].f );
                    sum.f = _mm_add_ps( sum.f, tmp.f );
                }

                kernelRow += pData->kernelSize.width;
            }

            if( chCount == AC4 )
            {
                // do not overwrite the alpha channel
                CBL_LIBRARY::CBL_UnalignedLoad::Load( tmp.i, (__m128i*)pd );
                sum.i = _mm_and_si128( sum.i, alphaMask.i );
                tmp.i = _mm_andnot_si128( alphaMask.i, tmp.i );
                sum.i = _mm_or_si128( sum.i, tmp.i );
            }

            StoreClass::Store( (float*)pd, sum.f );
        }

        static SYS_INLINE void SSE2_Aligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_AlignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }

        static SYS_INLINE void SSE2_Unaligned_Impl( DataType< SrcType, DstType > *pData, const XMM128 & zero, const XMM128 * kernel, const SrcType * ps, DstType * pd, const XMM128 & alphaMask )
        {
            SSE2_Impl< CBL_LIBRARY::CBL_UnalignedLoad, CBL_LIBRARY::CBL_UnalignedStore >( pData, zero, kernel, ps, pd, alphaMask );
        }
    };

    // here we combine the superclass and subprocessor class templates to form normal processor class as expected
    // by the framework.

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_64f :
        public FilterProcessor_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_8u_64f_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_32f :
        public FilterProcessor_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_8u_32f_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_16s :
        public FilterProcessor_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_8u_16s_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_8u_16s_NoDivision :
        public FilterProcessor_NoDivision_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_8u_16s_NoDivision_Impl, Fw32s >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_16s_32f :
        public FilterProcessor_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_16s_32f_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor_NxN_32f_32f :
        public FilterProcessor_NoDivision_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor_NxN_32f_32f_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor32f_NxN_8u_32f :
        public FilterProcessor_NoDivision_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor32f_NxN_8u_32f_Impl >
    {
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterProcessor32f_NxN_16s_32f :
        public FilterProcessor_NoDivision_superclass< chCount, SrcType, DstType, CalcType, Operator, DataType, FilterProcessor32f_NxN_16s_32f_Impl >
    {
    };

    // this is a custom implementation of the divide and conquer class template that reverses the order of kernel values as expected by the rest of the code.
    // it will also negate kernel values for negative divisor scenario
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
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        static FwStatus Run( const SrcType * pSrc, int srcStep,
                                           DstType * pDst, int dstStep,
                                     FwiSize dstRoiSize, const Fw32s * pKernel,
                                     KernelType kernelSize, AnchorType anchor,
                                     int divisor )
        {
            if( pSrc == NULL || pDst == NULL || pKernel == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !FilterCommon::IsValidMask( kernelSize ) )
                return fwStsMaskSizeErr;

            if( !FilterCommon::IsValidAnchor( kernelSize, anchor ) )
                return fwStsAnchorErr;

            if( divisor == 0 )
                return fwStsDivisorErr;

//                    Operator< chCount, SrcType, DstType, CalcType >::InitializeKernelBuffer( pKernelBuffer, pKernel, kernelSize );

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    void * pKernelBuffer = fwMalloc( kernelSize.width * kernelSize.height * sizeof(XMM128) );
                    if( pKernelBuffer == NULL )
                        return fwStsMemAllocErr;

                    XMM128 *pKernelBuf = (XMM128*)pKernelBuffer;
                    Processor< chCount, SrcType, DstType, CalcType, Operator, DataType >
                        ::SSE2InitializeKernelBuffer( kernelSize.width * kernelSize.height, pKernelBuf, pKernel );

                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    for( U32 i=0; i<tCount; ++i )
                    {
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].anchor	            = anchor;
                        data[i].divisor	            = divisor;
                        data[i].pBufferObj          = pKernelBuffer;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( chCount == AC4 && ( ((size_t)ps) & ((4*sizeof(SrcType))-1) ) ) // src data is not aligned, AC4 case
                            || ( ((size_t)ps) & (sizeof(SrcType)-1) ) // src data is not aligned, all other cases
                            || ( srcStep & 0x0F ) )                   // src row step size 16 bytes aligned
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.anchor = anchor;
                    data.divisor = divisor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    // this is a custom implementation of the divide and conquer class template that reverses the order of kernel values as expected by the rest of the code.
    // this is for the no-divisor scenario
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
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        static FwStatus Run( const SrcType * pSrc, int srcStep,
                                           DstType * pDst, int dstStep,
                                     FwiSize dstRoiSize, const Fw32f * pKernel,
                                     KernelType kernelSize, AnchorType anchor )
        {
            if( pSrc == NULL || pDst == NULL || pKernel == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( !FilterCommon::IsValidMask( kernelSize ) )
                return fwStsMaskSizeErr;

            if( !FilterCommon::IsValidAnchor( kernelSize, anchor ) )
                return fwStsAnchorErr;

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    void * pKernelBuffer = fwMalloc( kernelSize.width * kernelSize.height * sizeof(XMM128) );
                    if( pKernelBuffer == NULL )
                        return fwStsMemAllocErr;
        //                    Operator< chCount, SrcType, DstType, CalcType >::InitializeKernelBuffer( pKernelBuffer, pKernel, kernelSize );
                    XMM128 *pKernelBuf = (XMM128*)pKernelBuffer;
                    Processor< chCount, SrcType, DstType, CalcType, Operator, DataType >
                        ::SSE2InitializeKernelBuffer( kernelSize.width * kernelSize.height, pKernelBuf, pKernel );

                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    for( U32 i=0; i<tCount; ++i )
                    {
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].pKernel             = (void*)pKernel;
                        data[i].kernelSize          = kernelSize;
                        data[i].anchor	            = anchor;
                        data[i].pBufferObj          = pKernelBuffer;

                        if( i == 0 )
                            data[i].dstRoiSize.height = dstRoiSize.height - (tCount-1) * height;	// All extra pixels go here	
                        else
                            data[i].dstRoiSize.height = height;

                        if( ( chCount == AC4 && ( ((size_t)ps) & ((4*sizeof(SrcType))-1) ) ) // src data is not aligned, AC4 case
                            || ( ((size_t)ps) & (sizeof(SrcType)-1) ) // src data is not aligned, all other cases
                            || ( srcStep & 0x0F ) )                   // src row step size 16 bytes aligned
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2_Unaligned, &data[i] );
                        }
                        else
                        {
                            ThreadPool::Run( Processor< chCount, SrcType, DstType, 
                                                        CalcType, Operator, DataType >::SSE2, &data[i] );
                        }

                        ps = FW_REF::Offset( ps, data[i].dstRoiSize.height * srcStep );
                        pd = FW_REF::Offset( pd, data[i].dstRoiSize.height * dstStep );
                    }

                    ThreadPool::Wait();			
                    return fwStsNoErr; 
                }
        
            case DT_REFR:
                {
                    DataClass data;
                    data.pSrc = pSrc;
                    data.srcStep = srcStep;
                    data.pDst = pDst;
                    data.dstStep = dstStep;
                    data.dstRoiSize = dstRoiSize;
                    data.pKernel = (void*)pKernel;
                    data.kernelSize = kernelSize;
                    data.anchor = anchor;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };
}
} // namepsace OPT_LEVEL

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize.height * kernelSize.width, divisor );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        if( divisor == 1 )
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
        else if( divisor == -1 )
        {
            if( pKernel == NULL )
                return fwStsNullPtrErr;

            // create a negative copy of the kernel
            size_t count = kernelSize.width * kernelSize.height;
            Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
            for( size_t n = 0; n < count; n++ )
            {
                pTempKernel[n] = - pKernel[n];
            }

            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, anchor, 1 );
        }
        else
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
    case FilterCommon::arithmetics_24bit:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw32f, 
            FiltersGeneral::FilterProcessor_NxN_8u_32f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    case FilterCommon::arithmetics_32bit:
    default:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw64f, 
            FiltersGeneral::FilterProcessor_NxN_8u_64f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    } // switch
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize.height * kernelSize.width, divisor );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        if( divisor == 1 )
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C3,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
        else if( divisor == -1 )
        {
            if( pKernel == NULL )
                return fwStsNullPtrErr;

            // create a negative copy of the kernel
            size_t count = kernelSize.width * kernelSize.height;
            Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
            for( size_t n = 0; n < count; n++ )
            {
                pTempKernel[n] = - pKernel[n];
            }

            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C3,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, anchor, 1 );
        }
        else
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C3,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
    case FilterCommon::arithmetics_24bit:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw32f, 
            FiltersGeneral::FilterProcessor_NxN_8u_32f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    case FilterCommon::arithmetics_32bit:
    default:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw64f, 
            FiltersGeneral::FilterProcessor_NxN_8u_64f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    } // switch
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize.height * kernelSize.width, divisor );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        if( divisor == 1 )
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
        else if( divisor == -1 )
        {
            if( pKernel == NULL )
                return fwStsNullPtrErr;

            // create a negative copy of the kernel
            size_t count = kernelSize.width * kernelSize.height;
            Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
            for( size_t n = 0; n < count; n++ )
            {
                pTempKernel[n] = - pKernel[n];
            }

            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, anchor, 1 );
        }
        else
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                C4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
    case FilterCommon::arithmetics_24bit:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C4,
            Fw8u, 
            Fw8u, 
            Fw32f, 
            FiltersGeneral::FilterProcessor_NxN_8u_32f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    case FilterCommon::arithmetics_32bit:
    default:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            C4,
            Fw8u, 
            Fw8u, 
            Fw64f, 
            FiltersGeneral::FilterProcessor_NxN_8u_64f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    } // switch
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, FwiSize kernelSize,
                             FwiPoint anchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize.height * kernelSize.width, divisor );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        if( divisor == 1 )
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                AC4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
        else if( divisor == -1 )
        {
            if( pKernel == NULL )
                return fwStsNullPtrErr;

            // create a negative copy of the kernel
            size_t count = kernelSize.width * kernelSize.height;
            Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
            for( size_t n = 0; n < count; n++ )
            {
                pTempKernel[n] = - pKernel[n];
            }

            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                AC4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s_NoDivision,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, anchor, 1 );
        }
        else
        {
            return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
                AC4,
                Fw8u, 
                Fw8u, 
                Fw16s, 
                FiltersGeneral::FilterProcessor_NxN_8u_16s,
                FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
        }
    case FilterCommon::arithmetics_24bit:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw32f, 
            FiltersGeneral::FilterProcessor_NxN_8u_32f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    case FilterCommon::arithmetics_32bit:
    default:
        return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw64f, 
            FiltersGeneral::FilterProcessor_NxN_8u_64f,
            FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    } // switch
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C1R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor )
{
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //    C1,
    //    Fw16s, 
    //    Fw16s, 
    //    Fw32s, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //    FiltersGeneral::Filter, FilterCommon::DataWithKernel >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
        C1,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_16s_32f,
        FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C3R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor )
{
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //    C3,
    //    Fw16s, 
    //    Fw16s, 
    //    Fw32s, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //    FiltersGeneral::Filter, FilterCommon::DataWithKernel >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
        C3,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_16s_32f,
        FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_C4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
        C4,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_16s_32f,
        FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                              Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32s * pKernel, FwiSize kernelSize,
                              FwiPoint anchor, int divisor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
        AC4,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_16s_32f,
        FiltersGeneral::Filter, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C1R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_32f_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C3R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C3,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_32f_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_C4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C4,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_32f_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                              Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                              const Fw32f * pKernel, FwiSize kernelSize,
                              FwiPoint anchor )
{
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
    //    AC4,
    //    Fw32f, 
    //    Fw32f, 
    //    Fw32f, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor,
    //    FiltersGeneral::Filter32f, FilterCommon::DataWithKernel >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        AC4,
        Fw32f,
        Fw32f,
        Fw32f,
        FiltersGeneral::FilterProcessor_NxN_32f_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor )
{
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
    //    C1,
    //    Fw8u, 
    //    Fw8u, 
    //    Fw32f, 
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor,
    //    FiltersGeneral::Filter32f, FilterCommon::DataWithKernel >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C1,
        Fw8u,
        Fw8u,
        Fw32f, 
        FiltersGeneral::FilterProcessor32f_NxN_8u_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C3,
        Fw8u,
        Fw8u,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_8u_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C4,
        Fw8u,
        Fw8u,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_8u_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, FwiSize kernelSize,
                                FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        AC4,
        Fw8u,
        Fw8u,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_8u_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C1,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_16s_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C3,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_16s_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        C4,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_16s_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilter32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, FwiSize kernelSize,
                                 FwiPoint anchor )
{
    return FiltersGeneral::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
        AC4,
        Fw16s,
        Fw16s,
        Fw32f,
        FiltersGeneral::FilterProcessor32f_NxN_16s_32f,
        FiltersGeneral::Filter32f, FilterCommon::DataWithKernelAndBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, anchor );
}

#endif // BUILD_NUM_AT_LEAST

//#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
