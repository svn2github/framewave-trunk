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
 * \function process_rgbtoyuv422
 *
 * \brief
 *    This function processes conversion from RGB to yuv422
 *
 *
 *************************************************************************************
 */

void
process_rgbtoyuv422(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiRGBToYUV422_8u_C3P3R" :                                 */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and converts data        */
	/* in RGB color space to YCbCr color space.                                       */
	/* The result is  written to the destination location                             */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiRGBToYUV422_8u_C3P3R(p->pSrcDst, p->srcDstStep, p->pDst_yuv, p->dstStep_yuv, p->roiSize ) ;


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}




