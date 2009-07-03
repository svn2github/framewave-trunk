/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
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
 * \function process_illusion
 *
 * \brief
 *    This function processes the illusion effect opertation.
 *
 *
 *************************************************************************************
 */
void
process_illusion(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */

	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;
	p->srcStep				= 1 ;
	int k =0 ;

	Fw32f *sqrwidth ;
	Fw32f *sqrheight ;
	Fw32f val = 2 ;
	Fw32f threshold ;

	sqrwidth     = (float*)malloc(4);
	sqrheight    = (float*)malloc(4);

	sqrwidth[0]  = (Fw32f)p->bor_width ;
	sqrheight[0] = (Fw32f)p->bor_height ;


	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSqr_32f_C1IR" :                                           */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and calculates         */
	/* the square of the source data. Result is written back to the source location   */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSqr_32f_C1IR ( sqrwidth  , p->srcStep, p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	api_return_val = fwiSqr_32f_C1IR ( sqrheight , p->srcStep, p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiAdd_32f_C1IR" :                                           */
	/*                                                                                */
	/* This  functions steps through an ROI in two source buffers and add the         */
	/* data in buffer 2 to the data in buffer 1. Result is written back to the        */
	/* destination location															  */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiAdd_32f_C1IR ( sqrwidth  , p->srcStep , sqrheight , p->srcStep ,p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiSqrt_32f_C1IR" :                                          */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and calculates         */
	/* the square root of the source data. Result is written back to the source       */
	/* location																		  */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiSqrt_32f_C1IR( sqrheight , p->srcStep , p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiDivC_32f_C1IR" :                                          */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and divides the        */
	/* source data by a constant value. The quotient is written back to the source    */
	/* location																		  */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiDivC_32f_C1IR( val , sqrheight, p->srcStep, p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	float fscale = sqrheight[0] ;

	/*--------------------------------------------------------------------------------*/
	/* Calling API "fwiDivC_32f_C1IR" :                                          */
	/*                                                                                */
	/* This  functions steps through an ROI in a source buffer and divides the        */
	/* source data by a constant value. The quotient is written back to the source    */
	/* location																		  */
	/*                                                                                */
	/*--------------------------------------------------------------------------------*/

	api_return_val = fwiDivC_32f_C1IR( val , sqrheight, p->srcStep, p->spare_roiSize );
	if (api_return_val<0)
			throw api_return_val ;

	int foffset = (int)sqrheight[0] ;

	double xcen, ycen, tmp;
	double angle, radius;

	Fw32f *cx,*cy;

	xcen = p->bor_width  / 2.0 ;
	ycen = p->bor_height / 2.0 ;

	Fw32f *xx, *yy ;

	xx = (float*)malloc(4);
	yy = (float*)malloc(4);
	cx = (float*)malloc(4);
	cy = (float*)malloc(4);



	for (int j=0 ; j < p->bor_height ; j++)
	{
		for (int i=0 ; i < p->bor_width ; i++)
		{
			p->srcStep = 1 ;
			cx[0] = (i - xcen) / fscale ;
			cy[0] = (j - ycen) / fscale ;
			tmp   = 3.14159265 / (double)p->field ;

			angle = floor (atan2(cy[0],cx[0]) / 2.0 / tmp) * 2.0 * tmp + tmp ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiSqr_32f_C1IR" :                                           */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and calculates         */
			/* the square of the source data. Result is written back to the source location   */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiSqr_32f_C1IR ( cx , p->srcStep, p->spare_roiSize );
			if (api_return_val<0)
					throw api_return_val ;

			api_return_val = fwiSqr_32f_C1IR ( cy , p->srcStep, p->spare_roiSize );
			if (api_return_val<0)
					throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiAdd_32f_C1IR" :                                           */
			/*                                                                                */
			/* This  functions steps through an ROI in two source buffers and add the         */
			/* data in buffer 2 to the data in buffer 1. Result is written back to the        */
			/* destination location															  */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiAdd_32f_C1IR ( cx  , p->srcStep , cy , p->srcStep ,p->spare_roiSize );
			if (api_return_val<0)
					throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiSqrt_32f_C1IR" :                                          */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and calculates         */
			/* the square root of the source data. Result is written back to the source       */
			/* location																		  */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			api_return_val = fwiSqrt_32f_C1IR( cy , p->srcStep , p->spare_roiSize );
			if (api_return_val<0)
					throw api_return_val ;

			radius = cy[0] ;
			xx[0]  = (Fw32f)((int)(i - foffset * cos (angle)) );
			yy[0]  = (Fw32f)((int)(j - foffset * sin (angle)) );


			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = (Fw32f)(p->bor_width - 1) ;

			api_return_val = fwiThreshold_GT_32f_C1IR ( xx , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = 0;
			api_return_val = fwiThreshold_LT_32f_C1IR ( xx , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = (Fw32f)(p->bor_height - 1) ;

			api_return_val = fwiThreshold_GT_32f_C1IR ( yy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_32f_C1IR" :                                  */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/

			threshold = 0;
			api_return_val = fwiThreshold_LT_32f_C1IR ( yy , p->srcStep, p->spare_roiSize , threshold );

			if (api_return_val<0)
				throw api_return_val ;

			int a = (int)((yy[0] * p->bor_width * 3 ) + (xx[0] - 1) * 3) ;

			p->pSrcDst[k]   = p->pSrcDst[k]   + (int)(radius * (p->pSrc[a]   - p->pSrcDst[k]));
			p->pSrcDst[k+1] = p->pSrcDst[k+1] + (int)(radius * (p->pSrc[a+1] - p->pSrcDst[k+1]));
			p->pSrcDst[k+2] = p->pSrcDst[k+2] + (int)(radius * (p->pSrc[a+2] - p->pSrcDst[k+2]));

			p->srcStep = 3 ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_GT_8u_C3IR" :                                   */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* greater than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value[2] = 255 ;
			p->value[1] = 255 ;
			p->value[0] = 255 ;

			api_return_val = fwiThreshold_GT_8u_C3IR ( &p->pSrcDst[0] + k ,p->srcStep , p->spare_roiSize , p->value );
			if (api_return_val<0)
				throw api_return_val ;

			/*--------------------------------------------------------------------------------*/
			/* Calling API "fwiThreshold_LT_8u_C3IR" :                                   */
			/*                                                                                */
			/* This  functions steps through an ROI in a source buffer and compares           */
			/* source data to a specified threshold value. When the source data is            */
			/* lesser  than the threshold value, the output data is set to the threshold      */
			/* value.											                              */
			/*                                                                                */
			/*--------------------------------------------------------------------------------*/
			p->value[0] = 0 ;
			p->value[1] = 0 ;
			p->value[2] = 0 ;

			api_return_val = fwiThreshold_LT_8u_C3IR( &p->pSrcDst[0] + k ,p->srcStep , p->spare_roiSize , p->value );
			if (api_return_val<0)
				throw api_return_val ;

			k = k + 3 ;
		}
	}
	free(sqrwidth) ;
	free(sqrheight) ;
	free(xx) ;
	free(yy) ;
	free(cx) ;
	free(cy) ;

	TIME_FINISH(p)	/* Stop  Timer */

}






