/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*#DataBlock

#Parameters

ptr - Pointer to a memory buffer.
length - Specifies a number of bytes to be allocated.
StsCode - Specifies the status code used by the FwStatus function.
cpu - Specifies a CPU type.
numThr - Specifies the maximum number of threads that can be used by any Framewave function.
alignBytes - Specifies the number of bytes for alignment, must be a power of 2. NULL is returned if the value is less than 2.
workFn - Specifies work function.
param - Specifies some parameter.

#DataStructures

FwLibraryVersion - Library version information.
FwRoundMode - Rounding mode used in data type conversion.
FwWinType - Type of window used to generate FIR filter coefficients.

#Enumerators

FwBool - Type of Boolean values.
FwCpuType - Type of CPU with presence of streaming SIMD extensions.
FwCmpOp - Type of comparison operation.
FwHintAlgorithm - Hint to favor speed or accuracy.

*/

/*#Documentation

<SectionHeading>Base Library</SectionHeading>

<Paragraph>Base Library<SectDesc> functions are essential for primary tasks such as memory allocation and functions that manage the performance of other library functions.</SectDesc></Paragraph>

<Paragraph>This section is organized as follows.</Paragraph>

<SectLinks></SectLinks>

<Paragraph>Within the section, the <Bold>Basic Concepts</Bold> chapter provides overview information related to the functions in the library, 
and subsequent chapters provide detailed descriptions of library functions that perform operations of the same kind.</Paragraph>
<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 
<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description.</Paragraph>
<Paragraph>The <hlink><uri>need_link</uri><desc>Function Quick Reference</desc></hlink> provides an index and snapshot view of function optimizations.</Paragraph>


<Chapter>
<ChapterHeading>Basic Concepts</ChapterHeading>

<Paragraph>Base library functional descriptions include the following types of information.</Paragraph>

<Heading2>
<H2Name>Data Structures</H2Name>
<H2Link>base_structures</H2Link>
</Heading2>

<Paragraph>Base and other libraries function definitions use the following data structures.</Paragraph>

#PrintAllDataStructures

<Heading2>
<H2Name>Enumerators</H2Name>
<H2Link>base_enumerators</H2Link>
</Heading2>

<Paragraph>Base and other libraries function definitions use the following enumerators.</Paragraph>

#PrintAllEnumerators

<Heading2>
<H2Name>Parameter Glossary</H2Name>
<H2Link>base_parameters</H2Link>
</Heading2>

<Paragraph>Base library function definitions use the following parameters.</Paragraph>

#PrintAllParameters

</Chapter>

*/
