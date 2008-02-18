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

<VersionData>
<VersionNumber>1.0.0</VersionNumber>
<VersionDate>February, 2008</VersionDate>
</VersionData>

<AncillaryFiles>
	<FrameMaster></FrameMaster>
	<HeaderMaster></HeaderMaster>
	<FooterMaster></FooterMaster>
	<NavBar></NavBar>
</AncillaryFiles>

<FrontMatter>

<TitlePage></TitlePage>

<TOC></TOC>

<FrontSection>

<FrontHeading>
<FrontName>Copyright and Trademarks</FrontName>
<FrontLink>aa_007_tcd</FrontLink>
</FrontHeading>

<Paragraph>Copyright &#169; 2007-2008 The Framewave Group. All Rights Reserved.</Paragraph>
<Paragraph>Framewave is a trademark of The Framewave Group.</Paragraph>
<Paragraph>AMD is a registered trademark of Advanced Micro Devices, Inc.</Paragraph>
<Paragraph>Microsoft and Windows are registered trademarks of Microsoft Corporation.</Paragraph>
<Paragraph>Sun and Solaris are registered trademarks of Sun Microsystems, Inc.</Paragraph>
<Paragraph>Linux is a registered trademark of Linus Torvalds.</Paragraph>
<Paragraph>Apple and Mac OS are registered trademarks of Apple, Inc.</Paragraph>

</FrontSection>

<FrontSection>

<FrontHeading>
<FrontName>Change History</FrontName>
<FrontLink>aa_004_roc</FrontLink>
</FrontHeading>

<ChangeHistory>

<ChangeEntry>
<ChangeDate>02/2008</ChangeDate>
<ChangeVersion>1.0.0</ChangeVersion>
<ChangeDescription>Initial release.</ChangeDescription> 
</ChangeEntry>

</ChangeHistory>

</FrontSection>

</FrontMatter>

<Introduction>

<Heading1>
<H1Name>Introduction</H1Name>
<H1Link>aa_005_intro</H1Link>
</Heading1>

<Paragraph> Framewave (FW) is a collection of libraries that contain highly-optimized 
 functions for use in a variety of programming domains. All implementations 
 of the libraries provide C and C++ programmers ANSI C style interfaces.</Paragraph>

<Paragraph>Framewave consists of the following libraries:</Paragraph>

<IntroLinks></IntroLinks>

<Paragraph>Framewave functions are geared to yield maximum performance on the x86 and the AMD64 hardware architectures. 
Current implementations exploit multicore architecture and single instruction multiple data (SIMD) instructions. 
Specifically, streaming SIMD extensions and AMD&#174; family 10h technologies are used to optimize for speed. 
Programmers can concentrate on task functionality because Framewave handles performance. 
Many of the functions are threaded internally; the programmer has the flexibility of 
controlling the number of threads and of turning off threading. 
As architecture changes and new instructions are added, new code paths 
to take advantage of extensions can be added to Framewave without 
changing the programming interface and existing functionality.</Paragraph>

<Heading2>
<H2Name>About This Manual</H2Name>
<H2Link>aa_intro_001</H2Link>
</Heading2>

<Paragraph>This documentation is intended for experienced software developers. To understand the functional
descriptions, a developer must be reasonably proficient in the C programming language, and must have 
a working knowledge of application-specific terminology and techniques.</Paragraph>

<Paragraph>The documentation is divided into sections for each library. Within the sections, <Bold>Basic Concepts</Bold> chapters provide overview information related to the library  
and subsequent chapters provide detailed descriptions of the library functions.</Paragraph>

<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 

<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description. 
Most function groups are optimized for the same technologies, but the lists are function-specific; the use of a particular optimization in a single function 
does not indicate that all the functions in the group are optimized for that technology. When no list of optimizations is present, a function has only reference code. 
Absence of optimizations does not mean that a function runs more slowly -- use of a particular optimization may not increase the performance of the function.
</Paragraph>

<RefLink></RefLink>

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

<Paragraph>Refer to individual function descriptions for detailed information about return values.</Paragraph>

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

<Install>

<Heading1>
<H1Name>Installation and Operation</H1Name>
<H1Link>aa_006_install</H1Link>
</Heading1>

<Paragraph>This section contains general information about installing and using Framewave. Please refer to the README 
file in the installation package for the most recent information.</Paragraph>

<Heading2>
<H2Name>Library Files</H2Name>
<H2Link>aa_006_install</H2Link>
</Heading2>

<Paragraph>The include directory contains the following files.</Paragraph>

<Paragraph><Bold>fwBase.h </Bold>contains definitions of data types, data structures, 
	       enumerations, and declarations for core functions.</Paragraph>

<Paragraph><Bold>fwSignal.h </Bold>contains function declarations for signal processing.</Paragraph>

<ParagraphIndent>Essential Vector Calculation (Add, Sub, Or, Xor, etc.)</ParagraphIndent>
<ParagraphIndent>Fixed Accuracy Arithmetic (Inverse, Divide, Ln, etc.)</ParagraphIndent>
<ParagraphIndent>Vector Initialization Tasks (Copy, Move, etc.)</ParagraphIndent>
<ParagraphIndent>Domain Transform (Multiplication of vectors, etc.)</ParagraphIndent>

<Paragraph><Bold>fwImage.h </Bold>contains function declarations for image processing.</Paragraph>

<ParagraphIndent>Arithmetic and Logic operations (Add, Sub, Or, etc.)</ParagraphIndent>
<ParagraphIndent>Digital Filter Functions (Box Filter, Sobel Filter, etc.)</ParagraphIndent>
<ParagraphIndent>Color Model Conversion (RGBToYUV, RGBToYCbCr, etc.)</ParagraphIndent>
<ParagraphIndent>Threshold and Compare</ParagraphIndent>
<ParagraphIndent>Morphologic Operations</ParagraphIndent>
<ParagraphIndent>Geometric Operations</ParagraphIndent>

<Paragraph><Bold>fwVideo.h </Bold>contains function declarations for video processing.</Paragraph>

<ParagraphIndent>H.264 Decoder</ParagraphIndent>
<ParagraphIndent>H.264 Inverse Transform</ParagraphIndent>
<ParagraphIndent>H.264 Reconstruction</ParagraphIndent>
<ParagraphIndent>H.264 Intra-predict</ParagraphIndent>
<ParagraphIndent>H.264 Interpolation</ParagraphIndent>
<ParagraphIndent>H.264 Deblock filters</ParagraphIndent>
<ParagraphIndent>Motion Compensation</ParagraphIndent>
<ParagraphIndent>Motion Estimation</ParagraphIndent>
<ParagraphIndent>MPEG-1:Decoder, Inverse DCT, Inverse Quantization, Reconstruction of DCT block, Variable Length decoding</ParagraphIndent>
<ParagraphIndent>MPEG-2:Decoder, Inverse DCT, Inverse Quantization, Reconstruction of DCT block, Variable Length decoding</ParagraphIndent>

<Paragraph><Bold>fwJPEG.h </Bold>contains function declarations for JPEG processing.</Paragraph>

<ParagraphIndent>JPEG Color Conversion</ParagraphIndent>
<ParagraphIndent>JPEG Sampling</ParagraphIndent>
<ParagraphIndent>JPEG Level Shift, Planar-to-Pixel and Pixel-to-Planar</ParagraphIndent>
<ParagraphIndent>Quantization and Combined DCT</ParagraphIndent>
<ParagraphIndent>Huffman Codec</ParagraphIndent>

<Heading2>
<H2Name>Library Directory Hierarchy</H2Name>
<H2Link>aa_install_002</H2Link>
</Heading2>

<Paragraph>The directory hierarchy for each type of installation is as follows.</Paragraph>

<Paragraph><Bold>Microsoft&#174; Windows&#174; Operating Systems</Bold></Paragraph>

<ParagraphIndent>DLL Directories</ParagraphIndent>
<ParagraphIndent>The fwImage/fwSignal/fwBase/fwJPEG/fwVideo.lib files contain the 
import address table (IAT) for the corresponding functions. Rather than fetching the 
address of the function during run time, the .lib files are used to link against a DLL at compile time.</ParagraphIndent>

<ParagraphIndent>The fwImage/fwSignal/fwBase/fwJPEG/fwVideo.dll files contain implemented functions.</ParagraphIndent>

<Paragraph><Bold>Linux&#174; Operating Systems</Bold></Paragraph>

<ParagraphIndent>LIB Directories</ParagraphIndent>

<ParagraphIndent>The libfwImage.so/libfwSignal.so/libfwBase.so/libfwJPEG.so/libfwVideo.so files contain all shared libraries.</ParagraphIndent>

<Paragraph><Bold>Sun&#174; Solaris&#174; Operating System</Bold></Paragraph>

<ParagraphIndent>LIB Directories</ParagraphIndent>

<ParagraphIndent>The libfwImage.so/libfwSignal.so/libfwBase.so/libfwJPEG.so/libfwVideo.so files contain all shared libraries.</ParagraphIndent>

<Paragraph><Bold>Apple&#174; Mac OS&#174; Operating Systems</Bold></Paragraph>

<ParagraphIndent>LIB Directories</ParagraphIndent>

<ParagraphIndent>The libfwImage.dylib/libfwSignal.dylib/libfwBase.dylib/libfwJPEG.dylib/libfwVideo.dylib files contain all shared libraries.</ParagraphIndent>

<Heading2>
<H2Name>Setting Up Framewave</H2Name>
<H2Link>aa_install_003</H2Link>
</Heading2>

<Paragraph>Set up each type of installation as follows.</Paragraph>

<Paragraph><Bold>Microsoft&#174; Windows&#174; Operating Systems</Bold></Paragraph>
<Paragraph>Make sure the DLL files are in the search PATH using one of the following methods.</Paragraph>
<BulletedList>  
<Bullet>Copy the DLL files to the same folder as the project executable.</Bullet> 
<Bullet>Copy the DLL files to a folder that in the search path 
(for example, the Windows folder).</Bullet>
<Bullet>Modify the PATH environment variable to include the location of the Framewave DLLs 
(<Bold>Control Panel&gt;System&gt;Advanced&gt;Environment Variables</Bold>).</Bullet>
</BulletedList>
<Paragraph>Add the Framewave folder to the include search path within Microsoft Visual Studio (<Bold>Project&gt;Properties&gt;Configuration Properties&gt;C/C++&gt;General&gt; 
Additional Include Directories</Bold>).</Paragraph>

<Paragraph>Add the Framewave lib folder to the libraries search path within Visual Studio (<Bold>Project&gt;properties&gt;Configuration Properties&gt;Linker&gt;General&gt; 
Additional Library Directories</Bold>).</Paragraph>

<Paragraph><Bold>Linux&#174; Operating Systems</Bold></Paragraph>

<Paragraph>Assume this is a 64-bit installation and the installation directory is "ExampleDir".</Paragraph>

<Paragraph>To use the shared libraries, create the following symbolic links.</Paragraph>

<Pseudo>     cd ExampleDir/FW_1.0_Lin64/lib
     ln -sf ./libfwBase.so.1.0.0 libfwBase.so
     ln -sf ./libfwImage.so.1.0.0 libfwImage.so
     ln -sf ./libfwJPEG.so.1.0.0 libfwJPEG.so
     ln -sf ./libfwSignal.so.1.0.0 libfwSignal.so
     ln -sf ./libfwVideo.so.1.0.0 libfwVideo.so</Pseudo>
     
<Paragraph>Create similar symbolic links with the .so.1 extension.</Paragraph>

<Paragraph>To compile a cpp file that uses Framewave, for example test.cpp:</Paragraph>

<Pseudo>     g++ -m64 -c -IExampleDir/FW_1.0_Lin64 test.cpp</Pseudo>

<Paragraph>All Framewave libraries have dependency on fwBase.</Paragraph>

<ParagraphIndent>For example, to link with with the Image library,</ParagraphIndent> 
<Pseudo>     g++ -m64 -LExampleDir/FW_1.0_Lin64/lib test.o -lfwImage -lfwBase</Pseudo>

<Paragraph>It may be necessary to explicitly link in the stdc++, pthreads, 
or math libraries if they are not automatically linked in.</Paragraph> 

<Paragraph>Before running the application, make sure the ExampleDir/FW_1.0_Lin64/lib 
is in the shared library search path for the environment.</Paragraph>
   
<Paragraph><Bold>Sun&#174; Solaris&#174; Operating Systems</Bold></Paragraph>

<Paragraph>Assume this is a 64-bit installation and the installation directory is "ExampleDir".</Paragraph>

<Paragraph>To use the shared libraries, create the following symbolic links.</Paragraph>

<Pseudo>     cd ExampleDir/FW_1.0_Sol64/lib
     ln -sf ./libfwBase.so.1.0.0 libfwBase.so
     ln -sf ./libfwImage.so.1.0.0 libfwImage.so
     ln -sf ./libfwJPEG.so.1.0.0 libfwJPEG.so
     ln -sf ./libfwSignal.so.1.0.0 libfwSignal.so
     ln -sf ./libfwVideo.so.1.0.0 libfwVideo.so</Pseudo>
     
<Paragraph>Create similar symbolic links with the .so.1 extension.</Paragraph>

<Paragraph>To compile a cpp file that uses Framewave, for example test.cpp:</Paragraph>

<Pseudo>     CC -m64 -c -IExampleDir/FW_1.0_Sol64 test.cpp</Pseudo>

<Paragraph>All Framewave libraries have dependency on fwBase.</Paragraph> 

<ParagraphIndent>For example, to link with with the Image library,</ParagraphIndent> 
<Pseudo>     CC -m64 -LExampleDir/FW_1.0_Sol64/lib test.o -lfwImage -lfwBase -lrt</Pseudo>

<Paragraph>Before running the application, make sure the ExampleDir/FW_1.0_Sol64/lib 
is in the shared library search path for the environment.</Paragraph>

<Paragraph><Bold>Apple&#174; Mac OS&#174; Operating Systems</Bold></Paragraph>

<Paragraph>Assume this is a 64-bit installation and the installation directory is "ExampleDir".</Paragraph>

<Paragraph>To use the shared libraries, create the following symbolic links.</Paragraph>

<Pseudo>     cd ExampleDir/FW_1.0_Mac64/lib
    ln -sf ./libfwBase-1.0.dylib libfwBase.dylib
    ln -sf ./libfwImage-1.0.dylib libfwImage.dylib
    ln -sf ./libfwJPEG-1.0.dylib libfwJPEG.dylib
    ln -sf ./libfwSignal-1.0.dylib libfwSignal.dylib
    ln -sf ./libfwVideo-1.0.dylib libfwVideo.dylib</Pseudo>
     
<Paragraph>Create similar symbolic links with the .1.dylib extension.</Paragraph>

<Paragraph>To compile a cpp file that uses Framewave, for example test.cpp:</Paragraph>

<Pseudo>     g++ -m64 -c -IExampleDir/FW_1.0_Mac64 test.cpp</Pseudo>

<Paragraph>All Framewave libraries have dependency on fwBase.</Paragraph> 

<ParagraphIndent>For example, to link with with the Image library,</ParagraphIndent> 
<Pseudo>     g++ -m64 -LExampleDir/FW_1.0_Mac64/lib test.o -lfwImage -lfwBase</Pseudo>

<Paragraph>It may be necessary to explicitly link in the stdc++, pthreads, 
or math libraries if they are not automatically linked in.</Paragraph> 

<Paragraph>Before running the application, make sure the ExampleDir/FW_1.0_Mac64/lib 
is in the shared library search path for the environment.</Paragraph>

<Heading2>
<H2Name>Linker Issues</H2Name>
<H2Link>aa_install_004</H2Link>
</Heading2>

<Paragraph>This section describes how to resolve errors that occur while linking code to 
the Framewave static library for Microsoft&#174; Windows&#174; operating systems. 
This information does not apply to code that links to the dynamic DLL (shared) version of 
the Framewave library for Microsoft Windows, or to versions of the Framewave library for other operating systems.</Paragraph>

<Paragraph>Framewave static libraries for Microsoft Windows have a dependency on Microsoft Visual C++ 2005 
C run-time libraries. The project (executable or DLL) also has a dependency on the C run-time library. 
The linker automatically links to the run-time library used by the static library as well as to the 
run-time library specified in the project settings. A version mismatch between the library in the static 
library and the library specified in the project can cause the linker to display error messages and terminate.</Paragraph>

<Paragraph>To resolve this issue, use a static run-time library in the project.</Paragraph>

<ParagraphIndent>When linking to a release version of the static Framewave library, use the /MT compiler switch 
in the project settings. This links the project to the same static multithreaded version of the run-time 
Framewave library used by the static Framewave library.</ParagraphIndent>

<ParagraphIndent>When linking to a debug version of the static Framewave library (common in debug configurations), 
use the /MTd compiler switch in the project settings.  This links the project to the static multithreaded 
debug version of the run-time library.</ParagraphIndent>

<Paragraph>Most code generated by the Visual C++ compiler has some dependency on the C runtime libraries. 
Microsoft provides several different flavors of the run-time libraries (static vs. dynamic (DLL), debug vs. release), 
and functions are implemented differently in each flavor. These differences can cause linker errors if a project is 
linked to the wrong library flavor. The runtime library flavor is normally specified by a compiler switch 
(/MT, /MTd, /MD, /MDd). Mixing different library types in the same module (executable or DLL) can also lead to various 
linker or run-time issues, and is not recommended.</Paragraph>

<Paragraph>For more information see: </Paragraph>

<ParagraphIndent><hlink><uri>http://msdn2.microsoft.com/en-us/library/2kzt1wy3(VS.80).aspx</uri><target>_blank</target><desc>http://msdn2.microsoft.com/en-us/library/2kzt1wy3(VS.80).aspx</desc></hlink></ParagraphIndent>
<ParagraphIndent><hlink><uri>http://support.microsoft.com/kb/154753</uri><target>_blank</target><desc>http://support.microsoft.com/kb/154753</desc></hlink></ParagraphIndent>

</Install>

*/

};
