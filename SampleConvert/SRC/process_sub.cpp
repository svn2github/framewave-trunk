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
 * \function process_sub
 *
 * \brief
 *    This function processes sub operation.
 *
 *
 *************************************************************************************
 */
void
process_sub(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSub_8u_C3IRSfs" :                                         */
	/*                                                                                */
	/* These functions step through ROIs in two source buffers and subtract the data  */
	/* in buffer 1 from  data in buffer 2.                                            */
	/* The result is  written  to the destination                                     */
	/* location.                                                                      */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiSub_8u_C3IRSfs(p->pSrc,p->srcStep,p->pSrcDst, p->srcDstStep, p->roiSize, p->scaleFactor );


	TIME_FINISH(p)	/* Stop  Timer */
	if (api_return_val<0)
		throw api_return_val ;

}




