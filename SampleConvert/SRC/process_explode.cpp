/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>


#include"process_API.h"



/*!
 *************************************************************************************
 * \function process_explode
 *
 * \brief
 *    This function processes the explode opertation.
 *
 *
 *************************************************************************************
 */
void
process_explode(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */

	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;
	int k =0 ;

	Fw32f threshold ;
	Fw32f * xCopy   ;
	Fw32f * yCopy   ;
	int block = 15   ;

	xCopy = (float*)malloc(4);
	yCopy = (float*)malloc(4);

	for (int y=0 ; y < p->bor_height ; y++)
	{
		for (int x=0 ; x < p->bor_width ; x++)
		{
			xCopy[0] = (Fw32f)(x - block/2 + (rand() % block) ) ;
  			yCopy[0] = (Fw32f)(y - block/2 + (rand() % block) ) ;

			p->srcStep = 1 ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = (Fw32f)(p->bor_width - 1) ;

			api_return_val = fwiThreshold_GT_32f_C1IR ( xCopy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			threshold = 0 ;

			api_return_val = fwiThreshold_LT_32f_C1IR( xCopy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = (Fw32f)(p->bor_height - 1) ;

			api_return_val = fwiThreshold_GT_32f_C1IR ( yCopy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			threshold = 0 ;

			api_return_val = fwiThreshold_LT_32f_C1IR( yCopy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;


			p->srcStep = 3 ;
			int a = (int)((yCopy[0] * p->bor_width * 3)+ (xCopy[0]-1) * 3) ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiCopy_8u_C3R" :                                            */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and copies the         */
			/* data in the source data on to the destination location.                        */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiCopy_8u_C3R ( &p->pSrc[a], p->srcStep, &p->pSrcDst[k] , p->srcStep , p->spare_roiSize );

			if (api_return_val<0)
				throw api_return_val ;

			k = k+3;
		}
	}
	free(xCopy) ;
	free(yCopy) ;

	TIME_FINISH(p)	/* Stop  Timer */

}






