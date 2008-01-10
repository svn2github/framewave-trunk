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
 * \function process_fsobhoriz
 *
 * \brief
 *    This function processes Sobel Horizontal Filter operation.
 *
 *
 *************************************************************************************
 */

void
process_fsobhoriz(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/*  Calling API "fwiFilterSobelHoriz_8u_C3R" :                               */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer, applies a horizontal   */
	/*	Sobel operator to the source data, and write the filtered data to a           */
	/*	destination buffer. This function enhance and smoothens the horizontal edges  */
	/*	of an image.                                                                  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterSobelHoriz_8u_C3R(p->pSrc , p->srcStep, &p->pDst[(p->bor_width * 3) + 3] , p->dstStep, p->dstRoiSize);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}








