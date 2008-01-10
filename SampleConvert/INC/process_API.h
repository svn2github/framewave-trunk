/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _PROCESS_API_H_
#define _PROCESS_API_H_

#ifdef WINDOWS
	#include<Windows.h>
#endif

#ifdef LINUX
	#include<time.h>
	#include<sys/time.h>
#endif

#include"fwImage.h"

#ifdef WINDOWS
#define TIME_START { Timer t ; t.Start();
#define TIME_FINISH(x)	 (x)->time_taken = t.Sample();  }
#endif

#ifdef LINUX
	#define TIME_START { struct timeval start_time, end_time ; gettimeofday(&start_time, (struct timezone*)0 ) ;
	#define TIME_FINISH(x)	 gettimeofday(&end_time, (struct timezone*)0) ; (x)->time_taken =  ( (end_time.tv_sec-start_time.tv_sec)*1000000 + (end_time.tv_usec-start_time.tv_usec) ) / 1000000.0 ;    }
#endif

typedef struct _api_params {

	Fw8u		*pSrcDst ;
	int			srcDstStep ;
	FwiSize	roiSize ;
	FwiSize	spare_roiSize ;
	int			scaleFactor ;
	Fw8u		value[3] ;
	Fw8u		spare_value[3] ;

	Fw8u		*pSrc ;
	int			srcStep ;

	Fw8u		*pDst ;
	int			dstStep ;
/* YUV */
	Fw8u		*pSrc_yuv[3] ;
	int			srcStep_yuv[3] ;
	Fw8u		*pDst_yuv[3] ;
	int			dstStep_yuv[3] ;
/* Mirror */
	FwiAxis	flip ;
/* Rotate */
	double		angle ;
	FwiRect	srcRoi  ;
	FwiRect	dstRoi  ;
	double		xShift  ;
	double		yShift  ;
	int			interpolation ;

/* Resize */
	double		xFactor ;
	double		yFactor ;

/* Brightness & contrast */
	float			adjust ;

/* Contrast */
	Fw32f		*pSrc_con ;
	Fw32f		value_con[3] ;
	int			srcStep_con ;

/* Edge Detection*/
	Fw32f		*pSrc_edge1 ;
	Fw32f		*pSrc_edge2 ;
	Fw32f		*pSrc_edge3 ;

/* Border */
	int			bor_width ;
	int			bor_height ;
	int			bor_roi_wd ;
	int			bor_roi_ht ;
/* Blur */
	FwiSize	maskSize ;
	FwiPoint	anchor ;
	FwiSize	dstRoiSize ;
/* filter*/
	FwiMaskSize maskSize_filter ;
	Fw8u		 *pBuffer ;
	Fw32s	     *pKernel_32s ;
	int		     kernelSize ;
	int		     xAnchor ;
	int			 divisor ;
	FwiSize     kernelSize_f ;
	Fw8u		*temp_pSrcDst ;
/* saturate */
	Fw32f		 *pSrc_sat1 ;
	Fw32f		 *pSrc_sat2 ;
/* grid */
	int			 grid_size ;
	int			 grid_depth ;
	int			 grid_delta ;
/* noise */
	int			 noise_level ;
	int			 noise_delta[3] ;
/* blinds */
	int			 blinds_direction;
	Fw32f		 *color ;
/* emboss */
	Fw8u		 *grey ;
/* deinterlace */
	int			 field ;
/* glassy */
	int			 x_len ;
	int			 y_len ;
/* video */
	int			 videoType ;

	double		time_taken ;
}st_API_parameters ;


#ifdef WINDOWS
struct Timer
{
	LARGE_INTEGER start, stop, freq;
public:
	Timer(){ QueryPerformanceFrequency( &freq ); }
	void   Start  (){  QueryPerformanceCounter(&start); }
	double Sample()
	{
		QueryPerformanceCounter  ( &stop );
		double time = (double)(stop.QuadPart-start.QuadPart) / freq.QuadPart;
		return time;
	}
};

#endif

//* Header file for process_API.c */

void	process_negate		(st_API_parameters *p) ;
void	process_remove		(st_API_parameters *p) ;
void	process_crop		(st_API_parameters *p) ;
void	process_add			(st_API_parameters *p) ;
void	process_addC		(st_API_parameters *p) ;
void	process_sub			(st_API_parameters *p) ;
void	process_subC		(st_API_parameters *p) ;
void	process_xor			(st_API_parameters *p) ;
void	process_mirror		(st_API_parameters *p) ;
void	process_rotate		(st_API_parameters *p) ;
void	process_resize		(st_API_parameters *p) ;
void	process_brightness	(st_API_parameters *p) ;
void	process_contrast	(st_API_parameters *p) ;
void	process_grey		(st_API_parameters *p) ;
void	process_edgedetect  (st_API_parameters *p) ;
void	process_border		(st_API_parameters *p) ;
void    process_blur		(st_API_parameters *p) ;
void	process_sharp		(st_API_parameters *p) ;
void	process_gamma		(st_API_parameters *p) ;
void	process_saturate	(st_API_parameters *p) ;
void	process_hue			(st_API_parameters *p) ;

void	process_grid		(st_API_parameters *p) ;
void	process_noise		(st_API_parameters *p) ;
void	process_blinds		(st_API_parameters *p) ;
void	process_emboss		(st_API_parameters *p) ;
void	process_deinterlace (st_API_parameters *p) ;
void	process_explode		(st_API_parameters *p) ;
void	process_glassy		(st_API_parameters *p) ;
void	process_illusion	(st_API_parameters *p) ;
void	process_halftone	(st_API_parameters *p) ;
void	process_video		(st_API_parameters *p) ;

void	FW_display			(st_API_parameters *p) ;
void	FW_filter_adjustments(st_API_parameters *p) ;

void	process_fmedian		(st_API_parameters *p) ;
void	process_fgauss		(st_API_parameters *p) ;
void	process_flaplace	(st_API_parameters *p) ;
void	process_flowpass	(st_API_parameters *p) ;
void	process_fhighpass	(st_API_parameters *p) ;
void	process_fmedcolor	(st_API_parameters *p) ;
void	process_fmedcross	(st_API_parameters *p) ;
void	process_fmedvert	(st_API_parameters *p) ;
void	process_fmedhoriz	(st_API_parameters *p) ;
void	process_fsobvert	(st_API_parameters *p) ;
void	process_fsobhoriz	(st_API_parameters *p) ;
void	process_fmin		(st_API_parameters *p) ;
void	process_fmax		(st_API_parameters *p) ;
void	process_frobup		(st_API_parameters *p) ;
void	process_frobdown	(st_API_parameters *p) ;
void	process_fprewvert	(st_API_parameters *p) ;
void	process_fprewhoriz	(st_API_parameters *p) ;
void	process_frow		(st_API_parameters *p) ;
void	process_fcolumn		(st_API_parameters *p) ;
void	process_frectkernel	(st_API_parameters *p) ;

void	process_ycbcr420torgb(st_API_parameters *p) ;
void	process_ycbcr422torgb(st_API_parameters *p) ;
void	process_ycbcrtorgb	 (st_API_parameters *p) ;
void	process_yuv420torgb  (st_API_parameters *p) ;


void	process_rgbtoycbcr420(st_API_parameters *p) ;
void	process_rgbtoycbcr422(st_API_parameters *p) ;
void	process_rgbtoyuv422(st_API_parameters *p) ;
void	process_rgbtoyuv420  (st_API_parameters *p) ;
void	process_rgbtoyuv     (st_API_parameters *p) ;




#endif
