/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifdef USE_BMPAPI
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lm.h>
#include"fwImage.h"
#include <globals.h>


/*!
 *************************************************************************************
 * \function ConvertRGBToBMPBuffer
 *
 * \brief
 *    This function converts RGB to BMP buffer
 *
 *
 *************************************************************************************
 */
BYTE*
ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, long* newsize )
{

	FwiSize	roiSize ;

	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	roiSize.width = width ;
	roiSize.height = height ;
	if ( fwiMirror_8u_C3IR(Buffer, width*3, roiSize, fwAxsHorizontal) < 0 )
		return NULL ;

	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )
		padding++;

	int psw = scanlinebytes + padding;
	*newsize = height * psw;
	BYTE* newbuf = new BYTE[*newsize];
	memset ( newbuf, 0, *newsize );

	long bufpos = 0;
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			bufpos = y * 3 * width + x;
			newpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos];
			newbuf[newpos + 1] = Buffer[bufpos + 1];
			newbuf[newpos + 2] = Buffer[bufpos+2];
		}

	return newbuf;
}

/*!
 *************************************************************************************
 * \function ConvertBMPToRGBBuffer
 *
 * \brief
 *    This function converts BMP to RGB buffer
 *
 *
 *************************************************************************************
 */

BYTE*
ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height )
{
	FwiSize	roiSize ;


	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;



	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )
		padding++;

	int psw = scanlinebytes + padding;
	BYTE* newbuf = new BYTE[width*height*3];
	long bufpos = 0;
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos];
			newbuf[newpos + 1] = Buffer[bufpos+1];
			newbuf[newpos + 2] = Buffer[bufpos+2];
		}

	roiSize.width = width ;
	roiSize.height = height ;

	if ( fwiMirror_8u_C3IR(newbuf, width*3, roiSize, fwAxsHorizontal) < 0 )
		return NULL ;
	return newbuf;
}

/*!
 *************************************************************************************
 * \function SaveBMP
 *
 * \brief
 *    This function writes a BMP file
 *
 *
 *************************************************************************************
 */
bool
SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, char* bmpfile, st_input_format *bmp_data )
{

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	memset ( &bmfh, 0, sizeof (BITMAPFILEHEADER ) );
	memset ( &info, 0, sizeof (BITMAPINFOHEADER ) );


	bmfh.bfType = 0x4d42;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + paddedsize;
	bmfh.bfOffBits = 0x36;

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	char *szSource;

	szSource = &bmpfile[0];
	WCHAR szDest[100];
	MultiByteToWideChar( GetACP(), 0, szSource, -1, szDest, sizeof(szDest)/sizeof(szDest[0]) );



	HANDLE file = CreateFile ( bmpfile , GENERIC_WRITE, FILE_SHARE_READ,
		 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( file == NULL )
	{
		CloseHandle ( file );
		return false;
	}


	unsigned long bwritten;
	if ( WriteFile ( file, &bmfh, sizeof ( BITMAPFILEHEADER ), &bwritten, NULL ) == false )
	{
		CloseHandle ( file );
		return false;
	}

	if ( WriteFile ( file, &info, sizeof ( BITMAPINFOHEADER ), &bwritten, NULL ) == false )
	{
		CloseHandle ( file );
		return false;
	}

	if ( WriteFile ( file, Buffer, paddedsize, &bwritten, NULL ) == false )
	{
		CloseHandle ( file );
		return false;
	}
	CloseHandle ( file );
	return true;
}

/*!
 *************************************************************************************
 * \function LoadBMP
 *
 * \brief
 *    This function reads a BMP file
 *
 *
 *************************************************************************************
 */
BYTE*
LoadBMP ( int* width, int* height, long* size, char* bmpfile, st_input_format *bmp_data )
{

	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	DWORD bytesread;
	char *szSource;

	szSource = &bmpfile[0];
	WCHAR szDest[100];
	MultiByteToWideChar( GetACP(), 0, szSource, -1, szDest, sizeof(szDest)/sizeof(szDest[0]) );


	HANDLE file = CreateFile ( bmpfile , GENERIC_READ, FILE_SHARE_READ,
		 NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if ( NULL == file )
		return NULL;



	if ( ReadFile ( file, &bmpheader, sizeof ( BITMAPFILEHEADER ), &bytesread, NULL ) == false )
	{
		CloseHandle ( file );
		return NULL;
	}
	if ( ReadFile ( file, &bmpinfo, sizeof ( BITMAPINFOHEADER ), &bytesread, NULL ) == false )
	{
		CloseHandle ( file );
		return NULL;
	}
	if ( bmpheader.bfType != 'MB' )
	{
		CloseHandle ( file );
		return NULL;
	}
	*width   = bmpinfo.biWidth;
	*height  = abs ( bmpinfo.biHeight );
	if ( bmpinfo.biCompression != BI_RGB )
	{
		CloseHandle ( file );
		return NULL;
	}
	if ( bmpinfo.biBitCount != 24 )
	{
		CloseHandle ( file );
		return NULL;
	}

	bmp_data->bfType[0]		= 'B' ;
	bmp_data->bfType[1]		= 'M' ;
	bmp_data->bfSize		= bmpheader.bfSize ;
	bmp_data->bfReserved1	= bmpheader.bfReserved1 ;
	bmp_data->bfReserved2	= bmpheader.bfReserved2 ;
	bmp_data->bfOffBits		= bmpheader.bfOffBits ;

	bmp_data->biSize			= bmpinfo.biSize ;
	bmp_data->biWidth			= bmpinfo.biWidth ;
	bmp_data->biHeight			= bmpinfo.biHeight ;
	bmp_data->biPlanes			= bmpinfo.biPlanes ;
	bmp_data->biBitCount		= bmpinfo.biBitCount ;
	bmp_data->biCompression		= bmpinfo.biCompression ;
	bmp_data->biSizeImage		= bmpinfo.biSizeImage ;
	bmp_data->biXPelsPerMeter	= bmpinfo.biXPelsPerMeter ;
	bmp_data->biYPelsPerMeter	= bmpinfo.biYPelsPerMeter ;
	bmp_data->biClrUsed			= bmpinfo.biClrUsed ;
	bmp_data->biClrImportant	= bmpinfo.biClrImportant ;

	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[ *size ];

	SetFilePointer ( file, bmpheader.bfOffBits, NULL, FILE_BEGIN );

	if ( ReadFile ( file, Buffer, *size, &bytesread, NULL ) == false )
	{
		delete [] Buffer;
		CloseHandle ( file );
		return NULL;
	}

	CloseHandle ( file );
	return Buffer;
}

#endif
