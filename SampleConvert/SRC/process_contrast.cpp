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
 * \function process_contrast
 *
 * \brief
 *    This function processes the contrast opertation.
 *
 *
 *************************************************************************************
 */
void
process_contrast(st_API_parameters *p)
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

	p->value_con[2] = 255 ;
	p->value_con[1] = 255 ;
	p->value_con[0] = 255 ;

	api_return_val = fwiDivC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSubC_32f_C3IR" :                                          */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and subtract a         */
	/* specified constant value to the source data.                                   */
	/* The result is  written back to the source location                             */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	p->value_con[2] = 0.5 ;
	p->value_con[1] = 0.5 ;
	p->value_con[0] = 0.5 ;

	api_return_val = fwiSubC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );
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

	p->value_con[2] = p->adjust ;
	p->value_con[1] = p->adjust ;
	p->value_con[0] = p->adjust ;

	api_return_val = fwiMulC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );
	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiAddC_32f_C3IR" :                                          */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and adds a             */
	/* specified constant value to the source data.                                   */
	/* The result is  written back to the source location                             */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	p->value_con[2] = 0.5 ;
	p->value_con[1] = 0.5 ;
	p->value_con[0] = 0.5 ;

	api_return_val = fwiAddC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );
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

	p->value_con[2] = 255 ;
	p->value_con[1] = 255 ;
	p->value_con[0] = 255 ;

	api_return_val = fwiMulC_32f_C3IR ( p->value_con , p->pSrc_con , p->srcStep_con ,  p->roiSize );


	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;


}





