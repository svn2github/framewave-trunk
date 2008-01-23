Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.


/*****************************************************************************/ 
 *                                                                           *
 *  This file explains the build procedure and                               *
 *  usage of the FW sample application                                      *
 *                                                                           *
 *  The following topics are covered:                                        *
 *                                                                           *
 *  1. Directory Structure                                                   * 
 *  2. Build Steps                                                           *
 *  3. Program Usage                                                         *
 *  4. Options                                                               *
 *  5. Program Notes                                                         *
 *  6. Examples                                                              *
 *  7. Limitations                                                           *
 *                                                                           *
/*****************************************************************************/ 

1. Directory Structure
----------------------

The project uses the following directory structure:


 FW_SampleConvert
   |
   |---------SRC 
   |          |
   |          |-----*.cpp
   |          
   |          
   |           
   |---------INC            
   |          |
   |          |-----globals.h
   |          |
   |          |-----process_API.h
   |
   |---------bin
   |		|
   |		|	   
   |		|---------X32---------Linux/Solaris/Mac executable is created here
   |		|	      |
   |           	|	      |----Debug-----sample.exe(Windows debug Executable is created here)       
   |		|	      |
   |		|	      |----Release-----sample.exe(Windows release Executable is created here)
   |		|
   |		|          
   |		|---------X64----------Linux/Solaris/Mac executable is created here
   |		|	      |
   |           	|	      |----Debug-----sample.exe(Windows debug Executable is created here)       
   |		|	      |
   |		|	      |----Release-----sample.exe(Windows release Executable is created here)
   |		|             |
   |
   |           
   |---------butterfly.bmp(sample image file)




FW header fiels (fwImage.h fwBase.h) and FW binaries(fwBase and fwImage) come with the fw installation and are not part of the SampleConvert. 


2. Build Steps
--------------

Use one of the following sequences when building the project:

For Microsoft(R) Windows(R):

 1. Open sample sample.sln 
 2. Select the project configuration as 'Debug'OR 'Release'.
 3. Edit the 'additional library directories' section in the project's 
    Property Page to locate the FW libraries.
 4. Edit the 'additional include directories' section in the project's 
    Property Page to locate the FW header files    
 5. Build the project
 6. Use the application with a BMP/YUV file

Note: to run the sample application, PATH environment variable has to be set to locate the FW dll in case of dynamic linking. 

For LINUX(R):

 1. Edit the LinMakefile to change BITS_PROCESSOR depending on 32 bit/64 bit target build.
    For example: For 64-bit build assign BITS_PROCESSOR=64
 2. Edit the LinMakefile to change the FW_LIB_PATH to point to the location of the libraries.
    For example: if the FW is installed at /home/fw/libs32/ then assign FW_LIB_PATH=/home/fw/libs32/
 3. Edit the Makefile to change the FW_INC_PATH to point to the location of the FW header files.
    For example: if the header files are at /home/fw/headerfiles/ then assign FW_INC_PATH=/home/fw/headerfiles/    
 4. Edit the LinMakefile to select linker option for dynamic/static
    For example: if the linker option is selected as dynamic then assign LIB_TYPE=dynamic
 5. Build the release version of sample using LinMakefile (use the command "make -f LinMakefile")
 6. Use the application with a BMP/YUV files 
 
Note: to run the sample application Set the LD_LIBRARY_PATH environment variable to locate the FW libraries in case of dynamic linking. 

FOR Solaris(R):

 1. Edit the SolMakefile to change BITS_PROCESSOR depending on 32 bit/64 bit target build.
    For example: For 64-bit build assign BITS_PROCESSOR=64
 2. Edit the SolMakefile to change the FW_LIB_PATH to point to the location of the libraries.
    For example: if the FW is installed at /home/FW/libs32/ then assign FW_LIB_PATH=/home/FW/libs32/
 3. Edit the Makefile to change the FW_INC_PATH to point to the location of the FW header files.
    For example: if the header files are at /home/FW/headerfiles/ then assign FW_INC_PATH=/home/FW/headerfiles/    
 4. Edit the SolMakefile to select linker option for dynamic/static
    For example: if the linker option is selected as dynamic then assign LIB_TYPE=dynamic
 5. Build the release version of sample using SolMakefile (use the command "make -f SolMakefile")
 6. Use the application with a BMP/YUV files 

Note: 1)to run the sample application Set the LD_LIBRARY_PATH environment variable to locate the FW libraries in case of dynamic linking. 
 
FOR Mac(R):

 1. Edit the MacMakefile to change BITS_PROCESSOR depending on 32 bit/64 bit target build.
    For example: For 64-bit build assign BITS_PROCESSOR=64
 2. Edit the SolMakefile to change the FW_LIB_PATH to point to the location of the libraries.
    For example: if the FW is installed at /home/FW/libs32/ then assign FW_LIB_PATH=/home/FW/libs32/
 3. Edit the Makefile to change the FW_INC_PATH to point to the location of the FW header files.
    For example: if the header files are at /home/FW/headerfiles/ then assign FW_INC_PATH=/home/FW/headerfiles/    
 4. Edit the SolMakefile to select linker option for dynamic/static
    For example: if the linker option is selected as dynamic then assign LIB_TYPE=dynamic
 5. Build the release version of sample using SolMakefile (use the command "make -f MacMakefile")
 6. Use the application with a BMP/YUV files 

Note: 1)to run the sample application Set the DYLD_LIBRARY_PATH environment variable to locate the FW libraries in case of dynamic linking.  

3. Program Usage 
----------------

Use the following syntax to invoke the sample project:

sample.exe -input <input file> [options][sub_options] -output <output file> 


4. Options
----------

The following option arguments [options] are available:

-addimage <file>                   Add the image specified by <file> to the <input file> 

-addC <val1> <val2> <val3>         Add a specified constant value to the <input file> 

                                   <val1> specifies the value to be added to the 'RED'
                                   color component of the <input file>

                                   <val2> specifies the value to be added to the 'GREEN'
                                    color component of the <input file>

                                   <val3> specifies the value to be added to the 'BLUE'
                                   color component of the <input file>

-blinds <val> <R> <G> <B>          Create blinds effect on the image, in either the
                                   horizontal (0) or vertical (1) direction
 
                                   <R> specifies the value of 'RED'color component in the blinds

                                   <G> specifies the value of 'GREEN' color component in the blinds

                                   <B> specifies the value of 'BLUE'  color component in the blinds

-blur                              Blur the image using 3x3 mask

-bright <val>                      Adjust the brightness of an image by <val>
 
                                   The range of brightness values is -255 to +255

-border <val1> <val2> <val3>       Add a border of color specified by constant value from the
                                   <val1> <val2> <val3> arguments
 
-contrast <val>                    Adjust the contrast of an image by <val>

                                   The range of contrast values is -100 to +100

-crop <width> <height>             Crop the image by an area specified by <width> and <height>
  
-deinterlace <val>                 Interpolate an image when the ODD (1) or EVEN (0) field in
                                   an image is missing

-emboss                            Perform embossing effect on an image
 
-explode                           Perform explosion effect on an image
 
-edge                              Detect the edges of an image
  
-filter_column                     Perform Column filtering
  
-filter_gauss                      Perform Gaussian filtering using a 3x3 kernel
 
-filter_laplace                    Perform laplacian filtering using a 3x3 kernel
 
-filter_lowpass                    Perform lowpass filtering using a 3x3 kernel
 
-filter_hipass                     Perform highpass filtering using a 3x3 kernel
 
-filter_max                        Perform filtering with maximum value in the mask
 
-filter_medcolor                   Perform Median Color filtering using a 3x3 kernel
 
-filter_medcross                   Perform Median Cross filtering using a 3x3 kernel
 
-filter_medhoriz                   Perform Median Horizontal filtering using a 3x1 kernel
 
-filter_median                     Perform Median filtering using a 3x3 kernel
 
-filter_medvert                    Perform Median Vertical filtering using a 1x3 kernel
 
-filter_min                        Perform filtering with minimum value in the mask
 
-filter_prewhoriz                  Perform Prewitts horizontal filtering
 
-filter_prewvert                   Perform Prewitts vertical filtering
 
-filter_rectkernel                 Perform filtering using rectangular kernel
 
-filter_robdown                    Perform Roberts Down filtering
  
-filter_robup                      Perform Roberts Up filtering
  
-filter_row                        Perform Row filtering
  
-filter_sobhoriz                   Perform Sobel Horizontal filtering
  
-filter_sobvert                    Perform Sobel Vertical filtering
  
-flip <val>                        Flip the image on horizontal (0), vertical (1), or both axes(2)
 
-gamma <val>                       Perform gamma correction on an image by <val>
 
                                   The range of gamma correction values is 0 to 10
 
-glassy <width> <height>           Add glassy effect on an image of dimension <width> and <height>
  
                                   The range of glassy effect values is 5 to 75.
 
-grey                              Convert an image to grey scale
 
-grid <val>                        Form a three-dimensional grid on an image with the length
                                   specified by <val>

                                   The grid value must be a number between 10 and 100
 
-halftone                          Create a halftone of the image
  
-hue <val>                         Perform hue adjustment on an image as specified by <val>

                                   The hue value must be a number between 0 and 360

-illusion <val>                    Perform illusion effect on an image as specified by <val>
 
                                   The effect value must be a number between 0 and 100

-negate                            Perform bitwise negation of the <input file>

-noise <val>                       Add random noise to an image as specified by <val>
 
                                   The noise value must be a number between 0 and 100

-remove <string>                   Remove the color component specified by <string> 
                                   from the <input file>
 
                                   The value <string> can be 'RG', 'RB', 'B', etc.

                                   Examples:

                                     'RG' - Remove  RED  and  GREEN  color component

                                     'B' - Remove  BLUE  color component

-resize <Xval> <Yval>              Resize an image by x an y factors <Xval> and <Yval>'


-rotate <angle>                    Rotate the image by an angle specified by <angle>

-saturate <val>                    Perform saturation on an image as specified by <val>
 
                                   The saturation value must be in the range 0.0 to 2.0

-sharp                             Sharpen an image using the Sharpen Filter

-subimage <file>                   Subtract an image in the <input file> from the image
                                   specified by <file>
 
-subC <val1> <val2> <val3>         Subtract a specified constant value from the <input file> 
                                   <val1> specifies the value to be subtracted from the RED
                                   color component

                                   <val2> specifies the value to be subtracted from the GREEN
                                   color component

                                   <val3> specifies the value to be subtracted from the BLUE
                                   color component

-video <val>                       Create an image using the STAGGERED(0), TRIPPED (1), 3x3 (2),
                                   or DOTTED (3) video effect

-xorimage <file>                   Perform a bitwise exclusive logical OR of the image specified
                                   by <file> and the <input file>

sub_options
-----------
--roi <width> <height>             Perform all operations on an ROI in <input file>
                                   specified by <width> and <height>
				   


5. Program Notes
----------------

The following special conditions apply to the sample project:

1. The ROI is the entire image area of <input file> if it is not explicitly mentioned.

2. When the ROI is larger than the actual image size, the actual image size is treated as the ROI.

3. When multiple options are present, the output of the previous operation is the input to the current operation.

4. The syntax for specifying YUV files is YUV_file.yuv <Width> <Height> <Format>.

5. Planar YUV files with 422, 420 and 444 formats are supported.

6. File extensions specify the color space of the input and output image.



6. Examples 
-----------

The following are examples of syntax that can be used to invoke the sample project:

1. sample.exe -input in.bmp -add in_add.bmp -remove R -output out.bmp

   Add 'in_add.bmp' to 'in.bmp', remove the RED color component, 
   then write the resultant to 'out.bmp'

   For all these operations, ROI = width and height of 'in.bmp'

2. sample.exe -input in.bmp -negate -crop 200 100 --roi 400 300 -output out.bmp

   Negate 'in_.bmp' for an area of W=400 and H=300, crop an area of W=200 and H=100, 
   then write the resultant to 'out.bmp'

3. sample.exe -input in.yuv 352 288 420 -add in_add.yuv 176 144 444  -output out.yuv 352 244 420

     Add 'in_add.yuv' to 'in.yuv', then write the resultant to 'out.yuv'

4. sample.exe -input in.yuv 352 288 420 -output out.bmp

     Convert  in.yuv to 'out.bmp' --- YUV to RGB color space


7. Limitations
--------------

The following limitations apply to the sample project:

1. This application supports only 24 bpp BMP and YUV files.

2. This application does not validate command line parameters.

3. In Solaris all the filter options, -blur and -sharp are disabled. 
   

Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.

