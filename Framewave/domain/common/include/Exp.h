/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/

#ifndef _EXP_H_
#define _EXP_H_

#include "fe.h"

// Contains the exponential functions for all the data types.

// TABLES for exp 8u, 16s, 32s, 64s, 32f, 64f

namespace OPT_LEVEL
{

namespace EXP_TABLES {

    SYS_FORCEALIGN_16 static const Fw8u exp_8u_tbl[15][9] = 
    {
        {128, 255, 255, 255, 255, 255, 255, 255, 255},        // Factor -7
        { 64, 174, 255, 255, 255, 255, 255, 255, 255},        // Factor -6
        { 32,  87, 236, 255, 255, 255, 255, 255, 255},        // Factor -5
        { 16,  43, 118, 255, 255, 255, 255, 255, 255},        // Factor -4
        {  8,  22,  59, 161, 255, 255, 255, 255, 255},        // Factor -3
        {  4,  11,  30,  80, 218, 255, 255, 255, 255},        // Factor -2
        {  2,   5,  15,  40, 109, 255, 255, 255, 255},        // Factor -1
        {  1,   3,   7,  20,  55, 148, 255, 255, 255},        // Factor 0
        {  1,   1,   4,  10,  27,  74, 202, 255, 255},        // Factor 1
        {  0,   1,   2,   5,  14,  37, 101, 255, 255},        // Factor 2
        {  0,   1,   3,   7,  19,  50, 137, 255, 255},        // Factor 3
        {  0,   1,   3,   9,  25,  69, 186, 255, 255},        // Factor 4
        {  0,   1,   2,   5,  13,  34,  93, 253, 255},        // Factor 5
        {  0,   1,   2,   6,  17,  47, 127, 255, 255},        // Factor 6
        {  0,   1,   3,   9,  23,  63, 172, 255, 255}         // Factor 7
    };

    SYS_FORCEALIGN_16 static const Fw16s exp_16s_tbl[21][14] = 
    { 
        { 0, 1, 3, 7, 19, 51, 139, 377, 1024, 2784, 7566,  20568, 32767,  32767 },  //scale = -10
        { 0, 1, 3, 9, 25, 69, 188, 512, 1392, 3783, 10284, 27954, 32767,  32767 },  //scale = -9
        { 0, 1, 2, 5, 13, 35,  94, 256,  696, 1892, 5142,  13977, 32767,  32767 },  //scale = -8
        { 0, 1, 2, 6, 17, 47, 128, 348,  946, 2571, 6989,  18997, 32767,  32767 },  //scale = -7
        { 0, 1, 3, 9, 24, 64, 174, 473, 1285, 3494, 9498,  25819, 32767,  32767 },  //scale = -6
        { 0, 1, 2, 4, 12, 32,  87, 236,  643, 1747, 4749,  12910, 32767,  32767 },  //scale = -5
        { 0, 1, 2, 6, 16, 43, 118, 321,  874, 2375, 6455,  17546, 32767,  32767 },  //scale = -4
        { 0, 1, 3, 8, 22, 59, 161, 437, 1187, 3227, 8773,  23848, 32767,  32767 },  //scale = -3
        { 0, 1, 1, 4, 11, 30,  80, 218,  594, 1614, 4387,  11924, 32412,  32767 },  //scale = -2
        { 0, 1, 2, 5, 15, 40, 109, 297,  807, 2193, 5962,  16206, 32767,  32767 },  //scale = -1
        { 0, 1, 3, 7, 20, 55, 148, 403, 1097, 2981, 8103,  22026, 32767,  32767 },  //scale = 0
        { 0, 1, 1, 4, 10, 27,  74, 202,  548, 1490, 4052,  11013, 29937,  32767 },  //scale = 1
        { 0, 1, 2, 5, 14, 37, 101, 274,  745, 2026, 5507,  14969, 32767,  32767 },  //scale = 2
        { 0, 1, 3, 7, 19, 50, 137, 373, 1013, 2753, 7484,  20344, 32767,  32767 },  //scale = 3
        { 0, 1, 3, 9, 25, 69, 186, 506, 1377, 3742, 10172, 27651, 32767,  32767 },  //scale = 4
        { 0, 1, 2, 5, 13, 34,  93, 253,  688, 1871, 5086,  13825, 32767,  32767 },  //scale = 5
        { 0, 1, 2, 6, 17, 47, 127, 344,  936, 2543, 6913,  18791, 32767,  32767 },  //scale = 6
        { 0, 1, 3, 9, 23, 63, 172, 468, 1272, 3456, 9395,  25539, 32767,  32767 },  //scale = 7
        { 0, 1, 2, 4, 12, 32,  86, 234,  636, 1728, 4698,  12770, 32767,  32767 },  //scale = 8
        { 0, 1, 2, 6, 16, 43, 117, 318,  864, 2349, 6385,  17356, 32767,  32767 },  //scale = 9
        { 0, 1, 3, 8, 22, 58, 159, 432, 1174, 3192, 8678,  23589, 32767,  32767 },  //scale = 10
    };
    
    
    SYS_FORCEALIGN_16 static const float exp_32s_tbl[32] = 
    {
        1.0000001F    ,    // 0
        2.71828182846F,    7.38905609893F,    20.0855369232F,    54.5981500331F,    // 1 to 4
        148.413159103F,    403.428793493F,    1096.63315843F,    2980.95798704F,    // 5 to 8
        8103.08392758F,    22026.4657948F,    59874.1417152F,    162754.791419F,    // 9 - 12
        442413.392009F,    1202604.28416F,    3269017.37247F,    8886110.52051F,    // 13 - 16
        24154952.7536F,    65659969.1373F,    178482300.963F,     485165195.41F,    // 17 - 20
        1318815734.48F,    3584912846.13F,    9744803446.25F,    26489122129.8F,    // 21 - 24
    };
    
    SYS_FORCEALIGN_16 static const Fw64s exp_64s_tbl[64] =
    {
        1                   , 2                   , 7                    , 20                   ,    //0-3
        54                  , 148                 , 403                  , 1096                 ,    //4-7
        2980LL              , 8103LL              , 22026LL              , 59874LL              ,    //8-11
        162755LL            , 442413LL            , 1202604LL            , 3269017LL            ,    //12-15
        8886111LL           , 24154953LL          , 65659969LL           , 178482301LL          ,    //16-19
        485165195LL         , 1318815734LL        , 3584912846LL         , 9744803446LL         ,    //20-23
        26489122130LL       , 72004899337LL       , 195729609429LL       , 532048240602LL       ,    //24-27
        1446257064291LL     , 3931334297144LL     , 10686474581524LL     , 29048849665247LL     ,    //28-31
        78962960182681LL    , 214643579785916LL   , 583461742527455LL    , 1586013452313431LL   ,    //32-35
        4311231547115195LL  , 11719142372802611LL/*2*/, 31855931757113756LL, 86593400423993747LL,    //36-39
        235385266837019985LL, 639843493530054949LL, 1739274941520501047LL, 4727839468229346561LL,    //40-43
        0x7fffffffffffffffLL, 0x7fffffffffffffffLL, 0x7fffffffffffffffLL , 0x7fffffffffffffffLL ,    //44-47
        0x7fffffffffffffffLL, 0x7fffffffffffffffLL, 0x7fffffffffffffffLL , 0x7fffffffffffffffLL ,    //48-
        0x7fffffffffffffffLL, 0x7fffffffffffffffLL, 0x7fffffffffffffffLL , 0x7fffffffffffffffLL ,    //52-
        0x7fffffffffffffffLL, 0x7fffffffffffffffLL, 0x7fffffffffffffffLL , 0x7fffffffffffffffLL ,    //56-
        0x7fffffffffffffffLL, 0x7fffffffffffffffLL, 0x7fffffffffffffffLL , 0x7fffffffffffffffLL ,    //60-63
    };

    SYS_FORCEALIGN_16 static const double exp_64s_flt_tbl[64] = 
    {
        0.049787068367, 0.135335283237, 0.367879441171, 1.0,                //-3 to 0
        2.71828182846 , 7.38905609893 , 20.0855369232 , 54.5981500331,      // 1 to 4
        148.413159103 , 403.428793493 , 1096.63315843 , 2980.95798704,      // 5 to 8
        8103.08392758 , 22026.4657948 , 59874.1417152 , 162754.791419,      // 9 - 12
                                 442413.392009,               1202604.284164776777749,
                       3269017.372472110639301,               8886110.520507872636763,
                      24154952.753575298214775,              65659969.137330511138786,
                     178482300.963187260844910,             485165195.409790277969106,
                    1318815734.483214697209998,            3584912846.131591561681159,
                    9744803446.248902600034632,           26489122129.843472294139162,
                   72004899337.385872524161351,          195729609428.838764269776397,
                  532048240601.798616683747304,         1446257064291.475173677047422,
                 3931334297144.042074388620580,        10686474581524.462146990468650,
                29048849665247.425231085682111,        78962960182680.695160978022635,
               214643579785916.064624297761531,       583461742527454.881402902734610,
              1586013452313430.728129644625774,      4311231547115195.227113422292856,
             11719142372802611.308772939791190,     31855931757113756.220328671701298,
             86593400423993746.953606932719264,    235385266837019985.407899910749034,
            639843493530054949.222663403515570,   1739274941520501047.394681303611235,
           4727839468229346561.474457562744280,  12851600114359308275.809299632143099,
          34934271057485095348.034797233406099,  94961194206024488745.133649117118323,
         258131288619006739623.285800215273380, 701673591209763173865.471599886117405,
        1907346572495099690525.099840953848447,                                    -1,    //e(49), 
        -1, -1 , -1, -1, -1, -1, -1, -1, -1, -1
    };


  SYS_FORCEALIGN_16 static const double two_to_jby32_lead_64f[32] = {
        1, // 3ff00000 00000000
        1.0218971486541050808,    // 0x3FF059B0D3158540 ,
        1.0442737824274104241,    // 0x3FF0B5586CF98900 ,
        1.0671404006768199224,    // 0x3FF11301D0125B40 ,
        1.090507732665244589 ,    // 0x3FF172B83C7D5140 ,
        1.1143867425958831063,    // 0x3FF1D4873168B980 ,
        1.1387886347566791301,    // 0x3FF2387A6E756200 ,
        1.163724858777570148 ,    // 0x3FF29E9DF51FDEC0 ,
        1.189207115002716364 ,    // 0x3FF306FE0A31B700 ,
        1.2152473599804665128,    // 0x3FF371A7373AA9C0 ,
        1.2418578120734764525,    // 0x3FF3DEA64C123400 ,
        1.2690509571917232279,    // 0x3FF44E0860618900 ,
        1.2968395546510009808,    // 0x3FF4BFDAD5362A00 ,
        1.3252366431597408791,    // 0x3FF5342B569D4F80 ,
        1.3542555469368835475,    // 0x3FF5AB07DD485400 ,
        1.3839098819638309124,    // 0x3FF6247EB03A5580 ,
        1.4142135623730922589,    // 0x3FF6A09E667F3BC0 ,
        1.445180806977035104 ,    // 0x3FF71F75E8EC5F40 ,
        1.4768261459394977919,    // 0x3FF7A11473EB0180 ,
        1.5091644275934186226,    // 0x3FF82589994CCE00 ,
        1.5422108254079347489,    // 0x3FF8ACE5422AA0C0 ,
        1.5759808451078782809,    // 0x3FF93737B0CDC5C0 ,
        1.6104903319492507308,    // 0x3FF9C49182A3F080 ,
        1.6457554781539585065,    // 0x3FFA5503B23E2540 ,
        1.6817928305074190121,    // 0x3FFAE89F995AD380 ,
        1.7186192981224763798,    // 0x3FFB7F76F2FB5E40 ,
        1.7562521603732932363,    // 0x3FFC199BDD855280 ,
        1.7947090750030980644,    // 0x3FFCB720DCEF9040 ,
        1.8340080864093408763,    // 0x3FFD5818DCFBA480 ,
        1.8741676341102930792,    // 0x3FFDFC97337B9B40 ,
        1.9152065613971416269,    // 0x3FFEA4AFA2A490C0 ,
        1.9571441241754001794,    // 0x3FFF50765B6E4540 ,
    };
    SYS_FORCEALIGN_16 static const double two_to_jby32_trail_64f[32] = {
        0, // 00000000 00000000
        1.1597411706391361837e-14,    // 0x3D0A1D73E2A475B4
        3.4161879709308491346e-15,    // 0x3CEEC5317256E308
        3.6957597440571163423e-15,    // 0x3CF0A4EBBF1AED93
        1.3070163869778723193e-14,    // 0x3D0D6E6FBE462876
        9.4299761914197699096e-15,    // 0x3D053C02DC0144C8
        1.2523626002562007491e-14,    // 0x3D0C3360FD6D8E0B
        7.3657640108952735779e-15,    // 0x3D009612E8AFAD12
        4.7027568557403141035e-15,    // 0x3CF52DE8D5A46306
        2.3653643472485295323e-15,    // 0x3CE54E28AA05E8A9
        7.5960968433694342619e-15,    // 0x3D011ADA0911F09F
        9.9946751537577509698e-15,    // 0x3D068189B7A04EF8
        8.6851220948711086338e-15,    // 0x3D038EA1CBD7F621
        4.1550189774967398395e-16,    // 0x3CBDF0A83C49D86A
        9.1808382857243129714e-15,    // 0x3D04AC64980A8C8F
        1.0425179080372087638e-15,    // 0x3CD2C7C3E81BF4B7
        2.7899069308908777473e-15,    // 0x3CE921165F626CDD
        1.1516081874751687512e-14,    // 0x3D09EE91B8797785
        1.5194722889062912911e-15,    // 0x3CDB5F54408FDB37
        4.1172019608001656455e-15,    // 0x3CF28ACF88AFAB35
        6.074702681072821835e-15,    // 0x3CFB5BA7C55A192D
        8.205513465754879596e-15,    // 0x3D027A280E1F92A0
        3.5774208713702990206e-15,    // 0x3CF01C7C46B071F3
        6.3380367436891598263e-15,    // 0x3CFC8B424491CAF8
        1.0073997321832223817e-14,    // 0x3D06AF439A68BB99
        1.5357984302925880313e-15,    // 0x3CDBAA9EC206AD4F
        6.2468503448553655751e-15,    // 0x3CFC2220CB12A092
        9.1220562603541958323e-15,    // 0x3D048A81E5E8F4A5
        1.5871433067176753855e-15,    // 0x3CDC976816BAD9B8
        6.8221551185459294701e-15,    // 0x3CFEB968CAC39ED3
        5.666960267488854618e-15,    // 0x3CF9858F73A18F5E
        8.9607677910366677676e-17,    // 0x3C99D3E12DD8A18B
    };
}


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





