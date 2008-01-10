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
 * \function read_inputfile
 *
 * \brief
 *    This function reads an input file
 *
 *
 *************************************************************************************
 */
int
read_inputfile(char *bmp_file, st_input_format *bmp_data)
{
	FILE	*fp ;

	switch(bmp_data->file_type)
	{
		case BMP:
#ifdef USE_BMPAPI
					if (read_bmpfile_API(bmp_file, bmp_data) == RETN )
						return RETN ;
#else
					fp = fopen(bmp_file, "rb" ) ;
					if ( fp == NULL )
					{
						printf("Unable to open file '%s' \n", bmp_file ) ;
						return RETN ;
					}
					read_bmpfile(fp, bmp_data) ;
					fclose(fp)  ;
#endif
					break ;
		case YUV:
					fp = fopen(bmp_file, "rb" ) ;
					if ( fp == NULL )
					{
						printf("Unable to open file '%s' \n", bmp_file ) ;
						return RETN ;
					}
					read_yuvfile(fp, bmp_data) ;
					fclose(fp)  ;
					break ;
		default:
					break ;
	}



	return CONT ;
}


/*!
 *************************************************************************************
 * \function read_yuvfile
 *
 * \brief
 *    This function reads a 'yuv' file
 *
 *
 *************************************************************************************
 */
int
read_yuvfile(FILE *fp, st_input_format *bmp_data)
{
	int					image_size ;
	int					ht_x_wd ;
	st_API_parameters	api_params_yuv ;
	unsigned char		temp ;
	int					i ;
	FwiSize			roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;

	roiSize.width  = bmp_data->biWidth ;
	roiSize.height = bmp_data->biHeight ;


	image_size = (bmp_data->biWidth * bmp_data->biHeight * 3) + ((4-(bmp_data->biWidth % 4))* bmp_data->biHeight) ;
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

	bmp_data->image_data = (unsigned char *)malloc(image_size * sizeof(unsigned char) ) ;
	if( !(bmp_data->image_data) )
	{
		printf("Cannot allocate memory for 'bmp_data->image_data' \n" ) ;
		return RETN ;
	}

	switch(bmp_data->format)
	{
		case 444:
					bmp_data->YUV = (unsigned char *)malloc(3*ht_x_wd * sizeof(unsigned char) ) ;
					if( !(bmp_data->YUV) )
					{
						printf("Cannot allocate memory for 'bmp_data->YUV' \n" ) ;
						return RETN ;
					}
					fread(&bmp_data->YUV[0], 1, ht_x_wd * 3, fp ) ;

					api_params_yuv.pSrc_yuv[0] = bmp_data->YUV ;
					api_params_yuv.pSrc_yuv[1] = bmp_data->YUV + ht_x_wd ;
					api_params_yuv.pSrc_yuv[2] = bmp_data->YUV + 2*ht_x_wd ;

					api_params_yuv.srcStep_yuv[0] = bmp_data->biWidth  ;
					api_params_yuv.srcStep_yuv[1] = bmp_data->biWidth  ;
					api_params_yuv.srcStep_yuv[2] = bmp_data->biWidth  ;

					api_params_yuv.pDst    = bmp_data->image_data ;
					api_params_yuv.dstStep = bmp_data->biWidth * 3 ;

					api_params_yuv.roiSize.width  = bmp_data->biWidth ;
					api_params_yuv.roiSize.height = bmp_data->biHeight ;

					try {
						process_ycbcrtorgb(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}


					/* converting data into BGR format */

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/

					api_params_yuv.pSrcDst    = bmp_data->image_data ;
					api_params_yuv.srcDstStep = bmp_data->biWidth * 3 ;
					api_params_yuv.flip       = fwAxsHorizontal ;
/*
					try {
						process_mirror(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/
					break ;
		case 420:
					bmp_data->YUV = (unsigned char *)malloc( (size_t)(1.5*ht_x_wd * sizeof(unsigned char)) ) ;
					if( !(bmp_data->YUV) )
					{
						printf("Cannot allocate memory for 'bmp_data->YUV' \n" ) ;
						return RETN ;
					}
					fread(&bmp_data->YUV[0], 1, (size_t)(ht_x_wd * 1.5), fp ) ;

					api_params_yuv.pSrc_yuv[0] = bmp_data->YUV ;
					api_params_yuv.pSrc_yuv[1] = bmp_data->YUV + ht_x_wd ;
					api_params_yuv.pSrc_yuv[2] = bmp_data->YUV + ht_x_wd +  ht_x_wd/4;


					api_params_yuv.srcStep_yuv[0] = bmp_data->biWidth ;
					api_params_yuv.srcStep_yuv[1] = bmp_data->biWidth / 2 ;
					api_params_yuv.srcStep_yuv[2] = bmp_data->biWidth / 2 ;

					api_params_yuv.pDst    = bmp_data->image_data ;
					api_params_yuv.dstStep = bmp_data->biWidth * 3 ;

					api_params_yuv.roiSize.width  = bmp_data->biWidth ;
					api_params_yuv.roiSize.height = bmp_data->biHeight ;

					try {
						process_ycbcr420torgb(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}


					/* converting data into BGR format */

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/

					api_params_yuv.pSrcDst    = bmp_data->image_data ;
					api_params_yuv.srcDstStep = bmp_data->biWidth * 3 ;
					api_params_yuv.flip       = fwAxsHorizontal ;
/*
					try {
						process_mirror(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/

					break ;
		case 422:
					bmp_data->YUV = (unsigned char *)malloc(2*ht_x_wd * sizeof(unsigned char) ) ;
					if( !(bmp_data->YUV) )
					{
						printf("Cannot allocate memory for 'bmp_data->YUV' \n" ) ;
						return RETN ;
					}
					fread(&bmp_data->YUV[0], 1, ht_x_wd * 2, fp ) ;

					api_params_yuv.pSrc_yuv[0] = bmp_data->YUV ;
					api_params_yuv.pSrc_yuv[1] = bmp_data->YUV + ht_x_wd ;
					api_params_yuv.pSrc_yuv[2] = bmp_data->YUV + ht_x_wd +  ht_x_wd/2;


					api_params_yuv.srcStep_yuv[0] = bmp_data->biWidth ;
					api_params_yuv.srcStep_yuv[1] = bmp_data->biWidth/2  ;
					api_params_yuv.srcStep_yuv[2] = bmp_data->biWidth/2  ;

					api_params_yuv.pDst    = bmp_data->image_data ;
					api_params_yuv.dstStep = bmp_data->biWidth * 3 ;

					api_params_yuv.roiSize.width  = bmp_data->biWidth ;
					api_params_yuv.roiSize.height = bmp_data->biHeight ;

					try {
						process_ycbcr422torgb(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}


					/* converting data into BGR format */

					fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, roiSize, dstOrder);

					/*for(i=0;i<ht_x_wd *3;i=i+3)
					{
						temp = bmp_data->image_data[i] ;
						bmp_data->image_data[i] = bmp_data->image_data[i+2] ;
						bmp_data->image_data[i+2] = temp ;
					}*/

					api_params_yuv.pSrcDst    = bmp_data->image_data ;
					api_params_yuv.srcDstStep = bmp_data->biWidth * 3 ;
					api_params_yuv.flip       = fwAxsHorizontal ;
/*
					try {
						process_mirror(&api_params_yuv);
					}
					catch(int val){
						error_handler(val) ;
						return RETN ;
					}
*/
					break ;
		default:
					;
	}

	free(bmp_data->YUV) ;
	return CONT ;
}


#ifdef USE_BMPAPI
/*!
 *************************************************************************************
 * \function read_bmpfile_API
 *
 * \brief
 *    This function reads a 'bmp' file in 24 bpp format
 *
 *
 *************************************************************************************
 */

int
read_bmpfile_API(char *filename, st_input_format *bmp_data)
{
	int		width  ;
	int		height ;
	long	s, s2;


	FwiSize			roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;
	st_API_parameters	api_params ;


	bmp_data->image_data = LoadBMP ( &width, &height, &s, filename, bmp_data );
	if(NULL == bmp_data->image_data)
	{
		printf("Error reading BMP file \n") ;
		return RETN ;
	}
	bmp_data->image_data = ConvertBMPToRGBBuffer ( bmp_data->image_data, width, height );

	if(NULL == bmp_data->image_data)
	{
		printf("Error reading BMP file \n") ;
		return RETN ;
	}

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


	return CONT ;
}
#endif



/*!
 *************************************************************************************
 * \function read_bmpfile
 *
 * \brief
 *    This function reads a 'bmp' file in 24 bpp format
 *
 *
 *************************************************************************************
 */
int
read_bmpfile(FILE *fp, st_input_format *bmp_data)
{

	int		image_size = 0 ;
	int		rem = 0 ;
	int		check_pad ;
	unsigned char *tmp_image_data ;
	unsigned char c ;
	int		i ;
	int		j ;
	int		k ;
	FwiSize			roiSize ;
	int dstOrder[3] = { 2, 1, 0 } ;
	st_API_parameters	api_params ;


	/* BMP File Header */

	fread(&bmp_data->bfType[0],		1, 2, fp ) ; /* The characters "BM" */
	fread(&bmp_data->bfSize,		1, 4, fp ) ; /* The size of the file in bytes */
	fread(&bmp_data->bfReserved1,   1, 2, fp ) ; /* Unused - must be zero */
	fread(&bmp_data->bfReserved2,	1, 2, fp ) ; /* Unused - must be zero */
	fread(&bmp_data->bfOffBits,		1, 4, fp ) ; /* Offset to start of Pixel Data */

#ifdef DEBUG_PRINT
	printf("--------------------------------\n") ;
	printf("File Header \n") ;
	printf("--------------------------------\n") ;

	printf("1st 2 chars of bmp file (must be BM) is %c%c ....\n", bmp_data->bfType[0],bmp_data->bfType[1]) ;
	printf("Size of file in bytes %d \n", bmp_data->bfSize ) ;
	printf("Unused - must be zero %x \n", bmp_data->bfReserved1) ;
	printf("Unused - must be zero %x \n", bmp_data->bfReserved2) ;
	printf("Offset to start of Pixel Data %x \n", bmp_data->bfOffBits) ;
#endif


/* BMP Image Header */

	fread(&bmp_data->biSize,			1, 4, fp ) ; /* Header Size - Must be at least 40 */
	fread(&bmp_data->biWidth,			1, 4, fp ) ; /* Image width in pixels */
	fread(&bmp_data->biHeight,			1, 4, fp ) ; /* Image height in pixels */
	fread(&bmp_data->biPlanes,			1, 2, fp ) ; /* Must be 1 */
	fread(&bmp_data->biBitCount,		1, 2, fp ) ; /* Bits per pixel - 1, 2, 4, 8, 16, 24, or 32 */
	fread(&bmp_data->biCompression,		1, 4, fp ) ; /* Compression type (0 = uncompressed) */
	fread(&bmp_data->biSizeImage,		1, 4, fp ) ; /* Image Size - may be zero for uncompressed images */
	fread(&bmp_data->biXPelsPerMeter,	1, 4, fp ) ; /* Preferred resolution in pixels per meter */
	fread(&bmp_data->biYPelsPerMeter,	1, 4, fp ) ; /* Preferred resolution in pixels per meter */
	fread(&bmp_data->biClrUsed,			1, 4, fp ) ; /* Number Color Map entries that are actually used */
	fread(&bmp_data->biClrImportant,	1, 4, fp ) ; /*	Number of significant colors */

#ifdef DEBUG_PRINT
	printf("--------------------------------\n") ;
	printf("Image Header \n") ;
	printf("--------------------------------\n") ;
	printf("Header Size - Must be at least 40D OR 28H %x \n", bmp_data->biSize ) ;
	printf("Image width in pixels  %d \n", bmp_data->biWidth ) ;
	printf("Image height in pixels  %d \n", bmp_data->biHeight ) ;
	printf("Must be 1 %x \n", bmp_data->biPlanes) ;
	printf("Bits per pixel %d \n", bmp_data->biBitCount) ;

	printf("Compression type (0 = uncompressed) %x \n", bmp_data->biCompression ) ;
	printf("Image Size - may be zero for uncompressed images %x \n", bmp_data->biSizeImage) ;
	printf("Preferred resolution in pixels per meter %x \n", bmp_data->biXPelsPerMeter) ;
	printf("Preferred resolution in pixels per meter %x \n", bmp_data->biYPelsPerMeter) ;
	printf("Number Color Map entries that are actually used %x \n", bmp_data->biClrUsed) ;
	printf("Number of significant colors  %x \n", bmp_data->biClrImportant) ;
#endif


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


	if(bmp_data->biBitCount == 24 )
	{
		/* Read entire RGB Data */
		bmp_data->image_data = (unsigned char *)malloc(image_size * sizeof(unsigned char) ) ;
		if( !(bmp_data->image_data) )
		{
			printf("Cannot allocate memory for 'bmp_data->image_data' \n" ) ;
			return RETN ;
		}

		fread(&bmp_data->image_data[0], 1, image_size, fp ) ;


		/* Check for padding */
		if( rem == 1 || rem == 2 || rem == 3)
		{
			check_pad = (bmp_data->biWidth * 3)  ;
			tmp_image_data = bmp_data->image_data ;
			j=0 ; i=0 ;
			while(i<image_size)
			{
				bmp_data->image_data[j++] = tmp_image_data[i++] ;
				if( (j%check_pad) == 0 && j != 0)
					for(k=0;k<rem;k++)
						i++ ;
			}
		}
	}
	else
	{
		printf("Only '24 bpp' bmp file format supported\n" ) ;
		fclose(fp) ;
		return RETN ;
	}

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
// Test the read the file
/*
	{
		FILE *fp ;
		fp = fopen("test.raw", "wb" ) ;
		fwiSwapChannels_8u_C3IR( bmp_data->image_data, bmp_data->biWidth * 3, roiSize, dstOrder);
		fwrite(&bmp_data->image_data[0], 1, bmp_data->biWidth*bmp_data->biHeight*3, fp ) ;
		exit(1) ;
	}
*/
	return CONT ;
}






