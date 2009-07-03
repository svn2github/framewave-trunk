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
namespace FiltersSeparable
{

    // this is a customized Divide and Conquer class used by the FilterColumn functions.
    // it assumes that the kernel matrix is 1 element wide.
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
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        static inline FwStatus Run( const SrcType * pSrc, int srcStep, 
                                           DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, const Fw32s * pKernel,
                                     KernelType kernelSize, AnchorType anchor,
                                     int divisor )
        {
            // validate function arguments
            if( pSrc == NULL || pDst == NULL )
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

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    // allocate a temporary buffer that will store the kernel values in reverse order
                    void * pKernelBuffer = fwMalloc( kernelSize * sizeof(XMM128) );
                    if( pKernelBuffer == NULL )
                        return fwStsMemAllocErr;
                    XMM128 *pKernelBuf = (XMM128*)pKernelBuffer;
                    Processor< chCount, SrcType, DstType, CalcType, Operator, DataType >
                        ::SSE2InitializeKernelBuffer( kernelSize, pKernelBuf, pKernel );

                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // spin off worker threads
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

                        // call the aligned or unaligned data code path as appropriate
                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) ) // is src data 16 bytes aligned?
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
                    fwFree( pKernelBuffer );
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

                    // call reference implementation
                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    // this is a customized Divide and Conquer class used by the FilterRow functions.
    // it assumes that the kernel matrix is 1 element high.
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
    struct DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor
    {
        typedef DataType< SrcType, DstType > DataClass;
        typedef typename DataClass::KernelType KernelType;
        typedef typename DataClass::AnchorType AnchorType;

        static inline FwStatus Run( const SrcType * pSrc, int srcStep, 
                                           DstType * pDst, int dstStep, 
                                     FwiSize dstRoiSize, const Fw32s * pKernel,
                                     KernelType kernelSize, AnchorType anchor,
                                     int divisor )
        {
            // validate function arguments
            if( pSrc == NULL || pDst == NULL )
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

            switch( Dispatch::Type<DT_SSE2>() )
            {
            default:
            case DT_SSE3: 
            case DT_SSE2: 
                {
                    // allocate a temporary buffer that will store the kernel values in reverse order
                    void * pKernelBuffer = fwMalloc( kernelSize * sizeof(XMM128) );
                    if( pKernelBuffer == NULL )
                        return fwStsMemAllocErr;
//                    Operator< chCount, SrcType, DstType, CalcType >::InitializeKernelBuffer( pKernelBuffer, pKernel, kernelSize );
                    XMM128 *pKernelBuf = (XMM128*)pKernelBuffer;
                    Processor< chCount, SrcType, DstType, CalcType, Operator, DataType >
                        ::SSE2InitializeKernelBuffer( kernelSize, pKernelBuf, pKernel );

                    U32 tCount = ThreadPool::ThreadCount();
                    std::vector< DataClass > data;
                    data.resize( tCount );

                    U32 height = dstRoiSize.height / tCount;

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    // spin off worker threads
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

                        // call the aligned or unaligned data code path as appropriate
                        if( ( ((size_t)ps) & (sizeof(SrcType)-1) ) || ( srcStep & 0xF ) ) // is src data and step size 16 bytes aligned?
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
                    fwFree( pKernelBuffer );
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

                    // call reference implementation
                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );
                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };

    // reference implementation of the filterColumn_* innermost loop code
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
            
            int H = kernelSize;
            int P = H - yAnchor - 1;

            for( int m = 0; m < H; m++ )
            {
                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( m - P ) * srcStep );
                const Fw32s  * pKrn = pKernel + ( H - m - 1 );

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            sum = (CalcType)((int)sum / divisor);

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // reference implementation of the filterColumn32f_* innermost loop code
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
            
            int H = kernelSize;
            int P = H - yAnchor - 1;

            for( int m = 0; m < H; m++ )
            {
                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( m - P ) * srcStep );
                const Fw32f  * pKrn = pKernel + ( H - m - 1 );

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // SSE2 optimized implementation of the FilterColumn code that uses double precision floating point type
    // for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterColumnProcessor : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].d = _mm_set1_pd( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        // aligned code path
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor;
            zero.i = _mm_setzero_si128();
            divisor.d = _mm_set1_pd( (double)1 / (double)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0l, sum0h, sum1l, sum1h, sum2l, sum2h, sum3l, sum3h, tmp;

                    // load 16 bytes at a time
                    src0.i = _mm_load_si128( (__m128i*)ps );

                    // unpack values into 32 bit values
                    src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                    src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi16( src0.i, zero.i );
                    src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                    src2.i = _mm_unpacklo_epi16( src2.i, zero.i );


                    // multiply by kernel[0];
                    sum0l.d = _mm_cvtepi32_pd( src0.i );
                    sum0l.d = _mm_mul_pd( sum0l.d, kernel[0].d );
                    src0.i = _mm_srli_si128( src0.i, 8 );
                    src0.d = _mm_cvtepi32_pd( src0.i );
                    sum0h.d = _mm_mul_pd( src0.d, kernel[0].d );

                    sum1l.d = _mm_cvtepi32_pd( src1.i );
                    sum1l.d = _mm_mul_pd( sum1l.d, kernel[0].d );
                    src1.i = _mm_srli_si128( src1.i, 8 );
                    src1.d = _mm_cvtepi32_pd( src1.i );
                    sum1h.d = _mm_mul_pd( src1.d, kernel[0].d );

                    sum2l.d = _mm_cvtepi32_pd( src2.i );
                    sum2l.d = _mm_mul_pd( sum2l.d, kernel[0].d );
                    src2.i = _mm_srli_si128( src2.i, 8 );
                    src2.d = _mm_cvtepi32_pd( src2.i );
                    sum2h.d = _mm_mul_pd( src2.d, kernel[0].d );

                    sum3l.d = _mm_cvtepi32_pd( src3.i );
                    sum3l.d = _mm_mul_pd( sum3l.d, kernel[0].d );
                    src3.i = _mm_srli_si128( src3.i, 8 );
                    src3.d = _mm_cvtepi32_pd( src3.i );
                    sum3h.d = _mm_mul_pd( src3.d, kernel[0].d );

                    for( int N = 1; N < kernelSize; N++ )
                    {

                        // load 16 bytes at a time
                        src0.i = _mm_load_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack values into 32 bit values
                        src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                        src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                        src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                        src0.i = _mm_unpacklo_epi16( src0.i, zero.i );
                        src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                        src2.i = _mm_unpacklo_epi16( src2.i, zero.i );

                        // multiply by kernel[N] and add to sum;
                        tmp.d = _mm_cvtepi32_pd( src0.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum0l.d = _mm_add_pd( sum0l.d, tmp.d );
                        src0.i = _mm_srli_si128( src0.i, 8 );
                        src0.d = _mm_cvtepi32_pd( src0.i );
                        src0.d = _mm_mul_pd( src0.d, kernel[N].d );
                        sum0h.d = _mm_add_pd( sum0h.d, src0.d );

                        tmp.d = _mm_cvtepi32_pd( src1.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum1l.d = _mm_add_pd( sum1l.d, tmp.d );
                        src1.i = _mm_srli_si128( src1.i, 8 );
                        src1.d = _mm_cvtepi32_pd( src1.i );
                        src1.d = _mm_mul_pd( src1.d, kernel[N].d );
                        sum1h.d = _mm_add_pd( sum1h.d, src1.d );

                        tmp.d = _mm_cvtepi32_pd( src2.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum2l.d = _mm_add_pd( sum2l.d, tmp.d );
                        src2.i = _mm_srli_si128( src2.i, 8 );
                        src2.d = _mm_cvtepi32_pd( src2.i );
                        src2.d = _mm_mul_pd( src2.d, kernel[N].d );
                        sum2h.d = _mm_add_pd( sum2h.d, src2.d );

                        tmp.d = _mm_cvtepi32_pd( src3.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum3l.d = _mm_add_pd( sum3l.d, tmp.d );
                        src3.i = _mm_srli_si128( src3.i, 8 );
                        src3.d = _mm_cvtepi32_pd( src3.i );
                        src3.d = _mm_mul_pd( src3.d, kernel[N].d );
                        sum3h.d = _mm_add_pd( sum3h.d, src3.d );
                    } // for N

                    // divide by divisor
                    sum0l.d = _mm_mul_pd( sum0l.d, divisor.d );
                    sum0h.d = _mm_mul_pd( sum0h.d, divisor.d );
                    sum1l.d = _mm_mul_pd( sum1l.d, divisor.d );
                    sum1h.d = _mm_mul_pd( sum1h.d, divisor.d );
                    sum2l.d = _mm_mul_pd( sum2l.d, divisor.d );
                    sum2h.d = _mm_mul_pd( sum2h.d, divisor.d );
                    sum3l.d = _mm_mul_pd( sum3l.d, divisor.d );
                    sum3h.d = _mm_mul_pd( sum3h.d, divisor.d );

                    // convert to 32 bit integer
                    src0.i = _mm_cvtpd_epi32( sum0h.d );
                    sum0l.i = _mm_cvtpd_epi32( sum0l.d );
                    src1.i = _mm_cvtpd_epi32( sum1h.d );
                    sum1l.i = _mm_cvtpd_epi32( sum1l.d );
                    src2.i = _mm_cvtpd_epi32( sum2h.d );
                    sum2l.i = _mm_cvtpd_epi32( sum2l.d );
                    src3.i = _mm_cvtpd_epi32( sum3h.d );
                    sum3l.i = _mm_cvtpd_epi32( sum3l.d );
                    src0.i = _mm_slli_si128( src0.i, 8 );
                    src1.i = _mm_slli_si128( src1.i, 8 );
                    src2.i = _mm_slli_si128( src2.i, 8 );
                    src3.i = _mm_slli_si128( src3.i, 8 );
                    src0.i = _mm_or_si128( src0.i, sum0l.i );
                    src1.i = _mm_or_si128( src1.i, sum1l.i );
                    src2.i = _mm_or_si128( src2.i, sum2l.i );
                    src3.i = _mm_or_si128( src3.i, sum3l.i );

                    // saturate the result
                    src0.i = _mm_packs_epi32( src0.i, src1.i );
                    src2.i = _mm_packs_epi32( src2.i, src3.i );
                    src0.i = _mm_packus_epi16( src0.i, src2.i );

                    _mm_storeu_si128( (__m128i*)pd, src0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned code version of the function above
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor;
            zero.i = _mm_setzero_si128();
            divisor.d = _mm_set1_pd( (double)1 / (double)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 midwidth   = width  & (~0xF);		    // Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;	// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;		// End of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )		// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0l, sum0h, sum1l, sum1h, sum2l, sum2h, sum3l, sum3h, tmp;

                    // load 16 bytes at a time
                    src0.i = _mm_loadu_si128( (__m128i*)ps );

                    // unpack values into 32 bit values
                    src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                    src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi16( src0.i, zero.i );
                    src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                    src2.i = _mm_unpacklo_epi16( src2.i, zero.i );


                    // multiply by kernel[0];
                    sum0l.d = _mm_cvtepi32_pd( src0.i );
                    sum0l.d = _mm_mul_pd( sum0l.d, kernel[0].d );
                    src0.i = _mm_srli_si128( src0.i, 8 );
                    src0.d = _mm_cvtepi32_pd( src0.i );
                    sum0h.d = _mm_mul_pd( src0.d, kernel[0].d );

                    sum1l.d = _mm_cvtepi32_pd( src1.i );
                    sum1l.d = _mm_mul_pd( sum1l.d, kernel[0].d );
                    src1.i = _mm_srli_si128( src1.i, 8 );
                    src1.d = _mm_cvtepi32_pd( src1.i );
                    sum1h.d = _mm_mul_pd( src1.d, kernel[0].d );

                    sum2l.d = _mm_cvtepi32_pd( src2.i );
                    sum2l.d = _mm_mul_pd( sum2l.d, kernel[0].d );
                    src2.i = _mm_srli_si128( src2.i, 8 );
                    src2.d = _mm_cvtepi32_pd( src2.i );
                    sum2h.d = _mm_mul_pd( src2.d, kernel[0].d );

                    sum3l.d = _mm_cvtepi32_pd( src3.i );
                    sum3l.d = _mm_mul_pd( sum3l.d, kernel[0].d );
                    src3.i = _mm_srli_si128( src3.i, 8 );
                    src3.d = _mm_cvtepi32_pd( src3.i );
                    sum3h.d = _mm_mul_pd( src3.d, kernel[0].d );

                    for( int N = 1; N < kernelSize; N++ )
                    {

                        // load 16 bytes at a time
                        src0.i = _mm_loadu_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack values into 32 bit values
                        src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                        src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                        src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                        src0.i = _mm_unpacklo_epi16( src0.i, zero.i );
                        src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                        src2.i = _mm_unpacklo_epi16( src2.i, zero.i );

                        // multiply by kernel[N] and add to sum;
                        tmp.d = _mm_cvtepi32_pd( src0.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum0l.d = _mm_add_pd( sum0l.d, tmp.d );
                        src0.i = _mm_srli_si128( src0.i, 8 );
                        src0.d = _mm_cvtepi32_pd( src0.i );
                        src0.d = _mm_mul_pd( src0.d, kernel[N].d );
                        sum0h.d = _mm_add_pd( sum0h.d, src0.d );

                        tmp.d = _mm_cvtepi32_pd( src1.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum1l.d = _mm_add_pd( sum1l.d, tmp.d );
                        src1.i = _mm_srli_si128( src1.i, 8 );
                        src1.d = _mm_cvtepi32_pd( src1.i );
                        src1.d = _mm_mul_pd( src1.d, kernel[N].d );
                        sum1h.d = _mm_add_pd( sum1h.d, src1.d );

                        tmp.d = _mm_cvtepi32_pd( src2.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum2l.d = _mm_add_pd( sum2l.d, tmp.d );
                        src2.i = _mm_srli_si128( src2.i, 8 );
                        src2.d = _mm_cvtepi32_pd( src2.i );
                        src2.d = _mm_mul_pd( src2.d, kernel[N].d );
                        sum2h.d = _mm_add_pd( sum2h.d, src2.d );

                        tmp.d = _mm_cvtepi32_pd( src3.i );
                        tmp.d = _mm_mul_pd( tmp.d, kernel[N].d );
                        sum3l.d = _mm_add_pd( sum3l.d, tmp.d );
                        src3.i = _mm_srli_si128( src3.i, 8 );
                        src3.d = _mm_cvtepi32_pd( src3.i );
                        src3.d = _mm_mul_pd( src3.d, kernel[N].d );
                        sum3h.d = _mm_add_pd( sum3h.d, src3.d );
                    } // for N

                    // divide by divisor
                    sum0l.d = _mm_mul_pd( sum0l.d, divisor.d );
                    sum0h.d = _mm_mul_pd( sum0h.d, divisor.d );
                    sum1l.d = _mm_mul_pd( sum1l.d, divisor.d );
                    sum1h.d = _mm_mul_pd( sum1h.d, divisor.d );
                    sum2l.d = _mm_mul_pd( sum2l.d, divisor.d );
                    sum2h.d = _mm_mul_pd( sum2h.d, divisor.d );
                    sum3l.d = _mm_mul_pd( sum3l.d, divisor.d );
                    sum3h.d = _mm_mul_pd( sum3h.d, divisor.d );

                    // convert to 32 bit integer
                    src0.i = _mm_cvtpd_epi32( sum0h.d );
                    sum0l.i = _mm_cvtpd_epi32( sum0l.d );
                    src1.i = _mm_cvtpd_epi32( sum1h.d );
                    sum1l.i = _mm_cvtpd_epi32( sum1l.d );
                    src2.i = _mm_cvtpd_epi32( sum2h.d );
                    sum2l.i = _mm_cvtpd_epi32( sum2l.d );
                    src3.i = _mm_cvtpd_epi32( sum3h.d );
                    sum3l.i = _mm_cvtpd_epi32( sum3l.d );
                    src0.i = _mm_slli_si128( src0.i, 8 );
                    src1.i = _mm_slli_si128( src1.i, 8 );
                    src2.i = _mm_slli_si128( src2.i, 8 );
                    src3.i = _mm_slli_si128( src3.i, 8 );
                    src0.i = _mm_or_si128( src0.i, sum0l.i );
                    src1.i = _mm_or_si128( src1.i, sum1l.i );
                    src2.i = _mm_or_si128( src2.i, sum2l.i );
                    src3.i = _mm_or_si128( src3.i, sum3l.i );

                    // saturate the result
                    src0.i = _mm_packs_epi32( src0.i, src1.i );
                    src2.i = _mm_packs_epi32( src2.i, src3.i );
                    src0.i = _mm_packus_epi16( src0.i, src2.i );

                    _mm_storeu_si128( (__m128i*)pd, src0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // SSE2 optimized implementation of the FilterColumn code that uses single precision floating point type
    // for intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterColumnProcessor_32f : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0, sum1, sum2, sum3;

                    // load 16 bytes at a time
                    sum0.i = _mm_load_si128( (__m128i*)ps );

                    // unpack values
                    sum2.i = _mm_unpackhi_epi8( sum0.i, zero.i );
                    sum0.i = _mm_unpacklo_epi8( sum0.i, zero.i );
                    sum3.i = _mm_unpackhi_epi16( sum2.i, zero.i );
                    sum2.i = _mm_unpacklo_epi16( sum2.i, zero.i );
                    sum1.i = _mm_unpackhi_epi16( sum0.i, zero.i );
                    sum0.i = _mm_unpacklo_epi16( sum0.i, zero.i );

                    // convert to float
                    sum0.f = _mm_cvtepi32_ps( sum0.i );
                    sum1.f = _mm_cvtepi32_ps( sum1.i );
                    sum2.f = _mm_cvtepi32_ps( sum2.i );
                    sum3.f = _mm_cvtepi32_ps( sum3.i );

                    // multiply by kernel[0];
                    sum0.f = _mm_mul_ps( sum0.f, kernel[0].f );
                    sum1.f = _mm_mul_ps( sum1.f, kernel[0].f );
                    sum2.f = _mm_mul_ps( sum2.f, kernel[0].f );
                    sum3.f = _mm_mul_ps( sum3.f, kernel[0].f );

                    for( int N = 1; N < kernelSize; N++ )
                    {

                        // load 16 bytes at a time
                        src0.i = _mm_load_si128( (__m128i*)(ps + pData->srcStep * N ) );

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

                        // multiply by kernel[N] and add to sum;
                        src0.f = _mm_mul_ps( src0.f, kernel[N].f );
                        sum0.f = _mm_add_ps( sum0.f, src0.f );
                        src1.f = _mm_mul_ps( src1.f, kernel[N].f );
                        sum1.f = _mm_add_ps( sum1.f, src1.f );
                        src2.f = _mm_mul_ps( src2.f, kernel[N].f );
                        sum2.f = _mm_add_ps( sum2.f, src2.f );
                        src3.f = _mm_mul_ps( src3.f, kernel[N].f );
                        sum3.f = _mm_add_ps( sum3.f, src3.f );
                    } // for N

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

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned data version of the function above
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 endWidth   = width % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0, sum1, sum2, sum3;

                    // load 16 bytes at a time
                    sum0.i = _mm_loadu_si128( (__m128i*)ps );

                    // unpack values
                    sum2.i = _mm_unpackhi_epi8( sum0.i, zero.i );
                    sum0.i = _mm_unpacklo_epi8( sum0.i, zero.i );
                    sum3.i = _mm_unpackhi_epi16( sum2.i, zero.i );
                    sum2.i = _mm_unpacklo_epi16( sum2.i, zero.i );
                    sum1.i = _mm_unpackhi_epi16( sum0.i, zero.i );
                    sum0.i = _mm_unpacklo_epi16( sum0.i, zero.i );

                    // convert to float
                    sum0.f = _mm_cvtepi32_ps( sum0.i );
                    sum1.f = _mm_cvtepi32_ps( sum1.i );
                    sum2.f = _mm_cvtepi32_ps( sum2.i );
                    sum3.f = _mm_cvtepi32_ps( sum3.i );

                    // multiply by kernel[0];
                    sum0.f = _mm_mul_ps( sum0.f, kernel[0].f );
                    sum1.f = _mm_mul_ps( sum1.f, kernel[0].f );
                    sum2.f = _mm_mul_ps( sum2.f, kernel[0].f );
                    sum3.f = _mm_mul_ps( sum3.f, kernel[0].f );

                    for( int N = 1; N < kernelSize; N++ )
                    {

                        // load 16 bytes at a time
                        src0.i = _mm_loadu_si128( (__m128i*)(ps + pData->srcStep * N ) );

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

                        // multiply by kernel[N] and add to sum;
                        src0.f = _mm_mul_ps( src0.f, kernel[N].f );
                        sum0.f = _mm_add_ps( sum0.f, src0.f );
                        src1.f = _mm_mul_ps( src1.f, kernel[N].f );
                        sum1.f = _mm_add_ps( sum1.f, src1.f );
                        src2.f = _mm_mul_ps( src2.f, kernel[N].f );
                        sum2.f = _mm_add_ps( sum2.f, src2.f );
                        src3.f = _mm_mul_ps( src3.f, kernel[N].f );
                        sum3.f = _mm_add_ps( sum3.f, src3.f );
                    } // for N

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

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // SSE2 optimized implementation of filter column code that uses signed 16 bit integer type for intermediate
    // calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterColumnProcessor_16s : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].i = _mm_set1_epi16( (short)pKernel[ kernelSize - n - 1] );
            }
        }

        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.i = _mm_set1_epi16( (short)(65536 / pData->divisor) );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi;

                    // load 16 bytes at a time
                    sum_hi.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    sum_lo.i = _mm_unpacklo_epi8( sum_hi.i, zero.i );
                    sum_hi.i = _mm_unpackhi_epi8( sum_hi.i, zero.i );

                    // multiply by kernel[0];
                    sum_lo.i = _mm_mullo_epi16( sum_lo.i, kernel[0].i );
                    sum_hi.i = _mm_mullo_epi16( sum_hi.i, kernel[0].i );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // load 16 bytes at a time
                        src_hi.i = _mm_load_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack data
                        src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                        src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                        // multiply by kernel[N]
                        src_lo.i = _mm_mullo_epi16( src_lo.i, kernel[N].i );
                        src_hi.i = _mm_mullo_epi16( src_hi.i, kernel[N].i );

                        // add to sum
                        sum_lo.i = _mm_adds_epi16( sum_lo.i, src_lo.i );
                        sum_hi.i = _mm_adds_epi16( sum_hi.i, src_hi.i );
                    } // for N

                    // divide by divisor
                    sum_lo.i = CBL_SSE2::FastDivide_16s_const( sum_lo.i, divisor_inv.i );
                    sum_hi.i = CBL_SSE2::FastDivide_16s_const( sum_hi.i, divisor_inv.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned data version of the function above
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.i = _mm_set1_epi16( (short)(65536 / pData->divisor) );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 endWidth   = width % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi;

                    // load 16 bytes at a time
                    sum_hi.i = _mm_loadu_si128( (__m128i*)ps );

                    // unpack data
                    sum_lo.i = _mm_unpacklo_epi8( sum_hi.i, zero.i );
                    sum_hi.i = _mm_unpackhi_epi8( sum_hi.i, zero.i );

                    // multiply by kernel[0];
                    sum_lo.i = _mm_mullo_epi16( sum_lo.i, kernel[0].i );
                    sum_hi.i = _mm_mullo_epi16( sum_hi.i, kernel[0].i );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // load 16 bytes at a time
                        src_hi.i = _mm_loadu_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack data
                        src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                        src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                        // multiply by kernel[N]
                        src_lo.i = _mm_mullo_epi16( src_lo.i, kernel[N].i );
                        src_hi.i = _mm_mullo_epi16( src_hi.i, kernel[N].i );

                        // add to sum
                        sum_lo.i = _mm_adds_epi16( sum_lo.i, src_lo.i );
                        sum_hi.i = _mm_adds_epi16( sum_hi.i, src_hi.i );
                    } // for N

                    // divide by divisor
                    sum_lo.i = CBL_SSE2::FastDivide_16s_const( sum_lo.i, divisor_inv.i );
                    sum_hi.i = CBL_SSE2::FastDivide_16s_const( sum_hi.i, divisor_inv.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // SSE2 optimized implementation of filter column code that uses signed 16 bit integer type for intermediate
    // calculations.  In addition this implementation performs no division/scaling of the result.  This improves
    // performance for divisor values of 1 and -1
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterColumnProcessor_16s_NoDivision : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].i = _mm_set1_epi16( (short)pKernel[ kernelSize - n - 1] );
            }
        }

        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi;

                    // load 16 bytes at a time
                    sum_lo.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    sum_hi.i = _mm_unpackhi_epi8( sum_lo.i, zero.i );
                    sum_lo.i = _mm_unpacklo_epi8( sum_lo.i, zero.i );

                    // multiply by kernel[0];
                    sum_lo.i = _mm_mullo_epi16( sum_lo.i, kernel[0].i );
                    sum_hi.i = _mm_mullo_epi16( sum_hi.i, kernel[0].i );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // load 16 bytes at a time
                        src_hi.i = _mm_load_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack data
                        src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                        src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                        // multiply by kernel[N]
                        src_lo.i = _mm_mullo_epi16( src_lo.i, kernel[N].i );
                        src_hi.i = _mm_mullo_epi16( src_hi.i, kernel[N].i );

                        // add to sum
                        sum_lo.i = _mm_adds_epi16( sum_lo.i, src_lo.i );
                        sum_hi.i = _mm_adds_epi16( sum_hi.i, src_hi.i );
                    } // for N

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        // unaligned data version of the function above
        static inline void SSE2_Unaligned( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = (const SrcType*)(((Fw8u*)pData->pSrc) - anchorOffset * pData->srcStep);
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;

            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 endWidth   = width % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    midwidth = 0;
                }
                    
                const Fw8u *pEndMiddle = pd + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi;

                    // load 16 bytes at a time
                    sum_hi.i = _mm_loadu_si128( (__m128i*)ps );

                    // unpack data
                    sum_lo.i = _mm_unpacklo_epi8( sum_hi.i, zero.i );
                    sum_hi.i = _mm_unpackhi_epi8( sum_hi.i, zero.i );

                    // multiply by kernel[0];
                    sum_lo.i = _mm_mullo_epi16( sum_lo.i, kernel[0].i );
                    sum_hi.i = _mm_mullo_epi16( sum_hi.i, kernel[0].i );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // load 16 bytes at a time
                        src_hi.i = _mm_loadu_si128( (__m128i*)(ps + pData->srcStep * N ) );

                        // unpack data
                        src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                        src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                        // multiply by kernel[N]
                        src_lo.i = _mm_mullo_epi16( src_lo.i, kernel[N].i );
                        src_hi.i = _mm_mullo_epi16( src_hi.i, kernel[N].i );

                        // add to sum
                        sum_lo.i = _mm_adds_epi16( sum_lo.i, src_lo.i );
                        sum_hi.i = _mm_adds_epi16( sum_hi.i, src_hi.i );
                    } // for N

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( (const SrcType*)(((Fw8u*)ps)+anchorOffset*pData->srcStep), pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }
    };

    // Reference implementation of the FilterRow_* functions
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterRow
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].f = _mm_set1_ps( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        static inline void Calculate(   const SrcType * pSrcPtr, int /*srcStep*/,
                                        DstType * pDst, int /*dstStep*/,
                                        const Fw32s * pKernel,
                                        int kernelSize, 
                                        int xAnchor, int divisor )
        {
            CalcType sum = 0;
            
            int W = kernelSize;
            int Q = W - xAnchor - 1;

            for( int n = 0; n < W; n++ )
            {
                const SrcType * pSrc = pSrcPtr + ( n - Q ) * chCount;
                const Fw32s  * pKrn = pKernel + ( W - n - 1 );

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            sum = (CalcType)((int)sum / divisor);

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // Reference implementation of the FilterRow32f_* functions
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
            
            int W = kernelSize;
            int Q = W - xAnchor - 1;

            for( int n = 0; n < W; n++ )
            {
                const SrcType * pSrc = pSrcPtr + ( n - Q ) * chCount;
                const Fw32f  * pKrn = pKernel + ( W - n - 1 );

                sum = sum + (CalcType)(*pSrc) * (CalcType)(*pKrn);
            }

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses double precision floating point data type for
    // intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2InitializeKernelBuffer( int kernelSize, XMM128 *pKernelBuf, const Fw32s * pKernel )
        {
            for( int n = 0; n < kernelSize; n++ )
            {
                pKernelBuf[n].d = _mm_set1_pd( (float)pKernel[ kernelSize - n - 1] );
            }
        }

        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.d = _mm_set1_pd( (double)1 / (double)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, src4, src5, src6, src7, sum0, sum1, sum2, sum3, sum4, sum5, sum6, sum7, tmp;

                    // load 16 bytes at a time
                    src0.i = _mm_load_si128( (__m128i*)ps );

                    // unpack values
                    src4.i = _mm_unpackhi_epi8( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                    src6.i = _mm_unpackhi_epi16( src4.i, zero.i );
                    src4.i = _mm_unpacklo_epi16( src4.i, zero.i );
                    src2.i = _mm_unpackhi_epi16( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi16( src0.i, zero.i );

                    // convert to double precision
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

                    // multiply by kernel[4];
                    sum0.d = src0.d;
                    sum1.d = src1.d;
                    sum2.d = src2.d;
                    sum3.d = src3.d;
                    sum4.d = src4.d;
                    sum5.d = src5.d;
                    sum6.d = src6.d;
                    sum7.d = src7.d;
                    sum0.d = _mm_mul_pd( sum0.d, kernel[0].d );
                    sum1.d = _mm_mul_pd( sum1.d, kernel[0].d );
                    sum2.d = _mm_mul_pd( sum2.d, kernel[0].d );
                    sum3.d = _mm_mul_pd( sum3.d, kernel[0].d );
                    sum4.d = _mm_mul_pd( sum4.d, kernel[0].d );
                    sum5.d = _mm_mul_pd( sum5.d, kernel[0].d );
                    sum6.d = _mm_mul_pd( sum6.d, kernel[0].d );
                    sum7.d = _mm_mul_pd( sum7.d, kernel[0].d );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // shift the data in src0, src1, src2 and src3
                        src0.i = _mm_srli_si128( src0.i, 8 );
                        src0.f = _mm_movelh_ps( src0.f, src1.f );
                        src1.i = _mm_srli_si128( src1.i, 8 );
                        src1.f = _mm_movelh_ps( src1.f, src2.f );
                        src2.i = _mm_srli_si128( src2.i, 8 );
                        src2.f = _mm_movelh_ps( src2.f, src3.f );
                        src3.i = _mm_srli_si128( src3.i, 8 );
                        src3.f = _mm_movelh_ps( src3.f, src4.f );
                        src4.i = _mm_srli_si128( src4.i, 8 );
                        src4.f = _mm_movelh_ps( src4.f, src5.f );
                        src5.i = _mm_srli_si128( src5.i, 8 );
                        src5.f = _mm_movelh_ps( src5.f, src6.f );
                        src6.i = _mm_srli_si128( src6.i, 8 );
                        src6.f = _mm_movelh_ps( src6.f, src7.f );
                        src7.d = _mm_cvtsi32_sd( src7.d, *(ps + 15 + N) );
                        src7.i = _mm_shuffle_epi32( src7.i, CBL_SHUF_IMM( 1, 0, 3, 2 ) ); //@todo verify this...

                        // multiply by kernel[N] and add to sum;
                        tmp.d = _mm_mul_pd( src0.d, kernel[N].d );
                        sum0.d = _mm_add_pd( sum0.d, tmp.d );
                        tmp.d = _mm_mul_pd( src1.d, kernel[N].d );
                        sum1.d = _mm_add_pd( sum1.d, tmp.d );
                        tmp.d = _mm_mul_pd( src2.d, kernel[N].d );
                        sum2.d = _mm_add_pd( sum2.d, tmp.d );
                        tmp.d = _mm_mul_pd( src3.d, kernel[N].d );
                        sum3.d = _mm_add_pd( sum3.d, tmp.d );
                        tmp.d = _mm_mul_pd( src4.d, kernel[N].d );
                        sum4.d = _mm_add_pd( sum4.d, tmp.d );
                        tmp.d = _mm_mul_pd( src5.d, kernel[N].d );
                        sum5.d = _mm_add_pd( sum5.d, tmp.d );
                        tmp.d = _mm_mul_pd( src6.d, kernel[N].d );
                        sum6.d = _mm_add_pd( sum6.d, tmp.d );
                        tmp.d = _mm_mul_pd( src7.d, kernel[N].d );
                        sum7.d = _mm_add_pd( sum7.d, tmp.d );
                    } // for N

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

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses single precision floating point data type for
    // intermediate calculations
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_32f : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int kernelSize = pData->kernelSize;
            int anchor = pData->anchor;
            int anchorOffset = kernelSize - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)pData->divisor );

            XMM128 * kernel = (XMM128*)pData->pBufferObj;
            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0, sum1, sum2, sum3, tmp, ext4;

                    // load 16 bytes at a time
                    src0.i = _mm_load_si128( (__m128i*)ps );
                    ext4.i = _mm_cvtsi32_si128( *(int*)(ps + 16) );

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

                    // multiply by kernel[4];
                    sum0.i = src0.i;
                    sum1.i = src1.i;
                    sum2.i = src2.i;
                    sum3.i = src3.i;
                    sum0.f = _mm_mul_ps( sum0.f, kernel[0].f );
                    sum1.f = _mm_mul_ps( sum1.f, kernel[0].f );
                    sum2.f = _mm_mul_ps( sum2.f, kernel[0].f );
                    sum3.f = _mm_mul_ps( sum3.f, kernel[0].f );

                    for( int N = 1; N < kernelSize; N++ )
                    {
                        // shift the data in src0, src1, src2 and src3
                        src0.f = _mm_move_ss( src0.f, src1.f );
                        src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                        src1.f = _mm_move_ss( src1.f, src2.f );
                        src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                        src2.f = _mm_move_ss( src2.f, src3.f );
                        src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                        src3.f = _mm_cvtsi32_ss( src3.f, *(ps + 15 + N) );
                        src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );

                        // multiply by kernel[3] and add to sum;
                        tmp.f = src0.f;
                        tmp.f = _mm_mul_ps( tmp.f, kernel[N].f );
                        sum0.f = _mm_add_ps( sum0.f, tmp.f );
                        tmp.f = src1.f;
                        tmp.f = _mm_mul_ps( tmp.f, kernel[N].f );
                        sum1.f = _mm_add_ps( sum1.f, tmp.f );
                        tmp.f = src2.f;
                        tmp.f = _mm_mul_ps( tmp.f, kernel[N].f );
                        sum2.f = _mm_add_ps( sum2.f, tmp.f );
                        tmp.f = src3.f;
                        tmp.f = _mm_mul_ps( tmp.f, kernel[N].f );
                        sum3.f = _mm_add_ps( sum3.f, tmp.f );
                    } // for N

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

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses single precision floating point data type for
    // intermediate calculations.  It assumes that the kernel has length of 5 elements.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_5_32f : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 5 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv, maximum;
            zero.i = _mm_setzero_si128();
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)pData->divisor );
            maximum.f = _mm_set1_ps( (float)32767 );

            XMM128 kernel[5];
            for( int n = 0; n < 5; n++ )
            {
                kernel[n].f = _mm_set1_ps( (float)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0, sum1, sum2, sum3, tmp, ext4;

                    // load 16 bytes at a time
                    src0.i = _mm_load_si128( (__m128i*)ps );
                    ext4.i = _mm_cvtsi32_si128( *(int*)(ps + 16) );

                    // unpack values
                    src2.i = _mm_unpackhi_epi8( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi8( src0.i, zero.i );
                    src3.i = _mm_unpackhi_epi16( src2.i, zero.i );
                    src2.i = _mm_unpacklo_epi16( src2.i, zero.i );
                    src1.i = _mm_unpackhi_epi16( src0.i, zero.i );
                    src0.i = _mm_unpacklo_epi16( src0.i, zero.i );

                    ext4.i = _mm_unpacklo_epi8( ext4.i, zero.i );
                    ext4.i = _mm_unpacklo_epi16( ext4.i, zero.i );

                    // convert to float
                    src0.f = _mm_cvtepi32_ps( src0.i );
                    src1.f = _mm_cvtepi32_ps( src1.i );
                    src2.f = _mm_cvtepi32_ps( src2.i );
                    src3.f = _mm_cvtepi32_ps( src3.i );
                    ext4.f = _mm_cvtepi32_ps( ext4.i );

                    // multiply by kernel[4];
                    sum0.i = src0.i;
                    sum1.i = src1.i;
                    sum2.i = src2.i;
                    sum3.i = src3.i;
                    sum0.f = _mm_mul_ps( sum0.f, kernel[4].f );
                    sum1.f = _mm_mul_ps( sum1.f, kernel[4].f );
                    sum2.f = _mm_mul_ps( sum2.f, kernel[4].f );
                    sum3.f = _mm_mul_ps( sum3.f, kernel[4].f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_move_ss( src3.f, ext4.f );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    ext4.i = _mm_srli_si128( ext4.i, 4 );

                    // multiply by kernel[3] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[3].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[3].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[3].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[3].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_move_ss( src3.f, ext4.f );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    ext4.i = _mm_srli_si128( ext4.i, 4 );

                    // multiply by kernel[2] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[2].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[2].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[2].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[2].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_move_ss( src3.f, ext4.f );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    ext4.i = _mm_srli_si128( ext4.i, 4 );

                    // multiply by kernel[1] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_move_ss( src3.f, ext4.f );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    ext4.i = _mm_srli_si128( ext4.i, 4 );

                    // multiply by kernel[0] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );

                    // divide by divisor
                    sum0.f = _mm_mul_ps( sum0.f, divisor_inv.f );
                    sum1.f = _mm_mul_ps( sum1.f, divisor_inv.f );
                    sum2.f = _mm_mul_ps( sum2.f, divisor_inv.f );
                    sum3.f = _mm_mul_ps( sum3.f, divisor_inv.f );

                    // saturate to max 32 bit signed integer size
                    sum0.f = _mm_min_ps( sum0.f, maximum.f );
                    sum1.f = _mm_min_ps( sum1.f, maximum.f );
                    sum2.f = _mm_min_ps( sum2.f, maximum.f );
                    sum3.f = _mm_min_ps( sum3.f, maximum.f );

                    // convert to 32 bit integer
                    sum0.i = _mm_cvtps_epi32( sum0.f );
                    sum1.i = _mm_cvtps_epi32( sum1.f );
                    sum2.i = _mm_cvtps_epi32( sum2.f );
                    sum3.i = _mm_cvtps_epi32( sum3.f );

                    // saturate the result
                    sum0.i = _mm_packs_epi32( sum0.i, sum1.i );
                    sum2.i = _mm_packs_epi32( sum2.i, sum3.i );
                    sum0.i = _mm_packus_epi16( sum0.i, sum2.i );

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses 16 bit signed integer data type for
    // intermediate calculations.  It assumes that the kernel has length of 5 elements.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_5_16s : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 5 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.i = _mm_set1_epi16( (short)(65536 / pData->divisor) );

            XMM128 kernel[5];
            for( int n = 0; n < 5; n++ )
            {
                kernel[n].i = _mm_set1_epi16( (short)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi, tmp_lo, tmp_hi;

                    // load 16 bytes at a time
                    src_hi.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                    src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                    // multiply by kernel[4];
                    sum_lo.i = _mm_mullo_epi16( src_lo.i, kernel[4].i );
                    sum_hi.i = _mm_mullo_epi16( src_hi.i, kernel[4].i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 16), 7 );

                    
                    // multiply by kernel[3];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[3].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[3].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 17), 7 );
                    
                    // multiply by kernel[2];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[2].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[2].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 18), 7 );
                    
                    // multiply by kernel[1];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[1].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[1].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 19), 7 );
                    
                    // multiply by kernel[0];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[0].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[0].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // divide by divisor
                    sum_lo.i = CBL_SSE2::FastDivide_16s_const( sum_lo.i, divisor_inv.i );
                    sum_hi.i = CBL_SSE2::FastDivide_16s_const( sum_hi.i, divisor_inv.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses 16 bit signed integer data type for
    // intermediate calculations.  It assumes that the kernel has length of 5 elements.  In addition it
    // performs no scaling/division of the result (useful when divisor is 1 or -1)
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_5_16s_NoDivision : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 5 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 kernel[5];
            for( int n = 0; n < 5; n++ )
            {
                kernel[n].i = _mm_set1_epi16( (short)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.

                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi, tmp_lo, tmp_hi;

                    // load 16 bytes at a time
                    src_hi.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                    src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                    // multiply by kernel[4];
                    sum_lo.i = _mm_mullo_epi16( src_lo.i, kernel[4].i );
                    sum_hi.i = _mm_mullo_epi16( src_hi.i, kernel[4].i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 16), 7 );

                    
                    // multiply by kernel[3];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[3].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[3].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 17), 7 );
                    
                    // multiply by kernel[2];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[2].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[2].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 18), 7 );
                    
                    // multiply by kernel[1];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[1].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[1].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 19), 7 );
                    
                    // multiply by kernel[0];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[0].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[0].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                    
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses single precision floating point data type for
    // intermediate calculations.  It assumes that the kernel has length of 3 elements.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_3_32f : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 3 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv, maximum;
            zero.i = _mm_setzero_si128();
            divisor_inv.f = _mm_set1_ps( (float)1 / (float)pData->divisor );
            maximum.f = _mm_set1_ps( (float)32767 );

            XMM128 kernel[3];
            for( int n = 0; n < 3; n++ )
            {
                kernel[n].f = _mm_set1_ps( (float)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src0, src1, src2, src3, sum0, sum1, sum2, sum3, tmp;

                    // load 16 bytes at a time
                    src0.i = _mm_load_si128( (__m128i*)ps );

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

                    // multiply by kernel[2];
                    sum0.i = src0.i;
                    sum1.i = src1.i;
                    sum2.i = src2.i;
                    sum3.i = src3.i;
                    sum0.f = _mm_mul_ps( sum0.f, kernel[2].f );
                    sum1.f = _mm_mul_ps( sum1.f, kernel[2].f );
                    sum2.f = _mm_mul_ps( sum2.f, kernel[2].f );
                    sum3.f = _mm_mul_ps( sum3.f, kernel[2].f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_cvtsi32_ss( src3.f, *(ps + 16) );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );

                    // multiply by kernel[1] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[1].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );

                    // shift the data in src0, src1, src2 and src3
                    src0.f = _mm_move_ss( src0.f, src1.f );
                    src0.i = _mm_shuffle_epi32( src0.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src1.f = _mm_move_ss( src1.f, src2.f );
                    src1.i = _mm_shuffle_epi32( src1.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src2.f = _mm_move_ss( src2.f, src3.f );
                    src2.i = _mm_shuffle_epi32( src2.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );
                    src3.f = _mm_cvtsi32_ss( src3.f, *(ps + 17) );
                    src3.i = _mm_shuffle_epi32( src3.i, CBL_SHUF_IMM( 0, 3, 2, 1 ) );

                    // multiply by kernel[0] and add to sum;
                    tmp.f = src0.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum0.f = _mm_add_ps( sum0.f, tmp.f );
                    tmp.f = src1.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum1.f = _mm_add_ps( sum1.f, tmp.f );
                    tmp.f = src2.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum2.f = _mm_add_ps( sum2.f, tmp.f );
                    tmp.f = src3.f;
                    tmp.f = _mm_mul_ps( tmp.f, kernel[0].f );
                    sum3.f = _mm_add_ps( sum3.f, tmp.f );


                    // divide by divisor
                    sum0.f = _mm_mul_ps( sum0.f, divisor_inv.f );
                    sum1.f = _mm_mul_ps( sum1.f, divisor_inv.f );
                    sum2.f = _mm_mul_ps( sum2.f, divisor_inv.f );
                    sum3.f = _mm_mul_ps( sum3.f, divisor_inv.f );

                    // saturate to max 32 bit signed integer size
                    sum0.f = _mm_min_ps( sum0.f, maximum.f );
                    sum1.f = _mm_min_ps( sum1.f, maximum.f );
                    sum2.f = _mm_min_ps( sum2.f, maximum.f );
                    sum3.f = _mm_min_ps( sum3.f, maximum.f );

                    // convert to 32 bit integer
                    sum0.i = _mm_cvtps_epi32( sum0.f );
                    sum1.i = _mm_cvtps_epi32( sum1.f );
                    sum2.i = _mm_cvtps_epi32( sum2.f );
                    sum3.i = _mm_cvtps_epi32( sum3.f );

                    // saturate the result
                    sum0.i = _mm_packs_epi32( sum0.i, sum1.i );
                    sum2.i = _mm_packs_epi32( sum2.i, sum3.i );
                    sum0.i = _mm_packus_epi16( sum0.i, sum2.i );

                    _mm_storeu_si128( (__m128i*)pd, sum0.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses 16 bit signed integer data type for
    // intermediate calculations.  It assumes that the kernel has length of 3 elements.
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_3_16s : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 3 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero, divisor_inv;
            zero.i = _mm_setzero_si128();
            divisor_inv.i = _mm_set1_epi16( (short)(65536 / pData->divisor) );

            XMM128 kernel[3];
            for( int n = 0; n < 3; n++ )
            {
                kernel[n].i = _mm_set1_epi16( (short)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi, tmp_lo, tmp_hi;

                    // load 16 bytes at a time
                    src_hi.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                    src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                    // multiply by kernel[2];
                    sum_lo.i = _mm_mullo_epi16( src_lo.i, kernel[2].i );
                    sum_hi.i = _mm_mullo_epi16( src_hi.i, kernel[2].i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 16), 7 );

                    
                    // multiply by kernel[1];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[1].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[1].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 17), 7 );
                    
                    // multiply by kernel[0];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[0].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[0].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // divide by divisor
                    sum_lo.i = CBL_SSE2::FastDivide_16s_const( sum_lo.i, divisor_inv.i );
                    sum_hi.i = CBL_SSE2::FastDivide_16s_const( sum_hi.i, divisor_inv.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

    // SSE2 optimized version of the FilterRow code that uses 16 bit signed integer data type for
    // intermediate calculations.  It assumes that the kernel has length of 3 elements.  It does not 
    // perform any division/scaling of the result (useful if divisor is 1 or -1)
    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct FilterRowProcessor_3_16s_NoDivision : 
        public FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor<
        chCount, SrcType, DstType, CalcType, Operator, DataType >
    {
        static inline void SSE2( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;

            int anchor = pData->anchor;
            int anchorOffset = 3 - anchor - 1;
            const SrcType *pSrc = pData->pSrc - anchorOffset * chCount;
            DstType *pDst = pData->pDst;
            DstType *pDstEnd = (DstType *)(((Fw8u*)pData->pDst) + pData->dstStep * pData->dstRoiSize.height);
            Fw32s * pKernel = (Fw32s *)pData->pKernel;
            
            XMM128 zero;
            zero.i = _mm_setzero_si128();

            XMM128 kernel[3];
            for( int n = 0; n < 3; n++ )
            {
                kernel[n].i = _mm_set1_epi16( (short)pKernel[n] );
            }

            while( pDst < pDstEnd )
            {
                const SrcType *ps = pSrc;
                DstType *pd = pDst;

                U32 width      = pData->dstRoiSize.width;
                U32 startWidth = FW_REF::BytesToAlign16(ps);			// Bytes before alignment point.
                U32 endWidth   = (width-startWidth) % 16;			// Bytes at end that can't be processed 16 wide.
                U32 midwidth   = width - startWidth - endWidth;		// Aligned bytes in the middle that can be processed 16 wide.
                    
                if( width < 16 )
                {
                    startWidth = 0;
                    midwidth = 0;
                }
                    
                const Fw8u *pEndStart  = pd + startWidth;			// End of unaligned data / Start of aligned data 			
                const Fw8u *pEndMiddle = pd + startWidth + midwidth;				// End of aligned x 16 data.
                const Fw8u *pEnd       = pd + width;				// Enf of all the data.	
    
                /////////////////////////////////////////////////////////////////
                // process the unaligned "prefix" data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
                for( ; pd<pEndStart; ++pd, ++ps ) // Process 1 at a time until we hit an alignment boundary
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                for( ; pd<pEndMiddle; ps+=16, pd+=16 )				// Process 1 horizontal line (16 wide)
                {
                    XMM128 src_lo, src_hi, sum_lo, sum_hi, tmp_lo, tmp_hi;

                    // load 16 bytes at a time
                    src_hi.i = _mm_load_si128( (__m128i*)ps );

                    // unpack data
                    src_lo.i = _mm_unpacklo_epi8( src_hi.i, zero.i );
                    src_hi.i = _mm_unpackhi_epi8( src_hi.i, zero.i );

                    // multiply by kernel[2];
                    sum_lo.i = _mm_mullo_epi16( src_lo.i, kernel[2].i );
                    sum_hi.i = _mm_mullo_epi16( src_hi.i, kernel[2].i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 16), 7 );

                    
                    // multiply by kernel[1];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[1].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[1].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // shift the data in src_lo, src_hi
                    tmp_lo.i = _mm_slli_si128( src_hi.i, 14 );
                    src_lo.i = _mm_srli_si128( src_lo.i, 2 );
                    src_hi.i = _mm_srli_si128( src_hi.i, 2 );
                    src_lo.i = _mm_or_si128( src_lo.i, tmp_lo.i );
    		        src_hi.i = _mm_insert_epi16( src_hi.i, *(ps + 17), 7 );
                    
                    // multiply by kernel[0];
                    tmp_lo.i = _mm_mullo_epi16( src_lo.i, kernel[0].i );
                    tmp_hi.i = _mm_mullo_epi16( src_hi.i, kernel[0].i );

                    // add to sum
                    sum_lo.i = _mm_adds_epi16( sum_lo.i, tmp_lo.i );
                    sum_hi.i = _mm_adds_epi16( sum_hi.i, tmp_hi.i );

                    // saturate the result
                    sum_lo.i = _mm_packus_epi16( sum_lo.i, sum_hi.i );	// Saturate the calculated values.

                    _mm_storeu_si128( (__m128i*)pd, sum_lo.i );
//                    _mm_stream_si128( (__m128i*)pd, sum_lo.i );
                    
                }

                /////////////////////////////////////////////////////////////////
                // process the reminder data (up to 15 bytes)
                /////////////////////////////////////////////////////////////////
    
                for( ; pd<pEnd; ++pd, ++ps )					// Process the remainder.
                    Calculate( ps+anchorOffset, pData->srcStep, pd, pData->dstStep, pKernel, pData->kernelSize, pData->anchor, pData->divisor );

                pSrc = (const SrcType *)(((Fw8u*)pSrc) + pData->srcStep);
                pDst = (DstType *)(((Fw8u*)pDst) + pData->dstStep);
            }
        }

        static inline void SSE2_Unaligned( void *p )
        {
            SSE2( p );
        }
    };

} // namespace FiltersSeparable

} // namespace OPT_LEVEL

using namespace OPT_LEVEL;

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize, divisor );
    switch( atype )
    {
    case FilterCommon::arithmetics_16bit:
        if( divisor == 1 ) 
        {
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterColumnProcessor_16s_NoDivision,
                FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
        }
        else if( divisor == -1 ) 
        {
            if( pKernel == NULL )
                return fwStsNullPtrErr;

            // create a negative copy of the kernel
            size_t count = kernelSize;
            Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
            for( size_t n = 0; n < count; n++ )
            {
                pTempKernel[n] = - pKernel[n];
            }

            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterColumnProcessor_16s_NoDivision,
                FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, yAnchor, -divisor );
        }
        else
        {
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterColumnProcessor_16s,
                FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
        }
    case FilterCommon::arithmetics_24bit:
        return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw64s, 
            FiltersSeparable::FilterColumnProcessor_32f,
            FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
    case FilterCommon::arithmetics_32bit:
        /*
        return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw64s, 
            FiltersSeparable::FilterColumnProcessor_32s,
            FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
        */
    default:
        return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customverticalkernel_size_anchor_divisor< 
            C1,
            Fw8u, 
            Fw8u, 
            Fw64s, 
            FiltersSeparable::FilterColumnProcessor,
            FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernelAndBuffer >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
    //FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize, divisor );
    //switch( atype )
    //{
    //case FilterCommon::arithmetics_16bit:
    //case FilterCommon::arithmetics_24bit:
    //case FilterCommon::arithmetics_32bit:
    //    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //        C3,
    //        Fw8u, 
    //        Fw8u, 
    //        Fw32s, 
    //        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //        FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernel >
    //    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
    //
    //case FilterCommon::arithmetics_64bit:
    //default:
    //    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //        C3,
    //        Fw8u, 
    //        Fw8u, 
    //        Fw64s, 
    //        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //        FiltersSeparable::FilterColumn, FilterCommon::DataWithLinearKernel >
    //    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor, divisor );
    //}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C1R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C3R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_C4R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int yAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C1R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
    //return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor< 
    //    C1,
    //    Fw32f,
    //    Fw32f,
    //    Fw32f,
    //    FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor,
    //    FiltersSeparable::FilterColumn32f, FilterCommon::DataWithLinearKernel >
    //::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, yAnchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C3R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_C4R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterColumn32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int yAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = kernelSize;
    kernel.width = 1;
    anchor.y = yAnchor;
    anchor.x = 0;

    return fwiFilter32f_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C1R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize, divisor );
    if( kernelSize == 5 )
    {
        switch( atype )
        {
        case FilterCommon::arithmetics_16bit:
            if( divisor == 1 ) 
            {
                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_5_16s_NoDivision,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
            }
            else if( divisor == -1 ) 
            {
                if( pKernel == NULL )
                    return fwStsNullPtrErr;

                // create a negative copy of the kernel
                size_t count = kernelSize;
                Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
                for( size_t n = 0; n < count; n++ )
                {
                    pTempKernel[n] = - pKernel[n];
                }

                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_5_16s_NoDivision,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, xAnchor, -divisor );
            }
            else
            {
                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_5_16s,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
            }
        case FilterCommon::arithmetics_24bit:
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw32s,
                FiltersSeparable::FilterRowProcessor_5_32f,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        case FilterCommon::arithmetics_32bit:
/*
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw32s,
                FiltersSeparable::FilterRowProcessor_5_32s,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
*/
        default:
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterRowProcessor,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        }
    }
    else if( kernelSize == 3 )
    {
        switch( atype )
        {
        case FilterCommon::arithmetics_16bit:
            if( divisor == 1 ) 
            {
                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_3_16s_NoDivision,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
            }
            else if( divisor == -1 ) 
            {
                if( pKernel == NULL )
                    return fwStsNullPtrErr;

                // create a negative copy of the kernel
                size_t count = kernelSize;
                Fw32s * pTempKernel = (Fw32s *)alloca( count * sizeof(Fw32s) );
                for( size_t n = 0; n < count; n++ )
                {
                    pTempKernel[n] = - pKernel[n];
                }

                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_3_16s_NoDivision,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pTempKernel, kernelSize, xAnchor, -divisor );
            }
            else
            {
                return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                    C1,
                    Fw8u,
                    Fw8u,
                    Fw16s,
                    FiltersSeparable::FilterRowProcessor_3_16s,
                    FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
                ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
            }
        case FilterCommon::arithmetics_24bit:
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw32s,
                FiltersSeparable::FilterRowProcessor_3_32f,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        case FilterCommon::arithmetics_32bit:
/*
            return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_horizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw32s,
                FiltersSeparable::FilterRowProcessor_3_32s,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
*/
        default:
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterRowProcessor,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        }
    }
    else 
    {
        switch( atype )
        {
        case FilterCommon::arithmetics_16bit:
        case FilterCommon::arithmetics_24bit:
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterRowProcessor_32f,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        case FilterCommon::arithmetics_32bit:
        default:
            return FiltersSeparable::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_customhorizontalkernel_size_anchor_divisor< 
                C1,
                Fw8u, 
                Fw8u, 
                Fw64s, 
                FiltersSeparable::FilterRowProcessor,
                FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernelAndBuffer >
            ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
        }
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C3R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
    //FilterCommon::ArithmeticsType atype = FilterCommon::KernelAnalysis8bit( pKernel, kernelSize, divisor );
    //switch( atype )
    //{
    //case FilterCommon::arithmetics_16bit:
    //case FilterCommon::arithmetics_24bit:
    //case FilterCommon::arithmetics_32bit:
    //    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //        C3,
    //        Fw8u, 
    //        Fw8u, 
    //        Fw32s, 
    //        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //        FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
    //    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );

    //case FilterCommon::arithmetics_64bit:
    //default:
    //    return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor< 
    //        C3,
    //        Fw8u, 
    //        Fw8u, 
    //        Fw64s, 
    //        FilterCommon::TProcessor_pSrc_sStep_pDst_dStep_roi_kernel_size_anchor_divisor,
    //        FiltersSeparable::FilterRow, FilterCommon::DataWithLinearKernel >
    //    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernelSize, xAnchor, divisor );
    //}
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_C4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                             Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C1R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C3R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_C4R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                             Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32s * pKernel, int kernelSize,
                             int xAnchor, int divisor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor, divisor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C1R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_32f_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C3R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_32f_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_C4R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_32f_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                             Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                             const Fw32f * pKernel, int kernelSize,
                             int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter_32f_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_8u_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_8u_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_C4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_8u_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                const Fw32f * pKernel, int kernelSize,
                                int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_8u_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_16s_C1R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_16s_C3R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_C4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_16s_C4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterRow32f_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                 Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                 const Fw32f * pKernel, int kernelSize,
                                 int xAnchor )
{
    FwiSize kernel;
    FwiPoint anchor;

    kernel.height = 1;
    kernel.width = kernelSize;
    anchor.y = 0;
    anchor.x = xAnchor;

    return fwiFilter32f_16s_AC4R( pSrc, srcStep, pDst, dstStep, dstRoiSize, pKernel, kernel, anchor );
}

#endif // BUILD_NUM_AT_LEAST

//#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
