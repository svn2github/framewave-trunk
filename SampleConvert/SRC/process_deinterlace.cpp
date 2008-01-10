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
 * \function process_deinterlace
 *
 * \brief
 *    This function processes the opertation to deinterlace an image.
 *	  Deinterlace is useful for processing images from video capture cards.
 *    When only the odd or even fields get captured, deinterlace can be used to
 *    interpolate between the existing fields to correct this.
 *
 *
 *************************************************************************************
 */
void
process_deinterlace(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */


	p->spare_roiSize.width  = p->bor_width ;
	p->spare_roiSize.height = 1 ;

	int	k = 0 ;
	Fw8u	*up ;
	Fw8u	*down ;

	up   = (unsigned char*)malloc(p->bor_width * 3) ;
	down = (unsigned char*)malloc(p->bor_width * 3) ;


	for(int j=1; j<(p->bor_height -1); j++)
	{
		if (j % 2 == p->field)
		{
			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiCopy_8u_C3R" :                                            */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and copies the         */
			/* data in the source data on to the destination location.                        */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiCopy_8u_C3R (&p->pSrcDst[(j-1)*(p->bor_width * 3)], p->srcStep , up ,  p->srcStep , p->spare_roiSize );

			if (api_return_val<0)
				throw api_return_val ;

			api_return_val = fwiCopy_8u_C3R (&p->pSrcDst[(j+1)*(p->bor_width * 3)], p->srcStep , down ,  p->srcStep , p->spare_roiSize );

			if (api_return_val<0)
				throw api_return_val ;

			k = (j * p->bor_width * 3) ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiDivC_8u_C3IRSfs" :                                        */
			/*                                                                                */
			/* This function steps through an ROI in a source buffer and divides the          */
			/* source data by a constant value. The quotient is written back to the           */
			/* source location .                                                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value[0] = 2;
			p->value[1] = 2;
			p->value[2] = 2;

			api_return_val = fwiDivC_8u_C3IRSfs (p->value , up , p->srcStep , p->spare_roiSize , p->scaleFactor );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiDivC_8u_C3IRSfs" :                                        */
			/*                                                                                */
			/* This function steps through an ROI in a source buffer and divides the          */
			/* source data by a constant value. The quotient is written back to the           */
			/* source location .                                                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val =fwiDivC_8u_C3IRSfs (p->value , down , p->srcStep , p->spare_roiSize , p->scaleFactor );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiAdd_8u_C3RSfs" :                                          */
			/*                                                                                */
			/* These functions step through ROIs in two source buffers and add the data       */
			/* in buffer 2 to the data in buffer 1.                                           */
			/* The result is  written  to the buffer 3 location.                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val =fwiAdd_8u_C3RSfs ( up , p->srcStep , down , p->srcStep , &p->pSrcDst[k] , p->srcStep , p->spare_roiSize ,p->scaleFactor );

			if (api_return_val<0)
				throw api_return_val ;
		}
	}
	free(up) ;
	free(down) ;

	TIME_FINISH(p)	/* Stop  Timer */


}









