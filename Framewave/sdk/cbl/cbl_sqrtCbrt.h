/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_SQRT_CBRT_H__
#define __CBL_SQRT_CBRT_H__

namespace CBL_Library_Internal
{
    //src is expected to be +ve.
    CBL_INLINE __m128i div_exponent_by_3(const __m128i &src)
    {
    //	q = (a + (a>>2) ) >> 2 ; q = (q + (q >> 4) + a ) >>2;

	    __m128i reslt = _mm_srli_epi32(src, 2);
	    reslt = _mm_add_epi32(src, reslt);
	    reslt = _mm_srli_epi32(reslt, 2);
	    reslt = _mm_add_epi32(reslt, _mm_srli_epi32(reslt, 4));
	    reslt = _mm_add_epi32(src, reslt);
	    reslt = _mm_srli_epi32(reslt, 2);

	    return reslt;
    }       

    CBL_INLINE void invSqrtf_setup(__m128 &half, __m128 &three_by_2)
    {
	    half		= _mm_set1_ps(0.5F);
	    three_by_2	= _mm_set1_ps(1.5F);
    }

    CBL_INLINE void cbrt_A11_setup(__m128i &init_const, __m128 &one_by_3)
    {
	    init_const	= _mm_set1_epi32(0x2A4B2FF5);
	    one_by_3	= _mm_set1_ps(0.3333333333F);	
    }

    CBL_INLINE void inv_cbrtf_setup(__m128i &init_const, __m128 &four_by_3, __m128 &one_by_3, __m128d &four_by_3D)
    {
	    init_const	= _mm_set1_epi32(0x54A14517);
	    four_by_3	= _mm_set1_ps(/*4/3.0F*/1.33333333F);
	    one_by_3	= _mm_set1_ps(/*1/3.0F*/0.33333333F);
	    four_by_3D	= _mm_set1_pd(4/3.0);
    }


    CBL_INLINE void invSqrtf_iter(__m128 &dst, const __m128 &half_src, const __m128 &three_by_2)
    {	
	    // Newton iteration (1.5*old - (x/2)*old^3)

	    __m128 tmp = _mm_mul_ps(half_src, dst);
	    tmp = _mm_mul_ps(tmp, dst);
	    tmp = _mm_sub_ps(three_by_2, tmp);
	    dst = _mm_mul_ps(dst, tmp);
    }
    /* 
    InvSqrt (x<0)           NAN_32F
    InvSqrt (x=+0)          INF_32F
    InvSqrt (x=-0)          -INF_32F
    InvSqrt (x=-INF)        NAN_32F
    */

    CBL_INLINE void invSqrtf_special_case(const XMM128 &src, XMM128 &dst)
    {
	    /*__m128i cmp = _mm_cmpeq_epi32(src.i, _mm_setzero_si128());
	    dst.i = _mm_andnot_si128(cmp, dst.i);
	    cmp =  _mm_srli_epi32(_mm_slli_epi32(cmp,24), 1);       
	    dst.i = _mm_or_si128(dst.i, cmp);	*/
        
        XMM128 cmp ;
	    cmp.f = _mm_cmple_ps(src.f, _mm_setzero_ps());
	    dst.i = _mm_andnot_si128(cmp.i, dst.i);
	    cmp.i =  _mm_srli_epi32(_mm_slli_epi32(cmp.i, 24), 1);       
	    cmp.i = _mm_or_si128(cmp.i, _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 9));
	    dst.i = _mm_or_si128(dst.i, cmp.i);	
    }

    CBL_INLINE void cbrtf_init(const XMM128 &src, XMM128 &estimate, const __m128i &init_const)
    {
	    // if src = (2^e)*f, take 2^(-e/3) as the initial guess for inv_cbrt(src)
	    estimate.i = div_exponent_by_3(src.i);
	    estimate.i = _mm_add_epi32(init_const, estimate.i);
    }

    CBL_INLINE void cbrtf_iter(const __m128 &src, __m128 &dst, const __m128 &one_by_3) 
    {
	    //	x =  0.3333333333F * (x + x - src/(x*x)); 

	    __m128 r =  _mm_mul_ps(dst, dst);
	    r = _mm_div_ps(src,r);
	    r = _mm_add_ps(dst, r);
	    dst = _mm_add_ps(dst, r);
	    dst = _mm_mul_ps(dst, one_by_3);
    }
    CBL_INLINE void cbrt_special_case(const __m128 &src, __m128 &dst)
    {
	    //src = 0 => dst = 0 
	    __m128 cmp = _mm_cmpeq_ps(src, _mm_setzero_ps());
	    dst = _mm_andnot_ps(cmp, dst);
    }

    CBL_INLINE void inv_cbrtf_init(const XMM128 &src, XMM128 &estimate, const __m128i &init_const)
    {
	    // if src = (2^e)*f, take 2^(-e/3) as the initial guess for inv_cbrt(src)
	    estimate.i = div_exponent_by_3(src.i);
	    estimate.i = _mm_sub_epi32(init_const, estimate.i);
    }

    CBL_INLINE void inv_cbrtf_iter(const __m128 &src_by_3, XMM128 &dst, const __m128 &four_by_three) 
    {
	    //order of computation has to be "proper" to get A24 working.
	    __m128 temp; 

	    //	new = (4/3 - (src/3)*old^3)*old
	    temp = _mm_mul_ps(dst.f, src_by_3);
	    temp = _mm_mul_ps(temp, dst.f);
	    temp = _mm_mul_ps(temp, dst.f);
	    temp = _mm_sub_ps(four_by_three, temp);
	    dst.f = _mm_mul_ps(dst.f, temp);
    }

    CBL_INLINE void inv_cbrt_unit_2iter( XMM128 &src, XMM128 &dst, const __m128i &init_const,
										       const __m128 &four_by_3, const __m128& one_by_3)
    {
	    //get the initial guess
	    inv_cbrtf_init(src, dst, init_const);
    	
	    //proceed with Newton-raphson method for more accuracy
	    src.f = _mm_mul_ps(src.f, one_by_3);
	    inv_cbrtf_iter(src.f, dst, four_by_3);
	    inv_cbrtf_iter(src.f, dst, four_by_3);
    }

    /*
    InvCbrt (x=+0)      INF_32F
    InvCbrt (x=-0)      -INF_32F
    */ 

    CBL_INLINE void inv_cbrtf_special_case( const XMM128 &src, XMM128 &dst)
    {
	    //if src == 0, return inf
	    XMM128 cmp ;
	    cmp.f = _mm_cmpeq_ps(src.f, _mm_setzero_ps());
	    dst.f = _mm_andnot_ps(cmp.f, dst.f);
	    cmp.i = _mm_slli_epi32(_mm_srli_epi32(cmp.i, 24), 23) ;	//inf
	    dst.f = _mm_or_ps(dst.f, cmp.f);
    }


    CBL_INLINE void Sqrt_A11_sse2(const XMM128 &src, XMM128 &dst, const __m128 &half, const __m128 &three_by_2)
    {
	    dst.f = _mm_rsqrt_ps(src.f);
	    __m128 half_src = _mm_mul_ps(src.f, half);

	    // Newton iteration 
	    invSqrtf_iter(dst.f, half_src, three_by_2);
	    invSqrtf_special_case(src, dst );
        dst.f = _mm_div_ps(_mm_sub_ps(three_by_2, half), dst.f);    // sqrt = 1/(inv_sqrt)

    }

    CBL_INLINE void InvSqrt_A11_sse2(const XMM128 &src, XMM128 &dst, const __m128 &half, const __m128 &three_by_2)
    {
	    dst.f = _mm_rsqrt_ps(src.f);
	    __m128 half_src = _mm_mul_ps(src.f, half);

	    // Newton iteration 
	    invSqrtf_iter(dst.f, half_src, three_by_2);
	    invSqrtf_special_case(src, dst );
    }


    CBL_INLINE void Cbrt_A11_sse2(const XMM128 &src, XMM128 &dst, const __m128i &init_const, const __m128 &one_by_3)
    {
	    const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
    	
	    XMM128  tsrc, tdst ;
	    tsrc.i = _mm_xor_si128(src.i, sign);	
    	
	    cbrtf_init(tsrc, tdst, init_const);
	    cbrtf_iter(tsrc.f, tdst.f, one_by_3);
	    cbrtf_iter(tsrc.f, tdst.f, one_by_3);
	    cbrt_special_case(src.f, tdst.f);
	    dst.i = _mm_or_si128(tdst.i, sign);
    }

    CBL_INLINE void InvCbrt_A11_sse2(const XMM128 &src, XMM128 &dst, 
					     const __m128i &init_const, const __m128 &four_by_3, const __m128 &one_by_3)
    {
	    XMM128 tsrc ;
	    const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	    tsrc.i = _mm_xor_si128(src.i, sign);	

	    inv_cbrt_unit_2iter(tsrc, dst, init_const, four_by_3, one_by_3);	
	    inv_cbrtf_iter(tsrc.f, dst, four_by_3);
	    dst.i = _mm_or_si128(dst.i, sign);

	    inv_cbrtf_special_case( src, dst);
    }
} // namespace CBL_Library_Internal

#endif //__CBL_SQRT_CBRT_H__
