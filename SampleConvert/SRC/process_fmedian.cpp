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
 * \function process_fmedian
 *
 * \brief
 *    This function processes Median Filter operation.
 *
 *
 *************************************************************************************
 */

void
process_fmedian(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiFilterMedian_8u_C3R" :                                    */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer,						  */
	/*	replace each source pixel value with the median value of                      */
	/*	all pixels in the area defined by the mask, and write the                     */
	/*	filtered data to a destination buffer. The functions remove noise             */
	/*	without decreasing image brightness, as averaging filters do.                 */
    /*																				  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterMedian_8u_C3R(p->pSrc, p->srcStep, &p->pDst[(p->bor_width * 3) + 3] , p->dstStep, p->dstRoiSize, p->maskSize, p->anchor);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}









