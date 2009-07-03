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

namespace FilterWiener
{
    inline void GetBufferSize( FwiSize roiSize, FwiSize maskSize, int channels, int * pBufferSize )
    {
        roiSize;
        maskSize;
        channels;
        *pBufferSize = 16;
    }

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct Wiener
    {
        static inline void CalculateNoise( const SrcType * pSrcPtr, int srcStep, FwiSize roiSize, FwiSize maskSize, FwiPoint anchor, Fw32f noise[chCount], Fw8u * /*pBuffer*/ )
        {
            Fw32f sum[chCount];

            int maskCount = maskSize.height * maskSize.width;

            for( int ch = 0; ch < chCount; ch++ )
            {
                sum[ch] = 0;
                noise[ch] = 0;
            }

            const SrcType *pSrcEnd = (const SrcType *)( ((Fw8u*)pSrcPtr) + srcStep * roiSize.height );
            while( pSrcPtr < pSrcEnd )
            {
                const SrcType *pSrc = pSrcPtr;
                const SrcType *pLineEnd = pSrc + roiSize.width * chCount;
                while( pSrc < pLineEnd )
                {
                    for( int y = 0; y < maskSize.height; y++ )
                    {
                        for( int x = 0; x < maskSize.width; x++ )
                        {
                            for( int ch = 0; ch < chCount; ch++ )
                            {
                                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( y - anchor.y ) * srcStep );
                                SrcType value = *( pSrc + ( x - anchor.x ) * chCount + ch );
                                sum[ch] += value;
                            }
                        }
                    }

                    CalcType mean[chCount];
                    CalcType variance2[chCount];
                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        mean[ch] = (CalcType)sum[ch] / maskCount;
                        variance2[ch] = 0;
                    }

                    for( int y = 0; y < maskSize.height; y++ )
                    {
                        for( int x = 0; x < maskSize.width; x++ )
                        {
                            for( int ch = 0; ch < chCount; ch++ )
                            {
                                const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( y - anchor.y ) * srcStep );
                                SrcType value = *( pSrc + ( x - anchor.x ) * chCount + ch );
                                variance2[ch] += ( (CalcType)value - mean[ch] ) * ( (CalcType)value - mean[ch] );
                            }
                        }
                    }

                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        variance2[ch] /= maskCount;

                        // @TODO: Need to verify if a square root should be used here.
                        noise[ch] += sqrt(variance2[ch]);
                        //noise[ch] += variance2[ch]; 
                        sum[ch] = 0;
                    }
                    pSrc += chCount;
                }
                pSrcPtr = (const SrcType *)( ((Fw8u*)pSrcPtr) + srcStep );
            }

            int pixelCount = roiSize.height * roiSize.width;
            for( int ch = 0; ch < chCount; ch++ )
            {
                noise[ch] /= pixelCount;
            }
        }

        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/,
                                        FwiSize maskSize, FwiPoint anchor,
                                        const Fw32f noise[chCount], Fw8u * /*pBuffer*/ )
        {
            int maskCount = maskSize.height * maskSize.width;
            CalcType sum[chCount] = {0};

            for( int y = 0; y < maskSize.height; y++ )
            {
                for( int x = 0; x < maskSize.width; x++ )
                {
                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( y - anchor.y ) * srcStep );
                        SrcType value = *( pSrc + ( x - anchor.x ) * chCount + ch );
                        sum[ch] += (CalcType)value;
                    }
                }
            }

            CalcType mean[chCount];
            CalcType variance2[chCount];
            for( int ch = 0; ch < chCount; ch++ )
            {
                mean[ch] = (CalcType)sum[ch] / maskCount;
                variance2[ch] = 0;
            }

            for( int y = 0; y < maskSize.height; y++ )
            {
                for( int x = 0; x < maskSize.width; x++ )
                {
                    for( int ch = 0; ch < chCount; ch++ )
                    {
                        const SrcType * pSrc = (const SrcType *)(((Fw8u*)pSrcPtr) + ( y - anchor.y ) * srcStep );
                        SrcType value = *( pSrc + ( x - anchor.x ) * chCount + ch );
                        variance2[ch] += ( (CalcType)value - mean[ch] ) * ( (CalcType)value - mean[ch] );
                    }
                }
            }

            for( int ch = 0; ch < chCount; ch++ )
            {
                variance2[ch] /= maskCount;
                if( variance2[ch] == 0 )
                {
                    Fw32f result = mean[ch];
                    *pDst = FW_REF::Limits<DstType>::Sat( result );
                }
                else
                {
                    Fw32f result = mean[ch] + ( ( variance2[ch] - noise[ch] ) / variance2[ch] ) * ( (CalcType)*( pSrcPtr + ch ) - mean[ch] );
                    *pDst = FW_REF::Limits<DstType>::Sat( result );
                }
            }
        }
    };

    template< typename SrcDataType, typename DstDataType >
    struct DataWithNoise : public FilterCommon::Data< SrcDataType, DstDataType >
    {
        FwiSize    maskSize;
        FwiPoint   anchor;
        Fw32f     *noise;
        Fw8u      *pBuffer;
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType, 
              template< CH, typename, typename, typename > class Operator,
              template< typename, typename > class DataType >
    struct TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer : public Operator< chCount, SrcType, DstType, CalcType >
    {
        static inline void REFR( void *p )
        {
            DataType< SrcType, DstType > *pData = (DataType< SrcType, DstType >*) p;
            const SrcType * pSrc = pData->pSrc;
            DstType * pDst = pData->pDst;
            DstType * pEndDst = FW_REF::Offset( pData->pDst, pData->dstRoiSize.height * pData->dstStep );
            while( pDst < pEndDst )
            {
                const SrcType *pSrcElement = pSrc;
                DstType *pDstElement = pDst;
                DstType *pLineEnd = pDstElement + pData->dstRoiSize.width * chCount;
                while( pDstElement < pLineEnd )
                {
                    Calculate( pSrcElement, pData->srcStep, 
                               pDstElement, pData->dstStep, 
                               pData->maskSize, pData->anchor, 
                               pData->noise, pData->pBuffer );
                    pSrcElement += chCount;
                    pDstElement += chCount;
                }
                pDst = FW_REF::Offset(pDst, pData->dstStep);
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
    struct DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
    {
        typedef DataType< SrcType, DstType > DataClass;

        static inline FwStatus Run( const SrcType * pSrc, int srcStep, DstType * pDst, int dstStep, FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor, Fw32f noise[chCount], Fw8u * pBuffer )
        {
            if( pSrc == NULL || pDst == NULL )
                return fwStsNullPtrErr;
                                        
            if (dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )	
                return fwStsSizeErr;

            if (srcStep <= 0 || dstStep <= 0)	
                return fwStsStepErr;

            if( maskSize.width <= 0 || maskSize.height <= 0 )
                return fwStsMaskSizeErr;

            if( anchor.x >= maskSize.width || anchor.x <= 0 ||
                anchor.y >= maskSize.height || anchor.y <= 0 )
                return fwStsAnchorErr;

            bool fCalculateNoise = false;
            int effectiveChannels = (chCount == AC4) ? 3 : chCount;
            for( int ch = 0; ch < effectiveChannels; ch++ )
            {
                if( noise[ch] < 0 || noise[ch] > 1 )
                    return fwStsNoiseRangeErr;

                if( noise[ch] == 0 )
                {
                    fCalculateNoise = true;
                    break;
                }
            }
            if( fCalculateNoise )
            {
                Fw32f noiseBuffer[chCount];
                Operator< chCount, SrcType, DstType, CalcType >
                    ::CalculateNoise( pSrc, srcStep, dstRoiSize, maskSize, anchor, noiseBuffer, pBuffer );
                for( int ch = 0; ch < effectiveChannels; ch++ )
                {
                    if( noise[ch] == 0 )
                    {
                        noise[ch] = noiseBuffer[ch];
                    }
                }
            }

            int nBufferSize;
            GetBufferSize( dstRoiSize, maskSize, effectiveChannels, &nBufferSize );

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

                    const SrcType * ps = pSrc;
                    DstType * pd = pDst;

                    for( U32 i=0; i<tCount; ++i )
                    {				
                        data[i].pSrc				= ps;
                        data[i].pDst				= pd;	
                        data[i].srcStep				= srcStep;				
                        data[i].dstStep				= dstStep;
                        data[i].dstRoiSize.width    = dstRoiSize.width;
                        data[i].maskSize            = maskSize;
                        data[i].anchor	            = anchor;
                        data[i].noise               = noise;
                        data[i].pBuffer             = pBuffer;

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
                    data.maskSize = maskSize;
                    data.anchor	= anchor;
                    data.noise = noise;
                    data.pBuffer = pBuffer;

                    Processor< chCount, SrcType, DstType, 
                               CalcType, Operator, DataType >::REFR( &data );

                    return fwStsNoErr; 
                }
            } // switch
        } // Run
    };


} // namespace FilterWiener

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWienerGetBufferSize)( FwiSize dstRoiSize, FwiSize maskSize, int channels, int * pBufferSize )
{
    if( pBufferSize == NULL )
        return fwStsNullPtrErr;
    if( dstRoiSize.height <= 0 || dstRoiSize.width <= 0 )
        return fwStsSizeErr;
    if( maskSize.height <= 0 || maskSize.width <= 0 )
        return fwStsMaskSizeErr;
    if( channels < 1 || channels > 4 )
        return fwStsNumChannelsErr;

    dstRoiSize;
    maskSize;
    channels;
    *pBufferSize = 16;
    return fwStsNoErr; 
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_8u_C1R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[1], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C1,
        Fw8u,
        Fw8u,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_16s_C1R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[1], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C1,
        Fw16s,
        Fw16s,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_32f_C1R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[1], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C1,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_8u_C3R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C3,
        Fw8u,
        Fw8u,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_16s_C3R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C3,
        Fw16s,
        Fw16s,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_32f_C3R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C3,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_8u_C4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[4], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C4,
        Fw8u,
        Fw8u,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_16s_C4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[4], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C4,
        Fw16s,
        Fw16s,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_32f_C4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[4], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        C4,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_8u_AC4R)( const Fw8u * pSrc, int srcStep, Fw8u * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        AC4,
        Fw8u,
        Fw8u,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_16s_AC4R)( const Fw16s * pSrc, int srcStep, Fw16s * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        AC4,
        Fw16s,
        Fw16s,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterWiener_32f_AC4R)( const Fw32f * pSrc, int srcStep, Fw32f * pDst, int dstStep,
                                   FwiSize dstRoiSize, FwiSize maskSize, FwiPoint anchor,
                                   Fw32f noise[3], Fw8u * pBuffer )
{
    return FilterWiener::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer
        < 
        AC4,
        Fw32f,
        Fw32f,
        Fw32f,
        FilterWiener::TProcessor_pSrc_sStep_pDst_dStep_roi_mask_anchor_noise_buffer,
        FilterWiener::Wiener, 
        FilterWiener::DataWithNoise
        >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, noise, pBuffer );
}

#endif // BUILD_NUM_AT_LEAST

//#endif

#endif

