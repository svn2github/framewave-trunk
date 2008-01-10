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
 * \function process_rotate
 *
 * \brief
 *    This function processes rotate operation.
 *
 *
 *************************************************************************************
 */

void
process_rotate(st_API_parameters *p)
{
	int		api_return_val ;
	double  xCenter ;
	double	yCenter ;

	TIME_START		/* Start Timer */


	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiRotate_8u_C3R" :                                          */
	/*                                                                                */
	/* These functions step through an ROI in a source buffer, rotate the source      */
	/* data around (0,0) by a specified angle, shift the x-y coordinates according    */
	/* to the x and y shift values, and map the data into a destination ROI using a   */
	/* specified method of interpolation			                                  */
	/*--------------------------------------------------------------------------------*/
	api_return_val = fwiRotate_8u_C3R ( p->pSrc ,p->roiSize ,p->srcStep , p->srcRoi ,p->pDst,
						 p->dstStep ,  p->dstRoi , p->angle ,  p->xShift ,  p->yShift ,  p->interpolation );


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}



