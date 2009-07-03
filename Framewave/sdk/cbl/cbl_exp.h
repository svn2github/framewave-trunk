/*
Copyright (c) 2006-2009 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_EXP_H__
#define __CBL_EXP_H__

#include "cbl_common_primitives.h"

namespace CBL_EXP_TABLES 
{
/* 
     * for single precision floats(32f)
     * taken from :
     * "Table driven Implementation of the exponential function 
     * in IEEE floating-poitn arithmetic" - PING TAK PETER TANG 
    */
     CBL_FORCEALIGN_16 static const float two_to_jby32_lead_32f[32] = {
	    1.0000000000E+00F,  /* 0x3F800000 */
	    1.0214843750E+00F,  /* 0x3F82C000 */
	    1.0429687500E+00F,  /* 0x3F858000 */
	    1.0664062500E+00F,  /* 0x3F888000 */
	    1.0898437500E+00F,  /* 0x3F8B8000 */
	    1.1132812500E+00F,  /* 0x3F8E8000 */
	    1.1386718750E+00F,  /* 0x3F91C000 */
	    1.1621093750E+00F,  /* 0x3F94C000 */
	    1.1875000000E+00F,  /* 0x3F980000 */
	    1.2148437500E+00F,  /* 0x3F9B8000 */
	    1.2402343750E+00F,  /* 0x3F9EC000 */
	    1.2675781250E+00F,  /* 0x3FA24000 */
	    1.2949218750E+00F,  /* 0x3FA5C000 */
	    1.3242187500E+00F,  /* 0x3FA98000 */
	    1.3535156250E+00F,  /* 0x3FAD4000 */
	    1.3828125000E+00F,  /* 0x3FB10000 */
	    1.4140625000E+00F,  /* 0x3FB50000 */
	    1.4433593750E+00F,  /* 0x3FB8C000 */
	    1.4765625000E+00F,  /* 0x3FBD0000 */
	    1.5078125000E+00F,  /* 0x3FC10000 */
	    1.5410156250E+00F,  /* 0x3FC54000 */
	    1.5742187500E+00F,  /* 0x3FC98000 */
	    1.6093750000E+00F,  /* 0x3FCE0000 */
	    1.6445312500E+00F,  /* 0x3FD28000 */
	    1.6816406250E+00F,  /* 0x3FD74000 */
	    1.7167968750E+00F,  /* 0x3FDBC000 */
	    1.7558593750E+00F,  /* 0x3FE0C000 */
	    1.7929687500E+00F,  /* 0x3FE58000 */
	    1.8339843750E+00F,  /* 0x3FEAC000 */
	    1.8730468750E+00F,  /* 0x3FEFC000 */
	    1.9140625000E+00F,  /* 0x3FF50000 */
	    1.9570312500E+00F /* 0x3FFA8000 */
    };

  CBL_FORCEALIGN_16 static const float two_to_jby32_trail_32f[32] = {
	    0.0000000000E+00F,  /* 0x00000000 */
	    4.1277357377E-04F,  /* 0x39D86988 */
	    1.3050324051E-03F,  /* 0x3AAB0D9F */
	    7.3415064253E-04F,  /* 0x3A407404 */
	    6.6398258787E-04F,  /* 0x3A2E0F1E */
	    1.1054925853E-03F,  /* 0x3A90E62D */
	    1.1675967835E-04F,  /* 0x38F4DCE0 */
	    1.6154836630E-03F,  /* 0x3AD3BEA3 */
	    1.7071149778E-03F,  /* 0x3ADFC146 */
	    4.0360994171E-04F,  /* 0x39D39B9C */
	    1.6234370414E-03F,  /* 0x3AD4C982 */
	    1.4728321694E-03F,  /* 0x3AC10C0C */
	    1.9176795613E-03F,  /* 0x3AFB5AA6 */
	    1.0178930825E-03F,  /* 0x3A856AD3 */
	    7.3992193211E-04F,  /* 0x3A41F752 */
	    1.0973819299E-03F,  /* 0x3A8FD607 */
	    1.5106226783E-04F,  /* 0x391E6678 */
	    1.8214319134E-03F,  /* 0x3AEEBD1D */
	    2.6364589576E-04F,  /* 0x398A39F4 */
	    1.3519275235E-03F,  /* 0x3AB13329 */
	    1.1952003697E-03F,  /* 0x3A9CA845 */
	    1.7620950239E-03F,  /* 0x3AE6F619 */
	    1.1153318919E-03F,  /* 0x3A923054 */
	    1.2242280645E-03F,  /* 0x3AA07647 */
	    1.5220546629E-04F,  /* 0x391F9958 */
	    1.8224230735E-03F,  /* 0x3AEEDE5F */
	    3.9278529584E-04F,  /* 0x39CDEEC0 */
	    1.7403248930E-03F,  /* 0x3AE41B9D */
	    2.3711356334E-05F,  /* 0x37C6E7C0 */
	    1.1207590578E-03F,  /* 0x3A92E66F */
	    1.1440613307E-03F,  /* 0x3A95F454 */
	    1.1287408415E-04F /* 0x38ECB6D0 */
    };
    
  static const float two_to_jby32_table[32] = /*total of lead and trail for A11*/
    {	
	    1.00000000e+00F,		1.02189715e+00F,		1.04427378e+00F,		1.06714040e+00F,
	    1.09050773e+00F,		1.11438674e+00F,		1.13878863e+00F,		1.16372486e+00F,
	    1.18920711e+00F,		1.21524736e+00F,		1.24185781e+00F,		1.26905096e+00F,
	    1.29683955e+00F,		1.32523664e+00F,		1.35425555e+00F,		1.38390988e+00F,
	    1.41421356e+00F,		1.44518081e+00F,		1.47682615e+00F,		1.50916443e+00F,
	    1.54221083e+00F,		1.57598085e+00F,		1.61049033e+00F,		1.64575548e+00F,
	    1.68179283e+00F,		1.71861930e+00F,		1.75625216e+00F,		1.79470907e+00F,
	    1.83400809e+00F,		1.87416763e+00F,		1.91520656e+00F,		1.95714412e+00F
    };
} // namespace CBL_EXP_TABLES

namespace CBL_Library_Internal
{
    CBL_INLINE void exp_A11_setup(__m128 &thirtytwo_by_ln2, __m128 &ln2_by_32, __m128 &A1_A2, __m128 &max_min_arg)
    {
        thirtytwo_by_ln2 = _mm_set1_ps(4.6166240692E+01F); 
        ln2_by_32		 = _mm_set1_ps(0.02166084939249829F);	
        A1_A2			 = _mm_set_ps(1.66666666665260878863e-01F, 1.66666666665260878863e-01F, 
							          5.00000000000000008883e-01F, 5.00000000000000008883e-01F ); 
        max_min_arg		 = _mm_set_ps(88.7228390F, 88.7228390F, -87.5F, -87.5F);			
    }

    /* Exponential 32f special cases
     * (1) if x < min_exp_arg return 0 
     * (2) else if x > max_exp_arg then return inf (0x7F800000)
     * (3) else return dst. 
    */
    CBL_INLINE void exp_32f_special_case(const XMM128 &src, XMM128 &dst, const __m128 &max_min_arg )
    {	
	    XMM128 mask;

	    mask.f = _mm_cmpge_ps(src.f, _mm_movelh_ps(max_min_arg, max_min_arg)); //min
	    dst.i = _mm_and_si128(mask.i, dst.i );	// (1)
	    mask.f = _mm_cmpgt_ps(src.f, _mm_movehl_ps(max_min_arg, max_min_arg)); // max
	    dst.f = _mm_andnot_ps(mask.f, dst.f);	//(3)
    	
	    mask.i = _mm_slli_epi32(_mm_srli_epi32(mask.i, 24), 23);	//0x7f800000 i.e, inf
	    dst.i = _mm_or_si128(dst.i, mask.i);	//(2)
    }

    CBL_INLINE void exp_A11_unit(const __m128 &src, XMM128 &dst, \
							    const __m128 &thirtytwo_by_log2, const __m128 &log2_by_32, const __m128 &A1_A2) 
    {
	    __m128 p ; 
	    XMM128 m, n, j, r;

	    r.f = _mm_mul_ps(src, thirtytwo_by_log2);
	    n.i = _mm_cvtps_epi32(r.f);			//  n = nearest integer to r 

	    j.i = _mm_srli_epi32(_mm_slli_epi32(n.i, 27), 27);	//j = n%32(0x1f)

	    m.i = _mm_srai_epi32(n.i, 5);	// m = (n-j)/32
	    n.f = _mm_cvtepi32_ps(n.i);

	    r.f = _mm_sub_ps(src, _mm_mul_ps(n.f, log2_by_32));

	    // p = r1 + (r2 + r*r*( 5.00000000000000008883e-01F + r*( 1.66666666665260878863e-01F )));	
	    p = _mm_mul_ps(r.f, _mm_movehl_ps(A1_A2, A1_A2));
	    p = _mm_add_ps(p, _mm_movelh_ps(A1_A2, A1_A2));
	    p = _mm_mul_ps(p, r.f);
	    p = _mm_mul_ps(p, r.f);
	    p = _mm_add_ps(p, r.f);
    	
	    j.i = _mm_or_si128(j.i, _mm_srli_epi64(j.i, 24));
	    int i0 = _mm_extract_epi16(j.i, 0); 	
	    int i1 = _mm_extract_epi16(j.i, 4);
    	
	    r.f = _mm_set_ps(CBL_EXP_TABLES::two_to_jby32_table[i1>>8 ], 
					     CBL_EXP_TABLES::two_to_jby32_table[i1 &0xff],
					     CBL_EXP_TABLES::two_to_jby32_table[i0 >>8],
					     CBL_EXP_TABLES::two_to_jby32_table[i0 &0xff]);

	    // reslt = (2^m)*(r+ P*r)	

	    p = _mm_mul_ps(r.f, p);
	    r.f = _mm_add_ps(r.f, p);		
	    m.i = _mm_slli_epi32(m.i, 23);
	    dst.i = _mm_add_epi32(r.i, m.i);  
    }

    CBL_INLINE void Exp_A11_sse(const XMM128 &src, XMM128 &dst, 
				    const __m128 &thirtytwo_by_ln2, const __m128 &ln2_by_32,
				    const __m128 &A1_A2, const __m128 &max_min_arg)
    {
        exp_A11_unit(src.f, dst, thirtytwo_by_ln2, ln2_by_32, A1_A2);
        exp_32f_special_case(src, dst, max_min_arg);
    }


    /*-----*/
    CBL_INLINE void exp_32f(const __m128 &src, XMM128 &dst, 
				    const __m128 &thirtytwo_by_ln2, const __m128 &ln2_by_32_lead_trail, const __m128 &A1_A2) 
    {
	    __m128 p, r1, r2 ; 
	    XMM128 m, n, j, r;

	    r.f = _mm_mul_ps(src, thirtytwo_by_ln2);
	    n.i = _mm_cvtps_epi32(r.f);			//  n = nearest integer to r 

	    j.i = _mm_srli_epi32(_mm_slli_epi32(n.i, 27), 27);	//j = n%32(0x1f)

	    m.i = _mm_srai_epi32(n.i, 5);	// m = (n-j)/32
	    n.f = _mm_cvtepi32_ps(n.i);

	    r1 = _mm_movehl_ps(ln2_by_32_lead_trail, ln2_by_32_lead_trail);
	    r1 = _mm_mul_ps(r1, n.f);
	    r1 = _mm_sub_ps(src, r1);				// r1 = x-n*log2_by_32_lead    

	    r2 = _mm_movelh_ps(ln2_by_32_lead_trail, ln2_by_32_lead_trail);
	    r2 = _mm_mul_ps(r2, n.f);		// r2 =  -n*log2_by_32_trail
	    r.f = _mm_sub_ps(r1, r2) ;		//	  r.f = r1 + r2 i.e., (r1 -(-r2)

	    // p = r1 + (r2 + r*r*( 5.00000000000000008883e-01F + r*( 1.66666666665260878863e-01F )));	
	    p = _mm_mul_ps(r.f, _mm_movehl_ps(A1_A2, A1_A2));
	    p = _mm_add_ps(p, _mm_movelh_ps(A1_A2, A1_A2));
	    p = _mm_mul_ps(p, r.f);
	    p = _mm_mul_ps(p, r.f);
	    p = _mm_add_ps(p, r.f);

	    //s_lead(j)

	    int i3 = _mm_extract_epi16(j.i, 6);  int i2 = _mm_extract_epi16(j.i, 4);
	    int i1 = _mm_extract_epi16(j.i, 2);  int i0 = _mm_extract_epi16(j.i, 0);

    /*	j.i = _mm_or_si128(j.i, _mm_srli_epi64(j.i, 24));
	    int i0 = _mm_extract_epi16(j.i, 0); 	
	    int i1 = i0>>8 ; i0 = i0 & 0xff ;		
	    int i2 = _mm_extract_epi16(j.i, 4);
	    int i3 = i2>>8 ; i2 = i2 & 0xff ;			
    */
	    r1 = _mm_set_ps(CBL_EXP_TABLES::two_to_jby32_lead_32f[i3], CBL_EXP_TABLES::two_to_jby32_lead_32f[i2],
					    CBL_EXP_TABLES::two_to_jby32_lead_32f[i1], CBL_EXP_TABLES::two_to_jby32_lead_32f[i0]);
    	
	    r2 = _mm_set_ps(CBL_EXP_TABLES::two_to_jby32_trail_32f[i3], CBL_EXP_TABLES::two_to_jby32_trail_32f[i2],
					    CBL_EXP_TABLES::two_to_jby32_trail_32f[i1], CBL_EXP_TABLES::two_to_jby32_trail_32f[i0]);

	    // reslt = (2^m)*(s_lead(j) + (s_trail(j) + P *(s_lead(j)+s_trail(j) )))
	    // the order of above computation is VERY imp to maintain the precision

	    r.f = _mm_add_ps(r1, r2);
	    r.f = _mm_mul_ps(r.f, p);
	    r2 = _mm_add_ps(r2, r.f);

	    r.f = _mm_add_ps(r1, r2);
    	
	    /* from m, calculate (float)pow(2.0,m) */
	    m.i = _mm_slli_epi32(m.i, 23);
	    dst.i = _mm_add_epi32(r.i, m.i);  // this result is correct expcept for special cases.	
    }

    CBL_INLINE void exp_32f_unit(const XMM128 &src, XMM128 &dst, 
					    const __m128 &thirtytwo_by_ln2, const __m128 &ln2_by_32_lead_trail, 
					    const __m128 &A1_A2, const __m128 &max_min_arg) 
    {
	    XMM128 tsrc; 

	    tsrc.f = src.f ;
    //  if(_mm_movemask_ps(_mm_cmple_ps(src.f, _mm_movehl_ps(max_min_arg, max_min_arg))))
	        exp_32f(src.f, dst, thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2 );
       exp_32f_special_case(tsrc, dst, max_min_arg);
    }
} // namespace CBL_Library_Internal

#endif
