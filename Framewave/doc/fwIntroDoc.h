/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

// this extern "C" is needed to activate documentation - kludge
extern "C" {

/*#DataBlock

#DataTypes

Fw8s		-	8-bit signed integer
Fw8u		-	8-bit unsigned integer
Fw16s		-	16-bit signed integer
Fw16u		-	16-bit unsigned integer
Fw16sc		-	16-bit signed complex number

Fw32s		-	32-bit signed integer
Fw32u		-	32-bit unsigned integer
Fw32sc		-	32-bit signed complex number

Fw64s		-	64-bit signed integer
Fw64u		-	64-bit unsigned integer
Fw64sc		-	64-bit signed complex number

Fw32f		-	32-bit floating-point number
Fw32fc		-	32-bit floating-point complex number

Fw64f		-	64-bit floating-point number
Fw64fc		-	64-bit floating-point complex number

#ReturnValues

fwStsNoErr - No error detected.
fwStsNullPtrErr - A pointer passed to the function is null.
fwStsStepErr - The step value for the buffer is invalid.
fwStsSizeErr - The size is invalid (usually indicates negative or zero size).

fwStsThresholdErr - The less-than threshold is greater than the greater-than threshold.
fwStsEpsValErr - Eps is a negative value.


fwStsCoeffErr - The coefficient values have errors.
fwStsInterpolationErr - The interpolation value is incorrect or unsupported for the specified function.
fwStsMirrorFlipErr - The mirror flip value is invalid.
fwStsResizeNoOperationErr - A destination image dimension is less than one pixel.
fwStsNumChannelErr - The number of channels is incorrect.
fwStsQuadErr - The source or destination quadrangle degenerates into a triangle, line, or points; or the destination quadrangle has conflicted vertex coordinates.
fwStsRectErr - The width or height of the source ROI has a value less than or equal to one.
fwStsResizeFactorErr - The resize factor is less than or equal to zero.
fwStsWrongIntersectQuad - The quadrangle does not intersect the source or destination image (no operation is performed).
fwStsWrongIntersectROI - The ROI does not intersect the source or destination image (no operation is performed).

fwStsChannelOrderErr - The given destination order is not valid (contains values other than 0,1,2).

fwStsZeroMaskValuesErr - All mask values are equal to zero.
fwStsAnchorErr - The anchor is located outside the mask.
fwStsNotEvenStepErr - A step value for a floating point image is not divisible by four.
fwStsBadArgErr - Some argument is invalid.
fwStsBorderErr - The morphology borderType is an invalid value.

fwStsDivisorErr - The divisor is zero.
fwStsMaskErr - The mask size is invalid.
fwStsMaskSizeErr - The mask has an invalid value.
fwStsMemAllocErr - The function failed to allocate memory.

fwStsFftOrderErr - The specified order is out of bounds.
fwStsFftFlagErr - The flag value is incorrect.
fwStsContextMatchErr - The specification struture does not match the function or is invalid.

fwStsCpuMismatch - The specified CPU type cannot be assigned.

fwStsDomain - Argument out of function domain warning status code.
fwStsLnNegArg - Negative input buffer element warning status code.
fwStsLnZeroArg - Zero-valued input buffer element warning status code.
fwStsOverflow -  Overflow in operation warning status code.
fwStsUnderflow -  Underflow in operation warning status code.
fwStsSingularity - Singularity in operation warning status code.
fwStsDoubleSize - Image height and/or width is not a multiple of two warning status code.

fwStsDivByZeroErr - The divisor is zero.

fwStsH263VLCCodeErr - Illegal Huffman code encountered during VLC stream processing.
fwStsOutOfRangeErr - An a rgument is out of range or a point is outside an image.
fwStsResFloor - All result values are floored.

 

*/

/*#Documentation

<Introduction>

<Heading1>
<H1Name>Introduction</H1Name>
<H1Link>aa_005_intro</H1Link>
</Heading1>

<Paragraph>The Framewave library is a collection of highly-optimized 
 functions for use in a variety of programming domains. All implementations 
 of the libraries provide C and C++ programmers ANSI C style interfaces.</Paragraph>

<Paragraph>Framewave consists of the following libraries:</Paragraph>

<BulletedList>
<Bullet>The <hlink><uri>need_link</uri><desc>Base</desc></hlink> library consists of functions essential for primary tasks such as memory allocation and functions that manage the performance of other library functions.</Bullet>
<Bullet>The <hlink><uri>need_link</uri><desc>FFT</desc></hlink> library consists of functions that perform time and frequency domain conversions.</Bullet>
<Bullet>The <hlink><uri>need_link</uri><desc>Image Processing</desc></hlink> library consists of functions that perform a variety of image processing tasks.</Bullet> 
<Bullet>The <hlink><uri>need_link</uri><desc>JPEG</desc></hlink> library consists of functions that perform a variety of tasks related to image compression and decompression.</Bullet>
<Bullet>The <hlink><uri>need_link</uri><desc>Signal Processing</desc></hlink> library consists of primitives that perform signal processing.</Bullet> 
<Bullet>The <hlink><uri>need_link</uri><desc>Video</desc></hlink> library consists of functions that encode and decode moving images.</Bullet>
</BulletedList>

<Paragraph>Framewave functions are geared to yield maximum performance on x86 hardware architectures. 
Current implementations exploit multicore architecture and single instruction multiple data (SIMD) instructions. 
Specifically, streaming SIMD extensions and AMD family 10h technologies are used to optimize for speed. 
Programmers can concentrate on task functionality because Framewave handles performance. 
Many of the functions are threaded internally; the programmer has the flexibility of 
controlling the number of threads and of turning off threading. 
As the architecture is extended and new instructions are added, new code paths 
that take advantage of the extensions are added to Framewave, without 
changing the programming interface and existing functionality.</Paragraph>

<Heading2>
<H2Name>About This Manual</H2Name>
<H2Link>aa_intro_001</H2Link>
</Heading2>

<Paragraph>
Each Framewave release includes documentation which is unique to the release. 
The version number of the released documentation is the same as the version number of the Framewave release. 
As new groups of functions are added to Framewave, new sections, chapters, and functional descriptions are added to the documentation. 
There may be interim changes to the documentation to improve, clarify, or modify the content. 
There may also be changes that reflect interim changes to Framewave.
The Record of Changes in the on-line version of the documentation includes interim changes made between releases. 
The Record of Changes in the released documentation provides a summary of the differences between successive releases.
</Paragraph>

<Paragraph>Framewave documentation is written for experienced software developers. To understand the functional
descriptions, a developer must be reasonably proficient in the C programming language, and must have 
a working knowledge of application-specific terminology and techniques.</Paragraph>

<Paragraph>There are sections for each library. Within the sections, <Bold>Basic Concepts</Bold> chapters provide overview information related to the library  
and subsequent chapters provide detailed descriptions of the library functions.</Paragraph>

<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 

<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description. 
Most function groups are optimized for the same technologies, but the lists are function-specific; the use of a particular optimization in a single function 
does not indicate that all the functions in the group are optimized for that technology. When no list of optimizations is present, a function has only reference code. 
Absence of optimizations does not mean that a function runs more slowly -- use of a particular optimization may not increase the performance of the function.
</Paragraph>

<Paragraph>The <hlink><uri>fw_section_070.html</uri><desc>Function Quick Reference</desc></hlink> provides an index and snapshot view of function optimizations.</Paragraph>

<Heading2>
<H2Name>Data Types</H2Name>
<H2Link>aa_intro_002</H2Link>
</Heading2>

<Paragraph>Framewave function definitions use the following data types.</Paragraph>

#PrintAllDataTypes

<Heading2>
<H2Name>Return Value Glossary</H2Name>
<H2Link>aa_intro_003</H2Link>
</Heading2>

<Paragraph>All Framewave functions return the enumerated integer value <Bold>FwStatus</Bold>, which
precisely reports the result of execution. The following return value definitions are used.</Paragraph>

#PrintAllReturnValues

<Paragraph>Refer to the individual function descriptions for detailed information about return values.</Paragraph>

<Heading2>
<H2Name>Processing Operators</H2Name>
<H2Link>aa_intro_004</H2Link>
</Heading2>

<Paragraph>Framewave function definitions use the following notation to indicate specific operations.</Paragraph>

   <DefinitionList>
     <DefinitionEntry>
       <Term><Constant>I</Constant></Term>
       <Definition>Function operates in-place (source and destination locations are the same)</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><Constant>R</Constant></Term>
       <Definition>Function operates on an ROI (iterative, used only in the Image Processing library)</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><Constant>M</Constant></Term>
       <Definition>Function performs a masking operation</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><Constant>Sfs</Constant></Term>
       <Definition>Function performs a scaled integer calculation</Definition>
     </DefinitionEntry>
   </DefinitionList>

<Heading2>
<H2Name>References</H2Name>
<H2Link>aa_intro_005</H2Link>
</Heading2>

<Paragraph>AMD64 Architecture Programmers Manual:</Paragraph>
<ParagraphIndent>Volume 1, <Italic>Application Programming</Italic>, Order Number 24592</ParagraphIndent>
<ParagraphIndent>Volume 2, <Italic>System Programming</Italic>, Order Number 24593</ParagraphIndent>
<ParagraphIndent>Volume 3, <Italic>General-Purpose and System Instructions</Italic>, Order Number 24594</ParagraphIndent>
<ParagraphIndent>Volume 4, <Italic>128-Bit Media Instructions</Italic>, Order Number 26568</ParagraphIndent>
<ParagraphIndent>Volume 5, <Italic>64-Bit Media and x87 Floating-Point Instructions</Italic>, Order Number 26569</ParagraphIndent>

</Introduction>

*/

};
