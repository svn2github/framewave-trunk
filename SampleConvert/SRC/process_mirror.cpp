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
 * \function process_mirror
 *
 * \brief
 *    This function processes mirror operation.
 *
 *
 *************************************************************************************
 */

void
process_mirror(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMirror_8u_C3IR" :                                         */
	/*                                                                                */
	/* These functions step through an ROI in a source buffer and mirror the source   */
	/* data about the vertical axis, the horizontal axis, or both axes.               */
	/* The result is written back to destination location.                            */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiMirror_8u_C3IR(p->pSrcDst, p->srcDstStep, p->roiSize, p->flip);


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}


