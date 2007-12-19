/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FILTERS_FIXED_H__
#define __FILTERS_FIXED_H__

namespace OPT_LEVEL
{
namespace FiltersFixed
{
    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterPrewittHoriz
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            CalcType sum = - (CalcType)*(pSrcTop - chCount) - (CalcType)*pSrcTop - (CalcType)*(pSrcTop + chCount)
                           + (CalcType)*(pSrcBtm - chCount) + (CalcType)*pSrcBtm + (CalcType)*(pSrcBtm + chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterPrewittVert
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum = + (CalcType)*pVTL - (CalcType)*(pVTL + 2 * chCount )
                           + (CalcType)*pVML - (CalcType)*(pVML + 2 * chCount )
                           + (CalcType)*pVBL - (CalcType)*(pVBL + 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterScharrHoriz
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            CalcType sum = - 3 * (CalcType)*(pSrcTop - chCount) - 10 * (CalcType)*pSrcTop - 3 * (CalcType)*(pSrcTop + chCount)
                           + 3 * (CalcType)*(pSrcBtm - chCount) + 10 * (CalcType)*pSrcBtm + 3 * (CalcType)*(pSrcBtm + chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterScharrVert
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum = -  3 * (CalcType)*pVTL +  3 * (CalcType)*(pVTL + 2 * chCount )
                           - 10 * (CalcType)*pVML + 10 * (CalcType)*(pVML + 2 * chCount )
                           -  3 * (CalcType)*pVBL +  3 * (CalcType)*(pVBL + 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelHoriz
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            CalcType sum = - (CalcType)*(pSrcTop - chCount) - 2 * (CalcType)*pSrcTop - (CalcType)*(pSrcTop + chCount)
                           + (CalcType)*(pSrcBtm - chCount) + 2 * (CalcType)*pSrcBtm + (CalcType)*(pSrcBtm + chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelHoriz5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop2 = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep );
            const SrcType * pSrcTop  = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm  = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            const SrcType * pSrcBtm2 = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep );
            CalcType sum = -   (CalcType)*(pSrcTop2-2*chCount) - 4*(CalcType)*(pSrcTop2-chCount) -  6*(CalcType)*pSrcTop2 - 4*(CalcType)*(pSrcTop2+chCount) -   (CalcType)*(pSrcTop2+2*chCount)
                           - 2*(CalcType)*(pSrcTop -2*chCount) - 8*(CalcType)*(pSrcTop -chCount) - 12*(CalcType)*pSrcTop  - 8*(CalcType)*(pSrcTop +chCount) - 2*(CalcType)*(pSrcTop +2*chCount)
                           + 2*(CalcType)*(pSrcBtm -2*chCount) + 8*(CalcType)*(pSrcBtm -chCount) + 12*(CalcType)*pSrcBtm  + 8*(CalcType)*(pSrcBtm +chCount) + 2*(CalcType)*(pSrcBtm +2*chCount)
                           +   (CalcType)*(pSrcBtm2-2*chCount) + 4*(CalcType)*(pSrcBtm2-chCount) +  6*(CalcType)*pSrcBtm2 + 4*(CalcType)*(pSrcBtm2+chCount) +   (CalcType)*(pSrcBtm2+2*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelVert
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum =       (CalcType)*pVTL -     (CalcType)*(pVTL + 2 * chCount )
                           + 2 * (CalcType)*pVML - 2 * (CalcType)*(pVML + 2 * chCount )
                           +     (CalcType)*pVBL -     (CalcType)*(pVBL + 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelVert5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum =     (CalcType)*pV1L +  2*(CalcType)*(pV1L+chCount) -  2*(CalcType)*(pV1L+3*chCount ) -   (CalcType)*(pV1L+4*chCount)
                           + 4*(CalcType)*pV2L +  8*(CalcType)*(pV2L+chCount) -  8*(CalcType)*(pV2L+3*chCount ) - 4*(CalcType)*(pV2L+4*chCount)
                           + 6*(CalcType)*pV3L + 12*(CalcType)*(pV3L+chCount) - 12*(CalcType)*(pV3L+3*chCount ) - 6*(CalcType)*(pV3L+4*chCount)
                           + 4*(CalcType)*pV4L +  8*(CalcType)*(pV4L+chCount) -  8*(CalcType)*(pV4L+3*chCount ) - 4*(CalcType)*(pV4L+4*chCount)
                           +   (CalcType)*pV5L +  2*(CalcType)*(pV5L+chCount) -  2*(CalcType)*(pV5L+3*chCount ) -   (CalcType)*(pV5L+4*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelNegVert
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum =     (CalcType)*pVTL +     (CalcType)*(pVTL - 2 * chCount )
                         + 2 * (CalcType)*pVML + 2 * (CalcType)*(pVML - 2 * chCount )
                         +     (CalcType)*pVBL +     (CalcType)*(pVBL - 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelNegVert5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum =   (CalcType)*pV1L +  2*(CalcType)*(pV1L+chCount) -  2*(CalcType)*(pV1L+3*chCount ) -   (CalcType)*(pV1L+4*chCount)
                         + 4*(CalcType)*pV2L +  8*(CalcType)*(pV2L+chCount) -  8*(CalcType)*(pV2L+3*chCount ) - 4*(CalcType)*(pV2L+4*chCount)
                         + 6*(CalcType)*pV3L + 12*(CalcType)*(pV3L+chCount) - 12*(CalcType)*(pV3L+3*chCount ) - 6*(CalcType)*(pV3L+4*chCount)
                         + 4*(CalcType)*pV4L +  8*(CalcType)*(pV4L+chCount) -  8*(CalcType)*(pV4L+3*chCount ) - 4*(CalcType)*(pV4L+4*chCount)
                         +   (CalcType)*pV5L +  2*(CalcType)*(pV5L+chCount) -  2*(CalcType)*(pV5L+3*chCount ) -   (CalcType)*(pV5L+4*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelHorizSecond
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            CalcType sum =     (CalcType)*(pSrcTop - chCount) + 2 * (CalcType)*pSrcTop +     (CalcType)*(pSrcTop + chCount)
                         - 2 * (CalcType)*(pSrcPtr - chCount) - 4 * (CalcType)*pSrcPtr - 2 * (CalcType)*(pSrcPtr + chCount)
                         +     (CalcType)*(pSrcBtm - chCount) + 2 * (CalcType)*pSrcBtm +     (CalcType)*(pSrcBtm + chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelHorizSecond5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop2 = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep );
            const SrcType * pSrcBtm2 = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep );
            CalcType sum =   (CalcType)*(pSrcTop2-2*chCount) + 4*(CalcType)*(pSrcTop2-chCount) +  6*(CalcType)*pSrcTop2 + 4*(CalcType)*(pSrcTop2+chCount) +   (CalcType)*(pSrcTop2+2*chCount)
                         - 2*(CalcType)*(pSrcPtr -2*chCount) - 8*(CalcType)*(pSrcPtr -chCount) - 12*(CalcType)*pSrcPtr  - 8*(CalcType)*(pSrcPtr +chCount) - 2*(CalcType)*(pSrcPtr +2*chCount)
                         +   (CalcType)*(pSrcBtm2-2*chCount) + 4*(CalcType)*(pSrcBtm2-chCount) +  6*(CalcType)*pSrcBtm2 + 4*(CalcType)*(pSrcBtm2+chCount) +   (CalcType)*(pSrcBtm2+2*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelVertSecond
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum =     (CalcType)*pVTL - 2 * (CalcType)*(pVTL + chCount ) +     (CalcType)*(pVTL + 2 * chCount )
                         + 2 * (CalcType)*pVML - 4 * (CalcType)*(pVML + chCount ) + 2 * (CalcType)*(pVML + 2 * chCount )
                         +     (CalcType)*pVBL - 2 * (CalcType)*(pVBL + chCount ) +     (CalcType)*(pVBL + 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelVertSecond5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum =   (CalcType)*pV1L -  2*(CalcType)*(pV1L+2*chCount ) +   (CalcType)*(pV1L+4*chCount)
                         + 4*(CalcType)*pV2L -  8*(CalcType)*(pV2L+2*chCount ) + 4*(CalcType)*(pV2L+4*chCount)
                         + 6*(CalcType)*pV3L - 12*(CalcType)*(pV3L+2*chCount ) + 6*(CalcType)*(pV3L+4*chCount)
                         + 4*(CalcType)*pV4L -  8*(CalcType)*(pV4L+2*chCount ) + 4*(CalcType)*(pV4L+4*chCount)
                         +   (CalcType)*pV5L -  2*(CalcType)*(pV5L+2*chCount ) +   (CalcType)*(pV5L+4*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelCross
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            CalcType sum = - (CalcType)*(pSrcTop - chCount) + (CalcType)*(pSrcTop + chCount)
                           + (CalcType)*(pSrcBtm - chCount) - (CalcType)*(pSrcBtm + chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterSobelCross5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pSrcTop2 = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep );
            const SrcType * pSrcTop  = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep );
            const SrcType * pSrcBtm  = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep );
            const SrcType * pSrcBtm2 = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep );
            CalcType sum = -   (CalcType)*(pSrcTop2-2*chCount) - 2*(CalcType)*(pSrcTop2-chCount) + 2*(CalcType)*(pSrcTop2+chCount) +   (CalcType)*(pSrcTop2+2*chCount)
                           - 2*(CalcType)*(pSrcTop -2*chCount) - 4*(CalcType)*(pSrcTop -chCount) + 4*(CalcType)*(pSrcTop +chCount) + 2*(CalcType)*(pSrcTop +2*chCount)
                           + 2*(CalcType)*(pSrcBtm -2*chCount) + 4*(CalcType)*(pSrcBtm -chCount) - 4*(CalcType)*(pSrcBtm +chCount) - 2*(CalcType)*(pSrcBtm +2*chCount)
                           +   (CalcType)*(pSrcBtm2-2*chCount) + 2*(CalcType)*(pSrcBtm2-chCount) - 2*(CalcType)*(pSrcBtm2+chCount) -   (CalcType)*(pSrcBtm2+2*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterRobertsDown
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * p2ndVal = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType));
            CalcType sum = (CalcType)*pSrcPtr - (CalcType)*p2ndVal;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterRobertsUp
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * p2ndVal = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep + chCount * sizeof(SrcType));
            CalcType sum = (CalcType)*pSrcPtr - (CalcType)*p2ndVal;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterLfwace
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum = - (CalcType)*pVTL -     (CalcType)*(pVTL + chCount ) - (CalcType)*(pVTL + 2 * chCount )
                           - (CalcType)*pVML + 8 * (CalcType)*(pVML + chCount ) - (CalcType)*(pVML + 2 * chCount )
                           - (CalcType)*pVBL -     (CalcType)*(pVBL + chCount ) - (CalcType)*(pVBL + 2 * chCount );
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterLfwace5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum = -   (CalcType)*pV1L - 3*(CalcType)*(pV1L+chCount ) -  4*(CalcType)*(pV1L+2*chCount ) - 3*(CalcType)*(pV1L+3*chCount) -   (CalcType)*(pV1L+4*chCount)
                           - 3*(CalcType)*pV2L + 0*(CalcType)*(pV2L+chCount ) +  6*(CalcType)*(pV2L+2*chCount ) + 0*(CalcType)*(pV2L+3*chCount) - 3*(CalcType)*(pV2L+4*chCount)
                           - 4*(CalcType)*pV3L + 6*(CalcType)*(pV3L+chCount ) + 20*(CalcType)*(pV3L+2*chCount ) + 6*(CalcType)*(pV3L+3*chCount) - 4*(CalcType)*(pV3L+4*chCount)
                           - 3*(CalcType)*pV4L + 0*(CalcType)*(pV4L+chCount ) +  6*(CalcType)*(pV4L+2*chCount ) + 0*(CalcType)*(pV4L+3*chCount) - 3*(CalcType)*(pV4L+4*chCount)
                           -   (CalcType)*pV5L - 3*(CalcType)*(pV5L+chCount ) -  4*(CalcType)*(pV5L+2*chCount ) - 3*(CalcType)*(pV5L+3*chCount) -   (CalcType)*(pV5L+4*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterGauss
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum =     (CalcType)*pVTL + 2 * (CalcType)*(pVTL + chCount ) +     (CalcType)*(pVTL + 2 * chCount )
                         + 2 * (CalcType)*pVML + 4 * (CalcType)*(pVML + chCount ) + 2 * (CalcType)*(pVML + 2 * chCount )
                         +     (CalcType)*pVBL + 2 * (CalcType)*(pVBL + chCount ) +     (CalcType)*(pVBL + 2 * chCount );

            sum = sum / 16;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterGauss5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum =  2*(CalcType)*pV1L -  7*(CalcType)*(pV1L+chCount ) -  12*(CalcType)*(pV1L+2*chCount ) -  7*(CalcType)*(pV1L+3*chCount) -  2*(CalcType)*(pV1L+4*chCount)
                         +  7*(CalcType)*pV2L + 31*(CalcType)*(pV2L+chCount ) +  52*(CalcType)*(pV2L+2*chCount ) + 31*(CalcType)*(pV2L+3*chCount) -  7*(CalcType)*(pV2L+4*chCount)
                         + 12*(CalcType)*pV3L + 52*(CalcType)*(pV3L+chCount ) + 127*(CalcType)*(pV3L+2*chCount ) + 52*(CalcType)*(pV3L+3*chCount) - 12*(CalcType)*(pV3L+4*chCount)
                         +  7*(CalcType)*pV4L + 31*(CalcType)*(pV4L+chCount ) +  52*(CalcType)*(pV4L+2*chCount ) + 31*(CalcType)*(pV4L+3*chCount) -  7*(CalcType)*(pV4L+4*chCount)
                         +  2*(CalcType)*pV5L -  7*(CalcType)*(pV5L+chCount ) -  12*(CalcType)*(pV5L+2*chCount ) -  7*(CalcType)*(pV5L+3*chCount) -  2*(CalcType)*(pV5L+4*chCount);
            sum = sum / 571;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterHipass
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum = - (CalcType)*pVTL -     (CalcType)*(pVTL + chCount ) - (CalcType)*(pVTL + 2 * chCount )
                           - (CalcType)*pVML + 8 * (CalcType)*(pVML + chCount ) - (CalcType)*(pVML + 2 * chCount )
                           - (CalcType)*pVBL -     (CalcType)*(pVBL + chCount ) - (CalcType)*(pVBL + 2 * chCount );

            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterHipass5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum = - (CalcType)*pV1L - (CalcType)*(pV1L+chCount ) -      (CalcType)*(pV1L+2*chCount ) - (CalcType)*(pV1L+3*chCount) - (CalcType)*(pV1L+4*chCount)
                           - (CalcType)*pV2L - (CalcType)*(pV2L+chCount ) -      (CalcType)*(pV2L+2*chCount ) - (CalcType)*(pV2L+3*chCount) - (CalcType)*(pV2L+4*chCount)
                           - (CalcType)*pV3L - (CalcType)*(pV3L+chCount ) + 24 * (CalcType)*(pV3L+2*chCount  ) - (CalcType)*(pV3L+3*chCount) - (CalcType)*(pV3L+4*chCount)
                           - (CalcType)*pV4L - (CalcType)*(pV4L+chCount ) -      (CalcType)*(pV4L+2*chCount ) - (CalcType)*(pV4L+3*chCount) - (CalcType)*(pV4L+4*chCount)
                           - (CalcType)*pV5L - (CalcType)*(pV5L+chCount ) -      (CalcType)*(pV5L+2*chCount ) - (CalcType)*(pV5L+3*chCount) - (CalcType)*(pV5L+4*chCount);
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterLowpass
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pVTL = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep - chCount * sizeof(SrcType) );
            const SrcType * pVML = (const SrcType *)(((Fw8u*)pSrcPtr)           - chCount * sizeof(SrcType) );
            const SrcType * pVBL = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep - chCount * sizeof(SrcType) );
            CalcType sum = (CalcType)*pVTL + (CalcType)*(pVTL + chCount ) + (CalcType)*(pVTL + 2 * chCount )
                         + (CalcType)*pVML + (CalcType)*(pVML + chCount ) + (CalcType)*(pVML + 2 * chCount )
                         + (CalcType)*pVBL + (CalcType)*(pVBL + chCount ) + (CalcType)*(pVBL + 2 * chCount );

            sum = sum / 9;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterLowpass5x5
    {
        static inline void Calculate(   const SrcType * pSrcPtr, int srcStep,
                                        DstType * pDst, int /*dstStep*/ )
        {
            const SrcType * pV1L = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV2L = (const SrcType *)(((Fw8u*)pSrcPtr) -     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV3L = (const SrcType *)(((Fw8u*)pSrcPtr)               - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV4L = (const SrcType *)(((Fw8u*)pSrcPtr) +     srcStep - 2 * chCount * sizeof(SrcType) );
            const SrcType * pV5L = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep - 2 * chCount * sizeof(SrcType) );

            CalcType sum = (CalcType)*pV1L + (CalcType)*(pV1L+chCount ) + (CalcType)*(pV1L+2*chCount ) + (CalcType)*(pV1L+3*chCount) + (CalcType)*(pV1L+4*chCount)
                         + (CalcType)*pV2L + (CalcType)*(pV2L+chCount ) + (CalcType)*(pV2L+2*chCount ) + (CalcType)*(pV2L+3*chCount) + (CalcType)*(pV2L+4*chCount)
                         + (CalcType)*pV3L + (CalcType)*(pV3L+chCount ) + (CalcType)*(pV3L+2*chCount ) + (CalcType)*(pV3L+3*chCount) + (CalcType)*(pV3L+4*chCount)
                         + (CalcType)*pV4L + (CalcType)*(pV4L+chCount ) + (CalcType)*(pV4L+2*chCount ) + (CalcType)*(pV4L+3*chCount) + (CalcType)*(pV4L+4*chCount)
                         + (CalcType)*pV5L + (CalcType)*(pV5L+chCount ) + (CalcType)*(pV5L+2*chCount ) + (CalcType)*(pV5L+3*chCount) + (CalcType)*(pV5L+4*chCount);

            sum = sum / 25;
            *pDst = FW_REF::Limits<DstType>::Sat( sum );
        }
    };
};

} // namespace OPT_LEVEL

#endif // FILTERS_FIXED
