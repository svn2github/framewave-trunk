/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __CBL_CBL_LN_H__
#define __CBL_CBL_LN_H__

namespace CBL_LN_TABLES 
{
    CBL_FORCEALIGN_16 static const float ln_32f_lead_tbl[129] = {	
	    0.0000000000e+00F, 	7.7819824219e-03F, 	1.5502929688e-02F, 	2.3162841797e-02F, 	3.0761718750e-02F, 
	    3.8314819336e-02F, 	4.5806884766e-02F, 	5.3237915039e-02F, 	6.0623168945e-02F, 	6.7947387695e-02F, 
	    7.5210571289e-02F, 	8.2443237305e-02F, 	8.9599609375e-02F, 	9.6725463867e-02F, 	1.0379028320e-01F, 
	    1.1080932617e-01F, 	1.1778259277e-01F, 	1.2469482422e-01F, 	1.3156127930e-01F, 	1.3839721680e-01F, 
	    1.4517211914e-01F, 	1.5190124512e-01F, 	1.5859985352e-01F, 	1.6523742676e-01F, 	1.7184448242e-01F, 
	    1.7840576172e-01F, 	1.8492126465e-01F, 	1.9139099121e-01F, 	1.9781494141e-01F, 	2.0420837402e-01F, 
	    2.1055603027e-01F, 	2.1687316895e-01F, 	2.2312927246e-01F, 	2.2937011719e-01F, 	2.3556518555e-01F, 
	    2.4171447754e-01F, 	2.4783325195e-01F, 	2.5390625000e-01F, 	2.5994873047e-01F, 	2.6596069336e-01F, 
	    2.7192687988e-01F, 	2.7786254883e-01F, 	2.8376770020e-01F, 	2.8962707520e-01F, 	2.9545593262e-01F, 
	    3.0125427246e-01F, 	3.0702209473e-01F, 	3.1274414062e-01F, 	3.1845092773e-01F, 	3.2411193848e-01F, 
	    3.2974243164e-01F, 	3.3534240723e-01F, 	3.4091186523e-01F,  3.4646606445e-01F, 	3.5197448730e-01F, 
	    3.5745239258e-01F, 	3.6289978027e-01F, 	3.6831665039e-01F, 	3.7370300293e-01F, 	3.7907409668e-01F, 
	    3.8439941406e-01F, 	3.8970947266e-01F, 	3.9498901367e-01F, 	4.0023803711e-01F, 	4.0545654297e-01F, 
	    4.1065979004e-01F, 	4.1581726074e-01F, 	4.2095947266e-01F, 	4.2607116699e-01F, 	4.3116760254e-01F, 
	    4.3623352051e-01F, 	4.4126892090e-01F, 	4.4627380371e-01F, 	4.5126342773e-01F, 	4.5622253418e-01F, 
	    4.6116638184e-01F, 	4.6607971191e-01F, 	4.7097778320e-01F, 	4.7584533691e-01F, 	4.8068237305e-01F, 
	    4.8550415039e-01F, 	4.9029541016e-01F, 	4.9507141113e-01F, 	4.9981689453e-01F, 	5.0454711914e-01F, 
	    5.0924682617e-01F, 	5.1393127441e-01F, 	5.1860046387e-01F, 	5.2323913574e-01F, 	5.2786254883e-01F, 
	    5.3245544434e-01F, 	5.3703308105e-01F, 	5.4159545898e-01F, 	5.4612731934e-01F, 	5.5064392090e-01F, 
	    5.5513000488e-01F, 	5.5960083008e-01F, 	5.6405639648e-01F, 	5.6849670410e-01F, 	5.7290649414e-01F, 
	    5.7730102539e-01F, 	5.8168029785e-01F, 	5.8604431152e-01F, 	5.9037780762e-01F, 	5.9469604492e-01F, 
	    5.9899902344e-01F, 	6.0328674316e-01F, 	6.0754394531e-01F, 	6.1180114746e-01F, 	6.1602783203e-01F, 
	    6.2023925781e-01F, 	6.2442016602e-01F, 	6.2860107422e-01F, 	6.3275146484e-01F, 	6.3690185547e-01F, 
	    6.4102172852e-01F, 	6.4512634277e-01F, 	6.4921569824e-01F, 	6.5328979492e-01F, 	6.5734863281e-01F, 
	    6.6139221191e-01F, 	6.6542053223e-01F, 	6.6941833496e-01F, 	6.7341613770e-01F, 	6.7739868164e-01F, 
	    6.8135070801e-01F, 	6.8530273438e-01F, 	6.8922424316e-01F, 	6.9314575195e-01F,
    };

    CBL_FORCEALIGN_16 static	const float ln_32f_trail_tbl[129] = {
	    0.0000000000e+00F, 	1.5802017995e-07F, 	1.2568484653e-06F, 	4.2174846594e-06F, 	9.9399167537e-06F, 
	    4.0449661991e-06F, 	2.6512656692e-06F, 	6.5994797498e-06F, 	1.4528711223e-06F, 	3.2742131953e-06F, 
	    1.2849948525e-05F, 	4.3190638709e-07F, 	1.2549314687e-05F, 	4.1625913636e-06F, 	6.5104785186e-06F, 
	    5.0401684151e-06F, 	4.4288294596e-07F, 	8.6542822072e-06F, 	1.5078491844e-05F, 	5.1060622441e-06F, 
	    9.8907038729e-06F, 	1.4796908654e-05F, 	5.1766610136e-06F, 	1.2146137495e-05F, 	5.7745047842e-06F, 
	    1.8957540683e-06F, 	1.0738455745e-06F, 	3.8617886920e-06F, 	1.0801923670e-05F, 	7.1674052534e-06F, 
	    8.7388339121e-06F, 	7.6935530186e-07F, 	1.4278853272e-05F, 	3.9838773458e-06F, 	8.8576589191e-07F, 
	    5.4593480827e-06F, 	2.9119514563e-06F, 	8.9599809635e-06F, 	8.7939681760e-06F, 	2.8551377629e-06F, 
	    6.8356008293e-06F, 	5.9021753313e-06F, 	4.7293533212e-07F, 	6.2173877302e-06F, 	8.2802766484e-06F, 
	    7.0581172243e-06F, 	2.9405683494e-06F, 	1.1569378897e-05F, 	2.8033841596e-06F, 	7.5301776495e-06F, 
	    1.0854731843e-05F, 	1.3134694575e-05F, 	1.4721736218e-05F, 	7.0289308357e-07F, 	1.9358524907e-06F, 
	    3.4963436788e-06F, 	5.7134159310e-06F, 	8.9107680826e-06F, 	1.3406863897e-05F, 	4.2562552819e-06F, 
	    1.2284847832e-05F, 	7.2784837752e-06F, 	4.7945689940e-06F, 	5.1270176377e-06F, 	8.5651394144e-06F, 
	    1.3494620588e-07F, 	1.0634401523e-05F, 	9.8219878796e-06F, 	1.3228318713e-05F, 	5.8622793088e-06F, 
	    3.2462671056e-06F, 	5.6399064377e-06F, 	1.3298917482e-05F, 	1.1216405084e-05F, 	1.4899301900e-05F, 
	    9.3332862326e-06F, 	1.0018010537e-05F, 	1.9320156660e-06F, 	5.6795590142e-07F, 	6.1562988769e-06F, 
	    3.6653910758e-06F, 	8.5778889438e-06F, 	5.8556650390e-06F, 	1.0975025199e-05F, 	8.8916117703e-06F, 
	    1.5075617933e-05F, 	1.4476688172e-05F, 	7.3003408582e-06F, 	9.0080223604e-06F, 	4.5407927174e-06F, 
	    9.3545335343e-06F, 	8.3848421961e-06F, 	1.8234483694e-06F, 	5.1182621982e-06F, 	3.1970542248e-06F, 
	    1.1502657689e-05F, 	1.4957857298e-05F, 	1.3741800428e-05F, 	8.0312511063e-06F, 	1.3259421160e-05F, 
	    1.4339644199e-05F, 	1.1441783060e-05F, 	4.7334801407e-06F, 	9.6389849888e-06F, 	1.1062824818e-05F, 
	    9.1662085834e-06F, 	4.1082740218e-06F, 	1.1304912042e-05F, 	3.9364505544e-07F, 	2.0451842641e-06F, 
	    1.1519393576e-06F, 	1.3121996268e-05F, 	7.5852036241e-06F, 	1.5204727288e-05F, 	5.6067683192e-06F, 
	    9.4509053062e-06F, 	1.1618600147e-05F, 	1.2248382922e-05F, 	1.1477090871e-05F, 	9.4398958600e-06F, 
	    6.2703313025e-06F, 	2.1003185280e-06F, 	1.2318981692e-05F, 	6.5375168542e-06F, 	1.4195118114e-07F, 
	    8.5168000906e-06F, 	1.2687239194e-06F, 	9.0380747465e-06F, 	1.4286068203e-06F, 
    };
} // namespace CBL_LN_TABLES 

namespace CBL_Library_Internal
{
    CBL_INLINE void ln_A11_setup(__m128 &half_sqhalf, __m128 &p5, __m128 &p6_p7,
									    __m128 &p8_p9, __m128 &const1_const2)
    {
	    half_sqhalf = _mm_set_ps(0.5F, 0.5F, 0.7071067811865475F, 0.7071067811865475F);//half, sqrt(half)
	    p5			= _mm_set1_ps(1.4249322787E-1F);	//  p5
	    p6_p7		= _mm_set_ps(-1.6668057665E-1F, -1.6668057665E-1F, 2.0000714765E-1F, 2.0000714765E-1F);	//p6_p7
	    p8_p9		= _mm_set_ps(-2.4999993993E-1F, -2.4999993993E-1F, 3.3333331174E-1F, 3.3333331174E-1F);	//p8_p9
	    const1_const2 = _mm_set_ps(-2.12194440e-4F, -2.12194440e-4F, 0.693359375F, 0.693359375F); //const1_const2
    }

    CBL_INLINE void ln_A11_unit(const XMM128 &src, XMM128 &dst, const __m128 &hf_sqrthf,
								       const __m128 &p5, const __m128 &p6_p7, const __m128 &p8_p9, const __m128 &const1_const2)
    {
	    XMM128 x, one, half;
	    __m128 y, z, sqrt_half ; 
	    XMM128 e, cmp ;

	    half.f = _mm_movehl_ps(hf_sqrthf, hf_sqrthf );
	    sqrt_half = _mm_movelh_ps(hf_sqrthf, hf_sqrthf );
	    one.f = _mm_add_ps(half.f, half.f);

	    e.i = _mm_sub_epi32(src.i, one.i);
	    e.i = _mm_srai_epi32(e.i, 23);
	    e.f = _mm_cvtepi32_ps(e.i);			//  0 <= 2^(-e.f)*src < 1 
	    e.f = _mm_add_ps(e.f, one.f);

	    x.i	= _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9);
	    x.i = _mm_or_si128(x.i, half.i);

	    y = _mm_sub_ps(x.f, one.f);
	    cmp.f = _mm_cmplt_ps(x.f, sqrt_half);
	    e.f = _mm_sub_ps(e.f, _mm_and_ps(cmp.f, one.f));
	    x.i = _mm_and_si128(x.i, cmp.i);
	    x.f = _mm_add_ps(x.f, y);

	    z = _mm_mul_ps(x.f, x.f);

	    y = _mm_mul_ps(x.f, p5/*p5*/);
	    y = _mm_add_ps(y, _mm_movehl_ps(p6_p7, p6_p7)/*p6*/);
	    y = _mm_mul_ps(y, x.f);				  
	    y = _mm_add_ps(y, _mm_movelh_ps(p6_p7, p6_p7)/*p7*/);
	    y = _mm_mul_ps(y, x.f);				  
	    y = _mm_add_ps(y, _mm_movehl_ps(p8_p9, p8_p9)/*p8*/);
	    y = _mm_mul_ps(y, x.f);				  
	    y = _mm_add_ps(y, _mm_movelh_ps(p8_p9, p8_p9)/*p9*/);

	    y = _mm_mul_ps(y, x.f);
	    y = _mm_mul_ps(y, z);

	    __m128 tmp;
	    tmp = _mm_mul_ps(_mm_movehl_ps(const1_const2, const1_const2)/*const1*/, e.f);
	    y = _mm_add_ps(y, tmp);
	    tmp = _mm_mul_ps(z, half.f);
	    y = _mm_sub_ps(y, tmp);

	    z = _mm_add_ps(x.f, y);
	    tmp = _mm_mul_ps(_mm_movelh_ps(const1_const2, const1_const2)/*const2*/, e.f);
	    dst.f = _mm_add_ps(z, tmp);
    }

    CBL_INLINE void ln_32f_tang_setup(__m128 &ln2_lead, __m128 &ln2_trail, __m128 &A1_A2)
    {
	    //changes here should be made in powx_32f_setup also
	    ln2_lead	= _mm_set1_ps(0.693145751953125F);		//log2_lead
	    ln2_trail	= _mm_set1_ps(1.4286068202862268e-06F);		//log2_trail
	    A1_A2		= _mm_set_ps(0.333329854F, 0.333329854F, -0.500003886F, -0.500003886F); //A1_A2
    }

    /*	Reference:
     *	Table driven implementation of the Logarithm function in 
     *	IEEE floating point arithmetic -- PING TAK PETER TANG
     */

    CBL_INLINE void ln_32f_tang_unit(const XMM128 &src, XMM128 &dst, const __m128 &log2_lead, const __m128 &log2_trail,
								         const __m128 &A1_A2)
    {
	    XMM128 m, f_lead, f_trail; 
	    __m128 q, u,  l_lead, l_trail ;
	    XMM128 j;

	    const __m128i one_twenty7 = _mm_srli_epi32(_mm_slli_epi32( _mm_cmpeq_epi32(src.i, src.i), 25), 2); //0x3f800000

	    m.i = _mm_sub_epi32(src.i, one_twenty7);
	    m.i = _mm_srai_epi32(m.i, 23);
	    m.f = _mm_cvtepi32_ps(m.i);			//  1 <= 2^(-m.i)*X < 2 

	    j.i = _mm_srli_epi32(_mm_slli_epi32(src.i, 9), 9) ; // mantissa part	
	    f_trail.i = _mm_or_si128(j.i, one_twenty7);
    	
	    /*  7 bits of mantissa of src, excluding significand */
	    j.i = _mm_srli_epi32(j.i, 16);

	    // f_lead contains first 7 significant bits of mantissa of src => f_lead = 1 + j.i/128
	    f_lead.i = _mm_slli_epi32( j.i, 16); 
	    f_lead.i = _mm_or_si128(f_lead.i, one_twenty7);
    	
	    // f_trail contains the difference of mantissa and f_lead
	    f_trail.f = _mm_sub_ps(f_trail.f, f_lead.f);	

	    /* u = f/F ; q = u * u * (A1 + u*A2) */
	    u = _mm_div_ps(f_trail.f , f_lead.f );
	    q = _mm_mul_ps(u, _mm_movehl_ps(A1_A2, A1_A2)/*A1: 1/3*/ );
	    q = _mm_add_ps(q, _mm_movelh_ps(A1_A2, A1_A2)/*A2: -0.5*/ );
	    q = _mm_mul_ps(q, u);
	    q = _mm_mul_ps(q, u);

	    int i0 = _mm_extract_epi16(j.i, 0);	//j.u16[0]; 
	    int i1 = _mm_extract_epi16(j.i, 2);	//j.u16[2]; 
	    int i2 = _mm_extract_epi16(j.i, 4);	//j.u16[4]; 
	    int i3 = _mm_extract_epi16(j.i, 6);	//j.u16[6]; 

	    /*j.i = _mm_or_si128(j.i, _mm_srli_epi64(j.i, 24));
	    int i0 = _mm_extract_epi16(j.i, 0); 	
	    int i1 = i0>>8 ; i0 = i0 & 0xff ;		
	    int i2 = _mm_extract_epi16(j.i, 4);
	    int i3 = i2>>8 ; i2 = i2 & 0xff ;*/

	    // l_lead = m* log2_lead + c_lead(j)  
	    l_lead = _mm_mul_ps(log2_lead , m.f);
	    f_lead.f = _mm_set_ps(CBL_LN_TABLES::ln_32f_lead_tbl[i3], CBL_LN_TABLES::ln_32f_lead_tbl[i2],
						    CBL_LN_TABLES::ln_32f_lead_tbl[i1], CBL_LN_TABLES::ln_32f_lead_tbl[i0]);  
	    l_lead = _mm_add_ps(l_lead, f_lead.f);
    			
	    // l_trail = m* log2_trail + c_trail(j) 
	    l_trail = _mm_mul_ps(log2_trail, m.f);
	    f_trail.f = _mm_set_ps(CBL_LN_TABLES::ln_32f_trail_tbl[i3], CBL_LN_TABLES::ln_32f_trail_tbl[i2],
							    CBL_LN_TABLES::ln_32f_trail_tbl[i1], CBL_LN_TABLES::ln_32f_trail_tbl[i0]); 
	    l_trail = _mm_add_ps(l_trail, f_trail.f);

	    /* result = l_lead +(u + (q+l_trail)) */
	    q = _mm_add_ps(l_trail, q);
	    q = _mm_add_ps(q, u);
	    dst.f = _mm_add_ps(q, l_lead);
    }

    CBL_INLINE void ln_A11_special_case(const XMM128 &src, XMM128 &dst)
    {
	    // if src < 0, dst -> nan_32f(0xFFC00000) ;   else src = 0, dst -> -inf(0xFF800000);  else dst
    	
	    XMM128 cmp ;
	    cmp.f = _mm_cmple_ps(src.f, _mm_setzero_ps());
	    dst.i = _mm_andnot_si128(cmp.i, dst.i);
	    cmp.i = _mm_slli_epi32(cmp.i, 23); //neg_inf
    	
	    __m128i m = _mm_slli_epi32( _mm_srli_epi32(src.i, 31), 22);	//nan_32f
	    cmp.i = _mm_or_si128(cmp.i, m);
	    dst.i = _mm_or_si128(dst.i, cmp.i);
    }

    CBL_INLINE void Ln_A11_sse2(const XMM128 &src, XMM128 &dst, 
								       const __m128 &half_sqhalf, const __m128 &p5, 
								       const __m128 &p6_p7, const __m128 &p8_p9,
								       const __m128 &const1_const2)
    {
	    ln_A11_unit(src, dst, half_sqhalf, p5, p6_p7, p8_p9, const1_const2);
	    ln_A11_special_case(src, dst);
    }

    CBL_INLINE void Log10_A11_sse2(const XMM128 &src, XMM128 &dst,
									       const __m128 &hf_sqhf, const __m128 &p5,
									       const __m128 &p6_p7, const __m128 &p8_p9,
									       const __m128 &const1_const2)
    {
	    Ln_A11_sse2(src, dst, hf_sqhf, p5, p6_p7, p8_p9, const1_const2);
	    static const __m128 logE_base10 = CONST_SET1_32F(0.43429448190325176F);
	    dst.f = _mm_mul_ps(dst.f, logE_base10);
    }
} // namespace CBL_Library_Internal

#endif
