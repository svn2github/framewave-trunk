/*
Copyright (c) 2006-2008 Advanced Micro Devices, Inc. All Rights Reserved.
This software is subject to the Apache v2.0 License.
*/


#ifndef __SSE2_INTRINSICH__
#define __SSE2_INTRINSICH__

#include "fwBase.h"
#include "system.h"
#include "shortcuts.h"
#include "fwdev.h"
#include "FwSharedCode_SSE2.h"
#include "shuffle.h"

namespace OPT_LEVEL
{

// ============================= _mm functions =============================
SYS_INLINE STATIC void _mm_mul_16s_32s(__m128i const &s1,__m128i const &s2,__m128i &d1, __m128i &d2)
{
    __m128i temp1,temp2;
    temp1 = _mm_mullo_epi16(s1,s2);
    temp2 = _mm_mulhi_epi16(s1,s2);
    d1    = _mm_unpacklo_epi16(temp1,temp2); 
    d2    = _mm_unpackhi_epi16(temp1,temp2); 
}

SYS_INLINE STATIC __m128d _mm_mul_pd64fc(const __m128d &s1,const __m128d &s2)
{
    __m128d temp1, temp2, src1, src2;

    src1    = _mm_unpacklo_pd(s1, s1);
    src2    = _mm_unpackhi_pd(s1, s1);

    temp1   = _mm_mul_pd(src1,s2);
    temp2   = _mm_shufb0a1_pd(s2, s2);
    temp2   = _mm_mul_pd(src2,temp2);

    src1    = _mm_sub_sd(temp1, temp2);
    src2    = _mm_add_pd(temp1, temp2);

    return CBL_OPT::Shuffle_b1a0_64f(src1,src2);
}

SYS_INLINE STATIC __m128d _fw_mul_64fc_wm(const __m128d &s1,const __m128d &s2, const __m128d anegmask)
{
    __m128d temp1, temp2, b, a;
    temp1    = _mm_mul_pd(s1, s2);              // b1b2 | a1a2

    temp2    = _mm_shufb0a1_pd(s2, s2);         // b2 | a2 --> a2 | b2
    temp2    = _mm_mul_pd(s1, temp2);           // b1a2 | a1b2

                                                // b | a
                                                //
    b        = _mm_unpacklo_pd(temp1, temp2);   // b = a1b2  |  a1a2
    a        = _mm_unpackhi_pd(temp1, temp2);   // a = b1a2  |  b1b2

    a        = _mm_xor_pd(a, anegmask);         // a = a1b2  | -a1a2

    return      _mm_add_pd(a, b);
}


SYS_INLINE STATIC void _mm_cvt32f_64f(const XMM128 &s,XMM128 &d1,XMM128 &d2)
{
    d1.d = _mm_cvtps_pd(s.f);
    d2.i = _mm_srli_si128(s.i,8);
    d2.d = _mm_cvtps_pd(d2.f);
}

SYS_INLINE STATIC __m128 _mm_mul_ps32fc(const __m128 &s1,const __m128 &s2)
{
        __m128 src1,src2,temp1, temp2;

        src1  = _mm_shuffle_ps(s1,s1,_MM_SHUFFLE(2, 2, 0, 0));
        temp1 = _mm_mul_ps(src1,s2);
        src2  = _mm_shuffle_ps(s2,s2,_MM_SHUFFLE(2, 3, 0, 1));
        src1  = _mm_shuffle_ps(s1,s1,_MM_SHUFFLE(3, 3, 1, 1));
        temp2 = _mm_mul_ps(src1,src2);

        src1 = _mm_sub_ps(temp1,temp2);
        src2 = _mm_add_ps(temp1,temp2);

        src1 = _mm_shuffle_ps(src1,src2,_MM_SHUFFLE(3, 1, 2, 0));
        return _mm_shuffle_ps(src1,src1,_MM_SHUFFLE(3, 1, 2, 0));
}

SYS_INLINE STATIC __m128i mm_max_epi8(const __m128i &val1, const __m128i &val2)
{
    __m128i mask  = _mm_cmpgt_epi8(val1, val2);
    __m128i res1  = _mm_andnot_si128(mask,val2);
    __m128i res2  = _mm_and_si128(mask,val1);
    return  _mm_or_si128(res1, res2);
}

SYS_INLINE STATIC __m128i mm_min_epi8(const __m128i &val1, const __m128i &val2)
{
    __m128i mask  = _mm_cmplt_epi8(val1, val2);
    __m128i res1  = _mm_andnot_si128(mask,val2);
    __m128i res2  = _mm_and_si128(mask,val1);
    return  _mm_or_si128(res1, res2);
}


SYS_INLINE STATIC __m128i _mm_cmpgt_epu32(const __m128i &val1, const __m128i &val2)
{
    __m128i mask  = _mm_cmpgt_epi32(val1, val2);
    __m128i t1    = _mm_srai_epi32(val1, 31);
    __m128i t2    = _mm_srai_epi32(val2, 31);
    
    mask          = _mm_andnot_si128(_mm_xor_si128(t1, t2), mask);

    __m128i res   = _mm_andnot_si128(t2, t1);
    return          _mm_or_si128(res, mask);

}
SYS_INLINE STATIC __m128i _mm_cmpgt_epu16(const __m128i &val1, const __m128i &val2)
{

    __m128i mask  = _mm_cmpgt_epi16(val1, val2);
    __m128i t1    = _mm_srai_epi16(val1, 15);
    __m128i t2    = _mm_srai_epi16(val2, 15);
    

    mask          = _mm_andnot_si128(_mm_xor_si128(t1, t2), mask);

    __m128i res   = _mm_andnot_si128(t2, t1);

    return          _mm_or_si128(res, mask);

}


SYS_INLINE STATIC __m128i mm_max_epu32(const __m128i &val1, const __m128i &val2) 
{
    __m128i mask    = _mm_cmpgt_epu32(val1, val2);
    __m128i ret     = _mm_and_si128(mask, val1);
    mask            = _mm_andnot_si128(mask, val2);

    return            _mm_or_si128(ret, mask);
}

SYS_INLINE STATIC __m128i mm_min_epu32(const __m128i &val1, const __m128i &val2) 
{
    __m128i mask   = _mm_cmpgt_epu32(val1, val2);
    __m128i ret1   = _mm_and_si128(mask, val2);
     mask          = _mm_andnot_si128(mask, val1);

    return              _mm_or_si128(ret1,mask);
}

SYS_INLINE STATIC __m128i _mm_min_epi32(const __m128i &val1, const __m128i &val2) 
{
    __m128i mask    = _mm_cmplt_epi32(val1, val2);
    __m128i ret1    = _mm_and_si128(mask, val1);
             mask   = _mm_andnot_si128(mask, val2);

    return            _mm_or_si128(ret1,mask);
}

SYS_INLINE STATIC __m128i mm_max_epi32(const __m128i &val1, const __m128i &val2) 
{
    __m128i mask    = _mm_cmpgt_epi32(val1, val2);
    __m128i ret1    = _mm_and_si128(mask, val1);
           mask     = _mm_andnot_si128(mask, val2);

    return            _mm_or_si128(ret1,mask);
}

SYS_INLINE STATIC __m128i _mm_max_epu16(const __m128i &val1, const __m128i &val2) 
{
    __m128i mask    = _mm_cmpgt_epu16(val1, val2);

    __m128i ret     = _mm_and_si128(mask, val1);
            mask    = _mm_andnot_si128(mask, val2);

    return            _mm_or_si128(ret, mask);
}

SYS_INLINE STATIC __m128i _mm_min_epu16(const __m128i &val1, const __m128i &val2) 
{

    __m128i mask    = _mm_cmpgt_epu16(val1, val2);

    __m128i ret1    = _mm_and_si128(mask, val2);
            mask    = _mm_andnot_si128(mask, val1);

    return            _mm_or_si128(ret1,mask);
}


SYS_INLINE STATIC __m128i _mm_abs_epi16(const __m128i &src) // returns the absolute value of the 16bit signed value.
{
     /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
    __m128i complement, temp,dst  ;
    temp = _mm_srai_epi16(src, 15);
    complement = _mm_xor_si128(src, temp);  //one's complement of src
    dst  = _mm_sub_epi16 (complement, temp);
    temp = _mm_srli_epi16(dst, 15);
    dst  = _mm_sub_epi16(dst, temp);
    return dst;
}

SYS_INLINE STATIC __m128i mm_abs_epi32(const __m128i &src) // returns the absolute value of the 16bit signed value.
{
     /* do a one's complement and ( add 1 i.e., subtract -1 wich is same as 1111...1111) */
    __m128i complement, temp,dst  ;
    temp = _mm_srai_epi32(src, 31);
    complement = _mm_xor_si128(src, temp);  //one's complement of src
    dst  = _mm_sub_epi32 (complement, temp);
    temp = _mm_srli_epi32(dst, 31);
    dst  = _mm_sub_epi32(dst, temp);
    return dst;
}


SYS_INLINE STATIC __m128 _mm_absqr_ps(const __m128 &src)
{
    XMM128 temp, ret; ret.f = src;
    ret.f     = _mm_mul_ps(ret.f, ret.f);
//  temp.i    = _mm_shuffle_epi32(ret.i, _MM_SHUFFLE(2, 3, 0, 1));        // swap sqr(b)'s and sqr(a)'s positions
    temp.i    = _mm_shuf2301_epi32(ret.i);
    return      _mm_add_ps(ret.f, temp.f);                                // sqr(a) + sqr(b);
}

SYS_INLINE STATIC __m128d _mm_absqr_pd(const __m128d &src)
{
    XMM128 temp, ret; ret.d = src;
    ret.d      = _mm_mul_pd(ret.d, ret.d);
    temp.d     = _mm_shuffle_pd(ret.d, ret.d, _MM_SHUFFLE2(0, 1));     // swap ab positions
    return       _mm_add_pd(ret.d, temp.d);                            // return (sqr(a) + sqr(b)) in both slots;
}

SYS_INLINE STATIC __m128i _mm_absqr_epi16(const __m128i &src)
{
    XMM128 absqr;
    absqr.i = _mm_madd_epi16(src, src);
    absqr.i = _mm_packs_epi16(absqr.i, absqr.i);
//  absqr.i = _mm_shuffle_epi32(absqr.i, _MM_SHUFFLE(3, 1, 2, 0));
//  TODO: Replace the next three shuffles with one unpack
    absqr.i = _mm_shuf3120_epi32(absqr.i);
    absqr.i = _mm_shufflehi_epi16(absqr.i, _MM_SHUFFLE(3, 1, 2, 0));
    return    _mm_shufflelo_epi16(absqr.i, _MM_SHUFFLE(3, 1, 2, 0));
}


SYS_INLINE STATIC __m128 _mm_mag_ps(const __m128 &src)
{
    XMM128 ret, temp; ret.f = src;
    ret.f     = _mm_mul_ps(ret.f, ret.f);
    temp.i    = _mm_shuffle_epi32(ret.i, _MM_SHUFFLE(2, 3, 0, 1));        // swap sqr(b)'s and sqr(a)'s positions
//  temp.i    = _mm_shuffle_ps(ret.i, ret.i, _MM_SHUFFLE(2, 3, 0, 1));
    ret.f     = _mm_add_ps(ret.f, temp.f);                                // sqr(a) + sqr(b);
    return      _mm_mul_ps(ret.f, _mm_rsqrt_ps(ret.f));
}

SYS_INLINE STATIC __m128 _mm_rmag_ps(const __m128 &src)
{
    XMM128 ret, temp; ret.f = src;
    ret.f     = _mm_mul_ps(ret.f, ret.f);
//  temp.i    = _mm_shuffle_epi32(ret.i, _MM_SHUFFLE(2, 3, 0, 1));        // swap sqr(b)'s and sqr(a)'s positions
    temp.f    = _mm_shuffle_ps(ret.f, ret.f, _MM_SHUFFLE(2, 3, 0, 1));
    ret.f     = _mm_add_ps(ret.f, temp.f);                                // sqr(a) + sqr(b);
    return      _mm_rsqrt_ps(ret.f);
}

#pragma warning(disable:4700)
SYS_INLINE STATIC __m128d _mm_mag_pd(const __m128d &src)
{
    XMM128 ret, temp; ret.d = src;;
    ret.d     = _mm_mul_pd(ret.d, ret.d);
    temp.f    = _mm_setzero_ps();
    temp.f    = CBL_OPT::Shuffle_a3a2b3b2_32f(temp.f, ret.f);
    ret.d     = _mm_add_sd(ret.d, temp.d);                            // sqr(a) + sqr(b);
    ret.d     = _mm_sqrt_sd(ret.d, ret.d);

    return          _mm_unpacklo_pd(ret.d, ret.d);
}
#pragma warning(default:4700)

// ============================= _fw functions =============================

SYS_INLINE STATIC __m128i _fw_magex_16sc32s(const __m128i &src)
{
    XMM128 absqr32;
    absqr32.i    = _mm_madd_epi16(src, src);        // a * a + b * b
    absqr32.f    = _mm_cvtepi32_ps(absqr32.i);
    absqr32.f    = _mm_sqrt_ps(absqr32.f);          // root(a * a + b * b) = magnitude
    return         _mm_cvtps_epi32(absqr32.f);
}

SYS_INLINE STATIC __m128i _fw_mag_16sc32s(const __m128i &src)
{
    XMM128 absqr32;
    absqr32.i    = _mm_madd_epi16(src, src);                              // a * a + b * b
    absqr32.f    = _mm_cvtepi32_ps(absqr32.i);
    absqr32.f    = _mm_mul_ps(absqr32.f, _mm_rsqrt_ps(absqr32.f));        // root(a * a + b * b) = magnitude
    return         _mm_cvtps_epi32(absqr32.f);
}

SYS_INLINE STATIC __m128 _fw_mag_16sc32f(const __m128i &src)
{
    XMM128 absqr32;
    absqr32.i    = _mm_madd_epi16(src, src);                              // a * a + b * b
    absqr32.f    = _mm_cvtepi32_ps(absqr32.i);
    return         _mm_mul_ps(absqr32.f, _mm_rsqrt_ps(absqr32.f));        // root(a * a + b * b) = magnitude
}


// these variants don't check for the magnitude to be 0, since if magnitude
// is 0, it WILL NOT be less than level (since only gtzero levels are passed into
// the LT functions) hence these values will be cleared out anyway
SYS_INLINE __m128 _fw_threshval_32fc(const XMM128& complex, const XMM128 &level, const XMM128& rmag)
{
    static const __m128 inf         = CONST_SET1_32F(Const::INF_32F);
    static const __m128i maskb32    = CONST_SET_32I(0x00000000, (int) 0xFFFFFFFF, 0x00000000, (int) 0xFFFFFFFF);

    XMM128 ret, mask;
    ret.f    = _mm_mul_ps(complex.f, level.f);
    ret.f    = _mm_mul_ps(ret.f, rmag.f);        // = / mag

    mask.f   = _mm_cmpneq_ps(rmag.f, inf);       // if mag != 0
    ret.f    = _mm_and_ps(mask.f, ret.f);        // dst = src
    mask.f   = _mm_andnot_ps(mask.f, level.f);   // else { mask = level
    ret.f    = _mm_or_ps(ret.f, mask.f);         //        dst = mask }

    ret.i    = _mm_and_si128(ret.i, maskb32);
    return        ret.f;
}

SYS_INLINE __m128d _fw_threshval_64fc(const XMM128& complex, const XMM128 &level, const XMM128& mag)
{
    static const __m128d infd       = CONST_SET1_64F(Const::INF_64F);
    static const __m128i maskb64    = CONST_SET_32I(0x00000000, 0x00000000, (int) 0xFFFFFFFF, (int) 0xFFFFFFFF);

    XMM128 ret, mask;
    ret.d    = _mm_mul_pd(complex.d, level.d);
    ret.d    = _mm_div_pd(ret.d, mag.d);

    mask.d   = _mm_cmpneq_pd(mag.d, infd);        // if mag != 0
    ret.d    = _mm_and_pd(mask.d, ret.d);         // dst = src
    mask.d   = _mm_andnot_pd(mask.d, level.d);    // else { mask = level
    ret.d    = _mm_or_pd(ret.d, mask.d);          //        dst = mask }

    ret.i        = _mm_and_si128(ret.i, maskb64);
    return        ret.d;
}

SYS_INLINE STATIC __m128i _fw_threshval_16sc(const XMM128& complex, const XMM128 &level)
{
    XMM128 ret = complex, rethi, mag, maghi, temp;
    CBL_SSE2::Unpack16STo32S(ret.i, rethi.i);

    ret.f      = _mm_cvtepi32_ps(ret.i);
    rethi.f    = _mm_cvtepi32_ps(rethi.i);

    // since level is filled with the same 16 bit number over and over
    // clear out every alternate 16 bits to unpack into all 32 bit numbers
    __m128 levelf  = _mm_cvtepi32_ps(_mm_srai_epi32(_mm_slli_epi32(level.i, 16), 16));

    // get the magnitudes
    mag.f     = _mm_mag_ps(ret.f);
    maghi.f   = _mm_mag_ps(rethi.f);
    mag.f     = _mm_shuffle_ps(mag.f, maghi.f, _MM_SHUFFLE(2, 0, 2, 0));


    // do the final calculation
    temp.f    = _mm_shuffle_ps(ret.f, rethi.f, _MM_SHUFFLE(2, 0, 2, 0));    // get all the a's out

    temp.f    = _mm_mul_ps(temp.f, levelf);
    temp.f    = _mm_div_ps(temp.f, mag.f);                                  // mag's will be matching a's

    ret.f     = _mm_shuffle_ps(temp.f, temp.f, _MM_SHUFFLE(1, 1, 0, 0));    // put back the lo a's
    rethi.f   = _mm_shuffle_ps(temp.f, temp.f, _MM_SHUFFLE(3, 3, 2, 2));    // put back the hi a's

    ret.i     = _mm_cvtps_epi32(ret.f);
    rethi.i   = _mm_cvtps_epi32(rethi.f);

    ret.i     = _mm_packs_epi32(ret.i, rethi.i);

    return      _mm_srli_epi32(_mm_slli_epi32(ret.i, 16), 16);              // clear out all the b's and return
}


} // namespace OPT_LEVEL


#endif 

