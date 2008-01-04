/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#include "fwdev.h"
//#include "algorithm.h"
#include "FwSharedCode_SSE2.h"
#include "system.h"
#include "fwVideo.h"

using namespace OPT_LEVEL;

#if BUILD_NUM_AT_LEAST(101)

#define  IClip( Min, Max, Val) (((Val)<(Min))? (Min):(((Val)>(Max))? (Max):(Val)))

static inline int My_clip3( int v, int i_min, int i_max )
{
    return ( (v < i_min) ? i_min : (v > i_max) ? i_max : v );
}

static inline Fw8u clip_uint8( int a )
{
    if (a&(~255))
        return (Fw8u)((-a)>>31);
    else
        return (Fw8u)a;
}

extern SYS_FORCEALIGN_16 const Fw8u pb_01[16];
extern SYS_FORCEALIGN_16 const Fw8u pb_03[16];
extern SYS_FORCEALIGN_16 const Fw8u pb_a1[16];

#define DIFF_GT_SSE2(m2, m3, m4, m5, m6)	\
{	\
	m6 = m3;	\
	m5 = m2;	\
	m6 = _mm_subs_epu8(m6, m2); \
	m5 = _mm_subs_epu8(m5, m3); \
	m5 = _mm_or_si128 (m5, m6); \
	m5 = _mm_subs_epu8(m5, m4); \
}

#define LOAD_MASK_SSE2(edx, ecx)	\
{\
	xmm4=_mm_set1_epi8 ((Fw8u)edx);\
	xmm5=_mm_set1_epi8 ((Fw8u)ecx);\
	DIFF_GT_SSE2( xmm1, xmm2, xmm4, xmm7, xmm6) ;	\
	DIFF_GT_SSE2(xmm0, xmm1, xmm5, xmm4, xmm6) ;	\
	xmm7 = _mm_or_si128 (xmm7, xmm4);	\
	DIFF_GT_SSE2(xmm3, xmm2, xmm5, xmm4, xmm6) ;	\
	xmm7 = _mm_or_si128 (xmm7, xmm4);	\
	xmm6 = _mm_xor_si128 (xmm6, xmm6);	\
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm6);	\
}

#define DIFF_GT2_SSE2(m2, m3, m4, m5, m6)	\
  {\
	m6 = m3;	\
    m5 = m2;	\
	m6 = _mm_subs_epu8(m6, m2);	\
	m5 = _mm_subs_epu8(m5, m3);	\
	m6 = _mm_subs_epu8(m6, m4);	\
	m5 = _mm_subs_epu8(m5, m4);	\
	m5 = _mm_cmpeq_epi8(m5, m6);	\
}

#define LUMA_Q1_SSE2(m1, m2, r3, r4, m5, m6)	\
{	\
    m6 = xmm1;			\
    m6 = _mm_avg_epu8 (m6, xmm2);	\
	m2 = _mm_avg_epu8 (m2, m6);	\
	m6 = _mm_or_si128 (m6, _mm_loadu_si128((__m128i*)(r3))  );	\
	m6 = _mm_and_si128(m6, _mm_loadu_si128((__m128i*)pb_01) );	\
	m2 = _mm_subs_epu8(m2, m6);	\
    m6 = m1;	\
	m6 = _mm_subs_epu8(m6, m5);	\
	m5 = _mm_adds_epu8(m5, m1);	\
	m2 = _mm_max_epu8(m2, m6);	\
	m2 = _mm_min_epu8(m2, m5);	\
	_mm_storeu_si128( (__m128i*)r4, m2 );	\
}

#define DEBLOCK_P0_Q0_SSE2()	\
{\
    xmm5 = xmm1;	\
	xmm5 = _mm_xor_si128 (xmm5, xmm2);	\
	xmm5 = _mm_and_si128 (xmm5, _mm_loadu_si128((__m128i*)pb_01));	\
	xmm4 = _mm_cmpeq_epi8 (xmm4, xmm4);	\
	xmm3 = _mm_xor_si128 (xmm3, xmm4);	\
	xmm3 = _mm_avg_epu8 (xmm3, xmm0);	\
	xmm3 = _mm_avg_epu8 (xmm3, _mm_loadu_si128((__m128i*)pb_03));	\
	xmm4 = _mm_xor_si128 (xmm4, xmm1);	\
	xmm4 = _mm_avg_epu8 (xmm4, xmm2);	\
	xmm3 = _mm_avg_epu8 (xmm3, xmm5);	\
	xmm3 = _mm_adds_epu8 (xmm3, xmm4);	\
	xmm6 = _mm_loadu_si128( (__m128i*)pb_a1 );	\
	xmm6 = _mm_subs_epu8(xmm6, xmm3);	\
	xmm3 = _mm_subs_epu8(xmm3, _mm_loadu_si128((__m128i*)pb_a1) );	\
	xmm6 = _mm_min_epu8(xmm6, xmm7);	\
	xmm3 = _mm_min_epu8(xmm3, xmm7);	\
	xmm1 = _mm_subs_epu8(xmm1, xmm6);	\
	xmm2 = _mm_subs_epu8(xmm2, xmm3);	\
	xmm1 = _mm_adds_epu8 (xmm1, xmm3);	\
	xmm2 = _mm_adds_epu8 (xmm2, xmm6);\
}
static inline void deblock_luma_c( Fw8u *pix, Fw32s xstride, Fw32s ystride, 
								  Fw32s alpha, Fw32s beta, Fw8s *tc0 );
void static My_deblock_v_luma_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
	if(tc0[0]<0 || tc0[1]<0 || tc0[2]<0 || tc0[3]<0)
	{
		deblock_luma_c( pix, stride, 1, alpha, beta, tc0);
		return;
	}
	else
	{
		Fw8u *rdi, *r8;
		Fw32s *a;
		Fw32s rsi, edx, ecx;
		__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9;

	//	Fw8u* tempa = (Fw8u*) fwMalloc(16*sizeof(Fw8u));

		rdi = pix;
		rsi = stride;
		edx = alpha-1;
		ecx = beta-1;
	//    movd   xmm8, [r8] ; tc0
		a    = (int *)tc0;
		xmm8 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src

		r8  = rdi-3*stride ; // pix-3*stride
	//    movdqa  xmm0, [r8+rsi]    ; p1
		xmm0 = _mm_loadu_si128( (__m128i*)(r8+rsi) );	//p1
	//    movdqa  xmm1, [r8+2*rsi]  ; p0
		xmm1 = _mm_loadu_si128( (__m128i*)(r8+rsi*2) );	//p0
	//    movdqa  xmm2, [rdi]       ; q0
		xmm2 = _mm_loadu_si128( (__m128i*)(rdi) );		//q0
	//    movdqa  xmm3, [rdi+rsi]   ; q1
		xmm3 = _mm_loadu_si128( (__m128i*)(rdi+rsi) );	//q1
		LOAD_MASK_SSE2(  edx, ecx);

	//    punpcklbw xmm8, xmm8
		xmm8 = _mm_unpacklo_epi8(xmm8, xmm8);
	//    punpcklbw xmm8, xmm8 ; xmm8 = 4x tc0[3], 4x tc0[2], 4x tc0[1], 4x tc0[0]
		xmm8 = _mm_unpacklo_epi8(xmm8, xmm8);
	//    pcmpeqb xmm9, xmm9
		xmm9  = _mm_set1_epi8(0);    // zero;
		xmm9 = _mm_cmpeq_epi8 (xmm9, xmm9);
	//    pcmpeqb xmm9, xmm8
		xmm9 = _mm_cmpeq_epi8 (xmm9, xmm8);
	//    pandn   xmm9, xmm7
		xmm9 = _mm_andnot_si128 (xmm9, xmm7);
	//    pand    xmm8, xmm9
		xmm8 = _mm_and_si128 (xmm8, xmm9);
	/*
	{
	_mm_storeu_si128( (__m128i*)tempa, xmm8 ); // store 16 bytes
	printf("\n %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d",
		   tempa[0], tempa[1],tempa[2],tempa[3],tempa[4],tempa[5],tempa[6],tempa[7],
		   tempa[8], tempa[9],tempa[10],tempa[11],tempa[12],tempa[13],tempa[14],tempa[15]);
	getchar();
	}
	*/
	//    movdqa  xmm3, [r8] ; p2
		xmm3 = _mm_loadu_si128( (__m128i*)(r8) );	//p2

		DIFF_GT2_SSE2(xmm1,	xmm3,	xmm5,	xmm6,	xmm7);	//|p2-p0| > beta-1
		//|p2-p0| > beta-1
	//    pand    xmm6, xmm9
		xmm6 = _mm_and_si128 (xmm6, xmm9);
	//    movdqa  xmm7, xmm8
		xmm7 = xmm8;
	//    psubb   xmm7, xmm6
		xmm7 = _mm_sub_epi8 (xmm7, xmm6);
	//    pand    xmm6, xmm8
		xmm6 = _mm_and_si128 (xmm6, xmm8);
		LUMA_Q1_SSE2 (xmm0, xmm3, r8, (r8+rsi), xmm6, xmm4);

	//    movdqa  xmm4, [rdi+2*rsi] ; q2
		xmm4 = _mm_loadu_si128( (__m128i*)(rdi+2*rsi) );
		DIFF_GT2_SSE2( xmm2, xmm4, xmm5, xmm6, xmm3) ; //|q2-q0| > beta-1
	//    pand    xmm6, xmm9
		xmm6 = _mm_and_si128 (xmm6, xmm9);
	//    pand    xmm8, xmm6
		xmm8 = _mm_and_si128 (xmm8, xmm6);
	//    psubb   xmm7, xmm6
		xmm7 = _mm_sub_epi8 (xmm7, xmm6);
	//    movdqa  xmm3, [rdi+rsi]
		xmm3 = _mm_loadu_si128( (__m128i*)(rdi+rsi) );
		LUMA_Q1_SSE2( xmm3, xmm4, (rdi+2*rsi), (rdi+rsi), xmm8, xmm6);

		DEBLOCK_P0_Q0_SSE2();
	//    movdqa  [r8+2*rsi], xmm1
		_mm_storeu_si128( (__m128i*)(r8+2*rsi), xmm1 ); // store 16 bytes
	//    movdqa  [rdi], xmm2
		_mm_storeu_si128( (__m128i*)(rdi), xmm2 ); // store 16 bytes
	}
}

#define TRANSPOSE_SSE8x8(r1, l1, r2, l2) \
{\
	xmm0 = _mm_loadl_epi64( (__m128i*)(r1) );	\
	xmm1 = _mm_loadl_epi64( (__m128i*)(r1+l1) );	\
	xmm2 = _mm_loadl_epi64( (__m128i*)(r1+l1*2) );	\
	xmm3 = _mm_loadl_epi64( (__m128i*)(r1+l1*3) );	\
	xmm4 = _mm_loadl_epi64( (__m128i*)(r1+l1*4) );	\
	xmm5 = _mm_loadl_epi64( (__m128i*)(r1+l1*5) );	\
	xmm6 = _mm_loadl_epi64( (__m128i*)(r1+l1*6) );	\
	xmm7 = _mm_loadl_epi64( (__m128i*)(r1+l1*7) );	\
	xmm0 = _mm_unpacklo_epi8(xmm0, xmm1);	\
	xmm2 = _mm_unpacklo_epi8(xmm2, xmm3);	\
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm5);	\
	xmm6 = _mm_unpacklo_epi8(xmm6, xmm7);	\
	xmm1 = xmm0;	\
	xmm5 = xmm4;	\
	xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);	\
	xmm1 = _mm_unpackhi_epi16(xmm1, xmm2);	\
	xmm4 = _mm_unpacklo_epi16(xmm4, xmm6);	\
	xmm5 = _mm_unpackhi_epi16(xmm5, xmm6);	\
	xmm2 = xmm0;	\
	xmm3 = xmm1;	\
	xmm0 = _mm_unpacklo_epi32(xmm0, xmm4);	\
	xmm1 = _mm_unpacklo_epi32(xmm1, xmm5);	\
	xmm2 = _mm_unpackhi_epi32(xmm2, xmm4);	\
	xmm3 = _mm_unpackhi_epi32(xmm3, xmm5);	\
	xmm8 = xmm1;\
	_mm_storel_epi64( (__m128i*)(r2), xmm0 ); \
	xmm0 = _mm_unpackhi_epi64 (xmm0, xmm4);	\
	_mm_storel_epi64( (__m128i*)(r2+l2), xmm0 ); \
	_mm_storel_epi64( (__m128i*)(r2+l2*2), xmm2 ); \
	xmm8 = _mm_unpackhi_epi64 (xmm8, xmm6);	\
	xmm2 = _mm_unpackhi_epi64 (xmm2, xmm5);\
	_mm_storel_epi64( (__m128i*)(r2+l2*3), xmm2 ); \
	_mm_storel_epi64( (__m128i*)(r2+l2*4), xmm1 ); \
	_mm_storel_epi64( (__m128i*)(r2+l2*5), xmm8 ); \
	_mm_storel_epi64( (__m128i*)(r2+l2*6), xmm3 ); \
	xmm3 = _mm_unpackhi_epi64 (xmm3, xmm7);\
	_mm_storel_epi64( (__m128i*)(r2+l2*7), xmm3 ); \
}

/*
%macro SBUTTERFLY 4
    movq       %4, %2
    punpckl%1  %2, %3
    punpckh%1  %4, %3
%endmacro
; in: 8 rows of 8 (only the middle 6 pels are used) in %1..%8
; out: 6 rows of 8 in [%9+0*16] .. [%9+5*16]
%macro TRANSPOSE6x8_MEM 9
    movq  mm0, %1
    movq  mm1, %3
    movq  mm2, %5
    movq  mm3, %7
    SBUTTERFLY bw, mm0, %2, mm4
    SBUTTERFLY bw, mm1, %4, mm5
    SBUTTERFLY bw, mm2, %6, mm6
    movq  [%9+0x10], mm5
    SBUTTERFLY bw, mm3, %8, mm7
    SBUTTERFLY wd, mm0, mm1, mm5
    SBUTTERFLY wd, mm2, mm3, mm1
    punpckhdq mm0, mm2
    movq  [%9+0x00], mm0
    SBUTTERFLY wd, mm4, [%9+0x10], mm3
    SBUTTERFLY wd, mm6, mm7, mm2
    SBUTTERFLY dq, mm4, mm6, mm0
    SBUTTERFLY dq, mm5, mm1, mm7
    punpckldq mm3, mm2
    movq  [%9+0x10], mm5
    movq  [%9+0x20], mm7
    movq  [%9+0x30], mm4
    movq  [%9+0x40], mm0
    movq  [%9+0x50], mm3
%endmacro
*/

//-----------------------------------------------------------------------------
//   void My_deblock_h_luma_sse2( uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0 )
//-----------------------------------------------------------------------------
void  static My_deblock_h_luma_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
	Fw8u *rdi;
	SYS_FORCEALIGN_16  Fw8u pix_tmp[256];
	Fw8u *ptmp = &pix_tmp[0];
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;

	if(tc0[0]<0 || tc0[1]<0 || tc0[2]<0 || tc0[3]<0)
	{
		deblock_luma_c( pix, 1, stride, alpha, beta, tc0);
		return;
	}

    rdi = pix;
	TRANSPOSE_SSE8x8(rdi-4, stride, ptmp, 16);
	TRANSPOSE_SSE8x8(rdi-4+stride*4, stride, ptmp+8, 16);

	My_deblock_v_luma_sse2(ptmp+16*3, 16, alpha, beta, tc0);

	TRANSPOSE_SSE8x8(ptmp, 16, rdi-4, stride);
	TRANSPOSE_SSE8x8(ptmp+8, 16, rdi-4+stride*4, stride);
}

static inline void deblock_luma_c( Fw8u *pix, Fw32s xstride, Fw32s ystride, 
								  Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
    int i, d;
    for( i = 0; i < 4; i++ ) {
        if( tc0[i] < 0 ) {
            pix += 4*ystride;
            continue;
        }
        for( d = 0; d < 4; d++ ) {
            const int p2 = pix[-3*xstride];
            const int p1 = pix[-2*xstride];
            const int p0 = pix[-1*xstride];
            const int q0 = pix[ 0*xstride];
            const int q1 = pix[ 1*xstride];
            const int q2 = pix[ 2*xstride];
   
            if( abs( p0 - q0 ) < alpha &&
                abs( p1 - p0 ) < beta &&
                abs( q1 - q0 ) < beta ) {
   
                int tc = tc0[i];
                int delta;
   
                if( abs( p2 - p0 ) < beta ) {
                    pix[-2*xstride] = (Fw8u)(p1 + My_clip3( (( p2 + ((p0 + q0 + 1) >> 1)) >> 1) - p1, -tc0[i], tc0[i] ));
                    tc++; 
                }
                if( abs( q2 - q0 ) < beta ) {
                    pix[ 1*xstride] = (Fw8u)(q1 + My_clip3( (( q2 + ((p0 + q0 + 1) >> 1)) >> 1) - q1, -tc0[i], tc0[i] ));
                    tc++;
                }
    
                delta = My_clip3( (((q0 - p0 ) << 2) + (p1 - q1) + 4) >> 3, -tc, tc );
                pix[-1*xstride] = clip_uint8( p0 + delta );    /* p0' */
                pix[ 0*xstride] = clip_uint8( q0 - delta );    /* q0' */
            }
            pix += ystride;
        }
    }
}

void  static My_deblock_v_luma_intra_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta )
{
	Fw8u *rdi, *r8;
	Fw32s rsi, edx, ecx;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9;
	SYS_FORCEALIGN_16  Fw8u mask_all[16];
	SYS_FORCEALIGN_16  Fw8u mask_p0_q0[16];
	SYS_FORCEALIGN_16  Fw8u mask_p2_p0[16];
	SYS_FORCEALIGN_16  Fw8u mask_q2_q0[16];

//	Fw8u* tempa = (Fw8u*) fwMalloc(16*sizeof(Fw8u));

    rdi = pix;
    rsi = stride;
    edx = alpha-1;
    ecx = beta-1;
    r8  = rdi-3*stride ; // pix-3*stride

	xmm0 = _mm_loadu_si128( (__m128i*)(r8+rsi) );	// xmm0=p1
	xmm1 = _mm_loadu_si128( (__m128i*)(r8+rsi*2) );	// xmm1=p0
	xmm2 = _mm_loadu_si128( (__m128i*)(rdi) );		// xmm2=q0
	xmm3 = _mm_loadu_si128( (__m128i*)(rdi+rsi) );	// xmm3=q1


//    LOAD_MASK_SSE2(  edx, ecx);
	xmm4=_mm_set1_epi8 ((Fw8u)edx);	// m4=alpha-1
	xmm5=_mm_set1_epi8 ((Fw8u)ecx);	// m5=beta-1
	DIFF_GT_SSE2( xmm1, xmm2, xmm4, xmm7, xmm6) ;	// m7=|q0-p0|>A
	DIFF_GT_SSE2(xmm0, xmm1, xmm5, xmm4, xmm6) ;	// m4=|p0-p1|>B 
	xmm7 = _mm_or_si128 (xmm7, xmm4);	
	DIFF_GT_SSE2(xmm3, xmm2, xmm5, xmm4, xmm6) ;	// m4=|q1-q0|>B 
	xmm7 = _mm_or_si128 (xmm7, xmm4);
	xmm6 = _mm_xor_si128 (xmm6, xmm6);	
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm6);	// xmm7=~(|q1-q0|>B or |q0-p0|>A or|q1-q0|>B)
	_mm_storeu_si128( (__m128i*)mask_all, xmm7 ); // store 16 bytes

	xmm4=_mm_set1_epi8 ((Fw8u)((alpha>>2)+1));			 // m4=(alpha>>2)+1
	DIFF_GT_SSE2(xmm1, xmm2, xmm4, xmm5, xmm6) ; // m5=|p0-q0|>(alpha>>2)+1
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm5);
	_mm_storeu_si128( (__m128i*)mask_p0_q0, xmm7 ); // store 16 bytes

	xmm8 = _mm_loadu_si128( (__m128i*)(r8) );		// xmm0=p2
	xmm9 = _mm_loadu_si128( (__m128i*)(rdi+2*rsi) );	// xmm3=q2

	xmm4=_mm_set1_epi8 ((Fw8u)ecx);	// m4=beta-1
	DIFF_GT_SSE2(xmm1, xmm8, xmm4, xmm5, xmm6) ; // m5=|p2-p0|>beta
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm5);
	_mm_storeu_si128( (__m128i*)mask_p2_p0, xmm7 ); // store 16 bytes

	DIFF_GT_SSE2(xmm9, xmm2, xmm4, xmm5, xmm6) ; // m5=|q2-q0|>beta
	xmm7  = _mm_set1_epi8(0);    // zero;
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm5);
	_mm_storeu_si128( (__m128i*)mask_q2_q0, xmm7 ); // store 16 bytes

//////////////(2*p1+p0+q1+2)>>2:////////////////////
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(2);    // 2;
	xmm4 = xmm0;	//p1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// p1: u8->u16
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*p1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p1+p0
	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p1+p0+q1
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// 2*p1+p0+q1+2
	xmm4 = _mm_srli_epi16 (xmm4, 2);		// (2*p1+p0+q1+2)>>2

	xmm6 = xmm0;	//p1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// p1: u8->u16
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*p1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p1+p0
	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p1+p0+q1
	xmm7 = _mm_set1_epi16(2);    // 2;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// 2*p1+p0+q1+2
	xmm6 = _mm_srli_epi16 (xmm6, 2);		// (2*p1+p0+q1+2)>>2

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (2*p1+p0+q1+2)>>2: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_p2_p0) );	// mask_p2_p0
	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm6 = _mm_cmpeq_epi8 (xmm6, xmm5);					// ~mask_p0_0
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm5);					// ~mask_p2_p0
	xmm6 = _mm_or_si128  (xmm6, xmm7);					// ~mask_p0_0 | ~mask_p2_p0

	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & (~mask_p0_0 | ~mask_p2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm1);					// (mask_all&~mask_p0_q0)&p0 

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix-stride), xmm4 ); // (pix-stride)

////-----------(2*q1+q0+p1+2)>>2---------------------------
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(2);    // 2;
	xmm4 = xmm3;	//q1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// q1: u8->u16
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*q1
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q1+q0
	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p1: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q1+q0+p1
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// 2*q1+q0+p1+2
	xmm4 = _mm_srli_epi16 (xmm4, 2);		// (2*q1+q0+p1+2)>>2

	xmm6 = xmm3;	//q1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// q1: u8->u16
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*q1
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q1+q0
	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q1+q0+p1
	xmm7 = _mm_set1_epi16(2);    // 2;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// 2*q1+q0+p1+2
	xmm6 = _mm_srli_epi16 (xmm6, 2);		// (2*q1+q0+p1+2)>>2

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (2*q1+q0+p1+2)>>2: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_q2_q0) );	// mask_q2_q0
	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm6 = _mm_cmpeq_epi8 (xmm6, xmm5);					// ~mask_p0_q0
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm5);					// ~mask_q2_q0
	xmm6 = _mm_or_si128  (xmm6, xmm7);					// ~mask_p0_q0 | ~mask_q2_q0

	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & (~mask_p0_q0 | ~mask_q2_q0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm2);					// (mask_all&~mask_p0_q0)&q0 

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix), xmm4 ); // (pix)
//////////////////////////////////////////


//////////////(p2+2*(p1+p0+q0)+q1+4)>>3:////////////////////
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(4);    // 4;
	xmm4 = xmm0;	//p1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// p1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+p0+q0
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*(p1+p0+q0)
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p2+2*(p1+p0+q0)
	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q1: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p2+2*(p1+p0+q0)+q1
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// p2+2*(p1+p0+q0)+q1+4
	xmm4 = _mm_srli_epi16 (xmm4, 3);		// (p2+2*(p1+p0+q0)+q1+4)>>3

	xmm6 = xmm0;	//p1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// p1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0+q0
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*(p1+p0+q0)
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p2+2*(p1+p0+q0)
	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q1: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p2+2*(p1+p0+q0)+q1
	xmm7 = _mm_set1_epi16(4);    // 4;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// p2+2*(p1+p0+q0)+q1+4
	xmm6 = _mm_srli_epi16 (xmm6, 3);		// (p2+2*(p1+p0+q0)+q1+4)>>3

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (p2+2*p1+2*p0+2*q0+q1+4)>>3: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_p2_p0) );	// mask_p2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_p2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_p2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm1);					// (mask_all&~mask_p0_q0)&p0

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix-stride), xmm4 ); // (pix-stride)

////------------(p2+p1+p0+q0+2)>>2--------------------------
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(2);    // 4;
	xmm4 = xmm0;	//p1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// p1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+p0+q0
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p2+(p1+p0+q0)
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// p2+(p1+p0+q0)+2
	xmm4 = _mm_srli_epi16 (xmm4, 2);		// (p2+(p1+p0+q0)+2)>>2

	xmm6 = xmm0;	//p1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// p1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0+q0
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p2+(p1+p0+q0)
	xmm7 = _mm_set1_epi16(2);    // 2;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// p2+(p1+p0+q0)+2
	xmm6 = _mm_srli_epi16 (xmm6, 2);		// (p2+(p1+p0+q0)+2)>>2

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (p2+(p1+p0+q0)+2)>>2: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_p2_p0) );	// mask_p2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_p2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_p2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm0);					// (mask_all&~mask_p0_q0)&p1 

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix-2*stride), xmm4 ); // (pix-stride)


////----------(2*p3+3*p2+p1+p0+q0+4)>>3----------------------------

	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(4);    // 4;

	xmm4 = _mm_loadu_si128( (__m128i*)(r8) );// p3 = [-4*stride] fix r8-stride
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// p3: u8->u16
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*p3
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p2: u8->u16
//	xmm6 = xmm5;    // fix
//	xmm5 = _mm_slli_epi16 (xmm5, 1);		// 2*p2
//	xmm5 = _mm_add_epi16 (xmm5, xmm6);		// 3*p2	<<< fix
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2

	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p1: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2+p1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2+p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*p3+3*p2+p1+p0+q0
//	xmm6 = _mm_set1_epi16(4);    // 4;
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// 2*p3+3*p2+p1+p0+q0+4
	xmm4 = _mm_srli_epi16 (xmm4, 3);		// (2*p3+3*p2+p1+p0+q0+4)>>3

	xmm6 = _mm_loadu_si128( (__m128i*)(r8) );// p3 = [-4*stride] <<< fix r8-stride
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// p3: u8->u16
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*p3
	xmm5 = xmm8;	//p2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p2: u8->u16
//	xmm6 = xmm5;    // fix
//	xmm5 = _mm_slli_epi16 (xmm5, 1);		// 2*p2
//	xmm5 = _mm_add_epi16 (xmm5, xmm6);		// 3*p2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2

	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p1: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2+p1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2+p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*p3+3*p2+p1+p0+q0

	xmm7 = _mm_set1_epi16(4);    // 4;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// 2*p3+3*p2+p1+p0+q0+4
	xmm6 = _mm_srli_epi16 (xmm6, 3);		// (2*p3+3*p2+p1+p0+q0+4)>>3

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (p2+2*p1+2*p0+2*q0+q1+4)>>3: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_p2_p0) );	// mask_p2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_p2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_p2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm8);					// (mask_all&~mask_p0_q0)&p2

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix-3*stride), xmm4 ); // (pix-3*stride)

//////////////////////////////////////////

//////////////(p1+2*(p0+q0+q1)+q2+4)>>3:////////////////////
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(4);    // 4;
	xmm4 = xmm3;	//q1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// q1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// q1+p0+q0
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*(q1+p0+q0)
	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p1: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// p1+2*(p1+p0+q0)
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q2: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// q2+2*(p1+p0+q0)+p1
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// q2+2*(p1+p0+q0)+p1+4
	xmm4 = _mm_srli_epi16 (xmm4, 3);		// (q2+2*(p1+p0+q0)+p1+4)>>3

	xmm6 = xmm3;	//q1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// q1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0+q0
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*(q1+p0+q0)
	xmm5 = xmm0;	//p1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p1: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+2*(q1+p0+q0)
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q2: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// q2+2*(p1+p0+q0)+p1
	xmm7 = _mm_set1_epi16(4);    // 4;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// q2+2*(p1+p0+q0)+p1+4
	xmm6 = _mm_srli_epi16 (xmm6, 3);		// (q2+2*(p1+p0+q0)+p1+4)>>3

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (q2+2*p1+2*p0+2*q0+p1+4)>>3: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_q2_q0) );	// mask_q2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_q2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_q2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm2);					// (mask_all&~mask_p0_q0)&q0

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix), xmm4 ); // (pix)

////------------(p0+q0+q1+q2+2)>>2--------------------------
	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(2);    // 4;
	xmm4 = xmm3;	//q1
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// q1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// q1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// q1+p0+q0
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// q2+(q1+p0+q0)
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// q2+(q1+p0+q0)+2
	xmm4 = _mm_srli_epi16 (xmm4, 2);		// (q2+(q1+p0+q0)+2)>>2

	xmm6 = xmm3;	//q1
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// p1: u8->u16
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p1+p0+q0
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p2: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// p2+(p1+p0+q0)
	xmm7 = _mm_set1_epi16(2);    // 2;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// p2+(p1+p0+q0)+2
	xmm6 = _mm_srli_epi16 (xmm6, 2);		// (p2+(p1+p0+q0)+2)>>2

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (p2+(p1+p0+q0)+2)>>2: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_q2_q0) );	// mask_q2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_q2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_q2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm3);					// (mask_all&~mask_p0_q0)&q1

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix+stride), xmm4 ); // (pix+1*stride)
////----------(2*q3+3*q2+q1+q0+p0+4)>>3----------------------------

	xmm7 = _mm_set1_epi8(0);    // zero;
	xmm6 = _mm_set1_epi16(4);    // 4;

	xmm4 = _mm_loadu_si128( (__m128i*)(rdi+3*rsi) );// q3 = [3*stride]
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm7);	// q3: u8->u16
	xmm4 = _mm_slli_epi16 (xmm4, 1);		// 2*q3
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q2: u8->u16
//	xmm8 = xmm5;		// <<<<<<<<< fix
//	xmm5 = _mm_slli_epi16 (xmm5, 1);		// 2*q2
//	xmm5 = _mm_add_epi16 (xmm5, xmm8);		// 3*q2
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2

	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q1: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2+q1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2+q1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm4 = _mm_add_epi16 (xmm4, xmm5);		// 2*q3+3*q2+q1+p0+q0
	xmm4 = _mm_add_epi16 (xmm4, xmm6);		// 2*q3+3*q2+q1+p0+q0+4
	xmm4 = _mm_srli_epi16 (xmm4, 3);		// (2*q3+3*q2+q1+p0+q0+4)>>3

	xmm6 = _mm_loadu_si128( (__m128i*)(rdi+3*rsi) );// q3 = [3*stride]
	xmm6 = _mm_unpackhi_epi8(xmm6, xmm7);	// q3: u8->u16
	xmm6 = _mm_slli_epi16 (xmm6, 1);		// 2*q3
	xmm5 = xmm9;	//q2
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q2: u8->u16
//	xmm8 = xmm5;		// <<<<<<<<< fix
//	xmm5 = _mm_slli_epi16 (xmm5, 1);		// 2*q2
//	xmm5 = _mm_add_epi16 (xmm5, xmm8);		// 3*q2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2

	xmm5 = xmm3;	//q1
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q1: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2+q1
	xmm5 = xmm1;	//p0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// p0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2+q1+p0
	xmm5 = xmm2;	//q0
	xmm5 = _mm_unpackhi_epi8(xmm5, xmm7);	// q0: u8->u16
	xmm6 = _mm_add_epi16 (xmm6, xmm5);		// 2*q3+3*q2+q1+p0+q0
	xmm7 = _mm_set1_epi16(4);    // 4;
	xmm6 = _mm_add_epi16 (xmm6, xmm7);		// 2*q3+3*q2+q1+p0+q0+4
	xmm6 = _mm_srli_epi16 (xmm6, 3);		// (2*q3+3*q2+q1+p0+q0+4)>>3

	xmm4 = _mm_packus_epi16 (xmm4, xmm6);	// (p2+2*p1+2*p0+2*q0+q1+4)>>3: 16bit to 8bit

	xmm6 = _mm_loadu_si128( (__m128i*)(mask_p0_q0) );	// mask_p0_q0
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_q2_q0) );	// mask_q2_p0
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_p0_q0 & mask_q2_p0)
	xmm7 = _mm_loadu_si128( (__m128i*)(mask_all) );		// mask_all
	xmm6 = _mm_and_si128 (xmm6, xmm7);					// mask_all & mask_p0_q0 & mask_q2_p0)

	xmm4 = _mm_and_si128 (xmm4, xmm6);					// (mask_all&~mask_p0_q0)&(2*p1+p0+q1+2)>>2 

	xmm5 = _mm_set1_epi16(0);    // 0;
	xmm5 = _mm_cmpeq_epi8 (xmm5, xmm6);					// else
	xmm5 = _mm_and_si128  (xmm5, xmm9);					// (mask_all&~mask_p0_q0)&q2

	xmm4 = _mm_or_si128 (xmm4, xmm5);					// (2*p1+p0+q1+2)>>2 

	_mm_storeu_si128( (__m128i*)(pix+2*stride), xmm4 ); // (pix+2*stride)
}

void  static My_deblock_h_luma_intra_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta )
{
	Fw8u *rdi;
	SYS_FORCEALIGN_16  Fw8u pix_tmp[256];
	Fw8u *ptmp = &pix_tmp[0];
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;

    rdi = pix;
	TRANSPOSE_SSE8x8(rdi-4, stride, ptmp, 16);
	TRANSPOSE_SSE8x8(rdi-4+stride*4, stride, ptmp+8, 16);

	My_deblock_v_luma_intra_sse2(ptmp+16*3, 16, alpha, beta);

	TRANSPOSE_SSE8x8(ptmp, 16, rdi-4, stride);
	TRANSPOSE_SSE8x8(ptmp+8, 16, rdi-4+stride*4, stride);
}

static inline void deblock_luma_intra_c( Fw8u *pix, Fw32s xstride, Fw32s ystride, Fw32s alpha, Fw32s beta )
{
    int d;
    for( d = 0; d < 16; d++ ) {
        const int p2 = pix[-3*xstride];
        const int p1 = pix[-2*xstride];
        const int p0 = pix[-1*xstride];
        const int q0 = pix[ 0*xstride];
        const int q1 = pix[ 1*xstride];
        const int q2 = pix[ 2*xstride];

        if( abs( p0 - q0 ) < alpha &&
            abs( p1 - p0 ) < beta &&
            abs( q1 - q0 ) < beta ) {

            if(abs( p0 - q0 ) < ((alpha >> 2) + 2) ){
                if( abs( p2 - p0 ) < beta)
                {
                    const int p3 = pix[-4*xstride];
                    /* p0', p1', p2' */
                    pix[-1*xstride] = (Fw8u)(( p2 + 2*p1 + 2*p0 + 2*q0 + q1 + 4 ) >> 3);
                    pix[-2*xstride] = (Fw8u)(( p2 + p1 + p0 + q0 + 2 ) >> 2);
                    pix[-3*xstride] = (Fw8u)(( 2*p3 + 3*p2 + p1 + p0 + q0 + 4 ) >> 3);
                } else {
                    /* p0' */
                    pix[-1*xstride] = (Fw8u)(( 2*p1 + p0 + q1 + 2 ) >> 2);
                }
                if( abs( q2 - q0 ) < beta)
                {
                    const int q3 = pix[3*xstride];
                    /* q0', q1', q2' */
                    pix[0*xstride] = (Fw8u)(( p1 + 2*p0 + 2*q0 + 2*q1 + q2 + 4 ) >> 3);
                    pix[1*xstride] = (Fw8u)(( p0 + q0 + q1 + q2 + 2 ) >> 2);
                    pix[2*xstride] = (Fw8u)(( 2*q3 + 3*q2 + q1 + q0 + p0 + 4 ) >> 3);
                } else {
                    /* q0' */
                    pix[0*xstride] = (Fw8u)(( 2*q1 + q0 + p1 + 2 ) >> 2);
                }
            }else{
                /* p0', q0' */
                pix[-1*xstride] = (Fw8u)((Fw8u)(( 2*p1 + p0 + q1 + 2 ) >> 2));
                pix[ 0*xstride] = (Fw8u)((Fw8u)(( 2*q1 + q0 + p1 + 2 ) >> 2));
            }
        }
        pix += ystride;
    }
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_VerEdge_H264_8u_C1IR)( 
			Fw8u *pSrcDst, Fw32s  srcdstStep, 
			Fw8u *pAlpha,	 
			Fw8u *pBeta, 
			Fw8u *pThresholds, 
			Fw8u *pBS)
{
  Fw32s   xpel;
  Fw32s   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
  Fw32s   PelNum;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	PelNum = 16 ;	// for LUM
	for(SrcPtr=pSrcDst, xpel=0; xpel<PelNum; xpel+=4, SrcPtr+=4)
	{
		if(xpel == 0 || xpel == 3){	// external lines
			Alpha=pAlpha[0];
			Beta =pBeta[0];
		}
		else {						// internal lines
			Alpha=pAlpha[1]; 
			Beta =pBeta[1];
		}
	    if( pBS[0] < 4 ) {	// INTER filtering
			Fw8s tc0[4]; 
			Fw32s i; 
	        for(i=0; i<4; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;

			if( Dispatch_Type == DT_SSE2)
				My_deblock_h_luma_sse2(SrcPtr, srcdstStep, Alpha, Beta, tc0);
			else
				deblock_luma_c( SrcPtr, 1, srcdstStep, Alpha, Beta, tc0);
		} else {			// INTRA strong filtering
			if( Dispatch_Type == DT_SSE2)
				My_deblock_h_luma_intra_sse2( SrcPtr, srcdstStep, Alpha, Beta );
			else
				deblock_luma_intra_c( SrcPtr, 1, srcdstStep, Alpha, Beta );
			}
		pThresholds+=4;
		pBS+=4;
	}

  return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_HorEdge_H264_8u_C1IR)( Fw8u* pSrcDst,
			Fw32s srcdstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, 
			Fw8u* pBS)
{
  int   ypel;
  int   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

//	PelNum = 16 ;	// for LUM
	for(SrcPtr=pSrcDst, ypel=0; ypel<16; ypel+=4, SrcPtr+=(srcdstStep*4))
	{
		if(ypel == 0 || ypel == 3){	// external lines
			Alpha=pAlpha[0];
			Beta =pBeta[0];
		}
		else	{					// internal lines
			Alpha=pAlpha[1]; 
			Beta =pBeta[1];
		}
	    if( *pBS < 4 ) {	// INTER filtering
			Fw8s tc0[4]; 
			Fw32s i; 
	        for(i=0; i<4; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;

			if( Dispatch_Type == DT_SSE2)
				My_deblock_v_luma_sse2(SrcPtr, srcdstStep, Alpha, Beta, tc0);
			else
				deblock_luma_c( SrcPtr, srcdstStep, 1, Alpha, Beta, tc0);
		} 
		else {			// INTRA strong filtering

			if( Dispatch_Type == DT_SSE2)
				My_deblock_v_luma_intra_sse2( SrcPtr, srcdstStep, Alpha, Beta );
			else
				deblock_luma_intra_c( SrcPtr, srcdstStep, 1, Alpha, Beta );

		}
		pThresholds+=4;
		pBS+=4;
	}
	return fwStsNoErr;
}


//; out: %4 = |%1-%2|>%3
//; clobbers: %5
#define DIFF_GT_MMX(m2,m3,m4,m5,m6)	\
{	\
    m6 = m3;	\
    m5 = m2;	\
    m6 = _mm_subs_epu8(m6, m2);	\
    m5 = _mm_subs_epu8(m5, m3);	\
    m5 = _mm_or_si128(m5, m6);	\
    m5 = _mm_subs_epu8(m5, m4);	\
}
#define LOAD_MASK_MMX(r1,r2)	\
{	\
	xmm4=_mm_set1_epi8 ((Fw8u)r1);\
	xmm5=_mm_set1_epi8 ((Fw8u)r2);\
    DIFF_GT_MMX(xmm1, xmm2, xmm4, xmm7, xmm6);	\
    DIFF_GT_MMX(xmm0, xmm1, xmm5, xmm4, xmm6);	\
    xmm7 = _mm_or_si128(xmm7, xmm4);	\
    DIFF_GT_MMX(xmm3, xmm2, xmm5, xmm4, xmm6);	\
    xmm7 = _mm_or_si128(xmm7, xmm4);	\
	xmm6 = _mm_xor_si128(xmm6, xmm6);	\
	xmm7 = _mm_cmpeq_epi8 (xmm7, xmm6);	\
}

//; in: mm0=p1 mm1=p0 mm2=q0 mm3=q1 mm7=(tc&mask)
//; out: mm1=p0' mm2=q0'
//; clobbers: mm0,3-6
#define DEBLOCK_P0_Q0_MMX()	\
{\
    xmm5 = xmm1;	\
	xmm5 = _mm_xor_si128 (xmm5, xmm2);	\
	xmm5 = _mm_and_si128 (xmm5, _mm_loadu_si128((__m128i*)pb_01));	\
	xmm4 = _mm_cmpeq_epi8 (xmm4, xmm4);	\
	xmm3 = _mm_xor_si128 (xmm3, xmm4);	\
	xmm3 = _mm_avg_epu8 (xmm3, xmm0);	\
	xmm3 = _mm_avg_epu8 (xmm3, _mm_loadu_si128((__m128i*)pb_03));	\
	xmm4 = _mm_xor_si128 (xmm4, xmm1);	\
	xmm4 = _mm_avg_epu8 (xmm4, xmm2);	\
	xmm3 = _mm_avg_epu8 (xmm3, xmm5);	\
	xmm3 = _mm_adds_epu8 (xmm3, xmm4);	\
	xmm6 = _mm_loadu_si128( (__m128i*)pb_a1 );	\
	xmm6 = _mm_subs_epu8(xmm6, xmm3);	\
	xmm3 = _mm_subs_epu8(xmm3, _mm_loadu_si128((__m128i*)pb_a1) );	\
	xmm6 = _mm_min_epu8(xmm6, xmm7);	\
	xmm3 = _mm_min_epu8(xmm3, xmm7);	\
	xmm1 = _mm_subs_epu8(xmm1, xmm6);	\
	xmm2 = _mm_subs_epu8(xmm2, xmm3);	\
	xmm1 = _mm_adds_epu8 (xmm1, xmm3);	\
	xmm2 = _mm_adds_epu8 (xmm2, xmm6);	\
}

//;-----------------------------------------------------------------------------
//;   void My_deblock_v_chroma_mmxext( uint8_t *pix, int stride, int alpha, int beta, int8_t *tc0 )
//;-----------------------------------------------------------------------------
static inline void deblock_chroma_c( Fw8u *pix, Fw32s xstride, Fw32s ystride,
									Fw32s alpha, Fw32s beta, Fw8s *tc0 );
void static My_deblock_v_chroma_sse2(Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
	Fw8u *rdi, *rax;
	Fw32s *a;
	Fw32s rsi, edx, ecx;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;

    for( int i = 0; i < 4; i++ ) {
        const int tc = tc0[i];
        if( tc <= 0 ) {
			deblock_chroma_c( pix, stride, 1, alpha, beta, tc0);
            return;
        }
	}

//    CHROMA_V_START
    rdi = pix;
    rsi = stride;
    edx = alpha-1;
    ecx = beta-1;

    rax = rdi;
	rax = rdi - 2*rsi;
//    movq  mm0, [rax];
	xmm0 = _mm_loadl_epi64( (__m128i*)(rax) );
//    movq  mm1, [rax+rsi]
	xmm1 = _mm_loadl_epi64( (__m128i*)(rax+rsi) );
//    movq  mm2, [rdi]
	xmm2 = _mm_loadl_epi64( (__m128i*)(rdi) );
//    movq  mm3, [rdi+rsi]
	xmm3 = _mm_loadl_epi64( (__m128i*)(rdi+rsi) );

    LOAD_MASK_MMX(edx, ecx);

//    movd       mm6, [r8] ; tc0
	a    = (int *)tc0;
	xmm6 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src

//    punpcklbw  mm6, mm6
	xmm6 = _mm_unpacklo_epi8(xmm6, xmm6);
//    pand       mm7, mm6
	xmm7 = _mm_and_si128 (xmm7, xmm6);

    DEBLOCK_P0_Q0_MMX();

//    movq  [rax+rsi], mm1
	_mm_storel_epi64( (__m128i*)(rax+rsi), xmm1 ); // store 8 bytes
//    movq  [rdi], mm2
	_mm_storel_epi64( (__m128i*)(rdi), xmm2 ); // store 8 bytes

	return;
}

/*
; in: 8 rows of 4 bytes in %1..%8
; out: 4 rows of 8 bytes in mm0..mm3
*/
#define TRANSPOSE4x8_LOAD(r1,r2,r3,r4,r5,r6,r7,r8)	\
{	\
 	xmm0 = _mm_loadl_epi64( (__m128i*)(r1) );	\
 	xmm2 = _mm_loadl_epi64( (__m128i*)(r2) );	\
 	xmm1 = _mm_loadl_epi64( (__m128i*)(r3) );	\
 	xmm3 = _mm_loadl_epi64( (__m128i*)(r4) );	\
	xmm0 = _mm_unpacklo_epi8(xmm0, xmm2);	\
	xmm1 = _mm_unpacklo_epi8(xmm1, xmm3);	\
	xmm0 = _mm_unpacklo_epi16(xmm0, xmm1);	\
 	xmm4 = _mm_loadl_epi64( (__m128i*)(r5) );	\
 	xmm6 = _mm_loadl_epi64( (__m128i*)(r6) );	\
 	xmm5 = _mm_loadl_epi64( (__m128i*)(r7) );	\
 	xmm7 = _mm_loadl_epi64( (__m128i*)(r8) );	\
	xmm4 = _mm_unpacklo_epi8(xmm4, xmm6);	\
	xmm5 = _mm_unpacklo_epi8(xmm5, xmm7);	\
	xmm4 = _mm_unpacklo_epi16(xmm4, xmm5);	\
    xmm1 = xmm0;	\
    xmm0 = _mm_unpacklo_epi32(xmm0, xmm4);	\
    xmm1 = _mm_unpackhi_epi32(xmm1, xmm4);	\
	xmm2 = xmm1;	\
	xmm3 = xmm1;	\
	xmm1 = xmm0;	\
	xmm3 = _mm_srli_si128  (xmm3, 8);	\
	xmm1 = _mm_srli_si128  (xmm1, 8);	\
}
/*
; in: 4 rows of 8 bytes in mm0..mm3
; out: 8 rows of 4 bytes in %1..%8
*/
#define TRANSPOSE8x4_STORE(r1,r2,r3,r4,r5,r6,r7,r8)	\
{	\
	xmm0 = _mm_unpacklo_epi8(xmm0, xmm1);\
	xmm2 = _mm_unpacklo_epi8(xmm2, xmm3);\
	xmm1 = xmm0; \
    xmm3 = xmm2; \
	xmm1 = _mm_unpacklo_epi16(xmm1, xmm3);	\
	xmm0 = _mm_srli_si128  (xmm0, 8);	\
	xmm2 = _mm_srli_si128  (xmm2, 8);	\
	xmm0 = _mm_unpacklo_epi16(xmm0, xmm2);\
	*(r1)   = _mm_cvtsi128_si32 (xmm1);	\
	xmm1 = _mm_srli_si128  (xmm1, 4);	\
	*(r2)   = _mm_cvtsi128_si32 (xmm1);	\
	xmm1 = _mm_srli_si128  (xmm1, 4);	\
	*(r3)   = _mm_cvtsi128_si32 (xmm1);	\
	xmm1 = _mm_srli_si128  (xmm1, 4);	\
	*(r4)   = _mm_cvtsi128_si32 (xmm1);	\
	*(r5)   = _mm_cvtsi128_si32 (xmm0);	\
	xmm0 = _mm_srli_si128  (xmm0, 4);	\
	*(r6)   = _mm_cvtsi128_si32 (xmm0);	\
	xmm0 = _mm_srli_si128  (xmm0, 4);	\
	*(r7)   = _mm_cvtsi128_si32 (xmm0);	\
	xmm0 = _mm_srli_si128  (xmm0, 4);	\
	*(r8)   = _mm_cvtsi128_si32 (xmm0);	\
}

 void static My_deblock_h_chroma_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta, Fw8s *tc0 )
 {
	Fw8u *rdi, *rax;
	Fw32s *a;
	Fw32s rsi, edx, ecx, r9;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm10, xmm11;


    for( int i = 0; i < 4; i++ ) {
        const int tc = tc0[i];
        if( tc <= 0 ) {
			deblock_chroma_c( pix, 1, stride, alpha, beta, tc0);
            return;
        }
	}
//    CHROMA_H_START
/*
%macro CHROMA_H_START 0
    movsxd rsi, esi
    dec    edx
    dec    ecx
    sub    rdi, 2
    lea    r9, [rsi+rsi*2]
    mov    rax, rdi
    add    rdi, r9
%endmacro
*/
    rdi = pix;
    rsi = stride;
    edx = alpha-1;
    ecx = beta-1;

	rdi -= 2;
	r9  = rsi+rsi*2;
    rax = rdi;
	rdi += r9;

//%define PASS8ROWS(base, base3, stride, stride3) \
//    [base], [base+stride], [base+stride*2], [base3], \
//    [base3+stride], [base3+stride*2], [base3+stride3], [base3+stride*4]
// rax, rax+stride, rax+stride*2, 
// rdi, rdi+stride, rdi+stride*2, 
// rdi+stride3, rdi+stride*4
//    TRANSPOSE4x8_LOAD  PASS8ROWS(rax, rdi, rsi, r9)
	TRANSPOSE4x8_LOAD(rax, rax+stride, rax+stride*2, rdi, rdi+stride, rdi+stride*2, rdi+r9, rdi+stride*4);
//    movq  [rsp-8], mm0

//	_mm_storel_epi64( (__m128i*)(rsp-8), xmm0 ); // store 8 bytes
	xmm10 = xmm0;
//    movq  [rsp-16], mm3
	xmm11 = xmm3;

    LOAD_MASK_MMX( edx, ecx);

//    movd       mm6, [r8] ; tc0
	a    = (int *)tc0;
	xmm6 = _mm_cvtsi32_si128 (*a);			// get 4 bytes from src

//    punpcklbw  mm6, mm6
	xmm6 = _mm_unpacklo_epi8(xmm6, xmm6);
//    pand       mm7, mm6
	xmm7 = _mm_and_si128 (xmm7, xmm6);

    DEBLOCK_P0_Q0_MMX();

//    movq  mm0, [rsp-8]
	xmm0 = xmm10;
//    movq  mm3, [rsp-16]
	xmm3 = xmm11;

//    TRANSPOSE8x4_STORE PASS8ROWS(rax, rdi, rsi, r9)
//%define PASS8ROWS(base, base3, stride, stride3) \
//    [base], [base+stride], [base+stride*2], 
//	  [base3], [base3+stride], [base3+stride*2], 
//	  [base3+stride3], [base3+stride*4]
	TRANSPOSE8x4_STORE((Fw32s *)rax, (Fw32s *)(rax+rsi), (Fw32s *)(rax+rsi*2), 
						(Fw32s *)rdi, (Fw32s *)(rdi+rsi), (Fw32s *)(rdi+rsi*2), 
							(Fw32s *)(rdi+r9), (Fw32s *)(rdi+rsi*4));

    return;
 }

static inline void deblock_chroma_c( Fw8u *pix, Fw32s xstride, Fw32s ystride,
									Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
    int i, d;
    for( i = 0; i < 4; i++ ) {
        const int tc = tc0[i];
        if( tc <= 0 ) {
            pix += 2*ystride;
            continue;
        }
        for( d = 0; d < 2; d++ ) {
            const int p1 = pix[-2*xstride];
            const int p0 = pix[-1*xstride];
            const int q0 = pix[ 0*xstride];
            const int q1 = pix[ 1*xstride];

            if( abs( p0 - q0 ) < alpha &&
                abs( p1 - p0 ) < beta &&
                abs( q1 - q0 ) < beta ) {

                int delta = My_clip3( (((q0 - p0 ) << 2) + (p1 - q1) + 4) >> 3, -tc, tc );
                pix[-1*xstride] = clip_uint8( p0 + delta );    /* p0' */
                pix[ 0*xstride] = clip_uint8( q0 - delta );    /* q0' */
            }
            pix += ystride;
        }
    }
}

#define CHROMA_INTRA_P0(m1, m2, m3)	\
{	\
    xmm4 = m1;	\
	xmm4 = _mm_xor_si128(xmm4, m3);	\
	xmm4 = _mm_and_si128(xmm4, _mm_loadu_si128((__m128i*)pb_01) );	\
    m1 = _mm_avg_epu8(m1, m3);	\
	m1 = _mm_subs_epu8(m1, xmm4); \
    m1 = _mm_avg_epu8(m1, m2);	\
}
#define CHROMA_INTRA_BODY(r1, r2)	\
{	\
    LOAD_MASK_MMX((Fw8u)r1, (Fw8u)r2); \
    xmm5 = xmm1;	\
    xmm6 = xmm2;	\
    CHROMA_INTRA_P0(xmm1, xmm0, xmm3);	\
    CHROMA_INTRA_P0(xmm2, xmm3, xmm0);	\
	xmm1 = _mm_sub_epi8 (xmm1, xmm5);	\
	xmm2 = _mm_sub_epi8 (xmm2, xmm6);	\
	xmm1 = _mm_and_si128 (xmm1, xmm7);	\
	xmm2 = _mm_and_si128 (xmm2, xmm7);	\
	xmm1 = _mm_add_epi8(xmm1, xmm5);	\
	xmm2 = _mm_add_epi8(xmm2, xmm6);	\
}
//;-----------------------------------------------------------------------------
//;   void My_deblock_v_chroma_intra_mmxext( uint8_t *pix, int stride, int alpha, int beta )
//;-----------------------------------------------------------------------------
void  static My_deblock_v_chroma_intra_sse2(Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta )
{
	Fw8u *rdi, *rax;
	Fw32s rsi, edx, ecx;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;

//    CHROMA_V_START
/*
%macro CHROMA_V_START 0
    ; rdi = pix
    movsxd rsi, esi ; stride
    dec    edx      ; alpha-1
    dec    ecx      ; beta-1
    mov    rax, rdi
    sub    rax, rsi
    sub    rax, rsi
%endmacro
*/
    rdi = pix;
    rsi = stride;
    edx = alpha-1;
    ecx = beta-1;

    rax = rdi;
	rax = rdi - 2*rsi;
//    movq  mm0, [rax];
	xmm0 = _mm_loadl_epi64( (__m128i*)(rax) );
//    movq  mm1, [rax+rsi]
	xmm1 = _mm_loadl_epi64( (__m128i*)(rax+rsi) );
//    movq  mm2, [rdi]
	xmm2 = _mm_loadl_epi64( (__m128i*)(rdi) );
//    movq  mm3, [rdi+rsi]
	xmm3 = _mm_loadl_epi64( (__m128i*)(rdi+rsi) );

    CHROMA_INTRA_BODY(edx, ecx);

//    movq  [rax+rsi], mm1
	_mm_storel_epi64( (__m128i*)(rax+rsi), xmm1 ); // store 8 bytes
//    movq  [rdi], mm2
	_mm_storel_epi64( (__m128i*)(rdi), xmm2 ); // store 8 bytes

    return;
}

void  static My_deblock_h_chroma_intra_sse2(Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta )
{
	Fw8u *rdi, *rax;
	Fw32s rsi, edx, ecx, r9;
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
//    CHROMA_H_START
/*
%macro CHROMA_H_START 0
    movsxd rsi, esi
    dec    edx
    dec    ecx
    sub    rdi, 2
    lea    r9, [rsi+rsi*2]
    mov    rax, rdi
    add    rdi, r9
%endmacro
*/
    rdi = pix;
    rsi = stride;
    edx = alpha-1;
    ecx = beta-1;

	rdi -= 2;
	r9  = rsi+rsi*2;
    rax = rdi;
	rdi += r9;

//    TRANSPOSE4x8_LOAD  PASS8ROWS(rax, rdi, rsi, r9)
//%define PASS8ROWS(base, base3, stride, stride3) \
//    [base], [base+stride], [base+stride*2], 
//	  [base3], [base3+stride], [base3+stride*2], 
//	  [base3+stride3], [base3+stride*4]
	TRANSPOSE4x8_LOAD(rax, rax+rsi, rax+rsi*2, rdi, rdi+rsi, rdi+rsi*2, rdi+r9, rdi+rsi*4);
    CHROMA_INTRA_BODY(edx, ecx);
//    TRANSPOSE8x4_STORE PASS8ROWS(rax, rdi, rsi, r9)
	TRANSPOSE8x4_STORE((Fw32s *)rax, (Fw32s *)(rax+rsi), (Fw32s *)(rax+rsi*2), 
						(Fw32s *)rdi, (Fw32s *)(rdi+rsi), (Fw32s *)(rdi+rsi*2), 
							(Fw32s *)(rdi+r9), (Fw32s *)(rdi+rsi*4));
    return;
}

static inline void deblock_chroma_intra_c( Fw8u *pix, Fw32s xstride, Fw32s ystride, Fw32s alpha, Fw32s beta )
{   
    int d; 
    for( d = 0; d < 8; d++ ) {
        const int p1 = pix[-2*xstride];
        const int p0 = pix[-1*xstride];
        const int q0 = pix[ 0*xstride];
        const int q1 = pix[ 1*xstride];

        if( abs( p0 - q0 ) < alpha &&
            abs( p1 - p0 ) < beta &&
            abs( q1 - q0 ) < beta ) {

            pix[-1*xstride] = (Fw8u)((2*p1 + p0 + q1 + 2) >> 2);   /* p0' */
            pix[ 0*xstride] = (Fw8u)((2*q1 + q0 + p1 + 2) >> 2);   /* q0' */
        }

        pix += ystride;
    }
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_VerEdge_H264_8u_C1IR)( Fw8u* pSrcDst,
			Fw32s srcdstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, 
			Fw8u* pBS)
{
  int   xpel;
  int   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
  int   PelNum;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	PelNum = 4 ;	// for Chroma
	for(SrcPtr=pSrcDst, xpel=0; xpel<PelNum; xpel+=4, SrcPtr+=4)
	{
		if(xpel == 0)			// external lines
		{
			Alpha=pAlpha[0];
			Beta =pBeta[0];
		}
		else					// internal lines
		{
			Alpha=pAlpha[1]; 
			Beta =pBeta[1];
		}

	    if( pBS[0] < 4 ) {	// INTER filtering
			Fw8s tc0[4]; 
			Fw32s i; 
	        for(i=0; i<4; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;

			if( Dispatch_Type == DT_SSE2)
				My_deblock_h_chroma_sse2( SrcPtr, srcdstStep, Alpha, Beta, tc0);
			else
				deblock_chroma_c( SrcPtr, 1, srcdstStep, Alpha, Beta, tc0);

		} else {			// INTRA strong filtering
			if( Dispatch_Type == DT_SSE2)
				My_deblock_h_chroma_intra_sse2( SrcPtr, srcdstStep, Alpha, Beta);
			else
				deblock_chroma_intra_c( SrcPtr, 1, srcdstStep, Alpha, Beta );
		}
		pThresholds+=4;
		pBS+=4;
	}

  return fwStsNoErr;
}

FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_HorEdge_H264_8u_C1IR)( Fw8u* pSrcDst,
			Fw32s srcdstStep, Fw8u* pAlpha, Fw8u* pBeta, Fw8u* pThresholds, 
			Fw8u* pBS)
{
  int   ypel;
  int   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
  int   PelNum;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	PelNum = 4 ;	// for Chroma
	for(SrcPtr=pSrcDst, ypel=0; ypel<PelNum; ypel+=4, SrcPtr+=(srcdstStep*4))
	{
		if(ypel == 0)	{		// external lines
			Alpha=pAlpha[0];
			Beta =pBeta[0];
		}
		else	{				// internal lines
			Alpha=pAlpha[1]; 
			Beta =pBeta[1];
		}
	    if( pBS[0] < 4 ) {	// INTER filtering
			Fw8s tc0[4]; 
			Fw32s i; 
	        for(i=0; i<4; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;

			if( Dispatch_Type == DT_SSE2)
				My_deblock_v_chroma_sse2( SrcPtr, srcdstStep, Alpha, Beta, tc0);
			else
				deblock_chroma_c( SrcPtr, srcdstStep, 1, Alpha, Beta, tc0);

		} else {			// INTRA strong filtering

			if( Dispatch_Type == DT_SSE2)
				My_deblock_v_chroma_intra_sse2( SrcPtr, srcdstStep, Alpha, Beta);
			else
				deblock_chroma_intra_c( SrcPtr, srcdstStep, 1, Alpha, Beta );
		}
		pThresholds+=4;
		pBS+=4;
	}
	return fwStsNoErr;
}

void static My_deblock_h_luma_MBAFF_sse2( Fw8u *pix, Fw32s stride, Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
	Fw8u *rdi;
	SYS_FORCEALIGN_16  Fw8u pix_tmp[256];
	Fw8u *ptmp = &pix_tmp[0];
	__m128i xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8;

    rdi = pix;
	TRANSPOSE_SSE8x8(rdi-4, stride, ptmp, 16);

	My_deblock_v_luma_sse2(ptmp+16*3, 16, alpha, beta, tc0);

	TRANSPOSE_SSE8x8(ptmp, 16, rdi-4, stride);
}


static inline void deblock_luma_MBAFF_c( Fw8u *pix, Fw32s ystride, 
								  Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
    int i, d;
    for( i = 0; i < 2; i++ ) {
        if( tc0[i] < 0 ) {
            pix += 4*ystride;
            continue;
        }
        for( d = 0; d < 4; d++ ) {
            const int p2 = pix[-3];
            const int p1 = pix[-2];
            const int p0 = pix[-1];
            const int q0 = pix[ 0];
            const int q1 = pix[ 1];
            const int q2 = pix[ 2];
   
            if( abs( p0 - q0 ) < alpha &&
                abs( p1 - p0 ) < beta &&
                abs( q1 - q0 ) < beta ) {
   
                int tc = tc0[i];
                int delta;
   
                if( abs( p2 - p0 ) < beta ) {
                    pix[-2] = (Fw8u)(p1 + My_clip3( (( p2 + ((p0 + q0 + 1) >> 1)) >> 1) - p1, -tc0[i], tc0[i] ));
                    tc++; 
                }
                if( abs( q2 - q0 ) < beta ) {
                    pix[ 1] = (Fw8u)(q1 + My_clip3( (( q2 + ((p0 + q0 + 1) >> 1)) >> 1) - q1, -tc0[i], tc0[i] ));
                    tc++;
                }
    
                delta = My_clip3( (((q0 - p0 ) << 2) + (p1 - q1) + 4) >> 3, -tc, tc );
                pix[-1] = clip_uint8( p0 + delta );    /* p0' */
                pix[ 0] = clip_uint8( q0 - delta );    /* q0' */
            }
            pix += ystride;
        }
    }
}
static inline void deblock_luma_intra_MBAFF_c( Fw8u *pix, Fw32s ystride, Fw32s alpha, Fw32s beta )
{
    int d;
    for( d = 0; d < 8; d++ ) {
        const int p2 = pix[-3];
        const int p1 = pix[-2];
        const int p0 = pix[-1];
        const int q0 = pix[ 0];
        const int q1 = pix[ 1];
        const int q2 = pix[ 2];

        if( abs( p0 - q0 ) < alpha &&
            abs( p1 - p0 ) < beta &&
            abs( q1 - q0 ) < beta ) {

            if(abs( p0 - q0 ) < ((alpha >> 2) + 2) ){
                if( abs( p2 - p0 ) < beta)
                {
                    const int p3 = pix[-4];
                    /* p0', p1', p2' */
                    pix[-1] = (Fw8u)(( p2 + 2*p1 + 2*p0 + 2*q0 + q1 + 4 ) >> 3);
                    pix[-2] = (Fw8u)(( p2 + p1 + p0 + q0 + 2 ) >> 2);
                    pix[-3] = (Fw8u)(( 2*p3 + 3*p2 + p1 + p0 + q0 + 4 ) >> 3);
                } else {
                    /* p0' */
                    pix[-1] = (Fw8u)(( 2*p1 + p0 + q1 + 2 ) >> 2);
                }
                if( abs( q2 - q0 ) < beta)
                {
                    const int q3 = pix[3];
                    /* q0', q1', q2' */
                    pix[0] = (Fw8u)(( p1 + 2*p0 + 2*q0 + 2*q1 + q2 + 4 ) >> 3);
                    pix[1] = (Fw8u)(( p0 + q0 + q1 + q2 + 2 ) >> 2);
                    pix[2] = (Fw8u)(( 2*q3 + 3*q2 + q1 + q0 + p0 + 4 ) >> 3);
                } else {
                    /* q0' */
                    pix[0] = (Fw8u)(( 2*q1 + q0 + p1 + 2 ) >> 2);
                }
            }else{
                /* p0', q0' */
                pix[-1] = (Fw8u)((Fw8u)(( 2*p1 + p0 + q1 + 2 ) >> 2));
                pix[ 0] = (Fw8u)((Fw8u)(( 2*q1 + q0 + p1 + 2 ) >> 2));
            }
        }
        pix += ystride;
    }
}

#if 0
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingLuma_VerEdge_MBAFF_H264_8u_C1IR)( 
		Fw8u *pSrcDst, Fw32s srcdstStep, 
		Fw32u nAlpha, 	Fw32u nBeta, Fw8u *pThresholds, Fw8u *pBS)
{
   if (pSrcDst == NULL) return fwStsNullPtrErr;
  Fw32s   xpel;
  Fw32s   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
  Fw32s   PelNum;
	int Dispatch_Type = Dispatch::Type<DT_SSE2>();

	PelNum = 16 ;	// for LUM
	for(SrcPtr=pSrcDst, xpel=0; xpel<PelNum; xpel+=4, SrcPtr+=4)
	{
		Alpha=nAlpha;
		Beta =nBeta;
	    if( pBS[0] < 4 ) {	// INTER filtering
			Fw8s tc0[2]; 
			Fw32s i; 
	        for(i=0; i<2; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;

			if( Dispatch_Type == DT_SSE2)
				My_deblock_h_luma_MBAFF_sse2( SrcPtr, srcdstStep, Alpha, Beta, tc0);
			else
				deblock_luma_MBAFF_c( SrcPtr, srcdstStep, Alpha, Beta, tc0);

		} else {			// INTRA strong filtering
			deblock_luma_intra_MBAFF_c( SrcPtr, srcdstStep, Alpha, Beta );
		}
		pThresholds+=2;
		pBS+=2;
	}
  return fwStsNoErr;
}

static inline void deblock_chroma_MBAFF_c( Fw8u *pix, Fw32s ystride,
									Fw32s alpha, Fw32s beta, Fw8s *tc0 )
{
    int i, d;
    for( i = 0; i < 2; i++ ) {
        const int tc = tc0[i];
        if( tc <= 0 ) {
            pix += 2*ystride;
            continue;
        }
        for( d = 0; d < 2; d++ ) {
            const int p1 = pix[-2];
            const int p0 = pix[-1];
            const int q0 = pix[ 0];
            const int q1 = pix[ 1];

            if( abs( p0 - q0 ) < alpha &&
                abs( p1 - p0 ) < beta &&
                abs( q1 - q0 ) < beta ) {

                int delta = My_clip3( (((q0 - p0 ) << 2) + (p1 - q1) + 4) >> 3, -tc, tc );
                pix[-1] = clip_uint8( p0 + delta );    /* p0' */
                pix[ 0] = clip_uint8( q0 - delta );    /* q0' */
            }
            pix += ystride;
        }
    }
}
static inline void deblock_chroma_intra_MBAFF_c( Fw8u *pix, Fw32s ystride, Fw32s alpha, Fw32s beta )
{   
    int d; 
    for( d = 0; d < 4; d++ ) {
        const int p1 = pix[-2];
        const int p0 = pix[-1];
        const int q0 = pix[ 0];
        const int q1 = pix[ 1];

        if( abs( p0 - q0 ) < alpha &&
            abs( p1 - p0 ) < beta &&
            abs( q1 - q0 ) < beta ) {

            pix[-1] = (Fw8u)((2*p1 + p0 + q1 + 2) >> 2);   /* p0' */
            pix[ 0] = (Fw8u)((2*q1 + q0 + p1 + 2) >> 2);   /* q0' */
        }

        pix += ystride;
    }
}
FwStatus PREFIX_OPT(OPT_PREFIX, fwiFilterDeblockingChroma_VerEdge_MBAFF_H264_8u_C1IR)( Fw8u* pSrcDst, 
		Fw32s srcdstStep, Fw32u nAlpha, Fw32u nBeta, Fw8u* pThresholds, Fw8u* pBS)
{
   if (pSrcDst == NULL)
		return fwStsNullPtrErr;

  int   xpel;
  int   Alpha=0, Beta=0;
  Fw8u *SrcPtr;
  int   PelNum;

	PelNum = 4 ;	// for Chroma
	for(SrcPtr=pSrcDst, xpel=0; xpel<PelNum; xpel+=4, SrcPtr+=4)
	{
		Alpha=nAlpha;
		Beta =nBeta;
	    if( pBS[0] < 4 ) {	// INTER filtering
			Fw8s tc0[2]; 
			Fw32s i; 
	        for(i=0; i<2; i++) tc0[i] = pBS[i] ? pThresholds[i] : -1;
			deblock_chroma_MBAFF_c( SrcPtr, srcdstStep, Alpha, Beta, tc0);
		} else {			// INTRA strong filtering
			deblock_chroma_intra_MBAFF_c( SrcPtr, srcdstStep, Alpha, Beta );
		}
		pThresholds+=4;
		pBS+=4;
	}

  return fwStsNoErr;
}
#endif

#endif

// Please do NOT remove the above line for CPP files that need to be multipass compiled
// OREFR OSSE2 
