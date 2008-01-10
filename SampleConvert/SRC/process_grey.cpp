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
 * \function process_grey
 *
 * \brief
 *    This function processes RGB to Greyscale conversion.
 *
 *
 *************************************************************************************
 */

void
process_grey(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiDivC_8u_C3IRSfs" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and divides              */
	/* the source data by a constant value. The quotient is written back to           */
	/* the source location.                                                           */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiDivC_8u_C3IRSfs(p->value, p->pSrcDst, p->srcDstStep, p->roiSize, p->scaleFactor);


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}


