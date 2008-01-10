/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef _GLOBALS_H_
#define _GLOBALS_H_


//#include<Windows.h>
#include "process_API.h"

#ifdef LINUX
	#define	stricmp strcasecmp
#endif


/*
 * Constants
 */

#define MIN(X, Y)  ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)  ((X) > (Y) ? (X) : (Y))


#pragma warning( disable : 4996 )



#define		CONT	0
#define		RETN	1
#define		WARN	2
#define		NUM_OPTIONS	200

typedef enum filetype {
	BMP,
	YUV
} en_filetypes ;




typedef enum options {
	NEGATE,
	REMOVE,
	CROP,
	ADD,
	ADDC,
	SUB,
	SUBC,
	XOR,
	OUTPUT,
	ROI,
	FLIP,
	ROTATE,
	RESIZE,
	BRIGHT,
	CONTRAST,
	GREY,
	EDGE,
	BORDER,
	BLUR,
	SHARP,
	F_MEDIAN,
	F_GAUSS,
	F_LAPLACE,
	F_LOWPASS,
	F_HIGHPASS,
	F_MEDCOLOR,
	F_MEDCROSS,
	F_MEDVERT,
	F_MEDHORIZ,
	F_SOBVERT,
	F_SOBHORIZ,
	F_MIN,
	F_MAX,
	F_ROBUP,
	F_ROBDOWN,
	F_PREWVERT,
	F_PREWHORIZ,
	F_ROW,
	F_COLUMN,
	F_RECTKERNEL,
	GAMMA,
	SATURATE,
	HUE,
	GRID,
	NOISE,
	BLINDS,
	EMBOSS,
	DEINTERLACE,
	EXPLODE,
	GLASSY,
	ILLUSION,
	TONEHALF,
	VIDEO,
	DEF_VAL = 200
} en_options ;

/*
 * Structure Definition
 */

typedef struct _input_format {

	/* BMP File Header */
	unsigned	char	bfType[2] ;		/* The characters "BM" */
	unsigned	int		bfSize ;		/* The size of the file in bytes */
	unsigned	short	bfReserved1 ;	/* Unused - must be zero */
	unsigned	short	bfReserved2 ;	/* Unused - must be zero */
	unsigned	int		bfOffBits ;		/* Offset to start of Pixel Data */

	/* BMP Image Header */

	unsigned	int		biSize	;		/* Header Size - Must be at least 40 */
	unsigned	int		biWidth	;		/* Image width in pixels */
	unsigned	int		biHeight ;		/* Image height in pixels */
	unsigned	short	biPlanes ;		/* Must be 1 */
	unsigned	short	biBitCount ;	/* Bits per pixel - 1, 2, 4, 8, 16, 24, or 32 */
	unsigned	int		biCompression;	/* Compression type (0 = uncompressed) */
	unsigned	int		biSizeImage;	/* Image Size - may be zero for uncompressed images */
	unsigned	int		biXPelsPerMeter;/* Preferred resolution in pixels per meter */
	unsigned	int		biYPelsPerMeter;/* Preferred resolution in pixels per meter */
	unsigned	int		biClrUsed;		/* Number Color Map entries that are actually used */
	unsigned	int		biClrImportant;	/* 	Number of significant colors */


	/* Image Data */

	unsigned	char	*image_data ;

	/* YUV Data */

	unsigned	char	*Y ;
	unsigned	char	*U ;
	unsigned	char	*V ;

	unsigned	char	*YUV ;
	int					format ; // 420 : 422 : 444

	/* File type */

	en_filetypes		file_type ;

} st_input_format ;



/*
 * Function Prototypes
 */
int		usage(FILE *f_logfile) ;
int		in_file_attribute(char *file_in) ;
int		read_inputfile(char *bmp_file, st_input_format *bmp_data) ;
int		write_outputfile(char *bmp_file, st_input_format *bmp_data) ;
int		write_bmpfile(FILE *fp, st_input_format *bmp_data) ;
int		set_value(Fw8u value[3], char *remove_rgb ) ;
int		clear_bmp_struct(st_input_format *bmp_info_add) ;
int		error_handler(int api_return_val) ;
int		check_command_line(int argc, char *argv[], en_options *p_opts, char	*outfile ) ;
int		process_options(int argc, char *argv[], en_options *p_opts,  char *outfile) ;

int		check_values( char *argv[],int count ) ;
int		check_roi( char *argv[],int count ) ;
int		check_string( char *argv[],int count ) ;
int		check_file_type(char *file_name, st_input_format *bmp_data) ;
int		check_removearg(char *str) ;
int		check_range(char *str, int lo, int hi ) ;
int		check_range_float(char *str, float lo, float hi ) ;
int		check_yuvformat(char *str) ;
int		isnumber(char *str, int pos_neg ) ;
int		isnumber_float(char *str, int pos_neg ) ;

int		read_bmpfile(FILE *fp, st_input_format *bmp_data) ;
#ifdef USE_BMPAPI
		int	read_bmpfile_API(char *filename, st_input_format *bmp_data) ;
		int	write_bmpfile_API(char *bmp_file, st_input_format *bmp_data) ;
		BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, long* newsize );
		BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height );
		bool SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, char* bmpfile, st_input_format *bmp_data );
		BYTE* LoadBMP ( int* width, int* height, long* size, char* bmpfile, st_input_format *bmp_data );
#endif
int		read_yuvfile(FILE *fp, st_input_format *bmp_data) ;
int		write_yuvfile(FILE *fp, st_input_format *bmp_data) ;

int		set_rotate_params(double angle, st_input_format *bmp_info, st_API_parameters *p,int rot_cnt ) ;
int		set_resize_params(double xFactor, double yFactor, st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_contrast_params(st_input_format *bmp_info, st_API_parameters *p) ;
int		set_greyscale_params(st_input_format *bmp_info, st_API_parameters *p) ;
int		set_display_params(st_input_format *bmp_info, st_API_parameters *p) ;
int		set_edgedetect_params(st_input_format *bmp_info, st_API_parameters *p) ;
int		set_blur_params(st_input_format *bmp_info, st_API_parameters *p) ;
int		set_sharp_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmedian_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fgauss_params ( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_flaplace_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_flowpass_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fhighpass_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmedcolor_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmedcross_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmedvert_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmedhoriz_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fsobvert_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fsobhoriz_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmin_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fmax_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_frobup_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_frobdown_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fprewvert_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fprewhoriz_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_frow_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_fcolumn_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_frectkernel_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_gamma_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_saturate_params( st_input_format *bmp_info, st_API_parameters *p ) ;
int		set_hue_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_grid_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_noise_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_blinds_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_emboss_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_deinterlace_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_explode_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_glassy_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_illusion_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_halftone_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_video_params( st_input_format *bmp_info, st_API_parameters *p );
int		set_filter_adjustment_params( st_input_format *bmp_info, st_API_parameters *p );

#endif


