/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef __FW_REF_PMMINTRIN_H__
#define __FW_REF_PMMINTRIN_H__

/* Dummy pmmintrin.h file */

#include "fw_ref_mmdefs.h"

static inline __m128
_mm_addsub_ps (__m128 __X, __m128 __Y)
{
  FwInterrupt3();
  __X, __Y; 
  RZERO_m128;
}

static inline __m128
_mm_hadd_ps (__m128 __X, __m128 __Y)
{
  FwInterrupt3();
  __X, __Y;
  RZERO_m128;
}

static inline __m128
_mm_hsub_ps (__m128 __X, __m128 __Y)
{
  FwInterrupt3();
  __X, __Y;
  RZERO_m128;
}

static inline __m128
_mm_movehdup_ps (__m128 __X)
{
  FwInterrupt3();
  __X;
  RZERO_m128;
}

static inline __m128
_mm_moveldup_ps (__m128 __X)
{
  FwInterrupt3();
  __X;
  RZERO_m128;
}

static inline __m128d
_mm_addsub_pd (__m128d __X, __m128d __Y)
{
  FwInterrupt3();
  __X, __Y;
  RZERO_m128d;
}

static inline __m128d
_mm_hadd_pd (__m128d __X, __m128d __Y)
{
  FwInterrupt3();
  __X, __Y;
  RZERO_m128d;
}

static inline __m128d
_mm_hsub_pd (__m128d __X, __m128d __Y)
{
  FwInterrupt3();
  __X, __Y;
  RZERO_m128d;
}

static inline __m128d
_mm_loaddup_pd (double const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128d;
}

static inline __m128d
_mm_movedup_pd (__m128d __X)
{
  FwInterrupt3();
  __X;
  RZERO_m128d;
}

static inline __m128i
_mm_lddqu_si128 (__m128i const *__P)
{
  FwInterrupt3();
  __P;
  RZERO_m128i;
}

#endif // __FW_REF_PMMINTRIN_H__

