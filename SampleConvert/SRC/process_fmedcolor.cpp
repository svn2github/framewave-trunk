/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
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
 * \function process_fmedcolor
 *
 * \brief
 *    This function processes Median Color Filter operation.
 *
 *
 *************************************************************************************
 */

void
process_fmedcolor(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/*  Calling API "fwiFilterMedianColor_8u_C3R" :                                   */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer,						  */
	/*	replaces each source pixel value with the value of the pixel                  */
	/*  nearest to the source pixel in the area defined by the mask,                  */
	/*	and writes the filtered data to a destination buffer.                         */
	/*	The function removes noise without decreasing image brightness,               */
	/*	while preserving the correlation between color components.                    */
	/*	The distance between the source and mask pixels is calculated as a sum of     */
	/*	absolute values using the following formula:                                  */
	/*	abs(R(i)-R(j)) + abs(G(i)-G(j)) + abs(B(i)-B(j)).                             */
    /*                                                                                */
    /*																				  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterMedianColor_8u_C3R(p->pSrc , p->srcStep, &p->pDst[(p->bor_width * 3) + 3], p->dstStep, p->dstRoiSize, p->maskSize_filter);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}





