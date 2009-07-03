/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>



#include"globals.h"

/*!
 *************************************************************************************
 * \function error_handler
 *
 * \brief
 *    This function checks for the return value of API.
 *
 *
 *************************************************************************************
 */
int
error_handler(int api_return_val)
{
	if (api_return_val < 0)
	{
		printf("Error in API (Error code is %d)\n", api_return_val ) ;
		return RETN ;
	}

	if (api_return_val > 0)
	{
		printf("Warning in API \n" ) ;
		return WARN ;
	}
	return CONT ;
}

/*!
 *************************************************************************************
 * \function check_command_line
 *
 * \brief
 *    This function checks for comaand line args.
 *
 *
 *************************************************************************************
 */
int
check_command_line(int argc, char *argv[], en_options *p_opts, char *outfile )
{
	FILE			*f_logfile=NULL  ;
	st_input_format  bmp_info ;
	int				ret_val ;
	int				count ;


	/* Generating a Logfile for details */

#ifdef GEN_LOGFILE
	f_logfile = fopen("sample_log.txt", "w" ) ;
	if ( f_logfile == NULL )
	{
		printf("Warning : Cannot open the log file \n") ;
	}
#endif
	/* Checking for the number of parameters on the command line */





	if( argc < 5 )
	{
		usage(f_logfile) ;

		if ( f_logfile )
			fclose(f_logfile) ;
		return RETN  ;
	}


	/* Check for the order of command line */
	if ( stricmp(argv[1], "-input" ) != 0 )
	{
		usage(f_logfile) ;

		if ( f_logfile )
			fclose(f_logfile) ;
		return RETN ;
	}

	/* Checking for the input file attributes */
	if(in_file_attribute(argv[2]) == RETN )
	{
		return RETN ;
	}


	ret_val = check_file_type(argv[2], &bmp_info ) ;
	if ( ret_val == BMP )
		count = 3 ;
	else if(ret_val == YUV)
	{
		if (isnumber_int(argv[3], 1) == RETN || isnumber_int(argv[4], 1) == RETN )
		{
			printf("Error: Invalid argument specified for 'yuv' file \n") ;
			return RETN ;
		}
		if( check_yuvformat(argv[5]) == RETN )
		{
			printf("Error: Invalid yuv file format\n") ;
			return RETN ;
		}
		count = 6 ;
	}


	while( count != argc )
	{
/*  negate  */
		if(stricmp("-negate", argv[count]) == 0 )
		{
			*p_opts++ = NEGATE ;
			count++ ;
		}
/*  remove  */
		else if(stricmp("-remove", argv[count]) == 0 )
		{
			if ( check_removearg(argv[count+1]) == RETN )
			{
				printf("Error: Invalid argument specified for '-remove' option \n") ;
				return RETN ;
			}
			*p_opts++ = REMOVE ;
			count += 2 ;


		}
/*  crop  */
		else if(stricmp("-crop", argv[count]) == 0 )
		{

			if (isnumber_int(argv[count+1], 1) == RETN || isnumber_int(argv[count+2], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-crop' option \n") ;
				return RETN ;
			}
			if ( ( atoi( argv[count+1] ) <= 0 ) || ( atoi( argv[count+2])  <= 0) )
			{
				printf("Error: Invalid argument specified for '-crop' option \n") ;
				return RETN ;
			}
			*p_opts++ = CROP ;
			count += 3 ;
		}
/*  add  */
		else if(stricmp("-addimage", argv[count]) == 0 )
		{

			*p_opts++ = ADD ;
			if ( in_file_attribute(argv[count+1]) == RETN )
				return RETN ;

			ret_val = check_file_type(argv[count+1], &bmp_info ) ;
			if ( ret_val == BMP )
				count += 2 ;
			else if(ret_val == YUV)
			{
				if (isnumber_int(argv[count+2], 1) == RETN || isnumber_int(argv[count+3], 1) == RETN )
				{
					printf("Error: Invalid argument specified for 'yuv' file \n") ;
					return RETN ;
				}
				if( check_yuvformat(argv[count+4]) == RETN )
				{
					printf("Error: Invalid yuv file format\n") ;
					return RETN ;
				}
				count += 5 ;
			}

		}
/*  addC  */
		else if(stricmp("-addC", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN ||
				isnumber_int(argv[count+2], 1) == RETN ||
				isnumber_int(argv[count+3], 1) == RETN  )
			{
				printf("Error: Invalid argument specified for '-addC' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 255 ) == RETN ||
				 check_range(argv[count+2],  0, 255 ) == RETN ||
				 check_range(argv[count+3],  0, 255 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-addC' option \n") ;
				return RETN ;
			}

			*p_opts++ = ADDC ;
			count += 4 ;
		}
/*  sub  */
		else if(stricmp("-subimage", argv[count]) == 0 )
		{
			*p_opts++ = SUB ;
			if ( in_file_attribute(argv[count+1]) == RETN )
				return RETN ;

			ret_val = check_file_type(argv[count+1], &bmp_info ) ;
			if ( ret_val == BMP )
				count += 2 ;
			else if(ret_val == YUV)
			{
				if (isnumber_int(argv[count+2], 1) == RETN || isnumber_int(argv[count+3], 1) == RETN )
				{
					printf("Error: Invalid argument specified for 'yuv' file \n") ;
					return RETN ;
				}
				if( check_yuvformat(argv[count+4]) == RETN )
				{
					printf("Error: Invalid yuv file format\n") ;
					return RETN ;
				}
				count += 5 ;
			}
		}
/*  subC  */
		else if(stricmp("-subC", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN ||
				isnumber_int(argv[count+2], 1) == RETN ||
				isnumber_int(argv[count+3], 1) == RETN  )
			{
				printf("Error: Invalid argument specified for '-subC' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 255 ) == RETN ||
				 check_range(argv[count+2],  0, 255 ) == RETN ||
				 check_range(argv[count+3],  0, 255 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-subC' option \n") ;
				return RETN ;
			}
			*p_opts++ = SUBC ;
			count += 4 ;
		}
/*  xor  */
		else if(stricmp("-xorimage", argv[count]) == 0 )
		{
			*p_opts++ = XOR ;
			if ( in_file_attribute(argv[count+1]) == RETN )
				return RETN ;

			ret_val = check_file_type(argv[count+1], &bmp_info ) ;
			if ( ret_val == BMP )
				count += 2 ;
			else if(ret_val == YUV)
			{
				if (isnumber_int(argv[count+2], 1) == RETN || isnumber_int(argv[count+3], 1) == RETN )
				{
					printf("Error: Invalid argument specified for 'yuv' file \n") ;
					return RETN ;
				}
				if( check_yuvformat(argv[count+4]) == RETN )
				{
					printf("Error: Invalid yuv file format\n") ;
					return RETN ;
				}
				count += 5 ;
			}

		}
/*  output  */
		else if(stricmp("-output", argv[count]) == 0 )
		{
			*p_opts++ = OUTPUT ;
			if ( in_file_attribute(argv[count+1]) == RETN )
				return RETN ;
			strcpy(outfile,argv[count+1]) ;

			ret_val = check_file_type(outfile, &bmp_info ) ;
			if ( ret_val == BMP )
				count += 2 ;
			else if(ret_val == YUV)
				count += 5 ;

			if( count != argc )
			{
				printf("Error in command line \n" ) ;
				return RETN ;
			}

		}
/*  roi  */
		else if(stricmp("--roi", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN || isnumber_int(argv[count+2], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '--roi' option \n") ;
				return RETN ;
			}
			if ( ( atoi( argv[count+1] ) <= 0 ) || ( atoi( argv[count+2])  <= 0) )
			{
				printf("Error: Invalid argument specified for '--roi' option \n") ;
				return RETN ;
			}
			*p_opts++ = ROI ;
			count +=   3 ;
		}
/*  flip  */
		else if(stricmp("-flip", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-flip' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 2 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-flip' option \n") ;
				return RETN ;
			}
			*p_opts++ = FLIP ;
			count +=  2 ;
		}
/*  rotate  */
		else if(stricmp("-rotate", argv[count]) == 0 )
		{
			if (isnumber_float(argv[count+1], -1) == RETN )
			{
				printf("Error: Invalid argument specified for '-rotate' option \n") ;
				return RETN ;
			}
			if ( check_range_float(argv[count+1], -360.0, +360.0 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-rotate' option \n") ;
				return RETN ;
			}

			*p_opts++ = ROTATE ;
			count +=  2 ;
		}
/*  resize  */
		else if(stricmp("-resize", argv[count]) == 0 )
		{
			if (isnumber_float(argv[count+1], -1) == RETN || isnumber_float(argv[count+2], -1) == RETN )
			{
				printf("Error: Invalid argument specified for '-resize' option \n") ;
				return RETN ;
			}

			*p_opts++ = RESIZE ;
			count +=  3 ;
		}
/*  brightness  */
		else if(stricmp("-bright", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], -1) == RETN )
			{
				printf("Error: Invalid argument specified for '-bright' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  -255, 255 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-bright' option \n") ;
				return RETN ;
			}

			*p_opts++ = BRIGHT ;
			count +=  2 ;
		}
/*  contrast  */
		else if(stricmp("-contrast", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], -1) == RETN )
			{
				printf("Error: Invalid argument specified for '-contrast' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  -100, 100 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-contrast' option \n") ;
				return RETN ;
			}
			*p_opts++ = CONTRAST ;
			count +=  2 ;
		}
/*  greyscaling  */
		else if(stricmp("-grey", argv[count]) == 0 )
		{
			*p_opts++ = GREY ;
			count++ ;
		}
/*  edge detection  */
		else if(stricmp("-edge", argv[count]) == 0 )
		{
			*p_opts++ = EDGE ;
			count++ ;
		}
/*  border  */
		else if(stricmp("-border", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN ||
				isnumber_int(argv[count+2], 1) == RETN ||
				isnumber_int(argv[count+3], 1) == RETN  )
			{
				printf("Error: Invalid argument specified for '-border' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 255 ) == RETN ||
				 check_range(argv[count+2],  0, 255 ) == RETN ||
				 check_range(argv[count+3],  0, 255 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-border' option \n") ;
				return RETN ;
			}

			*p_opts++ = BORDER ;
			count+= 4 ;
		}
#ifndef SOL
/*  blur  */
		else if(stricmp("-blur", argv[count]) == 0 )
		{

			*p_opts++ = BLUR ;
			count++ ;
		}

/*  sharpen  */
		else if(stricmp("-sharp", argv[count]) == 0 )
		{
			*p_opts++ = SHARP ;
			count++ ;
		}

/*  Median Filter  */
		else if(stricmp("-filter_median", argv[count]) == 0 )
		{
			*p_opts++ = F_MEDIAN ;
			count++ ;
		}
/*  Gaussian Filter  */
		else if(stricmp("-filter_gauss", argv[count]) == 0 )
		{
			*p_opts++ = F_GAUSS ;
			count++ ;
		}
/*  Laplace Filter  */
		else if(stricmp("-filter_laplace", argv[count]) == 0 )
		{
			*p_opts++ = F_LAPLACE ;
			count++ ;
		}
/*  Lowpass Filter  */
		else if(stricmp("-filter_lowpass", argv[count]) == 0 )
		{
			*p_opts++ = F_LOWPASS ;
			count++ ;
		}
/*  Highpass Filter  */
		else if(stricmp("-filter_hipass", argv[count]) == 0 )
		{
			*p_opts++ = F_HIGHPASS ;
			count++ ;
		}

/*  Median Cross Filter  */
		else if(stricmp("-filter_medcross", argv[count]) == 0 )
		{
			*p_opts++ = F_MEDCROSS;
			count++ ;
		}
/*  Median Vertical Filter  */
		else if(stricmp("-filter_medvert", argv[count]) == 0 )
		{
			*p_opts++ = F_MEDVERT;
			count++ ;
		}
/*  Median Horizontal Filter  */
		else if(stricmp("-filter_medhoriz", argv[count]) == 0 )
		{
			*p_opts++ = F_MEDHORIZ;
			count++ ;
		}
/*  Sobel Vertical Filter  */
		else if(stricmp("-filter_sobvert", argv[count]) == 0 )
		{
			*p_opts++ = F_SOBVERT;
			count++ ;
		}
/*  Sobel Horizontal Filter  */
		else if(stricmp("-filter_sobhoriz", argv[count]) == 0 )
		{
			*p_opts++ = F_SOBHORIZ;
			count++ ;
		}
/*  Filter Minimum  */
		else if(stricmp("-filter_min", argv[count]) == 0 )
		{
			*p_opts++ = F_MIN;
			count++ ;
		}
/*  Filter Maximum  */
		else if(stricmp("-filter_max", argv[count]) == 0 )
		{
			*p_opts++ = F_MAX;
			count++ ;
		}
/*  Roberts Up Filter  */
		else if(stricmp("-filter_robup", argv[count]) == 0 )
		{
			*p_opts++ = F_ROBUP;
			count++ ;
		}
/*  Roberts Down Filter  */
		else if(stricmp("-filter_robdown", argv[count]) == 0 )
		{
			*p_opts++ = F_ROBDOWN;
			count++ ;
		}
/*  Prewitts Vertical Filter  */
		else if(stricmp("-filter_prewvert", argv[count]) == 0 )
		{
			*p_opts++ = F_PREWVERT;
			count++ ;
		}
/*  Prewitts Horizontal Filter  */
		else if(stricmp("-filter_prewhoriz", argv[count]) == 0 )
		{
			*p_opts++ = F_PREWHORIZ;
			count++ ;
		}
/*  Row Filter  */
		else if(stricmp("-filter_row", argv[count]) == 0 )
		{
			*p_opts++ = F_ROW;
			count++ ;
		}
/*  Column Filter  */
		else if(stricmp("-filter_column", argv[count]) == 0 )
		{
			*p_opts++ = F_COLUMN;
			count++ ;
		}
/*  Filter with rectangular kernel */
		else if(stricmp("-filter_rectkernel", argv[count]) == 0 )
		{
			*p_opts++ = F_RECTKERNEL;
			count++ ;
		}
/*  Median Color Filter  */
		else if(stricmp("-filter_medcolor", argv[count]) == 0 )
		{
			*p_opts++ = F_MEDCOLOR;
			count++ ;
		}
#endif


/*  Gamma Correction */
		else if(stricmp("-gamma", argv[count]) == 0 )
		{
			if (isnumber_float(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-gamma' option \n") ;
				return RETN ;
			}
			if ( check_range_float(argv[count+1],  0.0, 10.0 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-gamma' option \n") ;
				return RETN ;
			}
			*p_opts++ = GAMMA;
			count +=  2 ;
		}
/*  Saturation */
		else if(stricmp("-saturate", argv[count]) == 0 )
		{
			if (isnumber_float(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-saturate' option \n") ;
				return RETN ;
			}

			if ( check_range_float(argv[count+1],  0.0, 2.0 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-saturate' option \n") ;
				return RETN ;
			}
			*p_opts++ = SATURATE;
			count +=  2 ;
		}
/*  Hue */
		else if(stricmp("-hue", argv[count]) == 0 )
		{
			if (isnumber_float(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-hue' option \n") ;
				return RETN ;
			}

			if ( check_range_float(argv[count+1],  0.0, 360.0 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-hue' option \n") ;
				return RETN ;
			}
			*p_opts++ = HUE;
			count +=  2 ;
		}
/*  grid */
		else if(stricmp("-grid", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-grid' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  10, 100 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-grid' option \n") ;
				return RETN ;
			}
			*p_opts++ = GRID;
			count +=  2 ;
		}
/*  noise */
		else if(stricmp("-noise", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-noise' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 100 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-noise' option \n") ;
				return RETN ;
			}
			*p_opts++ = NOISE;
			count +=  2 ;
		}
/*  blinds */
		else if(stricmp("-blinds", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN ||
				isnumber_int(argv[count+2], 1) == RETN ||
				isnumber_int(argv[count+3], 1) == RETN ||
				isnumber_int(argv[count+4], 1) == RETN)
			{
				printf("Error: Invalid argument specified for '-blinds' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 1 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-blinds' option \n") ;
				return RETN ;
			}
			if ( check_range(argv[count+2],  0, 255 ) == RETN ||
				 check_range(argv[count+3],  0, 255 ) == RETN ||
				 check_range(argv[count+4],  0, 255 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-blinds' option \n") ;
				return RETN ;
			}
			*p_opts++ = BLINDS;
			count +=  5 ;
		}
/*  emboss */
		else if(stricmp("-emboss", argv[count]) == 0 )
		{
			*p_opts++ = EMBOSS;
			count +=  1 ;
		}
/*  deinterlace */
		else if(stricmp("-deinterlace", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-deinterlace' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 1 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-deinterlace' option \n") ;
				return RETN ;
			}
			*p_opts++ = DEINTERLACE;
			count +=  2 ;
		}
/*  explosion */
		else if(stricmp("-explode", argv[count]) == 0 )
		{
			*p_opts++ = EXPLODE;
			count +=  1 ;
		}
/*  glassy */
		else if(stricmp("-glassy", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN ||
				isnumber_int(argv[count+2], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-glassy' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  5, 75 ) == RETN ||
				 check_range(argv[count+2],  5, 75 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-glassy' option \n") ;
				return RETN ;
			}
			*p_opts++ = GLASSY;
			count +=  3 ;
		}
/*  illusion */
		else if(stricmp("-illusion", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-illusion' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 100 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-illusion' option \n") ;
				return RETN ;
			}
			*p_opts++ = ILLUSION;
			count +=  2 ;
		}
/*  halftone */
		else if(stricmp("-halftone", argv[count]) == 0 )
		{
			*p_opts++ = TONEHALF;
			count +=  1 ;
		}
/*  video */
		else if(stricmp("-video", argv[count]) == 0 )
		{
			if (isnumber_int(argv[count+1], 1) == RETN )
			{
				printf("Error: Invalid argument specified for '-video' option \n") ;
				return RETN ;
			}

			if ( check_range(argv[count+1],  0, 3 ) == RETN )
			{
				printf("Error: Invalid argument specified for '-video' option \n") ;
				return RETN ;
			}
			*p_opts++ = VIDEO;
			count +=  2 ;
		}
/*  else  */
		else
		{
			printf("Error in command line ...Unrecognized option/argument\n") ;
			return RETN ;
			count++ ;
		}
	}
	return CONT ;
}

/*!
 *************************************************************************************
 * \function check_roi
 *
 * \brief
 *    This function checks for command line args given for roi option.
 *
 *
 *************************************************************************************
 */
int
check_roi( char *argv[],int count )
{
	int roiwidth ;
	int roiheight ;

	roiwidth  = atoi(argv[count+1] ) ;
	roiheight = atoi(argv[count+2] ) ;

	if( (int) roiwidth <= 0 || (int) roiheight <= 0 )
	{
		printf(" Non-positive width or height\n");
		return WARN ;
	}

	return CONT ;
}
/*!
 *************************************************************************************
 * \function check_values
 *
 * \brief
 *    This function checks for command line args given for addc and subc options.
 *
 *
 *************************************************************************************
 */
int
check_values( char *argv[],int count )
{
	int val1 ;
	int val2 ;
	int val3 ;
	char *v4 ;

	val1 = atoi( argv[count+1] ) ;
	val2 = atoi( argv[count+2] ) ;
	val3 = atoi( argv[count+3] ) ;

	v4 = (argv[count+1]) ;
	if((!isdigit((int)v4[0])) && (val1 < 0))
	{
		printf("Invalid input parameter\n");
		return WARN ;
	}
	v4 = (argv[count+2]) ;
	if((!isdigit((int)v4[0])) && (val2 < 0))
	{
		printf("Invalid input parameter\n");
		return WARN ;
	}
	v4 = (argv[count+3]) ;
	if((!isdigit((int)v4[0])) && (val3 < 0))
	{
		printf("Invalid input parameter\n");
		return WARN ;
	}

	return CONT ;
}
/*!
 *************************************************************************************
 * \function check_removearg
 *
 * \brief
 *    This function checks for remove option.
 *
 *
 *************************************************************************************
 */

int
check_removearg(char *str)
{
	int stlen ;
	unsigned char c ;
	int	ret_val, i,j ;

	char c_par1 ;
	char c_par2 ;

	stlen = strlen(str) ;
	if (stlen > 3)
	{
		return RETN ;
	}

	if(stlen > 1)
	{
		for(i=0; i<stlen; i++)
		{
			for(j=i; j<(stlen-1); j++)
			{
				c_par1 = tolower(str[i]) ;
				c_par2 = tolower(str[j+1]) ;

				if(c_par1 == c_par2)
				{
					return RETN ;
				}
			}
		}
	}

	for(i=0; i<stlen; i++)
	{
		c = str[i] ;
		if(  c == 'B' || c == 'b' ||
			 c == 'G' || c == 'g' ||
			 c == 'R' || c == 'r'  )
		{
			 ret_val = CONT ;
		}
		else
			ret_val = RETN ;

	}

	return ret_val ;
}

/*!
 *************************************************************************************
 * \function isnumber_float
 *
 * \brief
 *    This function checks args for floating point number
 *
 *
 *************************************************************************************
 */
int
isnumber_float(char *str, int pos_neg )
{
	int i=0 ;
	int f_count = 0 ;

	if ((pos_neg == -1) && (str[0] == '-') )
		i++ ;


	while(str[i] != '\0' )
	{
		if( (isdigit(str[i]) == 0 ) && (str[i] != '.'))
			return RETN ;

		if(str[i] == '.')
			f_count++ ;
		i++ ;
	}

	if(f_count > 1)
		return RETN ;

	return CONT ;
}

/*!
 *************************************************************************************
 * \function isnumber_int
 *
 * \brief
 *    This function checks args for number
 *
 *
 *************************************************************************************
 */
int
isnumber_int(char *str, int pos_neg )
{
	int i=0 ;

	if ((pos_neg == -1) && (str[0] == '-') )
		i++ ;


	while(str[i] != '\0' )
	{
		if( isdigit(str[i]) == 0 )
			return RETN ;
		i++ ;
	}
	return CONT ;
}


/*!
 *************************************************************************************
 * \function check_range
 *
 * \brief
 *    This function checks for range of the string
 *
 *
 *************************************************************************************
 */
int
check_range(char *str, int lo, int hi )
{
	int val ;

	val = atoi(str) ;

	if(val < lo || val > hi)
		return RETN  ;
	else
		return CONT ;
}

/*!
 *************************************************************************************
 * \function check_range_float
 *
 * \brief
 *    This function checks for range of the string
 *
 *
 *************************************************************************************
 */
int
check_range_float(char *str, float lo, float hi )
{
	float val ;

	val = atof(str) ;

	if(val < lo || val > hi)
		return RETN  ;
	else
		return CONT ;
}

/*!
 *************************************************************************************
 * \function check_yuvformat
 *
 * \brief
 *    This function checks for yuv file format (420, 422 and 444)
 *
 *
 *************************************************************************************
 */
int
check_yuvformat(char *str)
{
	int val ;

	val = atoi(str) ;

	if(val == 420 || val == 422 || val == 444 )
		return CONT ;
	else
		return RETN ;
}



/*!
 *************************************************************************************
 * \function check_string
 *
 * \brief
 *    This function checks for command line args given for remove option.
 *
 *
 *************************************************************************************
 */
int
check_string( char *argv[],int count )
{
	char  *val1 ;

	val1 = (argv[count+1]) ;
	if(!isalpha((int)val1[0]))
	{
		printf("Invalid input parameter\n");
		return WARN ;
	}

	return CONT ;
}



