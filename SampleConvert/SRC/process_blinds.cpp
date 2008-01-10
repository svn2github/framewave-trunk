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
 * \function process_blinds
 *
 * \brief
 *    This function processes the opertation to add blinds effect on an image.
 *
 *
 *************************************************************************************
 */
void
process_blinds(st_API_parameters *p)
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
	Fw32f *result ;
	Fw32f *temp ;


	temp     =  (Fw32f*)fwMalloc(4*sizeof(Fw32f));
	result   =  (Fw32f*)fwMalloc(4*sizeof(Fw32f));

	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;
	p->srcStep_con          = 3 * 4 ;
	int max = p->bor_width ;

	if(p->bor_width > p->bor_height)
		max = p->bor_width ;
	else if(p->bor_height > p->bor_width)
		max = p->bor_height ;

	for(int y=0; y<p->bor_height; y++)
	{
		for(int x=0; x< p->bor_width; x++)
		{
			int     mode ;
			if (p->blinds_direction == 0) // horizontal direction
				mode = y % 25 ;
			else if (p->blinds_direction == 1) // vertical direction
				mode = x % 25 ;

			double     alpha = 255.0 * 80 / 100.0 / (25 - 1.0) ;

			temp[0] = (Fw32f)(((int)(mode * alpha)));
			if(temp[0] > 255)
			{
				temp[0] = 255;
			}
			if(temp[0] < 0)
			{
				temp[0] = 0;
			}
			temp[1] = temp[0];
			temp[2] = temp[0];

			if (temp[0] == 0xFF)
			{
				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiCopy_32f_C3R" :                                           */
				/*                                                                                */
				/* This  functions steps through an ROI in a source buffer and copies the         */
				/* data in the source data on to the destination location.                        */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiCopy_32f_C3R (p->color,  p->srcStep_con , &p->pSrc_con[0] + k,  p->srcStep_con , p->spare_roiSize );
				if (api_return_val<0)
					throw api_return_val ;

				k = k + 3;
			}

			if ((temp[0] != 0) && (temp[0] != 0xFF))
			{
				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiSub_32f_C3R" :                                            */
				/*                                                                                */
				/* This  function steps through an ROI in in two source buffers and subtracts the */
				/* data in buffer 2 to the data in buffer 1. The sum is written back to the       */
				/* source location											                      */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiSub_32f_C3R  ( &p->pSrc_con[0] + k , p->srcStep_con , p->color , p->srcStep_con , result , p->srcStep_con , p->spare_roiSize );
				if (api_return_val<0)
					throw api_return_val ;

				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiMul_32f_C3IR" :                                           */
				/*                                                                                */
				/* This  function steps through an ROI in in two source buffers and multiplies the*/
				/* data in buffer 2 to the data in buffer 1. The sum is written back to the       */
				/* source location											                      */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiMul_32f_C3IR ( temp , p->srcStep_con ,result ,p->srcStep_con , p->spare_roiSize  );
				if (api_return_val<0)
					throw api_return_val ;

				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiDivC_32f_C3IR" :                                          */
				/*                                                                                */
				/* This  function steps through an ROI in in two source buffers and divides the   */
				/* data in buffer 2 by the value. The sum is written back to the                  */
				/* source location											                      */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				p->value_con[2] = 255 ;
				p->value_con[1] = 255 ;
				p->value_con[0] = 255 ;

				api_return_val = fwiDivC_32f_C3IR ( p->value_con , result , p->srcStep_con , p->spare_roiSize );
				if (api_return_val<0)
					throw api_return_val ;

				/*--------------------------------------------------------------------------------*/
				/* Calling API "fwiAdd_32f_C3IR" :                                           */
				/*                                                                                */
				/* This  function steps through an ROI in in two source buffers and adds the      */
				/* data in buffer 2 to the data in buffer 1. The sum is written back to the       */
				/* source location											                      */
				/*                                                                                */
				/*--------------------------------------------------------------------------------*/

				api_return_val = fwiAdd_32f_C3IR ( result , p->srcStep_con, &p->pSrc_con[0] + k, p->srcStep_con , p->spare_roiSize );
				if (api_return_val<0)
					throw api_return_val ;

				k = k + 3;

			}

			else if(temp[0] == 0)
			{
				k = k + 3;
			}

		}
	}

	fwFree(temp );
	fwFree(result );

	TIME_FINISH(p)	/* Stop  Timer */

}








