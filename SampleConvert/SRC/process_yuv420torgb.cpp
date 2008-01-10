/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#include"process_API.h"


/*!
 *************************************************************************************
 * \function process_yuv420torgb
 *
 * \brief
 *    This function processes conversion from yuv420 to RGB.
 *
 *
 *************************************************************************************
 */

void
process_yuv420torgb(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiYUV420ToRGB_8u_P3C3R" :                                   */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and converts data        */
	/* in YUV color space to RGB color space.                                         */
	/* The result is  written to the destination location                             */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiYUV420ToRGB_8u_P3C3R(p->pSrc_yuv, p->srcStep_yuv, p->pDst, p->dstStep, p->roiSize ) ;



	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}





