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
 * \function process_sharp
 *
 * \brief
 *    This function processes sharpen operation.
 *
 *
 *************************************************************************************
 */

void
process_sharp(st_API_parameters *p)
{
	int		api_return_val = 0;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiFilterSharpen_8u_C3R" :                                   */
	/*                                                                                */
	/*	This function steps through an ROI in a source buffer,						  */
	/*	apply a sharpening filter to the source data, and write the result to a       */
	/*	destination buffer. The function uses a fixed 3X3 mask.                       */
	/*--------------------------------------------------------------------------------*/
#ifndef SOL
	api_return_val = fwiFilterSharpen_8u_C3R(p->pSrc , p->srcStep, &p->pDst[(p->bor_width * 3) + 3], p->dstStep, p->dstRoiSize);
#endif


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}



