/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FW_REF_EMMINTRIN_H__
#define __FW_REF_EMMINTRIN_H__

/* Dummy emmintrin.h file */

#include "fw_ref_mmdefs.h"

static inline __m128d
_mm_set_sd (double __F)
{
  FwInterrupt3();
  __F;
  RZERO_m128d;
}

static inline __m128d
_mm_set1_pd (double __F)
{
  FwInterrupt3();
  __F;
  RZERO_m128d;
}

static inline __m128d
_mm_set_pd1 (double __F)
{
  FwInterrupt3();
  __F;
  RZERO_m128d;
}

static inline __m128d
_mm_set_pd (double __W, double __X)
{
  FwInterrupt3();
  __W, __X;
  RZERO_m128d;
}

static inline __m128d
_mm_setr_pd (double __W, double __X)
{
  FwInterrupt3();
  __W, __X;
  RZERO_m128d;
}

static inline __m128d
_mm_setzero_pd (void)
{
  FwInterrupt3();
  RZERO_m128d;
}

static inline __m128d
_mm_move_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_load_pd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_loadu_pd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_load1_pd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_load_sd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_load_pd1 (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_loadr_pd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline void
_mm_store_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_storeu_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_store_sd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_storel_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_storeh_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_store1_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_store_pd1 (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline void
_mm_storer_pd (double *__P, __m128d __A)
{
  FwInterrupt3();
  __P, __A;
}

static inline int
_mm_cvtsi128_si32 (__m128i __A)
{
  FwInterrupt3();
  __A;
  return 0;
}

static inline __m128d
_mm_add_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_add_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_sub_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_sub_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_mul_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_mul_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_div_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_div_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_sqrt_pd (__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

static inline __m128d
_mm_sqrt_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_min_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_min_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_max_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_max_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_and_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d; 
}

static inline __m128d
_mm_andnot_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_or_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_xor_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;  
}

static inline __m128d
_mm_cmpeq_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;  
}

static inline __m128d
_mm_cmplt_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmple_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpgt_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpge_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpneq_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnlt_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnle_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpngt_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnge_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpeq_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmplt_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmple_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpgt_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpge_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpneq_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnlt_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnle_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpngt_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cmpnge_sd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128i
_mm_set_epi64 (__m64 __q1,  __m64 __q0)
{
  FwInterrupt3();
  __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_set_epi32 (int __q3, int __q2, int __q1, int __q0)
{
  FwInterrupt3();
  __q3, __q2, __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_set_epi16 (short __q7, short __q6, short __q5, short __q4,
	       short __q3, short __q2, short __q1, short __q0)
{
  FwInterrupt3();
  __q7, __q6, __q5, __q4, __q3, __q2, __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_set_epi8 (char __q15, char __q14, char __q13, char __q12,
	      char __q11, char __q10, char __q09, char __q08,
	      char __q07, char __q06, char __q05, char __q04,
	      char __q03, char __q02, char __q01, char __q00)
{
  FwInterrupt3();
  __q15, __q14, __q13, __q12, __q11, __q10, __q09, __q08;
  __q07, __q06, __q05, __q04, __q03, __q02, __q01, __q00;
  RZERO_m128i;
}

static inline __m128i
_mm_set1_epi64 (__m64 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_set1_epi32 (int __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_set1_epi16 (short __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_set1_epi8 (char __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_setr_epi64 (__m64 __q0, __m64 __q1)
{
  FwInterrupt3();
  __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_setr_epi32 (int __q0, int __q1, int __q2, int __q3)
{
  FwInterrupt3();
  __q3, __q2, __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_setr_epi16 (short __q0, short __q1, short __q2, short __q3,
	        short __q4, short __q5, short __q6, short __q7)
{
  FwInterrupt3();
  __q7, __q6, __q5, __q4, __q3, __q2, __q1, __q0;
  RZERO_m128i;
}

static inline __m128i
_mm_setr_epi8 (char __q00, char __q01, char __q02, char __q03,
	       char __q04, char __q05, char __q06, char __q07,
	       char __q08, char __q09, char __q10, char __q11,
	       char __q12, char __q13, char __q14, char __q15)
{
  FwInterrupt3();
  __q15, __q14, __q13, __q12, __q11, __q10, __q09, __q08;
  __q07, __q06, __q05, __q04, __q03, __q02, __q01, __q00;
  RZERO_m128i;
}

static inline __m128i
_mm_load_si128 (__m128i const *__P)
{
  FwInterrupt3();
  __P; 
  RZERO_m128i;
}

static inline __m128i
_mm_loadu_si128 (__m128i const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128i;
}

static inline __m128i
_mm_loadl_epi64 (__m128i const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128i;
}

static inline void
_mm_store_si128 (__m128i *__P, __m128i __B)
{
  FwInterrupt3();
  __P, __B;
}

static inline void
_mm_storeu_si128 (__m128i *__P, __m128i __B)
{
  FwInterrupt3();
  __P, __B;
}

static inline void
_mm_storel_epi64 (__m128i *__P, __m128i __B)
{
  FwInterrupt3();
  __P, __B;
}

static inline __m128i
_mm_movpi64_epi64 (__m64 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_move_epi64 (__m128i __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_setzero_si128 (void)
{
  FwInterrupt3();
  RZERO_m128i;
}

static inline __m128d
_mm_cvtepi32_pd (__m128i __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

static inline __m128
_mm_cvtepi32_ps (__m128i __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128;
}

static inline __m128i
_mm_cvtpd_epi32 (__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128
_mm_cvtpd_ps (__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128;
}

static inline __m128i
_mm_cvttpd_epi32 (__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128d
_mm_cvtpi32_pd (__m64 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

static inline __m128i
_mm_cvtps_epi32 (__m128 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128i
_mm_cvttps_epi32 (__m128 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128d
_mm_cvtps_pd (__m128 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

static inline int
_mm_cvtsd_si32 (__m128d __A)
{
  FwInterrupt3();
  __A;
  return 0;
}

static inline int
_mm_cvttsd_si32 (__m128d __A)
{
  FwInterrupt3();
  __A;
  return 0;
}

static inline __m128
_mm_cvtsd_ss (__m128 __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128;
}

static inline __m128d
_mm_cvtsi32_sd (__m128d __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_cvtss_sd (__m128d __A, __m128 __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_shuffle_pd(__m128d __A, __m128d __B, int __C)
{
  FwInterrupt3();
  __A, __B, __C;
  RZERO_m128d;
}

static inline __m128d
_mm_unpackhi_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_unpacklo_pd (__m128d __A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_loadh_pd (__m128d __A, double const *__B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline __m128d
_mm_loadl_pd (__m128d __A, double const *__B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128d;
}

static inline int
_mm_movemask_pd (__m128d __A)
{
  FwInterrupt3();
  __A;
  return 0;
}

static inline __m128i
_mm_packs_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_packs_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_packus_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpackhi_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpackhi_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpackhi_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpackhi_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpacklo_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpacklo_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpacklo_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_unpacklo_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_add_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_add_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_add_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_add_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_adds_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_adds_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_adds_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_adds_epu16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sub_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sub_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sub_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sub_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_subs_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_subs_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_subs_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_subs_epu16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_madd_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_mulhi_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_mullo_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_mul_epu32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_slli_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_slli_epi32 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_slli_epi64 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srai_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srai_epi32 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srli_si128(__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_slli_si128(__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srli_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srli_epi32 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srli_epi64 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sll_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sll_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sll_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sra_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sra_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srl_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srl_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_srl_epi64 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_and_si128 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_andnot_si128 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_or_si128 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_xor_si128 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpeq_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpeq_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpeq_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmplt_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmplt_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmplt_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpgt_epi8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpgt_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_cmpgt_epi32 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline int
_mm_extract_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  return 0;
}

static inline __m128i
_mm_insert_epi16 (__m128i __A, int __B, int __C)
{
  FwInterrupt3();
  __A, __B, __C;
  RZERO_m128i;
}

static inline __m128i
_mm_max_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_max_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_min_epi16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_min_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline int
_mm_movemask_epi8 (__m128i __A)
{
  FwInterrupt3();
  __A;
  return 0;
}

static inline __m128i
_mm_mulhi_epu16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_shufflehi_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_shufflelo_epi16 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_shuffle_epi32 (__m128i __A, int __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline void
_mm_maskmoveu_si128 (__m128i __A, __m128i __B, char *__C)
{
  FwInterrupt3();
  __A, __B, __C;
}

static inline __m128i
_mm_avg_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_avg_epu16 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline __m128i
_mm_sad_epu8 (__m128i __A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
  RZERO_m128i;
}

static inline void
_mm_stream_si32 (int *__A, int __B)
{
  FwInterrupt3();
  __A, __B;
}

static inline void
_mm_stream_si128 (__m128i *__A, __m128i __B)
{
  FwInterrupt3();
  __A, __B;
}

static inline void
_mm_stream_pd (double *__A, __m128d __B)
{
  FwInterrupt3();
  __A, __B;
}

#if !defined (WIN64)

static inline void
_mm_clflush (void const *__A)
{
  FwInterrupt3();
  __A;
}

static inline void
_mm_lfence (void)
{
  FwInterrupt3();
}

static inline void
_mm_mfence (void)
{
  FwInterrupt3();
}

#endif

static inline __m128i
_mm_cvtsi32_si128 (int __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128
_mm_castpd_ps(__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128;  
}

static inline __m128i
_mm_castpd_si128(__m128d __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128d
_mm_castps_pd(__m128 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

static inline __m128i
_mm_castps_si128(__m128 __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128i;
}

static inline __m128
_mm_castsi128_ps(__m128i __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128;
}

static inline __m128d
_mm_castsi128_pd(__m128i __A)
{
  FwInterrupt3();
  __A;
  RZERO_m128d;
}

#endif // __FW_REF_EMMINTRIN_H__

