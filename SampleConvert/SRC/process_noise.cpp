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
 * \function process_noise
 *
 * \brief
 *    This function processes the opertation to add random noise into an image.
 *
 *
 *************************************************************************************
 */
void
process_noise(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiConvert_8u32f_C3R" :									  */
	/*																				  */
	/*   This function steps through an ROI in a source buffer, convert the		      */
	/*	 source data to 32f data type, and writes the converted data to               */
	/*   a destination buffer.			                                              */
    /*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiConvert_8u32f_C3R(p->pSrc , p->srcStep, p->pSrc_con ,p->srcStep_con , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;

	int k =0 ;

	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;
	p->srcStep_con          = 3 * 4 ;

	for(int j=0; j<p->bor_height; j++)
	{
		for(int i=0; i< p->bor_width; i++)
		{
			for(int m=0; m<3; m++)
			{
				double x = (rand()/(double)RAND_MAX - 0.5) * p->noise_level ;
				if (x > 0.0)
					 x = (int)(x + 0.5) ;
				else
				     x = (int)(x - 0.5) ;
				p->noise_delta[m] = (int)x ;
			}

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiAddC_32f_C3IR" :                                          */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and adds a             */
			/* specified constant value to the source data.                                   */
			/* The result is  written back to the source location                             */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value_con[2] = (Fw32f)p->noise_delta[0] ;
			p->value_con[1] = (Fw32f)p->noise_delta[1] ;
			p->value_con[0] = (Fw32f)p->noise_delta[2] ;

			api_return_val = fwiAddC_32f_C3IR ( p->value_con , &p->pSrc_con[0] + k , p->srcStep_con , p->spare_roiSize );
			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_32f_C3IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value_con[2] = 255 ;
			p->value_con[1] = 255 ;
			p->value_con[0] = 255 ;

			api_return_val = fwiThreshold_GT_32f_C3IR ( &p->pSrc_con[0] + k ,p->srcStep_con , p->spare_roiSize , p->value_con );
			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_32f_C3IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value_con[0] = 0 ;
			p->value_con[1] = 0 ;
			p->value_con[2] = 0 ;

			api_return_val = fwiThreshold_LT_32f_C3IR( &p->pSrc_con[0] + k ,p->srcStep_con , p->spare_roiSize , p->value_con );
			if (api_return_val<0)
				throw api_return_val ;

			k = k+3;
		}
	}

	TIME_FINISH(p)	/* Stop  Timer */

}





