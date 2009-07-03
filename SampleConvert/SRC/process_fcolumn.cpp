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
 * \function process_fcolumn
 *
 * \brief
 *    This function processes Filter Column operation.
 *
 *
 *************************************************************************************
 */

void
process_fcolumn(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiFilterColumn_8u_C3R " :                                   */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer, calculates the sum     */
	/*	of products of the kernel values and the corresponding pixel values in an     */
	/*	area defined by the kernel size and anchor coordinate, then divide by the     */
	/*	divisor. The filtered data is written to a destination buffer.                */
    /*																				  */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterColumn_8u_C3R (p->pSrc + (p->bor_width * 3) , p->srcStep, &p->pDst[2*(p->bor_width * 3) + 6],
										  p->dstStep, p->dstRoiSize, p->pKernel_32s, p->kernelSize, p->xAnchor, p->divisor );
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}









