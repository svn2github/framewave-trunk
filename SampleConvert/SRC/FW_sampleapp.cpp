/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#include"globals.h"
#include"process_API.h"


/*!
 *************************************************************************************
 * \function main
 *
 * \brief
 *    This function processes all the command line arguments in the order that is
 *    specified in the command line .
 *
 *************************************************************************************
 */


int
main(int argc, char* argv[])
{
	en_options		options_arr[NUM_OPTIONS] ;
	char			outfile[800] ;


	for(int i=0; i<NUM_OPTIONS; i++)
	{
		options_arr[i] = DEF_VAL ;
	}

	/* Check Command line arguments */
	if ( check_command_line(argc, argv, &options_arr[0], &outfile[0] ) == RETN )
		return RETN ;

	/* Process Command line arguments */
	if ( process_options(argc, argv, &options_arr[0], outfile ) == RETN )
		return RETN ;

	return CONT ;
}


/*!
 *************************************************************************************
 * \function clear_bmp_struct
 *
 * \brief
 *    This function clears the bmp data structure.
 *
 *
 *************************************************************************************
 */
int
clear_bmp_struct(st_input_format *bmp_info_add)
{

	if( bmp_info_add->image_data  )
		free(bmp_info_add->image_data) ;
	return CONT ;
}


/*!
 *************************************************************************************
 * \function set_value
 *
 * \brief
 *    This function sets the 'value' array.
 *
 *
 *************************************************************************************
 */
int
set_value(Fw8u value[3], char *remove_rgb )
{
	int str_count ;
	int	i ;

	/* Update the value for RGB */
	value[0] = 1 ;
	value[1] = 1 ;
	value[2] = 1 ;
	str_count = strlen(remove_rgb) ;
	for(i=0; i<str_count; i++)
	{
		if( remove_rgb[i] == 'B' || remove_rgb[i] == 'b' )
			value[0] = 0 ;
		if( remove_rgb[i] == 'G' || remove_rgb[i] == 'g' )
			value[1] = 0 ;
		if( remove_rgb[i] == 'R' || remove_rgb[i] == 'r' )
			value[2] = 0 ;
	}
	return CONT ;
}


/*!
 *************************************************************************************
 * \function usage
 *
 * \brief
 *    This function lists the options available for the program
 *    specified in the command line .
 *
 *************************************************************************************
 */

int
usage(FILE *f_logfile)
{
	printf("\n\n") ;
	printf("Program usage: \n" ) ;
	printf("--------------\n\n") ;
	printf("sample.exe -input <input file> [options][sub_options] -output <output file> \n\n" ) ;

	fprintf(stdout,"\nwhere [options] can be:\n") ;
	fprintf(stdout,"-----------------------\n") ;

	fprintf(stdout,"-addimage <file>           : Add the image specified by <file> to the <input file> \n" ) ;
	fprintf(stdout,"-addC <v1> <v2> <v3>       : Add a specified constant value to the <input file> \n" ) ;
	fprintf(stdout,"-blinds <val> <R> <G> <B>  : Creates blinds effect on the image,either on Horizontal(0)or Verticle(1) direction \n" ) ;
#ifndef SOL
	fprintf(stdout,"-blur                      : Blurs the image using 11x3 mask \n" ) ;
#endif
	fprintf(stdout,"-bright <val>              : Adjust the Brightness of an image by <val> \n" ) ;
	fprintf(stdout,"-border <v1> <v2> <v3>     : Adding a border of color specified by constant value from the <v1> <v2> <v3> \n" ) ;
	fprintf(stdout,"-contrast <val>            : Adjust the contrast of an image by <val> \n" ) ;
	fprintf(stdout,"-crop <W> <H>              : Crops the image by an area specified by <W> and <H>  \n" ) ;
	fprintf(stdout,"-deinterlace <val>         : Interpolate an image when ODD (1) or EVEN (0) field in an image is missing \n" ) ;
	fprintf(stdout,"-edge                      : Detects the edges of an image \n" ) ;
	fprintf(stdout,"-emboss                    : Performs embossing effect on an image \n" ) ;
	fprintf(stdout,"-explode                   : Performs explosion effect on an image \n" ) ;

#ifndef SOL
	fprintf(stdout,"-filter_column             : Performs Column filtering \n" ) ;
	fprintf(stdout,"-filter_gauss              : Performs Gaussian filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_hipass             : Performs highpass filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_laplace            : Performs laplacian filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_lowpass            : Performs lowpass filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_max                : Performs filtering with maximum value in the mask\n" ) ;
	fprintf(stdout,"-filter_medcolor           : Performs Median Color filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_medhoriz           : Performs Median Horizontal filtering using a 3x1 kernel\n" ) ;
	fprintf(stdout,"-filter_medcross           : Performs Median Cross filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_median             : Performs Median filtering using a 3x3 kernel\n" ) ;
	fprintf(stdout,"-filter_medvert            : Performs Median Vertical filtering using a 1x3 kernel\n" ) ;
	fprintf(stdout,"-filter_min                : Performs filtering with minimum value in the mask\n" ) ;
	fprintf(stdout,"-filter_prewhoriz          : Performs Prewitts horizontal filtering \n" ) ;
	fprintf(stdout,"-filter_prewvert           : Performs Prewitts vertical filtering \n" ) ;
	fprintf(stdout,"-filter_rectkernel         : Performs filtering using rectangular kernel\n" ) ;
	fprintf(stdout,"-filter_robdown            : Performs Roberts Down filtering \n" ) ;
	fprintf(stdout,"-filter_robup              : Performs Roberts Up filtering \n" ) ;
	fprintf(stdout,"-filter_row                : Performs Row filtering \n" ) ;
	fprintf(stdout,"-filter_sobhoriz           : Performs Sobel Horizontal filtering \n" ) ;
	fprintf(stdout,"-filter_sobvert            : Performs Sobel Vertical filtering \n" ) ;
#endif
	fprintf(stdout,"-flip <val>                : Flips the image on Horizontal/Vertical axis\n" ) ;
	fprintf(stdout,"-gamma <val>               : Performs gamma correction on an image by <val> \n" ) ;
	fprintf(stdout,"-glassy <W> <H>            : Adds glassy effect of dimension <width> and <height> on an image\n" ) ;
	fprintf(stdout,"-grey                      : Convert an image into grey scale \n" ) ;
	fprintf(stdout,"-grid <val>                : Forms a 3-dimensional grid on the image with the length as specified by the <val> \n" ) ;
	fprintf(stdout,"-halftone                  : Creates a halftone of the image \n" ) ;
	fprintf(stdout,"-hue <val>                 : Performs hue adjustments on an image by <val> \n" ) ;
	fprintf(stdout,"-illusion <val>            : Performs illusion effect on an image by <val> \n" ) ;
	fprintf(stdout,"-negate                    : Bitwise negation of the <input file> \n" ) ;
	fprintf(stdout,"-noise <val>               : Adds random noise to the image by a level as specified by the <val> \n" ) ;
	fprintf(stdout,"-remove <string>           : <string> specifies the color component to be removed \n" ) ;
	fprintf(stdout,"-resize <Xval> <Yval>      : Resize an image by <Xval> and <Yval> as x and y factors respectively\n" ) ;

	fprintf(stdout,"-rotate <angle>            : Rotate the image by angle specified by <angle>\n" ) ;
	fprintf(stdout,"-saturate <val>            : Performs saturation on an image by <val> \n" ) ;
#ifndef SOL
	fprintf(stdout,"-sharp                     : Sharpens an image using Sharpen Filter \n" ) ;
#endif
	fprintf(stdout,"-subimage <file>           : Subtract <input file> from the image <file> \n" ) ;
	fprintf(stdout,"-subC <v1> <v2> <v3>       : Subtract a specified constant from the <input file> \n" ) ;
	fprintf(stdout,"-video <val>               : Creates an image having an effect as that of either STAGGERED(0), TRIPPED (1), 3x3 (2) or DOTTED (3) video\n" ) ;
	fprintf(stdout,"-xorimage <file>           : Bitwise XOR of <file> and the <input file> \n" ) ;

	fprintf(stdout,"\n[sub_options]\n") ;
	fprintf(stdout,"-------------\n") ;
	fprintf(stdout,"--roi <W> <H>              : All the operations are performed on the ROI specified\n" ) ;

	//fprintf(stdout, "\nNote: \n" ) ;
	//fprintf(stdout, "-----\n") ;
	//fprintf(stdout, "1. By default, ROI is treated as the entire image area of <input file>\n" ) ;
	//fprintf(stdout, "2. In a series of operations, output of API at stage (n-1) is input to the API at stage (n)\n" ) ;




	/* Logfile */
	if ( f_logfile == NULL )
		return RETN ;

	printf("\n\n/*** Please refer logfile 'sample_log.txt' for more details ***/\n\n" ) ;
	fprintf(f_logfile, "\n\n") ;
	fprintf(f_logfile, "Program usage: \n" ) ;
	fprintf(f_logfile,"--------------\n\n") ;
	fprintf(f_logfile,"sample.exe -input <input file> [options] -output <output file> \n" ) ;
	fprintf(f_logfile,"\n\n") ;
	fprintf(f_logfile,"where [options] can be:\n") ;
	fprintf(f_logfile,"-----------------------\n") ;


	fprintf(f_logfile,"-addimage <file>                 : Add the image specified by <file> to the <input file> \n" ) ;
	fprintf(f_logfile,"-addC <val1> <val2> <val3>       : Add a specified constant value to the <input file> \n" ) ;
	fprintf(f_logfile,"                                   <val1> specifies the value to be added to the 'RED'   color component of the <input file>\n" );
	fprintf(f_logfile,"                                   <val2> specifies the value to be added to the 'GREEN' color component of the <input file>\n" );
	fprintf(f_logfile,"                                   <val3> specifies the value to be added to the 'BLUE'  color component of the <input file>\n" );
	fprintf(f_logfile,"-blinds <val> <R><G><B>          : Creates blinds effect on the image,either on Horizontal(0)or Verticle(1) direction \n" ) ;
	fprintf(f_logfile,"                                   <R> specifies the value of 'RED'color component in the blinds\n" );
	fprintf(f_logfile,"                                   <G> specifies the value of 'GREEN' color component in the blinds\n" );
	fprintf(f_logfile,"                                   <B> specifies the value of 'BLUE'  color component in the blinds\n" );
	fprintf(f_logfile,"-blur                            : Blurs the image using 11x3 mask \n" ) ;
	fprintf(f_logfile,"-bright <val>                    : Adjust the Brightness of an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : The range of values to adjust brightness of an image is -255 to +255\n" ) ;
	fprintf(f_logfile,"-border <val1> <val2> <val3>     : Adding a border of color specified by constant value from the <val1> <val2> <val3> \n" ) ;
	fprintf(f_logfile,"-contrast <val>                  : Adjust the contrast of an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : The range of values to adjust contrast of an image is -100 to +100\n" ) ;
	fprintf(f_logfile,"-crop <width> <height>           : Crops the image by an area specified by <width> and <height>  \n" ) ;
	fprintf(f_logfile,"-deinterlace <val>               : Interpolate an image when ODD (1) or EVEN (0) field in an image is missing \n" ) ;
	fprintf(f_logfile,"-emboss                          : Performs embossing effect on an image \n" ) ;
	fprintf(f_logfile,"-explode                         : Performs explosion effect on an image \n" ) ;
	fprintf(f_logfile,"-edge                            : Detects the edges of an image \n" ) ;
	fprintf(f_logfile,"-filter_column                   : Performs Column filtering \n" ) ;
	fprintf(f_logfile,"-filter_gauss                    : Performs Gaussian filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_laplace                  : Performs laplacian filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_lowpass                  : Performs lowpass filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_hipass                   : Performs highpass filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_max                      : Performs filtering with maximum value in the mask\n" ) ;
	fprintf(f_logfile,"-filter_medcolor                 : Performs Median Color filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_medcross                 : Performs Median Cross filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_medhoriz                 : Performs Median Horizontal filtering using a 3x1 kernel\n" ) ;
	fprintf(f_logfile,"-filter_median                   : Performs Median filtering using a 3x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_medvert                  : Performs Median Vertical filtering using a 1x3 kernel\n" ) ;
	fprintf(f_logfile,"-filter_min                      : Performs filtering with minimum value in the mask\n" ) ;
	fprintf(f_logfile,"-filter_prewhoriz                : Performs Prewitts horizontal filtering \n" ) ;
	fprintf(f_logfile,"-filter_prewvert                 : Performs Prewitts vertical filtering \n" ) ;
	fprintf(f_logfile,"-filter_rectkernel               : Performs filtering using rectangular kernel\n" ) ;
	fprintf(f_logfile,"-filter_robdown                  : Performs Roberts Down filtering \n" ) ;
	fprintf(f_logfile,"-filter_robup                    : Performs Roberts Up filtering \n" ) ;
	fprintf(f_logfile,"-filter_row                      : Performs Row filtering \n" ) ;
	fprintf(f_logfile,"-filter_sobhoriz                 : Performs Sobel Horizontal filtering \n" ) ;
	fprintf(f_logfile,"-filter_sobvert                  : Performs Sobel Vertical filtering \n" ) ;
	fprintf(f_logfile,"-flip <val>                      : Flips the image on Horizontal(0), Vertical(1) OR Both axis(2)\n" ) ;
	fprintf(f_logfile,"-gamma   <val>                   : Performs gamma correction on an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : The range of values to do gamma correction of an image is 0 to 10\n" ) ;
	fprintf(f_logfile,"-glassy <width> <height>         : Adds glassy effect of dimension <width> and <height> on an image\n" ) ;
	fprintf(f_logfile,"                                 : The range of width and height of the glassy effect on an image is 5 to 75\n" ) ;
	fprintf(f_logfile,"-grey                            : Convert an image into grey scale \n" ) ;
	fprintf(f_logfile,"-grid <val>                      : Forms a 3-dimensional grid on the image with the length as specified by the <val> \n" ) ;
	fprintf(f_logfile,"                                 : where, <val> is a number between 10 & 100 \n" ) ;
	fprintf(f_logfile,"-halftone                        : Creates a halftone of the image \n" ) ;
	fprintf(f_logfile,"-hue <val>                       : Performs hue adjustments on an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : where, <val> is a number between 0 & 360 \n" ) ;
	fprintf(f_logfile,"-illusion <val>                  : Performs illusion effect on an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : where, <val> is a number between 0 & 100 \n" ) ;
	fprintf(f_logfile,"-negate                          : Bitwise negation of the <input file> \n" ) ;
	fprintf(f_logfile,"-noise <val>                     : Adds random noise to the image by a level as specified by the <val> \n" ) ;
	fprintf(f_logfile,"                                 : where, <val> is a number between 0 & 100 \n" ) ;
	fprintf(f_logfile,"-remove <string>                 : <string> specifies the color component to be removed from the <input file> \n" ) ;
	fprintf(f_logfile,"                                 : where, <string> can be 'RG', 'RB', 'B' etc. \n" ) ;
	fprintf(f_logfile,"                                 : 'RG' implies - Remove 'RED' and 'GREEN' color component from the <input file> \n" ) ;
	fprintf(f_logfile,"                                 : 'B'  implies - Remove 'BLUE' color component from the <input file> \n" ) ;
	fprintf(f_logfile,"-resize <Xval> <Yval>            : Resize an image by <Xval> and <Yval> as x and y factors respectively\n" ) ;
	fprintf(f_logfile,"--roi <width> <height>            : All the operations are performed on the ROI specified by <width> and <height> of the <input file> \n" ) ;
	fprintf(f_logfile,"-rotate <angle>                  : Rotate the image by an angle specified by <angle>\n" ) ;
	fprintf(f_logfile,"-saturate <val>                  : Performs saturation on an image by <val> \n" ) ;
	fprintf(f_logfile,"                                 : The range of values to do saturation of an image is 0.0 to 2.0\n" ) ;
	fprintf(f_logfile,"-sharp                           : Sharpens an image using Sharpen Filter \n" ) ;
	fprintf(f_logfile,"-subimage <file>                 : Subtract the input image <input file> from the image specified by <file> \n" ) ;
	fprintf(f_logfile,"-subC <val1> <val2> <val3>       : Subtract a specified constant value from the <input file> \n" ) ;
	fprintf(f_logfile,"                                   <val1> specifies the value to be subtracted from the 'RED'   color component of the <input file>\n" );
	fprintf(f_logfile,"                                   <val2> specifies the value to be subtracted from the 'GREEN' color component of the <input file>\n" );
	fprintf(f_logfile,"                                   <val3> specifies the value to be subtracted from the 'BLUE'  color component of the <input file>\n" );
	fprintf(f_logfile,"-video <val>                     : Creates an image having an effect as that of either STAGGERED(0), TRIPPED (1), 3x3 (2) or DOTTED (3) video\n" ) ;
	fprintf(f_logfile,"-xorimage <file>                 : Bitwise exclusive logical OR of image specified by <file> and the <input file> \n" ) ;


	fprintf(f_logfile, "\nNote: \n" ) ;
	fprintf(f_logfile, "-----\n") ;
	fprintf(f_logfile, "1. By default, ROI is treated as the entire image area of <input file>\n" ) ;
	fprintf(f_logfile, "2. If multiple ROIs are specified, then the last ROI specified will be taken into consideration\n" ) ;
	fprintf(f_logfile, "3. If multiple options are present, previous operation's output becomes current operation's input.\n" ) ;
	fprintf(f_logfile, "4. Syntax for specifying YUV files is YUV_file.yuv <Width> <Height> <Format> .\n" ) ;
	fprintf(f_logfile, "5. Planar YUV files with 422, 420 and 444 formats are supported.\n" ) ;
	fprintf(f_logfile, "6. File extensions specify the color space of the input and output image.\n" ) ;


	fprintf(f_logfile, "\nFor Example: \n" ) ;
	fprintf(f_logfile, "--------------\n") ;
	fprintf(f_logfile, "1. sample.exe -input in.bmp -addimage in_add.bmp -remove R -output out.bmp\n" ) ;
	fprintf(f_logfile, "   ( Add 'in_add.bmp' to 'in.bmp' then, remove the RED color component then, write the resultant in the 'out.bmp'\n" ) ;
	fprintf(f_logfile, "     For all these operations, ROI = Width and Height of 'in.bmp' )\n\n" ) ;

	fprintf(f_logfile, "2. sample.exe -input in.bmp -negate -crop 200 100 --roi 400 300 -output out.bmp\n" ) ;
	fprintf(f_logfile, "   ( Negate 'in.bmp' for an area of W=400 and H=300 then, crop an area of W=200 and H=100 then, write the resultant in the 'out.bmp')\n\n" ) ;

	fprintf(f_logfile, "3. sample.exe -input in.yuv 352 288 420  -addimage in_add.yuv 176 144 444  -output out.bmp\n" ) ;
	fprintf(f_logfile, "   ( Add 'in_add.yuv' to 'in.yuv' then, write the resultant in the 'out.bmp')\n\n" ) ;

	fprintf(f_logfile, "4. sample.exe -input in.yuv 352 288 420   -output out.bmp\n" ) ;
	fprintf(f_logfile, "   ( convert  'in.yuv' to 'out.bmp' --- YUV to RGB color space)\n" ) ;

	return RETN ;
}


/*!
 *************************************************************************************
 * \function in_file_attribute
 *
 * \brief
 *    This function checks for the input file attributes.
 *
 *
 *************************************************************************************
 */

int
in_file_attribute(char *file_in)
{

	char	ext[20] ;
	int		str_count ;
	int		i=0 ;

	str_count = strlen(file_in) ;

	str_count-- ;

	while ( (file_in[str_count] != '.')  && (str_count >= 0)  )
		str_count-- ;

	if( str_count >= 0 )
	{
		str_count++ ;
		while(file_in[str_count] != '\0')
			ext[i++] = file_in[str_count++] ;
		ext[i] = '\0' ;
	}
	else
	{
		printf("Error: No file extension for the file specified '%s' \n", file_in ) ;
		return RETN ;
	}


	if(	(stricmp(ext,"bmp")  != 0) && (stricmp(ext,"yuv")  != 0) )
	{
		printf("Unrecognised file specified '%s' \n", file_in ) ;
		return RETN ;
	}
	return CONT ;
}








