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
 * \function process_negate
 *
 * \brief
 *    This function processes negate operation.
 *
 *
 *************************************************************************************
 */

void
process_negate(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiNot_8u_C3IR" :                                            */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and performs a bitwise   */
	/* negation of the source data. The result is  written back to the source         */
	/* location.                                                                      */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiNot_8u_C3IR(p->pSrcDst, p->srcDstStep, p->roiSize );


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}


