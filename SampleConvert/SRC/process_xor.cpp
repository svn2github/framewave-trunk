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
 * \function process_xor
 *
 * \brief
 *    This function processes xor operation.
 *
 *
 *************************************************************************************
 */


void
process_xor(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */


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


