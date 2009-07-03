/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#include"process_API.h"



/*!
 *************************************************************************************
 * \function process_border
 *
 * \brief
 *    This function processes border operation.
 *
 *
 *************************************************************************************
 */

#include "fwBase.h"

void
process_border(st_API_parameters *p)
{
	int		api_return_val ;

    TIME_START		/* Start Timer */

    const Fw8u *value = p->value;
    unsigned char* dst = p->pSrcDst;
    int dstStep = p->srcDstStep;
    FwiSize roiSize;
    roiSize.width = p->roiSize.width;
    roiSize.height = 20;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSet_8u_C3R" :                                        	  */
	/*                                                                                */
	/* This  functions sets the value of first twenty rows (with user given value) in */
	/* the destination buffer to create border at the top of the image				  */
	/* 		                                                                          */
	/*--------------------------------------------------------------------------------*/

    api_return_val = fwiSet_8u_C3R(value, dst, dstStep, roiSize);
    if (api_return_val<0)
		throw api_return_val ;

    dst = p->pSrcDst + p->roiSize.width * (p->roiSize.height-20) * 3;

    /*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSet_8u_C3R" :                                        	  */
	/*                                                                                */
	/* This  functions sets the value of last twenty rows (with user given value) in  */
	/* the destination buffer to create border at the bottom of the image			  */
	/* 		                                                                          */
	/*--------------------------------------------------------------------------------*/

    api_return_val = fwiSet_8u_C3R(value, dst, dstStep, roiSize);
    if (api_return_val<0)
		throw api_return_val ;

    dst = p->pSrcDst;
    roiSize.width = 20;
    roiSize.height = p->roiSize.height;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSet_8u_C3R" :                                        	  */
	/*                                                                                */
	/* This  functions sets the value of first twenty columns (with user given value) */
	/* in the destination buffer to create border at the left side of the image		  */
	/* 		                                                                          */
	/*--------------------------------------------------------------------------------*/

    api_return_val = fwiSet_8u_C3R(value, dst, dstStep, roiSize);
    if (api_return_val<0)
		throw api_return_val;

    dst = p->pSrcDst + (p->roiSize.width-20) * 3;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSet_8u_C3R" :                                        	  */
	/*                                                                                */
	/* This  functions sets the value of last twenty columns (with user given value)  */
	/* in the destination buffer to create border at the right side  of the image	  */
	/* 		                                                                          */
	/*--------------------------------------------------------------------------------*/

    api_return_val = fwiSet_8u_C3R(value, dst, dstStep, roiSize);
    if (api_return_val<0)
		throw api_return_val;

	TIME_FINISH(p)	/* Stop  Timer */
}





