/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


#include"globals.h"
#include"process_API.h"


/*!
 *************************************************************************************
 * \function process_halftone
 *
 * \brief
 *    This function processes halftone effect operation.
 *
 *
 *************************************************************************************
 */

void
process_halftone(st_API_parameters *p)
{
	int		api_return_val ;
	Fw8u  BayerPattern[8][8] =
	{
		0 , 32, 8 , 40, 2 , 34, 10, 42,
		48, 16, 56, 24, 50, 18, 58, 26,
		12, 44, 4 , 36, 14, 46, 6 , 38,
		60, 28, 52, 20, 62, 30, 54, 22,
		3 , 35, 11, 43, 1 , 33, 9 , 41,
		51, 19, 59, 27, 49, 17, 57, 25,
		15, 47, 7 , 39, 13, 45, 5 , 37,
		63, 31, 55, 23, 61, 29, 53, 21
	};

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiDivC_8u_C3IRSfs" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in a source buffer and divides              */
	/* the source data by a constant value. The quotient is written back to           */
	/* the source location.                                                           */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiDivC_8u_C3IRSfs(p->value, p->pSrcDst, p->srcDstStep, p->roiSize, p->scaleFactor);
	if (api_return_val<0)
		throw api_return_val ;

	for(int k=0; k<(p->bor_width * p->bor_height * 3); k=k+3)
	{
		p->pSrcDst[k]   = p->pSrcDst[k] + p->pSrcDst[k+1] + p->pSrcDst[k+2] ;
		p->pSrcDst[k+1] = p->pSrcDst[k] ;
		p->pSrcDst[k+2] = p->pSrcDst[k] ;
	}
	int k=0;
	Fw8u *temp ;
	Fw8u val = 4 ;

	p->srcStep = 1;
	p->spare_roiSize.width  = 1;
	p->spare_roiSize.height = 1;

	for(int y=0; y<p->bor_height; y++)
	{
		for(int x=0; x< p->bor_width; x++)
		{
			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiDivC_8u_C3IRSfs" :                                        */
			/*                                                                                */
			/* This function steps through an ROI in a source buffer and divides              */
			/* the source data by a constant value. The quotient is written back to           */
			/* the source location.                                                           */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiDivC_8u_C1IRSfs ( val ,&p->pSrcDst[0]+k, p->srcStep , p->spare_roiSize , p->scaleFactor );
			if (api_return_val<0)
				throw api_return_val ;

			temp = &BayerPattern[y&7][x&7];

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiCompare_8u_C1R" :                                         */
			/*                                                                                */
			/* This function steps through an ROI in a source buffer and compares the         */
			/* data in buffer 1 to the data in buffer 2 using a specified compare operation.  */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiCompare_8u_C1R ( &p->pSrcDst[0]+k , p->srcStep , temp ,p->srcStep , &p->pDst[0] + k , p->srcStep , p->spare_roiSize , fwCmpGreater );
			if (api_return_val<0)
				throw api_return_val ;

			p->pDst[k+1] = p->pDst[k] ;
			p->pDst[k+2] = p->pDst[k] ;
			k=k+3;
		}
	}

	TIME_FINISH(p)	/* Stop  Timer */


}
