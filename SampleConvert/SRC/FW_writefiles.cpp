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
 * \function write_outputfile
 *
 * \brief
 *    This function writes a output.
 *
 *
 *************************************************************************************
 */
int
write_outputfile(char *bmp_file, st_input_format *bmp_data)
{
	FILE				*fp ;
	int					image_size ;
	int					ht_x_wd ;
	st_API_parameters	api_params_yuv ;
	unsigned char		temp ;
	int					i ;
	int					rem ;

	//image_size = (bmp_data->biWidth * bmp_data->biHeight * 3) + ((4-(bmp_data->biWidth % 4))* bmp_data->biHeight) ;


	rem = bmp_data->biWidth*3 % 4 ;
	if(rem != 0)
	{
		rem = 4 - rem ;
		image_size  =  ( ( (bmp_data->biWidth) * 3) + rem  ) * bmp_data->biHeight ;
	}
	else
	{
		image_size  =  (bmp_data->biWidth) * (bmp_data->biHeight) * 3 ;
	}

	ht_x_wd = bmp_data->biWidth * bmp_data->biHeight ;

	bmp_data->bfType[0] = 'B' ;
	bmp_data->bfType[1] = 'M' ;
	bmp_data->bfSize = image_size+54 ;

	bmp_data->bfReserved1 = 0 ;
	bmp_data->bfReserved2 = 0 ;
	bmp_data->bfOffBits = 54 ;

	/* Image Header */
	bmp_data->biSize = 40 ;
	bmp_data->biPlanes = 1 ;
	bmp_data->biBitCount = 24 ;
	bmp_data->biCompression = 0 ;
	bmp_data->biSizeImage = image_size ;

	bmp_data->biXPelsPerMeter = 0 ;
	bmp_data->biYPelsPerMeter = 0 ;
	bmp_data->biClrUsed = 0 ;
	bmp_data->biClrImportant = 0 ;


	check_file_type(bmp_file, bmp_data ) ;



	switch(bmp_data->file_type)
	{
		case BMP:
#ifdef USE_BMPAPI
					if ( write_bmpfile_API(bmp_file, bmp_data) == RETN )
						return RETN ;
#else
					fp = fopen(bmp_file, "wb" ) ;
					if ( fp == NULL )
					{
						printf("Unable to open file '%s' \n", bmp_file ) ;
						return RETN ;
					}
					write_bmpfile(fp, bmp_data) ;
					fclose(fp)  ;
#endif
					break ;
		case YUV:
					fp = fopen(bmp_file, "wb" ) ;
					if ( fp == NULL )
					{
						printf("Unable to open file '%s' \n", bmp_file ) ;
						return RETN ;
					}
					write_yuvfile(fp, bmp_data) ;
					fclose(fp)  ;
					break ;
		default:
					break ;
	}




	return CONT ;
}








/*!
 *************************************************************************************
 * \function write_yuvfile
 *
 * \brief
 *    This function writes a 'yuv' file
 *
 *
 *************************************************************************************
 */
int
write_yuvfile(FILE *fp, st_input_format *bmp_data)
{
	int					ht_x_wd ;
	unsigned char		temp ;
	int					i ;
	FwiSize			roiSize ;
	FwiAxis			flip ;

	st_API_parameters	api_params_yuv ;

	FwiSize			swap_roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;

	swap_roiSize.width  = bmp_data->biWidth ;
	swap_roiSize.height = bmp_data->biHeight ;

	/* Set the values of data structure */


	api_params_yuv.roiSize.width = bmp_data->biWidth ;
	api_params_yuv.roiSize.height = bmp_data->biHeight ;
	api_params_yuv.flip = fwAxsHorizontal ;

	ht_x_wd = bmp_data->biWidth * bmp_data->biHeight ;

	api_params_yuv.pSrcDst = bmp_data->image_data ;
	api_params_yuv.srcDstStep = bmp_data->biWidth * 3 ;


	for(i=0; i<3; i++)
	{
		api_params_yuv.pDst_yuv[i] = (unsigned char *)malloc(ht_x_wd*sizeof(char)) ;
		if(api_params_yuv.pDst_yuv[i] == NULL )
		{
			printf("Cannot allocate memory\n") ;
			return RETN ;
		}
	}

	switch(bmp_data->format)
	{
		case 444:
					api_params_yuv.dstStep_yuv[0] = bmp_data->biWidth ;

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, swap_roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/
/*
					try{
						process_mirror(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/

					try{
						process_rgbtoyuv(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}

					fwrite(api_params_yuv.pDst_yuv[0], 1, ht_x_wd, fp ) ;
					fwrite(api_params_yuv.pDst_yuv[1], 1, ht_x_wd, fp ) ;
					fwrite(api_params_yuv.pDst_yuv[2], 1, ht_x_wd, fp ) ;

					break ;
		case 420:
					api_params_yuv.dstStep_yuv[0] = bmp_data->biWidth ;
					api_params_yuv.dstStep_yuv[1] = bmp_data->biWidth/2 ;
					api_params_yuv.dstStep_yuv[2] = bmp_data->biWidth/2 ;

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, swap_roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/

/*
					try{
						process_mirror(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/

					try{
						process_rgbtoycbcr420(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}


					fwrite(api_params_yuv.pDst_yuv[0], 1, ht_x_wd,   fp ) ;
					fwrite(api_params_yuv.pDst_yuv[1], 1, ht_x_wd/4, fp ) ;
					fwrite(api_params_yuv.pDst_yuv[2], 1, ht_x_wd/4, fp ) ;
					break ;
		case 422:
					api_params_yuv.dstStep_yuv[0] = bmp_data->biWidth ;
					api_params_yuv.dstStep_yuv[1] = bmp_data->biWidth/2 ;
					api_params_yuv.dstStep_yuv[2] = bmp_data->biWidth/2 ;

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, swap_roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/

/*
					try{
						process_mirror(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/
					try{
						process_rgbtoyuv422(&api_params_yuv) ;
					}catch(int val){
						error_handler(val) ;
						return RETN ;
					}
					fwrite(api_params_yuv.pDst_yuv[0], 1, ht_x_wd,   fp ) ;
					fwrite(api_params_yuv.pDst_yuv[1], 1, ht_x_wd/2, fp ) ;
					fwrite(api_params_yuv.pDst_yuv[2], 1, ht_x_wd/2, fp ) ;
		default:
					;
	}

	for(i=0; i<3; i++)
		free(api_params_yuv.pDst_yuv[i]) ;


	return CONT ;
}


#ifdef USE_BMPAPI

/*!
 *************************************************************************************
 * \function write_bmpfile_API
 *
 * \brief
 *    This function writes a 'bmp' file in 24 bpp format
 *
 *
 *************************************************************************************
 */
int
write_bmpfile_API(char *bmp_file, st_input_format *bmp_data)
{
	long	s, s2;

	FwiSize			roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;
	st_API_parameters	api_params ;

	roiSize.width  = bmp_data->biWidth ;
	roiSize.height = bmp_data->biHeight ;

	api_params.pSrcDst		= bmp_data->image_data ;
	api_params.srcDstStep	= bmp_data->biWidth * 3 ;
	api_params.flip			= fwAxsHorizontal ;
	api_params.dstStep		= bmp_data->biWidth * 3 ;
	api_params.roiSize		= roiSize ;

	//fwiMirror_8u_C3IR(bmp_data->image_data, bmp_data->biWidth * 3, roiSize, fwAxsHorizontal);


	try {
		process_mirror(&api_params);
	}
	catch(int val){
		error_handler(val) ;
		return RETN ;
	}



	bmp_data->image_data = ConvertRGBToBMPBuffer ( bmp_data->image_data, bmp_data->biWidth, bmp_data->biHeight, &s2 );
	if(NULL == bmp_data->image_data)
	{
		printf("Error writing BMP file \n") ;
		return RETN ;
	}
	SaveBMP ( bmp_data->image_data, bmp_data->biWidth, bmp_data->biHeight, s2, bmp_file, bmp_data );
	if(NULL == bmp_data->image_data)
	{
		printf("Error writing BMP file \n") ;
		return RETN ;
	}
	return CONT ;
}
#endif



/*!
 *************************************************************************************
 * \function write_bmpfile
 *
 * \brief
 *    This function writes a 'bmp' file in 24 bpp format
 *
 *
 *************************************************************************************
 */
int
write_bmpfile(FILE *fp, st_input_format *bmp_data)
{

	int		image_size = 0 ;
	int		check_pad ;
	int		rem ;
	unsigned char *tmp_image_data ;
	int		i, count=0 ;
	int		j ;
	int		k ;

	FwiSize			roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;
	st_API_parameters	api_params ;



/* BMP File Header */

	fwrite(&bmp_data->bfType[0],		1, 2, fp ) ; /* The characters "BM" */
	fwrite(&bmp_data->bfSize,			1, 4, fp ) ; /* The size of the file in bytes */
	fwrite(&bmp_data->bfReserved1,		1, 2, fp ) ; /* Unused - must be zero */
	fwrite(&bmp_data->bfReserved2,		1, 2, fp ) ; /* Unused - must be zero */
	fwrite(&bmp_data->bfOffBits,		1, 4, fp ) ; /* Offset to start of Pixel Data */

/* BMP Image Header */

	fwrite(&bmp_data->biSize,			1, 4, fp ) ; /* Header Size - Must be at least 40 */
	fwrite(&bmp_data->biWidth,			1, 4, fp ) ; /* Image width in pixels */
	fwrite(&bmp_data->biHeight,			1, 4, fp ) ; /* Image height in pixels */
	fwrite(&bmp_data->biPlanes,			1, 2, fp ) ; /* Must be 1 */
	fwrite(&bmp_data->biBitCount,		1, 2, fp ) ; /* Bits per pixel - 1, 2, 4, 8, 16, 24, or 32 */
	fwrite(&bmp_data->biCompression,	1, 4, fp ) ; /* Compression type (0 = uncompressed) */
	fwrite(&bmp_data->biSizeImage,		1, 4, fp ) ; /* Image Size - may be zero for uncompressed images */
	fwrite(&bmp_data->biXPelsPerMeter,	1, 4, fp ) ; /* Preferred resolution in pixels per meter */
	fwrite(&bmp_data->biYPelsPerMeter,	1, 4, fp ) ; /* Preferred resolution in pixels per meter */
	fwrite(&bmp_data->biClrUsed,		1, 4, fp ) ; /* Number Color Map entries that are actually used */
	fwrite(&bmp_data->biClrImportant,	1, 4, fp ) ; /*	Number of significant colors */





	roiSize.width  = bmp_data->biWidth ;
	roiSize.height = bmp_data->biHeight ;
	//fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, roiSize, dstOrder);

	api_params.pSrcDst		= bmp_data->image_data ;
	api_params.srcDstStep	= bmp_data->biWidth * 3 ;
	api_params.flip			= fwAxsHorizontal ;
	api_params.dstStep		= bmp_data->biWidth * 3 ;
	api_params.roiSize		= roiSize ;

	//fwiMirror_8u_C3IR(bmp_data->image_data, bmp_data->biWidth * 3, roiSize, fwAxsHorizontal);


	try {
		process_mirror(&api_params);
	}
	catch(int val){
		error_handler(val) ;
		return RETN ;
	}






	rem = bmp_data->biWidth*3 % 4 ;
	if(rem != 0)
	{
		rem = 4 - rem ;
		image_size  =  ( ( (bmp_data->biWidth) * 3) + rem  ) * bmp_data->biHeight ;
	}
	else
	{
		image_size  =  (bmp_data->biWidth) * (bmp_data->biHeight) * 3 ;
	}


	/* Check for padding */
	if( rem == 1 || rem == 2 || rem == 3)
	{
		tmp_image_data = (unsigned char *)malloc(image_size * sizeof(unsigned char) ) ;
		if( !tmp_image_data )
		{
			printf("Cannot allocate memory for 'tmp_image_data' \n" ) ;
			return RETN ;
		}
		memcpy(tmp_image_data, bmp_data->image_data, image_size ) ;
		check_pad = (bmp_data->biWidth * 3)  ;

		j=0 ; i=0 ;
		while(j<image_size)
		{
			bmp_data->image_data[j++] = tmp_image_data[i++] ;
			if( (i%check_pad) == 0 && i != 0)
				for(k=0;k<rem;k++)
					bmp_data->image_data[j++] = 0 ;
		}
		free(tmp_image_data) ;
	}


	fwrite(&bmp_data->image_data[0], 1, image_size, fp ) ;

	return CONT ;
}




