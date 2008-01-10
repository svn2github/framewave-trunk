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
 * \function process_resize
 *
 * \brief
 *    This function processes resize operation.
 *
 *
 *************************************************************************************
 */

void
process_resize(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiResize_8u_C3R" :                                          */
	/*                                                                                */
	/* These functions step through an ROI in a source buffer and map the source      */
	/* data into a destination ROI, using x-y scaling factors and a specified         */
	/* method of interpolation			                                              */
	/*--------------------------------------------------------------------------------*/



	api_return_val = fwiResize_8u_C3R ( p->pSrc, p->roiSize, p->srcStep, p->srcRoi,p->pDst,
										p->dstStep,p->spare_roiSize, p->xFactor, p->yFactor, p->interpolation );

	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}


