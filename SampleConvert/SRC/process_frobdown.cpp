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
 * \function process_frobdown
 *
 * \brief
 *    This function processes Roberts Down Filter operation.
 *
 *
 *************************************************************************************
 */

void
process_frobdown(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/*  Calling API "fwiFilterRobertsDown_8u_C3R" :                              */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer, applies a horizontal   */
	/*	Roberts operator to the source data, and write the filtered data to a         */
	/*	destination buffer. This function provides a gross approximation of the       */
	/*	horizontal  pixel gradient													  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterRobertsDown_8u_C3R(p->pSrc , p->srcStep, &p->pDst[(p->bor_width * 3) + 3] , p->dstStep, p->dstRoiSize);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}






