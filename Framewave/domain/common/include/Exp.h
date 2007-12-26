/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _EXP_H_
#define _EXP_H_

#include "fe.h"

// Contains the exponential functions for all the data types.

// TABLES for exp 8u, 16s, 32s, 64s, 32f, 64f


namespace EXP_TABLES {

    SYS_FORCEALIGN_16 extern const Fw8u exp_8u_tbl[15][9];
    SYS_FORCEALIGN_16 extern const Fw16s exp_16s_tbl[21][14];
    SYS_FORCEALIGN_16 extern const float exp_32s_tbl[32];
    SYS_FORCEALIGN_16 extern const Fw64s exp_64s_tbl[64];
    SYS_FORCEALIGN_16 extern const double exp_64s_flt_tbl[64];
    SYS_FORCEALIGN_16 extern const double two_to_jby32_lead_64f[32];
    SYS_FORCEALIGN_16 extern const double two_to_jby32_trail_64f[32];
}






namespace OPT_LEVEL
{


//        REFERENCE UNIT FUNCTIONS

ISV exp_8u_ref_unit(const Fw8u &src, Fw8u &dst, const int &scaleFactor)
    {
    const Fw8u start[] = { 0, 0, 1, 2, 2, 3, 4};
    Fw8u strt = (scaleFactor > 0) ? start[scaleFactor-1] : 0 ;
    Fw8u tsrc = (src > strt) ? src - strt : 0;
    if (tsrc >= 8) {
        dst = 255 ;
        return ;
    }
    dst = EXP_TABLES::exp_8u_tbl[scaleFactor + 7 ][tsrc];
}


ISV exp_16s_ref_unit(const Fw16s &src, Fw16s &dst, const int &scaleFactor)
{
    Fw16s start[21] = { 8, 7, 7, 6, 5, 5, 4, 3, 3, 2, 1, 1, 0, -1, -2, -2, -3, -4, -4, -5, -6} ;
    Fw32s strt = start[scaleFactor+10] ;
    Fw32s tsrc = ((src+strt) < 13 ) ? src + strt : 13;
    tsrc = (tsrc > 0) ? tsrc : 0;
    dst = EXP_TABLES::exp_16s_tbl[scaleFactor+10][tsrc];
}

IS Fw32s exp_32s_ref_unit(const Fw32s &src, float scale, int max_exp_arg)
{
    if (src > max_exp_arg) return 0x7FFFFFFF ;    
    else if (src < 0) return  0;
    else return (Fw32s)(scale * EXP_TABLES::exp_32s_tbl[src] + 0.5F);
}

//        REFERENCE CALLING FUNCTIONS

template <CH cd>
ISV Exp_8u_ref(const Fw8u *src, Fw8u *dst, int scaleFactor)
{    
    for(int i=0 ; i< cd ; i++)
        exp_8u_ref_unit(src[i], dst[i], scaleFactor);
}

template <CH cd>
ISV Exp_16s_ref( const Fw16s *src, Fw16s *dst, int scaleFactor )
{    
    for(int i=0 ; i< cd ; i++)
        exp_16s_ref_unit(src[i], dst[i], scaleFactor);
}

template <CH cd>
ISV Exp_32s_ref( const Fw32s *s, Fw32s *d, float scale, int max_exp_arg )
{
    for(int i=0 ; i< cd ; i++)
        d[i] = exp_32s_ref_unit(s[i],  scale, max_exp_arg);    
}

template <CH cd>
ISV Exp_32f_ref(const Fw32f *s, Fw32f *d)
{
    for(int i=0 ; i< cd ; i++)
        d[i] = expf(s[i] );    
}    

//templated to work for sexp32f_64f also, along with exp_64f
template< class TS, class TD, CH cd >
ISV Exp_64f_ref(const TS *s, TD *d)
{
    for(int i=0 ; i< cd ; i++)
        d[i] = (TD) exp((TD)s[i] );
}    

SYS_INLINE STATIC void Exp_A11_ref( const Fw32f *s, Fw32f *d )
{
        float x = fabs(s[0]) ;

        if(x >  88.7228390F) 
        {
            d[0] = (s[0] > 0) ? Const::INF_32F : 0 ;
            return ;
        }
        else 
        {
            union {
                float f;
                int i;
            } m, tmp ;

            int j, n;
            float r, p ;

            r = s[0] * 4.6166240692E+01F ; 
            n = (int)(r + 0.5);                         //  n = nearest integer to r 
            j = (n << 27) >> 27;                        //n%32 ;
            m.i = n >> 5 ;
        
            r = s[0] - n*0.02166084939249829F ;         // r.f = _mm_sub_ps(src, _mm_mul_ps(n.f, log2_by_32));
            p = r + r*r*( 5.00000000000000008883e-01F + r*( 1.66666666665260878863e-01F ));
            r = CBL_EXP_TABLES::two_to_jby32_table[j & 0x1f];

            // reslt = (2^m)*(r+ P*r)
            tmp.f = r + r*p ;
            d[0] = (float)SCALBN(tmp.f, m.i);
            return ;
        }        
    }

//        SSE2 SETUP FUNCTIONS

ISV exp_8u_setup(int scaleFactor, __m128i &start, __m128i &max_index)
{
    const Fw8u start_index[] = { 0, 0, 1, 2, 2, 3, 4};
    Fw8u strt   = (scaleFactor > 0) ? start_index[scaleFactor-1] : 0 ;
    start        = _mm_set1_epi8(strt); 
    max_index    = _mm_set1_epi8(8) ;                           // max index into array exp_8u_tbl    
}

ISV exp_16s_setup(int scaleFactor, __m128i &start, __m128i &max_index)
{
    Fw16s start_index[21] = { 8, 7, 7, 6, 5, 5, 4, 3, 3, 2, 1, 1, 0, -1, -2, -2, -3, -4, -4, -5, -6} ;
    max_index    = _mm_set1_epi16(13) ;                         // max index into array exp_16s
    start        = _mm_set1_epi16(start_index[scaleFactor+10]); // "10" comes from the max scale factor dealt in the function
}

ISV exp_32s_setup(float scale, int max_arg, __m128 &mm_scale, __m128i &mm_max_arg)
{
    mm_scale    = _mm_set1_ps(scale);
    mm_max_arg  = _mm_set1_epi32(max_arg);                      //max_arg
}

ISV exp_32f_setup(__m128 &thirtytwo_by_ln2, __m128 &ln2_by_32_lead_trail, __m128 &A1_A2, __m128 &max_min_arg)
{
    thirtytwo_by_ln2    = _mm_set1_ps(4.6166240692E+01F); 
    ln2_by_32_lead_trail= _mm_set_ps (2.1659851074E-02F, 2.1659851074E-02F,
                                      9.9831822808E-07F, 9.9831822808E-07F); 
    A1_A2               = _mm_set_ps (1.66666666665260878863e-01F, 1.66666666665260878863e-01F,
                                      5.00000000000000008883e-01F, 5.00000000000000008883e-01F ); 
    max_min_arg         = _mm_set_ps (88.7228390F, 88.7228390F, -87.33654470F, -87.33654470F);        
}

ISV exp_64f_setup(__m128d &thirtytwo_by_ln2_A5, __m128d &ln2_by_32_lead_trail, __m128d &A1_A2, __m128d &A3_A4)
{
    thirtytwo_by_ln2_A5  = _mm_set_pd(46.166241308446828384   , 1.38889490863777199667e-03); 
    ln2_by_32_lead_trail = _mm_set_pd(0.021660849390173098072 , 2.3251928468788740148e-12); 
    A1_A2                = _mm_set_pd(1.66666666665260878863e-01, 5.00000000000000008883e-01);    
    A3_A4                = _mm_set_pd(8.33336798434219616221e-03, 4.16666666662260795726e-02);    
}

//        SSE2 UNIT FUNCTIONS

ISV exp_8u_unit(const XMM128 &src, XMM128 &dst, int scale_plus_7, const __m128i &strt, const __m128i &max_index )
{
#define exp_8u_set(x) EXP_TABLES::exp_8u_tbl[scale_plus_7][x]

    XMM128  tsrc; 

    tsrc.i = _mm_subs_epu8(src.i, strt);
    tsrc.i = _mm_min_epu8(tsrc.i, max_index);            // if src >max_index, make it max_index 

    int i0 = tsrc.u32[0] ;
    int i1 = tsrc.u32[1] ;
    int i2 = tsrc.u32[2] ;
    int i3 = tsrc.u32[3] ;

    dst.i = _mm_set_epi32(  (exp_8u_set((Fw8u)(i3 >> 24)) <<24) |
                            (exp_8u_set((Fw8u)(i3 >> 16)) <<16) | 
                            (exp_8u_set((Fw8u)(i3 >> 8) ) <<8 ) |
                             exp_8u_set((Fw8u)i3),

                            (exp_8u_set((Fw8u)(i2 >> 24)) <<24) |
                            (exp_8u_set((Fw8u)(i2 >> 16)) <<16) | 
                            (exp_8u_set((Fw8u)(i2 >> 8) ) <<8 ) |
                             exp_8u_set((Fw8u)i2),

                            (exp_8u_set((Fw8u)(i1 >> 24)) <<24) |
                            (exp_8u_set((Fw8u)(i1 >> 16)) <<16) | 
                            (exp_8u_set((Fw8u)(i1 >> 8) ) <<8 ) |
                             exp_8u_set((Fw8u)i1),

                            (exp_8u_set((Fw8u)(i0 >> 24)) <<24) |
                            (exp_8u_set((Fw8u)(i0 >> 16)) <<16) | 
                            (exp_8u_set((Fw8u)(i0 >> 8) ) <<8 ) |
                             exp_8u_set((Fw8u)i0));
#undef exp_8u_set
}

ISV exp_16s_unit(const __m128i &src, __m128i &dst,
                 int scaleFactor, const __m128i &strt, const __m128i &max_index )
{    
    #define exp_16s_set(x) EXP_TABLES::exp_16s_tbl[scaleFactor][(x)]

    XMM128  tsrc; 
    
    tsrc.i = _mm_adds_epi16(src, strt);
    tsrc.i = _mm_max_epi16(tsrc.i, _mm_setzero_si128());        //if src<0, make it 0
    tsrc.i = _mm_min_epi16(tsrc.i, max_index);                  // if src >max_index, make it max_index 

    int u0 = tsrc.u32[0];
    int u1 = tsrc.u32[1];
    int u2 = tsrc.u32[2];
    int u3 = tsrc.u32[3];

    dst =_mm_set_epi16(exp_16s_set( u3 >> 16), exp_16s_set( u3 & 0xFF), 
                exp_16s_set( u2 >> 16), exp_16s_set( u2 & 0xFF),
                exp_16s_set( u1 >> 16), exp_16s_set( u1 & 0xFF),
                exp_16s_set( u0 >> 16), exp_16s_set( u0 & 0xFF));

    #undef exp_16s_set        
}

ISV exp_32s(const __m128i &src, XMM128 &dst, const __m128 &scale)                                        
{
    XMM128 tsrc; 

    tsrc.i = _mm_srli_epi32(_mm_slli_epi32(src, 27), 27);    // <= no array access violation, since arr size is 32

    // trying to minimize __mm_extract_epi16 for looking up table
    tsrc.i = _mm_or_si128(tsrc.i, _mm_srli_epi64(tsrc.i, 24));

    int i0 = _mm_extract_epi16(tsrc.i, 0); 
    int i1 = _mm_extract_epi16(tsrc.i, 4);

    dst.f = _mm_set_ps(EXP_TABLES::exp_32s_tbl[(i1>>8)], EXP_TABLES::exp_32s_tbl[(Fw8u)(i1)],
                       EXP_TABLES::exp_32s_tbl[(i0>>8)], EXP_TABLES::exp_32s_tbl[(Fw8u)(i0)]);

    dst.f = _mm_mul_ps(dst.f, scale);
    dst.i = _mm_cvtps_epi32(dst.f);
}

// Exponential 32s special cases
// for arg < min_arg, return 0
// for arguments > MAX_EXP_ARG, return MAX_32S (0x7FFFFFFF)    

ISV exp_32s_special_case(const __m128i &src, __m128i &dst, const __m128i &max_arg)
{

    
    __m128i cmp1 = _mm_cmplt_epi32(src, _mm_setzero_si128());
    __m128i cmp2 = _mm_cmpgt_epi32(src, max_arg);
            cmp1 = _mm_or_si128(cmp1, cmp2);
             dst = _mm_andnot_si128(cmp1, dst); 
             dst = _mm_or_si128(dst, _mm_srli_epi32(cmp2, 1) );
}

ISV exp_32s_unit(const __m128i &src, XMM128 &dst, const __m128 &scale, const __m128i &max_arg)
{
    XMM128 tsrc;
    tsrc.i = src ;
    exp_32s(src, dst, scale);
    exp_32s_special_case(tsrc.i, dst.i, max_arg);
}

ISV exp_64f(const __m128d &src, XMM128 &dst, 
            const __m128d& thirtytwo_by_ln2_A5, 
            const __m128d& ln2_by_32_lead_trail,
            const __m128d& A1_A2, const __m128d& A3_A4)
{    
    __m128d p, r1, r2 ; 
    XMM128 m, n, j, r;

    r.d = _mm_unpackhi_pd(thirtytwo_by_ln2_A5, thirtytwo_by_ln2_A5);
    r.d = _mm_mul_pd(r.d, src);
    n.i = _mm_cvtpd_epi32(r.d);                                                             //  n = nearest integer to r.d
    j.i = _mm_slli_epi32(n.i, 27);
    j.i = _mm_srli_epi32(j.i, 27);                                                          // j = n%32

    m.i = _mm_srai_epi32(n.i, 5);                                                           // m = (n - j)/32
    n.d = _mm_cvtepi32_pd(n.i);

    r1 = _mm_mul_pd(n.d, _mm_unpackhi_pd(ln2_by_32_lead_trail, ln2_by_32_lead_trail));
    r1 = _mm_sub_pd(src, r1);                                                               // r1 = x-n*ln2_by_32_lead

    r2 = _mm_mul_pd(n.d, _mm_unpacklo_pd(ln2_by_32_lead_trail, ln2_by_32_lead_trail));      // r2 =  -n*ln2_by_32_trail
    r.d = _mm_sub_pd(r1, r2) ;                                                              // r = r1 + r2;
    
    /*    p = r1 + (r2 +r*r*( 5.00000000000000008883e-01 +
                      r*( 1.66666666665260878863e-01 +
                      r*( 4.16666666662260795726e-02 +
                      r*( 8.33336798434219616221e-03 +
                      r*( 1.38889490863777199667e-03 ))))));
    */
    
    p = _mm_mul_pd(r.d, _mm_unpacklo_pd(thirtytwo_by_ln2_A5, thirtytwo_by_ln2_A5)/*A5*/);
    p = _mm_add_pd(p, _mm_unpackhi_pd(A3_A4, A3_A4));
    p = _mm_mul_pd(p, r.d);
    p = _mm_add_pd(p, _mm_unpacklo_pd(A3_A4, A3_A4));
    p = _mm_mul_pd(r.d,p);
    p = _mm_add_pd(p, _mm_unpackhi_pd(A1_A2, A1_A2));
    p = _mm_mul_pd(p, r.d);
    p = _mm_add_pd(p, _mm_unpacklo_pd(A1_A2, A1_A2));
    p = _mm_mul_pd(p, r.d);
    p = _mm_mul_pd(p, r.d);
    p = _mm_sub_pd(p, r2);
    p = _mm_add_pd(p, r1);

    //s_lead(j)

    int i0 = _mm_extract_epi16(j.i, 0); int i1 = _mm_extract_epi16(j.i, 2);

    r1 = _mm_set_pd(EXP_TABLES::two_to_jby32_lead_64f[i1], EXP_TABLES::two_to_jby32_lead_64f[i0]);
    //s_trail(j)
    r2 = _mm_set_pd(EXP_TABLES::two_to_jby32_trail_64f[i1], EXP_TABLES::two_to_jby32_trail_64f[i0]);

    // reslt = (2^m)*(s_lead(j) + (s_trail(j) + P *(s_lead(j)+s_trail(j) )))
    // the order of above computation is VERY imp to maintain the precision

    r.d = _mm_add_pd(r1, r2);
    r.d = _mm_mul_pd(r.d, p);
    r2  = _mm_add_pd(r2, r.d);

    r.d = _mm_add_pd(r1, r2);

    // from m, calculate (double)pow(2.0,m)
    m.f   = _mm_shuffle_ps(m.f, m.f, _MM_SHUFFLE(3,1, 2,0));
    m.i   = _mm_slli_epi64(m.i, 52);
    dst.i = _mm_add_epi64(m.i, r.i);
}

ISV exp_64f_special_case(const XMM128 &src, XMM128 &dst)
{
    /*  if |src| > max_arg then 
            dst -> inf_64f if src >0 
            dst -> 0 if src < 0
        else dst */

    XMM128 cmp;
  //static const __m128d max_min_arg = CONST_SET_64F(709.88698417006428438274676547919, -743.73498503707675593781123986305);

    static const __m128d max_min_arg = CONST_SET_64F(709.7827128933839967276924307167, -708.39641853226410621681158499121);

    cmp.d = _mm_cmpge_pd(src.d, _mm_unpacklo_pd(max_min_arg, max_min_arg)); //min
    dst.d = _mm_and_pd(cmp.d, dst.d );      // (1)
    cmp.d = _mm_cmpgt_pd(src.d, _mm_unpackhi_pd(max_min_arg, max_min_arg)); // max
    dst.d = _mm_andnot_pd(cmp.d, dst.d);    //(3)
    
    cmp.i = _mm_slli_epi64(_mm_srli_epi64(cmp.i, 53), 52);    //0x7ff0000000000000 i.e, inf
    dst.i = _mm_or_si128(dst.i, cmp.i);     //(2)
}

// SSE2 CALLING FUNCTIONS

template <CH cd>
ISV Exp_8u_sse(const XMM128 *src, XMM128 *dst, 
               int scaleFactor, const __m128i &start_index, const __m128i &max_index )
{
    int scale = scaleFactor+7;
    for(int i=0 ; i < cd ; i++) 
        exp_8u_unit(src[i], dst[i], scale, start_index, max_index);
}

template <CH cd>
ISV Exp_16s_sse(const XMM128 *src, XMM128 *dst, 
                int scaleFactor, const __m128i &start_index, const __m128i &max_index)
{
    int scale = scaleFactor+10;
    for(int i=0 ; i < cd ; i++) 
        exp_16s_unit(src[i].i, dst[i].i,  scale, start_index, max_index);    
}

template <CH cd>                       
ISV Exp_32s_sse(const XMM128 *src, XMM128 *dst, const __m128 &scale, const __m128i &max_exp_arg)
{
    for(int i=0 ; i < cd ; i++) 
        exp_32s_unit(src[i].i, dst[i], scale, max_exp_arg);     
}

ISV Exp_64s_scale_NZ( const Fw64s *s, Fw64s *d, double scale, Fw64s max_arg )
{
    if(s[0]< -3){ d[0] = 0 ; return ;}
    d[0] = (s[0] <= max_arg) ? (Fw64s)((EXP_TABLES::exp_64s_flt_tbl[s[0]+3])*scale) : 0x7FFFFFFFFFFFFFFFLL ;
}

ISV Exp_64s_scale_Zero(  const Fw64s *s, Fw64s *d)
{
    if(s[0]< 0){ d[0] = 0 ; return ; }
    d[0] = (s[0] < 44) ? EXP_TABLES::exp_64s_tbl[s[0]] : 0x7FFFFFFFFFFFFFFFLL;
}

template <CH cd>
ISV Exp_32f_sse(const XMM128 *src, XMM128 *dst,
                const __m128 &thirtytwo_by_ln2, const __m128 &ln2_by_32_lead_trail,
                const __m128 &A1_A2, const __m128 &max_min_arg)
{    
    for(int i=0 ; i < cd ; i++) 
        CBL_LIBRARY::CBL_Library_Internal::exp_32f_unit(src[i], dst[i], thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, max_min_arg);    
}

ISV Exp_64f_sse(const XMM128 &src, XMM128 &dst, 
                const __m128d& thirtytwo_by_ln2_A5, 
                const __m128d& ln2_by_32_lead_trail,
                const __m128d& A1_A2, const __m128d& A3_A4)
{    
    XMM128 tsrc ;
    tsrc.d = src.d ;

    exp_64f(src.d, dst, thirtytwo_by_ln2_A5, ln2_by_32_lead_trail, A1_A2, A3_A4);
    exp_64f_special_case(tsrc, dst);    
}

ISV Exp_32f64f_sse(const XMM128 &src, XMM128 &dst, 
                   const __m128d& thirtytwo_by_ln2_A5, 
                   const __m128d& ln2_by_32_lead_trail,
                   const __m128d& A1_A2, const __m128d& A3_A4)
{    
    XMM128 tsrc_d ;
    tsrc_d.d = _mm_cvtps_pd(src.f);
    exp_64f(tsrc_d.d, dst, thirtytwo_by_ln2_A5, ln2_by_32_lead_trail, A1_A2, A3_A4);
    exp_64f_special_case(tsrc_d, dst);
}

// STRUCTURES of Exp_8u, _16s, _32s, _32f,_64f for 1 and 2 buffers.


  template<CH cd>
  struct Exp_8u_B1 : public fe1< Fw8u, cd >
  {   
        FE_SSE2_REF
     
        XMM128 start_index, max_index ;
        int scaleFactor ;

         Exp_8u_B1(int sf){ scaleFactor = sf;  }

         IV SSE2_Init()
         {
             exp_8u_setup( scaleFactor, start_index.i, max_index.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Exp_8u_sse<cd>(r.dst, r.dst, scaleFactor, start_index.i, max_index.i);
         }      
         IV REFR( Fw8u *sd) const
         {         
               Exp_8u_ref<cd>( sd, sd, scaleFactor);
         }      
    } ;

    template<CH cd>
    struct Exp_8u_B2 : public fe2< Fw8u, cd, Fw8u, cd >
    {   
        FE_SSE2_REF    

        XMM128 start_index, max_index ;
        int scaleFactor ;
             
        Exp_8u_B2(int sf) { scaleFactor = sf; }

         IV SSE2_Init()
         {
             exp_8u_setup(scaleFactor, start_index.i, max_index.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Exp_8u_sse<cd>(r.src1, r.dst, scaleFactor, start_index.i, max_index.i);
         }      
         IV REFR( const Fw8u *s, Fw8u *d) const
         {         
              Exp_8u_ref<cd >(s, d, scaleFactor);
         }      
    } ;

    template<CH cd>
    struct Exp_16s_B1 : public fe1< Fw16s, cd >
    {   
        FE_SSE2_REF
     
        XMM128 start_index, max_index ;
        int scaleFactor ;

         Exp_16s_B1(int sf){ scaleFactor = sf; }

         IV SSE2_Init()
         {
             exp_16s_setup(scaleFactor, start_index.i, max_index.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Exp_16s_sse<cd>(r.dst, r.dst, scaleFactor, start_index.i, max_index.i);
         }      
         IV REFR( Fw16s *sd) const
         {         
               Exp_16s_ref<cd>( sd, sd, scaleFactor);
         }
    } ;

    template<CH  cd>
    struct Exp_16s_B2 : public fe2< Fw16s, cd, Fw16s, cd >
    {   
        FE_SSE2_REF

        XMM128 start_index, max_index ;
        int scaleFactor ;
             
        Exp_16s_B2(int sf) { scaleFactor = sf; }

         IV SSE2_Init()
         {
             exp_16s_setup(scaleFactor, start_index.i, max_index.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Exp_16s_sse<cd>(r.src1, r.dst, scaleFactor, start_index.i, max_index.i);
         }      
         IV REFR( const Fw16s *s, Fw16s *d) const
         {         
              Exp_16s_ref<cd>(s, d, scaleFactor);
         }
    } ;

    template<CH cd>
    struct Exp_32s_B1 : public fe1< Fw32s, cd >
    {   
        FE_SSE2_REF
     
        XMM128 mm_scale, mm_max_arg ;
        float scale;
        int max_arg ;

        Exp_32s_B1(int scaleFactor)
           {
            scale = powf(2.0F, (float)-scaleFactor);  
            max_arg = (int)(0.69314718055994529F * (31+scaleFactor));   // to confirm e(x)*2**-s < (2**31 -1)
        }

         IV SSE2_Init()
         {
             exp_32s_setup(scale, max_arg, mm_scale.f, mm_max_arg.i);
         }
         IV SSE2( RegFile & r ) const 
         {      
             Exp_32s_sse<cd>(r.dst, r.dst, mm_scale.f, mm_max_arg.i);
         }      
         IV REFR( Fw32s *sd) const
         {
               Exp_32s_ref<cd>( sd, sd, scale, max_arg);
         }
    } ;

    template<CH cd>
    struct Exp_32s_B2 : public fe2< Fw32s, cd, Fw32s, cd >
    {   
        FE_SSE2_REF
     
        XMM128 mm_scale, mm_max_arg ;
        float scale;
        int max_arg ;

        Exp_32s_B2(int scaleFactor)
           {
            scale = powf(2.0F, (float)-scaleFactor);  
            max_arg = (int)(0.69314718055994529F * (31+scaleFactor));   //  to confirm e(x)*2**-s < (2**31 -1)
        }

         IV SSE2_Init()
         {
             exp_32s_setup(scale, max_arg, mm_scale.f, mm_max_arg.i);
         }
         IV SSE2( RegFile & r ) const
         {      
             Exp_32s_sse<cd>(r.src1, r.dst, mm_scale.f, mm_max_arg.i);
         }      
         IV REFR(const Fw32s *s, Fw32s *d) const
         {         
             Exp_32s_ref<cd>( s, d, scale, max_arg);
         }      
    } ;

    struct Exp_64s_scale0_B1 : public fe1< Fw64s, C1 >
    {   
        FE_REF
     
        double scale;
        Fw64s max_arg ;
         
         IV REFR( Fw64s *sd) const
         {         
               Exp_64s_scale_Zero( sd, sd);
         }  
    } ;

    struct Exp_64s_scale0_B2 : public fe2< Fw64s, C1, Fw64s, C1 >
    {   
        FE_REF
     
        double scale;
        Fw64s max_arg ;
          
         
         IV REFR(const Fw64s *s, Fw64s *d) const
         {         
               Exp_64s_scale_Zero( s, d);
         }  
    } ;

    struct Exp_64s_scale_NZ_B1 : public fe1< Fw64s, C1 >
    {   
        FE_REF
     
        double scale;
        Fw64s max_arg ;

        Exp_64s_scale_NZ_B1(int scaleFactor)
           {
             scale = pow(2.0, -scaleFactor);
             max_arg = (Fw64s) (0.69314718055994529 * (63+scaleFactor));
        }      
         
         IV REFR( Fw64s *sd) const
         {         
               Exp_64s_scale_NZ( sd, sd, scale, max_arg);
         }  
    } ;

    struct Exp_64s_scale_NZ_B2 : public fe2< Fw64s, C1, Fw64s, C1 >
    {   
        FE_REF
     
        double scale;
        Fw64s max_arg ;

        Exp_64s_scale_NZ_B2(int scaleFactor)
           {
             scale = pow(2.0, -scaleFactor);
             max_arg = (Fw64s) (0.69314718055994529 * (63+scaleFactor));
        }
        
         IV REFR( const Fw64s *s, Fw64s *d) const
         {         
               Exp_64s_scale_NZ( s, d, scale, max_arg);
         }  
    } ;

    template<CH cd>
    struct Exp_32f_B1 : public fe1< Fw32f, cd >
    {   
        FE_SSE2_REF
     
        XMM128 thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, max_min_arg;

        IV SSE2_Init()
        {
            exp_32f_setup(thirtytwo_by_ln2.f, ln2_by_32_lead_trail.f, A1_A2.f, max_min_arg.f);
        }
        IV SSE2( RegFile & r ) const
        {
            Exp_32f_sse<cd>(r.dst, r.dst, thirtytwo_by_ln2.f, ln2_by_32_lead_trail.f, A1_A2.f, max_min_arg.f);
        }
        IV REFR( Fw32f *sd) const
        {
            Exp_32f_ref<cd>( sd, sd);
        }
    } ;


    template<CH cd>
    struct Exp_32f_B2 : public fe2< Fw32f, cd, Fw32f, cd >
    {   
        FE_SSE2_REF
     
        XMM128 thirtytwo_by_ln2, ln2_by_32_lead_trail, A1_A2, max_min_arg;

        IV SSE2_Init()
        {
            exp_32f_setup(thirtytwo_by_ln2.f, ln2_by_32_lead_trail.f, A1_A2.f, max_min_arg.f);
        }
        IV SSE2( RegFile & r ) const
        {      
            Exp_32f_sse<cd>(r.src1, r.dst, thirtytwo_by_ln2.f, ln2_by_32_lead_trail.f, A1_A2.f, max_min_arg.f);
        }
        IV REFR( const Fw32f *s, Fw32f *d) const
        {
            Exp_32f_ref<cd>( s, d);
        }
    } ;

    template<CH cd>
    struct Exp_64f_B1 : public fe1< Fw64f, cd >
    {   
        FE_SSE2_REF

        XMM128 thirtytwo_by_ln2_A5, ln2_by_32_lead_trail, A1_A2, A3_A4;

        IV SSE2_Init()
        {
            exp_64f_setup(thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }
        IV SSE2( RegFile & r ) const
        {      
            Exp_64f_sse(r.dst[0], r.dst[0], thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }       
        IV REFR( Fw64f *sd) const
        {         
            Exp_64f_ref<Fw64f, Fw64f, C1>( sd, sd);
        }      
    } ;


    template<CH cd>
    struct Exp_64f_B2 : public fe2< Fw64f, cd, Fw64f, cd >
    {   
        FE_SSE2_REF
     
        XMM128 thirtytwo_by_ln2_A5, ln2_by_32_lead_trail, A1_A2, A3_A4;

        IV SSE2_Init()
        {
            exp_64f_setup(thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }
        IV SSE2( RegFile & r ) const
        {      
            Exp_64f_sse(r.src1[0], r.dst[0], thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }      
        IV REFR( const Fw64f *s, Fw64f *d) const
        {         
            Exp_64f_ref<Fw64f, Fw64f, C1>( s, d);
        }
    } ;

    template<CH cd>
    struct Exp_32f64f_B2 : public fe2< Fw32f, cd, Fw64f, cd >
    {   
        FE_SSE2_REF
     
        XMM128 thirtytwo_by_ln2_A5, ln2_by_32_lead_trail, A1_A2, A3_A4;

        IV SSE2_Init()
        {
            exp_64f_setup(thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }
        IV SSE2( RegFile & r ) const
        {      
            Exp_32f64f_sse(r.src1[0], r.dst[0], thirtytwo_by_ln2_A5.d, ln2_by_32_lead_trail.d, A1_A2.d, A3_A4.d);
        }      
        IV REFR( const Fw32f *s, Fw64f *d) const
        {         
            Exp_64f_ref<Fw32f, Fw64f, C1>( s, d);
        }
    } ;

    struct Exp_A11 : public fe2< Fw32f, C1, Fw32f, C1 >
    {   
        FE_SSE2_REF
        XMM128 thirtytwo_by_ln2, ln2_by_32, A1_A2, max_min_arg ;

        IV SSE2_Init()
        {
            CBL_LIBRARY::CBL_Library_Internal::exp_A11_setup(thirtytwo_by_ln2.f, ln2_by_32.f, A1_A2.f, max_min_arg.f);
        }
        IV SSE2( RegFile & r ) const
        {
            CBL_LIBRARY::CBL_Library_Internal::Exp_A11_sse(r.src1[0], r.dst[0], thirtytwo_by_ln2.f, ln2_by_32.f, A1_A2.f, max_min_arg.f);
        }      
        IV REFR( const Fw32f *s, Fw32f *d) const
        {
           Exp_A11_ref( s, d);
        }
    } ;


} // namespace OPT_LEVEL

#endif  //_EXP_H_





