/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
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
 * \function process_emboss
 *
 * \brief
 *    This function processes the opertation to emboss an image.
 *
 *
 *************************************************************************************
 */
void
process_emboss(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */


	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;

	int	k					= 0 ;
	int	sum_present			= 0 ;
	int	sum_next			= 0 ;
	Fw8u	threshold		= 0 ;


	for(int y=1; y<p->bor_height; y++)
	{
		for(int x=0; x<= (p->bor_width) - 1; x++)
		{

			if (x == (p->bor_width ) - 1)
			{
				p->srcStep = 3 ;

				p->grey[0] = 128 ;
				p->grey[1] = 128 ;
				p->grey[2] = 128 ;

				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiCopy_8u_C3R" :                                           */
				/*                                                                                */
				/* This  functions steps through an ROI in a source buffer and copies the         */
				/* data in the source data on to the destination location.                        */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiCopy_8u_C3R (p->grey, p->srcStep , &p->pDst[0] + k ,  p->srcStep , p->spare_roiSize );

				if (api_return_val<0)
					throw api_return_val ;

				k = k + 3;

			}

			else
			{
				p->srcStep = 1 ;

				sum_present = p->pSrcDst[k]   + p->pSrcDst[k+1] + p->pSrcDst[k+2] ;

				sum_next    = p->pSrcDst[k+3] + p->pSrcDst[k+4] + p->pSrcDst[k+5] ;

				p->grey[0]  = 2 * (sum_present - sum_next) / 3 + 128 ;


				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiThreshold_GT_8u_C1IR" :                                   */
				/*                                                                                */
				/* This  functions steps through an ROI in a source buffer and compares           */
				/* source data to a specified threshold value. When the source data is            */
				/* greater than the threshold value, the output data is set to the threshold      */
				/* value.											                              */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				threshold = 255 ;
				api_return_val = fwiThreshold_GT_8u_C1IR ( p->grey , p->srcStep , p->spare_roiSize , threshold );

				if (api_return_val<0)
					throw api_return_val ;
				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiThreshold_LT_8u_C1IR" :                                  */
				/*                                                                                */
				/* This  functions steps through an ROI in a source buffer and compares           */
				/* source data to a specified threshold value. When the source data is            */
				/* lesser  than the threshold value, the output data is set to the threshold      */
				/* value.											                              */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				threshold = 0 ;
				api_return_val = fwiThreshold_LT_8u_C1IR ( p->grey , p->srcStep , p->spare_roiSize , threshold );

				if (api_return_val<0)
					throw api_return_val ;

				p->grey[1] = p->grey[0] ;
				p->grey[2] = p->grey[0] ;

				p->srcStep = 3 ;

				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiCopy_8u_C3R" :                                           */
				/*                                                                                */
				/* This  functions steps through an ROI in a source buffer and copies the         */
				/* data in the source data on to the destination location.                        */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiCopy_8u_C3R (p->grey, p->srcStep , &p->pDst[0] + k , p->srcStep , p->spare_roiSize );

				if (api_return_val<0)
					throw api_return_val ;

				k = k + 3;
			}

		}
	}
	p->srcStep              = p->bor_width *3;
	p->spare_roiSize.width  = p->bor_width ;
	p->spare_roiSize.height = p->bor_height ;
	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiCopy_8u_C3R" :                                           */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and copies the         */
	/* data in the source data on to the destination location.                        */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiCopy_8u_C3R (p->pDst, p->srcStep , p->pSrcDst , p->srcStep , p->spare_roiSize );

	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;

}








