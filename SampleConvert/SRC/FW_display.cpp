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
 *******************************************************************************************
 * \function FW_display
 *
 * \brief
 *    This function processes the image to be display when an ROI smaller than
 *    the original image is used. By using this function, the original image is retained
 *    and the changes made on the image according to the options will appear only in the
 *    ROI region.
 *
 *	  Inputs:
 *    p->pSrc    : Contains the original image
 *	  p->pSrcDst : Contains the changes made on the ROI
 *	  p->Dst	 : Contains a black image of the dimensions that of the original image
 *
 *	  Output:
 *	  p->pSrc
 *
 ******************************************************************************************
 */

void
FW_display(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMulC_8u_C3IRSfs" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and multiply the source  */
	/* data by a specified constant value.                                            */
	/*                                                                                */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiMulC_8u_C3IRSfs(p->spare_value, p->pSrc, p->srcDstStep, p->roiSize, p->scaleFactor );

	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiAdd_8u_C3IRSfs" :                                         */
	/*                                                                                */
	/* This function step through ROIs in two source buffers and add the data       */
	/* in buffer 2 to the data in buffer 1.                                           */
	/* The result is  written  to the destination location.                           */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiAdd_8u_C3IRSfs(p->pSrcDst,p->srcStep,p->pDst,p->srcDstStep, p->roiSize, p->scaleFactor );

	if (api_return_val<0)
		throw api_return_val ;


	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiAdd_8u_C3IRSfs" :                                         */
	/*                                                                                */
	/* This function step through ROIs in two source buffers and add the data       */
	/* in buffer 2 to the data in buffer 1.                                           */
	/* The result is  written  to the destination location.                           */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiAdd_8u_C3IRSfs(p->pDst,p->srcStep,p->pSrc,p->srcDstStep, p->spare_roiSize, p->scaleFactor );

	if (api_return_val<0)
		throw api_return_val ;

	TIME_FINISH(p)	/* Stop  Timer */



}


