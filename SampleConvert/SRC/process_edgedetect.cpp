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
 * \function process_edgedetect
 *
 * \brief
 *    This function processes the edge detection opertation.
 *
 *
 *************************************************************************************
 */
void
process_edgedetect(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSub_32f_C3IR" :									          */
	/*																				  */
	/*   These function steps through ROIs in two source buffers and subtract         */
	/*   the data in buffer 1 from the data in buffer 2. The difference is            */
	/*   written back to the destination buffer.		                              */
    /*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSub_32f_C3IR(p->pSrc_edge1 , p->srcStep * 4, p->pSrc_edge2 ,p->dstStep * 4 , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSqr_32f_C3IR" :									          */
	/*																				  */
	/* These function steps through an ROI in a source buffer and calculate           */
	/* the square of the source data. Results is written back to the source location  */
	/*																				  */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSqr_32f_C3IR(p->pSrc_edge2 ,p->srcStep * 4 , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSub_32f_C3IR" :									          */
	/*																				  */
	/*   These function steps through ROIs in two source buffers and subtract         */
	/*   the data in buffer 1 from the data in buffer 2. The difference is            */
	/*   written back to the destination buffer.		                              */
    /*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSub_32f_C3IR(p->pSrc_edge1 , p->srcStep * 4, p->pSrc_edge3 ,p->dstStep * 4 , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSqr_32f_C3IR" :									          */
	/*																				  */
	/* These function steps through an ROI in a source buffer and calculate           */
	/* the square of the source data. Results is written back to the source location  */
	/*																				  */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSqr_32f_C3IR(p->pSrc_edge3 ,p->srcStep * 4, p->roiSize);

	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;


}





