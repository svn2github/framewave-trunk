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
 * \function process_fmedcross
 *
 * \brief
 *    This function processes Median Cross Filter operation.
 *
 *
 *************************************************************************************
 */

void
process_fmedcross(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/*  Calling API "fwiFilterMedianCross_8u_C3R" :                              */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer,						  */
	/*	replaces each source pixel value with the median value of all                 */
	/*  pixels in the area defined by the cross mask,                                 */
	/*	and writes the filtered data to a destination buffer.                         */
	/*	The function removes noise without decreasing image brightness.               */
    /*																				  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterMedianCross_8u_C3R(p->pSrc , p->srcStep, &p->pDst[(p->bor_width * 3) + 3] , p->dstStep, p->dstRoiSize, p->maskSize_filter);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}







