/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*#DataBlock

#Parameters

pDst - Pointer to a location in a destination buffer.
pSrc - Pointer to a location in a source buffer.
pSrc1 - Pointer to a location in source buffer one.
pSrc2 - Pointer to a location in source buffer two.
pSrcDst - Pointer to a location in a buffer that contains both the source and destination. 
srcStep - Source buffer step size (width of the buffer in bytes).
dstStep - Destination buffer step size (width of the buffer in bytes).
dstSize - Destination image size. 
srcSize - Source image size.
roiSize - Specifies the height and width of an ROI.

pSize - Pointer to the size of an internal structure.
pSrcBGR - Pointer to source image ROI for BGR image format.
pDstBGR - Pointer to destination image ROI for BGR image format.
pSrcCMYK - Pointer to source image ROI for CMYK image format.
pDstCMYK - Pointer to destination image ROI for CMYK image format.
pSrcCMYK[4] - Pointer to source image ROI for CMYK image format (four-channel planar).
pDstCMYK[4] - Pointer to destination image ROI for CMYK image format (four-channel planar).
pSrcRGB - Pointer to source image ROI for RGB image format.
pDstRGB - Pointer to destination image ROI for RGB image format.
pSrcRGB[3] - Pointer to source image ROI for RGB image format (three-channel planar).
pDstRGB[3] - Pointer to destination image ROI for RGB image format (three-channel planar).
pDstY - Pointer to destination image ROI for grayscale image format.
pSrcYCCK[4] - Pointer to source image ROI for YCCK image format (four-channel planar).
pDstYCCK[4] - Pointer to destination image ROI for YCCK image format (four-channel planar).
pSrcYCbCr[3] - Pointer to source image ROI for YCbCr image format (three-channel planar).
pDstYCbCr[3] - Pointer to destination image ROI for YCbCr image format (three-channel planar).
pSrcMCU[3] - Array of pointers to source MCU image blocks. 
pDstMCU[3] - Array of pointers to destination MCU image blocks.
pSrcMCU[4] - Array of pointers to source MCU image blocks. 
pDstMCU[4] - Array of pointers to destination MCU image blocks.
pQuantFwdTable - Pointer to the forward quantization table (encoder).
pQuantInvTable - Pointer to the inverse quantization table (decoder).
pQuantRawTable - Pointer to the raw quantization table.
qualityFactor - JPEG image quality factor (value between 1 and 100).

Al - Progressive JPEG successive approximation parameter for actual point transform.
Ss - Progressive JPEG spectral selection start index.
Se - Progressive JPEG spectral selection end index.
bFlushState - Indicates the last 8X8 block in a scan.
dstLenBytes - Destination buffer length in bytes.
srcLenBytes - Source buffer length in bytes.
pAcStatistics[256] - Pointer to Huffman symbol statistics for AC coefficient. 
pDcStatistics[256] - Pointer to Huffman symbol statistics for DC coefficient. 
pStatistics[256] - Pointer to Huffman symbol statistics.
pLastDC - Pointer to the last DC coefficient, which is in the previous 8X8 block.
pAcTable - Pointer to the Huffman AC coefficient table.
pDcTable - Pointer to the Huffman DC coefficient table.
pDecHuffSpec - Pointer to the FwiDecodeHuffmanSpec structure.
pDecHuffState - Pointer to the FwiDecodeHuffmanState structure. 
pDstBitsLen - Pointer to destination buffer length in bits.
pDstCurrPos - Pointer to the shift value at the current destination buffer in bytes.
pSrcCurrPos - Pointer to the shift value at the current source buffer in bytes.
pEncHuffSpec - Pointer to the FwiEncodeHuffmanSpec structure. 
pEncHuffState - Pointer to the FwiEncodeHuffmanState structure. 
pListBits - Pointer to the Bits list.
pListVals - Pointer to the Vals list.
pMarker - Pointer to the JPEG marker position.
pNumValidPrefetchedBits - Pointer to the value of valid bits in the prefetched buffer. 
pPrefetchedBits - Pointer to the prefetched buffer, which contains decoded data from the previous block.
pSrcBitsLen - Pointer to the source buffer length in bits.
srcWidth - Width of the source row in pixels.

#DataStructures

FwiEncodeHuffmanSpec - Internal data structure for Huffman encoder table.
FwiEncodeHuffmanState - Internal data structure for Huffman encoder state.
FwiDecodeHuffmanSpec - Internal data structure for Huffman decoder table.
FwiDecodeHuffmanState - Internal data structure for Huffman decoder state.

*/


/*#Documentation

<SectionHeading>JPEG Library</SectionHeading>

<Paragraph>JPEG Library functions perform a variety of tasks related to Joint Photographic Experts Group image manipulation.</Paragraph>

<Paragraph>This section is organized as follows.</Paragraph>

<BulletedList> 
<Bullet><hlink><uri>need_link</uri><desc>Basic Concepts</desc></hlink> provides an overview of the information contained in the functional descriptions.</Bullet> 
<Bullet><hlink><uri>need_link</uri><desc>Library Version</desc></hlink> describes the Framewave function that provides library version information.</Bullet> 
<Bullet><hlink><uri>need_link</uri><desc>Image Compression Functions</desc></hlink> describes the individual functions.</Bullet>
</BulletedList>

<Paragraph>Within the section, the <Bold>Basic Concepts</Bold> chapter provides overview information related to the functions in the library, 
and subsequent chapters provide detailed descriptions of library functions that perform operations of the same kind.</Paragraph>
<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 
<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description.</Paragraph>
<Paragraph>The <hlink><uri>need_link</uri><desc>Function Quick Reference</desc></hlink> provides an index and snapshot view of function optimizations.</Paragraph>

<Chapter>
<ChapterHeading>Basic Concepts</ChapterHeading>

<Paragraph>Image Compression library functional descriptions include the following types of information.</Paragraph>

<Heading2>
<H2Name>Data Structures</H2Name>
<H2Link>jpeg_structures</H2Link>
</Heading2>

<Paragraph>JPEG library function definitions use the following data structures.</Paragraph>

#PrintAllDataStructures

<Heading2>
<H2Name>Parameter Glossary</H2Name>
<H2Link>jpeg_parameters</H2Link>
</Heading2>

<Paragraph>JPEG library function definitions use the following parameters.</Paragraph>

#PrintAllParameters

</Chapter>

*/
