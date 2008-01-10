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
 * \function process_remove
 *
 * \brief
 *    This function processes removing a color component from the RGB array
 *
 *
 *************************************************************************************
 */

void
process_remove(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiMulC_8u_C3IRSfs" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and multiply the source  */
	/* data by a specified constant value.                                            */
	/*                                                                                */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/


	api_return_val = fwiMulC_8u_C3IRSfs( p->value, p->pSrcDst, p->srcDstStep, p->roiSize, p->scaleFactor );


	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;

}


