/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_POW_H__
#define __CBL_POW_H__

namespace CBL_Library_Internal
{
    /*
     * Check whether the given float number is a long integer.
     * Return value would be all_f's in the places where the input is integer or 0 if not.
     * "is_even" parameter would be all_f's in the places where the input is a even integer or 0 if not.
    */
    CBL_INLINE __m128i cbl_is_int( const XMM128 &src, __m128i &is_even)
    {
	    XMM128 mantissa, exp ;

	    static const __m128i zero = CONST_SETZERO_32I(); //_mm_setzero_si128();
	    static const __m128i exp_mask = CONST_SET1_32I(0x7f800000); 
	    exp.i = _mm_and_si128(src.i, exp_mask);

	    exp.i =  _mm_srli_epi32(exp.i, 23); 
	    exp.i = _mm_sub_epi32(exp.i, _mm_srli_epi32(exp_mask, 24));	//subtract 127, get the real exponent
	    exp.i = _mm_max_epi16(exp.i, zero);		
	    exp.i = _mm_min_epi16(exp.i, _mm_srli_epi32(exp_mask, 26));	//31
	    mantissa.i = _mm_set_epi32(src.u32[3]<< _mm_extract_epi16(exp.i, 6), \
								    src.u32[2]<< _mm_extract_epi16(exp.i, 4), \
								    src.u32[1]<< _mm_extract_epi16(exp.i, 2), \
								    src.u32[0]<< _mm_extract_epi16(exp.i, 0));	// do it better way ??

	    mantissa.i = _mm_slli_epi32(mantissa.i, 8); 
	    is_even = _mm_cmpeq_epi32(mantissa.i, zero);
	    mantissa.i = _mm_slli_epi32(mantissa.i, 1);
	    return _mm_cmpeq_epi32(mantissa.i, zero);	
    }

    /*
     * Check whether the given float number is a long integer.
     * Return value would be all_f's in the places where the input is long integer or 0 if not.
     * "is_even" parameter would be all_f's in the places where the input is a even integer or 0 if not.
    */
    CBL_INLINE __m128i cbl_is_longint(const XMM128 &src, __m128i &is_even)
    {
	    XMM128 mantissa, exp ;

	    static const __m128i zero = CONST_SETZERO_64I();  
	    static const __m128i exp_mask = CONST_SET1_64I( 0x7ff0000000000000ll );  //0x7ff00000 0x0;
	    exp.i = _mm_and_si128(src.i, exp_mask);

	    exp.i =  _mm_srli_epi64(exp.i, 52); 
	    exp.i = _mm_sub_epi32(exp.i, _mm_srli_epi64(exp_mask, 53));	//subtract 1023, get the real exponent
	    exp.i = _mm_max_epi16(exp.i, zero);		
	    exp.i = _mm_min_epi16(exp.i, _mm_srli_epi64(exp_mask, 57));	//63
    	
	    U64 a =  src.u64[1]<< _mm_extract_epi16(exp.i, 4);
	    U64 b =  src.u64[0]<< _mm_extract_epi16(exp.i, 0);

	    mantissa.i = _mm_set_epi32( (int)(a>>32), (int)a, (int)(b>>32), (int)b);	//there is no set_epi64 
	    mantissa.i = _mm_slli_epi64(mantissa.i, 11); 
	    is_even = _mm_cmpeq_epi32(mantissa.i, zero);
	    mantissa.i = _mm_slli_epi64(mantissa.i, 1);
	    return _mm_cmpeq_epi32(mantissa.i, zero);	
    }

    CBL_INLINE void pow_A11_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, 
				     const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &ln_A1_A2)
    {
	    static const __m128 thirtytwo_by_ln2		= CONST_SET1_32F(4.6166240692E+01F); //thirtytwo_by_ln2
	    static const __m128 ln2_by_32_lead_trail   = CONST_SET_32F(2.1659851074E-02F, 2.1659851074E-02F, 9.9831822808E-07F, 9.9831822808E-07F); //ln2_by_32_lead , ln2_by_32_trail
	    static const __m128 A1_A2					= CONST_SET_32F(1.66666666665260878863e-01F, 1.66666666665260878863e-01F, 
														    5.00000000000000008883e-01F, 5.00000000000000008883e-01F ); //A1_A2
	    static const __m128 max_min_arg				= CONST_SET_32F(88.7228390F, 88.7228390F, -87.33654470F, -87.33654470F);	//max_arg, min_arg		
    								
	    // src1^src2 = exp(src2 * ln(src1))		
	    XMM128  tdst ;
	    ln_32f_tang_unit(src1, tdst ,ln2_lead, ln2_trail, ln_A1_A2);
	    tdst.f = _mm_mul_ps(tdst.f, src2.f);
	    tdst.f = _mm_andnot_ps(_mm_cmpeq_ps(src2.f, _mm_setzero_ps()), tdst.f);	// x**0 -> 1 for any x
	    exp_32f_unit(tdst, dst, thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, max_min_arg);
    }

    CBL_INLINE void pow_32f_special_case( const XMM128 &src1, const XMM128 &src2, XMM128 &dst, const __m128i &is_src2_int)
    {
	    XMM128 tsrc2;
	    tsrc2.f = src2.f ;
	    const __m128i zero  = _mm_setzero_si128();

	    //		Pow (x<0, y=NON_INT) ->	NAN_32F		
	    __m128i cmp1 = _mm_cmplt_epi32(src1.i, zero);
        __m128i cmp2 = _mm_andnot_si128(is_src2_int, _mm_cmpeq_epi32(zero, zero));

	    __m128i cmp = _mm_and_si128(cmp1, cmp2);
        // modified _mm_srli_epi32(cmp, 23)  to _mm_srli_epi32(cmp, 22) to match Ref code
        //Ref code gives it as IND instead of NAN and hence the change
	    __m128i nan = _mm_slli_epi32(_mm_srli_epi32(cmp, 22), 22);	//0xffc00000
	    dst.i = _mm_andnot_si128(cmp, dst.i);
	    dst.i = _mm_or_si128(dst.i, nan);

	    //		Pow (x=+0, y=-ve number) ->  INF_32F
	    cmp1 = _mm_cmpeq_epi32(src1.i, zero);
	    cmp2 = _mm_cmplt_epi32(tsrc2.i, zero);
	    cmp = _mm_and_si128(cmp1, cmp2);
        dst.i = _mm_andnot_si128(cmp, dst.i);
	    cmp = _mm_slli_epi32(_mm_srli_epi32(cmp, 24), 23);	//0x7f800000
	    dst.i = _mm_or_si128(dst.i, cmp);
        //Pow ( x=+0 , y>0 number) -> 0
        cmp2 = _mm_cmpgt_epi32(tsrc2.i, zero);
        cmp = _mm_and_si128(cmp1, cmp2);
        dst.i = _mm_andnot_si128(cmp, dst.i);
    }

    CBL_INLINE void Pow_A11_sse2(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, 
				     const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &A1_A2)
    {
	    __m128i sign_s1 = _mm_slli_epi32(_mm_srli_epi32(src1.i, 31), 31);
	    XMM128 tsrc;
	    tsrc.i  = _mm_xor_si128(src1.i, sign_s1);

	    pow_A11_unit(tsrc, src2, dst, ln2_lead, ln2_trail, A1_A2);
    	
	    __m128i s2_is_int, s2_is_even, s2_is_odd;
	    s2_is_int = cbl_is_int(src2, s2_is_even);
    	
	    s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_int);
	    s2_is_odd = _mm_and_si128(s2_is_odd, sign_s1);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	    dst.i = _mm_or_si128(dst.i, s2_is_odd);
	    pow_32f_special_case(src1, src2, dst, s2_is_int );
    }
} // namespace CBL_Library_Internal

#endif //__CBL_POW_H__
