/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __THRESHOLD_IMG_IMP__
#define __THRESHOLD_IMG_IMP__ 

#include "fwdev.h"
#include "FwSharedCode_SSE2.h"

namespace OPT_LEVEL
{

template< class TS, class TD>
SYS_INLINE STATIC void Threshold_1_lt(const TS * s, CH, TD * d, CH,TS level) 
{
	*d = (*s <  level) ? level : *s; 
}

template< class TS, class TD>
SYS_INLINE STATIC void Threshold_1_gt(const TS * s, CH, TD * d, CH,TS level) 
{
	*d = (*s >  level) ? level : *s; 
}

template< class TD>
SYS_INLINE STATIC void Threshold_1_lt(TD * d, CH,TD level) 
{
	*d = (*d <  level) ? level : *d; 
}

template<class TD>
SYS_INLINE STATIC void Threshold_1_gt(TD * d, CH,TD level) 
{
	*d = (*d >  level) ? level : *d; 
}

SYS_INLINE STATIC void Th_setup_8u_c1(__m128i &mlevel,Fw8u level)
{
	mlevel = _mm_set1_epi8(level);
}

SYS_INLINE STATIC __m128i Th_sse2_8u_c1_lt(const __m128i &src,const __m128i &level)
{
	return  _mm_max_epu8(src,level);
}

SYS_INLINE STATIC __m128i Th_sse2_8u_c1_gt(const __m128i &src,const __m128i &level)
{
	return  _mm_min_epu8(src,level);
}


SYS_INLINE STATIC void Th_setup_16s_c1(__m128i &mlevel,Fw16s level)
{
	mlevel = _mm_set1_epi16(level);
}

SYS_INLINE STATIC __m128i Th_sse2_16s_c1_lt(const __m128i &src,const __m128i &level)
{
	return  _mm_max_epi16(src,level);
}

SYS_INLINE STATIC __m128i Th_sse2_16s_c1_gt(const __m128i &src,const __m128i &level)
{
	return  _mm_min_epi16(src,level);
}


SYS_INLINE STATIC void Th_setup_32f_c1(__m128 &mlevel,Fw32f level)
{
	mlevel = _mm_set1_ps(level);
}

SYS_INLINE STATIC __m128 Th_sse2_32f_c1_lt(const __m128 &src,const __m128 &level)
{
	return  _mm_max_ps(src,level);
}

SYS_INLINE STATIC __m128 Th_sse2_32f_c1_gt(const __m128 &src,const __m128 &level)
{
	return  _mm_min_ps(src,level);
}


template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_lt(const TS * s,TD * d,const TS level1,const TS level2,const TS level3)
{
	// code valid for C3 and AC4 only
	d[0] = (s[0] <  level1) ? level1: s[0]; 
	d[1] = (s[1] <  level2) ? level2: s[1]; 
	d[2] = (s[2] <  level3) ? level3: s[2]; 
}

template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_gt(const TS * s,TD * d,const TS level1,const TS level2,const TS level3)
{
	// code valid for C3 and AC4 only
	d[0] = (s[0] >  level1) ? level1: s[0]; 
	d[1] = (s[1] >  level2) ? level2: s[1]; 
	d[2] = (s[2] >  level3) ? level3: s[2]; 
}


template<class TD > 
SYS_INLINE STATIC void Threshold_lt(TD * d,const TD level1,const TD level2,const TD level3)
{
	// code valid for C3 and AC4 only
	d[0] = (d[0] <  level1) ? level1: d[0]; 
	d[1] = (d[1] <  level2) ? level2: d[1]; 
	d[2] = (d[2] <  level3) ? level3: d[2]; 
}

template<class TD > 
SYS_INLINE STATIC void Threshold_gt(TD * d,const TD level1,const TD level2,const TD level3)
{
	// code valid for C3 and AC4 only
	d[0] = (d[0] >  level1) ? level1: d[0]; 
	d[1] = (d[1] >  level2) ? level2: d[1]; 
	d[2] = (d[2] >  level3) ? level3: d[2]; 
}

SYS_INLINE STATIC void Th_setup_8u_c3(__m128i &mLevel1,__m128i &mLevel2,__m128i &mLevel3,const Fw8u &t0,const Fw8u &t1,const Fw8u &t2)
{
	mLevel3 = _mm_set_epi8(t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2);
	mLevel2 = _mm_set_epi8(t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1);
	mLevel1 = _mm_set_epi8(t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0,t2,t1,t0);
}

SYS_INLINE STATIC void Th_sse2_8u_c3_lt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										__m128i &dst1,__m128i &dst2,__m128i &dst3,
										const __m128i &mLevel1,const __m128i &mLevel2,const __m128i &mLevel3)
{
	dst3 = _mm_max_epu8(src3, mLevel3);
	dst2 = _mm_max_epu8(src2, mLevel2);
	dst1 = _mm_max_epu8(src1, mLevel1);
}

SYS_INLINE STATIC void Th_sse2_8u_c3_gt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										__m128i &dst1,__m128i &dst2,__m128i &dst3,
										const __m128i &mLevel1,const __m128i &mLevel2,const __m128i &mLevel3)
{
	dst3 = _mm_min_epu8(src3, mLevel3);
	dst2 = _mm_min_epu8(src2, mLevel2);
	dst1 = _mm_min_epu8(src1, mLevel1);
}




SYS_INLINE STATIC void Th_setup_16s_c3(__m128i &mLevel1,__m128i &mLevel2,__m128i &mLevel3,const Fw16s &t0,const Fw16s &t1,const Fw16s &t2)
{
	mLevel3  = _mm_set_epi16(t2,t1,t0,t2,t1,t0,t2,t1);
	mLevel2  = _mm_set_epi16(t0,t2,t1,t0,t2,t1,t0,t2);
	mLevel1  = _mm_set_epi16(t1,t0,t2,t1,t0,t2,t1,t0);
}

SYS_INLINE STATIC void Th_sse2_16s_c3_lt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										__m128i &dst1,__m128i &dst2,__m128i &dst3,
										const __m128i &mLevel1,const __m128i &mLevel2,const __m128i &mLevel3)
{
	dst3 = _mm_max_epi16(src3, mLevel3);
	dst2 = _mm_max_epi16(src2, mLevel2);
	dst1 = _mm_max_epi16(src1, mLevel1);
}

SYS_INLINE STATIC void Th_sse2_16s_c3_gt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										__m128i &dst1,__m128i &dst2,__m128i &dst3,
										const __m128i &mLevel1,const __m128i &mLevel2,const __m128i &mLevel3)
{
	dst3 = _mm_min_epi16(src3, mLevel3);
	dst2 = _mm_min_epi16(src2, mLevel2);
	dst1 = _mm_min_epi16(src1, mLevel1);
}


SYS_INLINE STATIC void Th_setup_32f_c3(__m128 &mLevel1,__m128 &mLevel2,__m128 &mLevel3,const Fw32f &t0,const Fw32f &t1,const Fw32f &t2)
{
	mLevel3 = _mm_set_ps(t2,t1,t0,t2);
	mLevel2 = _mm_set_ps(t1,t0,t2,t1);
	mLevel1 = _mm_set_ps(t0,t2,t1,t0);
}

SYS_INLINE STATIC void Th_sse2_32f_c3_lt(const __m128 &src1,const __m128 &src2,const __m128 &src3,
										__m128 &dst1,__m128 &dst2,__m128 &dst3,
										const __m128 &mLevel1,const __m128 &mLevel2,const __m128 &mLevel3)
{
	dst3 = _mm_max_ps(src3, mLevel3);
	dst2 = _mm_max_ps(src2, mLevel2);
	dst1 = _mm_max_ps(src1, mLevel1);
}

SYS_INLINE STATIC void Th_sse2_32f_c3_gt(const __m128 &src1,const __m128 &src2,const __m128 &src3,
										__m128 &dst1,__m128 &dst2,__m128 &dst3,
										const __m128 &mLevel1,const __m128 &mLevel2,const __m128 &mLevel3)
{
	dst3 = _mm_min_ps(src3, mLevel3);
	dst2 = _mm_min_ps(src2, mLevel2);
	dst1 = _mm_min_ps(src1, mLevel1);
}


SYS_INLINE STATIC void Th_setup_8u_ac4(__m128i &mask,__m128i &mLevel,const Fw8u &t0,const Fw8u &t1,const Fw8u &t2)
{
	mLevel = _mm_set_epi8(t0,t2,t1,t0,t0,t2,t1,t0,t0,t2,t1,t0,t0,t2,t1,t0);
	mask = _mm_set1_epi32((S32)0x00FFFFFF);
}

SYS_INLINE STATIC __m128i Th_sse2_8u_ac4_lt(const __m128i &src,const __m128i &dest,const __m128i &level,const __m128i &mask)
{
	__m128i tmp = _mm_max_epu8(src, level);
	return _mm_or_si128( _mm_and_si128(mask, tmp), _mm_andnot_si128(mask, dest) );
}
SYS_INLINE STATIC __m128i Th_sse2_8u_ac4_gt(const __m128i &src,const __m128i &dest,const __m128i &level,const __m128i &mask)
{
	__m128i tmp = _mm_min_epu8(src, level);
	return _mm_or_si128( _mm_and_si128(mask, tmp), _mm_andnot_si128(mask, dest) );
}

SYS_INLINE STATIC void Th_setup_16s_ac4(__m128i &mask,__m128i &mLevel,const Fw16s &t0,const Fw16s &t1,const Fw16s &t2)
{
	mLevel =_mm_set_epi16(t0,t2,t1,t0,t0,t2,t1,t0); 
	mask =  _mm_set_epi32((S32)0x0000FFFF,(S32)0xFFFFFFFF,(S32)0x0000FFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC __m128i Th_sse2_16s_ac4_lt(const __m128i &src,const __m128i &dest,const __m128i &level,const __m128i &mask)
{
	__m128i tmp = _mm_max_epi16(src, level);
	return _mm_or_si128( _mm_and_si128(mask, tmp), _mm_andnot_si128(mask, dest) );
}

SYS_INLINE STATIC __m128i Th_sse2_16s_ac4_gt(const __m128i &src,const __m128i &dest,const __m128i &level,const __m128i &mask)
{
	__m128i tmp = _mm_min_epi16(src, level);
	return _mm_or_si128( _mm_and_si128(mask, tmp), _mm_andnot_si128(mask, dest) );
}

SYS_INLINE STATIC void Th_setup_32f_ac4(__m128i &maski,__m128 &mLevel,const Fw32f &t0,const Fw32f &t1,const Fw32f &t2)
{
	mLevel = _mm_set_ps(t0,t2,t1,t0);
	maski = _mm_set_epi32((S32)0x00000000,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC __m128 Th_sse2_32f_ac4_lt(const __m128 &src,const __m128 &dest,const __m128 &level,const __m128 &mask)
{
	__m128 tmp = _mm_max_ps(src, level);
	return _mm_or_ps( _mm_and_ps(mask, tmp), _mm_andnot_ps(mask, dest) );
}

SYS_INLINE STATIC __m128 Th_sse2_32f_ac4_gt(const __m128 &src,const __m128 &dest,const __m128 &level,const __m128 &mask)
{
	__m128 tmp = _mm_min_ps(src, level);
	return _mm_or_ps( _mm_and_ps(mask, tmp), _mm_andnot_ps(mask, dest) );
}


template< class TS, class TD >  
SYS_INLINE STATIC void Threshold_Val_1_lt( const TS * s,TD * d, TS level, TS value) 
{ 
	*d = (*s < level) ? value : *s; 
}
template<class TD >  
SYS_INLINE STATIC void Threshold_Val_1_lt(TD * d, TD level, TD value) 
{ 
	*d = (*d < level) ? value : *d; 
}

template< class TS, class TD >  
SYS_INLINE STATIC void Threshold_Val_1_gt( const TS * s,TD * d, TS level, TS value) 
{ 
	*d = (*s > level) ? value : *s; 
}
template<class TD >  
SYS_INLINE STATIC void Threshold_Val_1_gt(TD * d, TD level, TD value) 
{ 
	*d = (*d > level) ? value : *d; 
}




template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_Val_lt(const TS * s,TD * d,const TS *level, const TS *value)
{
	d[0] = (s[0] < level[0]) ? value[0] : s[0]; 
	d[1] = (s[1] < level[1]) ? value[1] : s[1]; 
	d[2] = (s[2] < level[2]) ? value[2] : s[2]; 
}

template<class TD > 
SYS_INLINE STATIC void Threshold_Val_lt(TD * d,const TD *level, const TD *value)
{
	d[0] = (d[0] < level[0]) ? value[0] : d[0]; 
	d[1] = (d[1] < level[1]) ? value[1] : d[1]; 
	d[2] = (d[2] < level[2]) ? value[2] : d[2]; 
}

template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_Val_gt(const TS * s,TD * d,const TS *level, const TS *value)
{
	d[0] = (s[0] > level[0]) ? value[1] : s[0]; 
	d[1] = (s[1] > level[1]) ? value[1] : s[1]; 
	d[2] = (s[2] > level[2]) ? value[2] : s[2]; 
}

template<class TD > 
SYS_INLINE STATIC void Threshold_Val_gt(TD * d,const TD *level, const TD *value)
{
	d[0] = (d[0] > level[0]) ? value[0] : d[0]; 
	d[1] = (d[1] > level[1]) ? value[1] : d[1]; 
	d[2] = (d[2] > level[2]) ? value[2] : d[2]; 
}

///// C4
template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_Val_c4_lt(const TS * s,TD * d,const TS *level, const TS *value)
{
	d[0] = (s[0] < level[0]) ? value[0] : s[0]; 
	d[1] = (s[1] < level[1]) ? value[1] : s[1]; 
	d[2] = (s[2] < level[2]) ? value[2] : s[2]; 
	d[3] = (s[3] < level[3]) ? value[3] : s[3];
}

template<class TD > 
SYS_INLINE STATIC void Threshold_Val_c4_lt(TD * d,const TD *level, const TD *value)
{
	d[0] = (d[0] < level[0]) ? value[0] : d[0]; 
	d[1] = (d[1] < level[1]) ? value[1] : d[1]; 
	d[2] = (d[2] < level[2]) ? value[2] : d[2]; 
	d[3] = (d[3] < level[3]) ? value[3] : d[3];
}

template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_Val_c4_gt(const TS * s,TD * d,const TS *level, const TS *value)
{
	d[0] = (s[0] > level[0]) ? value[0] : s[0]; 
	d[1] = (s[1] > level[1]) ? value[1] : s[1]; 
	d[2] = (s[2] > level[2]) ? value[2] : s[2]; 
	d[3] = (s[3] > level[3]) ? value[3] : s[3];
}

template<class TD > 
SYS_INLINE STATIC void Threshold_Val_c4_gt(TD * d,const TD *level, const TD *value)
{
	d[0] = (d[0] > level[0]) ? value[0] : d[0]; 
	d[1] = (d[1] > level[1]) ? value[0] : d[1]; 
	d[2] = (d[2] > level[2]) ? value[0] : d[2]; 
	d[3] = (d[3] > level[3]) ? value[3] : d[3];
}
/////


SYS_INLINE STATIC void ThVal_setup_8u_c1(__m128i &mLevel,__m128i &mValue,const Fw8u level, const Fw8u value)
{
	mLevel = _mm_set1_epi8(level);
	mValue = _mm_set1_epi8(value);
}

SYS_INLINE STATIC void ThVal_setup_16s_c1(__m128i &mLevel,__m128i &mValue,const Fw16s level, const Fw16s value)
{
	mLevel = _mm_set1_epi16(level);
	mValue = _mm_set1_epi16(value);
}

SYS_INLINE STATIC void ThVal_setup_32f_c1(__m128 &mLevel,__m128 &mValue,const Fw32f level, const Fw32f value)
{
	mLevel = _mm_set1_ps(level);
	mValue = _mm_set1_ps(value);
}

SYS_INLINE STATIC void ThVal_setup_8u_c3(__m128i &mLevel1, __m128i &mLevel2,__m128i &mLevel3,
										 __m128i &mValue1, __m128i &mValue2,__m128i &mValue3,
										 const Fw8u *t,const Fw8u *v)	
{

	mLevel3 = _mm_set_epi8(t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2]);
	mLevel2 = _mm_set_epi8(t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1]);
	mLevel1 = _mm_set_epi8(t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0]);

	mValue3 = _mm_set_epi8(v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2]);
	mValue2 = _mm_set_epi8(v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1]);
	mValue1 = _mm_set_epi8(v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0]);
}

SYS_INLINE STATIC void ThVal_setup_16s_c3(__m128i &mLevel1, __m128i &mLevel2,__m128i &mLevel3,
										 __m128i &mValue1, __m128i &mValue2,__m128i &mValue3,
										 const Fw16s *t,const Fw16s *v)	
{

	mLevel3 = _mm_set_epi16(t[2],t[1],t[0],t[2],t[1],t[0],t[2],t[1]);
	mLevel2 = _mm_set_epi16(t[0],t[2],t[1],t[0],t[2],t[1],t[0],t[2]);
	mLevel1 = _mm_set_epi16(t[1],t[0],t[2],t[1],t[0],t[2],t[1],t[0]);

	mValue3 = _mm_set_epi16(v[2],v[1],v[0],v[2],v[1],v[0],v[2],v[1]);
	mValue2 = _mm_set_epi16(v[0],v[2],v[1],v[0],v[2],v[1],v[0],v[2]);
	mValue1 = _mm_set_epi16(v[1],v[0],v[2],v[1],v[0],v[2],v[1],v[0]);
}

SYS_INLINE STATIC void ThVal_setup_32f_c3(__m128 &mLevel1, __m128 &mLevel2,__m128 &mLevel3,
										 __m128 &mValue1, __m128 &mValue2,__m128 &mValue3,
										 const Fw32f *t,const Fw32f *v)	
{

	mLevel3 = _mm_set_ps(t[2],t[1],t[0],t[2]);
	mLevel2 = _mm_set_ps(t[1],t[0],t[2],t[1]);
	mLevel1 = _mm_set_ps(t[0],t[2],t[1],t[0]);

	mValue3 = _mm_set_ps(v[2],v[1],v[0],v[2]);
	mValue2 = _mm_set_ps(v[1],v[0],v[2],v[1]);
	mValue1 = _mm_set_ps(v[0],v[2],v[1],v[0]);
}

SYS_INLINE STATIC void ThVal_setup_8u_ac4(__m128i &mLevel,__m128i &mValue,__m128i &mask, const Fw8u *t,const Fw8u *v)	
{

	mLevel = _mm_set_epi8(t[0],t[2],t[1],t[0],t[0],t[2],t[1],t[0],t[0],t[2],t[1],t[0],t[0],t[2],t[1],t[0]);
	mValue = _mm_set_epi8(v[0],v[2],v[1],v[0],v[0],v[2],v[1],v[0],v[0],v[2],v[1],v[0],v[0],v[2],v[1],v[0]);
	mask   = _mm_set1_epi32((S32)0x00FFFFFF);
}

SYS_INLINE STATIC void ThVal_setup_16s_ac4(__m128i &mLevel,__m128i &mValue,__m128i &mask, const Fw16s *t,const Fw16s *v)	
{
	mLevel = _mm_set_epi16(t[0],t[2],t[1],t[0],t[0],t[2],t[1],t[0]);
	mValue = _mm_set_epi16(v[0],v[2],v[1],v[0],v[0],v[2],v[1],v[0]);
	mask   = _mm_set_epi32((S32)0x0000FFFF,(S32)0xFFFFFFFF,(S32)0x0000FFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC void ThVal_setup_32f_ac4(__m128 &mLevel,__m128 &mValue,__m128i &mask, const Fw32f *t,const Fw32f *v)	
{
	mLevel = _mm_set_ps(t[0],t[2],t[1],t[0]);
	mValue = _mm_set_ps(v[0],v[2],v[1],v[0]);
	mask   = _mm_set_epi32((S32)0x00000000,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC __m128i ThVal_sse2_8u_c1_lt(const __m128i &src, const __m128i &level, const __m128i &value)
{

		__m128i tmp = _mm_xor_si128(_mm_cmpeq_epi8(_mm_max_epu8(src, level), level),
								_mm_cmpeq_epi8(src, level) );
		return  _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp,src));
}

SYS_INLINE STATIC __m128i ThVal_sse2_8u_c1_gt(const __m128i &src, const __m128i &level, const __m128i &value)
{
		__m128i tmp = _mm_xor_si128(_mm_cmpeq_epi8(_mm_min_epu8(src, level), level),
								_mm_cmpeq_epi8(src, level) );
		return  _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp,src));
}

SYS_INLINE STATIC __m128i ThVal_sse2_16s_c1_lt(const __m128i &src, const __m128i &level, const __m128i &value)
{
	__m128i tmp = _mm_cmplt_epi16(src, level);
	return _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp, src));
}

SYS_INLINE STATIC __m128i ThVal_sse2_16s_c1_gt(const __m128i &src, const __m128i &level, const __m128i &value)
{
	__m128i tmp = _mm_cmpgt_epi16(src, level);
	return _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp, src));
}

SYS_INLINE STATIC __m128 ThVal_sse2_32f_c1_lt(const __m128 &src, const __m128 &level, const __m128 &value)
{
	__m128 tmp = _mm_cmplt_ps(src, level);
	return _mm_or_ps(_mm_and_ps(tmp, value), _mm_andnot_ps(tmp, src));
}

SYS_INLINE STATIC __m128 ThVal_sse2_32f_c1_gt(const __m128 &src, const __m128 &level, const __m128 &value)
{
	__m128 tmp = _mm_cmpgt_ps(src, level);
	return _mm_or_ps(_mm_and_ps(tmp, value), _mm_andnot_ps(tmp, src));
}

SYS_INLINE STATIC void ThVal_sse2_8u_c3_lt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										   __m128i &dst1,__m128i &dst2,__m128i &dst3,
										   const __m128i &level1,const __m128i &level2,const __m128i &level3,
										   const __m128i &value1,const __m128i &value2,const __m128i &value3)
{
		__m128i tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src3, level3), level3),
								_mm_cmpeq_epi8(src3, level3) );
		dst3 = _mm_or_si128(_mm_and_si128(tmp, value3), _mm_andnot_si128(tmp, src3));

		tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src2, level2), level2),
								_mm_cmpeq_epi8(src2, level2) );
		dst2 = _mm_or_si128(_mm_and_si128(tmp, value2), _mm_andnot_si128(tmp, src2));

		tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src1, level1), level1),
								_mm_cmpeq_epi8(src1, level1) );
		dst1 = _mm_or_si128(_mm_and_si128(tmp, value1), _mm_andnot_si128(tmp, src1));
}


SYS_INLINE STATIC void ThVal_sse2_8u_c3_gt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										   __m128i &dst1,__m128i &dst2,__m128i &dst3,
										   const __m128i &level1,const __m128i &level2,const __m128i &level3,
										   const __m128i &value1,const __m128i &value2,const __m128i &value3)
{
		__m128i tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src3, level3), level3),
								_mm_cmpeq_epi8(src3, level3) );
		dst3 = _mm_or_si128(_mm_and_si128(tmp, value3), _mm_andnot_si128(tmp, src3));

		tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src2, level2), level2),
								_mm_cmpeq_epi8(src2, level2) );
		dst2 = _mm_or_si128(_mm_and_si128(tmp, value2), _mm_andnot_si128(tmp, src2));

		tmp	 = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src1, level1), level1),
								_mm_cmpeq_epi8(src1, level1) );
		dst1 = _mm_or_si128(_mm_and_si128(tmp, value1), _mm_andnot_si128(tmp, src1));
}

SYS_INLINE STATIC void ThVal_sse2_16s_c3_lt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										   __m128i &dst1,__m128i &dst2,__m128i &dst3,
										   const __m128i &level1,const __m128i &level2,const __m128i &level3,
										   const __m128i &value1,const __m128i &value2,const __m128i &value3)
{
			__m128i tmp = _mm_cmplt_epi16(src3, level3);
			dst3 = _mm_or_si128(_mm_and_si128(tmp, value3), _mm_andnot_si128(tmp, src3));

			tmp = _mm_cmplt_epi16(src2, level2);
			dst2 = _mm_or_si128(_mm_and_si128(tmp, value2), _mm_andnot_si128(tmp, src2));

			tmp = _mm_cmplt_epi16(src1, level1);
			dst1 = _mm_or_si128(_mm_and_si128(tmp, value1), _mm_andnot_si128(tmp, src1));
}

SYS_INLINE STATIC void ThVal_sse2_16s_c3_gt(const __m128i &src1,const __m128i &src2,const __m128i &src3,
										   __m128i &dst1,__m128i &dst2,__m128i &dst3,
										   const __m128i &level1,const __m128i &level2,const __m128i &level3,
										   const __m128i &value1,const __m128i &value2,const __m128i &value3)
{
			__m128i tmp = _mm_cmpgt_epi16(src3, level3);
			dst3 = _mm_or_si128(_mm_and_si128(tmp, value3), _mm_andnot_si128(tmp, src3));

			tmp = _mm_cmpgt_epi16(src2, level2);
			dst2 = _mm_or_si128(_mm_and_si128(tmp, value2), _mm_andnot_si128(tmp, src2));

			tmp = _mm_cmpgt_epi16(src1, level1);
			dst1 = _mm_or_si128(_mm_and_si128(tmp, value1), _mm_andnot_si128(tmp, src1));
}

SYS_INLINE STATIC void ThVal_sse2_32f_c3_lt(const __m128 &src1,const __m128 &src2,const __m128 &src3,
										   __m128 &dst1,__m128 &dst2,__m128 &dst3,
										   const __m128 &level1,const __m128 &level2,const __m128 &level3,
										   const __m128 &value1,const __m128 &value2,const __m128 &value3)
{
			__m128 tmp = _mm_cmplt_ps(src3, level3);
			dst3 = _mm_or_ps(_mm_and_ps(tmp, value3), _mm_andnot_ps(tmp, src3));

			tmp = _mm_cmplt_ps(src2, level2);
			dst2 = _mm_or_ps(_mm_and_ps(tmp, value2), _mm_andnot_ps(tmp, src2));

			tmp = _mm_cmplt_ps(src1, level1);
			dst1 = _mm_or_ps(_mm_and_ps(tmp, value1), _mm_andnot_ps(tmp, src1));
}

SYS_INLINE STATIC void ThVal_sse2_32f_c3_gt(const __m128 &src1,const __m128 &src2,const __m128 &src3,
										   __m128 &dst1,__m128 &dst2,__m128 &dst3,
										   const __m128 &level1,const __m128 &level2,const __m128 &level3,
										   const __m128 &value1,const __m128 &value2,const __m128 &value3)
{
			__m128 tmp = _mm_cmpgt_ps(src3, level3);
			dst3 = _mm_or_ps(_mm_and_ps(tmp, value3), _mm_andnot_ps(tmp, src3));

			tmp = _mm_cmpgt_ps(src2, level2);
			dst2 = _mm_or_ps(_mm_and_ps(tmp, value2), _mm_andnot_ps(tmp, src2));

			tmp = _mm_cmpgt_ps(src1, level1);
			dst1 = _mm_or_ps(_mm_and_ps(tmp, value1), _mm_andnot_ps(tmp, src1));
}

SYS_INLINE STATIC __m128i  ThVal_sse2_8u_ac4_lt(const __m128i &src, const __m128i &dst,const __m128i &level, const __m128i &value,const __m128i &mask)
{
		__m128i tmp = _mm_xor_si128(_mm_cmpeq_epi8(_mm_max_epu8(src, level), level),
								_mm_cmpeq_epi8(src, level) );
		tmp =   _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp,src));

		return  _mm_or_si128( _mm_and_si128(mask,tmp), _mm_andnot_si128(mask, dst) );
}

SYS_INLINE STATIC __m128i ThVal_sse2_8u_ac4_gt(const __m128i &src, const __m128i &dst,const __m128i &level, const __m128i &value,const __m128i &mask)
{
		__m128i tmp = _mm_xor_si128(_mm_cmpeq_epi8(_mm_min_epu8(src, level), level),
								_mm_cmpeq_epi8(src, level) );
		tmp =   _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp,src));

		return  _mm_or_si128( _mm_and_si128(mask,tmp), _mm_andnot_si128(mask, dst) );
}

SYS_INLINE STATIC __m128i ThVal_sse2_16s_ac4_lt(const __m128i &src,const __m128i &dst, const __m128i &level, const __m128i &value,const __m128i &mask)
{
	__m128i tmp = _mm_cmplt_epi16(src, level);
	tmp =  _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp, src));
		
	return  _mm_or_si128( _mm_and_si128(mask,tmp), _mm_andnot_si128(mask, dst) );
}

SYS_INLINE STATIC __m128i ThVal_sse2_16s_ac4_gt(const __m128i &src,const __m128i &dst, const __m128i &level, const __m128i &value,const __m128i &mask)
{
	__m128i tmp = _mm_cmpgt_epi16(src, level);
	tmp =  _mm_or_si128(_mm_and_si128(tmp, value), _mm_andnot_si128(tmp, src));
		
	return  _mm_or_si128( _mm_and_si128(mask,tmp), _mm_andnot_si128(mask, dst) );
}

SYS_INLINE STATIC __m128 ThVal_sse2_32f_ac4_lt(const __m128 &src,const __m128 &dst, const __m128 &level, const __m128 &value,const __m128 &mask)
{
	__m128 tmp = _mm_cmplt_ps(src, level);
	tmp =  _mm_or_ps(_mm_and_ps(tmp, value), _mm_andnot_ps(tmp, src));

	return  _mm_or_ps( _mm_and_ps(mask,tmp), _mm_andnot_ps(mask, dst) );
}

SYS_INLINE STATIC __m128 ThVal_sse2_32f_ac4_gt(const __m128 &src,const __m128 &dst, const __m128 &level, const __m128 &value,const __m128 &mask)
{
	__m128 tmp = _mm_cmpgt_ps(src, level);
	tmp =  _mm_or_ps(_mm_and_ps(tmp, value), _mm_andnot_ps(tmp, src));

	return  _mm_or_ps( _mm_and_ps(mask,tmp), _mm_andnot_ps(mask, dst) );
}

SYS_INLINE STATIC void ThVal_setup_8u_c4(__m128i &mLevel, __m128i &mValue,Fw8u *t,Fw8u *v)
{
	mLevel = _mm_set_epi8(t[3],t[2],t[1],t[0],t[3],t[2],t[1],t[0],t[3],t[2],t[1],t[0],t[3],t[2],t[1],t[0]);
	mValue = _mm_set_epi8(v[3],v[2],v[1],v[0],v[3],v[2],v[1],v[0],v[3],v[2],v[1],v[0],v[3],v[2],v[1],v[0]);
}

SYS_INLINE STATIC void ThVal_setup_16s_c4(__m128i &mLevel, __m128i &mValue,Fw16s *t,Fw16s *v)
{
	mLevel = _mm_set_epi16(t[3],t[2],t[1],t[0],t[3],t[2],t[1],t[0]);
	mValue = _mm_set_epi16(v[3],v[2],v[1],v[0],v[3],v[2],v[1],v[0]);
}

SYS_INLINE STATIC void ThVal_setup_32f_c4(__m128 &mLevel, __m128 &mValue,Fw32f *t,Fw32f *v)
{
	mLevel = _mm_set_ps(t[3],t[2],t[1],t[0]);
	mValue = _mm_set_ps(v[3],v[2],v[1],v[0]);
}

// 1 channel, in-place
template< class TD >  
SYS_INLINE STATIC void Threshold_LTValGTVal_1(TD * d,TD levelLt,TD valueLt,TD levelGt,TD valueGt)
{
	*d = (*d < levelLt) ? valueLt: ( (*d > levelGt) ? valueGt : *d );
}

// 1 channel, not in-place
template< class TS, class TD >  
SYS_INLINE STATIC void Threshold_LTValGTVal_1(const TS * s,TD * d,TD levelLt,TD valueLt,TD levelGt,TD valueGt)
{
	*d = (*s < levelLt) ? valueLt: ( (*s > levelGt) ? valueGt : *s );
}


// multichannel, in-place
template< class TD > 
SYS_INLINE STATIC void Threshold_LTValGTVal(TD * d,const TD *levelLt,const TD *valueLt,const TD *levelGt,const TD *valueGt)
{
	d[0] = (d[0] < levelLt[0]) ? valueLt[0]: ( (d[0] > levelGt[0]) ? valueGt[0] : d[0] );
	d[1] = (d[1] < levelLt[1]) ? valueLt[1]: ( (d[1] > levelGt[1]) ? valueGt[1] : d[1] );
	d[2] = (d[2] < levelLt[2]) ? valueLt[2]: ( (d[2] > levelGt[2]) ? valueGt[2] : d[2] );
}



// multichannel, not in-place
template< class TS, class TD > 
SYS_INLINE STATIC void Threshold_LTValGTVal(const TS * s,TD * d,const TS *levelLt,const TS *valueLt,const TS *levelGt,const TS *valueGt)
{
	// code valid for C3 and AC4 only
	d[0] = (s[0] < levelLt[0]) ? valueLt[0]: ( (s[0] > levelGt[0]) ? valueGt[0] : s[0] );
	d[1] = (s[1] < levelLt[1]) ? valueLt[1]: ( (s[1] > levelGt[1]) ? valueGt[1] : s[1] );
	d[2] = (s[2] < levelLt[2]) ? valueLt[2]: ( (s[2] > levelGt[2]) ? valueGt[2] : s[2] );
}


SYS_INLINE STATIC void ThLTGT_setup_8u_c1(__m128i &mlevelLt, __m128i &mvalueLt, __m128i &mlevelGt, __m128i &mvalueGt,
											Fw8u levelLt, Fw8u valueLt, Fw8u levelGt, Fw8u valueGt)
{
	mlevelLt = _mm_set1_epi8(levelLt);
	mvalueLt = _mm_set1_epi8(valueLt);
	mlevelGt = _mm_set1_epi8(levelGt);
	mvalueGt = _mm_set1_epi8(valueGt);
}

SYS_INLINE STATIC void ThLTGT_setup_16s_c1(__m128i &mlevelLt, __m128i &mvalueLt, __m128i &mlevelGt, __m128i &mvalueGt,
											Fw16s levelLt, Fw16s valueLt, Fw16s levelGt, Fw16s valueGt)
{
	mlevelLt = _mm_set1_epi16(levelLt);
	mvalueLt = _mm_set1_epi16(valueLt);
	mlevelGt = _mm_set1_epi16(levelGt);
	mvalueGt = _mm_set1_epi16(valueGt);
}

SYS_INLINE STATIC void ThLTGT_setup_32f_c1(__m128 &mlevelLt, __m128 &mvalueLt, __m128 &mlevelGt, __m128 &mvalueGt,
											Fw32f levelLt, Fw32f valueLt, Fw32f levelGt, Fw32f valueGt)
{
	mlevelLt = _mm_set1_ps(levelLt);
	mvalueLt = _mm_set1_ps(valueLt);
	mlevelGt = _mm_set1_ps(levelGt);
	mvalueGt = _mm_set1_ps(valueGt);
}

SYS_INLINE STATIC void ThLTGT_setup_8u_ac4(__m128i &mlevelLt, __m128i &mvalueLt, __m128i &mlevelGt, __m128i &mvalueGt,__m128i &mask,
											Fw8u *tl, Fw8u *vl, Fw8u *tg, Fw8u *vg)
{
	
	mlevelLt = _mm_set_epi8(tl[0],tl[2],tl[1],tl[0],tl[0],tl[2],tl[1],tl[0],tl[0],tl[2],tl[1],tl[0],tl[0],tl[2],tl[1],tl[0]);
	mvalueLt = _mm_set_epi8(vl[0],vl[2],vl[1],vl[0],vl[0],vl[2],vl[1],vl[0],vl[0],vl[2],vl[1],vl[0],vl[0],vl[2],vl[1],vl[0]);	
	mlevelGt = _mm_set_epi8(tg[0],tg[2],tg[1],tg[0],tg[0],tg[2],tg[1],tg[0],tg[0],tg[2],tg[1],tg[0],tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_epi8(vg[0],vg[2],vg[1],vg[0],vg[0],vg[2],vg[1],vg[0],vg[0],vg[2],vg[1],vg[0],vg[0],vg[2],vg[1],vg[0]);
	mask= _mm_set1_epi32((S32)0x00FFFFFF);
}

SYS_INLINE STATIC void ThLTGT_setup_16s_ac4(__m128i &mlevelLt, __m128i &mvalueLt, __m128i &mlevelGt, __m128i &mvalueGt,__m128i &mask,
											Fw16s *tl, Fw16s *vl, Fw16s *tg, Fw16s *vg)
{
	
	mlevelLt = _mm_set_epi16(tl[0],tl[2],tl[1],tl[0],tl[0],tl[2],tl[1],tl[0]);
	mvalueLt = _mm_set_epi16(vl[0],vl[2],vl[1],vl[0],vl[0],vl[2],vl[1],vl[0]);	
	mlevelGt = _mm_set_epi16(tg[0],tg[2],tg[1],tg[0],tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_epi16(vg[0],vg[2],vg[1],vg[0],vg[0],vg[2],vg[1],vg[0]);

	mask= _mm_set_epi32((S32)0x0000FFFF,(S32)0xFFFFFFFF,(S32)0x0000FFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC void ThLTGT_setup_32f_ac4(__m128 &mlevelLt, __m128 &mvalueLt, __m128 &mlevelGt, __m128 &mvalueGt,__m128i &mask,
											Fw32f *tl, Fw32f *vl, Fw32f *tg, Fw32f *vg)
{
	
	mlevelLt = _mm_set_ps(tl[0],tl[2],tl[1],tl[0]);
	mvalueLt = _mm_set_ps(vl[0],vl[2],vl[1],vl[0]);	
	mlevelGt = _mm_set_ps(tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_ps(vg[0],vg[2],vg[1],vg[0]);

	mask= _mm_set_epi32((S32)0x00000000,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF,(S32)0xFFFFFFFF);
}

SYS_INLINE STATIC __m128i ThLTGT_sse2_8u(const __m128i &src,const __m128i &levelLt,const __m128i &valueLt, const __m128i &levelGt, const __m128i &valueGt)
{
		__m128i tmp = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src, levelLt), levelLt),
								_mm_cmpeq_epi8(src, levelLt) );
		__m128i dst = _mm_or_si128(_mm_and_si128(tmp, valueLt), _mm_andnot_si128(tmp, src));
		
		tmp = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src, levelGt), levelGt),
								_mm_cmpeq_epi8(src, levelGt) );
		return  _mm_or_si128(_mm_and_si128(tmp, valueGt), _mm_andnot_si128(tmp, dst));
}

SYS_INLINE STATIC __m128i ThLTGT_sse2_16s(const __m128i &src,const __m128i &levelLt,const __m128i &valueLt, const __m128i &levelGt, const __m128i &valueGt)
{
		__m128i tmp = _mm_cmplt_epi16(src, levelLt);
		__m128i dst = _mm_or_si128(_mm_and_si128(tmp, valueLt), _mm_andnot_si128(tmp, src));

		tmp = _mm_cmpgt_epi16(src, levelGt);
		return  _mm_or_si128(_mm_and_si128(tmp, valueGt), _mm_andnot_si128(tmp, dst));
}


SYS_INLINE STATIC __m128 ThLTGT_sse2_32f(const __m128 &src,const __m128 &levelLt,const __m128 &valueLt, const __m128 &levelGt, const __m128 &valueGt)
{
		__m128 tmp = _mm_cmplt_ps(src, levelLt);
		__m128 dst = _mm_or_ps(_mm_and_ps(tmp, valueLt), _mm_andnot_ps(tmp, src));

		tmp = _mm_cmpgt_ps(src, levelGt);
		return  _mm_or_ps(_mm_and_ps(tmp, valueGt), _mm_andnot_ps(tmp, dst));
}


SYS_INLINE STATIC __m128i ThLTGT_sse2_8u_ac4(const __m128i &src,const __m128i &dst,const __m128i &mask, const __m128i &levelLt,const __m128i &valueLt, const __m128i &levelGt, const __m128i &valueGt)
{
		__m128i tmp = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_max_epu8(src, levelLt), levelLt),
								_mm_cmpeq_epi8(src, levelLt) );
		__m128i dst1 = _mm_or_si128(_mm_and_si128(tmp, valueLt), _mm_andnot_si128(tmp, src));
		
		tmp = _mm_xor_si128(	_mm_cmpeq_epi8(_mm_min_epu8(src, levelGt), levelGt),
								_mm_cmpeq_epi8(src, levelGt) );

		__m128i dst_tmp =   _mm_or_si128(_mm_and_si128(tmp, valueGt), _mm_andnot_si128(tmp, dst1));
		return  _mm_or_si128( _mm_and_si128(mask, dst_tmp), _mm_andnot_si128(mask,dst) );

}

SYS_INLINE STATIC __m128i ThLTGT_sse2_16s_ac4(const __m128i &src,const __m128i &dst,const __m128i &mask,const __m128i &levelLt,const __m128i &valueLt, const __m128i &levelGt, const __m128i &valueGt)
{
		__m128i tmp = _mm_cmplt_epi16(src, levelLt);
		__m128i dst1 = _mm_or_si128(_mm_and_si128(tmp, valueLt), _mm_andnot_si128(tmp, src));

		tmp = _mm_cmpgt_epi16(src, levelGt);
		__m128i dst_tmp =  _mm_or_si128(_mm_and_si128(tmp, valueGt), _mm_andnot_si128(tmp, dst1)); 
		return  _mm_or_si128( _mm_and_si128(mask, dst_tmp), _mm_andnot_si128(mask,dst) );
}


SYS_INLINE STATIC __m128 ThLTGT_sse2_32f_ac4(const __m128 &src,const __m128 &dst,const __m128 &mask,const __m128 &levelLt,const __m128 &valueLt, const __m128 &levelGt, const __m128 &valueGt)
{
		__m128 tmp = _mm_cmplt_ps(src, levelLt);
		__m128 dst1 = _mm_or_ps(_mm_and_ps(tmp, valueLt), _mm_andnot_ps(tmp, src));

		tmp = _mm_cmpgt_ps(src, levelGt);
		__m128 dst_tmp =   _mm_or_ps(_mm_and_ps(tmp, valueGt), _mm_andnot_ps(tmp, dst1));
		return  _mm_or_ps( _mm_and_ps(mask, dst_tmp), _mm_andnot_ps(mask,dst) );
}

SYS_INLINE STATIC void ThLTGT_setup_8u_c3(__m128i &mlevelLt,__m128i &mValueLt,__m128i &mlevelGt, __m128i &mvalueGt,const Fw8u *tl,const Fw8u *vl,const Fw8u *tg,const Fw8u *vg) 
{

	mlevelLt = _mm_set_epi8(tl[0],tl[2],tl[1],tl[0],tl[2],tl[1],tl[0],tl[2],tl[1],tl[0],tl[2],tl[1],tl[0],tl[2],tl[1],tl[0]);
	mValueLt = _mm_set_epi8(vl[0],vl[2],vl[1],vl[0],vl[2],vl[1],vl[0],vl[2],vl[1],vl[0],vl[2],vl[1],vl[0],vl[2],vl[1],vl[0]);
	mlevelGt = _mm_set_epi8(tg[0],tg[2],tg[1],tg[0],tg[2],tg[1],tg[0],tg[2],tg[1],tg[0],tg[2],tg[1],tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_epi8(vg[0],vg[2],vg[1],vg[0],vg[2],vg[1],vg[0],vg[2],vg[1],vg[0],vg[2],vg[1],vg[0],vg[2],vg[1],vg[0]);
}

SYS_INLINE STATIC void ThLTGT_setup_16s_c3(__m128i &mlevelLt,__m128i &mValueLt,__m128i &mlevelGt, __m128i &mvalueGt,const Fw16s *tl,const Fw16s *vl,const Fw16s *tg,const Fw16s *vg) 
{

	mlevelLt = _mm_set_epi16(tl[1],tl[0],tl[2],tl[1],tl[0],tl[2],tl[1],tl[0]);
	mValueLt = _mm_set_epi16(vl[1],vl[0],vl[2],vl[1],vl[0],vl[2],vl[1],vl[0]);
	mlevelGt = _mm_set_epi16(tg[1],tg[0],tg[2],tg[1],tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_epi16(vg[1],vg[0],vg[2],vg[1],vg[0],vg[2],vg[1],vg[0]);
}
SYS_INLINE STATIC void ThLTGT_setup_32f_c3(__m128 &mlevelLt,__m128 &mValueLt,__m128 &mlevelGt, __m128 &mvalueGt,const Fw32f *tl,const Fw32f *vl,const Fw32f *tg,const Fw32f *vg) 
{

	mlevelLt = _mm_set_ps(tl[0],tl[2],tl[1],tl[0]);
	mValueLt = _mm_set_ps(vl[0],vl[2],vl[1],vl[0]);
	mlevelGt = _mm_set_ps(tg[0],tg[2],tg[1],tg[0]);
	mvalueGt = _mm_set_ps(vg[0],vg[2],vg[1],vg[0]);
}

} // OPT_LEVEL
#endif
