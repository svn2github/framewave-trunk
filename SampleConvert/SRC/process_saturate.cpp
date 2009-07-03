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
 * \function process_saturate
 *
 * \brief
 *    This function processes the saturation opertation.
 *
 *
 *************************************************************************************
 */
void
process_saturate(st_API_parameters *p)
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

	api_return_val = fwiConvert_8u32f_C3R(p->pSrc , p->srcStep, p->pSrc_sat1 ,p->srcStep_con , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;


	for(int i=0; i<(p->bor_width * p->bor_height * 3); i++)
	{
		p->pSrc_sat2[i] = sqrt((p->pSrc_sat1[i]   * p->pSrc_sat1[i])    * p->value_con[0] +
							   (p->pSrc_sat1[i+1] * p->pSrc_sat1[i+1 ]) * p->value_con[1] +
							   (p->pSrc_sat1[i+2] * p->pSrc_sat1[i+2 ]) * p->value_con[2]) ;
	}

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSub_32f_C3IR" :                                           */
	/*                                                                                */
	/* These functions step through ROIs in two source buffers and subtract the data  */
	/* in buffer 1 from  data in buffer 2.                                            */
	/* The result is  written  to the destination                                     */
	/* location.                                                                      */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiSub_32f_C3IR(p->pSrc_sat2,  p->srcStep_con, p->pSrc_sat1,  p->srcStep_con, p->roiSize );
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMulC_32f_C3IR" :                                          */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and multiply the source  */
	/* data by a specified constant value.                                            */
	/*                                                                                */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	p->value_con[0] = p->adjust ;
	p->value_con[1] = p->adjust ;
	p->value_con[2] = p->adjust ;

	api_return_val = fwiMulC_32f_C3IR ( p->value_con , p->pSrc_sat1 , p->srcStep_con ,  p->roiSize );
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiAdd_32f_C3IR" :                                           */
	/*                                                                                */
	/* These functions step through ROIs in two source buffers and add the data       */
	/* in buffer 2 to the data in buffer 1.                                           */
	/* The result is  written  to the destination location.                           */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiAdd_32f_C3IR(p->pSrc_sat2,  p->srcStep_con, p->pSrc_sat1,  p->srcStep_con, p->roiSize );

	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;


}





