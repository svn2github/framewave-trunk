/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _DIV_SQRT_CBRT_H_
#define _DIV_SQRT_CBRT_H_

#include "SSE2Intrinsic.h"
#include "fe.h"

/*
 * Contains implementaitons of Inv, Div, Sqrt, InvSqrt functions with accuracies A11, A21, A24, A50 and A53
 */

namespace OPT_LEVEL
{

namespace CBRT_TABLES{

/*
 * for i = 0, ..., 385
 *   form x(i) with high word 0x3ff00000 + (i << 13) and low word 0;
 *   then TBL[i] = cbrt(x(i)) rounded to 17 significant bits
 */
	static const double __libm_TBL_cbrt[] = {
	 1.00000000000000000e+00, 1.00259399414062500e+00, 1.00518798828125000e+00,
	 1.00775146484375000e+00, 1.01031494140625000e+00, 1.01284790039062500e+00,
	 1.01538085937500000e+00, 1.01791381835937500e+00, 1.02041625976562500e+00,
	 1.02290344238281250e+00, 1.02539062500000000e+00, 1.02786254882812500e+00,
	 1.03031921386718750e+00, 1.03277587890625000e+00, 1.03520202636718750e+00,
	 1.03762817382812500e+00, 1.04003906250000000e+00, 1.04244995117187500e+00,
	 1.04483032226562500e+00, 1.04721069335937500e+00, 1.04959106445312500e+00,
	 1.05194091796875000e+00, 1.05429077148437500e+00, 1.05662536621093750e+00,
	 1.05895996093750000e+00, 1.06127929687500000e+00, 1.06358337402343750e+00,
	 1.06587219238281250e+00, 1.06816101074218750e+00, 1.07044982910156250e+00,
	 1.07270812988281250e+00, 1.07496643066406250e+00, 1.07722473144531250e+00,
	 1.07945251464843750e+00, 1.08168029785156250e+00, 1.08390808105468750e+00,
	 1.08612060546875000e+00, 1.08831787109375000e+00, 1.09051513671875000e+00,
	 1.09269714355468750e+00, 1.09487915039062500e+00, 1.09704589843750000e+00,
	 1.09921264648437500e+00, 1.10136413574218750e+00, 1.10350036621093750e+00,
	 1.10563659667968750e+00, 1.10775756835937500e+00, 1.10987854003906250e+00,
	 1.11198425292968750e+00, 1.11408996582031250e+00, 1.11618041992187500e+00,
	 1.11827087402343750e+00, 1.12034606933593750e+00, 1.12242126464843750e+00,
	 1.12448120117187500e+00, 1.12654113769531250e+00, 1.12858581542968750e+00,
	 1.13063049316406250e+00, 1.13265991210937500e+00, 1.13468933105468750e+00,
	 1.13670349121093750e+00, 1.13871765136718750e+00, 1.14073181152343750e+00,
	 1.14273071289062500e+00, 1.14471435546875000e+00, 1.14669799804687500e+00,
	 1.14868164062500000e+00, 1.15065002441406250e+00, 1.15260314941406250e+00,
	 1.15457153320312500e+00, 1.15650939941406250e+00, 1.15846252441406250e+00,
	 1.16040039062500000e+00, 1.16232299804687500e+00, 1.16424560546875000e+00,
	 1.16616821289062500e+00, 1.16807556152343750e+00, 1.16998291015625000e+00,
	 1.17189025878906250e+00, 1.17378234863281250e+00, 1.17567443847656250e+00,
	 1.17755126953125000e+00, 1.17942810058593750e+00, 1.18128967285156250e+00,
	 1.18315124511718750e+00, 1.18501281738281250e+00, 1.18685913085937500e+00,
	 1.18870544433593750e+00, 1.19055175781250000e+00, 1.19238281250000000e+00,
	 1.19421386718750000e+00, 1.19602966308593750e+00, 1.19786071777343750e+00,
	 1.19966125488281250e+00, 1.20147705078125000e+00, 1.20327758789062500e+00,
	 1.20507812500000000e+00, 1.20686340332031250e+00, 1.20864868164062500e+00,
	 1.21043395996093750e+00, 1.21220397949218750e+00, 1.21397399902343750e+00,
	 1.21572875976562500e+00, 1.21749877929687500e+00, 1.21925354003906250e+00,
	 1.22099304199218750e+00, 1.22274780273437500e+00, 1.22448730468750000e+00,
	 1.22621154785156250e+00, 1.22795104980468750e+00, 1.22967529296875000e+00,
	 1.23138427734375000e+00, 1.23310852050781250e+00, 1.23481750488281250e+00,
	 1.23652648925781250e+00, 1.23822021484375000e+00, 1.23991394042968750e+00,
	 1.24160766601562500e+00, 1.24330139160156250e+00, 1.24497985839843750e+00,
	 1.24665832519531250e+00, 1.24833679199218750e+00, 1.25000000000000000e+00,
	 1.25166320800781250e+00, 1.25332641601562500e+00, 1.25497436523437500e+00,
	 1.25663757324218750e+00, 1.25828552246093750e+00, 1.25991821289062500e+00,
	 1.26319885253906250e+00, 1.26644897460937500e+00, 1.26968383789062500e+00,
	 1.27290344238281250e+00, 1.27612304687500000e+00, 1.27931213378906250e+00,
	 1.28248596191406250e+00, 1.28564453125000000e+00, 1.28878784179687500e+00,
	 1.29191589355468750e+00, 1.29502868652343750e+00, 1.29812622070312500e+00,
	 1.30120849609375000e+00, 1.30427551269531250e+00, 1.30732727050781250e+00,
	 1.31036376953125000e+00, 1.31340026855468750e+00, 1.31640625000000000e+00,
	 1.31941223144531250e+00, 1.32238769531250000e+00, 1.32536315917968750e+00,
	 1.32832336425781250e+00, 1.33126831054687500e+00, 1.33419799804687500e+00,
	 1.33712768554687500e+00, 1.34002685546875000e+00, 1.34292602539062500e+00,
	 1.34580993652343750e+00, 1.34867858886718750e+00, 1.35153198242187500e+00,
	 1.35437011718750000e+00, 1.35720825195312500e+00, 1.36003112792968750e+00,
	 1.36283874511718750e+00, 1.36564636230468750e+00, 1.36842346191406250e+00,
	 1.37120056152343750e+00, 1.37396240234375000e+00, 1.37672424316406250e+00,
	 1.37945556640625000e+00, 1.38218688964843750e+00, 1.38491821289062500e+00,
	 1.38761901855468750e+00, 1.39031982421875000e+00, 1.39302062988281250e+00,
	 1.39569091796875000e+00, 1.39836120605468750e+00, 1.40101623535156250e+00,
	 1.40367126464843750e+00, 1.40631103515625000e+00, 1.40893554687500000e+00,
	 1.41156005859375000e+00, 1.41416931152343750e+00, 1.41676330566406250e+00,
	 1.41935729980468750e+00, 1.42193603515625000e+00, 1.42449951171875000e+00,
	 1.42706298828125000e+00, 1.42962646484375000e+00, 1.43215942382812500e+00,
	 1.43469238281250000e+00, 1.43722534179687500e+00, 1.43974304199218750e+00,
	 1.44224548339843750e+00, 1.44474792480468750e+00, 1.44723510742187500e+00,
	 1.44972229003906250e+00, 1.45219421386718750e+00, 1.45466613769531250e+00,
	 1.45712280273437500e+00, 1.45956420898437500e+00, 1.46200561523437500e+00,
	 1.46444702148437500e+00, 1.46687316894531250e+00, 1.46928405761718750e+00,
	 1.47169494628906250e+00, 1.47409057617187500e+00, 1.47648620605468750e+00,
	 1.47886657714843750e+00, 1.48124694824218750e+00, 1.48361206054687500e+00,
	 1.48597717285156250e+00, 1.48834228515625000e+00, 1.49067687988281250e+00,
	 1.49302673339843750e+00, 1.49536132812500000e+00, 1.49768066406250000e+00,
	 1.50000000000000000e+00, 1.50230407714843750e+00, 1.50460815429687500e+00,
	 1.50691223144531250e+00, 1.50920104980468750e+00, 1.51148986816406250e+00,
	 1.51376342773437500e+00, 1.51603698730468750e+00, 1.51829528808593750e+00,
	 1.52055358886718750e+00, 1.52279663085937500e+00, 1.52503967285156250e+00,
	 1.52728271484375000e+00, 1.52951049804687500e+00, 1.53173828125000000e+00,
	 1.53395080566406250e+00, 1.53616333007812500e+00, 1.53836059570312500e+00,
	 1.54055786132812500e+00, 1.54275512695312500e+00, 1.54493713378906250e+00,
	 1.54711914062500000e+00, 1.54928588867187500e+00, 1.55145263671875000e+00,
	 1.55361938476562500e+00, 1.55577087402343750e+00, 1.55792236328125000e+00,
	 1.56005859375000000e+00, 1.56219482421875000e+00, 1.56433105468750000e+00,
	 1.56645202636718750e+00, 1.56857299804687500e+00, 1.57069396972656250e+00,
	 1.57279968261718750e+00, 1.57490539550781250e+00, 1.57699584960937500e+00,
	 1.57908630371093750e+00, 1.58117675781250000e+00, 1.58325195312500000e+00,
	 1.58532714843750000e+00, 1.58740234375000000e+00, 1.59152221679687500e+00,
	 1.59562683105468750e+00, 1.59970092773437500e+00, 1.60375976562500000e+00,
	 1.60780334472656250e+00, 1.61183166503906250e+00, 1.61582946777343750e+00,
	 1.61981201171875000e+00, 1.62376403808593750e+00, 1.62770080566406250e+00,
	 1.63162231445312500e+00, 1.63552856445312500e+00, 1.63941955566406250e+00,
	 1.64328002929687500e+00, 1.64714050292968750e+00, 1.65097045898437500e+00,
	 1.65476989746093750e+00, 1.65856933593750000e+00, 1.66235351562500000e+00,
	 1.66610717773437500e+00, 1.66986083984375000e+00, 1.67358398437500000e+00,
	 1.67729187011718750e+00, 1.68098449707031250e+00, 1.68466186523437500e+00,
	 1.68832397460937500e+00, 1.69197082519531250e+00, 1.69560241699218750e+00,
	 1.69921875000000000e+00, 1.70281982421875000e+00, 1.70640563964843750e+00,
	 1.70997619628906250e+00, 1.71353149414062500e+00, 1.71707153320312500e+00,
	 1.72059631347656250e+00, 1.72410583496093750e+00, 1.72760009765625000e+00,
	 1.73109436035156250e+00, 1.73455810546875000e+00, 1.73800659179687500e+00,
	 1.74145507812500000e+00, 1.74488830566406250e+00, 1.74829101562500000e+00,
	 1.75169372558593750e+00, 1.75508117675781250e+00, 1.75846862792968750e+00,
	 1.76182556152343750e+00, 1.76516723632812500e+00, 1.76850891113281250e+00,
	 1.77183532714843750e+00, 1.77514648437500000e+00, 1.77844238281250000e+00,
	 1.78173828125000000e+00, 1.78500366210937500e+00, 1.78826904296875000e+00,
	 1.79151916503906250e+00, 1.79476928710937500e+00, 1.79798889160156250e+00,
	 1.80120849609375000e+00, 1.80441284179687500e+00, 1.80760192871093750e+00,
	 1.81079101562500000e+00, 1.81396484375000000e+00, 1.81712341308593750e+00,
	 1.82026672363281250e+00, 1.82341003417968750e+00, 1.82653808593750000e+00,
	 1.82965087890625000e+00, 1.83276367187500000e+00, 1.83586120605468750e+00,
	 1.83894348144531250e+00, 1.84201049804687500e+00, 1.84507751464843750e+00,
	 1.84812927246093750e+00, 1.85118103027343750e+00, 1.85421752929687500e+00,
	 1.85723876953125000e+00, 1.86026000976562500e+00, 1.86326599121093750e+00,
	 1.86625671386718750e+00, 1.86924743652343750e+00, 1.87222290039062500e+00,
	 1.87518310546875000e+00, 1.87814331054687500e+00, 1.88108825683593750e+00,
	 1.88403320312500000e+00, 1.88696289062500000e+00, 1.88987731933593750e+00,
	 1.89279174804687500e+00, 1.89569091796875000e+00, 1.89859008789062500e+00,
	 1.90147399902343750e+00, 1.90435791015625000e+00, 1.90722656250000000e+00,
	 1.91007995605468750e+00, 1.91293334960937500e+00, 1.91577148437500000e+00,
	 1.91860961914062500e+00, 1.92143249511718750e+00, 1.92425537109375000e+00,
	 1.92706298828125000e+00, 1.92985534667968750e+00, 1.93264770507812500e+00,
	 1.93544006347656250e+00, 1.93821716308593750e+00, 1.94097900390625000e+00,
	 1.94374084472656250e+00, 1.94650268554687500e+00, 1.94924926757812500e+00,
	 1.95198059082031250e+00, 1.95471191406250000e+00, 1.95742797851562500e+00,
	 1.96014404296875000e+00, 1.96286010742187500e+00, 1.96556091308593750e+00,
	 1.96824645996093750e+00, 1.97093200683593750e+00, 1.97361755371093750e+00,
	 1.97628784179687500e+00, 1.97894287109375000e+00, 1.98159790039062500e+00,
	 1.98425292968750000e+00, 1.98689270019531250e+00, 1.98953247070312500e+00,
	 1.99215698242187500e+00, 1.99478149414062500e+00, 1.99739074707031250e+00,
	 2.00000000000000000e+00,
	};
}



/*
 *	UTLIITY FUNCTIONS
 */

//src is expected to be +ve.
IS __m128i divd_exponent_by_3(const __m128i &src)
{
//	q1 = (a + (a>>2) ) >> 2 ; q2 = (q1 + (q1 >> 4) + a ) >>6; reslt = q1+q2
// above is equivalent to  multiplying with 0.01010101010101010 i.e, 1/3

	__m128i q1 = _mm_srli_epi64(src, 2);
	q1 = _mm_add_epi64(src, q1);
	q1 = _mm_srli_epi64(q1, 2);

	__m128i q2 = _mm_add_epi64(q1, _mm_srli_epi64(q1, 4));
	q2 = _mm_add_epi64(src, q2);
	q2 = _mm_srli_epi64(q2, 6);
	q2 = _mm_add_epi64(q1, q2);

	return q2;
}

/*
 *		REFERENCE UNIT FUNCTIONS
 */

IS float ref_inv_cbrtf(float x)
{
#if defined( WIN32 ) || defined( _WIN32 ) ||  defined( WIN64 ) || defined( _WIN64 )
    float x_by_3 ;

    int sign = 0;
    int ix = *(int*) &x ;
    sign = (ix>>31) << 31 ;

    if ( x == 0)
		return 1/x ;

    ix = ix ^ sign ;
    if( ix >= 0x7f800000)
        return 1/x ;  

    x_by_3 = x * 0.3333333333F ;

    ix = 0x54A14517 - ix/3 ;

    ix = ix ^ sign ;
    x = *(float*) &ix ;

    x =  (1.3333333333F - x_by_3 * x*x*x)*x ;
    x =  (1.3333333333F - x_by_3 * x*x*x)*x ;
    x =  (1.3333333333F - x_by_3 * x*x*x)*x ;
    x =  (1.3333333333F - x_by_3 * x*x*x)*x ;
    x =  (float)((1.3333333333333333333333 - (double)x_by_3 * x*x*x)*x );
    return x ;
#else
    return 1.0f/cbrtf(x);
#endif
}

IS float ref_cbrtf(const float &src)
{
#if defined( WIN32 ) || defined( _WIN32 ) ||  defined( WIN64 ) || defined( _WIN64 )

    int sign = 0;
	float x = src ;
    int ix = *(int*) &x ;
    sign = (ix>>31) << 31 ;

    if ( x == 0 || ix >= 0x7f800000)
		return x ;

	ix = ix ^ sign ;
    ix = 0x2A4B2FF5 + ix/3 ;
    ix = ix ^ sign ;
    x = *(float*) &ix ;

    x =  0.3333333333F * (x + x + src/(x*x));
    x =  0.3333333333F * (x + x + src/(x*x));
	x =  0.3333333333F * (x + x + src/(x*x));
	x =  0.3333333333F * (x + x + src/(x*x));

    return x ;
#else
    return cbrtf(src);
#endif
}
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

#define HI	1
#define LO	0

static double ref_cbrtd(double in) {

	 const unsigned 	B1 = 715094163 ; /* B1 = (682-0.03306235651)*2**20 */
						
	 const double	C =  5.42857142857142815906e-01, /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
					D = -7.05306122448979611050e-01, /* -864/1225 = 0xBFE691DE, 0x2532C834 */
					E =  1.41428571428571436819e+00, /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
					F =  1.60714285714285720630e+00, /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
					G =  3.57142857142857150787e-01; /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */
	int	hx;
	double r,s,w;
	unsigned sign;
	union {
		unsigned int	i[2];
		double			d;
	} x, t;

	x.d = in ;
	t.d = 0.0 ;

	if(in == 0.0) return 0;
	sign = x.i[HI] & 0x80000000 ;
	x.i[HI] ^= sign ;
	hx = x.i[HI];	
    t.i[HI]=hx/3+B1;	

    /* new cbrt to 23 bits, may be implemented in single precision */
	r=t.d*t.d/x.d;
	s=C+r*t.d;
	t.d *= G+(F*s)/((s+E)*s+D);	

    /* chopped to 20 bits and make it larger than cbrt(x) */ 
	t.i[LO]=0; t.i[HI]+=0x00000001;


    /* one step newton iteration to 53 bits with error less than 0.667 ulps */

	s = t.d*t.d;		/* t*t is exact */
	r = x.d/s;
	w = t.d+t.d;
	r = (r-t.d)/(w+r);	/* r-s is exact */
	t.d = t.d + t.d * r ;

    /* retore the sign bit */
	t.i[HI] |= sign;
	return(t.d);
}
#undef HI
#undef LO


/*
 *		REFERENCE FUNCTION CALLS
 */

ISV InvSqrtf_ref(const Fw32f &src, Fw32f &dst) 
{
	dst = 1.0F/sqrtf(src);
}

ISV InvSqrtd_ref(const Fw64f &src, Fw64f &dst) 
{
	dst = 1.0/sqrt(src);
}

ISV Cbrtf_ref(const Fw32f &src, Fw32f &dst) 
{
	dst = ref_cbrtf(src);
}

ISV Cbrtd_ref(const Fw64f &src, Fw64f &dst) 
{
	dst = ref_cbrtd(src);
}

ISV InvCbrtf_ref(const Fw32f &src, Fw32f &dst) 
{
	dst = ref_inv_cbrtf(src) ;
}

ISV InvCbrtd_ref(const Fw64f &src, Fw64f &dst) 
{
	dst = 1/ref_cbrtd(src);
}

ISV Cbrt_32s16s_ref( const Fw32s &src, Fw16s &dst, Fw32f scale )
{
	if(src == 0) { dst = 0; return ;}
	dst = FW_REF::Limits<Fw16s>::Sat( CBL_LIBRARY::FloatToInt(ref_cbrtf((Fw32f)src) * scale ));
}

/*
 *		SSE2 SETUP FUNCTIONS
 */ 

ISV invSqrtd_setup(__m128d &half, __m128d &three_by_2, __m128i &init_const)
{
	half		= _mm_set1_pd(0.5);
	three_by_2	= _mm_set1_pd(1.5);
	init_const	= _mm_set_epi32(0x5FE6A09E, 0x667F3BCD, 0x5FE6A09E, 0x667F3BCD);
}	

ISV cbrtf_setup(__m128i &init_const, __m128 &four_by_3, __m128 &one_by_3)
{
	init_const	= _mm_set1_epi32(0x54A14517);
	four_by_3	= _mm_set1_ps(/*4/3.0F*/1.33333333F);
	one_by_3	= _mm_set1_ps(/*1/3.0F*/0.33333333F);
}

ISV cbrtd_setup(__m128i &init_const, __m128d &p1_p2, __m128d &p3_p4, __m128d &p5_p6)
{
	init_const =_mm_set_epi32(0x2AA00000, 0,0x2AA00000,0 ); 
 /*
 * The polynomial p(x) := p1*x + p2*x^2 + ... + p6*x^6 satisfies 
 * |(1+x)^(1/3) - 1 - p(x)| < 2^-63  for |x| < 0.003914
 */
	p1_p2 = _mm_set_pd( 3.3333333333333334073562e-0001,	-1.11111111111111111992797989e-0001);
	p3_p4 = _mm_set_pd( 6.1728395057850669571030e-0002, -4.11522633731005164138964638e-0002);
	p5_p6 = _mm_set_pd(  3.017883431052687281517e-0002,	-2.34723340038386971009665073e-0002);
}

ISV cbrtd_libm_setup(__m128i &init_const, __m128d &c_d, __m128d &e_f, __m128d &const_1_g)
{
	init_const	=  _mm_set_epi32(715094163, 0, 715094163, 0);
	c_d			= _mm_set_pd(5.42857142857142815906e-01, -7.05306122448979611050e-01); // c, d
	e_f			= _mm_set_pd(1.41428571428571436819e+00, 1.60714285714285720630e+00); // e, f
	const_1_g	= _mm_set_pd(1.0, 3.57142857142857150787e-01);	//1, g
}

ISV inv_cbrtd_setup(__m128i &init_const, __m128d &four_by_3, __m128d &one_by_3)
{
	init_const	= _mm_set_epi32(0x55400000, 0, 0x55400000, 0);
	four_by_3	= _mm_set1_pd(1.333333333333333/*4/3.0*/);	
	one_by_3	= _mm_set1_pd(0.333333333333333/*1/3.0*/);	
}

ISV cbrt_32s16s_setup( float scale, __m128i &init_const, __m128 &four_by_3,
					  __m128 &one_by_3, __m128 &one_byscale_cube)
{
	init_const	= _mm_set1_epi32(0x54A14517); 
	four_by_3	= _mm_set1_ps(1.33333333F);
	one_by_3	= _mm_set1_ps(0.33333333F);
	one_byscale_cube	= _mm_set1_ps(1.0F/(scale*scale*scale));
}	

/*
 *		SSE2 UNIT FUNCTIONS
 */

ISV invSqrtd_iter(__m128d &dst, const __m128d &half_src, const __m128d &three_by_2)
{	
	// Newton iteration (1.5*old - (x/2)*old^3)
	__m128d tmp = _mm_mul_pd(half_src, dst);
	tmp = _mm_mul_pd(tmp, dst);
	tmp = _mm_sub_pd(three_by_2, tmp);
	dst = _mm_mul_pd(dst, tmp);
}

/*
InvSqrt (x<0)           NAN_64F
InvSqrt (x=+0)          INF_64F
InvSqrt (x=-0)          -INF_64F
InvSqrt (x=-INF)        NAN_64F
*/ 

ISV invSqrtd_special_case(const XMM128 &src, XMM128 &dst)
{
	XMM128 cmp ;
	cmp.d = _mm_cmple_pd(src.d, _mm_setzero_pd());
	dst.i = _mm_andnot_si128(cmp.i, dst.i);
	cmp.i =  _mm_srli_epi64(_mm_slli_epi64(cmp.i ,53), 1);       
	cmp.i = _mm_or_si128(cmp.i, _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 51));
	dst.i = _mm_or_si128(dst.i, cmp.i);	
}

//cbrt

ISV cbrtf_last_iter(const __m128 &src, __m128 &dst) 
{
	//	r = src/(x^2) ; new = x +  x*(r-x)/(2x+r), where x is prior estimate

	__m128 r =  _mm_mul_ps(dst, dst);
	r = _mm_div_ps(src,r);
	__m128 w = _mm_add_ps(dst, dst);
	w = _mm_add_ps(w, r);
	r = _mm_sub_ps(r, dst);
	r = _mm_mul_ps(dst, r);
	r = _mm_div_ps(r, w);
	dst = _mm_add_ps(dst, r);
}

// src is assumed to be +ve
SYS_INLINE  STATIC void cbrtd_new_unit(const XMM128 &src, XMM128 &dst, const __m128i &init_const , const __m128d &p1_p2, 
								   const __m128d &p3_p4, const __m128d &p5_p6)
{
	XMM128 xx, yy;
	__m128i hx, j, ex ;
	__m128d u, w, t ;

	ex = _mm_srli_epi64(src.i, 52);
	int d1 = _mm_extract_epi16(ex, 4)/3;
	int d2 = _mm_extract_epi16(ex, 0)/3;
	ex = _mm_insert_epi16( ex, d1, 4);
	ex = _mm_insert_epi16( ex, d2, 0);
	j = _mm_slli_epi64(ex, 52);

	hx = _mm_sub_epi64(src.i, j);
	hx = _mm_sub_epi64(hx, j);
	hx = _mm_sub_epi64(hx, j);

	ex = _mm_srli_epi64(hx, 13);
	u = _mm_set_pd(CBRT_TABLES::__libm_TBL_cbrt[_mm_extract_epi16(ex, 6)],
					CBRT_TABLES::__libm_TBL_cbrt[_mm_extract_epi16(ex, 2)]);

	xx.i = _mm_add_epi32(init_const, _mm_srli_epi32(init_const, 1)); //_mm_set_epi32(0x3ff00000, 0, 0x3ff00000, 0)
	xx.i = _mm_add_epi32(xx.i, hx ); 

	w = _mm_mul_pd(u, u);
	w = _mm_mul_pd(w, u);
	t = _mm_sub_pd(xx.d, w);
	t = _mm_div_pd(t, w);

	yy.i = _mm_add_epi64(j, init_const);
	yy.i = _mm_srli_epi64(yy.i, 32);
	yy.i = _mm_slli_epi64(yy.i, 32);

	w = _mm_mul_pd(t, t);
	__m128d tmp ;
	dst.d = _mm_mul_pd(_mm_unpacklo_pd(p5_p6, p5_p6), w);
	tmp = _mm_mul_pd(_mm_unpackhi_pd(p5_p6, p5_p6), t);
	dst.d = _mm_add_pd(dst.d, tmp);
	dst.d = _mm_add_pd(dst.d, _mm_unpacklo_pd(p3_p4, p3_p4));
	tmp = _mm_mul_pd(w, w);
	dst.d = _mm_mul_pd(dst.d, tmp);

	w = _mm_mul_pd(w, _mm_unpackhi_pd(p3_p4, p3_p4));
	tmp = _mm_mul_pd(t, _mm_unpacklo_pd(p1_p2, p1_p2));
	tmp = _mm_add_pd(tmp, w);
	tmp = _mm_add_pd(tmp, _mm_unpackhi_pd(p1_p2, p1_p2));
	tmp = _mm_mul_pd(tmp, t);
	
	dst.d = _mm_add_pd(dst.d, tmp);
	dst.d = _mm_mul_pd(dst.d, u);
	dst.d = _mm_add_pd(dst.d, u);
	dst.d = _mm_mul_pd(dst.d, yy.d);
}

ISV Cbrt_A53_sse2(const XMM128 &src, XMM128 &dst,
				  const __m128i &init_const, const __m128d &p1_p2, 
				  const __m128d &p3_p4, const __m128d &p5_p6)
{
	XMM128 tsrc ;
	const __m128i sign = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63);
	tsrc.i = _mm_xor_si128(src.i, sign);

	cbrtd_new_unit(tsrc, dst, init_const, p1_p2, p3_p4, p5_p6);
	dst.i = _mm_or_si128(dst.i, sign);

	//if src == 0, return 0
	XMM128 cmp;
	cmp.d = _mm_cmpeq_pd(src.d, _mm_setzero_pd());
	dst.d = _mm_andnot_pd(cmp.d, dst.d);
}

//  InvCbrt 
//	Implemented using Newton-Raphson's method

ISV inv_cbrtd_init(const XMM128 &src, XMM128 &estimate, const __m128i &init_const)
{
	// if src = (2^e)*f, take 2^(-e/3) as the initial guess for inv_cbrt(src)
	estimate.i = divd_exponent_by_3(src.i);
	estimate.i = _mm_sub_epi64(init_const, estimate.i);
}

ISV inv_cbrtd_iter(const __m128d &src_by_3, __m128d &dst, const __m128d &four_by_three)
{
	__m128d temp; 

//	new = (4/3 - (src/3)*old^3)*old
// dont do old^3 directly, that could turn out to be a denormalized number, which eats up LOTS of cycles.

	temp = _mm_mul_pd(dst, dst);
	temp = _mm_mul_pd(temp, src_by_3);
	temp = _mm_mul_pd(temp, dst);
	temp = _mm_sub_pd(four_by_three, temp);
	dst = _mm_mul_pd(dst, temp);
}

ISV inv_cbrtd_last_iter(const __m128d &src, __m128d &dst)
{
	STATIC const __m128d one = CONST_SET1_64F(1.0);
	__m128d r, num, denom;

	r = _mm_mul_pd(src, dst);
	r = _mm_mul_pd(r, dst);
	r = _mm_mul_pd(r, dst);
	
	num =  _mm_add_pd(one, one);
	num = _mm_add_pd(num, r);
	num = _mm_mul_pd(num, dst);
	denom = _mm_add_pd(r,r);
	denom = _mm_add_pd(denom, one);

	dst = _mm_div_pd(num, denom);
}

ISV invCbrtd_unit( XMM128 &src, XMM128 &dst, const __m128i &init_const,
									 const __m128d &four_by_three , const __m128d &one_by_3)
{
	//get the initial guess
	inv_cbrtd_init(src, dst,  init_const);
	src.d = _mm_mul_pd(src.d, one_by_3);

	inv_cbrtd_iter(src.d, dst.d,  four_by_three);
	inv_cbrtd_iter(src.d, dst.d,  four_by_three);
	inv_cbrtd_iter(src.d, dst.d,  four_by_three);
	inv_cbrtd_iter(src.d, dst.d,  four_by_three);
	inv_cbrtd_iter(src.d, dst.d,  four_by_three);
}

/*
InvCbrt (x=+0)      INF_64F
InvCbrt (x=-0)      -INF_64F
*/
ISV inv_cbrtd_special_case( const XMM128 &src, XMM128 &dst)
{
	//if src == 0, return inf
	XMM128 cmp;
	cmp.d = _mm_cmpeq_pd(src.d, _mm_setzero_pd());
	dst.d = _mm_andnot_pd(cmp.d, dst.d);
	cmp.i = _mm_slli_epi64(_mm_srli_epi64(cmp.i, 53), 52) ;
	dst.d = _mm_or_pd(dst.d, cmp.d);
}

ISV cbrtd_libm_unit(const XMM128 &src, XMM128 &dst, const __m128i &init_const
									   , const __m128d &const_C_D, const __m128d &const_E_F
									   , const __m128d &one_const_G)
{
	XMM128 t;
	__m128d s, r, w; 

//	t.i = divd_exponent_by_3(src.i);
	t.i = _mm_set_epi32(src.u32[3]/3, 0, src.u32[1]/3, 0);
	t.i = _mm_add_epi64(t.i, init_const);
	r = _mm_mul_pd(t.d, t.d);
	r = _mm_div_pd(r, src.d);
	s = _mm_mul_pd(r, t.d);
	s = _mm_add_pd(s, _mm_unpackhi_pd(const_C_D, const_C_D) ); //C

//	t.d *= G+(F*s)/((s+E)*s+D);	

	__m128d u, v ;
	v = _mm_add_pd(s, _mm_unpackhi_pd(const_E_F, const_E_F)); //E
	v = _mm_mul_pd(v, s);
	v = _mm_add_pd(v, _mm_unpacklo_pd(const_C_D, const_C_D)); //D
	u = _mm_mul_pd(_mm_unpacklo_pd(const_E_F, const_E_F), s); //F
	u = _mm_div_pd(u, v);
	u = _mm_add_pd(u, _mm_unpacklo_pd(one_const_G, one_const_G));	//G
	t.d = _mm_mul_pd(t.d, u);
	t.i = _mm_slli_epi64(_mm_srli_epi64(t.i, 32), 32);
	t.i = _mm_add_epi32(t.i, _mm_srli_epi32(init_const, 29)); //_mm_set_epi32(1, 0, 1, 0));

	s = _mm_mul_pd(t.d, t.d);
	r = _mm_div_pd(src.d, s);
	w = _mm_add_pd(t.d, t.d);
	u = _mm_sub_pd(r, t.d);
	v = _mm_add_pd(w, r);
	r = _mm_div_pd(u, v);
	r = _mm_mul_pd(r, t.d);
	dst.d = _mm_add_pd(t.d, r);	
}

/*
 *		SSE2 FUNCTION CALLS
 */ 

ISV InvSqrt_A21_sse2(const XMM128 &src, XMM128 &dst, const __m128 &half, const __m128 &three_by_2)
{
	dst.f = _mm_rsqrt_ps(src.f);
	__m128 half_src = _mm_mul_ps(src.f, half);

	// Newton iteration 
	CBL_LIBRARY::CBL_Library_Internal::invSqrtf_iter(dst.f, half_src, three_by_2);
	CBL_LIBRARY::CBL_Library_Internal::invSqrtf_iter(dst.f, half_src, three_by_2);

	CBL_LIBRARY::CBL_Library_Internal::invSqrtf_special_case(src, dst );
}

ISV InvSqrt_A24_sse2(const XMM128 &src, XMM128 &dst, const __m128 &one)
{
	dst.f = _mm_div_ps(one, _mm_sqrt_ps(src.f));
}

ISV InvSqrt_A50_sse2(const XMM128 &src, XMM128 &dst, 
					 const __m128d &half, const __m128d &three_by_2, const __m128i &init_const)
{
	dst.i = _mm_sub_epi64(init_const, _mm_srli_epi64(src.i, 1));
	__m128d half_src = _mm_mul_pd(src.d, half);

	// Newton iteration 
	invSqrtd_iter(dst.d, half_src, three_by_2);
	invSqrtd_iter(dst.d, half_src, three_by_2);
	invSqrtd_iter(dst.d, half_src, three_by_2);
	invSqrtd_iter(dst.d, half_src, three_by_2);

	invSqrtd_special_case(src, dst );
}

ISV InvSqrt_A53_sse2(const XMM128 &src, XMM128 &dst, const __m128d &one)
{
	dst.d = _mm_div_pd(one, _mm_sqrt_pd(src.d));
}

ISV Cbrt_A21_sse2(const XMM128 &src, XMM128 &dst, 
				  const __m128i &init_const, const __m128 &four_by_3, const __m128 &one_by_3)
{
	const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	const __m128 one = _mm_sub_ps(four_by_3, one_by_3);	// 4/3 - 1/3 = 1

	XMM128 tsrc ;
	tsrc.i = _mm_xor_si128(src.i, sign);

	CBL_LIBRARY::CBL_Library_Internal::inv_cbrt_unit_2iter(tsrc, dst, init_const, four_by_3, one_by_3);
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, dst,  four_by_3);
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, dst,  four_by_3);

	dst.f = _mm_div_ps(one, dst.f);
	CBL_LIBRARY::CBL_Library_Internal::cbrt_special_case(src.f, dst.f);
	dst.i = _mm_or_si128(dst.i, sign);
}

ISV Cbrt_A24_sse2(const XMM128 &src, XMM128 &dst, 
				  const __m128i &init_const, const __m128 &four_by_3, const __m128 &one_by_3)
{
	const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	XMM128 tsrc, tdst, ts;
	tsrc.i = _mm_xor_si128(src.i, sign);
	ts.i = tsrc.i;

	CBL_LIBRARY::CBL_Library_Internal::inv_cbrt_unit_2iter(tsrc, tdst, init_const, four_by_3, one_by_3);
	tdst.f = _mm_rcp_ps(tdst.f);	// cbrt from inv_cbrt
	cbrtf_last_iter(ts.f, tdst.f);

	CBL_LIBRARY::CBL_Library_Internal::cbrt_special_case(src.f, tdst.f);
	dst.i = _mm_or_si128(tdst.i, sign);
}

ISV Cbrt_A50_sse2(const XMM128 &src, XMM128 &dst, 
				  const __m128i &init_const, const __m128d &four_by_3, const __m128d &one_by_3)
{
	const __m128i sign = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63);
	const __m128d one = _mm_sub_pd(four_by_3, one_by_3);
	XMM128 tsrc ;

	tsrc.i = _mm_xor_si128(src.i, sign);	
	tsrc.d = _mm_div_pd(one, tsrc.d);

	invCbrtd_unit(tsrc, dst, init_const, four_by_3, one_by_3);

	//if src == 0, return 0
	XMM128 cmp;
	cmp.d = _mm_cmpeq_pd(src.d, _mm_setzero_pd());
	dst.d = _mm_andnot_pd(cmp.d, dst.d);
	dst.i = _mm_or_si128(dst.i, sign);
}


//Inv Cbrt

ISV InvCbrt_A21_sse2(const XMM128 &src, XMM128 &dst, 
					 const __m128i &init_const, const __m128 &four_by_3, const __m128 &one_by_3)
{
	XMM128 tsrc ;
	const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	tsrc.i = _mm_xor_si128(src.i, sign);	

	CBL_LIBRARY::CBL_Library_Internal::inv_cbrt_unit_2iter(tsrc, dst, init_const, four_by_3, one_by_3);		
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, dst, four_by_3);
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, dst, four_by_3);
	dst.i = _mm_or_si128(dst.i, sign);

	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_special_case( src, dst);
}

ISV InvCbrt_A24_sse2(const XMM128 &src, XMM128 &dst, 
					 const __m128i &init_const, const __m128 &four_by_3, 
					 const __m128 &one_by_3, const __m128d &four_by_3d)
{
	XMM128 tsrc ;
	const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	
	tsrc.i = _mm_xor_si128(src.i, sign);	
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrt_unit_2iter(tsrc, dst, init_const, four_by_3, one_by_3);	
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, dst, four_by_3);

	// do last iteration in double to get required accuracy!
	XMM128 s_lo, s_hi, d_lo, d_hi ;

	_mm_cvt32f_64f(tsrc, s_lo, s_hi);
	_mm_cvt32f_64f(dst, d_lo, d_hi);

	inv_cbrtd_iter(s_lo.d, d_lo.d, four_by_3d);
	inv_cbrtd_iter(s_hi.d, d_hi.d, four_by_3d);
	
	d_lo.f = _mm_cvtpd_ps(d_lo.d);
	d_hi.f = _mm_cvtpd_ps(d_hi.d);
	dst.f = CBL_OPTLEVEL::Shuffle_b1b0a1a0_32f( d_lo.f, d_hi.f);

	dst.i = _mm_or_si128(dst.i, sign);

	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_special_case( src, dst);
}

ISV InvCbrt_A50_sse2(const XMM128 &src, XMM128 &dst, 
				  const __m128i &init_const, const __m128d &four_by_3, const __m128d &one_by_3)
{
	XMM128 tsrc ;
	const __m128i sign = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63);
	tsrc.i = _mm_xor_si128(src.i, sign);	

	invCbrtd_unit(tsrc, dst, init_const, four_by_3, one_by_3);
	dst.i = _mm_or_si128(dst.i, sign);

	inv_cbrtd_special_case( src, dst);
}

ISV InvCbrt_A53_sse2(const XMM128 &src, XMM128 &dst, 
				  const __m128i &init_const, const __m128d &four_by_3, const __m128d &one_by_3)
{
	const __m128i sign = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63);
	XMM128 tsrc, ts, tdst ;

	tsrc.i = _mm_xor_si128(src.i, sign);	
	ts.i = tsrc.i ;
	invCbrtd_unit(tsrc, tdst, init_const, four_by_3, one_by_3);
	inv_cbrtd_last_iter(ts.d, tdst.d);
	dst.i = _mm_or_si128(tdst.i, sign);

	inv_cbrtd_special_case( src, dst);
}

ISV InvCbrt_A53_libm_SSE2(const XMM128 &src, XMM128 &dst, 
						  const __m128i &init_const, const __m128d &const_C_D, 
						  const __m128d &const_E_F, const __m128d &const_1_G)
{
	XMM128 tsrc ;
	const __m128i sign = _mm_slli_epi64(_mm_srli_epi64(src.i, 63), 63);
	tsrc.i = _mm_xor_si128(src.i, sign);	

	cbrtd_libm_unit(tsrc, dst, init_const, const_C_D, const_E_F, const_1_G);
    dst.d = _mm_div_pd(_mm_unpackhi_pd(const_1_G, const_1_G), dst.d); // dst <- 1/dst
    dst.i = _mm_or_si128(dst.i, sign);

	inv_cbrtd_special_case( src, dst);

}

SYS_INLINE STATIC void Cbrt_32s16s_sse2(const XMM128 &src, XMM128 &dst, 
										const __m128i &init_const, const __m128 &four_by_3,
										const __m128 &one_by_3, const __m128 &one_byscale_cube)
{
	const __m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);

	XMM128 tsrc, tdst, cmp  ;
	tsrc.f = _mm_cvtepi32_ps(src.i);	
	cmp.f = _mm_cmpeq_ps(tsrc.f, _mm_setzero_ps());
	tsrc.i = _mm_xor_si128(tsrc.i, sign);	
	
	tsrc.f = _mm_div_ps(one_byscale_cube, tsrc.f);
	
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrt_unit_2iter(tsrc, tdst, init_const, four_by_3, one_by_3);
	CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_iter(tsrc.f, tdst,  four_by_3);
	tdst.i = _mm_andnot_si128(cmp.i, tdst.i);

	tdst.i = _mm_or_si128(tdst.i, sign);
	tdst.i = _mm_cvtps_epi32(tdst.f);
	dst.i = _mm_packs_epi32(tdst.i, tdst.i);
}

/*
 * STRUCTURES for Inv, Div, Sqrt, InvSqrt functions
 */

//Inv
struct Inv_32f : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF

	XMM128 one ;

	Inv_32f(){}

	IV SSE2_Init()
	{
		one.f = _mm_set1_ps(1.0F);
	}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].f = _mm_div_ps(one.f, r.src1[0].f);
	 }
	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 d[0] = 1.0F/s[0];
	 } 
};

struct Inv_64f : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF

	XMM128 one ;

	Inv_64f(){}

	IV SSE2_Init()
	{
		one.d = _mm_set1_pd(1.0);
	}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].d = _mm_div_pd(one.d, r.src1[0].d);
	 }
	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 d[0] = 1.0/s[0];
	 } 
};

//Div
struct Div_32f : public fe3<Fw32f, C1, Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF

	Div_32f(){}

	IV SSE2_Init(){}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].f = _mm_div_ps(r.src1[0].f, r.src2[0].f);
	 }
	 IV REFR( const Fw32f *s1, const Fw32f *s2, Fw32f *d) const                     
	 {         
		 d[0] = s1[0]/s2[0];
	 } 
};

struct Div_64f : public fe3<Fw64f, C1, Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF

	Div_64f(){}

	IV SSE2_Init(){}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].d = _mm_div_pd(r.src1[0].d, r.src2[0].d);
	 }
	 IV REFR( const Fw64f *s1, const Fw64f *s2, Fw64f *d) const                     
	 {         
		 d[0] = s1[0]/s2[0];
	 } 
};

//Sqrt
struct Sqrt_32f : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF

	Sqrt_32f(){}

	IV SSE2_Init(){}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].f = _mm_sqrt_ps(r.src1[0].f);
	 }
	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 d[0] = sqrtf(s[0]);
	 } 
};

struct Sqrt_64f : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF

	Sqrt_64f(){}

	IV SSE2_Init(){}

	 IV SSE2( RegFile & r ) const                     
	 {      
		 r.dst[0].d = _mm_sqrt_pd(r.src1[0].d);
	 }
	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 d[0] = sqrt(s[0]);
	 } 
};

//Inverse Sqrt
struct InvSqrt_A11 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 half, three_by_2 ;

	InvSqrt_A11(){}

	IV SSE2_Init()
	{
        CBL_LIBRARY::CBL_Library_Internal::invSqrtf_setup(half.f, three_by_2.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 CBL_LIBRARY::CBL_Library_Internal::InvSqrt_A11_sse2(r.src1[0], r.dst[0], half.f, three_by_2.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvSqrtf_ref(s[0], d[0]);
	 } 
};

struct InvSqrt_A21 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 half, three_by_2 ;

	InvSqrt_A21(){}

	IV SSE2_Init()
	{
		CBL_LIBRARY::CBL_Library_Internal::invSqrtf_setup(half.f, three_by_2.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvSqrt_A21_sse2(r.src1[0], r.dst[0], half.f, three_by_2.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvSqrtf_ref(s[0], d[0]);
	 } 
};
	
struct InvSqrt_A24 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 one ;

	InvSqrt_A24(){}

	IV SSE2_Init()
	{
		one.f = _mm_set1_ps(1.0F);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvSqrt_A24_sse2(r.src1[0], r.dst[0], one.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvSqrtf_ref(s[0], d[0]);
	 } 
};

struct InvSqrt_A50 : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF
	XMM128 half, three_by_2, init_const ;

	InvSqrt_A50(){}

	IV SSE2_Init()
	{
		invSqrtd_setup(half.d, three_by_2.d, init_const.i);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvSqrt_A50_sse2(r.src1[0], r.dst[0], half.d, three_by_2.d, init_const.i);		 
	 }

	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 InvSqrtd_ref(s[0], d[0]);
	 } 
};


struct InvSqrt_A53 : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF
	XMM128 one ;

	InvSqrt_A53(){}

	IV SSE2_Init()
	{
		one.d = _mm_set1_pd(1.0);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvSqrt_A53_sse2(r.src1[0], r.dst[0], one.d);		 
	 }

	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 InvSqrtd_ref(s[0], d[0]);
	 } 
};

// Cbrt
struct Cbrt_32f_A11 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, one_by_3 ;

	Cbrt_32f_A11(){}

	IV SSE2_Init()
	{
		CBL_LIBRARY::CBL_Library_Internal::cbrt_A11_setup(init_const.i, one_by_3.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 CBL_LIBRARY::CBL_Library_Internal::Cbrt_A11_sse2(r.src1[0], r.dst[0], init_const.i, one_by_3.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 Cbrtf_ref(s[0], d[0]);
	 } 
};

struct Cbrt_32f_A21 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3;

	Cbrt_32f_A21(){}

	IV SSE2_Init()
	{
		cbrtf_setup(init_const.i, four_by_3.f, one_by_3.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Cbrt_A21_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 Cbrtf_ref(s[0], d[0]);
	 } 
};
	
struct Cbrt_32f_A24 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3;

	Cbrt_32f_A24(){}

	IV SSE2_Init()
	{
		cbrtf_setup(init_const.i, four_by_3.f, one_by_3.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Cbrt_A24_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 Cbrtf_ref(s[0], d[0]);
	 } 
};

struct Cbrt_64f_A53 : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, p1_p2, p3_p4, p5_p6;

	Cbrt_64f_A53(){}

	IV SSE2_Init()
	{
		cbrtd_setup(init_const.i, p1_p2.d, p3_p4.d, p5_p6.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Cbrt_A53_sse2(r.src1[0], r.dst[0], init_const.i, p1_p2.d, p3_p4.d, p5_p6.d);		 
	 }

	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 Cbrtd_ref(s[0], d[0]);
	 } 
};

//InvCbrt
struct InvCbrt_32f_A11 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3, four_by_3D ;

	InvCbrt_32f_A11(){}

	IV SSE2_Init()
	{
		CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_setup(init_const.i, four_by_3.f, one_by_3.f, four_by_3D.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 CBL_LIBRARY::CBL_Library_Internal::InvCbrt_A11_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvCbrtf_ref(s[0], d[0]);
	 } 
};

struct InvCbrt_32f_A21 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3, four_by_3D ;

	InvCbrt_32f_A21(){}

	IV SSE2_Init()
	{
        CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_setup(init_const.i, four_by_3.f, one_by_3.f, four_by_3D.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvCbrt_A21_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvCbrtf_ref(s[0], d[0]);
	 } 
};

struct InvCbrt_32f_A24 : public fe2<Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3, four_by_3D ;

	InvCbrt_32f_A24(){}

	IV SSE2_Init()
	{
		CBL_LIBRARY::CBL_Library_Internal::inv_cbrtf_setup(init_const.i, four_by_3.f, one_by_3.f, four_by_3D.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvCbrt_A24_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f, four_by_3D.d);		 
	 }

	 IV REFR( const Fw32f *s, Fw32f *d) const                     
	 {         
		 InvCbrtf_ref(s[0], d[0]);
	 } 
};

struct InvCbrt_64f_A50 : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, four_by_3, one_by_3;

	InvCbrt_64f_A50(){}

	IV SSE2_Init()
	{
		inv_cbrtd_setup(init_const.i, four_by_3.d, one_by_3.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvCbrt_A50_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.d, one_by_3.d);		 
	 }

	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 InvCbrtd_ref(s[0], d[0]);
	 } 
};
 
struct InvCbrt_64f_A53 : public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_SSE2_REF
	XMM128 init_const, c_d, e_f, const_1_g;

	InvCbrt_64f_A53(){}

	IV SSE2_Init()
	{
		cbrtd_libm_setup(init_const.i, c_d.d, e_f.d, const_1_g.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 InvCbrt_A53_libm_SSE2(r.src1[0], r.dst[0], init_const.i, c_d.d, e_f.d, const_1_g.d);		 
	 }

	 IV REFR( const Fw64f *s, Fw64f *d) const                     
	 {         
		 InvCbrtd_ref(s[0], d[0]);
	 } 
};

struct Cubrt_32s16s : public fe2<Fw32s, C1, Fw16s, C1>
{
	FE_SSE2_REF
	XMM128  init_const, four_by_3, one_by_3, one_byscale_cube;
	Fw32f scale;

	Cubrt_32s16s(int sf){scale = calcScale(sf);}

	IV SSE2_Init()
	{
		cbrt_32s16s_setup( scale, init_const.i, four_by_3.f, one_by_3.f, one_byscale_cube.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Cbrt_32s16s_sse2(r.src1[0], r.dst[0], init_const.i, four_by_3.f, one_by_3.f, one_byscale_cube.f);		 
	 }

	 IV REFR( const Fw32s *s, Fw16s *d) const                     
	 {         
		 Cbrt_32s16s_ref(s[0], d[0], scale);
	 } 
};

#ifdef _NOTUSED_
ISV inv_iter(__m128 &dst, const __m128 &src)
{
	// new = 2*old - x*old^2
	__m128 tmp = _mm_mul_ps(dst, src);
	tmp = _mm_mul_ps(tmp, dst);
	dst = _mm_add_ps(dst, dst);
	dst = _mm_sub_ps(dst, tmp);
}

ISV Inv_A11_sse2(const FnDat &, RegDescriptor &reg)
{
	dst.f = _mm_rcp_ps(src.f);	
	inv_iter(dst.f, src.f);	
}

#endif // _NOTUSED_

} // namespace OPT_LEVEL

#endif //_DIV_SQRT_CBRT_H_



