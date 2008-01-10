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
 * \function process_gamma
 *
 * \brief
 *    This function processes the gamma correction opertation.
 *
 *
 *************************************************************************************
 */
void
process_gamma(st_API_parameters *p)
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


	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiDivC_32f_C3IR" :									      */
	/*																				  */
	/*   This function steps through an ROI in a source buffer and divides            */
	/*   the source data by a constant value. The quotient is written back to         */
	/*   the source location.                                                         */
    /*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiDivC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );
	if (api_return_val<0)
		throw api_return_val ;



	for(int i=0; i<(p->bor_width * p->bor_height * 3); i++)
	{
		p->pSrc_con[i] = pow( (float)p->pSrc_con[i], p->adjust );
	}


	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMulC_32f_C3IR" :                                          */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and multiply the source  */
	/* data by a specified constant value.                                            */
	/*                                                                                */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiMulC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );


	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;


}





