/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _POWER_H_
#define _POWER_H_

#include "fe.h"

namespace OPT_LEVEL
{
/*
 *		REFERENCE UNIT FUNCTIONS
 */

/*
* Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(_MSC_VER)
#pragma warning( disable : 4723 ) /* disables potential divide by 0 warning */
#endif

SYS_INLINE STATIC double pow_ref(double x, double y)
{
static const double		bp[] = {1.0, 1.5,},
						dp_h[] = { 0.0, 5.84962487220764160156e-01,}, /* 0x3FE2B803, 0x40000000 */
						dp_l[] = { 0.0, 1.35003920212974897128e-08,}, /* 0x3E4CFDEB, 0x43CFD006 */
						zero    =  0.0,
						one	=  1.0,
						two	=  2.0,
						two53	=  9007199254740992.0,	/* 0x43400000, 0x00000000 */
						really_big	=  1.0e300,
						tiny    =  1.0e-300,
							/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
						L1  =  5.99999999999994648725e-01, /* 0x3FE33333, 0x33333303 */
						L2  =  4.28571428578550184252e-01, /* 0x3FDB6DB6, 0xDB6FABFF */
						L3  =  3.33333329818377432918e-01, /* 0x3FD55555, 0x518F264D */
						L4  =  2.72728123808534006489e-01, /* 0x3FD17460, 0xA91D4101 */
						L5  =  2.30660745775561754067e-01, /* 0x3FCD864A, 0x93C9DB65 */
						L6  =  2.06975017800338417784e-01, /* 0x3FCA7E28, 0x4A454EEF */
						P1   =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
						P2   = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
						P3   =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
						P4   = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
						P5   =  4.13813679705723846039e-08, /* 0x3E663769, 0x72BEA4D0 */
						lg2  =  6.93147180559945286227e-01, /* 0x3FE62E42, 0xFEFA39EF */
						lg2_h  =  6.93147182464599609375e-01, /* 0x3FE62E43, 0x00000000 */
						lg2_l  = -1.90465429995776804525e-09, /* 0xBE205C61, 0x0CA86C39 */
						ovt =  8.0085662595372944372e-0017, /* -(1024-log2(ovfl+.5ulp)) */
						cp    =  9.61796693925975554329e-01, /* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
						cp_h  =  9.61796700954437255859e-01, /* 0x3FEEC709, 0xE0000000 =(float)cp */
						cp_l  = -7.02846165095275826516e-09, /* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h*/
						ivln2    =  1.44269504088896338700e+00, /* 0x3FF71547, 0x652B82FE =1/ln2 */
						ivln2_h  =  1.44269502162933349609e+00, /* 0x3FF71547, 0x60000000 =24b 1/ln2*/
						ivln2_l  =  1.92596299112661746887e-08; /* 0x3E54AE0B, 0xF85DDF44 =1/ln2 tail*/



	//Little Endian
	#define HI	1
	#define LO	0

    union {
		unsigned int	i[2];
		double			d;
	} ux, uy, uz;
    
	double y1,t1,p_h,t,z,ax;
	double z_h,z_l,p_l;
	double t2,r,s,u,v,w;
	int i,j,k,yisint,n;
	int hx,hy,ix,iy;
	unsigned lx,ly;

    ux.d = x; uy.d = y;
	hx = ux.i[HI]; lx = ux.i[LO];
	hy = uy.i[HI]; ly = uy.i[LO];
	ix = hx&0x7fffffff;  iy = hy&0x7fffffff;

    /* y==zero: x**0 = 1 */
	if((iy|ly)==0) return one;

    /* +-NaN return x+y */
	if((x != x) || (y != y))
		return x+y;

	/* special value of y */
	if(ly==0) {
	    if (iy==0x7ff00000) {	/* y is +-inf */
	        if(((ix-0x3ff00000)|lx)==0)
				return one;	// +-1 ** inf
	        else if (ix >= 0x3ff00000)/* (|x|>1)**+-inf = inf,0 */
			    return (hy>=0)? y: zero;
	        else			/* (|x|<1)**-,+inf = inf,0 */
				return (hy<0)?-y: zero;
	    }
	    if(iy==0x3ff00000) {	/* y is  +-1 */
			if(hy<0) return one/x; 
			else return x;
	    }
	    if(hy==0x40000000) return x*x; /* y is  2 */
		if(hy == 0x3fe00000 && hx >= 0) return sqrt(x);
	}

    /* determine if y is an odd int when x < 0
     * yisint = 0	... y is not an integer
     * yisint = 1	... y is an odd int
     * yisint = 2	... y is an even int
     */
	yisint  = 0;
	if(hx<0) {
	    if(iy>=0x43400000) yisint = 2; // even integer y 
	    else if(iy>=0x3ff00000) {
		k = (iy>>20)-0x3ff;	   // exponent 
		if(k>20) {
		    j = ly>>(52-k);
		    if((j<<(52-k))==(int)ly) yisint = 2-(j&1);
		} else if(ly==0) {
		    j = iy>>(20-k);
		    if((j<<(20-k))==iy) yisint = 2-(j&1);
		}
	    }
	}

	ax   = fabs(x);
    /* special value of x */
	if(lx==0) {
	    if(ix==0x7ff00000||ix==0||ix==0x3ff00000){
		z = ax;			/*x is +-0,+-inf,+-1*/
		if(hy<0) z = one/z;	/* z = (1/|x|) */
		if(hx<0) {
		    if(((ix-0x3ff00000)|yisint)==0) {
				z = (z-z)/(z-z); /* (-1)**non-int is NaN */
		    } else if(yisint==1) {
				z = -z;		/* (x<0)**odd = -(|x|**odd) */
			}
		}
		return z;
	    }
	}

    /* (x<0)**(non-int) is NaN */
	if((((hx>>31)+1)|yisint)==0) return (x-x)/(x-x);

    /* |y| is really_big */
	if(iy>0x41e00000) { /* if |y| > 2**31 */
	    if(iy>0x43f00000){	/* if |y| > 2**64, must o/uflow */
			if(ix<=0x3fefffff) return (hy<0)? Const::INF_64F:(double)(tiny*tiny);
			if(ix>=0x3ff00000) return (hy>0)? Const::INF_64F:(double)(tiny*tiny);
	    }
	/* over/underflow if x is not close to one */
	    if(ix<0x3fefffff) return (hy<0)? Const::INF_64F:(double)(tiny*tiny);
	    if(ix>0x3ff00000) return (hy>0)? Const::INF_64F:(double)(tiny*tiny);
	/* now |1-x| is tiny <= 2**-20, suffice to compute
	   log(x) by x-x^2/2+x^3/3-x^4/4 */
	    t = x-1;		/* t has 20 trailing zeros */
	    w = (t*t)*(0.5-t*(0.3333333333333333333333-t*0.25));
	    u = ivln2_h*t;	/* ivln2_h has 21 sig. bits */
	    v = t*ivln2_l-w*ivln2;
	    t1 = u+v;
            uz.d = t1;
	    uz.i[LO] = 0;
            t1 = uz.d;
	    t2 = v-(t1-u);
	} else {
	    double s_h,t_h;
	    double s2,s_l,t_l;
	    n = 0;
	/* take care subnormal number */
	    if(ix<0x00100000)
		{ax *= two53; n -= 53; uz.d = ax; ix = uz.i[HI]; }
	    n  += ((ix)>>20)-0x3ff;
	    j  = ix&0x000fffff;
	/* determine interval */
	    ix = j|0x3ff00000;		/* normalize ix */
	    if(j<=0x3988E) k=0;		/* |x|<sqrt(3/2) */
	    else if(j<0xBB67A) k=1;	/* |x|<sqrt(3)   */
	    else {k=0;n+=1;ix -= 0x00100000;}
            uz.d = ax;
	    uz.i[HI] = ix;
            ax = uz.d;

	/* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
	    u = ax-bp[k];		/* bp[0]=1.0, bp[1]=1.5 */
	    v = one/(ax+bp[k]);
	    s = u*v;
	    s_h = s;
            uz.d = s_h;
	    uz.i[LO] = 0;
            s_h = uz.d;
	/* t_h=ax+bp[k] High */
	    t_h = zero;
            uz.d = t_h;
	    uz.i[HI]=((ix>>1)|0x20000000)+0x00080000+(k<<18);
            t_h = uz.d;
	    t_l = ax - (t_h-bp[k]);
	    s_l = v*((u-s_h*t_h)-s_h*t_l);
	/* compute log(ax) */
	    s2 = s*s;
	    r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));
	    r += s_l*(s_h+s);
	    s2  = s_h*s_h;
	    t_h = 3.0+s2+r;
            uz.d = t_h;
	    uz.i[LO] = 0;
            t_h = uz.d;
	    t_l = r-((t_h-3.0)-s2);
	/* u+v = s*(1+...) */
	    u = s_h*t_h;
	    v = s_l*t_h+t_l*s;
	/* 2/(3log2)*(s+...) */
	    p_h = u+v;
            uz.d = p_h;
	    uz.i[LO] = 0;
        p_h = uz.d;
	    p_l = v-(p_h-u);
	    z_h = cp_h*p_h;		/* cp_h+cp_l = 2/(3*log2) */
	    z_l = cp_l*p_h+p_l*cp+dp_l[k];
	/* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
	    t = (double)n;
	    t1 = (((z_h+z_l)+dp_h[k])+t);
            uz.d = t1;
	    uz.i[LO] = 0;
            t1 = uz.d;
	    t2 = z_l-(((t1-t)-dp_h[k])-z_h);
	}

	s = one; /* s (sign of result -ve**odd) = -1 else = 1 */
	if((((hx>>31)+1)|(yisint-1))==0) s = -one;/* (-ve)**(odd int) */

    /* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	y1  = y;
        uy.d = y1;
	uy.i[LO] = 0;
        y1 = uy.d;
	p_l = (y-y1)*t1+y*t2;
	p_h = y1*t1;
	z = p_l+p_h;
        uz.d = z;
	j = uz.i[HI];
	i = uz.i[LO];

	if (j>=0x40900000) {				/* z >= 1024 */
	    if(((j-0x40900000)|i)!=0)			/* if z > 1024 */
		return s*really_big*really_big;			/* overflow */
	    else {
		if(p_l+ovt>z-p_h) return s*really_big*really_big;	/* overflow */
	    }
	} else if((j&0x7fffffff)>=0x4090cc00 ) {	/* z <= -1075 */
	    if(((j-0xc090cc00)|i)!=0) 		/* z < -1075 */
		return s*tiny*tiny;		/* underflow */
	    else {
		if(p_l<=z-p_h) return s*tiny*tiny;	/* underflow */
	    }
	}
    /*
     * compute 2**(p_h+p_l)
     */
	i = j&0x7fffffff;
	k = (i>>20)-0x3ff;
	n = 0;
	if(i>0x3fe00000) {		/* if |z| > 0.5, set n = [z+0.5] */
	    n = j+(0x00100000>>(k+1));
	    k = ((n&0x7fffffff)>>20)-0x3ff;	/* new k for n */
	    t = zero;
            uz.d = t;
	    uz.i[HI] = (n&~(0x000fffff>>k));
            t = uz.d;
	    n = ((n&0x000fffff)|0x00100000)>>(20-k);
	    if(j<0) n = -n;
	    p_h -= t;
	}
	t = p_l+p_h;
        uz.d = t;
	uz.i[LO] = 0;
    t = uz.d;
	u = t*lg2_h;
	v = (p_l-(t-p_h))*lg2+t*lg2_l;
	z = u+v;
	w = v-(z-u);
	t  = z*z;
	t1  = z - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
	r  = (z*t1)/(t1-two)-(w+z*w);
	z  = one-(r-z);
        uz.d = z;
	j  = uz.i[HI];
	j += (n<<20);
	if((j>>20)<=0)
		SCALBN(z, n);
	else {
		uz.d = z; uz.i[HI] += (n<<20); z = uz.d; 
	}
	return s*z;
}


/*
 *		REFERENCE CALLING FUNCTIONS
 */

ISV Powf_ref(const Fw32f &src1, const Fw32f &src2, Fw32f &dst)
{
	dst = powf( src1, src2);
}

ISV Powd_ref(const Fw64f &src1, const Fw64f &src2, Fw64f &dst)
{
	dst = pow( src1, src2);
}

ISV PowXf_ref(const Fw32f &src, const Fw32f &value, Fw32f &dst)
{
	dst = powf( src, value);
}

ISV PowXd_ref(const Fw64f &src, const Fw64f &value, Fw64f &dst)
{
	dst = pow( src, value);
}

SYS_INLINE  STATIC void Powdx_fast_ref(const Fw64f &src, const Fw64f &value, Fw64f &dst)
{
	dst = pow_ref(src, value);
}

SYS_INLINE  STATIC void Powd_fast_ref(const Fw64f &src1, const Fw64f &src2, Fw64f &dst )
{
	dst = pow_ref(src1, src2);
}


/*
 *		SSE2 SETUP FUNCTIONS
 */

SYS_INLINE STATIC void powXint_32f_setup(__m128d &one)
{
	one = _mm_set1_pd(1.0);
}

SYS_INLINE STATIC void powXint_64f_setup(__m128d &one)
{
	one = _mm_set1_pd(1.0);
}

SYS_INLINE  STATIC void powx_A11_setup(Fw32f val, __m128 &ln2_lead, __m128 &ln2_trail, __m128 &A1_A2, __m128 &value)
{
	ln2_lead	= _mm_set1_ps(0.693145751953125F);		//log2_lead
	ln2_trail	= _mm_set1_ps(1.4286068202862268e-06F);		//log2_trail
	A1_A2		= _mm_set_ps(0.333329854F, 0.333329854F, -0.500003886F, -0.500003886F); //A1_A2	
	value		= _mm_set1_ps(val);	//value
}

/*SYS_INLINE  STATIC void Powx_64f_setup(FnDat &dat)
{
	dat.xmmData[0].d = _mm_set1_pd( dat.Get<Fw64f>(0));

	dat.xmmData[1].d = _mm_set1_pd(6.9314718055994450e-1);	//log2_lead
	dat.xmmData[2].d = _mm_set1_pd(8.0034658537607260e-16);	//log2_trail
	
	__m128i val_is_long, val_is_even ;
    val_is_long = CBL_LIBRARY::CBL_Library_Internal::cbl_is_longint(dat.xmmData[0], val_is_even);
	dat.xmmData[3].i = val_is_long;
	dat.xmmData[4].i = val_is_even;
}
*/
/*
 *		SSE2 UNIT FUNCTIONS
 */

ISV pow_64f_unit(const XMM128 &x, const XMM128 &y, XMM128 &dst,
					const __m128d &ln2_lead, const __m128d &ln2_trail, const __m128d &a1_a2)
					
{												   
	// src1^src2 = exp(src2 * ln(src1))		
	XMM128 tdst ;
	ln_64f_tang(x, tdst, ln2_lead, ln2_trail, a1_a2);  
	ln_64f_special_case(x, tdst);
 
	tdst.d = _mm_mul_pd(tdst.d, y.d);
	tdst.d = _mm_andnot_pd(_mm_cmpeq_pd(y.d, _mm_setzero_pd()), tdst.d);	// x**0 -> 1 for any x

	static const __m128d thirtytwo_by_ln2		= CONST_SET_64F (46.166241308446828384, 1.38889490863777199667e-03); // 32/ln(2), A5
	static const __m128d ln2_by_32_lead_trail	= CONST_SETR_64F(0.021660849390173098072, 2.3251928468788740148e-12); //  0x3F962E42FEF00000
	static const __m128d A1_A2					= CONST_SET_64F ( 1.66666666665260878863e-01, 5.00000000000000008883e-01);	//A1_A2
	static const __m128d A3_A4					= CONST_SET_64F ( 8.33336798434219616221e-03, 4.16666666662260795726e-02);	//A3_A4
	
	exp_64f(tdst.d, dst, thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, A3_A4);
	exp_64f_special_case(tdst, dst);
}

//used in pow_64f	-- can be cleaned and optimized a LOOOOT
ISV ln2_64f (const XMM128 &x,  XMM128 &t1, XMM128 &t2)
{
	XMM128 ix, j, n, ax ;
	int k0, k1, j0, j1;
	double bp[] = {1.0, 1.5};

	ix.i = _mm_and_si128(x.i, _mm_set_epi32(0x7fffffff, 0, 0x7fffffff, 0));
	n.i = _mm_srli_epi32(ix.i, 20) ;
	n.i = _mm_sub_epi32(n.i, _mm_set_epi32(0x3ff, 0, 0x3ff, 0));
	j.i = _mm_srli_epi32(_mm_slli_epi32(ix.i, 12), 12);
	ix.i = _mm_or_si128(j.i, _mm_set_epi32(0x3ff00000, 0, 0x3ff00000, 0));
	j0 = j.u32[1] ; j1 = j.u32[3];

	if(j0 <= 0x3988E) k0 = 0;		/* |x|<sqrt(3/2) */
    else if(j0 <0xBB67A) k0 =1;	/* |x|<sqrt(3)   */
    else {k0 = 0;n.u32[1]+=1;ix.u32[1] -= 0x00100000;}

    if(j1 <= 0x3988E) k1 = 0;		/* |x|<sqrt(3/2) */
    else if(j1 <0xBB67A) k1 =1;	/* |x|<sqrt(3)   */
    else {k1 = 0;n.u32[3]+=1;ix.u32[3] -= 0x00100000;}

	ax.i = ix.i ; 

	__m128d u, v, ss; 
	XMM128 tmp, s_h, s_l, t_h, t_l ;

	u = _mm_sub_pd(ax.d, _mm_set_pd(bp[k1], bp[k0]));
	v = _mm_add_pd(ax.d, _mm_set_pd(bp[k1], bp[k0]));
	v = _mm_div_pd(_mm_set1_pd(1.0), v);
	
	ss = _mm_mul_pd(u, v);
	s_h.d = ss ;
	s_h.i = _mm_slli_epi64(_mm_srli_epi64(s_h.i, 32), 32);
	t_h.i = _mm_srli_epi32(ix.i, 1);
	t_h.i = _mm_or_si128(t_h.i, _mm_set_epi32(0x20000000, 0, 0x20000000, 0));
	t_h.i = _mm_add_epi32(t_h.i, _mm_set_epi32(0x00080000, 0, 0x00080000, 0));
	tmp.i = _mm_set_epi32(k1<<18, 0, k0 <<18, 0);
	t_h.i = _mm_add_epi32(t_h.i, tmp.i);

	t_l.d = _mm_sub_pd(t_h.d, _mm_set_pd(bp[k1], bp[k0]));
	t_l.d = _mm_sub_pd(ax.d, t_l.d);
	// s_l = v*((u-s_h*t_h)-s_h*t_l);
	s_l.d = _mm_mul_pd(s_h.d, t_l.d);
	tmp.d = _mm_mul_pd(s_h.d, t_h.d);
	tmp.d = _mm_sub_pd(u, tmp.d);
	s_l.d = _mm_sub_pd(tmp.d, s_l.d);
	s_l.d = _mm_mul_pd(s_l.d, v);

	/* compute ln(ax) */
   __m128d s2, r ;

	s2 = _mm_mul_pd(ss, ss);

	static const __m128d L1  =  CONST_SET1_64F(5.99999999999994648725e-01), /* 0x3FE33333, 0x33333303 */
						 L2  =  CONST_SET1_64F(4.28571428578550184252e-01), /* 0x3FDB6DB6, 0xDB6FABFF */
						 L3  =  CONST_SET1_64F(3.33333329818377432918e-01), /* 0x3FD55555, 0x518F264D */
						 L4  =  CONST_SET1_64F(2.72728123808534006489e-01), /* 0x3FD17460, 0xA91D4101 */
						 L5  =  CONST_SET1_64F(2.30660745775561754067e-01), /* 0x3FCD864A, 0x93C9DB65 */
						 L6  =  CONST_SET1_64F(2.06975017800338417784e-01); /* 0x3FCA7E28, 0x4A454EEF */
//	r = s2*s2*(L1+s2*(L2+s2*(L3+s2*(L4+s2*(L5+s2*L6)))));

	r = _mm_mul_pd(L6, s2);
	r = _mm_add_pd(r, L5);
	r = _mm_mul_pd(r, s2);
	r = _mm_add_pd(r, L4);
	r = _mm_mul_pd(r, s2); 
	r = _mm_add_pd(r, L3);
	r = _mm_mul_pd(r, s2); 
	r = _mm_add_pd(r, L2);
	r = _mm_mul_pd(r, s2); 
	r = _mm_add_pd(r, L1);
	r = _mm_mul_pd(r, s2); 
	r = _mm_mul_pd(r, s2); 

	tmp.d = _mm_add_pd(s_h.d, ss);
	tmp.d = _mm_mul_pd(tmp.d, s_l.d);
	r = _mm_add_pd(r, tmp.d);

	s2 = _mm_mul_pd(s_h.d, s_h.d);
	t_h.d = _mm_add_pd(r, s2);
	t_h.d = _mm_add_pd(t_h.d, _mm_set1_pd(3.0));
	t_h.i = _mm_slli_epi64(_mm_srli_epi64(t_h.i, 32), 32);

	//  t_l = r-((t_h-3.0)-s2);
	t_l.d = _mm_sub_pd(t_h.d, _mm_set1_pd(3.0));
	t_l.d = _mm_sub_pd(t_l.d, s2);
	t_l.d = _mm_sub_pd(r, t_l.d);

	/* u+v = ss*(1+...) */
	u = _mm_mul_pd(s_h.d, t_h.d);
	tmp.d = _mm_mul_pd(s_l.d, t_h.d);
	v = _mm_mul_pd(t_l.d, ss);
	v = _mm_add_pd(v, tmp.d);

	XMM128 p_h, p_l, z_h, z_l ;
	const double cp    =  9.61796693925975554329e-01 ,
		cp_h  =  9.61796700954437255859e-01 ,
		cp_l  = -7.02846165095275826516e-09 ,
		dp_h[] = { 0.0, 5.84962487220764160156e-01,}, /* 0x3FE2B803, 0x40000000 */
		dp_l[] = { 0.0, 1.35003920212974897128e-08,}; /* 0x3E4CFDEB, 0x43CFD006 */

	/* 2/(3ln2)*(ss+...) */
    p_h.d = _mm_add_pd(u, v);
	p_h.i = _mm_slli_epi64(_mm_srli_epi64(p_h.i, 32), 32);
	p_l.d = _mm_sub_pd(v, _mm_sub_pd(p_h.d, u));
    z_h.d = _mm_mul_pd(_mm_set1_pd(cp_h), p_h.d);		/* cp_h+cp_l = 2/(3*ln2) */

  //  z_l = cp_l*p_h+p_l*cp+dp_l[k];
	z_l.d = _mm_mul_pd(_mm_set1_pd(cp_l), p_h.d);
	tmp.d = _mm_mul_pd(_mm_set1_pd(cp), p_l.d);
	z_l.d = _mm_add_pd(z_l.d, tmp.d);
	tmp.d = _mm_set_pd(dp_l[k1], dp_l[k0]);
	z_l.d = _mm_add_pd(z_l.d, tmp.d);

	/* ln2(ax) = (ss+..)*2/(3*ln2) = n + dp_h + z_h + z_l */
	n.i = _mm_shuffle_epi32(n.i, _MM_SHUFFLE(2,0,3,1));
	 __m128d t = _mm_cvtepi32_pd(n.i);
//	 t1 = (((z_h+z_l)+dp_h[k])+t);
	 t1.d = _mm_add_pd(z_h.d, z_l.d);
	 tmp.d = _mm_set_pd(dp_h[k1], dp_h[k0]);
	 t1.d = _mm_add_pd(t1.d, tmp.d);
	 t1.d = _mm_add_pd(t1.d, t);
	 t1.i = _mm_slli_epi64(_mm_srli_epi64(t1.i, 32), 32);

//	 t2 = z_l-(((t1-t)-dp_h[k])-z_h);	
	 t2.d = _mm_sub_pd(t1.d, t);
	 tmp.d = _mm_set_pd(dp_h[k1], dp_h[k0]);
	 t2.d = _mm_sub_pd(t2.d, tmp.d);
	 t2.d = _mm_sub_pd(t2.d, z_h.d);
	 t2.d = _mm_sub_pd(z_l.d, t2.d);
}

ISV two_power_64f_special_case(const XMM128 &src, XMM128 &dst)
{
	/*  if |src| > max_arg then 
			dst -> inf_64f if src >0 
			dst -> 0 if src < 0
		else dst */

	XMM128 cmp;
	static const __m128d max_min_arg = CONST_SET_64F(1024, -1024);	//to be exactly put!

	cmp.d = _mm_cmpge_pd(src.d, _mm_unpacklo_pd(max_min_arg, max_min_arg)); //min
	dst.d = _mm_and_pd(cmp.d, dst.d );	// (1)
	cmp.d = _mm_cmpgt_pd(src.d, _mm_unpackhi_pd(max_min_arg, max_min_arg)); // max
	dst.d = _mm_andnot_pd(cmp.d, dst.d);	//(3)
	
	cmp.i = _mm_slli_epi64(_mm_srli_epi64(cmp.i, 53), 52);	//0x7ff0000000000000 i.e, inf
	dst.i = _mm_or_si128(dst.i, cmp.i);	//(2)
}

ISV pow_double_unit( const XMM128 &x,  const XMM128 &y, XMM128 &dst,
										const __m128d &ln2_lead, const __m128d &ln2_trail)
{
	XMM128 t1, t2, t, t_c;

	ln2_64f(x, t1, t2); 

	t1.d = _mm_mul_pd(t1.d, y.d);
	t2.d = _mm_mul_pd(t2.d, y.d);
	t.d = _mm_add_pd(t1.d, t2.d);
	t_c.d = t.d ;

	 // compute 2**(t)
     
	XMM128 n, tmp ; 

	n.i = _mm_cvtpd_epi32(t.d);
	tmp.d = _mm_cvtepi32_pd(n.i);
	t1.d = _mm_sub_pd(t1.d, tmp.d);
	n.i = _mm_shuffle_epi32(n.i, _MM_SHUFFLE(3, 1, 2, 0));	
	n.i = _mm_slli_epi64(n.i, 52);
	t.d = _mm_add_pd(t1.d, t2.d);

	XMM128 one ; 
	one.i = _mm_set_epi32(0x3FF00000, 0, 0x3FF00000, 0);
	t.i = _mm_slli_epi64(_mm_srli_epi64(t.i, 32), 32);

	__m128d u, v ;
	u = _mm_mul_pd(t.d, ln2_lead);
	v = _mm_sub_pd(t.d, t1.d);	//t-p_h
	v = _mm_sub_pd(t2.d, v);	// p_l - (t-p_h)
	v = _mm_mul_pd(v, _mm_add_pd(ln2_lead, ln2_trail));
	t.d = _mm_mul_pd(ln2_trail, t.d);
	v = _mm_add_pd(v, t.d);

	XMM128 z, r, w ;
	z.d = _mm_add_pd(u, v);	//z is now ln2*y'
	
	w.d = _mm_sub_pd(z.d, u);
	w.d = _mm_sub_pd(v, w.d);
	t.d = _mm_mul_pd(z.d, z.d);
	
	static const __m128d P1   = CONST_SET1_64F(  1.66666666666666019037e-01);
	static const __m128d P2   = CONST_SET1_64F( -2.77777777770155933842e-03);
	static const __m128d P3   = CONST_SET1_64F(  6.61375632143793436117e-05);
	static const __m128d P4   = CONST_SET1_64F( -1.65339022054652515390e-06);
	static const __m128d P5   = CONST_SET1_64F(  4.13813679705723846039e-08);

	//	t1  = z - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));

	t1.d = _mm_mul_pd(t.d, P5);
	t1.d = _mm_add_pd(t1.d, P4);
	t1.d = _mm_mul_pd(t1.d, t.d);
	t1.d = _mm_add_pd(t1.d, P3);
	t1.d = _mm_mul_pd(t1.d, t.d);
	t1.d = _mm_add_pd(t1.d, P2);
	t1.d = _mm_mul_pd(t1.d, t.d);
	t1.d = _mm_add_pd(t1.d, P1);
	t1.d = _mm_mul_pd(t1.d, t.d);
	t1.d = _mm_sub_pd(z.d, t1.d);

	//r  = (z*t1)/(t1-two)-(w+z*w);
	u = _mm_sub_pd(t1.d, _mm_set1_pd(2.0));
	v = _mm_mul_pd(z.d, w.d);
	v = _mm_add_pd(v, w.d);

	r.d = _mm_mul_pd(z.d, t1.d);
	r.d = _mm_div_pd(r.d, u);
	r.d = _mm_sub_pd(r.d, v);

	z.d = _mm_sub_pd(r.d, z.d);
	z.d = _mm_sub_pd(one.d, z.d);
	dst.i = _mm_add_epi64(n.i, z.i);

//	exp_64f_special_case(z, dst);
	two_power_64f_special_case(t_c, dst);
/*
    //this also works
	static const __m128d thirtytwo_by_ln2 = _mm_set_pd(46.166241308446828384, 1.38889490863777199667e-03); // 32/ln(2), A5
	static const __m128d ln2_by_32_lead_trail = _mm_setr_pd(0.021660849390173098072, 2.3251928468788740148e-12); //  0x3F962E42FEF00000
	static const __m128d A1_A2= _mm_set_pd( 1.66666666665260878863e-01, 5.00000000000000008883e-01);	//A1_A2
	static const __m128d A3_A4= _mm_set_pd( 8.33336798434219616221e-03, 4.16666666662260795726e-02);	//A3_A4
	
	exp_64f(z.d, dst, thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, A3_A4);
	exp_64f_special_case(z, dst);
	dst.i = _mm_add_epi64(n.i, dst.i);
*/
}

ISV pow_A32f_unit(const XMM128 &src1, const XMM128 &src2, XMM128 &dst,
				  const __m128d &ln2_lead, const __m128d &ln2_trail,const __m128d &A1_A2)
{	// src1^src2 = exp(src2 * ln(src1))		
	 XMM128 x1, x2, y1, y2, tdst1, tdst2, tsrc2 ;
	 
	x1.d = _mm_cvtps_pd(src1.f);
	y1.d = _mm_cvtps_pd(src2.f);
	pow_64f_unit(x1, y1, tdst1, ln2_lead, ln2_trail, A1_A2);
	tdst1.f = _mm_cvtpd_ps(tdst1.d);
	
	tsrc2.f = src2.f ;

	x2.i = _mm_srli_si128(src1.i, 8);	x2.d = _mm_cvtps_pd(x2.f);
	y2.i = _mm_srli_si128(tsrc2.i, 8);	y2.d = _mm_cvtps_pd(y2.f);
	pow_64f_unit(x2, y2, tdst2, ln2_lead, ln2_trail, A1_A2);
	tdst2.f = _mm_cvtpd_ps(tdst2.d);
	
	dst.f = _mm_shuffle_ps(tdst1.f, tdst2.f, _MM_SHUFFLE(1, 0, 1, 0));
}

ISV pow_64f_special_case( const XMM128 &src1, const __m128d &src2, XMM128 &dst, const __m128i &is_src2_lint)
{
	XMM128 cmp1, cmp2;
	const __m128d zero  = _mm_setzero_pd();

	//		Pow (x<0, y=NON_INT) ->	NAN_64F		
	cmp1.d = _mm_cmplt_pd(src1.d, zero);
    cmp2.i = _mm_andnot_si128(is_src2_lint, _mm_cmpeq_epi32(src1.i, src1.i));

	__m128i cmp = _mm_and_si128(cmp1.i, cmp2.i);
	__m128i nan = _mm_slli_epi64(_mm_srli_epi64(cmp, 52), 51);	//7FF8000000000000
	dst.i = _mm_andnot_si128(cmp, dst.i);
	dst.i = _mm_or_si128(dst.i, nan);

	//		Pow (x=+0, y=-ve number) ->  INF_64F
	cmp1.d = _mm_cmpeq_pd(src1.d, zero);
	cmp2.d = _mm_cmplt_pd(src2, zero);
	cmp = _mm_and_si128(cmp1.i, cmp2.i);
	dst.i = _mm_andnot_si128(cmp, dst.i);
	cmp = _mm_slli_epi64(_mm_srli_epi64(cmp, 53), 52);	//0x7ff00000 00000000
	dst.i = _mm_or_si128(dst.i, cmp);
}

SYS_INLINE STATIC void powXint_32f_unit(const XMM128 &base, int e, XMM128 &result, const __m128d &one)
{
	int sign, e1 ;
    XMM128 temp;
	__m128d temp1,temp2;
    XMM128 result1, result2;

    temp1 = _mm_cvtps_pd(base.f);
    temp.i= _mm_srli_si128(base.i, 8);
    temp2 = _mm_cvtps_pd(temp.f);

	result1.d = one ;
    result2.d = one;
	
	sign = e & (0x80000000);
    if(sign)
        {
        e1 = ~e;
        e1 = e1 + 0x00000001;
        }
    else
	    e1 = e;			

	while(e1 != 0)
	{
		 if (e1 & 1)
             {
     		 result1.d = _mm_mul_pd(result1.d, temp1);
             result2.d = _mm_mul_pd(result2.d, temp2);
             }
		 temp1 = _mm_mul_pd(temp1, temp1);
         temp2 = _mm_mul_pd(temp2, temp2);
	 	 e1 >>= 1 ;
	}
	if(sign)
        {
		result1.d = _mm_div_pd(one, result1.d);
        result2.d = _mm_div_pd(one, result2.d);
        }
    result1.f = _mm_cvtpd_ps(result1.d);
    result2.f = _mm_cvtpd_ps(result2.d);
    result.d =  _mm_unpacklo_pd(result1.d, result2.d);
}

SYS_INLINE STATIC void powXint_64f_unit(const __m128d &base, Fw64s e, __m128d &result, const __m128d &one)
{
	Fw64s sign, e1 ;
	__m128d temp = base;
	result = one ;
	
	sign = e & (1ULL <<63);
	if(sign)
        {
        e1 = ~e;
        e1 = e1 + 0x00000001;
        }
    else
	    e1 = e;			

	while(e1 != 0)
	{
		 if (e1 & 1)
     		 result = _mm_mul_pd(result, temp);
		 temp = _mm_mul_pd(temp, temp);	
	 	 e1 >>= 1 ;
	}
	if(sign)
		result = _mm_div_pd(one, result);
}


/*
 *		SSE2 CALLING FUNCTIONS
 */

ISV Pow_32f_sse2(const XMM128 &src1, const XMM128 &src2, XMM128 &dst,
				 const __m128d &ln2_lead, const __m128d &ln2_trail,const __m128d &A1_A2)
{	
	__m128i sign_s1 = _mm_slli_epi32(_mm_srli_epi32(src1.i, 31), 31);
	XMM128 tsrc;
	tsrc.i  = _mm_xor_si128(src1.i, sign_s1);
	
	pow_A32f_unit(tsrc, src2, dst, ln2_lead, ln2_trail, A1_A2);

	__m128i s2_is_int, s2_is_even, s2_is_odd;
    s2_is_int = CBL_LIBRARY::CBL_Library_Internal::cbl_is_int(src2, s2_is_even);
	
	s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_int);
	s2_is_odd = _mm_and_si128(s2_is_odd, sign_s1);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	dst.i = _mm_or_si128(dst.i, s2_is_odd);
    CBL_LIBRARY::CBL_Library_Internal::pow_32f_special_case(src1, src2, dst, s2_is_int );
}

ISV Pow_64f_sse2(const XMM128 &src1, const XMM128 &src2, XMM128 &dst, 
				 const __m128d &ln2_lead, const __m128d &ln2_trail)
{
	__m128i sign = _mm_slli_epi64(_mm_srli_epi64(src1.i, 63), 63);
	XMM128 tsrc;
	tsrc.i  = _mm_xor_si128(src1.i, sign);

	pow_double_unit(tsrc, src2, dst, ln2_lead, ln2_trail);

	__m128i s2_is_longint, s2_is_even, s2_is_odd;
    s2_is_longint = CBL_LIBRARY::CBL_Library_Internal::cbl_is_longint(src2, s2_is_even);	
	
	s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_longint);
	s2_is_odd = _mm_and_si128(s2_is_odd, sign);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	dst.i = _mm_or_si128(dst.i, s2_is_odd);
	pow_64f_special_case(src1, src2.d, dst, s2_is_longint );
}

// powx
SYS_INLINE STATIC void Powx_A11_sse2(const XMM128 &src1, const XMM128 &value, XMM128 &dst, 
									 const __m128 &ln2_lead, const __m128 &ln2_trail, const __m128 &A1_A2)
{
	XMM128 tsrc;
	__m128i sign = _mm_slli_epi32(_mm_srli_epi32(src1.i, 31), 31);
	tsrc.i  = _mm_xor_si128(src1.i, sign);
 
    CBL_LIBRARY::CBL_Library_Internal::pow_A11_unit(tsrc, value, dst, ln2_lead, ln2_trail, A1_A2);

	__m128i s2_is_int, s2_is_even, s2_is_odd;
	s2_is_int = CBL_LIBRARY::CBL_Library_Internal::cbl_is_int(value, s2_is_even);	//should be moved to setup_function
	
	s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_int);
	s2_is_odd = _mm_and_si128(s2_is_odd, sign);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	dst.i = _mm_or_si128(dst.i, s2_is_odd);
    CBL_LIBRARY::CBL_Library_Internal::pow_32f_special_case(src1, value, dst, s2_is_int );
}

SYS_INLINE  STATIC void Powx_32f_sse2(const XMM128 &src1, const XMM128 &value, XMM128 &dst, 
									 const __m128d &ln2_lead, const __m128d &ln2_trail, const __m128d &A1_A2)
{	
	__m128i sign = _mm_slli_epi32(_mm_srli_epi32(src1.i, 31), 31);
	XMM128 tsrc;
	tsrc.i  = _mm_xor_si128(src1.i, sign);

	pow_A32f_unit( tsrc, value, dst, ln2_lead, ln2_trail, A1_A2);
	
	__m128i s2_is_int, s2_is_even, s2_is_odd;
	s2_is_int = CBL_LIBRARY::CBL_Library_Internal::cbl_is_int(value, s2_is_even);	//should be moved to setup_function
	
	s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_int);
	s2_is_odd = _mm_and_si128(s2_is_odd, sign);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	dst.i = _mm_or_si128(dst.i, s2_is_odd);
    CBL_LIBRARY::CBL_Library_Internal::pow_32f_special_case(src1, value, dst, s2_is_int );
}

SYS_INLINE  STATIC void Powx_64f_sse2(const XMM128 &src, const XMM128 &value, XMM128 &dst, 
									  const __m128d &ln2_lead, const __m128d & ln2_trail,
									  const __m128i &s2_is_longint, const __m128i &s2_is_even)
{
	XMM128 tsrc;
	__m128i sign = _mm_slli_epi32(_mm_srli_epi32(src.i, 31), 31);
	tsrc.i  = _mm_xor_si128(src.i, sign);
	pow_double_unit(src, value, dst, ln2_lead, ln2_trail);
		
	__m128i s2_is_odd = _mm_andnot_si128(s2_is_even, s2_is_longint);
	s2_is_odd = _mm_and_si128(s2_is_odd, sign);		//s1 is -ve, s2 is odd => s1^s2 should  be -ve.
	dst.i = _mm_or_si128(dst.i, s2_is_odd);
	pow_64f_special_case(src, value.d, dst, s2_is_longint );
}

SYS_INLINE  STATIC void PowXint_32f_sse2(const XMM128 &src, XMM128 &dst,
										 const Fw32f &value, const __m128d &one)
{
	powXint_32f_unit( src, (int)value, dst, one);
}
 
SYS_INLINE  STATIC void PowXint_64f_sse2(const XMM128 &src, const Fw64f &value, 
										 XMM128 &dst, const __m128d &one)
{
	powXint_64f_unit( src.d, (Fw64s)value, dst.d, one);
}

/*
 *		Structures of pow and powx for A11, A21, A24, A50 and A53
 */


struct Pow_32f_A11 : public fe3<Fw32f, C1, Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	Pow_32f_A11() {}
	
	XMM128 ln2_lead, ln2_trail, A1_A2 ;

	IV SSE2_Init()
	{
        CBL_LIBRARY::CBL_Library_Internal::ln_32f_tang_setup(ln2_lead.f, ln2_trail.f, A1_A2.f);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
         CBL_LIBRARY::CBL_Library_Internal::Pow_A11_sse2(r.src1[0], r.src2[0], r.dst[0], ln2_lead.f, ln2_trail.f, A1_A2.f);		 
	 }

	 IV REFR( const Fw32f *s1, const Fw32f *s2, Fw32f *d) const                     
	 {         
		 Powf_ref(s1[0], s2[0], d[0]);
	 } 
};

struct Pow_32f: public fe3<Fw32f, C1, Fw32f, C1, Fw32f, C1>
{
	FE_SSE2_REF
	XMM128 ln2_lead, ln2_trail, A1_A2 ;
	
	Pow_32f() {}

	IV SSE2_Init()
	{
		ln_64f_tang_setup(ln2_lead.d, ln2_trail.d, A1_A2.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Pow_32f_sse2(r.src1[0], r.src2[0], r.dst[0], ln2_lead.d, ln2_trail.d, A1_A2.d);		 
	 }

	 IV REFR( const Fw32f *s1, const Fw32f *s2, Fw32f *d) const                     
	 {         
		 Powf_ref(s1[0], s2[0], d[0]);
	 } 
};

struct Pow_64f: public fe3<Fw64f, C1, Fw64f, C1, Fw64f, C1>
{
	FE_REF
	Pow_64f() {}
	
	 IV REFR( const Fw64f *s1, const Fw64f *s2, Fw64f *d) const                     
	 {         
		 Powd_fast_ref(s1[0], s2[0], d[0]);
	 } 
};

struct PowXint_32f : public fe2<Fw32f, C1, Fw32f, C1 >
{
	FE_SSE2_REF
	
	Fw32f value ;
	XMM128 one;
	PowXint_32f(Fw32f ConstValue ) 
	{
		value = ConstValue;
	}

	IV SSE2_Init()
	{
		powXint_32f_setup(one.d);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 PowXint_32f_sse2(r.src1[0], r.dst[0], value, one.d);		 
	 }

	 IV REFR( const Fw32f *s1, Fw32f *d) const                     
	 {         
		 PowXf_ref(s1[0], value, d[0]);
	 } 
};

struct Powx_A11 : public fe2<Fw32f, C1, Fw32f, C1 >
{
	FE_SSE2_REF
	
	Fw32f val ;
	XMM128 ln2_lead, ln2_trail, A1_A2, value ;

	Powx_A11(Fw32f ConstValue ) 
	{
		val = ConstValue;
	}

	IV SSE2_Init()
	{
		powx_A11_setup(val, ln2_lead.f, ln2_trail.f, A1_A2.f, value.f );
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Powx_A11_sse2(r.src1[0], value, r.dst[0], ln2_lead.f, ln2_trail.f, A1_A2.f);		 
	 }

	 IV REFR( const Fw32f *s1, Fw32f *d) const                     
	 {         
		 PowXf_ref(s1[0], val, d[0]);
	 } 
};


struct Powx_32f: public fe2<Fw32f, C1, Fw32f, C1 >
{
	FE_SSE2_REF

	Fw32f val;
	XMM128 ln2_lead, ln2_trail, A1_A2, value ;
	
	Powx_32f(Fw32f ConstValue) 
	{
		val = ConstValue ;
	}
	
	IV SSE2_Init()
	{
		ln_64f_tang_setup(ln2_lead.d, ln2_trail.d, A1_A2.d);
		value.f = _mm_set1_ps(val);
	}

	 IV SSE2( RegFile & r ) const                     
	 {   
		 Powx_32f_sse2(r.src1[0], value, r.dst[0], ln2_lead.d, ln2_trail.d, A1_A2.d);		 
	 }

	 IV REFR( const Fw32f *s1, Fw32f *d) const                     
	 {         
		 PowXf_ref(s1[0], val, d[0]);
	 } 
};

struct PowXint_64f: public fe2<Fw64f, C1, Fw64f, C1 >
{
	FE_SSE2_REF

	Fw64f value;
	XMM128 one;
	
	PowXint_64f(Fw64f ConstValue) 
	{
		value = ConstValue ;
	}
	
	IV SSE2_Init()
	{
		powXint_64f_setup(one.d);
	}
 
	 IV SSE2( RegFile & r ) const                     
	 {   
		 PowXint_64f_sse2(r.src1[0], value, r.dst[0], one.d);		 
	 }

	 IV REFR( const Fw64f *s1, Fw64f *d) const                     
	 {         
		 PowXd_ref(s1[0], value, d[0]);
	 } 
};


struct Powx_64f: public fe2<Fw64f, C1, Fw64f, C1>
{
	FE_REF
	Fw64f val;
	Powx_64f(Fw64f ConstValue)
	{
		val = ConstValue ;
	}
	
	 IV REFR( const Fw64f *s1, Fw64f *d) const                     
	 {         
		 Powdx_fast_ref(s1[0], val, d[0]);
	 } 
};

} // namespace OPT_LEVEL

#endif //_POWER_H_


