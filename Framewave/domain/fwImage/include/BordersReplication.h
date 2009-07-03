/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __BORDERREPLICATION_H__
#define __BORDERREPLICATION_H__

namespace OPT_LEVEL
{

namespace BordersReplication
{
    // this class implements a temporary buffer object that contains parts of the source image data,
    // and automatically replicates borders so that the data can be used with functions that assume
    // existnace of borders around ROI source data
    template< CH chCount, typename SrcType >
    class BorderBuffer
    {
        const SrcType *pSrc;
        int srcStep;
        int srcHeight;
        int srcOffset;
        FwiSize dstRoiSize;
        FwiSize maskSize;
        FwiPoint anchor;

        int nSrcLines;
        int nWidth;
        int nTotalWidth;
        int nAlignedWidth;
        int nAlignedWidthInBytes;
        int nColumnsBefore;
        int nColumnsAfter;
        int nLinesBefore;
        int nLinesAfter;
        int nRightBorderStart;
        int nLastSrcColumn;
        int nSrcLineLength;
        SrcType *pBuffer;
        SrcType *pOverlappedData;
        int nOffset;
        int nSrcLine;

    public:

        // default constructor
        BorderBuffer()
        {
            pSrc = NULL;
            srcStep = 0;
            srcHeight = 0;
            srcOffset = 0;
            nSrcLines = 0;
            nWidth = 0;
            nTotalWidth = 0;
            nAlignedWidth = 0;
            nAlignedWidthInBytes = 0;
            nColumnsBefore = 0;
            nColumnsAfter = 0;
            nLinesBefore = 0;
            nLinesAfter = 0;
            nRightBorderStart = 0;
            nLastSrcColumn = 0;
            nSrcLineLength = 0;
            pBuffer = NULL;
            nOffset = 0;
            nSrcLine = 0;
            pOverlappedData = NULL;
        }

        // copy constructor
        BorderBuffer( const BorderBuffer & other )
        {
            if( &other != this )
                Duplicate( other );
        }

        // specialized constructor
        BorderBuffer( const SrcType * pSrc, int srcStep, int srcHeight, int srcOffset,
                      FwiSize & dstRoiSize,
                      FwiSize & maskSize, 
                      FwiPoint & anchor,
                      Fw8u *pBuffer )
        {
            Initialize( pSrc, srcStep, srcHeight, srcOffset, dstRoiSize, maskSize, anchor, pBuffer );
            InitializeBuffer();
        }

        // assignment opertator
        BorderBuffer& operator=( const BorderBuffer& other )
        {
            Duplicate( other );
            return *this;
        }

        // implements the assignment oprator and the copy constructor
        void Duplicate( const BorderBuffer & other )
        {
            pSrc = other.pSrc;
            srcStep = other.srcStep;
            srcHeight = other.srcHeight;
            srcOffset = other.srcOffset;
            dstRoiSize = other.dstRoiSize;
            maskSize = other.maskSize;
            anchor = other.anchor;
            int nSrcLines = other.nSrcLines;
            nWidth = other.nWidth;
            nTotalWidth = other.nTotalWidth;
            nAlignedWidth = other.nAlignedWidth;
            nAlignedWidthInBytes = other.nAlignedWidthInBytes;
            nColumnsBefore = other.nColumnsBefore;
            nColumnsAfter = other.nColumnsAfter;
            nLinesBefore = other.nLinesBefore;
            nLinesAfter = other.nLinesAfter;
            nRightBorderStart = other.nRightBorderStart;
            nLastSrcColumn = other.nLastSrcColumn;
            nSrcLineLength = other.nSrcLineLength;
            pBuffer = other.pBuffer;
            pOverlappedData = other.pOverlappedData;
            nOffset = other.nOffset;
            nSrcLine = other.nSrcLine;
        }

        // performs initialization and initial border replication
        void Initialize( const SrcType * pSrc, int srcStep, int srcHeight, int srcOffset,
                          const FwiSize & dstRoiSize,
                          const FwiSize & maskSize,
                          const FwiPoint & anchor,
                          Fw8u *pBuffer )
        {
            this->pSrc = pSrc;
            this->srcStep = srcStep;
            this->srcHeight = srcHeight;
            this->srcOffset = srcOffset;
            this->dstRoiSize = dstRoiSize;
            this->maskSize = maskSize;
            this->anchor = anchor;
            this->pBuffer = (SrcType*)( ( (size_t)pBuffer & (~0xF) ) + 16 ); // make sure that pbuffer is 16 bytes aligned
            
            nWidth = dstRoiSize.width + maskSize.width - 1;
            nAlignedWidthInBytes = ( ( nWidth * chCount * sizeof(SrcType) ) & (~0xF) ) + 16;
            assert( ( 16 % sizeof(SrcType) ) == 0 ); // assume that 16 MOD sizof(SrcType) == 0
            nAlignedWidth = nAlignedWidthInBytes / sizeof(SrcType);
            nColumnsBefore = anchor.x;
            nColumnsAfter = maskSize.width - anchor.x - 1;
            nLinesBefore = anchor.y;
            nLinesAfter = maskSize.height - anchor.y - 1;
            nSrcLines = srcHeight - srcOffset;
            nRightBorderStart = ( nWidth - nColumnsAfter ) * chCount;
            nLastSrcColumn = ( dstRoiSize.width - 1 ) * chCount;
            nSrcLineLength = dstRoiSize.width * chCount * sizeof(SrcType);
            nOffset = 0;
            nSrcLine = 0;

            int nReplicatedBorderSize = nAlignedWidthInBytes * maskSize.height;
            //int nOverlappedBorderSize = dstRoiSize.width * ( maskSize.height - 1 ) * chCount * sizeof( SrcType );
            pOverlappedData = (SrcType *)( ((Fw8u*)this->pBuffer) + nReplicatedBorderSize );

            InitializeBuffer();
        }

        // copies the data from the original source buffer into temporary buffer
        // the temporary buffer is larger (wider) to accomodate the borders
        // it also replicates the borders
        inline void InitializeBuffer()
        {
            int nFirstNonBorderLine = 0;
            int nFirstNonBorderOffset = srcOffset - nLinesBefore;
            if( nFirstNonBorderOffset < 0 )
            {
                nFirstNonBorderLine -= nFirstNonBorderOffset;
                nFirstNonBorderOffset = 0;
            }

            int nLastNonBorderLine = maskSize.height - 1;
            int nLastNonBorderOffset = nFirstNonBorderOffset + maskSize.height - 1 - nFirstNonBorderLine;
            if( nLastNonBorderOffset >= srcHeight )
            {
                nLastNonBorderLine -= nLastNonBorderOffset - srcHeight + 1;
            }

            // fill the non-border lines
            const SrcType *ps = (SrcType*)(((Fw8u*)pSrc) + ( nFirstNonBorderOffset - srcOffset ) * srcStep);
            SrcType *pd = (SrcType*)(((Fw8u*)pBuffer) + nFirstNonBorderLine * nAlignedWidthInBytes);
            SrcType *pdEnd = (SrcType*)(((Fw8u*)pBuffer) + (nLastNonBorderLine + 1) * nAlignedWidthInBytes);
            while( pd < pdEnd )
            {
                for( int x = 0; x < nColumnsBefore; x++ )
                {
                    for( int channel = 0; channel < chCount; channel++ )
                    {
                        pd[ x * chCount + channel ] = ps[channel];
                    }
                }
                memcpy( pd + nColumnsBefore * chCount, ps, dstRoiSize.width * chCount * sizeof(SrcType) );
                for( int x = 0; x < nColumnsAfter; x++ )
                {
                    for( int channel = 0; channel < chCount; channel++ )
                    {
                        pd[ nRightBorderStart + x * chCount + channel ] = ps[ nLastSrcColumn + channel ];
                    }
                }
                pd += nAlignedWidth;
                ps = (SrcType*)(((Fw8u*)ps) + srcStep);
            }

            // build the top border by duplicating the first non-border line as necessary
            ps = (SrcType*)(((Fw8u*)pBuffer) + nFirstNonBorderLine * nAlignedWidthInBytes);
            pd = (SrcType*)pBuffer;
            pdEnd = (SrcType*)ps;
            while( pd < pdEnd )
            {
                memcpy( pd, ps, nAlignedWidthInBytes );
                pd += nAlignedWidth;
            }

            // build the bottom border by duplicating the last non-border line as necessary
            ps = (SrcType*)(((Fw8u*)pBuffer) + nLastNonBorderLine * nAlignedWidthInBytes);
            pd = (SrcType*)(ps + nAlignedWidth);
            pdEnd = (SrcType*)(((Fw8u*)pBuffer) + maskSize.height * nAlignedWidthInBytes);
            while( pd < pdEnd )
            {
                memcpy( pd, ps, nAlignedWidthInBytes );
                pd += nAlignedWidth;
            }

            // copy the overlapped data
            int nOverlappedLines = nLinesAfter;
            if( dstRoiSize.height + nLinesAfter > nSrcLines )
            {
                nOverlappedLines -= dstRoiSize.height + nLinesAfter - nSrcLines;
            }
            for( int n = 0; n < nOverlappedLines; n++ )
            {
                const Fw8u *ps = ((const Fw8u*)pSrc) + ( dstRoiSize.height + n ) * srcStep;
                Fw8u *pd = ((Fw8u*)pOverlappedData) + n * nSrcLineLength;
                memcpy( pd, ps, nSrcLineLength );
            }
        }

        inline const SrcType * GetSrcLine( size_t lineIndex )
        {
            assert( pBuffer );
            return pBuffer + ( ( ( nOffset + lineIndex ) % maskSize.height ) * nAlignedWidth );
        }

        inline const SrcType * GetSrcElement( int col, size_t lineIndex )
        {
            assert( pBuffer );
            const SrcType *pLine = pBuffer + ( ( ( nOffset + lineIndex ) % maskSize.height ) * nAlignedWidth );
            return pLine + ( col + anchor.x ) * chCount;
        }

        // "scrolls" through the source buffer, shifts the data in the temporary buffer one line up, and
        // populates the bottom line with contents from the next source image line, performs border 
        // replication on the new line
        inline void RollBuffer()
        {
            assert( pBuffer );
            if( nSrcLine == ( dstRoiSize.height - 1 ) )
            {
                assert( false );
                return;
            }
            nSrcLine++;

            int y = nSrcLine + ( maskSize.height - anchor.y - 1 );

            if( y < nSrcLines )
            {
                const SrcType *ps;
                if( y < dstRoiSize.height )
                {
                    // use the source data from the original buffer
                    ps = (SrcType*)(((Fw8u*)pSrc) + srcStep * y);
                }
                else
                {
                    // use the source data from the overlapped buffer copy
                    const int nOffsetInOverlapBuffer = y - dstRoiSize.height;
                    ps = (SrcType*)(((Fw8u*)pOverlappedData) + nOffsetInOverlapBuffer * nSrcLineLength);
                }

                SrcType *pd = pBuffer + nOffset * nAlignedWidth;
                for( int x = 0; x < nColumnsBefore; x++ )
                {
                    for( int channel = 0; channel < chCount; channel++ )
                    {
                        pd[ x * chCount + channel ] = ps[channel];
                    }
                }
                memcpy( pd + nColumnsBefore * chCount, ps, nSrcLineLength );
                for( int x = 0; x < nColumnsAfter; x++ )
                {
                    for( int channel = 0; channel < chCount; channel++ )
                    {
                        pd[ nRightBorderStart + x * chCount + channel ] = ps[ nLastSrcColumn + channel ];
                    }
                }
            }
            else
            {
                const int delta = ( y - nSrcLines ) + 1;
                const int srcIndex = ( nOffset - delta + maskSize.height ) % maskSize.height;
                const SrcType *ps = pBuffer + srcIndex * nAlignedWidth;
                SrcType *pd = pBuffer + nOffset * nAlignedWidth;
                memcpy( pd, ps, nAlignedWidthInBytes );
            }

            nOffset = ( nOffset + 1 ) % maskSize.height;
        }

        inline static FwStatus GetBufferSize( int roiWidth, FwiSize maskSize, int *pBufferSize )
        {
            if( pBufferSize == NULL )
                return fwStsNullPtrErr;

            if( roiWidth <= 0 || maskSize.width <= 0 || maskSize.width <= 0 )
                return fwStsSizeErr;

            int nLineSize = ( roiWidth + maskSize.width - 1 ) * chCount * sizeof( SrcType );
            int nAlignedLineSize = ( nLineSize & (~0x0F) ) + 16;
            int nReplicatedBorderSize = maskSize.height * nAlignedLineSize;
            int nOverlappedBorderSize = roiWidth * ( maskSize.height - 1 ) * chCount * sizeof( SrcType );

            *pBufferSize = nReplicatedBorderSize + nOverlappedBorderSize + 16;

            return fwStsNoErr;
        }
    };

    template< CH chCount, typename SrcType, int borderWidth >
    class BorderReplicator
    {
    };

    template< CH chCount, typename SrcType, int borderWidth >
    class ImageDuplicator
    {
    public:
        inline static void DuplicateImage( SrcType *pDst, int dstStep, const SrcType *pSrc, int srcStep, const FwiSize & roiSize )
        {
            const int lineLength = roiSize.width * chCount * sizeof(SrcType);
            pDst = (SrcType*)(((Fw8u*)pDst) + dstStep * borderWidth );
            pDst += borderWidth * chCount;
            const SrcType *pSrcEnd = (const SrcType*)(((Fw8u*)pSrc) + srcStep * roiSize.height);
            while( pSrc < pSrcEnd )
            {
                memcpy( pDst, pSrc, lineLength );
                pDst = (SrcType*)(((Fw8u*)pDst) + dstStep );
                pSrc = (const SrcType*)(((Fw8u*)pSrc) + srcStep ); 
            }
        }
    };

    // 3 channel specialized version of BorderReplicator class
    template< typename SrcType,  int borderWidth >
    class BorderReplicator< C3, SrcType, borderWidth >
    {
        // replicates the border using constant value algorithm (all pixels of the border are set to the same value)
        // 
        // xx xxxxx xx
        // xx xxxxx xx
        //   -------
        // xx|abcde|xx
        // xx|fghij|xx
        // xx|klmno|xx
        //   -------
        // xx xxxxx xx
        // xx xxxxx xx
        //
        inline static void ConstBorder( SrcType *pDst, int width, int height, SrcType borderValue[3] )
        {
            const int chCount = 3;
            const int nBorderWidthBytes = borderWidth * 3 * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * borderWidth );

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

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                // left and right borders
                SrcType *pElement = pDst;
                SrcType *pElement2 = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pElement < pLeftBorderEnd )
                {
                    *(pElement+0) = borderValue[0];
                    *(pElement+1) = borderValue[1];
                    *(pElement+2) = borderValue[2];
                    *(pElement2+0) = borderValue[0];
                    *(pElement2+1) = borderValue[1];
                    *(pElement2+2) = borderValue[2];
                    pElement += chCount;
                    pElement2 += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

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

        // replicates the border using the edge pxiel value
        // 
        // aa abcde ee
        // aa abcde ee
        //   -------
        // aa|abcde|ee
        // ff|fghij|jj
        // kk|klmno|oo
        //   -------
        // kk klmno oo
        // kk klmno oo
        //
        inline static void ReplBorder( SrcType *pBuffer, int width, int height )
        {
            const int chCount = 3;
            const int nBorderWidthBytes = borderWidth * chCount * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRightSrcOffset = (width - 1) * chCount;

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                const SrcType * pLeftSrc = pLeftBorderEnd;
                const SrcType * pRightSrc = pLeftBorderEnd + nRightSrcOffset;

                SrcType *pLeftElement = pDst;
                SrcType *pRightElement = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *(pLeftElement+0) = *(pLeftSrc+0);
                    *(pLeftElement+1) = *(pLeftSrc+1);
                    *(pLeftElement+2) = *(pLeftSrc+2);
                    // right border
                    *(pRightElement+0) = *(pRightSrc+0);
                    *(pRightElement+1) = *(pRightSrc+1);
                    *(pRightElement+2) = *(pRightSrc+2);

                    pLeftElement += chCount;
                    pRightElement += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = pTop;
            SrcType * pSrc2 = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );

            // top and bottom borders
            pDst = pBuffer;
            SrcType * pDst2 = pBtm;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
            }
        }

        // replicates the border using wrap algorithm
        // 
        // ij fghij fg
        // no klmno kl
        //   -------
        // de|abcde|ab
        // ij|fghij|fg
        // no|klmno|kl
        //   -------
        // de abcde ab
        // ij fghij fg
        //
        inline static void WrapBorder( SrcType *pBuffer, int width, int height )
        {
            const int chCount = 3;
            const int nBorderWidthBytes = borderWidth * chCount * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRoiWidthBytes = width * chCount;

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                const SrcType * pLeftSrc = (SrcType *)(((Fw8u*)pDst) + nRoiWidthBytes );
                const SrcType * pRightSrc = pLeftBorderEnd;

                SrcType *pLeftElement = pDst;
                SrcType *pRightElement = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *(pLeftElement+0) = *(pLeftSrc+0);
                    *(pLeftElement+1) = *(pLeftSrc+1);
                    *(pLeftElement+2) = *(pLeftSrc+2);
                    // right border
                    *(pRightElement+0) = *(pRightSrc+0);
                    *(pRightElement+1) = *(pRightSrc+1);
                    *(pRightElement+2) = *(pRightSrc+2);

                    pLeftElement += chCount;
                    pRightElement += chCount;
                    pLeftSrc += chCount;
                    pRightSrc += chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = (SrcType *)(((Fw8u*)pBtm) - borderWidth * nAlignedStepSize );
            SrcType * pSrc2 = pTop;

            // top and bottom borders
            pDst = pBuffer;
            SrcType * pDst2 = pBtm;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
                pSrc2 = (SrcType *)(((Fw8u*)pSrc2) + nAlignedStepSize );
            }
        }

        // replicates the border using the mirror algorithm
        // 
        // gf fghij ji
        // ba abcde ed
        //   -------
        // ba|abcde|ed
        // gf|fghij|ji
        // lk|klmno|on
        //   -------
        // lk klmno on
        // ij fghij ji
        //
        inline static void MirrorBorder( SrcType *pBuffer, int width, int height )
        {
            const int chCount = 3;
            const int nBorderWidthBytes = borderWidth * chCount * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRoiWidthBytes = width * chCount;

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftElement = pDst;
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                const SrcType * pLeftSrc = pLeftBorderEnd + chCount * (borderWidth - 1);

                SrcType *pRightElement = (SrcType *)(((Fw8u*)pDst) + nRoiWidthBytes + nBorderWidthBytes );
                const SrcType * pRightSrc = (SrcType *)(((Fw8u*)pRightElement) - chCount * sizeof(SrcType) );

                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *(pLeftElement+0) = *(pLeftSrc+0);
                    *(pLeftElement+1) = *(pLeftSrc+1);
                    *(pLeftElement+2) = *(pLeftSrc+2);
                    // right border
                    *(pRightElement+0) = *(pRightSrc+0);
                    *(pRightElement+1) = *(pRightSrc+1);
                    *(pRightElement+2) = *(pRightSrc+2);

                    pLeftElement += chCount;
                    pRightElement += chCount;
                    pLeftSrc -= chCount;
                    pRightSrc -= chCount;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = pTop;
            SrcType * pSrc2 = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );

            // top and bottom borders
            pDst = (SrcType *)(((Fw8u*)pTop) - nAlignedStepSize );
            SrcType * pDst2 = pBtm;
            while( pDst >= pBuffer )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) - nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
                pSrc2 = (SrcType *)(((Fw8u*)pSrc2) - nAlignedStepSize );
            }
        }

        inline static void Mirror2Border( SrcType *pBuffer, int width, int height )
        {
            pBuffer;
            width;
            height;
        }
        
        inline static int StepSize( const int width )
        {
            const int chCount = 3;
            int len = ( width + 2 * borderWidth ) * chCount * sizeof( SrcType );
            return len;
        }

    public:
        inline static int AlignedStepSize( const int width )
        {
            int len = StepSize( width ) + 16;
            len &= ~(0xF);
            return len;
        }

        inline static FwStatus GetBufferSize( const FwiSize & roiSize, int *pBufferSize )
        {
            if( pBufferSize == NULL )
                return fwStsNullPtrErr;
            if( roiSize.width <= 0 || roiSize.height <= 0 )
                return fwStsSizeErr;

            int bytes = AlignedStepSize( roiSize.width );
            bytes *= roiSize.height + 2 * borderWidth;
            *pBufferSize = bytes;
            return fwStsNoErr;
        }

        inline static FwStatus CreateBorder( SrcType *pBuffer, const FwiSize & roiSize, FwiBorderType borderType, SrcType borderValue[3], SrcType **pNewSrc )
        {
            const int chCount = 3;

            switch( borderType )
            {
            case fwBorderConst:
                ConstBorder( pBuffer, roiSize.width, roiSize.height, borderValue );
                break;
            case fwBorderRepl:
                ReplBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderWrap:
                WrapBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderMirror:
                MirrorBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderMirror2:
                Mirror2Border( pBuffer, roiSize.width, roiSize.height );
                break;
            }

            int nAlignedStepSize = AlignedStepSize( roiSize.width );
            SrcType * pCorrectedSrc = (SrcType *)(((Fw8u*)pBuffer) + borderWidth * nAlignedStepSize );
            *pNewSrc = pCorrectedSrc + borderWidth * chCount;

            return fwStsNoErr;
        }
    };

    template< typename SrcType, int borderWidth >
    class BorderReplicator< C1, SrcType, borderWidth >
    {
        inline static void ConstBorder( SrcType *pDst, int width, int height, SrcType borderValue[1] )
        {
            const int nBorderWidthBytes = borderWidth * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );

            SrcType * pTop = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );
            SrcType * pEnd = (SrcType *)(((Fw8u*)pBtm) + nAlignedStepSize * borderWidth );

            // top border
            while( pDst < pTop )
            {
                SrcType *pElement = pDst;
                SrcType *pLineEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                while( pElement < pLineEnd )
                {
                    *pElement = borderValue[0];
                    pElement++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                // left border
                SrcType *pElement = pDst;
                SrcType *pElement2 = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pElement < pLeftBorderEnd )
                {
                    *pElement = borderValue[0];
                    *pElement2 = borderValue[0];
                    pElement++;
                    pElement2++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            // bottom border
            while( pDst < pEnd )
            {
                SrcType *pElement = pDst;
                SrcType *pLineEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                while( pElement < pLineEnd )
                {
                    *pElement = borderValue[0];
                    pElement++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }
        }

        inline static void ReplBorder( SrcType *pBuffer, int width, int height )
        {
            const int nBorderWidthBytes = borderWidth * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRightSrcOffset = (width - 1);

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                const SrcType * pLeftSrc = pLeftBorderEnd;
                const SrcType * pRightSrc = pLeftBorderEnd + nRightSrcOffset;

                SrcType *pLeftElement = pDst;
                SrcType *pRightElement = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *pLeftElement = *pLeftSrc;
                    // right border
                    *pRightElement = *pRightSrc;

                    pLeftElement++;
                    pRightElement++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = pTop;
            SrcType * pSrc2 = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );

            // top and bottom borders
            pDst = pBuffer;
            SrcType * pDst2 = pBtm;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
            }
        }

        inline static void WrapBorder( SrcType *pBuffer, int width, int height )
        {
            const int nBorderWidthBytes = borderWidth * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRoiWidthBytes = width;

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                SrcType *pRightBorderEnd = (SrcType *)(((Fw8u*)pDst) + nStepSize );

                const SrcType * pLeftSrc = (SrcType *)(((Fw8u*)pDst) + nRoiWidthBytes );
                const SrcType * pRightSrc = pLeftBorderEnd;

                SrcType *pLeftElement = pDst;
                SrcType *pRightElement = (SrcType *)(((Fw8u*)pRightBorderEnd) - nBorderWidthBytes );
                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *pLeftElement = *pLeftSrc;
                    // right border
                    *pRightElement = *pRightSrc;

                    pLeftElement++;
                    pRightElement++;
                    pLeftSrc++;
                    pRightSrc++;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = (SrcType *)(((Fw8u*)pBtm) - borderWidth * nAlignedStepSize );
            SrcType * pSrc2 = pTop;

            // top and bottom borders
            pDst = pBuffer;
            SrcType * pDst2 = pBtm;
            while( pDst < pTop )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
                pSrc2 = (SrcType *)(((Fw8u*)pSrc2) + nAlignedStepSize );
            }
        }

        inline static void MirrorBorder( SrcType *pBuffer, int width, int height )
        {
            const int nBorderWidthBytes = borderWidth * sizeof(SrcType);
            const int nStepSize = StepSize( width );
            const int nAlignedStepSize = AlignedStepSize( width );
            const int nRoiWidthBytes = width;

            SrcType * pTop = (SrcType *)(((Fw8u*)pBuffer) + nAlignedStepSize * borderWidth );
            SrcType * pBtm = (SrcType *)(((Fw8u*)pTop) + nAlignedStepSize * height );

            SrcType * pDst = pTop;

            while( pDst < pBtm )
            {
                SrcType *pLeftElement = pDst;
                SrcType *pLeftBorderEnd = (SrcType *)(((Fw8u*)pDst) + nBorderWidthBytes );
                const SrcType * pLeftSrc = pLeftBorderEnd + (borderWidth - 1);

                SrcType *pRightElement = (SrcType *)(((Fw8u*)pDst) + nRoiWidthBytes + nBorderWidthBytes );
                const SrcType * pRightSrc = pRightElement - 1;

                while( pLeftElement < pLeftBorderEnd )
                {
                    // left border
                    *pLeftElement = *pLeftSrc;
                    // right border
                    *pRightElement = *pRightSrc;

                    pLeftElement++;
                    pRightElement++;
                    pLeftSrc--;
                    pRightSrc--;
                }

                pDst = (SrcType *)(((Fw8u*)pDst) + nAlignedStepSize );
            }

            SrcType * pSrc = pTop;
            SrcType * pSrc2 = (SrcType *)(((Fw8u*)pBtm) - nAlignedStepSize );

            // top and bottom borders
            pDst = (SrcType *)(((Fw8u*)pTop) - nAlignedStepSize );
            SrcType * pDst2 = pBtm;
            while( pDst >= pBuffer )
            {
                memcpy( pDst, pSrc, nStepSize );
                memcpy( pDst2, pSrc2, nStepSize );
                pDst = (SrcType *)(((Fw8u*)pDst) - nAlignedStepSize );
                pDst2 = (SrcType *)(((Fw8u*)pDst2) + nAlignedStepSize );
                pSrc = (SrcType *)(((Fw8u*)pSrc) + nAlignedStepSize );
                pSrc2 = (SrcType *)(((Fw8u*)pSrc2) - nAlignedStepSize );
            }
        }

        inline static void Mirror2Border( SrcType *pBuffer, int width, int height )
        {
            pBuffer;
            width;
            height;
        }
        
        inline static int StepSize( const int width )
        {
            int len = ( width + 2 * borderWidth ) * sizeof( SrcType );
            return len;
        }

    public:
        inline static int AlignedStepSize( const int width )
        {
            int len = StepSize( width ) + 16;
            len &= ~(0xF);
            return len;
        }

        inline static FwStatus GetBufferSize( const FwiSize & roiSize, int *pBufferSize )
        {
            if( pBufferSize == NULL )
                return fwStsNullPtrErr;
            if( roiSize.width <= 0 || roiSize.height <= 0 )
                return fwStsSizeErr;

            int bytes = AlignedStepSize( roiSize.width );
            bytes *= roiSize.height + 2 * borderWidth;
            *pBufferSize = bytes;
            return fwStsNoErr;
        }

        // This function will replicate the border using one of the specified algorithms
        inline static FwStatus CreateBorder( SrcType *pBuffer, const FwiSize & roiSize, FwiBorderType borderType, SrcType borderValue[1], SrcType **pNewSrc )
        {
            switch( borderType )
            {
            case fwBorderConst:
                ConstBorder( pBuffer, roiSize.width, roiSize.height, borderValue );
                break;
            case fwBorderRepl:
                ReplBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderWrap:
                WrapBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderMirror:
                MirrorBorder( pBuffer, roiSize.width, roiSize.height );
                break;
            case fwBorderMirror2:
                Mirror2Border( pBuffer, roiSize.width, roiSize.height );
                break;
            }

            int nAlignedStepSize = AlignedStepSize( roiSize.width );
            SrcType * pCorrectedSrc = (SrcType *)(((Fw8u*)pBuffer) + borderWidth * nAlignedStepSize );
            *pNewSrc = pCorrectedSrc + borderWidth;
            return fwStsNoErr;
        }
    };

} // namespace BordersReplication

}; // OPT_LEVEL
#endif // __BORDERREPLICATION_H__
