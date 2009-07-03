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
 * \function process_crop
 *
 * \brief
 *    This function processes crop operation.
 *
 *
 *************************************************************************************
 */

void
process_crop(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */


	/* Crop operation is perfomed by calling API's "MulC" and "Xor"              */



	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMulC_8u_C3IRSfs" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and multiply the source  */
	/* data by a specified constant value.                                            */
	/*                                                                                */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiMulC_8u_C3IRSfs(p->spare_value, p->pSrcDst, p->srcDstStep, p->spare_roiSize, p->scaleFactor );

	if (api_return_val<0)
		throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiXor_8u_C3IR" :                                            */
	/*                                                                                */
	/* This function steps through an ROI in two source buffer and and performs       */
	/* a bitwise exclusive logical OR (XOR) of the data in buffer 1 and the data      */
	/* in buffer 2.                                                                   */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiXor_8u_C3IR(p->pSrc, p->srcStep, p->pSrcDst,  p->srcDstStep, p->roiSize );


	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;

}



