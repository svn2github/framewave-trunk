/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>


#include"globals.h"
#include"process_API.h"



#define SET_YUV_ATTR(p, x, y, z) (p).biWidth = atoi(x) ;\
									(p).biHeight = atoi(y);\
									(p).format = atoi(z);
/*!
 *************************************************************************************
 * \function process_options
 *
 * \brief
 *    This processes options
 *
 *
 *************************************************************************************
 */


int
process_options(int argc, char *argv[], en_options *p_opts, char *outfile)
{
	st_input_format	bmp_info ;
	st_input_format	bmp_info_add ;
	int				roi_val = 0  ;
	int				count = 0  ;
	int				image_size ;
	int				ret_val ;
	int				rem ;


	/* API local variables */

	Fw8u			*pSrcDst = NULL ;
	FwiSize		roiSize ;
	int				srcDstStep  ;
	int				scaleFactor = 0 ;
	Fw8u			*pSrc = NULL ;
	int				srcStep = 0  ;
	int				rotate_count = 0;

	st_API_parameters api_params ;
	FwiSize		tmp_roiSize ;


	/* Read BMP file */


	if( check_file_type(argv[2], &bmp_info) == YUV)
	{
		SET_YUV_ATTR(bmp_info,argv[3],argv[4],argv[5])
	}

	if ( read_inputfile(argv[2], &bmp_info) == RETN )
	{
		return RETN ;
	}

	/* Set the ROI size specified in the command line...By default ROI is entire image */



	count = 3 ;

	while( count < argc )
	{
		if ( stricmp(argv[count], "--roi" ) == 0 )
		{
			roiSize.width  = atoi(argv[count+1] ) ;
			roiSize.height = atoi(argv[count+2] ) ;
			roi_val = 1 ;
		}
		count++ ;
	}

	if ( !roi_val )
	{
		roiSize.width  = bmp_info.biWidth  ;
		roiSize.height = bmp_info.biHeight ;
	}

	/* Set the srcDstStep...As image is in the 24 bpp format width has to be multiplied by 3 */

	srcDstStep = bmp_info.biWidth * 3 ;

	pSrcDst = bmp_info.image_data ;

	if(bmp_info.file_type == BMP )
		count = 3 ;
	else if(bmp_info.file_type == YUV )
		count = 6 ;

	/* Parse options and perform the operations in the order specified in the command line*/

	printf("Processing image '%s' ...\n\n", argv[2] ) ;
	printf("---------------------------------------------\n") ;
	printf("Operation           |    Time taken(seconds) \n") ;
	printf("---------------------------------------------\n") ;



	api_params.pSrcDst		= pSrcDst ;
	api_params.srcDstStep	= srcDstStep ;
	api_params.roiSize		= roiSize ;
	api_params.scaleFactor  = 0 ;

	try {
		while( count != argc )
		{
			api_params.pSrcDst		  = bmp_info.image_data ;
			api_params.srcDstStep	  = bmp_info.biWidth * 3 ;

			if ( roi_val )
			{
				api_params.roiSize.width  = MIN(roiSize.width, bmp_info.biWidth ) ;
				api_params.roiSize.height = MIN(roiSize.height, bmp_info.biHeight ) ;
			}
			else
			{
				api_params.roiSize.width  = bmp_info.biWidth  ;
				api_params.roiSize.height = bmp_info.biHeight ;
			}

			switch(*p_opts++)
			{
				case NEGATE:
							process_negate(&api_params) ;
							printf("'NOT'               |       %10.6f   \n", api_params.time_taken) ;
							count++ ;
							break ;
				case REMOVE:
							if ( set_value(api_params.value, argv[count+1] ) == RETN )
								return RETN ;

							process_remove(&api_params) ;
							printf("'MULC'              |       %10.6f   \n", api_params.time_taken) ;
							count += 2 ;
							break ;
				case CROP:
							api_params.spare_roiSize.width  = atoi(argv[count+1]) ;
							api_params.spare_roiSize.height = atoi(argv[count+2]) ;

							api_params.spare_roiSize.width  = MIN(api_params.spare_roiSize.width, bmp_info.biWidth ) ;
							api_params.spare_roiSize.height = MIN(api_params.spare_roiSize.height,bmp_info.biHeight ) ;

							api_params.spare_value[0] = 0 ;
							api_params.spare_value[1] = 0 ;
							api_params.spare_value[2] = 0 ;

							if( check_file_type(argv[2], &bmp_info_add) == YUV)
							{
								SET_YUV_ATTR(bmp_info_add,argv[3],argv[4],argv[5])
							}
							if ( read_inputfile(argv[2], &bmp_info_add) == RETN )
								return RETN ;

							api_params.pSrc = (unsigned char *)malloc(bmp_info.biWidth * bmp_info.biHeight * 3 * sizeof(unsigned char) ) ;
							if( !(api_params.pSrc) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}
							api_params.srcStep = bmp_info.biWidth * 3 ;
							memcpy(api_params.pSrc, bmp_info.image_data, bmp_info.biWidth * bmp_info.biHeight * 3 ) ;

							tmp_roiSize = api_params.roiSize ;
							api_params.roiSize.width  = bmp_info.biWidth ;
							api_params.roiSize.height = bmp_info.biHeight ;


							process_crop(&api_params) ;

							api_params.roiSize = tmp_roiSize ;
							clear_bmp_struct(&bmp_info_add) ;
							free(api_params.pSrc) ;
							printf("'CROP'              |       %10.6f   \n", api_params.time_taken) ;
							count += 3 ;
							break ;
				case ADD:
							ret_val = check_file_type(argv[count+1], &bmp_info_add) ;
							if( ret_val == YUV)
							{
								SET_YUV_ATTR(bmp_info_add,argv[count+2],argv[count+3],argv[count+4])
							}
							if ( read_inputfile(argv[count+1], &bmp_info_add) == RETN )
								return RETN ;

							api_params.pSrc    = bmp_info_add.image_data ;
							api_params.srcStep = bmp_info_add.biWidth * 3 ;

							api_params.spare_roiSize = api_params.roiSize ;

							api_params.roiSize.width  = MIN(api_params.roiSize.width, bmp_info_add.biWidth) ;
							api_params.roiSize.height = MIN(api_params.roiSize.height,bmp_info_add.biHeight) ;

							process_add(&api_params) ;
							printf("'ADD'               |       %10.6f   \n", api_params.time_taken) ;

							api_params.roiSize = api_params.spare_roiSize   ;

							clear_bmp_struct(&bmp_info_add) ;
							if ( ret_val == BMP )
								count += 2 ;
							else if(ret_val == YUV)
								count += 5 ;
							break ;
				case ADDC:
							api_params.value[2] = atoi( argv[count+1] ) ;
							api_params.value[1] = atoi( argv[count+2] ) ;
							api_params.value[0] = atoi( argv[count+3] ) ;

							process_addC(&api_params) ;

							printf("'ADDC'              |       %10.6f   \n", api_params.time_taken) ;
							count += 4 ;
							break ;
				case SUB:
							ret_val = check_file_type(argv[count+1], &bmp_info_add) ;
							if( ret_val == YUV)
							{
								SET_YUV_ATTR(bmp_info_add,argv[count+2],argv[count+3],argv[count+4])
							}
							if ( read_inputfile(argv[count+1], &bmp_info_add) == RETN )
							{
								return RETN ;
							}
							api_params.pSrc    = bmp_info_add.image_data ;
							api_params.srcStep = bmp_info_add.biWidth * 3 ;

							api_params.spare_roiSize = api_params.roiSize ;

							api_params.roiSize.width  = MIN(api_params.roiSize.width, bmp_info_add.biWidth) ;
							api_params.roiSize.height = MIN(api_params.roiSize.height,bmp_info_add.biHeight) ;


							process_sub(&api_params) ;
							printf("'SUB'               |       %10.6f   \n", api_params.time_taken) ;

							api_params.roiSize = api_params.spare_roiSize   ;
							clear_bmp_struct(&bmp_info_add) ;
							if ( ret_val == BMP )
								count += 2 ;
							else if(ret_val == YUV)
								count += 5 ;
							break ;
				case SUBC:
							api_params.value[2] = atoi( argv[count+1] ) ;
							api_params.value[1] = atoi( argv[count+2] ) ;
							api_params.value[0] = atoi( argv[count+3] ) ;

							process_subC(&api_params) ;

							printf("'SUBC'              |       %10.6f   \n", api_params.time_taken) ;
							count += 4 ;
							break ;
				case XOR:
							ret_val = check_file_type(argv[count+1], &bmp_info_add) ;
							if( ret_val == YUV)
							{
								SET_YUV_ATTR(bmp_info_add,argv[count+2],argv[count+3],argv[count+4])
							}
							if ( read_inputfile(argv[count+1], &bmp_info_add) == RETN )
							{
								return RETN ;
							}
							api_params.pSrc    = bmp_info_add.image_data ;
							api_params.srcStep = bmp_info_add.biWidth * 3 ;

							api_params.spare_roiSize = api_params.roiSize ;

							api_params.roiSize.width  = MIN(api_params.roiSize.width, bmp_info_add.biWidth) ;
							api_params.roiSize.height = MIN(api_params.roiSize.height,bmp_info_add.biHeight) ;


							process_xor(&api_params) ;
							printf("'XOR'               |       %10.6f   \n", api_params.time_taken) ;

							api_params.roiSize = api_params.spare_roiSize   ;
							clear_bmp_struct(&bmp_info_add) ;
							if ( ret_val == BMP )
								count += 2 ;
							else if(ret_val == YUV)
								count += 5 ;
							break ;
				case FLIP:
							/* flip = 0 --- fwAxsHorizontal	*/
							/* flip = 1 --- fwAxsVertical		*/
							/* flip = 2 --- fwAxsBoth			*/

							api_params.flip = (FwiAxis)atoi(argv[count+1]) ;

							process_mirror(&api_params) ;

							printf("'FLIP'              |       %10.6f   \n", api_params.time_taken) ;
							count += 2 ;
							break ;
				case ROTATE:
							rotate_count = 1;

							set_rotate_params(atof(argv[count+1]), &bmp_info, &api_params, rotate_count ) ;


							process_rotate(&api_params) ;
							printf("'ROTATE'            |       %10.6f   \n", api_params.time_taken) ;

							rem = bmp_info.biWidth*3 % 4 ;
							if(rem != 0)
							{
								rem = 4 - rem ;
								image_size  =  ( ( (bmp_info.biWidth) * 3) + rem  ) * bmp_info.biHeight ;
							}
							else
							{
								image_size  =  (bmp_info.biWidth) * (bmp_info.biHeight) * 3 ;
							}



							free(bmp_info.image_data) ;
							bmp_info.image_data = (unsigned char *)malloc(image_size * sizeof(unsigned char) ) ;
							if( !(bmp_info.image_data) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}
							api_params.roiSize =  api_params.spare_roiSize ;

							memcpy(bmp_info.image_data, api_params.pDst, bmp_info.biWidth * bmp_info.biHeight * 3 ) ;
							free(api_params.pDst) ;


							count += 2 ;
							break ;
				case RESIZE:
							set_resize_params(atof(argv[count+1]), atof(argv[count+2]), &bmp_info, &api_params ) ;
							process_resize(&api_params) ;
							printf("'RESIZE'            |       %10.6f   \n", api_params.time_taken) ;

							rem = bmp_info.biWidth*3 % 4 ;
							if(rem != 0)
							{
								rem = 4 - rem ;
								image_size  =  ( ( (bmp_info.biWidth) * 3) + rem  ) * bmp_info.biHeight ;
							}
							else
							{
								image_size  =  (bmp_info.biWidth) * (bmp_info.biHeight) * 3 ;
							}

							//image_size = (bmp_info.biWidth * bmp_info.biHeight * 3) + ((4-(bmp_info.biWidth % 4))* bmp_info.biHeight) ;

							free(bmp_info.image_data) ;
							bmp_info.image_data = (unsigned char *)malloc(image_size * sizeof(unsigned char) ) ;
							if( !(bmp_info.image_data) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}
							api_params.roiSize =  api_params.spare_roiSize ;

							memcpy(bmp_info.image_data, api_params.pDst, bmp_info.biWidth * bmp_info.biHeight * 3 ) ;
							free(api_params.pDst) ;

							count += 3 ;
							break ;
				case BRIGHT:

							api_params.adjust = atof(argv[count+1]) ;

							api_params.value[2] = (Fw8u)(api_params.adjust) ;
							api_params.value[1] = (Fw8u)(api_params.adjust) ;
							api_params.value[0] = (Fw8u)(api_params.adjust) ;

							process_brightness(&api_params) ;

							printf("'BRIGHT'            |       %10.6f   \n", api_params.time_taken) ;
							count += 2 ;
							break ;
				case CONTRAST:

							api_params.adjust = atof(argv[count+1]) ;

							set_contrast_params( &bmp_info, &api_params ) ;

							process_contrast(&api_params) ;

							printf("'CONTRAST'          |       %10.6f   \n", api_params.time_taken) ;

							api_params.pSrcDst = (unsigned char *)malloc(bmp_info.biWidth * bmp_info.biHeight * 3 * sizeof(unsigned char) ) ;
							if( !(api_params.pSrcDst) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								 double pixel = api_params.pSrc_con[i];
								 if (pixel  < 0)
									pixel  = 0;
								 if (pixel  > 255)
									pixel  = 255;
								 api_params.pSrc_con[i] = pixel  ;
								 api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_con[i] ;
							}
							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
								free(api_params.pDst) ;
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pSrc_con) ;
							free(api_params.pSrcDst) ;
							count += 2 ;
							break ;
				case GREY:
							set_greyscale_params( &bmp_info, &api_params ) ;

							api_params.value[2] = 3 ;
							api_params.value[1] = 3 ;
							api_params.value[0] = 3 ;

							process_grey(&api_params) ;
							printf("'GREY'              |       %10.6f   \n", api_params.time_taken) ;
							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i=i+3)
							{
								int temp = api_params.pSrcDst[i] + api_params.pSrcDst[i+1] + api_params.pSrcDst[i+2] ;
								api_params.pSrcDst[i]   = temp ;
								api_params.pSrcDst[i+1] = temp ;
								api_params.pSrcDst[i+2] = temp ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
								free(api_params.pSrc) ;
								free(api_params.pDst) ;
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}


							count++ ;
							break ;
				case EDGE:
							set_edgedetect_params( &bmp_info, &api_params ) ;

							process_edgedetect(&api_params) ;

							printf("'EDGE'              |       %10.6f   \n", api_params.time_taken) ;

							float temp1;
							float temp2;

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i+=3)
							{
								temp1 = api_params.pSrc_edge2[i] + api_params.pSrc_edge2[i+1] + api_params.pSrc_edge2[i+2];
								temp2 = api_params.pSrc_edge3[i] + api_params.pSrc_edge3[i+1] + api_params.pSrc_edge3[i+2];
								if((sqrt(temp1)>=50)||
								  (sqrt(temp2)>=50))
								{
									bmp_info.image_data[i] = 255 ;
									bmp_info.image_data[i+1] = 255 ;
									bmp_info.image_data[i+2] = 255 ;
								}
								else
								{
									bmp_info.image_data[i] = 0 ;
									bmp_info.image_data[i+1] = 0 ;
									bmp_info.image_data[i+2] = 0 ;
								}
							}

							free(api_params.pSrc_edge1) ;
							free(api_params.pSrc_edge2) ;
							free(api_params.pSrc_edge3) ;
							count ++ ;
							break ;
				case BORDER:
							api_params.value[2] = atoi( argv[count+1] ) ;
							api_params.value[1] = atoi( argv[count+2] ) ;
							api_params.value[0] = atoi( argv[count+3] ) ;

/*							api_params.spare_value[2] = 0 ;
							api_params.spare_value[1] = 0 ;
							api_params.spare_value[0] = 0 ;

							api_params.bor_width  = bmp_info.biWidth ;
							api_params.bor_height = bmp_info.biHeight ;

							api_params.bor_roi_wd  = api_params.roiSize.width ;
							api_params.bor_roi_ht  = api_params.roiSize.height ;
*/


							api_params.srcDstStep = bmp_info.biWidth * 3 ;
							api_params.roiSize.width  = bmp_info.biWidth;
				            api_params.roiSize.height = bmp_info.biHeight;

							process_border(&api_params) ;

							printf("'BORDER'            |       %10.6f   \n", api_params.time_taken) ;
							count += 4 ;
							break ;
#ifndef SOL
				case BLUR:

							set_blur_params( &bmp_info, &api_params ) ;

							process_blur(&api_params) ;

							printf("'BLUR'              |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case SHARP:
							set_sharp_params( &bmp_info, &api_params ) ;

							process_sharp(&api_params) ;

							printf("'SHARP'             |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MEDIAN:
							set_fmedian_params( &bmp_info, &api_params ) ;

							process_fmedian(&api_params) ;

							printf("'FILTER_MEDIAN'     |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_GAUSS:
							set_fgauss_params( &bmp_info, &api_params ) ;

							process_fgauss(&api_params) ;

							printf("'FILTER_GAUSS'      |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_LAPLACE:
							set_flaplace_params( &bmp_info, &api_params ) ;

							process_flaplace(&api_params) ;

							printf("'FILTER_LAPLACE'    |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_LOWPASS:
							set_flowpass_params( &bmp_info, &api_params ) ;

							process_flowpass(&api_params) ;

							printf("'FILTER_LOWPASS'    |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;


							count ++ ;
							break ;
				case F_HIGHPASS:
							set_fhighpass_params( &bmp_info, &api_params ) ;

							process_fhighpass(&api_params) ;

							printf("'FILTER_HIPASS'     |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MEDCROSS:
							set_fmedcross_params( &bmp_info, &api_params ) ;

							process_fmedcross(&api_params) ;

							printf("'FILTER_MEDCROSS'   |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MEDVERT:
							set_fmedvert_params( &bmp_info, &api_params ) ;

							process_fmedvert(&api_params) ;

							printf("'FILTER_MEDVERT'    |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MEDHORIZ:
							set_fmedhoriz_params( &bmp_info, &api_params ) ;

							process_fmedhoriz(&api_params) ;

							printf("'FILTER_MEDHORIZ'   |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_SOBVERT:
							set_fsobvert_params( &bmp_info, &api_params ) ;

							process_fsobvert(&api_params) ;

							printf("'FILTER_SOBVERT'    |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_SOBHORIZ:
							set_fsobhoriz_params( &bmp_info, &api_params ) ;

							process_fsobhoriz(&api_params) ;

							printf("'FILTER_SOBHORIZ'   |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MIN:
							set_fmin_params( &bmp_info, &api_params ) ;

							process_fmin(&api_params) ;

							printf("'FILTER_MIN'        |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_MAX:
							set_fmax_params( &bmp_info, &api_params ) ;

							process_fmax(&api_params) ;

							printf("'FILTER_MAX'        |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_ROBUP:
							set_frobup_params( &bmp_info, &api_params ) ;

							process_frobup(&api_params) ;

							printf("'FILTER_ROBUP'      |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_ROBDOWN:
							set_frobdown_params( &bmp_info, &api_params ) ;

							process_frobdown(&api_params) ;

							printf("'FILTER_ROBDOWN'    |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_PREWVERT:
							set_fprewvert_params( &bmp_info, &api_params ) ;

							process_fprewvert(&api_params) ;

							printf("'FILTER_PREWVERT'   |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_PREWHORIZ:
							set_fprewhoriz_params( &bmp_info, &api_params ) ;

							process_fprewhoriz(&api_params) ;

							printf("'FILTER_PREWHORIZ'  |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
				case F_ROW:
							set_frow_params( &bmp_info, &api_params ) ;

							process_frow(&api_params) ;

							printf("'FILTER_ROW'        |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);

							free(api_params.pDst) ;
							free(api_params.pKernel_32s) ;

							count ++ ;
							break ;
				case F_COLUMN:
							set_fcolumn_params( &bmp_info, &api_params ) ;

							process_fcolumn(&api_params) ;

							printf("'FILTER_COLUMN'     |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);

							free(api_params.pDst) ;
							free(api_params.pKernel_32s) ;

							count ++ ;
							break ;
				case F_RECTKERNEL:
							set_frectkernel_params( &bmp_info, &api_params ) ;

							process_frectkernel(&api_params) ;

							printf("'FILTER_RECTKERNEL' |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							free(api_params.pKernel_32s) ;

							count ++ ;
							break ;
				case F_MEDCOLOR:
							set_fmedcolor_params( &bmp_info, &api_params ) ;

							process_fmedcolor(&api_params) ;

							printf("'FILTER_MEDCOLOR'   |       %10.6f   \n", api_params.time_taken) ;

							memcpy(api_params.pSrcDst,api_params.pDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							free(api_params.pDst) ;

							count ++ ;
							break ;
#endif

				case GAMMA:

							api_params.adjust = atof(argv[count+1]) ;

							set_gamma_params( &bmp_info, &api_params ) ;

							process_gamma(&api_params) ;

							printf("'GAMMA'             |       %10.6f   \n", api_params.time_taken) ;

							api_params.pSrcDst = (unsigned char *)malloc(bmp_info.biWidth * bmp_info.biHeight * 3 * sizeof(unsigned char) ) ;
							if( !(api_params.pSrcDst) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								 api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_con[i] ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
								free(api_params.pDst) ;
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pSrc_con) ;
							free(api_params.pSrcDst) ;
							count += 2 ;
							break ;
				case SATURATE:

							api_params.adjust = atof(argv[count+1]) ;

							set_saturate_params( &bmp_info, &api_params ) ;

							process_saturate(&api_params) ;

							printf("'SATURATE'          |       %10.6f   \n", api_params.time_taken) ;

							api_params.pSrcDst = (unsigned char *)malloc(bmp_info.biWidth * bmp_info.biHeight * 3 * sizeof(unsigned char) ) ;
							if( !(api_params.pSrcDst) )
							{
								printf("Error: Cannot allocate memory \n" ) ;
								return RETN ;
							}

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_sat1[i] ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
								free(api_params.pDst) ;
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pSrc_sat1) ;
							free(api_params.pSrc_sat2) ;
							free(api_params.pSrcDst) ;
							count += 2 ;
							break ;
					case HUE:

							api_params.adjust = atof(argv[count+1]) ;

							set_hue_params( &bmp_info, &api_params ) ;

							process_hue(&api_params) ;

							printf("'HUE'               |       %10.6f   \n", api_params.time_taken) ;

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case GRID:

							api_params.grid_size = atoi(argv[count+1]) ;

							set_grid_params( &bmp_info, &api_params ) ;

							process_grid(&api_params) ;

							printf("'GRID'              |       %10.6f   \n", api_params.time_taken) ;

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								 api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_con[i] ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pSrc_con) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case NOISE:

							api_params.noise_level = atoi(argv[count+1]) ;

							set_noise_params( &bmp_info, &api_params ) ;

							process_noise(&api_params) ;

							printf("'NOISE'             |       %10.6f   \n", api_params.time_taken) ;

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								 api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_con[i] ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pSrc_con) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case BLINDS:

							api_params.blinds_direction = atoi(argv[count+1]) ;

							set_blinds_params( &bmp_info, &api_params ) ;

							api_params.color[2] = (Fw32f)atoi(argv[count+2]) ;
							api_params.color[1] = (Fw32f)atoi(argv[count+3]) ;
							api_params.color[0] = (Fw32f)atoi(argv[count+4]) ;

							process_blinds(&api_params) ;

							printf("'BLINDS'            |       %10.6f   \n", api_params.time_taken) ;

							for(int i=0;i<(bmp_info.biWidth * bmp_info.biHeight * 3);i++)
							{
								 api_params.pSrcDst[i] = (unsigned char) api_params.pSrc_con[i] ;
							}

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);

							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pSrc_con) ;
							free(api_params.color) ;
							free(api_params.pSrc) ;
							count += 5 ;
							break ;
				case EMBOSS:

							set_emboss_params( &bmp_info, &api_params ) ;

							process_emboss(&api_params) ;

							printf("'EMBOSS'            |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.grey) ;
							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 1 ;
							break ;
				case DEINTERLACE:

							api_params.field = atoi(argv[count+1]) ;

							set_deinterlace_params( &bmp_info, &api_params ) ;

							process_deinterlace(&api_params) ;

							printf("'DEINTERLACE'       |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case EXPLODE:

							set_explode_params( &bmp_info, &api_params ) ;

							process_explode(&api_params) ;

							printf("'EXPLODE'           |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 1 ;
							break ;
				case GLASSY:

							api_params.x_len = atoi(argv[count+1]) ;
							api_params.y_len = atoi(argv[count+2]) ;

							set_glassy_params( &bmp_info, &api_params ) ;

							process_glassy(&api_params) ;

							printf("'GLASSY'            |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 3 ;
							break ;
				case ILLUSION:

							api_params.field = atoi(argv[count+1]) ;

							set_illusion_params( &bmp_info, &api_params ) ;

							process_illusion(&api_params) ;

							printf("'ILLUSION'          |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case TONEHALF:

							set_halftone_params( &bmp_info, &api_params ) ;

							process_halftone(&api_params) ;

							printf("'HALFTONE'          |       %10.6f   \n", api_params.time_taken) ;

							memcpy(bmp_info.image_data, api_params.pDst, bmp_info.biWidth * bmp_info.biHeight * 3 ) ;

							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 1 ;
							break ;
				case VIDEO:

							api_params.videoType = atoi(argv[count+1]) ;

							set_video_params( &bmp_info, &api_params ) ;

							process_video(&api_params) ;

							printf("'VIDEO'             |       %10.6f   \n", api_params.time_taken) ;


							if((api_params.roiSize.width != bmp_info.biWidth) ||
								(api_params.roiSize.height != bmp_info.biHeight))
							{
								set_display_params (&bmp_info, &api_params);

								FW_display(&api_params);
							    printf("'IMAGE DISPLAY'     |       %10.6f   \n", api_params.time_taken) ;

								memcpy(bmp_info.image_data,api_params.pSrc,bmp_info.biWidth * bmp_info.biHeight * 3);
							}
							else
							{
								memcpy(bmp_info.image_data,api_params.pSrcDst,bmp_info.biWidth * bmp_info.biHeight * 3);
							}

							free(api_params.pDst) ;
							free(api_params.pSrc) ;
							count += 2 ;
							break ;
				case OUTPUT:
							if( check_file_type(argv[count+1], &bmp_info) == YUV)
							{
								bmp_info.format = atoi(argv[count+4]) ;
								count += 4 ;
							}
							else
							{
								count += 2 ;
							}
							break ;
				case ROI:
							count += 3 ;
							break ;

				default:
							count++ ;


			}
		}
	}
	catch(int val){
		error_handler(val) ;
		return RETN ;
	}


	printf("---------------------------------------------\n") ;

	if (write_outputfile(outfile, &bmp_info) == RETN )
		return RETN ;
	clear_bmp_struct(&bmp_info) ;
	printf("\nDone!  Generated output file '%s' \n\n", outfile) ;
	return CONT ;
}


/*!
 *************************************************************************************
 * \function set_video_params
 *
 * \brief
 *    This function sets the parameters for halftone effect operation
 *
 *
 *************************************************************************************
 */
int
set_video_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_halftone_params
 *
 * \brief
 *    This function sets the parameters for halftone effect operation
 *
 *
 *************************************************************************************
 */
int
set_halftone_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->value[0] = 3 ;
	p->value[1] = 3 ;
	p->value[2] = 3 ;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_illusion_params
 *
 * \brief
 *    This function sets the parameters for illusion effect operation
 *
 *
 *************************************************************************************
 */
int
set_illusion_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_glassy_params
 *
 * \brief
 *    This function sets the parameters for glassy operation
 *
 *
 *************************************************************************************
 */
int
set_glassy_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_explode_params
 *
 * \brief
 *    This function sets the parameters for explode operation
 *
 *
 *************************************************************************************
 */
int
set_explode_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_deinterlace_params
 *
 * \brief
 *    This function sets the parameters for deinterlace operation
 *
 *
 *************************************************************************************
 */
int
set_deinterlace_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;


	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_emboss_params
 *
 * \brief
 *    This function sets the parameters for emboss operation
 *
 *
 *************************************************************************************
 */
int
set_emboss_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->grey    =  (unsigned char *)malloc(4 * sizeof(unsigned char));
	if(p->grey == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_blinds_params
 *
 * \brief
 *    This function sets the parameters for adding blinds effect operation
 *
 *
 *************************************************************************************
 */
int
set_blinds_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->color    =  (float *)malloc(4 * sizeof(float));
	if(p->color == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->pSrc_con  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_con == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_noise_params
 *
 * \brief
 *    This function sets the parameters for adding random noise operation
 *
 *
 *************************************************************************************
 */
int
set_noise_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->pSrc_con  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_con == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_grid_params
 *
 * \brief
 *    This function sets the parameters for grid operation
 *
 *
 *************************************************************************************
 */
int
set_grid_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->grid_depth  = 150 ;

	p->pSrc_con  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_con == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_hue_params
 *
 * \brief
 *    This function sets the parameters for hue operation
 *
 *
 *************************************************************************************
 */
int
set_hue_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->srcStep	= bmp_info->biWidth * 3 ;
	p->dstStep  = bmp_info->biWidth * 3 ;

	p->pDst  = (unsigned char *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 ) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_saturate_params
 *
 * \brief
 *    This function sets the parameters for saturation operation
 *
 *
 *************************************************************************************
 */
int
set_saturate_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data ;
	p->srcStep	= bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->pSrc_sat1  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_sat1 == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc_sat2  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_sat2 == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->value_con[0] = (Fw32f)0.241 ;
	p->value_con[1] = (Fw32f)0.691 ;
	p->value_con[2] = (Fw32f)0.068 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_gamma_params
 *
 * \brief
 *    This function sets the parameters for gamma correction operation
 *
 *
 *************************************************************************************
 */
int
set_gamma_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data ;
	p->srcStep	= bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->pSrc_con  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_con == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->adjust = 1.0 / p->adjust ;

	p->value_con[2] = 255 ;
	p->value_con[1] = 255 ;
	p->value_con[0] = 255 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	memcpy(p->pSrc_con,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_display_params
 *
 * \brief
 *    This function sets the parameters for display of image when the ROI specified
 *	  is smaller than the original image
 *
 *
 *************************************************************************************
 */
int
set_display_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pDst = (unsigned char*)calloc((bmp_info->biWidth * bmp_info->biHeight * 3),sizeof(unsigned char)) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->srcStep	= (bmp_info->biWidth * 3) ;

	p->roiSize.width  = p->roiSize.width ;
	p->roiSize.height = p->roiSize.height ;

	p->spare_roiSize.width  = bmp_info->biWidth  ;
	p->spare_roiSize.height = bmp_info->biHeight ;

	p->spare_value[2] = 0 ;
	p->spare_value[1] = 0 ;
	p->spare_value[0] = 0 ;


	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_greyscale_params
 *
 * \brief
 *    This function sets the parameters for RGB to Grey scale conversion
 *
 *
 *************************************************************************************
 */
int
set_greyscale_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pSrc == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	memcpy(p->pSrc,p->pSrcDst,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_frectkernel_params
 *
 * \brief
 *    This function sets the parameters for filter operation with rectangular kernel
 *
 *
 *************************************************************************************
 */
int
set_frectkernel_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->kernelSize_f.width  = 2 ;
	p->kernelSize_f.height = 2 ;

	p->anchor.x = 1;
	p->anchor.y = 1;

	p->pKernel_32s = (signed int*)malloc(10 * (sizeof(int)));
	if(p->pKernel_32s == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->divisor     = 1 ;

	p->pKernel_32s[0] = 0 ;
	p->pKernel_32s[1] = 1 ;
	p->pKernel_32s[2] = 0 ;
	p->pKernel_32s[3] = 0 ;


	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fcolumn_params
 *
 * \brief
 *    This function sets the parameters for Column filter operation
 *
 *
 *************************************************************************************
 */
int
set_fcolumn_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 6;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 4 ;
	p->dstRoiSize.height = p->roiSize.height - 4 ;

	p->kernelSize  = 4 ;
	p->pKernel_32s = (signed int*)malloc(p->kernelSize * (sizeof(int)));
	if(p->pKernel_32s == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->xAnchor     = 1 ;
	p->divisor     = 1 ;

	p->pKernel_32s[0] = 0 ;
	p->pKernel_32s[1] = 1 ;
	p->pKernel_32s[2] = 0 ;
	p->pKernel_32s[3] = 0 ;


	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_frow_params
 *
 * \brief
 *    This function sets the parameters for Row filter operation
 *
 *
 *************************************************************************************
 */
int
set_frow_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->kernelSize  = 4 ;
	p->pKernel_32s = (signed int*)malloc(p->kernelSize * (sizeof(int)));
	if(p->pKernel_32s == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->xAnchor     = 1 ;
	p->divisor     = 1 ;

	p->pKernel_32s[0] = 0 ;
	p->pKernel_32s[1] = 1 ;
	p->pKernel_32s[2] = 0 ;
	p->pKernel_32s[3] = 0 ;


	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fprewhoriz_params
 *
 * \brief
 *    This function sets the parameters for Prewitts horizontal filter operation
 *
 *
 *************************************************************************************
 */
int
set_fprewhoriz_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_fprewvert_params
 *
 * \brief
 *    This function sets the parameters for Prewitts vertical filter operation
 *
 *
 *************************************************************************************
 */
int
set_fprewvert_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_frobdown_params
 *
 * \brief
 *    This function sets the parameters for Roberts Down filter operation
 *
 *
 *************************************************************************************
 */
int
set_frobdown_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_frobup_params
 *
 * \brief
 *    This function sets the parameters for Roberts Up filter operation
 *
 *
 *************************************************************************************
 */
int
set_frobup_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_fmax_params
 *
 * \brief
 *    This function sets the parameters for filter maximum operation
 *
 *
 *************************************************************************************
 */
int
set_fmax_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize.width  = 3 ;
	p->maskSize.height = 3 ;

	p->anchor.x = 1 ;
	p->anchor.y = 1 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmin_params
 *
 * \brief
 *    This function sets the parameters for filter minimum operation
 *
 *
 *************************************************************************************
 */
int
set_fmin_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize.width  = 3 ;
	p->maskSize.height = 3 ;

	p->anchor.x = 1 ;
	p->anchor.y = 1 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fsobhoriz_params
 *
 * \brief
 *    This function sets the parameters for Sobel horizontal filter operation
 *
 *
 *************************************************************************************
 */
int
set_fsobhoriz_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_fsobvert_params
 *
 * \brief
 *    This function sets the parameters for Sobel vertical filter operation
 *
 *
 *************************************************************************************
 */
int
set_fsobvert_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmedhoriz_params
 *
 * \brief
 *    This function sets the parameters for Median Horizontal filter operation
 *
 *
 *************************************************************************************
 */
int
set_fmedhoriz_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x1 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmedvert_params
 *
 * \brief
 *    This function sets the parameters for Median Vertical filter operation
 *
 *
 *************************************************************************************
 */
int
set_fmedvert_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize1x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmedcross_params
 *
 * \brief
 *    This function sets the parameters for Median Cross filter operation
 *
 *
 *************************************************************************************
 */
int
set_fmedcross_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmedcolor_params
 *
 * \brief
 *    This function sets the parameters for Median Color filter operation
 *
 *
 *************************************************************************************
 */
int
set_fmedcolor_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}
/*!
 *************************************************************************************
 * \function set_fhighpass_params
 *
 * \brief
 *    This function sets the parameters for Highpass filter operation
 *
 *
 *************************************************************************************
 */
int
set_fhighpass_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_flowpass_params
 *
 * \brief
 *    This function sets the parameters for Lowpass filter operation
 *
 *
 *************************************************************************************
 */
int
set_flowpass_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_flaplace_params
 *
 * \brief
 *    This function sets the parameters for Laplacian filter operation
 *
 *
 *************************************************************************************
 */
int
set_flaplace_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}


	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);


	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fgauss_params
 *
 * \brief
 *    This function sets the parameters for Gaussian filter operation
 *
 *
 *************************************************************************************
 */
int
set_fgauss_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2  ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize_filter  = fwMskSize3x3 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_fmedian_params
 *
 * \brief
 *    This function sets the parameters for median filter operation
 *
 *
 *************************************************************************************
 */
int
set_fmedian_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3 ;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->maskSize.width  = 3 ;
	p->maskSize.height = 3 ;

	p->anchor.x = 1 ;
	p->anchor.y = 1 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_sharp_params
 *
 * \brief
 *    This function sets the parameters for sharpen operation
 *
 *
 *************************************************************************************
 */
int
set_sharp_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3 ;

	p->srcStep	= (bmp_info->biWidth * 3) ;
	p->dstStep  = (bmp_info->biWidth * 3) ;


	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width  = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_blur_params
 *
 * \brief
 *    This function sets the parameters for blur operation
 *
 *
 *************************************************************************************
 */
int
set_blur_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data + (bmp_info->biWidth * 3) + 3 ;

	p->srcStep	= (bmp_info->biWidth * 3)  ;
	p->dstStep  = (bmp_info->biWidth * 3)  ;

	p->dstRoiSize.width  = p->roiSize.width  - 2 ;
	p->dstRoiSize.height = p->roiSize.height - 2 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->anchor.x = 1  ;
	p->anchor.y = 1  ;

	p->maskSize.width =  3  ;
	p->maskSize.height = 3   ;


	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	memcpy(p->pDst,bmp_info->image_data,bmp_info->biWidth * bmp_info->biHeight * 3);

	return CONT ;
}


/*!
 *************************************************************************************
 * \function set_edgedetect_params
 *
 * \brief
 *    This function sets the parameters for edge detection operation
 *
 *
 *************************************************************************************
 */
int
set_edgedetect_params( st_input_format *bmp_info, st_API_parameters *p )
{
	int t=0 ;
	int j=0 ;
	int k=0 ;

	p->pSrc		   = bmp_info->image_data ;
	p->srcStep	   = bmp_info->biWidth * 3 ;
	p->dstStep	   = bmp_info->biWidth * 3 ;

	p->pSrc_edge1  = (float *)calloc(bmp_info->biWidth * bmp_info->biHeight * 3 + 2, sizeof(float)) ;
	if(p->pSrc_edge1 == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc_edge2  = (float *)calloc(bmp_info->biWidth * bmp_info->biHeight * 3 + 2, sizeof(float)) ;
	if(p->pSrc_edge2 == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	p->pSrc_edge3  = (float *)calloc(bmp_info->biWidth * bmp_info->biHeight * 3 + 2, sizeof(float)) ;
	if(p->pSrc_edge3 == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	for(int y=0;y<bmp_info->biHeight;++y)
	{
		for(int x=0; x < bmp_info->biWidth*3; x=x+3 )
		{
			t= (j + bmp_info->biWidth + 1)* 3;

			if(t > (bmp_info->biWidth * bmp_info->biHeight * 3))
			{
				t = bmp_info->biWidth * bmp_info->biHeight * 3;
			}

			p->pSrc_edge1[k]   = p->pSrc[k] ;
			p->pSrc_edge1[k+1] = p->pSrc[k+1] ;
			p->pSrc_edge1[k+2] = p->pSrc[k+2] ;

			p->pSrc_edge2[k]   = p->pSrc[k+3] ;
			p->pSrc_edge2[k+1] = p->pSrc[k+4] ;
			p->pSrc_edge2[k+2] = p->pSrc[k+5] ;

			p->pSrc_edge3[k]   = p->pSrc[t] ;
			p->pSrc_edge3[k+1] = p->pSrc[t+1] ;
			p->pSrc_edge3[k+2] = p->pSrc[t+2] ;
			k=k+3;
			j++;
		}
	}

	return CONT ;
}

/*!
 *************************************************************************************
 * \function set_contrast_params
 *
 * \brief
 *    This function sets the parameters for contrast operation
 *
 *
 *************************************************************************************
 */
int
set_contrast_params( st_input_format *bmp_info, st_API_parameters *p )
{
	p->pSrc		= bmp_info->image_data ;
	p->srcStep	= bmp_info->biWidth * 3 ;

	p->srcStep_con = bmp_info->biWidth * 3 * 4 ;

	p->pSrc_con  = (float *)malloc(bmp_info->biWidth * bmp_info->biHeight * 3 * 4) ;
	if(p->pSrc_con == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}
	// initialize contrast values
	if (p->adjust < -100)
	{
		p->adjust = -100;
	}
	if (p->adjust >  100)
	{
		p->adjust = 100;
	}

	p->adjust = (100.0 + p->adjust) / 100.0 ;
	p->adjust *= p->adjust ;

	return CONT ;
}


/*!
 *************************************************************************************
 * \function set_resize_params
 *
 * \brief
 *    This function sets the parameters for resize operation
 *
 *
 *************************************************************************************
 */
int
set_resize_params(double xFactor, double yFactor, st_input_format *bmp_info, st_API_parameters *p )
{
	p->xFactor	= xFactor ;
	p->yFactor  = yFactor ;

	p->pSrc		= bmp_info->image_data ;
	p->srcStep	= bmp_info->biWidth * 3 ;

	p->srcRoi.width  = p->roiSize.width  ;
	p->srcRoi.height = p->roiSize.height ;
	p->srcRoi.x = 0 ;
	p->srcRoi.y = 0 ;

	/* Changing Width and Height of original image */

	bmp_info->biWidth  =  (unsigned int)(bmp_info->biWidth  * xFactor) ;  // Change in structure
	bmp_info->biHeight =  (unsigned int)(bmp_info->biHeight * yFactor) ;  // Change in structure

	p->spare_roiSize.width  = bmp_info->biWidth  ;
	p->spare_roiSize.height = bmp_info->biHeight ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->dstStep = bmp_info->biWidth * 3 ;
	p->xShift = 0 ;
	p->yShift = 0 ;
	p->interpolation = 1 ;

	return CONT ;
}




/*!
 *************************************************************************************
 * \function set_rotate_params
 *
 * \brief
 *    This function sets the parameters for rotate operation
 *
 *
 *************************************************************************************
 */
int
set_rotate_params(double angle, st_input_format *bmp_info, st_API_parameters *p, int rot_cnt )
{
	p->angle	= angle ;

	p->pSrc		= bmp_info->image_data ;
	p->srcStep	= bmp_info->biWidth * 3 ;

	p->bor_width = bmp_info->biWidth;
	p->bor_height = bmp_info->biHeight;

	p->srcRoi.width = p->roiSize.width ;
	p->srcRoi.height = p->roiSize.height ;
	p->srcRoi.x = 0 ;
	p->srcRoi.y = 0 ;

	p->xShift = 0 ;
	p->yShift = 0 ;



	/* Changing Width and Height of original image */

	if(rot_cnt == 1)
	{
		double x;
		double y;


		if(p->angle < 0)
			p->angle +=360;
		int angle = (int)(p->angle/90);
		double rad = p->angle - ( angle*90);
		rad = rad * 3.141592654/180;

		if(((p->angle >= 0) && (p->angle <= 89)) || (p->angle == 360))
		{
			bmp_info->biWidth = (unsigned int )(p->bor_width * cos(rad) + p->bor_height * sin(rad));
			bmp_info->biHeight = (unsigned int )(p->bor_width * sin(rad) + p->bor_height * cos(rad));
			p->xShift = 0;
			p->yShift = p->bor_width * sin(rad);
		}
		else if((p->angle >= 90) && (p->angle <= 179))
		{
			bmp_info->biWidth = (unsigned int )(p->bor_width * sin(rad) + p->bor_height * cos(rad));
			bmp_info->biHeight = (unsigned int )(p->bor_width * cos(rad) + p->bor_height * sin(rad));
			x = p->bor_width * sin(rad);
			y = p->bor_height * cos(rad);
			p->xShift = p->bor_width * sin(rad);
			p->yShift = bmp_info->biHeight;
		}
		else if((p->angle >= 180) && (p->angle <= 269))
		{
			bmp_info->biWidth = (unsigned int )(p->bor_width * cos(rad) + p->bor_height * sin(rad));
			bmp_info->biHeight = (unsigned int ) (p->bor_width * sin(rad) + p->bor_height * cos(rad));
			x = p->bor_height * sin(rad);
			y = p->bor_width * cos(rad);
			p->xShift = bmp_info->biWidth;
			p->yShift =p->bor_height * cos(rad);
		}
		else if((p->angle >= 270) && (p->angle <= 359))
		{
			bmp_info->biWidth = (unsigned int )(p->bor_width * sin(rad) + p->bor_height * cos(rad));
			bmp_info->biHeight = (unsigned int )(p->bor_width * cos(rad) + p->bor_height * sin(rad));
			p->xShift = p->bor_height * cos(rad) ;
			p->yShift = 0;
		}

	}


	p->dstRoi.width =  bmp_info->biWidth ;
	p->dstRoi.height = bmp_info->biHeight ;
	p->dstRoi.x = 0 ;
	p->dstRoi.y = 0 ;

	p->pDst  = (unsigned char*)malloc(bmp_info->biWidth * bmp_info->biHeight * 3) ;
	if(p->pDst == NULL)
	{
		printf("Error : Cannot allocate memory \n") ;
		return RETN ;
	}

	p->dstStep = bmp_info->biWidth * 3 ;

	p->interpolation = 2 ;

	return CONT ;
}




/*!
 *************************************************************************************
 * \function check_yuv
 *
 * \brief
 *    This function returns 1 if YUV else returns 0
 *
 *
 *************************************************************************************
 */
int
check_file_type(char *file_name, st_input_format *bmp_info )
{
	int len ;
	len = strlen(file_name) ;
	if( (file_name[len-1] == 'v' || file_name[len-1] == 'V') &&
		(file_name[len-2] == 'u' || file_name[len-2] == 'U') &&
		(file_name[len-3] == 'y' || file_name[len-3] == 'Y')   )
	{
		bmp_info->file_type = YUV ;

		return YUV ;
	}
	if( (file_name[len-1] == 'p' || file_name[len-1] == 'P') &&
		(file_name[len-2] == 'm' || file_name[len-2] == 'M') &&
		(file_name[len-3] == 'b' || file_name[len-3] == 'B')   )
	{
		bmp_info->file_type = BMP ;

		return BMP ;
	}

	return WARN ;
}








