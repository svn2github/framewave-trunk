/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

/*#DataBlock

#Parameters

pSrc - Pointer to a location in a source buffer.
pSrc1 - Pointer to a location in source buffer one.
pSrc2 - Pointer to a location in source buffer two.
pSrc[3] - Pointer to a location in a three-channel planar source buffer (array values point to a location in each plane).
pSrc[4] - Pointer to a location in a four-channel planar source buffer (array values point to a location in each plane).
pDst - Pointer to a location in a destination buffer.
pDst1 - Pointer to a location in destination buffer one.
pDst2 - Pointer to a location in destination buffer two.
pDst[3] - Pointer to a location in a three-channel planar destination buffer (array values point to a location in each plane).
pDst[4] - Pointer to a location in a four-channel planar destination buffer (array values point to a location in each plane).
pSrcDst - Pointer to a location in a buffer that contains both the source and destination. 
pMask - Pointer to a location in a buffer that contains a mask to be used in the operation.


srcStep - Source buffer step size (width of the buffer in bytes).
src1Step - Source buffer one step size (width of the buffer in bytes).
src2Step - Source buffer two step size (width of the buffer in bytes).
srcStep[3] - Source three-channel planar buffer step size (array values define width of each plane in bytes).
dstStep - Destination buffer step size (width of the buffer in bytes).
dst1Step - Destination buffer one step size (width of the buffer in bytes).
dst2Step - Destination buffer two step size (width of the buffer in bytes).
dstStep[3] - Destination three-channel planar buffer step size (array values define width of each plane in bytes).
srcDstStep - Source and destination buffer step size in bytes (width of both buffers in bytes).
maskStep - Mask buffer step size in bytes (width of the buffer in bytes).

alpha - Alpha (RGBA transparency) constant value.
const - Constant value.
value - Specified value.
value[3] - An array of three specified values.
value[4] - An array of four specified values.

roiSize - Specifies the height and width of an ROI.
imgSize - Specifies the dimensions of an entire image.
interpolation - Specifies the method of interpolation.
scaleFactor - Specifies the integer scaling factor for the Sfs operation. The returned result is multiplied by 2^(-scaleFactor).

angle - Specifies the rotation angle in degrees, counterclockwise.
bound[2][2] - Boundary rectangle vertex coordinates for a transformed source ROI.
coeffs[2][3] - Coefficients for Warp Affine transform.
coeffs[2][4] - Coefficients for Warp Affine Bilinear transform.
coeffs[3][3] - Coefficients for Warp Affine Perspective transform.  
coeffs[3] - Transformation coefficient values.
dstQuad[4][2] - Destination quadrangle vertex coordinates.
srcQuad[4][2] - Source quadrangle vertex coordinates.
quad[4][2] - Quadrangle vertex coordinates.
dstRoi - Destination image ROI.
srcRoi - Source image ROI.
dstRoiSize - Destination image ROI size. 
dstSize - Destination image size. 
srcSize - Source image size.
flip - Image mirror flip mode. 
nChannel - Number of channels in the image.
pBuffer - Pointer to the location of a temporary buffer.
pBufferSize - Pointer to the size of a temporary buffer.
xCenter - X coordinate of center.
yCenter - Y coordinate of center.
xShift - Shift value for X axis direction.
yShift - Shift value for Y axis direction.
xShear - Shear value for X axis in Shear transformation.
yShear - Shear value for Y axis in Shear transformation.
xFactor - Factor value for X axis direction.
yFactor - Factor value for Y axis direction .
xFr - Inverse factor value for X axis direction.
yFr - Inverse factor value for Y axis direction .
pxMap - Pointer to X axis mapping table array.   
pyMap - Pointer to Y axis mapping table array.
xMapStep - X axis mapping table step size (width of the buffer in bytes). 
yMapStep - Y axis mapping table step size (width of the buffer in bytes).  

offset - A floating-point value that specifies the starting offset for ImageRamp functions.
axis - An enumeration that describes the type of gradiation in ImageRamp functions.
slope - A floating-point value that specifies the amount of gradiation in ImageRamp functions.

hint - Suggests a level of precision using one of the following values (fwAlgHintNone, fwAlgHintFast, fwAlgHintAccurate).
vMin - Minimum value.
vMax - Maximum value.

maskSize - Specifies the size of a mask used in calculations.
pKernel - Pointer to the kernel array.
kernelSize - Specifies the size of the kernel used in calculations.

eps - Tolerance value.
fwCmpOp - Specifies a comparison operation to be performed (fwCmpLess, fwCmpGreater, fwCmpLessEq, fwCmpEq, or fwCmpGreaterEq).
threshold - Threshold for each pixel.											   	
threshold[3] - Three-channel threshold for each pixel.												   
threshold[4] - Four-channel threshold for each pixel.												   	
thresholdGT - Three-channel threshold for each pixel in greater-than operation.											   		
thresholdGT[3] - Three-channel threshold for each pixel in greater-than operation.												   
thresholdLT - Threshold for each pixel in less-than operation.
thresholdLT[3] - Three-channel threshold for each pixel in less-than operation.
valueGT - Threshold for each pixel in greater-than operation.
valueGT[3] - Three-channel threshold for each pixel in greater-than operation.
valueLT - Threshold for each pixel in less-than operation.
valueLT[3] - Three-channel threshold for each pixel in less-than operation.

dstOrder[3] - Specifies destination buffer channel order. For example, dstOrder[1] = 0 indicates that source channel 0 is destination channel 1.

pState - Pointer to the morphology state structure.
ppState - Double pointer to the morphology state structure.
anchor - A pixel locating the kernel (mask) according to source image.
border - The set of source image boundary pixels.
borderType - Specifies a boundary type.
mask - An analysis window (only non-zero mask array values are processed).
pSize - Pointer to the size of an internal structure.
roiWidth - Width of ROI image in pixels.

divisor - An integer value by which a result is divided.
borderValue - A constant value assigned to the pixels in a constant border (not used by any other border type).
borderValue[3] - A three-channel constant value assigned to the pixels in a constant border (not used by any other border type).
ppDst - Double pointer to the destination image ROI.
srcSize1 - Specifies the size of source image one.
srcSize2 - Specifies the size of source image two.
srcStep1 - Source image one step size (width of the buffer in bytes).
srcStep2 - Source image two step size (width of the buffer in bytes).
xAnchor - Anchor cell that specifies the horizontal alignment of the kernel with respect to the input pixel.
yAnchor - Anchor cell that specifies the vertical alignment of the kernel with respect to the input pixel.

heightPixels - Height of an image (number of pixels in one column of image).
widthPixels - Width of an image (number of pixels in one row of image).
pStepBytes - Pointer to the step size, in bytes.
ptr - Pointer to a buffer.
pSum - Pointer to the sum of values.
cubeDimensions[3] - dimensions of the 3D-LUT cube in x, y and z directions .(eg. (17,17,17), (33,33,33) etc)
cubeMax[3] - The maximum values that can occur in 3D-LUT cube in x, y and z directions.(eg. (1023,1023,1023),4095,4095,4095) for 10 bit and 12 bit outputs respectively)
pCube - Pointer to the 3d_LUT cube. The pixel values in the table should be filled along z-axis first, then along y-axis and finally along x-axis
pLUTSpec - pointer to the FwiLUTSpec structure
ppLUTSpec - pointer to a pointer to FwiLUTSpec structure

#DataStructures

FwiPoint - The point represented by (x, y).
FwiSize - The size of a rectangle represented by (width, height).
FwiRect - The position and size of a rectangle represented by (x, y, width, height).
	
FwiEncodeHuffmanSpec - Huffman encoder specification.
FwiEncodeHuffmanState - Huffman encoder state.
FwiDecodeHuffmanSpec - Huffman decoder specification.
FwiDecodeHuffmanState - Huffman decoder state.
FwiMorphState - Morphology state.
FwiMorphAdvState - Advanced morphology state.
FwiMorphGrayState - Advanced morphology state.
FwiLUTSpec - Internal data structure for 3D-LUT table.

#Enumerators

FwiAxis - Image intensity ramp for fwiImageRamp functions or flip axes for fwiMirror functions.
FwiBorderType - Border type for filter functions.
FwiMaskSize - Neighborhood area for filter and morphological functions.
FwiAlphaType - Type of compositing operation for alpha composition functions.
FwiDitherType - Type of dithering for fwiReduceBits function.

*/

/*#Documentation

<SectionHeading>Image Processing Library</SectionHeading>

<Paragraph>Image Processing Library<SectDesc> functions perform a variety of tasks related to image processing.</SectDesc></Paragraph>

<Paragraph>This section is organized as follows.</Paragraph>

<SectLinks></SectLinks>

<Paragraph>Within the section, the <Bold>Basic Concepts</Bold> chapter provides overview information related to the functions in the library, 
and subsequent chapters provide detailed descriptions of library functions that perform operations of the same kind.</Paragraph>
<Paragraph>Each detailed description consists of a <Bold>Function Name</Bold> followed by a short description, a <Bold>Synopsis</Bold> of the function syntax, 
a list of function <Bold>Parameters</Bold>, a detailed <Bold>Description</Bold> of the function, and a list of <Bold>Return Values.</Bold></Paragraph> 
<Paragraph>When a function is optimized for one or more technologies, a list of <Bold>Supported Technologies</Bold> is provided after the short description.</Paragraph>

<RefLink></RefLink>

<Chapter>
<ChapterHeading>Basic Concepts</ChapterHeading>

<Paragraph>This <ChaptDesc> chapter provides an overview of the information contained in the functional descriptions.</ChaptDesc></Paragraph>

<Paragraph>Image library functional descriptions include the following types of information.</Paragraph>

<Heading2>
<H2Name>Data Structures</H2Name>
<H2Link>image_structures</H2Link>
</Heading2>

<Paragraph>Image library function definitions use the following data structures.</Paragraph>

#PrintAllDataStructures

<Heading2>
<H2Name>Enumerators</H2Name>
<H2Link>image_enumerators</H2Link>
</Heading2>

<Paragraph>Image library function definitions use the following enumerators.</Paragraph>

#PrintAllEnumerators

<Heading2>
<H2Name>Color Channel Buffers</H2Name>
<H2Link>image_channel_buffers</H2Link>
</Heading2>

<Paragraph>Image library function definitions use the following color channel buffer types.</Paragraph>

   <DefinitionList>
     <DefinitionEntry>
       <Term><DataType>C1</DataType></Term>
       <Definition>Single-channel color</Definition>
     </DefinitionEntry>
      <DefinitionEntry>
       <Term><DataType>C2</DataType></Term>
       <Definition>Two-channel color</Definition>
     </DefinitionEntry><DefinitionEntry>
       <Term><DataType>C3</DataType></Term>
       <Definition>Three-channel color</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><DataType>C4</DataType></Term>
       <Definition>Four-channel color</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><DataType>AC4</DataType></Term>
       <Definition>Three-channel color with an additional alpha (transparency) channel</Definition>
     </DefinitionEntry>
   </DefinitionList>

<Paragraph>A channel is a pixel-ordered grayscale representation of a single color. 
The RGB color model uses three channels, one each for red, green, and blue color data. 
RGBA adds a fourth channel for transparency.</Paragraph>

<Paragraph>Image processing functions generally show source and destination buffers in a combined pair, such as C4P4.
Use of a single buffer type indicates either that the source and destination buffers are
of the same type, or that the function writes data back to the source location.</Paragraph>

<Heading2>
<H2Name>Color Plane Buffers</H2Name>
<H2Link>image_plane_buffers</H2Link>
</Heading2>

<Paragraph>Image library functions use the following color plane buffer types.</Paragraph>

   <DefinitionList>
     <DefinitionEntry>
       <Term><DataType>P1</DataType></Term>
       <Definition>Single-plane color</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><DataType>P3</DataType></Term>
       <Definition>Three-plane color</Definition>
     </DefinitionEntry>
     <DefinitionEntry>
       <Term><DataType>P4</DataType></Term>
       <Definition>Four-plane color</Definition>
     </DefinitionEntry>
   </DefinitionList>

<Paragraph>Planes map color spaces. A color space is defined by a color model and a gamut, or range of colors. 
Many color spaces can be represented by three planes (XYZ coordinated), but there are spaces of
greater and lesser dimensions. A single-plane color can be viewed as a slice through a larger color space.</Paragraph>

<Paragraph>Functions generally show source and destination buffers in a combined pair, such as P4C4.
Use of a single buffer type indicates either that the source and destination buffers are of the same type,
or that the function writes data back to the source buffer.</Paragraph>

<Heading2>
<H2Name>Regions of Interest</H2Name>
<H2Link>image_roi</H2Link>
</Heading2>

<Paragraph>Many image library functions use the concept of a Region of Interest 
 (ROI). A programmer can choose to manipulate specific regions of an image 
 buffer rather than the entire image. The following figure shows an ROI 
 for a typical in-place (same source and destination) operation.</Paragraph>

<Figure>
	<SRC>roi_diag.gif</SRC>
	<ALT>ROI Diagram</ALT>
	<BORDER>0</BORDER>
</Figure>

<Paragraph>The outer rectangle represents an image buffer. The inner grey rectangle represents the ROI inside
the buffer.</Paragraph>

<Paragraph>Four items of information are required to define the ROI:</Paragraph>

<ParagraphIndent>A pointer to the starting pixel of the ROI (pSrcDst).</ParagraphIndent>
<ParagraphIndent>The number of bytes from the starting pixel of the ROI to the end of that image row (srcDstStep).</ParagraphIndent>
<ParagraphIndent>The number of pixels in one row of the ROI (roiSize.width).</ParagraphIndent>
<ParagraphIndent>The number of pixels in one column of the ROI (roiSize.height).</ParagraphIndent>

<Paragraph>ROIs for operations that use separate source and destination buffers are defined in the same way.</Paragraph>

<Heading2>
<H2Name>Image Masking</H2Name>
<H2Link>image_mask</H2Link>
</Heading2>

<Paragraph>In an iterative operation that involves image buffers, it may be necessary to skip operating on certain portions of the ROI. 
This is accomplished by using a mask of the same size as the ROI. When a mask is used, the function writes a result to a destination pixel 
only when the corresponding pixel value in the mask is non-zero. The function does not write a result to a destination pixel when the 
corresponding pixel value in the mask is zero.</Paragraph>

<Heading2>
<H2Name>Borders In Image Functions</H2Name>
<H2Link>image_borders</H2Link>
</Heading2>

<Paragraph>Many Framewave image processing functions that use the values of adjscent pixels to calculate a destination value require 
source images with defined border areas. Image processing functions typically assume that referenced adjacent pixels are available, but this is not always the case. 
The area on which a function operates is defined by ROI position, by kernel or mask size, and by anchor cell position. 
The following diagram shows a source image with borders, a destination image, and a kernel mask with anchor points.</Paragraph>

<Figure>
	<SRC>border_dwg.gif</SRC>
	<ALT>Border Diagram</ALT>
	<BORDER>0</BORDER>

</Figure>

<Paragraph>Anchors position the kernel so that an anchor cell coincides with an input pixel. 
However, when an input pixel is near the edge of an image, the kernel can include adjacent pixels locations that are 
outside the source image or ROI. Data for these adjacent locations must be provided in order for the function to work correctly.</Paragraph>
 
<Paragraph>The general approach is to first determine whether additional border pixels are required, then define the pixels as needed. 
When an ROI includes boundary pixels, border extension is required; when an ROI does not include boundary pixels, border extension in not required. 
To define the border pixels, either use an Framewave function that fills in the border with defined pixel values, or apply a locally-developed extension method.</Paragraph>
 
<Paragraph>The following order fill methods are used.</Paragraph>
 
<ParagraphIndent><Bold>Zero fill</Bold> - the border area is extended with zeros.</ParagraphIndent>
<ParagraphIndent><Bold>Constant fill</Bold> - the border area is extended with a specified constant value.</ParagraphIndent>  
<ParagraphIndent><Bold>Extension</Bold> - the border area is created by copying the edge and corner pixels.</ParagraphIndent>
<ParagraphIndent><Bold>Reflection</Bold> - the border area is created by reflection of the outer edge of the source image.</ParagraphIndent>
<ParagraphIndent><Bold>Wrap</Bold> - the border area is created by toroidal wrapping of the image plane, effectively joining opposite edges of the image.</ParagraphIndent>

<Paragraph>For example, the <FunctionName>FilterSharpen</FunctionName> function always uses a 3X3 kernel mask 
with the anchor in the center, location (1,1) in zero-based coordinates, relative to the top left corner of the mask. 
For the function to operate correctly, the source image must have a complete one-pixel border on both sides, top and bottom.</Paragraph>

<Paragraph>Consider a <FunctionName>Filter32f</FunctionName> function with variable-size mask and anchor:</Paragraph>

<ParagraphIndent>Let the source image be 19 by 17 pixels.</ParagraphIndent>
<ParagraphIndent>Let the mask size be 4 by 5 pixels.</ParagraphIndent>
<ParagraphIndent>Let the anchor coordinates be (2,1) relative to the top left corner.</ParagraphIndent>

<Paragraph>The maximum ROI size that can be safely used with the source image is 16 by 13 pixels, and the minimum destination image size is 16 by 13.</Paragraph>

<Paragraph>Assume that:</Paragraph>
<ParagraphIndent><Italic>W<sub>src</sub></Italic> and <Italic>H<sub>src</sub></Italic> are the width and height of the source image in pixels.</ParagraphIndent>
<ParagraphIndent><Italic>W<sub>mask</sub></Italic> and <Italic>H<sub>mask</sub></Italic> are the width and height of the mask in pixels.</ParagraphIndent>
<ParagraphIndent><Italic>X<sub>anchor</sub></Italic> and <Italic>Y<sub>anchor</sub></Italic> are the zero-based coordinates of the anchor point, relative to the top left corner of the mask.</ParagraphIndent>
<ParagraphIndent><Italic>Channels</Italic> is the number of channels in the image (in non-planar format).</ParagraphIndent>
<ParagraphIndent><Italic>ElementSize</Italic> is the size of a single value in the source image, for example, four (bytes) for single precision floating point.</ParagraphIndent>
<Paragraph>The following formulas calculate the offset from the top left corner of the image to the top left corner of the non-border area of the image.</Paragraph>
<Paragraph><Italic>Offset</Italic> = <Italic>StepSize<sub>src</sub></Italic> x <Italic>Y<sub>anchor</sub></Italic> + <Italic>X<sub>anchor</sub></Italic> x <Italic>Channels</Italic> x <Italic>ElementSize</Italic></Paragraph>
<Paragraph>Where:</Paragraph>
<ParagraphIndent><Italic>StepSize<sub>src</sub></Italic> = <Italic>W<sub>src</sub></Italic> x <Italic>Channels</Italic> x <Italic>ElementSize</Italic></ParagraphIndent>
<Paragraph>Maximum ROI size is:</Paragraph>
<ParagraphIndent><Italic>W<sub>ROI</sub></Italic> = <Italic>W<sub>src</sub></Italic> - <Italic>W<sub>mask</sub></Italic> + 1</ParagraphIndent>
<ParagraphIndent><Italic>H<sub>ROI</sub></Italic> = <Italic>H<sub>src</sub></Italic> - <Italic>H<sub>mask</sub></Italic> + 1</ParagraphIndent>
<Paragraph>The destination image size is at least the size of the ROI.</Paragraph>
<Paragraph>Some Framewave functions assume that the anchor coordinates are relative to the bottom right corner of the mask. 
In this case, the formulas must be modified accordingly.</Paragraph>
<Paragraph>It is possible to have a source image with a much larger source step-size value than specified in the formula. 
This typically occurs when using an Framewave function to process a small part of a source image. 
In this case, the offset value can also be much larger, pointing somewhere in the middle of the source image data buffer. 
The developer must make sure there is a safe border area around the image that the function can safely read or modify.</Paragraph>
<Paragraph></Paragraph>

<Heading2>
<H2Name>Interpolation Modes</H2Name>
<H2Link>image_modes</H2Link>
</Heading2>

<Paragraph>The following interpolation modes are available in functions that use the interpolation parameter.</Paragraph>

<Paragraph>FWI_INTER_NN: nearest neighbor interpolation</Paragraph>
<Paragraph>FWI_INTER_LINEAR: linear interpolation</Paragraph>
<Paragraph>FWI_INTER_CUBIC: cubic interpolation</Paragraph>
<Paragraph>FWI_INTER_SUPER: supersampling</Paragraph>
<Paragraph>FWI_INTER_LANCZOS: interpolation with Lanczos window function</Paragraph>

<Heading2>
<H2Name>Parameter Glossary</H2Name>
<H2Link>image_parameters</H2Link>
</Heading2>

<Paragraph>Image library function definitions use the following parameters.</Paragraph>

#PrintAllParameters

</Chapter>

*/
