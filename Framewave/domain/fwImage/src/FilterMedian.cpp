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

#define ABSOLUTEVALUE(x) (((x)<0)?(-(x)):(x))

namespace OPT_LEVEL
{
namespace FiltersMedian
{
    template< typename CalcType >
    struct FilterMedianBase
    {
        static inline void OnePassSort( CalcType *pSortedList, int nListLength, int nStartIndex )
        {
            int nIndex = nStartIndex;
            while( nIndex > 0 && pSortedList[nIndex] < pSortedList[nIndex - 1] )
            {
                CalcType temp = pSortedList[nIndex - 1];
                pSortedList[nIndex - 1] = pSortedList[nIndex];
                pSortedList[nIndex] = temp;
                nIndex--;
            }
            while( nIndex < (nListLength-1) && pSortedList[nIndex] > pSortedList[nIndex + 1] )
            {
                CalcType temp = pSortedList[nIndex + 1];
                pSortedList[nIndex + 1] = pSortedList[nIndex];
                pSortedList[nIndex] = temp;
                nIndex++;
            }
        }

        static inline int BinarySearch( CalcType *pSortedList, int nListLength, CalcType element )
        {
            int startIndex = 0;
            int endIndex = nListLength - 1;
            int midIndex = ( startIndex + endIndex ) / 2;
            while( pSortedList[midIndex] != element )
            {
                if( element < pSortedList[midIndex] )
                {
                    // search to the left
                    endIndex = midIndex - 1;
                    ASSERT( endIndex >= 0 );
                    ASSERT( startIndex <= endIndex );
                    midIndex = ( startIndex + endIndex ) / 2;
                }
                else
                {
                    startIndex = midIndex + 1;
                    ASSERT( startIndex < nListLength );
                    ASSERT( startIndex <= endIndex );
                    midIndex = ( startIndex + endIndex ) / 2;
                }
            }
            ASSERT( pSortedList[midIndex] == element );
            return midIndex;
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMedian : public FilterMedianBase< CalcType >
    {
        static inline int GetMinimumBufferSize( const FwiSize & maskSize )
        {
            return maskSize.width * maskSize.height * chCount * sizeof( CalcType );
        }

        static inline void Calculate(  const SrcType * pSrc, int srcStep,
                                             DstType * pDst, int /*dstStep*/,
                                       const FwiSize & maskSize, 
                                       const FwiPoint & /*anchor*/,
                                       void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            SrcType * pEnd = (SrcType*)(((Fw8u*)(pSrc)) + maskSize.height * srcStep);
            int nListLength[chCount] = {0};
            while( pSrc < pEnd )
            {
                const SrcType *pElement = pSrc;
                const SrcType *LastElement = pSrc + maskSize.width * chCount;
                int channel = 0;
                while( pElement < LastElement )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength[channel]) = *pElement;
                    nListLength[channel]++;
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength[channel], nListLength[channel] - 1 );
                    pElement++;
                    channel = ( channel + 1 ) % chCount;
                }
                pSrc = (SrcType*)(((Fw8u*)(pSrc)) + srcStep);
            }

            for( int ch = 0; ch < chCount; ch++ )
            {
                pDst[ch] = (DstType)*(pSortedList + nSortedListLength * ch + middleIndex);
            }
        }

        static inline void CalculateRight(  const SrcType * pSrc, int srcStep,
                                                  DstType * pDst, int /*dstStep*/,
                                            const FwiSize & maskSize, 
                                            const FwiPoint & /*anchor*/,
                                            void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            SrcType * pEnd = (SrcType*)(((Fw8u*)(pSrc)) + maskSize.height * srcStep);
            while( pSrc < pEnd )
            {
                const SrcType *pOldElement = pSrc;
                const SrcType *pNewElement = pSrc + maskSize.width * chCount;
                const SrcType *LastElement = pOldElement + chCount;
                int channel = 0;
                while( pOldElement < LastElement )
                {
                    int nOldIndex = BinarySearch( pSortedList + nSortedListLength * channel, nSortedListLength, *pOldElement );
                    *(pSortedList + nSortedListLength * channel + nOldIndex) = *pNewElement;
                    OnePassSort( pSortedList + nSortedListLength * channel, nSortedListLength, nOldIndex );

                    channel = ( channel + 1 ) % chCount;
                    pOldElement++;
                    pNewElement++;
                }
                pSrc = (SrcType*)(((Fw8u*)pSrc) + srcStep);
            } // while

            for( int channel = 0; channel < chCount; channel++ )
            {
                *pDst = (DstType)*(pSortedList + nSortedListLength * channel + middleIndex);
                pDst++;
            }
        }

        static inline void CalculateLeft(  const SrcType * pSrc, int srcStep,
                                                 DstType * pDst, int /*dstStep*/,
                                           const FwiSize & maskSize, 
                                           const FwiPoint & /*anchor*/,
                                           void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            SrcType * pEnd = (SrcType*)(((Fw8u*)(pSrc)) + maskSize.height * srcStep);
            while( pSrc < pEnd )
            {
                const SrcType *pOldElement = pSrc + (maskSize.width - 1) * chCount;
                const SrcType *pNewElement = pSrc - chCount;
                const SrcType *LastElement = pOldElement + chCount;
                int channel = 0;
                while( pOldElement < LastElement )
                {
                    int nOldIndex = BinarySearch( pSortedList + nSortedListLength * channel, nSortedListLength, *pOldElement );
                    *(pSortedList + nSortedListLength * channel + nOldIndex) = *pNewElement;
                    OnePassSort( pSortedList + nSortedListLength * channel, nSortedListLength, nOldIndex );

                    channel = ( channel + 1 ) % chCount;
                    pOldElement++;
                    pNewElement++;
                }
                pSrc = (SrcType*)(((Fw8u*)pSrc) + srcStep);
            } // while

            for( int channel = 0; channel < chCount; channel++ )
            {
                *pDst = (DstType)*(pSortedList + nSortedListLength * channel + middleIndex);
                pDst++;
            }
        }

        static inline void CalculateDown(  const SrcType * pSrc, int srcStep,
                                                 DstType * pDst, int /*dstStep*/,
                                           const FwiSize & maskSize, 
                                           const FwiPoint & /*anchor*/,
                                           void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) + maskSize.height * srcStep);
            const SrcType *LastElement = pOldElement + maskSize.width * chCount;
            int channel = 0;
            while( pOldElement < LastElement )
            {
                int nOldIndex = BinarySearch( pSortedList + nSortedListLength * channel, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * channel + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * channel, nSortedListLength, nOldIndex );

                channel = ( channel + 1 ) % chCount;
                pOldElement++;
                pNewElement++;
            }

            for( int channel = 0; channel < chCount; channel++ )
            {
                *pDst = (DstType)*(pSortedList + nSortedListLength * channel + middleIndex);
                pDst++;
            }
        }

        static inline void CalculateUp(  const SrcType * pSrc, int srcStep,
                                               DstType * pDst, int /*dstStep*/,
                                         const FwiSize & maskSize, 
                                         const FwiPoint & /*anchor*/,
                                         void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            const SrcType *pOldElement = (const SrcType *)(((Fw8u*)pSrc) + (maskSize.height-1) * srcStep);
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) - srcStep);
            const SrcType *LastElement = pOldElement + maskSize.width * chCount;
            int channel = 0;
            while( pOldElement < LastElement )
            {
                int nOldIndex = BinarySearch( pSortedList + nSortedListLength * channel, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * channel + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * channel, nSortedListLength, nOldIndex );

                channel = ( channel + 1 ) % chCount;
                pOldElement++;
                pNewElement++;
            }

            for( int channel = 0; channel < chCount; channel++ )
            {
                *pDst = (DstType)*(pSortedList + nSortedListLength * channel + middleIndex);
                pDst++;
            }
        }
    };

    template< typename SrcType, typename DstType, typename CalcType >
    struct FilterMedian< AC4, SrcType, DstType, CalcType > 
        : public FilterMedianBase< CalcType >
    {
        static inline int GetMinimumBufferSize( const FwiSize & maskSize )
        {
            return maskSize.width * maskSize.height * C4 * sizeof( CalcType );
        }

        static inline void Calculate(  const SrcType * pSrc, int srcStep,
                                             DstType * pDst, int /*dstStep*/,
                                       const FwiSize & maskSize, 
                                       const FwiPoint & /*anchor*/,
                                       void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            SrcType * pEnd = (SrcType*)(((Fw8u*)(pSrc)) + maskSize.height * srcStep);
            int nListLength = 0;
            while( pSrc < pEnd )
            {
                const SrcType *pElement = pSrc;
                const SrcType *LastElement = pSrc + maskSize.width * C4;
                while( pElement < LastElement )
                {
                    *(pSortedList + nSortedListLength * 0 + nListLength) = *pElement;
                    *(pSortedList + nSortedListLength * 1 + nListLength) = *(pElement+1);
                    *(pSortedList + nSortedListLength * 2 + nListLength) = *(pElement+2);
                    nListLength++;
                    OnePassSort( pSortedList + nSortedListLength * 0, nListLength, nListLength - 1 );
                    OnePassSort( pSortedList + nSortedListLength * 1, nListLength, nListLength - 1 );
                    OnePassSort( pSortedList + nSortedListLength * 2, nListLength, nListLength - 1 );
                    pElement += C4;
                }
                pSrc = (SrcType*)(((Fw8u*)(pSrc)) + srcStep);
            }

            pDst[0] = (DstType)*(pSortedList + nSortedListLength * 0 + middleIndex);
            pDst[1] = (DstType)*(pSortedList + nSortedListLength * 1 + middleIndex);
            pDst[2] = (DstType)*(pSortedList + nSortedListLength * 2 + middleIndex);
        }

        static inline void CalculateRight(  const SrcType * pSrc, int srcStep,
                                                  DstType * pDst, int /*dstStep*/,
                                            const FwiSize & maskSize, 
                                            const FwiPoint & /*anchor*/,
                                            void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            SrcType * pEnd = (SrcType*)(((Fw8u*)(pSrc)) + maskSize.height * srcStep);
            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = pSrc + maskSize.width * C4;
            while( pOldElement < pEnd )
            {
                int nOldIndex;

                // red channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 0, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * 0 + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * 0, nSortedListLength, nOldIndex );

                // green channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 1, nSortedListLength, *(pOldElement+1) );
                *(pSortedList + nSortedListLength * 1 + nOldIndex) = *(pNewElement+1);
                OnePassSort( pSortedList + nSortedListLength * 1, nSortedListLength, nOldIndex );

                // blue channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 2, nSortedListLength, *(pOldElement+2) );
                *(pSortedList + nSortedListLength * 2 + nOldIndex) = *(pNewElement+2);
                OnePassSort( pSortedList + nSortedListLength * 2, nSortedListLength, nOldIndex );

                pOldElement = (SrcType*)(((Fw8u*)pOldElement) + srcStep);
                pNewElement = (SrcType*)(((Fw8u*)pNewElement) + srcStep);
            } // while

            *(pDst+0) = (DstType)*(pSortedList + nSortedListLength * 0 + middleIndex);
            *(pDst+1) = (DstType)*(pSortedList + nSortedListLength * 1 + middleIndex);
            *(pDst+2) = (DstType)*(pSortedList + nSortedListLength * 2 + middleIndex);
        }

        static inline void CalculateLeft(  const SrcType * pSrc, int srcStep,
                                                 DstType * pDst, int /*dstStep*/,
                                           const FwiSize & maskSize, 
                                           const FwiPoint & /*anchor*/,
                                           void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;
            int nOldIndex;

            const SrcType *pOldElement = pSrc + (maskSize.width - 1) * C4;
            const SrcType * pEnd = (const SrcType*)(((Fw8u*)(pOldElement)) + maskSize.height * srcStep);
            const SrcType *pNewElement = pSrc - C4;
            while( pOldElement < pEnd )
            {
                // red channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 0, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * 0 + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * 0, nSortedListLength, nOldIndex );

                // green channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 1, nSortedListLength, *(pOldElement+1) );
                *(pSortedList + nSortedListLength * 1 + nOldIndex) = *(pNewElement+1);
                OnePassSort( pSortedList + nSortedListLength * 1, nSortedListLength, nOldIndex );

                // blue channel
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 2, nSortedListLength, *(pOldElement+2) );
                *(pSortedList + nSortedListLength * 2 + nOldIndex) = *(pNewElement+2);
                OnePassSort( pSortedList + nSortedListLength * 2, nSortedListLength, nOldIndex );

                pOldElement = (SrcType*)(((Fw8u*)pOldElement) + srcStep);
                pNewElement = (SrcType*)(((Fw8u*)pNewElement) + srcStep);
            } // while

            *(pDst+0) = (DstType)*(pSortedList + nSortedListLength * 0 + middleIndex);
            *(pDst+1) = (DstType)*(pSortedList + nSortedListLength * 1 + middleIndex);
            *(pDst+2) = (DstType)*(pSortedList + nSortedListLength * 2 + middleIndex);
        }

        static inline void CalculateDown(  const SrcType * pSrc, int srcStep,
                                                 DstType * pDst, int /*dstStep*/,
                                           const FwiSize & maskSize, 
                                           const FwiPoint & /*anchor*/,
                                           void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            const SrcType *pOldElement = pSrc;
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) + maskSize.height * srcStep);
            const SrcType *LastElement = pOldElement + maskSize.width * C4;
            int nOldIndex;
            while( pOldElement < LastElement )
            {
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 0, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * 0 + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * 0, nSortedListLength, nOldIndex );

                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 1, nSortedListLength, *(pOldElement+1) );
                *(pSortedList + nSortedListLength * 1 + nOldIndex) = *(pNewElement+1);
                OnePassSort( pSortedList + nSortedListLength * 1, nSortedListLength, nOldIndex );

                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 2, nSortedListLength, *(pOldElement+2) );
                *(pSortedList + nSortedListLength * 2 + nOldIndex) = *(pNewElement+2);
                OnePassSort( pSortedList + nSortedListLength * 2, nSortedListLength, nOldIndex );

                pOldElement+=4;
                pNewElement+=4;
            }

            *(pDst+0) = (DstType)*(pSortedList + nSortedListLength * 0 + middleIndex);
            *(pDst+1) = (DstType)*(pSortedList + nSortedListLength * 1 + middleIndex);
            *(pDst+2) = (DstType)*(pSortedList + nSortedListLength * 2 + middleIndex);
        }

        static inline void CalculateUp(  const SrcType * pSrc, int srcStep,
                                               DstType * pDst, int /*dstStep*/,
                                         const FwiSize & maskSize, 
                                         const FwiPoint & /*anchor*/,
                                         void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width * maskSize.height;
            const int middleIndex = maskSize.height * maskSize.width / 2;

            const SrcType *pOldElement = (const SrcType *)(((Fw8u*)pSrc) + (maskSize.height-1) * srcStep);
            const SrcType *pNewElement = (const SrcType *)(((Fw8u*)pSrc) - srcStep);
            const SrcType *LastElement = pOldElement + maskSize.width * C4;
            int nOldIndex;
            while( pOldElement < LastElement )
            {
                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 0, nSortedListLength, *pOldElement );
                *(pSortedList + nSortedListLength * 0 + nOldIndex) = *pNewElement;
                OnePassSort( pSortedList + nSortedListLength * 0, nSortedListLength, nOldIndex );

                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 1, nSortedListLength, *(pOldElement+1) );
                *(pSortedList + nSortedListLength * 1 + nOldIndex) = *(pNewElement+1);
                OnePassSort( pSortedList + nSortedListLength * 1, nSortedListLength, nOldIndex );

                nOldIndex = BinarySearch( pSortedList + nSortedListLength * 2, nSortedListLength, *(pOldElement+2) );
                *(pSortedList + nSortedListLength * 2 + nOldIndex) = *(pNewElement+2);
                OnePassSort( pSortedList + nSortedListLength * 2, nSortedListLength, nOldIndex );

                pOldElement+=4;
                pNewElement+=4;
            }

            *(pDst+0) = (DstType)*(pSortedList + nSortedListLength * 0 + middleIndex);
            *(pDst+1) = (DstType)*(pSortedList + nSortedListLength * 1 + middleIndex);
            *(pDst+2) = (DstType)*(pSortedList + nSortedListLength * 2 + middleIndex);
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMedianCross : public FilterMedianBase< CalcType >
    {
        static inline int GetMinimumBufferSize( const FwiSize & maskSize )
        {
            return ( maskSize.width + maskSize.height - 1 ) * chCount * sizeof( CalcType );
        }

        static inline void CalculateAllChannels(  const SrcType * pSrc, int srcStep,
                                                  DstType * pDst, int /*dstStep*/,
                                                  const FwiSize & maskSize, 
                                                  const FwiPoint & anchor,
                                                  void* pTempBuffer )
        {
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width + maskSize.height - 1;
            const int middleIndex = nSortedListLength / 2;

            const SrcType * pElement = (SrcType*)(((Fw8u*)(pSrc)) - anchor.y * srcStep);
            const SrcType * pEnd = (const SrcType*)(((Fw8u*)(pElement)) + maskSize.height * srcStep);
            int nListLength = 0;
            while( pElement < pEnd )
            {
                for( int channel = 0; channel < chCount; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement = (SrcType*)(((Fw8u*)(pElement)) + srcStep);
            }

            pElement = pSrc - anchor.y * chCount;
            pEnd = pElement + maskSize.width * chCount;
            while( pElement < pSrc )
            {
                for( int channel = 0; channel < chCount; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement += chCount;
            }

            pElement = pSrc + chCount;
            while( pElement < pEnd )
            {
                for( int channel = 0; channel < chCount; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement += chCount;
            }

            for( int ch = 0; ch < chCount; ch++ )
            {
                pDst[ch] = (DstType)*(pSortedList + nSortedListLength * ch + middleIndex);
            }
        }
    };

    template< typename SrcType, typename DstType, typename CalcType >
    struct FilterMedianCross< AC4, SrcType, DstType, CalcType > : public FilterMedianBase< CalcType >
    {
        static inline int GetMinimumBufferSize( const FwiSize & maskSize )
        {
            return ( maskSize.width + maskSize.height - 1 ) * C3 * sizeof( CalcType );
        }

        static inline void CalculateAllChannels(  const SrcType * pSrc, int srcStep,
                                                  DstType * pDst, int /*dstStep*/,
                                                  const FwiSize & maskSize, 
                                                  const FwiPoint & anchor,
                                                  void* pTempBuffer )
        {
            int chCount = 4;
            CalcType *pSortedList = (CalcType*)pTempBuffer;
            const int nSortedListLength = maskSize.width + maskSize.height - 1;
            const int middleIndex = nSortedListLength / 2;

            const SrcType * pElement = (SrcType*)(((Fw8u*)(pSrc)) - anchor.y * srcStep);
            const SrcType * pEnd = (const SrcType*)(((Fw8u*)(pElement)) + maskSize.height * srcStep);
            int nListLength = 0;
            while( pElement < pEnd )
            {
                for( int channel = 0; channel < C3; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement = (SrcType*)(((Fw8u*)(pElement)) + srcStep);
            }

            pElement = pSrc - anchor.y * chCount;
            pEnd = pElement + maskSize.width * chCount;
            while( pElement < pSrc )
            {
                for( int channel = 0; channel < C3; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement += chCount;
            }

            pElement = pSrc + chCount;
            while( pElement < pEnd )
            {
                for( int channel = 0; channel < C3; channel++ )
                {
                    *(pSortedList + nSortedListLength * channel + nListLength) = *(pElement + channel);
                    OnePassSort( pSortedList + nSortedListLength * channel, nListLength + 1, nListLength );
                }
                nListLength++;
                pElement += chCount;
            }

            for( int ch = 0; ch < C3; ch++ )
            {
                pDst[ch] = (DstType)*(pSortedList + nSortedListLength * ch + middleIndex);
            }
        }
    };

#if BUILD_NUM_AT_LEAST( 100 )

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMedianColor3x3
    {
        static inline int GetMinimumBufferSize( const FwiSize & maskSize )
        {
            return maskSize.width * maskSize.height * chCount * sizeof( CalcType );
        }

        static inline CalcType Distance( const SrcType * pPixel1, const SrcType *pPixel2 )
        {
            CalcType dR = (CalcType)ABSOLUTEVALUE((CalcType)*pPixel1 - (CalcType)*pPixel2);
            CalcType dG = (CalcType)ABSOLUTEVALUE((CalcType)*(pPixel1+1) - (CalcType)*(pPixel2+1));
            CalcType dB = (CalcType)ABSOLUTEVALUE((CalcType)*(pPixel1+2) - (CalcType)*(pPixel2+2));
            return dR + dG + dB;
        }

        static inline const SrcType * FindMinimum3x3( const SrcType * pSrcPtr, int srcStep, CalcType & minDist )
        {
            CalcType tempDist; 
            const SrcType * pSrcAbove = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep);
            const SrcType * pSrcBelow = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep);
            const SrcType * pMinPtr = pSrcPtr - chCount;
            minDist = Distance( pSrcPtr, pMinPtr );

            tempDist = Distance( pSrcPtr, pSrcPtr + chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcPtr + chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove - chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove - chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove + chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove + chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow - chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow - chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow + chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow + chCount;
                minDist = tempDist;
            }
            return pMinPtr;
        }

        static inline void CalculateAllChannels(   const SrcType * pSrcPtr, int srcStep,
                                                   DstType * pDst, int /*dstStep*/,
                                                   const FwiSize & /*maskSize*/, 
                                                   const FwiPoint & /*anchor*/ )
        {
            CalcType minDist;
            const SrcType * pMinPtr = FindMinimum3x3( pSrcPtr, srcStep, minDist );
            *(pDst+0) = *(pMinPtr+0);
            *(pDst+1) = *(pMinPtr+1);
            *(pDst+2) = *(pMinPtr+2);
        }
    };

    template< CH chCount, typename SrcType, typename DstType, typename CalcType >
    struct FilterMedianColor5x5 : public FilterMedianColor3x3< chCount, SrcType, DstType, CalcType >
    {
        static inline const SrcType * FindMinimum5x5( const SrcType * pSrcPtr, int srcStep, CalcType & minDist )
        {
            CalcType tempDist; 
            const SrcType * pSrcAbove2 = (const SrcType *)(((Fw8u*)pSrcPtr) - 2 * srcStep);
            const SrcType * pSrcAbove = (const SrcType *)(((Fw8u*)pSrcPtr) - srcStep);
            const SrcType * pSrcBelow = (const SrcType *)(((Fw8u*)pSrcPtr) + srcStep);
            const SrcType * pSrcBelow2 = (const SrcType *)(((Fw8u*)pSrcPtr) + 2 * srcStep);
            
            const SrcType * pMinPtr = FindMinimum3x3( pSrcPtr, srcStep, minDist );

            tempDist = Distance( pSrcPtr, pSrcPtr - chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcPtr - chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcPtr + chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcPtr + chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove - chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove - chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove2 - chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove2 - chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove2 - chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove2 - chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove2 + chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove2 + chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove2 + chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove2 + chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcAbove + chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcAbove + chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow - chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow - chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow2 - chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow2 - chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow2 - chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow2 - chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow2 + chCount );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow2 + chCount;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow2 + chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow2 + chCount * 2;
                minDist = tempDist;
            }
            tempDist = Distance( pSrcPtr, pSrcBelow + chCount * 2 );
            if( tempDist < minDist )
            {
                pMinPtr = pSrcBelow + chCount * 2;
                minDist = tempDist;
            }
            return pMinPtr;
        }
        static inline void CalculateAllChannels(   const SrcType * pSrcPtr, int srcStep,
                                                   DstType * pDst, int /*dstStep*/,
                                                   const FwiSize & /*maskSize*/, 
                                                   const FwiPoint & /*anchor*/ )
        {
            CalcType minDist;
            const SrcType * pMinPtr = FindMinimum5x5( pSrcPtr, srcStep, minDist );
            *(pDst+0) = *(pMinPtr+0);
            *(pDst+1) = *(pMinPtr+1);
            *(pDst+2) = *(pMinPtr+2);
        }
    };

#endif // BUILD_NUM_AT_LEAST( 100 )

} // namespace FiltersMedian
} // namespace OPT_LEVEL

using namespace OPT_LEVEL;


FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s,
        Fw16s,
        Fw16s,
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedian_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiSize maskSize, FwiPoint anchor )
{
    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianHoriz_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize3x1:
        realMaskSize.width = 3;
        realMaskSize.height = 1;
        anchor.x = 1;
        anchor.y = 0;
        break;
    case fwMskSize5x1:
        realMaskSize.width = 5;
        realMaskSize.height = 1;
        anchor.x = 2;
        anchor.y = 0;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagRDLD,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C1R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C3R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_C4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                               Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C1R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C3R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_C4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianVert_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                               Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                               FwiMaskSize maskSize )
{
    FwiPoint anchor;
    FwiSize realMaskSize;

    switch( maskSize )
    {
    case fwMskSize1x3:
        realMaskSize.width = 1;
        realMaskSize.height = 3;
        anchor.x = 0;
        anchor.y = 1;
        break;
    case fwMskSize1x5:
        realMaskSize.width = 1;
        realMaskSize.height = 5;
        anchor.x = 0;
        anchor.y = 2;
        break;

    default:
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s, 
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessor_ZigZagDRUR,
        FiltersMedian::FilterMedian, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_C1R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw8u, 
        Fw8u, 
        Fw8u, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_C1R)( const Fw16s * pSrc, int srcStep,
                                        Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C1,
        Fw16s,
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                        Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        C3,
        Fw16s,
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianCross_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                        Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    FwiSize realMaskSize;
    FwiPoint anchor;

    if( maskSize == fwMskSize3x3 )
    {
        realMaskSize.height = 3;
        realMaskSize.width = 3;
        anchor.x = 1;
        anchor.y = 1;
    }
    else if( maskSize == fwMskSize5x5 )
    {
        realMaskSize.height = 5;
        realMaskSize.width = 5;
        anchor.x = 2;
        anchor.y = 2;
    }
    else
    {
        return fwStsMaskSizeErr;
    }

    return FilterCommon::DivideAndConquerWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
        AC4,
        Fw16s,
        Fw16s, 
        Fw16s, 
        FilterCommon::TProcessorAllChannelsWithBuffer_pSrc_sStep_pDst_dStep_roi_mask_anchor,
        FiltersMedian::FilterMedianCross, FilterCommon::DataWithBuffer >
    ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
}

#endif // BUILD_NUM_AT_LEAST

#if BUILD_NUM_AT_LEAST( 100 )

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_8u_C3R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_8u_AC4R)( const Fw8u * pSrc, int srcStep,
                                        Fw8u * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw8u, 
            Fw8u, 
            Fw16s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_16s_C3R)( const Fw16s * pSrc, int srcStep,
                                        Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_16s_AC4R)( const Fw16s * pSrc, int srcStep,
                                        Fw16s * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw16s, 
            Fw16s, 
            Fw32s, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_32f_C3R)( const Fw32f * pSrc, int srcStep,
                                        Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            C3,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterMedianColor_32f_AC4R)( const Fw32f * pSrc, int srcStep,
                                        Fw32f * pDst, int dstStep, FwiSize dstRoiSize,
                                        FwiMaskSize maskSize )
{
    if( maskSize == fwMskSize3x3 )
    {
        FwiSize realMaskSize = { 3, 3 };
        FwiPoint anchor = { 1, 1 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor3x3, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else if( maskSize == fwMskSize5x5 )
    {
        FwiSize realMaskSize = { 5, 5 };
        FwiPoint anchor = { 2, 2 };
        return FilterCommon::DivideAndConquer_pSrc_sStep_pDst_dStep_roi_mask_anchor< 
            AC4,
            Fw32f, 
            Fw32f, 
            Fw32f, 
            FilterCommon::TProcessorAllChannels_pSrc_sStep_pDst_dStep_roi_mask_anchor,
            FiltersMedian::FilterMedianColor5x5, FilterCommon::DataWithMask >
        ::Run( pSrc, srcStep, pDst, dstStep, dstRoiSize, realMaskSize, anchor );
    }
    else
    {
        return fwStsMaskSizeErr;
    }
}

#endif // BUILD_NUM_AT_LEAST( 100 )

//#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
