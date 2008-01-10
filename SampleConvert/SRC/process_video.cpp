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
 * \function process_video
 *
 * \brief
 *    This function processes the opertation to have either staggered, triped,
 *    3x3 or dotted video effect on an image.
 *
 *
 *************************************************************************************
 */
void
process_video(st_API_parameters *p)
{

	int		api_return_val ;

	TIME_START		/* Start Timer */
	int   pattern_wd[] = {2, 1, 3, 5}  ;
	int   pattern_ht[] = {6, 3, 3, 15} ;

	int   video_pattern[4][15 * 5] =
	{
		{
			0, 1,
			0, 2,
			1, 2,
			1, 0,
			2, 0,
			2, 1,
		},
		{
			0,
			1,
			2,
		},
		{
			0, 1, 2,
			2, 0, 1,
			1, 2, 0,
		},
		{
			0, 1, 2, 0, 0,
			1, 1, 1, 2, 0,
			0, 1, 2, 2, 2,
			0, 0, 1, 2, 0,
			0, 1, 1, 1, 2,
			2, 0, 1, 2, 2,
			0, 0, 0, 1, 2,
			2, 0, 1, 1, 1,
			2, 2, 0, 1, 2,
			2, 0, 0, 0, 1,
			1, 2, 0, 1, 1,
			2, 2, 2, 0, 1,
			1, 2, 0, 0, 0,
			1, 1, 2, 0, 1,
			1, 2, 2, 2, 0,
		}
	};
	int wd  = pattern_wd[p->videoType] ;
    int ht  = pattern_ht[p->videoType] ;

	Fw8u val   = 2;
	int	k   	= 0 ;
	Fw8u threshold	= 0 ;

	p->spare_roiSize.width  = 1 ;
	p->spare_roiSize.height = 1 ;
	p->srcStep = 1 ;




	for(int y=0; y<p->bor_height; y++)
	{
		for(int x=0; x< p->bor_width; x++)
		{
			for (int i=0; i<3; i++)
			{
				if (video_pattern[p->videoType][wd * (y % ht) + (x % wd)] == i)
				{

					/*--------------------------------------------------------------------------------*/
					/* Calling API "fwiMulC_8u_C1IRSfs" :                                        */
					/*                                                                                */
					/* This function steps through an ROI in a source buffer and multiply the source  */
					/* data by a specified constant value.                                            */
					/*                                                                                */
					/*                                                                                */
					/*--------------------------------------------------------------------------------*/

					api_return_val = fwiMulC_8u_C1IRSfs ( val , &p->pSrcDst[0]+k , p->srcStep , p->spare_roiSize , p->scaleFactor );
					if (api_return_val<0)
						throw api_return_val ;


					/*--------------------------------------------------------------------------------*/
					/* Calling API "fwiThreshold_GT_8u_C1IR" :                                   */
					/*                                                                                */
					/* This  functions steps through an ROI in a source buffer and compares           */
					/* source data to a specified threshold value. When the source data is            */
					/* greater than the threshold value, the output data is set to the threshold      */
					/* value.											                              */
					/*                                                                                */
					/*--------------------------------------------------------------------------------*/

					threshold = 255 ;
					api_return_val = fwiThreshold_GT_8u_C1IR ( &p->pSrcDst[0]+k , p->srcStep , p->spare_roiSize , threshold );
					if (api_return_val<0)
						throw api_return_val ;

					/*--------------------------------------------------------------------------------*/
					/* Calling API "fwiThreshold_LT_8u_C1IR" :                                  */
					/*                                                                                */
					/* This  functions steps through an ROI in a source buffer and compares           */
					/* source data to a specified threshold value. When the source data is            */
					/* lesser  than the threshold value, the output data is set to the threshold      */
					/* value.											                              */
					/*                                                                                */
					/*--------------------------------------------------------------------------------*/

					threshold = 0 ;
					api_return_val = fwiThreshold_LT_8u_C1IR ( &p->pSrcDst[0]+k , p->srcStep , p->spare_roiSize , threshold );
					if (api_return_val<0)
						throw api_return_val ;
					k++;
				}
				else
				{
					k++;
				}
			}

		}
	}
	TIME_FINISH(p)	/* Stop  Timer */
}








