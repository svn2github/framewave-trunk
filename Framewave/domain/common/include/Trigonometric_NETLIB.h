/*
Portions copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
*/

/*
* ====================================================
* Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
*
* Developed at SunSoft, a Sun Microsystems, Inc. business.
* Permission to use, copy, modify, and distribute this
* software is freely granted, provided that this notice * is preserved.
* ====================================================
*/

#ifndef __TRIGONOMETRIC_NETLIB_H__
#define __TRIGONOMETRIC_NETLIB_H__

#include "fe.h"

///////////////////////////////////////// CONSTANTS ////////////////////////////////////////////

// used by scalbn
extern const double
two54 ,  /* 0x43500000, 0x00000000 */
twom54,  /* 0x3C900000, 0x00000000 */
huge,
tiny;

extern const int init_jk[];
extern const double PIo2[];
extern const double
zero,
one ,
two24 ,  /* 0x41700000, 0x00000000 */
twon24;  /* 0x3E700000, 0x00000000 */

extern const int two_over_pi[];
extern const int npio2_hw[];

extern const double
half ,   /* 0x3FE00000, 0x00000000 */
invpio2, /* 0x3FE45F30, 0x6DC9C883 */
pio2_1 , /* 0x3FF921FB, 0x54400000 */
pio2_1t, /* 0x3DD0B461, 0x1A626331 */
pio2_2 , /* 0x3DD0B461, 0x1A600000 */
pio2_2t, /* 0x3BA3198A, 0x2E037073 */
pio2_3 , /* 0x3BA3198A, 0x2E000000 */
pio2_3t; /* 0x397B839A, 0x252049C1 */

// used by cos
extern const double
C1_d,    /* 0x3FA55555, 0x5555554C */
C2_d,    /* 0xBF56C16C, 0x16C15177 */
C3_d,    /* 0x3EFA01A0, 0x19CB1590 */
C4_d,    /* 0xBE927E4F, 0x809C52AD */
C5_d,    /* 0x3E21EE9E, 0xBDB4B1C4 */
C6_d;    /* 0xBDA8FAE9, 0xBE8838D4 */

// used by sin
extern const double
S1,      /* 0xBFC55555, 0x55555549 */
S2,      /* 0x3F811111, 0x1110F8A6 */
S3,      /* 0xBF2A01A0, 0x19C161D5 */
S4,      /* 0x3EC71DE3, 0x57B1FE7D */
S5,      /* 0xBE5AE5E6, 0x8A2B9CEB */
S6;      /* 0x3DE5D93A, 0x5ACFD57C */

// used by tan
extern const double xxx[];

#define one xxx[13]
#define pio4 xxx[14]
#define pio4lo xxx[15]
#define T xxx

// used by acos
extern const double
pio2_hi_d,/* 0x3FF921FB, 0x54442D18 */
pio2_lo_d,/* 0x3C91A626, 0x33145C07 */
pS0_d,    /* 0x3FC55555, 0x55555555 */
pS1_d,    /* 0xBFD4D612, 0x03EB6F7D */
pS2_d,    /* 0x3FC9C155, 0x0E884455 */
pS3_d,    /* 0xBFA48228, 0xB5688F3B */
pS4_d,    /* 0x3F49EFE0, 0x7501B288 */
pS5_d,    /* 0x3F023DE1, 0x0DFDF709 */
qS1_d,    /* 0xC0033A27, 0x1C8A2D4B */
qS2_d,    /* 0x40002AE5, 0x9C598AC8 */
qS3_d,    /* 0xBFE6066C, 0x1B8D0159 */
qS4_d;    /* 0x3FB3B8C5, 0xB12E9282 */

extern const float
pio2_hi_f,/* 0x3fc90fda */
pio2_lo_f,/* 0x33a22168 */
pS0_f,    /* 0x3e2aaaab */
pS1_f,    /* 0xbea6b090 */
pS2_f,    /* 0x3e4e0aa8 */
pS3_f,    /* 0xbd241146 */
pS4_f,    /* 0x3a4f7f04 */
pS5_f,    /* 0x3811ef08 */
qS1_f,    /* 0xc019d139 */
qS2_f,    /* 0x4001572d */
qS3_f,    /* 0xbf303361 */
qS4_f;    /* 0x3d9dc62e */

// used by asin
extern const double pio4_hi_d;/* 0x3FE921FB, 0x54442D18 */

extern const float pio4_hi_f; /* 0x3f490fda */

// used by atan
SYS_FORCEALIGN_16 extern const double atanhi_d[];
SYS_FORCEALIGN_16 extern const double atanlo_d[];
SYS_FORCEALIGN_16 extern const double aT_d[];
SYS_FORCEALIGN_16 extern const float atanhi_f[];
SYS_FORCEALIGN_16 extern const float atanlo_f[];
SYS_FORCEALIGN_16 extern const float aT_f[];

// used by atan2
SYS_FORCEALIGN_16 extern const float
huge_f,one_f;

SYS_FORCEALIGN_16 extern const double
huge_d,one_d;

SYS_FORCEALIGN_16 extern const float
pi_f ,
pi_o_4_f ,
pi3_o_4_f,
pi_hi_f ,
pi_lo_f ;

SYS_FORCEALIGN_16 extern const double
pi_d ,
pi_o_4_d ,
pi3_o_4_d,
pi_hi_d ,
pi_lo_d ;

// used by expm1
extern const double
tiny_d,
o_threshold_d,/* 0x40862E42, 0xFEFA39EF */
ln2_hi_d,     /* 0x3fe62e42, 0xfee00000 */
ln2_lo_d,     /* 0x3dea39ef, 0x35793c76 */
invln2_d,     /* 0x3ff71547, 0x652b82fe */

/* scaled coefficients related to expm1 */
Q1_d,         /* BFA11111 111110F4 */
Q2_d,         /* 3F5A01A0 19FE5585 */
Q3_d,         /* BF14CE19 9EAADBB7 */
Q4_d,         /* 3ED0CFCA 86E65239 */
Q5_d;         /* BE8AFDB7 6E09C32D */

//used by ieee754_exp
extern const double
halF_d[2],
twom1000_d,   /* 2**-1000=0x01700000,0*/
u_threshold_d,/* 0xc0874910, 0xD52D3051 */
ln2HI_d[2],   /* 0x3fe62e42, 0xfee00000 */
ln2LO_d[2],   /* 0x3dea39ef, 0x35793c76 */
P1_d,         /* 0x3FC55555, 0x5555553E */
P2_d,         /* 0xBF66C16C, 0x16BEBD93 */
P3_d,         /* 0x3F11566A, 0xAF25DE2C */
P4_d,         /* 0xBEBBBD41, 0xC5D26BF1 */
P5_d;         /* 0x3E663769, 0x72BEA4D0 */

// used by cosh
extern const double half_d;
extern const float half_f;

// used by sinh
extern const double shuge_d;
extern const float shuge_f;

// used by tanh
extern const double two_d;
extern const float two_f;

// used by acosh
extern const float
tiny_f,
ln2_hi_f,             /* 0x3f317180 */
ln2_lo_f;             /* 0x3717f7d1 */
extern const double
ln2_d;                /* 0x3FE62E42, 0xFEFA39EF */
extern const float
ln2_f;                /* 0x3f317218 */

// used by log1p
extern const double
two54_d,/* 43500000 00000000 */
Lp1_d,  /* 3FE55555 55555593 */
Lp2_d,  /* 3FD99999 9997FA04 */
Lp3_d,  /* 3FD24924 94229359 */
Lp4_d,  /* 3FCC71C5 1D8E78AF */
Lp5_d,  /* 3FC74664 96CB03DE */
Lp6_d,  /* 3FC39A09 D078C69F */
Lp7_d;  /* 3FC2F112 DF3E5244 */

extern const double zero_d;

// used by iee754_log
extern const double
Lg1_d,  /* 3FE55555 55555593 */
Lg2_d,  /* 3FD99999 9997FA04 */
Lg3_d,  /* 3FD24924 94229359 */
Lg4_d,  /* 3FCC71C5 1D8E78AF */
Lg5_d,  /* 3FC74664 96CB03DE */
Lg6_d,  /* 3FC39A09 D078C69F */
Lg7_d;  /* 3FC2F112 DF3E5244 */

// used by log1pf
extern const float
two25_f,/* 0x4c000000 */
Lp1_f,  /* 3F2AAAAB */
Lp2_f,  /* 3ECCCCCD */
Lp3_f,  /* 3E924925 */
Lp4_f,  /* 3E638E29 */
Lp5_f,  /* 3E3A3325 */
Lp6_f,  /* 3E1CD04F */
Lp7_f;  /* 3E178897 */

extern const float zero_f;

// used by ieee754_logpf
extern const float
Lg1_f,  /* 0xaaaaaa.0p-24 */
Lg2_f,  /* 0xccce13.0p-25 */
Lg3_f,  /* 0x91e9ee.0p-25 */
Lg4_f;  /* 0xf89e26.0p-26 */


///////////////////////////////////////// MACROS ///////////////////////////////////////////////

namespace OPT_LEVEL
{

IS double __kernel_sin(double x, double y, int iy);

union union_for_double
    {
    double d;
    int i[2];           // higher part or lower part depends upon little ndian or big endian
    unsigned int ui[2]; // for little endian 1 is higher and 0 is lower
    };

union union_for_float
    {
    float f;
    int i;
    unsigned int ui;
    };

///////////////////////////////////////// COS DOUBLE REF CODE ////////////////////////////////////////////

IS double copysign(double x, double y)
    {    
    union_for_double hx,hy;                                  // unsigned int hx,hy;    
    hx.d = x;                                                // GET_HIGH_WORD(hx,x);    
    hy.d = y;                                                // GET_HIGH_WORD(hy,y);    
    hx.ui[1] = ((hx.ui[1]&0x7fffffff)|(hy.ui[1]&0x80000000));// SET_HIGH_WORD(x,(hx&0x7fffffff)|(hy&0x80000000));
    x = hx.d;                                                // keep the low part as it is
    return x;
    }

IS double scalbn (double x, int n)
    {
    int k;
    union_for_double hx;
    hx.d = x;                                                // EXTRACT_WORDS(hx,lx,x);    
    k = ((hx.i[1])&0x7ff00000)>>20;                          // k = (hx&0x7ff00000)>>20; /* extract exponent */
    if (k==0) {                                              // 0 or subnormal x         
        if (((hx.i[0])|((hx.i[1])&0x7fffffff))==0)           // if ((lx|(hx&0x7fffffff))==0) 
            return x;                                        // +-0 
        x *= two54;        
        hx.d = x;                                            // GET_HIGH_WORD(hx,x);
        k = (((hx.i[1])&0x7ff00000)>>20) - 54;
        if (n< -50000) return tiny*x;                        // underflow
        }
    if (k==0x7ff) return x+x;                                // NaN or Inf
    k = k+n;
    if (k > 0x7fe) return huge*copysign(huge,x);             // overflow
    if (k > 0)                                               // normal result
        {        
        union_for_double temp;
        temp.d = x;                                          // SET_HIGH_WORD(x,(hx&0x800fffff)|(k<<20));
        temp.i[1] = ((hx.i[1])&0x800fffff)|(k<<20);
        x = temp.d;
        return x;
        }
    if (k <= -54)
        if (n > 50000)                                       // in case integer overflow in n+k
            return huge*copysign(huge,x);                    // overflow
        else return tiny*copysign(tiny,x);                   // underflow
        k += 54;                                             // subnormal result        
        union_for_double temp;
        temp.d = x;                                          // SET_HIGH_WORD(x,(hx&0x800fffff)|(k<<20));
        temp.i[1] = ((hx.i[1])&0x800fffff)|(k<<20);
        x = temp.d;
        return x*twom54;
    }

IS int __kernel_rem_pio2(double *x, double *y, int e0, int nx, int prec, const int *ipio2)
    {
    int jz,jx,jv,jp,jk,carry,n,iq[20],i,j,k,m,q0,ih;
    double z,fw,f[20],fq[20],q[20];

    jk = init_jk[prec];                                      // initialize jk
    jp = jk;    
    jx = nx-1;                                               // determine jx,jv,q0, note that 3>q0
    jv = (e0-3)/24; if(jv<0) jv=0;
    q0 = e0-24*(jv+1);    
    j = jv-jx; m = jx+jk;                                    // set up f[0] to f[jx+jk] where f[jx+jk] = ipio2[jv+jk]
    for(i=0;i<=m;i++,j++) f[i] = (j<0)? zero : (double) ipio2[j];    
    for (i=0;i<=jk;i++) {                                    // compute q[0],q[1],...q[jk]
        for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j]; q[i] = fw;
        }
    jz = jk;
recompute:    
    for(i=0,j=jz,z=q[jz];j>0;i++,j--) {                      // distill q[] into iq[] reversingly
        fw = (double)((int)(twon24* z));
        iq[i] = (int)(z-two24*fw);
        z = q[j-1]+fw;
        }

    z = scalbn(z,q0);                                        // actual value of z
    z -= 8.0*floor(z*0.125);                                 // trim off integer >= 8
    n = (int) z;                                             // compute n
    z -= (double)n;
    ih = 0;
    if(q0>0) {                                               // need iq[jz-1] to determine n */
        i = (iq[jz-1]>>(24-q0)); n += i;
        iq[jz-1] -= i<<(24-q0);
        ih = iq[jz-1]>>(23-q0);
        } else if(q0==0) ih = iq[jz-1]>>23;
        else if(z>=0.5) ih=2;

        if(ih>0) {                                           // q > 0.5
            n += 1; carry = 0;
            for(i=0;i<jz ;i++) {                             // compute 1-q
                j = iq[i];
                if(carry==0) {
                    if(j!=0) {
                        carry = 1; iq[i] = 0x1000000- j;
                        }
                    } else iq[i] = 0xffffff - j;
                }
            if(q0>0) {                                       // rare case: chance is 1 in 12
                switch(q0) {
                           case 1:iq[jz-1] &= 0x7fffff; break;
                           case 2:iq[jz-1] &= 0x3fffff; break;
                    }
                }
            if(ih==2) {
                z = one - z;
                if(carry!=0) z -= scalbn(one,q0);
                }
            }

        if(z==zero) {                                        // check if recomputation is needed
            j = 0;
            for (i=jz-1;i>=jk;i--) j |= iq[i];
            if(j==0) {                                       // need recomputation
                for(k=1;iq[jk-k]==0;k++);                    // k = no. of terms needed

                for(i=jz+1;i<=jz+k;i++) {                    // add q[jz+1] to q[jz+k]
                    f[jx+i] = (double) ipio2[jv+i];
                    for(j=0,fw=0.0;j<=jx;j++) fw += x[j]*f[jx+i-j];
                    q[i] = fw;
                    }
                jz += k;
                goto recompute;
                }
            }

        if(z==0.0) {                                         // chop off zero terms
            jz -= 1; q0 -= 24;
            while(iq[jz]==0) { jz--; q0-=24;}
            } else {                                         // break z into 24-bit if necessary
                z = scalbn(z,-q0);
                if(z>=two24) { fw = (double)((int)(twon24*z));
                iq[jz] = (int)(z-two24*fw);
                jz += 1; q0 += 24;
                iq[jz] = (int) fw;
                    } else iq[jz] = (int) z ;
            }
        
        fw = scalbn(one,q0);                                 // convert integer "bit" chunk to floating-point value
        for(i=jz;i>=0;i--) {
            q[i] = fw*(double)iq[i]; fw*=twon24;
            }
        
        for(i=jz;i>=0;i--) {                                 // compute PIo2[0,...,jp]*q[jz,...,0]
            for(fw=0.0,k=0;k<=jp&&k<=jz-i;k++) fw += PIo2[k]*q[i+k];
            fq[jz-i] = fw;
            }
        
        switch(prec) {                                       // compress fq[] into y[] */
              case 0:
                       fw = 0.0;
                       for (i=jz;i>=0;i--) fw += fq[i];
                       y[0] = (ih==0)? fw: -fw; break;
               case 1:
               case 2:
                       fw = 0.0;
                       for (i=jz;i>=0;i--) fw += fq[i]; y[0] = (ih==0)? fw: -fw; fw = fq[0]-fw;
                       for (i=1;i<=jz;i++) fw += fq[i];
                       y[1] = (ih==0)? fw: -fw; break;
               case 3:                                       // painful
                       for (i=jz;i>0;i--) {
                            fw = fq[i-1]+fq[i]; fq[i] += fq[i-1]-fw;
                            fq[i-1] = fw;
                        }
                       for (i=jz;i>1;i--) {
                            fw = fq[i-1]+fq[i]; fq[i] += fq[i-1]-fw;
                            fq[i-1] = fw;
                        }
                      for (fw=0.0,i=jz;i>=2;i--) fw += fq[i]; if(ih==0) {
                            y[0] = fq[0]; y[1] = fq[1]; y[2] = fw;
                        } else {
                            y[0] = -fq[0]; y[1] = -fq[1]; y[2] = -fw;
                        }
                      }
         return n&7;
    }

IS int __ieee754_rem_pio2(double x, double *y)
    {
    double z,w,t,r,fn;
    double tx[3];    
    int e0,i,j,nx,n, ix;                                     // int e0,i,j,nx,n,ix,hx;
    union_for_double hx;    

    hx.d = x;                                                // GET_HIGH_WORD(hx,x); /* high word of x */   
    ix = (hx.i[1])&0x7fffffff;
    if(ix<=0x3fe921fb)                                       // |x| ~<= pi/4 , no need for reduction
        {y[0] = x; y[1] = 0; return 0;}
    if(ix<0x4002d97c) {                                      // |x| < 3pi/4, special case with n=+-1
        if((hx.i[1])>0) { z = x - pio2_1;
        if(ix!=0x3ff921fb) {                                 // 33+53 bit pi is good enough
            y[0] = z - pio2_1t;
            y[1] = (z-y[0])-pio2_1t;
            } else {                                         // near pi/2, use 33+33+53 bit pi
                z -= pio2_2;
                y[0] = z - pio2_2t;
                y[1] = (z-y[0])-pio2_2t;
            }
        return 1;
            } else {                                         // negative x
                z = x + pio2_1;
                if(ix!=0x3ff921fb) {                         // 33+53 bit pi is good enough
                    y[0] = z + pio2_1t;
                    y[1] = (z-y[0])+pio2_1t;
                    } else {                                 // near pi/2, use 33+33+53 bit pi
                        z += pio2_2;
                        y[0] = z + pio2_2t;
                        y[1] = (z-y[0])+pio2_2t;
                    }
                return -1;
            }
        }
    if(ix<=0x413921fb) {                                     // |x| ~<= 2^19*(pi/2), medium size
        t = fabs(x);
        n = (int) (t*invpio2+half);
        fn = (double)n;
        r = t-fn*pio2_1;
        w = fn*pio2_1t;                                      // 1st round good to 85 bit
        if(n<32&&ix!=npio2_hw[n-1]) { y[0] = r-w;            // quick check no cancellation
            } else {
                union_for_double high;
                j = ix>>20;
                y[0] = r-w;                
                high.d = y[0];                               // GET_HIGH_WORD(high,y[0]);                
                i = j-(((high.ui[1])>>20)&0x7ff);            // i = j-((high>>20)&0x7ff);
                if(i>16) {                                   // 2nd iteration needed, good to 118
                    t = r;
                    w = fn*pio2_2; r = t-w;
                    w = fn*pio2_2t-((t-r)-w); y[0] = r-w;                    
                    high.d = y[0];                           //GET_HIGH_WORD(high,y[0]);
                    i = j-(((high.ui[1])>>20)&0x7ff);
                    if(i>49) {                               // 3rd iteration need, 151 bits acc
                        t = r;                               // will cover all possible cases
                        w = fn*pio2_3; r = t-w;
                        w = fn*pio2_3t-((t-r)-w); y[0] = r-w;
                        }
                    }
            }
        y[1] = (r-y[0])-w;
        if((hx.i[1])<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
        else return n;
        }
    // all other (large) arguments
    if(ix>=0x7ff00000) {                                     // x is inf or NaN
        y[0]=y[1]=x-x; return 0;
        }
    union_for_double temp;
    temp.d = x;                                              // to get the lower part of x to z;
    e0 = (ix>>20)-1046;                                      // e0 = ilogb(z)-23;
    temp.i[1] = ix - ((int)(e0<<20));                        // compute the high part to put into z;
    z = temp.d;                                              // copy the original low part of x and new high part

    for(i=0;i<2;i++) {
        tx[i] = (double)((int)(z));
        z = (z-tx[i])*two24;
        }
    tx[2] = z;
    nx = 3;
    while(tx[nx-1]==zero) nx--;                              // skip zero term
    n = __kernel_rem_pio2(tx,y,e0,nx,2,two_over_pi);
    if(hx.i[1]<0) {y[0] = -y[0]; y[1] = -y[1]; return -n;}
    return n;
    }

IS double __kernel_cos(double x, double y)
    {
    double hz,z,r,w;

    z = x*x;
    r = z*(C1_d+z*(C2_d+z*(C3_d+z*(C4_d+z*(C5_d+z*C6_d)))));
    hz = (float)0.5*z;
    w = one-hz;
    return w + (((one-w)-hz) + (z*r-x*y));
    }

IS double Cos_64f(double x)
    {
    double y[2],z=0.0;
    int n, ix;
    union_for_double temp;
    temp.d = x;
    ix = temp.i[1];    
    ix &= 0x7fffffff;                                        // |x| ~< pi/4
    if(ix <= 0x3fe921fb) {
        if(ix<0x3e400000)                                    // if x < 2**-27
            if(((int)x)==0) return 1.0;                      // generate inexact
        return __kernel_cos(x,z);
        }    
    else if (ix>=0x7ff00000) return x-x;                     // cos(Inf or NaN) is NaN    
    else {                                                   // argument reduction needed
        n = __ieee754_rem_pio2(x,y);
        switch(n&3) {
                     case 0: return __kernel_cos(y[0],y[1]);
                     case 1: return -__kernel_sin(y[0],y[1],1);
                     case 2: return -__kernel_cos(y[0],y[1]);
                    default: return __kernel_sin(y[0],y[1],1);
            }
        }
    }

ISV Cos_64f_A53( const double * s, double * d)
    {
    *d = Cos_64f(*s);
    }

///////////////////////////////////////// COS FLOAT REF CODE ////////////////////////////////////////////

ISV Cos_32f_A24( const float * s, float * d)
    {
    *d = (float)Cos_64f(*s);
    }

///////////////////////////////////////// SIN DOUBLE REF CODE ////////////////////////////////////////////

IS double __kernel_sin(double x, double y, int iy)
    {
    double z,r,v;

    z = x*x;
    v = z*x;
    r = S2+z*(S3+z*(S4+z*(S5+z*S6)));
    if(iy==0) return x+v*(S1+z*r);
    else return x-((z*(half*y-v*r)-y)-v*S1);
    }

IS double Sin_64f(double x)
    {
    double y[2],z=0.0;
    int n, ix;
    union_for_double temp;
    temp.d = x;
    ix = temp.i[1];    
    ix &= 0x7fffffff;                                        // |x| ~< pi/4
    if(ix <= 0x3fe921fb) {
        if(ix<0x3e400000)                                    // |x| < 2**-27
            {if((int)x==0) return x;}                        // generate inexact
        return __kernel_sin(x,z,0);
        }    
    else if (ix>=0x7ff00000) return x-x;                     // sin(Inf or NaN) is NaN    
    else {                                                   // argument reduction needed
        n = __ieee754_rem_pio2(x,y);
        switch(n&3) {
                     case 0: return __kernel_sin(y[0],y[1],1);
                     case 1: return __kernel_cos(y[0],y[1]);
                     case 2: return -__kernel_sin(y[0],y[1],1);
                    default: return -__kernel_cos(y[0],y[1]);
            }
        }
    }

ISV Sin_64f_A53( const double * s, double * d)
    {
    *d = Sin_64f(*s);
    }

///////////////////////////////////////// SIN FLOAT REF CODE ////////////////////////////////////////////

ISV Sin_32f_A24( const float * s, float * d)
    {
    *d = (float)Sin_64f(*s);
    }

///////////////////////////////////////// SINCOS DOUBLE REF CODE ////////////////////////////////////////////

template<class TD>
ISV SinCos_64f(const double x, double *d1,double *d2)
    {
    double y[2],z=0.0;
    int n, ix;
    union_for_double temp;
    temp.d = x;
    ix = temp.i[1];    
    ix &= 0x7fffffff;                                        // |x| ~< pi/4
    if(ix <= 0x3fe921fb)
        {
        if(ix<0x3e400000)                                    // |x| < 2**-27
            {
            if((int)x==0)
                {
                *d1 = (TD)x;
                *d2 = (TD)1.0;
                return;
                } 
            }
        *d1 = (TD) __kernel_sin(x,z,0);
        *d2 = (TD)__kernel_cos(x,z);
        return;
        }    
    else if (ix>=0x7ff00000)                                 // sin(Inf or NaN) is NaN
        {
        *d1 = *d2 = (TD)(x-x);
        return;
        }    
    else {                                                   // argument reduction needed
        n = __ieee754_rem_pio2(x,y);
        switch(n&3) {
                     case 0: *d1 = (TD)__kernel_sin(y[0],y[1],1);
                             *d2 = (TD)__kernel_cos(y[0],y[1]);
                              return;
                     case 1: *d1 = (TD) __kernel_cos(y[0],y[1]);
                             *d2 = (TD) -__kernel_sin(y[0],y[1],1);
                              return;
                     case 2: *d1 = (TD) -__kernel_sin(y[0],y[1],1);
                             *d2 = (TD) -__kernel_cos(y[0],y[1]);
                              return;
                    default: *d1 = (TD) -__kernel_cos(y[0],y[1]);
                             *d2 = (TD) __kernel_sin(y[0],y[1],1);
                              return;
            }
        }
    }

template<class TS, class TD1, class TD2>
ISV SinCos_64f_A53( TS * s, TD1 * d1, TD2 * d2)
    {
    s;d1;d2;
    //SinCos_64f<Fw64f>(*s,d1,d2);
    }

///////////////////////////////////////// SINCOS FLOAT REF CODE ////////////////////////////////////////////

//Needs special attention from foreach 1d , 2 destinations should be taken care.
template<class TS, class TD1, class TD2>
ISV SinCos_32f_A24( TS * s, TD1 * d1, TD2 *d2)
    {
    s;d1;d2;
    //SinCos_64f<Fw32f>(*s,d1,d2);
    }

///////////////////////////////////////// TAN DOUBLE REF CODE ////////////////////////////////////////////

IS double __kernel_tan(double x, double y, int iy)
    {
    double z, r, v, w, s;
    int ix;
    union_for_double hx;
    hx.d =x;
    ix = hx.i[1] & 0x7fffffff;                               // high word of |x| 
    if (ix >= 0x3FE59428) {                                  // |x| >= 0.6744
        if (hx.i[1] < 0) {
            x = -x;
            y = -y;
            }
        z = pio4 - x;
        w = pio4lo - y;
        x = z + w;
        y = 0.0;
        }
    z = x * x;
    w = z * z;
   
    // Break x^5*(T[1]+x^2*T[2]+...) into x^5(T[1]+x^4*T[3]+...+x^20*T[11]) + x^5(x^2*(T[2]+x^4*T[4]+...+x^22*[T12]))
    r = T[1] + w * (T[3] + w * (T[5] + w * (T[7] + w * (T[9] +
        w * T[11]))));
    v = z * (T[2] + w * (T[4] + w * (T[6] + w * (T[8] + w * (T[10] +
        w * T[12])))));
    s = z * x;
    r = y + z * (s * (r + v) + y);
    r += T[0] * s;
    w = x + r;
    if (ix >= 0x3FE59428) {
        v = (double) iy;
        return (double) (1 - ((hx.i[1] >> 30) & 2)) *
            (v - 2.0 * (x - (w * w / (w + v) - r)));
        }
    if (iy == 1)
        return w;
    else {
        /*
        * if allow error up to 2 ulp, simply return
        * -1.0 / (x+r) here
        */
        /* compute -1.0 / (x+r) accurately */
        double a, t;
        z = w;
        union_for_double temp;
        temp.d = z;
        temp.i[0] = 0;
        z = temp.d;
        v = r - (z - x);                                     // z+v = r+x
        t = a = -1.0 / w;                                    // a = -1.0/w        
        temp.d = t;                                          // SET_LOW_WORD(t,0);
        temp.i[0] = 0;
        t = temp.d;

        s = 1.0 + t * z;
        return t + a * (s + t * v);
        }
    }

IS double Tan_64f(double x)
    {
    double y[2],z=0.0;
    int n;
    union_for_double ix;    
    ix.d =x;                                                 // GET_HIGH_WORD(ix,x);    
    ix.i[1] &= 0x7fffffff;
    if(ix.i[1] <= 0x3fe921fb) {                              // |x| ~< pi/4
        if(ix.i[1]<0x3e300000)                               // x < 2**-28
            if((int)x==0) return x;                          // generate inexact
        return __kernel_tan(x,z,1);
        }    
    else if (ix.i[1]>=0x7ff00000) return x-x;                // tan(Inf or NaN) is NaN    
    else {                                                   // argument reduction needed
        n = __ieee754_rem_pio2(x,y);
        return __kernel_tan(y[0],y[1],1-((n&1)<<1));         // 1 -- n even -1 -- n odd                                                    
        }
    }

ISV Tan_64f_A53( const double * s, double * d)
    {
    *d = Tan_64f(*s);
    }

///////////////////////////////////////// TAN FLOAT REF CODE ////////////////////////////////////////////

ISV Tan_32f_A24( const float * s, float * d)
    {
    *d = (float)Tan_64f(*s);
    }

///////////////////////////////////////// ACOS DOUBLE REF CODE ////////////////////////////////////////////

IS double Acos_64f(double x)
    {
    double z,p,q,r,w,s,c;
    int ix;
    union_for_double hx, df;
    hx.d =x;
    ix = hx.i[1]&0x7fffffff;
    if(ix>=0x3ff00000) {                                     // |x| >= 1
        if(((ix-0x3ff00000)|hx.ui[0])==0) {                  // |x|==1
            if(hx.i[1]>0) return 0.0;                        // acos(1) = 0
            else return pi_d+2.0*pio2_lo_d;                  // acos(-1)= pi
            }
        return (x-x)/(x-x);                                  // acos(|x|>1) is NaN
        }
    if(ix<0x3fe00000) {                                      // |x| < 0.5
        if(ix<=0x3c600000) return pio2_hi_d+pio2_lo_d;       // if|x|<2**-57
        z = x*x;
        p = z*(pS0_d+z*(pS1_d+z*(pS2_d+z*(pS3_d+z*(pS4_d+z*pS5_d)))));
        q = one_d+z*(qS1_d+z*(qS2_d+z*(qS3_d+z*qS4_d)));
        r = p/q;
        return pio2_hi_d - (x - (pio2_lo_d-x*r));
        } else if (hx.i[1]<0) {                              // x < -0.5
            z = (one_d+x)*0.5;
            p = z*(pS0_d+z*(pS1_d+z*(pS2_d+z*(pS3_d+z*(pS4_d+z*pS5_d)))));
            q = one_d+z*(qS1_d+z*(qS2_d+z*(qS3_d+z*qS4_d)));
            s = sqrt(z);
            r = p/q;
            w = r*s-pio2_lo_d;
            return pi_d - 2.0*(s+w);
        } else {                                             // x > 0.5
            z = (one_d-x)*0.5;
            s = sqrt(z);
            df.d = s;            
            df.i[0] = 0;                                     // SET_LOW_WORD(df,0);
            c = (z-df.d*df.d)/(s+df.d);
            p = z*(pS0_d+z*(pS1_d+z*(pS2_d+z*(pS3_d+z*(pS4_d+z*pS5_d)))));
            q = one_d+z*(qS1_d+z*(qS2_d+z*(qS3_d+z*qS4_d)));
            r = p/q;
            w = r*s+c;
            return 2.0*(df.d+w);
            }
    }

ISV Acos_64f_A53( const double * s, double * d)
    {
    *d = Acos_64f(*s);
    }

///////////////////////////////////////// ACOS FLOAT REF CODE ////////////////////////////////////////////

IS float Acos_32f(float x)
    {
    float z,p,q,r,w,s,c,df;    
    int ix;
    union_for_float hx;    
    hx.f = x;
    ix = hx.i&0x7fffffff;
    if(ix==0x3f800000) {                                     // |x|==1
        if(hx.i>0) return 0.0;                               // acos(1) = 0
        else return pi_f+(float)2.0*pio2_lo_f;               // acos(-1)= pi
        } else if(ix>0x3f800000) {                           // |x| >= 1
            return (x-x)/(x-x);                              // acos(|x|>1) is NaN
        }
    if(ix<0x3f000000) {                                      // |x| < 0.5
        if(ix<=0x23000000) return pio2_hi_f+pio2_lo_f;       // if|x|<2**-57
        z = x*x;
        p = z*(pS0_f+z*(pS1_f+z*(pS2_f+z*(pS3_f+z*(pS4_f+z*pS5_f)))));
        q = one_f+z*(qS1_f+z*(qS2_f+z*(qS3_f+z*qS4_f)));
        r = p/q;
        return pio2_hi_f - (x - (pio2_lo_f-x*r));
        } else if (hx.i<0) {                                 // x < -0.5
            z = (one_f+x)*(float)0.5;
            p = z*(pS0_f+z*(pS1_f+z*(pS2_f+z*(pS3_f+z*(pS4_f+z*pS5_f)))));
            q = one_f+z*(qS1_f+z*(qS2_f+z*(qS3_f+z*qS4_f)));
            s = sqrt(z);
            r = p/q;
            w = r*s-pio2_lo_f;
            return pi_f - (float)2.0*(s+w);
        } else {                                             // x > 0.5
            union_for_float idf;
            z = (float)((one_f-x)*(float)0.5);
            s = sqrt(z);
            df = s;            
            idf.f = df;                                      //GET_FLOAT_WORD(idf,df);
            idf.i = idf.i&0xfffff000;            
            df = idf.f;                                      //SET_FLOAT_WORD(df,idf);
            c = (z-df*df)/(s+df);
            p = z*(pS0_f+z*(pS1_f+z*(pS2_f+z*(pS3_f+z*(pS4_f+z*pS5_f)))));
            q = one_f+z*(qS1_f+z*(qS2_f+z*(qS3_f+z*qS4_f)));
            r = p/q;
            w = r*s+c;
            return (float)2.0*(df+w);
            }
    }

ISV Acos_32f_A24( const float * s, float * d)
    {
    *d = Acos_32f(*s);
    }

///////////////////////////////////////// ASIN DOUBLE REF CODE ////////////////////////////////////////////

IS double Asin_64f(double x)
    {
    double t=0.0,w,p,q,c,r,s;
    int ix;
    union_for_double hx;    
    hx.d = x;                                                // GET_HIGH_WORD(hx,x);
    ix = hx.i[1]&0x7fffffff;
    if(ix>= 0x3ff00000) {                                    // |x|>= 1
        if(((ix-0x3ff00000)|hx.ui[0])==0)            
            return x*pio2_hi_d+x*pio2_lo_d;                  // asin(1)=+-pi/2 with inexact
        return (x-x)/(x-x);                                  // asin(|x|>1) is NaN
      } else if (ix<0x3fe00000) {                            // |x|<0.5
                if(ix<0x3e400000) {                          // if |x| < 2**-27
                    if(huge_d+x>one_d) return x;             // return x with inexact if x!=0
                    } else t = x*x;
                p = t*(pS0_d+t*(pS1_d+t*(pS2_d+t*(pS3_d+t*(pS4_d+t*pS5_d)))));
                q = one_d+t*(qS1_d+t*(qS2_d+t*(qS3_d+t*qS4_d)));
                w = p/q;
                return x+x*w;
            }        
        w = one_d-fabs(x);                                   // 1> |x|>= 0.5
        t = w*0.5;
        p = t*(pS0_d+t*(pS1_d+t*(pS2_d+t*(pS3_d+t*(pS4_d+t*pS5_d)))));
        q = one_d+t*(qS1_d+t*(qS2_d+t*(qS3_d+t*qS4_d)));
        s = sqrt(t);
        if(ix>=0x3FEF3333) {                                 // if |x| > 0.975
            w = p/q;
            t = pio2_hi_d-(2.0*(s+s*w)-pio2_lo_d);
            } else {
                union_for_double temp;
                temp.d = s;                                  // preserve the low word of s;                
                temp.i[0] = 0;                               // SET_LOW_WORD(w,0);
                w = temp.d;
                c = (t-w*w)/(s+w);
                r = p/q;
                p = 2.0*s*r-(pio2_lo_d-2.0*c);
                q = pio4_hi_d-2.0*w;
                t = pio4_hi_d-(p-q);
            }
        if(hx.i[1]>0) return t;
        else return -t;
    }

ISV Asin_64f_A53( const double * s,double * d)
    {
    *d = Asin_64f(*s);
    }

///////////////////////////////////////// ASIN FLOAT REF CODE ////////////////////////////////////////////

IS float Asin_32f(float x)
    {
    float t=0.0,w,p,q,c,r,s;
    int ix;
    union_for_float hx;
    hx.f = x;
    ix = hx.i&0x7fffffff;
    if(ix==0x3f800000) {        
        return x*pio2_hi_f+x*pio2_lo_f;                      // asin(1)=+-pi/2 with inexact
        } else if(ix> 0x3f800000) {                          // |x|>= 1
            return (x-x)/(x-x);                              // asin(|x|>1) is NaN
        } else if (ix<0x3f000000) {                          // |x|<0.5 
            if(ix<0x32000000) {                              // if |x| < 2**-27
                if(huge_f+x>one_f) return x;                 // return x with inexact if x!=0
                } else
                    t = x*x;
                p = t*(pS0_f+t*(pS1_f+t*(pS2_f+t*(pS3_f+t*(pS4_f+t*pS5_f)))));
                q = one_f+t*(qS1_f+t*(qS2_f+t*(qS3_f+t*qS4_f)));
                w = p/q;
                return x+x*w;
            }        
        w = one_f-fabsf(x);                                  // 1> |x|>= 0.5
        t = w*(float)0.5;
        p = t*(pS0_f+t*(pS1_f+t*(pS2_f+t*(pS3_f+t*(pS4_f+t*pS5_f)))));
        q = one_f+t*(qS1_f+t*(qS2_f+t*(qS3_f+t*qS4_f)));
        s = sqrt(t);
        if(ix>=0x3F79999A) {                                 // if |x| > 0.975
            w = p/q;
            t = pio2_hi_f-((float)2.0*(s+s*w)-pio2_lo_f);
            } else {
                union_for_float iw;                
                iw.f =s;                                     //w = s;                
                iw.i = iw.i&0xfffff000;                      // GET_FLOAT_WORD(iw,w);                
                w = iw.f;                                    //SET_FLOAT_WORD(w, iw);
                c = (t-w*w)/(s+w);
                r = p/q;
                p = (float)2.0*s*r-(pio2_lo_f-(float)2.0*c);
                q = pio4_hi_f-(float)2.0*w;
                t = pio4_hi_f-(p-q);
            }
        if(hx.i>0) return t; else return -t;
    }

ISV Asin_32f_A24( const float * s, float * d)
    {
    *d = Asin_32f(*s);
    }

///////////////////////////////////////// ATAN DOUBLE REF CODE ////////////////////////////////////////////

ISV Atan_64f_A53( const double * s, double * d)
    {
    *d = atan(*s);
    }

ISV Atan_64f_A53_inplace( double * d)
    {
    *d = atan(*d);
    }

///////////////////////////////////////// ATAN FLOAT REF CODE ////////////////////////////////////////////

ISV Atan_32f_A24( const float * s, float * d)
    {
    *d = atan(*s);
    }

ISV Atan_32f_A24_inplace( float * d)
    {
    *d = atan(*d);
    }

///////////////////////////////////////// ATAN2 DOUBLE REF CODE ////////////////////////////////////////////

IS double Atan2_64f(double y, double x)
    {
    double z;
    int k,m,ix,iy;
    union_for_double hx,hy;

    hx.d = x;
    ix = hx.i[1]&0x7fffffff;
    hy.d = y;
    iy = hy.i[1]&0x7fffffff;
    m = ((hy.i[1]>>31)&1)|((hx.i[1]>>30)&2);                 // 2*sign(x)+sign(y)    
    if((iy|hy.ui[0])==0) {                                   // when y = 0
        switch(m) {
                   case 0:
                   case 1: return y;                         // atan(+-0,+anything)=+-0 
                   case 2: return pi_d;                      // pi_d+tiny_d;/* atan(+0,-anything) = pi
                   case 3: return -pi_d;                     //-pi_d-tiny_d;/* atan(-0,-anything) =-pi
            }
        }    
    if(ix==0x7ff00000) {                                     // when x is INF
        if(iy==0x7ff00000) {
            switch(m) {
                       case 0: return pi_o_4_d;              // pi_o_4_d+tiny_d;/* atan(+INF,+INF) */
                       case 1: return -pi_o_4_d;             // -pi_o_4_d-tiny_d;/* atan(-INF,+INF) */
                       case 2: return pi3_o_4_d;             // (float)3.0*pi_o_4_d+tiny_d;/*atan(+INF,-INF)*/
                       case 3: return -pi3_o_4_d;            // (float)-3.0*pi_o_4_d-tiny_d;/*atan(-INF,-INF)*/
                }
            } else {
                switch(m) {
                           case 2: return pi_d;              // pi_d+tiny_d ; /* atan(+...,-INF) */
                           case 3: return -pi_d;             // -pi_d-tiny_d ; /* atan(-...,-INF) */
                    }
            }
        }    
    k = (iy-ix)>>20;                                         // compute y/x
    if(hx.i[1]<0&&k<-60) z=0.0;                              // |y|/x < -2**60
    else z= atan(fabs(y/x));                                 // safe to do y/x
    switch (m) {
                case 0: return z;                            // atan(+,+)
                case 1: return -z;                           // atan(-,+)
                case 2: return pi_hi_d-(z-pi_lo_d);          // atan(+,-)
               default: return (z-pi_lo_d)-pi_hi_d;          // atan(-,-)
        }
    }

ISV Atan2_64f_A53( const double * s1, const double * s2, double * d)
    {
    *d = Atan2_64f(*s1,*s2);
    }

///////////////////////////////////////// ATAN2 FLOAT REF CODE ////////////////////////////////////////////

IS float Atan2_32f(float y, float x)
    {
    float z;
    int k,m,ix,iy;
    union_for_float hx, hy;
    hx.f = x;
    ix = hx.i&0x7fffffff;
    hy.f = y;
    iy = hy.i&0x7fffffff;
    m = ((hy.i>>31)&1)|((hx.i>>30)&2);                       // 2*sign(x)+sign(y)    
    if(iy<0x00800000L) {                                     // when y = 0
        switch(m) {
                   case 0:
                   case 1: return y;                         // atan(+-0,+anything)=+-0
                   case 2: return pi_f;                      // pi_f+tiny_f;/* atan(+0,-anything) = pi 
                   case 3: return -pi_f;                     //-pi_f-tiny_f;/* atan(-0,-anything) =-pi 
            }
        }    
    if(ix==0x7f800000L) {                                    // when x is INF
        if(iy==0x7f800000L) {
            switch(m) {
                       case 0: return pi_o_4_f;              // pi_o_4_f+tiny_f;/* atan(+INF,+INF) */
                       case 1: return -pi_o_4_f;             // -pi_o_4_f-tiny_f;/* atan(-INF,+INF) */
                       case 2: return pi3_o_4_f;             // (float)3.0*pi_o_4_f+tiny_f;/*atan(+INF,-INF)*/
                       case 3: return -pi3_o_4_f;            // (float)-3.0*pi_o_4_f-tiny_f;/*atan(-INF,-INF)*/
                }
            } else {
                switch(m) {
                           case 2: return pi_f;              // pi_f+tiny_f ; /* atan(+...,-INF)
                           case 3: return -pi_f;             // -pi_f-tiny_f ; /* atan(-...,-INF)
                    }
            }
        }    
    k = (iy-ix)>>23;                                         // compute y/x
    if(hx.i<0&&k<-60) z=0.0;                                 // |y|/x < -2**60
    else z= atan(fabsf(y/x));                                // safe to do y/x

    switch (m) {
                case 0: return z;                            // atan(+,+)
                case 1: return -z;                           // atan(-,+)
                case 2: return pi_hi_f-(z-pi_lo_f);          // atan(+,-) 
               default: return (z-pi_lo_f)-pi_hi_f;          // atan(-,-)
        }
    }

ISV Atan2_32f_A24( const float * s1, const float * s2, float * d)
    {
    *d = Atan2_32f(*s1,*s2);
    }

///////////////////////////////////////// COSH DOUBLE REF CODE ////////////////////////////////////////////

IS double expm1(double x)
    {
    double y,hi,lo,c=0,t,e,hxs,hfx,r1;
    int k,xsb;
    union_for_double hx;
    hx.d = x;
    xsb = hx.ui[1]&0x80000000;                               // sign bit of x 
    if(xsb==0) y=x; else y= -x;                              // y = |x| 
    hx.ui[1] &= 0x7fffffff;                                  // high word of |x| 

    // filter out huge and non-finite argument 
    if(hx.ui[1] >= 0x4043687A) {                             // if |x|>=56*ln2 
        if(hx.ui[1] >= 0x40862E42) {                         // if |x|>=709.78... 
            if(hx.ui[1] >=0x7ff00000) {
                if(((hx.ui[1]&0xfffff)|hx.ui[0])!=0)
                    return x+x;                              // NaN 
                else return (xsb==0)? x:-1.0;                // exp(+-inf)={inf,-1} 
                }
            if(x > o_threshold_d)
                return CBL_F64_MAX;                          // overflow 
            }
        if(xsb!=0) {                                         // x < -56*ln2, return -1.0 with inexact 
            if(x+tiny_d<0.0)                                 // raise inexact 
                return tiny_d-one_d;                         // return -1 
            }
        }

    // argument reduction 
    if(hx.ui[1] > 0x3fd62e42) {                              // if |x| > 0.5 ln2 
        if(hx.ui[1] < 0x3FF0A2B2) {                          // and |x| < 1.5 ln2 
            if(xsb==0)
                {hi = x - ln2_hi_d; lo = ln2_lo_d; k = 1;}
            else
                {hi = x + ln2_hi_d; lo = -ln2_lo_d; k = -1;}
            } else {
                k = (int)(invln2_d*x+((xsb==0)?0.5:-0.5));
                t = k;
                hi = x - t*ln2_hi_d;                         // t*ln2_hi is exact here 
                lo = t*ln2_lo_d;
            }
        x = hi - lo;
        c = (hi-x)-lo;
        }
    else if(hx.ui[1] < 0x3c900000) {                         // when |x|<2**-54, return x 
        t = huge_d+x;                                        // return x with inexact flags when x!=0 
        return x - (t-(huge_d+x));
        }
    else k = 0;

    // x is now in primary range 
    hfx = 0.5*x;
    hxs = x*hfx;
    r1 = one_d+hxs*(Q1_d+hxs*(Q2_d+hxs*(Q3_d+hxs*(Q4_d+hxs*Q5_d))));
    t = 3.0-r1*hfx;
    e = hxs*((r1-t)/(6.0 - x*t));
    if(k==0) return x - (x*e-hxs);                           // c is 0 
    else {
        e = (x*(e-c)-c);
        e -= hxs;
        if(k== -1) return 0.5*(x-e)-0.5;
        if(k==1)
            if(x < -0.25) return -2.0*(e-(x+0.5));
            else return one_d+2.0*(x-e);
            if (k <= -2 || k>56) {                           // suffice to return exp(x)-1 
                union_for_double high;
                y = one_d-(e-x);                
                high.d = y;                                  // GET_HIGH_WORD(high,y);                
                high.ui[1] = high.ui[1]+(k<<20);             // add k to y's exponent // preserve the low word for y
                y = high.d;
                return y-one_d;
                }
            t = (double)one_d;
            if(k<20) {
                union_for_double high;                
                high.d = t;                                  //SET_HIGH_WORD(t,0x3ff00000 - (0x200000>>k)); // t=1-2^-k 
                high.i[1] = 0x3ff00000 - (0x200000>>k);
                t = high.d;
                y = t-(e-x);                
                high.d = y;                                  //GET_HIGH_WORD(high,y);                
                high.i[1] = high.ui[1]+(k<<20);              //SET_HIGH_WORD(y,high+(k<<20)); // add k to y's exponent 
                y = high.d;
                } else {
                    union_for_double high;                    
                    high.d = t;                              //SET_HIGH_WORD(t,((0x3ff-k)<<20)); // 2^-k 
                    high.i[1] = ((0x3ff-k)<<20);
                    t = high.d;
                    y = x-(e+t);
                    y += one_d;                    
                    high.d = y;                              //GET_HIGH_WORD(high,y);                    
                    high.i[1] = high.ui[1] + (k<<20);
                    y = high.d;
                }
        }
    return y;
    }

IS double __ieee754_exp(double x)                            // default IEEE double exp 
    {
    double y,hi=0.0,lo=0.0,c,t;
    int k=0,xsb;
    union_for_double hx;
    hx.d = x;
    xsb = (hx.ui[1]>>31)&1;                                  // sign bit of x 
    hx.ui[1] &= 0x7fffffff;                                  // high word of |x| 

    // filter out non-finite argument 
    if(hx.ui[1] >= 0x40862E42) {                             // if |x|>=709.78... 
        if(hx.ui[1]>=0x7ff00000) {
            if(((hx.ui[1]&0xfffff)|hx.ui[0])!=0)
                return x+x;                                  // NaN 
            else return (xsb==0)? x:0.0;                     // exp(+-inf)={inf,0} 
            }        
        if(x > o_threshold_d) return CBL_F64_MAX;            // overflow 
        if(x < u_threshold_d) return twom1000_d*twom1000_d;  // underflow 
        }

    // argument reduction 
    if(hx.ui[1] > 0x3fd62e42) {                              // if  |x| > 0.5 ln2         
        if(hx.ui[1] < 0x3FF0A2B2) {                          // and |x| < 1.5 ln2
        hi = x-ln2HI_d[xsb]; lo=ln2LO_d[xsb]; k = 1-xsb-xsb;
        } else {
            k = (int)(invln2_d*x+halF_d[xsb]);
            t = k;
            hi = x - t*ln2HI_d[0];                           // t*ln2HI is exact here 
            lo = t*ln2LO_d[0];
        }
    x = hi - lo;
        } else if(hx.ui[1] < 0x3e300000) {                   // when |x|<2**-28 
            if(huge_d+x>one_d) return one_d+x;               // trigger inexact 
        }
        else k = 0;        
        t = x*x;                                             // x is now in primary range 
        c = x - t*(P1_d+t*(P2_d+t*(P3_d+t*(P4_d+t*P5_d))));
        if(k==0) return one_d-((x*c)/(c-2.0)-x);
        else y = one_d-((lo-(x*c)/(2.0-c))-hi);
        if(k >= -1021) {
            union_for_double hy;            
            hy.d =y;                                         // GET_HIGH_WORD(hy,y);            
            hy.i[1] = hy.ui[1]+(k<<20);                      // SET_HIGH_WORD(y,hy+(k<<20)); // add k to y's exponent 
            y = hy.d;
            return y;
            } else {
                union_for_double hy;
                hy.d = y;
                hy.i[1] = hy.ui[1] +((k+1000)<<20);
                y = hy.d;
                return y*twom1000_d;
            }
    }

IS double Cosh_64f(double x)
    {
    double t,w;
    int ix;
    union_for_double hx;
    hx.d = x;
    ix = hx.i[1]&0x7fffffff;    
    if(ix>=0x7ff00000) return x*x;                           // x is INF or NaN     
    if(ix<0x3fd62e43) {                                      // |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) 
        t = expm1(fabs(x));
        w = one_d+t;
        if (ix<0x3c800000) return w;                         // cosh(tiny) = 1 
        return one_d+(t*t)/(w+w);
        }    
    if (ix < 0x40360000) {                                   // |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; 
        t = __ieee754_exp(fabs(x));
        return half_d*t+half_d/t;
        }    
    if (ix < 0x40862E42) return half_d*__ieee754_exp(fabs(x));// |x| in [22, log(maxdouble)] return half*exp(|x|)     
    if (ix<0x408633CE ||                                      // |x| in [log(maxdouble), overflowthresold] 
        ((ix==0x408633ce)&&((hx.ui[0])<=(unsigned int)0x8fb9f87d))) {
            w = __ieee754_exp(half_d*fabs(x));
            t = half_d*w;
            return t*w;
        }
    return Const::INF_64F;                                   // |x| > overflowthresold, cosh(x) overflow 
    }

ISV Cosh_64f_A53( const double * s, double * d)
    {
    *d = Cosh_64f(*s);
    }

///////////////////////////////////////// COSH FLOAT REF CODE ////////////////////////////////////////////

IS float Cosh_32f(float x)
    {
    float t,w;
    int ix;
    union_for_float temp;
    temp.f =x;
    ix = temp.i;
    ix &= 0x7fffffff;    
    if(ix>=0x7f800000) return x*x;                           // x is INF or NaN     
    if(ix<0x3eb17218) {                                      // |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) 
        t = (float)expm1(fabsf(x));
        w = one_f+t;
        if (ix<0x39800000) return one_f;                     // cosh(tiny) = 1 
        return one_f+(t*t)/(w+w);
        }    
    if (ix < 0x41100000) {                                   // |x| in [0.5*ln2,9], return (exp(|x|)+1/exp(|x|))/2; 
        t = (float)__ieee754_exp(fabsf(x));
        return half_f*t+half_f/t;
        }                                                    // |x| in [9, log(maxfloat)] return half*exp(|x|) 
    if (ix < 0x42b17217) return (float)(half_f*__ieee754_exp(fabsf(x)));    
    if (ix<=0x42b2d4fc) {                                    // |x| in [log(maxfloat), overflowthresold] 
        w = (float)__ieee754_exp(half_f*fabsf(x));
        t = half_f*w;
        return t*w;
        }    
    return Const::INF_32F;                                   // |x| > overflowthresold, cosh(x) overflow 
    }

ISV Cosh_32f_A24( const float * s, float * d)
    {
    *d = Cosh_32f(*s);
    }

///////////////////////////////////////// SINH DOUBLE REF CODE ////////////////////////////////////////////

IS double Sinh_64f(double x)
    {
    double t,w,h;
    int ix;
    union_for_double hx;
    hx.d =x;
    ix = hx.i[1]&0x7fffffff;    
    if(ix>=0x7ff00000) return x+x;                           // x is INF or NaN 
    h = 0.5;
    if (hx.i[1]<0) h = -h;
                                                             // |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) 
    if (ix < 0x40360000) {                                   // |x|<22 
        if (ix<0x3e300000)                                   // |x|<2**-28 
            if(shuge_d+x>one_d) return x;                    // sinh(tiny) = tiny with inexact 
        t = expm1(fabs(x));
        if(ix<0x3ff00000) return h*(2.0*t-t*t/(t+one_d));
        return h*(t+t/(t+one_d));
        }    
    if (ix < 0x40862E42)                                     // |x| in [22, log(maxdouble)] return 0.5*exp(|x|) 
        return h*__ieee754_exp(fabs(x));
                                                             // |x| in [log(maxdouble), overflowthresold] 
    if (ix<0x408633CE || ((ix==0x408633ce)&&(hx.ui[0]<=(unsigned int)0x8fb9f87d))) {
        w = __ieee754_exp(0.5*fabs(x));
        t = h*w;
        return t*w;
        }    
    return x*shuge_d;                                        // |x| > overflowthresold, sinh(x) overflow 
    }

ISV Sinh_64f_A53( const double * s, double * d)
    {
    *d = Sinh_64f(*s);
    }

///////////////////////////////////////// SINH FLOAT REF CODE ////////////////////////////////////////////

IS float Sinh_32f(float x)
    {
    float t,w,h;
    int ix;
    union_for_float hx;
    hx.f =x;
    ix = hx.i&0x7fffffff;    
    if(ix>=0x7f800000) return x+x;                           // x is INF or NaN 
    h = 0.5;
    if (hx.i<0) h = -h;
                                                             // |x| in [0,9], return sign(x)*0.5*(E+E/(E+1))) 
    if (ix < 0x41100000) {                                   // |x|<9 
        if (ix<0x39800000)                                   // |x|<2**-12 
            if(shuge_f+x>one_f) return x;                    // sinh(tiny) = tiny with inexact 
        t = (float)expm1(fabsf(x));
        if(ix<0x3f800000) return h*((float)2.0*t-t*t/(t+one_f));
        return h*(t+t/(t+one_f));
        }
    
    if (ix < 0x42b17217)                                     // |x| in [9, logf(maxfloat)] return 0.5*exp(|x|) 
        return (float)(h*__ieee754_exp(fabsf(x)));
    
    if (ix<=0x42b2d4fc) {                                    // |x| in [logf(maxfloat), overflowthresold] 
        w = (float)__ieee754_exp((float)0.5*fabsf(x));
        t = h*w;
        return t*w;
        }    
    return x*shuge_f;                                        // |x| > overflowthresold, sinh(x) overflow 
    }

ISV Sinh_32f_A24( const float * s, float * d)
    {
    *d = Sinh_32f(*s);
    }

///////////////////////////////////////// TANH DOUBLE REF CODE ////////////////////////////////////////////

IS double Tanh_64f(double x)
    {
    double t,z;
    int ix;
    union_for_double hx;
    hx.d = x;
    ix = hx.i[1]&0x7fffffff;    
    if(ix>=0x7ff00000) {                                     // x is INF or NaN 
        if (hx.i[1]>=0) return one_d/x+one_d;                // tanh(+-inf)=+-1 
        else return one_d/x-one_d;                           // tanh(NaN) = NaN 
        }
    if (ix < 0x40360000) {                                   // |x|<22 
        if (ix<0x3e300000) {                                 // |x|<2**-28 
            if(huge_d+x>one_d) return x;                     // tanh(tiny) = tiny with inexact 
            }
        if (ix>=0x3ff00000) {                                // |x|>=1 
            t = expm1(two_d*fabs(x));
            z = one_d - two_d/(t+two_d);
            } else {
                t = expm1(-two_d*fabs(x));
                z= -t/(t+two_d);
            }        
        } else {                                             // |x| >= 22, return +-1 
            z = one_d - tiny_d;                              // raise inexact flag 
        }
    return (hx.i[1]>=0)? z: -z;
    }

ISV Tanh_64f_A53( const double * s, double * d)
    {
    *d = Tanh_64f(*s);
    }

///////////////////////////////////////// TANH FLOAT REF CODE ////////////////////////////////////////////

IS float Tanh_32f(float x)
    {
    float t,z;
    int ix;
    union_for_float hx;
    hx.f = x;
    ix = hx.i&0x7fffffff;    
    if(ix>=0x7f800000) {                                     // x is INF or NaN 
        if (hx.i>=0) return one_f/x+one_f;                   // tanh(+-inf)=+-1 
        else return one_f/x-one_f;                           // tanh(NaN) = NaN 
        }
    if (ix < 0x41100000) {                                   // |x|<9 
        if (ix<0x39800000) {                                 // |x|<2**-12 
            if(huge_f+x>one_f) return x;                     // tanh(tiny) = tiny with inexact 
            }
        if (ix>=0x3f800000) {                                // |x|>=1 
            t = (float)expm1(two_f*fabsf(x));
            z = one_f - two_f/(t+two_f);
            } else {
                t = (float)expm1(-two_f*fabsf(x));
                z= -t/(t+two_f);
            }        
        } else {                                             // |x| >= 9, return +-1 
            z = one_f - tiny_f;                              // raise inexact flag 
        }
    return (hx.i>=0)? z: -z;
    }

ISV Tanh_32f_A24( const float * s, float * d)
    {
    *d = Tanh_32f(*s);
    }

///////////////////////////////////////// ACOSH DOUBLE REF CODE ////////////////////////////////////////////

IS double log1p(double x)
    {
    double hfsq,f=0,c=0,s,z,R,u;
    int k,hu=0,ax;
    union_for_double hx;
    hx.d = x;
    ax = hx.i[1]&0x7fffffff;
    k = 1;
    if (hx.i[1] < 0x3FDA827A) {                              // 1+x < sqrt(2)+ 
        if(ax>=0x3ff00000) {                                 // x <= -1.0             
            if(x==-1.0) return Const::INF_64F;               // if(x==-1.0) return -two54_d/zero_d; // log1p(-1)=+inf 
            else return (x-x)/(x-x);                         // log1p(x<-1)=NaN 
            }
        if(ax<0x3e200000) {                                  // |x| < 2**-29 
            if(two54_d+x>zero_d                              // raise inexact 
                &&ax<0x3c900000)                             // |x| < 2**-54 
                return x;
            else
                return x - x*x*0.5;
            }
        if(hx.i[1]>0||hx.i[1]<=((int)0xbfd2bec4)) {
            k=0;f=x;hu=1;}                                   // sqrt(2)/2- <= 1+x < sqrt(2)+ 
        }
    if (hx.i[1] >= 0x7ff00000) return x+x;
    if(k!=0) {
        if(hx.i[1]<0x43400000) {
            u = 1.0+x;
            union_for_double temp;
            temp.d = u;
            hu = temp.i[1];
            k = (hu>>20)-1023;
            c = (k>0)? 1.0-(u-x):x-(u-1.0);                  // correction term 
            c /= u;
            } else {
                u = x;
                union_for_double temp;
                temp.d = u;
                hu = temp.i[1];
                k = (hu>>20)-1023;
                c = 0;
            }
        hu &= 0x000fffff;
        /*
        * The approximation to sqrt(2) used in thresholds is not
        * critical. However, the ones used above must give less
        * strict bounds than the one here so that the k==0 case is
        * never reached from here, since here we have committed to
        * using the correction term but don't use it if k==0.*/
        
        if(hu<0x6a09e) {                                     // u ~< sqrt(2)              
            union_for_double temp;
            temp.d = u;                                      // SET_HIGH_WORD(u,hu|0x3ff00000);
            temp.i[1] = hu|0x3ff00000;                       // normalize u 
            u = temp.d;
            } else {
                k += 1;                
                union_for_double temp;
                temp.d = u;
                temp.i[1] = hu|0x3fe00000;                   // SET_HIGH_WORD(u,hu|0x3fe00000); 
                u = temp.d;                                  // normalize u/2 
                hu = (0x00100000-hu)>>2;
            }
        f = u-1.0;
        }
    hfsq=0.5*f*f;
    if(hu==0) {                                              // |f| < 2**-20 
        if(f==zero_d) if(k==0) return zero_d;
        else {c += k*ln2_lo_d; return k*ln2_hi_d+c;}
        R = hfsq*(1.0-0.66666666666666666*f);
        if(k==0) return f-R; else
            return k*ln2_hi_d-((R-(k*ln2_lo_d+c))-f);
        }
    s = f/(2.0+f);
    z = s*s;
    R = z*(Lp1_d+z*(Lp2_d+z*(Lp3_d+z*(Lp4_d+z*(Lp5_d+z*(Lp6_d+z*Lp7_d))))));
    if(k==0) return f-(hfsq-s*(hfsq+R)); else
        return k*ln2_hi_d-((hfsq-(s*(hfsq+R)+(k*ln2_lo_d+c)))-f);
    }

IS double __ieee754_log(double x)
    {
    double hfsq,f,s,z,R,w,t1,t2,dk;
    int k,i,j;
    union_for_double hx;
    hx.d = x;
    k=0;
    if (hx.i[1] < 0x00100000) {                              // x < 2**-1022         
        if (((hx.i[1]&0x7fffffff)|hx.ui[0])==0)              // if (((hx&0x7fffffff)|lx)==0)  
            return Const::INF_64F;                           // return -two54_d/zero_d; // log(+-0)=-inf         
        if (hx.i[1]<0)                                       // if (hx<0)
            return Const::NAN_64F;                           // return (x-x)/zero_d; // log(-#) = NaN 
        k -= 54; x *= two54_d;                               // subnormal number, scale up x 
        hx.d = x;
        } if (hx.i[1] >= 0x7ff00000) return x+x;
    k += (hx.i[1]>>20)-1023;
    hx.i[1] &= 0x000fffff;
    i = (hx.i[1]+0x95f64)&0x100000;     
    union_for_double temp;                                   // normalize x or x/2 
    temp.d = x;
    temp.i[1] = (hx.i[1])|(i^0x3ff00000);                    // SET_HIGH_WORD(x,hx|(i^0x3ff00000));
    x = temp.d;
    k += (i>>20);
    f = x-1.0;
    if((0x000fffff&(2+hx.i[1]))<3) {                         // -2**-20 <= f < 2**-20 
        if(f==zero_d) if(k==0) return zero_d; else {dk=(double)k;
        return dk*ln2_hi_d+dk*ln2_lo_d;}
        R = f*f*(0.5-0.33333333333333333*f);
        if(k==0) return f-R; else {dk=(double)k;
        return dk*ln2_hi_d-((R-dk*ln2_lo_d)-f);}
        }
    s = f/(2.0+f); dk = (double)k;
    z = s*s;
    i = hx.i[1]-0x6147a;
    w = z*z;
    j = 0x6b851-hx.i[1];
    t1= w*(Lg2_d+w*(Lg4_d+w*Lg6_d)); t2= z*(Lg1_d+w*(Lg3_d+w*(Lg5_d+w*Lg7_d))); i |= j;
    R = t2+t1;
    if(i>0) {
        hfsq=0.5*f*f;
        if(k==0) return f-(hfsq-s*(hfsq+R)); else
            return dk*ln2_hi_d-((hfsq-(s*(hfsq+R)+dk*ln2_lo_d))-f);
        } else {
            if(k==0) return f-s*(f-R); else
                return dk*ln2_hi_d-((s*(f-R)-dk*ln2_lo_d)-f);
        }
    }

IS double Acosh_64f(double x)
    {
    double t;
    int hx;
    unsigned int lx;    
    union_for_double temp;
    temp.d = x;
    hx = temp.i[1];                                          // EXTRACT_WORDS(hx,lx,x);
    lx = temp.ui[0];

    if(hx<0x3ff00000) {                                      // x < 1         
        return Const::NAN_64F;                               // return (x-x)/(x-x);
        } else if(hx >=0x41b00000) {                         // x > 2**28 
            if(hx >=0x7ff00000) {                            // x is inf of NaN 
                return x+x;
                } else return __ieee754_log(x)+ln2_d;        // acosh(huge)=log(2x) 
        } else if(((hx-0x3ff00000)|lx)==0) {
            return 0.0;                                      // acosh(1) = 0 
            } else if (hx > 0x40000000) {                    // 2**28 > x > 2 
                t=x*x;
                return __ieee754_log(2.0*x-one_d/(x+sqrt(t-one_d)));
            } else {                                         // 1<x<2 
                t = x-one_d;
                return log1p(t+sqrt(2.0*t+t*t));
                }
    }

ISV Acosh_64f_A53( const double * s, double * d)
    {
    *d = Acosh_64f(*s);
    }

///////////////////////////////////////// ACOSH FLOAT REF CODE ////////////////////////////////////////////

IS float log1pf(float x)
    {
    float hfsq,f=0,c=0,s,z,R,u;
    int k,hx,hu=0,ax;
    union_for_float temp;
    temp.f = x;
    hx = temp.i;
    ax = hx&0x7fffffff;
    k = 1;
    if (hx < 0x3ed413d0) {                                   // 1+x < sqrt(2)+ 
        if(ax>=0x3f800000) {                                 // x <= -1.0             
            if(x==(float)-1.0)                               // if(x==(float)-1.0) 
                return Const::INF_32F;                       // return -two25_f/zero_f; // log1p(-1)=+inf 
            else return (x-x)/(x-x);                         // log1p(x<-1)=NaN 
            }
        if(ax<0x31000000) {                                  // |x| < 2**-29 
            if(two25_f+x>zero_f                              // raise inexact 
                &&ax<0x24800000)                             // |x| < 2**-54 
                return x;
            else
                return x - x*x*(float)0.5;
            }
        if(hx>0||hx<=((int)0xbe95f619)) {
            k=0;f=x;hu=1;}                                   // sqrt(2)/2- <= 1+x < sqrt(2)+ 
        }
    if (hx >= 0x7f800000) return x+x;
    if(k!=0) {
        if(hx<0x5a000000) {
            *(volatile float *)&u = (float)1.0+x;
            temp.f = u;
            hu = temp.i;
            k = (hu>>23)-127;            
            c = (k>0)? (float)1.0-(u-x):x-(u-(float)1.0);    // correction term 
            c /= u;
            } else {
                u = x;
                temp.f = u;
                hu = temp.i;
                k = (hu>>23)-127;
                c = 0;
            }
        hu &= 0x007fffff;
        /*
        * The approximation to sqrt(2) used in thresholds is not
        * critical. However, the ones used above must give less
        * strict bounds than the one here so that the k==0 case is
        * never reached from here, since here we have committed to
        * using the correction term but don't use it if k==0.*/
        
        if(hu<0x3504f4) {                                    // u < sqrt(2)        
            temp.i = hu|0x3f800000;                          // int temp = hu|0x3f800000;// normalize u 
            u = temp.f;

            } else {
                k += 1;               
                temp.i = hu|0x3f000000;                      // int temp = hu|0x3f000000; // normalize u/2 
                u = temp.f;
                hu = (0x00800000-hu)>>2;
            }
        f = u-(float)1.0;
        }
    hfsq=(float)0.5*f*f;
    if(hu==0) {                                              // |f| < 2**-20 
        if(f==zero_f) if(k==0) return zero_f;
        else {c += k*ln2_lo_f; return k*ln2_hi_f+c;}
        R = hfsq*((float)1.0-(float)0.66666666666666666*f);
        if(k==0) return f-R; else
            return k*ln2_hi_f-((R-(k*ln2_lo_f+c))-f);
        }
    s = f/((float)2.0+f);
    z = s*s;
    R = z*(Lp1_f+z*(Lp2_f+z*(Lp3_f+z*(Lp4_f+z*(Lp5_f+z*(Lp6_f+z*Lp7_f))))));
    if(k==0) return f-(hfsq-s*(hfsq+R)); else
        return k*ln2_hi_f-((hfsq-(s*(hfsq+R)+(k*ln2_lo_f+c)))-f);
    }

IS float __ieee754_logf(float x)
    {
    float hfsq,f,s,z,R,w,t1,t2,dk;
    int k,ix,i,j;
    union_for_float temp;
    temp.f = x;
    ix = temp.i;
    k=0;
    if (ix < 0x00800000) {                                   // x < 2**-126 
        if ((ix&0x7fffffff)==0)            
            return -Const::INF_32F;                          // return -two25_f/zero_f; // log(+-0)=-inf          
        if (ix<0)                                            // if (ix<0)
            return Const::NAN_32F;                           // return (x-x)/zero_f; // log(-#) = NaN 
        k -= 25; x *= two25_f;                               // subnormal number, scale up x 
        union_for_float temp;
        temp.f = x;
        ix = temp.i;
        }
    if (ix >= 0x7f800000) return x+x;
    k += (ix>>23)-127;
    ix &= 0x007fffff;
    i = (ix+(0x95f64<<3))&0x800000;     
    temp.i = ix|(i^0x3f800000);                              // int temp = ix|(i^0x3f800000); // normalize x or x/2
    x = temp.f;
    k += (i>>23);
    f = x-(float)1.0;
    if((0x007fffff&(0x8000+ix))<0xc000) {                    // -2**-9 <= f < 2**-9 
        if(f==zero_f) if(k==0) return zero_f; else {dk=(float)k;
        return dk*ln2_hi_f+dk*ln2_lo_f;}
        R = f*f*((float)0.5-(float)0.33333333333333333*f);
        if(k==0) return f-R; else {dk=(float)k;
        return dk*ln2_hi_f-((R-dk*ln2_lo_f)-f);}
        }
    s = f/((float)2.0+f);
    dk = (float)k;
    z = s*s;
    i = ix-(0x6147a<<3);
    w = z*z;
    j = (0x6b851<<3)-ix;
    t1= w*(Lg2_f+w*Lg4_f);
    t2= z*(Lg1_f+w*Lg3_f);
    i |= j;
    R = t2+t1;
    if(i>0) {
        hfsq=(float)0.5*f*f;
        if(k==0) return f-(hfsq-s*(hfsq+R)); else
            return dk*ln2_hi_f-((hfsq-(s*(hfsq+R)+dk*ln2_lo_f))-f);
        } else {
            if(k==0) return f-s*(f-R); else
                return dk*ln2_hi_f-((s*(f-R)-dk*ln2_lo_f)-f);
        }
    }

IS float Acosh_32f(float x)
    {
    float t;
    int hx;
    union_for_float temp;
    temp.f = x;
    hx = temp.i;

    if(hx<0x3f800000) {                                      // x < 1         
        return Const::NAN_32F;                               // return (x-x)/(x-x);
        } else if(hx >=0x4d800000) {                         // x > 2**28 
            if(hx >=0x7f800000) {                            // x is inf of NaN 
                return x+x;
                } else
                    return __ieee754_logf(x)+ln2_f;          // acosh(huge)=log(2x) 
        } else if (hx==0x3f800000) {
            return 0.0;                                      // acosh(1) = 0 
            } else if (hx > 0x40000000) {                    // 2**28 > x > 2 
                t=x*x;
                return __ieee754_logf((float)2.0*x-one_f/(x+sqrt(t-one_f)));
            } else {                                         // 1<x<2 
                t = x-one_f;
                return log1pf(t+sqrt((float)2.0*t+t*t));
                }
    }

ISV Acosh_32f_A24( const float * s, float * d)
    {
    *d = Acosh_32f(*s);
    }

///////////////////////////////////////// ASINH DOUBLE REF CODE ////////////////////////////////////////////

IS double Asinh_64f(double x)
    {
    double t,w;
    int hx,ix;
    union_for_double temp;
    temp.d = x;
    hx = temp.i[1];

    ix = hx&0x7fffffff;
    if(ix>=0x7ff00000) return x+x;                           // x is inf or NaN 
    if(ix< 0x3e300000) {                                     // |x|<2**-28 
        if(huge_d+x>one_d) return x;                         // return x inexact except 0 
        }
    if(ix>0x41b00000) {                                      // |x| > 2**28 
        w = __ieee754_log(fabs(x))+ln2_d;
        } else if (ix>0x40000000) {                          // 2**28 > |x| > 2.0 
            t = fabs(x);
            w = __ieee754_log(2.0*t+one_d/(sqrt(x*x+one_d)+t));
        } else {                                             // 2.0 > |x| > 2**-28 
            t = x*x;
            w =log1p(fabs(x)+t/(one_d+sqrt(one_d+t)));
            }
        if(hx>0) return w; else return -w;
    }

ISV Asinh_64f_A53( const double * s, double * d)
    {
    *d = Asinh_64f(*s);
    }

///////////////////////////////////////// ASINH FLOAT REF CODE ////////////////////////////////////////////

IS float Asinh_32f(float x)
    {
    float t,w;
    int hx,ix;
    union_for_float temp;
    temp.f = x;
    hx = temp.i;

    ix = hx&0x7fffffff;
    if(ix>=0x7f800000) return x+x;                           // x is inf or NaN 
    if(ix< 0x31800000) {                                     // |x|<2**-28 
        if(huge_f+x>one_f) return x;                         // return x inexact except 0 
        }
    if(ix>0x4d800000) {                                      // |x| > 2**28 
        w = __ieee754_logf(fabsf(x))+ln2_f;
        } else if (ix>0x40000000) {                          // 2**28 > |x| > 2.0 
            t = fabsf(x);
            w = __ieee754_logf((float)2.0*t+one_f/(sqrt(x*x+one_f)+t));
        } else {                                             // 2.0 > |x| > 2**-28 
            t = x*x;
            w =log1pf(fabsf(x)+t/(one_f+sqrt(one_f+t)));
            }
        if(hx>0) return w; else return -w;
    }

ISV Asinh_32f_A24( const float * s, float * d)
    {
    *d = Asinh_32f(*s);
    }

///////////////////////////////////////// ATANH DOUBLE REF CODE ////////////////////////////////////////////

IS double Atanh_64f(double x)
    {
    double t;
    int hx,ix;
    unsigned int lx;
    union_for_double temp;
    temp.d = x;
    hx = temp.i[1];
    lx = temp.i[0];

    ix = hx&0x7fffffff;
    if ((ix|((lx|(-(signed int)lx))>>31))>0x3ff00000)        // |x|>1         
        return Const::NAN_64F;                               // return (x-x)/(x-x);    
    if(ix==0x3ff00000)                                       // if(ix==0x3ff00000) return x/zero_d;
        {
        if(x>0)
            return Const::INF_64F;
        else
            return -Const::INF_64F;
        }

    if(ix<0x3e300000&&(huge_d+x)>zero_d) return x;           // x<2**-28 
    temp.d = x;
    temp.i[1] = ix;
    x = temp.d;

    if(ix<0x3fe00000) {                                      // x < 0.5 
        t = x+x;
        t = 0.5*log1p(t+t*x/(one_d-x));
        } else t = 0.5*log1p((x+x)/(one_d-x));
    if(hx>=0) return t; else return -t;
    }

ISV Atanh_64f_A53( const double * s, double * d)
    {
    *d = Atanh_64f(*s);
    }

///////////////////////////////////////// ATANH FLOAT REF CODE ////////////////////////////////////////////

IS float Atanh_32f(float x)
    {
    float t;
    int hx,ix;
    union_for_float temp;
    temp.f = x;
    hx = temp.i;

    ix = hx&0x7fffffff;
    if (ix>0x3f800000)                                       // |x|>1 
        return (x-x)/(x-x);
    
    
    if(ix==0x3ff00000)                                       // if(ix==0x3f800000) // return x/zero_f;
        {
        if(x>0)
            return Const::INF_32F;
        else
            return -Const::INF_32F;
        }
    if(ix<0x31800000&&(huge_f+x)>zero_f) return x;           // x<2**-28 
    temp.i = ix;
    x = temp.f;
    if(ix<0x3f000000) {                                      // x < 0.5 
        t = x+x;
        t = (float)0.5*log1pf(t+t*x/(one_f-x));
        } else
            t = (float)0.5*log1pf((x+x)/(one_f-x));
        if(hx>=0) return t; else return -t;
    }

ISV Atanh_32f_A24( const float * s, float * d)
    {
    *d = Atanh_32f(*s);
    }

} // OPT_LEVEL

#endif


