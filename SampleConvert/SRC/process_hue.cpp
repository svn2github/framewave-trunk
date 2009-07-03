/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>


#include"globals.h"
#include"process_API.h"


/*!
 *************************************************************************************
 * \function process_hue
 *
 * \brief
 *    This function processes adjust hue of an image.
 *
 *
 *************************************************************************************
 */

void
process_hue(st_API_parameters *p)
{
	int		api_return_val ;

	TIME_START		/* Start Timer */

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiRGBToHLS_8u_C3R" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in the source buffer, converts the		  */
	/* source data from the RGB color model to the HLS color space.                   */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiRGBToHLS_8u_C3R ( p->pSrcDst , p->srcStep , p->pDst ,p->dstStep , p->roiSize);
	if (api_return_val<0)
		throw api_return_val ;

	if (p->adjust != 100)
	{
		for(int i=0; i<p->bor_width*p->bor_height*3; i=i+3)
		{
			p->value_con[0] = (float)p->pDst[i];
			p->value_con[0] = fmod(p->value_con[0]+p->adjust+360.0,360.0);
			p->pDst[i] =(unsigned char) p->value_con[0] ;
		}
	}

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiHLSToRGB_8u_C3R" :                                        */
	/*                                                                                */
	/* This function steps through an ROI in the source buffer, converts the		  */
	/* source data from the HLS color model to the RGB color space.                   */
	/*--------------------------------------------------------------------------------*/
	api_return_val = fwiHLSToRGB_8u_C3R ( p->pDst , p->srcStep , p->pSrcDst ,p->dstStep , p->roiSize);

	TIME_FINISH(p)	/* Stop  Timer */

	if (api_return_val<0)
		throw api_return_val ;
}


