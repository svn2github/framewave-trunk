/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_PACKUNPACK_PRIMITIVES_SSE2_H__
#define __CBL_PACKUNPACK_PRIMITIVES_SSE2_H__

/// @cond OPT_DETAILS

/*! 
    \param[in] srcLo Two double type values
    \param[in] srcHi Two double type values
    \return Saturated four 32 bit signed values
	\par Notes:
    The function packs four double values from two parameters to four 32bit signed integers.    
    \par Input:
    \n  srcLo = (a1,a0)
    \n  srcHi = (b1,b0)
    \par Output:
    \n  return value  = (b1,b0,a1,a0)
*/

CBL_INLINE __m128i CBL_SSE2::Pack64FTo32S( const __m128d & srcLo, const __m128d & srcHi )
{
    XMM128 sl, sh;
	static const __m128d max_val = CONST_SET1_64F((F64)CBL_S32_MAX);
	static const __m128d min_val = CONST_SET1_64F(-(F64)CBL_S32_MIN);

	sl.d		= _mm_min_pd(srcLo, max_val);
	sl.d		= _mm_max_pd(sl.d, min_val);

	sh.d		= _mm_min_pd(srcHi, max_val);
	sh.d		= _mm_max_pd(sh.d, min_val);

	sl.i		= _mm_cvtpd_epi32(sl.d); 
	sh.i		= _mm_cvtpd_epi32(sh.d);

	sh.i		= _mm_slli_si128(sh.i, 8); 
	sl.i		= _mm_or_si128(sl.i, sh.i);
    return sl.i;
}

/*! 
    \param[in] src Two double type values
    \return Saturated two 16 bit signed values
	\par Notes:
    The function packs two double values to 16bit signed integers.    
    \par Input:
    \n  src = (a1,a0)
    \par Output:
    \n  return value  = (x,x,x,x,x,x,a1,a0)
*/
CBL_INLINE __m128i CBL_SSE2::Pack64FTo16S( const __m128d & src )
{
	static const __m128d max_val = CONST_SET1_64F((F64)CBL_S32_MAX);
	static const __m128d min_val = CONST_SET1_64F(-(F64)CBL_S32_MIN);
    XMM128 result;

	// saturate src to min and max possible 16 bit dst values
	result.d = _mm_min_pd(src, max_val);
	result.d = _mm_max_pd(result.d, min_val);

	// convert dst to integer. nothing will overflow now
	result.i = _mm_cvtpd_epi32(result.d);
	// pack in dst to 16 bit
	result.i = _mm_packs_epi32(result.i, result.i);

    return result.i;
}


/*! 
    \param[in] src Two nonnegative double type values
    \return Saturated two 16 bit signed values
	\par Notes:
    The function packs two nonnegative double values to 16bit signed integers.    
    \par Input:
    \n  src = (a1,a0)
    \par Output:
    \n  return value  = (x,x,x,x,x,x,a1,a0)
*/
CBL_INLINE __m128i CBL_SSE2::Pack64FTo16S_nonegative( const __m128d & src )
{
	static const __m128d max_val = CONST_SET1_64F((F64)CBL_S32_MAX);
    XMM128 result;

	// saturate src to min and max possible 16 bit dst values
	result.d = _mm_min_pd(src, max_val);

	// convert dst to integer. nothing will overflow now
	result.i = _mm_cvtpd_epi32(result.d);
	// pack in dst to 16 bit
	result.i = _mm_packs_epi32(result.i, result.i);

    return result.i;
}


/*! 
    \param[in,out] srcDstLo four Unsigned 32bit values.
    \param[out] dstHi  two Unsigned 64bit values.
    \return none
	\par Notes:
    The function Unpacks four unsigned 32bit values to four unsigned 64bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a1,a0)
    \n  dstHi  = (a3,a2)
*/
CBL_INLINE void CBL_SSE2::Unpack32UTo64U( __m128i & srcDstLo, __m128i & dstHi )
{
	__m128i const zero = _mm_setzero_si128();

	dstHi    = _mm_unpackhi_epi32( srcDstLo, zero ); 
	srcDstLo = _mm_unpacklo_epi32( srcDstLo, zero ); 
}


/*! 
    \param[in] src four signed 32bit values.
    \param[out] dstLo  two double type values
    \param[out] dstHi  two double type values
    \return none
	\par Notes:
    The function Unpacks four signed 32bit values to four double type values in two registers.    
    \par Input:
    \n  src = (a3,a2,a1,a0)
    \par Output:
    \n  dstLo  = (a1,a0)
    \n  dstHi  = (a3,a2)
*/
CBL_INLINE void CBL_SSE2::Unpack32STo64F( const __m128i & src, __m128d & dstLo, __m128d & dstHi )
{
	__m128i temp = _mm_srli_si128(src, 8);

	// convert and move lower two numbers to 'low' double register
	dstLo = _mm_cvtepi32_pd(src);
	// convert and move upper two numbers to 'hi' double register
	dstHi    = _mm_cvtepi32_pd(temp);
}


/*! 
    \param[in,out] srcDstLo eight signed 16bit values.
    \param[out] dstHi  four signed 32bit values
    \return none
	\par Notes:
    The function Unpacks eight signed 16bit values to four signed 32bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a7,a6,a5,a4,a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a3,a2,a1,a0)
    \n  dstHi  = (a7,a6,a5,a4)
*/

CBL_INLINE void CBL_SSE2::Unpack16STo32S( __m128i & srcDstLo, __m128i & dstHi )
{
	__m128i signext = _mm_srai_epi16(srcDstLo, 15);

	dstHi    = _mm_unpackhi_epi16( srcDstLo, signext ); 
	srcDstLo = _mm_unpacklo_epi16( srcDstLo, signext ); 
}


/*! 
    \param[in,out] srcDstLo eight unsigned 16bit values.
    \param[out] dstHi  four unsigned 32bit values
    \return none
	\par Notes:
    The function Unpacks eight unsigned 16bit values to four unsigned 32bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a7,a6,a5,a4,a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a3,a2,a1,a0)
    \n  dstHi  = (a7,a6,a5,a4)
*/

CBL_INLINE void CBL_SSE2::Unpack16UTo32U( __m128i &srcDstLo, __m128i & dstHi )
{
	const __m128i zero =  _mm_setzero_si128();

	dstHi    = _mm_unpackhi_epi16( srcDstLo, zero ); 
	srcDstLo = _mm_unpacklo_epi16( srcDstLo, zero ); 
}


/*! 
    \param[in,out] srcDstLo sixteen unsigned 8bit values.
    \param[out] dstHi  eight unsigned 16bit values
    \return none
	\par Notes:
    The function Unpacks sixteen unsigned 8bit values to eight unsigned 16bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a7,a6,a5,a4,a3,a2,a1,a0)
    \n  dstHi  = (a15,a14,a13,a12,a11,a10,a9)
*/
CBL_INLINE void CBL_SSE2::Unpack8UTo16U( __m128i & srcDstLo, __m128i & dstHi )
{
	const __m128i zero =  _mm_setzero_si128();

	dstHi    = _mm_unpackhi_epi8( srcDstLo, zero );
	srcDstLo = _mm_unpacklo_epi8( srcDstLo, zero );
}

/*! 
    \param[in,out] srcDstLo sixteen signed 8bit values.
    \param[out] dstHi  eight signed 16bit values
    \return none
	\par Notes:
    The function Unpacks sixteen signed 8bit values to eight signed 16bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a7,a6,a5,a4,a3,a2,a1,a0)
    \n  dstHi  = (a15,a14,a13,a12,a11,a10,a9)
*/
CBL_INLINE void CBL_SSE2::Unpack8STo16S( __m128i & srcDstLo, __m128i & dstHi )
{
		static const __m128i zero = CONST_SETZERO_8I();
		
		__m128i sign = _mm_cmplt_epi8(srcDstLo,zero);

		dstHi    = _mm_unpackhi_epi8( srcDstLo, sign );
		srcDstLo = _mm_unpacklo_epi8( srcDstLo, sign );
}


/*! 
    \param[in,out] srcDstLo sixteen unsigned 8bit values.
    \param[out] dstHi  eight signed 16bit values
    \return none
	\par Notes:
    The function Unpacks sixteen unsigned 8bit values to eight signed 16bit values in two registers.    
    \par Input:
    \n  srcDstLo = (a15,a14,a13,a12,a11,a10,a9,a8,a7,a6,a5,a4,a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo  = (a7,a6,a5,a4,a3,a2,a1,a0)
    \n  dstHi  = (a15,a14,a13,a12,a11,a10,a9)
*/
CBL_INLINE  void CBL_SSE2::Unpack8UTo16S( __m128i &srcDstLo, __m128i &dstHi )
{
	 const __m128i zero =  CONST_SETZERO_8I();

	dstHi    = _mm_unpackhi_epi8( srcDstLo, zero );
	srcDstLo = _mm_unpacklo_epi8( srcDstLo, zero );
}


/*! 
    \param[in] srcLo two double type values
    \param[in] src  four 32bit signed values
    \return four 32bit signed values in a register
	\par Notes:
    The function Packs two double and two 32bit signed values to four 32bit signed values    
    \par Input:
    \n  srcLo = (a1,a0)
    \n  src = (b3,b2,b1,b0)
    \par Output:
    \n  return value = (b3,b2,a1,a0)
*/
CBL_INLINE  __m128i CBL_SSE2::Pack64FTo32SLo( const __m128d &srcLo, const __m128i &src )
{
	 const static __m128d max_val = CONST_SET1_64F((double)0x7FFFFFFF);
	 const static __m128d min_val = CONST_SET1_64F(-(double)0x80000000);

	__m128d	lo	= _mm_min_pd(srcLo, max_val);
			lo	= _mm_max_pd(lo, min_val);

// TODO: FIX THIS, workaround for apple compiler bug
#ifdef __APPLE__
    XMM128 	Lo;
	Lo.i= _mm_cvtpd_epi32(lo); 
	return _mm_or_si128(Lo.i, _mm_slli_si128(_mm_srli_si128(src, 8), 8));
#else
	XMM128 	Lo,s1;
	Lo.i= _mm_cvtpd_epi32(lo); 
	s1.i = src;
	Lo.d = _mm_move_sd(s1.d,Lo.d);
	return Lo.i;
#endif
}


/*! 
    \param[in] src1 four float type values
    \param[in] src2 four float type values 
    \return eight 16bit signed values in a register
	\par Notes:
    The function Packs eight float type values to 16bit signed values.
    \par Input:
    \n  src1 = (a3,a2,a1,a0)
    \n  src2 = (b3,b2,b1,b0)
    \par Output:
    \n  return value = (b3,b2,b1,b0,a3,a2,a1,a0)
*/
CBL_INLINE  __m128i CBL_SSE2::Pack32FTo16S(const __m128 &src1,const __m128 &src2)
{
	XMM128 s1,s2;
	
	 const static __m128 f_val		= CONST_SET1_32F((float)0x7FFF);
	 const static __m128 f_min_val	= CONST_SET1_32F(-(float)0x8000);

	s1.f	= _mm_min_ps(src1,f_val);
	s1.f	= _mm_max_ps(s1.f,f_min_val);
	s1.i		=  _mm_cvtps_epi32 (s1.f);

	s2.f	= _mm_min_ps(src2,f_val);
	s2.f	= _mm_max_ps(s2.f,f_min_val);
	s2.i	=  _mm_cvtps_epi32(s2.f);

	return  _mm_packs_epi32(s1.i, s2.i);

}


/*! 
    \param[in,out] srcDstLo four 32bit signed values
    \param[out] dstHi four 32bit signed values
    \return none
	\par Notes:
    The function Unpacks four 32bit signed values to 64bit signed values in two registers.
    \par Input:
    \n  srcDstLo = (a3,a2,a1,a0)
    \par Output:
    \n  srcDstLo = (a1,a0)
    \n  dstHi = (a3,a2)
*/
CBL_INLINE  void CBL_SSE2::Unpack32STo64S( __m128i &srcDstLo, __m128i &dstHi )
{
	__m128i signext = _mm_srai_epi32(srcDstLo, 31);

	dstHi    = _mm_unpackhi_epi32( srcDstLo, signext ); 
	srcDstLo = _mm_unpacklo_epi32( srcDstLo, signext ); 	
}


/*! 
    \param[in] srcLo two 64bit signed values
    \param[in] srcHi two 64bit signed values
    \return four 32bit signed integers
	\par Notes:
    The function packs four 64bit signed values to 32bit signed values.
    \par Input:
    \n  srcLo = (a1,a0)
    \n  srcHi = (b1,b0)
    \par Output:
    \n  return value = (b1,b0,a1,a0)
*/
CBL_INLINE  __m128i CBL_SSE2::Pack64STo32S( const __m128i &srcLo, const __m128i &srcHi )
{

	__m128i  h1= _mm_unpacklo_epi32(srcLo,srcHi);
	__m128i  h2= _mm_unpackhi_epi32(srcLo,srcHi);
	__m128i hBytes	= _mm_unpackhi_epi32(h1,h2);
	__m128i lBytes	= _mm_unpacklo_epi32(h1,h2);
	 const __m128i zero =  CONST_SETZERO_32I();
	 const __m128i nOne =  CONST_SET1_32I(0xFFFFFFFF);

	__m128i x = _mm_cmplt_epi32(hBytes,nOne);
			h1 = _mm_slli_epi32(x,31);
		
	__m128i y = _mm_cmpgt_epi32(hBytes,zero);
			h2 = _mm_srli_epi32(y,1);

	__m128i	z = _mm_and_si128(_mm_cmpeq_epi32(hBytes,zero),_mm_srai_epi32(lBytes,31));
	__m128i h3 = _mm_srli_epi32(z,1);

	lBytes = 	_mm_andnot_si128(_mm_or_si128(_mm_or_si128(x,y),z),lBytes);

	return _mm_or_si128(_mm_or_si128(h1,h2),_mm_or_si128(h3,lBytes));
}


/*! 
    \param[in] srcLo two 64bit signed values
    \param[in] srcHi two 64bit signed values
    \return four float type values.
	\par Notes:
    The function packs four 64bit signed values to 32bit float values.
    \par Input:
    \n  srcLo = (a1,a0)
    \n  srcHi = (b1,b0)
    \par Output:
    \n  return value = (b1,b0,a1,a0)
*/
CBL_INLINE  __m128 CBL_SSE2::Pack64STo32F( const __m128i &srcLo, const __m128i &srcHi )
{
	XMM128 slo,shi,dest;
	slo.i = srcLo;
	shi.i = srcHi;
	
	dest.f32[0] = (F32) slo.s64[0];
	dest.f32[1] = (F32) slo.s64[1];
	dest.f32[2] = (F32) shi.s64[0];
	dest.f32[3] = (F32) shi.s64[1];

	return dest.f;
}

/// @endcond

#endif // __CBL_PACKUNPACK_PRIMITIVES_SSE2_H__
